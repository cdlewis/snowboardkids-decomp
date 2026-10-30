#!/usr/bin/env python3
"""Derive movable graphics and RAM placement from the extracted linker script."""

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))


def graphics_layout(source):
    from tools.course_relocations import graphics_groups
    for bank, group in graphics_groups(Path(__file__).resolve().parents[1]):
        if not any('.'+s['name']+' ' in source for s in group):
            continue
        for spec in group:
            name = spec['name']
            marker = f'{name}_VRAM = ADDR(.{name});'
            if source.count(marker) != 1:
                raise ValueError(f'{name}: expected one graphics root binding')
            adjustment = 0x02000000+spec['start']-bank['graphics_start']-spec['vram']
            source = source.replace(marker, f'{name}_VRAM = {name}_command_000000 - 0x{adjustment:X};')
        for previous, following in zip(group, group[1:]):
            name = following['name']
            source, count = re.subn(rf'(\.{re.escape(name)}) 0x[\da-fA-F]+ :',
                                    rf"\1 ALIGN({previous['name']}_VRAM_END, 8) :", source)
            if count != 1:
                raise ValueError(f'{name}: expected one graphics section placement')
    return source


def runtime_index_inputs(source, build_dir, *, palettes=True, snowboards=False):
    inputs = [('src/us', 'menu/renderer', name) for name in ('menu_renderer', 'menu_render_utils')] if palettes else []
    if snowboards:
        inputs.append(('src', 'race/player', 'race_player_model_renderer'))
    for source_root, directory, name in inputs:
        original = f'{build_dir}/{source_root}/{directory}/{name}.o'
        if original not in source:
            raise ValueError(f'{name}: missing original renderer linker input')
        source = source.replace(original, f'{build_dir}/runtime-indices/{name}.o')
    return source


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--runtime-indices", action="store_true")
    parser.add_argument("--runtime-snowboards", action="store_true")
    parser.add_argument("--build-dir", default="build/shiftable")
    args = parser.parse_args()

    build_dir = args.build_dir.rstrip("/")
    source = args.input.read_text().replace("build/", build_dir + "/")
    source = graphics_layout(source)
    if args.runtime_indices or args.runtime_snowboards:
        source = runtime_index_inputs(source, build_dir,
                                      palettes=args.runtime_indices, snowboards=args.runtime_snowboards)
    source, count = re.subn(r"\.buffers_bss 0x[0-9A-Fa-f]+ \(NOLOAD\)",
                            ".buffers_bss ALIGN(main_BSS_END, 64) (NOLOAD)", source)
    if count != 1:
        parser.error("expected one runtime buffer section")
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(source)


if __name__ == "__main__":
    main()
