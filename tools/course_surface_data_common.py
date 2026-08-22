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
        lz_tokens=(
            bytes.fromhex(str(compression["lz_tokens"]))
            if compression.get("lz_tokens") is not None
            else None
        ),
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
        coordinate_indices = face["coordinate_indices"]
        output.extend(
            struct.pack(
                ">HHHbB",
                *(parse_int(index) for index in coordinate_indices),
                parse_int(face["surface_type"]),
                parse_int(face["skip_first_edge_check"]),
            )
        )

    surfaces = manifest["surfaces"]
    output.extend(len(surfaces).to_bytes(2, "big"))
    for surface in surfaces:
        output.extend(struct.pack(">hhhh", *(parse_int(index) for index in surface["neighbor_indices"])))
        output.extend(struct.pack(">hhhh", *(parse_int(index) for index in surface["boundary_coordinate_indices"])))
        output.extend(
            struct.pack(
                ">hhHHHH",
                parse_int(surface["reference_coordinate_index"]),
                parse_int(surface["path_angle"]),
                parse_int(surface["face_start_index"]),
                parse_int(surface["face_end_index"]),
                parse_int(surface["unused_18"]),
                parse_int(surface["edge_clamp_flags"]),
            )
        )

    return bytes(output)
