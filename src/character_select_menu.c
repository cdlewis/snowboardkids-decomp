#include "common.h"
#include "effect_task_scheduler.h"
#include "asset_decompression.h"
#include "character_select_menu.h"
#include "input_task_scheduler.h"

typedef struct {
    char pad[0x18];
    s32 fade;
    s32 timer;
} CharacterSelectMenuState;

// exitMode/readyCount are misnomers kept for continuity with earlier
// GLOBAL_ASM comments - they are not "reason for exit" or "players ready".
// Both are driven by the race_hud.c actor tasks func_80004960 schedules for
// this scene's UI widgets (banner/panel/message actors), not by player
// input state:
//   exitMode: a panel/UI display-mode stepper. func_800177F8 (fully
//     matched) sets it to 1 when the "select your character" banner
//     finishes fading out; func_80017C34 (fully matched, see PR matching
//     it) reads it to drive the player-panel's slide-in Y position. Later,
//     func_80005290 separately reuses the same field with value 3 to mean
//     "confirmed, exiting the scene" - that's just its most visible use,
//     not its only one.
//   readyCount: set to a hardcoded 1 by func_80017F94 (fully matched) once
//     a sliding message-banner actor finishes moving into position - a
//     UI-animation-complete flag, not a per-player ready count. Confirmed
//     live via BizHawk: freezing this at 0 during character select blocks
//     the player-selection panel from ever appearing/updating, while
//     character-sprite animation (driven separately) keeps running.
typedef struct {
    u8 phase;
    u8 exitMode;
    u8 readyCount;
    u8 pad3;
    char pad4[4];
    s16 fade;
    s16 unkA;
    u8 confirmSelection;
    u8 unkD;
    u8 blockDecrease;
    u8 blockIncrease;
    s16 cursorX;
    s16 cursorY;
    s8 playerSelections[4];
} CharacterSelectState;

typedef struct {
    u16 characterId;
} CharacterId;

typedef struct {
    char pad0[5];
    u8 characterId;
    char pad6[2];
    u8 isCpu;
    char pad9[0x603];
} CharacterSelectPlayer;

extern s32 func_80013F88(s32, s32, s32);
extern void func_80045914(void);
extern void func_80072138(s32, s32);
extern void func_80004CC8(void);
extern void func_80005468(void);
extern void func_800704F0(void);
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_800720E4(s32);
extern void func_800179D4(EffectTask *);
extern void func_80017D08(EffectTask *);
extern void func_80018060(EffectTask *);
extern void func_800183DC(EffectTask *);
extern void func_80018B6C(EffectTask *);
extern void func_800191A0(EffectTask *);

extern CharacterSelectMenuState *D_801235B8;
extern CharacterSelectState D_8010AE50;
extern CharacterId D_800B3400[];
extern CharacterSelectPlayer D_80121D80[];
extern EffectTask *D_8010ADDC;
extern EffectTask *D_8010ADE0;
extern EffectTask *D_8010ADE4;
extern EffectTask *D_8010ADE8;
extern EffectTask *D_8010ADEC;
extern f32 D_800E0980;
extern s16 D_800DEF14;
extern s16 D_8010ADF0[];
extern s8 D_800DEED4;
extern s8 D_800EC9C1;
extern s8 D_8010AE53;
extern s8 D_8010AE54;
extern s8 D_8010AE55;
extern s8 D_8010AE56;
// Per-player "currently highlighted" index. Confirmed live via RAM
// watch/poke (BizHawk), not derivable from source alone:
//   During character-roster browsing (CharacterSelectState.phase == 3, the
//     interactive steady state - see the phase sequence documented on
//     func_80004CC8 below): index into the D_800B3400 lookup table, NOT the
//     same value as CharacterSelectPlayer.characterId (see func_80004960
//     above, which matches player->characterId against D_800B3400[j] and
//     stores j here). Observed: 1 = Slash, 2 = Nancy, 3 = Jam, 4 = Linda,
//     5 = Tommy. 0 or 6 = Shinobin (ambiguous - not unlocked in the test
//     savegame, so which one couldn't be confirmed).
//   During board-type selection: 0 = Free Style, 1 = All Around, 2 = Alpine.
//     3 highlights the "back" button. 4 and 5 block confirming but still
//     allow navigating back to 0-2 - likely locked/unowned special boards;
//     exact meaning of 4 vs 5 (and whether there are more) not yet confirmed.
//     IMPORTANT: this is address reuse, not the same active phase/struct as
//     character-roster browsing. CharacterSelectState.phase is confirmed
//     live to go stale at 4 (the "is this ok?" confirm dialog) once you
//     leave the character-select scene, and never takes on a "board type"
//     meaning. Board-select is driven by different, still-unidentified code
//     that happens to reuse this same address for its own cursor index.
extern s8 D_8010AE64[];
extern s32 D_801235B4;
extern s32 D_80123778;
extern u8 D_245A80[];
extern u8 D_24C8E0[];
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_59AAA0[];
extern u8 D_59DFE0[];
extern u8 D_60F1A0[];
extern u8 D_60F990[];
extern u8 D_800B3190;
extern u8 D_800EC9E5;
extern u8 D_8010ADF8;
extern u8 D_8010AE68;
extern u8 D_80121B55;
extern u8 D_80123750;
extern u8 D_80123751;

