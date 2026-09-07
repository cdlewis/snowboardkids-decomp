"""Lossless indexed N64 images. PNG palettes are previews; manifest words are exact."""
from pathlib import Path

import png


def rgba16(value):
    return tuple(((value >> shift) & 31) * 255 // 31 for shift in (11, 6, 1)) + (255 if value & 1 else 0,)


def write_indexed(path, data, width, height, fmt, palette):
    bits = {"ci4": 4, "ci8": 8}[fmt]
    count = width * height
    if width <= 0 or height <= 0 or len(data) != (count * bits + 7) // 8:
        raise ValueError(f"{path}: invalid image size")
    pixels = list(data) if bits == 8 else [n for b in data for n in (b >> 4, b & 15)]
    colors = [rgba16(v) for v in palette]
    if len(colors) != 1 << bits:
        raise ValueError(f"{path}: expected {1 << bits} palette colors")
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("wb") as stream:
        png.Writer(width, height, palette=colors, bitdepth=bits).write(
            stream, (pixels[row * width:(row + 1) * width] for row in range(height)))


def read_indexed(path, width, height, fmt, padding_nibble=0):
    bits = {"ci4": 4, "ci8": 8}[fmt]
    with Path(path).open("rb") as stream:
        w, h, rows, info = png.Reader(file=stream).read()
        if (w, h) != (width, height) or "palette" not in info:
            raise ValueError(f"{path}: expected indexed {width}x{height} PNG")
        pixels = [int(v) for row in rows for v in row]
    if any(v >= 1 << bits for v in pixels):
        raise ValueError(f"{path}: pixel index exceeds {fmt} range")
    if bits == 8:
        return bytes(pixels)
    if not 0 <= padding_nibble < 16:
        raise ValueError(f"{path}: invalid padding nibble")
    if len(pixels) % 2:
        pixels.append(padding_nibble)
    return bytes((a << 4) | b for a, b in zip(pixels[::2], pixels[1::2]))


def image_bytes(image, base_path=Path(".")):
    if "path" in image:
        if "data" in image:
            raise ValueError("image must have either path or data, not both")
        return read_indexed(base_path / image["path"], int(image["width"]), int(image["height"]),
                            image["format"], int(image.get("padding_nibble", 0)))
    return bytes.fromhex(str(image["data"]))


def externalize_image(image, base_path, relative_path, palette):
    data = bytes.fromhex(str(image.pop("data")))
    write_indexed(base_path / relative_path, data, image["width"], image["height"], image["format"], palette)
    image["path"] = relative_path
    if image["format"] == "ci4" and image["width"] * image["height"] % 2:
        image["padding_nibble"] = data[-1] & 15


def externalize_sprites(manifest, path):
    palettes = manifest["palettes"]
    for image in manifest["images"]:
        entries = [e for e in manifest["entries"] if e["image_offset"] == image["offset"]]
        index = entries[0]["palette_index"]
        colors = 16 if image["format"] == "ci4" else 256
        palette = [v for p in palettes[index:index + colors // 16] for v in p["values"]]
        externalize_image(image, path.parent, f"{manifest['name']}/{image['name']}.png", palette)
        for entry in entries:
            if entry["palette_index"] == index:
                entry["preview"] = image["path"]
            else:
                slot = entry["palette_index"]
                alternate = [v for p in palettes[slot:slot + colors // 16] for v in p["values"]]
                relative = f"{manifest['name']}/previews/{entry['name']}.png"
                write_indexed(path.parent / relative, image_bytes(image, path.parent), image["width"], image["height"], image["format"], alternate)
                entry["preview"] = relative
    manifest["schema_version"] = 2


def write_tilemap_preview(manifest, path):
    """Compose the tile grid using SK1's one-based image and flip conventions."""
    h = manifest["header"]
    width, height = h["grid_width"] * h["tile_width"], h["grid_height"] * h["tile_height"]
    rows = [bytearray(width*4) for _ in range(height)]
    cache = {}
    for cell, index in enumerate(manifest["tilemap"]):
        if not index:
            continue
        tile = manifest["tiles"][index]
        image = manifest["images"][tile["image_index"]-1]
        key = tile["image_index"]
        if key not in cache:
            data = image_bytes(image, path.parent)
            cache[key] = list(data) if image["format"] == "ci8" else [n for b in data for n in (b >> 4,b & 15)]
        slots = 16 if image["format"] == "ci8" else 1
        palette = [rgba16(v) for p in manifest["palettes"][tile["palette_index"]:tile["palette_index"]+slots] for v in p]
        x0, y0 = (cell % h["grid_width"])*h["tile_width"], (cell // h["grid_width"])*h["tile_height"]
        for y in range(h["tile_height"]):
            sy = h["tile_height"]-1-y if tile["flip"] & 2 else y
            for x in range(h["tile_width"]):
                sx = h["tile_width"]-1-x if tile["flip"] & 1 else x
                color = palette[cache[key][sy*h["tile_width"]+sx]]
                rows[y0+y][(x0+x)*4:(x0+x+1)*4] = bytes(color)
    relative = f"{manifest['name']}/preview.png"
    with (path.parent / relative).open("wb") as stream:
        png.Writer(width, height, greyscale=False, alpha=True).write(stream, rows)
    manifest["preview"] = relative


def externalize_model(manifest, path):
    palettes = [p for p in manifest["parts"] if p["type"] == "palette"]
    for part in manifest["parts"]:
        if part["type"] != "texture":
            continue
        colors = 16 if part["format"] == "ci4" else 256
        candidates = [p for p in palettes if p["colors"] == colors and
                      set(p.get("load_slots", [])) & set(part.get("palette_slots", []))]
        # A TMEM slot can be reused by many palettes. Do not imply a unique pairing.
        part["palette_candidates"] = [p["offset"] for p in candidates]
        palette = candidates[0]["values"] if candidates else [((i * 31 // (colors - 1)) * 0x842) | 1 for i in range(colors)]
        part["preview_palette_offset"] = candidates[0]["offset"] if candidates else None
        externalize_image(part, path.parent, f"{manifest['name']}/texture_{part['offset']:06X}.png", palette)
    manifest["schema_version"] = 2
