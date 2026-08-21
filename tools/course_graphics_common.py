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
