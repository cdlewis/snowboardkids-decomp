# Course asset inventory

Snowboard Kids has ten course environments in ROM: the nine race courses and
Animal Land, which is used by the Trick Game. The course index used by race code
is not progression order; Rookie Mountain is index 9 even though it is the first
normal race course presented to the player.

The canonical course-index mapping is:

| Course index | Course | Primary use |
| ---: | --- | --- |
| 0 | Big Snowman | Race and skill games |
| 1 | Sunset Rock | Race and skill games |
| 2 | Night Highway | Race |
| 3 | Grass Valley | Race |
| 4 | Dizzy-Land | Race |
| 5 | Quicksand Valley | Race |
| 6 | Silver Mountain | Race |
| 7 | Animal Land | Trick Game |
| 8 | Ninja Land | Secret race |
| 9 | Rookie Mountain | Race, training, and transition scenes |

The ROM also contains older or internal English labels in the default save-data
template. `BEGISNOW`, `TRAPED`, `SNOW GARDEN`, and `ZIPANGU` correspond to Big
Snowman, Sunset Rock, Dizzy-Land, and Ninja Land respectively. They are aliases,
not additional environments. Asset names use the retail US course names.

## Per-course assets

Each environment is assembled from up to four independently loaded assets:

- `<COURSE>_COURSE_GRAPHICS` is an uncompressed RSP segment 2 graphics bundle.
  It contains the nested display-list graph and a small amount of embedded
  resource data; it is not the entire level.
- `<COURSE>_COURSE_MODEL_RESOURCES` is a compressed RSP segment 3 bundle. It
  contains the course vertices plus texture, palette, and other model data.
- `<COURSE>_COURSE_SURFACE_DATA` is compressed collision and course-path data.
- `<COURSE>_COURSE_SPRITES` is an optional compressed course-specific sprite
  table. Animal Land does not load or have one.

The ranges below are half-open ROM ranges (`start` through, but not including,
`end`). Sizes are compressed sizes for assets loaded through `LOAD_ASSET`.

| Id | Course | Segment 2 graphics | Model resources | Surface data | Sprites |
| ---: | --- | --- | --- | --- | --- |
| 0 | Big Snowman | `0x0E35C0..0x0EC9A0` (`0x93E0`) | `0x163410..0x171480` (`0xE070`) | `0x21D9D0..0x222500` (`0x4B30`) | `0x1EC0F0..0x1EC4A0` (`0x3B0`) |
| 1 | Sunset Rock | `0x0EC9A0..0x0F6160` (`0x97C0`) | `0x171480..0x17D220` (`0xBDA0`) | `0x222500..0x2274A0` (`0x4FA0`) | `0x1EC4A0..0x1EC810` (`0x370`) |
| 2 | Night Highway | `0x0F6160..0x0FEDA0` (`0x8C40`) | `0x17D220..0x186910` (`0x96F0`) | `0x2274A0..0x22AE50` (`0x39B0`) | `0x1EC810..0x1ECD00` (`0x4F0`) |
| 3 | Grass Valley | `0x0FEDA0..0x10AAB0` (`0xBD10`) | `0x186910..0x197490` (`0x10B80`) | `0x22AE50..0x2308E0` (`0x5A90`) | `0x1ECD00..0x1ED910` (`0xC10`) |
| 4 | Dizzy-Land | `0x10AAB0..0x118670` (`0xDBC0`) | `0x197490..0x1A5620` (`0xE190`) | `0x2308E0..0x2349E0` (`0x4100`) | `0x1ED910..0x1EDC80` (`0x370`) |
| 5 | Quicksand Valley | `0x118670..0x125C90` (`0xD620`) | `0x1A5620..0x1B3160` (`0xDB40`) | `0x2349E0..0x23A7D0` (`0x5DF0`) | `0x1EDC80..0x1EE060` (`0x3E0`) |
| 6 | Silver Mountain | `0x125C90..0x131A90` (`0xBE00`) | `0x1B3160..0x1C24B0` (`0xF350`) | `0x23A7D0..0x23E9D0` (`0x4200`) | `0x1EE060..0x1EEEA0` (`0xE40`) |
| 7 | Animal Land | `0x131A90..0x1384B0` (`0x6A20`) | `0x1C24B0..0x1C9630` (`0x7180`) | `0x23E9D0..0x23FCC0` (`0x12F0`) | None |
| 8 | Ninja Land | `0x1384B0..0x13F3B0` (`0x6F00`) | `0x1C9630..0x1D3070` (`0x9A40`) | `0x23FCC0..0x241160` (`0x14A0`) | `0x1EEEA0..0x1EF1D0` (`0x330`) |
| 9 | Rookie Mountain | `0x13F3B0..0x145380` (`0x5FD0`) | `0x1D3070..0x1D82B0` (`0x5240`) | `0x241160..0x2427D0` (`0x1670`) | `0x1EF1D0..0x1EF530` (`0x360`) |

