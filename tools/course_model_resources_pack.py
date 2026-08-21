#!/usr/bin/env python3
from __future__ import annotations

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from tools.course_graphics_common import compression_metadata_from_manifest, pack_course_model_resources
from tools.course_surface_data_common import load_yaml
from tools.huffman_asset import compress_huffman_asset


def main() -> None:
    parser = argparse.ArgumentParser(description="Pack editable SK1 course model resources.")
    parser.add_argument("manifest", type=Path)
    parser.add_argument("--out", required=True, type=Path)
    args = parser.parse_args()

    manifest = load_yaml(args.manifest)
    packed = pack_course_model_resources(manifest)
    compressed = compress_huffman_asset(packed, compression_metadata_from_manifest(manifest))
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_bytes(compressed)


if __name__ == "__main__":
    main()
