"""Index source-resident assets and export readable text/effect inspection views.

These assets already rebuild from C. Their source declarations remain authoritative.
"""
import re
import struct
import subprocess

from tools.course_surface_data_common import write_yaml, load_yaml
from tools.music_assets import parse_events, encode_music_part
from tools.textconv import parse_charmap, process_source, decode_text, encode_text


def source_files(root):
    """Only repository sources, excluding generated matching workspaces/contexts."""
    result = subprocess.run(["git", "ls-files", "-z", "--", "src"], cwd=root,
                            check=True, capture_output=True)
    return [root / p for p in result.stdout.decode().split("\0") if p.endswith(".c")]


def literal_values(body):
    body = re.sub(r"/\*.*?\*/|//[^\n]*", "", body, flags=re.S)
    if re.sub(r"0x[\da-fA-F]+|\d+|[\s,{}]", "", body):
        raise ValueError("initializer contains non-literal expressions")
    return [int(v, 0) for v in re.findall(r"0x[\da-fA-F]+|\d+", body)]


def extract_source_assets(root, rom):
    chars, names = parse_charmap(root / "tools/charmap.txt")
    entries = []
    types = "Gfx|Vtx|MenuGlyphScript|CharacterModelPart|MenuPalette|RaceCourseSceneryEntry|RaceCourseSurface"
    for path in source_files(root):
        source = path.read_text()
        converted = process_source(source, chars, names)
        for match in re.finditer(r"\b(" + types + r")\s+(\w+)\s*(\[[^;=]*?\])\s*=\s*\{(.*?)\};", converted, re.S):
            kind, name, dimensions, body = match.groups()
            entry = dict(name=name, type=kind, source=str(path.relative_to(root)), authority="source", status="source_backed")
            if kind == "MenuGlyphScript":
                values = literal_values(body)
                text = decode_text(values, chars, names)
                if encode_text(text, chars, names, False) != values:
                    raise ValueError(f"{name}: text view does not round-trip")
                relative = f"assets/source_views/text/{name}.txt"
                output = root / relative
                output.parent.mkdir(parents=True, exist_ok=True)
                output.write_text(text)
                entry.update(preview=relative, declaration_dimensions=dimensions, encoded_word_count=len(values))
            entries.append(entry)

    # The engine passes D_800DB098 through D_800DB8FC to MusStartEffect.
    # Verify the existing C initializer against the original ROM before decoding it.
    path = root / "src/audio/sound_manager.c"
    source = path.read_text()
    match = re.search(r"u32 D_800DB098\[[^]]+\]\s*=\s*\{(.*?)\};", source, re.S)
    if match:
        words = literal_values(match[1])
        data = struct.pack(f">{len(words)}I", *words)
        main = next(s for s in load_yaml(root / "snowboardkids.yaml")["segments"] if isinstance(s,dict) and s.get("name") == "main")
        rom_offset = main["start"] + (0x800DB098 - main["vram"])
        if rom[rom_offset:rom_offset+len(data)] != data:
            raise ValueError("source sound-effect bank differs from original ROM")
        table = re.search(r"u8 \*D_800DB8FC\[[^]]*\]\s*=\s*\{(.*?)\};", source, re.S)
        if not table:
            raise ValueError("missing sound-effect pointer table")
        offsets = [int(s,0)*4 for s in re.findall(r"&D_800DB098\[(0x[0-9A-Fa-f]+|\d+)\]", table[1])]
        starts = sorted(set(offsets))
        effects = []
        for start, end in zip(starts, starts[1:]+[len(data)]):
            events, stop = parse_events(data,start,end)
            part = dict(type="sequence", offset=start, events=events)
            if encode_music_part(part) != data[start:stop]:
                raise ValueError("sound-effect listing does not round-trip")
            effects.append(dict(offset=start, events=events, unused_tail=data[stop:end].hex()))
        relative = "assets/source_views/sound_effects.yaml"
        write_yaml(root / relative, dict(schema_version=1, authority="src/audio/sound_manager.c",
                                        rom_start=rom_offset, effect_offsets=offsets, effects=effects))
        entries.append(dict(name="D_800DB098", type="sound_effects", source="src/audio/sound_manager.c",
                            authority="source", status="source_backed", preview=relative))
    write_yaml(root / "assets/source_views/index.yaml", dict(schema_version=1, assets=entries))
    return entries
