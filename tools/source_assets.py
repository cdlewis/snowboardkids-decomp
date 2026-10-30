"""Index source-resident assets and export readable text/effect inspection views.

These assets already rebuild from C. Their source declarations remain authoritative.
"""
import ast
import operator
import re
import subprocess
from tools.source_versions import read_source

from tools.course_surface_data_common import write_yaml, load_yaml
from tools.music_assets import parse_events, encode_music_part
from tools.textconv import parse_charmap, process_source, decode_text, encode_text


def source_files(root):
    """Repository sources, including unstaged additions but excluding ignored files."""
    result = subprocess.run(["git", "ls-files", "-z", "--cached", "--others", "--exclude-standard", "--", "src"], cwd=root,
                            check=True, capture_output=True)
    return sorted(root / p for p in result.stdout.decode().split("\0") if p.endswith(".c") and (root / p).is_file())


def literal_values(body):
    body = re.sub(r"/\*.*?\*/|//[^\n]*", "", body, flags=re.S)
    if re.sub(r"0x[\da-fA-F]+|\d+|[\s,{}]", "", body):
        raise ValueError("initializer contains non-literal expressions")
    return [int(v, 0) for v in re.findall(r"0x[\da-fA-F]+|\d+", body)]


def sound_effect_ids(root):
    source = (root / "include/game/audio/sound_ids.h").read_text()
    body = re.search(r"typedef enum SoundEffectId\s*\{(.*?)\}", source, re.S)[1]
    return {name: int(value, 0) for name, value in
            re.findall(r"\b(SFX_\w+)\s*=\s*(0x[0-9A-Fa-f]+|\d+)", body)}


def sequence_bytes(root, body):
    """Expand the sequence macros and read their integer byte expressions."""
    # The C compiler resolves enum operands; expose the same values to cpp.
    effect_defines = "".join(f"#define {name} {value}\n" for name, value in sound_effect_ids(root).items())
    expanded = subprocess.run(
        ["cpp", "-P", "-undef", "-nostdinc", "-I", str(root / "include"), "-"],
        input='#include "libmus/sequence_commands.h"\n#include "game/audio/sound_wave_ids.h"\n' + effect_defines + body,
        text=True, capture_output=True, check=True,
    ).stdout
    operators = {ast.BitOr: operator.or_, ast.BitAnd: operator.and_,
                 ast.LShift: operator.lshift, ast.RShift: operator.rshift}

    def integer(node):
        if isinstance(node, ast.Constant) and type(node.value) is int:
            return node.value
        if isinstance(node, ast.BinOp) and type(node.op) in operators:
            return operators[type(node.op)](integer(node.left), integer(node.right))
        raise ValueError("unsupported sequence byte expression")

    return bytes(integer(ast.parse(value.strip(), mode="eval").body)
                 for value in expanded.split(",") if value.strip())


def sound_effect_bank(root, source):
    """Read command arrays with IDO's four-byte alignment between symbols."""
    arrays = re.findall(
        r"u8 (gSoundEffect\w+Commands)\[\]\s*=\s*\{(.*?)\};", source, re.S,
    )
    if not arrays:
        raise ValueError("missing sound-effect command bank")
    data, offsets = bytearray(), {}
    for name, body in arrays:
        value = sequence_bytes(root, body)
        offsets[name] = len(data)
        data.extend(value)
        data.extend(bytes(-len(data) % 4))
    return bytes(data), offsets


def extract_source_assets(root, rom):
    chars, names = parse_charmap(root / "tools/charmap.txt")
    entries = []
    types = "Gfx|Vtx|MenuGlyphScript|CharacterModelPart|MenuPalette|RaceCourseSceneryEntry|RaceCourseSurface"
    for path in source_files(root):
        source = read_source(path)
        converted = process_source(source, chars, names)
        for match in re.finditer(r"\b(" + types + r")\s+(\w+)\s*(\[[^;=]*?\])\s*=\s*\{(.*?)\}\s*;", converted, re.S):
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

    # The engine passes individual command arrays through gSoundEffectCommandPointers to MusStartEffect.
    # Verify the existing C initializer against the original ROM before decoding it.
    path = root / "src/audio/sound_manager.c"
    source = read_source(path)
    if re.search(r"u8 gSoundEffect\w+Commands\[", source):
        data, command_offsets = sound_effect_bank(root, source)
        main = next(s for s in load_yaml(root / "snowboardkids.yaml")["segments"] if isinstance(s,dict) and s.get("name") == "main")
        rom_offset = main["start"] + (0x800DB098 - main["vram"])
        if rom[rom_offset:rom_offset+len(data)] != data:
            raise ValueError("source sound-effect bank differs from original ROM")
        table = re.search(r"u8 \*gSoundEffectCommandPointers\[[^]]*\]\s*=\s*\{(.*?)\};", source, re.S)
        if not table:
            raise ValueError("missing sound-effect pointer table")
        offsets = [command_offsets[name] for name in re.findall(r"\bgSoundEffect\w+Commands\b", table[1])]
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
        entries.append(dict(name="sound_effect_commands", type="sound_effects", source="src/audio/sound_manager.c",
                            authority="source", status="source_backed", preview=relative))
    write_yaml(root / "assets/source_views/index.yaml", dict(schema_version=1, assets=entries))
    return entries
