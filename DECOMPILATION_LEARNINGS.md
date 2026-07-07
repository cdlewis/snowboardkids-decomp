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
- Be careful when a newly matched narrow-parameter callee sits before its callers
  in the same translation unit. A visible `s16` prototype can change the callers'
  argument setup even if the callee itself matches. In enqueuePositionalSoundRequest, the local
  callee matched naturally with `s16` parameters, but the neighboring wrappers
  only matched when they saw the original promoted `s32` signature; the callee
  then had to read the low halfword from the homed promoted arguments.
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
- For narrow locals passed to a helper and then used in a later branch, IDO may
  spill a promoted copy across the call and grow the stack frame. Introducing a
  separate `u32`/`unsigned int` draw argument and branching on that widened temp
  can preserve the target stack frame while leaving the original `u16` local for
  subsequent calls (func_80030EF0).
- For draw helpers with many stack arguments, the visible prototype can affect
  both promotion and local placement. In func_8002BA38, declaring the alpha
  helper parameter as `u16` prevented an extra promoted spill, while a live
  `s32 zero = 0` local used for the zero arguments moved the `u16` alpha local
  from `sp+0x3E` to the target `sp+0x3A` without changing the emitted zero
  stores.
- The common four-tile menu panel draw pattern (`func_8000F030` tile indices
  3-6) matches when the `x + 0x40` and/or `y + 0x40` coordinates are explicitly
  cast back to `s16`. This reproduces the target `sll`/`sra` sign-extension
  before the draw calls (func_8001958C, func_80029598).
- Before tuning register allocation, verify function signatures and call
  argument order against the target assembly. Wrong argument order produces
  misleading register-allocation diffs and wastes permutation time.
- If a target loads a symbol by taking the base of a neighboring array or table
  and then applying a field/element offset, using the apparent field symbol
  directly can produce a different `lui/addiu` pair and shift nearby scheduling.
  For example, an asset id at `D_80112130 + 0x42` may need to be written as
  `D_80112130[0x21]` rather than the equivalent named `D_80112172` symbol.
- Split `double` constants in rodata may have labels on both 32-bit words.
  Declaring and using the high-word label as an `f64` can still produce the
  target linked addresses even if the disassembly spells the low-word load as
  `D_xxxx+4` versus the explicit low-word label. In func_8009F194 this left a
  workspace diff only in relocation names, while the final linked code matched.
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
- For callbacks that initialize a struct field and then immediately pass that
  field to another helper, IDO may store-forward the value instead of reloading
  from the field. Marking just that field volatile can force the target-shaped
  store/reload and recover the branch span/instruction count, but it may still
  leave pure temp-register naming differences (func_8006B6C8 improved from
  90.895% to 98.684% this way).
- A 4-case `switch` (cases 0-3 plus default) compiles to a comparison chain
  (`beqz`/`beq` ladder), but adding an explicit empty `case 4: break;` flips IDO
  to emit a 5-entry `.late_rodata` jump table (`sltiu $at, $v0, 5; jr`). When a
  target's switch uses a jump table whose entry count exceeds the obvious case
  count, look for the "extra" case being an empty body identical to default
  (func_80023198 vs the matched func_800219E4, which lacks case 4 and uses a
  ladder). The empty case is often the terminal state of the state machine.
- State-machine switches may need an explicit empty terminal case even when the
  post-switch cleanup handles that same state. In func_80021F80, adding
  `case 6: break;` was required to make IDO emit the target's 7-entry jump table
  instead of a 6-entry table for cases 0-5.
- IDO can emit a genuinely unreachable (dead) `li` after an unconditional `b`
  when compiling a complex `||`/`&&` short-circuit with comma-operator-style
  intermediate assignments to the same variable (e.g. the threshold-selection
  `if ((a==2) || ((t=-0x88, a==1) && (t=-0x88, b==0))) t=-0x8A;` in
  func_80023198). This dead `li $v0, -0x88` after `li $v0, -0x8a; b .merge` is
  not reproducible from cleaner equivalent C (if/else chain, ternary, or
  pre-initialized variable) — those either fold it or relocate the constant.
  Hoisting the second global read into a local before the condition
  (`var = D_...; ... && (... var == 0)`) reproduces the eager load but not the
  dead instruction; treat the residual ~one-instruction dead-store diff as an
  IDO artefact rather than chasing it.
- For tight pointer-walk loops with two global base addresses, IDO can schedule
  independent `addiu %lo` materializations differently depending on the exact
  source nesting/statement shape even when the control flow is equivalent.
  Keeping the nested `do { ... } while (0)` and inner loop body collapsed to the
  proven source shape fixed a pure two-instruction scheduling swap in
  func_8006D700 and func_80045914.
- IDO aggressively unrolls *structured* counted loops (`for`/`while`/`do-while`)
  whose trip count is computable at runtime from two address operands: it emits
  a `subu` (end-start) + `andi N,0x..` remainder pre-loop + an unrolled body
  (the same `& (factor-1)` idiom as memset). This fires for clean pointer
  reductions like `for (p = &A; p != &B; p += 4) { if (*p == -1) n++; }`. To get
  the plain `bne p,end` do-while the original game shipped, write the loop as an
  *unstructured* goto loop (label + `if (p != end) goto label;`); IDO's loop
  unroller only runs on structured loops, so the goto form is left alone. This
  is how func_8000C114 matched (its `&A`/`&B` are adjacent BSS symbol labels).
  Note the codebase already expresses such loops in the collapsed
  `do { ... } while (0);` form (see func_8000B690) — keep that exact shape;
  IDO's codegen here is sensitive to source line layout even for token-identical
  input, so a reformatted (multi-line) version can re-introduce a two-instruction
  `addiu %lo` scheduling swap.

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
- Replacing manual stride math with a typed fixed-size array can still need the
  original local temporary shape for register allocation. Directly passing
  `array[index]` may reorder independent loads, while assigning
  `Element *ptr = &array[index];` and passing `*ptr` preserves typed access and
  can keep the original register order.
- For small struct copies, prefer the aggregate assignment if the target copies
  consecutive words through `$at` rather than independent temps. In
  enqueuePositionalSoundRequest, three scalar `s32` field assignments compiled to
  `$t8`/`$a0`/`$t9`, while `node->pos = *pos` for a three-word
  `SoundPosition` emitted the target `$at`/`$t9`/`$at` load-store sequence.
- Some globals are aliases into the middle of a larger struct array. Rewriting
  an access through the larger base struct can be semantically cleaner but still
  change IDO scheduling or symbol offsets; keep the alias symbol when a function
  was originally matched through that address, and document the stride with a
  named constant or local type where possible.

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
- Renaming a Splat C segment also changes the generated
  `asm/nonmatchings/<segment>/...` path expected by `#pragma GLOBAL_ASM`.
  Update the pragmas after changing `snowboardkids.yaml`, then rebuild from
  extraction so stale old-directory paths do not hide include failures.
- Pure `nop` text ranges may be intentional padding between libultra objects.
  Keep them assembly-backed when C would emit a real function prologue/return
  sequence or different alignment filler.
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
  `func_80048E60` in `render_asset_utils.c` falling through into `func_80048E80` in `49A80.c`).
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
  top region's `$a0`. This dropped `func_8008C704` (`src/race_player_state.c`) from 81% to 100%.
- Takeaway: when the target reloads an array base at a loop rather than reusing a
  hoisted saved register, prefer a strength-reduced indexed `for` loop over an
  explicit pointer-bump `do`/`while`.
- In `RaceInputPlayer`, the index at offset `0x0` should be `s16`, not `u16`.
  State-transition routines such as `func_800971B8` and `func_8009782C` pass it
  to `func_80095F90` with a signed halfword load (`lh`); declaring it unsigned
  changes those calls to `lhu` and breaks the ROM checksum.

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

## func_800716A4 (src/effect_task_scheduler.c)

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

## effect_task_scheduler.c pool layout

The scheduler node used by the old `71AC0.c` segment is a 0x118-byte
`EffectTask`. The first fields are a linked-list node (`prev`, `next`), callback
pointer at offset 0x8, type/priority halfwords at 0xC/0xE, signed `unk10`,
`callbackTimer` at 0x14, `isActive` at 0x16, and 0x40 argument slots beginning
at 0x18. `func_80071408` clears those argument slots as a u32 array, so declaring
them as `void *args[0x40]` keeps the stride and avoids raw offset stores.

The init assembly in `func_80070EC0` also shows the allocation table is built in
four interleaved 0x460-byte pages, where 0x460 is exactly four 0x118-byte
`EffectTask`s. The labels around `D_80112780`/`D_80112784` and
`D_801214D8`/`D_80121820` are split inside larger scheduler objects, so check
neighboring BSS labels before assuming each label is a standalone allocation.

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
same file (`src/render_asset_utils.c`): both index an 8-byte-element table at `arg0+8`,
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

## func_8007105C — linked-list drain with a global cursor (100%)

Walks a linked list (head D_80112784), mirroring each node into a global cursor
D_80121848, clearing unk14, calling each node's unk8 method with a pointer to
itself, then advancing via unk4.

High-value codegen insight (writing the loop body through the global rather than
the local copy makes IDO keep the reloaded cursor in a callee-saved register and
emit the seemingly-redundant move a0,s0). Body:

    D_80121848->unk14 = 0;
    D_80121848->unk8(D_80121848);   // -> lw s0,0(s1); lw t9,8(s0); or a0,s0,zero
    s0 = D_80121848->unk4;

The residual 0.7% (two diff lines) was pure temp-register naming: target emits
lui t6 / lw t7 for the two scratch loads; clean C emitted v0 / t6. The root
cause was the prologue %hi temp (v0 vs t6) cascading into the post-call temp
(v0/t6 vs t6/t7). Two source-level changes (found by the decomp-permuter) steer
IDO's allocator to the target's t6/t7 pair, both semantically harmless:

  1. Capture the global store into the local as a single expression instead of
     separate statements — `s0 = (D_80121848 = D_80112784);` rather than
     `s0 = D_80112784; D_80121848 = s0;`. This makes the prologue %hi temp t6.
  2. Clear unk14 through a named int temp rather than the literal 0 —
     `new_var = 0; D_80121848->unk14 = new_var;` instead of
     `D_80121848->unk14 = 0;`. The extra temp lifetime shifts the post-call
     reload to t7.

When a >95% match differs only in which t-register a scratch lui/lw pair lands
in, try (a) fusing a store-and-assign into one expression and (b) routing a
constant through a named local — both perturb IDO's whole-function temp
numbering without changing behavior.

## func_8007115C — sibling linked-list drain (100% via a zero-instruction hint)

Same loop body as func_8007105C above, but the cursor head is read straight from
D_80121848 (no separate D_80112784 source). With a single global, IDO refuses to
keep the node in a callee-saved home the way the two-global init of the sibling
does: clean formulations plateau at 92%. The natural body

    s0->unk14 = 0;
    s0 = D_80121848;
    s0->unk8(s0);
    s0 = D_80121848->unk4;
    D_80121848 = s0;

folds the `s0 = D_80121848` reload straight into `a0` (`lw a0,0(s1); lw t9,8(a0)`)
instead of the target's `lw s0,0(s1); lw t9,8(s0); or a0,s0,zero`. Writing the
body through the global (the trick that fixed the sibling) here regresses to 74%
because IDO colors `s0` as `&D_80121848` rather than the node.

The only thing that recovered the `move a0,s0` was a dead, three-operand `&&`
condition immediately before the call:

    if ((s0 && s0) && s0) {}

