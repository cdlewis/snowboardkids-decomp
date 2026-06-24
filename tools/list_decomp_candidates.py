#!/usr/bin/env python3
"""List remaining decompilation candidates for assigned source files.

By default this excludes functions that already have a handoff note at
nonmatchings/<func>/NEAR_MISS.md, so agents do not keep retrying documented
near misses before exhausting fresh candidates.
"""

from __future__ import annotations

import argparse
import re
from pathlib import Path


PRAGMA_RE = re.compile(r'#pragma\s+GLOBAL_ASM\("([^"]+)"\)')


def iter_pragmas(source_file: Path):
    try:
        text = source_file.read_text(errors="ignore")
    except OSError as exc:
        raise SystemExit(f"error: cannot read {source_file}: {exc}") from exc

    for line_no, line in enumerate(text.splitlines(), 1):
        match = PRAGMA_RE.search(line)
        if not match:
            continue
        asm_path = Path(match.group(1))
        func_name = asm_path.stem
        yield {
            "source": source_file,
            "line": line_no,
            "asm": asm_path,
            "func": func_name,
            "near_miss": Path("nonmatchings") / func_name / "NEAR_MISS.md",
        }


def asm_size(path: Path) -> int:
    try:
        return path.stat().st_size
    except OSError:
        return -1


def main() -> int:
    parser = argparse.ArgumentParser(
        description=(
            "List remaining GLOBAL_ASM candidates in assigned files, excluding "
            "functions with nonmatchings/<func>/NEAR_MISS.md by default."
        )
    )
    parser.add_argument(
        "files",
        nargs="+",
        help="Assigned source files to scan, e.g. src/1050.c src/562B0.c",
    )
    parser.add_argument(
        "--include-near-misses",
        action="store_true",
        help="Include functions that already have NEAR_MISS.md notes.",
    )
    parser.add_argument(
        "--summary",
        action="store_true",
        help="Print only total/open/near-miss counts.",
    )
    args = parser.parse_args()

    rows = []
    skipped_near_misses = 0
    total = 0

    for file_arg in args.files:
        for row in iter_pragmas(Path(file_arg)):
            total += 1
            row["asm_size"] = asm_size(row["asm"])
            row["has_near_miss"] = row["near_miss"].exists()
            if row["has_near_miss"] and not args.include_near_misses:
                skipped_near_misses += 1
                continue
            rows.append(row)

    rows.sort(key=lambda row: (row["asm_size"] < 0, row["asm_size"], str(row["source"]), row["line"]))

    if args.summary:
        print(f"total_pragmas={total}")
        print(f"eligible={len(rows)}")
        print(f"near_miss_skipped={skipped_near_misses}")
        return 0

    print("func\tsource:line\tasm_size\tnear_miss\tasm")
    for row in rows:
        size = "missing" if row["asm_size"] < 0 else str(row["asm_size"])
        near_miss = str(row["near_miss"]) if row["has_near_miss"] else "-"
        print(
            f"{row['func']}\t{row['source']}:{row['line']}\t"
            f"{size}\t{near_miss}\t{row['asm']}"
        )

    print(
        f"# total_pragmas={total} eligible={len(rows)} "
        f"near_miss_skipped={skipped_near_misses}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
