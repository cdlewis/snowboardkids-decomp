---
name: decompile-function
description: Decompile a specified Snowboard Kids assembly function into matching C, or improve an existing partial match. Use for per-function matching, integration, and verification; not general build or tooling changes.
---

# Decompile Assembly to C Code

Use this workflow when decompiling a specified assembly function to matching C or improving an existing partial match. Its attempt and stop rules apply to that function-matching task. Run repository commands from the repository root unless a step directs you into a matching workspace.

## Step 1

First we need to spin up a decomp environment for the function, run:

```
./tools/claude --bootstrap-only <function name>
```

Move to the directory created by the script. This will be `nonmatchings/<function name>-<number (optional)>`.

Run `./build.sh <file>.c` in this directory to compile an attempt and compare it with the target function. You may need to make several attempts. Each attempt should be in a new file (`base_1.c`, `base_2.c`, ... `base_n.c`, etc).

## Step 2 (successful match, integrate changes into project)

If you are able to match the function, update the C code to use it. The C code will be importing an assembly file, something along the lines of `INCLUDE_ASM/asm/nonmatchings/<function name>`. Replace this with the actual C code.

- Update the rest of the project to fix any build issues.
- If the function is defined in a header file (located in include/), this will also need to be updated. These other usages may teach you about the correct type of your function arguments or return types. DO NOT JUST MAKE EVERYTHING void\*!.
- Make sure to search for any existing function / struct declarations in the project (under src/ and include/). We do not want duplicate or redundant declarations.

Verify that the project still builds successfully by running `./tools/build-and-verify.sh`. If this check fails, the decompilation is NOT complete, even if individual functions appear to match.

- If the checksum fails after your changes, use `python3 tools/asm-differ/diff.py --no-pager <function>` to check ALL functions in the modified file(s). Look for functions that access the same structs you modified. Fix any mismatches before declaring success.

## Step 3: Commit your successful match or improvement on existing partial match

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

## When to stop

**These rules are mandatory. Do not override them.**

If the build script tells you to stop, you MUST stop immediately. Do not make another attempt. Report:
- Best score and which file achieved it
- Summary of approaches tried
- Analysis of remaining differences
- Whether the remaining issues seem solvable or may need a different strategy