IDO folds this to **zero instructions** (the objdump is identical to the target,
27 instrs, nothing added) — it is purely an allocator-pinning hint that keeps
`s0` live as a callee-saved home across the call setup. Two-operand forms
(`if (s0 && s0)`, `if (s0)`) optimize away with no allocation effect; you need
three `&&` operands. Unlike the func_8003DBE8 case (where the permuter hack only
reached 95.7% *and* added real instructions), this hint reaches a true 100%
byte match with no fabricated logic in the ROM, so it was accepted with an
explanatory comment.

## IDO 5.3 stack slot ordering depends on declaration order

When a function has multiple stack locals (e.g. a u16 and an s32), IDO 5.3
assigns stack home slots based on the order of declaration. For func_8003AE8C
(frame 0x28), declaring s32 sp20 then u16 temp placed sp20 at 0x24(sp).
Reversing to u16 temp then s32 sp20 (mirroring sibling func_8003B074) placed
sp20 at 0x20(sp) as in the target - a 100% match. When chasing a lone sw/lw
stack-offset mismatch against a known-good sibling, mirror its local-variable
declaration order exactly.

## IDO 5.3: split declaration+assignment of a pointer arg hoists its `lui`

For func_80021EA8, passing a computed array-pointer argument inline as
`func(..., &D_800B6210[D_80121B50 * 0x94], ...)` produced correct code but
emitted the base-address `lui %hi(D_800B6210)` one instruction too late vs the
target (98.89% — only that single reordered `lui`). The pointer matched only
when its declaration and assignment were *separate statements*:

```c
u8 *ptr;
ptr = &D_800B6210[D_80121B50 * 0x94];
func_80013154(arg0->unk18, arg0->unk1A, ptr, 1, arg0->unk1C, 0);
```

A combined initializer (`u8 *ptr = &D_800B6210[...];`) did NOT match — IDO
schedules the `lui %hi` earlier only with the split form, letting the scheduler
interleave it between the halfword loads. When chasing a single misplaced
address-materialization `lui`, try splitting a local pointer's declaration from
its assignment. (decomp-permuter found this directly; the manual
combined-initializer attempt stayed at 98.89%.)

## IDO 5.3: reusing a just-stored global value forces the target's register reuse

For func_8007D190, the target reuses a value it just stored to a global: it
computes `t6 = ptr+2`, stores `D_80121B90 = t6`, then builds the next pointer as
`v1 = t7*6 + t6` (reusing `t6`, not re-adding 2). Matching this required
*referencing the stored global by name* on the next line rather than re-deriving
the expression:

```c
D_80121B90 = ptr + 2;
v1 = D_80121B90 + *(u16 *)ptr * 6;   /* reuses the ptr+2 the compiler kept in a reg */
D_80121B94 = v1 + 2;
v1 = D_80121B94 + *(u16 *)v1 * 8;    /* reuses v1+2 */
D_80121B98 = v1 + 2;
```

Writing `v1 = ptr + 2 + ...` (re-deriving) instead made IDO emit a separate
`addiu v1,v1,2` and dropped the match to 85%. The final `D_80121B98` also needed
a *two-step* computation (`v1 = ...; D_80121B98 = v1 + 2;`) to get the
`addu v1,t2,t0` + `addiu t4,v1,2` pair instead of a folded `addu t4,t2,t0`.
Reassigning the same local across steps matches IDO's habit of reusing a single
temp register (`v1`) through a chain of pointer arithmetic.

## IDO 5.3: compound assignment steers the temp register for high-half loads

For func_800710CC (and the sibling func_8007105C already in the tree), the
target materializes a global pointer with a *separate* temp register:

    lui  t6, %hi(D_80112784)
    lw   s0, %lo(D_80112784)(t6)

Writing `Struct *s0 = D_80112784;` instead made IDO fold the base into the
destination: `lui s0,%hi(...); lw s0,%lo(...)(s0)` (98.6% — only temp-register
naming differed). The fix that reached 100% was a compound assignment that
stores through a global in the same expression:

    EffectTask *s0 = (D_80121848 = D_80112784);

The captured store to `D_80121848` makes IDO keep the loaded value live across
the store, so it allocates a distinct temp (`t6`) for the `lui` base rather than
reusing `s0`. When a global-pointer load's `lui` base register differs from its
`lw` destination, try a compound `local = (global = expr)` initializer.


## IDO 5.3: source order of struct-field stores steers temp register numbering

For func_800721B8 (a sibling of the already-matched func_80072138), the body
writes several byte fields of a QueueEntry72430 (6x u8). The compiled store
sequence is scheduled out of source order (unk0, unk2, unk3, unk5, unk1, unk4),
and the temp registers used for the loaded constants (1, 0x80) and reloaded
args (arg0, arg2) are numbered t0..t3. With the stores written in offset
order (unk0, unk2, unk3, unk5, unk1, unk4), IDO swapped two of the temp names
(arg0 t2, 0x80 t1) versus the target (arg0 t1, 0x80 t2), giving 99.5% with
identical control flow.

Writing the field assignments in ascending field order -- specifically moving
the unk1 (arg0) store to appear immediately after unk0 and before unk2/unk3 --

    temp_v1->unk0 = 1;
    temp_v1->unk1 = (u8)arg0;
    temp_v1->unk2 = (u8)arg1;
    temp_v1->unk3 = 0x80;
    temp_v1->unk5 = 0;
    temp_v1->unk4 = (u8)arg2;

made IDO allocate the temp registers in the exact target numbering. The emitted
store schedule is unchanged; only the source order of the assignments influenced
register numbering. When only temp-register names differ in an otherwise
identical sequence of struct stores, reorder the assignments in C (rather than
reordering logic) to steer the allocator.

## Array-decay vs pointer load for struct fields (func_8006D7D4)

When a struct field is passed *by address* into a helper (the assembly shows
`addiu $aX, base, offset`), the C field must be an array/struct so it decays to
its address, NOT a pointer (`void *`/typed pointer). If the field is declared as
a pointer, IDO emits `lw $aX, offset(base)` (loads the stored value) instead of
`addiu` (computes the address), dropping the match from 100%.

Concrete case: `Object6E120` has a 0x64-byte region starting at 0x30. The
function passes the start (`unk30`, offset 0x30) to one helper and an interior
spot (`unk50`, offset 0x50) to another. Modeling both as `char` arrays
(`char unk30[0x14]; ...; char unk50[0x44];`) and writing `obj->unk30` /
`obj->unk50` (no `&`) yields the `addiu` address computation the target wants.
A `void *unk30` declaration forced a value load and broke the match.

This mirrors the existing `func_8003DDD0` (src/3E9D0.c), which uses
`char unk30[0x14]` the same way — `temp->unk30` decays to `&unk30[0]`.

## Reload reused switch variable before final check (func_8002E6E4)

A function that switches on a state byte (`arg0->unk24`) and then, after the
switch, tests the same state (`if (var_v0 == 2)`) can land in a different
register than the target even when every instruction matches. m2c carries the
case-local value of the switch variable through to the final test, so IDO parks
the constant in one v-register and the state byte in another, mismatching the
target (which keeps the state byte in `v0` and reuses an arg register for the
literal).

The fix: re-read the state from memory right before the final test:

```c
    switch (var_v0) { ... }
    var_v0 = arg0->unk24;   /* <-- reload, not carried from the switch */
    if (var_v0 == 2) { ... }
```

IDO hoists the reload so each path reads `lbu` once and the register allocation
falls into line. This was found by decomp-permuter at a 94.5% baseline where the
*only* diff was register names (`v0` vs `v1`, arg-reg vs `v0` for the literal) —
a strong signal that the permuter, not hand-editing, is the right tool.

## Forcing IDO commutative-operand order with the comma operator (func_8009CF30)

IDO's codegen for `or`/`add` is commutative, and the operand register order in
the emitted instruction is not determined purely by source order. Loading two
bytes then computing `(arg1[0] << 8) | arg1[1]` consistently emitted
`or tX, arg1_1_reg, shift_reg` (loaded byte first), while the target wanted
`or t1, t8, t9` (shift result first).

Splitting the computation through a `short` intermediate fixed the register
allocation but left the operand order flipped:

    short new_var;
    new_var = arg1[0] << 8;
    new_var = arg1[1] | new_var;            /* -> or t1, t9, t8 (wrong) */

Wrapping the right operand in a comma expression flips the order IDO assigns
the operands, giving the exact target:

    new_var = arg1[1] | ((0, new_var));     /* -> or t1, t8, t9 (right) */

`(0, x)` is a standard decomp idiom for forcing evaluation/operand order; it
has no runtime effect. Found via decomp-permuter from a 98.75% baseline where
the only diff was this single commutative-operand flip.

## Two-store layout: materialize return pointer before the zero store (func_8009CF30)

For "read N bytes big-endian, store a u16, store 0, return ptr+N" functions
(sibling of the matched `func_8009CF1C`), IDO wants the return pointer
(`addiu v0, a1, N`) emitted before the stores, not in the `jr` delay slot.
Achieved by computing the return value into a named temporary before the zero
store, plus a trailing no-op pointer dance (`arg1 += N; arg1 -= N;`) mirroring
`func_8009CF1C` (`arg1++; arg1--;`). The zero store then lands in the `jr`
delay slot naturally.

## Array-vs-pointer global declarations affect indirection (func_8007024C)

m2c emitted `D_80121D80->unk24` for `lw t8, 0x24(v1)` where `v1 = %hi/%lo(D_80121D80)`.
Declaring the global as `extern Player6E120 *D_80121D80` (a pointer) produced an
extra indirection (`lw t8, 0(v1); lw t9, 0x24(t8)`) — wrong. The global must be
declared as an **array** (`extern Player6E120 D_80121D80[]`) so the symbol address
is the struct base and `->field` compiles to a single `lw r, off(base)`.

## Viewing a shared global under a different struct via inline cast

`D_80121D80` is shared across files typed as `Player6E120[]` (position fields at
0x290+). `func_8007024C` instead reads offsets 0x1C/0x20/0x24 of player 0. Rather
than byte-pointer arithmetic, define a small view struct
`typedef struct { u8 pad0[0x1C]; s32 unk1C; s32 unk20; s32 unk24; } PlayerPos6E120;`
and access via `((PlayerPos6E120 *)D_80121D80)->unk24`. The cast is a no-op at
runtime and reproduces the exact `lw r, off(&D_80121D80)` codegen (verified 100%
both as a local-typed direct access and as repeated inline casts; the inline-cast
form was kept to mirror the matched sibling `func_80070198`).

## Instruction scheduling depends on statement order (func_80017168)

For a struct-init function that copies fields and relocates several
`field + base` offsets to pointers, IDO -O2 keeps a computed pointer alive across
an intervening load only when the source statements are in the right order. With
the `+0x10` (constant-offset) store emitted between two `field+base` stores, the
scheduler hoists the `addiu base,0x10` early and stores it late, matching the
target register allocation (t8 kept live across the `lhu` for the next field).
Writing the `+0x10` assignment immediately after the first `field+base` store
instead causes the scheduler to store it immediately (different temp register and
ordering) -- same semantics, ~78% score. The fix was purely reordering the four
pointer assignments to `unk18, unk14, unk1C, unk20`. Also: IDO emits
`addu r, field, base` (field operand first) from source `field + base`, not
`base + field`.

## Typed struct access from void*/s32 parameters

When a function's ABI signature is `void*, s32` (e.g. because the source arg comes
from a helper returning `s32` and the dest is an embedded sub-struct at varying
offsets), keep that ABI-compatible prototype and cast the parameters to a local
typed view struct at the top of the body: `DstView *dst = arg0; SrcView *src =
(SrcView*)arg1;`. The assignment is a register no-op (stays in $a0/$a1), so
codegen is identical to a directly-typed parameter while avoiding churn across
many callers and conflicting prototypes.

## Fixed-point matrix packing (func_800486BC)

