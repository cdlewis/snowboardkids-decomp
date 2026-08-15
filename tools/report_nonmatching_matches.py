#!/usr/bin/env python3
"""Report logged match progress from nonmatchings/ workspaces.

This scans nonmatchings/**/match_log.txt and tools/scratches.json, ignores
functions that no longer exist in either active assembly tree or are already
present in asm/matchings according to the selected filter scope, and reports:

- functions with a logged 100% match that have not been moved to asm/matchings
- decomp.me scratches with a 100% match that have not been moved to asm/matchings
- partially matched functions, ordered by best logged match percentage
- decomp.me scratch scores, when available, as supplemental context
"""

from __future__ import annotations

import argparse
import json
import os
import re
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable

from nonmatching_match_log import certified_attempt_path, parse_match_log_line

ASM_LABEL_RE = re.compile(r"^\s*(?:glabel|dlabel)\s+(?P<name>[A-Za-z_][A-Za-z0-9_]*)\b", re.MULTILINE)
ASM_LABEL_LINE_RE = re.compile(r"^\s*(?:glabel|dlabel)\s+(?P<name>[A-Za-z_][A-Za-z0-9_]*)\b")
ASM_ADDRESS_RE = re.compile(r"/\*\s+[0-9A-Fa-f]+\s+(?P<addr>[0-9A-Fa-f]{8})\s+[0-9A-Fa-f]{8}\s+\*/")
FUNC_ADDR_NAME_RE = re.compile(r"^func_(?P<addr>[0-9A-Fa-f]{8})$")
SYMBOL_ADDR_RE = re.compile(
    r"^\s*(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*=\s*0x(?P<addr>[0-9A-Fa-f]{8})\s*;(?P<comment>.*)$"
)
GFX_REGION_ALLOC_PTR_RE = re.compile(r"\bg(?:fx)?RegionAllocPtr\b", re.IGNORECASE)
FULL_MATCH_PERCENT = 100.0


@dataclass(frozen=True)
class MatchResult:
    function: str
    percent: float
    attempt: str
    source_root: Path
    workspace: Path | None = None


@dataclass(frozen=True)
class ScratchResult:
    function: str
    percent: float
    score: int | float
    max_score: int | float | None
    slug: str
    match_override: bool
    author: str | None = None


@dataclass(frozen=True)
class ReportRow:
    local: MatchResult
    scratch: ScratchResult | None = None
    uses_gfx_region_alloc_ptr: bool = False


def repo_root_from_script() -> Path:
    return Path(__file__).resolve().parent.parent


def default_scan_roots(repo_root: Path) -> list[Path]:
    roots = [repo_root]
    for sibling in ("sbk-a", "sbk-b", "sbk-c"):
        sibling_root = repo_root.parent / sibling
        if sibling_root.is_dir() and sibling_root.resolve() != repo_root.resolve():
            roots.append(sibling_root)
    return roots


def unique_resolved_paths(paths: Iterable[Path]) -> list[Path]:
    roots: list[Path] = []
    seen: set[Path] = set()

    for path in paths:
        resolved = path.resolve()
        if resolved in seen:
            continue
        seen.add(resolved)
        roots.append(resolved)

    return roots


def function_name_from_workspace(workspace: Path) -> str:
    return workspace.name.split("-", 1)[0]


def function_name_aliases(repo_root: Path) -> dict[str, str]:
    aliases: dict[str, str] = {}
    symbol_addrs_path = repo_root / "symbol_addrs.txt"
    if not symbol_addrs_path.is_file():
        return aliases

    try:
        lines = symbol_addrs_path.read_text(errors="ignore").splitlines()
    except OSError as exc:
        print(f"warning: could not read {symbol_addrs_path}: {exc}", file=sys.stderr)
        return aliases

    for line in lines:
        match = SYMBOL_ADDR_RE.match(line)
        if match is None or "type:func" not in match.group("comment"):
            continue

        name = match.group("name")
        alias = f"func_{match.group('addr').upper()}"
        if name != alias:
            aliases[alias] = name

    return aliases


def canonical_function_name(function: str, aliases: dict[str, str]) -> str:
    address_match = FUNC_ADDR_NAME_RE.match(function)
    if address_match is None:
        return function

    return aliases.get(f"func_{address_match.group('addr').upper()}", function)


def canonicalize_function_names(names: set[str], aliases: dict[str, str]) -> set[str]:
    return names | {canonical_function_name(name, aliases) for name in names}


