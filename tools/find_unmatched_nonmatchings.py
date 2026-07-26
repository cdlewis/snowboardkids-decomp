#!/usr/bin/env python3
"""
List generated nonmatchings workspaces that do not have a full local match.

The script scans nonmatchings/**/match_log.txt files, finds the best logged
percentage for existing C attempts in each workspace, and reports workspaces
whose best attempt is below 100%.
"""

from __future__ import annotations

import argparse
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable

from nonmatching_match_log import certified_attempt_path, parse_match_log_line

FULL_MATCH_PERCENT = 100.0


@dataclass(frozen=True)
class Attempt:
    path: Path
    percent: float


@dataclass(frozen=True)
class WorkspaceStatus:
    workspace: Path
    best_attempt: Attempt | None
    logged_attempt_count: int
    c_file_count: int


def repo_root_from_script() -> Path:
    return Path(__file__).resolve().parent.parent


def parse_match_log(log_path: Path, repo_root: Path) -> list[Attempt]:
    attempts: list[Attempt] = []
    workspace = log_path.parent
    nonmatchings_dir = (repo_root / "nonmatchings").resolve()

    try:
        lines = log_path.read_text().splitlines()
    except OSError:
        return attempts

    for line in lines:
        entry = parse_match_log_line(line)
        if entry is None:
            continue

        attempt_path = certified_attempt_path(entry, workspace)
        if attempt_path is None:
            continue

        # Ignore stale or accidental paths outside nonmatchings.
        if not attempt_path.exists() or not attempt_path.is_file():
            continue
        if nonmatchings_dir not in attempt_path.parents:
            continue

        attempts.append(Attempt(attempt_path, entry.percent))

    return attempts


def best_attempt(attempts: Iterable[Attempt]) -> Attempt | None:
    best: Attempt | None = None
    for attempt in attempts:
        if best is None or attempt.percent > best.percent:
            best = attempt
    return best


def base_workspace_name(workspace: Path) -> str:
    return workspace.name.split("-", 1)[0]


def matched_function_names(repo_root: Path) -> set[str]:
    matchings_dir = repo_root / "asm" / "matchings"
    if not matchings_dir.is_dir():
        return set()
    return {path.stem for path in matchings_dir.rglob("*.s")}


def workspace_statuses(repo_root: Path) -> list[WorkspaceStatus]:
    nonmatchings_dir = repo_root / "nonmatchings"
    matched_functions = matched_function_names(repo_root)
    statuses: list[WorkspaceStatus] = []

    for workspace in sorted(path for path in nonmatchings_dir.iterdir() if path.is_dir()):
        if workspace.name == "__pycache__":
            continue
        if base_workspace_name(workspace) in matched_functions:
            continue

        c_files = sorted(workspace.glob("*.c"))
        log_path = workspace / "match_log.txt"
        attempts = parse_match_log(log_path, repo_root) if log_path.exists() else []
        best = best_attempt(attempts)

        if best is None or best.percent < FULL_MATCH_PERCENT:
            statuses.append(
                WorkspaceStatus(
                    workspace=workspace,
                    best_attempt=best,
                    logged_attempt_count=len(attempts),
                    c_file_count=len(c_files),
                )
            )

    statuses.sort(
        key=lambda status: (
            -(status.best_attempt.percent if status.best_attempt is not None else -1.0),
            str(status.workspace),
        )
    )
    return statuses


def format_path(path: Path, repo_root: Path) -> str:
    try:
        return str(path.relative_to(repo_root))
    except ValueError:
        return str(path)


def print_statuses(statuses: list[WorkspaceStatus], repo_root: Path) -> None:
    print(f"{'Workspace':<56} {'Best':>8}  {'Best file':<16} {'Logged':>6} {'C files':>7}")
    print("-" * 100)

    for status in statuses:
        if status.best_attempt is None:
            best_percent = "none"
            best_file = ""
        else:
            best_percent = f"{status.best_attempt.percent:7.3f}%"
            best_file = format_path(status.best_attempt.path, status.workspace)

        print(
            f"{format_path(status.workspace, repo_root):<56} "
            f"{best_percent:>8}  "
            f"{best_file:<16} "
            f"{status.logged_attempt_count:>6} "
            f"{status.c_file_count:>7}"
        )


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Find nonmatchings/ workspaces whose best existing logged C attempt is not 100%."
    )
    parser.add_argument(
        "--repo-root",
        type=Path,
        default=repo_root_from_script(),
        help="Repository root to scan. Defaults to the parent of this script's tools directory.",
    )
    args = parser.parse_args()

    repo_root = args.repo_root.resolve()
    nonmatchings_dir = repo_root / "nonmatchings"
    if not nonmatchings_dir.is_dir():
        print(f"error: {nonmatchings_dir} does not exist", file=sys.stderr)
        return 2

    statuses = workspace_statuses(repo_root)
    if not statuses:
        print("All nonmatchings workspaces with logged existing C attempts are fully matched.")
        return 0

    print_statuses(statuses, repo_root)
    print(f"\nTotal not fully matched: {len(statuses)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