A function that converts a 3x4 fixed-point source matrix (s16 cells at 0x00..0x10
plus s32 translation words at 0x14/0x18/0x1C) into a packed display matrix (16
s32 words) matched cleanly by declaring two view structs and writing each output
word as a single `low | high` expression. Key points for codegen:

- Source cells must be `s16` so reads emit `lh` and signed shifts emit `sra`
  (`>> 12`, `>> 16`). Translation fields are `s32` read with `lw`.
- `(field << 4) & 0xFFFF0000` emits `sll ...,4` + `and ...,0xFFFF0000` (lui-loaded
  mask); `(field << 4) & 0xFFFF` emits `sll` + `andi`. `(field << 20)` and
  `(field << 16)` emit `sll` with the matching shift amount.
- The OR operand order in the source matters for register allocation: write the
  low-16 part first then the high-16 part (`low | high`) to match the `or rd, low,
  high` ordering exactly. Casting `void*` args to local typed-view structs
  (`Src *src = arg0; Dst *dst = arg1;`) is a register no-op and matches the
  `void*, void*` ABI used by the callers.

## Callback counter with modulo scheduling (func_80039CEC)

A timer/counter callback in the Struct3A0E0 family (`func_80039CEC`) increments
`arg0->unk2A`, then tests `(value % 28) == 0` and `(value / 28) == 3`. The
literal m2c output (`u16 temp = arg0->unk2A + 1; arg0->unk2A = temp;`) reached
only ~95%: IDO materialized the masked value in a fresh temp register and
emitted two spurious `move` instructions, and the divisor constant 28 landed in
`a0` instead of `v1`, shifting every branch offset.

Two changes produced a 100% match:

- Declare the counter `s32 temp` (not `u16`). The mask `andi v0, t9, 0xffff`
  still emits because `unk2A` is `u16`, but the wider local lets the value live
  in `v0` directly with no shuffling.
- Use a compound assignment so the incremented value is captured in one
  expression: `temp = (arg0->unk2A += 1);`. This makes IDO keep the unmasked
  sum (`t9`) for the store and the masked sum (`v0`) for both the `%` and `/`
  divisions, eliminating the extra moves and letting the divisor constant settle
  in `v1`.

General takeaway: when a 16-bit field is incremented and both the new field
value and a widened copy are needed, prefer `temp = (field += n)` over a
two-statement `temp = field + n; field = temp;` to avoid spurious register
moves.

## Repeated global reads vs. hoisted temp (func_8004097C)

m2c hoists a repeatedly-tested global into a `u8 temp = global;` local and tests
`temp` several times. That matched only ~74%: IDO spilled the temp to the stack
(`sw v1,0x18(sp)` / `lw v1,0x18(sp)` around each call) and grew the frame from
0x18 to 0x20. The target instead re-reads the global directly in each `if`.

Reading the global directly each time gives the exact pattern: IDO loads the
global once into a register and reuses it across non-call checks, but reloads it
from memory after every call (because the callee could mutate it) — and never
spills. So when a global is compared several times with function calls between
the checks, prefer direct repeated reads over a hoisted temp local.

## Reassign-to-parameter + temp for shifted return (func_80097AE8)

`func_80097AE8` (sine-table lookup, `s16` arg, frameless leaf) needed two tricks
to match IDO -O2 -mips1 codegen:

1. The masked angle had to be written back into the parameter: `arg0 &= 0xFFF;`.
   Reassigning to the `s16` parameter makes IDO keep the sign-extended value in
   the `a0` "home" register (after the usual dead `sw a0,0(sp)` homing store),
   reproducing `move t7,a0` / `andi` / `sra a0,...`. Using a separate local
   (`s16 angle = arg0 & 0xFFF;`) put the value in `v1` instead and dropped the
   `move` — wrong registers, ~84%.

2. The table value's `>> 3` had to land in a temp (`t2`), with only the final
   `s16` narrowing reaching `v0`. Returning the expression directly loads the
   table straight into `v0` and shifts in place. The fix is to capture the
   shifted value in a named local first, then return that local. This separates
   the load+shift (into `v1`/`t2`) from the return narrowing (into `v0`),
   matching `lui v1` / `lh v1` / `sra t2,v1,3` / `sll t3` / `sra v0`.

General takeaway: when a return expression's intermediate computation must
occupy a temp register distinct from `v0`, bind it to a named local and return
that local rather than returning the expression directly. And when a narrow
parameter is narrowed further in-place, reassign to the parameter instead of
introducing a new local so the value stays in the argument's home register.

## Operand order in `a + b` controls register allocation (func_8004FA44)

