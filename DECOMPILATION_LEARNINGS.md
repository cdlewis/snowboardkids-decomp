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
- IDO homes narrow (`char`/`unsigned char`/`u8`/`s16`/`u16`) parameters to their stack slot
  at function entry even in a frame-less leaf: declaring a parameter as
  `unsigned char` instead of `s32` emits a dead `sw $aN, K($sp)` of the incoming
  register followed by an `andi` to recover the clean byte. This is the source
  of the otherwise-mysterious dead argument-store-with-no-prologue pattern.
  Match it by typing the parameter as the narrow type the callee semantically
  uses. This also fires for `s16`/`u16`: an `s16` parameter used directly in a
  full-width `subu` still emits a lone dead `sw $a1, 4($sp)` with no `andi`
  recovery, because IDO only inserts the narrowing reload when the value is
  reloaded rather than reused straight from the home register (func_80056348).
  uses, not the wider type the caller passes.
- IDO's `-O2` memset loop idiom (unrolling with an `& 3` remainder loop) is NOT
  triggered by a `while (n--) *p++ = c;` form — that compiles to a plain
  single-store byte loop. The `n--` post-decrement also leaves a dead
  `move $vN, $a2` shadow each iteration (the discarded return value of `n--`),
  which is expected and should not be eliminated by rewriting the loop.
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
- Statement order drives IDO's temp-register allocation order. When a sequence
  of independent stores each load a stack arg into a fresh temp, IDO allocates
  temps (`$t6`, `$t7`, `$t8`, ...) in source order. A constant store (e.g.
  `node->unk16 = 2;`) also grabs the next temp at the point it appears in
  source. If the target allocates the constant to the *last* temp, write the
  constant assignment *after* the arg-derived stores it should follow, even
  though the scheduler ultimately interleaves the stores. Reordering just the
  constant assignment fixed a pure scheduling/register diff in func_800994F4
  (the constant store filled the final load's delay slot and landed in `$t9`
  instead of `$t8`).
- For tight pointer-walk loops with two global base addresses, IDO can schedule
  independent `addiu %lo` materializations differently depending on the exact
  source nesting/statement shape even when the control flow is equivalent.
  Keeping the nested `do { ... } while (0)` and inner loop body collapsed to the
  proven source shape fixed a pure two-instruction scheduling swap in
  func_8006D700.

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
- When a function's extracted range is exactly `jr $ra; nop; jr $ra; nop`
  (e.g. `func_8006E2B4`, size 0x10), it is two consecutive empty functions, not
  one. A single `void f(void){}` only emits one `jr $ra; nop` (8 bytes); IDO
  never emits two `jr $ra` from one function (even `return; return;` folds to
  one). Match it by defining two empty functions back-to-back in the same `.c`
  file (the second one filling the 0x800..E2BC slot), so the segment keeps its
  original byte length. IDO 5.3 does not fold identical empty functions, so each
  stays a distinct `jr $ra; nop`.
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

## Register allocation via strength-reduced induction vs explicit pointer

- A loop that bumps a pointer over an array (`player = base; do { f(player); player += STRIDE; } while (...)`)
- causes IDO to hoist the shared `base` address into a callee-saved register and
  reuse it everywhere (`move s1, s3`), forcing an extra saved register and
  `move a0, s3` at the call. The target instead kept the base in `$a0` for a tight
  top region (it was passed as the first arg to a call) and **reloaded it fresh**
  into `$s1` at loop entry (`lui s1; addiu s1`).
- The fix is to write the loop as an index-based `for` whose address is computed
  inline: `for (i = 0; i < n; i++) { f(base + i * STRIDE); }`. IDO strength-reduces
  `i * STRIDE` into a pointer that is initialized with a **fresh** `lui`/`addiu` of
  `base` in the loop preheader (the `blez` delay slot), so it does not CSE with the
  top region's `$a0`. This dropped `func_8008C704` (`src/8CAB0.c`) from 81% to 100%.
- Takeaway: when the target reloads an array base at a loop rather than reusing a
  hoisted saved register, prefer a strength-reduced indexed `for` loop over an
  explicit pointer-bump `do`/`while`.

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

- When a global is both read (for a condition) and written in the same
  function, IDO at -O2 will CSE the address into one materialized pointer
  (`lui %hi; addiu %lo; lw 0(reg)` ... `sw 0(reg)`), reusing one register for
  both accesses. The target may instead use the per-access idiom
  (`lui %hi; lw %lo(reg)` with the value loaded into the SAME register that
  held `%hi`, destroying the address) and a FRESH `lui %hi` for the store.
  This happens when the stored value is NOT the literal `$zero` but a
  register-held value: routing the constant through a named local
  (`temp = 0; ... D_xxx = temp;`) prevents IDO from folding the store to
  `sw $zero` and re-CSE-ing the address. For `func_8009FF40` (`src/9CE70.c`):
  ```c
  if ((osAiGetLength() >> 2) == 0) {
      temp = 0;
      if (D_800DF2A4 != temp) {
          return;
      }
      D_800DF2A4 = temp;
  }
  ```
  The local `temp` is the whole difference between 79% (materialized address)
  and a 100% match. `volatile` on the global did NOT help here — only the
  register-held store value defeated the address CSE.

- Register allocation for a load-vs-computed-value swap (IDO 5.3 -O2 -mips1):
  For `func_8005638C` (a delta-clamp on a struct field), naming the loaded
  value as a local (`s32 cur = arg0->unk24; s32 diff = arg1 - cur;`) made IDO
  put the load in `$v0` and the diff in `$v1` — the opposite of the target
  (load `$v1`, diff `$v0`). Writing it WITHOUT naming the load, reading the
  field inline in the subtraction and using compound assignment, flipped the
  allocation to the correct registers:
  ```c
  s32 diff = arg1 - arg0->unk24;
  if (diff >= 0x2001) diff = 0x2000;
  if (diff < -0x2000) diff = -0x2000;
  arg0->unk24 += diff;
  ```
  IDO CSEs the two reads of `arg0->unk24` (the inline read and the one inside
  `+=`) into a single `lw`, and promotes the named `diff` to the primary
  register (`$v0`). Letting a struct-field read stay inline (un-named) rather
  than hoisting it into a temp can be what picks the "right" home register.

- To produce a `li $reg, imm; addu` pair instead of IDO folding `x + imm` into
  `addiu x, imm`, keep the value in a named local that is reassigned
  (`diff = -0x2000;`) and added later as register+register. A single final
  store of the form `arg0->field += diff` (with `diff` conditionally clamped)
  schedules the `addu` into the branch delay slot and emits `li + addu` for the
  clamped path — matching the target's two-add structure exactly.

