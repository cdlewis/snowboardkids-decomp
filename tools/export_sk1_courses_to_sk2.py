#!/usr/bin/env python3
"""Export one or all SK1 courses as ROM-derived Snowboard Kids 2 packages."""

from __future__ import annotations

import argparse
from pathlib import Path
import sys

REPO_ROOT = Path(__file__).resolve().parent.parent
if str(REPO_ROOT) not in sys.path:
    sys.path.insert(0, str(REPO_ROOT))

from tools.sk1_to_sk2_course import (
    export_course,
    load_course_configs,
    load_graphics_metadata,
    verify_rom,
)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, default=Path("snowboardkids.z64"))
    parser.add_argument("--output", type=Path, default=Path("build/sk2_course_export"))
    selection = parser.add_mutually_exclusive_group(required=True)
    selection.add_argument("--all", action="store_true")
    selection.add_argument("--course", help="course key, for example big_snowman")
    args = parser.parse_args()

    repo_root = REPO_ROOT
    rom = args.rom.read_bytes()
    verify_rom(rom)
    courses = load_course_configs(repo_root)
    if args.course:
        courses = [course for course in courses if course["key"] == args.course]
        if not courses:
            parser.error(f"unknown course key {args.course!r}")
    metadata = load_graphics_metadata(repo_root)
    for course in courses:
        destination = export_course(rom, course, metadata, args.output)
        print(f"{course['name']}: {destination}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
