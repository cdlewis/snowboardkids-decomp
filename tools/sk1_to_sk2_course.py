#!/usr/bin/env python3
"""ROM-driven conversion of Snowboard Kids course assets to SK2 layouts."""

from __future__ import annotations

import hashlib
import struct
from pathlib import Path
from typing import Any

import yaml

from tools.course_graphics_common import trace_course_graphics
from tools.course_sprite_table_common import parse_course_sprite_table
from tools.huffman_asset import decompress_huffman_asset

EXPECTED_ROM_SHA1 = "1583bacc9046a360df8ea4d536942155247e154c"
FIXED_POINT_SCALE_DIVISOR = 2

# Indices instantiated by race_scene_setup.c for each course.
PICKUP_INDICES = (
    (*range(0x00, 0x06), 0x3F, *range(0x4E, 0x53)),
    (*range(0x06, 0x0E), *range(0x53, 0x57)),
    (*range(0x0E, 0x15), 0x42, *range(0x57, 0x5A)),
    (*range(0x15, 0x1E), *range(0x43, 0x46), *range(0x5A, 0x5E)),
    (*range(0x1E, 0x27), *range(0x5E, 0x61)),
    (*range(0x27, 0x2F), 0x40, 0x41, *range(0x61, 0x67)),
    (*range(0x2F, 0x37), *range(0x67, 0x6B)),
    (),
    (*range(0x46, 0x4C),),
    (0x37, 0x38, 0x39, 0x3B, 0x3C, 0x3D),
)


def parse_int(value: Any) -> int:
    return int(value, 0) if isinstance(value, str) else int(value)


def fixed17_to_fixed16(value: int) -> int:
    """Convert signed SK1 fixed point to SK2 while preserving real position."""
    return value // FIXED_POINT_SCALE_DIVISOR


def align(value: int, alignment: int = 0x10) -> int:
    return (value + alignment - 1) & ~(alignment - 1)


def compress_sno(data: bytes) -> bytes:
    """Encode the simple LZ stream used by Snowboard Kids 2 assets."""
    output = bytearray()
    positions: list[list[int]] = [[] for _ in range(0x100)]
    cursor = 0
    while cursor < len(data):
        best_count = 0
        best_offset = 0
        window_start = max(0, cursor - 0xFFE)
        for candidate in reversed(positions[data[cursor]]):
            if candidate < window_start:
                break
            count = 0
            while (
                count < 0xF
                and cursor + count < len(data)
                and data[candidate + count] == data[cursor + count]
            ):
                count += 1
            if count > best_count:
                best_count = count
                best_offset = cursor - candidate
                if count == 0xF:
                    break
        if best_count:
            output.extend(((best_count << 4) | (best_offset >> 8), best_offset & 0xFF))
            for value in data[cursor : cursor + best_count]:
                positions[value].append(cursor)
                cursor += 1
        else:
            output.extend((0, data[cursor]))
            positions[data[cursor]].append(cursor)
            cursor += 1
    output.extend(bytes(align(len(output)) - len(output)))
    return bytes(output)


def decompress_sno(data: bytes, output_size: int) -> bytes:
    output = bytearray()
    cursor = 0
    while len(output) < output_size:
        first, second = data[cursor : cursor + 2]
        cursor += 2
        if first == 0:
            output.append(second)
            continue
        count = first >> 4
        distance = ((first & 0xF) << 8) | second
        if not count or not distance or distance > len(output):
            raise ValueError("invalid Sno back-reference")
        for _ in range(count):
            output.append(output[-distance])
            if len(output) == output_size:
                break
    return bytes(output)


def _convert_geometry_flags(flags: int) -> int:
    converted = flags & (0x00000001 | 0x00000004 | 0x001F0000)
    if flags & 0x00000200:
        converted |= 0x00200000
    if flags & 0x00001000:
        converted |= 0x00000200
    if flags & 0x00002000:
        converted |= 0x00000400
    return converted


