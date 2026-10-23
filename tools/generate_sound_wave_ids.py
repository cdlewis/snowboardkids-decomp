#!/usr/bin/env python3
"""Generate wave IDs from the original pointer bank, effect scripts, and music.

Effect names come from sound_ids.h; the ROM supplies their actual sample indices.
The bank has no sample names, so all slots also receive an offset-based identity.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from tools.audio_assets import parse_pointer_bank
from tools.course_surface_data_common import load_yaml
from tools.huffman_asset import decompress_huffman_asset
from tools.music_assets import parse_events, parse_music
from tools.source_assets import sound_effect_ids

ROOT = Path(__file__).resolve().parents[1]
HEADER = Path("include/game/audio/sound_wave_ids.h")


def wave_index(event):
    operands = event["operands"]
    return operands[0] if len(operands) == 1 else ((operands[0] & 127) << 8) | operands[1]


def collect_waves(root, rom):
    config = load_yaml(root / "snowboardkids.yaml")
    if hashlib.sha1(rom).hexdigest() != config["sha1"]:
        raise ValueError("wave ID generation requires the original US ROM")
    segments = [s for s in config["segments"] if isinstance(s, list) or "start" in s]
    ranges = []
    for entry, following in zip(segments, segments[1:]):
        if isinstance(entry, dict):
            end = following["start"] if isinstance(following, dict) else following[0]
            ranges.append((entry, end))
    bank_segment, bank_end = next((s, end) for s, end in ranges if s.get("name") == "SOUND_POINTER_BANK")
    bank = parse_pointer_bank(decompress_huffman_asset(rom[bank_segment["start"]:bank_end])[0])
    header = next(p for p in bank["parts"] if p["type"] == "bank_header")
    tables = {p["offset"]: p for p in bank["parts"] if p["type"] == "wavetable"}
    sample_base = next(s["start"] for s, _ in ranges if s.get("type") == "sample_bank"
                       and s.get("bank_asset") == "SOUND_POINTER_BANK")
    waves = []
    for index, offset in enumerate(header["wave_offsets"]):
        table = tables[offset]
        waves.append(dict(index=index, constant=f"SOUND_WAVE_SAMPLE_{table['sample_offset']:06X}",
                          sample_offset=table["sample_offset"], sample_rom=sample_base + table["sample_offset"],
                          sample_size=table["sample_size"], effects=[], music=[]))
    if len({w["constant"] for w in waves}) != len(waves):
        raise ValueError("multiple wave slots share a sample offset; their identities need disambiguation")

    def record(index, kind, name):
        if not 0 <= index < len(waves):
            raise ValueError(f"{name}: wave {index} exceeds the pointer bank")
        if name not in waves[index][kind]:
            waves[index][kind].append(name)

    effect_ids = {value: name for name, value in sound_effect_ids(root).items()}
    if set(effect_ids) != set(range(len(effect_ids))):
        raise ValueError("sound effect IDs must cover every pointer-table slot")
    symbols = (root / "symbol_addrs.txt").read_text()
    table_symbol = re.search(r"^gSoundEffectCommandPointers = .*?//[^\n]*rom:(0x[0-9A-Fa-f]+)[^\n]*size:(0x[0-9A-Fa-f]+)",
                             symbols, re.M)
    if not table_symbol or int(table_symbol[2], 0) != 4 * len(effect_ids):
        raise ValueError("sound effect pointer-table metadata disagrees with sound_ids.h")
    table_rom = int(table_symbol[1], 0)
    main = next(s for s, _ in ranges if s.get("name") == "main")
    pointers = struct.unpack_from(f">{len(effect_ids)}I", rom, table_rom)
    source = (root / "src.us/audio/sound_manager.c").read_text()
    source_table = re.search(r"u8 \*gSoundEffectCommandPointers\[[^]]*\]\s*=\s*\{(.*?)\};", source, re.S)
    commands = re.findall(r"\bgSoundEffect\w+Commands\b", source_table[1]) if source_table else []
    if len(commands) != len(effect_ids):
        raise ValueError("effect command table disagrees with sound_ids.h")
    for index, command in enumerate(commands):
        role = command.removeprefix("gSoundEffect").removesuffix("Commands").lower()
        if role != effect_ids[index].removeprefix("SFX_").replace("_", "").lower():
            raise ValueError(f"{effect_ids[index]} does not select its named command stream")
        address = re.search(r"^" + command + r" = (0x[0-9A-Fa-f]+);", symbols, re.M)
        if not address or int(address[1], 0) != pointers[index]:
            raise ValueError(f"{command} does not match the original fxHeader pointer")
    offsets = [main["start"] + pointer - main["vram"] for pointer in pointers]
    starts = sorted(set(offsets))
    if not main["start"] <= starts[0] < starts[-1] < table_rom:
        raise ValueError("effect command pointers lie outside the expected ROM range")
    ends = dict(zip(starts, starts[1:] + [table_rom]))
    aliases = {}
    for effect_id, start in enumerate(offsets):
        events, _ = parse_events(rom, start, ends[start])
        indices = {wave_index(event) for event in events if event.get("command") == "wave"}
        for index in sorted(indices):
            record(index, "effects", effect_ids[effect_id])
        if len(indices) == 1:
            aliases["SOUND_WAVE_" + effect_ids[effect_id].removeprefix("SFX_")] = indices.pop()
        elif indices:
            raise ValueError(f"{effect_ids[effect_id]}: multiple waves need distinct role names")
    for segment, end in ranges:
        if segment.get("type") != "music_sequence":
            continue
        music = parse_music(rom[segment["start"]:end])
        for part in music["parts"]:
            if part["type"] == "sequence":
                for event in part["events"]:
                    if event.get("command") == "drums":
                        raise ValueError("drum-map wave references need separate decoding")
                    if event.get("command") == "wave":
                        record(wave_index(event), "music", segment["name"])
    return waves, aliases


def render_header(waves, aliases):
    lines = ["#ifndef SOUND_WAVE_IDS_H", "#define SOUND_WAVE_IDS_H", "",
             "/* Generated by tools/generate_sound_wave_ids.py from the US ROM.",
             " * Wave-bank indices are separate from gameplay sound-effect IDs.",
             " * Sample identities use offsets relative to the sample-stream base.",
             " * Effect aliases describe script usage; aliases can share one sample.",
             " */"]
    lines.extend(f"#define {wave['constant']} {wave['index']}" for wave in waves)
    lines.extend(["", "/* Roles established by sound_ids.h and the corresponding effect bytecode. */"])
    lines.extend(f"#define {name} {waves[index]['constant']}" for name, index in aliases.items())
    lines.extend(["", f"#define SOUND_WAVE_COUNT {len(waves)}", "", "#endif", ""])
    return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, default=ROOT / "snowboardkids.z64")
    parser.add_argument("--check", action="store_true", help="verify the generated header without rewriting it")
    parser.add_argument("--report", type=Path, default=ROOT / "build/sound-wave-usage.json")
    args = parser.parse_args()
    waves, aliases = collect_waves(ROOT, args.rom.read_bytes())
    generated = render_header(waves, aliases)
    if args.check:
        if not (ROOT / HEADER).exists() or (ROOT / HEADER).read_text() != generated:
            raise ValueError(f"{HEADER} is stale; run tools/generate_sound_wave_ids.py")
    else:
        (ROOT / HEADER).write_text(generated)
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(dict(waves=waves, effect_aliases=aliases), indent=2) + "\n")
    print(f"{len(waves)} wave IDs and {len(aliases)} effect aliases verified against ROM data.")


if __name__ == "__main__":
    main()
