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

- `LEVEL_<COURSE>` is an uncompressed RSP segment 2 graphics bundle. It contains
  vertices, display lists, and other display-list resources. Despite the current
  `LEVEL_` prefix, it is not the entire level.
- `LEVEL_<COURSE>_TEXTURES` is a compressed RSP segment 3 texture bundle.
- `LEVEL_<COURSE>_SURFACE_DATA` is compressed collision and course-path data.
- `LEVEL_<COURSE>_SPRITES` is an optional compressed course-specific sprite
  table. Animal Land does not load or have one.

The ranges below are half-open ROM ranges (`start` through, but not including,
`end`). Sizes are compressed sizes for assets loaded through `LOAD_ASSET`.

| Id | Course | Segment 2 graphics | Textures | Surface data | Sprites |
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

## Graphics bundle entry points

The segment 2 graphics bundles currently have address-derived subsegment names.
Confirmed top-level entry points are:

| Course | Main course display list | Backdrop display list(s) |
| --- | --- | --- |
| Big Snowman | `_EBEC0` | `_EBFA0`, `_EC310` |
| Sunset Rock | `_F57D0` | `_F5920` |
| Night Highway | `_FE788` | `_FE8F0` |
| Grass Valley | `_10A1A0` | `_10A280` |
| Dizzy-Land | `_1167F8` | `_1169F0` |
| Quicksand Valley | `_1246D0` | `_1248A8`, `_124E18` |
| Silver Mountain | `_131448` | `_131558` |
| Animal Land | `_137EC0` | `_137FD8` |
| Ninja Land | `_13EC60` | `_13ED30` |
| Rookie Mountain | `_144B88` | `_144C58` |

Dizzy-Land also contains several display lists used by course-specific race
effects. The remaining address-derived splits can contain nested display lists,
vertices, or other graphics resources. They should be renamed only after their
contents and callers are identified; being unreferenced directly from C does not
mean a split is unused by another display list.

## Surface-data layout

After decompression, surface data is stored as consecutive native big-endian
tables:

```text
u16 coordinate_count
Vec3s coordinates[coordinate_count]

u16 face_count
RaceMotionFace faces[face_count]       // 8 bytes per face

RaceMotionSurface surfaces[]          // 0x1C bytes per surface

u16 final_value
```

Each `RaceMotionSurface` contains four neighboring surface indices, four boundary
coordinate indices, a path/position index, an angle, and four additional 16-bit
properties. The asset size supplies the surface count after accounting for the
trailing, not-yet-understood `final_value`. This is the predecessor of Snowboard
Kids 2's extracted `track_sector_mesh` format.

## Remaining documentation work

- Add structured extractors for the segment 2 graphics, texture, and sprite
  assets, comparable to Snowboard Kids 2's course asset types.
- Identify and rename nested segment 2 display-list/resource splits.
- Decode the compressed texture and sprite-table formats.
- Name the unknown fields in `RaceMotionFace`, `RaceMotionSurface`, and
  `RaceCourseStartEntry` from their runtime uses.
