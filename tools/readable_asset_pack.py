#!/usr/bin/env python3
import argparse
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.course_surface_data_common import load_yaml
from tools.readable_assets import pack_asset
from tools.asset_parts import write_dependencies


def main():
    p = argparse.ArgumentParser(description="Rebuild an editable SK1 asset")
    p.add_argument("manifest", type=Path)
    p.add_argument("--out", type=Path, required=True)
    a = p.parse_args()
    manifest = load_yaml(a.manifest)
    result = pack_asset(manifest, a.manifest.parent)
    a.out.parent.mkdir(parents=True, exist_ok=True)
    a.out.write_bytes(result)
    write_dependencies(a.manifest, manifest, a.out)


if __name__ == "__main__":
    main()
