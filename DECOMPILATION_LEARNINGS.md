# Decompilation Learnings

Reusable IDO 5.3 compiler behavior, matching patterns, and verified
layout/linking rules for this project. **Keep this file generic.** It is not a
log of individual function matches — do not add specific addresses, register
triples (`t6/t7/t8`), match percentages, specific constants, or sibling-tracking
prose. Those belong in commit messages, not here. Each entry below is a
principle you can apply to a *new* function; if a note only made sense for one
function, it has been removed.

## When to use these vs. the permuter

Most of the entries under "Register allocation" describe clean,
semantics-preserving source shapes that steer IDO's allocator toward a target
register choice. They are things to try by hand once the instruction sequence
and control flow already match and only register naming differs. When a match
is structurally correct and you cannot find the nudge by hand, the
decomp-permuter is the right tool — but verify each permuter suggestion in
isolation rather than importing a whole batch, since some of its output (e.g.
`& 0xFFFFFFFFFFFFFFFF`, no-op `^ 0`, dead-code-after-return) is artefactual.

## IDO codegen: optimization level and control flow

- **Branch direction and fall-through matter.** For `if (cond) { A } else { B }`,
  IDO usually branches to `B` when `cond` is false and lays out `A` as
  fall-through. Rewrite equivalent conditions to match the target's branch
  opcode and fall-through path.
- **`-O2` vs `-O1`:** `-O2` keeps variables in callee-saved registers more
  aggressively and fills branch delay slots better. `-O1` more often spills
  values that live across calls. If a near-perfect match differs mainly by
  saved-register vs. temp-plus-spill choices, check the optimization level the
  surrounding object uses before forcing source changes.
- **IDO range-check idiom:** a signed range test `[LO, HI)` is written as two
  fail-fast gotos — `if (d >= HI) goto next; if (d < LO) goto next;` — which
  compiles to `slt at,d,HI; beqz at,next` then `slt at,d,LO; bnez at,next`
  (mixed `beqz`/`bnez` polarity). A compound `if (d < HI && d >= LO)` produces
  two `beqz`s and will not match. Use the goto form when the target shows mixed
  polarity.
- **Switches and jump tables:** a comparison-chain ladder (`beqz`/`beq`)
  becomes a `.late_rodata` jump table when the case count grows past the
  obvious set. If a target's switch uses a jump table whose entry count exceeds
  the visible case count, look for a missing **empty terminal case** (often
  `case N: break;` identical to `default`) — state-machine switches frequently
  need this explicit terminal case even when the post-switch cleanup handles
  that same state.
- **Restoring a C jump table can expose a missing object boundary.** If the
  table has the correct semantic size but later rodata/BSS symbols shift, check
  whether the decomp project merged original translation units. Reconstructing
  the text/rodata/BSS split lets linker section alignment restore the padding
  naturally; do not attach padding to an unrelated assembly include.
- **A no-op expression in a switch selector can change temp register without
  changing control flow** (e.g. `switch (x ^ 0)` vs `switch (x)`). Reach for
  this only when the diff is a pure register-name swap on the dispatch value.
- **Dead `li` after an unconditional `b`:** IDO can emit a genuinely
  unreachable `li` when compiling a complex `||`/`&&` short-circuit with
  comma-operator-style intermediate assignments to the same variable. This is
  usually not reproducible from cleaner equivalent C (if/else, ternary,
  pre-initialized variable) — treat the residual ~one-instruction dead store as
  an IDO artefact rather than chasing it.
- **Statement order drives temp allocation order.** When a sequence of
  independent stores each load a stack arg into a fresh temp, IDO numbers the
  temps (`$t6`, `$t7`, `$t8`, …) in source order. A constant store also grabs
  the next temp at the point it appears. If the target assigns a constant to
  the *last* temp, write that constant assignment *after* the arg-derived
  stores it should follow, even though the scheduler ultimately interleaves
  them. Reordering just the constant assignment can fix a pure
  scheduling/register diff.
