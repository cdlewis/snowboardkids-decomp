from pathlib import Path
from typing import Optional

from pygfxd import (
    GfxdEndian,
    gfxd_buffer_to_string,
    gfxd_cimg_callback,
    gfxd_dl_callback,
    gfxd_endian,
    gfxd_execute,
    gfxd_f3dex,
    gfxd_input_buffer,
    gfxd_light_callback,
    gfxd_lookat_callback,
    gfxd_macro_dflt,
    gfxd_macro_fn,
    gfxd_mtx_callback,
    gfxd_output_buffer,
    gfxd_printf,
    gfxd_puts,
    gfxd_target,
    gfxd_timg_callback,
    gfxd_tlut_callback,
    gfxd_vp_callback,
    gfxd_vtx_callback,
    gfxd_zimg_callback,
)
from splat.segtypes.common.segment import CommonSegment
from splat.util import log, options


def course_display_list_symbol(address: int, exclusive_ram_id: object) -> Optional[str]:
    """Name bundle-internal segment-2 calls without making them linker assets."""
    if address >> 24 != 2 or not isinstance(exclusive_ram_id, str):
        return None
    if not exclusive_ram_id.startswith("level_"):
        return None
    course = exclusive_ram_id.removeprefix("level_").upper()
    return f"{course}_COURSE_GRAPHICS_display_list_{address & 0xFFFFFF:06X}"


class N64SegCourse_display_list(CommonSegment):
    @staticmethod
    def is_data() -> bool:
        return True

    @property
    def statistics_type(self):
        return "course_display_list"

    def get_linker_section(self) -> str:
        return ".data"

    def get_section_flags(self) -> Optional[str]:
        return "wa"

    def out_path(self) -> Path:
        return options.opts.asset_path / "course_display_lists" / f"{self.name}.c"

    def should_split(self) -> bool:
        return self.extract and (
            options.opts.is_mode_active(self.type)
            or options.opts.is_mode_active("bin")
            or options.opts.is_mode_active("all")
        )

    def _macro_fn(self):
        gfxd_puts("    ")
        gfxd_macro_dflt()
        gfxd_puts(",\n")
        return 0

    def _print_segmented_address(self, address: int) -> int:
        gfxd_printf(f"0x{address:08X}")
        return 1

    def _print_display_list_address(self, address: int) -> int:
        exclusive_ram_id = self.yaml.get("exclusive_ram_id") if isinstance(self.yaml, dict) else None
        symbol = course_display_list_symbol(address, exclusive_ram_id)
        gfxd_printf(symbol if symbol is not None else f"0x{address:08X}")
        return 1

    def _disassemble_chunk(self, data: bytes) -> str:
        gfxd_input_buffer(data)
        output_bytes = bytes(len(data) * 160)
        output = gfxd_output_buffer(output_bytes, len(output_bytes))

        gfxd_target(gfxd_f3dex)
        gfxd_endian(GfxdEndian.big, 4)
        gfxd_macro_fn(self._macro_fn)
        gfxd_tlut_callback(lambda address, index, count: self._print_segmented_address(address))
        gfxd_timg_callback(lambda address, fmt, size, width, height, palette: self._print_segmented_address(address))
        gfxd_cimg_callback(lambda address, fmt, size, width: self._print_segmented_address(address))
        gfxd_zimg_callback(self._print_segmented_address)
        gfxd_dl_callback(self._print_display_list_address)
        gfxd_mtx_callback(self._print_segmented_address)
        gfxd_lookat_callback(lambda address, count: self._print_segmented_address(address))
        gfxd_light_callback(lambda address, count: self._print_segmented_address(address))
        gfxd_vtx_callback(lambda address, count: self._print_segmented_address(address))
        gfxd_vp_callback(self._print_segmented_address)
        gfxd_execute()
        disassembly = gfxd_buffer_to_string(output)
        if "(Gfx)" in disassembly:
            return "    [embedded resource bytes or commands that cannot be decoded losslessly]"
        return disassembly

    @staticmethod
    def _raw_words(data: bytes) -> str:
        lines = []
        for offset in range(0, len(data), 8):
            word0 = int.from_bytes(data[offset : offset + 4], "big")
            word1 = int.from_bytes(data[offset + 4 : offset + 8], "big")
            lines.append(f"    {{{{0x{word0:08X}, 0x{word1:08X}}}}}, /* Exact source word; see decoded reference above. */")
        return "\n".join(lines)

    def _disassemble(self, data: bytes) -> str:
        lines = []
        chunk_start = 0
        for offset in range(0, len(data), 8):
            if data[offset] != 0xB8:  # F3DEX G_ENDDL
                continue
            chunk = data[chunk_start : offset + 8]
            lines.extend(self._disassemble_chunk(chunk).rstrip().splitlines())
            chunk_start = offset + 8

        if chunk_start < len(data):
            lines.extend(self._disassemble_chunk(data[chunk_start:]).rstrip().splitlines())

        return "\n".join(line for line in lines if line)

    def split(self, rom_bytes: bytes):
        if self.rom_end is None:
            log.error(f"course display-list segment {self.name} needs to know where it ends")
        assert isinstance(self.rom_start, int)
        assert isinstance(self.rom_end, int)

        data = rom_bytes[self.rom_start : self.rom_end]
        if len(data) % 8:
            log.error(f"course display-list segment {self.name} size 0x{len(data):X} is not a multiple of 8")

        lines = [
            f"/* Auto-generated from ROM range 0x{self.rom_start:X}-0x{self.rom_end:X}. */",
            "/* Rebuildable F3DEX command stream from a course segment-2 graphics bundle. */",
            "/*",
            " * Decoded F3DEX reference:",
            *(f" * {line}" for line in self._disassemble(data).splitlines()),
            " */",
            "",
            '#include "common.h"',
            "",
            f"Gfx {self.name}[] = {{",
            self._raw_words(data),
            "};",
            "",
        ]

        path = self.out_path()
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text("\n".join(lines), encoding="utf-8", newline="\n")
        self.log(f"Wrote {self.name} to {path}")
