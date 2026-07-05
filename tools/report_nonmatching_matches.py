#!/usr/bin/env python3
"""Report logged match progress from nonmatchings/ workspaces.

This scans nonmatchings/**/match_log.txt and tools/scratches.json, ignores
functions already present in asm/matchings, and reports:

- functions with a logged 100% match that have not been moved to asm/matchings
- partially matched functions, ordered by best known match percentage
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
    source: str
    attempt: str
    workspace: Path | None = None


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
            source="local",
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


def parse_scratch_results(scratches_path: Path, matched_functions: set[str]) -> Iterable[MatchResult]:
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
        override = " override" if scratch.get("match_override") else ""
        yield MatchResult(
            function=function,
            percent=percent,
            source="decomp.me",
            attempt=f"https://decomp.me/scratch/{slug} (score={score}/{max_score}{override})",
        )


def is_better_result(candidate: MatchResult, current: MatchResult | None) -> bool:
    if current is None:
        return True
    if candidate.percent != current.percent:
        return candidate.percent > current.percent
    if candidate.source != current.source:
        return candidate.source == "decomp.me"
    return candidate.attempt < current.attempt


def best_results_by_function(repo_root: Path, scratches_path: Path | None) -> dict[str, MatchResult]:
    nonmatchings_dir = repo_root / "nonmatchings"
    matched_functions = matched_function_names(repo_root)
    best_by_function: dict[str, MatchResult] = {}

    for log_path in sorted(nonmatchings_dir.glob("*/match_log.txt")):
        function = function_name_from_workspace(log_path.parent)
        if function in matched_functions:
            continue

        for result in parse_match_log(log_path, repo_root):
            best = best_by_function.get(result.function)
            if is_better_result(result, best):
                best_by_function[result.function] = result

    if scratches_path is not None:
        for result in parse_scratch_results(scratches_path, matched_functions):
            best = best_by_function.get(result.function)
            if is_better_result(result, best):
                best_by_function[result.function] = result

    return best_by_function


def relative_path(path: str, repo_root: Path) -> str:
    candidate = Path(path)
    if not candidate.is_absolute():
        return path

    try:
        return str(candidate.relative_to(repo_root))
    except ValueError:
        return path


def print_rows(title: str, rows: list[MatchResult], repo_root: Path) -> None:
    print(title)
    if not rows:
        print("  none")
        return

    print(f"{'Function':<24} {'Best':>9}  {'Source':<9}  {'Attempt'}")
    print("-" * 104)
    for row in rows:
        print(
            f"{row.function:<24} "
            f"{row.percent:8.3f}%  "
            f"{row.source:<9}  "
            f"{relative_path(row.attempt, repo_root)}"
        )


def main() -> int:
    parser = argparse.ArgumentParser(
        description=(
            "Report 100% and partial matches from nonmatchings/**/match_log.txt, "
            "plus decomp.me scratches from tools/scratches.json, excluding functions "
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

    results = best_results_by_function(repo_root, scratches_path).values()
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
