#include "common.h"
#include "assets.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/menu/character_select/character_select_menu.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/character_select/character_select_ui.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"

typedef struct {
    char pad[0x18];
    s32 fade;
    s32 timer;
} CharacterSelectMenuState;

typedef struct {
    u16 characterId;
} CharacterId;

CharacterSelectState gCharacterSelectHudState;
u8 D_8010AE68;

extern void releaseMenuAssetHandles(void);
extern void enqueueSoundEffect(s32, s32);
extern void updateCharacterSelectMenu(void);
extern void fadeOutCharacterSelectMenu(void);
extern void requestMusicSequenceBank(s32);
extern CharacterSelectMenuState *gCurrentGameTask;
extern CharacterId gCharacterSelectIdOrder[];
extern CallbackTask *gActiveMenuTask;
extern CallbackTask *D_8010ADE0;
extern CallbackTask *D_8010ADE4;
extern CallbackTask *D_8010ADE8;
extern CallbackTask *D_8010ADEC;
extern s16 gMenuFadeAlpha;
extern s16 gMenuInputRepeatTimers[];
extern s8 gFramebufferSwapDelay;
extern s8 gMenuSelectionConfirmTimer;
extern s8 D_8010AE64[];
// Per-player highlighted index. During character-roster browsing
// (gCharacterSelectHudState.phase == 3), this indexes gCharacterSelectIdOrder,
// not RacePlayer.characterId directly. Live-observed values: 1 = Slash,
// 2 = Nancy, 3 = Jam, 4 = Linda, 5 = Tommy; 0/6 are the two Shinobin slots.
// During later board-type selection, this address is reused by different code:
// 0 = Free Style, 1 = All Around, 2 = Alpine, 3 = back, and 4/5 appear to be
// locked/unowned special board states.
extern s32 gMenuFlowState;
extern s32 gPlayerInputPressed;
extern u8 D_60F1A0[];
extern u8 gMainMenuSecretCodeUnlocked;
extern u8 D_800EC9E5;
extern u8 gMenuExitSelection;
extern u8 gPlayerCount;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

void initCharacterSelectMenu(void) {
    s32 i;
    s32 j;

    if ((D_800EC9E5 == 0) || (gMenuExitSelection == 1)) {
        requestMusicSequenceBank(1);
        resetAllViewports();
        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
        gFramebufferSwapDelay = 0;
        gMenuSelectionConfirmTimer = 0;
        gMenuExitSelection = 0;
        gCurrentGameTask->fade = 0xFF;
        gCurrentGameTask->timer = 0;
        gMenuFadeAlpha = gCurrentGameTask->fade;
        LOAD_ASSET(_59AAA0, 0x21);
        LOAD_ASSET(_245A80, 0x1F);
        LOAD_ASSET(_593D10, 0x22);
        LOAD_ASSET(_598A70, 0x23);
        LOAD_ASSET(_60F1A0, 0x29);
        initCallbackTaskScheduler(0);

        for (i = 0; i < gPlayerCount; i++) {
            for (j = 0; j != 6; j++) {
                if (gRacePlayers[i].selectedCharacterId == gCharacterSelectIdOrder[j].characterId) {
                    D_8010AE64[i] = j;
                    break;
                }
            }
        }
    } else {
        gCurrentGameTask->fade = 1;
        gMenuFadeAlpha = 1;

        for (i = 0; i < gPlayerCount; i++) {
            gRacePlayers[i].selectedCharacterId = i;
            D_8010AE64[i] = i + 1;
        }
    }

    gActiveMenuTask = createCallbackTask(initCharacterSelectConfirmationBanner, 0, 0x64);
    D_8010ADEC = createCallbackTask(initCharacterSelectSelectedCharacterTokens, 0, 0x64);
    D_8010ADE8 = createCallbackTask(initCharacterSelectPlayerCursorMarkers, 0, 0x64);
    D_8010ADE0 = createCallbackTask(initCharacterSelectRosterIcons, 0, 0x64);
    createCallbackTask(initCharacterSelectPlayerStatsPanels, 0, 0x64);
    D_8010ADE4 = createCallbackTask(initCharacterSelectPlayerPanelFrames, 0, 0x63);

    setCurrentGameTaskCallback(updateCharacterSelectMenu, 0);

    for (i = 0; i < gPlayerCount; i++) {
        gRacePlayers[i].menuState = 0;
        gMenuInputRepeatTimers[i] = 0;
    }
    gCharacterSelectHudState.phase = 0;
    gCharacterSelectHudState.exitMode = 0;
    gCharacterSelectHudState.readyCount = 0;
    gCharacterSelectHudState.fade = 0;
    gCharacterSelectHudState.unkA = 0;
    gCharacterSelectHudState.confirmSelection = 0;
    gCharacterSelectHudState.unkD = 0;
    gCharacterSelectHudState.blockDecrease = 0;
    gCharacterSelectHudState.blockIncrease = 0;
    gCharacterSelectHudState.cursorX = 0;
    gCharacterSelectHudState.cursorY = 0;

    i = -1;
    while (++i < 4) {
        gCharacterSelectHudState.selectedTokenState[i] = 0;
    }
    D_8010AE68 = 0;
    if (gMainMenuSecretCodeUnlocked != 0) {
        D_8010AE68 = 1;
    }
    updateCallbackTasks();
}

