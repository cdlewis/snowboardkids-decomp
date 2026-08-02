# CLAUDE.md

## Your Job

You are decompiling Nintendo 64 assembly code from Snowboard Kids. Your goal is to generate C code for `$functionName` that, when compiled, 100% matches the target assembly code.

The compiler is IDO 5.3 with flags `-O2 -mips1`. We follow the C89 standard.

### Laying the Foundation

Before making matching attempts, gather context for `$functionName` and make `base.c` compile.

MAKE THE MINIMAL SET OF CHANGES NECESSARY TO COMPILE `base.c`. This is important for getting an accurate baseline match percentage.

Specifically, the subagent should:

<subagent-instructions>
1. Explore how $functionName is used in the codebase. Look at ../../src, ../../include as well as the unmatched code (../../asm/nonmatchings). Write a summary of what the $functionName is and how it's used to `LEARNINGS.md`.
2. Ensure that base.c compiles successfully. Ensure that any missing types are present. base.c should only depend on "common.h". Any other missing types should be provided inline rather than via #include statements. Do not stop until base.c can be successfully built. Report status and a brief summary of your findings upon completion.
3. Verify that the function signatures base.c relies upon have the correct number of arguments and correct types for those arguments as well as for the return type. These are frequently halucinated or otherwise decompiled incorrectly.
4. Consider the purpose of the function and what it's trying to do. Does the C code reflect that purpose? Note any logical errors in the code.
5. Report back on its progress and findings
</subagent-instructions>

### Build Loop

After `base.c` builds successfully, repeat the following steps:

1. Run `./build.sh <attempt>.c` to build and get a diff against the target assembly. A score of 100% indicates a perfect match.
2. Inspect `base_diff`, `target.s`, and generated object dumps. Look for areas where the control flow and instructions do not match.
3. Come up with a plan to improve the match. Consider what the original developers intended to write given the function's broader purpose.
4. Create a new file (`base_n.c` where `n` is your attempt number) with changes you expect to improve the match. Start small and work incrementally — if you test multiple changes at once they may interact poorly.
5. Record useful observations in `LEARNINGS.md`.
6. Stop when a 100% match is reached, when the build script tells you to stop, or when you are unable to make progress (40 attempts without any improvement to the match percentage).

## Tools

- `./build.sh <file>.c` compiles an attempt and scores it against the target assembly.
- `./diff.sh <file>.o` shows a normalized diff for a previously built attempt.
- `./objdump.py <file>.o` dumps normalized MIPS assembly.
- `./map_asm_to_c.py <file>.o <line>` maps an assembly line back to annotated C when debug output is available.

## Coding Guidelines

- Look for existing types, prototypes, and globals in `../../src` and `../../include` before adding new ones.
- Prefer struct fields and array indexing over manual pointer arithmetic.
- Keep variable declarations at the start of each function or block as required by C89.
- Do not add comments to candidate C code unless they are already needed for project style.
- Do not use `GLOBAL_ASM`, `INCLUDE_ASM`, or inline assembly as a matching shortcut.
- Only use #define for constants, do not abuse it to hack around struct access issues or to try different permutations.

## Decompilation Strategy
Learnings from past decompilations can be found at `DECOMPILATION_LEARNINGS.md`.

### General Approach
- Think about what the function is *doing* within the game. What is its purpose? Structure the code to fulfill that purpose — this is the surest path to a 100% match.
- Focus on control flow differences over register or stack differences. Register and stack issues are easy to fix later.
- Look for clues in how the function is called and how it calls other functions.

### Cleaning Up Decompilation Artefacts
Literal decompilation often produces artefacts. Watch for these common patterns:

<artefact name="for-loops">
m2c struggles with `for` loops. The compiler often pulls out the condition so it can bail early if it's never met. Note that the comparison operator may also change (e.g. `<` becomes `<=`).

This code:
```c
for (i = 0; i < 10; i++) {
    // stuff
}
```

Often decompiles as:
```c
i = 0;
if (i < 10) {
    do {
        // stuff
        i++;
    } while (i < 10);
}
```
</artefact>

<artefact name="gotos">
Developers rarely, if ever, write GOTO statements but they show up often in decompilation output because many different kinds of control flow are represented as branches and jumps in assembly. Assume that GOTOs are just a decompilation artefact.
</artefact>

<artefact name="duplicated-variables">
It's far more likely that a single variable is being reused rather than many temporary variables. Literal decompilation and permuting often produce unnecessary re-assignments that hurt the match rate.
</artefact>

<artefact name="shifts-instead-of-arithmetic">
Arithmetic is often converted to shifts:
- `x >> 2` → `x / 4`
- `x << 2` → `x * 4`
</artefact>

<artefact name="false-returns">
Explicit returns in assembly might actually be fall-through returns:
```c
if (condition) {
    // main work
} else {
    // alternative path
    return X;
}
return Y;  // fall-through from if-branch
```
</artefact>

<artefact name="unrolled-loops">
IDO will sometimes unroll loops for 'performance' reasons but this decision is not visible to the disassembler.

This code:
```c
for (i = 0; i < 4; i++) {
    updatePlayer(i)
}
```

Often compiles as:
```c
updatePlayer(0)
updatePlayer(1)
updatePlayer(2)
updatePlayer(3)

Similarly:
```c
for (i = 0; i < 4; i++) {
    players[i].Coins = 0;
}
```

Often compiles as
```c
players[0].Coins = 0;
players[1].Coins = 0;
players[2].Coins = 0;
players[3].Coins = 0;
```
</artefact>

### Large Functions
Don't be intimidated — these are often straightforward if approached methodically:

1. Get the function to compile first, filling in missing types, undefined functions, etc.
2. Focus on control flow. Compare `base.c` against `target.s` — m2c's generated control flow can be convoluted and misleading. Make a checklist of problems and work through them one by one.
3. Large structs are easy. Often there are significant gaps between fields, so just focus on getting the field accesses correct.

### Graphics Microcode (F3Dex)

gRegionAllocPtr is indicative of dynamic display list construction. Use the decompile-microcode skill to help match these functions. *Never* engage in manual manipulation of gRegionAllocPtr. *Always* use gbi.h macros. 

The permuter may sometimes be helpful but it is not aware of the macros in gbi.h and will often try to replace them with their underlying commands. This is not what you want. Do not use its results if it requires you to remove gbi.h macros.

You can use the --preserve-f3dex argument when calling the permuter to explicitly prevent it from expanding maros. This can be useful for fixing register assignment issues on very close matches.

### Data Layout

Think carefully about how data is used by your function. The symbols (data/rodata/bss) used by your function may not reflect the true data layout. Often this will show up as many contiguous/adjacent symbols that are accessed sequentially (array). Or data that otherwise looks like it rolls up into a common struct. In such cases, you should update symbol_addrs to reflect the true layout even if this means wider project refactors.