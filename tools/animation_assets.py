"""Animation layouts consumed by race_motion and main_menu_scene_model."""
import struct


def parse_animation(data, family, part_count=12):
    wide = family == "scene_animation"
    word = 4 if wide else 2
    if len(data) < word:
        raise ValueError("truncated animation table")
    first = int.from_bytes(data[:word], "big") * 2
    if not 0 < first <= len(data) or first % word:
        raise ValueError("invalid animation table size")
    offsets = [int.from_bytes(data[p:p+word], "big") for p in range(0, first, word)]
    starts = sorted(set(o * 2 for o in offsets))
    if starts[0] != first:
        raise ValueError("animation overlaps offset table")
    animations = []
    for start, end in zip(starts, starts[1:] + [len(data)]):
        count = struct.unpack_from(">h", data, start)[0]
        stride = part_count * 12 if wide else 12 + part_count * 4
        if count < 0 or start + 2 + count * stride != end:
            raise ValueError(f"animation at 0x{start:X}: inconsistent frame count/stride")
        frames = []
        for pos in range(start+2, end, stride):
            if wide:
                values = struct.unpack_from(f">{part_count * 6}h", data, pos)
                frames.append(dict(offsets=[list(values[i:i+3]) for i in range(0, part_count*3, 3)],
                                   rotations=[list(values[i:i+3]) for i in range(part_count*3, part_count*6, 3)]))
            else:
                translations = struct.unpack_from(">6h", data, pos)
                rotations, reserved = [], []
                for i in range(part_count):
                    a, b = struct.unpack_from(">HH", data, pos+12+i*4)
                    rotations.append([(a >> 8)*16 + (b & 1)*8, (a & 255)*16 + ((b >> 1) & 1)*8,
                                      (b >> 8)*16 + ((b >> 2) & 1)*8])
                    reserved.append(b & 0xF8)
                frames.append(dict(offsets=[list(translations[:3]), list(translations[3:])],
                                   rotations=rotations, reserved_rotation_bits=reserved))
        animations.append(dict(offset=start, frame_count=count, frames=frames))
    return dict(offset_unit="u16", animation_offsets=offsets, part_count=part_count, animations=animations)


def pack_animation(m):
    wide = m["format"] == "scene_animation"
    word = 4 if wide else 2
    out = bytearray(b"".join(o.to_bytes(word, "big") for o in m["animation_offsets"]))
    count = m["part_count"]
    for animation in m["animations"]:
        if len(out) != animation["offset"] or len(animation["frames"]) != animation["frame_count"]:
            raise ValueError("animation offset/count mismatch")
        out.extend(struct.pack(">h", animation["frame_count"]))
        for frame in animation["frames"]:
            if len(frame["offsets"]) != (count if wide else 2) or len(frame["rotations"]) != count:
                raise ValueError("animation joint count mismatch")
            for xyz in frame["offsets"]:
                out.extend(struct.pack(">3h", *xyz))
            if wide:
                for xyz in frame["rotations"]:
                    out.extend(struct.pack(">3h", *xyz))
            else:
                if len(frame["reserved_rotation_bits"]) != count:
                    raise ValueError("rotation reserved-bit count mismatch")
                for xyz, reserved in zip(frame["rotations"], frame["reserved_rotation_bits"]):
                    if len(xyz) != 3 or any(v < 0 or v > 4088 or v % 8 for v in xyz) or reserved & ~0xF8:
                        raise ValueError("race rotation must be 0..4088 in steps of 8")
                    x, y, z = xyz
                    out.extend(struct.pack(">HH", (x//16 << 8) | y//16,
                                           (z//16 << 8) | ((x & 8) >> 3) | ((y & 8) >> 2) | ((z & 8) >> 1) | reserved))
    parse_animation(bytes(out), m["format"], count)
    return bytes(out)