- IDO register allocation for a global's address: whether the base address of
  a global (`lui $a0/%hi; addiu`) lands in `$a0` vs `$a1`, and which temp holds
  a shifted index (`$t7` vs `$t8`), depends on how many times the source
  touches the global. In a free-list "pop" (`func_80071C84`, counterpart to the
  `stack[sp++] = node` push), the natural `return stack[--sp];` compiled to
  `$a1`/`$t7` (98%) while the real ROM uses `$a0`/`$t8`. The fix was to re-read
  the global after decrementing it and use *that* read as the array index:
  ```c
  count = D_8012193C;
  if (count == 0) return NULL;
  D_8012193C--;
  return D_80121940[D_8012193C];   // re-read post-decrement value as index
  ```
  The extra (legitimate) memory read changes liveness and flips the allocator
  to the target's registers — 100%. decomp-permuter could only reach 0
  differences here by adding `volatile` + dead-code-after-return hacks; a clean
  extra read of the global was the real solution. When stuck on pure
  temp-register naming, try adding a *meaningful* extra access to the relevant
  global/field rather than reaching for permuter artefacts.

- IDO register-allocation for a saved parameter: when a function needs to
  preserve its `arg0` (`$a0`) across `jal` calls that clobber it, IDO either
  stores `$a0` directly (`sw $a0; ... lw $v1`) or first copies it to `$a1`
  (`move $a1,$a0; sw $a1; ... lw $a1`). The `move a1,a0` form appears when the
  source declares the parameter as an untyped `void *arg0` and immediately
  assigns it to a typed local (`Struct *temp = arg0;`) — the local gets
  `$a1`. Declaring the parameter directly as the struct pointer instead makes
  IDO keep it in `$v1` (via a plain `sw $a0`). For these per-state callback
  functions (`func_80032A88` in 33680.c), the `void *arg0` + `temp = arg0`
  pattern (matching the surrounding matched siblings) is the one that hits
  100%, even though the struct-pointer signature is "cleaner". When only the
  home register of a saved argument differs, check how sibling functions in the
  same file spell their parameter.

- IDO struct-copy codegen for contiguous field groups: when three contiguous
  4-byte fields (e.g. offsets 0x48/0x4C/0x50) are each copied from a matching
  12-byte struct array element (`D_800D61C0[idx]`), the natural-looking
  per-field source
  (`*(s32*)(arg0+0x48)=temp->a; ...=temp->b; ...=temp->c;`) compiles with the
  array-pointer temp in `$v0` and the load results spread across `$t9/$t0/$t1`.
  The *original* source was a single struct assignment
  `*(Vec3i*)((u8*)arg0+0x48) = D_800D61C0[*(u16*)((u8*)arg0+0x10)];`, which
  makes IDO hold the element pointer in `$t9` and reuse `$at` for two of the
  loads — 100%. This matches the established pattern already used elsewhere in
  57EA0.c (`*(Vec3i*)((u8*)arg0+0x18) = D_800D6xxx[...]`). When a small block
  of contiguous loads/stores only differs in temp-register naming, try folding
  them into one struct-typed assignment rather than chasing it with the
  permuter.

- Capturing a compound-assignment result to control IDO load placement: for a
  field that is incremented and then passed straight to a call
  (`*(s32*)((u8*)arg0+0x18) += 0x28000; func(..., newval, ...)`), writing it as
  a temp plus separate store (`newval = *p + k; *p = newval;`) makes IDO load
  the field straight into the argument register (`$a1`) and add in place
  (`addu a1,a1,at`). The original source instead captured the value of the
  compound assignment itself — `temp = *(s32*)((u8*)arg0+0x18) += 0x28000;` —
  which makes IDO load the old value into a scratch temp (`$t6`) and only then
  add into `$a1`, matching the target. When a load feeding an argument differs
  only by being in a scratch temp vs. the arg register, try assigning from the
  `+=` expression rather than through a separate temp.

- Pointer-local for a repeated byte-offset field access: accessing the same
  offset twice (read then write of a u16 at 0x2A) as bare casts
  (`*(u16*)((u8*)arg0+0x2A)`) makes IDO re-derive the address each time. Giving
  it a named local first (`u8 *new_var = (u8*)arg0 + 0x2A; ... *(u16*)new_var`)
  reproduces the target's single address-computation and its specific temp
  register choice. This `new_var` spelling is already used by sibling callback
  functions in 3A0E0.c (e.g. func_80039584), so reach for it when matching the
  per-state callbacks in that file.

### func_8009CCC0 (variable-length sequence value reader, 9CE70.c)

Reads a 1- or 2-byte value: if the first byte's high bit is set, the value is
`((b0 & 0x7F) << 8) | b1`, else just `b0`. Stored as `u16` at `arg0 + 0xCC`;
returns the advanced read pointer. Two register-allocation artifacts were needed
for a 100% match, both forced by the permuter and semantically harmless:

- The byte-shift must be written as `((v & 0x7F) << 1) << 7`, not `(v & 0x7F)
  << 8`. The single `<< 8` reorders the `andi`/`sll` temp allocation; the split
  shift reproduces the target's `andi t7,v0,0x7f` / `sll t9,t7,0x8` exactly.
- The second byte load must go through an assigned temp:
  `v |= *(new_var = arg1)`. A plain `v |= *arg1` allocates the load into a
  different register. Assigning the pointer to `new_var` first forces IDO to
  materialize the address into the same register the target uses (`lbu t0`).

So when a u8 load feeding a bitwise-OR won't take the right register, route the
pointer through a named `u8 *new_var = ...` temp before the dereference.

### func_8009D40C (9CE70.c, byte-transform-then-add event handler)

Sibling of func_8009D3BC/func_8009D45C: reads a byte, calls `func_8009F4C8`,
stores the result, then stores `result + next_byte` to a struct field (0xEF here,
0x118 / 0xF2 in the siblings). Unlike func_8009D3BC (which re-reads the field
from memory and so emits an `lb` reload), this target keeps the result live in a
register (`move t6,v0`) and reuses it for the add.

A clean two-temp version (`temp_v0` for the result, used in both stores) matches
at 98.7% but IDO allocates the result to `t8` and the second byte to `t6` — the
opposite of the target's `t6`/`t7`/`t8` sequence. The fix is to route the result
through the byte temp inside the add expression:

```c
*(s8 *)((u8 *)arg0 + 0xEF) = *arg1 + (temp_a0 = temp_v0);
```

The inline assignment-expression `(temp_a0 = temp_v0)` makes the result value
flow through `temp_a0`'s register home (which IDO allocated as `t6`, the first
temp), reproducing `move t6,v0` / `lbu t7,0(a1)` / `addu t8,t6,t7` exactly. A
separate statement (`temp_a0 = temp_v0;` on its own line) does NOT work — the
assignment must be inlined into the RHS of the add. Same pattern as the
func_8009CCC0 note above: when a value feeds an arithmetic op but lands in the
wrong register, route it through a named temp inline within the expression.

### `struct { s32 ret; s32 pad; }` for spilled call results (IDO stack layout)

When a function spills a single `s32` local across a call (e.g. the return value
of `func_80041FB4` held while `func_800428C8` runs), IDO 5.3 places the local at
`0x24(sp)` — but the original target places it at `0x20(sp)`. The 4-byte
difference is a stack-layout mismatch that costs ~0.25% even though every
instruction otherwise matches.