`func_8004FA44` initializes three position fields (`0x18/0x1C/0x20`) each as
`array_value + ((func_800430D0() - 0x80) << 10)`. The function calls
`func_800430D0()` three separate times (it returns a varying/random value, so
the result can't be reused), and reloads the `0x10` halfword index before each
array lookup (the call could clobber it).

After getting control flow and instruction selection correct, the match sat at
~93% purely on register-allocation differences in the three add blocks. The
winning change was purely the **operand order of the final `+`**: writing
`array_value + shift` (array value as the first/left operand) instead of
`shift + array_value` flipped IDO's register assignment for the `addu` operand
pair to exactly match the target across all three blocks → 100%.

Takeaway: for `+` (commutative), IDO -O2 assigns the destination/operand
registers based on source operand order. When a match is functionally perfect
but differs only in which temp register holds each operand of an `addu`, try
swapping the order of the two addends in the C source. The decomp-permuter
surfaced this; a `& 0xFFFFFFFF` mask the permuter also suggested turned out to
be unnecessary once operand order was fixed — so verify each permuter
suggestion in isolation rather than importing the whole batch.

## Statement grouping can affect independent setup scheduling (func_80018BC0)

`func_80018BC0` matched only when the initial loop setup kept the original
single-line grouping:

```c
base = arg0; i = 0; if (D_80121B55 > 0) { player = D_80121D80; do {
```

Splitting those assignments and the `if` over separate statements produced the
same control flow and register choices, but IDO swapped two independent `addiu`
instructions that initialized saved-register global bases. The ROM checksum
still failed even though the function was semantically identical.

Takeaway: when a diff is down to independent setup instructions being reordered,
try restoring the original statement grouping/formatting before changing
types or control flow. This can preserve IDO's scheduling without introducing
dummy dependencies or extra locals.

## Struct field signedness preserves `lh` vs `lhu` (func_80032534)

When replacing manual offset loads with typed struct fields, keep the field's
signedness aligned with the target load. In `func_80032534`, modeling the
animated scale at offset `0x1C` as `s16` changed the two target `lhu`
instructions into `lh`. Changing only that field to `u16` restored the exact
loads without changing the surrounding struct layout or source logic.
## main_menu_scene_actor_0 callback actor cleanup

- Renaming `src/33680.c` to `src/main_menu_scene_actor_0.c` and retyping
  `Struct33680` to `MainMenuSceneActor` can remain checksum-clean, but several
  callbacks still need their old local alias pattern (`MainMenuSceneActor *temp =
  arg0;`). Removing that alias from functions such as `func_80032A88` changes
  IDO's register choice and shrinks the function by one instruction.
- Raw stores like `*(u16 *)((s32)arg0 + 0x2A) = 0` and
  `*(s16 *)((s32)arg0 + 0x2C) = 0x16` can be replaced with `arg0->unk2A` and
  `arg0->unk2C` without affecting codegen as long as the surrounding local alias
  structure is preserved where the function previously had one.

## Signed hex bounds in actor movement clamps

When replacing manual offset loads with struct fields, keep explicit `(s32)`
casts on high hex comparison bounds such as `(s32)0xFFD00000`. Without the cast,
IDO treats the constant as unsigned and emits `sltu` instead of the target
signed `slt`, even though the field itself is `s32`.

## Overlapping array views in main_menu_effects

`func_8003C118` draws from a sliding three-point line stored across actor fields
`0x18..0x22`: `x = linePositions[frame]` and
`y = linePositions[frame + 3]`. A direct pair of array subscripts is cleaner but
IDO computes the indexed address twice and lengthens the function. The matching
shape keeps a typed actor overlay pointer advanced by
`frame * sizeof(linePositions[0])`, then reads `linePosition->x` and
`linePosition->offsetY`.

Also keep the frame index unsigned for this view. Using the signed `frameIndex`
field changes the target `lhu 0x24` into `lh 0x24`.

## Halfword-relative animation offsets and `addu` order

`func_80041DD4` reads a main-menu model animation bank whose frame offsets are
stored as halfword offsets relative to the start of the bank. A clean union
array expression like `&bank->frameData[bank->frameOffsets[index]]` is
semantically right, but IDO emits the final address add as `bank + offset`.
The target uses `offset + bank`.

For checksum-clean cleanup, keep the bank typed but write the address as
`offset * sizeof(s16) + (s32)bank` when the target operand order matters.

## Table setup scheduling in course asset loader

`func_800440F4` can be expressed cleanly with `RomAssetRange` tables and a
`CoursePlayerState` slice, but the best typed rewrite currently stalls at
97.5%. The remaining diff is mostly independent setup scheduling: IDO moves the
player base and loop counter initialization before the three range-table base
`addiu`s, while the target initializes the range-table saved registers first.

For future matching, preserve the typed range/player structures, but focus on
statement grouping or local lifetime changes that influence the order of those
independent saved-register initializers.

## Avoid hoisting typed strided-array pointers across calls

In `func_8004FA44`, replacing repeated `D_80121EE8[playerIndex]` loads with a
single local `RaceItemEffectPlayerState *player` was cleaner but shortened the
function by `0x3C` bytes. IDO kept the local player pointer on the stack across
three `func_800430D0()` calls and reused it, while the target recomputes the
`0x60C` stride and reloads each coordinate after each call.

When cleaning pointer arithmetic around large strided arrays, typed indexing can
still match as long as each load is written independently. Avoid introducing a
local element pointer if the target visibly recomputes the stride between calls.

## Overlapping actor fields and halfword tables

`main_menu_overlay_effects` actors reuse offset `0x18` as a word, a halfword
pair, and a byte flag in different callbacks. A union with word/halfword/byte
views preserves the original `lw`, `lh`, `lhu`, and `sb` instruction choices
while removing manual offset casts from the function bodies.

For small coordinate tables like `D_800D57B0`, struct or two-dimensional-array
views can perturb IDO's temporary register choices. The checksum-clean form in
`func_80054714` keeps the data as a typed `s16` array and indexes
`playerIndex * 2`, then stores through typed actor halfword fields.

## Adjacent BSS labels as one state view

`player_select_menu` uses `D_8010AE70` as a small state struct, but
`func_80005788` also reads the halfword at `D_8010AE70 + 4`, which is the
adjacent `D_8010AE74` BSS label. A local struct view that includes the `+4`
field documents the access pattern without needing manual pointer arithmetic.

## Interior labels in strided tables

`race_course_effects` has `D_800DA804` as the start of a 0x14-byte table, with
additional labels such as `D_800DA80C` and `D_800DA814` pointing at fields
inside the first entry. A single local `CourseMarkerEntry *entry` is cleaner,
but IDO keeps that pointer in an extra saved register across calls and shrinks
`func_8006CBBC`.

For checksum-clean cleanup, keep the main table typed and add typed 0x14-byte
views for interior labels. This preserves the target's repeated stride
recomputation while removing raw byte-pointer math.

## Race camera strided arrays and independent base loads

`race_camera` can use typed `RaceCamera` and partial `RacePlayerState` views for
the `0xB0` camera slots and `0x60C` player slots, but two small functions are
particularly sensitive to IDO scheduling. In `func_8006F984`, typed slot access
causes IDO to keep the `0x60C` stride in `a2` and reload `a1` before calling
`func_8006D8B4`; the target reuses `a1` throughout. In `func_8006D700`, clean
loop forms preserve size but can swap the independent `D_801124A0` and
`D_801121E0` base initializers.

When cleaning this segment further, keep typed struct views, but expect local
lifetime and statement grouping to affect register choice even when the emitted
instruction count is unchanged.

## Byte signedness can affect constant lifetime

In `func_80083CFC`, typing the rider side field at `0x2DC` as `s8` caused IDO
to reuse the later `1` halfword constant for that byte store, hoisting
`li v1, 1` before the branch and shifting labels. The original byte-pointer
store matched when the field was typed as `u8`, letting IDO emit a separate
temporary for the byte assignment and load the shared halfword `1` after the
branch merge.

When replacing byte-pointer stores with struct fields, preserve observed
signedness where possible; even a same-sized `s8`/`u8` change can perturb
constant lifetimes.

## Renamed segments regenerate nonmatching asm paths

After naming a Splat C segment, the extracted `asm/nonmatchings/<segment>/`
directory follows the new segment name. Any remaining `#pragma GLOBAL_ASM`
lines in the renamed C file must point at the regenerated named directory, not
the old hex-address directory.

The asm processor also does not process `#pragma GLOBAL_ASM` correctly when the
pragma is hidden inside a C preprocessor conditional. Keep the pragma at top
level, and put typed exploratory C under a separate `#ifdef NON_MATCHING` block
below it if the default build still needs the assembly.

## Statement grouping can affect independent address setup

In `course_select_menu`, expanding compact object update loops into normal
multi-line blocks kept the same instruction count but swapped independent
`addiu` address materialization order for `D_800EC9C4` and `D_801121E0`.
Keeping the original grouped statement shape allowed typed struct fields and
arrays to match without returning to raw pointer math.

## Volatile padding can preserve leaf-adjacent stack frames

In `func_80062530`, the typed C matched all instructions except IDO shrank the
stack frame from `0x48` to `0x28`. Adding an otherwise unused
`volatile u8 padding[0x20]` local restored the target frame size without
changing scheduling or register allocation.

## Cross-translation-unit prototype mismatches on return type

func_80072938 is defined as a void function in game_audio.c (it forwards to
func_8009DDE4), but in race_flow.c the call site reads the return register
(bnez on v0 after jal). The original game declares it inconsistently across
translation units. C does not cross-check prototypes between TUs, so declaring
it as returning s32 locally in race_flow.c is correct and necessary for the
if (call() == 0) codegen; do not "fix" the definition in the other file.

## Extern pointer globals are reloaded between aliased accesses

For a sequence like `D_801235B8->fadeTimer -= 1; if (D_801235B8->fadeTimer == 0)`,
IDO re-emits the lui/lw of the D_801235B8 global between the store and the
re-read (it cannot prove the store through the pointer does not alias the
global itself). This is expected; writing the field access naturally in C
reproduces it.

## Stack-held pointer locals can preserve repeated handle lookups

In `func_800437F0`, keeping a local `s16 *` pointing into the asset handle table
matches the target's stack spill and reload around repeated `func_80043040`
calls. A structured view of `D_80112130` can still match cleanly as long as the
field at offset `0x40` is represented separately from the first 0x20 handles.

### Chained assignment produces `andi` truncation (func_800219E4)

When the original source uses a chained assignment to update a `u8` field and a
`u8` local from the same `int` value, IDO keeps the value in one register and
emits an `andi reg, reg, 0xff` for the local:

```c
state = arg0->sprite.bytes.state = 2;
```

compiles to:

```
li      $t1, 2
sb      $t1, 0x1C($a3)      # store to the u8 field
andi    $v1, $t1, 0xff      # truncate to u8 for the local
```

A plain `arg0->sprite.bytes.state = 2; state = 2;` instead emits `li $v1, 2`,
which mismatches. Reach for the chained-assignment form when the diff shows an
`andi ..., 0xff` against a register that was just stored via `sb`.

### Redundant reload shifts a switch value home register (func_800219E4)

IDO can home the switch-controlling value in `$v1` (with a `move $v0, $v1`
scratch copy for the later comparisons) instead of `$v0`. If the only diff is a
`$v0`/`$v1` swap on the loaded switch value, adding a redundant reload of the
field right before the trailing use forces `$v1` as the home register:

```c
state = arg0->sprite.bytes.state;   /* redundant: provably equal on all paths */
if (state == 4) { ... }
```

IDO proves the reload is redundant (the local already equals the field on every
path), so it emits no extra load — it only changes register allocation. This is
purely a codegen nudge, not a logic change.

### Cast variant when the trailing reload is absent (func_800196CC)

func_800196CC has the same shape as func_800219E4 (u8 state machine, case 1
reuses the literal `1`, `state = arg0->sprite.bytes.state = 2` chained
assignment) but its target has *no* `lbu` right before the final
`if (state == 3)` — so the redundant-reload trick above would add a load and
mismatch. The nudge that works here is a `(unsigned int)` cast on the trailing
comparison instead:

```c
if ((unsigned int)state == 3) { ... }
```

`state` is already zero-extended in the register from the `lbu`, so the cast
emits no extra instruction — it only reshuffles IDO's register allocation so
the hoisted case-1 constant `1` does not take the switch's `$v0` scratch copy
slot. Plain `if (state == 3)` regresses to ~97.5% (missing `move $v0, $v1` in
the switch dispatch). Reach for this when the only diff is the switch-dispatch
register plus a missing `move $v0, $v1`, and the target lacks a trailing field
reload.

### func_8002C9A0 — struct field signedness decides the twin's register allocation

`func_8002C9A0` is the `ShopMenuWidgetActor` twin of `func_800196CC` (identical
state machine, identical `(unsigned int)state == 3` cast and chained
`state = arg0->sprite.bytes.state = 2`). Copying only the function body from the
matched twin plateaued at ~98.4% with pure temp-register diffs (case-0 constants
`-0x88`/`1` landing in `t9`/`a0` instead of the target's `t9`/`t0`, and the case
1/2 constant temps shifted by one). The lone cause: the existing
`ShopMenuWidgetActor.sprite` union declared the byte as `s8 state`, whereas
`PlayerSelectWidgetActor` uses `struct { u8 state; u8 pad1D; } bytes`.

Reading an `s8` field into a `u8` local still emits `lbu` (the load matches), so
the difference is invisible at the instruction level — only IDO's
constant-materialization register allocation in the switch cases differs between
the `s8`-field and `u8`-field code paths. Changing the union to mirror the
player-select layout (`bytes.state` as `u8`) reached 100% with no other change.
Lesson: when a function is a known twin of an already-matched one, copy the
struct field layout verbatim, not just the body — field signedness drives
register allocation even when the generated loads/stores look identical.

### func_8002E114 — a clean twin copy when struct layout already matches

`func_8002E114` is the `ShopMenuWidgetActor` twin of `func_8001B6D8`
(`PlayerSelectWidgetActor`), with the same scroll-out state machine. Unlike
`func_8002C9A0`, the `ShopMenuWidgetActor` layout here already mirrored the
player-select struct (`sprite.index` as `s16`, `transition.bytes.state` as
`u8`), so copying the body verbatim from the matched twin — swapping only the
draw callback (`func_8002E0B0` for `func_8001B674`) and the actor type — matched
100% on the first real attempt. Lesson: when the struct field signedness/offsets
already align with the matched twin, a body copy is sufficient; no per-case
register tuning is needed.

### func_8002E32C — another ShopMenuWidgetActor scroll-out twin

`func_8002E32C` is the same scroll-out state machine as `func_8002E114` and
`func_8001B6D8`, differing only in the draw callback passed to `func_800483FC`
(`func_8002E250` here). Since the `ShopMenuWidgetActor` layout was already
correct from the `func_8002E114` match, copying the body verbatim and swapping
only the callback matched 100% on the first attempt. Lesson: the scroll-out
state-machine family (`sprite.index += 0x26` fade-in → wait for `D_80121D88` →
`x += 0x20` slide-out → `func_800716E4`) is now a known pattern across both
actor types; new twins in this family are body-copy-and-swap-callback.

### func_80027498 — CourseSelectWidgetActor scroll-out state machine

`func_80027498` is the `CourseSelectWidgetActor` member of the same scroll-out
state-machine family as `func_8002C390` (and the shop/player-select twins):
case 0 fades the sprite in (`spriteIndex += 0x26` up to `0x100`), case 1 waits
for a transition trigger, case 2 slides out (`x += 0x20`). The transition
condition here is `(D_80121D88 == 3) || (D_801235B8->screenState == 9)` (an OR
of two conditions, compiled as short-circuit `beq`/`bne` to a shared state=2
block), and the slide-out terminates with a paired `func_800716E4` /
`func_800291F0(7)` call sequence.

The structurally-correct body matched 96.6% with only register-allocation
differences (`state` landing in `v0` instead of `v1`, and a `move v1,v0`
instead of a memory reload in case 2). The permuter found the 100% fix: the
original source does **not** reload `state = arg0->state` inside case 2.
Instead it reloads once *after* the entire switch (`state = arg0->state;`
immediately before the post-switch `if`). That single post-switch reload lets
IDO keep the live `state` in `v1` and emit `lbu v1,0x1e` on the case-2 path.
Lesson: when a `switch` updates a local from a struct field in some cases but
not others, a single reload *after* the switch (rather than per-case) is the
shape IDO expects, and is the difference between a `move` and a `lbu` reload.

### `func_8001B520` — scroll-out state machine with a side-effect global write

`func_8001B520` (player_select_ui) is the same scroll-out state-machine family
as `func_8002B05C` (case 0 fades `spriteIndex` up to `0x100`, case 1 waits for
`D_80121D88 == 1`, case 2 slides `x -= 0x20` until `< -0xFF`). The one
structural difference from its twin: after the switch it writes
`D_8010AE74 = arg0->sprite.spriteIndex;` before the usual `state == 3` /
`func_800483FC` tail.

Even though the target asm shows `lh v0,0x1c(a2)` emitted inside *each* switch
case (rather than once at the merge point), the clean source form
`D_8010AE74 = arg0->sprite.spriteIndex;` placed *after* the switch matches
100% on the first try. IDO distributes that single post-switch field read
across the case branches itself — you do not need a per-case temp (`var_v0`)
to reproduce the per-case loads. This mirrors the `func_80027498` lesson: a
single post-switch read of a struct field is the shape IDO expects, even when
the resulting asm looks like it was hand-distributed.

### `func_8002FAB8` — scroll-out state machine with pre-switch global check

`func_8002FAB8` (shop_menu_ui) is the same scroll-out widget family as
`func_800196CC` (player_select_ui): state 0 slides `x` to a snap point, state 1
is a wait, state 2 slides off-screen, state 3 transitions to the terminal
state 4 which calls `func_800716E4`. The structural difference is a
pre-switch check: `if (D_801235B8->unk20 == 3 || == 9) state = 2;`, then the
state is re-read from the struct before the switch.

Two codegen points worth remembering:

- **Assignment-expression for u8 state propagation.** Case 3 needs the asm
  `andi v1,t7,0xff` (zero-extend the stored u8 into the state variable). Write
  it as `state = arg0->sprite.bytes.state = 4;` (assignment-expression), not as
  two separate statements (`...= 4; state = 4;`). The two-statement form emits
  `li v1,4`; the assignment-expression form emits the `andi` IDO expects. Same
  pattern used in `func_800196CC` case 1.

- **`(u32)state == N` to fix the state register.** With the pre-switch global
  check, IDO loaded the post-switch `state` into `v0` instead of `v1` (the
  register the target uses for the switch dispatch), leaving a v0/v1 swap
  through the whole dispatch (3 instructions). Casting the *final* tail
  comparison to `if ((u32)state == 4)` was the one-line fix that made IDO
  allocate the loaded state into `v1`. A pure register-allocation difference
  with no control-flow impact — the permuter solved it instantly, but the clean
  hand form is just the single cast on the tail comparison.

To support the `D_801235B8->unk20` access, `MainMenuState` (file-local typedef
in shop_menu_ui.c) gained an `s32 unk20;` field at offset 0x20. The same global
`D_801235B8` carries different file-local struct types across
shop_menu_ui.c / main_menu_ui.c / main_menu.c — that is the existing project
convention, so extending the shop-local typedef does not conflict.

- **Constant reused between switch dispatch and a later store fixes the
  v0/v1 split.** In `func_800152D0`, `switch (arg0->state)` over cases 0/1/2
  plus `D_801235B4 = 1;` in case 2. IDO loads the switch expression into `v0`
  and the case-1 comparison constant `1` into `v1` via the first branch's delay
  slot (`beqz v0,...; li v1,1`). Because that same literal `1` is stored to
  `D_801235B4` later, IDO keeps `v1=1` live across the whole function and
  reuses it for the `sw`. This in turn frees `v0` mid-function so the spawned
  child pointer (`arg0->child`) lands in `v0`, matching the target. Writing
  `switch (arg0->state)` directly (no `u8 state = ...` local) was what produced
  the `v0`-for-expression / `v1`-for-constant split.

- **Declare a discarded-return call with its real return type to fix register
  allocation.** `func_80072138` actually `return 0` (ends with `or v0,zero,zero`)
  but the project declares it `void` and discards the result. With the `void`
  prototype, IDO allocated `arg0->x` into `v0` (wrong); declaring it
  `extern s32 func_80072138(s32, s32);` in this TU made IDO put `x` into `v1`,
  matching the target. The callee is unaffected (defined once elsewhere); only
  the caller's view of the return register changed. This is safe per-TU.

- **Named local for a dereferenced pointer field.** `arg0->child->state = 2;`
  written directly allocated the child pointer into a `t` register; writing
  `child = arg0->child; child->state = 2;` (named local) landed it in `v0`,
  matching the target. Same access, different register preference.

- **Permuter found the 100% match here**, but its output used artefacts
  (`new_var = arg0` alias, `unsigned short` return on `func_80072138`). The
  clean hand form was: drop the alias, keep the named `child` local, and use the
  truthful `s32` return type. Always reduce permuter output to the minimal
  logical change before integrating.

- **Check for a near-identical sibling function before adding complexity.**
  `func_80015680` was byte-for-byte identical in structure to the already-matched
  `func_800152D0` (same scroll-out state machine), differing only in three
  constants: the spawned child callback (`func_80015B20` vs `func_800157B4`), a
  comparison value (`D_80121B55 == 4` vs `== 3`), and the draw callback
  (`func_8001543C` vs `func_8001508C`). Copying the reference's exact form (typed
  `MenuItemActor *` arg, named `child` local, no `s16 x` temp) gave an immediate
  100% match. Earlier attempts that introduced a `s16 x` temp / `void*` arg
  stalled at 97.7% from register-allocation drift.

