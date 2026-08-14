#!/usr/bin/env python3
"""Generate the static remaining-functions report in docs/index.html."""

from __future__ import annotations

import argparse
import html
import json
import re
import shutil
import subprocess
import sys
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path
from typing import Iterable

from report_nonmatching_matches import (
    ScratchResult,
    best_scratch_results_by_function,
    default_scan_roots,
    function_name_aliases,
    matched_function_names,
    unique_resolved_paths,
)

GLABEL_RE = re.compile(r"^\s*glabel\s+(?P<name>[A-Za-z_][A-Za-z0-9_]*)\b", re.MULTILINE)
SIZE_RE = re.compile(r"^\s*nonmatching\s+\S+\s*,\s*0x(?P<size>[0-9A-Fa-f]+)\s*$", re.MULTILINE)
DECOMP_ME_URL = "https://decomp.me/scratch"
PREVIEW_WIDTH = 1200
PREVIEW_HEIGHT = 630
CHROMIUM_EXECUTABLES = (
    "chromium",
    "chromium-browser",
    "google-chrome",
    "google-chrome-stable",
    "chrome",
)


@dataclass(frozen=True)
class RemainingFunction:
    name: str
    asm_path: Path
    size: int | None


def repo_root_from_script() -> Path:
    return Path(__file__).resolve().parent.parent


def find_remaining_functions(repo_root: Path) -> list[RemainingFunction]:
    """Return code labels in asm/nonmatchings, excluding data-only assembly."""
    asm_root = repo_root / "asm" / "nonmatchings"
    functions: list[RemainingFunction] = []
    if not asm_root.is_dir():
        return functions

    for path in sorted(asm_root.rglob("*.s")):
        try:
            source = path.read_text(errors="ignore")
        except OSError as exc:
            print(f"warning: could not read {path}: {exc}", file=sys.stderr)
            continue
        size_match = SIZE_RE.search(source)
        size = int(size_match.group("size"), 16) if size_match else None
        for match in GLABEL_RE.finditer(source):
            functions.append(
                RemainingFunction(
                    name=match.group("name"),
                    asm_path=path.relative_to(repo_root),
                    size=size,
                )
            )
    return functions


def progress_measures(report_path: Path) -> dict[str, object]:
    if not report_path.is_file():
        return {}
    try:
        report = json.loads(report_path.read_text())
    except (OSError, json.JSONDecodeError) as exc:
        print(f"warning: could not read {report_path}: {exc}", file=sys.stderr)
        return {}
    measures = report.get("measures") if isinstance(report, dict) else None
    return measures if isinstance(measures, dict) else {}


def render_rows(
    functions: Iterable[RemainingFunction],
    scratch_results: dict[str, ScratchResult],
) -> str:
    ordered = sorted(
        functions,
        key=lambda function: (
            function.name not in scratch_results,
            -scratch_results[function.name].percent if function.name in scratch_results else 0,
            function.name,
        ),
    )
    rows: list[str] = []
    for function in ordered:
        size = f"{function.size:,} bytes" if function.size is not None else "size unknown"
        scratch = scratch_results.get(function.name)
        if scratch is None:
            row_class = "function-row needs-match"
            percent = 0.0
            author = "Needs a published match"
            score = '<strong class="missing-score">Not available</strong>'
            action = '<span class="missing-badge">Needs match</span>'
        else:
            row_class = "function-row"
            percent = scratch.percent
            author = scratch.author or "anonymous"
            scratch_url = f"{DECOMP_ME_URL}/{html.escape(scratch.slug)}"
            score = (
                f'<div class="score-track" aria-label="Match {percent:.3f}%">'
                f'<span style="width:{max(0.0, min(percent, 100.0)):.3f}%"></span></div>'
                f'<strong>{percent:.3f}%</strong>'
            )
            action = (
                f'<a class="action" href="{scratch_url}" target="_blank" '
                'rel="noopener">decomp.me ↗</a>'
            )
        rows.append(
            f'''        <article class="{row_class}" data-name="{html.escape(function.name.lower())}" data-score="{percent:.6f}">
          <div class="function-cell">
            <span class="function-name">{html.escape(function.name)}</span>
            <span class="function-size">{size}</span>
          </div>
          <div class="source-cell">{html.escape(author)}</div>
          <div class="score-cell">{score}</div>
          <div class="actions">{action}</div>
        </article>'''
        )
    return "\n".join(rows)


