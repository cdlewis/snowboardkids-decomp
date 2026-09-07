"""Display-list and vertex bundles with resources embedded in RSP segment 2."""
import re
import struct

from tools.asset_parts import complete_parts, pack_parts
from tools.course_graphics_common import trace_course_graphics, collect_course_texture_references
from tools.asset_images import externalize_model, image_bytes

VERTEX_FIELDS = ("x", "y", "z", "flag", "s", "t", "r", "g", "b", "a")
OPCODES = {0x04: "G_VTX", 0x06: "G_DL", 0xB1: "G_TRI2", 0xB6: "G_CLEARGEOMETRYMODE",
           0xB7: "G_SETGEOMETRYMODE", 0xB8: "G_ENDDL", 0xB9: "G_SETOTHERMODE_L",
           0xBA: "G_SETOTHERMODE_H", 0xBB: "G_TEXTURE", 0xBF: "G_TRI1", 0xE6: "G_RDPLOADSYNC",
           0xE7: "G_RDPPIPESYNC", 0xE8: "G_RDPTILESYNC", 0xF0: "G_LOADTLUT", 0xF2: "G_SETTILESIZE",
           0xF3: "G_LOADBLOCK", 0xF5: "G_SETTILE", 0xFA: "G_SETPRIMCOLOR", 0xFC: "G_SETCOMBINE", 0xFD: "G_SETTIMG"}


def parse_embedded_model(data, roots, path):
    graph = trace_course_graphics(data, roots)
    labels = bytearray(len(data))
    for node in graph.display_lists:
        labels[node.offset:node.end] = bytes([1]) * (node.end-node.offset)
    for v in graph.vertex_references:
        if v.segment != 2:
            raise ValueError("embedded model references external vertices")
        end = v.offset+v.count*16
        if end > len(data) or v.offset % 8 or any(labels[v.offset:end]) and any(x == 1 for x in labels[v.offset:end]):
            raise ValueError("embedded vertex range overlaps commands or exceeds bundle")
        labels[v.offset:end] = bytes([2]) * (end-v.offset)
    parts, pos = [], 0
    while pos < len(data):
        end = pos+1
        while end < len(data) and labels[end] == labels[pos]:
            end += 1
        if labels[pos] == 1:
            rel = f"{path.stem}/display_list_{pos:06X}.c"
            target = path.parent / rel
            target.parent.mkdir(parents=True, exist_ok=True)
            lines = ['#include "common.h"', f"Gfx display_list_{pos:06X}[] = {{"]
            for offset in range(pos, end, 8):
                a,b = struct.unpack_from(">II", data, offset)
                lines.append(f"    {{{{0x{a:08X}, 0x{b:08X}}}}}, /* {OPCODES.get(a >> 24, 'RSP/RDP command')} @ 0x{offset:06X} */")
            target.write_text("\n".join(lines + ["};", ""]))
            parts.append(dict(type="display_list", offset=pos, size=end-pos, path=rel))
        elif labels[pos] == 2:
            parts.append(dict(type="vertices", offset=pos, size=end-pos,
                              vertices=[dict(zip(VERTEX_FIELDS, struct.unpack_from(">hhhHhhBBBB", data, p))) for p in range(pos,end,16)]))
        pos = end
    textures, palettes = collect_course_texture_references(data, graph, len(data), 2)
    for t in textures:
        parts.append(dict(type="texture", offset=t.offset, size=t.size, format=t.format, width=t.width,
                          height=t.height, palette_slots=list(t.palette_slots), data=data[t.offset:t.offset+t.size].hex()))
    for p in palettes:
        parts.append(dict(type="palette", offset=p.offset, size=p.colors*2, colors=p.colors,
                          load_slots=list(p.load_slots), values=list(struct.unpack_from(f">{p.colors}H",data,p.offset))))
    m = dict(name=path.stem, root_offsets=roots, parts=complete_parts(data, parts))
    externalize_model(m,path)
    return m


def pack_embedded_model(m, base):
    def encode(p):
        if p["type"] == "vertices":
            return b"".join(struct.pack(">hhhHhhBBBB", *(v[k] for k in VERTEX_FIELDS)) for v in p["vertices"])
        if p["type"] == "texture":
            return image_bytes(p, base)
        if p["type"] == "palette":
            return struct.pack(f">{p['colors']}H", *p["values"])
        if p["type"] == "display_list":
            source = re.sub(r"/\*.*?\*/|//[^\n]*", "", (base / p["path"]).read_text(), flags=re.S)
            body = source[source.index("= {")+3:source.rindex("}")]
            words = re.findall(r"\{\{\s*0x([0-9A-Fa-f]{8}),\s*0x([0-9A-Fa-f]{8})\s*\}\}\s*,", body)
            remainder = re.sub(r"\{\{\s*0x[0-9A-Fa-f]{8},\s*0x[0-9A-Fa-f]{8}\s*\}\}\s*,", "", body)
            if remainder.strip():
                raise ValueError("display-list sources require exact pairs of 32-bit hex words")
            return bytes.fromhex("".join(a+b for a,b in words))
        raise ValueError(f"unsupported embedded model part {p['type']}")
    result = pack_parts(m["parts"], m["decompressed_size"], encode)
    trace_course_graphics(result, m["root_offsets"])
    return result
