# Matching-game interface contracts

This source audit accompanies the upstream cleanup. The matching ROM is the
acceptance criterion for source changes. No gameplay or original-hardware visual
validation was performed. Recomp sampling, widescreen, audio clamps, framebuffer
readback policy, and save storage are separate portability work.

## Camera lifecycle

`gRaceCameras` contains four 0xB0-byte `RaceCamera` objects. `initialized` is the
byte union at 0xAC. Its unsigned `value` and signed `signedValue` members
preserve the original IDO constant sharing without duplicating the camera layout. `resetRaceCameras` clears it in camera zero, copies that camera to
all remaining slots, and assigns player indices. `fadeInRaceGameplayViewports`
activates participating cameras. Multiplayer viewport shutdown clears individual
slots; demo playback also activates cameras. Launch-ramp player updates can set
it to 2 before restoring 1. Positional audio considers only value 1 when measuring
distance. Course-select previews also use the array: viewport width alone cannot
identify gameplay activation.

The item texture-effect head array ends at the camera-array address. That use is
an end sentinel, not another camera layout. Audio's former `D_801124A0[]` end
sentinel was the camera array's end, not the current-camera pointer stored there.

## Pak protocols

`ControllerPakRumbleCheckPromptTransition` starts at 0x8010ADD0. Its state member
at +2 is the start of `gControllerPakRumbleCheckPromptState`; both views end at
0x8010ADDA. The confirmation byte at 0x8010ADD9 has a linker alias too. These are
views of shared bytes, not three allocations. Message indices have their own
meaning and width; they are not scale or worker status values.

| Rumble state | Actor/parent behavior |
| --- | --- |
| 0 | Open; choose insertion prompt or warning based on the parent's selection |
| 1 | Wait for insertion acknowledgment |
| 2 | Initialize motors for participating controllers |
| 3 | Acknowledge DO NOT REMOVE RUMBLE PAK |
| 4 | Fade closed |
| 5 | Remove actor and advance parent to character select |
| 6 | Initial motor probe, before deciding whether the warning can open |
| 7 | Hold after probe for the transition timer |
| 8 | Draw device summary, although message index remains 2 |
| 9 | Confirm device selection or return to insertion |

Message indices 0, 1, 2, 3 select insertion, checking, do-not-remove, and
confirmation text. State 8's summary is selected by state, not by text index.
Motor initialization precedes the warning; filtering text indices cannot safely
identify cosmetic-only pages.

The player-count prompt is a different protocol. State 5 displays CHECKING
CONTROLLER PAK and dispatches `initRaceSetupSaveMenu`; state 6 displays CHECK
COMPLETE and accepts confirmation; state 7 fades completion out; state 8 is
dismissed. Save panels may request state 6 again through `forceUpdate`. This
applies to one through four players, with work performed for participating
controllers. Skipping state 5 would bypass save initialization.

Worker results must be interpreted through their producers and consumers:
9 means no matching save with a free directory entry and sufficient space;
0xA follows absent/wrong-device probe results; 0xB means insufficient free blocks;
0xC means all sixteen file slots are used; 0x10 follows ID-fatal when the motor
probe identified a rumble device. Request phases 0 through 4 are not these
outcomes, and prompt states using the same numbers are unrelated.

`readControllerPakSave` sums bytes from offset 4 through 0x78DF and compares the
stored checksum. Read errors and checksum failures increment retries. Successful
semantic validation clears retries; a semantic rejection does not itself clear
or increment them on this path. Setup also initially clears retries. Thus retry
history matters; this cleanup does not assert that every invalid save is accepted
or change the original validation behavior.

## Shared menu scratch and render queues

The words at 0x8010ADE0, 0x8010ADE4, and 0x8010ADE8 are scene-owned scratch slots.
Character-select roster/frame/cursor actors, course-preview tasks, save-panel
actors, and an ending tumbling-snowboard actor reuse them. Reset-only users also
view them as scalar zero. The public union exposes these alternative views with
four-byte target pointers; it does not add storage or allow simultaneous owners.

The model callback bank occupies 0x80124898 through 0x801248F7: eight 0x0C slots.
Only the first word of each slot is traversed as a head, corresponding to former
pointer indices 0, 3, ..., 21. The other two words are retained without inferred
semantics. Existing named head aliases and the effect queue's special modelview
matrix setup must remain at their original addresses. Reset clears head words;
submission preserves bank order.

The unnamed queues at 0x801248C8, 0x801248E0, and 0x801248EC serve multiple effects
and course/UI producers. Single-effect names would overstate ownership. No C
producer was found for 0x80124848; it is checked before camera setup and cleared
on reset, so its address-based name remains.

Each of the 13 normal and 13 ghost model rows retains six texture variants and
its separate allocation. Body part n uses matrix n+1; zero is the snowboard.
No anatomical interpretation is inferred from table order.