def render_document(
    template: str,
    functions: list[RemainingFunction],
    scratch_results: dict[str, ScratchResult],
    measures: dict[str, object],
) -> str:
    scratched = sum(function.name in scratch_results for function in functions)
    remaining_bytes = sum(function.size or 0 for function in functions)
    code_percent = measures.get("matched_code_percent")
    code_text = f"{code_percent:.2f}%" if isinstance(code_percent, (int, float)) else "—"
    replacements = {
        "{{REMAINING_COUNT}}": str(len(functions)),
        "{{SCRATCH_COUNT}}": str(scratched),
        "{{REMAINING_BYTES}}": f"{remaining_bytes:,}",
        "{{CODE_PERCENT}}": code_text,
        "{{GENERATED_AT}}": datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M UTC"),
        "{{FUNCTION_ROWS}}": render_rows(functions, scratch_results),
    }
    for marker, value in replacements.items():
        template = template.replace(marker, value)
    return template


def collect_scratch_results(
    repo_root: Path,
    scratches_paths: list[Path],
) -> dict[str, ScratchResult]:
    primary_aliases = function_name_aliases(repo_root)
    return best_scratch_results_by_function(
        scratches_paths,
        matched_function_names(repo_root),
        primary_aliases,
    )


def find_chromium() -> str:
    """Return a Chromium-compatible browser executable from PATH."""
    for executable in CHROMIUM_EXECUTABLES:
        resolved = shutil.which(executable)
        if resolved:
            return resolved
    raise RuntimeError(
        "could not generate the social preview: Chromium or Google Chrome "
        "was not found in PATH"
    )


def screenshot_preview(html_path: Path, output_path: Path) -> None:
    """Render the report's social-media preview with headless Chromium."""
    output_path.parent.mkdir(parents=True, exist_ok=True)
    command = [
        find_chromium(),
        "--headless",
        "--disable-gpu",
        "--no-sandbox",
        "--hide-scrollbars",
        f"--window-size={PREVIEW_WIDTH},{PREVIEW_HEIGHT}",
        f"--screenshot={output_path}",
        html_path.as_uri(),
    ]
    try:
        subprocess.run(command, check=True, capture_output=True, text=True)
    except subprocess.CalledProcessError as exc:
        detail = exc.stderr.strip() or exc.stdout.strip() or f"exit status {exc.returncode}"
        raise RuntimeError(f"could not generate the social preview: {detail}") from exc

    if not output_path.is_file() or output_path.stat().st_size == 0:
        raise RuntimeError(f"Chromium did not write the social preview to {output_path}")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo-root", type=Path, default=repo_root_from_script())
    parser.add_argument("--scan-root", type=Path, action="append", default=[])
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()

    repo_root = args.repo_root.resolve()
    default_roots = default_scan_roots(repo_root)
    scan_roots = unique_resolved_paths([*default_roots, *args.scan_root])
    scan_roots = [root for root in scan_roots if (root / "nonmatchings").is_dir()]
    scratches_paths = [
        root / "tools" / "scratches.json"
        for root in scan_roots
        if (root / "tools" / "scratches.json").is_file()
    ]
    functions = find_remaining_functions(repo_root)
    scratch = collect_scratch_results(repo_root, scratches_paths)

    template_path = Path(__file__).resolve().parent / "decompilation_summary" / "template.html"
    output_path = args.output.resolve() if args.output else repo_root / "docs" / "index.html"
    preview_path = output_path.with_name("og.png")
    document = render_document(
        template_path.read_text(),
        functions,
        scratch,
        progress_measures(repo_root / "report.json"),
    )
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(document)
    screenshot_preview(output_path, preview_path)
    print(
        f"Wrote {output_path} and {preview_path} with "
        f"{sum(function.name in scratch for function in functions)} scratches "
        f"for {len(functions)} remaining functions"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
