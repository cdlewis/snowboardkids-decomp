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

**Do not trust a permuter score without re-checking the object.** The
`compile.sh` used by this project's permuter workspaces invokes `cc` directly —
no `asm-processor`, and it never strips `.mdebug` — so its scorer reads debug and
symbol collateral rather than `.text` alone. On a register-only residual this
produced a *43% apparent improvement* (score 80 against a base of 140) on a
candidate whose `.text` was `instruction-words-identical` to the base. Rebuild
any permuter winner through `build.sh`/the project pipeline and compare the
object before believing the number; on a near-match its score gradient can be
pure noise, in which case the search cannot converge and is better stopped.

**Check permuter winners for stale captures, not just for artefactual
operators.** A frequent and easily missed invalid mutation is hoisting a read
of a variable into a new local *before* code that still writes to it, then
using the local at the original site. It looks like an innocuous CSE and the
diff is three lines, but it silently discards every later update. On one
function the permuter's top three candidates all captured a rectangle edge
before the clipping code clamped it, so the emitted primitive would have
ignored clipping entirely. Locate every later assignment to the captured
variable before adopting such a variant.

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
- **Physical source-line grouping can affect scheduling.** With otherwise
  identical statements and control flow, putting a run of setup statements on
  one source line can change how IDO interleaves independent address
  materialization instructions. This is a last-mile option when the remaining
  diff is instruction order only and ordinary statement reordering has no
  effect.
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
- **Same-width prototype signedness can also steer caller allocation.** Even
  when both parameter types use the same ABI width, changing a caller-facing
  prototype from signed to unsigned can alter IDO's internal conversions,
  register coloring, and stack homes without adding an explicit conversion
  instruction. Verify the declaration visible in the original translation
  unit rather than treating `s32` and `u32` as interchangeable.
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

- **Read IDO's pre-`as1` output before theorising about an allocation
  residual.** `cc -S` (same flags, minus `-c`) writes the assembly that `ugen`
  produced: final register numbers, `.loc` line records, and unexpanded
  pseudo-ops such as `div $15, $14, 2`. That is the layer where register
  allocation is decided, so a "schedule" difference in the object is often a
  register difference upstream — `as1` reorders around the physical registers
  it is handed. Diffing two candidates' `-S` output isolates one changed
  decision where diffing their objects shows dozens of shifted rows. It also
  shows which instructions `as1` invents: a stack reload of a homed parameter
  becomes the object's `move`, and `div x, y, 2` becomes the
  `bgez`/`addiu`/`sra` block, so an apparent basic-block boundary in the object
  need not exist when the source statement is scheduled.
- **uopt's `v0/v1/a0/a1` group is assigned by value *kind*, not by source
  order.** For a symmetric pair of computations (two field loads feeding two
  shifts, say), IDO consistently colors both loads with the low pair and both
  derived values with the high pair. Measured non-levers for that grouping:
  local declaration order, statement order, per-axis nested scopes, expression
  grouping, separate carrier variables, and source line layout. Only the order
  *within* each pair follows the source. If the target interleaves the pair
  per computation instead, the original source did not have that symmetric
  four-local shape — look for a different variable structure rather than
  permuting the one you have.
