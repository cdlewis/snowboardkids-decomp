# Readable asset extraction

`make extract` now decodes every top-level asset segment in the supported US ROM.
It generates editable, semantically grouped N64 asset bundles and self-contained
Blender-importable GLB previews, using Splat extensions and exact packers. The final ROM fill region
remains padding. Game code and assets already defined in C retain their existing
source definitions.

Splat segments declare their actual type directly: `sprite_table`, `tilemap`,
`race_animation`, `scene_animation`, `embedded_model`, `replay`, `pointer_bank`,
`sample_bank`, or `music_sequence`. These use thin format-specific adapters over
shared extraction code; there is no `asset_format` selector in the ROM config.
Existing bundle layouts migrate their decoder names automatically without moving
editable files or changing linker object identities.

```sh
make extract
make verify-assets
make asset-report
make asset-previews
./tools/build-and-verify.sh
```

`make extract` requires the ROM with the SHA1 in `snowboardkids.yaml`. On first
extraction it creates the bundles. Subsequent extraction preserves existing bundle
sources and refreshes their GLBs. `make clean` now preserves **all assets**; it
only removes build/assembly artifacts. Build edits with `make`; intentional edits
will naturally change the original-ROM checksum. GLBs are never ROM build inputs.

All generated Nintendo data remains ignored under `assets/` and `build/`.
No sibling SK2 checkout is required to extract or rebuild.

## Output locations

| Location | Contents |
| --- | --- |
| `assets/index.yaml` | Searchable bundle catalog, models, previews, and animation counts |
| `assets/items/<name>/` | All 11 gameplay items: icons, sprite/effect references, and pan/fan geometry with GLBs |
| `assets/characters/{slash,wendy,jam,linda,tommy,ninja}/` | Character geometry/textures, race animations, normal/ghost GLBs and scene-animation GLBs |
| `assets/courses/<course>/` | One course manifest, assembled `preview.glb`, geometry, textures, sprites, collision, paths and props |
| `assets/models/snowboards/` | Snowboard geometry and GLBs including external texture variants |
| `assets/models/unidentified/` | Other models with stable address-based identities |
| `assets/sprites/`, `assets/tilemaps/`, `assets/animations/`, `assets/replays/` | Family-specific bundles; unconfirmed font/image identities retain sprite IDs |
| `assets/audio/`, `assets/shared/audio_samples/` | Music, pointer banks, sample mappings, shared VADPCM and WAV files |
| `assets/shared/` | Resources used by multiple bundles and references to source-resident assets |
| `assets/text/`, `assets/models/source/` | C-authoritative text manifests and the source-resident fog-panel model |
| `assets/layout/` | ROM segment placement, compression metadata and generated scatter/build rules |
| `assets/source_views/` | Generated inspection views referenced by C-authoritative bundles |
| `build/asset-report.json` | ROM ranges, decoders, consumers, outputs, coverage, and optional reconstruction results |
| `build/model-preview-report.json` | Per-model GLB export success/errors, triangle counts, and animation counts |

Extraction writes intermediate records to `build/asset-staging/`. The old
`assets/course_*` and `assets/readable/` directories are retired during migration;
the original tree is recoverable under `asset-backups/semantic-*/assets/`.

### Bundles and Blender

Open `assets/items/pan/pan.glb` or `assets/courses/rookie_mountain/preview.glb`
directly with Blender's glTF importer.
Textures are embedded; copying the GLB alone is sufficient. For characters, open
`normal.glb` or `ghost.glb`; `scene_215be0.glb` and `scene_5e34a0.glb` expose the two
scene animation banks. Clips use stored frame order at a reference 60 Hz; runtime
playback speed, interpolation, and gameplay effects can change their timing.

Course previews assemble terrain, backdrop, effects and placed scenery. Hide the
backdrop node in Blender to inspect terrain from outside the enclosing sky shell.
Scenery placement and procedural item behavior remain C-authoritative; manifests
identify their source tables/renderers. Public placement values control the
inspection preview, not C-to-ROM gameplay edits.

Each `asset.yaml` identifies its previews, owned files and semantic components.
ROM-shaped records and address bindings live only under `assets/layout/`.
Geometry and textures used only by the pan are stored beside
it, while shared resources have one canonical owner. Palette words and vertices
are editable YAML; display lists retain exact command words. Original segment
offsets are retained where needed to resolve N64 pointers. Layout manifests scatter
these sources back into the original ROM sections without changing linker symbols.

GLBs are **viewing/export artifacts**, not Blender-to-ROM round-trip inputs.
Use `make asset-previews` after editing N64 sources. Export errors are fatal and
recorded in the preview report. Lighting, fog and N64 combiner effects are
approximations; actual draw-time palette bindings, baked vertex colors, and UV
modifications are decoded. Scrolling surfaces export their initial texture phase.
Runtime sprite/effect geometry is indexed with its source renderer instead of
being misrepresented as a complete standalone model.

### Migrating an existing extraction

```sh
make migrate-assets
make asset-previews
make verify-assets
```

Migration validates all 254 packed sections against the pre-migration canonical
bytes before swapping trees. It preserves canonical edits and refuses conflicting
legacy edits or destinations. Repeated extraction retains canonical sources.
`tools/semantic_assets.py --check` validates the public structure. The old
`index-models` command is intentionally disabled for the semantic catalog.

