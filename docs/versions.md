# Multi-version development

Select the region with `VERSION=us|jp`; US is the default. Shared sources live in
`src`, regional sources in `src/us` and `src/jp`, and shared headers in
`include`. Builds define
`VERSION_US` or `VERSION_JP` respectively.

## Build and verify

```bash
./tools/build-and-verify.sh
./tools/build-and-verify.sh --version jp
```

Each command cleans, extracts, builds, and verifies the selected ROM against
its original SHA1 checksum. The regions have separate build outputs.

## Regional paths

| Purpose | US | JP |
| --- | --- | --- |
| Splat config | `snowboardkids.yaml` | `snowboardkids.jp.yaml` |
| Expected checksum | `snowboardkids.sha1` | `snowboardkids.jp.sha1` |
| Symbols | `symbol_addrs.txt` | `symbol_addrs.jp.txt` |
| Relocations | `reloc_addrs.txt` | `reloc_addrs.jp.txt` |
| Generated assembly | `asm/` | `asm.jp/` |
| Extracted assets | `assets/` | `assets.jp/` |
| Shared source root | `src/` | `src/` |
| Unmigrated regional sources | `src/us/` | `src/jp/` |
| Build directory | `build/` | `build/jp/` |
| Generated linker script | `snowboardkids.ld` | `snowboardkids.jp.ld` |
| Generated unresolved symbols | `undefined_*_auto.txt` | `undefined_*_auto.jp.txt` |

## Sharing sources

Shared sources include fixed-point and spatial math, player input, movement and
rumble, race motion and projectiles, several race effects and menu flows, and
the Jam, Linda, Nancy, and Slash credits. Their source contents are identical
across regions, so they need no regional conditionals. The race-start transition
also shares its sine-table include.

The identical libultra sources, assembly, and local headers also live in `src/`,
along with libmus's standalone sample and DMA sources. Libultra compiler and
assembler settings apply to both shared and regional objects. Differing library
sources and libmus fragments included by regional players remain regional.

Files that differed only in comments, whitespace, or declaration placement use
one shared form. Shared title-demo initialization retains a `VERSION_JP` branch
for its local `register` declaration to preserve the original register allocation.
Course-select BSS tables and player-property data retain their special assembler
rules, and the optional snowboard renderer builds from its shared source.

Identical source text can still depend on regional content: the title-demo data
file remains regional because it includes different player snapshot tables.
Spatial math shares its source while retaining a JP-only object padding trim.

Both Splat configurations use `src_path: src`. Shared files have ordinary
subsegments with no directory override:

```yaml
- [0x98680, c, math/fixed_point_math]
```

Unmigrated files keep their logical segment names and use `dir: us` or `dir: jp`:

```yaml
- { start: 0x9A2A0, type: c, name: engine/system_runtime, dir: us }
```

To migrate another file, move one copy out of its regional subfolder, remove the
other regional copy, and remove the directory overrides in both YAMLs. Keep each
region's existing ROM start. Associated `.data`, `.rodata`, and `.bss` subsegments
resolve through their C sibling; standalone data and handwritten assembly entries
also need their directory overrides removed when their sources become shared.

Objects remain separate: shared files build under `build/src/` for US and
`build/jp/src/` for JP. Unmigrated files build under `build/src/us/` and
`build/jp/src/jp/`, respectively. RSP microcode remains regional, including its
extracted binary paths under `assets/us/rsp/` and `assets.jp/jp/rsp/`.

Preserve any per-file compiler flags or section postprocessing when changing
object paths. Use `#if defined(VERSION_JP)` or `VERSION_US` for actual regional
differences, and verify both original ROM checksums after each migration batch.

## Matching functions and data

Use the selected region's YAML configuration, symbol file, and relocation file.
Keep addresses specific to that region.

Rebuild before comparing a function or data symbol:

```bash
python3 tools/asm-differ/diff.py --version jp --no-pager main
python3 tools/data-differ/data_diff.py --version jp --find-first-mismatch
```

Both differs also accept `VERSION=jp`. Single-symbol data comparisons need
`rom:0x... size:0x...` metadata in the region's symbol file.

Readable asset tools, progress reports, and shiftable builds currently support
US only.