- **Statement order across a block of *independent* sibling assignments is a
  first-class allocation lever, even when declaration order is not.** Four
  mutually independent clip-bound assignments (`min/max` on each axis, each one
  global load, one halving, one subtract-or-add, one shift) were permuted 24
  ways with the instruction sequence, instruction count, frame size and every
  stack home held constant. Positional word mismatches ranged from 271 to 397
  out of 422 — one statement order colored the whole downstream `Gfx` macro
  tail correctly and another rotated it by one slot. The same candidate set was
  immune to all 65 declaration-order permutations and to 183 blank-line layouts.
  So when a residual is "same opcodes, permuted registers", permute the order of
  independent *statements* before spending variants on declarations, types, or
  physical layout; and rank the permutation on positional words, because the
  reordering penalty in `dist.py` hides this lever entirely.

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
- **Materialize a shared negation with compound assignment.** When the same
  negative value feeds multiple later expressions, `value *= -1;` followed by
  uses of `value` can emit the same single `negu` as repeating `-value`, while
  changing IDO's SSA lifetimes and temporary-register allocation. This is a
  useful last-mile form when the target clearly computes one shared negation.
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
- **A redundant narrow-read mask can phase-shift ugen without surviving in
  the object.** Instrumented IDO 5.3 shows that `(u16_value & 0xFFFF)` may
  allocate and free a temporary even when the assembler removes the redundant
  instruction after `lhu`. Moving that mask across one expression can move a
  single FIFO allocation to the other side of the expression, changing its
  register while restoring the register sequence immediately afterward.
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
- **Narrow a wide quotient through a result temporary to preserve a copy
  move.** When a target keeps an extra `move` after strength-reducing a signed
  division, compute the product in an `s32`, assign the quotient to a `u16`
  result, then widen that result for later packing. If range analysis proves
  the quotient fits, IDO emits no explicit mask but retains the assignment
  move; keeping the entire calculation in one `s32` can coalesce it away and
  rotate every following temporary register.
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
- **A one-time forward-goto initialization block can reorder register
  allocation without changing the emitted CFG.** When an invariant local takes
  the first temp register but a loop-local value needs it instead, branch to an
  initialization label placed textually between the loop locals whose order the
  target implies, then branch back to the loop header. IDO can fold the one-time
  path into the prologue while retaining the label's source/IR ordering for
  register coloring. Use this only for a final register-only mismatch and
  verify that the compiled branches disappear completely.
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
- **Use two-stage array indexing when the target adds offsets in stages.**
  Equivalent expressions can give IDO different temporary-register orders.
  When the target forms `table + frame` before adding a scaled index, write
  `(&table[frame])[index * stride]` instead of
  `table[frame + (index * stride)]`. This remains typed array indexing while
  preserving the target's address-construction order.

### What uopt's coloring actually does (measured, not inferred)

These were measured on IDO 5.3 `-O2 -mips1` with an instrumented `uopt`
(`ido-static-recomp`'s `uopt.instrumented` dropped into a copy of
`tools/ido-recomp/linux`, gated by checking that it reproduces the stock object
byte-identically with its trace variables unset). They constrain which nudges
above can possibly work, so read them before spending a variant.

- **Colors are chosen lowest-index-first, and the pool includes `t0`–`t5`.**
  The order is `c1=v0 c2=v1 c3=a0 c4=a1 c5=a2 c6=a3 c7=t0 … c12=t5`, then
  `c14–c22 = s0–s8`. A `t`-register in the output is therefore *not* evidence of
  a ugen temporary at this optimization level — uopt colors them. Tools that
  classify `t0`–`t9` as always-ugen (a claim probed under `-mips2`) will
  mislabel a plain coloring difference as a pool-versus-temp "class crossing"
  and send you after web *formation* when the real question is web *ordering*.
- **Webs are colored in descending `save`, and ties break on web number**,
  which is construction chronology. Two webs with identical interference
  neighbour sets are indistinguishable to the allocator, so whichever was
  constructed first takes the lower-index register. That makes statement order
  at the *creation* site a real dial: swapping two adjacent pointer
  initializations swaps their registers.
- **`save = totalsave / nocs`, and `nocs = ((n - 2) >> 2) + 2` for `n`
  occurrences.** So `save` is a coarse, non-monotone dial: adding occurrences
  raises `totalsave` but jumps `nocs` at `n = 2` and `n = 6`, which can *halve*
  a web's priority. One occurrence in a loop body outranks three occurrences
  spread over a loop plus a cold block. Compute the target before adding reads.
- **A discarded read is a pure priority *penalty* on the web it reads.** It
  adds occurrences, so it can push `nocs` over one of its steps, but it
  contributes nothing to `totalsave` — the quotient can only fall. A dead
  `if (v && v) {}` or `if (v) {}` filler added to reach the target instruction
  count is therefore *not* allocation-neutral for `v`: it can drop `v`'s web
  below a competitor it would otherwise have tied with and beaten on web
  number, silently swapping the two registers. When such a filler is load
  bearing, spell it against a variable that is *not* part of the register
  residual; reading the contested variable buys instruction count at the price
  of its color.