Pack an individual bundled ROM section with:

```sh
python3 tools/asset_bundles.py pack _1D82B0 --out /tmp/shared-race.bin
```

Stable ROM-address names are retained for assets without established semantic
names. The report connects them to their loaders. For example, `_14B450` is the
snowboard bundle, `_245A80` contains race UI sprites including CI8 portraits,
`_5DCBE0` is the title tilemap, and `_275A90` is the Mus pointer bank.

## Editing and rebuilding

Bundle/layout manifests use `schema_version: 2`. Internal records retain their
decoder-specific schemas. A texture has either an inline `data` field or a PNG
`path`, never both.
Image paths are relative to the manifest. Packers emit per-object `.d` files so
image, display-list, replay, and sample edits trigger the correct rebuild,
including when a manifest changes its source paths.

```sh
python3 tools/asset_bundles.py pack _245A80 --out /tmp/ui.bin
python3 tools/asset_bundles.py pack BIG_SNOWMAN_COURSE_MODEL_RESOURCES --out /tmp/course.bin
```

- PNGs must remain indexed and keep their declared dimensions. Pixel **indices**
  are authoritative; exact RGBA5551 palette words live in the manifest. Changing
  only a PNG's display palette does not change the game's palette. This preserves
  duplicate colors, transparent entries, shared palettes, and unused nibbles.
- Shared image offsets have one editable image source. Sprite entries with a
  different palette get separate inspection previews. `preview` files are not
  packer inputs.
- Model texture previews record `palette_candidates` and
  `preview_palette_offset`. A reused TMEM slot can have several candidate
  palettes; the default PNG palette is an inspection aid, not a claim about a
  unique rendered material. Display lists retain the actual palette selection.
- Tilemaps preserve their tile definitions, grid, palette slots, flips, padding,
  and one-based image indices. `preview.png` assembles the full grid. Tile zero
  is empty; unused tile records are retained.
- Geometry remains N64 display lists and vertices, with original offsets and
  rendering state. The snowboard bundle separates embedded vertices, textures,
  and command streams. Command-word C sources retain bits that macro
  disassembly can normalize. GLB previews interpret these exact sources without
  participating in their rebuild.
- Race animation frames expose two translation triples and twelve packed joint
  rotations. Rotations are 0..4088 in steps of 8; unused packed bits are retained.
  Menu/scene animations contain fourteen translation and rotation triples.
  Offset-table aliases and frame counts remain exact.
- Replay CSVs retain four players × 2,400 frames, with button bits and signed
  stick coordinates. Row order and counts are validated.
- Music manifests expose named Mus commands, notes, envelopes, and volume/pitch
  streams. Operands and variable-length duration encodings retain their original
  bytes. Loop and jump commands are listings, not an offline music synthesizer.
- The pointer bank exposes wave records, ADPCM codebooks, and loop states.
  Original `.vadpcm` samples are authoritative; WAVs are inspection previews at
  a reference 22,050 Hz. The runtime applies pitch separately. A sample crossing
  the original ROM boundary has one shared encoded file, referenced by slices
  from both segment manifests. WAV re-encoding is not used for matching builds.
- C-resident assets remain editable in their indexed `source` files. Generated
  text and sound-effect views are inspection outputs; edit the original C text
  or data definitions to change the ROM. Text decoding preserves control words,
  embedded terminators, and unknown glyphs through reversible escapes:

```sh
python3 tools/textconv.py tools/charmap.txt input.bin output.txt --decode
```

Compression retains Huffman tables, LZ token choices, padding bits, and unused
tails. Structural edits that change encoded size do not automatically relocate
other assets; the original ROM layout remains the matching target.

## Coverage and validation

The initial converted US inventory has 254 readable top-level assets and one
trailing fill region. This counts ROM segments, not individual images or models.
Source-resident declarations have a separate index; procedural graphics created
at runtime are not separate ROM assets. `readable` means structurally decoded,
not that every field or visual identity has a semantic name.

Nonzero unclassified parts are reported as `partial`; preserving their bytes
does not count as readable extraction. Known zero padding is reported separately
from unknown data. `make verify-assets` reconstructs each registered asset and
compares it with its original ROM slice, including compression tails. It exits
nonzero on missing sources, invalid packing, or mismatches.

Run the focused tests with:

```sh
PYTHONPATH=tools:. python3 -m unittest tools.test_readable_assets \
  tools.test_asset_bundles \
  tools.test_course_graphics tools.test_course_sprite_table \
  tools.test_course_surface_data tools.test_course_display_list \
  tools.test_huffman_asset tools.test_textconv \
  tools.test_course_definitions tools.test_sk1_to_sk2_course
```

The tests cover palette/index preservation, shared image edits, mixed CI4/CI8,
animation packing, tilemap addressing, ADPCM prediction, music loop boundaries,
malformed inputs, and reversible text. Full ROM verification remains the final
matching check. Existing course-to-SK2 binary export continues to read the
original ROM directly.

Optional real Blender validation (Blender is not needed for extraction/builds):

```sh
blender --background --python tools/check_model_previews_blender.py -- \
  --render-dir build/model-preview-checks
```

This imports every GLB in the export report, checks model/animation presence, and
renders the pan, a character, a snowboard, and a course for visual inspection.