- **A uniform address shift in `--find-first-mismatch` (e.g. all data/code
  symbols offset by 0x2F0) points at an unrelated size change elsewhere in the
  TU list, not your function.** Verify by stashing unrelated dirty files and
  rebuilding before chasing a phantom mismatch in your own code.

- **`u8` switch-dispatch locals can cause register-allocation drift (state held
  in v1 plus an extra move v1,v0), but a 32-bit local (u32) holds v0.**
  func_80021C98 (character-select widget scroll-out state machine, sibling of
  func_8002B8B4) matched at 98.3% with u8 state; the only diff was the dispatch
  variable living in v1 instead of v0 plus a leading copy that shifted every
  branch target by 4. Changing the local to u32 (keeping the compound
  state = arg0->sprite.bytes.state = N; form for cases that set then test it)
  let IDO keep it in v0 for a perfect match. The u8 field reads/writes are
  unaffected; only the local type matters.

- **For an `if/else` ternary-threshold over two globals, hoisting only the
  second global into a local (not the first) reproduces IDO's exact register
  choice.** In func_80023198 (character-select widget scroll-in state machine),
  case 0 picks an x threshold from `(D_800EC9C2 == 2) || ((D_800EC9C2 == 1) &&
  (D_80121B5E == 0))`. The target loads `D_800EC9C2` into v0 (inline, used once
  per comparison) and `D_80121B5E` into v1. Writing `unk = D_80121B5E;` as a
  hoisted local while referencing `D_800EC9C2` inline gives v0/v1 correctly
  (99.9% in the workspace, 100% once integrated with the named jumptable).
  Pre-loading both globals into locals pushes the second into `a0` (wrong);
  referencing both inline defers the second load into the `&&` instead of
  hoisting it before the first comparison. A clean `if (...) { target = -0x8A; }
  else { target = -0x88; }` also reproduces the otherwise-mysterious unreachable
  dead `li v0,-0x88` IDO emits after the true-branch store — no comma
  expressions or `?:` hacks needed.

- **IDO reloads a global array element after any store through an unrelated
  pointer due to aliasing, and picks `sltiu` vs `slti` from the operand's
  signedness, not the comparison direction.** In func_80057710 (race UI prompt
  timer), the source reads `D_8012207C[arg0->index].flags` (a 0x60C-stride
  per-player flag block) in two separate `if` conditions. After stores to
  `arg0->timer` (through the actor pointer), IDO conservatively recomputes
  `&D_8012207C[index]` *and* reloads `index` from the actor (the store may alias
  it); on the path with no intervening store it reuses the cached flags in v0.
  Writing the natural source (two independent `if (...flags & ...)`) reproduces
  this exactly, with no manual temp variables. The timer thresholds against
  `0x1E` emit `sltiu` only when the field is `u32`; a signed field yields
  `slti`, so the field had to be `u32` here. `D_8012207C` is
  `&D_80121D80[0].flags` (player-state base + 0x2FC) reused as its own
  0x60C-stride array base.

- **When a stride value is both passed to a callee and used to index an array,
  IDO 5.3 keeps it in a single register (`a1`) only if the multiply explicitly
  references the stride *variable*; array indexing (implicit `sizeof`) makes it
  spill to a second register.** In `func_8006F984` (race camera update), the
  camera focus is copied from `D_80121D80[playerIndex].state.cameraPos` (three
  component stores) and then `func_8006D8B4(D_80121D80, stride)` is called with
  `stride = 0x60C`. The target emits a single `li a1,0x60c` early and reuses
  `a1` for all three `multu` (playerIndex * 0x60C) plus the call arg. Writing
  the access as plain array indexing (`D_80121D80[idx].state.cameraPos.x`) makes
  IDO load `0x60C` into `a2` for the `sizeof`-derived multiplies and a separate
  `a1` for the call (96.9% match, just a2-vs-a1 renames). Writing the access as
  explicit stride-based pointer arithmetic — `((RacePlayerState *)((u8
  *)D_80121D80 + playerIndex * stride))->cameraPos.x` — forces the multiply to
  use the stride variable, so IDO allocates it to `a1` throughout and matches
  100%. The codebase genuinely threads a `stride` parameter (`func_8006D8B4`
  takes `RacePlayerSlot*, s32 stride`), so stride-driven arithmetic is the
  natural source here, not a decomp artefact. (decomp-permuter instead found a
  dead `if (!D_80121D80) {}` block that also perturbs allocation to 100% —
  rejected as a nonsensical artefact in favor of the stride variable.)

- **Referencing a global directly in multiple conditions (vs. caching it in a
  local) changes IDO 5.3's `lui` base register for the load.** In
  `func_80022198` (character-select text draw), the global `D_80121B5E` (a `u8`
  mode selector) is read in three conditions (`< 2`, `== 0`, `== 1`). Caching it
  as `u8 mode = D_80121B5E;` compiled to `lui v0,%hi(...); lbu a1,%lo(...)(v0)` —
  a separate scratch (`v0`) for the address high bits. The target instead emits
  `lui a1,%hi(...); lbu a1,%lo(...)(a1)` — the destination register doubles as
  the `lui` base. Referencing `D_80121B5E` directly in each condition lets IDO
  CSE the repeated reads into a single load that lands directly in `a1`,
  matching 100%. The value is still loaded only once (one `lbu`); the
  difference is purely which register holds the `lui` high half. Generalizable
  pattern: when a small global is used 2-3 times and the only diff is the `lui`
  scratch register, drop the local and read the global inline.

- **Taking the address of an otherwise-scalar local (`&pfs`) shrinks IDO 5.3's
  stack frame.** In `func_80001584` (controller-pak init/repair), a `pfs`
  pointer local that is only spilled to and reloaded from one stack slot
  produced a `0x28`-byte frame, but the target uses `0x20`; the only difference
  was frame sizing (every slot offset shifted together). decomp-permuter found
  that introducing `OSPfs **sp18 = &pfs;` and passing `*sp18` to the call
  forces IDO to classify `pfs` as an address-taken local, allocating it into
  the addressed-locals region and collapsing the frame to `0x20` (100% match).
  This is an IDO frame-allocation quirk, not a semantic change. The codebase
  already tolerates such layout hacks (`func_800340D8` uses a
  `struct { s32 ret; s32 pad; } locals;` to control layout). When the only diff
  between a 99% match and 100% is the `addiu sp` prologue/epilogue magnitude
  and all slot offsets shift together, look for an address-taken (or
  struct-padded) local to retune the frame.

- **IDO 5.3 forwards a just-stored scalar field to later reads but re-emits the
  mask, producing a redundant `andi`.** In `func_8003C180` (main-menu effect
  update), `arg0->introTimer` is incremented, stored, then read again in two
  later conditions. The target emits the load/add/store once, then for each
  later `(u16)arg0->introTimer` reference it re-derives the value from the
  forwarded store register (`andi v0,t7,0xffff`) instead of reloading from
  memory. Writing the source as direct inline field reads reproduces this
  exactly; caching the value in a local makes IDO CSE the redundant `andi`
  away and mismatches. Generalizable: when the target has a redundant-looking
  `andi rX,rY,0xffff` after a store of `rY`, the original likely re-read the
  field inline rather than caching it.

- **Signed vs unsigned halfword field access selects `lh` vs `lhu`.** In the
  same function the `0x24` field is a union of `s16 frameIndex` / `u16 uFrameIndex`. The target loads it with `lhu` (unsigned). Accessing it as the
  `s16` member compiled to `lh`; using the `u16 uFrameIndex` union member
  compiled to `lhu` and matched. When a union field's load sign differs from
  the target, switch union members rather than adding casts.

- **Address-taken local forces IDO to reload from stack, defeating
  address-register CSE for globals.** In `func_80048388` (render arena bump
  allocator), the natural source `temp = D_801121B8 + ALIGN4(size); ...
  D_801121B8 = temp;` lets IDO CSE the global's address into one register
  (`lui rX,%hi; addiu rX,rX,%lo`) and reuse it for both the load and the store.
  The target instead recomputes the address twice (`lui t7,%hi; lw t7,%lo(t7)`
  for the read, `lui at,%hi; sw ..,%lo(at)` for the write) and reloads the
  computed value from its stack slot before the store. Writing
  `s32 *p = &temp; ... D_801121B8 = *p;` (semantically `= temp`) marks `temp`
  as address-taken, forcing it into the addressed-locals region and a reload on
  every use — reproducing the target's separate `lui`/`sw` and a 100% match.
  Same family as the address-taken-local frame trick above; here it controls
  reload/CSE behavior rather than frame size.