The fix is to declare the spilled result as a two-word local:

```c
struct {
    s32 ret;
    s32 pad;
} l;

l.ret = func_80041FB4(0);
func_800428C8(0);
if (l.ret == 1) { ... }
```

The extra `pad` member shifts IDO's local allocation so `ret` lands at `0x20(sp)`
exactly. This is the same idiom already used throughout `src/33680.c`
(`func_80032C74`, `func_80033D0C`, `func_80033D64`, `func_800340D8`) for the
"call `func_80041FB4`, then `func_800428C8`, then test the saved result" pattern.
See `func_80032AF0`.

## IDO s0 vs stack-spill for an argument: pre-increment ties arg0 to s0

In `func_80033EA4` (src/33680.c), the argument `arg0` (a struct pointer) is used
4 times, all deep inside nested `if`s after several calls. IDO 5.3 chose to keep
it in callee-saved `$s0` (`move s0,a0` at the prologue, 0x20 frame, s0 saved at
0x18(sp)). Writing the field update as a separate temp (`newval = arg0->unk2A + 1;
arg0->unk2A = newval; if (newval == 5)`) caused IDO to instead **spill arg0 to the
stack** (`sw a0,0x18(sp)` + reloads) — a ~15-line cascade of register/frame
differences even though the logic was identical.

The fix was the pre-increment form, which both reads and writes `arg0->unk2A` in
one expression and is compared directly:

```c
if (++arg0->unk2A == 5) {
    func_80071824((s32) arg0, func_80033E54);
    arg0->unk2A = 0;
}
```

This made IDO pin `arg0` to `$s0` and reproduce the exact target register
allocation (including the `lhu t8 / addiu t9 / andi t0 / sh t9` sequence — note
the store uses the unmasked increment result, and the mask is only for the
compare). Takeaway: when an argument is spilling but the target keeps it in an
`s`-register, try restructuring the expression that uses it (pre-increment/post-
increment vs. explicit temp) — the read+write-in-one-expression form tends to
raise the argument's live-range priority so IDO assigns it a saved register.

## func_80033F5C (src/33680.c)

Sibling functions in this state-machine file share an identical skeleton. func_80033F5C matched 100% on the first try by copying the already-matched func_80033EA4 and only changing the function scheduled by func_80071824 (func_80033EA4 instead of func_80033E54). When a function sits next to an already-matched near-twin, mirroring its source form is the fastest path to a match.

## func_80034AB0 (src/33680.c)

Matched 100% on the first try by mirroring the sibling state-machine pattern.
Two notes:

- The asm calls `func_80071408(func_8003C870, 0, 0x64)` with **3 args**, even
  though `$a3` still holds `arg0` at the call site. IDO simply leaves the
  incoming `arg0` in `$a3` and never clears it; the callee only consumes
  `$a0`-`$a2`. So do NOT pass a phantom 4th argument — the project-wide
  signature is `void *func_80071408(void *, s32, s32)`. A stale `$a3` at a call
  is not evidence of a 4th parameter.
- The `u16` counter pattern `arg0->unk2A += 1; if (arg0->unk2A == N)` compiles
  to the `lhu/addiu/andi 0xFFFF/sh/bne` sequence (the `andi` mask is only for
  the compare; the store uses the unmasked increment). No temporary variable
  needed — the field read-modify-write form reproduces it directly.

## func_8003969C (src/3A0E0.c)

State-machine sibling of func_80039610/func_80039584. Matched 100% via the
permuter after a 98.6% hand attempt. The remaining differences were pure
register allocation in the `lhu/addiu/andi/sh/bne` counter block and the
`unk20 += 0x80000` else-branch.

Two changes flipped it to a perfect match, both mirroring the already-matched
func_80039610 source form:

- Mask the load explicitly: `(*(u16*)(... + 0x2A) & 0xFFFF) + 1` rather than
  `*(u16*)(...) + 1`. IDO only emits the `andi t8,t7,0xffff` on the compare
  when the source masks the *load*; an unmasked load let IDO keep the value in
  `$v0` instead of `$t6`.
- Store through a fresh `u16 *new_var = (u16*)(... + 0x2A)` pointer and compare
  against an `unsigned int new_var2 = 0xFFFF` variable (not a literal). This
  pins the exact `t6/t7/t8` (and `t8/t9/t0` in the else) register sequence.

Takeaway: when only register names differ in a u16 read-modify-write counter,
reach for the permuter — and prefer the masked-load + pointer-variable form
that the file's already-matched twins use.

## func_8003A46C (src/3A0E0.c)

Same `lhu/addiu/andi/sh/bne` counter sibling as func_8003969C, but accessing
`unk2A` through a typed struct field (`arg0->unk2A`) rather than a cast pointer.
A 98.7% hand attempt differed only by register names: target loaded the counter
into `$t7` while IDO reused `$v0` (free after the preceding `jal`), cascading
`t8/t9/t0/t1` vs `t7/t8/t9/t0`.

The single fix (found by the permuter) was masking the load explicitly:
`temp_t8 = (arg0->unk2A & 0xFFFF) + 1;` instead of `arg0->unk2A + 1`. The mask
is semantically a no-op (`lhu` already zero-extends) but it pushes IDO off the
`$v0` reuse and onto the sequential `$t6/$t7/...` allocation — confirming the
func_8003969C lesson holds for typed-field access, not just cast pointers.
Also: declaring `temp_t8` before `sp20` placed the spilled `sp20` at `0x20($sp)`
(matching the target) rather than `0x24($sp)`, since IDO reserves stack slots in
declaration order (high offset first).

## func_800716A4 (src/71AC0.c)

`func_800716A4` is sensitive to the signedness of the halfword field written at
offset `0x10`. With a typed struct field declared as `u16`, IDO 5.3 scheduled
the fourth-argument save before the call and put `move a1,t6` in the call delay
slot. Declaring the field as `s16` produced the target order:

```c
void *t = func_800711D0(arg0, arg1 & 0xFFFF, arg2);
if (t != NULL) {
    obj->unk10 = arg3;
}
```

which emits `move a1,t6` before the `jal` and uses `sw a3,0x24(sp)` in the delay
slot. The verified source also keeps the matched compact initializer/null-check
shape; splitting the code into separate assignment-style statements changed the
same call setup scheduling. Takeaway: when a struct field store is a matching
`sh` but call scheduling differs, check field signedness and small source-shape
changes before falling back to raw pointer offsets.

## func_8003B074 (src/3A0E0.c)

This function has two locals that must cross a `jal`: a spilled `s32` (the
return of `func_80041FB4`) and a register-only `u16` counter temp. The function
matched at 99.79% with only a single stack-offset difference: the spilled `s32`
landed at `0x24(sp)` instead of the target's `0x20(sp)`.

IDO 5.3 assigns local stack slots top-down in declaration order. With only the
`s32` declared, it took the high slot (`0x24`). Declaring the `u16` temp
*before* the `s32` made the temp take the high slot and pushed the `s32` down to
`0x20`, matching the target — even though the temp itself is never spilled and
stays in a register. The temp's declaration order mattered; its block scope did
not.

