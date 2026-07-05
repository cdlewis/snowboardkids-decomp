#!/usr/bin/env python3
"""Report logged match progress from nonmatchings/ workspaces.

This scans nonmatchings/**/match_log.txt and tools/scratches.json, ignores
functions already present in asm/matchings, and reports:

- functions with a logged 100% match that have not been moved to asm/matchings
- partially matched functions, ordered by best logged match percentage
- decomp.me scratch scores, when available, as supplemental context
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable


MATCH_LINE_RE = re.compile(r"(?P<filename>[A-Za-z0-9_./-]+\.c)\s+(?P<percent>\d+(?:\.\d+)?)\s*%")
ASM_LABEL_RE = re.compile(r"^\s*(?:glabel|dlabel)\s+(?P<name>[A-Za-z_][A-Za-z0-9_]*)\b", re.MULTILINE)
ASM_LABEL_LINE_RE = re.compile(r"^\s*(?:glabel|dlabel)\s+(?P<name>[A-Za-z_][A-Za-z0-9_]*)\b")
ASM_ADDRESS_RE = re.compile(r"/\*\s+[0-9A-Fa-f]+\s+(?P<addr>[0-9A-Fa-f]{8})\s+[0-9A-Fa-f]{8}\s+\*/")
FULL_MATCH_PERCENT = 100.0


@dataclass(frozen=True)
class MatchResult:
    function: str
    percent: float
    attempt: str
    workspace: Path | None = None


@dataclass(frozen=True)
class ScratchResult:
    function: str
    percent: float
    score: int | float
    max_score: int | float | None
    slug: str
    match_override: bool


@dataclass(frozen=True)
class ReportRow:
    local: MatchResult
    scratch: ScratchResult | None = None


def repo_root_from_script() -> Path:
    return Path(__file__).resolve().parent.parent


def function_name_from_workspace(workspace: Path) -> str:
    return workspace.name.split("-", 1)[0]


def matched_function_names(repo_root: Path) -> set[str]:
    matchings_dir = repo_root / "asm" / "matchings"
    if not matchings_dir.is_dir():
        return set()

    names: set[str] = set()
    for path in matchings_dir.rglob("*.s"):
        names.add(path.stem)
        try:
            text = path.read_text(errors="ignore")
        except OSError:
            continue
        names.update(match.group("name") for match in ASM_LABEL_RE.finditer(text))
        names.update(matched_function_address_aliases(text))
    return names


def matched_function_address_aliases(text: str) -> set[str]:
    aliases: set[str] = set()
    awaiting_label_address = False

    for line in text.splitlines():
        if ASM_LABEL_LINE_RE.match(line):
            awaiting_label_address = True
            continue

        if not awaiting_label_address:
            continue

        match = ASM_ADDRESS_RE.search(line)
        if match is None:
            continue

        aliases.add(f"func_{match.group('addr').upper()}")
        awaiting_label_address = False

    return aliases


def parse_match_log(log_path: Path, repo_root: Path) -> Iterable[MatchResult]:
    workspace = log_path.parent
    function = function_name_from_workspace(workspace)
    nonmatchings_dir = (repo_root / "nonmatchings").resolve()

    try:
        lines = log_path.read_text().splitlines()
    except OSError as exc:
        print(f"warning: could not read {log_path}: {exc}", file=sys.stderr)
        return

    for line in lines:
        match = MATCH_LINE_RE.search(line)
        if match is None:
            continue

        attempt = (workspace / match.group("filename")).resolve()

        # Ignore stale log entries and accidental paths outside nonmatchings.
        if not attempt.is_file() or nonmatchings_dir not in attempt.parents:
            continue

        try:
            percent = float(match.group("percent"))
        except ValueError:
            continue

        yield MatchResult(
            function=function,
            percent=percent,
            attempt=str(attempt),
            workspace=workspace,
        )


def scratch_match_percent(scratch: dict) -> float | None:
    score = scratch.get("score")
    max_score = scratch.get("max_score")

    if not isinstance(score, (int, float)):
        return None

    if score == 0:
        return FULL_MATCH_PERCENT

    if not isinstance(max_score, (int, float)) or max_score <= 0:
        return None

    return max(0.0, (max_score - score) / max_score * 100.0)


def parse_scratch_results(scratches_path: Path, matched_functions: set[str]) -> Iterable[ScratchResult]:
    if not scratches_path.is_file():
        return

    try:
        scratches = json.loads(scratches_path.read_text())
    except (OSError, json.JSONDecodeError) as exc:
        print(f"warning: could not read {scratches_path}: {exc}", file=sys.stderr)
        return

    if not isinstance(scratches, list):
        print(f"warning: expected {scratches_path} to contain a JSON array", file=sys.stderr)
        return

    for scratch in scratches:
        if not isinstance(scratch, dict):
            continue

        function = scratch.get("name")
        if not isinstance(function, str) or not function or function in matched_functions:
            continue

        percent = scratch_match_percent(scratch)
        if percent is None:
            continue

        slug = scratch.get("slug") or scratch.get("id")
        if not slug:
            continue

        score = scratch.get("score")
        max_score = scratch.get("max_score")
        yield ScratchResult(
            function=function,
            percent=percent,
            score=score,
            max_score=max_score if isinstance(max_score, (int, float)) else None,
            slug=str(slug),
            match_override=bool(scratch.get("match_override")),
        )


def is_better_match(candidate: MatchResult, current: MatchResult | None) -> bool:
    if current is None:
        return True
    if candidate.percent != current.percent:
        return candidate.percent > current.percent
    return candidate.attempt < current.attempt


def is_better_scratch(candidate: ScratchResult, current: ScratchResult | None) -> bool:
    if current is None:
        return True
    if candidate.percent != current.percent:
        return candidate.percent > current.percent
    return candidate.slug < current.slug


def best_local_results_by_function(repo_root: Path) -> dict[str, MatchResult]:
    nonmatchings_dir = repo_root / "nonmatchings"
    matched_functions = matched_function_names(repo_root)
    best_by_function: dict[str, MatchResult] = {}

    for log_path in sorted(nonmatchings_dir.glob("*/match_log.txt")):
        function = function_name_from_workspace(log_path.parent)
        if function in matched_functions:
            continue

        for result in parse_match_log(log_path, repo_root):
            best = best_by_function.get(result.function)
            if is_better_match(result, best):
                best_by_function[result.function] = result

    return best_by_function


def best_scratch_results_by_function(
    scratches_path: Path | None,
    matched_functions: set[str],
) -> dict[str, ScratchResult]:
    best_by_function: dict[str, ScratchResult] = {}
    if scratches_path is not None:
        for result in parse_scratch_results(scratches_path, matched_functions):
            best = best_by_function.get(result.function)
            if is_better_scratch(result, best):
                best_by_function[result.function] = result

    return best_by_function


def report_rows_by_function(repo_root: Path, scratches_path: Path | None) -> dict[str, ReportRow]:
    local_results = best_local_results_by_function(repo_root)
    scratch_results = best_scratch_results_by_function(scratches_path, matched_function_names(repo_root))

    return {
        function: ReportRow(local=local, scratch=scratch_results.get(function))
        for function, local in local_results.items()
    }


def relative_path(path: str, repo_root: Path) -> str:
    candidate = Path(path)
    if not candidate.is_absolute():
        return path

    try:
        return str(candidate.relative_to(repo_root))
    except ValueError:
        return path


def format_local_attempt(result: MatchResult) -> str:
    return Path(result.attempt).name


def format_local_workspace(result: MatchResult, repo_root: Path) -> str:
    if result.workspace is None:
        return relative_path(result.attempt, repo_root)

    return relative_path(str(result.workspace), repo_root)


def format_scratch(scratch: ScratchResult | None) -> str:
    if scratch is None:
        return ""

    score_text = f"{scratch.score}/{scratch.max_score}" if scratch.max_score is not None else str(scratch.score)
    override = " override" if scratch.match_override else ""
    return f"🐸 {scratch.percent:.3f}% ({score_text}{override}) {scratch.slug}"


def print_rows(title: str, rows: list[ReportRow], repo_root: Path) -> None:
    print(title)
    if not rows:
        print("  none")
        return

    print(f"{'Function':<24} {'Local':>9}  {'Attempt'}")
    print("-" * 48)
    for row in rows:
        local_attempt = format_local_attempt(row.local)
        print(
            f"{row.local.function:<24} "
            f"{row.local.percent:8.3f}%  "
            f"{local_attempt}"
        )
        print(f"{'':<24} {'':>9}   local: {format_local_workspace(row.local, repo_root)}")
        scratch_text = format_scratch(row.scratch)
        if scratch_text:
            print(f"{'':<24} {'':>9}   {scratch_text}")


def main() -> int:
    parser = argparse.ArgumentParser(
        description=(
            "Report 100% and partial matches from nonmatchings/**/match_log.txt, "
            "supplemented with decomp.me scratches from tools/scratches.json, excluding functions "
            "already in asm/matchings."
        )
    )
    parser.add_argument(
        "--repo-root",
        type=Path,
        default=repo_root_from_script(),
        help="Repository root to scan. Defaults to the parent of this script's tools directory.",
    )
    parser.add_argument(
        "--partial-only",
        action="store_true",
        help="Only print partially matched functions.",
    )
    parser.add_argument(
        "--full-only",
        action="store_true",
        help="Only print 100% matched functions missing from asm/matchings.",
    )
    parser.add_argument(
        "--scratches",
        type=Path,
        default=None,
        help="Path to decomp.me scratch JSON. Defaults to tools/scratches.json if it exists.",
    )
    parser.add_argument(
        "--no-scratches",
        action="store_true",
        help="Only use local nonmatchings/**/match_log.txt entries.",
    )
    args = parser.parse_args()

    if args.partial_only and args.full_only:
        print("error: --partial-only and --full-only cannot be used together", file=sys.stderr)
        return 2
    if args.no_scratches and args.scratches is not None:
        print("error: --no-scratches and --scratches cannot be used together", file=sys.stderr)
        return 2

    repo_root = args.repo_root.resolve()
    nonmatchings_dir = repo_root / "nonmatchings"
    if not nonmatchings_dir.is_dir():
        print(f"error: {nonmatchings_dir} does not exist", file=sys.stderr)
        return 2

    scratches_path = None
    if not args.no_scratches:
        scratches_path = args.scratches.resolve() if args.scratches else repo_root / "tools" / "scratches.json"

    results = report_rows_by_function(repo_root, scratches_path).values()
    full_matches = sorted(
        (row for row in results if row.local.percent >= FULL_MATCH_PERCENT),
        key=lambda row: row.local.function,
    )
    partial_matches = sorted(
        (row for row in results if row.local.percent < FULL_MATCH_PERCENT),
        key=lambda row: (-row.local.percent, row.local.function),
    )

    if not args.partial_only:
        print_rows("100% matched in match_log but missing from asm/matchings", full_matches, repo_root)
    if not args.partial_only and not args.full_only:
        print()
    if not args.full_only:
        print_rows("Partially matched functions", partial_matches, repo_root)

    if not args.partial_only and not args.full_only:
        print(f"\nTotals: full={len(full_matches)} partial={len(partial_matches)}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