`loadRaceCourseAssets` loads these assets into handles 8, 9, `0x1B`, and
`0x1D`. Animal Land omits handle `0x1D`. Other race assets following these
ranges are shared race, character, UI, or effect resources and should not be
treated as additional course environments merely because race code loads them.

## Graphics and model resources

All ten segment 2 graphics bundles are now source-backed. They are divided into
71 rebuildable ranges under `assets/course_display_lists`: one complete leading
graphics range per course and 61 main, backdrop, effect, and runtime-identified
auxiliary ranges. Each is represented as exact `Gfx` source words with an F3DEX
macro decode included as a reference. The source words remain authoritative
because a few ranges contain embedded resource data, and some otherwise valid
commands contain bits that a macro disassembler normalizes.

Confirmed top-level entry points are:

| Course | Main course display list | Backdrop display list(s) |
| --- | --- | --- |
| Big Snowman | `BIG_SNOWMAN_COURSE_DISPLAY_LIST` | `BIG_SNOWMAN_COURSE_BACKDROP_DISPLAY_LIST`, `BIG_SNOWMAN_COURSE_SECONDARY_BACKDROP_DISPLAY_LIST` |
| Sunset Rock | `SUNSET_ROCK_COURSE_DISPLAY_LIST` | `SUNSET_ROCK_COURSE_BACKDROP_DISPLAY_LIST` |
| Night Highway | `NIGHT_HIGHWAY_COURSE_DISPLAY_LIST` | `NIGHT_HIGHWAY_COURSE_BACKDROP_DISPLAY_LIST` |
| Grass Valley | `GRASS_VALLEY_COURSE_DISPLAY_LIST` | `GRASS_VALLEY_COURSE_BACKDROP_DISPLAY_LIST` |
| Dizzy-Land | `DIZZY_LAND_COURSE_DISPLAY_LIST` | `DIZZY_LAND_COURSE_BACKDROP_DISPLAY_LIST` |
| Quicksand Valley | `QUICKSAND_VALLEY_COURSE_DISPLAY_LIST` | `QUICKSAND_VALLEY_COURSE_BACKDROP_DISPLAY_LIST`, `QUICKSAND_VALLEY_COURSE_SECONDARY_BACKDROP_DISPLAY_LIST` |
| Silver Mountain | `SILVER_MOUNTAIN_COURSE_DISPLAY_LIST` | `SILVER_MOUNTAIN_COURSE_BACKDROP_DISPLAY_LIST` |
| Animal Land | `ANIMAL_LAND_COURSE_DISPLAY_LIST` | `ANIMAL_LAND_COURSE_BACKDROP_DISPLAY_LIST` |
| Ninja Land | `NINJA_LAND_COURSE_DISPLAY_LIST` | `NINJA_LAND_COURSE_BACKDROP_DISPLAY_LIST` |
| Rookie Mountain | `ROOKIE_MOUNTAIN_COURSE_DISPLAY_LIST` | `ROOKIE_MOUNTAIN_COURSE_BACKDROP_DISPLAY_LIST` |

