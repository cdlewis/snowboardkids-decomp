from __future__ import annotations

import struct
from pathlib import Path
from typing import Optional

from splat.segtypes.common.segment import CommonSegment
from splat.util import log, options

from tools.course_graphics_common import parse_int, trace_course_graphics
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
        root_offset = self._required_int("root_offset")
        graph = trace_course_graphics(rom_bytes[graphics_start:graphics_end], [root_offset])

        decompressed, compression = decompress_huffman_asset(rom_bytes[self.rom_start : self.rom_end])
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

        parts = []
        offset = 0
        while offset < len(decompressed):
            is_vertices = vertex_mask[offset] != 0
            end = offset + 1
            while end < len(decompressed) and (vertex_mask[end] != 0) == is_vertices:
                end += 1

            if is_vertices:
                if offset % 0x10 or end % 0x10:
                    log.error(f"course model resources {self.name} has an unaligned merged vertex range")
                vertices = [
                    self._vertex_dict(decompressed[vertex_offset : vertex_offset + 0x10])
                    for vertex_offset in range(offset, end, 0x10)
                ]
                parts.append({"type": "vertices", "offset": offset, "vertices": vertices})
            else:
                parts.append({"type": "raw", "offset": offset, "data": decompressed[offset:end].hex()})
            offset = end

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
