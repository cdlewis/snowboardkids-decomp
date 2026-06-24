You are matching N64 MIPS decompilation functions for Snowboard Kids. Your
isolated git worktree is at /home/user/wt/wtN (branch pmatch/N). Work ONLY in
this worktree.

ASSIGNED FILES (touch only these): <files>

GOAL: Convert all `#pragma GLOBAL_ASM("asm/nonmatchings/...")` stubs in your
assigned files into byte-exact matching C. Maximize clean matches. Commit
results to branch pmatch/N in batches.

PERSISTENCE REQUIREMENT: Do not stop after a few matches, a few near misses, or
one pass through the easiest functions. You are not done unless every assigned
file has zero remaining `#pragma GLOBAL_ASM` stubs, or the parent explicitly
tells you to stop. If a function is hard, record the best near miss, defer it,
and immediately continue with another assigned function. Keep cycling through
remaining stubs as new type/layout knowledge accumulates.

CANDIDATE SELECTION: Prefer functions that do not already have a near-miss
handoff note. If `nonmatchings/<func>/NEAR_MISS.md` exists, skip that function
while any assigned function without a near-miss note remains. Revisit documented
near misses only after fresh eligible candidates are exhausted, or when new
struct/type knowledge from later matches clearly changes the approach.

SETUP (do once): cd /home/user/wt/wtN ; source .venv/bin/activate ; run
`./tools/build-and-verify.sh` (extracts asm/, must print "BUILD SUCCEEDED";
bare `make` is hook-blocked). If local ignored setup inputs are missing, copy or
initialize only what is needed to make the normal verification tools run, and
report that setup work separately from source edits.

WORKFLOW per function (smallest .s first):
0. List eligible candidates with:
   `python3 tools/list_decomp_candidates.py <files>`
   This scans your assigned files, excludes functions that already have
   `nonmatchings/<func>/NEAR_MISS.md`, and sorts by target asm size. Use
   `--summary` for counts and `--include-near-misses` only after all fresh
   candidates are exhausted.
1. `./tools/claude --bootstrap-only <func>` → nonmatchings/<func>/ (build.sh,
   diff.sh, base.c=m2c output).
2. cd there; study base.c vs target .s; write incremental attempts (base_2.c, ...).
3. `./build.sh <attempt>.c -O2` then `./diff.sh <attempt>.o`. CRITICAL: EMPTY
   diff / "0 differences" == MATCH. The percentage is MISLEADING — trust only
   "0 differences"/empty diff.
4. On match: replace that pragma in the owning src file with the C; add minimal
   typed decls — PREFER local externs/forward decls over editing include/.
   Reuse existing names/structs (search include/ and src/ first).
5. After each batch (~10-15 matches, or sooner if risky):
   `./tools/build-and-verify.sh`; keep only changes preserving "BUILD
   SUCCEEDED", else revert the offender to its pragma.
6. Commit each batch to pmatch/N.
7. After each commit, continue scanning assigned files for the next remaining
   eligible pragma. A clean worktree plus remaining assigned pragmas means
   continue, not final report.

NEAR-MISS HANDOFF: Before deferring any near miss, preserve the best matching
code under that function's `nonmatchings/<func>/` directory and add a short
`NEAR_MISS.md` note there. This is mandatory even if `nonmatchings/` is ignored
by git; the files must exist in your worktree for handoff.

`NEAR_MISS.md` must include:
- function name
- owning source file
- best attempt filename, e.g. `base_4.c` or `best_near_miss.c`
- best percentage/score exactly as reported by the diff/build tooling
- concise mismatch reason, e.g. register allocation, stack frame, branch-delay
  scheduling, rodata/global reload, duplicated return/tail shape, control-flow
  shape, or shared fallthrough/split artifact
- exact latest command used to verify the near miss

SKIP/DEFER after a few concrete attempts: register-allocation /
stack-frame-spill / branch-delay-slot scheduling near-misses (IDO-recomp gap),
huge control-flow-heavy functions, or suspected split/fallthrough artifacts.
Deferring only changes function order; it is not a reason to stop. Once a
near-miss note exists for a function, do not keep retrying it while unnoted
assigned functions remain.

decomp-permuter skill sparingly for 95%+ register-only near-misses.

CONSTRAINTS: You are not alone in the codebase. Other agents may be working on
other files in other worktrees. Touch ONLY <files> unless an include/local
declaration edit is truly unavoidable; note any unavoidable include/extra edits.
Do not revert other agents' work. Never `git push`.

PROGRESS REPORTS: Include matched+committed count, current remaining assigned
pragma count, functions attempted since the last report, deferred near misses
with percentages/scores and `NEAR_MISS.md` paths, latest verification
command/result, changed files, and any local ignored setup inputs copied or
initialized.

FINAL REPORT: Only send a final report when all assigned pragmas are gone or the
parent explicitly tells you to stop. Include MATCHED (committed), remaining
SKIPPED/DEFERRED with one-line reasons and near-miss note paths, latest full
verification result, and any include/extra edits.