- **A local that is coalesced away still reserves a register.** A `u8` temp
  holding a loaded byte across a couple of increments can emit no instruction of
  its own (ugen coalesces it onto the temp that carries the load) and yet hold a
  caller-saved register in the interference graph, blocking every web it
  overlaps. Deleting the temp is what frees the register; no spelling of the
  temp's *type* (7 tried) or of its uses changes this.
- **`cfe` canonicalizes redundant pointer decoration before `uopt` sees it.**
  `*(p + 0)`, `*(p - 0)`, `p[0]`, `*(u8 *)p`, `(p, *p)`, `p = p + 0`, `p += 0`,
  `p = &p[0]`, `p = p` and `p = p + 1` versus `p += 1` all produce a
  byte-identical object. The "redundant mask" lever that works on integers has
  **no pointer analogue** — do not plan a web-weight campaign around one.
- **Declaration order of register-allocated locals is inert.** Moving each of
  four locals to all 29 positions in a declaration block (112 builds) produced a
  single object. When no local is frame-homed, declaration order moves neither
  the coloring order nor the frame; spend the variant elsewhere.
- **`force_declined` in a globalcolor trace is a real dead end, not a hint.**
  If forcing a web onto the target register is declined because that register is
  held by an *interfering* web, no reweighting can win it. Either the blocking
  web must lose the register, or the two must be colored in the other order.

- **A local initialised to a literal is folded away before webs exist, so a
  "constant carrier" can never recolour a constant.** When the target holds a
  CSE'd literal in a register you cannot reach, the obvious move is to declare
  a local for it and use that local at the comparison, `div` and `mul` sites.
  It does not work: uopt constant-propagates the carrier at every use and the
  object comes back byte-identical to the literal spelling, with the carrier's
  frame slot as the only trace (declared locals get a home whether or not they
  survive). Measured across `s32`, `u32`, `s16`, `s8`, `u8` and `char`
  carriers, assigned at the top of the function and mid-function, and also as a
  graft onto an existing dead local: every variant produced the same object.
  Work the *priority* of the compiler's own constant web instead - occurrence
  count and which blocks the occurrences sit in.
- **Subscripting by a variable blocks the fold-and-hoist that a pointer walk
  invites.** `arr[i] = v; arr[i + 1] = v;` keeps `i` in a register and computes
  `base + i*2` at the site. The equivalent `p = arr; p += i;` followed by
  `p[0]`, `p[1]` lets uopt constant-fold the whole address when `i` is a known
  constant and then hoist it out of the branch entirely. So when the target
  shows `li k / sll / addu` for an index whose value is obviously constant, the
  original used the subscript form; the pointer form cannot reproduce it.
- **A hoisted-literal web cannot be moved, but the *scratch local* next to it
  decides which register it may keep.** When a constant is used twice in one
  region IDO builds its own web for it, splits it, and colors the surviving
  piece lowest-free-index. You cannot reach that web from source (see the
  "constant carrier" bullet above), so work the *blockers* instead: whichever
  local the source assigns in that region carries its web forward into the
  following region, and therefore forbids its own color to the constant. Two
  sources that emit byte-identical instructions can still differ here, because
  the choice of local is pure web formation. Concretely, if the constant is
  landing on the register held by a later variable `A` and you want it on the
  register held by later variable `B`, stop using `B` as the region's scratch
  and use `A` instead: `B`'s color becomes reachable and `A`'s is blocked. The
  live-range reach is one-directional and *late* — only an assignment in the
  last blocks before the constant's final use creates the interference, so an
  earlier block's use of the same local is inert.
- **Register-allocation residuals of this shape are best diagnosed from the
  `webdetail` record, not by guessing.** With an instrumented `uopt`,
  `CDX_DETAIL_WEB=all` prints `type=2 … table=N raw10=0x0000000N` for a hoisted
  integer literal `N` and `type=3 … raw10=<negative frame offset>` for a local
  variable, so a web can be tied to a source symbol by its declaration offset
  instead of by forcing colors and reading the diff. Then `CDX_FORCE=p1:wN=cM`
  on the literal's web confirms ownership in one build, and the `p1dec`
  `forbidden0` mask says exactly which register must be freed and which must
  be blocked before any source variant is worth compiling.