Takeaway: when a single spilled local is one slot too high, declaring an
additional (even register-only) local *before* it shifts IDO's slot assignment
down. Stack-slot placement of spilled locals is governed by declaration order,
not just by which variables actually spill.

## func_80033B20 (callback timer pattern)

- This is one of many sibling "timer callback" functions in `src/33680.c` (e.g. `func_80032AF0`). They share an identical skeleton: call `func_80041FB4(0)` + `func_800428C8(0)`, and on `ret == 1` increment a u16 counter field (`unk2A`); when it hits a threshold, reset to 0 and re-arm the next callback via `func_80071824`. Recognizing the sibling pattern makes these near-trivial.
- **IDO stack-layout quirk:** storing the return value of `func_80041FB4` in a plain `s32 ret;` local placed it at `0x24(sp)`, but the target uses `0x20(sp)`. The match in the sibling functions (and the target) is achieved by declaring the local as a struct with a trailing pad:
  ```c
  struct { s32 ret; s32 pad; } l;
  l.ret = func_80041FB4(0);
  ```
  This forces IDO 5.3 to allocate the value at `0x20(sp)`. Reuse this idiom for any of these callback functions whose target stores `v0` at `0x20(sp)`.
- m2c reported `a0` as "unset register" because the function is invoked through a function pointer (`func_80071824((s32) arg0, func_80033B20)`), so the calling context isn't visible to it. The real signature is `void func_80033B20(Struct33680 *arg0)`.

### IDO scheduler instability w.r.t. source formatting (delay-slot fill)