// updateCharacterSelectMenu best match: 69.415% (nonmatchings/updateCharacterSelectMenu-5787290371232622032/base_46.c)
// Per-frame driver for the character roster screen only. Live RAM-watch
// testing observed gCharacterSelectHudState.phase as:
//   0 = transient init value
//   1 = "select your character" banner shown, non-interactive
//   2 = likely one-frame transition
//   3 = interactive roster
//   4 = "is this ok?" Yes/No confirmation dialog
#pragma GLOBAL_ASM("asm/nonmatchings/menu/character_select/character_select_menu/updateCharacterSelectMenu.s")

#ifdef NON_MATCHING
void updateCharacterSelectMenu(void) {
    u8 sp6A;
    u8 sp68;
    s16 sp64;
    void *sp54;
    u8 *readyBytePtr;
    s32 sp50;
    u8 cachedIsCpu;
    s32 sp48;
    s32 sp40;
    s16 *charSfxTable;
    CharacterSelectPlayer *var_s4;
    CharacterSelectState *var_v1_3;
    s16 *temp_t1;
    s16 *var_v0;
    s16 temp_t7;
    s16 temp_t8;
    s16 temp_v1;
    s32 var_ra;
    volatile int savedRa;
    s32 *var_s7;
    s32 temp_a1;
    s32 temp_s6;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a2;
    s32 var_s0;
    s32 var_s1;
    s32 var_t1;
    u32 var_v0_2;
    s32 var_v0_3;
    s32 var_v1_2;
    u8 *temp_fp;
    s8 temp_t2;
    s8 temp_v0_4;
    u16 temp_v0_3;
    u16 temp_v1_2;
    u16 var_v1;
    u8 temp_a3;
    u8 temp_t6;
    u8 var_t5;
    void *temp_v1_3;
    extern s32 D_80123758[];

    temp_v0 = D_801235B8->fade;
    if (temp_v0 != 0) {
        D_801235B8->fade = func_80013F88((s32) (s16) temp_v0, 0x24, 0);
        if (D_801235B8->fade == 0) {
            D_8010AE50.phase = 1;
        }
    } else if ((s32) D_8010AE50.phase < 3) {
        if ((D_8010AE50.phase == 1) && (D_8010AE50.fade == 0x100) && ((D_80123778 & 0x8000) || (D_80123778 & 0x1000))) {
            D_8010AE50.phase = 2;
            func_80072138(1, 0x32);
            temp_a3 = D_80121B55;
            var_v0 = &D_800EC9C8;
            if ((s32) temp_a3 > 0) {
                do {
                    temp_v1 = *var_v0;
                    if ((temp_v1 == 0x12) || (temp_v1 == 5)) {
                        *var_v0 = 0x13;
                    }
                    var_v0 += 1;
                } while ((u32) var_v0 < (u32) (&D_800EC9C8 + temp_a3));
            }
        }
    } else if (D_8010AE50.readyCount == 1) {
        var_s1 = 1;
        var_ra = 5;
        if (D_8010AE50.blockDecrease != 0) {
            var_s1 = 0;
        }
        var_s4 = D_80121D80;
        if (D_8010AE50.blockIncrease != 0) {
            var_ra = 6;
        }
        temp_a3 = D_80121B55;
        var_s0 = 0;
        var_a0 = 0;
        if ((s32) D_80121B55 > 0) {
            var_t5 = sp6A;
            var_s7 = (s32 *) &D_80123778;
            do {
                cachedIsCpu = var_s4->isCpu;
                sp48 = var_a0;
                if (cachedIsCpu == 0) {
                    temp_fp = (u8 *) &gCharacterSelectHudState.playerSelections[var_s0];
                    temp_t2 = *temp_fp;
                    temp_v0_2 = D_80123758[var_s0];
                    temp_a1 = temp_v0_2 & 0x40100;
                    temp_t1 = &D_8010ADF0[var_s0];
                    var_a0_2 = temp_t2 & 0xFF;
                    sp50 = (s32) temp_t2;
                    charSfxTable = D_800B340C;
                    if ((temp_a1 == 0) && !(temp_v0_2 & 0x80200)) {
                        *temp_t1 = 0;
                    }
                    temp_s6 = *var_s7;
                    temp_v0_3 = (u16) *temp_t1;
                    if ((temp_s6 & 0x80200) || ((temp_v0_2 & 0x80200) && ((s32) temp_v0_3 >= 0xB) && (temp_v0_3 & 1))) {
                        temp_v1_2 = (u16) *temp_t1;
                        if (temp_v1_2 == 0) {
                            *temp_t1 = temp_v1_2 + 1;
                        }
                        if (var_s1 < var_a0_2) {
                            var_a0_2 = (var_a0_2 - 1) & 0xFF;
                            var_t5 = 1;
                        }
                    }
                    var_v1 = (u16) *temp_t1;
                    if ((temp_s6 & 0x40100) || ((temp_a1 != 0) && ((s32) var_v1 >= 0xB) && (var_v1 & 1))) {
                        if (var_v1 == 0) {
                            temp_t8 = var_v1 + 1;
                            *temp_t1 = temp_t8;
                            var_v1 = temp_t8 & 0xFFFF;
                        }
                        if (var_a0_2 < var_ra) {
                            var_a0_2 = (var_a0_2 + 1) & 0xFF;
                            var_t5 = 2;
                        }
                    }
                    temp_t7 = var_v1 + 1;
                    if (var_v1 != 0) {
                        *temp_t1 = temp_t7;
                        if ((temp_t7 & 0xFFFF) == 0xFFFF) {
                            *temp_t1 = 0xC;
                        }
                    }
                    if (D_8010AE68 == 0) {
                        var_t1 = 0;
                        var_v1_2 = 0;
loop_48:
                        var_v0_2 = 0;
                        if ((s32) D_80121B55 > 0) {
                            do {
                                if ((var_v0_2 != var_s0) &&
                                    (var_a0_2 == gCharacterSelectHudState.playerSelections[var_v0_2])) {
                                    var_v1_2 = (var_v1_2 + 1) & 0xFF;
                                }
                                var_v0_2 += 1;
                            } while (var_v0_2 < (s32) D_80121B55);
                        }
                        var_t1 += 1;
                        if ((gCharacterSelectHudState.blockDecrease == 0) && (var_a0_2 == 0)) {
                            var_v1_2 = (var_v1_2 + 1) & 0xFF;
                        }
                        if ((gCharacterSelectHudState.blockIncrease == 0) && (var_a0_2 == 6)) {
                            var_v1_2 = (var_v1_2 + 1) & 0xFF;
                        }
                        if (var_v1_2 != 0) {
                            if (var_t5 == 1) {
                                var_a0_2 = (var_a0_2 - 1) & 0xFF;
                            } else {
                                var_a0_2 = (var_a0_2 + 1) & 0xFF;
                            }
                            var_a2 = var_a0_2;
                            if (var_a0_2 < var_s1) {
                                var_a0_2 = temp_t2 & 0xFF;
                                var_a2 = var_a0_2;
                            }
                            if (var_ra < var_a2) {
                                var_a0_2 = temp_t2 & 0xFF;
                            }
                        }
                        if (var_v1_2 != 0) {
                            var_v1_2 = 0;
                            if (var_t1 == 7) {

                            } else {
                                goto loop_48;
                            }
                        }
                    }
                    if (var_a0_2 != sp50) {
                        sp64 = var_ra;
                        sp40 = var_a0_2;
                        sp6A = var_t5;
                        func_80072138(0x19, 0x32);
                        var_ra = (s16) (u16) sp64;
                    }
                    *temp_fp = (s8) var_a0_2;
                    temp_v0_4 = *temp_fp;
                    var_s4->characterId = (u8) D_800B3400[temp_v0_4].characterId;
                    if (((*var_s7 & 0x8000) || (*var_s7 & 0x1000)) && (var_s4->isCpu == 0)) {
                        temp_v1_3 = (u8 *) &D_8010AE50 + var_s0;
                        sp64 = var_ra;
                        savedRa = (s16) (u16) sp64;
                        sp54 = temp_v1_3;
                        sp6A = var_t5;
                        func_80072138((s32) charSfxTable[temp_v0_4], 0x32);
                        var_ra = savedRa;
                        var_s4->isCpu = 1;
                        ((u8 *) temp_v1_3)[3] = 1;
                    }
                }
                var_a0 = sp48 + 4;
                if ((*var_s7 & 0x4000) && (cachedIsCpu != 0)) {
                    var_s4->isCpu = 0;
                    gCharacterSelectHudState.selectedTokenState[var_s0] = 0;
                }
                cachedIsCpu = var_s4->isCpu;
                var_s0 += 1;
                var_s4 = (CharacterSelectPlayer *) ((u8 *) var_s4 + 0x60C);
                var_s7 += 1;
            } while (var_s0 < (s32) D_80121B55);
            sp6A = var_t5;
            sp68 = cachedIsCpu & 1;
        }
        if (sp68 != 0) {
            var_v0_3 = 0;
            if ((s32) D_80121B55 > 0) {
                var_v1_3 = &D_8010AE50;
                do {
                    readyBytePtr = (u8 *) var_v1_3;
                    temp_t6 = readyBytePtr[3];
                    var_v1_3 = (CharacterSelectState *) ((u8 *) var_v1_3 + 1);
                    var_v0_3 += temp_t6;
                } while ((u32) var_v1_3 < (u32) ((u8 *) &D_8010AE50 + D_80121B55));
            }
            if (var_v0_3 == (D_80121B55 * 3)) {
                D_8010AE50.phase = 4;
                D_8010AE50.confirmSelection = 0;
                D_8010AE50.unkD = 0;
                D_8010AE50.unkA = 0x100;
                func_8009956C(func_80005290, 0);
            }
        }
    }
    func_8007105C();
}
#endif