- **A discarded read also extends the live range of the `%hi` web it names.**
  The priority-penalty rule above is not the only effect: `if (G && G) {}` in a
  branch that does not otherwise mention `G` keeps `%hi(G)` live through that
  branch, which changes where IDO materialises the `lui`. Use it when the
  target hoists an address into an early slot that your build fills with an
  unrelated literal. Both the count and the site are dials and both saturate -
  in one case one read was inert, two and three gave the same object, four or
  more regressed, and the same construct at nine other statement boundaries
  only regressed.

- **Tail liveness is a whole-function allocation dial.** Giving an *existing*
  local one more occurrence in the function's last loop — e.g. carrying a
  redundant `playerIndex++` alongside a pointer walk that already terminates on
  the pointer — keeps that variable live to the end of the function and can
  re-color the allocation hundreds of instructions earlier. On
  `updateMultiplayerCourseSelectMenu` it moved the residual from 150 positional
  words to 94 *and* dissolved a three-instruction scheduling rotation about 700
  instructions upstream, turning a `structure-mismatch` into a pure
  `allocation-mismatch` (0 opcode mismatches, 0 gaps). Reach for it when a
  structural residual sits far from any construct you can vary and every local
  edit at the residual has failed: the cause can be that the target keeps a
  carrier live past the point your version lets it die. The natural spelling is
  a parallel index or cursor, which this codebase's loops already use
  idiomatically; the permuter finds the same mechanism as a read of an
  *uninitialised* variable in the tail, which is the same dial written as
  undefined behaviour — take the mechanism, not the mutation.

- **Spelling decides whether a discarded read is free.** `if ((s32) V);` (a cast
  guarding an *empty statement*) emitted nothing here, while `if (V) {}` with a
  braced body cost a branch pair and shrank the frame from `0xB0` to `0xA8`,
  which regresses everything downstream. Before reading a count or a site as
  evidence, confirm the construct is actually zero-footprint on the current
  parent by checking `insns` and `frame` on the compared object, not by
  assuming it from the shape.

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
- **Repeat a logically redundant zero index before indexed pointer walks.**
  When an index is already zero through preceding control flow but IDO still
  emits a multiply to initialize strided array cursors, assigning zero again
  immediately before the loop can make the optimizer prove the initial value
  and collapse every cursor to its array base. With two equivalent assignments
  around unrelated setup, IDO may fold one into a strength-reduced cursor while
  retaining the other as a dead `move reg, zero` in a preceding call's delay
  slot.
- **Naming a strength-reduced array cursor changes the address arithmetic, not
  just its register.** `p = &s->arr[i]` or `p = s->arr; p[i]` folds
  `offsetof(arr)` into the cursor itself (`addiu p, base, OFFSET`), while the
  plain `s->arr[i].field` expression keeps the member offset as the load/store
  *displacement* and strength-reduces only `i * stride`. So when the target
  initialises a cursor with a bare `move cur, base` and carries a large
  displacement on the access, the cursor is IDO's own induction variable: no
  pointer local reproduces it, and introducing one also tends to drop an
  instruction by folding the bias. Read the displacement before deciding a
  cursor is a source-level pointer.
