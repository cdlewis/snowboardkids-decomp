# CLAUDE.md

## Repository Overview

This is a matching decompilation project for Snowboard Kids (N64). The goal is to create C code that, when compiled, produces the exact same ROM as the original game.

## Project Structure

- `src` shared C code and assembly.
- `src/us` unmigrated US C code and assembly.
- `src/jp` JP sources; assembly-only segments are extracted under `asm.jp`.
- `include` shared headers and assembly macros.
- `asm/nonmatchings` unmatched assembly functions extracted from the ROM.
- `asm/data` extracted data assembly.
- `assets` binary asset blobs extracted from the ROM.
- `symbol_addrs.txt` project symbol names and addresses.
- `snowboardkids.yaml` Splat configuration for ROM extraction and segment layout.

## Tools

- `./tools/build-and-verify.sh` rebuilds extracted assets/asm, builds the ROM, and verifies the SHA1 checksum.
- `python3 tools/asm-differ/diff.py --no-pager <function name>` compares compiled assembly against the target for a specific function. Note that you *must* rebuild the project manually before digging or you will potentially operate on stale data.
- `python3 tools/data-differ/data_diff.py <symbol>` or `./tools/diff-data <symbol>` compares binary data between the target ROM and compiled output for a specific data symbol.
- `python3 tools/data-differ/data_diff.py --find-first-mismatch` scans data symbols in ROM order and shows the first mismatch. Use this after checksum failures when a data variable may be responsible.

## Matching Data

After changing data definitions or data-related struct layout, build first:

```bash
./tools/build-and-verify.sh
```

If the checksum fails and a data mismatch is suspected, locate the first mismatching symbol:

```bash
python3 tools/data-differ/data_diff.py --find-first-mismatch
```

For a known symbol, compare it directly:

```bash
python3 tools/data-differ/data_diff.py <symbol>
./tools/diff-data <symbol>
```

The data differ uses `symbol_addrs.txt` metadata for single-symbol diffs. Symbols need ROM and size annotations such as `rom:0x... size:0x...` for precise comparisons.

## Code Quality Standards

### Avoid Pointer Arithmetic

When you see pointer arithmetic patterns like `*(type*)((u8*)ptr + offset)`:

1. **Identify the access pattern:**
   - What offset is being accessed? (e.g., `0xC` means field at offset 12)
   - Is it accessing an array element? (e.g., `arg1 * 36` means 36-byte elements)
   - What field within the element? (e.g., `+ 0xA` means field at offset 10)

2. **Create appropriate structs:**
   - Define the element struct with correct size and field offsets
   - Define the container struct with pointer at correct offset
   - Use meaningful names or `unk[Offset]` naming convention

3. **Verify struct sizes:**
   - Calculate total size to ensure it matches the multiplier in pointer arithmetic
   - Example: `arg1 * 36` means struct must be exactly 36 (0x24) bytes

### Struct Modification and Extension

When modifying struct definitions:

- Search the entire codebase for other references to the same struct
- Check if other functions access fields at nearby offsets
- Verify ALL affected functions still match after struct changes
- Example: If you add a field at offset 0x14, search for all functions accessing that struct and verify they still compile to the correct offsets

### Avoid Redundant Declarations

After adding your decompiled function, check for any redundant extern declarations:

1. **Search for existing declarations**: For each extern function you used, search the codebase to see if it's already declared in a header file:
   - Use `rg "void functionName" include/` to search headers
   - Use `rg "void functionName" src/us/` to search source files and source-local headers

2. **Remove redundant externs**: If a function is already declared in an included header file, remove your extern declaration to avoid duplication

3. **Verify the build still works** after removing redundant externs

Example: If you added `extern void setCallback(void *);` but `task_scheduler.h` (which is already included) declares it, remove your extern declaration.

## Code Structure

C files should be organized in the following way:

- Macro definitions
- Struct definitions
- Global variables
- Function declarations
- Function implementations

You should proactively reorganize code to preserve this structure.
