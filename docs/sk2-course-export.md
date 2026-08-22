# Snowboard Kids 2 course export

`tools/export_sk1_courses_to_sk2.py` creates ROM-derived course packages whose
asset payloads use the layouts consumed by the Snowboard Kids 2 course loader.
The repository does not contain the generated Nintendo data. Output defaults to
the ignored `build/sk2_course_export` directory and requires a verified North
American Snowboard Kids ROM.

```bash
python3 tools/export_sk1_courses_to_sk2.py --rom snowboardkids.z64 --all
```

Use `--course big_snowman` to export one course, or `--output PATH` to select a
different generated-data directory. Each course directory contains:

- `manifest.yaml`, an ABI-versioned inventory suitable for an SK2 importer;
- uncompressed F3DEX2 `display_lists.bin` using segment 1 for display-list calls
  and segment 2 for model resources;
- raw and Sno-compressed model resources, track mesh, texture table (when the
  SK1 course has one), gold-coin positions, and item-box positions;
- `runtime_hints.yaml`, with converted start/camera/scenery positions and named
  main/backdrop entry points; and
- `conversion_report.yaml`, which records every intentional approximation.

The `source/` subdirectory retains the decompressed SK1 graphics, surface, and
optional sprite payloads. This makes a generated package lossless even where
the SK2 semantic mapping is still approximate. These files are ROM-derived and
remain ignored along with the converted assets.

The exporter preserves model and collision `s16` coordinates. SK1 runtime
positions use one more fractional bit than SK2, so those signed values are
divided by two. The display-list converter only rewrites commands reachable
from documented roots; unrelated and embedded bytes in the graphics bundle are
preserved exactly.

## Integration boundary

The binary members correspond directly to SK2 `CourseAssetBundle` fields.
`sceneAnimation` is null because SK1 has no equivalent per-course asset. The
recompilation project remains responsible for supplying its `CourseDefinition`
environment, preview, race, and overlay fields and for deciding how SK1 surface
types map to SK2 surface material indices.

The track converter intentionally records uncertain semantics instead of
claiming an exact mapping: SK1 surface type bits are retained in face flags,
`surface_index` is neutral, the SK1 face range is used for both SK2 face ranges,
and unknown sector bytes are zeroed except for path-reference indices. These
choices make all source geometry and topology available without hiding the
remaining integration work.