- **Changing which local carries a loop index has global blast radius.**
  Substituting a different local for a loop index, with no change to the
  emitted arithmetic, can shift register assignment from the *first*
  instructions of the function, because web construction chronology is global.
  Always re-measure the whole object: a substitution that fixes the two
  contested registers inside the loop can move a hundred opcodes elsewhere and
  be a large net regression.
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
- **A symbolic pointer bound can keep an indexed loop structured without
  unrolling it.** When a constant bound such as `i < 16` makes IDO unroll an
  otherwise-correct byte copy, compare `&source[i]` against a separate end
  symbol instead. For incomplete external arrays, IDO cannot fold the trip
  count, but it can still strength-reduce the typed source and destination
  indexing into the target's simple pointer walk.
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
- **Solve the frame arithmetically instead of guessing at padding locals.** The
  frame is `round8(argArea + saveArea + T + L)`, where `L` is the declared-locals
  block (top-anchored at the frame size, packed top-down in declaration order)
  and `T` is the compiler's temp area below it. `argArea` and `saveArea` are
  readable straight off the target's prologue, and `T` has a floor that is fixed
  for a given instruction stream: once opcodes and registers match, build two
  candidates with different `L` and solve for it. From then on the target's `L`
  is arithmetic, not search — and if the required `L` is impossible, that is a
  proof about the source rather than a plateau.
- **`L` is set by the deepest local that needs memory, not by the declaration
  count.** IDO assigns every local a virtual offset in declaration order, but
  reserves frame space only down to the deepest one that actually needs a home
  (spilled, address-taken, or an aggregate). Locals declared *below* that one
  are free if they stay in registers, which is a clean way to add a carrier
  without moving any existing slot. Conversely, a narrow local declared *above*
  a spilled one is what holds the block's size up, so deleting it moves every
  lower home.
- **Read the compiler's own answer rather than inferring stack homes.** IDO emits
  an `.mdebug` section that the project build strips. Recompiling the same
  source with `-g3` added keeps `-O2` codegen and lists every local as an
  `stLocal`/`scAbs` symbol whose home is `frameSize + value`. That turns "which
  variable owns this offset" from a guess into a lookup, and it is how a frame
  model should be validated before any variant is spent on it.
- **A narrow spill below the locals block is a compiler temp, not a local.** A
  spill of a declared narrow local goes to that local's own home, so an `sh`/
  `lhu` or `sb`/`lbu` pair at an offset *below* the block bottom identifies a
  compiler expression temp — its slot sits immediately under the locals block.
  Reaching that slot needs the value to be an expression rather than a named
  variable; a narrowing conversion used at several sites is the usual producer,
  and it is worth grepping already-matched functions for the shape before
  inventing spellings.
- **uopt will not hoist control flow out of a loop.** A branch-selected value
  (ternary, `&&`/`||` in value position) written inline inside a loop is
  recomputed every iteration, whatever it reads. Caching its operands in locals
  does not help: the obstacle is the branch, not the memory access. So a
  loop-carried value whose definition needs a branch cannot be loop-invariant
  code motion's output, and must be a named local in the preheader.
- **Combine block scopes with declaration order to tune small frames.** Moving
  non-overlapping pointer iterators into their actual loop scopes can shrink an
  otherwise-correct frame without changing the instruction stream. An unused
  `register` local immediately after a value that crosses calls can then shift
  that value's spill slot within the smaller frame; test the two changes
  together because either one alone may choose a different frame size.
- **Named CSE temporaries can enlarge a frame without changing instructions.**
  IDO may give explicit intermediate locals stack homes even when optimization
  keeps their values entirely in registers. Repeating the clean expression and
  letting IDO perform common-subexpression elimination can preserve the exact
  arithmetic and registers while removing an otherwise unexplained rounded
  frame-size increment.
- **Repeated GBI macro expansion sites can dominate a large frame.** Dynamic
  display-list macros declare scoped pointer temporaries. IDO's frontend can
  reserve stack storage per source expansion even when those temporaries do not
  survive into recognizable loads and stores. Replacing a straight run with a
  natural counted loop leaves one macro expansion site and can therefore shrink
  the frame substantially; use this as source-lineage evidence, then verify that
  the optimizer reproduces the target's unrolled command stream before keeping
  the loop.
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
- **A signed carrier defeats CSE between two macro masks of the same value.**
  GBI's `_SHIFTL(v, s, w)` masks through `(unsigned int)(v) & mask`, while
  hand-written macro arithmetic such as `gDPLoadTLUT_pal16`'s
  `(256 + (((pal) & 0xf) * 16))` masks `v` directly. When the same variable
  feeds both, uopt only CSEs the two masks if the variable's own type is
  unsigned — with an `s32`/`int` carrier the two `and`s are different
  expressions and IDO emits `andi` twice, one of them redundantly masking an
  already-masked value. If the target shows a single `andi` whose result is
  reused across the two expansions, make the carrier unsigned *and* pass it to
  the macro unmasked; pre-masking the argument yourself re-creates the second
  `andi`. The same reasoning applies to any pair of macros where one casts to
  `unsigned` before masking and the other does not.