Dizzy-Land's directly referenced course effects are also named: the three-part
particle, bumper, trailing-particle pair, and spinning-object pair. The runtime
table `gRaceCourseRankModelDisplayLists` now identifies the two lap-state lists
used by each course's finish-line rank model. Their containing ranges retain the
conservative `COURSE_AUXILIARY_DISPLAY_LISTS` name where they also contain
unrelated effects or entry points.

| Course | Active rank-model display lists |
| --- | --- |
| Big Snowman | `0x02009200` (surface 1), `0x02008E40`, `0x02009020` (finish line) |
| Sunset Rock | `0x020091E0`, `0x020093E8` |
| Night Highway | None; scene setup does not create this model |
| Grass Valley | `0x0200B6F0`, `0x0200B880` |
| Dizzy-Land | `0x0200D448`, `0x0200D5E0` |
| Quicksand Valley | `0x0200C5A0`, `0x0200C760` |
| Silver Mountain | `0x0200BAD8`, `0x0200BC68` |
| Animal Land | None; the Trick Game does not create this model |
| Ninja Land | `0x02006BA0`, `0x02006D50` |
| Rookie Mountain | `0x02005BA8`, `0x02005DB8` |

Each two-list pair is now split into `RANK_MODEL_LAPS_REMAINING` and
`RANK_MODEL_FINAL_LAP` assets, matching the runtime lap-state selection. The
otherwise anonymous course slide table is named `gRaceCourseSlideSpriteInit`;
its eight display-list roots are split into two Sunset Rock assets, one Night
Highway asset, two Dizzy-Land assets, two Quicksand Valley assets, and one
Animal Land asset. The two display lists rendered by course trigger volumes are
also named for Sunset Rock and Animal Land.

Every confirmed top-level entry point was recursively traced through segment 2
`G_DL` commands. The extractor also scans every aligned source word for valid,
in-bounds resource commands, covering packed display lists whose entry points
have not yet been given semantic names. Their `G_VTX`, `G_SETTIMG`,
`G_LOADBLOCK`, and `G_LOADTLUT` commands address the compressed segment 3
model-resource bundle. Resources are extracted as editable N64 `Vtx` records,
CI4 textures, and RGBA16 palettes. Texture storage dimensions come from the
load-block byte count and render-tile stride; `G_SETTILESIZE` is not used as an
allocation size because courses frequently use it to repeat a smaller stored
texture.

All ten model-resource manifests now have complete structured byte coverage and
no anonymous raw parts. One eight-vertex block in Big Snowman and three
16-color palettes (one in Silver Mountain and two in Rookie Mountain) are not
referenced by any source-backed command; they remain editable structured parts
with `referenced: false`. Exact Huffman tables, bit padding, and LZ token choices
are preserved, so unchanged manifests reproduce the original compressed stream.

| Course | Display lists | Root-traced vertex loads | All packed vertex loads | Vertices | CI4 textures | RGBA16 palettes |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Big Snowman | 24 | 175 | 179 | 4,604 | 39 | 21 |
| Sunset Rock | 37 | 166 | 176 | 3,872 | 37 | 16 |
| Night Highway | 40 | 162 | 166 | 3,532 | 44 | 15 |
| Grass Valley | 23 | 228 | 230 | 6,180 | 70 | 19 |
| Dizzy-Land | 66 | 252 | 268 | 5,628 | 63 | 18 |
| Quicksand Valley | 56 | 233 | 262 | 5,616 | 52 | 14 |
| Silver Mountain | 29 | 236 | 238 | 6,115 | 66 | 24 |
| Animal Land | 30 | 131 | 135 | 3,032 | 46 | 12 |
| Ninja Land | 21 | 124 | 126 | 3,037 | 67 | 16 |
| Rookie Mountain | 21 | 85 | 87 | 1,896 | 26 | 11 |

This accounts for 347 recursively traced display lists, 1,867 packed vertex-load
commands covering 43,512 vertex records, 510 textures, and 166 palettes across
all ten environments. The asset is still called model resources, rather than
textures, because the runtime treats it as a complete RSP segment containing
all of those resource classes.

## Course sprite-table layout