- **A folded empty pointer condition can steer induction-update scheduling.**
  Once loop control, registers, and memory accesses already match, a repeated
  empty test such as `if (((!p) && (!p)) && (!p)) {}` can change which of two
  independent pointer increments IDO schedules immediately after its store
  while emitting no instructions for the condition itself. Reserve this for a
  final pure instruction-order mismatch.
- **Recover accumulators from a folded first use.** If one call receives a
  literal but the following calls use an incremented value held in a saved
  register, model the source as a single accumulator starting at the first
  call's value and increment it between calls. IDO can fold the initial use to
  a literal while keeping the incremented value live for the later calls.

## IDO codegen: parameter homing and narrowing

- **Narrow parameters (`u8`/`s8`/`u16`/`s16`) are homed to their stack slot at
  function entry** even in a frame-less leaf: declaring a parameter as, say,
  `unsigned char` instead of `s32` emits a dead `sw $aN, K($sp)` of the
  incoming register followed by an `andi` to recover the clean byte. This is
  the source of the otherwise-mysterious dead-argument-store-with-no-prologue
  pattern. Match it by typing the parameter as the narrow type the callee
  semantically uses, not the wider type the caller passes.
  - For `s16`/`u16`, IDO only inserts the narrowing reload when the value is
    *reloaded* rather than reused straight from the home register, so an `s16`
    parameter used directly in a full-width `subu` can still emit a lone dead
    `sw` with no `andi` recovery.
  - The same dummy store (`sw $aN, K($sp)` plus an `andi`) appears for a `u16`
    *array-index* parameter; with `s32` plus an explicit `& 0xFFFF` IDO emits
    only the `andi`. Use the `u16` parameter type to reproduce the store.
- **Narrowing a parameter further in-place:** to keep a narrowed value in the
  argument's home register, reassign to the parameter (`arg0 &= 0xFFF;`)
  rather than introducing a separate local, which would move the value to a
  different register.
- **Beware narrow-prototype side effects within a TU.** A newly matched
  narrow-parameter callee that sits *before* its callers in the same
  translation unit can change the callers' argument setup even if the callee
  itself matches. Sometimes the callee must read the low halfword from the
  homed promoted argument to keep sibling callers matching.
- **Stale `$a3` at a call is not a 4th argument.** IDO simply leaves an
  incoming `arg0` in `$a3` and never clears it; the callee only consumes
  `$a0`-`$a2`. A dead argument register at a callsite is not evidence of an
  extra parameter.
- **Cross-TU prototype mismatches are real.** C does not cross-check
  prototypes between translation units. A function may legitimately be
  declared `void` in its defining TU and returning `s32` in another TU whose
  call site tests `$v0` after the `jal`. Do not "fix" the definition to match
  one call site — match each call site's own read of the result.

## IDO codegen: register allocation nudges

IDO colors registers by its internal temp numbering, not by source variable
identity. Textually identical expressions can compile to distinct loads, and
that extra load can shift a base pointer into the register the target uses.
The nudges below are semantics-preserving; use them when instruction sequence
and control flow already match and only register *names* differ.

- **Don't cache a value the target keeps reloading.** If a field/global is
  read several times across `jal` calls and the target reloads it fresh before
  each use, reference it directly each time rather than hoisting it into a
  local. Conversely, if the target CSEs two reads, avoid a named temp that
  would prevent the CSE; letting a struct-field read stay inline (un-named)
  can pick the "right" home register.
- **Reusing a just-stored value: reference the stored lvalue by name.** When
  the target reuses a value it just stored to a global/field (rather than
  re-deriving the expression), write the next expression against the stored
  lvalue (`D_x = ptr + 2; v = D_x + ...`) instead of re-deriving
  (`v = ptr + 2 + ...`). Reassigning the same local through a chain of pointer
  arithmetic reproduces IDO's habit of reusing one temp register.
