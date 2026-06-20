# Decompilation Learnings

Record project-specific compiler behavior, matching patterns, and verified struct or data-layout insights here as they are discovered.

## IDO Optimization Levels

- **Most ultra IO/OS files match at `-O1`.** Audio library files (`src/ultra/audio/`) and GU math files (`src/ultra/gu/`) often require `-O2`. Some complex audio files (`reverb.c`, `env.c`, `xprintf.c`, `xldtob.c`) require direct IDO `-O3` because the asm-processor rejects `-O3` due to function reordering.
- **IDO `-O3` reorders functions within an object.** The compiler may emit functions in a different order than the source file. When matching `-O3` objects, the ROM function order is the authoritative order, not the upstream source order. In practice IDO `-O3` emits functions in *reverse* source order for small files, so keeping the upstream source order (e.g. a `Pull`/`Param` pair with `Pull` first) yields the ROM order (`Param` first). If a near-perfect `-O2` match differs only by saved-register (`s0`-`s7`) vs temp-register-plus-spill choice, try `-O3` + `IDO_DIRECT`: `-O3` register allocation, not a source change, is usually the cause.
- **At `-O2`, IDO uses callee-saved registers more aggressively** and fills branch delay slots better than `-O1`. At `-O1`, the compiler uses stack slots for variables surviving across calls, producing larger frames.

## Local vs Extern Data Definitions

The IDO compiler generates significantly different code when global variables are defined locally in the same translation unit vs declared as `extern`:

- **Local definitions**: The compiler keeps base addresses in registers across multiple 32-bit stores for 64-bit assignments, producing optimal instruction scheduling.
- **Extern declarations**: The compiler reloads base addresses between stores, generating extra `lui` instructions and missing delay slot optimization opportunities.

**Workaround for linker conflicts**: When a C file must define data locally (to match codegen), handle the resulting multiple-definition errors by:
1. Adding symbol addresses to `linker_scripts/libultra_syms.ld`.
2. Adding `--allow-multiple-definition` to `LDFLAGS`.
3. Stripping the object's `.data`/`.bss` entries from the generated linker script after extraction.

## Zero-Initialized Globals with IDO

With IDO, `static u8 var = 0` and `u32 var = 0` generate `.data` section entries, not `.bss`. This means matching ultra files with zero-initialized globals will emit both `.data` and `.bss` sections that need to be split from existing raw data/BSS segments.

## Symbol Addresses: 0x700... vs 0x800...

Symbols in `symbol_addrs.txt` with `0x700...` addresses are splat placeholders that must be converted to runtime `0x800...` VRAM addresses before the linker can resolve them. Always check for `0x700A` entries when adding ultra source that calls external functions. Verify the correct VRAM address by searching the asm files for the function label.

## Symbol Renames and Extraction

Changing names in `symbol_addrs.txt` causes splat to regenerate asm files with the new labels during `make extract`. Manual asm file edits are overwritten by extraction.

## C Text Alignment and Segment Splitting

- **C object `.text` sections pad to 16-byte alignment.** If a matched function's text is not 16-byte aligned, splitting at its exact end address shifts the following raw asm. Own a slightly larger aligned range instead.
- **When C text is smaller than the original asm range**, a new asm segment is needed to cover the remaining functions.
- **Rodata segments must be split at the correct ROM offset**, not based on symbol VRAM addresses. Use the linker map file to find the correct split points.

## `hasm` for Handwritten Assembly

For libultra functions that are handwritten assembly, use the `hasm` segment type in the YAML instead of `asm`. Upstream libc assembly can sometimes be reused directly as `hasm` without local macro rewrites. Use the project's `macro.inc` conventions (`glabel`/`endlabel`, COP0 register aliases).

## VERSION_I Code Paths

This project uses VERSION_I. Key differences from later versions:
- Uses `CartRomHandle`/`LeoDiskHandle`, not `__Dom1SpeedParam`/`__Dom2SpeedParam`.
- No `_FINALROM` debug thread code.
- Different alignment checks in some IO functions (e.g., `& 0x3FFF == 0x2000` vs `& 0x1FFF == 0`).
- Helper functions may clear all 16 words of `__osPfsPifRam` before setting `pifstatus`.
- Simpler timer/path logic without `__OSGlobalIntMask` or `BUILD_VERSION >= VERSION_K` guards.
- EPI functions use `WAIT_ON_IOBUSY` + direct address access, not the full `EPI_SYNC` handle-domain logic.

## `#pragma weak` Aliases

`#pragma weak alias = real` directives produce correct weak symbols that the linker resolves under IDO. This matches the original symbol layout where multiple names point to the same address.

## ROM Order vs Upstream Source Order

The target ROM may order functions differently from the upstream source file. Always check the ROM function order and port functions in that order so object boundaries line up. This applies to string helpers, math functions, ldiv/lldiv, and audio Param/Pull pairs.

## YAML Comments Can Be Misleading

Comments like `ultra:somefile.c` on YAML segments are often wrong or cover too much text. Verify actual function boundaries against disassembly and `symbol_addrs.txt`. A single commented segment may contain multiple source files or different functions than the comment suggests.

## `ALIGNED` Macro Requires `PRinternal/macros.h`

`ALIGNED(x)` expands to `__attribute__((aligned(x)))`, which is redefined as a no-op for IDO. Ultra source files using `ALIGNED` must include `PRinternal/macros.h` or IDO will fail with a syntax error.

## Disabled Asserts

Several upstream IO files include `assert.h`, but this repo builds with `NDEBUG`. Remove the `assert.h` include and the assert statements rather than adding a local assert header. The disabled assertions don't affect codegen.

## `_bcopy` vs `bcopy`

The header declares `bcopy` but the linked symbol is `_bcopy`. Use `_bcopy` in source to match the actual linker symbol.

## Combining Small Adjacent Functions

Small related functions from separate upstream files (e.g., `osSpTaskYield` and `osSpTaskYielded`) can be combined into a single C source file when they share the same headers and are trivially small. The remaining unmatched functions become a separate asm segment at the split address.

## Own the Full Range to Avoid Alignment Padding

If splitting after a matched helper function causes the C object's `.text` section to pad and shift following raw asm, match the entire upstream file instead so internal functions pack without section-end padding.

## Reuse of Upstream Source

For straightforward libultra functions that don't use `_DEBUG` code, have no local data, and just call simple helpers around struct field assignments, the upstream ultralib source often matches directly without modification. Always try the unmodified upstream source first before making changes.
## Converting a Jump-Table Function to Per-Function GLOBAL_ASM Breaks Rodata Refs

When a raw `asm` segment contains a function with a jump table, the table lives
in a separate `.rodata` object and references the code's local `.L` labels. As a
single raw asm segment these resolve, but converting the segment to a `c` segment
extracts each function into its own `GLOBAL_ASM` `.s` file where those `.L`
labels are file-local and no longer visible to the rodata object, producing
`undefined reference to .L...` link errors. Don't convert such a segment to `c`
unless you also handle the jump-table function's labels.
