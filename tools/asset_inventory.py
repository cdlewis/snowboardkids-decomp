#!/usr/bin/env python3
"""Report readable coverage and verify each registered asset against its ROM slice."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.course_surface_data_common import load_yaml, pack_course_surface_data
from tools.course_sprite_table_common import pack_course_sprite_table
from tools.course_graphics_common import pack_course_model_resources, compression_metadata_from_manifest
from tools.huffman_asset import compress_huffman_asset
from tools.readable_assets import pack_asset
from tools.source_assets import extract_source_assets, source_files
from tools.asset_bundles import ASSET_TYPES

ROOT = Path(__file__).resolve().parents[1]


def inventory(root, rom, verify=False):
    config = load_yaml(root / "snowboardkids.yaml")
    if hashlib.sha1(rom).hexdigest() != config["sha1"]:
        raise ValueError("ROM does not match snowboardkids.yaml SHA1")
    segments = [s for s in config["segments"] if isinstance(s, list) or "start" in s]
    consumers = {}
    for path in source_files(root):
        source = path.read_text()
        for name in set(re.findall(r"(?:LOAD_ASSET|LOAD_RAW_ASSET|DMA_ASSET|USE_ASSET)\(\s*(\w+)|\b(\w+)_ROM_(?:START|END)\b", source)):
            consumers.setdefault(name[0] or name[1], []).append(str(path.relative_to(root)))
    result = []
    layout_path = root / 'assets/layout/segments.yaml'
    canonical = {r['name']: r for r in load_yaml(layout_path)['segments']} if layout_path.exists() else {}
    families = {"course_surface_data": ("course_surface_data", pack_course_surface_data),
                "course_sprite_table": ("course_sprite_tables", pack_course_sprite_table),
                "course_model_resources": ("course_model_resources", pack_course_model_resources),
                "model_resources": ("course_model_resources", pack_course_model_resources),
                **{kind: ("readable", pack_asset) for kind in ASSET_TYPES}}
    for entry, following in zip(segments, segments[1:]):
        s = entry if isinstance(entry, dict) else dict(start=entry[0], type=entry[1], name=f"{entry[0]:X}")
        start = s["start"]
        end = following["start"] if isinstance(following, dict) else following[0]
        if s["type"] in ("code", "header") or start < 0x1000:
            continue
        name = str(s.get("name", f"{start:X}"))
        row = dict(name=name, rom_start=start, rom_end=end, rom_size=end-start,
                   decoder=s["type"], consumers=consumers.get(name, []), outputs=[],
                   status="unclassified", rebuild="not_checked")
        if name in canonical:
            from tools.asset_bundles import pack_segment
            record = canonical[name]
            row.update(bundle=record['bundle'], outputs=[record['bundle'], record['source']],
                       status='readable' if (root/record['source']).exists() else 'missing')
            if verify:
                try:
                    row['rebuild'] = 'match' if pack_segment(root,record)==rom[start:end] else 'mismatch'
                except (ValueError, KeyError, OSError) as exc:
                    row.update(rebuild='error', error=str(exc))
        elif len(set(rom[start:end])) == 1 and rom[start] in (0, 255):
            row["status"] = "padding"
        elif s["type"] in families:
            directory, packer = families[s["type"]]
            path = root / "assets" / directory / f"{name}.yaml"
            row["outputs"] = [str(path.relative_to(root))]
            if path.exists():
                m = load_yaml(path)
                raw = [dict(offset=p["offset"], size=len(bytes.fromhex(str(p["data"]))))
                       for p in m.get("parts", []) if p["type"] == "raw" and any(bytes.fromhex(str(p["data"])))]
                raw += [dict(offset=p["offset"], size=len(bytes.fromhex(str(p["data"])))) for p in m.get("raw_parts", []) if any(bytes.fromhex(str(p["data"])))]
                padding = [dict(offset=p["offset"], size=len(bytes.fromhex(str(p["data"]))))
                           for p in m.get("parts", []) + m.get("raw_parts", [])
                           if "data" in p and p.get("type", "raw") in ("raw", "padding") and not any(bytes.fromhex(str(p["data"])))]
                row.update(format=m["format"], status="partial" if raw else "readable",
                           decompressed_size=m.get("decompressed_size"), unclassified_ranges=raw, padding_ranges=padding)
                row["outputs"] += [str((path.parent / p["path"]).relative_to(root))
                                   for p in m.get("images", []) + m.get("parts", []) if "path" in p]
                if "path" in m:
                    row["outputs"].append(str((path.parent / m["path"]).relative_to(root)))
                if verify:
                    try:
                        if s["type"] in ASSET_TYPES:
                            rebuilt = packer(m, path.parent)
                        else:
                            payload = packer(m) if s["type"] == "course_surface_data" else packer(m, path.parent)
                            rebuilt = compress_huffman_asset(payload, compression_metadata_from_manifest(m))
                        row["rebuild"] = "match" if rebuilt == rom[start:end] else "mismatch"
                    except (ValueError, KeyError, OSError) as exc:
                        row.update(rebuild="error", error=str(exc))
            else:
                row["status"] = "missing"
                if verify:
                    row["rebuild"] = "error"
        elif s["type"] == "course_display_list":
            path = root / "assets/course_display_lists" / f"{name}.c"
            row["outputs"] = [str(path.relative_to(root))]
            row["status"] = "readable" if path.exists() else "missing"
            if verify:
                if path.exists():
                    words = re.findall(r"\{\{0x([0-9A-F]{8}), 0x([0-9A-F]{8})\}\}", path.read_text())
                    rebuilt = bytes.fromhex("".join(a+b for a,b in words))
                    row["rebuild"] = "match" if rebuilt == rom[start:end] else "mismatch"
                else:
                    row["rebuild"] = "error"
        result.append(row)
    return dict(schema_version=1, rom_sha1=config["sha1"],
                coverage_scope="Top-level ROM assets; source-resident assets require a separate semantic audit.",
                assets=result)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--rom", type=Path, default=ROOT / "snowboardkids.z64")
    p.add_argument("--output", type=Path, default=ROOT / "build/asset-report.json")
    p.add_argument("--verify", action="store_true")
    p.add_argument("--source-views", action="store_true", help="regenerate inspection views for existing C assets")
    p.add_argument("--source-views-only", action="store_true")
    a = p.parse_args()
    if a.source_views_only:
        extract_source_assets(ROOT, a.rom.read_bytes())
        return 0
    report = inventory(ROOT, a.rom.read_bytes(), a.verify)
    if a.source_views:
        report["source_assets"] = extract_source_assets(ROOT, a.rom.read_bytes())
    elif (ROOT / "assets/source_views/index.yaml").exists():
        report["source_assets"] = load_yaml(ROOT / "assets/source_views/index.yaml")["assets"]
    a.output.parent.mkdir(parents=True, exist_ok=True)
    a.output.write_text(json.dumps(report, indent=2) + "\n")
    rows = report["assets"]
    print(", ".join(f"{sum(r['status'] == status for r in rows)} {status}" for status in ("readable", "partial", "unclassified", "padding", "missing")))
    failures = [r for r in rows if r["rebuild"] in ("mismatch", "error")]
    if a.verify:
        print(f"{sum(r['rebuild'] == 'match' for r in rows)} exact asset matches; {len(failures)} failures")
        for r in failures:
            print(f"{r['name']}: {r.get('error', r['rebuild'])}", file=sys.stderr)
    print(f"Report: {a.output}")
    return bool(failures)


if __name__ == "__main__":
    raise SystemExit(main())
