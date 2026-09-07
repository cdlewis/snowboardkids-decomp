"""Exact, non-overlapping binary partitions for structured asset codecs."""
from pathlib import Path


def write_dependencies(manifest_path, manifest, output):
    """Refresh per-object dependencies after each pack, including edited paths."""
    paths = [manifest_path]
    for p in manifest.get("images", []) + manifest.get("parts", []):
        if "path" in p:
            paths.append(manifest_path.parent / p["path"])
    if "path" in manifest:
        paths.append(manifest_path.parent / manifest["path"])
    def escape(path):
        return str(path).replace("\\", "\\\\").replace(" ", "\\ ").replace("#", "\\#").replace(":", "\\:")
    paths = list(dict.fromkeys(map(escape,paths)))
    output.with_suffix(".d").write_text(escape(output.with_suffix(".o")) + ": " + " ".join(paths) + "\n" +
                                        "".join(p + ":\n" for p in paths))


def complete_parts(data, parts):
    out, cursor = [], 0
    for part in sorted(parts, key=lambda p: p["offset"]):
        start, size = part["offset"], part["size"]
        if start < cursor or size < 0 or start+size > len(data):
            raise ValueError(f"invalid/overlapping part at 0x{start:X}")
        if start > cursor:
            gap = data[cursor:start]
            out.append(dict(type="padding" if not any(gap) else "raw", offset=cursor, size=len(gap), data=gap.hex()))
        out.append(part)
        cursor = start+size
    if cursor < len(data):
        gap = data[cursor:]
        out.append(dict(type="padding" if not any(gap) else "raw", offset=cursor, size=len(gap), data=gap.hex()))
    return out


def pack_parts(parts, size, encode):
    out = bytearray()
    for p in parts:
        if p["offset"] != len(out):
            raise ValueError(f"part coverage error at 0x{len(out):X}")
        data = bytes.fromhex(p["data"]) if p["type"] in ("raw", "padding") else encode(p)
        if len(data) != p["size"]:
            raise ValueError(f"part size mismatch at 0x{p['offset']:X}")
        out.extend(data)
    if len(out) != size:
        raise ValueError("incomplete asset coverage")
    return bytes(out)
