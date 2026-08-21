#!/usr/bin/env python3
from __future__ import annotations

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from tools.course_surface_data_common import load_yaml, metadata_from_manifest, pack_course_surface_data
from tools.huffman_asset import compress_huffman_asset


def main() -> int:
    parser = argparse.ArgumentParser(description="Pack editable Snowboard Kids course surface data.")
    parser.add_argument("manifest", type=Path)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()

    manifest = load_yaml(args.manifest)
    packed = pack_course_surface_data(manifest)
    compressed = compress_huffman_asset(packed, metadata_from_manifest(manifest))
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_bytes(compressed)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
