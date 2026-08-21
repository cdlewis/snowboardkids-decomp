from __future__ import annotations

from pathlib import Path
from typing import Optional

from splat.segtypes.common.segment import CommonSegment
from splat.util import log, options

from tools.course_sprite_table_common import parse_course_sprite_table
from tools.course_surface_data_common import write_yaml
from tools.huffman_asset import decompress_huffman_asset


class N64SegCourse_sprite_table(CommonSegment):
    @staticmethod
    def is_data() -> bool:
        return True

    @property
    def statistics_type(self):
        return "course_sprite_table"

    def get_linker_section(self) -> str:
        return ".data"

    def get_section_flags(self) -> Optional[str]:
        return "wa"

    def out_path(self) -> Path:
        return options.opts.asset_path / "course_sprite_tables" / f"{self.name}.yaml"

    def should_split(self) -> bool:
        return self.extract and (
            options.opts.is_mode_active(self.type)
            or options.opts.is_mode_active("bin")
            or options.opts.is_mode_active("all")
        )

    def split(self, rom_bytes: bytes):
        if self.rom_end is None:
            log.error(f"course sprite-table segment {self.name} needs to know where it ends")
        assert isinstance(self.rom_start, int)
        assert isinstance(self.rom_end, int)

        decompressed, compression = decompress_huffman_asset(rom_bytes[self.rom_start : self.rom_end])
        parsed = parse_course_sprite_table(decompressed)
        manifest = {
            "name": self.name,
            "format": "course_sprite_table",
            "decompressed_size": len(decompressed),
            "compression": {
                "flags": compression.flags,
                "huffman_table": compression.table.hex(),
                "padding_bits": compression.padding_bits,
                "unused_tail": compression.unused_tail.hex(),
                "lz_tokens": compression.lz_tokens.hex() if compression.lz_tokens is not None else None,
            },
            **parsed,
        }
        write_yaml(self.out_path(), manifest)
        self.log(f"Wrote {self.name} to {self.out_path()}")