- **Compound assignment steers the result's register.** Capturing the result
  of a compound assignment — `temp = (field += n);` or `var = *(p += k);` —
  makes IDO keep the unmasked/in-register sum in the place the target wants,
  whereas a two-statement `temp = field + n; field = temp;` often puts it
  straight into an argument register. When a load feeding an argument lands in
  the wrong register, try assigning from the `+=` expression itself rather
  than through a separate temp.
  - Inlining the assignment *inside* the consuming expression
    (`*p = *arg1 + (temp = result);`) can flip register homes where a
    separate statement on its own line does not.
- **Compound OR after a known-zero store can fold to a direct constant store.**
  If a field is cleared earlier in the same block, `field |= MASK` may emit the
  same single `sw` of `MASK` as `field = MASK`, while giving IDO different
  internal temporaries and register allocation. This is a useful, natural
  nudge when the instruction stream already matches and only a nearby mask
  test uses the wrong temp register.
- **Mask a narrow read explicitly to push off register reuse.** On a `u16`
  field, writing `(field & 0xFFFF) + 1` instead of `field + 1` is
  semantically a no-op (`lhu` already zero-extends) but can push IDO off a
  free-register reuse and onto the sequential temp numbering the target uses.
- **Pre-increment expresses "store raw, compare masked".** For a narrow field
  both stored and compared, `++field` (or `field += 1`) makes IDO store the raw
  increment and mask separately into a fresh register for the compare. A
  separate `temp = field + 1` tends to store the *masked* value instead.
- **A bare widening cast on a comparison steers `v0`/`v1`.** `(u32)temp == K`
  or `(unsigned int)state == K` emits no extra instruction (the value is
  already zero-extended) but reshuffles which v-register the comparison value
  lands in. Good for a value used in several branches.
- **Operand order in commutative `+`/`or` controls register allocation.** IDO
  assigns the destination/operand registers based on source operand order.
  When a match is functionally perfect but differs only in which temp holds
  each operand of an `addu`/`or`, try swapping the addends in the C source.
- **A double subtraction can prevent IDO from canonicalizing addends.** When
  both `a + b` and `b + a` emit `addu dest,b,a`, the equivalent form
  `a - (0 - b)` can emit `addu dest,a,b`. Use it only when the value ranges
  make the intermediate negation well-defined.
- **Narrow a dead local in place before its final commutative use.** Writing
  `value >>= N; consume(field + value);` can preserve the target shift while
  making the field the first `addu` operand. The inline equivalent
  `consume((value >> N) + field);` often gives IDO the opposite operand order.
- **Split chained arithmetic assignments on a narrow parameter.** Writing
  `arg = arg * K; arg = arg - bias;` can emit the same shift/subtract/narrow
  sequence as `arg = (arg * K) - bias`, while changing IDO's SSA coalescing
  enough to rotate the surrounding temporary registers. This is a clean nudge
  when the instruction stream already matches and only register allocation
  differs.
- **Force commutative operand order with the comma operator.** `(0, x)` is a
  standard decomp idiom that flips the operand order IDO assigns for a
  commutative op, with no runtime effect.
- **A constant-true conditional can change temp allocation without emitting
  code.** Wrapping a region in `if (1) { ... }` can move an otherwise identical
  value between `$v0` and `$v1` under `-O2`; a plain compound block does not
  necessarily have the same effect. Reserve this for a final register-only
  mismatch after confirming that the condition emits no branch.
- **Fold a pointer-global assignment into its first field access.** Writing
  `(current = ptr)->field` can emit the same store and field load as two
  statements while giving IDO a distinct internal temporary for the assigned
  pointer. This can fix an otherwise isolated register-allocation mismatch and
  is especially plausible when matched sibling functions use the same shape.
- **Bind a return expression's intermediate to a named local.** When an
  intermediate computation (e.g. a shift) must occupy a temp distinct from
  `$v0`, capture it in a named local and return that local, rather than
  returning the expression directly (which loads the operand straight into
  `$v0` and shifts in place).
