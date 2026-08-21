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
            coord0, coord1, coord2, face_index, unknown_07 = struct.unpack(">HHHbB", decompressed[offset : offset + 8])
            faces.append(
                {
                    "coord0": coord0,
                    "coord1": coord1,
                    "coord2": coord2,
                    "face_index": face_index,
                    "unknown_07": unknown_07,
                }
            )
            offset += 8

        remaining = len(decompressed) - offset
        if remaining < 2 or (remaining - 2) % 0x1C:
            log.error(
                f"course surface data {self.name} has 0x{remaining:X} bytes after its face table; "
                "expected 0x1C-byte surfaces followed by a u16"
            )

        surfaces = []
        surface_end = len(decompressed) - 2
        while offset < surface_end:
            chunk = decompressed[offset : offset + 0x1C]
            neighbors = struct.unpack(">hhhh", chunk[0:8])
            coords = struct.unpack(">HHHH", chunk[8:0x10])
            position_index, angle = struct.unpack(">hh", chunk[0x10:0x14])
            unknown = struct.unpack(">HHHH", chunk[0x14:0x1C])
            surface = {f"neighbor{i}": value for i, value in enumerate(neighbors)}
            surface.update({f"coord{i}": value for i, value in enumerate(coords)})
            surface.update({"position_index": position_index, "angle": angle})
            surface.update({f"unknown_{offset_:02x}": value for offset_, value in zip(range(0x14, 0x1C, 2), unknown)})
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
            "final_value": int.from_bytes(decompressed[-2:], "big"),
        }
        write_yaml(self.out_path(), manifest)
        self.log(f"Wrote {self.name} to {self.out_path()}")
