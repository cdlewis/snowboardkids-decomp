#!/usr/bin/env python3
"""Shared parsing and packing helpers for editable SK1 course surface data."""

from __future__ import annotations

import struct
from pathlib import Path

import yaml

from tools.huffman_asset import CompressionMetadata


def parse_int(value) -> int:
    return int(value, 0) if isinstance(value, str) else int(value)


def load_yaml(path: Path) -> dict:
    with path.open("r", encoding="utf-8") as stream:
        return yaml.safe_load(stream)


def write_yaml(path: Path, data: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(yaml.safe_dump(data, sort_keys=False), encoding="utf-8", newline="\n")


def metadata_from_manifest(manifest: dict) -> CompressionMetadata:
    compression = manifest["compression"]
    return CompressionMetadata(
        flags=parse_int(compression["flags"]),
        table=bytes.fromhex(str(compression["huffman_table"])),
        padding_bits=str(compression.get("padding_bits", "")),
        unused_tail=bytes.fromhex(str(compression.get("unused_tail", ""))),
    )


def pack_course_surface_data(manifest: dict) -> bytes:
    output = bytearray()

    coordinates = manifest["coordinates"]
    output.extend(len(coordinates).to_bytes(2, "big"))
    for coordinate in coordinates:
        output.extend(
            struct.pack(
                ">hhh",
                parse_int(coordinate["x"]),
                parse_int(coordinate["y"]),
                parse_int(coordinate["z"]),
            )
        )

    faces = manifest["faces"]
    output.extend(len(faces).to_bytes(2, "big"))
    for face in faces:
        output.extend(
            struct.pack(
                ">HHHbB",
                parse_int(face["coord0"]),
                parse_int(face["coord1"]),
                parse_int(face["coord2"]),
                parse_int(face["face_index"]),
                parse_int(face["unknown_07"]),
            )
        )

    for surface in manifest["surfaces"]:
        output.extend(struct.pack(">hhhh", *(parse_int(surface[f"neighbor{i}"]) for i in range(4))))
        output.extend(struct.pack(">HHHH", *(parse_int(surface[f"coord{i}"]) for i in range(4))))
        output.extend(struct.pack(">hh", parse_int(surface["position_index"]), parse_int(surface["angle"])))
        output.extend(struct.pack(">HHHH", *(parse_int(surface[f"unknown_{offset:02x}"]) for offset in range(0x14, 0x1C, 2))))

    output.extend(parse_int(manifest["final_value"]).to_bytes(2, "big"))
    return bytes(output)