- **A local alias of an argument forces a callee-saved register.** When an
  argument is spilling to the stack but the target keeps it in an `s`-register,
  introducing `void *new_var = arg0;` at the top and using `new_var`
  throughout raises the argument's live-range priority so IDO colors it into
  `$s0` (or whichever saved register). Mirroring the parameter spelling of
  already-matched sibling functions in the same file is the most reliable
  guide for `s`-register vs. stack-spill choices.
- **Adding a *meaningful* extra read beats permuter artefacts.** When stuck on
  pure temp-register naming, try a legitimate extra access to the relevant
  global/field (e.g. re-reading a counter global post-decrement to use as an
  array index) rather than reaching for `volatile` or dead-code hacks. The
  extra read changes liveness and can flip the allocator to the target's
  registers cleanly.
- **Let GBI bitfield macros perform their own narrowing.** Macros such as
  `gDPSetPrimColor` mask fields to their encoded width while packing command
  words. A caller-side mask on the same argument is redundant and can extend
  a temporary's live range enough to shift register allocation throughout the
  following display-list writes.
- **Split declaration from assignment for a pointer local to hoist its `lui`.**
  `T *ptr; ptr = &arr[expr]; f(ptr);` can schedule the base-address `lui %hi`
  earlier than the combined initializer `T *ptr = &arr[expr];`. When chasing a
  single misplaced address-materialization `lui`, try splitting.
- **Source order of struct-field stores steers temp numbering.** When only
  temp-register names differ across an otherwise identical sequence of field
  stores, reorder the assignments in C (not the logic) — writing them in
  ascending field order is a common winner — to renumber the allocator.
- **Use scalar temporaries to batch adjacent field loads before stores.** For
  neighboring struct fields that are transformed then stored, direct
  `dst->x = f(src->x); dst->y = f(src->y); ...` can emit load-use nops.
  Loading each source field into named locals first can reproduce IDO's batched
  loads followed by arithmetic and stores.

## IDO codegen: loop shape and strength reduction

- **A terminal backward `goto` can align an unreachable epilogue.** IDO may
  emit `.align 5` between an infinite loop's final branch and its dead
  epilogue. The number of resulting nops depends on the function's offset in
  the full translation unit, so an isolated matching workspace can show excess
  padding even when the integrated function has the exact target length.
- **Pointer-bump `do`/`while` vs. strength-reduced indexed `for`.** A loop
  that bumps a pointer over an array makes IDO hoist the shared base address
  into a callee-saved register and reuse it everywhere, forcing an extra saved
  register. When the target instead *reloads the base fresh* at loop entry,
  write the loop as an index-based `for` whose address is computed inline
  (`f(base + i * STRIDE);`). IDO strength-reduces `i * STRIDE` into a pointer
  initialized with a fresh `lui`/`addiu` in the preheader, so it does not CSE
  with any base held earlier.
- **Repeat a logically redundant zero index before parallel indexed walks.**
  When an index is already zero through preceding control flow but IDO still
  emits a multiply to initialize several strided array cursors, assigning zero
  again immediately before the loop can make the optimizer prove the initial
  value and collapse every cursor to its array base. The redundant assignment
  itself is then folded away.
- **A descending array-index loop can become a pointer walk.** For a loop such
  as `for (i = count - 1; i >= 0; i--)`, IDO may replace the integer induction
  variable with a pointer initialized to the last element, decrement it by the
  element size, and terminate with an unsigned pointer comparison against the
  array base. Do not assume that a target `addiu ptr, -sizeof(element)` loop
  was written with an explicit pointer.
- **Parallel indexed fields can share one induction cursor.** When a struct
  contains parallel arrays with the same element width, repeated
  `actor->x[i]` and `actor->y[i]` accesses can become one pointer cursor that
  advances by the element size while retaining the arrays' field-offset
  difference. Prefer the typed indexed form when a target bumps one cursor but
  loads several fixed offsets from it.