- **Arithmetic on a `u16` local zero-extends first; the `s16` field does not.**
  `u16 m = obj->s16Field; ... m - 1` emits `andi ...,0xffff` before the
  `addiu`, because the local's declared type forces a 16-bit-clean value.
  Reading the `s16` field directly (`obj->s16Field - 1`) reuses the already
  sign-extended register and emits only the `addiu`. When a target shows the
  bare `addiu` at one site and the `andi` at another, that is evidence of two
  differently typed source expressions, not one reused local.
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
- **Preserve short BSS contributions at assembly time, not by rewriting ELF
  objects.** IDO's object output and GNU `as` normally round a `.bss` section
  up to its 16-byte alignment. When the target needs a data-only C object's
  logical BSS size, compile it to IDO assembly and assemble that output with
  GNU `as -no-pad-sections`; apply the same assembler option to adjacent raw
  BSS objects. Verify the unlinked section sizes with `readelf` before linking.
- **Local definition vs. `extern` affects codegen.** IDO generates different
  code for globals defined in the current TU (can keep base addresses in
  registers across adjacent stores) versus globals declared `extern` (can
  force extra address reloads and worse delay-slot scheduling).
- **A shared `lui $at` across two stores means the symbols are TU-defined.**
  For `extern` globals, every macro store expands with its own `lui $at`;
  IDO only emits one `lui` serving two `%lo` stores (often with the second
  store in a branch delay slot) when it laid out both objects itself. If the
  target shows `sb x,%lo(A)($at)` and `sh y,%lo(B)($at)` off a single `lui`,
  define the spanning object (typically a struct/union covering both
  addresses) in the file, give the file a `.bss` split at that VRAM, and add
  an alias linker script for the interior symbol names that other TUs still
  reference. Splat names interior offsets as separate symbols, so the reloc
  names in a compiled attempt (`A+4`) and the target (`B`) can differ while
  the linked bytes are identical.
- **Counting `lui $at` per store tells you how many symbols the source saw.**
  A direct reference to a whole global (scalar, or an array/struct element at a
  constant index) folds the offset into the `%lo` and costs one `lui $at` each
  time. But once one *object* is referenced enough times in a region and a
  register is free, uopt promotes `&object` to a base register and every later
  access becomes `off(base)`. So `N` stores to `N` distinct globals emit `N`
  `lui $at`, while `N` stores to `N` fields of one aggregate emit one
  `lui`/`addiu` pair plus `N` offset stores. If the target shows a fresh
  `lui $at,%hi(S)` before every `%lo(S+k)` store, the source did **not** see
  those slots as one aggregate, no matter how the extractor named them --
  splat labels any address inside a sized symbol as `S+k`, which hides the
  distinction. Re-model the storage as separate globals; a struct/union
  overlay macro can keep the aggregate spelling for the other translation
  units that genuinely index it with a variable.
- **Mixed folded and based addressing off one symbol means two source views.**
  When part of a run addresses `%lo(S+k)($at)` and the tail addresses
  `k(reg)` off a materialized `&S`, the tail came through a pointer or cast
  expression (for example a `(Layout *)&S` overlay), whose address *is* a
  common subexpression and therefore gets promoted, while the plain global
  references never form one. Reproduce both spellings rather than picking one.
- **Two names for one address suppress IDO's cross-block load CSE.** If the
  target reloads a global that your candidate keeps in a register across
  branches, and no call or aliasing store separates the two reads, the
  original source most likely read the address through two different objects
  (a struct member in one place, a standalone global in the other). uopt
  value-numbers by object, so distinct symbols never merge. A linker-script
  alias (`aliasName = realSymbol;`) plus an `extern` declaration reproduces
  this without moving any storage, and leaves the linked bytes identical.
  Prefer it to `volatile`: a volatile access materializes the address into a
  register (`lui`/`addiu`/`0(reg)`) instead of emitting the target's direct
  `lui`/`%lo` pair.
