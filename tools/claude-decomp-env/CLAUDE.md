# CLAUDE.md

## Your Job

You are decompiling Nintendo 64 assembly code from Snowboard Kids. Your goal is to generate C code for `$functionName` that, when compiled, 100% matches the target assembly code.

The project currently builds C with IDO through `tools/asm-processor`, using the same effective flags as the root Makefile: `-O1 -mips2`. Follow C89 declaration style.

### Laying the Foundation

Before making matching attempts, gather context for `$functionName` and make `base.c` compile.

1. Explore how `$functionName` is used in `../../src`, `../../include`, and `../../asm/nonmatchings`.
2. Write concise notes about purpose, callers, callees, relevant globals, and suspected types to `LEARNINGS.md`.
3. Make the minimum changes necessary for `base.c` to compile. `base.c` should only include `common.h`; add any missing local declarations inline.

### Build Loop

After `base.c` builds:

1. Run `./build.sh base.c` to compile and compare against `target.o`.
2. Inspect `base_diff`, `target.s`, and generated object dumps.
3. Create incremental attempts as `base_2.c`, `base_3.c`, etc.
4. Record useful observations in `LEARNINGS.md`.
5. If a function is nearly matching but stuck on register allocation, or if manual progress has stalled, use the `decomp-permuter` skill from the project root. Run it with a timeout and point `--source-file` at the best attempt, for example:

```bash
timeout 300s ./tools/permuter --source-file nonmatchings/<function-name>/base_N.c <function name>
```

6. Stop when a 100% match is reached or when the build script tells you to stop.

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

## Decompilation Strategy

Focus on the function's intended purpose, then refine control flow, stack layout, and register allocation. Literal decompiler output often contains misleading gotos, duplicated temporaries, and shifted arithmetic where the original source was simpler.
