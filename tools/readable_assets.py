"""SK1 asset-family codecs shared by Splat, packers, and verification."""
from pathlib import Path
import struct
import csv

from tools.asset_images import externalize_sprites, externalize_image, image_bytes, write_tilemap_preview
from tools.course_sprite_table_common import parse_course_sprite_table, pack_course_sprite_table
from tools.course_graphics_common import compression_metadata_from_manifest
from tools.huffman_asset import decompress_huffman_asset, compress_huffman_asset
from tools.animation_assets import parse_animation, pack_animation
from tools.audio_assets import parse_pointer_bank, pack_pointer_bank, pack_samples
from tools.model_assets import parse_embedded_model, pack_embedded_model
from tools.music_assets import parse_music, pack_music


def compression_dict(metadata):
    return dict(flags=metadata.flags, huffman_table=metadata.table.hex(), padding_bits=metadata.padding_bits,
                unused_tail=metadata.unused_tail.hex(),
                lz_tokens=metadata.lz_tokens.hex() if metadata.lz_tokens is not None else None)


def parse_tilemap(data, fmt):
    if len(data) < 16:
        raise ValueError("truncated tilemap header")
    keys = ("grid_width", "grid_height", "tile_width", "tile_height", "tile_count",
            "tilemap_offset", "palette_offset", "image_offset")
    header = dict(zip(keys, struct.unpack_from(">8H", data)))
    gw, gh, w, h, count, map_start, palette_start, image_start = header.values()
    if not all((gw, gh, w, h)) or not (16 + count * 4 <= map_start <= palette_start <= image_start <= len(data)):
        raise ValueError("invalid tilemap dimensions or offsets")
    if map_start + gw * gh * 2 > palette_start or (image_start - palette_start) % 32:
        raise ValueError("invalid tilemap/palette ranges")
    tiles = [dict(zip(("image_index", "palette_index", "flip"), struct.unpack_from(">hBB", data, 16 + i * 4))) for i in range(count)]
    indices = list(struct.unpack_from(f">{gw * gh}h", data, map_start))
    if any(i < 0 or i >= count for i in indices):
        raise ValueError("invalid tilemap tile reference")
    palettes = [list(struct.unpack_from(">16H", data, p)) for p in range(palette_start, image_start, 32)]
    image_size = (w * h + 1) // 2 if fmt == "ci4" else w * h
    image_count, tail = divmod(len(data) - image_start, image_size)
    if tail:
        raise ValueError("partial tile image")
    for t in tiles[1:]:
        if not 0 <= t["image_index"] <= image_count or t["palette_index"] + (1 if fmt == "ci4" else 16) > len(palettes) or not 0 <= t["flip"] < 4:
            raise ValueError("invalid tile image/palette reference")
    if any(i and tiles[i]["image_index"] == 0 for i in indices):
        raise ValueError("tilemap references an empty image through a nonzero tile")
    images = [dict(name=f"tile_{i:03d}", offset=image_start + i * image_size, format=fmt, width=w, height=h,
                   data=data[image_start+i*image_size:image_start+(i+1)*image_size].hex()) for i in range(image_count)]
    return dict(header=header, tiles=tiles, tilemap=indices, palettes=palettes, images=images,
                table_padding=data[16+count*4:map_start].hex(), map_padding=data[map_start+gw*gh*2:palette_start].hex())


def pack_tilemap(m, base):
    h = m["header"]
    keys = ("grid_width", "grid_height", "tile_width", "tile_height", "tile_count",
            "tilemap_offset", "palette_offset", "image_offset")
    if len(m["tiles"]) != h["tile_count"] or len(m["tilemap"]) != h["grid_width"] * h["grid_height"]:
        raise ValueError("tilemap count mismatch")
    out = bytearray(struct.pack(">8H", *(h[k] for k in keys)))
    for t in m["tiles"]:
        out.extend(struct.pack(">hBB", t["image_index"], t["palette_index"], t["flip"]))
    out.extend(bytes.fromhex(m["table_padding"]))
    if len(out) != h["tilemap_offset"]:
        raise ValueError("tilemap offset mismatch")
    out.extend(struct.pack(f">{len(m['tilemap'])}h", *m["tilemap"]))
    out.extend(bytes.fromhex(m["map_padding"]))
    if len(out) != h["palette_offset"]:
        raise ValueError("palette offset mismatch")
    for p in m["palettes"]:
        out.extend(struct.pack(">16H", *p))
    if len(out) != h["image_offset"]:
        raise ValueError("image offset mismatch")
    for image in m["images"]:
        if image["offset"] != len(out):
            raise ValueError("tile image offset mismatch")
        out.extend(image_bytes(image, base))
    # Apply the same reference checks when packing edited sources.
    parse_tilemap(bytes(out), m["image_format"])
    return bytes(out)