- **A linker-script alias is the cheap way to test a second declaration.**
  `aliasName = realSymbol;` in a `-T` script plus an `extern` of the right
  type gives uopt a distinct object at the same address without moving any
  storage, so the experiment is reversible and the linked bytes never change.
  Test the placement one read at a time: aliasing every reference is usually
  worse than aliasing the two or three the target actually reloads, and an
  alias in the wrong place adds a load the target does not have.
- **Base-register promotion is register-availability driven, so it does not
  always yield to naming.** uopt promotes the address of an object only when a
  register is free at that point, which is why the same symbol can be promoted
  in one region of the target and folded in another. If splitting or aliasing
  the symbol does not change the addressing form, stop: the residual is
  downstream pressure from some other region, and it usually resolves when
  that region becomes faithful rather than by more edits at the promotion site.
- **Consolidating extracted symbols into aggregates can silently cost a
  match.** "Clean up the `D_xxxxxxxx` symbols into one struct" commits are
  invisible to the ROM checksum but change every access in unmatched
  functions from folded to based addressing. When a partial match regresses
  for no apparent source reason, check whether the symbols it reads were
  merged since the score was recorded.
- **Try a fixed-trip array loop before adding interior-symbol aliases.** For
  small `extern` multidimensional arrays, IDO can fully unroll the loop while
  still emitting a fresh address load for each interleaved row access. The
  equivalent straight-line field assignments may instead cache the aggregate
  base, even though both forms perform the same copies.
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

- **The relocation table decides between two spellings that address the same
  byte.** Adjacent globals make several source expressions name one address:
  with `u8 A[4][4]` immediately followed by `u8 B[4][3]`, `B[0][i - 1]` and
  `A[4][i - 1]` are the same pointer. They are not the same object. The first
  relocates against `B` and needs an explicit `addiu -1`; the second relocates
  against `A` with addend 0xf and folds into the load displacement, which is
  what the target did. `decomp-workbench compare --symbol F target.o cand.o`
  lists relocation target differences directly - check them before treating a
  residual as register allocation, because a wrong symbol also drags in extra
  address arithmetic and can look structural.

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
- **An aligned source split can preserve orphan late-rodata padding without
  assembly.** When a logical flow boundary is also 16-byte aligned in `.text`,
  split the C segment there and split its `.rodata` before the next aligned
  constant or jump table. The first object's natural section-end padding can
  supply the required zero word while the second object's late rodata starts at
  the original address. Confirm both object section sizes and the full checksum.
- **Choose the C form according to the original float's section.** With IDO
  5.3, a scalar global `const f32` can land in `.data`, a global const array
  lands in early `.rodata`, and a float literal used by a function lands in
  late rodata. When removing a `GLOBAL_ASM` that owns late-rodata float
  constants, using the equivalent literals can preserve their ordering and
  addresses when no other code needs the symbols.
- **YAML comments are hints, not proof.** Verify actual function/file
  boundaries against disassembly, `symbol_addrs.txt`, and the linker map.

## Matching workflow

- **Drive large source sweeps with the project's own scorer, not a proxy.**
  Three metrics can rank the same candidate set in three different orders.
  Positional instruction-word mismatches favour whichever variant happens to
  align, and will crown a candidate that merely moved a block: relocating a
  22-instruction region scored 210 positional words against a 484 parent while
  being strictly further away. An opcode-aligned edit script (insertions +
  deletions + residual) is better but still disagrees. `dist.py` is
  decomp-permuter's scorer — 100 per insertion/deletion, 60 per reordering, 5
  per register difference — and it is the number the repository records, so it
  is the only ranking that decides whether a candidate is an improvement. On
  one 60,000-variant sweep the two proxy winners scored 85.494% and 90.159%
  against the parent's 90.694%.