// func_80004960 best match: 90.714%
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_menu/func_80004960.s")

#ifdef NON_MATCHING
void func_80004960(void) {
    CharacterSelectPlayer *player;
    CharacterId *character;
    CharacterSelectState *state;
    s32 i;
    s32 j;
    s32 playerCount;
    u8 *selectionPtr;
    s16 *timerPtr;
    s16 *timerEnd;

    if ((D_800EC9E5 == 0) || (D_8010ADF8 == 1)) {
        func_800720E4(1);
        func_800704F0();
        func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, D_800E0980);
        D_800DEED4 = 0;
        D_800EC9C1 = 0;
        D_8010ADF8 = 0;
        D_801235B8->fade = 0xFF;
        D_801235B8->timer = 0;
        D_800DEF14 = D_801235B8->fade;
        func_800437F0(D_59AAA0, D_59DFE0, 0x21);
        func_800437F0(D_245A80, D_24C8E0, 0x1F);
        func_800437F0(D_593D10, D_598A70, 0x22);
        func_800437F0(D_598A70, D_59AAA0, 0x23);
        func_800437F0(D_60F1A0, D_60F990, 0x29);
        func_80070EC0(0);

        playerCount = D_80121B55;
        i = 0;
        if (playerCount > 0) {
            player = D_80121D80;
            do {
                character = D_800B3400;
                j = 0;
loop_1:
                if (player->characterId == character->characterId) {
                    D_8010AE64[i] = j;
                } else {
                    j++;
                    character++;
                    if (j != 6) {
                        goto loop_1;
                    }
                }
                i++;
                player++;
            } while (i < playerCount);
        }
    } else {
        D_801235B8->fade = 1;
        D_800DEF14 = 1;
        playerCount = D_80121B55;
        i = 0;
        if (playerCount > 0) {
            selectionPtr = (u8 *) D_8010AE64;
            player = D_80121D80;
            do {
                j = i + 1;
                player->characterId = i;
                i = j;
                player++;
                selectionPtr++;
                selectionPtr[-1] = j;
            } while (j < playerCount);
        }
    }

    D_8010ADDC = func_80071408(func_800179D4, 0, 0x64);
    D_8010ADEC = func_80071408(func_800191A0, 0, 0x64);
    D_8010ADE8 = func_80071408(func_800183DC, 0, 0x64);
    D_8010ADE0 = func_80071408(func_80018060, 0, 0x64);
    func_80071408(func_80018B6C, 0, 0x64);
    D_8010ADE4 = func_80071408(func_80017D08, 0, 0x63);
    func_8009956C(func_80004CC8, 0);

    playerCount = D_80121B55;
    if (playerCount > 0) {
        timerPtr = D_8010ADF0;
        timerEnd = timerPtr + playerCount;
        player = D_80121D80;
        do {
            timerPtr++;
            player++;
            player[-1].isCpu = 0;
            timerPtr[-1] = 0;
        } while ((u32) timerPtr < (u32) timerEnd);
    }

    state = &D_8010AE50;
    state->phase = 0;
    state->exitMode = 0;
    state->readyCount = 0;
    state->fade = 0;
    state->unkA = 0;
    state->confirmSelection = 0;
    state->unkD = 0;
    state->blockDecrease = 0;
    state->blockIncrease = 0;
    state->cursorX = 0;
    state->cursorY = 0;
    D_8010AE53 = 0;
    D_8010AE54 = 0;
    D_8010AE55 = 0;
    D_8010AE56 = 0;
    D_8010AE68 = 0;
    if (D_800B3190 != 0) {
        D_8010AE68 = 1;
    }
    func_8007105C();
}
#endif