def assembly_function_names(repo_root: Path, directory: str) -> set[str]:
    assembly_dir = repo_root / "asm" / directory
    if not assembly_dir.is_dir():
        return set()

    names: set[str] = set()
    for path in assembly_dir.rglob("*.s"):
        names.add(path.stem)
        try:
            text = path.read_text(errors="ignore")
        except OSError:
            continue
        names.update(match.group("name") for match in ASM_LABEL_RE.finditer(text))
        names.update(matched_function_address_aliases(text))
    return names


def matched_function_names(repo_root: Path) -> set[str]:
    return assembly_function_names(repo_root, "matchings")


def existing_function_names(repo_root: Path) -> set[str]:
    return matched_function_names(repo_root) | assembly_function_names(repo_root, "nonmatchings")


def matched_function_names_by_root(repo_roots: Iterable[Path]) -> dict[Path, set[str]]:
    return {repo_root: matched_function_names(repo_root) for repo_root in repo_roots}


def existing_function_names_by_root(repo_roots: Iterable[Path]) -> dict[Path, set[str]]:
    return {repo_root: existing_function_names(repo_root) for repo_root in repo_roots}


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


def parse_match_log(log_path: Path, repo_root: Path, aliases: dict[str, str]) -> Iterable[MatchResult]:
    workspace = log_path.parent
    function = canonical_function_name(function_name_from_workspace(workspace), aliases)
    nonmatchings_dir = (repo_root / "nonmatchings").resolve()

    try:
        lines = log_path.read_text().splitlines()
    except OSError as exc:
        print(f"warning: could not read {log_path}: {exc}", file=sys.stderr)
        return

    for line in lines:
        entry = parse_match_log_line(line)
        if entry is None:
            continue

        attempt = certified_attempt_path(entry, workspace)
        if attempt is None:
            continue

        # Ignore stale log entries and accidental paths outside nonmatchings.
        if not attempt.is_file() or nonmatchings_dir not in attempt.parents:
            continue

        yield MatchResult(
            function=function,
            percent=entry.percent,
            attempt=str(attempt),
            source_root=repo_root,
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


def parse_scratch_results(
    scratches_path: Path,
    matched_functions: set[str],
    aliases: dict[str, str],
) -> Iterable[ScratchResult]:
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

        scratch_name = scratch.get("name")
        if not isinstance(scratch_name, str) or not scratch_name:
            continue

        function = canonical_function_name(scratch_name, aliases)
        if function in matched_functions:
            continue

        percent = scratch_match_percent(scratch)
        if percent is None:
            continue

        slug = scratch.get("slug") or scratch.get("id")
        if not slug:
            continue

        score = scratch.get("score")
        max_score = scratch.get("max_score")
        owner = scratch.get("owner")
        author = owner.get("username") if isinstance(owner, dict) else None
        yield ScratchResult(
            function=function,
            percent=percent,
            score=score,
            max_score=max_score if isinstance(max_score, (int, float)) else None,
            slug=str(slug),
            match_override=bool(scratch.get("match_override")),
            author=author if isinstance(author, str) and author else None,
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


def best_local_results_by_function(
    scan_roots: Iterable[Path],
    primary_matched_functions: set[str],
    matched_functions_by_root: dict[Path, set[str]],
    primary_existing_functions: set[str],
    existing_functions_by_root: dict[Path, set[str]],
    aliases_by_root: dict[Path, dict[str, str]],
    filter_scope: str,
) -> dict[str, MatchResult]:
    best_by_function: dict[str, MatchResult] = {}

    for scan_root in scan_roots:
        source_matched_functions = matched_functions_by_root.get(scan_root, set())
        source_existing_functions = existing_functions_by_root.get(scan_root, set())
        aliases = aliases_by_root.get(scan_root, {})
        nonmatchings_dir = scan_root / "nonmatchings"
        for log_path in sorted(nonmatchings_dir.glob("*/match_log.txt")):
            function = canonical_function_name(function_name_from_workspace(log_path.parent), aliases)
            if function not in primary_existing_functions and function not in source_existing_functions:
                continue
            if filter_scope in {"primary", "both"} and function in primary_matched_functions:
                continue
            if filter_scope in {"source", "both"} and function in source_matched_functions:
                continue

            for result in parse_match_log(log_path, scan_root, aliases):
                best = best_by_function.get(result.function)
                if is_better_match(result, best):
                    best_by_function[result.function] = result

    return best_by_function


def best_scratch_results_by_function(
    scratches_paths: Iterable[Path],
    matched_functions: set[str],
    aliases: dict[str, str],
) -> dict[str, ScratchResult]:
    best_by_function: dict[str, ScratchResult] = {}
    for scratches_path in scratches_paths:
        for result in parse_scratch_results(scratches_path, matched_functions, aliases):
            best = best_by_function.get(result.function)
            if is_better_scratch(result, best):
                best_by_function[result.function] = result

    return best_by_function


def full_unintegrated_scratch_results(
    repo_root: Path,
    scratches_paths: Iterable[Path],
) -> list[ScratchResult]:
    aliases = function_name_aliases(repo_root)
    matched_functions = canonicalize_function_names(matched_function_names(repo_root), aliases)
    scratch_results = best_scratch_results_by_function(
        scratches_paths,
        matched_functions,
        aliases,
    )
    return sorted(
        (result for result in scratch_results.values() if result.percent >= FULL_MATCH_PERCENT),
        key=lambda result: result.function,
    )


def report_rows_by_function(
    repo_root: Path,
    scan_roots: Iterable[Path],
    scratches_paths: Iterable[Path],
    filter_scope: str,
) -> dict[str, ReportRow]:
    scan_roots = list(scan_roots)
    matched_functions_by_root = matched_function_names_by_root([repo_root, *scan_roots])
    existing_functions_by_root = existing_function_names_by_root([repo_root, *scan_roots])
    primary_aliases = function_name_aliases(repo_root)
    aliases_by_root = {
        root: {**function_name_aliases(root), **primary_aliases}
        for root in [repo_root, *scan_roots]
    }
    matched_functions_by_root = {
        root: canonicalize_function_names(names, aliases_by_root.get(root, {}))
        for root, names in matched_functions_by_root.items()
    }
    existing_functions_by_root = {
        root: canonicalize_function_names(names, aliases_by_root.get(root, {}))
        for root, names in existing_functions_by_root.items()
    }
    primary_matched_functions = matched_functions_by_root.get(repo_root, set())
    primary_existing_functions = existing_functions_by_root.get(repo_root, set())
    local_results = best_local_results_by_function(
        scan_roots,
        primary_matched_functions,
        matched_functions_by_root,
        primary_existing_functions,
        existing_functions_by_root,
        aliases_by_root,
        filter_scope,
    )
    scratch_results = best_scratch_results_by_function(
        scratches_paths,
        primary_matched_functions,
        primary_aliases,
    )

    return {
        function: ReportRow(
            local=local,
            scratch=scratch_results.get(function),
            uses_gfx_region_alloc_ptr=uses_gfx_region_alloc_ptr(local),
        )
        for function, local in local_results.items()
    }


def path_contains_gfx_region_alloc_ptr(path: Path) -> bool:
    try:
        return GFX_REGION_ALLOC_PTR_RE.search(path.read_text(errors="ignore")) is not None
    except OSError:
        return False


def uses_gfx_region_alloc_ptr(result: MatchResult) -> bool:
    paths = [Path(result.attempt)]
    if result.workspace is not None:
        paths.append(result.workspace / "target.s")

    return any(path_contains_gfx_region_alloc_ptr(path) for path in paths)


def relative_path(path: str, repo_root: Path) -> str:
    candidate = Path(path)
    if not candidate.is_absolute():
        return path

    try:
        return str(candidate.relative_to(repo_root))
    except ValueError:
        return os.path.relpath(candidate, repo_root)


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
        marker = " ✨" if row.uses_gfx_region_alloc_ptr else ""
        function = f"🐸 {row.local.function}{marker}"
        print(
            f"{function:<24} "
            f"{row.local.percent:8.3f}%  "
            f"{local_attempt}"
        )
        print(f"{'':<24} {'':>9}   local: {format_local_workspace(row.local, repo_root)}")
        scratch_text = format_scratch(row.scratch)
        if scratch_text:
            print(f"{'':<24} {'':>9}   {scratch_text}")


def print_scratch_rows(title: str, rows: list[ScratchResult]) -> None:
    print(title)
    if not rows:
        print("  none")
        return

    print(f"{'Function':<32} {'Scratch'}")
    print("-" * 64)
    for scratch in rows:
        print(f"{scratch.function:<32} {format_scratch(scratch)}")


def main() -> int:
    parser = argparse.ArgumentParser(
        description=(
            "Report full and partial matches from nonmatchings/**/match_log.txt, "
            "supplemented with decomp.me scratches from tools/scratches.json. By default this scans "
            "the current checkout and sibling worktrees when present, excluding functions that no "
            "longer exist in their assembly trees or are already matched in either the primary repo "
            "root or the scan root that produced each match_log entry."
        )
    )
    parser.add_argument(
        "--repo-root",
        type=Path,
        default=repo_root_from_script(),
        help=(
            "Primary repository root used for matched-function filtering and relative paths. "
            "Defaults to the parent of this script's tools directory."
        ),
    )
    parser.add_argument(
        "--scan-root",
        type=Path,
        action="append",
        default=[],
        help=(
            "Additional repository/workspace root to scan for nonmatchings/*/match_log.txt. "
            "Can be repeated. Defaults already include --repo-root and ../sbk-a when present."
        ),
    )
    parser.add_argument(
        "--partial-only",
        action="store_true",
        help="Only print partially matched functions.",
    )
    parser.add_argument(
        "--full-only",
        action="store_true",
        help="Only print 100%% matched functions missing from asm/matchings.",
    )
    parser.add_argument(
        "--scratches",
        type=Path,
        default=None,
        help=(
            "Path to decomp.me scratch JSON. Defaults to tools/scratches.json under each scan root "
            "if it exists."
        ),
    )
    parser.add_argument(
        "--no-scratches",
        action="store_true",
        help="Only use local nonmatchings/**/match_log.txt entries.",
    )
    parser.add_argument(
        "--min-percent",
        type=float,
        default=0.0,
        help="Only report local match_log entries at or above this percentage.",
    )
    parser.add_argument(
        "--filter-scope",
        choices=("primary", "source", "both"),
        default="both",
        help=(
            "Which asm/matchings tree suppresses local match_log rows: primary uses --repo-root only; "
            "source uses each scan root; both uses either one. Defaults to both."
        ),
    )
    args = parser.parse_args()

    if args.partial_only and args.full_only:
        print("error: --partial-only and --full-only cannot be used together", file=sys.stderr)
        return 2
    if args.no_scratches and args.scratches is not None:
        print("error: --no-scratches and --scratches cannot be used together", file=sys.stderr)
        return 2

    repo_root = args.repo_root.resolve()
    default_roots = unique_resolved_paths(default_scan_roots(repo_root))
    explicit_roots = unique_resolved_paths(args.scan_root)
    scan_roots = unique_resolved_paths([*default_roots, *explicit_roots])
    explicit_root_set = set(explicit_roots)

    valid_scan_roots: list[Path] = []
    for scan_root in scan_roots:
        nonmatchings_dir = scan_root / "nonmatchings"
        if nonmatchings_dir.is_dir():
            valid_scan_roots.append(scan_root)
        elif scan_root in explicit_root_set:
            print(f"warning: skipping {scan_root}; {nonmatchings_dir} does not exist", file=sys.stderr)

    if not valid_scan_roots:
        print("error: no scan roots with a nonmatchings directory", file=sys.stderr)
        return 2

    scratches_paths: list[Path] = []
    if not args.no_scratches:
        if args.scratches is not None:
            scratches_paths = [args.scratches.resolve()]
        else:
            scratches_paths = [
                scan_root / "tools" / "scratches.json"
                for scan_root in valid_scan_roots
                if (scan_root / "tools" / "scratches.json").is_file()
            ]

    results = [
        row
        for row in report_rows_by_function(repo_root, valid_scan_roots, scratches_paths, args.filter_scope).values()
        if row.local.percent >= args.min_percent
    ]
    full_matches = sorted(
        (row for row in results if row.local.percent >= FULL_MATCH_PERCENT),
        key=lambda row: row.local.function,
    )
    partial_matches = sorted(
        (row for row in results if row.local.percent < FULL_MATCH_PERCENT),
        key=lambda row: (-row.local.percent, row.local.function),
    )
    full_scratch_matches = full_unintegrated_scratch_results(repo_root, scratches_paths)

    if not args.partial_only:
        print_rows("100% matched in match_log but missing from primary asm/matchings", full_matches, repo_root)
        if scratches_paths:
            print()
            print_scratch_rows(
                "100% matched on decomp.me but missing from primary asm/matchings",
                full_scratch_matches,
            )
    if not args.partial_only and not args.full_only:
        print()
    if not args.full_only:
        print_rows("Partially matched functions", partial_matches, repo_root)

    if not args.partial_only and not args.full_only:
        print(
            f"\nTotals: full={len(full_matches)} "
            f"decomp.me_full={len(full_scratch_matches)} partial={len(partial_matches)}"
        )

    return 0


if __name__ == "__main__":
    sys.exit(main())