- **A function's codegen does not depend on the rest of its translation unit.**
  Compiling a candidate standalone against the project headers, with the
  per-function workspace's flags, reproduces the in-project object's function
  bytes and `dist.py` score exactly — verified by recompiling five already
  matched siblings from one file through both paths, all
  `instruction-words-identical`. Standalone compiles are far cheaper than
  rebuilding the whole unit, so use them for sweeps and reserve the full-unit
  build for the final verification. Do not assume "this function is sensitive
  to its surrounding file" without measuring it; that claim has been recorded
  before and did not hold up.


- **Rebuild a per-function workspace baseline from the project's own headers,
  not from an archived scratch prelude.** The same function body scored 95.749%
  against a decomp.me-style typedef prelude and 98.851% against the real
  project headers in the same workspace with the same compiler. A prelude that
  merely compiles still models a different translation unit, so every ranking
  taken against it is measuring the wrong program. Reconstruct the baseline
  first and confirm it reproduces (or beats) the score recorded in the source
  comment before starting a campaign.

- **A recorded best-match percentage is only meaningful with its environment.**
  Scores from a per-function workspace depend on the target object's symbol
  names and on whatever local type definitions the candidate carried. After a
  project-wide symbol rename or struct consolidation, an old candidate can
  score twenty points lower without its source having changed. Re-measure the
  in-tree body against a freshly bootstrapped workspace before treating a
  recorded number as the bar to beat, and prove the harness first by
  recompiling a previously recorded candidate in its own workspace.

- **Prove line-number insensitivity before chasing statement-line levers.**
  This project builds without `-g`, and inserting blank lines anywhere in a
  function produces a byte-identical object. External guidance that points at
  statement line assignment, `#line` ties or physical-layout reflow assumes a
  debug-bearing build; run the blank-line control first and skip the whole
  family when it comes back inert.

- **Rank near-matches on object truth, not only on the workspace score.**
  `dist.py`'s sequence cost can rank a candidate holding an extra instruction
  above one whose instruction count already equals the target: the extra
  instruction shifts every later row, but the scorer's alignment absorbs it
  while a genuinely different schedule is penalised. Before concluding which
  of two near-matches is closer, compare instruction count, frame size, and
  positional word mismatches. Two candidates can differ by 0.4% in the
  workspace score and by an order of magnitude in positional word mismatches,
  in opposite directions. The gap can be far wider than that: a candidate with
  six wrong relocations and 268 of 348 positional opcodes differing scored
  *better* than one with zero wrong relocations and 28, because the scorer's
  insertion/deletion/reordering penalties dwarf its register penalty and its
  diff realigns shifted code for free. When the two disagree, the relocation
  table and the positional opcode count are the honest oracle.
- **Verify a fake is load-bearing before keeping it.** Scaffolding accumulated
  across attempts is not automatically doing work. Recompile with each fake
  removed individually and compare object hashes: `if (1) {}` blocks,
  dead-variable-reuse tricks like `dead = expr, dead` for a repeated macro
  argument, `ptr[1].field` plus a later `ptr++` in place of `&base->arr[i]`
  and `ptr->field`, and `if (c >= 0) { ... }` versus an early `if (c < 0)
  return;` are all frequently inert. Ones that are not — a `volatile` narrow
  parameter forcing a stack reload, a redundant `& 0xFFFF` — show up
  immediately as a large regression.
- **Declared locals are not free in a GBI-heavy leaf.** Naming the viewport
  bounds (`minX`, `maxY`, `halfHeight`, ...) in a function that already keeps
  several values live pushes them out of registers and gives each a stack home,
  growing the frame in 8-byte steps. If the target's frame is fixed and its
  bounds live in scratch registers, they were anonymous common subexpressions;
  copying a matched sibling's named-bound idiom into a busier function will
  fail on frame size alone.

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
- **Confirm last-mile scheduling in the original translation unit.** A natural
  candidate can retain a load-delay `nop` or prepare a call argument one
  instruction later in a single-function workspace, yet schedule exactly in
  the real source file. Before preserving an unnatural expression solely to
  fix a one-instruction scheduling mismatch, compile the clean candidate in its
  original translation unit and diff it there; the full ROM verification remains
  authoritative.
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