// func_80004CC8 best match: 52.734% (nonmatchings/func_80004CC8-7273315160691878794/base_4.c)
// This is the per-frame driver for the character roster screen only (NOT
// board-type selection - see the D_8010AE64 comment above). Confirmed live
// via BizHawk RAM watch, CharacterSelectState.phase sequences as:
//   0 = transient init value set by func_80004960, essentially invisible
//   1 = "select your character" banner shown, non-interactive
//   2 = not directly observed (likely a 1-frame transition)
//   3 = interactive roster, steady state
//   4 = the "is this ok?" Yes/No confirm dialog (func_80005290, gated on
//       CharacterSelectState.confirmSelection)
// After phase 4 is confirmed, this scene tears down and CharacterSelectState
// is never touched again for the rest of the flow (mode/course/board select,
// race).
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_menu/func_80004CC8.s")

// func_80005290 best match: 85.264%
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_menu/func_80005290.s")

#ifdef NON_MATCHING
void func_80005290(void) {
    CharacterSelectState *state;
    CharacterSelectPlayer *player;
    u8 *readyPtr;
    u8 *readyEnd;
    s32 oldSelection;
    s32 buttons;
    s32 selection;
    u8 playerCount;

    state = &D_8010AE50;
    if (state->fade == 0x100) {
        buttons = D_80123778;
        selection = state->confirmSelection;
        oldSelection = selection;

        if ((buttons & 0x10800) && (selection != 0)) {
            state->confirmSelection = selection - 1;
            selection = (u8) (selection - 1);
        } else if (buttons & 0x20400) {
            if (selection == 0) {
                state->confirmSelection = selection + 1;
                selection = (u8) (selection + 1);
            }
        }

        if (selection != oldSelection) {
            func_80072138(0x19, 0x32);
            buttons = D_80123778;
        }

        if ((buttons & 0x8000) || (buttons & 0x1000)) {
            func_80072138(0x18, 0x32);
            state = &D_8010AE50;
            if (state->confirmSelection == 0) {
                func_8009956C(func_80005468, 0);
                state = &D_8010AE50;
                state->cursorX = 0x8C;
                state->cursorY = 0x44;
                state->exitMode = 3;
            } else {
                playerCount = D_80121B55;
                player = D_80121D80;
                if ((s32) playerCount > 0) {
                    readyPtr = (u8 *) &D_8010AE50;
                    readyEnd = playerCount + readyPtr;
                    do {
                        readyPtr++;
                        player++;
                        player[-1].isCpu = 0;
                        readyPtr[2] = 0;
                    } while ((u32) readyPtr < (u32) readyEnd);
                }
                func_8009956C(func_80004CC8, 0);
                state = &D_8010AE50;
                state->phase = 3;
                state->fade = 0;
            }
        } else if (buttons & 0x4000) {
            func_80072138(0x18, 0x32);
            playerCount = D_80121B55;
            player = D_80121D80;
            if ((s32) playerCount > 0) {
                readyPtr = (u8 *) &D_8010AE50;
                readyEnd = playerCount + readyPtr;
                do {
                    readyPtr++;
                    player++;
                    player[-1].isCpu = 0;
                    readyPtr[2] = 0;
                } while ((u32) readyPtr < (u32) readyEnd);
            }
            func_8009956C(func_80004CC8, 0);
            state = &D_8010AE50;
            state->phase = 3;
            state->fade = 0;
        }
    }
    func_8007105C();
}
#endif

void func_80005468(void) {
    if (D_801235B8->fade != 0xFF) {
        D_801235B8->fade = func_80013F88((s16) D_801235B8->fade, 0x24, 1);
        if (D_801235B8->fade == 0xFF) {
            D_80123751 = 1;
        } else {
            func_8007105C();
        }
    } else {
        if (D_80123750 == 2) {
            func_80045914();
            D_80123751 = 0;
            D_800DEED4 = 0;
            D_801235B4 = 0;
            func_80099658(2);
            func_8009954C(4);
        }
    }
}
