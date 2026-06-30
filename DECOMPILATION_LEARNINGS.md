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