`func_80071664` (a thin wrapper: call `func_80071408(arg0, arg1 & 0xFFFF, arg2)`, then store `arg3` into the returned struct's `unk10`) only reaches 100% when written as a **single-line / "golfed" function body**. Written with conventional multi-line indentation, IDO 5.3 schedules the pre-`jal` block as:

```
andi t6,a1,0xffff
sw   a3,0x24(sp)
jal  func_80071408
move a1,t6        ; delay slot
```

…whereas the target wants:

```
andi t6,a1,0xffff
move a1,t6
jal  func_80071408
sw   a3,0x24(sp)  ; delay slot
```

Both are valid (the `move a1,t6` arg-setup and the `sw a3` spill are independent candidates for the `jal` delay slot), but IDO's delay-slot filler picks a different candidate depending on the source layout — the multi-line version fills the slot with `move`, the one-line version with `sw`. The choice is sensitive purely to whitespace/line structure, not to semantics. Its sibling `func_800716A4` (identical structure, calls `func_800711D0`) is already checked in as a one-liner and matches; writing `func_80071664` in the same one-line style yields a perfect match. When two structurally identical wrapper functions sit in the same file, prefer keeping them in the same source style.

## Signed vs unsigned comparison constants > INT_MAX (IDO 5.3)

When a comparison literal exceeds `INT_MAX` (e.g. `0xFF600001`), IDO 5.3 treats it as unsigned, so `var_a1 < 0xFF600001U` (with `var_a1` a signed `s32`) emits **`sltu`**. To force a **signed** `slt`, write the constant as its negative decimal/hex equivalent instead: `0xFF600001` as a signed 32-bit value is `-0x9FFFFF`, so `var_a1 < -0x9FFFFF` produces `slt`. This came up matching `func_8003A3E0` (sibling of `func_8003A078`, which uses in-range constants `0x800000`/`0x800001` and naturally emits `slt`). Note the surrounding reset store `arg0->unk18 = 0xFF600000` still emits a clean `lui t8,0xff60` even with the out-of-range literal, so only the comparison needs the signed form.

## Sibling allocator-init wrappers (func_8003CB78 / func_8003D218) (IDO 5.3)

`func_8003D218` is a sibling of the already-matched `func_8003CB78`. Both are tiny
init wrappers of the form:

```c
void func_8003D218(s16 arg0, s16 arg1, u8 arg2) {
    Struct3CAF0 *temp_v0 = (Struct3CAF0 *)(D_8010ADDC = (s32)func_80071408(func_8003D1EC, 0, 0x64));

    temp_v0->unk18 = arg0;
    temp_v0->unk1A = arg1;
    temp_v0->unk10 = arg2;
}
```

Key observations:
- The `D_8010ADDC = (s32)func_80071408(...)` assignment embedded inside the cast is exactly how IDO emits `sw $v0, D_8010ADDC` immediately after the `jal`, before the struct stores. Don't split it into two statements.
- The `u8 arg2` parameter is loaded with `lbu` from the stack spill but stored to the struct field with `sh` (since the field is `s16`). `temp_v0->unk10 = arg2;` reproduces this `lbu`+`sh` pair.
- The struct field at `0x10` (`unk10`) lives inside what was previously `pad0[0x18]` of `Struct3CAF0`. It is read back as `lhu 0x10($a0)` in the related `func_8003D124`, confirming it is a real `s16` field. Split the padding (`pad0[0x10]` + `unk10` + `pad12[6]`) rather than introducing a new struct type, so the layout and all sibling functions stay intact.
- Pre-existing call-site `extern`s in other files (`33680.c`, `35E20.c`) declare this function as `(s32,s32,s32)`. The ROM already matches with those in place, so leave them — retyping them risks changing the callers' argument-setup codegen.

## func_8003C0A4 — allocator-init wrapper with a high-byte store (IDO 5.3)

`func_8003C0A4` is a sibling of the matched `func_8003C420`. Same shape
(allocate a `Struct3CAF0` via `func_80071408`, stash the pointer in
`D_8010ADDC`, then set a few fields), but with an extra `u8 arg3` that is
stored to the **high byte of the `s16` at offset 0x24** (`sb $t9, 0x25($v0)`):

```c
void func_8003C0A4(s16 arg0, s16 arg1, u8 arg2, u8 arg3) {
    Struct3CAF0 *temp_v0 = (Struct3CAF0 *)(D_8010ADDC = (s32)func_80071408(func_8003C078, 0, 0x63));

    temp_v0->unk1C = arg0;
    temp_v0->unk1E = arg1;
    temp_v0->unk26 = arg2;
    ((u8 *) &temp_v0->unk24)[1] = arg3;
}
```

Key observations:
- Offset `0x25` falls inside `unk24` (`s16` at `0x24`, i.e. bytes `0x24`/`0x25`
  on big-endian MIPS). There is no standalone `unk25` field. The byte at `0x25`
  is already accessed this way elsewhere in the file (e.g. `func_8003BEF0` uses
  `((u8 *) &arg0->unk24)[1]`), so reuse that idiom rather than reshaping the
  struct.
- The four fields are written in increasing address order (`unk1C`, `unk1E`,
  `unk26`, then byte `0x25`); the `sb` for `0x25` comes after the `sb` for
  `0x26` in the target, so keep that statement order.
- Note this differs from the `func_8003D218` sibling: there the `u8` arg is
  stored with `sh` into a real `s16` field (`unk10`). Here `arg3` is genuinely a
  byte poking into the middle of a wider field, so `sb`/the cast-index form is
  correct.
- Unlike the guidance for `func_8003D218`, the three call-site `extern`s
  (`33680.c`, `35E20.c`, `37FE0.c`) were retyped from `(s32,s32,s32,s32)` to
  `(s16,s16,u8,u8)` and the ROM still matched. Retyping is safe here because all
  call sites pass integer literals.

## func_800349A8 (state callback in 33680.c)

Pattern: increment a `u16` counter field, then branch on the new value against
two constants (0x19 and 2). m2c produced:

```c
u16 temp = arg0->unk2A + 1;
arg0->unk2A = temp;
if (temp == 0x19) { ... }
else if (temp == 2) { ... }
```

This compiled to 99.1% but with a register mismatch: it stored the *masked*
value (`andi v1,...; sh v1`) whereas the target stores the *raw* increment
(`addiu t7,...; sh t7`) and masks separately into `v0` for the comparisons.

Two changes together gave 100%:

1. Use **pre-increment** `temp = ++arg0->unk2A;` instead of a separate
   `+ 1` assignment. This makes IDO store the raw increment (`sh t7`) and
   compute the masked value separately — matching the target's store/compare
   split. (Got to 99.67%, only `v1` vs `v0` left.)
2. Cast the first comparison to `(u32)` — `if ((u32) temp == 0x19)`. This nudges
   the masked value into register `v0` (reused for the second comparison too)
   instead of `v1`. The `(u32)` cast is a legitimate, semantics-preserving
   codegen nudge, not a permuter artefact.

Takeaways:
- When a value is both stored to a narrow field and used in comparisons, prefer
  `++field` to express "store raw, compare masked" — IDO codegens that split
  naturally.
- A bare `(u32)` cast on a comparison is a clean way to steer IDO's register
  choice between `v0`/`v1` for a value used in multiple branches.
- decomp-permuter confirmed the structural fix; its `& 0xFFFFFFFFFFFFFFFF` /
  `new_var` outputs were artefacts — the pre-increment + cast form is the clean
  equivalent.

## Inter-function alignment nops in isolated workspace builds

When matching `func_800996FC` (a `main`-like idle/thread-spawn function), the
isolated workspace (`./build.sh base.c`) scored 97.2% with exactly one
difference: the target had one extra trailing `nop` that mine lacked. The
function body was otherwise byte-identical.

Cause: the extra `nop` is **inter-function alignment padding**, not part of the
function's logic. IDO pads each function so the *next* function starts on a
16-byte boundary. `func_800996FC` ends at `0x9A38C` (not 16-aligned), so IDO
appends one `nop` to push `func_80099790` (the following function) to
`0x9A390` (16-aligned). In the isolated workspace there is no following
function, so IDO never emits that pad — it's an artifact of single-function
extraction, not of the C source.

Takeaway: if a workspace diff shows *only* trailing `nop`(s) and the rest is
identical, the source is correct. Integrate it into the real source file and run
`./tools/build-and-verify.sh` — the real (multi-function) build emits the
alignment pad automatically and the SHA1 will match. Don't try to force extra
nops via C.

## osCreatePiManager / idle-loop bootstrap pattern

`func_800996FC` is the main-game thread entry: it creates the PI manager
(`osCreatePiManager(0x96, cmdQ, cmdBuf, 0xC8)`), spins up the game thread on
`func_800998E4`, starts it, drops its own priority to 0
(`osSetThreadPri(0, 0)`), then idles forever (`while (1) { ; }`). The infinite
loop + unreachable epilogue (`lw ra; addiu sp; jr ra`) is emitted by IDO as
dead code after the loop — write it as a plain `while (1)` and let IDO handle
the epilogue. `osSetThreadPri(0, 0)` uses `0` (not `NULL`) as the thread arg to
match `move a0, zero`.

## Animation/state callback functions sharing a common shape (3A0E0.c)

The 3A0E0.c file contains a family of per-frame entity/state callback functions
(`func_8003A7EC`, `func_8003B074`, `func_8003B134`, etc.) that all take
`void *arg0` (a state struct) and follow the same skeleton:

1. `func_80041FB4(3)` returns a status code (saved; often compared `== 1`).
2. Mutate fields at byte offsets 0x18/0x1C/0x20 (s32 position/velocity-ish
   values) and 0x2A (a u16 frame counter) via raw `(u8*)arg0 + offset` casts.
3. Branch on the counter/state, possibly calling `func_80071824` to install a
   *new* callback (the function pointer to another func in this family),
   setting `D_8010B1A2` (an animation/state id), and/or `func_80041DD4(3, N)`.
4. Always finish with
   `func_8004209C(3, *(s32*)(...0x18), *(s32*)(...0x1C), *(s32*)(...0x20))`
   then `func_800428C8(3)`.

When matching these, mirror the offset-cast style exactly — including the
`& 0xFFFF` mask on the u16 read before `+1` (IDO loads `lhu` then `addiu` then
`andi 0xFFFF`). For large 32-bit addends that don't fit sign-extended imm16
(e.g. `0xFFFB8000`, `0xFFFB0000`), write the full hex literal directly in the
`+=`; IDO emits `lui`+`ori` to materialize it. The signed `<` comparison
against a value like `0xE00001` matches `slt` directly.

`func_8003B134` matched on the first attempt using this pattern as a template
from `func_8003A7EC`.

`func_8003AFC0` is another member of this callback family, structurally
identical to `func_8003973C` (its sibling). The only variation: it adds
`0x48000` (instead of `0x18000`) to `unk18`, and — when `func_80041FB4(3) == 0`
— increments the `0x2A` frame counter and, on reaching `2`, calls
`func_8003C0A4(-0x14, -0x40, 3, 1)` before the usual `D_8010B1A2 == 0x16`
state transition into `func_8003AF6C`.

Key codegen note: a first attempt using a named local (`var_a1 = *(...0x18) +
0x48000;`) compiled to `lw a1, 0x18(s0)` / `addu a1, a1, at` and reached only
98.6% — every remaining diff was a *register name* (`a1` vs `t6`, `v0` vs
`t8`, `t8` vs `t1`). Matching the sibling's exact idiom —
`*(s32*)(...0x18) = *(s32*)(...0x18) + 0x48000;` then *reloading*
`*(s32*)(...0x18)` inside the `func_8004209C(...)` call — drives IDO to emit
`lw t6` / `addu a1, t6, at` and the `t1`/`t8` temporaries the target uses,
giving 100%. When a sibling already matches, port its C idioms verbatim rather
than re-deriving; named temporaries change register allocation enough to cost
the match even when the logic is identical.

`func_800354BC` (in `35E20.c`) is another member of the same callback family,
sibling to `func_8003542C`. It branches on the `0x2A` u16 counter being `< 5`
to pick a direction (`1` or `-1`), then adds `0x76000` to `unk18` and
`var_v0 << 19` to `unk1C`, before the usual `func_8004209C`/`func_800428C8`
tail and a `sp20 == 1` state transition into `func_8003542C`.

Two codegen notes from matching it:

- **Stack slot for the saved return value:** the target stores the
  `func_80041FB4(1)` result at `0x20(sp)`, not `0x24(sp)`. Declaring an
  `s32 unused;` before `s32 sp20;` (mirroring `func_8003542C`) reserves the
  `0x24` slot and pushes `sp20` down to `0x20`. A single spilled local with no
  `unused` lands at `0x24` and costs ~0.2%.

- **Reading `unk2A` inline vs. a named temp:** a first attempt used a
  `u16 temp_v1 = arg0->unk2A;` local read once at the top and reused for both
  the `< 5` comparison and the `+1` store. That landed at 99.2% — the only
  diffs were register names (`v1` vs `t0`, `t1` vs `t2`, `t2` vs `t3`). Writing
  the comparison directly on the field (`if (arg0->unk2A < 5)`) and the store
  as `arg0->unk2A = arg0->unk2A + 1;` (no named temp) lets IDO CSE the two
  reads into the single `lhu $v1` the target uses, and the register allocation
  falls into place for 100%. As with the `func_8003AFC0` note: named
  temporaries perturb register allocation even when semantically identical.

`func_800378E0` (in `37FE0.c`) is the segment-2 counterpart of
`func_800354BC`. Same callback family: branch on the `0x2A` u16 counter
`< 5` to pick direction (`1`/`-1`), bump `unk18` and `unk1C`, then the
`func_8004209C`/`func_800428C8` tail and a `sp20 == 1` transition into
`func_80037868`. Differences are just constants: `0x5D000` added to
`unk18` (vs `0x76000`), and the `unk1C` increment.

Codegen note: the target computes the `unk1C` increment as
`sll t9,v0,2; subu t9,t9,v0; sll t9,t9,0x11` — i.e. `v0 * 3 << 17`, the
IDO strength-reduction for `* 0x60000`. Writing the source as
`(var_v0 * 3) << 17` reproduced the `sll`/`subu`/`sll` chain but allocated
the final result into `t0` (target keeps it in `t9`), landing at 99.3% with
only register-name diffs. Writing it as `var_v0 * 0x60000` instead let IDO
pick the exact target registers for 100%. So when a multiply-by-non-power-of-2
appears, prefer the plain literal (`* 0x60000`) over a hand-decomposed
`(* 3) << 17` — the literal gives IDO freedom to match the original register
allocation. The `s32 unused;` slot trick from `func_800354BC` was still
needed to land `sp20` at `0x20(sp)`.

`func_80034418` (in `33680.c`) is the segment-0 counterpart in this same
callback family. Identical shape: branch `unk2A < 5` for direction, bump
`unk18`/`unk1C`, `func_8004209C`/`func_800428C8` tail, `sp20 == 1`
transition into `func_800343A0`. Constants: `unk18 += 0xFFF80000` (i.e.
`-0x80000`), `unk1C += var_v0 * 0x60000`, index `0`, callback arg `0x21`.
Same two tricks applied: plain literal `* 0x60000` (not a decomposed shift)
and the leading `s32 unused;` to place `sp20` at `0x20(sp)` — 100% on the
first real attempt. The `void func_80034418(void);` stub declaration was
retyped to `Struct33680 *arg0`; the existing `func_80071824((s32) arg0,
func_80034418)` call site needed no cast.

`func_800397C4` (in `3A0E0.c`) is the segment-3 counterpart in this same
callback family (siblings: `func_800354BC`, `func_800378E0`, `func_80034418`).
Same shape: branch `unk2A < 5` for direction (`1`/`-1`), bump fields, then
the `func_8004209C`/`func_800428C8` tail and a `sp20 == 1` transition into
`func_8003973C` with callback arg `0x5D`. Constants here: `unk18 += 0x76000`,
`unk1C += var_v0 << 19`, and uniquely this sibling also bumps `unk20 +=
0xFFFA0000` (`-0x60000`). The leading `s32 unused;` was again required to
place `sp20` at `0x20(sp)`. Two integration styles both matched 100%: a
typed struct (u16 field at offset `0x2A`, so `pad24[6]` not `[0xA]`) and the
file's prevailing `void *arg0` + `(u8 *)` pointer-arithmetic style (matching
`func_8003973C`/`func_80039880`). Chose the pointer-arithmetic form to stay
consistent with the rest of `3A0E0.c`, since its existing `Struct3A0E0` has a
different layout (`unk2E` at `0x2E`) used by other functions.

