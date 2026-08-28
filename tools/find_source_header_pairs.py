#!/usr/bin/env python3
"""List game-owned C files and the same-named headers they include."""

import argparse
import json
import re
from pathlib import Path


SOURCE_ROOT = Path("src")
INCLUDE_ROOT = Path("include")
EXCLUDED_SOURCE_ROOTS = (Path("src/ultra"),)
GENERATED_CONTEXT_NAME = "c" + "tx.c"
INCLUDE_RE = re.compile(r'^\s*#\s*include\s+"([^"]+\.h)"', re.MULTILINE)


def is_excluded(path: Path) -> bool:
    return any(path == root or root in path.parents for root in EXCLUDED_SOURCE_ROOTS)


def find_pairs() -> list[str]:
    pairs: list[str] = []

    for source_path in sorted(SOURCE_ROOT.rglob("*.c"), key=str):
        if is_excluded(source_path) or source_path.name == GENERATED_CONTEXT_NAME:
            continue

        source = source_path.read_text()
        matching_headers = {
            INCLUDE_ROOT / include_path
            for include_path in INCLUDE_RE.findall(source)
            if Path(include_path).stem == source_path.stem
            and (INCLUDE_ROOT / include_path).is_file()
        }

        for header_path in sorted(matching_headers, key=str):
            pairs.append(f"{source_path} {header_path}")

    return pairs


def main() -> None:
    parser = argparse.ArgumentParser(
        description="List source/header pairs suitable for task-runner."
    )
    parser.add_argument(
        "--json", action="store_true", help="emit a JSON array for task-runner"
    )
    args = parser.parse_args()

    pairs = find_pairs()
    if args.json:
        print(json.dumps(pairs))
    else:
        print("\n".join(pairs))


if __name__ == "__main__":
    main()
