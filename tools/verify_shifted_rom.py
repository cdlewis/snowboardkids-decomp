#!/usr/bin/env python3
"""Verify linked ROM-boundary words in the main code/data segment relocate."""

import argparse
import re
import struct
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
    parser.add_argument("base_rom", type=Path)
    parser.add_argument("shifted_rom", type=Path)
    parser.add_argument("base_map", type=Path)
    parser.add_argument("shifted_map", type=Path)
    parser.add_argument("--after", required=True)
    parser.add_argument("--pad-size", type=parse_int, required=True)
    args = parser.parse_args()

    base_symbols = read_symbols(args.base_map)
    shifted_symbols = read_symbols(args.shifted_map)
    cutoff = base_symbols[f"{args.after}_ROM_END"]
    main_start = base_symbols["main_ROM_START"]
    main_end = base_symbols["main_ROM_END"]
    main_vram = base_symbols["main_VRAM"]
    data_start = main_start + (base_symbols["main_DATA_START"] - main_vram)
    base_rom = args.base_rom.read_bytes()
    shifted_rom = args.shifted_rom.read_bytes()
    boundaries = {
        value
        for name, value in base_symbols.items()
        if name.endswith(("_ROM_START", "_ROM_END")) and cutoff <= value < len(base_rom)
    }

    checked = 0
    failures = []
    # MIPS instruction words can coincidentally equal a ROM boundary. Full ROM
    # pointers live in linked data/rodata; code references are covered by the
    # source scanner and linker relocations.
    for base_offset in range(data_start, main_end - 3, 4):
        value = struct.unpack_from(">I", base_rom, base_offset)[0]
        if value not in boundaries:
            continue
        shifted_offset = base_offset + (args.pad_size if base_offset >= cutoff else 0)
        shifted_value = struct.unpack_from(">I", shifted_rom, shifted_offset)[0]
        expected = value + args.pad_size
        checked += 1
        if shifted_value != expected:
            failures.append(
                f"ROM 0x{base_offset:X}: pointer 0x{value:X} expected 0x{expected:X}, got 0x{shifted_value:X}"
            )

    if failures:
        raise SystemExit("ROM relocation verification failed:\n  " + "\n  ".join(failures[:40]))
    print(f"shifted ROM OK: checked {checked} relocated boundary word(s) in main")


if __name__ == "__main__":
    main()
