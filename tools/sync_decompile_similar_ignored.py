#!/usr/bin/env python3
"""Synchronize task-runner/decompile-similar/ignored.log across SBK checkouts."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
import sys
from typing import Iterable


DEFAULT_REPOS = (
    "snowboardkids-decomp",
    "sbk-a",
    "sbk-b",
    "sbk-c",
)
IGNORE_LOG = Path("task-runner/decompile-similar/ignored.log")


def default_code_dir() -> Path:
    return Path(__file__).resolve().parents[2]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Merge task-runner/decompile-similar/ignored.log entries across "
            "snowboardkids-decomp, sbk-a, sbk-b, and sbk-c."
        )
    )
    parser.add_argument(
        "--code-dir",
        type=Path,
        default=default_code_dir(),
        help="Parent directory containing the SBK checkouts. Defaults to %(default)s.",
    )
    parser.add_argument(
        "--repo",
        action="append",
        dest="repos",
        help=(
            "Repo directory name to include. May be passed multiple times. "
            "Defaults to snowboardkids-decomp, sbk-a, sbk-b, sbk-c."
        ),
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Show what would change without writing files.",
    )
    return parser.parse_args()


def load_entries(path: Path) -> list[tuple[str, object]]:
    if not path.exists():
        return []

    entries: list[tuple[str, object]] = []
    with path.open() as infile:
        for line_number, line in enumerate(infile, start=1):
            stripped = line.strip()
            if not stripped:
                continue

            try:
                parsed = json.loads(stripped)
            except json.JSONDecodeError as err:
                raise ValueError(f"{path}:{line_number}: invalid JSON: {err}") from err

            canonical = json.dumps(parsed, separators=(",", ":"))
            entries.append((canonical, parsed))

    return entries


def merge_entries(paths: Iterable[Path]) -> list[tuple[str, object]]:
    seen: set[str] = set()
    merged: list[tuple[str, object]] = []

    for path in paths:
        for canonical, parsed in load_entries(path):
            if canonical in seen:
                continue
            seen.add(canonical)
            merged.append((canonical, parsed))

    return merged


def render_log(entries: list[tuple[str, object]]) -> str:
    lines = [json.dumps(parsed, separators=(",", ":")) for _, parsed in entries]
    return "\n".join(lines) + ("\n" if lines else "")


def main() -> int:
    args = parse_args()
    repos = tuple(args.repos or DEFAULT_REPOS)
    paths = [args.code_dir / repo / IGNORE_LOG for repo in repos]

    try:
        merged_entries = merge_entries(paths)
    except ValueError as err:
        print(err, file=sys.stderr)
        return 1

    output = render_log(merged_entries)
    print(f"Merged {len(merged_entries)} unique ignore entries from {len(paths)} repos.")

    changed = 0
    for path in paths:
        old_output = path.read_text() if path.exists() else None
        if old_output == output:
            print(f"unchanged {path}")
            continue

        changed += 1
        if args.dry_run:
            old_count = len(load_entries(path))
            print(f"would update {path} ({old_count} -> {len(merged_entries)} entries)")
            continue

        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(output)
        print(f"updated {path}")

    if args.dry_run:
        print(f"Dry run: {changed} file(s) would be updated.")
    else:
        print(f"Updated {changed} file(s).")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
