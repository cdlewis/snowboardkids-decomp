#!/usr/bin/env python3
"""Inject ROM-only padding after a linker segment for shiftability testing."""

import argparse
from pathlib import Path


def parse_int(value: str) -> int:
    return int(value, 0)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--after", required=True, help="segment name without _ROM_END")
    parser.add_argument("--pad-size", type=parse_int, default=0x10)
    args = parser.parse_args()

    if args.pad_size <= 0 or args.pad_size % 16 != 0:
        parser.error("--pad-size must be a positive multiple of 16")

    source = args.input.read_text()
    marker = f"    {args.after}_ROM_END = __romPos;"
    if source.count(marker) != 1:
        parser.error(f"expected exactly one linker marker: {marker.strip()}")

    injection = (
        f"{marker}\n"
        f"    /* ROM shiftability test padding; VRAM location counter is unchanged. */\n"
        f"    __romPos += 0x{args.pad_size:X};"
    )
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(source.replace(marker, injection, 1))


if __name__ == "__main__":
    main()