`func_8003CAD8` (in `3CAF0.c`) is a short timer/animation-step callback in the
`3CAF0` family (sibling: `func_8003D2F4`). Shape: `arg0->unk1E++`; when it hits
`2`, reset to `0` and bump `unk1C++;` then if `unk1C == 5` call
`func_800716E4(arg0)`, else `func_800483FC(&D_80124868, func_8003CA70, arg0)`.
Matched 100% on the first real attempt by mirroring `func_8003D2F4`'s style
(`temp_a2 = arg0`, plain post-increment, plain `==`, plain `++`, no `& 0xFFFF`
masks). Key point: `unk1E++`/`unk1C == 5` need `lhu`/`andi ...,0xffff` in the
target asm, which requires the struct fields at `0x1C`/`0x1E` to be `u16`, not
`s16` (an `s16` field produces `lh` and drops the `andi`). The file's base
`Struct3CAF0` keeps these as `s16` for other matched functions, so the correct
fix was to retype the function parameter to the existing `Struct3CAF0d`
(`u16` at `0x1C`/`0x1E`) — same trick `func_8003D2F4` already uses. The
callback is passed to `func_80071824(void *, void *)`, so changing the
parameter type needed no cast at the call site.

`func_8003C7E4` (in `3CAF0.c`) is another timer/animation-step callback in the
`3CAF0` family (sibling: `func_8003D2F4`). Shape: `arg0->unk18++`,
`arg0->unk1A--`, `arg0->unk1E++`; when `unk1E == 3` reset to `0` and bump
`unk1C++;` then if `unk1C == 6` call `func_800716E4(arg0)`, else
`func_800483FC(&D_80124868, func_8003C77C, arg0)`. Matched 100% on the first
real attempt by mirroring `func_8003D2F4`'s style (`temp_a2 = arg0`, plain
post-increment, plain `==`, plain `++`, no `& 0xFFFF` masks). Same as
`func_8003CAD8`: the `lhu` loads of `unk1E`/`unk1C` require those fields to be
`u16`, so the parameter was retyped to the existing `Struct3CAF0d` view (which
has `u16` at `0x1C`/`0x1E` and `s16` at `0x18`/`0x1A`, matching the mixed
`lh`/`lhu` access pattern). Note `unk18`/`unk1A` are loaded signed (`lh`) so
they stay `s16`; only `0x1C`/`0x1E` are `u16`.