def _remap_resource_address(address: int) -> int:
    segment = address >> 24
    if segment == 2:
        return 0x01000000 | (address & 0xFFFFFF)
    if segment == 3:
        return 0x02000000 | (address & 0xFFFFFF)
    return address


def convert_f3dex_command(word0: int, word1: int) -> tuple[int, int]:
    """Translate one reachable SK1 F3DEX command to F3DEX2."""
    opcode = word0 >> 24
    if opcode == 0x04:  # G_VTX
        count = (word0 >> 10) & 0x3F
        v0 = ((word0 >> 16) & 0xFF) // 2
        word0 = 0x01000000 | (count << 12) | ((v0 + count) << 1)
        word1 = _remap_resource_address(word1)
    elif opcode == 0x06:  # G_DL
        word0 = 0xDE000000 | (word0 & 0x00FFFFFF)
        if word1 >> 24 == 2:
            word1 = 0x01000000 | (word1 & 0xFFFFFF)
    elif opcode == 0xB1:  # G_TRI2
        word0 = 0x06000000 | (word0 & 0xFFFFFF)
    elif opcode == 0xB2:  # G_MODIFYVTX
        word0 = 0x02000000 | (word0 & 0xFFFFFF)
    elif opcode == 0xB6:  # G_CLEARGEOMETRYMODE
        flags = _convert_geometry_flags(word1)
        word0, word1 = 0xD9000000 | ((~flags) & 0xFFFFFF), 0
    elif opcode == 0xB7:  # G_SETGEOMETRYMODE
        word0, word1 = 0xD9FFFFFF, _convert_geometry_flags(word1)
    elif opcode == 0xB8:  # G_ENDDL
        word0, word1 = 0xDF000000, 0
    elif opcode in (0xB9, 0xBA):  # G_SETOTHERMODE_L/H
        shift = (word0 >> 8) & 0xFF
        length = word0 & 0xFF
        new_opcode = 0xE2 if opcode == 0xB9 else 0xE3
        word0 = (new_opcode << 24) | ((32 - shift - length) << 8) | (length - 1)
    elif opcode == 0xBB:  # G_TEXTURE (the enable field moves left one bit)
        word0 = 0xD7000000 | (word0 & 0x00FFFF00) | ((word0 & 0xFF) << 1)
    elif opcode == 0xBE:  # G_CULLDL
        word0 = 0x03000000 | (word0 & 0xFFFF)
    elif opcode == 0xBF:  # G_TRI1
        word0, word1 = 0x05000000 | (word1 & 0xFFFFFF), 0
    elif opcode in (0xE6, 0xE7, 0xE8, 0xF0, 0xF2, 0xF3, 0xF5, 0xFC, 0xFD):
        if opcode == 0xFD:
            word1 = _remap_resource_address(word1)
    else:
        raise ValueError(f"unsupported reachable F3DEX opcode 0x{opcode:02X}")
    return word0, word1


def convert_display_lists(
    bundle: bytes, root_offsets: list[int]
) -> tuple[bytes, dict[int, int]]:
    graph = trace_course_graphics(bundle, root_offsets)
    output = bytearray(bundle)
    counts: dict[int, int] = {}
    for node in graph.display_lists:
        for offset in range(node.offset, node.end, 8):
            word0, word1 = struct.unpack_from(">II", bundle, offset)
            counts[word0 >> 24] = counts.get(word0 >> 24, 0) + 1
            struct.pack_into(
                ">II", output, offset, *convert_f3dex_command(word0, word1)
            )
    return bytes(output), counts