- **Structured counted loops can get unrolled; unstructured goto loops do
  not.** IDO aggressively unrolls structured `for`/`while`/`do-while` loops
  whose trip count is computable at runtime from two address operands, using a
  `subu` + `andi N,0x..` remainder pre-loop plus an unrolled body (the same
  `& (factor-1)` idiom as memset). If the target shipped a plain `bne p,end`
  do-while, write the loop as an unstructured goto loop (label +
  `if (p != end) goto label;`) — IDO's unroller only runs on structured loops.
- **Prefer a plain multiply literal over a hand-decomposed shift.** When a
  multiply by a non-power-of-2 appears, writing `x * 0x60000` gives IDO the
  freedom to emit its strength-reduced `sll`/`subu`/`sll` chain in the target's
  registers. A hand-decomposed `(x * 3) << 17` reproduces the instructions but
  tends to fix the result in the wrong temp.
- **Keep the project's collapsed `do { ... } while (0);` shape verbatim.**
  IDO's codegen for pointer-walk loops and display-list setup wrappers is
  sensitive to source line layout even for token-identical input; a
  reformatted (multi-line) version can re-introduce a two-instruction `addiu
  %lo` scheduling swap. Matched siblings in the same file should stay in the
  same source style (one-liner vs. multi-line) — both can be valid fillings of
  the same `jal` delay slot, and IDO's filler picks differently per layout.
- **Hoist an invariant pointer temp before a call to change spill strategy.**
  Computing an address-of-field temp *before* a `jal` (when the base is
  invariant across the call) is semantics-preserving and can reproduce a
  target that spills the argument to its stack home and reloads it, rather
  than keeping it in a register copy with direct offsets.

## IDO codegen: stack layout

- **Stack slots are assigned top-down in declaration order.** IDO reserves
  local stack slots in declaration order (high offset first). If a spilled
  local lands one slot too high, declaring an additional — even register-only
  — local *before* it shifts the real local down to the target offset.
  Declaring locals in the same order as a matched sibling is the safe move.
  Even an unused narrow local can preserve the frame size and alignment of
  every lower slot, so do not remove one merely because it emits no direct
  load or store.
- **Combine block scopes with declaration order to tune small frames.** Moving
  non-overlapping pointer iterators into their actual loop scopes can shrink an
  otherwise-correct frame without changing the instruction stream. An unused
  `register` local immediately after a value that crosses calls can then shift
  that value's spill slot within the smaller frame; test the two changes
  together because either one alone may choose a different frame size.
- **The `struct { s32 v; s32 pad; }` idiom for a spilled call result.** When a
  single spilled `s32` lands at `0x24(sp)` but the target uses `0x20(sp)`,
  declaring it as a two-word struct with a trailing pad shifts IDO's
  allocation so the value lands at `0x20(sp)`. Reuse the surrounding file's
  existing idiom rather than inventing a new one.
- **`volatile` padding to preserve a leaf frame.** If typed C matches all
  instructions but IDO shrank the stack frame, an otherwise-unused
  `volatile u8 padding[N]` local can restore the target frame size without
  changing scheduling or register allocation.
- **Object-relative alignment nops are padding, not logic.** IDO may emit an
  alignment directive before an unreachable epilogue or the next function.
  A single-function workspace starts at a different section offset than the
  real multi-function object, so the same directive can produce a different
  number of trailing `nop`s even when the live instructions match. Inspect
  the compiler's assembly output and verify the candidate in the real
  translation unit instead of forcing the workspace padding via C.

## IDO codegen: signedness and comparisons

- **Hex literals above `0x7FFFFFFF` are `unsigned int` in C89** and force an
  unsigned comparison (`sltu`). Cast to signed — `(s32)0xFA000001` — or write
  the negative decimal/hex equivalent (`-0x9FFFFF` for `0xFF600001`) so IDO
  emits the signed `slt` the target uses. This applies to comparison bounds
  even when the surrounding reset store still emits a clean `lui` for the same
  literal.
- **`lhu` vs `lh` is determined by the struct field type.** A `u16` field
  emits `lhu`; an `s16` field emits `lh`. Casts on an `s16` field give `lh` +
  `andi`, never a clean `lhu`. When the same byte offsets are read signed by
  some functions and unsigned by others, pick the struct view whose field type
  matches the load (the project keeps both `s16`- and `u16`-field view structs
  for this reason).