`func_80068DB4` (in `67360.c`) is a timer/animation-step callback (sibling:
`func_80068BF0`). Shape: when `D_80121B56 == 0`, advance a fixed-point
velocity: `unk60` holds the fractional accumulator; `var_v1 = (unk2C += unk60)`,
then `unk60 += 0xFFFF0000` (subtract a whole unit from the fraction). If the
new `unk2C < unk20` (overshot the floor), clamp `unk2C = unk20`, set
`unk60 = 0x30000`, call `func_80071824(arg0, func_80068CD4)` to switch states,
and reload `var_v1`. Then if `var_v1 < 0`, fire four `func_80088C80` calls on
`&arg0->unk28` with args `(0xC0000, 0x180000, 0..3)`. Always ends with
`func_800483FC(&D_801248D4, func_800681A4, arg0)`.

m2c produced a 99.66% match with only register-numbering differences (target
used `t9`/`t0`, mine used `t8`/`t9`). The fix was rewriting the two-line
````c
var_v1 = arg0->unk2C + temp_v0;
arg0->unk2C = var_v1;
````
as the single compound-assignment form
````c
var_v1 = (arg0->unk2C += temp_v0);
````
This is the same family of pattern noted before: collapsing an
add-then-store-then-read-back into a compound assignment lets IDO allocate the
result directly to the register it ends up in, fixing the temp-register order.
The `3 ^ 0` and `goto dummy_label` the permuter emitted alongside were pure
noise and were not kept.

`func_80036B54` (in `36F80.c`) is a state callback for actor segment 4
(sibling: `func_800388C0`). Shape: when `func_80041FB4(4) == 0`, compute
`var_v0 = (arg0->unk2A < 5) ? 1 : -1`, then drift the fixed-point position:
`unk1C += var_v0 * 0x3E000` and `unk18 += 0xFFF60000`, then
`func_8004209C(4, unk18, unk1C, unk20)`. Otherwise (timed out) clamp
`unk1C = 0x6C000`, re-call `func_8004209C`, then
`func_80071824(arg0, func_80036AC4)` and `func_80041DD4(4, 9)`. Always ends
with `func_800428C8(4)`.

The compound-assignment pattern struck again here. A 99.79% match had a single
register-allocation diff: the target loaded `unk18` into a temp (`t0`) and
later did `addu a1, t0, at`, while named-temp C (`s32 new_unk18 = arg0->unk18 +
0xFFF60000; arg0->unk18 = new_unk18; func(..., new_unk18, ...)`) loaded it
straight into `a1`. Rewriting the if-branch as in-place mutation
````c
arg0->unk1C += var_v0 * 0x3E000;
arg0->unk18 += (s32)0xFFF60000;
func_8004209C(4, arg0->unk18, arg0->unk1C, arg0->unk20);
````
(letting the compiler reuse the in-register result for the call arg) matched
100%. decomp-permuter could not improve the 99.79% version — it only emitted
same-score `& 0xFFFF...` noise — confirming this is best solved by hand via
the compound-assignment idiom.

### func_80045990 (IDO 5.3): `short`-typed array index produces a dead `addiu`

`func_80045990` is a near-twin of the already-matched `func_800459D4` in the
same file (`src/464E0.c`): both index an 8-byte-element table at `arg0+8`,
compute `temp_v0 = arg0 + *(s32*)(arg0+4)*8`, then `temp_v0 += 8`. The
difference is that `func_80045990` reads its shift amount from the table
(`lhu 0xC(temp_v1) << 5`) instead of taking it as a `u16` argument, so it has
one fewer parameter.

Two non-obvious codegen facts:

1. The dummy `sw a1, 4(sp)` (no stack frame, stores into the caller's home
   area) appears because the index parameter is `u16`, not `s32`. With a `u16`
   parameter IDO emits `andi t6,a1,0xffff` *and* the dead `sw a1,4(sp)`; with
   `s32` + an explicit `& 0xFFFF` it emits only the `andi`. Declaring the index
   `u16` (matching `func_800459D4`) reproduces the store exactly.

2. The function ends with a dead `addiu v1, v1, 8` (v1 holds `temp_v1`). This
   is *not* reproducible with `temp_v1[1].field` indexing — that folds the +8
   into the load displacement (`lw 8(v1)`, `lhu 0xc(v1)`) and emits no trailing
   addiu. The trigger is indexing with a **`short`-typed variable**:
   ```c
   short idx = 1;
   *arg2  = (void *)(arg0 + temp_v1[idx].unk0);
   *arg3 = (void *)((temp_v1[idx].unk4 << 5) + temp_v0);
   ```
   IDO materializes `temp_v1 + idx*8` for the `short` index but leaves the
   loads as `8(v1)`/`0xc(v1)`, so the pointer bump survives as a dead
   `addiu v1,v1,8` in the delay-slot region. decomp-permuter found this; it was
   not derivable by hand. Lesson: when a function matches except for a trailing
   dead pointer-arithmetic instruction, try a `short`/`char`-typed index
   variable before anything else.

- Two adjacent field updates (`unk18 += A; unk20 += B;`) where the first
  result is captured into a variable reused later (`var_a1 = unk18 + A;`) can
  land at ~98% with only temp-register naming differences (e.g. the load goes
  straight into `a1` instead of a `t6` temp). The fix is to write the update as
  a single `+=` assignment-expression whose value feeds the variable:
  `var_a1 = *(s32 *)((char *)arg0 + 0x18) += (s32)0xFFFD0000;`
  This forces IDO to load the field into a temp before the add, matching the
  target's `lw t6; addu a1,t6,at` exactly. decomp-permuter surfaced this; it
  was a pure register-allocation difference with identical control flow.

### func_8003CF98 (sprite/tile counter callback)

- A `Struct3CAF0` callback that increments `unk1E`, and when it wraps to 1
  resets `unk1E=0` / bumps `unk1C`. The whole function is a counter+reset plus
  a final `if (unk1E < 0xF)` dispatch. The natural form matches 100%:
  ```c
  if (arg0->unk1C < 5) {
      arg0->unk1E++;
      if (arg0->unk1E == 1) { arg0->unk1E = 0; arg0->unk1C++; }
  } else {
      arg0->unk1E++;
  }
  if (arg0->unk1E < 0xF) { func_800483FC(...); return; }
  func_800716E4(temp_a2); D_8010B1A2 = 3;
  ```