## HUD and sprite coordinates

Item-effect offsets are eight bytes indexed by `itemEffectType`, then combined
with `itemEffectCount - 1`. Action-effect tile IDs are eight bytes indexed by
`actionEffectType`. Neither table represents timer digits.

Two-player callbacks differ from the three/four-player callbacks. Viewport HUD
work is queued before the shared progress meter. The meter uses the race overlay
queue in one-player modes and the menu queue in multiplayer. Entry 0x50 is drawn
at `(xBase + 4, yBase + 4)` relative to the menu viewport center. Sprite bounds
and visible strokes are different geometry. The three-player callback draws
popup-font entry 0x90 (NO ENTRY) in the unused quadrant; it does not draw dividers.

The two-player score formatter intentionally uses separate stack objects as
buffer/end boundaries. Matching depends on that source shape. A portable
replacement should allocate explicit storage including terminator space and
limit drawing to five characters; it is not introduced into this ROM.

For scaled asset-table helpers, scale is a right-shift exponent: 1 means half
size. The reduced sprite is centered within the original dimensions before
clipping: a 32-pixel image at scale 1 begins eight pixels after the supplied x
and spans sixteen pixels. Coordinates are viewport-relative and may be negative.
Only the final packed texture-rectangle coordinates need unsigned-range clipping.
Rectangle coordinates, s10.5 source origins, and derivatives have distinct units;
scaled clipping must advance source origins by the source/destination ratio.

The original scaled paths use dimension-valued inclusive tile endpoints and
+0.5 source origins. The portability reference instead uses dimension minus one,
-0.5 origins, and scaled source clipping. Original commands remain authoritative
for matching; renderer symptoms do not establish original-hardware appearance.

## Race starter sampling evidence

The two panels are 48x40 CI4. Loader argument order and texture-image format
identify the old `palette3A/B` fields as images and old `image3A/B` as palettes.
The eight vertices already contain s10.5 half-texel coordinates: horizontal
-16..1520, vertical -16..1264. Do not apply another half-texel offset.

For each panel, original LoadTile w1 is `0x070600A0` and SetTileSize w1 is
`0x000C00A0`, encoding inclusive (48,40) bounds. The portability patch's (47,39)
values are `0x0705E09C` and `0x000BC09C`. The matched macro reconstruction retains the
original values and all intervening synchronization, tile, palette, matrix,
vertex, and triangle commands.

## Runtime and audio

`D_369000` is physical 0x00369000, the GPU address alias of `gDepthBuffer` at
0x80369000. It is not an asset. Framebuffer users supply GPU operands or compare
presentation pointers; this does not authorize adding renderer readback policy.

`D_80328480` follows the game-thread stack. The apparent reference in
`bootThreadMain` is `gGameThreadStack + sizeof(gGameThreadStack)`, passed as the
stack top. No buffer initialization/use establishing a purpose was found, so
its size-derived region-allocation name is not promoted to a semantic symbol.

The four-byte `gFramebufferSubmissionCountdown` allocation has one semantic
counter at byte zero; the scheduler does not treat it as four independent
counters. Trailing bytes preserve storage. The counter controls task-submission
frequency, not VI presentation delay.

Audio uses stereo frames of two 16-bit samples (four bytes). AI queued bytes are
converted to frames with `>> 2`. Target length rounds up to a multiple of sixteen;
minimum is target minus sixteen, and allocation is `(target + 0x68) * 4` bytes.
`buildAudioTask` masks its computed frame count with `0xFFF0`, narrows to signed
16-bit `outLen`, then compares it as unsigned against the minimum. This assumes
a bounded hardware FIFO. A host queue can violate that assumption and turn a
negative narrowed count into a large unsigned byte request. The matching game
retains its arithmetic and ABI; host clamps belong in the port. Stop/start
critical sections continue protecting shared libmus state.

Title-demo reveal height is a byte measured in viewport-height units, advancing
by 0x10 from zero to 0xB0. Its original volatile access pattern is preserved.

## Verification evidence

The declaration cleanup, scratch/queue unification, and runtime/protocol cleanup
were each rebuilt to the original ROM SHA1. The starter macro candidate
`nonmatchings/renderRaceStartOverlay/base_3.c` scored 100.000% with zero assembly
differences before integration. It retains 32 commands, including both original
LoadTile and SetTileSize words. Earlier macro candidates differed in transform
copy scheduling and stack padding; no sampling correction was applied.

The integrated macro renderer also passes the full ROM checksum. Run
`./tools/build-and-verify.sh`, then `python3 tools/check-interface-layouts.py` to
repeat the ROM, MIPS-target layout, and linker-alias checks. The latter checks
field offsets and pointer widths without allocating any runtime test data.
