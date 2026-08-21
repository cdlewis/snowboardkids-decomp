from __future__ import annotations

import struct
from pathlib import Path
from typing import Optional

from splat.segtypes.common.segment import CommonSegment
from splat.util import log, options

from tools.course_surface_data_common import write_yaml
from tools.huffman_asset import decompress_huffman_asset


class N64SegCourse_surface_data(CommonSegment):
    @staticmethod
    def is_data() -> bool:
        return True

    @property
    def statistics_type(self):
        return "course_surface_data"

    def get_linker_section(self) -> str:
        return ".data"

    def get_section_flags(self) -> Optional[str]:
        return "wa"

    def out_path(self) -> Path:
        return options.opts.asset_path / "course_surface_data" / f"{self.name}.yaml"

    def should_split(self) -> bool:
        return self.extract and (
            options.opts.is_mode_active(self.type)
            or options.opts.is_mode_active("bin")
            or options.opts.is_mode_active("all")
        )

    def split(self, rom_bytes: bytes):
        if self.rom_end is None:
            log.error(f"course surface data segment {self.name} needs to know where it ends")
        assert isinstance(self.rom_start, int)
        assert isinstance(self.rom_end, int)

        decompressed, compression = decompress_huffman_asset(rom_bytes[self.rom_start : self.rom_end])
        offset = 0

        coordinate_count = int.from_bytes(decompressed[offset : offset + 2], "big")
        offset += 2
        coordinates = []
        for _ in range(coordinate_count):
            if offset + 6 > len(decompressed):
                log.error(f"course surface data {self.name} has a truncated coordinate table")
            x, y, z = struct.unpack(">hhh", decompressed[offset : offset + 6])
            coordinates.append({"x": x, "y": y, "z": z})
            offset += 6

        face_count = int.from_bytes(decompressed[offset : offset + 2], "big")
        offset += 2
        faces = []
        for _ in range(face_count):
            if offset + 8 > len(decompressed):
                log.error(f"course surface data {self.name} has a truncated face table")
            coord0, coord1, coord2, surface_type, skip_first_edge_check = struct.unpack(
                ">HHHbB", decompressed[offset : offset + 8]
            )
            faces.append(
                {
                    "coordinate_indices": [coord0, coord1, coord2],
                    "surface_type": surface_type,
                    "skip_first_edge_check": skip_first_edge_check,
                }
            )
            offset += 8

        if offset + 2 > len(decompressed):
            log.error(f"course surface data {self.name} has no surface count")
        surface_count = int.from_bytes(decompressed[offset : offset + 2], "big")
        offset += 2

        remaining = len(decompressed) - offset
        if remaining != surface_count * 0x1C:
            log.error(
                f"course surface data {self.name} has 0x{remaining:X} bytes after its face table; "
                f"expected {surface_count} 0x1C-byte surfaces"
            )

        surfaces = []
        for _ in range(surface_count):
            chunk = decompressed[offset : offset + 0x1C]
            neighbors = struct.unpack(">hhhh", chunk[0:8])
            boundary_coords = struct.unpack(">hhhh", chunk[8:0x10])
            reference_coord_index, path_angle, face_start_index, face_end_index, unknown_18, edge_clamp_flags = (
                struct.unpack(">hhHHHH", chunk[0x10:0x1C])
            )
            surface = {
                "neighbor_indices": list(neighbors),
                "boundary_coordinate_indices": list(boundary_coords),
                "reference_coordinate_index": reference_coord_index,
                "path_angle": path_angle,
                "face_start_index": face_start_index,
                "face_end_index": face_end_index,
                "unknown_18": unknown_18,
                "edge_clamp_flags": edge_clamp_flags,
            }
            surfaces.append(surface)
            offset += 0x1C

        manifest = {
            "name": self.name,
            "format": "course_surface_data",
            "compression": {
                "flags": compression.flags,
                "huffman_table": compression.table.hex(),
                "padding_bits": compression.padding_bits,
                "unused_tail": compression.unused_tail.hex(),
            },
            "coordinates": coordinates,
            "faces": faces,
            "surfaces": surfaces,
        }
        write_yaml(self.out_path(), manifest)
        self.log(f"Wrote {self.name} to {self.out_path()}")
