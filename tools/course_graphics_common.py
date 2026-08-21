#!/usr/bin/env python3
"""Graph tracing and editable packing helpers for SK1 course graphics."""

from __future__ import annotations

import struct
from dataclasses import dataclass

from tools.huffman_asset import CompressionMetadata


@dataclass(frozen=True)
class DisplayListNode:
    offset: int
    end: int


@dataclass(frozen=True)
class VertexReference:
    command_offset: int
    segment: int
    offset: int
    count: int


@dataclass
class CourseGraphicsGraph:
    display_lists: list[DisplayListNode]
    vertex_references: list[VertexReference]


@dataclass(frozen=True)
class TextureReference:
    offset: int
    size: int
    format: str
    width: int
    height: int
    palette_slots: tuple[int, ...]


@dataclass(frozen=True)
class PaletteReference:
    offset: int
    colors: int
    load_slots: tuple[int, ...]


def parse_int(value) -> int:
    return int(value, 0) if isinstance(value, str) else int(value)


def trace_course_graphics(bundle: bytes, root_offsets: list[int]) -> CourseGraphicsGraph:
    nodes: dict[int, DisplayListNode] = {}
    vertex_references: list[VertexReference] = []
    active: set[int] = set()

    def trace(offset: int) -> None:
        if offset in nodes or offset in active:
            return
        if offset < 0 or offset + 8 > len(bundle) or offset % 8:
            raise ValueError(f"invalid course display-list offset 0x{offset:X}")

        active.add(offset)
        position = offset
        while position + 8 <= len(bundle):
            word0, word1 = struct.unpack(">II", bundle[position : position + 8])
            opcode = word0 >> 24

            if opcode == 0x06 and word1 >> 24 == 2:  # F3DEX G_DL
                target = word1 & 0x00FFFFFF
                if target >= len(bundle):
                    raise ValueError(
                        f"display list at 0x{position:X} targets segment-2 offset 0x{target:X} "
                        f"outside the 0x{len(bundle):X}-byte bundle"
                    )
                trace(target)
            elif opcode == 0x04:  # F3DEX G_VTX
                count = (word0 >> 10) & 0x3F
                if count == 0:
                    raise ValueError(f"vertex command at 0x{position:X} has a zero count")
                vertex_references.append(
                    VertexReference(
                        command_offset=position,
                        segment=word1 >> 24,
                        offset=word1 & 0x00FFFFFF,
                        count=count,
                    )
                )

            position += 8
            if opcode == 0xB8:  # F3DEX G_ENDDL
                nodes[offset] = DisplayListNode(offset=offset, end=position)
                active.remove(offset)
                return

        raise ValueError(f"display list at 0x{offset:X} has no G_ENDDL before the bundle end")

    for root_offset in root_offsets:
        trace(root_offset)

    return CourseGraphicsGraph(
        display_lists=sorted(nodes.values(), key=lambda node: node.offset),
        vertex_references=vertex_references,
    )