def parse_surface_data(data: bytes) -> dict[str, list[dict[str, Any]]]:
    cursor = 0
    coordinate_count = struct.unpack_from(">H", data, cursor)[0]
    cursor += 2
    coordinates = []
    for _ in range(coordinate_count):
        x, y, z = struct.unpack_from(">hhh", data, cursor)
        cursor += 6
        coordinates.append({"x": x, "y": y, "z": z})
    face_count = struct.unpack_from(">H", data, cursor)[0]
    cursor += 2
    faces = []
    for _ in range(face_count):
        v0, v1, v2, surface_type, skip = struct.unpack_from(">HHHbB", data, cursor)
        cursor += 8
        faces.append(
            {
                "coordinate_indices": [v0, v1, v2],
                "surface_type": surface_type,
                "skip": skip,
            }
        )
    surface_count = struct.unpack_from(">H", data, cursor)[0]
    cursor += 2
    surfaces = []
    for _ in range(surface_count):
        neighbors = list(struct.unpack_from(">hhhh", data, cursor))
        boundary = list(struct.unpack_from(">hhhh", data, cursor + 8))
        reference, angle, face_start, face_end, unused, clamps = struct.unpack_from(
            ">hhHHHH", data, cursor + 16
        )
        cursor += 0x1C
        surfaces.append(
            {
                "neighbors": neighbors,
                "boundary": boundary,
                "reference": reference,
                "path_angle": angle,
                "face_start": face_start,
                "face_end": face_end,
                "unused": unused,
                "edge_clamp_flags": clamps,
            }
        )
    if cursor != len(data):
        raise ValueError(f"surface data has 0x{len(data) - cursor:X} trailing bytes")
    return {"coordinates": coordinates, "faces": faces, "surfaces": surfaces}


def build_track_mesh(surface_data: bytes) -> tuple[bytes, dict[str, int]]:
    source = parse_surface_data(surface_data)
    output = bytearray(struct.pack(">H", len(source["coordinates"])))
    for coordinate in source["coordinates"]:
        output.extend(
            struct.pack(">hhh", coordinate["x"], coordinate["y"], coordinate["z"])
        )
    output.extend(struct.pack(">H", len(source["faces"])))
    surface_types: set[int] = set()
    for face in source["faces"]:
        surface_type = face["surface_type"] & 0x3F
        surface_types.add(surface_type)
        flags = (surface_type << 2) | (face["skip"] & 1)
        output.extend(struct.pack(">HHHBB", *face["coordinate_indices"], flags, 0))
    output.extend(struct.pack(">H", len(source["surfaces"])))
    for surface in source["surfaces"]:
        forward = surface["neighbors"][1]
        next_reference = surface["reference"]
        if 0 <= forward < len(source["surfaces"]):
            next_reference = source["surfaces"][forward]["reference"]
        count = surface["face_end"] - surface["face_start"]
        output.extend(struct.pack(">hhhh", *surface["neighbors"]))
        output.extend(bytes(4))
        output.extend(
            struct.pack(
                ">HHHH", surface["face_start"], count, surface["face_start"], count
            )
        )
        output.extend(
            struct.pack(
                ">HHHH",
                surface["boundary"][0],
                surface["reference"] & 0xFFFF,
                surface["boundary"][1],
                surface["boundary"][2],
            )
        )
        output.extend(
            struct.pack(">HH", next_reference & 0xFFFF, surface["boundary"][3])
        )
        output.extend(bytes(4))
    return bytes(output), {
        "vertex_count": len(source["coordinates"]),
        "face_count": len(source["faces"]),
        "sector_count": len(source["surfaces"]),
        "distinct_sk1_surface_types": len(surface_types),
    }


def build_texture_table(sprite_data: bytes) -> tuple[bytes, int]:
    source = parse_course_sprite_table(sprite_data)
    entries = source["entries"]
    output = bytearray(struct.pack(">II", 0, len(entries)))
    payload_offset = 8 + len(entries) * 0x10 + len(source["palettes"]) * 0x20
    image_offsets: dict[int, int] = {}
    cursor = payload_offset
    images_by_offset = {image["offset"]: image for image in source["images"]}
    for old_offset in sorted(images_by_offset):
        image_offsets[old_offset] = cursor
        cursor += len(bytes.fromhex(images_by_offset[old_offset]["data"]))
    for entry in entries:
        output.extend(
            struct.pack(
                ">IHHH",
                image_offsets[entry["image_offset"]],
                entry["palette_index"],
                entry["width"],
                entry["height"],
            )
        )
        output.extend(bytes(6))
    for palette in source["palettes"]:
        output.extend(b"".join(value.to_bytes(2, "big") for value in palette["values"]))
    for old_offset in sorted(images_by_offset):
        output.extend(bytes.fromhex(images_by_offset[old_offset]["data"]))
    return bytes(output), len(entries)


