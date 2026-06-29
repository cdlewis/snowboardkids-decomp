#!/usr/bin/env python3
"""Report logged match progress from nonmatchings/ workspaces.

This scans nonmatchings/**/match_log.txt, ignores functions already present in
asm/matchings, and reports:

- functions with a logged 100% match that have not been moved to asm/matchings
- partially matched functions, ordered by best logged match percentage
"""

from __future__ import annotations

import argparse
import re
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable


MATCH_LINE_RE = re.compile(r"(?P<filename>[A-Za-z0-9_./-]+\.c)\s+(?P<percent>\d+(?:\.\d+)?)\s*%")
FULL_MATCH_PERCENT = 100.0


@dataclass(frozen=True)
class MatchResult:
    function: str
    percent: float
    attempt: Path
    workspace: Path


def repo_root_from_script() -> Path:
    return Path(__file__).resolve().parent.parent


def function_name_from_workspace(workspace: Path) -> str:
    return workspace.name.split("-", 1)[0]


def matched_function_names(repo_root: Path) -> set[str]:
    matchings_dir = repo_root / "asm" / "matchings"
    if not matchings_dir.is_dir():
        return set()
    return {path.stem for path in matchings_dir.rglob("*.s")}


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

        yield MatchResult(function=function, percent=percent, attempt=attempt, workspace=workspace)


def best_results_by_function(repo_root: Path) -> dict[str, MatchResult]:
    nonmatchings_dir = repo_root / "nonmatchings"
    matched_functions = matched_function_names(repo_root)
    best_by_function: dict[str, MatchResult] = {}

    for log_path in sorted(nonmatchings_dir.glob("*/match_log.txt")):
        function = function_name_from_workspace(log_path.parent)
        if function in matched_functions:
            continue

        for result in parse_match_log(log_path, repo_root):
            best = best_by_function.get(result.function)
            if best is None or result.percent > best.percent:
                best_by_function[result.function] = result

    return best_by_function


def relative_path(path: Path, repo_root: Path) -> str:
    try:
        return str(path.relative_to(repo_root))
    except ValueError:
        return str(path)


def print_rows(title: str, rows: list[MatchResult], repo_root: Path) -> None:
    print(title)
    if not rows:
        print("  none")
        return

    print(f"{'Function':<24} {'Best':>9}  {'Attempt'}")
    print("-" * 80)
    for row in rows:
        print(
            f"{row.function:<24} "
            f"{row.percent:8.3f}%  "
            f"{relative_path(row.attempt, repo_root)}"
        )


def main() -> int:
    parser = argparse.ArgumentParser(
        description=(
            "Report 100% and partial matches from nonmatchings/**/match_log.txt, "
            "excluding functions already in asm/matchings."
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
    args = parser.parse_args()

    if args.partial_only and args.full_only:
        print("error: --partial-only and --full-only cannot be used together", file=sys.stderr)
        return 2

    repo_root = args.repo_root.resolve()
    nonmatchings_dir = repo_root / "nonmatchings"
    if not nonmatchings_dir.is_dir():
        print(f"error: {nonmatchings_dir} does not exist", file=sys.stderr)
        return 2

    results = best_results_by_function(repo_root).values()
    full_matches = sorted(
        (result for result in results if result.percent >= FULL_MATCH_PERCENT),
        key=lambda result: result.function,
    )
    partial_matches = sorted(
        (result for result in results if result.percent < FULL_MATCH_PERCENT),
        key=lambda result: (-result.percent, result.function),
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
