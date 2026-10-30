---
name: match-ultralib-segment
description: Match a given segment to a libultra ROM segment. Use this when converting unidentified asm libultra ranges such as osRecvMesg to matching C source.
---

# Match an Ultralib Segment

## Core Rules

- Match libultra by compiling local source under `src/ultra`; do not patch linker scripts, generated linker scripts, or archive-member references.
- Preserve the upstream libultra subdirectory when possible, for example `../ultralib/src/io/pfsnumfiles.c` becomes `src/ultra/io/pfsnumfiles.c`.
- Keep unidentified neighboring libultra functions as separate `asm` segments until each function is matched as source.
- Source under `src/ultra` should use real libultra names, not `func_...` or `D_...` placeholders.
- Add required callable ROM symbols to `symbol_addrs.txt` with runtime VRAM addresses, not `0x700...` splat placeholder addresses. Never add libultra function aliases to `linker_scripts/libultra_syms.ld`.
- If a libultra source file emits `.data` or `.bss`, match those sections in `snowboardkids.yaml`; do not remove the C definitions, make them `extern`, allow multiple definitions, add linker-script aliases, or delete generated linker entries.
- When asm references a field inside a known libultra struct, annotate the owning struct symbol in `symbol_addrs.txt` with its real type and size. Do not add a linker-script alias for the field address.

## Workflow

1. Inspect the target function before trusting upstream source. Version-specific libultra code can differ from the checked-in upstream copy:

   ```sh
   python3 tools/asm-differ/diff.py --no-pager osPfsNumFiles
   ```

   Watch for local deltas such as extra initialization loops, literal stores, or changed command/status values.

2. Find the upstream source and headers:

   ```sh
   rg -n "osPfsNumFiles" ../ultralib/src ../ultralib/include
   ```

3. Copy or port the upstream file into `src/ultra`, preserving the source subtree. Verify macro values in local headers before using semantic-looking names; for example `CONT_CMD_NOP` may be `0xff`, while the target may want a literal `0`. Treat the upstream as a starting point — the compiled ROM may have been built from a different version of the source. Build early and let the diff guide iterations rather than analyzing the assembly exhaustively beforehand.

   ```text
   ../ultralib/src/os/recvmesg.c -> src/ultra/os/recvmesg.c
   ../ultralib/src/io/pfsnumfiles.c -> src/ultra/io/pfsnumfiles.c
   ```

4. Replace the matching ROM range in `snowboardkids.yaml` with a normal C segment:

   ```yaml
   - [0xA1590, c, ultra/os/recvmesg]
   - [0xA2650, c, ultra/io/pfsnumfiles]
   ```

5. If the new source object emits `.data` or `.bss`, match those sections before trying linker workarounds. First inspect the object:

   ```sh
   mips-linux-gnu-objdump -h build/src/ultra/os/initialize.o
   mips-linux-gnu-nm -n build/src/ultra/os/initialize.o
   ```

   Then split the existing raw data/BSS ranges in `snowboardkids.yaml` so the C file owns exactly the emitted data. Keep the definitions in C when they are part of the upstream file; changing them to `extern` can alter IDO codegen even when it looks semantically equivalent.

   Example for `ultra/os/initialize`, whose `.data` lives inside `B1C00.data.s` and whose `.bss` lives inside the main BSS block:

   ```yaml
   - [0xB1C00, data]
   - [0xDFED0, .data, ultra/os/initialize]
   - [0xDFEF0, data]
   ...
   - { type: bss, name: bss_chunk_a, vram: 0x800E29C0 }
   - { type: .bss, name: ultra/os/initialize, vram: 0x8015CCD0 }
   - { type: bss, name: bss_chunk_b, vram: 0x8015CCE0 }
   ```

   For `initialize.c`, the matched `.data` range is `0xDFED0..0xDFEF0` and contains `osClockRate`, `osViClock`, `__osShutdown`, and `__OSGlobalIntMask`. `__osFinalrom` is BSS at `0x8015CCD0..0x8015CCE0`. The raw ranges before and after remain raw segments; give split BSS chunks stable names so Splat does not collapse or mis-name them.

6. If the new source calls functions or references data already present elsewhere in the ROM, add the real ultralib symbol names to `symbol_addrs.txt`. Avoid using aliases to make the work with unlabeled symbols (those starting with D_ or func_). **Important:** symbols with `0x700...` addresses are splat placeholders and must be converted to runtime `0x800...` VRAM addresses before the linker can resolve them. A linker-script alias such as `osSendMesg = 0x800A0D30;` in `linker_scripts/libultra_syms.ld` is the wrong fix; update `symbol_addrs.txt`, the real asm/C labels, and call sites instead. Scan for any `0x700A` entries that the new source calls:

   ```text
   __osDisableInt = 0x800A61B0; // type:func
   __osRestoreInt = 0x800A61D0; // type:func
   __osContPifRam = 0x8015CA00;
   ```

   Before moving on, verify that no libultra function alias was added to the linker scripts, and that the matched/called functions use real names instead of address-derived placeholders:

   ```sh
   rg -n "= 0x800A" linker_scripts
   rg -n "func_800A[0-9A-Fa-f]+" symbol_addrs.txt src asm
   ```

   If the second command finds unrelated existing placeholders, do not rename them blindly. Fix only the symbols involved in the segment being matched.