- **Key lessons:**
  1. m2c's explicit-temp form (`temp = (u16)x+1; var = temp & 0xFFFF; ...`) and
     a redundant `var_v0 = 0 & 0xFFFF` only reached ~93%. The 100% form lets
     IDO CSE the masked post-increment itself: writing `(u16)` reads via the
     field type and letting the natural `++` / `==` / `<` reuse one masked value
     reproduces both the `andi v0,t8,0xffff` (CSE'd compare value) *and* the
     reset-path `andi v0,zero,0xffff` (IDO does not fold `andi reg,zero,imm` —
     a literal `0 & 0xFFFF` folds to `move`, but `(u16)` of a just-stored zero
     stays as `andi v0,zero,0xffff`).
  2. **`lhu` vs `lh` is determined by the struct field type, per-function.** The
     same byte offsets 0x1C/0x1E are read as `lh` (signed) by functions typed
     `Struct3CAF0*` (s16 fields) and as `lhu` (unsigned) by functions typed
     `Struct3CAF0d*` (u16 fields). This function needs `lhu`, so its parameter
     must be `Struct3CAF0d*`, not `Struct3CAF0*` — even though other callbacks
     in the same file use the s16 struct. Casts on an s16 field give `lh`+`andi`,
     never a clean `lhu`; pick the struct variant whose field type matches the
     load.

## func_8003D88C

Small `Struct3CAF0` callback. Calls `func_800483FC`, then on
`D_8010B1A2 == 0x10` installs a callback, sets fields, and writes a flag.

- **Hoisting a pointer temp before a call changes IDO's spill strategy.**
  The body stores to `arg0->unk20` and `arg0->unk22`. Writing the natural
  ```c
  func_800483FC(...);
  if (...) {
      ...
      s16 *t = &arg0->unk20;
      t[0] = 0; t[1] = 0x100;
  }
  ```
  makes IDO keep `arg0` in a register copy (`move a2,a0`) and use direct
  offsets (`sh 0x20(a2)`, `sh 0x22(a2)`), which mismatches. The target
  instead spills `arg0` to its stack home and reloads it (`sw a0,0x18(sp)` /
  `lw t8,0x18(sp)`), then materializes the pointer (`addiu v0,t8,0x20`;
  `sh 0(v0)`, `sh 2(v0)`).
- **Fix: compute the pointer temp *before* the call:**
  ```c
  s16 *t = &arg0->unk20;   // hoisted above func_800483FC
  func_800483FC(...);
  if (...) { ...; t[0]=0; t[1]=0x100; ... }
  ```
  The address `&arg0->unk20` is invariant across the call (arg0 itself
  isn't reassigned), so hoisting is semantics-preserving and is what
  reproduces the spill/reload + materialized-pointer codegen. The
  decomp-permuter found this; it's a legitimate transformation, not noise.

## func_8003329C

Sibling of `func_80032D7C` (same `Struct33680` callback shape). Two lessons:

- **Don't introduce a `u16` temp for `field + 1` when the original reused the
  field.** m2c produced `u16 temp = arg0->unk2A + 1; ...; arg0->unk2A = temp;
  if (temp < 3)`. That made IDO mask the value into the temp register and
  store the *masked* value (`sh $masked`). The target keeps the unmasked
  `+1` (`addiu t9,t8,1`), stores it (`sh t9`), and only masks into a fresh
  register for the comparison (`andi t0,t9,0xffff; slti at,t0,3`). Writing it
  inline as the reference function does matches exactly:
  ```c
  arg0->unk2A += 1;
  if (arg0->unk2A < 3) { ... }
  ```
  (The `if (arg0->unk2A == 2)` check further down re-loads from memory, so the
  `== 2` and `< 3` tests intentionally use different value sources.)

- **A dummy `s32 unused;` declaration controls the stack slot of a real
  local.** IDO (5.3, -O2) assigns locals to *decreasing* stack offsets in
  declaration order. With only `s32 sp20;`, it landed at `0x24(sp)`; the target
  has it at `0x20(sp)`. Declaring `s32 unused;` *before* `s32 sp20;` (exactly
  as the already-matched `func_80032D7C` does) pushes `sp20` down to `0x20(sp)`
  for a 100% match. The `unused` slot occupies `0x24(sp)` and is never read.

## Local alias forces callee-saved register allocation (s0)

When a function keeps `arg0` live across several `jal` calls AND uses it inside a
loop, IDO 5.3 may still keep it in a caller-saved register (`a2`) and
spill/reload it around each call (frame `-0x18`, no `s0` save). The target may
instead keep it in callee-saved `s0` (frame `-0x20`, `sw s0`).

A reliable way to force the `s0` allocation is to introduce a **local alias** of
the argument at the top of the function and use that alias everywhere:

```c
void func(void *arg0) {
    void *new_var = arg0;   /* local copy -> IDO colors this into s0 */
    /* ... use new_var instead of arg0 ... */
}
```

This was the only difference between a 70% (a2 + stack spill) and a 100% (s0)
match in `func_8006713C`. Compare with `func_80065764`/`func_800666B0`, which
keep the loop counter in a register and spill `arg0` to the stack instead.


### Don't cache a struct-field value the target keeps reloading from memory

In `func_80039E5C` (a state-machine callback in `src/3A0E0.c`), the value
`arg0->unk18` is read several times across `jal` calls. Writing it into a local
(`var_a1 = arg0->unk18 += ...; ... var_a1 = arg0->unk18;`) caused IDO 5.3 to
spill that local into a callee-saved register (`s1`), adding `sw/lw s1` pairs
and a larger frame — a clear mismatch against a target that reloads
`lw a1, 0x18(s0)` fresh before every use.

The fix was to drop the local entirely and reference the field directly each
time:

```c
arg0->unk18 += 0xFFFD8000;
if ((sp == 1) && (arg0->unk18 < 0x800001)) { ... }
else if ((sp == 1) && (arg0->unk18 >= 0x800001)) { ... }
func_8004209C(3, arg0->unk18, arg0->unk1C, arg0->unk20);
```

Note the compiler still reuses the in-register sum (`t7`) for the *first*
comparison right after the `+=`, but reloads from memory for later uses and
after calls — matching the target exactly. If a target repeatedly loads a field
from memory instead of holding it in a saved register, prefer direct field
access over a caching local. Compare with the `var_a1`-style functions
(`func_8003A324`, `func_80039F7C`) where the local *is* present in the target.

## IDO dead-store elimination blocks a redundant-store match (func_8003DBE8)

Target asm contains a genuinely *dead* store sequence on one field:
```
lhu   t6, 0x1C(a0)
addiu t8, t6, 1
andi  t9, t8, 1
sh    t8, 0x1C(a0)   ; stores the +1 value (immediately overwritten)
...
sh    t9, 0x1C(a0)   ; stores the &1 value (delay slot)
```
i.e. the source wrote `x->unk1C = x->unk1C + 1; x->unk1C &= 1;` (or the
temp-variable equivalent). At `-O2` IDO **eliminates the first store** for any
clean formulation:
- temp variables (`t8 = ...+1; t9 = t8 & 1; x->unk1C = t8; x->unk1C = t9;`) →
  DSE removes `sh t8`.
- field ops (`x->unk1C &= 1;`) → IDO *reloads* the field (`lh` + `nop`), which
  is worse.
- `++x->unk1C` → loads via `lh` (signed) and reloads, also wrong.

The only thing that restored the dead store was a non-clean permuter hack (a
`char` intermediate between the two stores + empty `if(1){}` scheduling
barriers), and even that only reached 95.7%, not 100%. The remaining gap is
purely register allocation (`lhu t6` vs `lhu v1`) and IDO's choice to factor
`a0+0x20` as a base for the `unk22` accesses (`2(v0)` vs `0x22(a0)`) — neither
yielded to clean C. Takeaway: when a target shows an overwritten store to the
same field, IDO's DSE may make a clean 100% match unreachable; record the best
clean score rather than forcing artefacts.
