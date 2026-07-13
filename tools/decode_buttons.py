#!/usr/bin/env python3
"""
Decode N64 controller button bitmasks.

Takes a hex number and returns which buttons are pressed.
"""

from __future__ import annotations

import argparse


# Button definitions from include/PR/os_cont.h.
BUTTONS = {
    0x80000: "STICK_LEFT",
    0x40000: "STICK_RIGHT",
    0x20000: "STICK_DOWN",
    0x10000: "STICK_UP",
    0x8000: "A_BUTTON (CONT_A)",
    0x4000: "B_BUTTON (CONT_B)",
    0x2000: "Z_TRIG (CONT_G)",
    0x1000: "START_BUTTON (CONT_START)",
    0x0800: "U_JPAD (CONT_UP)",
    0x0400: "D_JPAD (CONT_DOWN)",
    0x0200: "L_JPAD (CONT_LEFT)",
    0x0100: "R_JPAD (CONT_RIGHT)",
    0x0020: "L_TRIG (CONT_L)",
    0x0010: "R_TRIG (CONT_R)",
    0x0008: "U_CBUTTONS (CONT_E)",
    0x0004: "D_CBUTTONS (CONT_D)",
    0x0002: "L_CBUTTONS (CONT_C)",
    0x0001: "R_CBUTTONS (CONT_F)",
}


def parse_value(value: str) -> int:
    if value.startswith("0x"):
        return int(value, 0)
    return int(value, 16)


def decode_buttons(value: int) -> tuple[list[tuple[int, str]], int]:
    """Decode a button bitmask and return pressed buttons and unknown bits."""
    pressed = []
    remaining = value

    for mask, name in sorted(BUTTONS.items(), reverse=True):
        if value & mask:
            pressed.append((mask, name))
            remaining &= ~mask

    return pressed, remaining


def main() -> int:
    parser = argparse.ArgumentParser(description="Decode an N64 controller button bitmask.")
    parser.add_argument("value", help="hex value to decode, with or without 0x prefix")
    args = parser.parse_args()

    try:
        value = parse_value(args.value)
    except ValueError as error:
        print(f"Error: Invalid hex value '{args.value}'")
        print(f"Details: {error}")
        return 1

    pressed, remaining = decode_buttons(value)

    print(f"Bitmask: {args.value} (decimal: {value}, binary: {bin(value)})")
    print()

    if pressed:
        print("Buttons pressed:")
        for mask, name in pressed:
            print(f"  - 0x{mask:04X} = {name}")
    else:
        print("No recognized buttons pressed")

    if remaining != 0:
        print()
        print(f"Warning: Unrecognized bits: 0x{remaining:04X}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