def _position_payload(records: list[bytes], sentinel: bytes) -> bytes:
    data = bytearray(struct.pack(">I", 4))
    for record in records:
        data.extend(record)
    data.extend(sentinel)
    data.extend(bytes(align(len(data)) - len(data)))
    return bytes(data)


def build_gold_coins(rom: bytes, course_id: int) -> tuple[bytes, int, str | None]:
    if course_id == 7:
        return (
            _position_payload([], bytes.fromhex("ff000000000000000000000000000000")),
            0,
            "Animal Land reuses course 0's SK1 pointer; omitted for trick mode",
        )
    pointer = struct.unpack_from(">I", rom, 0xD9DE8 + course_id * 4)[0]
    cursor = pointer - 0x7FFFF400
    records = []
    while True:
        enabled, model_index, x, y, z = struct.unpack_from(">hhiii", rom, cursor)
        cursor += 0x10
        if model_index == -1:
            break
        records.append(
            struct.pack(
                ">bbhiii",
                0,
                0,
                0,
                fixed17_to_fixed16(x),
                fixed17_to_fixed16(y),
                fixed17_to_fixed16(z),
            )
        )
    return (
        _position_payload(records, bytes.fromhex("ff000000000000000000000000000000")),
        len(records),
        None,
    )


def build_item_boxes(rom: bytes, course_id: int) -> tuple[bytes, int]:
    records = []
    for index in PICKUP_INDICES[course_id]:
        x, y, z, rotation, variant = struct.unpack_from(
            ">iiihh", rom, 0xDA118 + index * 0x10
        )
        if variant not in (0, 1):
            raise ValueError(f"pickup {index:#x} has unsupported SK1 variant {variant}")
        records.append(
            struct.pack(
                ">hhiii",
                variant,
                rotation,
                fixed17_to_fixed16(x),
                fixed17_to_fixed16(y),
                fixed17_to_fixed16(z),
            )
        )
    return _position_payload(
        records, bytes.fromhex("ffff0000000000000000000000000000")
    ), len(records)


def load_course_configs(repo_root: Path) -> list[dict[str, Any]]:
    return [
        yaml.safe_load(path.read_text(encoding="utf-8"))
        for path in sorted((repo_root / "config/courses").glob("*.yaml"))
    ]


def load_graphics_metadata(repo_root: Path) -> dict[str, dict[str, Any]]:
    config = yaml.safe_load(
        (repo_root / "snowboardkids.yaml").read_text(encoding="utf-8")
    )
    return {
        segment["name"]: segment
        for segment in config["segments"]
        if isinstance(segment, dict) and segment.get("type") == "course_model_resources"
    }


