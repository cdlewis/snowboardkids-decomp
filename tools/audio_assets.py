"""Mus pointer banks and N64 VADPCM samples, using the game's ALWaveTable ABI."""
import struct
import wave

from tools.asset_parts import complete_parts, pack_parts


def parse_pointer_bank(data):
    if len(data) < 20 or data[:14] != b"N64 PtrTables ":
        raise ValueError("invalid Mus pointer-bank signature")
    count = struct.unpack_from(">I", data, 16)[0]
    if not 0 < count <= (len(data)-20)//4:
        raise ValueError("invalid wave count")
    offsets = list(struct.unpack_from(f">{count}I", data, 20))
    parts = [dict(type="bank_header", offset=0, size=20+count*4,
                  signature=data[:16].hex(), wave_offsets=offsets)]
    books, loops = set(), set()
    for pos in sorted(set(offsets)):
        if pos < 20+count*4 or pos+20 > len(data):
            raise ValueError("wave offset outside bank")
        values = struct.unpack_from(">IIBBHII", data, pos)
        p = dict(zip(("sample_offset", "sample_size", "wave_type", "flags", "reserved", "loop_offset", "book_offset"), values))
        if p["wave_type"] != 0 or p["flags"] != 0:
            raise ValueError("expected unrelocated ADPCM wave")
        parts.append(dict(type="wavetable", offset=pos, size=20, **p))
        books.add(p["book_offset"])
        if p["loop_offset"]:
            loops.add(p["loop_offset"])
    for pos in sorted(books):
        if not 0 < pos <= len(data)-8:
            raise ValueError("invalid codebook offset")
        order, predictors = struct.unpack_from(">II", data, pos)
        count = order * predictors * 8
        if not 0 < order <= 8 or not 0 < predictors <= 16 or pos+8+count*2 > len(data):
            raise ValueError("invalid ADPCM codebook dimensions")
        parts.append(dict(type="codebook", offset=pos, size=8+count*2, order=order, predictors=predictors,
                          coefficients=list(struct.unpack_from(f">{count}h", data, pos+8))))
    for pos in sorted(loops):
        if not 0 < pos <= len(data)-44:
            raise ValueError("invalid ADPCM loop offset")
        start, end, count = struct.unpack_from(">III", data, pos)
        if start > end:
            raise ValueError("invalid ADPCM loop range")
        parts.append(dict(type="loop", offset=pos, size=44, start=start, end=end, count=count,
                          state=list(struct.unpack_from(">16h", data, pos+12))))
    return dict(parts=complete_parts(data, parts))


def encode_pointer_part(p):
    if p["type"] == "bank_header":
        return bytes.fromhex(p["signature"]) + struct.pack(f">{len(p['wave_offsets'])+1}I", len(p["wave_offsets"]), *p["wave_offsets"])
    if p["type"] == "wavetable":
        return struct.pack(">IIBBHII", *(p[k] for k in ("sample_offset", "sample_size", "wave_type", "flags", "reserved", "loop_offset", "book_offset")))
    if p["type"] == "codebook":
        if len(p["coefficients"]) != p["order"] * p["predictors"] * 8:
            raise ValueError("codebook coefficient count mismatch")
        return struct.pack(f">II{len(p['coefficients'])}h", p["order"], p["predictors"], *p["coefficients"])
    if p["type"] == "loop":
        return struct.pack(">III16h", p["start"], p["end"], p["count"], *p["state"])
    raise ValueError(f"unknown pointer-bank part {p['type']}")


def pack_pointer_bank(m):
    data = pack_parts(m["parts"], m["decompressed_size"], encode_pointer_part)
    parse_pointer_bank(data)
    return data


def decode_vadpcm(data, book):
    order, predictors, coefficients = book["order"], book["predictors"], book["coefficients"]
    if len(data) % 9 or not 1 <= order <= 8 or len(coefficients) != order*predictors*8:
        raise ValueError("invalid VADPCM frames/codebook")
    history = [0] * order
    result = []
    for pos in range(0, len(data), 9):
        scale, predictor = data[pos] >> 4, data[pos] & 15
        if predictor >= predictors:
            raise ValueError(f"invalid predictor at sample byte 0x{pos:X}")
        table = coefficients[predictor*order*8:(predictor+1)*order*8]
        residuals = [(n if n < 8 else n-16) << scale for b in data[pos+1:pos+9] for n in (b >> 4, b & 15)]
        for half in (residuals[:8], residuals[8:]):
            decoded = []
            for i in range(8):
                value = (half[i] << 11) + sum(table[j*8+i] * history[j] for j in range(order))
                value += sum(table[(order-1)*8+i-j-1] * half[j] for j in range(i))
                # SDK decoder uses a signed 32-bit accumulator and floor division.
                value = ((value + (1 << 31)) % (1 << 32)) - (1 << 31)
                decoded.append(value >> 11)
            history = decoded[-order:]
            result.extend(max(-32768, min(32767, v)) for v in decoded)
    return result


def extract_samples(rom, start, end, sample_base, bank, path):
    parts = []
    if start == sample_base:
        if rom[start:start+16] != b"N64 WaveTables \0":
            raise ValueError("invalid wave-bank signature")
        parts.append(dict(type="signature", offset=0, size=16, text="N64 WaveTables ", terminator=0))
    books = {p["offset"]: p for p in bank["parts"] if p["type"] == "codebook"}
    waves = [p for p in bank["parts"] if p["type"] == "wavetable"]
    seen = set()
    for w in waves:
        a = sample_base + w["sample_offset"]
        b = a + w["sample_size"]
        if b > len(rom) or a < 0:
            raise ValueError("sample exceeds ROM")
        if a >= end or b <= start or (a, b) in seen:
            continue
        seen.add((a,b))
        relative = f"samples/sample_{w['sample_offset']:06X}.vadpcm"
        target = path.parent / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(rom[a:b])
        pcm = decode_vadpcm(rom[a:b], books[w["book_offset"]])
        with wave.open(str(target.with_suffix(".wav")), "wb") as output:
            output.setparams((1, 2, 22050, 0, "NONE", "not compressed"))
            output.writeframes(struct.pack(f"<{len(pcm)}h", *pcm))
        parts.append(dict(type="sample", offset=max(a,start)-start, size=min(b,end)-max(a,start),
                          path=relative, source_offset=max(a,start)-a, source_size=b-a,
                          book_offset=w["book_offset"], loop_offset=w["loop_offset"],
                          preview_sample_rate=22050,
                          preview_note="Reference playback rate; the engine applies per-note pitch."))
    return dict(parts=complete_parts(rom[start:end], parts))


def pack_samples(m, base):
    def encode(p):
        if p["type"] == "signature":
            return p["text"].encode("ascii") + bytes([p["terminator"]])
        data = (base / p["path"]).read_bytes()
        if len(data) != p["source_size"]:
            raise ValueError(f"{p['path']}: encoded sample size changed")
        return data[p["source_offset"]:p["source_offset"]+p["size"]]
    return pack_parts(m["parts"], m["decompressed_size"], encode)