def collect_course_texture_references(
    bundle: bytes, graph: CourseGraphicsGraph, resource_size: int
) -> tuple[list[TextureReference], list[PaletteReference]]:
    """Find segment-3 CI textures and TLUTs loaded by a traced F3DEX graph."""
    textures: dict[int, dict] = {}
    palettes: dict[int, dict] = {}

    for node in graph.display_lists:
        texture_image = None
        loaded_texture = None
        load_tile = None
        render_tile = None

        for position in range(node.offset, node.end, 8):
            word0, word1 = struct.unpack(">II", bundle[position : position + 8])
            opcode = word0 >> 24

            if opcode == 0xFD and word1 >> 24 == 3:  # G_SETTIMG
                texture_image = {
                    "offset": word1 & 0x00FFFFFF,
                    "format": (word0 >> 21) & 0x7,
                    "size": (word0 >> 19) & 0x3,
                }
                loaded_texture = None
                load_tile = None
                render_tile = None
            elif opcode == 0xF5:  # G_SETTILE
                tile = {
                    "format": (word0 >> 21) & 0x7,
                    "size": (word0 >> 19) & 0x3,
                    "line": (word0 >> 9) & 0x1FF,
                    "tmem": word0 & 0x1FF,
                    "tile": (word1 >> 24) & 0x7,
                    "palette": (word1 >> 20) & 0xF,
                }
                if tile["tile"] == 7:
                    load_tile = tile
                else:
                    render_tile = tile
            elif opcode == 0xF3 and texture_image is not None:  # G_LOADBLOCK
                texel_count = ((word1 >> 12) & 0xFFF) + 1
                byte_count = (texel_count * (1 << texture_image["size"]) + 1) // 2
                loaded_texture = {"offset": texture_image["offset"], "size": byte_count}
            elif opcode == 0xF0 and texture_image is not None:  # G_LOADTLUT
                color_count = ((word1 >> 14) & 0x3FF) + 1
                start = texture_image["offset"]
                end = start + color_count * 2
                if start < 0 or end > resource_size:
                    raise ValueError(f"palette range 0x{start:X}-0x{end:X} exceeds model resources")
                slot = None
                if load_tile is not None and load_tile["tmem"] >= 0x100:
                    slot = (load_tile["tmem"] - 0x100) // 0x10
                old = palettes.setdefault(start, {"colors": color_count, "slots": set()})
                if old["colors"] != color_count:
                    raise ValueError(f"palette at 0x{start:X} is loaded with inconsistent sizes")
                if slot is not None:
                    old["slots"].add(slot)
                texture_image = None
                loaded_texture = None
            elif opcode == 0xF2 and loaded_texture is not None and render_tile is not None:  # G_SETTILESIZE
                fmt = render_tile["format"]
                siz = render_tile["size"]
                if fmt != 2 or siz not in (0, 1):  # Only confirmed CI4/CI8 resources.
                    continue
                row_bytes = render_tile["line"] * 8
                if row_bytes == 0 or loaded_texture["size"] % row_bytes:
                    raise ValueError(
                        f"texture at 0x{loaded_texture['offset']:X} has an invalid tile stride"
                    )
                width = row_bytes * (2 if siz == 0 else 1)
                height = loaded_texture["size"] // row_bytes
                start = loaded_texture["offset"]
                end = start + loaded_texture["size"]
                if start < 0 or end > resource_size:
                    raise ValueError(f"texture range 0x{start:X}-0x{end:X} exceeds model resources")

                values = {
                    "size": loaded_texture["size"],
                    "format": "ci4" if siz == 0 else "ci8",
                    "width": width,
                    "height": height,
                }
                old = textures.setdefault(start, {**values, "slots": set()})
                if any(old[key] != value for key, value in values.items()):
                    raise ValueError(f"texture at 0x{start:X} is loaded with inconsistent dimensions")
                old["slots"].add(render_tile["palette"])

    texture_references = [
        TextureReference(
            offset=offset,
            size=values["size"],
            format=values["format"],
            width=values["width"],
            height=values["height"],
            palette_slots=tuple(sorted(values["slots"])),
        )
        for offset, values in sorted(textures.items())
    ]
    palette_references = [
        PaletteReference(offset=offset, colors=values["colors"], load_slots=tuple(sorted(values["slots"])))
        for offset, values in sorted(palettes.items())
    ]

    ranges = sorted(
        [(item.offset, item.offset + item.size) for item in texture_references]
        + [(item.offset, item.offset + item.colors * 2) for item in palette_references]
    )
    for (_, previous_end), (start, _) in zip(ranges, ranges[1:]):
        if start < previous_end:
            raise ValueError(f"model-resource ranges overlap at 0x{start:X}")

    return texture_references, palette_references


def compression_metadata_from_manifest(manifest: dict) -> CompressionMetadata:
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


def pack_course_model_resources(manifest: dict) -> bytes:
    size = parse_int(manifest["decompressed_size"])
    output = bytearray(size)
    written = bytearray(size)

    for part in sorted(manifest["parts"], key=lambda item: parse_int(item["offset"])):
        start = parse_int(part["offset"])
        if part["type"] == "raw":
            data = bytes.fromhex(str(part["data"]))
        elif part["type"] == "vertices":
            data = bytearray()
            for vertex in part["vertices"]:
                data.extend(
                    struct.pack(
                        ">hhhHhhBBBB",
                        parse_int(vertex["x"]),
                        parse_int(vertex["y"]),
                        parse_int(vertex["z"]),
                        parse_int(vertex["flag"]),
                        parse_int(vertex["s"]),
                        parse_int(vertex["t"]),
                        parse_int(vertex["r"]),
                        parse_int(vertex["g"]),
                        parse_int(vertex["b"]),
                        parse_int(vertex["a"]),
                    )
                )
            data = bytes(data)
        elif part["type"] == "texture":
            data = bytes.fromhex(str(part["data"]))
            expected_size = (parse_int(part["width"]) * parse_int(part["height"]))
            if part["format"] == "ci4":
                expected_size = (expected_size + 1) // 2
            elif part["format"] != "ci8":
                raise ValueError(f"unsupported course texture format {part['format']!r}")
            if len(data) != expected_size:
                raise ValueError(
                    f"texture at 0x{start:X} has 0x{len(data):X} bytes, expected 0x{expected_size:X}"
                )
        elif part["type"] == "palette":
            data = b"".join(parse_int(value).to_bytes(2, "big") for value in part["values"])
            if len(data) != parse_int(part["colors"]) * 2:
                raise ValueError(f"palette at 0x{start:X} has the wrong color count")
        else:
            raise ValueError(f"unsupported course model-resource part type {part['type']!r}")

        end = start + len(data)
        if start < 0 or end > size:
            raise ValueError(f"part at 0x{start:X}-0x{end:X} exceeds decompressed size 0x{size:X}")
        if any(written[start:end]):
            raise ValueError(f"part at 0x{start:X}-0x{end:X} overlaps an earlier part")
        output[start:end] = data
        written[start:end] = b"\x01" * len(data)

    if not all(written):
        raise ValueError(f"manifest does not cover decompressed offset 0x{written.index(0):X}")
    return bytes(output)