def build_runtime_hints(
    rom: bytes, course: dict[str, Any], root_offsets: list[int]
) -> dict[str, Any]:
    course_id = int(course["legacy_id"])
    offset = 0xBA140 + course_id * 0x48

    def s16(relative: int) -> int:
        return struct.unpack_from(">h", rom, offset + relative)[0]

    def s32(relative: int) -> int:
        return struct.unpack_from(">i", rom, offset + relative)[0]

    def position(relative: int) -> dict[str, int]:
        return {
            axis: fixed17_to_fixed16(s32(relative + index * 4))
            for index, axis in enumerate(("x", "y", "z"))
        }

    start = {
        "path_index": s16(0x00),
        "lap_count": s16(0x02),
        "spiral_exit_sector": s16(0x04),
        "spiral_origin": position(0x08),
        "spiral_angle": s16(0x14),
        "launch_ramp_angle": s16(0x16),
        "launch_ramp_target": {
            "x": fixed17_to_fixed16(s32(0x18)),
            "z": fixed17_to_fixed16(s32(0x1C)),
        },
        "launch_ramp_origin": position(0x20),
        "camera_position": position(0x2C),
        "positive_lookahead_start": s16(0x38),
        "positive_lookahead_end": s16(0x3A),
        "finish_line_sector": s16(0x3C),
        "max_sector": s16(0x3E),
        "start_surface_target": {
            "x": fixed17_to_fixed16(s32(0x40)),
            "z": fixed17_to_fixed16(s32(0x44)),
        },
    }

    pointer = struct.unpack_from(">I", rom, 0xDB33C + course_id * 4)[0]
    cursor = pointer - 0x7FFFF400
    scenery = []
    while True:
        model_index, unknown_02, x, y, z, rotation, unknown_12 = struct.unpack_from(
            ">hhiiihh", rom, cursor
        )
        cursor += 0x14
        if model_index == -1:
            break
        scenery.append(
            {
                "model_index": model_index,
                "unknown_02": unknown_02,
                "position": {
                    "x": fixed17_to_fixed16(x),
                    "y": fixed17_to_fixed16(y),
                    "z": fixed17_to_fixed16(z),
                },
                "rotation": rotation,
                "unknown_12": unknown_12,
            }
        )
    render_names = [
        course["render"]["main_display_list"],
        *course["render"]["backdrop_display_lists"],
    ]
    return {
        "coordinate_conversion": "signed fixed-point values divided by 2 (SK1 fraction bits 17 to SK2 fraction bits 16)",
        "render_entry_points": [
            {
                "role": "main" if index == 0 else "backdrop",
                "symbol": name,
                "segment_1_offset": root_offsets[index],
            }
            for index, name in enumerate(render_names)
        ],
        "race_start": start,
        "scenery_entries": scenery,
    }


def verify_rom(rom: bytes) -> None:
    digest = hashlib.sha1(rom).hexdigest()
    if digest != EXPECTED_ROM_SHA1:
        raise ValueError(f"unsupported ROM SHA1 {digest}; expected {EXPECTED_ROM_SHA1}")


def _write_asset(
    directory: Path, name: str, data: bytes, sno: bool = True
) -> dict[str, Any]:
    raw_path = directory / f"{name}.bin"
    raw_path.write_bytes(data)
    result: dict[str, Any] = {
        "path": f"assets/{raw_path.name}",
        "decompressed_size": len(data),
        "sha256": hashlib.sha256(data).hexdigest(),
    }
    if sno:
        compressed = compress_sno(data)
        sno_path = directory / f"{name}.sno"
        sno_path.write_bytes(compressed)
        result.update(
            {
                "sno_path": f"assets/{sno_path.name}",
                "compressed_size": len(compressed),
                "sno_sha256": hashlib.sha256(compressed).hexdigest(),
            }
        )
    return result


def _write_source_payload(directory: Path, name: str, data: bytes) -> dict[str, Any]:
    path = directory / name
    path.write_bytes(data)
    return {
        "path": f"source/{name}",
        "size": len(data),
        "sha256": hashlib.sha256(data).hexdigest(),
    }


