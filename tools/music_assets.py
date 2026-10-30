"""Reversible Mus sequence listings; opcodes follow src/libmus/player.c's jumptable."""
import struct

from tools.asset_parts import complete_parts, pack_parts

COMMANDS = "stop wave port portoff defa tempo cutoff endit vibup vibdown viboff length ignore trans ignore_trans distort envelope envoff envon troff tron for next wobble wobbleoff velon veloff velocity pan stereo drums drumsoff print goto reverb randNote randVolume randPan volume startfx bendrange".split()
LENGTHS = [0, -1, 1, 0, 7, 1, 2, 1, 3, 3, 0, -1, 0, 1, 0, 1, -1, 0, 0, 0, 0, 1, 0, 3, 0, 0, 0, 1, 1, 2, 1, 0, 0, 6, 1, 2, 2, 2, 1, -1, 1]


def varlen(data, pos, end):
    if pos >= end:
        raise ValueError("truncated variable-length value")
    size = 2 if data[pos] & 128 else 1
    if pos+size > end:
        raise ValueError("truncated variable-length value")
    return size


def parse_events(data, start, end):
    events, pos, velocity, length, ignore = [], start, False, 0, False
    loops = []
    while pos < end:
        begin, code = pos, data[pos]
        pos += 1
        if code >= 128:
            index = code & 127
            if index >= len(COMMANDS):
                raise ValueError(f"unsupported sequence opcode 0x{code:X} at 0x{begin:X}")
            name, count = COMMANDS[index], LENGTHS[index]
            if count == -1:
                count = varlen(data, pos, end)
            if pos+count > end:
                raise ValueError("truncated sequence command")
            args = list(data[pos:pos+count])
            pos += count
            events.append(dict(offset=begin, command=name, operands=args))
            if name == "velon":
                velocity = True
            elif name in ("veloff", "velocity"):
                velocity = False
            elif name == "length":
                length = args[0] if count == 1 else ((args[0] & 127) << 8) | args[1]
            elif name == "ignore":
                ignore = True
            elif name == "for":
                loops.append(args[0])
            elif name == "next":
                if not loops:
                    raise ValueError("sequence next without for")
                if loops.pop() == 255:
                    break
            elif name in ("stop", "goto"):
                break
        else:
            event = dict(offset=begin, note=code)
            if velocity:
                if pos >= end:
                    raise ValueError("truncated note velocity")
                event["velocity_index"] = data[pos]
                pos += 1
            if ignore or not length:
                count = varlen(data, pos, end)
                event["duration_bytes"] = list(data[pos:pos+count])
                pos += count
            ignore = False
            events.append(event)
    return events, pos


def parse_music(data):
    if len(data) < 24:
        raise ValueError("truncated sequence header")
    count, seq, vol, pitch, envelopes, drums = struct.unpack_from(">6I", data)
    if not 0 < count <= 32:
        raise ValueError("invalid sequence track count")
    parts = [dict(type="sequence_header", offset=0, size=24, track_count=count,
                  sequence_table=seq, volume_table=vol, pitch_table=pitch, envelope_table=envelopes, drum_table=drums)]
    roles = {}
    for name, offset in (("sequence",seq), ("volume",vol), ("pitch",pitch)):
        if not 24 <= offset <= len(data)-count*4:
            raise ValueError("sequence pointer table exceeds asset")
        pointers = list(struct.unpack_from(f">{count}I", data, offset))
        parts.append(dict(type="pointers", role=name, offset=offset, size=count*4, values=pointers))
        for p in pointers:
            if p:
                if not 24 <= p < len(data):
                    raise ValueError("sequence stream pointer exceeds asset")
                roles.setdefault(p, set()).add(name)
    # Table lengths are bounded by the next referenced structure, not guessed from contents.
    fixed_ends = [p["offset"]+p["size"] for p in parts]
    boundaries = sorted(set(roles) | {envelopes, drums, len(data)} - {0})
    for pos, end in zip(boundaries, boundaries[1:]):
        if pos < max(fixed_ends):
            raise ValueError("sequence stream overlaps header tables")
        role = roles.get(pos, set())
        if pos == drums:
            if (end-pos) % 4:
                raise ValueError("unaligned drum pointer table")
            parts.append(dict(type="pointers", role="drums", offset=pos, size=end-pos,
                              values=list(struct.unpack_from(f">{(end-pos)//4}I",data,pos))))
        elif pos == envelopes:
            if (end-pos) % 7:
                raise ValueError("partial envelope table")
            parts.append(dict(type="envelopes", offset=pos, size=end-pos,
                              entries=[list(data[p:p+7]) for p in range(pos,end,7)]))
        elif "sequence" in role:
            events, stop = parse_events(data,pos,end)
            parts.append(dict(type="sequence", offset=pos, size=stop-pos, events=events))
        else:
            values, p = [], pos
            while p < end:
                begin, value = p, data[p]
                p += 1
                event = dict(value=value & 127)
                if value & 128:
                    size = varlen(data,p,end)
                    event["duration_bytes"] = list(data[p:p+size])
                    p += size
                values.append(event)
            parts.append(dict(type="modulation", role=sorted(role), offset=pos, size=end-pos, events=values))
    return dict(parts=complete_parts(data, parts))


def encode_music_part(p):
    kind = p["type"]
    if kind == "sequence_header":
        return struct.pack(">6I", *(p[k] for k in ("track_count", "sequence_table", "volume_table", "pitch_table", "envelope_table", "drum_table")))
    if kind == "pointers":
        return struct.pack(f">{len(p['values'])}I", *p["values"])
    if kind == "envelopes":
        return b"".join(bytes(e) for e in p["entries"])
    out = bytearray()
    if kind == "sequence":
        for event in p["events"]:
            if event["offset"] != p["offset"] + len(out):
                raise ValueError("sequence event offset changed")
            if "command" in event:
                out.append(128+COMMANDS.index(event["command"]))
                out.extend(event["operands"])
            else:
                if not 0 <= event["note"] < 128:
                    raise ValueError("note outside 0..127")
                out.append(event["note"])
                if "velocity_index" in event:
                    out.append(event["velocity_index"])
                out.extend(event.get("duration_bytes", []))
    elif kind == "modulation":
        for event in p["events"]:
            if not 0 <= event["value"] < 128:
                raise ValueError("modulation value outside 0..127")
            out.append(event["value"] | (128 if "duration_bytes" in event else 0))
            out.extend(event.get("duration_bytes", []))
    else:
        raise ValueError(f"unsupported sequence part {kind}")
    return bytes(out)


def pack_music(m):
    data = pack_parts(m["parts"], m["decompressed_size"], encode_music_part)
    parse_music(data)
    return data
