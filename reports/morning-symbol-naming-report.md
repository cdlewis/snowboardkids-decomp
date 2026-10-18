# Symbol naming review

Applied **155 placeholder-symbol renames** across US and JP, producing **121 descriptive names** because corresponding regional symbols share names. Updated 53 existing files: 46 C sources, 3 shared headers, 2 linker scripts, and 2 regional symbol files. No data definitions, initializers, types, function logic, or source ordering were changed.

The complete original inventory has **241 distinct address-style placeholders**: 213 used by C or headers and 28 found only in linker scripts or metadata. **86 entries remain unchanged**: 19 high-confidence linker-only aliases, 45 medium-confidence proposals, and 22 tentative proposals. Standard input constants such as `D_JPAD` are not placeholders.

The complete mapping, usage evidence, source references, confidence, and applied/deferred status are in [the naming inventory](placeholder-symbol-names.md) and [the CSV](placeholder-symbol-names.csv).

## What changed

- Named default save records, course checkpoint events, CPU board configurations, collectible placement lists, challenge time limits, and course texture-marker lists.
- Named sparkle, burst-text, stun-icon, snow-particle, projectile, and results-label data according to their actual consumers.
- Named the current camera pointer, selected-character token task, pause selection, result award recipient, and ghost-record state.
- Gave scheduler views and render queue heads structural names while retaining their overlapping addresses and opaque boundaries. Queue heads retain numeric queue indices because stronger pass semantics are unproven.
- Named podium display-list variants, rotating/static menu boards, and the physical depth-buffer alias without changing segmented addresses or linker equations.
- Reused existing US handler-table names for corresponding JP tables.
- Preserved existing symbol annotations and registered newly named symbols at baseline regional addresses. Added ROM annotations to newly registered main-segment data where applicable.

Examples:

| Original symbol | Applied name | Basis |
| --- | --- | --- |
| `D_801124A0` | `gCurrentRaceCamera` | Assigned before camera callback dispatch and used throughout camera updates. |
| `D_80121B57` | `gRacePauseMenuSelection` | Selects one of the three pause-menu rows. |
| `D_800B31C8` | `gDefaultTimeTrialRecords` | Copied into default save time-trial records. |
| `D_800D60A0` | `gRacePlayerSparkleQuadVertices` | Vertex load in sparkle rendering. |
| `D_800DC928` | `gScoreAttackTimeLimitsByCourse` | Course-indexed challenge time limit for score attack. |
| `D_801248C8` | `gModelRenderCallbackQueue4Head` | Head at queue base plus four 0xC-byte slots. |

## Verification

- Untouched US baseline: original SHA1 matched.
- Untouched JP baseline: original SHA1 matched.
- Final US build: original SHA1 matched (`1583bacc9046a360df8ea4d536942155247e154c`).
- Final JP build: original SHA1 matched (`96480c8fa7285d9e850f6ba002e56bcb7ada9796`).
- Regional address audit: 117 US and 88 JP renamed symbols retain their baseline addresses, including 15 US and 15 JP BSS symbols.
- Automated source audit: every changed C/header/linker file exactly matches identifier substitution against its saved baseline.
- Remaining placeholder audit: exactly 86 original names remain; none of the 155 renamed identifiers survives in tracked source/config/metadata.
- Original inventory coverage: all 241 symbols have a name/proposal, evidence, and a source reference; duplicate metadata names are checked before applying changes.

Validation commands: `./tools/build-and-verify.sh` and `./tools/build-and-verify.sh --version jp`. Both completed successfully before and after the renames. `git diff --check` also passes.

## Deferred work worth reviewing

`D_800D7650` is defined using `RaceUiRankTextRenderEntry` while another file consumes it as `RaceCoursePropModelEntry`. `D_800DBD10` is packed data defined in the audio file and consumed as replay course-grid records. Their ownership and layout should be understood before assigning a single authoritative type/name.

Unused zero words and nearby trailing data remain unchanged. Placement alone does not establish padding, and deleting or combining these objects could break matching.

Sound initialization tables have useful proposed roles, but the semantics of the `unk28` initialization parameters remain incomplete. Several JP description strings and the player `unk18` statistic also need further research. Their conservative proposals remain in the inventory.

Linker-only field aliases and shared-model display-list offsets remain unchanged in this pass. Typed member expressions may be preferable for the field aliases, provided the compiler preserves the required relocations. Visual object identity is still unknown for several shared-model display lists.

The two-camera boundary now named `gCourseSelectCameraUpdateEnd` remains separate from the existing four-camera `gRaceCamerasEnd`. The prize-table extension now named `gRacePrizeAmountsRemainingCourses` retains its original adjacency because payout code indexes across the original object boundary.

The commit includes the descriptive renames, regional symbol metadata, and these review reports.

## Follow-up: linker alias removal test

The naming pass renamed seven existing US linker aliases and two existing JP aliases; it did not introduce new aliases. Removing these nine entries was tested and failed at link time. The entries were restored.

US generates the descriptive absolute definitions in `undefined_syms_auto.txt`, but the US link command does not include that generated file. Symbol-address metadata alone therefore does not resolve these external references in the current US build. JP does link its generated undefined-symbol files, but the two alias names at duplicated segmented addresses are absent from the generated definitions; other names at those addresses are emitted instead.

Eliminating these aliases requires a deliberate change to asset references or external-symbol generation/linking. The alias-removal experiment did not change source references, symbol metadata, or build tooling.
