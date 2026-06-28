# Decompilation Learnings

Project-specific lessons that apply when matching new functions or adding new
source files. Keep this file focused on reusable compiler behavior, matching
patterns, and verified layout/linking rules.

## IDO Codegen Patterns

- Branch direction matters. For `if (cond) { A } else { B }`, IDO usually emits
  a branch to `B` when `cond` is false and lays out `A` as fall-through. Rewrite
  equivalent conditions to match the target branch opcode and fall-through path.
- `-O2` keeps variables in callee-saved registers more aggressively and fills
  branch delay slots better than `-O1`. `-O1` more often spills values that live
  across calls.
- If a near-perfect match differs mainly by saved-register choices versus
  temp-register-plus-spill choices, try the optimization level used by the
  surrounding object before forcing source changes.
- When assigning a global pointer and then using it, IDO may reload through the
  global pointer rather than reuse the literal object address. Write C through
  the same global pointer path if the target does so.
- If a value is unchanged on the false path of an `if`, a redundant reload after
  the conditional block can be source-equivalent and may match targets where IDO
  reloads only after the taken call path while reusing the pre-branch value
  otherwise.
- Explicit casts can affect sign-extension and register allocation. If the
  target sign-extends an argument with `sll`/`sra`, try making the promotion
  explicit in C, even when the callee parameter type is narrower.
- Before tuning register allocation, verify function signatures and call
  argument order against the target assembly. Wrong argument order produces
  misleading register-allocation diffs and wastes permutation time.

## Structs, Types, and Data Access

- Prefer typed struct and array access over manual pointer arithmetic. Matching
  code should express known fields with `->`, `.`, and indexed access.
- Struct sizes and field offsets must be verified from assembly access patterns.
  A small layout mistake can shift codegen across every function that shares the
  type.
- Use the most specific parameter and local types the surrounding code supports.
  Broad `void *` typing can hide useful offset information and produce unstable
  casts or address arithmetic.
- For known global pointer variables, distinguish between accessing the pointer
  variable and accessing the pointed-to object directly. IDO often generates
  observably different code for those two source shapes.

## Globals, Data, and Linker Behavior

- IDO generates different code for globals defined in the current translation
  unit versus globals declared `extern`.
  - Local definitions can let IDO keep base addresses in registers across
    adjacent stores.
  - `extern` declarations can force extra address reloads and worse delay-slot
    scheduling.
- IDO may place explicitly zero-initialized globals such as `static u8 x = 0`
  in `.data`, not `.bss`. Account for this when splitting raw data/BSS segments.
- Symbols in `symbol_addrs.txt` using `0x700...` addresses are splat
  placeholders. Convert them to runtime `0x800...` VRAM addresses before relying
  on linker resolution.
- Symbol names in `symbol_addrs.txt` drive extraction. Manual edits to extracted
  asm labels are overwritten by `make extract`; rename symbols at the source.
- `#pragma weak alias = real` is the correct way to express weak aliases under
  IDO when multiple names resolve to the same address.
- For data mismatches after source changes, use the data differ to find the
  first mismatching symbol before making layout assumptions.

## Segment Splitting and Alignment

- C object `.text` sections pad to 16-byte alignment. Splitting a segment at an
  exact unaligned function end can shift following raw asm.
- Some extracted single-function asm ranges may include an unnamed adjacent tiny
  function. A trailing `jr $ra; nop` after an earlier return is likely a missing
  empty function boundary, not padding.
- If matched C is smaller than the original asm range, add or adjust asm
  segments so the remaining bytes/functions are still owned.
- When section-end padding causes shifts, it may be better to match the full
  upstream file or a larger aligned range instead of a tiny helper function.
- Split rodata by ROM offset, not by VRAM address. Use the linker map to confirm
  split points.
- Jump-table functions need special care when converting raw asm segments to C.
  Tables in rodata may reference file-local `.L` labels from the original asm;
  splitting functions into separate `GLOBAL_ASM` files can break those label
  references.
- YAML comments are hints, not proof. Verify actual function/file boundaries
  against disassembly, `symbol_addrs.txt`, and the linker map.

## Functions split across subsegment boundaries

- A function with no `jr $ra` before the next symbol is a **fall-through** into the
  following code. If splat placed a `[addr, c]` subsegment boundary inside it, the
  function is split into two `GLOBAL_ASM` stubs in two different `.c` files (e.g.
  `func_80048E60` in `464E0.c` falling through into `func_80048E80` in `49A80.c`).
  The "second" half is never reached via `jal` and its first instruction reuses a
  register loaded at the end of the "first" half — the giveaway.
- To merge them at the project level: move the subsegment boundary in
  `snowboardkids.yaml` to the next real function start, add an explicit
  `size:0xNNN` to the function in `symbol_addrs.txt` so splat extracts the whole
  thing into one `asm/nonmatchings/<seg>/<func>.s`, rename the now-shifted
  segment's `.c` (and its `asm/nonmatchings/` dir), and delete the obsolete
  second-half stub and its `GLOBAL_ASM`. A single C function cannot span two
  `.o` files, so the boundary move is mandatory.
- IDO range-check idiom: a value passes when it is in a signed range
  `[LO, HI)` written as two fail-fast gotos:
  `if (d >= HI) goto next; if (d < LO) goto next;`. This compiles to
  `slt at,d,HI; beqz at,next` then `slt at,d,LO; bnez at,next` — note the
  mixed `beqz`/`bnez` polarity. A compound `if (d < HI && d >= LO)` instead
  produces two `beqz`s and will not match. The goto form is required.
- Hex literals above `0x7FFFFFFF` (e.g. `0xFA000001`) have type `unsigned int`
  in C89 and would force an *unsigned* comparison (`sltu`). Cast to signed —
  `(s32)0xFA000001` — so IDO emits the signed `slt` the target uses.
- IDO's scheduler sometimes preloads a shared constant into a register at a
  block's entry and reuses it across subsequent blocks, while the target loads
  it lazily in a branch delay slot and reloads it per block. This is hard to
  force from C and is good decomp-permuter territory once control flow matches.

## Register allocation via redundant reloads

- IDO's register allocator picks colors based on its internal temp numbering,
  not source variable identity. Two C expressions that are textually identical
  (e.g. reading `arg0->unk54`) but written as separate source expressions can
  compile to two distinct loads, and that extra load shifts the base pointer
  into a different register (e.g. `$a2` instead of `$v1`) — matching the target
  exactly. Reusing a single local (`temp_a2`) for both let IDO CSE the load and
  produced a non-matching register assignment.
- A matching assignment was only achieved when (a) the field was re-read inline
  for the second use AND (b) the result was routed through a named intermediate
  variable assigned before the store. The intermediate prevented the store
  expression from being folded back into a form that re-CSE'd the reload.
- Concretely, for `func_8009D308` (`src/9CE70.c`):
  ```c
  temp_a2 = *(u8 **)((s32)arg0 + 0x54);
  phi = (s32)temp_a2 + (*(s32 **)((s32)(*(u8 **)((s32)arg0 + 0x54)) + 0x14))[*arg1];
  *(s32 *)((s32)arg0 + 0x70) = phi;
  ```
  The double read of `0x54(arg0)` is intentional and required for the match.
  This is good decomp-permuter territory once the instruction sequence matches
  but a single base register differs.