- Statement ordering within an independent block can flip IDO's register
  allocation even when the data flow is identical. In func_800601F8, the
  `unk54 += 1` halfword store had to be placed *after* the `unk4C = temp - 0x8000`
  store (rather than right after the `temp = unk4C` load) to reproduce the
  target's register/scheduling choices. All statements were independent and
  semantically reorderable — only the textual order changed — yet it took the
  match from 93% (register-only diffs) to 100%. When the only diff between a
  candidate and the target is temp-register naming on otherwise-identical
  instructions, try permuting the order of independent statements before
  reaching for the permuter.

- IDO's -O2 scheduler can also be sensitive to source line grouping in ways that
  look like pure whitespace. In func_8000BEC0, writing the loop preamble on
  separate lines emitted three addiu materializations in the wrong order
  (99.46%). Joining the call, the var_s0 assignment, and the do-loop header onto
  one line reordered the schedule to 100%. The tokens were identical; only line
  breaks differed. When stuck on a pure instruction-reordering diff at 99%+,
  let decomp-permuter search for the grouping the scheduler wants.

- A narrowly-scoped volatile cast can preserve register lifetime without
  perturbing neighboring functions. In `func_8009C6DC`, declaring
  `D_800DF158` itself as volatile produced the desired address register reuse
  in that function, but changed codegen in the following `func_8009C77C`.
  Keeping the extern as `s32` and writing only the two stores as
  `*(volatile unsigned int *)&D_800DF158 = ...` preserved the target's live
  address register for the yield path while leaving other direct accesses to
  the same global non-volatile.

- In `func_80041D20`, a semantically clean `RomAssetRange { start, end }`
  table access compiled to the right loads but skipped the target's prologue
  `move $t6, $a1`, causing a register-coloring cascade and a 95% score. The
  matching source treats `D_800D4020`/`D_800D4050` as flat `RomAssetAddress`
  word tables and indexes the start/end words with `modelIndex * 2`; this
  makes IDO emit `move $t6, $a1` then `sll $v1, $t6, 3`. A typed overlay is
  still useful for unrelated state: declaring `D_80112130` as a contextual
  `MainMenuModelAssetHandles` struct lets the model handle slots be accessed
  as fields while preserving the same codegen as raw `D_80112130[index + 0x2D]`.

- `func_8002C800` (shop_menu_ui) is a near-twin of `func_8001952C`
  (player_select_ui): both initialize a "row actor" struct (s16[5] at 0x18,
  s16 at 0x22, s8 at 0x24/0x25/0x26) and register a callback via
  `func_80071824`. When a sibling function in another segment already
  matches, mirroring its structure verbatim (only changing the magic
  constant and the callback symbol) yields a 100% match on the first try.
  IDO -O2 unrolls the `for (i = 0; i < 5; i++)` over `unk18[i]` into the
  `addu $v0, $a0, 2` + sequential `sh` pattern, so the natural loop form is
  the correct source. The `void(*)(void)` callback passed to a `void *`
  param produces a benign Warning 709, identical to the reference function —
  leave it; it does not affect codegen or the ROM.

- `func_8002A27C` (player_count_select_ui): a widget state machine whose
  top-level logic reads as an `if / else if / else` chain over
  `(D_80121B5E >= (u16)spriteIndex) && (y != -0x48)` /
  `(D_80121B5E < (u16)spriteIndex) && (y != -0x140)` / fall-through.
  m2c emits a goto-laden recompute of the slt because the original factored
  the shared else block; the clean `else if` with `&&` reproduces IDO's
  exact branch layout (the `&&` short-circuit generates the bnez-to-else-if
  and the redundant recompute of `slt $at, $v0, $v1` in the delay slot /
  fall-through). Three details were needed for a perfect match:
  (1) cast the compared s16 field with `(u16)` so IDO loads it via `lhu`
  (without the cast it emits `lh` — a real opcode mismatch);
  (2) inline the field access in both comparisons rather than hoisting a
  local, so `D_80121B5E` is loaded first into `$v0` (a local declaration
  loads spriteIndex first and swaps the v0/v1 allocation);
  (3) declare the switch scratch as `int state`, NOT `u8 state` — the `u8`
  type pushes the constant `3` into `$a0` and the state local into `$v1`,
  whereas `int` leaves state in `$v0` and the constant in `$v1`, matching
  the target. The switch needs explicit empty `case 0:` and `case 5:`
  (both break) so IDO emits the 6-entry jump table (cases 1-4 alone produce
  an if-else chain instead). The only residual diff in the standalone
  workspace was the jump-table symbol name (`jtbl_800E0EC0` vs `.rodata`),
  a normalization artifact — the integrated build matches the SHA1.

- `func_8001A704` (player_select_ui): the player-select counterpart of
  `func_8002A27C` (player_count_select_ui). Identical state machine, with
  `D_800EC9C2` (u8) replacing `D_80121B5E` and `func_8001A490` replacing
  `func_8002A008` as the trailing callback. Lifting the matched
  `func_8002A27C` body verbatim with those two substitutions (keeping the
  `(u16)` cast on spriteIndex, the inline field accesses, `int state`, and
  the explicit empty `case 0:`/`case 5:`) produces a 100% match on the
  first attempt. Confirms the func_8002A27C learnings generalize across
  sibling widget callbacks in this UI module.

- `func_8001A270` (player_select_ui): a near-exact clone of the already-
  matched `func_8001A704` in the same file — same state machine, same
  struct accesses, only the trailing `func_800483FC` callback differs
  (`func_80019FFC` instead of `func_8001A490`). Copying the `func_8001A704`
  body verbatim with that single substitution matches 100% (the sole
  residual diff in the standalone workspace was the jump-table symbol
  name `.rodata` vs `jtbl_800E0B00`, a normalization artifact; the
  integrated build verifies via SHA1). Useful reminder: when a target
  function has a sibling already matched in the same translation unit,
  diffing the two target assemblies first (here they differ only in the
  callback address) avoids re-deriving the whole control flow.

- `func_8001F0B0` (character_select_ui): sibling widget callback of the
  matched `func_8001A270` (player_select_ui) — same per-state machine
  (slide-in on `y`, counter increment in state 3, exit slide on `x`),
  but with a 6-entry jumptable (states 0-5) instead of 5. State 3 splits
  the `D_80121D88` checks into two independent `if` blocks (==1 -> 4,
  ==7 -> 5) rather than an if/else, and there is a dedicated `case 5`
  that does `arg0->x += 0x20` then reloads state before the post-switch
  `state == 5 && x >= 0x94` exit check.

  Notable codegen quirk: under IDO 5.3 -O2, the compiler eliminates the
  state reload inside `case 5` (`lbu v0, 0x1e(a2)`) because nothing on
  the dispatch -> case-5 path writes `transition.bytes.state`, so it
  reuses the switch-discriminant register (emitting a `nop` in the load
  slot). The target ROM keeps the reload. The reliable way to force IDO
  to emit the redundant load is to take the field's address into a local
  pointer (`u8 *stateField = &arg0->transition.bytes.state;`) and read
  through it in case 5 (and the else-branch read). The aliased pointer
  access defeats the load-elimination pass without adding runtime cost
  (the pointer is folded away — the surrounding asm is byte-identical to
  the direct-access version except for the kept `lbu`). The randomized
  transformer found the same effect via an anonymous `union *new_var` plus
  a `(long)` cast, but the plain `u8 *` is cleaner. As with
  `func_8001A270`, the standalone workspace shows one residual diff
  (jump-table symbol name `.rodata` vs `jtbl_800E0C7C`), which is a
  diff-tool normalization artifact; the integrated build verifies via SHA1.

## func_8002AB24 (player_count_select_ui)

- **u16 field reads vs s16**: reading a halfword field with `lhu` (unsigned)
  while dividing it by 2 with the signed `sra`+`bgez` rounding pattern is
  produced by declaring the field `u16`. `u16 / 2` promotes to `int` (signed) in
  C, so IDO emits the signed-divide-by-2 pattern even though the load is `lhu`.
  An `s16` field would give `lh` (wrong) and a literal `u16`-local-only approach
  still loads the field with `lh`. The field type itself must be `u16`.
- **Same offset, two signednesses via union members**: the 0x1E halfword is
  written as a negative literal (`= -0x74`) by one function (needs `s16` so the
  store uses `addiu` sign-extension) but read/divided as `u16` (`lhu` + signed
  halving) by a bounce routine. Model this with two union members at the same
  offset (`s16 alpha; u16 step;`), not by flipping one type — flipping breaks the
  other function. `addiu` vs `ori` for the negative literal is the tell: asm-differ
  renders both as `li`, so it shows 0 diffs and hides the regression. A ROM
  byte-diff (`cmp -l`) catches what asm-differ normalizes away.
- **Top-condition register allocation (v0 vs v1)**: when D_80121B5E is live
  across the point where `state` is also read, IDO splits them across `v0`/`v1`.
  The winning structure reads `state` into a local once at the top of the
  `else` branch and reuses it for the `< 6` check, rather than re-reading the
  field inline — this forces `state` into `v0` and `D` into `v1`.

## func_8002D9EC (shop_menu_ui)

