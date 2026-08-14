#!/usr/bin/env python3
"""Generate the static remaining-functions report in docs/index.html."""

from __future__ import annotations

import argparse
import html
import json
import re
import sys
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path
from typing import Iterable

from report_nonmatching_matches import (
    MatchResult,
    ScratchResult,
    best_local_results_by_function,
    best_scratch_results_by_function,
    canonicalize_function_names,
    default_scan_roots,
    existing_function_names_by_root,
    function_name_aliases,
    matched_function_names_by_root,
    unique_resolved_paths,
)

GLABEL_RE = re.compile(r"^\s*glabel\s+(?P<name>[A-Za-z_][A-Za-z0-9_]*)\b", re.MULTILINE)
SIZE_RE = re.compile(r"^\s*nonmatching\s+\S+\s*,\s*0x(?P<size>[0-9A-Fa-f]+)\s*$", re.MULTILINE)
REPOSITORY_BLOB_URL = "https://github.com/cdlewis/snowboardkids-decomp/blob/main"
DECOMP_ME_URL = "https://decomp.me/scratch"


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


def format_percent(percent: float | None) -> str:
    return "—" if percent is None else f"{percent:.3f}%"


def best_percent(local: MatchResult | None, scratch: ScratchResult | None) -> float | None:
    scores = [result.percent for result in (local, scratch) if result is not None]
    return max(scores) if scores else None


def best_source(local: MatchResult | None, scratch: ScratchResult | None) -> str:
    if local is None and scratch is None:
        return "No attempt logged"
    if local is None:
        return f"decomp.me · {scratch.author}" if scratch and scratch.author else "decomp.me"
    if scratch is None or local.percent >= scratch.percent:
        return "Local workspace"
    return f"decomp.me · {scratch.author}" if scratch.author else "decomp.me"


def render_rows(
    functions: Iterable[RemainingFunction],
    local_results: dict[str, MatchResult],
    scratch_results: dict[str, ScratchResult],
) -> str:
    ordered = sorted(
        functions,
        key=lambda function: (
            -(best_percent(local_results.get(function.name), scratch_results.get(function.name)) or -1),
            function.name,
        ),
    )
    rows: list[str] = []
    for function in ordered:
        local = local_results.get(function.name)
        scratch = scratch_results.get(function.name)
        percent = best_percent(local, scratch)
        width = max(0.0, min(percent or 0.0, 100.0))
        size = f"{function.size:,} bytes" if function.size is not None else "size unknown"
        asm_url = f"{REPOSITORY_BLOB_URL}/{function.asm_path.as_posix()}"
        scratch_link = ""
        if scratch is not None:
            scratch_link = (
                f'<a class="action scratch" href="{DECOMP_ME_URL}/{html.escape(scratch.slug)}" '
                'target="_blank" rel="noopener">scratch ↗</a>'
            )
        rows.append(
            f'''        <article class="function-row" data-name="{html.escape(function.name.lower())}" data-score="{percent or 0:.6f}">
          <div class="function-cell">
            <a class="function-name" href="{asm_url}" target="_blank" rel="noopener">{html.escape(function.name)}</a>
            <span class="function-size">{size}</span>
          </div>
          <div class="source-cell">{html.escape(best_source(local, scratch))}</div>
          <div class="score-cell">
            <div class="score-track" aria-label="Best match {format_percent(percent)}"><span style="width:{width:.3f}%"></span></div>
            <strong>{format_percent(percent)}</strong>
            <span class="score-detail">local {format_percent(local.percent if local else None)} · scratch {format_percent(scratch.percent if scratch else None)}</span>
          </div>
          <div class="actions"><a class="action" href="{asm_url}" target="_blank" rel="noopener">assembly ↗</a>{scratch_link}</div>
        </article>'''
        )
    return "\n".join(rows)


def render_document(
    template: str,
    functions: list[RemainingFunction],
    local_results: dict[str, MatchResult],
    scratch_results: dict[str, ScratchResult],
    measures: dict[str, object],
) -> str:
    scored = sum(
        best_percent(local_results.get(function.name), scratch_results.get(function.name)) is not None
        for function in functions
    )
    remaining_bytes = sum(function.size or 0 for function in functions)
    code_percent = measures.get("matched_code_percent")
    code_text = f"{code_percent:.2f}%" if isinstance(code_percent, (int, float)) else "—"
    replacements = {
        "{{REMAINING_COUNT}}": str(len(functions)),
        "{{SCORED_COUNT}}": str(scored),
        "{{REMAINING_BYTES}}": f"{remaining_bytes:,}",
        "{{CODE_PERCENT}}": code_text,
        "{{GENERATED_AT}}": datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M UTC"),
        "{{FUNCTION_ROWS}}": render_rows(functions, local_results, scratch_results),
    }
    for marker, value in replacements.items():
        template = template.replace(marker, value)
    return template


def collect_results(
    repo_root: Path,
    scan_roots: list[Path],
    scratches_paths: list[Path],
) -> tuple[dict[str, MatchResult], dict[str, ScratchResult]]:
    roots = unique_resolved_paths([repo_root, *scan_roots])
    primary_aliases = function_name_aliases(repo_root)
    aliases = {
        root: {**function_name_aliases(root), **primary_aliases}
        for root in roots
    }
    matched = matched_function_names_by_root(roots)
    existing = existing_function_names_by_root(roots)
    matched = {
        root: canonicalize_function_names(names, aliases[root])
        for root, names in matched.items()
    }
    existing = {
        root: canonicalize_function_names(names, aliases[root])
        for root, names in existing.items()
    }
    primary_matched = matched.get(repo_root, set())
    primary_existing = existing.get(repo_root, set())
    local = best_local_results_by_function(
        roots,
        primary_matched,
        matched,
        primary_existing,
        existing,
        aliases,
        "both",
    )
    scratch = best_scratch_results_by_function(
        scratches_paths,
        primary_matched,
        primary_aliases,
    )
    return local, scratch


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
    local, scratch = collect_results(repo_root, scan_roots, scratches_paths)

    template_path = Path(__file__).resolve().parent / "decompilation_summary" / "template.html"
    output_path = args.output.resolve() if args.output else repo_root / "docs" / "index.html"
    document = render_document(
        template_path.read_text(),
        functions,
        local,
        scratch,
        progress_measures(repo_root / "report.json"),
    )
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(document)
    print(
        f"Wrote {output_path} with {len(functions)} remaining functions "
        f"({sum(name in local or name in scratch for name in (f.name for f in functions))} scored)"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