7. If asm references an address inside a known libultra struct, fix the owning symbol metadata instead of creating a standalone `D_...` symbol. The desired output is an addend reference such as `%hi(__osPiDevMgr + 0x8)`, not a generated field label and not a linker-script alias.

   Example: `0x800DF2F8` is `__osPiDevMgr.cmdQueue`, where `__osPiDevMgr` starts at `0x800DF2F0` and `OSDevMgr.cmdQueue` is at offset `0x8`. Annotate the struct:

   ```text
   __osPiDevMgr = 0x800DF2F0; // type:OSDevMgr size:0x1C
   ```

   Then regenerate/extract as needed and verify the asm uses the base symbol plus addend:

   ```sh
   rg -n "D_800DF2F8|__osPiDevMgr \\+ 0x8" asm symbol_addrs.txt linker_scripts
   ```

   Do not add this to `linker_scripts/libultra_syms.ld`:

   ```ld
   D_800DF2F8 = 0x800DF2F8;
   ```

   The struct size should come from the header layout and the observed next symbol or BSS/data range. For `OSDevMgr`, the fields are seven 4-byte words, so the size is `0x1C`.

8. Build and verify:

   ```sh
   ./tools/build-and-verify.sh
   ```

9. If verification fails, diff every function emitted by the new source before looking for linker or data issues:

   ```sh
   python3 tools/asm-differ/diff.py --no-pager osPfsNumFiles
   ```

10. Document Learnings:

Update the DECOMPILATION_LEARNINGS.md file in the root of the project with any additional learnings that you gathered as part of matching this segment. Only add **generic, broadly applicable lessons** — things that would help when matching a *different* function in the future. Do not add per-file details such as specific VRAM/ROM addresses, individual symbol names, exact segment split locations, or step-by-step accounts of what happened for one particular function. Ask: "would this lesson help me match an unrelated ultra function tomorrow?" If not, leave it out.

## Build Flags

The game code uses -O2 but ultralib specifically should use -O1 instead. The build version can also affect libultra behavior, so check the compile line for both `-O1` and the expected `BUILD_VERSION` such as `BUILD_VERSION=VERSION_I`. You can use the following pattern to override the flags:

```make
$(BUILD_DIR)/src/ultra/io/%.o: C_OPT = -O1
```

## VERSION_I Specifics

This project uses `BUILD_VERSION=VERSION_I`. When porting upstream ultralib source, apply these adjustments:

- `__osPfsSelectBank` takes 1 argument (`OSPfs*`), not 2. The `SELECT_BANK` macro sets `pfs->activebank` then calls `__osPfsSelectBank(pfs)`.
- No `__osPfsCheckRamArea` function — skip any `BUILD_VERSION >= VERSION_J` blocks that call it.
- Uses `for` loops instead of `bcopy` for memory copies.
- No `PFS_ID_BROKEN` status flag handling.
- The upstream source may contain calls that were not present in the actual VERSION_I build. Trust the assembly over the upstream — if a call appears in upstream but not in the target assembly, omit it.

## Incorrect Segments

Be mindful that you may need to adjust the start/end points of segments as well as the segment name to match it to the correct ultralib c file.

## Data, Rodata, and BSS Matching

Ultralib segments may contain data, rodata, and/or bss which must be similarly matched. These are likely unlabelled. You will need to find the appropriate data/rodata/bss segment and link it back to your matched ultralib segment, e.g:

```
- [0xDFED0, .data, ultra/os/initialize]
- [0xE15B0, .rodata, myUltraLibSegment]
- { type: .bss, name: ultra/os/initialize, vram: 0x8015CCD0 }
```

First check whether the compiled object actually emits these sections:

```sh
mips-linux-gnu-objdump -h build/src/ultra/io/pfsnumfiles.o
```

If the object only has `.text` plus compiler metadata such as `.options` and `.reginfo`, continue matching instruction generation before adding data or rodata segments.

If the object emits data/BSS that already exists in a broad raw segment, split the broad segment around the emitted range. Do not use linker scripts to alias the symbols or Makefile commands to strip sections out of the generated linker script. Keep the C definitions and let the corresponding `.data`/`.bss` YAML entries own those sections.

You can use the match-data-file skill to help match  the correct segment. Note that it might not be properly annotated (yet) in snowboardkids.yaml.

## Avoid

- Do not mutate linker scripts with `perl -0pi`, `perl -pe`, or similar build hacks.
- Do not edit the Makefile to `sed` generated linker scripts or add `--allow-multiple-definition` to hide duplicate data.
- Do not add libultra function aliases to `linker_scripts/libultra_syms.ld`; define the symbol in `symbol_addrs.txt` at its runtime `0x800...` address and rename the real labels/references.
- Do not add libultra data aliases to `linker_scripts/libultra_syms.ld`; match the data/BSS section in `snowboardkids.yaml`.
- Do not add linker-script aliases for struct field addresses such as `D_800DF2F8`. Annotate the owning symbol with `type:` and `size:` in `symbol_addrs.txt` so asm can reference `symbol + offset`.
- Do not change real libultra data definitions to `extern` just to avoid duplicate storage; it can change IDO codegen. Match the emitted data/BSS segments instead.
- Do not replace archive-member linker entries by text substitution.
- Do not link `libultra_rom.a` just to extract one matched object.
- Do not add a custom `symbol_addrs.ld`; use `symbol_addrs.txt`.
- Do not disable `undefined_funcs_auto.txt` to keep a local libultra source dependency unresolved.
- Do not leave `func_...` or `D_...` references in `src/ultra`.
