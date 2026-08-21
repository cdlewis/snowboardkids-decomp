from __future__ import annotations

import struct
from pathlib import Path
from typing import Optional

from splat.segtypes.common.segment import CommonSegment
from splat.util import log, options

from tools.course_graphics_common import collect_course_texture_references, parse_int, trace_course_graphics
from tools.course_surface_data_common import write_yaml
from tools.huffman_asset import decompress_huffman_asset


class N64SegCourse_model_resources(CommonSegment):
    @staticmethod
    def is_data() -> bool:
        return True

    @property
    def statistics_type(self):
        return "course_model_resources"

    def get_linker_section(self) -> str:
        return ".data"

    def get_section_flags(self) -> Optional[str]:
        return "wa"

    def out_path(self) -> Path:
        return options.opts.asset_path / "course_model_resources" / f"{self.name}.yaml"

    def should_split(self) -> bool:
        return self.extract and (
            options.opts.is_mode_active(self.type)
            or options.opts.is_mode_active("bin")
            or options.opts.is_mode_active("all")
        )

    def _required_int(self, key: str) -> int:
        if not isinstance(self.yaml, dict) or key not in self.yaml:
            log.error(f"course model-resource segment {self.name} needs {key}")
        assert isinstance(self.yaml, dict)
        return parse_int(self.yaml[key])

    def _root_offsets(self) -> list[int]:
        if not isinstance(self.yaml, dict):
            log.error(f"course model-resource segment {self.name} needs root_offsets")
        assert isinstance(self.yaml, dict)
        values = self.yaml.get("root_offsets")
        if values is None:
            return [self._required_int("root_offset")]
        if not isinstance(values, list) or not values:
            log.error(f"course model-resource segment {self.name} has invalid root_offsets")
        return [parse_int(value) for value in values]

    @staticmethod
    def _vertex_dict(data: bytes) -> dict:
        x, y, z, flag, s, t, r, g, b, a = struct.unpack(">hhhHhhBBBB", data)
        return {"x": x, "y": y, "z": z, "flag": flag, "s": s, "t": t, "r": r, "g": g, "b": b, "a": a}

    def split(self, rom_bytes: bytes):
        if self.rom_end is None:
            log.error(f"course model-resource segment {self.name} needs to know where it ends")
        assert isinstance(self.rom_start, int)
        assert isinstance(self.rom_end, int)

        graphics_start = self._required_int("graphics_start")
        graphics_end = self._required_int("graphics_end")
        graph = trace_course_graphics(rom_bytes[graphics_start:graphics_end], self._root_offsets())

        decompressed, compression = decompress_huffman_asset(rom_bytes[self.rom_start : self.rom_end])
        textures, palettes = collect_course_texture_references(
            rom_bytes[graphics_start:graphics_end], graph, len(decompressed)
        )
        vertex_mask = bytearray(len(decompressed))
        segment3_references = [reference for reference in graph.vertex_references if reference.segment == 3]
        for reference in segment3_references:
            start = reference.offset
            end = start + reference.count * 0x10
            if start % 0x10 or end > len(decompressed):
                log.error(
                    f"course model resources {self.name} has invalid vertex range "
                    f"0x{start:X}-0x{end:X}"
                )
            vertex_mask[start:end] = b"\x01" * (end - start)

        classified_ranges = []
        offset = 0
        while offset < len(decompressed):
            if not vertex_mask[offset]:
                offset += 1
                continue
            end = offset + 1
            while end < len(decompressed) and vertex_mask[end]:
                end += 1
            if offset % 0x10 or end % 0x10:
                log.error(f"course model resources {self.name} has an unaligned merged vertex range")
            classified_ranges.append(
                (
                    offset,
                    end,
                    {
                        "type": "vertices",
                        "offset": offset,
                        "vertices": [
                            self._vertex_dict(decompressed[vertex_offset : vertex_offset + 0x10])
                            for vertex_offset in range(offset, end, 0x10)
                        ],
                    },
                )
            )
            offset = end

        for texture in textures:
            start = texture.offset
            end = start + texture.size
            classified_ranges.append(
                (
                    start,
                    end,
                    {
                        "type": "texture",
                        "offset": start,
                        "format": texture.format,
                        "width": texture.width,
                        "height": texture.height,
                        "palette_slots": list(texture.palette_slots),
                        "data": decompressed[start:end].hex(),
                    },
                )
            )

        for palette in palettes:
            start = palette.offset
            end = start + palette.colors * 2
            classified_ranges.append(
                (
                    start,
                    end,
                    {
                        "type": "palette",
                        "offset": start,
                        "format": "rgba16",
                        "colors": palette.colors,
                        "load_slots": list(palette.load_slots),
                        "values": [
                            int.from_bytes(decompressed[value_offset : value_offset + 2], "big")
                            for value_offset in range(start, end, 2)
                        ],
                    },
                )
            )

        parts = []
        cursor = 0
        for start, end, part in sorted(classified_ranges):
            if start < cursor:
                log.error(f"course model resources {self.name} has overlapping parts at 0x{start:X}")
            if start > cursor:
                parts.append({"type": "raw", "offset": cursor, "data": decompressed[cursor:start].hex()})
            parts.append(part)
            cursor = end
        if cursor < len(decompressed):
            parts.append({"type": "raw", "offset": cursor, "data": decompressed[cursor:].hex()})

        manifest = {
            "name": self.name,
            "format": "course_model_resources",
            "decompressed_size": len(decompressed),
            "graphics_graph": {
                "display_list_count": len(graph.display_lists),
                "vertex_load_count": len(segment3_references),
                "unique_vertex_range_count": len(
                    {(reference.offset, reference.count) for reference in segment3_references}
                ),
                "texture_count": len(textures),
                "palette_count": len(palettes),
            },
            "compression": {
                "flags": compression.flags,
                "huffman_table": compression.table.hex(),
                "padding_bits": compression.padding_bits,
                "unused_tail": compression.unused_tail.hex(),
                "lz_tokens": compression.lz_tokens.hex() if compression.lz_tokens is not None else None,
            },
            "parts": parts,
        }
        write_yaml(self.out_path(), manifest)
        self.log(f"Wrote {self.name} to {self.out_path()}")
