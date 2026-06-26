# CLAUDE.md

## Repository Overview

This is a matching decompilation project for Snowboard Kids (N64). The goal is to create C code that, when compiled, produces the exact same ROM as the original game.

## Project Structure

- `src` decompiled or partially decompiled C code.
- `include` shared headers and assembly macros.
- `asm/nonmatchings` unmatched assembly functions extracted from the ROM.
- `asm/data` extracted data assembly.
- `assets` binary asset blobs extracted from the ROM.
- `symbol_addrs.txt` project symbol names and addresses.
- `snowboardkids.yaml` Splat configuration for ROM extraction and segment layout.

## Tools

- `./tools/build-and-verify.sh` rebuilds extracted assets/asm, builds the ROM, and verifies the SHA1 checksum.
- `python3 tools/asm-differ/diff.py --no-pager <function name>` compares compiled assembly against the target for a specific function.
- `./tools/claude --bootstrap-only <function name>` creates a per-function matching workspace under `nonmatchings/`.
- `./build.sh <file>.c` inside a generated matching workspace compiles one attempt and compares it to the target function.
- `python3 tools/data-differ/data_diff.py <symbol>` or `./tools/diff-data <symbol>` compares binary data between the target ROM and compiled output for a specific data symbol.
- `python3 tools/data-differ/data_diff.py --find-first-mismatch` scans data symbols in ROM order and shows the first mismatch. Use this after checksum failures when a data variable may be responsible.

## Tasks

### Decompile Assembly to C Code

You may be given a function and asked to decompile it to C code.

#### Step 1

First we need to spin up a decomp environment for the function, run:

```
./tools/claude --bootstrap-only <function name>
```

Move to the directory created by the script. This will be `nonmatchings/<function name>-<number (optional)>`.

Use the tools in this directory to match the function. You may need to make several attempts. Each attempt should be in a new file (`base_1.c`, `base_2.c`, ... `base_n.c`, etc).

#### Step 2 (successful match, integrate changes into project)

If you are able to match the function, update the C code to use it. The C code will be importing an assembly file, something along the lines of `INCLUDE_ASM/asm/nonmatchings/<function name>`. Replace this with the actual C code.

- Update the rest of the project to fix any build issues.
- If the function is defined in a header file (located in include/), this will also need to be updated. These other usages may teach you about the correct type of your function arguments or return types. DO NOT JUST MAKE EVERYTHING void\*!.
- Make sure to search for any existing function / struct declarations in the project (under src/ and include/). We do not want duplicate or redundant declarations.

Verify that the project still builds successfully by running `./tools/build-and-verify.sh`. If this check fails, the decompilation is NOT complete, even if individual functions appear to match.

- If the checksum fails after your changes, use `python3 tools/asm-differ/diff.py --no-pager <function>` to check ALL functions in the modified file(s). Look for functions that access the same structs you modified. Fix any mismatches before declaring success.

#### Step 3: Commit your successful match or improvement on existing partial match

If you are able to get a perfect matching decompilation, commit the changes with an appropriate message describing what you matched and how.

If you are unable to get a perfect match but have improved upon the previous best match (or there is no such previous match), record your best match next to the included ASM:

```
// your_function best match: XX%

#pragma GLOBAL_ASM("asm/nonmatchings/segment/your_function")

#ifdef NON_MATCHING
// your non-matching function
#endif
```

Respect any pre-commit hooks that prevent you from committing your change. A failed hook indicates that you have not correctly updated the C code.

You are done. Do not attempt to find the next closest match.

#### When to stop

**These rules are mandatory. Do not override them.**

If the build script tells you to stop, you MUST stop immediately. Do not make another attempt. Report:
- Best score and which file achieved it
- Summary of approaches tried
- Analysis of remaining differences
- Whether the remaining issues seem solvable or may need a different strategy

## Validation Checklist

Before declaring any changes to C code complete (including decompiling functions), verify:

- [ ] No pointer arithmetic with manual offset calculations
- [ ] All struct field accesses use `->` or `.` operators
- [ ] No `void*` parameters that should be typed structs
- [ ] Struct sizes match the assembly access patterns
- [ ] `./tools/build-and-verify.sh` succeeds


### Capture Learnings

Update DECOMPILATION_LEARNINGS.md with any new insights from matching functions in this project. These include compiler behavior, codegen quirks, and patterns specific to IDO 5.3.

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
   - Use `rg "void functionName" src/` to search source files and source-local headers

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