def validate_exported_course(directory: Path) -> None:
    """Validate package paths, compression, and variable-length SK2 layouts."""
    manifest = yaml.safe_load((directory / "manifest.yaml").read_text(encoding="utf-8"))
    for name, source in manifest["sk1_source_payloads"].items():
        data = (directory / source["path"]).read_bytes()
        if (
            len(data) != source["size"]
            or hashlib.sha256(data).hexdigest() != source["sha256"]
        ):
            raise ValueError(
                f"{directory}: {name} source payload does not match manifest"
            )
    for name, asset in manifest["assets"].items():
        if asset is None:
            continue
        raw = (directory / asset["path"]).read_bytes()
        if len(raw) != asset["decompressed_size"]:
            raise ValueError(f"{directory}: {name} raw size does not match manifest")
        if hashlib.sha256(raw).hexdigest() != asset["sha256"]:
            raise ValueError(
                f"{directory}: {name} raw checksum does not match manifest"
            )
        if "sno_path" in asset:
            compressed = (directory / asset["sno_path"]).read_bytes()
            if len(compressed) != asset["compressed_size"] or len(compressed) % 0x10:
                raise ValueError(f"{directory}: {name} Sno size/alignment is invalid")
            if hashlib.sha256(compressed).hexdigest() != asset["sno_sha256"]:
                raise ValueError(
                    f"{directory}: {name} Sno checksum does not match manifest"
                )
            if decompress_sno(compressed, len(raw)) != raw:
                raise ValueError(f"{directory}: {name} Sno round-trip failed")

    display_lists = (
        directory / manifest["assets"]["display_lists"]["path"]
    ).read_bytes()
    visited: set[int] = set()
    pending = list(manifest["display_list_root_offsets"])
    while pending:
        root = pending.pop()
        if root in visited:
            continue
        visited.add(root)
        cursor = root
        while cursor + 8 <= len(display_lists):
            word0, word1 = struct.unpack_from(">II", display_lists, cursor)
            opcode = word0 >> 24
            if opcode == 0xDE:
                if word1 >> 24 != 1:
                    raise ValueError(
                        f"{directory}: display-list call at 0x{cursor:X} is not segment 1"
                    )
                pending.append(word1 & 0xFFFFFF)
            elif opcode in (0x01, 0xFD) and word1 >> 24 == 3:
                raise ValueError(
                    f"{directory}: resource reference at 0x{cursor:X} still uses segment 3"
                )
            cursor += 8
            if opcode == 0xDF:
                break
        else:
            raise ValueError(f"{directory}: F3DEX2 list at 0x{root:X} has no G_ENDDL")

    track = (directory / manifest["assets"]["track_mesh"]["path"]).read_bytes()
    vertex_count = int.from_bytes(track[0:2], "big")
    cursor = 2 + vertex_count * 6
    face_count = int.from_bytes(track[cursor : cursor + 2], "big")
    cursor += 2 + face_count * 8
    sector_count = int.from_bytes(track[cursor : cursor + 2], "big")
    cursor += 2
    if cursor + sector_count * 0x24 != len(track):
        raise ValueError(
            f"{directory}: track mesh count fields do not describe its size"
        )
    counts = manifest["counts"]
    if (vertex_count, face_count, sector_count) != (
        counts["vertex_count"],
        counts["face_count"],
        counts["sector_count"],
    ):
        raise ValueError(f"{directory}: track mesh counts do not match manifest")

    texture_asset = manifest["assets"]["texture_table"]
    if texture_asset is not None:
        texture = (directory / texture_asset["path"]).read_bytes()
        entry_count = int.from_bytes(texture[4:8], "big")
        palette_base = 8 + entry_count * 0x10
        for index in range(entry_count):
            entry_offset = 8 + index * 0x10
            image_offset, palette, width, height = struct.unpack_from(
                ">IHHH", texture, entry_offset
            )
            if palette_base + (palette + 1) * 0x20 > len(texture) or image_offset + (
                width * height + 1
            ) // 2 > len(texture):
                raise ValueError(
                    f"{directory}: texture table entry {index} is out of range"
                )