- Sibling of the matched func_8002AB24 (player_count_select_ui) and func_8001F0B0 (character_select_ui) -- same per-state widget machine (slide on y, counter bump in state 3, bounce via transition.counter, exit slide on x). The shop variant moves the exit-position check out of the case body and into the post-switch guard (state == 5 && arg0->x >= 0x94), so its case 5 only does arg0->x += 0x20 and then must reload item.bytes.state for that guard.
- Same redundant-load codegen quirk as func_8001F0B0: with no write to the state byte on the dispatch -> case-5 path, IDO 5.3 -O2 reuses the switch-discriminant register and emits a nop in the load slot. The documented fix (taking the field's address into a u8 *stateField local once at function entry and reading *stateField in case 5) defeats the load-elimination pass with zero runtime cost. Only the case-5 read needs the pointer; the else-branch and other case reads stay direct.
- As with the other widget siblings, the standalone workspace shows one residual diff (jump-table symbol .rodata vs jtbl_800E0FB0), a diff-tool normalization artifact; the integrated build verifies via SHA1.

## func_80020818 (character_select_ui)

- Another sibling of the func_8002AB24 widget state-machine, mirrored almost
  verbatim. The struct field twist is the same: the bounce halfword at 0x1E
  (`transition.alpha`) must be `u16` to get `lhu` loads + signed `/2` halving,
  while sibling functions write the overlapping `transition.bytes.state` (u8)
  fields unaffected by the union member's signedness.
- This variant's condition compares two globals (`D_80121B50 == D_8010AE80`)
  instead of global-vs-constant. Operand order matters for register allocation:
  writing `D_80121B50 == D_8010AE80` (s16 first, u16 second) yields the target's
  `lh a1` / `lhu v1` assignment; the reverse order swaps `a1`/`v1`. Match the
  reference function's operand order literally.
- Same case-5 load-elimination nop quirk: fixed by taking
  `u8 *stateField = &arg0->row.bytes.subState;` at function entry and reading
  `*stateField` in case 5 only.
- Standalone workspace again shows one residual diff (jump-table symbol
  `.rodata` vs `jtbl_800E0CF4`), a diff-tool normalization artifact; the
  integrated build verifies via SHA1.

## IDO 5.3: keeping a base pointer for a single struct-field access

When you only access one member of a sub-struct/union (e.g. `arg0->angleVelocity`
at offset 0x22, base `angle` at 0x20), IDO -O2 folds a local pointer like
`s16 *t = &arg0->angle; t[1]++;` down to direct addressing (`lh 0x22(a0)`),
producing a `nop` in the would-be delay slot.

The target instead materialises the base (`addiu $v0, $a0, 0x20` then `lh 2($v0)`)
— a pattern IDO only emits when the pointer assignment is hoisted high enough to
have unconditional lifetime. Move the `t = &arg0->member;` assignment **out of the
deeply-nested conditional block** and up to a block that always executes; IDO then
keeps the base pointer and reuses it for every `[1]` access.

Seen in `func_8003DCCC` (and the sibling `func_8003DBE8`, which shares the same
author/pattern). The decomp-permuter is the quickest way to surface this hoist.

- `func_8002A710` is the same player-count select widget state machine as
  `func_8002A27C`, with the render callback changed from `func_8002A008` to
  `func_8002A49C` and the initializer setting the 0x1C halfword to 2 instead
  of 1. As with `func_8002A27C`, the `state < 4` fallback belongs only in the
  final `else` path where the actor is already at its target y-position; moving
  it after the initial branch chain creates dead movement states and no longer
  reflects the target assembly. The standalone workspace reaches an
  instruction-identical 99.915% score, with the remaining diff caused by the
  target's named `.late_rodata` jump table (`jtbl_800E0ED8`) versus IDO's
  unnamed generated `.rodata` table.

## func_80041FB4 (main_menu_scene_model)

- The target keeps the model pointer live across `func_80041E90` by storing it
  to the stack, then reloads it for the post-call `framesRemaining == 1` check.
  A deliberately separate model local plus a zero local (`0 & 0xFFu`) preserves
  the target's branch shape and zero return setup; simplifying to direct
  `return 0` / `return 1` control flow is likely to collapse labels and change
  scheduling.

## func_8007BDE4 (race_position_tracker)

- A chained `if (x == 0/1/2/3) return K;` over an argument followed by a single
  table-lookup return can compile to slightly different IDO register allocation
  depending on how the lookup is written. Writing the lookup as one big
  expression causes IDO to load the table base into a t register, leaving
  pathIndex in a1 and emitting an early shift in the first branch delay slot.
- Hoisting the table pointer into a named local (`s8 *entry = table[idx];`)
  makes IDO reuse a1 for the table base load (lui/lw), which forces it to
  preserve pathIndex with an early `move a2, a1` in the first delay slot and
  defer the index shift to the last bne delay slot, exactly matching the target.
  The sibling func_8007BCFC shows the same preserve-the-path-index-in-a-higher
  arg register pattern (or a3, a1, zero).

## Libultra VERSION_I Audio

- VERSION_I libultra audio C objects may require `-O3`. The local
  asm-processor wrapper only accepts up to `-O2`, so per-object `-O3` audio
  matches should compile through the direct local IDO driver.

## func_80099464 (input_task_scheduler)

- For the linked-list removal/free-list return path, IDO's register allocation
  only matched when the byte counter decrement was written as
  `(D_80123700 & 0xFFu) - 1`, and the free-list index kept an intentionally
  redundant chain of `& 0xFFu` masks before the final `u8` cast. Simplifying the
  expression to `D_80123700 - 1` or a single mask preserves behavior but reuses
  `v1` for the byte load and misses the target's `t0`/`t1` allocation.

## func_8009B5F4 (game_boot)

- The target loads `D_800DF140` with `lh`, so the fade timer should be treated
  as signed halfword in this file even though nearby setup functions only store
  positive constants into it.
- Returning the first display-list command pointer makes IDO keep
  `D_80124830`'s address in `a3` and the first command pointer in `v1`, matching
  the target's opening sequence. A void function with the same side effects
  instead reuses `v0`/`a2` for the display-list globals and drifts further from
  the target.

## func_80013F88 (menu_rendering)

- For small helper functions that narrow arguments, an old-style definition can
  be required to reproduce IDO's argument home stores and delayed narrowing.
  `func_80013F88` matches only when `step` is declared as `s16` and `increase`
  as `u8` in the K&R parameter declaration; keeping a prototype in scope before
  the definition changes the entry sequence and breaks the match.
- Writing the decrement path as an empty `if (increase) { } else { step = step *
  -1; }` preserved the target branch shape and temporary-register negation.
  Simplifying it to `if (!increase) step = -step;` kept the behavior but missed
  the final register allocation.

## func_8002F854 (shop_menu_ui)

- Shop menu widget actors can overlay their ordinary widget fields at
  `0x18..0x24` with a ten-element `s16` position array at `0x18..0x2A`.
  Initializers and state transitions that walk all ten slots match cleanly when
  this is represented as a union with `s16 cursorPositions[10]`, avoiding raw
  pointer arithmetic while preserving the existing named field accesses.
- A simple `for (i = 0; i < 10; i++)` loop over that array emitted the target's
  compact pointer-increment loop for setting every slot to `-0xFC`; no manual
  unrolling was needed.

## func_8000D340 (race_to_main_menu_transition)

- The transition initializer is straight-line setup code, but IDO is sensitive
  to the small globals around `D_8010B1A8`. The target clears `D_8010B1A8` and
  then computes `&D_8010B1A8 + 1` before clearing the next four bytes in the
  order `+1`, `+2`, `+3`, `+0`; writing the maintainable version as named
  globals (`D_8010B1A9` through `D_8010B1AC`) is clearer but does not preserve
  that exact register schedule.
- Introducing a temporary for the transition state fade read after setting
  `D_801235B8->fade = 0xFF` made IDO keep `&D_801235B8` in `a2`, matching the
  target's final block more closely. It can shift unrelated stack local slots,
  so asset-size spill offsets should be rechecked after adding such temporaries.

## func_8003BC9C (main_menu_debug_ui)

- Declaring the pressed-button global as `s32 D_80123778[]` and indexing
  `D_80123778[0]` made IDO keep the global base address live in `v1`, matching
  the target's repeated button checks. A scalar `s32` declaration emitted direct
  reloads or kept the base in `a1`.
- For the debug object `x` movement, direct `arg0->x` comparisons and updates
  matched the target allocation better than introducing local `s16 x` temporaries
  for both directions.

## func_8003A944 (main_menu_scene_actor_3)

- The actor field at offset `0x2C` is `animTimer`, not `timer`; using the
  existing struct field avoids manual offset access and fixes the target's
  initial `lhu`.
- Adding an otherwise unused local before the saved `func_80041FB4` result can
  move that spill from `sp+0x24` to the target's `sp+0x20`. The remaining
  mismatch is a redundant unconditional branch that only schedules
  `count = 2` in the delay slot.

## func_8006BB50 (race_course_effects)

- The course moving-effect initializer uses `RaceMovingEffect` offsets:
  velocity at `0x18`, position at `0x24`, matrix scratch at `0x30`, and timer
  at `0x50`. `CourseSpawnEntry` has 0x48-byte entries with position fields at
  `0x20..0x28`.
- Repeated volatile reads of `D_80121B50` are needed to get IDO to emit the
  target's three separate `multu index, 0x48` sequences for the course-position
  offsets. A direct `D_800B9540[D_80121B50]` typed access is cleaner but IDO
  CSEs the entry pointer and misses the target shape.
- Decomp-permuter improved register allocation from the clean typed candidate,
  but its best output added an empty post-call `if` that only perturbed
  registers. Do not carry that artefact into source.

## func_8006C088 (race_course_effects)

- This is another course moving-effect initializer. It also uses
  `RaceMovingEffect.unk52` as a stored rotation/angle halfword after
  `timer` at `0x50`.
- `D_800B9554` should be typed as a 0x48-byte entry table with the angle at
  offset `0x0`, not as a flat `s16[]` indexed by `course * 0x24`; the typed
  table makes IDO reuse the `course * 0x48` byte offset register and matches the
  target exactly.
- `CourseSpawnEntry` has another vector at offsets `0x08..0x10`, separate from
  the previously used position vector at `0x20..0x28`.

## func_8003048C (controller_pak_menu_ui)

- `D_8010AF80` is the same small prompt transition shape used by the shop menu:
  a halfword `x` at offset `0x0` and byte `state` at offset `0x2`. The function
  copies that halfword directly into the title actor's scale field when the
  cached state changes.
- The closest typed candidate is sensitive to load scheduling around
  `D_8010AF80.state` and `arg0->blinkState`: assigning the global state
  temporary before reading the actor state gives the right `$v0/$v1` allocation,
  but IDO schedules the two `lbu` instructions in the opposite order from the
  target.

## func_80021B58 (character_select_ui)

- The character-select 2x2 panel draw helper matches the course-select
  `func_80025FA8` pattern exactly: four straight-line `func_8000F030` calls with
  fresh `func_80043040(handle)` loads for each tile, signed 16-bit casts only on
  the `+ 0x40` coordinate variants, and tile indices 3 through 6.

## func_800218A4 (character_select_ui)

- This function is the same 2x2 character-select panel draw pattern as
  `func_80021B58`: four `func_8000F030` calls using `arg0->x`, `arg0->y`,
  `func_80043040(D_80112172)`, 0x20 tile dimensions, and tile indices 3 through
  6. The explicit `(s16)` casts are only needed on coordinates with the `+ 0x40`
  offset.

## func_8001958C (player_select_ui)

- This player-select draw helper matches the same four-call 2x2 panel pattern as
  `func_800218A4`, using `D_80112172` and tile indices 3 through 6. Keep the
  explicit `(s16)` casts on the `x + 0x40` and `y + 0x40` coordinate arguments;
  the unoffset `x` and `y` fields should be passed directly.

## func_8005475C (main_menu_overlay_effects)

- A two-word display-list cursor typed as a struct can match the raw
  `D_80124830` command writes cleanly. For this function, IDO's instruction
  scheduling depends on keeping the display-list writes and draw calls inside a
  single-line `do { ... } while (0)` block; formatting the same block across
  multiple lines changes the constant-load ordering and misses the target.
- The empty `if (arg0) { }` after the first two draw calls is behaviorally inert
  because `arg0` has already been dereferenced. It forces IDO to home and reload
  the actor pointer in the target pattern.

## func_80031D3C (controller_pak_menu_ui)

- The render actor near the controller pak delete prompt has a different
  `0x1C..0x20` field layout than `ControllerPakDeletePromptActor`: signed
  `scale` at `0x1C`, `u16 timer` at `0x1E`, and byte `selectedOption` at
  `0x20`. Reusing the delete-prompt struct swaps the timer and selected option
  loads and also changes signedness in `func_80032534`.

## func_80054EC4 (main_menu_overlay_effects)

- `MainMenuOverlayEffectActor` offset `0x30` is used both as a full `s32` angle
  accumulator and as its low halfword at offset `0x32`; model it as an
  `OverlayActorWord` union to avoid manual offset access.
- The typed candidate reaches 99.44%. The remaining diff is pure temp-register
  allocation in the spawned actor copy block. Whole-union assignments force
  `$at` for the contiguous word copies, while `.word` assignments shift the
  temps to `$t*`; moving the `alpha = 0xC0` store after `spriteIndex` improves
  scheduling but still leaves register-name drift.

## func_80055530 (main_menu_overlay_effects)

- The closest display-list setup candidate reaches 99.419%. Single-line
  `do { ... } while (0)` blocks are needed around the texture/palette setup tail
  to match IDO's constant/address load scheduling for `D_800DEE50` and
  `D_2000E70`.
- The remaining mismatch is register allocation around the two
  `func_80043040` calls: the target saves the command pointer in the delay slot
  from `$v1` and reloads it into `$a2`/`$a3`; the closest C forms either keep the
  pre-call `$v1` shape or the post-call reload shape, but not both.

## func_80053C90 (main_menu_overlay_effects)

- This is the same display-list setup pattern as `func_80055530`, but uses
  texture handle `D_8011214C`, palette handle `D_80112154`, and display list
  `D_20058A8`.
- The best confirmed candidate reaches 99.677%. An otherwise inert empty
  `if ((!D_80124830) && (!D_80124830)) { }` after loading the palette command
  pointer fixes the post-call `$a2`/`$a3` reloads, but moves the initial texture
  command cursor from target `$v1` to `$a2`, leaving a pure register-allocation
  mismatch.

## func_8006C698 (race_course_effects)

- This course effect initializer uses the same `D_800DA764[D_80121B50]`
  0x10-byte entry as both sound parameters and a position/angle record:
  `x/y/z` at offsets `0x0..0x8` and `angle` at `0xC`.
- For the local vector passed to `func_80098590`, declaring the saved matrix
  pointer before the `Vec3i` local places the vector at `sp+0x28`, matching the
  target stack layout exactly.

## func_8006D384 (race_course_effects)

- `D_800DA840` is a 0x1C-byte course trigger table: signed scale halfwords at
  offsets `0x0..0x4`, fixed-point world position at `0x8..0x10`, pitch/yaw at
  `0x14..0x16`, and a display-list pointer word at `0x18`.
- IDO places a 0x38-byte scratch struct at `sp+0x28`; adding an unused trailing
  word makes the local 0x3C bytes and moves the useful fields to the target
  offsets `sp+0x24` (dest), `sp+0x30` (source), and `sp+0x3C` (matrix).
- The first source-vector zeroing must assign `y` before `x` to match the
  target store order; the second zeroing uses `x` before `y`.

## func_8006EF1C (race_camera)

- Direct `RacePlayerSlot` array indexing plus compound `+=` assignments matches
  the focus smoothing update. Introducing explicit camera/player locals keeps
  the same behavior but shifts the long-lived globals into later argument
  registers.
- `func_8006D8B4` must be declared old-style in this file: `func_8006F984`
  still passes the live player base and stride, while `func_8006EF1C` calls it
  with no explicit arguments and leaves the existing temps in `$a0/$a1`.

## func_8002F8DC (shop_menu_ui)

- The course selection globals `D_8010AF72` and `D_8010AF73` need unsigned-byte
  declarations here to produce `lbu` loads for the shop course-icon window draw.
- An otherwise unused `s32` local before the `u16` tile index places the halfword
  temporary at `sp+0x3A`, matching IDO's stack layout for the two
  `func_8001061C` calls.

## func_8002E9E4 (shop_menu_ui)

- The closest structured candidate reaches 94.119%. The target keeps the shop
  menu state base `D_80121D80` in `$a1` and the price decrement amount in `$a0`;
  equivalent C forms tested so far put the state base in `$a0` and the amount in
  `$v0`, shortening the decrement branch tree by one delay-slot pattern.
- Actor byte `0x25` is the slide/purchase timer paired with slide state byte
  `0x24`; modeling it on the slide union avoids using the unrelated transition
  timer at `0x1F`.

## func_80077DA0 (race_flow)

- When one global halfword needs both signed and unsigned loads in a function,
  a local `s16`/`u16` union view can preserve `lh` for signed table indexing and
  direct `lhu` for switch dispatch without introducing casts that emit `andi`.
- `D_800B9542` is indexed with a 0x48-byte stride here. A structured table entry
  with the leading `s16` field matches the address arithmetic cleanly.

## func_80048278 (render_asset_utils)

- For byte-string drawing loops, assigning the string pointer local before the
  y-coordinate local can push the pointer into `$s1` and the y-coordinate into
  `$s2`, matching IDO's preferred allocation for this function.
- A word-sized volatile character parameter on the callee declaration avoids an
  extra `andi` before `func_80047E88`, but it also flips the newline compare
  operand order; the best candidate still has only branch/register-order drift.

## func_8007BCFC (race_position_tracker)

- The closest structured candidate reaches 99.474%. Preloading the course/path
  table entry before copying `pathIndex` lets IDO schedule `move $a3, $a1` in
  the initial branch delay slot while preserving the real lookup after the
  special course early returns.
- Naming the loaded signed path byte as an `s8` local keeps the load signed and
  preserves the clamp structure, but IDO still chooses a temporary register for
  the byte before shifting where the target reuses `$a1`.

## func_80018060 (race_hud)

- The best functional candidate reaches 75.509%. A typed `RacePlayerState`
  entry with a 0x78F8 stride and byte `flags` at offset `0x4B` matches the
  data access pattern, while `RaceHudMessageActor` needs byte views for offsets
  `0x22/0x23` in addition to the existing `speedY` halfword.
- The remaining blocker is global byte store codegen. C forms using scalar
  externs, volatile externs, volatile casts, array-style externs, and address
  casts all caused IDO to hoist `D_8010AE5E`/`D_8010AE5F` addresses or spill the
  actor pointer. The target emits direct `lui $at`/`sb` pairs inside the loop
  while keeping the actor in `$a0`, player cursor in `$v1`, and loop counter in
  `$v0`.
## func_8007276C audio handle restart

- For this function, keeping a local pointer to `D_80121B08[playerIndex]` produced identical instructions but an oversized `0x30` stack frame. Re-indexing `D_80121B08[playerIndex]` at each use let IDO create the target temporary stack slot and matched the `0x28` frame exactly.
- The race player record position used for positional audio is at offset `0x1C`, and the player record stride is `0x60C`.

## func_80000450 (main_menu)

- The best functional candidate reaches 85.817%. The controller count loop
  matches when written as a label/goto post-increment test over `D_800B30F0`;
  structured `while` forms introduce an extra pre-test.
- The remaining blocker is BSS-adjacent global store codegen. Scalar externs
  emit extra `lui` instructions for labels like `D_800E4C1A`, while struct,
  array, volatile, and constant-offset cast forms all make IDO materialize a
  base pointer. The target reuses `$at` high halves and emits direct
  symbol-relative stores for the controller state bytes.

## func_8009F4C8 (player_commands)

- This RNG scale helper only consumes its first argument even though existing
  call sites pass script/state context. Matching required an old-style
  declaration and a one-argument definition; declaring the unused parameters made
  IDO spill `$a0`-`$a2` to the stack.
- For the eight RNG advances, `for (var_v0 = 0; var_v0 != 8;) { var_v0 += 4; ... }`
  matched the initial address setup order. The equivalent `do` loop scheduled
  `move $v0, $zero` before the `%lo(D_8015A684)` addiu.

## func_8003112C (controller_pak_menu_ui)

- For adjacent `u16` alpha locals that must survive calls, keeping a local copy
  of the actor pointer and a no-op empty branch in the inactive path made IDO
  choose the target `v1`/`t0` allocation and `sp+0x3A`/`sp+0x38` halfword slots.

## func_80027408 (course_select_ui)

- When a `u16` tile index must survive a nested `func_80043040` call and become
  the fourth argument to `func_8000F8AC`, an extra unused `u16` local plus a
  promoted `u32` copy can make IDO keep the value in `$a3` and spill it at the
  target halfword stack slot.

## func_80021F80 (character_select_ui)

- This state-machine update hit 99.5% but wouldn't go perfect until two
  codegen-forcing tricks were applied. (1) Fold the global-state load into the
  comparison as `if (state != (globalState = D_8010AE88[1]))` rather than
  hoisting the load to its own statement — this pinned the load into the branch
  delay-slot ordering. (2) In `case 0`, write `arg0->transition.bytes.timer =
  state * 0;` instead of `= 0;`. IDO materializes the zero via `sb zero,`
  either way, but the multiply-by-state form reproduces the exact register
  assignment; a literal `0` changed the chosen register for the store.
- Operating on `arg0` directly (rather than copying it to a local `actor`
  pointer) was sufficient here — unlike `func_8003112C`, no extra live actor
  copy was needed to pin allocation.

## func_80023434 (character_select_ui)

- This function shares the global transition-state sync pattern from
  `func_80021F80`, but the shorter switch made IDO swap the current state and
  global state between `$a1`/`$v1`. Five empty `if (1) {}` no-ops immediately
  after storing `globalState` back to `arg0->transition.bytes.state` preserve
  the target allocation: global state in `$a1`, current/switch state in `$v1`.

## func_8002AE3C (player_count_select_ui)

- The `D_8010AF50` player-count cursor uses the same five-empty-`if` allocation
  trick as `func_80023434`: fold the global state load into the comparison,
  store it back to the actor, then place the no-ops before assigning
  `state = globalState`. This keeps the global state in `$a1` and the switch
  state in `$v1`.

## func_8002DCE8 (shop_menu_ui)

- This shop cursor callback shares the `func_8002AE3C` global-state sync shape,
  but also copies the global timer halfword into `transition.bytes.timer`. The
  same five empty `if (1) {}` statements after storing `globalState` preserve
  IDO's `$a1`/`$v1` allocation for the switch.
- The target's final global-state write reloads the field address with
  `lui $at`/`sb` instead of reusing the live `D_8010AF18` base in `$a3`. Writing
  through a byte alias symbol at `D_8010AF18 + 0x28` (`D_8010AF40`) produces
  the target instruction form. When adding such an alias inside a sized `.bss`
  symbol, remove the parent symbol's stale `size:` metadata so the range check
  does not report the intentional split as a mismatch.

## func_80020CEC (character_select_ui)

- For short initializer chains that derive two locals from one mode byte, using
  an `int` local for the loaded mode preserves the target `$v0` comparisons. An
  empty `if (1) {}` in the `mode == 0` arm and assigning the `mode == 2` row
  before `y = -0x60` reproduce IDO's branch-delay scheduling exactly.

## func_80030CC4 (controller_pak_menu_ui)

- This confirm-prompt transition callback follows the same folded global-state
  sync and five empty `if (1) {}` allocation pattern as `func_8002DCE8`. The
  best non-matching form still differs in the state-change block by one store
  scheduling issue: IDO emits the `optionScale = 0x100` halfword store before
  the `timer = 0` store, while the target stores timer, moves the switch state,
  then stores option scale.

## func_80099288 (input_task_scheduler)

- The render-buffer status byte at `D_8012496E` and `D_8013CF8E` matches cleanly
  as an array of structs with stride `0x18620` and a `status` byte at offset
  zero from the named symbol. This avoids hardcoded pointer arithmetic while
  preserving IDO's multiply-by-`0x18620` shift/subtract sequence.
- Declaring `func_8009B704` as taking a `u8` keeps `D_80123752` loaded into
  `$a0` for both the framebuffer-status index and the later call; a wider
  prototype makes IDO keep the value in `$v0` and emit `move $a0, $v0` in the
  call delay slot.
- After `func_8009B704`, writing `D_800DEED0 = D_800DEED4` before decrementing
  `D_80123750` gives IDO the target load schedule while still placing the delay
  store in the branch delay slot.

## func_80048BBC (render_asset_utils)

- For a 0x40-byte command block initializer, writing the computed `unk1C` field
  before clearing `unk20` lets IDO schedule the source `unk1C` load before the
  `unk20` zero store, while still emitting the target `unk1C` store afterward.

## func_8004885C (render_asset_utils)

- The allocation wrapper around the `func_800486BC` command transform matches
  cleanly when it uses the same typed `GfxCommandSource`/`GfxCommandDest`
  accesses and returns the allocated destination pointer directly.

## func_8004F33C (race_item_effects)

- When replacing a halfword field plus explicit padding with a word-sized union
  at the same offset, remove the old padding field. Keeping both preserves C
  validity but silently shifts later fields and breaks every access after the
  union.

## func_8007031C (race_camera)

- Direct `D_801124A0->field` accesses matched better than hoisting the camera
  pointer into a local. IDO reused the loaded global pointer for adjacent
  non-call work, then reloaded it after helper calls before later field reads.
  For chained camera expressions with calls in the middle, keep the global
  field accesses direct unless the target clearly preserves a local pointer.

## func_8003C634 (main_menu_effects)

- For small byte-state switches, switching directly on the struct byte field can
  match better than hoisting the state into a `u8` local. In this function,
  `switch (arg0->characterId)` plus `arg0->characterId = arg0->characterId + 1`
  produced the target byte reload/copy sequence, while the local-state version
  only differed by register allocation.
