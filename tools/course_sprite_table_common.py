#!/usr/bin/env python3
"""Parsing and packing helpers for Snowboard Kids course sprite tables."""

from __future__ import annotations

import struct

from tools.course_graphics_common import parse_int


def parse_course_sprite_table(data: bytes) -> dict:
    if len(data) < 8:
        raise ValueError("course sprite table is shorter than its header")
    unknown_0, entry_count = struct.unpack(">II", data[:8])
    entry_end = 8 + entry_count * 8
    if entry_end > len(data):
        raise ValueError(f"course sprite table has invalid entry count {entry_count}")

    entries = []
    image_specs: dict[int, tuple[int, int]] = {}
    palette_count = 0
    for index in range(entry_count):
        offset = 8 + index * 8
        image_offset, palette_index, width, height = struct.unpack(">IHBB", data[offset : offset + 8])
        entries.append(
            {
                "name": f"sprite_{index:02d}",
                "image_offset": image_offset,
                "palette_index": palette_index,
                "width": width,
                "height": height,
            }
        )
        dimensions = (width, height)
        if image_offset in image_specs and image_specs[image_offset] != dimensions:
            raise ValueError(f"shared sprite image at 0x{image_offset:X} has inconsistent dimensions")
        image_specs[image_offset] = dimensions
        palette_count = max(palette_count, palette_index + 1)

    palette_end = entry_end + palette_count * 0x20
    if palette_end > len(data):
        raise ValueError("course sprite palette table exceeds the decompressed asset")
    palettes = []
    for index in range(palette_count):
        offset = entry_end + index * 0x20
        palettes.append(
            {
                "name": f"palette_{index:02d}",
                "offset": offset,
                "format": "rgba16",
                "colors": 16,
                "values": [int.from_bytes(data[pos : pos + 2], "big") for pos in range(offset, offset + 0x20, 2)],
            }
        )

    images = []
    covered = [(0, palette_end)]
    for image_offset, (width, height) in sorted(image_specs.items()):
        size = (width * height + 1) // 2
        end = image_offset + size
        if image_offset < palette_end or end > len(data):
            raise ValueError(f"sprite image range 0x{image_offset:X}-0x{end:X} is invalid")
        images.append(
            {
                "name": f"image_{image_offset:04X}",
                "offset": image_offset,
                "format": "ci4",
                "width": width,
                "height": height,
                "data": data[image_offset:end].hex(),
            }
        )
        covered.append((image_offset, end))

    raw_parts = []
    cursor = 0
    for start, end in sorted(covered):
        if start < cursor:
            raise ValueError(f"course sprite table ranges overlap at 0x{start:X}")
        if start > cursor:
            raw_parts.append({"offset": cursor, "data": data[cursor:start].hex()})
        cursor = end
    if cursor < len(data):
        raw_parts.append({"offset": cursor, "data": data[cursor:].hex()})

    return {
        "unknown_0": unknown_0,
        "entry_count": entry_count,
        "entries": entries,
        "palettes": palettes,
        "images": images,
        "raw_parts": raw_parts,
    }


def pack_course_sprite_table(manifest: dict) -> bytes:
    size = parse_int(manifest["decompressed_size"])
    output = bytearray(size)
    written = bytearray(size)

    def write(start: int, data: bytes, description: str) -> None:
        end = start + len(data)
        if start < 0 or end > size:
            raise ValueError(f"{description} range 0x{start:X}-0x{end:X} exceeds decompressed size")
        if any(written[start:end]):
            raise ValueError(f"{description} overlaps an earlier part at 0x{start:X}")
        output[start:end] = data
        written[start:end] = b"\x01" * len(data)

    entries = manifest["entries"]
    if parse_int(manifest["entry_count"]) != len(entries):
        raise ValueError("course sprite entry count does not match its entries")
    write(0, struct.pack(">II", parse_int(manifest["unknown_0"]), len(entries)), "sprite header")
    for index, entry in enumerate(entries):
        write(
            8 + index * 8,
            struct.pack(
                ">IHBB",
                parse_int(entry["image_offset"]),
                parse_int(entry["palette_index"]),
                parse_int(entry["width"]),
                parse_int(entry["height"]),
            ),
            f"sprite entry {index}",
        )

    for palette in manifest["palettes"]:
        values = b"".join(parse_int(value).to_bytes(2, "big") for value in palette["values"])
        if len(values) != parse_int(palette["colors"]) * 2:
            raise ValueError(f"sprite palette {palette['name']} has the wrong color count")
        write(parse_int(palette["offset"]), values, f"sprite palette {palette['name']}")

    for image in manifest["images"]:
        data = bytes.fromhex(str(image["data"]))
        expected = (parse_int(image["width"]) * parse_int(image["height"]) + 1) // 2
        if len(data) != expected:
            raise ValueError(f"sprite image {image['name']} has 0x{len(data):X} bytes, expected 0x{expected:X}")
        write(parse_int(image["offset"]), data, f"sprite image {image['name']}")

    for raw in manifest.get("raw_parts", []):
        write(parse_int(raw["offset"]), bytes.fromhex(str(raw["data"])), "raw sprite data")

    if not all(written):
        raise ValueError(f"course sprite manifest does not cover decompressed offset 0x{written.index(0):X}")
    return bytes(output)