- **Byte signedness can perturb constant lifetime.** Even a same-sized
  `s8`/`u8` field-type change can shift which constant IDO reuses for a store
  and thus hoist or delay a `li`. Preserve observed byte signedness when
  replacing byte-pointer stores with struct fields.

## Structs, types, and data access

- **Prefer typed struct/array access over manual pointer arithmetic.** Express
  known fields with `->`, `.`, and indexed access; verify field offsets and
  total struct sizes against the assembly access patterns, since a small
  layout mistake shifts codegen across every function sharing the type.
- **Use post-increment on typed pointer fields for bytecode cursors.** When a
  target loads a cursor field, reads through the old pointer, increments it,
  and stores it back, model the field as the pointed-to byte type and write
  `*state->cursor++`. Keeping the cursor as an integer and spelling out casted
  pointer temporaries obscures the field's role and changes IDO's temp
  allocation even when the instruction sequence is otherwise equivalent.
- **Use the most specific parameter/local types the surrounding code
  supports.** Broad `void *` typing hides useful offset information and
  produces unstable casts. When the ABI signature must stay `void *`/`s32`
  (e.g. a callback dispatched through a function pointer), keep that prototype
  and cast to a local typed view struct at the top of the body — the
  assignment is a register no-op.
- **Array-decay vs. pointer field matters for `addiu`-vs-`lw`.** When a field
  is passed *by address* (the assembly shows `addiu $aX, base, offset`), model
  it as an array/struct so it decays to its address; a pointer field emits
  `lw` (loads the stored value) instead and breaks the match.
- **Array vs. pointer declaration of a global controls indirection.** A global
  accessed as `global->field` must be declared as an **array**
  (`extern T global[]`) so the symbol address is the struct base and `->field`
  compiles to a single `lw r, off(base)`. Declaring it a pointer adds an extra
  indirection.
- **Overlapping fields via union views.** When a region is reused as a word, a
  halfword pair, and a byte flag across callbacks, a union with word/halfword/
  byte views preserves the original `lw`/`lh`/`lhu`/`sb` instruction choices
  while removing manual offset casts. Adjacent BSS labels or interior labels
  in strided tables can often be represented by extending a view struct rather
  than introducing new globals.
- **Avoid hoisting a typed strided-array pointer across calls if the target
  recomputes.** Typed indexing can still match as long as each load is written
  independently. If the target visibly recomputes a stride between calls,
  don't introduce a local element pointer — IDO will keep it on the stack and
  shorten the function.
- **Viewing a shared global under a different struct via inline cast.** When a
  global typed one way in one file is read at different offsets in another, a
  small view struct with leading padding and an inline cast
  (`((View *)global)->field`) is a runtime no-op and reproduces the exact
  single-load codegen.

## Globals, data, and linker behavior

- **Adjacent narrow BSS globals do not imply a struct.** IDO naturally packs
  distinct byte and halfword globals at their type alignment within one
  translation unit. The containing `.bss` contribution can then receive
  larger section-end padding, so distinguish object-boundary padding from
  padding between C objects before inferring an aggregate type.
- **Local definition vs. `extern` affects codegen.** IDO generates different
  code for globals defined in the current TU (can keep base addresses in
  registers across adjacent stores) versus globals declared `extern` (can
  force extra address reloads and worse delay-slot scheduling).
- **Explicitly zero-initialized globals (`static u8 x = 0`) may land in
  `.data`, not `.bss`.** Account for this when splitting raw data/BSS
  segments.
- **A global both read and written can defeat address CSE.** When the stored
  value is a register-held value (not `$zero`), routing the constant through a
  named local (`temp = 0; ... D_x = temp;`) can prevent IDO from folding the
  store to `sw $zero` and re-CSE-ing the address — useful when the target uses
  a fresh `lui %hi` per access. `volatile` on the global typically does *not*
  help here; only the register-held store value defeats the address CSE.