def extract_asset(blob, spec, path):
    if spec.get("compression") == "none":
        data, metadata = blob, None
    else:
        data, metadata = decompress_huffman_asset(blob)
    family = spec["type"]
    m = dict(schema_version=2, name=spec["name"], format=family, decompressed_size=len(data),
             compression=compression_dict(metadata) if metadata else "none")
    if family == "sprite_table":
        ci8 = list(spec.get("ci8_entries", []))
        for start, end in spec.get("ci8_entry_ranges", []):
            ci8.extend(range(start, end))
        m.update(parse_course_sprite_table(data, ci8, True))
        externalize_sprites(m, path)
    elif family == "tilemap":
        m["image_format"] = spec["image_format"]
        m.update(parse_tilemap(data, m["image_format"]))
        for i, image in enumerate(m["images"]):
            tiles = [t for t in m["tiles"][1:] if t["image_index"] == i + 1]
            slots = sorted({t["palette_index"] for t in tiles})
            image["palette_slots"] = slots
            slot = slots[0] if slots else 0
            count = 1 if m["image_format"] == "ci4" else 16
            palette = [v for p in m["palettes"][slot:slot+count] for v in p]
            externalize_image(image, path.parent, f"{m['name']}/{image['name']}.png", palette)
        write_tilemap_preview(m,path)
    elif family in ("race_animation", "scene_animation"):
        m.update(parse_animation(data, family, spec.get("part_count", 12)))
    elif family == "pointer_bank":
        m.update(parse_pointer_bank(data))
    elif family == "embedded_model":
        m.update(parse_embedded_model(data, spec["root_offsets"], path))
    elif family == "music_sequence":
        m.update(parse_music(data))
    elif family == "replay":
        if len(data) != 4 * 2400 * 3:
            raise ValueError("unexpected replay size")
        relative = f"{m['name']}/inputs.csv"
        target = path.parent / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        with target.open("w", newline="") as stream:
            writer = csv.writer(stream, lineterminator="\n")
            writer.writerow(("player", "frame", "buttons", "stick_x", "stick_y"))
            for i in range(4 * 2400):
                writer.writerow((i // 2400, i % 2400, *struct.unpack_from(">Bbb", data, i*3)))
        m.update(path=relative, player_count=4, frames_per_player=2400)
    else:
        raise ValueError(f"unsupported asset format {family}")
    return m


def pack_payload(m, base):
    if m.get("schema_version") != 2:
        raise ValueError("unsupported readable asset schema version")
    if m["format"] == "sprite_table":
        payload = pack_course_sprite_table(m, base)
    elif m["format"] == "tilemap":
        payload = pack_tilemap(m, base)
    elif m["format"] in ("race_animation", "scene_animation"):
        payload = pack_animation(m)
    elif m["format"] == "pointer_bank":
        payload = pack_pointer_bank(m)
    elif m["format"] == "sample_bank":
        payload = pack_samples(m, base)
    elif m["format"] == "embedded_model":
        payload = pack_embedded_model(m, base)
    elif m["format"] == "music_sequence":
        payload = pack_music(m)
    elif m["format"] == "replay":
        out = bytearray()
        with (base / m["path"]).open(newline="") as stream:
            rows = csv.DictReader(stream)
            for i, row in enumerate(rows):
                if int(row["player"]) != i // m["frames_per_player"] or int(row["frame"]) != i % m["frames_per_player"]:
                    raise ValueError("replay rows must be in player/frame order")
                out.extend(struct.pack(">Bbb", *(int(row[k], 0) for k in ("buttons", "stick_x", "stick_y"))))
        if len(out) != m["player_count"] * m["frames_per_player"] * 3:
            raise ValueError("replay frame count mismatch")
        payload = bytes(out)
    else:
        raise ValueError(f"unsupported asset format {m['format']}")
    if len(payload) != m["decompressed_size"]:
        raise ValueError("decompressed size mismatch")
    return payload


def pack_asset(m, base):
    payload = pack_payload(m, base)
    return payload if m["compression"] == "none" else compress_huffman_asset(payload, compression_metadata_from_manifest(m))