// updateCharacterSelectConfirmationMenu best match: 91.299% (nonmatchings/updateCharacterSelectConfirmationMenu-2188069624939011928/base_15.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/character_select/character_select_menu/updateCharacterSelectConfirmationMenu.s")

#ifdef NON_MATCHING
void updateCharacterSelectConfirmationMenu(void) {
    CharacterSelectState *state;
    RacePlayer *player;
    u8 *readyPtr;
    u8 *readyEnd;
    s32 oldSelection;
    s32 buttons;
    s32 keepGoing;
    u8 playerCount;

    if (gCharacterSelectHudState.fade == 0x100) {
        buttons = gPlayerInputPressed;
        oldSelection = gCharacterSelectHudState.confirmSelection;

        if ((buttons & (STICK_UP | U_JPAD)) && (oldSelection != 0)) {
            gCharacterSelectHudState.confirmSelection = oldSelection - 1;
        } else if ((buttons & (STICK_DOWN | D_JPAD)) && (oldSelection == 0)) {
            gCharacterSelectHudState.confirmSelection = oldSelection + 1;
        }

        if (gCharacterSelectHudState.confirmSelection != oldSelection) {
            enqueueSoundEffect(0x19, 0x32);
            buttons = gPlayerInputPressed;
        }

        if ((buttons & A_BUTTON) || (buttons & START_BUTTON)) {
            enqueueSoundEffect(0x18, 0x32);
            if (gCharacterSelectHudState.confirmSelection == 0) {
                setCurrentGameTaskCallback(fadeOutCharacterSelectMenu, 0);
                gCharacterSelectHudState.cursorX = 0x8C;
                gCharacterSelectHudState.cursorY = 0x44;
                gCharacterSelectHudState.exitMode = 3;
            } else {
                playerCount = gPlayerCount;
                player = gRacePlayers;
                if ((s32) playerCount > 0) {
                    readyPtr = (u8 *) &gCharacterSelectHudState;
                    readyEnd = playerCount + readyPtr;
                    do {
                        readyPtr++;
                        keepGoing = (u32) readyPtr < (u32) readyEnd;
                        player++;
                        player[-1].menuState = 0;
                        readyPtr[2] = 0;
                    } while (keepGoing);
                }
                setCurrentGameTaskCallback(updateCharacterSelectMenu, 0);
                gCharacterSelectHudState.phase = 3;
                gCharacterSelectHudState.fade = 0;
            }
        } else if (buttons & B_BUTTON) {
            enqueueSoundEffect(0x18, 0x32);
            playerCount = gPlayerCount;
            player = gRacePlayers;
            if ((s32) playerCount > 0) {
                readyPtr = (u8 *) &gCharacterSelectHudState;
                readyEnd = playerCount + readyPtr;
                do {
                    readyPtr++;
                    keepGoing = (u32) readyPtr < (u32) readyEnd;
                    player++;
                    player[-1].menuState = 0;
                    readyPtr[2] = 0;
                } while (keepGoing);
            }
            setCurrentGameTaskCallback(updateCharacterSelectMenu, 0);
            state = &gCharacterSelectHudState;
            state->phase = 3;
            state->fade = 0;
        }
    }
    updateCallbackTasks();
}
#endif

void fadeOutCharacterSelectMenu(void) {
    if (gCurrentGameTask->fade != 0xFF) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) gCurrentGameTask->fade, 0x24, 1);
        if (gCurrentGameTask->fade == 0xFF) {
            gFramebufferSwapHold = 1;
        } else {
            updateCallbackTasks();
        }
    } else {
        if (gPendingFramebufferSwapCount == 2) {
            releaseMenuAssetHandles();
            gFramebufferSwapHold = 0;
            gFramebufferSwapDelay = 0;
            gMenuFlowState = 0;
            resumeGameTask(2);
            removeGameTask(4);
        }
    }
}