- **Use an owning struct field to distinguish an interior-symbol alias.** If a
  byte has both a standalone symbol and a named field in an enclosing global
  struct, reading through one name and writing through the other prevents IDO
  from CSE-ing their addresses. The relocations have different spellings in an
  isolated object, but resolve to identical linked instructions; verify this
  shape with the full-ROM checksum.
- **Extern pointer globals are reloaded between aliased accesses.** For
  `g->field -= 1; if (g->field == 0)`, IDO re-emits the `lui`/`lw` of `g`
  between the store and the re-read (it cannot prove the store does not alias
  the global itself). This is expected; writing the field access naturally
  reproduces it.
- **`#pragma weak alias = real`** is the correct way to express weak aliases
  under IDO when multiple names resolve to the same address.
- **Symbols in `symbol_addrs.txt` using `0x700...` addresses are splat
  placeholders.** Convert them to runtime `0x800...` VRAM addresses before
  relying on linker resolution. Symbol names drive extraction; manual edits to
  extracted asm labels are overwritten by `make extract`, so rename at the
  source. For data mismatches after source changes, use the data differ
  (`--find-first-mismatch`) before making layout assumptions.

## Segment splitting and alignment

- **C object `.text` pads to 16-byte alignment.** Splitting a segment at an
  exact unaligned function end can shift following raw asm. When section-end
  padding causes shifts, it may be better to match the full upstream file or a
  larger aligned range instead of a tiny helper.
- **Renaming a Splat C segment changes the generated
  `asm/nonmatchings/<segment>/...` path** expected by `#pragma GLOBAL_ASM`.
  Update the pragmas after changing `snowboardkids.yaml`, then rebuild from
  extraction so stale old-directory paths do not hide include failures. The
  asm processor does not process `#pragma GLOBAL_ASM` hidden inside a `#if`;
  keep the pragma at top level and put exploratory C under a separate
  `#ifdef NON_MATCHING` block.
- **Pure `nop` text ranges may be intentional padding** between libultra
  objects; keep them assembly-backed when C would emit a real prologue/return
  or different alignment filler.
- **Two consecutive `jr $ra; nop` (a 0x10 range) is two empty functions, not
  one.** IDO never emits two `jr $ra` from one function (even `return;
  return;` folds), and does not fold identical empty functions. Match it by
  defining two empty functions back-to-back in the same `.c` file.
- **Fall-through functions split across subsegment boundaries.** A function
  with no `jr $ra` before the next symbol falls through into the following
  code; if splat placed a `[addr, c]` boundary inside it, it becomes two
  `GLOBAL_ASM` stubs in two `.c` files (the second half is never reached via
  `jal` and its first instruction reuses a register loaded at the end of the
  first). To merge: move the boundary in the YAML to the next real function
  start, add `size:0xNNN` in `symbol_addrs.txt` so splat extracts the whole
  function into one `.s`, rename the shifted segment's `.c` and its
  `asm/nonmatchings/` dir, and delete the obsolete second-half stub. A single
  C function cannot span two `.o` files, so the boundary move is mandatory.
- **Split rodata by ROM offset, not by VRAM address;** confirm split points
  with the linker map. Jump-table functions need care when converting raw asm
  to C: rodata tables may reference file-local `.L` labels that break when a
  function is split into separate `GLOBAL_ASM` files.
- **Preserve late-rodata padding when replacing a `GLOBAL_ASM`.** If a matched
  C function emits one fewer trailing padding word and shifts the next
  unmatched function's jump tables, declare the padding address as a typed
  symbol in `symbol_addrs.txt` and migrate it to the following function with
  `function_owner` and `force_migration:true`. This keeps the fix reproducible
  across `make extract`; edits to generated assembly are discarded.
- **Choose the C form according to the original float's section.** With IDO
  5.3, a scalar global `const f32` can land in `.data`, a global const array
  lands in early `.rodata`, and a float literal used by a function lands in
  late rodata. When removing a `GLOBAL_ASM` that owns late-rodata float
  constants, using the equivalent literals can preserve their ordering and
  addresses when no other code needs the symbols.