The nine optional course sprite assets use the same `AssetTable` format as the
menu and race UI renderers:

```text
u32 unknown_0
u32 entry_count
CourseSpriteEntry entries[entry_count] // image offset, palette index, width, height
u16 rgba16_palettes[][16]
u8 ci4_images[]
```

Every table parses with complete byte coverage. Animal Land has no table because
the Trick Game does not load one.

| Course | Entries | Palettes | CI4 images | Decompressed size |
| --- | ---: | ---: | ---: | ---: |
| Big Snowman | 2 | 1 | 2 | `0x758` |
| Sunset Rock | 2 | 1 | 2 | `0x758` |
| Night Highway | 5 | 2 | 5 | `0xD90` |
| Grass Valley | 8 | 5 | 8 | `0x1288` |
| Dizzy-Land | 2 | 1 | 2 | `0x758` |
| Quicksand Valley | 2 | 1 | 2 | `0x758` |
| Silver Mountain | 8 | 5 | 8 | `0x1808` |
| Ninja Land | 2 | 1 | 2 | `0x758` |
| Rookie Mountain | 2 | 1 | 2 | `0x758` |

## Surface-data layout

After decompression, surface data is stored as consecutive native big-endian
tables:

```text
u16 coordinate_count
Vec3s coordinates[coordinate_count]

u16 face_count
RaceCourseSurfaceFace faces[face_count] // 8 bytes per face

u16 surface_count
RaceCourseSurface surfaces[surface_count] // 0x1C bytes per surface
```

Each face contains three coordinate indices, the gameplay surface type, and a
boolean that skips the first triangle-edge containment test.

Each `RaceCourseSurface` contains four neighboring surface indices, four
boundary coordinate indices, a reference coordinate, the path heading angle,
an inclusive/exclusive range into the face table, one unknown value at offset
`0x18`, and endpoint-clamping flags at offset `0x1A`. This is the predecessor of
Snowboard Kids 2's extracted `track_sector_mesh` format.

All ten assets parse completely, and their declared surface counts agree with
the runtime course metadata:

| Course | Coordinates | Faces | Surfaces |
| --- | ---: | ---: | ---: |
| Big Snowman | 1,283 | 1,349 | 182 |
| Sunset Rock | 1,361 | 1,522 | 160 |
| Night Highway | 1,004 | 841 | 197 |
| Grass Valley | 1,547 | 1,280 | 293 |
| Dizzy-Land | 1,128 | 951 | 212 |
| Quicksand Valley | 1,532 | 1,327 | 328 |
| Silver Mountain | 1,144 | 840 | 253 |
| Animal Land | 372 | 480 | 40 |
| Ninja Land | 381 | 321 | 68 |
| Rookie Mountain | 422 | 388 | 67 |

The editable YAML manifests use these same names: `surface_type`,
`skip_first_edge_check`, `neighbor_indices`, `boundary_coordinate_indices`,
`reference_coordinate_index`, `path_angle`, `face_start_index`,
`face_end_index`, and `edge_clamp_flags`. The word at offset `0x18` is preserved
as `unused_18`: no runtime code reads it, so the name documents the evidence
without assigning an unsupported gameplay meaning.

The remaining fields in `RaceCourseStartEntry` are now named from their runtime
uses:

| Field | Runtime role |
| --- | --- |
| `spiralExitSurfaceIndex` | Surface assigned to a player leaving the spiral course object |
| `spiralOrigin` | Shared world-space origin for that object and its player exit |
| `launchRampTargetX`, `launchRampTargetZ` | Trigger center and alignment target for the launch-ramp sequence |
| `launchRampOrigin` | World-space origin used to spawn the animated launch-ramp object |
| `positiveLookaheadSurfaceStartIndex`, `positiveLookaheadSurfaceEndIndex` | Inclusive surface range using positive rather than negative path lookahead |
| `startSurfaceTargetX`, `startSurfaceTargetZ` | Steering and progress target used while on the start surface |

## Remaining documentation work

- Give semantic names to individual nested display lists as their runtime or
  visual roles are established.