def export_course(
    rom: bytes,
    course: dict[str, Any],
    graphics_meta: dict[str, dict[str, Any]],
    output_root: Path,
) -> Path:
    course_id = int(course["legacy_id"])
    directory = output_root / course["key"]
    assets_dir = directory / "assets"
    assets_dir.mkdir(parents=True, exist_ok=True)
    source_dir = directory / "source"
    source_dir.mkdir(parents=True, exist_ok=True)
    source_assets = course["assets"]
    meta = graphics_meta[source_assets["model_resources"]["symbol"]]

    graphics = rom[
        parse_int(source_assets["graphics"]["rom_start"]) : parse_int(
            source_assets["graphics"]["rom_end"]
        )
    ]
    display_lists, opcode_counts = convert_display_lists(
        graphics, [parse_int(value) for value in meta["root_offsets"]]
    )
    model_compressed = rom[
        parse_int(source_assets["model_resources"]["rom_start"]) : parse_int(
            source_assets["model_resources"]["rom_end"]
        )
    ]
    model_resources, _ = decompress_huffman_asset(model_compressed)
    surface_compressed = rom[
        parse_int(source_assets["surface_data"]["rom_start"]) : parse_int(
            source_assets["surface_data"]["rom_end"]
        )
    ]
    surface_data, _ = decompress_huffman_asset(surface_compressed)
    track_mesh, track_stats = build_track_mesh(surface_data)
    gold, gold_count, gold_note = build_gold_coins(rom, course_id)
    item_boxes, item_count = build_item_boxes(rom, course_id)

    exported = {
        "display_lists": _write_asset(
            assets_dir, "display_lists", display_lists, sno=False
        ),
        "model_resources": _write_asset(assets_dir, "model_resources", model_resources),
        "track_mesh": _write_asset(assets_dir, "track_mesh", track_mesh),
        "gold_coins": _write_asset(assets_dir, "gold_coins", gold),
        "item_boxes": _write_asset(assets_dir, "item_boxes", item_boxes),
        "scene_animation": None,
    }
    source_payloads = {
        "graphics_f3dex": _write_source_payload(
            source_dir, "graphics.f3dex.bin", graphics
        ),
        "surface_data": _write_source_payload(
            source_dir, "surface_data.sk1.bin", surface_data
        ),
    }
    sprite = source_assets.get("sprites")
    texture_count = 0
    if sprite:
        compressed = rom[parse_int(sprite["rom_start"]) : parse_int(sprite["rom_end"])]
        sprite_data, _ = decompress_huffman_asset(compressed)
        texture_table, texture_count = build_texture_table(sprite_data)
        exported["texture_table"] = _write_asset(
            assets_dir, "texture_table", texture_table
        )
        source_payloads["sprite_table"] = _write_source_payload(
            source_dir, "sprite_table.sk1.bin", sprite_data
        )
    else:
        exported["texture_table"] = None

    manifest = {
        "format": "snowboard_kids_2_course_asset_package",
        "abi_version": 1,
        "source": {
            "game": "Snowboard Kids",
            "rom_sha1": EXPECTED_ROM_SHA1,
            "course_id": course_id,
            "course_key": course["key"],
            "course_name": course["name"],
            "rom_assets": {
                name: {"start": asset["rom_start"], "end": asset["rom_end"]}
                for name, asset in source_assets.items()
                if asset is not None
            },
        },
        "segment_mapping": {"display_lists": 1, "model_resources": 2},
        "display_list_root_offsets": [
            parse_int(value) for value in meta["root_offsets"]
        ],
        "assets": exported,
        "sk1_source_payloads": source_payloads,
        "counts": {
            **track_stats,
            "texture_entries": texture_count,
            "gold_coins": gold_count,
            "item_boxes": item_count,
        },
    }
    (directory / "manifest.yaml").write_text(
        yaml.safe_dump(manifest, sort_keys=False), encoding="utf-8"
    )
    runtime_hints = build_runtime_hints(
        rom, course, [parse_int(value) for value in meta["root_offsets"]]
    )
    (directory / "runtime_hints.yaml").write_text(
        yaml.safe_dump(runtime_hints, sort_keys=False), encoding="utf-8"
    )
    report = {
        "status": "converted_with_documented_approximations",
        "f3dex_opcode_counts": {
            f"0x{opcode:02X}": count for opcode, count in sorted(opcode_counts.items())
        },
        "approximations": [
            "SK1 face surface_type is retained in F3DEX-style flag bits; SK2 surface_index is 0",
            "SK1 face range is used for both SK2 face and height-face ranges",
            "SK2 sector unknown bytes are zero except path reference indices at 0x16 and 0x1C",
            "SK1 fixed-point object positions are divided by 2; mesh and model s16 coordinates are unchanged",
        ],
    }
    if gold_note:
        report["notes"] = [gold_note]
    (directory / "conversion_report.yaml").write_text(
        yaml.safe_dump(report, sort_keys=False), encoding="utf-8"
    )
    validate_exported_course(directory)
    return directory