- **YAML comments are hints, not proof.** Verify actual function/file
  boundaries against disassembly, `symbol_addrs.txt`, and the linker map.

## Matching workflow

- **Mirror matched siblings verbatim.** When a function sits next to an
  already-matched near-twin (common in state-machine callback families),
  copying the sibling's source form — including its struct field layout,
  parameter spelling, local-alias pattern, and statement grouping — is the
  fastest path to a match. Field signedness drives register allocation even
  when the generated loads/stores look identical, so copy the *layout*, not
  just the body. When the layout already aligns, a body copy is sufficient.
- **Verify function signatures and call argument order against the target
  assembly *before* tuning register allocation.** Wrong argument order
  produces misleading register-allocation diffs and wastes permutation time.
- **Use explicit `else` assignments when IDO keeps a final branch.** In small
  value-selection blocks such as `default; if (cond) override;`, IDO may
  collapse the override into fall-through. Rewriting as `if (cond) override;
  else default;` can preserve the target's explicit branch and delay-slot
  assignment without changing semantics.
- **Cast an operand, not the sum, for 64-bit signed addition.** For expressions
  like averaging two `s32`s, `(s64)(a + b) / 2` lets the 32-bit add overflow
  before widening. `((s64)a + b) / 2` emits the carry/sign-extension sequence
  before `__ll_div`.
- **After a successful match, check the whole modified file, not just the
  matched function** — functions accessing the same structs can break when a
  struct layout changes. Run `./tools/build-and-verify.sh`; if the checksum
  fails, diff all functions in the modified file(s).
- **Pre-commit hooks are authoritative.** A failed hook means the C update is
  incomplete; do not bypass it.
- **IDO is occasionally line-break sensitive for instruction scheduling.** Two
  source files that are token-for-token identical except for where statements
  are split across lines can produce different prologue instruction ordering
  (e.g. an independent `li` and `lui`/`addiu` pair swapping). If a cleanup that
  only changes statement types/variable names flips two adjacent prologue
  instructions while leaving the entire function body byte-identical, try
  preserving the original line grouping — e.g. keep an initializer on the same
  line as the `for` (`player = gRacePlayers; for (...)`) rather than on its own
  line. The decomp-permuter will surface this as a whitespace-only winning diff.
- **The `register` storage-class specifier is usually inert in IDO 5.3 and can
  be dropped.** For a local that must already live in a callee-saved register
  (because it is live across a call), IDO selects that saved register on its
  own; a `register` qualifier changes neither the allocation nor the codegen, so
  it is pure noise from the decompiler and safe to remove. (Do not confuse this
  with genuinely load-bearing qualifiers like a `volatile` parameter that forces
  a stack reload on every use.)
- **IDO 5.3 instruction scheduling is path-sensitive to source-level statement
  presence, not just semantics.** When cleaning up an already-matching
  function, edits that are semantic no-ops can still break the match because
  IDO keys its scheduling off the source expression tree. Confirmed breakers:
  removing a redundant self-alias (a local that just holds a copy of another
  pointer and is used in its place), deleting an empty constant-true block
  (`if (1) {}`), deleting an assignment that is overwritten before any use, and
  dropping a redundant mask whose result is immediately re-cast at the call site
  so the mask emits no instruction. In every case the assembly was unchanged at
  the edit site but the surrounding address-computation scheduling reordered.
  Conversely, pure identifier renames and swapping one union field name for
  another that names the same byte at the same offset (e.g. a `u8` `pad[0]`
  alias for a `u8` field both at offset 6) are always safe. When a matching
  function is dominated by decompiler pointer-arith artifacts (scratch pointer
  arrays, a sliding base pointer reinterpreted to read per-player fields, or
  `ptr[12]` / `ptr[32]` indexing to reach two arrays off one shared base),
  expect those artifacts to be load-bearing and restrict cleanup to renames
  unless you can re-derive the exact address-math scheduling by hand.
