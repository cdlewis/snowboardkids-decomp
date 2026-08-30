#!/usr/bin/env python3
"""Verify that a test pad moves ROM symbols but does not move VRAM symbols."""

import argparse
import re
from pathlib import Path


SYMBOL_RE = re.compile(r"^\s*(0x[0-9A-Fa-f]+)\s+([A-Za-z_][A-Za-z0-9_]*)\s*=")


def parse_int(value: str) -> int:
    return int(value, 0)


def read_symbols(path: Path) -> dict[str, int]:
    symbols = {}
    for line in path.read_text(errors="replace").splitlines():
        match = SYMBOL_RE.match(line)
        if match:
            symbols.setdefault(match.group(2), int(match.group(1), 16))
    return symbols


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("base_map", type=Path)
    parser.add_argument("shifted_map", type=Path)
    parser.add_argument("--after", required=True)
    parser.add_argument("--pad-size", type=parse_int, required=True)
    args = parser.parse_args()

    base = read_symbols(args.base_map)
    shifted = read_symbols(args.shifted_map)
    boundary_name = f"{args.after}_ROM_END"
    if boundary_name not in base:
        parser.error(f"{boundary_name} is absent from {args.base_map}")
    cutoff = base[boundary_name]

    failures = []
    checked_rom = 0
    checked_vram = 0
    for name, base_value in base.items():
        if name not in shifted:
            continue
        shifted_value = shifted[name]
        if name.endswith(("_ROM_START", "_ROM_END")):
            expected = base_value + (args.pad_size if base_value >= cutoff and name != boundary_name else 0)
            checked_rom += 1
            if shifted_value != expected:
                failures.append(f"{name}: expected 0x{expected:X}, got 0x{shifted_value:X}")
        elif name.endswith(("_VRAM", "_VRAM_END")):
            checked_vram += 1
            if shifted_value != base_value:
                failures.append(f"{name}: VRAM moved from 0x{base_value:X} to 0x{shifted_value:X}")

    if failures:
        raise SystemExit("shift verification failed:\n  " + "\n  ".join(failures[:40]))
    print(f"shift map OK: checked {checked_rom} ROM symbols and {checked_vram} VRAM symbols")


if __name__ == "__main__":
    main()
