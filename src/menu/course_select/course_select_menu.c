#include "common.h"
#include "game/save_data.h"
#include "assets.h"
#include "game/engine/asset_manager.h"
#include "game/race/camera/race_camera.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/relocatable_heap.h"
#include "game/menu/character_select/character_select_menu.h"
#include "game/menu/course_select/course_select_menu.h"
#include "game/menu/course_select/course_select_ui.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/race/effects/race_start_transition.h"
#include "game/menu/course_select/course_select_shop_ui.h"
#include "game/engine/system_runtime.h"
#include "game/menu/race_setup/race_setup_ui.h"
#include "game/engine/viewport_manager.h"
#include "game/audio/sound_manager.h"
#include "game/race/player/race_player_input.h"

typedef union {
    RacePlayer *selection;
    RaceCamera *object;
} CourseSelectPointer;

u32 gCourseUnlockPrices[COURSE_UNLOCK_PRICE_COUNT] = {
    3000, 2000, 1000, 12000, 10000, 11000, 24000, 20000, 22000, 80000, 100000, 1000,
};

u16 gCourseDetailsPreviewCourseTiles[] = {
    0, 1, 2, 3, 4, 5,
};

u16 gCourseDetailsPreviewExtraTiles[] = {
    6, 7, 8, 9, 10, 11, 12, 13, 0, 0,
};

extern s16 gMenuFadeAlpha;
#ifdef NON_MATCHING
extern u8 D_800EC9C0;
#else
extern s8 D_800EC9C0;
#endif
extern s8 D_8010AE64;
extern u8 D_8010AEA0[];
#ifdef NON_MATCHING
extern volatile char D_8010AEA4;
#else
extern s8 D_8010AEA4;
#endif
extern s8 D_8010AEA8;
extern u8 D_8010AEAC[];
extern s8 D_8010AEB0;
extern u8 D_8010AEF8[];
extern u8 D_8010AEFB[];
extern u8 gCourseSelectExtraCourseIds[];
extern u8 gMultiplayerCourseSelectExtraCourseIds[RACE_PLAYER_COUNT][3];
extern s16 D_8010AED0;
extern u8 gMenuExitSelection;
extern u8 D_8010AECC;
extern u8 gCourseSelectExtraCourseColumnState;
extern u8 gShopMenuDescriptionSeen;
extern u8 gShopMenuShowNewCoursesMessage;
extern s16 gCoursePreviewViewportHeight;
extern RaceCamera D_801121E0[];
extern RaceCamera D_80112340;
extern s8 gCourseSelectSelectedCourseSavedSlot;
extern s32 gMenuFlowState;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern s32 D_8010AEE8[];
extern u8 D_8010AEF7[];
extern u8 D_8010AF06[];
extern u16 D_8010AF44;
extern void releaseMenuAssetHandles(void);
extern s32 enqueueSoundEffect(s32, s32);

void initCourseSelectMenu(void) {
    u32 size;
    s32 i;
    s32 mask;
    s32 showNewCourses;

    requestMusicSequenceBank(3);
    resetRaceCameras();
    resetAllViewports();
    configureViewport(0, 0xE8, 0x78, 0x90, 0xD0, 0xA0, 0xF0, 0.6666666865f);

    for (i = 0; i < 4; i++) {
        D_801121E0[i].update = updateMenuCameraObjectLookAtOriginCallback;
        D_801121E0[i].distance = 0xA40000;
        D_8010AEA0[i] = 0;
        D_8010AEAC[i] = 0;
    }

    gFramebufferSwapDelay.value = 0;
    gCurrentGameTask->fade = 0xFF;
    LOAD_ASSET(_5CBA80, 0x21);
    LOAD_ASSET(_593D10, 0x22);
    LOAD_ASSET(_598A70, 0x23);
    LOAD_ASSET(_59AAA0, 0x24);
    LOAD_ASSET(_5A1ED0, 0x25);
    LOAD_ASSET(_59DFE0, 0x26);
    LOAD_ASSET(_5D4280, 0x27);

    size = ASSET_SIZE(_14B450);
    gAssetHandles[0xC] = allocRelocatableHeapBlock(size);
    DMA_ASSET(_14B450, (void *)getRelocatableHeapBlockBase(gAssetHandles[0xC]), size);
    LOAD_ASSET(_1EF530, 0xD);
    LOAD_ASSET(_1E74E0, 0x1C);
    initCallbackTaskScheduler(0);
    createCallbackTask((CallbackTaskCallback)initMenuIconTilemapSpriteActor, 0, 0x5E);

    gMenuSelectionConfirmTimer = 0;
    D_800EC9C0 = 0;
    gCourseDetailsCloseFromBack = 0;
    gRacePlayers[0].menuState = 0;
    gRacePlayers[0].menuSelection = 0;
    gCharacterSelectHudState.highlightedRosterIndices[0] = 0;
    gActiveMenuTask = 0;
    gCurrentGameTask->timer = 0;
    gMenuExitSelection = 0;
    showNewCourses = 0;
    gShopMenuDescriptionSeen = showNewCourses;
    gShopMenuShowNewCoursesMessage = showNewCourses;
    gCourseDetailsMenuSelection = 0;
    gCourseDetailsPreviewPage = 0;
    D_8010AED0 = 0;
    D_8010AEA8 = 0;
    D_8010AEA4 = 0;
    D_8010AEB0 = 0;
    gMenuChoicePromptState[0] = 0;
    D_8010AECC = 0;
    gMenuInputRepeatTimers[0] = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;

    mask = 1;
    for (i = 0; i < 3; i++) {
        if (gGameSaveDataBuffer[0].extraCourseUnlockFlags & mask) {
            if (!(gGameSaveDataBuffer[0].extraCourseUnlockFlags & (mask << 3))) {
                gShopMenuShowNewCoursesMessage = 1;
                gShopMenuDescriptionSeen = 1;
                gGameSaveDataBuffer[0].extraCourseUnlockFlags |= mask << 3;
            }
        }
        mask <<= 1;
    }

    if (gCourseSelectModeSelection == 2) {
        gCourseSelectModeSelection = 0;
    }

    setCurrentGameTaskCallback(updateCourseSelectModeMenu, 0);
    updateCallbackTasks();

    for (i = 0; i < 4; i++) {
        gCourseSelectStatus.unk0Array[i] = 0;
        gCourseSelectStatus.unk4Array[i] = 0;
        gCourseSelectStatus.unk8Array[i] = 0;
        gCourseSelectStatus.unkCArray[i] = 0;
        gCourseSelectStatus.unk10Array[i] = 0;
        gCourseSelectStatus.unk14[i] = 0;
        gCourseSelectStatus.unk1C[i] = 0;
        gCourseSelectStatus.unk24[i] = 0;
    }
    gCourseSelectStatus.transitionState = 0;
    gCourseSelectStatus.unk28 = 0;
    gCourseSelectStatus.unk2A = 0;
    gCourseSelectStatus.unk2C = 0;
    gCourseSelectStatus.unk2E = 0;
}

// updateCourseSelectModeMenu best match: 99.706%
// (nonmatchings/updateCourseSelectModeMenu-7812531368330432019/base_23.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_menu/updateCourseSelectModeMenu.s")

#ifdef NON_MATCHING
void updateCourseSelectModeMenu(void) {
    s32 pressed;
    s32 input;
    s32 upInput;
    s32 pressedInput;
    s32 timer;
    s32 selection;
    s32 zero;
    s32 oldSelection;
    s32 decrementedSelection;
    s32 incrementedSelection;
    register s32 timerValue;
    register s32 nextTimer;
    s32 timerAlias1;
    s32 timerAlias2;
    s32 timerAlias3;
    register s32 repeatTimerCopy;
    u32 pressedAlias;
    s32 *inputPtr;

    zero = 0;
    inputPtr = gPlayerInputPressed;
    if (gCurrentGameTask->fade != zero) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16)gCurrentGameTask->fade, 0x24, 0);
        if (gCurrentGameTask->fade == 0) {
            createCallbackTask((CallbackTaskCallback)initShopMenuModeChoiceRows, 0, 0x63);
            if (gShopMenuDescriptionSeen == 0) {
                enqueueSoundEffect(0x44, 0x32);
            }
        }
    } else {
        timer = gMenuSelectionConfirmTimer;
        timerValue = (timerAlias3 = (timerAlias2 = (timerAlias1 = timer)));
        if (timer == 0) {
            if ((gRacePlayers[0].menuState == 0) && (gCourseSelectStatus.unk28 == 1)) {
                selection = gCourseSelectModeSelection;
                oldSelection = selection;
                pressed = (timerValue = gPlayerInputHeld[0]);
                upInput = pressed & (STICK_UP | U_JPAD);
                if ((upInput == 0) && !(pressed & (STICK_DOWN | D_JPAD))) {
                    gMenuInputRepeatTimers[0] = 0;
                }
                pressedInput = (pressedAlias = gPlayerInputPressed[0]);
                if ((pressedInput & (STICK_UP | U_JPAD)) ||
                    ((upInput != 0) && ((s32)(u16)gMenuInputRepeatTimers[0] >= 0xB) &&
                     (((s32)(u16)gMenuInputRepeatTimers[0] % 3) == 0))) {
                    timer = (u16)gMenuInputRepeatTimers[0];
                    timer = (u16)gMenuInputRepeatTimers[0];
                    nextTimer = timer + (1 & 0xFFFFFFFFFFFFFFFFu);
                    decrementedSelection = selection - 1;
                    if (timer == 0) {
                        gMenuInputRepeatTimers[0] = nextTimer;
                        timer = (u16)nextTimer;
                    }
                    if (selection > 0) {
                        gCourseSelectModeSelection = decrementedSelection;
                        selection = (u8)decrementedSelection;
                    }
                } else {
                    timer = (u16)gMenuInputRepeatTimers[0];
                    if ((pressedInput & (STICK_DOWN | D_JPAD)) ||
                        ((pressed & (STICK_DOWN | D_JPAD)) && ((repeatTimerCopy = timer ^ 0) >= 0xB) &&
                         ((repeatTimerCopy % 3) == 0))) {
                        if (timer == 0) {
                            nextTimer = timer + 1;
                            gMenuInputRepeatTimers[0] = nextTimer;
                            do {
                            } while (0);
                            timer = (u16)nextTimer;
                        }
                        if (selection < 2) {
                            incrementedSelection = selection + 1;
                            gCourseSelectModeSelection = incrementedSelection;
                            selection = ((((((u8)incrementedSelection) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu;
                        }
                    }
                }
                repeatTimerCopy = selection ^ 0;
                if (timer != 0) {
                    if (1) {}
                    nextTimer = timer + ((((((1 & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) &
                                           0xFFFFFFFFFFFFFFFFu) &
                                          0xFFFFFFFFFFFFFFFFu) &
                                         0xFFFFFFFFFFFFFFFFu);
                    timer = 0;
                    gMenuInputRepeatTimers[timer] = nextTimer;
                    if ((((u16)nextTimer) ^ 0) == 0xFFFF) {
                        selection = 0;
                        gMenuInputRepeatTimers[selection] = 0xC;
                    }
                }
                if (repeatTimerCopy != oldSelection) {
                    enqueueSoundEffect(0x19, 0x32);
                    if (gShopMenuDescriptionSeen == 0) {
                        gShopMenuDescriptionSeen = 1;
                    }
                    if (gShopMenuShowNewCoursesMessage == 1) {
                        gShopMenuShowNewCoursesMessage = 0;
                    }
                }
                input = *inputPtr;
                if (((input & START_BUTTON) || ((*inputPtr) & A_BUTTON)) && (gMenuFlowState == 2)) {
                    gMenuSelectionConfirmTimer = 1;
                    gCourseSelectStatus.unk28 = 2;
                    gCourseSelectStatus.unk2A = 0x100;
                    if ((s32)gCourseSelectModeSelection < 2) {
                        gMenuExitSelection = 0;
                        if (1) {}
                        if (1) {}
                        if (1) {}
                        if (1) {}
                        if (1) {}
                        if (1) {
                            if (1) {}
                        }
                        enqueueSoundEffect(0x18, 0x32);
                    } else {
                        gMenuExitSelection = 1;
                        enqueueSoundEffect(0x46, 0x32);
                    }
                    timer = gMenuSelectionConfirmTimer;
                    timerValue = timer;
                } else {
                    timer = gMenuSelectionConfirmTimer;
                    timerValue = ((u32)timer) ^ (((*inputPtr) & A_BUTTON) * 0);
                    if (((*inputPtr) & B_BUTTON) && (gMenuFlowState == 2) && (timerValue == 0)) {
                        gMenuSelectionConfirmTimer = 1;
                        gCourseSelectStatus.unk28 = 2;
                        gCourseSelectStatus.unk2A = 0x100;
                        gMenuExitSelection = 1;
                        enqueueSoundEffect(0x46, 0x32);
                        timer = gMenuSelectionConfirmTimer;
                        timerValue = timer;
                    }
                }
            }
        } else {
            timer = (timerValue = (u8)(timer + 1));
            gMenuSelectionConfirmTimer = timer;
        }
        if (timerValue == 8) {
            gRacePlayers[0].menuState = 1;
            gMenuSelectionConfirmTimer = timer + 1;
        }
        if (gRacePlayers[0].menuState == 2) {
            gMenuSelectionConfirmTimer = 0;
            if (gMenuExitSelection == 0) {
                setCurrentGameTaskCallback(initCourseSelectCourseList, 0);
            } else {
                setCurrentGameTaskCallback(exitCourseSelectMenu, gCourseSelectStatus.unk28 * 0);
                requestMusicSequenceStop(8);
                gMenuExitSelection = 0;
                gCourseSelectExtraCourseColumnState = 0;
            }
        }
    }
    gMenuFlowState = gMenuSelectionConfirmTimer * 0;
    updateCallbackTasks();
}
#endif

void initCourseSelectCourseList(void) {
    s32 i;
    s32 listMask;
    s32 column;
    s32 selected;
    s32 one;

    gRacePlayers[0].menuState = 0;
    D_8010AEA0[0] = 0;
    gMenuSelectionConfirmTimer = 0;
    gMenuInputRepeatTimers[0] = 0;
    createCallbackTask((CallbackTaskCallback)initCourseSelectCourseIconList, 0, 0x63);
    D_8010ADE8 = createCallbackTask((CallbackTaskCallback)initCourseSelectExtraCourseIconList, 0, 0x61);

    if (gGameSaveDataBuffer[0].extraCourseUnlockFlags & 7) {
        D_8010AEA0[0] = 1;
    }

    for (i = 0; i < 3; i++) {
        D_8010AEF8[i] = i;
    }

    one = 1;
    if (D_8010AEA0[0] == one) {
        column = one;
        for (i = 9; i < 0xC; i++) {
            if (gGameSaveDataBuffer[0].extraCourseUnlockFlags & column) {
                D_8010AEFB[0] = i;
                break;
            }
            column <<= 1;
        }

        listMask = one;
        for (i = 9, column = 0; i < 0xC; i++) {
            gMultiplayerCourseSelectExtraCourseIds[0][column] = 0;
            if (gGameSaveDataBuffer[0].extraCourseUnlockFlags & listMask) {
                gMultiplayerCourseSelectExtraCourseIds[0][column] = i;
                column++;
            }
            listMask <<= 1;
        }
    } else {
        D_8010AEFB[0] = 0;
    }

    if ((gRacePlayers[0].menuSelection >= 9) && (gRacePlayers[0].menuSelection < 0xC)) {
        selected = gCourseSelectExtraCourseColumnState;
        column = 3;
    } else {
        selected = gCourseSelectStatus.unk2E;
        column = 0;
        if (one == selected) {
            selected = (gCourseSelectStatus.unk2E = 0);
        } else {
            column = gRacePlayers[0].menuSelection % 3;
        }
    }

    gCharacterSelectHudState.highlightedRosterIndices[0] = column;
    if (one == selected) {
        column--;
    }

    gRacePlayers[0].menuSelection = D_8010AEF8[column];
    setCurrentGameTaskCallback(updateCourseSelectCourseList, 0);
    updateCallbackTasks();
}

// updateCourseSelectCourseList best match: 81.336%
// (nonmatchings/updateCourseSelectCourseList-6078661025080551018/base_96.c)
// Campaign partial: 706 positional words, 147 alignment gaps, 874/885 instructions, frame 0x48.
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_menu/updateCourseSelectCourseList.s")

#if 0 /* Superseded without consulting the previous attempts. */
#if 0
void updateCourseSelectCourseList(void) {
    u8 sp32;
    s8 sp31;
    volatile CourseSelectPointer sp18;
    RacePlayer *var_v0_5;
    s16 temp_t6;
    s16 temp_t7;
    s16 temp_v0;
    s32 *var_v1;
    s32 temp_t4;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a3;
    s32 new_var;
    s8 temp_a0_2;
    u16 var_v1_2;
    s32 var_a1;
    u8 var_t1;
    s32 var_t8;
    s32 var_v0_3;
    u8 var_v0_4;
    u8 var_v0_6;
    RacePlayer *var_v0;
    RacePlayer *var_v0_2;

    var_a0 = 0;
    if (gRacePlayers[0].menuState == 9) {
        var_a3 = 0;
        if ((s32) gPlayerCount > 0) {
            var_v1 = D_8010AEE8;
            do {
                if ((*var_v1 != 0) || (temp_v0 = gMenuChoicePromptState[var_a3], (temp_v0 == 1)) || (temp_v0 >= 5)) {
                    var_a0 += 1;
                }
                var_a3 += 1;
                var_v1 += 1;
            } while (var_a3 < (s32) gPlayerCount);
        }
        if (var_a0 == 0) {
            var_a3 = 0;
            if ((s32) gPlayerCount < 3) {
                var_a1 = gPlayerCount;
            } else {
                var_a1 = 4;
            }
            for (var_a3 = 0; var_a3 < (s32) var_a1; var_a3++) {
                gRacePlayers[var_a3].menuState = 3;
            }
            D_800EC9C0 = 0xF;
            gMenuFlowState = 1;
        }
        var_v0_3 = (u8) D_800EC9C0;
    } else if ((u8) D_800EC9C0 == 0) {

        var_a3 = 0;
        if (gMenuChoicePromptState[0] == 0) {
            if ((gCourseSelectStatus.unk0[0] == 1) && (gRacePlayers[0].menuState == 0) && !(D_8010AECC & 1)) {
                if ((gMenuSelectionVariant == 5) || (*D_8010AEA0 == 0) || (var_t1 = 4, (gCourseSelectModeSelection == 1))) {
                    var_t1 = 3;
                }
                sp32 = var_t1;
                if ((s32) gPlayerCount >= 2) {
                    var_t1 = (var_t1 - 1) & 0xFF;
                }
                temp_v0_2 = gPlayerInputHeld[0] & 0x10800;
                sp31 = gCharacterSelectHudState.highlightedRosterIndices[0];
                if ((temp_v0_2 == 0) && !(gPlayerInputHeld[0] & 0x20400)) {
                    gMenuInputRepeatTimers[0] = 0;
                }
                if ((gPlayerInputPressed[0] & 0x10800) || ((temp_v0_2 != 0) && ((s32) (u16) gMenuInputRepeatTimers[0] >= 9))) {
                    if ((u16) gMenuInputRepeatTimers[0] == 0) {
                        gMenuInputRepeatTimers[0] = (u16) gMenuInputRepeatTimers[0] + 1;
                    }
                    if (gCharacterSelectHudState.highlightedRosterIndices[0] > 0) {
                        gCharacterSelectHudState.highlightedRosterIndices[0] -= 1;
                        enqueueSoundEffect(0x19, 0x32);
                        var_a3 = 0;
                    }
                } else if ((gPlayerInputPressed[0] & 0x20400) || ((gPlayerInputHeld[0] & 0x20400) && ((s32) (u16) gMenuInputRepeatTimers[0] >= 9))) {
                    if ((u16) gMenuInputRepeatTimers[0] == 0) {
                        gMenuInputRepeatTimers[0] = (u16) gMenuInputRepeatTimers[0] + 1;
                    }
                    if (gCharacterSelectHudState.highlightedRosterIndices[0] < (s32) var_t1) {
                        gCharacterSelectHudState.highlightedRosterIndices[0] += 1;
                        enqueueSoundEffect(0x19, 0x32);
                        var_a3 = 0;
                    }
                }
                if ((u16) gMenuInputRepeatTimers[0] != 0) {
                    var_v1_2 = (u16) gMenuInputRepeatTimers[0] + 1;
                    gMenuInputRepeatTimers[0] = var_v1_2;
                    if (var_v1_2 == 0xFFFF) {
                        gMenuInputRepeatTimers[0] = 0xA;
                    }
                }
                if ((sp32 != gCharacterSelectHudState.highlightedRosterIndices[0]) && (sp32 != (u8) sp31) &&
                    ((u8) sp31 != gCharacterSelectHudState.highlightedRosterIndices[0])) {
                    D_8010AECC += 1;
                    if ((s32) (u8) sp31 < gCharacterSelectHudState.highlightedRosterIndices[0]) {
                        D_8010AEE8[0] = 0xFF800000;
                    } else {
                        D_8010AEE8[0] = 0x800000;
                    }
                }
                if (sp32 == gCharacterSelectHudState.highlightedRosterIndices[0]) {
                    var_v0_4 = *(&D_8010AEF7 + sp32);
                    gCourseSelectExtraCourseColumnState = 2;
                } else {
                    var_v0_4 = D_8010AEF8[gCharacterSelectHudState.highlightedRosterIndices[0]];
                    gCourseSelectExtraCourseColumnState = 0;
                }
                gRacePlayers[0].menuSelection = var_v0_4;
                if ((s32) gRacePlayers[0].menuSelection >= 9) {
                    gRacePlayers[0].selectionUnlockState = 0;
                } else {
                    temp_a0_2 = gGameSaveDataBuffer[0].courseUnlockStates[gRacePlayers[0].menuSelection];
                    if (temp_a0_2 == -1) {
                        gRacePlayers[0].selectionUnlockState = (u8) ((s32) gRacePlayers[0].menuSelection % 3);
                    } else {
                        gRacePlayers[0].selectionUnlockState = (u8) temp_a0_2;
                    }
                }
                if ((D_8010AEE8[0] == 0) && ((gPlayerInputPressed[0] & 0x1000) || (gPlayerInputPressed[0] & 0x8000))) {
                    enqueueSoundEffect(1, 0x32);
                    var_a3 = 0;
                    if ((gPlayerCount == 1) && (var_t1 == gCharacterSelectHudState.highlightedRosterIndices[0])) {
                        gRacePlayers[0].menuState = 9;
                        gMenuFlowState = 1;
                    } else {
                        D_8010AEA4 = 1;
                        gMenuChoicePromptState[0] = 9;
                        gMenuInputRepeatTimers[0] = 0;
                        gRacePlayers[0].menuState = 1;
                        if (gCourseSelectModeSelection == 0) {
                            createCallbackTask((CallbackTaskCallback)initCourseUnlockPricePanel, 0, 0x62);
                            var_a3 = 0;
                        }
                    }
                } else if (gPlayerInputPressed[0] & 0x4000) {
                    enqueueSoundEffect(1, 0x32);
                    gMenuInputRepeatTimers[0] = 0;
                    var_a3 = 0;
                    var_a1 = gPlayerCount;
                    if ((s32) var_a1 > 0) {
                        var_v0_5 = gRacePlayers;
                        do {
                            var_v0_5->menuState = 9;
                            var_v0_5 += 1;
                        } while ((u32) var_v0_5 < (u32) (gRacePlayers + var_a1));
                    }
                }
            }
            if ((gRacePlayers[0].menuState == 1) && (gCourseSelectModeSelection == 1)) {
                setCurrentGameTaskCallback(updateCourseSelectUnlockCourseList, 0);
                gCurrentGameTask->screenState = 0;
            }
        } else {
            new_var = 2;
            if (gMenuChoicePromptState[0] == 9) {
                D_8010AEA4 = (u8) D_8010AEA4 + 1;
            } else {
                D_8010AEA4 = 0;
            }
            if ((gMenuChoicePromptState[0] >= new_var) && (gMenuChoicePromptState[0] < 5)) {
                if (!(gPlayerInputHeld[0] & 0x10800) && !(gPlayerInputHeld[0] & 0x20400)) {
                    gMenuInputRepeatTimers[0] = 0;
                }
                temp_t4 = gPlayerInputPressed[0];
                if ((temp_t4 & 0x10800) || ((gPlayerInputHeld[0] & 0x10800) && ((s32) (u16) gMenuInputRepeatTimers[0] >= 9) && ((u16) gMenuInputRepeatTimers[0] & 1))) {
                    var_v1_2 = (u16) gMenuInputRepeatTimers[0];
                    if (var_v1_2 == 0) {
                        var_v1_2 = (var_v1_2 + 1) & 0xFFFF;
                    }
                    gMenuInputRepeatTimers[0] = (s16) var_v1_2;
                    if (gMenuChoicePromptState[0] >= 3) {
                        gMenuChoicePromptState[0] -= 1;
                        enqueueSoundEffect(0x19, 0x32);
                        var_a3 = 0;
                        D_8010AF44 = 0;
                    }
                } else {
                    var_v1_2 = (u16) gMenuInputRepeatTimers[0];
                    if ((temp_t4 & 0x20400) || ((gPlayerInputHeld[0] & 0x20400) && ((s32) var_v1_2 >= 9) && (var_v1_2 & 1))) {
                        if (var_v1_2 == 0) {
                            var_v1_2 = (var_v1_2 + 1) & 0xFFFF;
                        }
                        gMenuInputRepeatTimers[0] = (s16) var_v1_2;
                        if (gMenuChoicePromptState[0] < (gCourseSelectStatus.unk24[0] + 1)) {
                            gMenuChoicePromptState[0] += 1;
                            enqueueSoundEffect(0x19, 0x32);
                            var_a3 = 0;
                            D_8010AF44 = 0;
                        }
                    }
                }
                if ((u16) gMenuInputRepeatTimers[0] != 0) {
                    var_v1_2 = (u16) gMenuInputRepeatTimers[0] + 1;
                    gMenuInputRepeatTimers[0] = var_v1_2;
                    if (var_v1_2 == 0xFFFF) {
                        gMenuInputRepeatTimers[0] = 0xA;
                    }
                }
                if ((*D_8010AEA0 == 1) && (gCharacterSelectHudState.highlightedRosterIndices[0] == 3)) {
                    var_t8 = *(&D_8010AF06 + gMenuChoicePromptState[0]);
                } else {
                    var_t8 = ((gMenuChoicePromptState[0] * 3) + ((s32) gRacePlayers[0].menuSelection % 3)) - 6;
                }
                gRacePlayers[0].menuSelection = var_t8;
                temp_t4 = gPlayerInputPressed[0];
                if (temp_t4 & 0x4000) {
                    enqueueSoundEffect(0x18, 0x32);
                    D_8010AF44 = 0;
                    gRacePlayers[0].menuSelection = D_8010AEF8[gCharacterSelectHudState.highlightedRosterIndices[0]];
                    gMenuInputRepeatTimers[0] = 0;
                    var_a3 = 0;
                    gMenuChoicePromptState[0] += 3;
                } else if ((temp_t4 & 0x8000) || (temp_t4 & 0x1000)) {
                    if (gGameSaveDataBuffer[0].courseUnlockStates[gRacePlayers[0].menuSelection] == -1) {
                        if ((u32) gRacePlayers[0].money >= (u32) gCourseUnlockPrices[gRacePlayers[0].menuSelection]) {
                            enqueueSoundEffect(0x49, 0x32);
                            var_a3 = 0;
                            if ((*D_8010AEA0 == 1) && (gCharacterSelectHudState.highlightedRosterIndices[0] == 3)) {
                                var_v0_6 = *(&D_8010AF06 + gMenuChoicePromptState[0]);
                            } else {
                                var_v0_6 = (((gMenuChoicePromptState[0] * 3) + ((s32) gRacePlayers[0].menuSelection % 3)) - 6) & 0xFF;
                            }
                            gCourseSelectSelectedCourseSavedSlot =
                                gGameSaveDataBuffer[0].courseUnlockStates[var_v0_6];
                            gRacePlayers[0].menuSelection = var_v0_6;
                            if (D_8010AECC == 0) {
                                gCourseSelectStatus.unk14[0] = 0;
                                gCourseSelectStatus.unk4Array[0] = 7;
                            } else {
                                gCourseSelectStatus.unk1C[0] = 0;
                                gCourseSelectStatus.unk8Array[0] = 7;
                            }
                        } else if (D_8010AF44 == 0) {
                            enqueueSoundEffect(0x47, 0x32);
                            var_a3 = 0;
                            D_8010AF44 = 1;
                        }
                    } else if (D_8010AF44 == 0) {
                        D_8010AF44 = new_var;
                    }
                }
            }
        }
        if ((gCourseSelectStatus.unk4Array[0] == 7) || (gCourseSelectStatus.unk8Array[0] == 7)) {
            gCurrentGameTask->timer = 0;
            setCurrentGameTaskCallback(updateCourseSelectPurchasePrompt, 0);
            createCallbackTask((CallbackTaskCallback)initCourseUnlockPurchasePrompt, 0, 0x64);
        }
        var_v0_3 = (u8) D_800EC9C0;
    } else {
        D_800EC9C0 = (u8) D_800EC9C0 + 1;
        var_v0_3 = (u8) D_800EC9C0;
        if ((s32) var_v0_3 >= 0x22) {
            D_800EC9C0 = 0;
            var_v0_3 = 0;
        }
    }
    if (var_v0_3 == 0x1B) {
        sp18.selection = gRacePlayers + var_a3;
        gCurrentGameTask->fade = 1;
        gRacePlayers[0].menuState = 0;
        setCurrentGameTaskCallback(updateCourseSelectModeMenu, 0);
        sp18.selection->menuSelection = D_8010AEF8[gCharacterSelectHudState.highlightedRosterIndices[0]];
        if (gMenuFlowState == 1) {
            gRacePlayers[0].menuSelection = 0;
        }
        gMenuFlowState = 0;
        D_800EC9C0 = 0;
        gMenuChoicePromptState[0] = 0;
        D_8010AECC = 0;
        gCourseSelectStatus.unk0[0] = 0;
        gCourseSelectStatus.unk4Array[0] = 0;
        gCourseSelectStatus.unk8Array[0] = 0;
        gCourseSelectStatus.unkCArray[0] = 0;
        gCourseSelectStatus.unk10Array[0] = 0;
        gCourseSelectStatus.unk14[0] = 0;
        gCourseSelectStatus.unk1C[0] = 0;
        gCourseSelectStatus.unk24[0] = 0;
        gCourseSelectStatus.unk0Array[1] = 0;
        gCourseSelectStatus.unk4Array[1] = 0;
        gCourseSelectStatus.unk8Array[1] = 0;
        gCourseSelectStatus.unkCArray[1] = 0;
        gCourseSelectStatus.unk10Array[1] = 0;
        gCourseSelectStatus.unk14[1] = 0;
        gCourseSelectStatus.unk1C[1] = 0;
        gCourseSelectStatus.unk24[1] = 0;
        gCourseSelectStatus.unk0Array[2] = 0;
        gCourseSelectStatus.unk4Array[2] = 0;
        gCourseSelectStatus.unk8Array[2] = 0;
        gCourseSelectStatus.unkCArray[2] = 0;
        gCourseSelectStatus.unk10Array[2] = 0;
        gCourseSelectStatus.unk14[2] = 0;
        gCourseSelectStatus.unk1C[2] = 0;
        gCourseSelectStatus.unk24[2] = 0;
        gCourseSelectStatus.unk0Array[3] = 0;
        gCourseSelectStatus.unk4Array[3] = 0;
        gCourseSelectStatus.unk8Array[3] = 0;
        gCourseSelectStatus.unkCArray[3] = 0;
        gCourseSelectStatus.unk10Array[3] = 0;
        gCourseSelectStatus.unk14[3] = 0;
        gCourseSelectStatus.unk1C[3] = 0;
        gCourseSelectStatus.unk24[3] = 0;
        gCourseSelectStatus.transitionState = 0;
        gCourseSelectStatus.unk28 = 0;
        gCourseSelectStatus.unk2A = 0;
        gCourseSelectStatus.unk2C = 0;
        gCourseSelectStatus.unk2E = 0;
    }
    var_a3 = 0;
    if ((s32) gPlayerCount > 0) {
        sp18.object = D_801121E0;
        do {
            gCurrentMenuCameraObject = sp18.object;
            sp18.object->update();
            var_a3 += 1;
            sp18.object += 1;
        } while (var_a3 < (s32) gPlayerCount);
    }
    updateCallbackTasks();
}

#else
void updateCourseSelectCourseList(void) {
    CourseSelectPointer pointer;
    s32 count;
    s32 i;
    s32 row;
    s32 held;
    s32 pressed;
    s32 heldUp;
    s32 maxVisible;
    u8 maxColumn;
    u8 maxColumnOriginal;
    u8 originalColumn;
    s8 currentColumn;
    u16 repeat;
    u8 courseId;
    s32 *momentum;
    u32 playerEnd;

    count = 0;
    if ((u8)D_800EC9C0 == 9) {
        i = 0;
        if ((s32)gPlayerCount > 0) {
            momentum = D_8010AEE8;
            do {
                if ((*momentum != 0) ||
                    ((row = gMenuChoicePromptState[i]) == 1) ||
                    (row >= 5)) {
                    count++;
                }
                i++;
                momentum++;
            } while (i < (s32)gPlayerCount);
        }

        if (count == 0) {
            i = 0;
            maxVisible = (gPlayerCount < 3) ? gPlayerCount : 4;
            for (i = 0; i < maxVisible; i++) {
                gRacePlayers[i].menuState = 3;
            }
            D_800EC9C0 = 0xF;
            gMenuFlowState = 1;
        }
    } else if ((u8)D_800EC9C0 == 0) {
        row = gMenuChoicePromptState[0];
        i = 0;
        if (row == 0) {
            if ((gCourseSelectStatus.unk0Array[0] == 1) &&
                (gRacePlayers[0].menuState == 0) &&
                !(D_8010AECC & 1)) {
                if ((gRacePlayers[0].selectedCharacterId == 5) ||
                    (D_8010AEA0[0] == 0) ||
                    (gCourseSelectModeSelection == 1)) {
                    maxColumnOriginal = 3;
                } else {
                    maxColumnOriginal = 4;
                }
                maxColumn = maxColumnOriginal;
                if ((gPlayerCount + 1) > 2) {
                    maxColumn--;
                }

                held = gPlayerInputHeld[0];
                originalColumn = gCharacterSelectHudState.highlightedRosterIndices[0];
                heldUp = held & (STICK_UP | U_JPAD);
                if ((heldUp == 0) && !(held & (STICK_DOWN | D_JPAD))) {
                    gMenuInputRepeatTimers[0] = 0;
                }

                pressed = gPlayerInputPressed[0];
                if ((pressed & (STICK_UP | U_JPAD)) ||
                    ((heldUp != 0) && ((u16)gMenuInputRepeatTimers[0] >= 9))) {
                    if ((u16)gMenuInputRepeatTimers[0] == 0) {
                        gMenuInputRepeatTimers[0] = (u16)gMenuInputRepeatTimers[0] + 1;
                    }
                    if (gCharacterSelectHudState.highlightedRosterIndices[0] > 0) {
                        gCharacterSelectHudState.highlightedRosterIndices[0]--;
                        enqueueSoundEffect(0x19, 0x32);
                    }
                } else if ((pressed & (STICK_DOWN | D_JPAD)) ||
                           ((held & (STICK_DOWN | D_JPAD)) &&
                            ((u16)gMenuInputRepeatTimers[0] >= 9))) {
                    if ((u16)gMenuInputRepeatTimers[0] == 0) {
                        gMenuInputRepeatTimers[0] = (u16)gMenuInputRepeatTimers[0] + 1;
                    }
                    if (gCharacterSelectHudState.highlightedRosterIndices[0] < maxColumn) {
                        gCharacterSelectHudState.highlightedRosterIndices[0]++;
                        enqueueSoundEffect(0x19, 0x32);
                    }
                }

                repeat = gMenuInputRepeatTimers[0];
                if (repeat != 0) {
                    repeat++;
                    gMenuInputRepeatTimers[0] = repeat;
                    if (repeat == 0xFFFF) {
                        gMenuInputRepeatTimers[0] = 0xA;
                    }
                }

                currentColumn = gCharacterSelectHudState.highlightedRosterIndices[0];
                if ((maxColumnOriginal != currentColumn) &&
                    (maxColumnOriginal != originalColumn) &&
                    (originalColumn != currentColumn)) {
                    D_8010AECC++;
                    if (originalColumn < currentColumn) {
                        D_8010AEE8[0] = -0x800000;
                    } else {
                        D_8010AEE8[0] = 0x800000;
                    }
                }

                if (maxColumnOriginal == currentColumn) {
                    gRacePlayers[0].menuSelection = D_8010AEF7[currentColumn];
                    gCourseSelectExtraCourseColumnState = 2;
                } else {
                    gRacePlayers[0].menuSelection = D_8010AEF8[currentColumn];
                    gCourseSelectExtraCourseColumnState = 0;
                }

                courseId = gRacePlayers[0].menuSelection;
                if (courseId >= 9) {
                    gCourseSelectSelectedCourseSavedSlot = 0;
                } else {
                    gCourseSelectSelectedCourseSavedSlot = gGameSaveDataBuffer[0].courseUnlockStates[courseId];
                    if (gCourseSelectSelectedCourseSavedSlot == -1) {
                        gCourseSelectSelectedCourseSavedSlot = courseId % 3;
                    }
                }

                if ((D_8010AEE8[0] == 0) &&
                    ((pressed & START_BUTTON) || (pressed & A_BUTTON))) {
                    enqueueSoundEffect(1, 0x32);
                    if ((gPlayerCount == 1) && (maxColumn == currentColumn)) {
                        gRacePlayers[0].menuState = 9;
                        gMenuFlowState = 1;
                    } else {
                        D_8010AEA4 = 1;
                        gMenuChoicePromptState[0] = 9;
                        gMenuInputRepeatTimers[0] = 0;
                        gRacePlayers[0].menuState = 1;
                        if (gCourseSelectModeSelection == 0) {
                            createCallbackTask(
                                (CallbackTaskCallback)initCourseUnlockPricePanel, 0, 0x62);
                        }
                    }
                } else if (pressed & B_BUTTON) {
                    enqueueSoundEffect(1, 0x32);
                    gMenuInputRepeatTimers[0] = 0;
                    pointer.selection = gRacePlayers;
                    if ((s32)gPlayerCount > 0) {
                        playerEnd = (u32)(gRacePlayers + gPlayerCount);
                        do {
                            pointer.selection->menuState = 9;
                            pointer.selection++;
                        } while ((u32)pointer.selection < playerEnd);
                    }
                }
            }

            if ((gRacePlayers[0].menuState == 1) &&
                (gCourseSelectModeSelection == 1)) {
                setCurrentGameTaskCallback(updateCourseSelectUnlockCourseList, 0);
                gCurrentGameTask->screenState = 0;
            }
        } else {
            s16 promptRow;

            if (row == 9) {
                D_8010AEA4++;
            } else {
                D_8010AEA4 = 0;
            }

            promptRow = *(volatile s16 *)&gMenuChoicePromptState[0];
            if ((promptRow >= 2) && (promptRow < 5)) {
                held = gPlayerInputHeld[0];
                heldUp = held & (STICK_UP | U_JPAD);
                if ((heldUp == 0) && !(held & (STICK_DOWN | D_JPAD))) {
                    gMenuInputRepeatTimers[0] = 0;
                }

                pressed = gPlayerInputPressed[0];
                repeat = gMenuInputRepeatTimers[0];
                if ((pressed & (STICK_UP | U_JPAD)) ||
                    ((heldUp != 0) && (repeat >= 9) && (repeat & 1))) {
                    if (repeat == 0) {
                        repeat++;
                    }
                    gMenuInputRepeatTimers[0] = repeat;
                    if (gMenuChoicePromptState[0] >= 3) {
                        gMenuChoicePromptState[0]--;
                        enqueueSoundEffect(0x19, 0x32);
                        pressed = gPlayerInputPressed[0];
                        D_8010AF44 = 0;
                    }
                } else if ((pressed & (STICK_DOWN | D_JPAD)) ||
                           ((held & (STICK_DOWN | D_JPAD)) &&
                            (repeat >= 9) && (repeat & 1))) {
                    if (repeat == 0) {
                        repeat++;
                    }
                    gMenuInputRepeatTimers[0] = repeat;
                    if (gMenuChoicePromptState[0] < gCourseSelectStatus.unk24[0] + 1) {
                        gMenuChoicePromptState[0]++;
                        enqueueSoundEffect(0x19, 0x32);
                        pressed = gPlayerInputPressed[0];
                        D_8010AF44 = 0;
                    }
                }

                repeat = gMenuInputRepeatTimers[0];
                if (repeat != 0) {
                    repeat++;
                    gMenuInputRepeatTimers[0] = repeat;
                    if (repeat == 0xFFFF) {
                        gMenuInputRepeatTimers[0] = 0xA;
                    }
                }
                held = 2;

                row = gMenuChoicePromptState[0];
                if ((D_8010AEA0[0] == 1) &&
                    (gCharacterSelectHudState.highlightedRosterIndices[0] == 3)) {
                    gRacePlayers[0].menuSelection = D_8010AF06[row];
                } else {
                    gRacePlayers[0].menuSelection =
                        (row * 3) + (gRacePlayers[0].menuSelection % 3) - 6;
                }

                if (pressed & B_BUTTON) {
                    enqueueSoundEffect(0x18, 0x32);
                    D_8010AF44 = 0;
                    gRacePlayers[0].menuSelection =
                        D_8010AEF8[gCharacterSelectHudState.highlightedRosterIndices[0]];
                    gMenuInputRepeatTimers[0] = 0;
                    gMenuChoicePromptState[0] += 3;
                } else if ((pressed & A_BUTTON) || (pressed & START_BUTTON)) {
                    if (gGameSaveDataBuffer[0].courseUnlockStates[gRacePlayers[0].menuSelection] == -1) {
                        if ((u32)gRacePlayers[0].money >=
                            (u32)gCourseUnlockPrices[gRacePlayers[0].menuSelection]) {
                            enqueueSoundEffect(0x49, 0x32);
                            row = gMenuChoicePromptState[0];
                            if ((D_8010AEA0[0] == 1) &&
                                (gCharacterSelectHudState.highlightedRosterIndices[0] == 3)) {
                                courseId = D_8010AF06[row];
                            } else {
                                courseId =
                                    (row * 3) + (gRacePlayers[0].menuSelection % 3) - 6;
                            }
                            gCourseSelectSelectedCourseSavedSlot =
                                gGameSaveDataBuffer[0].courseUnlockStates[courseId];
                            gRacePlayers[0].menuSelection = courseId;
                            if (D_8010AECC == 0) {
                                gCourseSelectStatus.unk14[0] = 0;
                                gCourseSelectStatus.unk4Array[0] = 7;
                            } else {
                                gCourseSelectStatus.unk1C[0] = 0;
                                gCourseSelectStatus.unk8Array[0] = 7;
                            }
                        } else if (D_8010AF44 == 0) {
                            enqueueSoundEffect(0x47, 0x32);
                            D_8010AF44 = 1;
                        }
                    } else if (D_8010AF44 == 0) {
                        D_8010AF44 = held;
                    }
                }
            }
        }

        if ((gCourseSelectStatus.unk4Array[0] == 7) ||
            (gCourseSelectStatus.unk8Array[0] == 7)) {
            gCurrentGameTask->timer = 0;
            setCurrentGameTaskCallback(updateCourseSelectPurchasePrompt, 0);
            createCallbackTask(
                (CallbackTaskCallback)initCourseUnlockPurchasePrompt, 0, 0x64);
        }
    } else {
        D_800EC9C0++;
        if ((u8)D_800EC9C0 >= 0x22) {
            D_800EC9C0 = 0;
        }
    }

    if ((u8)D_800EC9C0 == 0x1B) {
        pointer.selection = gRacePlayers + i;
        gCurrentGameTask->fade = 1;
        gRacePlayers[0].menuState = 0;
        setCurrentGameTaskCallback(updateCourseSelectModeMenu, 0);
        pointer.selection->menuSelection =
            D_8010AEF8[gCharacterSelectHudState.highlightedRosterIndices[0]];
        if (gMenuFlowState == 1) {
            gRacePlayers[0].menuSelection = 0;
        }
        gMenuFlowState = 0;
        D_800EC9C0 = 0;
        gMenuChoicePromptState[0] = 0;
        D_8010AECC = 0;

        for (i = 0; i < 4; i++) {
            gCourseSelectStatus.unk0Array[i] = 0;
            gCourseSelectStatus.unk4Array[i] = 0;
            gCourseSelectStatus.unk8Array[i] = 0;
            gCourseSelectStatus.unkCArray[i] = 0;
            gCourseSelectStatus.unk10Array[i] = 0;
            gCourseSelectStatus.unk14[i] = 0;
            gCourseSelectStatus.unk1C[i] = 0;
            gCourseSelectStatus.unk24[i] = 0;
        }
        gCourseSelectStatus.transitionState = 0;
        gCourseSelectStatus.unk28 = 0;
        gCourseSelectStatus.unk2A = 0;
        gCourseSelectStatus.unk2C = 0;
        gCourseSelectStatus.unk2E = 0;
    }

    i = 0;
    if ((s32)gPlayerCount > 0) {
        pointer.object = D_801121E0;
        do {
            gCurrentMenuCameraObject = pointer.object;
            pointer.object->update();
            i++;
            pointer.object++;
        } while (i < (s32)gPlayerCount);
    }
    updateCallbackTasks();
}
#endif
#endif

#if 0
void updateCourseSelectCourseList(void) {
    RacePlayer *var_v0_5;
    s16 temp_t6;
    s16 temp_t7;
    s16 temp_v0;
    s32 *var_v1;
    s32 temp_t4;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a3;
    s32 new_var;
    s8 temp_a0_2;
    u16 var_v1_2;
    s32 var_a1;
    u8 var_t1;
    s32 var_t8;
    s32 var_v0_3;
    u8 var_v0_4;
    u8 var_v0_6;
    RacePlayer *var_v0;
    RacePlayer *var_v0_2;
    u8 sp32;
    s8 sp31;
    volatile CourseSelectPointer sp18;

    var_a0 = 0;
    if (*(volatile u8 *)&gRacePlayers[0].menuState == 9) {
        var_a3 = 0;
        if ((s32) gPlayerCount > 0) {
            var_v1 = D_8010AEE8;
            do {
                if ((*var_v1 != 0) || (temp_v0 = gMenuChoicePromptState[var_a3], (temp_v0 == 1)) || (temp_v0 >= 5)) {
                    var_a0 += 1;
                }
                var_a3 += 1;
                var_v1 += 1;
            } while (var_a3 < (s32) gPlayerCount);
        }
        if (var_a0 == 0) {
            var_a3 = 0;
            if ((s32) gPlayerCount < 3) {
                var_a1 = gPlayerCount;
            } else {
                var_a1 = 4;
            }
            var_a3 = 0;
            if ((s32)var_a1 > 0) {
                do {
                    gRacePlayers[var_a3].menuState = 3;
                    var_a3++;
                } while (var_a3 < (s32)var_a1);
            }
            D_800EC9C0 = 0xF;
            gMenuFlowState = 1;
        }
        var_v0_3 = (u8) D_800EC9C0;
    } else if ((u8) D_800EC9C0 == 0) {

        var_a3 = 0;
        if (gMenuChoicePromptState[0] == 0) {
            if ((gCourseSelectStatus.unk0[0] == 1) && (gRacePlayers[0].menuState == 0) && !(D_8010AECC & 1)) {
                if ((gRacePlayers[0].selectedCharacterId == 5) || (*D_8010AEA0 == 0) || (var_t1 = 4, (gCourseSelectModeSelection == 1))) {
                    var_t1 = 3;
                }
                sp32 = var_t1;
                if ((s32) gPlayerCount >= 2) {
                    var_t1 = (var_t1 - 1) & 0xFF;
                }
                temp_v0_2 = gPlayerInputHeld[0] & 0x10800;
                sp31 = gCharacterSelectHudState.highlightedRosterIndices[0];
                if ((temp_v0_2 == 0) && !(gPlayerInputHeld[0] & 0x20400)) {
                    gMenuInputRepeatTimers[0] = 0;
                }
                if ((gPlayerInputPressed[0] & 0x10800) || ((temp_v0_2 != 0) && ((s32) (u16) gMenuInputRepeatTimers[0] >= 9))) {
                    if ((u16) gMenuInputRepeatTimers[0] == 0) {
                        gMenuInputRepeatTimers[0] = (u16) gMenuInputRepeatTimers[0] + 1;
                    }
                    if (gCharacterSelectHudState.highlightedRosterIndices[0] > 0) {
                        gCharacterSelectHudState.highlightedRosterIndices[0] -= 1;
                        enqueueSoundEffect(0x19, 0x32);
                        var_a3 = 0;
                    }
                } else if ((gPlayerInputPressed[0] & 0x20400) || ((gPlayerInputHeld[0] & 0x20400) && ((s32) (u16) gMenuInputRepeatTimers[0] >= 9))) {
                    if ((u16) gMenuInputRepeatTimers[0] == 0) {
                        gMenuInputRepeatTimers[0] = (u16) gMenuInputRepeatTimers[0] + 1;
                    }
                    if (gCharacterSelectHudState.highlightedRosterIndices[0] < (s32) var_t1) {
                        gCharacterSelectHudState.highlightedRosterIndices[0] += 1;
                        enqueueSoundEffect(0x19, 0x32);
                        var_a3 = 0;
                    }
                }
                if ((u16) gMenuInputRepeatTimers[0] != 0) {
                    var_v1_2 = (u16) gMenuInputRepeatTimers[0] + 1;
                    gMenuInputRepeatTimers[0] = var_v1_2;
                    if (var_v1_2 == 0xFFFF) {
                        gMenuInputRepeatTimers[0] = 0xA;
                    }
                }
                if ((sp32 != gCharacterSelectHudState.highlightedRosterIndices[0]) && (sp32 != (u8) sp31) &&
                    ((u8) sp31 != gCharacterSelectHudState.highlightedRosterIndices[0])) {
                    D_8010AECC += 1;
                    if ((s32) (u8) sp31 < gCharacterSelectHudState.highlightedRosterIndices[0]) {
                        D_8010AEE8[0] = 0xFF800000;
                    } else {
                        D_8010AEE8[0] = 0x800000;
                    }
                }
                if (sp32 == gCharacterSelectHudState.highlightedRosterIndices[0]) {
                    var_v0_4 = D_8010AEF7[sp32];
                    gCourseSelectExtraCourseColumnState = 2;
                } else {
                    var_v0_4 = D_8010AEF8[gCharacterSelectHudState.highlightedRosterIndices[0]];
                    gCourseSelectExtraCourseColumnState = 0;
                }
                gRacePlayers[0].menuSelection = var_v0_4;
                if ((s32) gRacePlayers[0].menuSelection >= 9) {
                    gRacePlayers[0].selectionUnlockState = 0;
                } else {
                    temp_a0_2 = gGameSaveDataBuffer[0].courseUnlockStates[gRacePlayers[0].menuSelection];
                    if (temp_a0_2 == -1) {
                        gRacePlayers[0].selectionUnlockState = (u8) ((s32) gRacePlayers[0].menuSelection % 3);
                    } else {
                        gRacePlayers[0].selectionUnlockState = (u8) temp_a0_2;
                    }
                }
                if ((D_8010AEE8[0] == 0) && ((gPlayerInputPressed[0] & 0x1000) || (gPlayerInputPressed[0] & 0x8000))) {
                    enqueueSoundEffect(1, 0x32);
                    var_a3 = 0;
                    if ((gPlayerCount == 1) && (var_t1 == gCharacterSelectHudState.highlightedRosterIndices[0])) {
                        gRacePlayers[0].menuState = 9;
                        gMenuFlowState = 1;
                    } else {
                        D_8010AEA4 = 1;
                        gMenuChoicePromptState[0] = 9;
                        gMenuInputRepeatTimers[0] = 0;
                        gRacePlayers[0].menuState = 1;
                        if (gCourseSelectModeSelection == 0) {
                            createCallbackTask((CallbackTaskCallback)initCourseUnlockPricePanel, 0, 0x62);
                            var_a3 = 0;
                        }
                    }
                } else if (gPlayerInputPressed[0] & 0x4000) {
                    enqueueSoundEffect(1, 0x32);
                    gMenuInputRepeatTimers[0] = 0;
                    var_a3 = 0;
                    var_a1 = gPlayerCount;
                    if ((s32) var_a1 > 0) {
                        var_v0_5 = gRacePlayers;
                        do {
                            var_v0_5->menuState = 9;
                            var_v0_5 += 1;
                        } while ((u32) var_v0_5 < (u32) (gRacePlayers + var_a1));
                    }
                }
            }
            if ((gRacePlayers[0].menuState == 1) && (gCourseSelectModeSelection == 1)) {
                setCurrentGameTaskCallback(updateCourseSelectUnlockCourseList, 0);
                gCurrentGameTask->screenState = 0;
            }
        } else {
            new_var = 2;
            if (gMenuChoicePromptState[0] == 9) {
                D_8010AEA4 = (u8) D_8010AEA4 + 1;
            } else {
                D_8010AEA4 = 0;
            }
            if ((gMenuChoicePromptState[0] >= new_var) && (gMenuChoicePromptState[0] < 5)) {
                if (!(gPlayerInputHeld[0] & 0x10800) && !(gPlayerInputHeld[0] & 0x20400)) {
                    gMenuInputRepeatTimers[0] = 0;
                }
                temp_t4 = gPlayerInputPressed[0];
                if ((temp_t4 & 0x10800) || ((gPlayerInputHeld[0] & 0x10800) && ((s32) (u16) gMenuInputRepeatTimers[0] >= 9) && ((u16) gMenuInputRepeatTimers[0] & 1))) {
                    var_v1_2 = (u16) gMenuInputRepeatTimers[0];
                    if (var_v1_2 == 0) {
                        var_v1_2 = (var_v1_2 + 1) & 0xFFFF;
                    }
                    gMenuInputRepeatTimers[0] = (s16) var_v1_2;
                    if (gMenuChoicePromptState[0] >= 3) {
                        gMenuChoicePromptState[0] -= 1;
                        enqueueSoundEffect(0x19, 0x32);
                        var_a3 = 0;
                        D_8010AF44 = 0;
                    }
                } else {
                    var_v1_2 = (u16) gMenuInputRepeatTimers[0];
                    if ((temp_t4 & 0x20400) || ((gPlayerInputHeld[0] & 0x20400) && ((s32) var_v1_2 >= 9) && (var_v1_2 & 1))) {
                        if (var_v1_2 == 0) {
                            var_v1_2 = (var_v1_2 + 1) & 0xFFFF;
                        }
                        gMenuInputRepeatTimers[0] = (s16) var_v1_2;
                        if (gMenuChoicePromptState[0] < (gCourseSelectStatus.unk24[0] + 1)) {
                            gMenuChoicePromptState[0] += 1;
                            enqueueSoundEffect(0x19, 0x32);
                            var_a3 = 0;
                            D_8010AF44 = 0;
                        }
                    }
                }
                if ((u16) gMenuInputRepeatTimers[0] != 0) {
                    var_v1_2 = (u16) gMenuInputRepeatTimers[0] + 1;
                    gMenuInputRepeatTimers[0] = var_v1_2;
                    if (var_v1_2 == 0xFFFF) {
                        gMenuInputRepeatTimers[0] = 0xA;
                    }
                }
                if ((*D_8010AEA0 == 1) && (gCharacterSelectHudState.highlightedRosterIndices[0] == 3)) {
                    var_t8 = D_8010AF06[gMenuChoicePromptState[0]];
                } else {
                    var_t8 = ((gMenuChoicePromptState[0] * 3) + ((s32) gRacePlayers[0].menuSelection % 3)) - 6;
                }
                gRacePlayers[0].menuSelection = var_t8;
                temp_t4 = gPlayerInputPressed[0];
                if (temp_t4 & 0x4000) {
                    enqueueSoundEffect(0x18, 0x32);
                    D_8010AF44 = 0;
                    gRacePlayers[0].menuSelection = D_8010AEF8[gCharacterSelectHudState.highlightedRosterIndices[0]];
                    gMenuInputRepeatTimers[0] = 0;
                    var_a3 = 0;
                    gMenuChoicePromptState[0] += 3;
                } else if ((temp_t4 & 0x8000) || (temp_t4 & 0x1000)) {
                    if (gGameSaveDataBuffer[0].courseUnlockStates[gRacePlayers[0].menuSelection] == -1) {
                        if ((u32) gRacePlayers[0].money >= (u32) gCourseUnlockPrices[gRacePlayers[0].menuSelection]) {
                            enqueueSoundEffect(0x49, 0x32);
                            var_a3 = 0;
                            if ((*D_8010AEA0 == 1) && (gCharacterSelectHudState.highlightedRosterIndices[0] == 3)) {
                                var_v0_6 = D_8010AF06[gMenuChoicePromptState[0]];
                            } else {
                                var_v0_6 = (((gMenuChoicePromptState[0] * 3) + ((s32) gRacePlayers[0].menuSelection % 3)) - 6) & 0xFF;
                            }
                            *(volatile u8 *)&gRacePlayers[0].selectionUnlockState =
                                gGameSaveDataBuffer[0].courseUnlockStates[var_v0_6];
                            *(volatile u8 *)&gRacePlayers[0].menuSelection = var_v0_6;
                            if (D_8010AECC == 0) {
                                gCourseSelectStatus.unk14[0] = 0;
                                gCourseSelectStatus.unk4Array[0] = 7;
                            } else {
                                gCourseSelectStatus.unk1C[0] = 0;
                                gCourseSelectStatus.unk8Array[0] = 7;
                            }
                        } else if (D_8010AF44 == 0) {
                            enqueueSoundEffect(0x47, 0x32);
                            var_a3 = 0;
                            D_8010AF44 = 1;
                        }
                    } else if (D_8010AF44 == 0) {
                        D_8010AF44 = new_var;
                    }
                }
            }
        }
        if ((gCourseSelectStatus.unk4Array[0] == 7) || (gCourseSelectStatus.unk8Array[0] == 7)) {
            gCurrentGameTask->timer = 0;
            setCurrentGameTaskCallback(updateCourseSelectPurchasePrompt, 0);
            createCallbackTask((CallbackTaskCallback)initCourseUnlockPurchasePrompt, 0, 0x64);
        }
        var_v0_3 = (u8) D_800EC9C0;
    } else {
        D_800EC9C0 = (u8) D_800EC9C0 + 1;
        var_v0_3 = (u8) D_800EC9C0;
        if ((s32) var_v0_3 >= 0x22) {
            D_800EC9C0 = 0;
            var_v0_3 = 0;
        }
    }
    if (var_v0_3 == 0x1B) {
        sp18.selection = gRacePlayers + var_a3;
        gCurrentGameTask->fade = 1;
        gRacePlayers[0].menuState = 0;
        setCurrentGameTaskCallback(updateCourseSelectModeMenu, 0);
        sp18.selection->menuSelection = D_8010AEF8[gCharacterSelectHudState.highlightedRosterIndices[0]];
        if (gMenuFlowState == 1) {
            gRacePlayers[0].menuSelection = 0;
        }
        gMenuFlowState = 0;
        D_800EC9C0 = 0;
        gMenuChoicePromptState[0] = 0;
        D_8010AECC = 0;
        gCourseSelectStatus.unk0[0] = 0;
        gCourseSelectStatus.unk4Array[0] = 0;
        gCourseSelectStatus.unk8Array[0] = 0;
        gCourseSelectStatus.unkCArray[0] = 0;
        gCourseSelectStatus.unk10Array[0] = 0;
        gCourseSelectStatus.unk14[0] = 0;
        gCourseSelectStatus.unk1C[0] = 0;
        gCourseSelectStatus.unk24[0] = 0;
        gCourseSelectStatus.unk0Array[1] = 0;
        gCourseSelectStatus.unk4Array[1] = 0;
        gCourseSelectStatus.unk8Array[1] = 0;
        gCourseSelectStatus.unkCArray[1] = 0;
        gCourseSelectStatus.unk10Array[1] = 0;
        gCourseSelectStatus.unk14[1] = 0;
        gCourseSelectStatus.unk1C[1] = 0;
        gCourseSelectStatus.unk24[1] = 0;
        gCourseSelectStatus.unk0Array[2] = 0;
        gCourseSelectStatus.unk4Array[2] = 0;
        gCourseSelectStatus.unk8Array[2] = 0;
        gCourseSelectStatus.unkCArray[2] = 0;
        gCourseSelectStatus.unk10Array[2] = 0;
        gCourseSelectStatus.unk14[2] = 0;
        gCourseSelectStatus.unk1C[2] = 0;
        gCourseSelectStatus.unk24[2] = 0;
        gCourseSelectStatus.unk0Array[3] = 0;
        gCourseSelectStatus.unk4Array[3] = 0;
        gCourseSelectStatus.unk8Array[3] = 0;
        gCourseSelectStatus.unkCArray[3] = 0;
        gCourseSelectStatus.unk10Array[3] = 0;
        gCourseSelectStatus.unk14[3] = 0;
        gCourseSelectStatus.unk1C[3] = 0;
        gCourseSelectStatus.unk24[3] = 0;
        gCourseSelectStatus.transitionState = 0;
        gCourseSelectStatus.unk28 = 0;
        gCourseSelectStatus.unk2A = 0;
        gCourseSelectStatus.unk2C = 0;
        gCourseSelectStatus.unk2E = 0;
    }
    var_a3 = 0;
    if ((s32) gPlayerCount > 0) {
        sp18.object = D_801121E0;
        do {
            gCurrentMenuCameraObject = sp18.object;
            sp18.object->update();
            var_a3 += 1;
            sp18.object += 1;
        } while (var_a3 < (s32) gPlayerCount);
    }
    updateCallbackTasks();
}
#endif

#ifdef NON_MATCHING
#define ATTEMPT2
#define ATTEMPT_CHARSTATE
#define ATTEMPT_SOUND_S16
#define ATTEMPT_TRANSITION_S32
#define ATTEMPT_NEW_TWO
#define ATTEMPT_VART8_S32

#define COURSE_COLUMN gCharacterSelectHudState.highlightedRosterIndices[0]
#define RACE_BASE gRacePlayers
#define RACE_ZERO gRacePlayers[0]
#define COURSE_SELECTION RACE_BASE[var_a3].menuSelection
extern u8 D_8010AF19;
extern u8 D_8010AF1A;
extern u8 D_8010AF1B;
extern u8 D_8010AF1C;
extern u8 D_8010AF1D;
extern u8 D_8010AF1E;
extern u8 D_8010AF1F;
extern u8 D_8010AF20;
extern u8 D_8010AF21;
extern u8 D_8010AF22;
extern u8 D_8010AF23;
extern u8 D_8010AF24;
extern u8 D_8010AF25;
extern u8 D_8010AF26;
extern u8 D_8010AF27;
extern u8 D_8010AF28;
extern u8 D_8010AF29;
extern u8 D_8010AF2A;
extern u8 D_8010AF2B;
extern s16 D_8010AF2C;
extern s16 D_8010AF2E;
extern s16 D_8010AF30;
extern s16 D_8010AF32;
extern s16 D_8010AF34;
extern s16 D_8010AF36;
extern s16 D_8010AF38;
extern s16 D_8010AF3A;
extern u8 D_8010AF3C;
extern u8 D_8010AF3D;
extern u8 D_8010AF3E;
extern u8 D_8010AF3F;

void updateCourseSelectCourseList(void) {
    s32 var_a3;
#ifdef ATTEMPT_VOLATILE
    volatile u8 sp32;
    volatile s8 sp31;
#else
    u8 sp32;
#ifndef ATTEMPT_REORDER_SP31
    s8 sp31;
#endif
#endif
#ifdef ATTEMPT_SPLIT_POINTERS
#elif defined(ATTEMPT_NONVOLATILE_POINTER)
    CourseSelectPointer sp18;
#else
    volatile CourseSelectPointer sp18;
#endif
    RacePlayer *var_v0_5;
#ifdef ATTEMPT_SPLIT_POINTERS
    RaceCamera *var_v0_7;
#endif
#ifdef ATTEMPT_REORDER_SP31
    s8 sp31;
#endif
    s32 temp_t4;
    s32 temp_v0_2;
    s32 var_a0;
#ifdef ATTEMPT_NEW_TWO
    s32 new_var;
#endif
    s8 temp_a0_2;
    u16 var_v1_2;
#ifdef ATTEMPT2
    s32 var_a1;
#else
    u8 var_a1;
#endif
    u8 var_t1;
    s32 var_t8;
#ifdef ATTEMPT_TRANSITION_S32
    s32 var_v0_3;
#else
    u8 var_v0_3;
#endif
    u8 var_v0_4;
    s32 var_v0_6;

    var_a0 = 0;
    if (RACE_ZERO.menuState == 9) {
        var_a3 = 0;
        if ((s32)gPlayerCount > 0) {
            while (1) {
                if ((D_8010AEE8[var_a3] != 0) || (gMenuChoicePromptState[var_a3] == 1) ||
                    (gMenuChoicePromptState[var_a3] >= 5)) {
                    var_a0 += 1;
                }
                var_a3 += 1;
                if (!(var_a3 < (s32)gPlayerCount)) {
                    break;
                }
            }
        }
        if (var_a0 == 0) {
            var_a3 = 0;
            if ((s32)gPlayerCount < 3) {
                var_a1 = gPlayerCount;
            } else {
                var_a1 = 4;
            }
            var_a3 = 0;
            if ((s32)var_a1 > 0) {
                do {
                    RACE_BASE[var_a3].menuState = 3;
                    var_a3++;
                } while (var_a3 < (s32)var_a1);
            }
            D_800EC9C0 = 0xF;
            gMenuFlowState = 1;
        }
        var_v0_3 = (u8)D_800EC9C0;
    } else if ((u8)D_800EC9C0 == 0) {
        var_a3 = 0;
        if (gMenuChoicePromptState[0] == 0) {
            if ((gCourseSelectStatus.unk0[0] == 1) && (RACE_BASE[var_a3].menuState == 0) &&
                !(D_8010AECC & 1)) {
                if ((RACE_ZERO.selectedCharacterId == 5) || (*D_8010AEA0 == 0) ||
                    (var_t1 = 4, (gCourseSelectModeSelection == 1))) {
                    var_t1 = 3;
                }
                sp32 = var_t1;
                if ((s32)gPlayerCount >= 2) {
                    var_t1 = (var_t1 - 1) & 0xFF;
                }
                temp_v0_2 = gPlayerInputHeld[0] & 0x10800;
                sp31 = COURSE_COLUMN;
                if ((temp_v0_2 == 0) && !(gPlayerInputHeld[0] & 0x20400)) {
                    gMenuInputRepeatTimers[0] = 0;
                }
                if ((gPlayerInputPressed[0] & 0x10800) ||
                    ((temp_v0_2 != 0) && ((s32)(u16)gMenuInputRepeatTimers[0] >= 9))) {
                    if ((u16)gMenuInputRepeatTimers[0] == 0) {
                        gMenuInputRepeatTimers[0] = (u16)gMenuInputRepeatTimers[0] + 1;
                    }
                    if (COURSE_COLUMN > 0) {
                        COURSE_COLUMN -= 1;
                        enqueueSoundEffect(0x19, 0x32);
                        var_a3 = 0;
                    }
                } else if ((gPlayerInputPressed[0] & 0x20400) ||
                           ((gPlayerInputHeld[0] & 0x20400) && ((s32)(u16)gMenuInputRepeatTimers[0] >= 9))) {
                    if ((u16)gMenuInputRepeatTimers[0] == 0) {
                        gMenuInputRepeatTimers[0] = (u16)gMenuInputRepeatTimers[0] + 1;
                    }
                    if (COURSE_COLUMN < (s32)var_t1) {
                        COURSE_COLUMN += 1;
                        enqueueSoundEffect(0x19, 0x32);
                        var_a3 = 0;
                    }
                }
                if ((u16)gMenuInputRepeatTimers[0] != 0) {
                    var_v1_2 = (u16)gMenuInputRepeatTimers[0] + 1;
                    gMenuInputRepeatTimers[0] = var_v1_2;
                    if (var_v1_2 == 0xFFFF) {
                        gMenuInputRepeatTimers[0] = 0xA;
                    }
                }
                if ((sp32 != COURSE_COLUMN) && (sp32 != (u8)sp31) && ((u8)sp31 != COURSE_COLUMN)) {
                    D_8010AECC += 1;
                    if ((s32)(u8)sp31 < COURSE_COLUMN) {
                        D_8010AEE8[0] = 0xFF800000;
                    } else {
                        D_8010AEE8[0] = 0x800000;
                    }
                }
                if (sp32 == COURSE_COLUMN) {
                    var_v0_4 = D_8010AEF7[sp32];
                    gCourseSelectExtraCourseColumnState = 2;
                    COURSE_SELECTION = var_v0_4;
                } else {
                    var_v0_4 = D_8010AEF8[COURSE_COLUMN];
                    gCourseSelectExtraCourseColumnState = 0;
                    COURSE_SELECTION = var_v0_4;
                }
#ifdef ATTEMPT_PLAYER
                var_v0 = RACE_BASE;
                if ((s32)var_v0->unk6 >= 9) {
                    var_v0->unk7 = 0;
                } else {
                    temp_a0_2 = gGameSaveDataBuffer[0].courseUnlockStates[var_v0->unk6];
                    if (temp_a0_2 == -1) {
                        var_v0->unk7 = (u8)((s32)var_v0->unk6 % 3);
                    } else {
                        var_v0->unk7 = (u8)temp_a0_2;
                    }
                }
#else
                if ((s32)RACE_ZERO.menuSelection >= 9) {
                    RACE_ZERO.selectionUnlockState = 0;
                } else {
                    temp_a0_2 = gGameSaveDataBuffer[0].courseUnlockStates[RACE_ZERO.menuSelection];
                    if (temp_a0_2 == -1) {
                        RACE_ZERO.selectionUnlockState = (u8)((s32)RACE_ZERO.menuSelection % 3);
                    } else {
                        RACE_ZERO.selectionUnlockState = (u8)temp_a0_2;
                    }
                }
#endif
#ifdef ATTEMPT_PRESSED
                if ((D_8010AEE8[0] == 0) &&
                    (temp_t4 = gPlayerInputPressed[0], ((temp_t4 & 0x1000) || (temp_t4 & 0x8000)))) {
#else
                if ((D_8010AEE8[0] == 0) && ((gPlayerInputPressed[0] & 0x1000) || (gPlayerInputPressed[0] & 0x8000))) {
#endif
                    enqueueSoundEffect(1, 0x32);
                    var_a3 = 0;
                    if ((gPlayerCount == 1) && (var_t1 == COURSE_COLUMN)) {
                        RACE_ZERO.menuState = 9;
                        gMenuFlowState = 1;
                    } else {
                        D_8010AEA4 = 1;
                        gMenuChoicePromptState[0] = 9;
                        gMenuInputRepeatTimers[0] = 0;
                        RACE_ZERO.menuState = 1;
                        if (gCourseSelectModeSelection == 0) {
                            createCallbackTask((void (*)(void *))initCourseUnlockPricePanel, 0, 0x62);
                            var_a3 = 0;
                        }
                    }
#ifdef ATTEMPT_PRESSED
                } else if ((temp_t4 = gPlayerInputPressed[0]) & 0x4000) {
#else
                } else if (gPlayerInputPressed[0] & 0x4000) {
#endif
                    enqueueSoundEffect(1, 0x32);
                    gMenuInputRepeatTimers[0] = 0;
                    var_a3 = 0;
                    var_a1 = gPlayerCount;
                    if ((s32)var_a1 > 0) {
                        var_v0_5 = RACE_BASE;
                        while (1) {
                            var_v0_5->menuState = 9;
                            var_v0_5 += 1;
                            if (!((u32)var_v0_5 < (u32)(RACE_BASE + var_a1))) {
                                break;
                            }
                        }
                    }
                }
            }
            if ((RACE_ZERO.menuState == 1) && (gCourseSelectModeSelection == 1)) {
                setCurrentGameTaskCallback(updateCourseSelectUnlockCourseList, 0);
                gCurrentGameTask->screenState = 0;
            }
        } else {
#ifdef ATTEMPT_NEW_TWO
            new_var = 2;
#endif
            if (gMenuChoicePromptState[0] == 9) {
                D_8010AEA4 = (u8)D_8010AEA4 + 1;
            } else {
                D_8010AEA4 = 0;
            }
            if ((
                    gMenuChoicePromptState[0] >=
#ifdef ATTEMPT_NEW_TWO
                    new_var
#else
                    2
#endif
                ) &&
                (gMenuChoicePromptState[0] < 5)) {
                if (!(gPlayerInputHeld[0] & 0x10800) && !(gPlayerInputHeld[0] & 0x20400)) {
                    gMenuInputRepeatTimers[0] = 0;
                }
                temp_t4 = gPlayerInputPressed[0];
                if ((temp_t4 & 0x10800) ||
                    ((gPlayerInputHeld[0] & 0x10800) && ((s32)(u16)gMenuInputRepeatTimers[0] >= 9) &&
                     ((u16)gMenuInputRepeatTimers[0] & 1))) {
                    var_v1_2 = (u16)gMenuInputRepeatTimers[0];
                    if (var_v1_2 == 0) {
                        var_v1_2 = (var_v1_2 + 1) & 0xFFFF;
                    }
                    gMenuInputRepeatTimers[0] = (s16)var_v1_2;
                    if (gMenuChoicePromptState[0] >= 3) {
                        gMenuChoicePromptState[0] -= 1;
                        enqueueSoundEffect(0x19, 0x32);
                        var_a3 = 0;
                        D_8010AF44 = 0;
                    }
                } else {
                    var_v1_2 = (u16)gMenuInputRepeatTimers[0];
                    if ((temp_t4 & 0x20400) ||
                        ((gPlayerInputHeld[0] & 0x20400) && ((s32)var_v1_2 >= 9) && (var_v1_2 & 1))) {
                        if (var_v1_2 == 0) {
                            var_v1_2 = (var_v1_2 + 1) & 0xFFFF;
                        }
                        gMenuInputRepeatTimers[0] = (s16)var_v1_2;
                        if (gMenuChoicePromptState[0] < (D_8010AF3C + 1)) {
                            gMenuChoicePromptState[0] += 1;
                            enqueueSoundEffect(0x19, 0x32);
                            var_a3 = 0;
                            D_8010AF44 = 0;
                        }
                    }
                }
                if ((u16)gMenuInputRepeatTimers[0] != 0) {
                    var_v1_2 = (u16)gMenuInputRepeatTimers[0] + 1;
                    gMenuInputRepeatTimers[0] = var_v1_2;
                    if (var_v1_2 == 0xFFFF) {
                        gMenuInputRepeatTimers[0] = 0xA;
                    }
                }
                COURSE_SELECTION =
                    ((*D_8010AEA0 == 1) && (COURSE_COLUMN == 3))
                        ? gCourseSelectExtraCourseIds[gMenuChoicePromptState[0]]
                        : ((gMenuChoicePromptState[0] * 3) + ((s32)COURSE_SELECTION % 3)) - 6;
                temp_t4 = gPlayerInputPressed[0];
                if (temp_t4 & 0x4000) {
                    enqueueSoundEffect(0x18, 0x32);
                    D_8010AF44 = 0;
                    COURSE_SELECTION = D_8010AEF8[COURSE_COLUMN];
                    gMenuInputRepeatTimers[0] = 0;
                    var_a3 = 0;
                    gMenuChoicePromptState[0] += 3;
                } else if ((temp_t4 & 0x8000) || (temp_t4 & 0x1000)) {
#ifdef ATTEMPT_TEMP_TWO
                    temp_t7 = 2;
#endif
                    if (gGameSaveDataBuffer[0].courseUnlockStates[COURSE_SELECTION] == -1) {
                        if ((u32)RACE_ZERO.money >= (u32)gCourseUnlockPrices[COURSE_SELECTION]) {
                            enqueueSoundEffect(0x49, 0x32);
                            var_a3 = 0;
                            if ((*D_8010AEA0 == 1) && (COURSE_COLUMN == 3)) {
                                var_v0_6 = gCourseSelectExtraCourseIds[gMenuChoicePromptState[0]];
                            } else {
                                var_v0_6 =
                                    (((gMenuChoicePromptState[0] * 3) + ((s32)COURSE_SELECTION % 3)) - 6) &
                                    0xFF;
                            }
                            *(volatile u8 *)&RACE_ZERO.selectionUnlockState =
                                gGameSaveDataBuffer[0].courseUnlockStates[var_v0_6];
                            *(volatile u8 *)&COURSE_SELECTION = var_v0_6;
                            if (D_8010AECC == 0) {
                                gCourseSelectStatus.unk14[0] = 0;
                                gCourseSelectStatus.unk4Array[0] = 7;
                            } else {
                                gCourseSelectStatus.unk1C[0] = 0;
                                gCourseSelectStatus.unk8Array[0] = 7;
                            }
                        } else if (D_8010AF44 == 0) {
                            enqueueSoundEffect(0x47, 0x32);
                            var_a3 = 0;
                            D_8010AF44 = 1;
                        }
                    } else if (D_8010AF44 == 0) {
#ifdef ATTEMPT_NEW_TWO
                        D_8010AF44 = new_var;
#elif defined(ATTEMPT_TEMP_TWO)
                        D_8010AF44 = temp_t7;
#else
                    D_8010AF44 = 2;
#endif
                    }
                }
            }
        }
        if ((gCourseSelectStatus.unk4Array[0] == 7) || (gCourseSelectStatus.unk8Array[0] == 7)) {
            gCurrentGameTask->timer = 0;
            setCurrentGameTaskCallback(updateCourseSelectPurchasePrompt, 0);
            createCallbackTask((void (*)(void *))initCourseUnlockPurchasePrompt, 0, 0x64);
        }
        var_v0_3 = (u8)D_800EC9C0;
    } else {
        D_800EC9C0 = (u8)D_800EC9C0 + 1;
        var_v0_3 = (u8)D_800EC9C0;
        if ((s32)var_v0_3 >= 0x22) {
            D_800EC9C0 = 0;
            var_v0_3 = 0;
        }
    }
    if (var_v0_3 == 0x1B) {
#ifdef ATTEMPT_SPLIT_POINTERS
        var_v0_2 = RACE_BASE + var_a3;
#else
        sp18.selection = RACE_BASE + var_a3;
#endif
        gCurrentGameTask->fade = 1;
        RACE_ZERO.menuState = 0;
        setCurrentGameTaskCallback(updateCourseSelectModeMenu, 0);
#ifdef ATTEMPT_SPLIT_POINTERS
        var_v0_2->unk6 = D_8010AEF8[COURSE_COLUMN];
#else
        sp18.selection->menuSelection = D_8010AEF8[COURSE_COLUMN];
#endif
        if (gMenuFlowState == 1) {
            RACE_ZERO.menuSelection = 0;
        }
        gMenuFlowState = 0;
        D_800EC9C0 = 0;
        gMenuChoicePromptState[0] = 0;
        D_8010AECC = 0;
        gCourseSelectStatus.unk0[0] = 0;
        D_8010AF1C = 0;
        D_8010AF20 = 0;
        D_8010AF24 = 0;
        D_8010AF28 = 0;
        D_8010AF2C = 0;
        D_8010AF34 = 0;
        sp32 = 0;
        D_8010AF3C = sp32;
        D_8010AF19 = 0;
        D_8010AF1D = 0;
        D_8010AF21 = 0;
        D_8010AF25 = 0;
        D_8010AF29 = 0;
        D_8010AF2E = 0;
        D_8010AF36 = 0;
        D_8010AF3D = 0;
        D_8010AF1A = 0;
        D_8010AF1E = 0;
        D_8010AF22 = 0;
        D_8010AF26 = 0;
        D_8010AF2A = 0;
        D_8010AF30 = 0;
        D_8010AF38 = 0;
        D_8010AF3E = 0;
        D_8010AF1B = 0;
        D_8010AF1F = 0;
        D_8010AF23 = 0;
        D_8010AF27 = 0;
        D_8010AF2B = 0;
        D_8010AF32 = 0;
        D_8010AF3A = 0;
        D_8010AF3F = 0;
        gCourseSelectStatus.transitionState = 0;
        gCourseSelectStatus.unk28 = 0;
        gCourseSelectStatus.unk2A = 0;
        gCourseSelectStatus.unk2C = 0;
        gCourseSelectStatus.unk2E = 0;
    }
    var_a3 = 0;
    if ((s32)gPlayerCount > 0) {
#ifdef ATTEMPT_SPLIT_POINTERS
        var_v0_7 = D_801121E0;
        while (1) {
            gCurrentMenuCameraObject = var_v0_7;
            var_v0_7->update();
            var_a3 += 1;
            var_v0_7 += 1;
            if (!(var_a3 < (s32)gPlayerCount)) {
                break;
            }
        }
#else
        sp18.object = D_801121E0;
        while (1) {
            gCurrentMenuCameraObject = sp18.object;
            sp18.object->update();
            var_a3 += 1;
            sp18.object += 1;
            if (!(var_a3 < (s32)gPlayerCount)) {
                break;
            }
        }
#endif
    }
    updateCallbackTasks();
}

#undef COURSE_COLUMN
#undef COURSE_SELECTION
#undef RACE_BASE
#undef RACE_ZERO
#undef ATTEMPT2
#undef ATTEMPT_CHARSTATE
#undef ATTEMPT_SOUND_S16
#undef ATTEMPT_TRANSITION_S32
#undef ATTEMPT_NEW_TWO
#undef ATTEMPT_VART8_S32
#endif

void updateCourseSelectPurchasePrompt(void) {
    s32 i;

    if (gCurrentGameTask->timer < 2) {
        if ((gPlayerInputPressed[0] & (STICK_UP | U_JPAD)) && (gCurrentGameTask->timer != 0)) {
            gCurrentGameTask->timer--;
            enqueueSoundEffect(0x19, 0x32);
        } else if ((gPlayerInputPressed[0] & (STICK_DOWN | D_JPAD)) && (gCurrentGameTask->timer != 1)) {
            gCurrentGameTask->timer++;
            enqueueSoundEffect(0x19, 0x32);
        }

        if (gPlayerInputPressed[0] & A_BUTTON) {
            gMenuFlowState = 1;
            if (gCurrentGameTask->timer == 1) {
                enqueueSoundEffect(0x18, 0x32);
                if (D_8010AECC == 0) {
                    gCourseSelectStatus.unk4Array[0] = 1;
                } else {
                    gCourseSelectStatus.unk8Array[0] = 1;
                }
                setCurrentGameTaskCallback(updateCourseSelectCourseList, 0);
            } else {
                enqueueSoundEffect(0x45, 0x32);
                gCurrentGameTask->timer += 2;
            }
        } else if (gPlayerInputPressed[0] & B_BUTTON) {
            gMenuFlowState = 1;
            enqueueSoundEffect(0x18, 0x32);
            if (D_8010AECC == 0) {
                gCourseSelectStatus.unk4Array[0] = 1;
            } else {
                gCourseSelectStatus.unk8Array[0] = 1;
            }
            setCurrentGameTaskCallback(updateCourseSelectCourseList, 0);
        }
    } else if (gCurrentGameTask->timer >= 4) {
        gCurrentGameTask->timer = 0;
        gRacePlayers[0].menuState = 9;
        setCurrentGameTaskCallback(updateCourseSelectCourseList, 0);
    }

    for (i = 0; i < gPlayerCount; i++) {
        (gCurrentMenuCameraObject = &D_801121E0[i])->update();
    }
    updateCallbackTasks();
}

void updateCourseSelectUnlockCourseList(void) {
    s32 i;
    s32 rowOffset;
    u8 selection;
    s32 promptRow;
    s32 courseGridOffset;
    s32 playerIndex;
    s32 columnCount;
    playerIndex = 0;
    if (gMenuChoicePromptState[0] == 9) {
        D_8010AEA4 = ((u8)D_8010AEA4) + 1;
    } else {
        D_8010AEA4 = 0;
    }
    if ((gMenuChoicePromptState[playerIndex] >= 2) && (gMenuChoicePromptState[playerIndex] < 5)) {
        if ((!(gPlayerInputHeld[playerIndex] & (STICK_UP | U_JPAD))) &&
            (!(gPlayerInputHeld[playerIndex] & (STICK_DOWN | D_JPAD)))) {
            gMenuInputRepeatTimers[playerIndex] = playerIndex;
        }
        if ((gPlayerInputPressed[playerIndex] & (STICK_UP | U_JPAD)) ||
            (((gPlayerInputHeld[playerIndex] & (STICK_UP | U_JPAD)) &&
              (((s32)gMenuInputRepeatTimers[playerIndex]) >= 9)) &&
             (gMenuInputRepeatTimers[playerIndex] & 1))) {
            if (!gMenuInputRepeatTimers[playerIndex]) {
                gMenuInputRepeatTimers[playerIndex] += 1;
            }
            if (gMenuChoicePromptState[playerIndex] >= 3) {
                gMenuChoicePromptState[playerIndex] = gMenuChoicePromptState[playerIndex] - 1;
                enqueueSoundEffect(0x19, 0x32);
            }
        } else if ((gPlayerInputPressed[playerIndex] & (STICK_DOWN | D_JPAD)) ||
                   (((gPlayerInputHeld[playerIndex] & (STICK_DOWN | D_JPAD)) &&
                     (gMenuInputRepeatTimers[playerIndex] >= 9)) &&
                    (gMenuInputRepeatTimers[playerIndex] & 1))) {
            if (!gMenuInputRepeatTimers[playerIndex]) {
                gMenuInputRepeatTimers[playerIndex] = gMenuInputRepeatTimers[playerIndex] + 1;
            }
            if (gMenuChoicePromptState[playerIndex] < (gCourseSelectStatus.unk24[playerIndex] + 1)) {
                gMenuChoicePromptState[playerIndex] += 1;
                enqueueSoundEffect(0x19, 0x32);
            }
        }
        columnCount = 3;
        if (gMenuInputRepeatTimers[playerIndex]) {
            gMenuInputRepeatTimers[playerIndex] += 1;
            if (gMenuInputRepeatTimers[playerIndex] == 0xFFFF) {
                gMenuInputRepeatTimers[playerIndex] = 0xA;
            }
        }
        selection = gRacePlayers[playerIndex].menuSelection;
        gRacePlayers[playerIndex].menuSelection =
            (selection = ((gMenuChoicePromptState[playerIndex] * columnCount) + (((s32)selection) % columnCount)) -
                         (courseGridOffset = 6));
        if (gPlayerInputPressed[playerIndex] & B_BUTTON) {
            enqueueSoundEffect(0x18, 0x32);
            gMenuInputRepeatTimers[playerIndex] = 0;
            gMenuChoicePromptState[playerIndex] += 3;
            selection = gRacePlayers[playerIndex].menuSelection;
            gRacePlayers[0].menuSelection = (selection = ((s32)selection) % columnCount);
        } else if ((gPlayerInputPressed[playerIndex] & A_BUTTON) ||
                   (gPlayerInputPressed[playerIndex] & START_BUTTON)) {
            enqueueSoundEffect(0x18, 0x32);
            rowOffset = columnCount;
            i = gMenuChoicePromptState[playerIndex] * columnCount;
            promptRow = gMenuChoicePromptState[playerIndex];
            rowOffset = promptRow * rowOffset;
            selection = (s32)gRacePlayers[0].menuSelection;
            selection = (rowOffset + (selection % columnCount)) - ((0, courseGridOffset));
            gRacePlayers[0].selectionUnlockState = gGameSaveDataBuffer[playerIndex].courseUnlockStates[selection];
            gRacePlayers[playerIndex].menuSelection = selection;
            if (!D_8010AECC) {
                gCourseSelectStatus.unk14[0] = playerIndex;
                gCourseSelectStatus.unk4Array[playerIndex] = 7;
            } else {
                gCourseSelectStatus.unk1C[0] = playerIndex;
                gCourseSelectStatus.unk8Array[playerIndex] = 7;
            }
        }
    } else if (!gMenuChoicePromptState[0]) {
        setCurrentGameTaskCallback(updateCourseSelectCourseList, 0);
    }
    if ((gCourseSelectStatus.unk4Array[0] == 7) || (gCourseSelectStatus.unk8Array[0] == 7)) {
        gCurrentGameTask->screenState = 1;
        setCurrentGameTaskCallback(initCourseSelectCourseDetailsMenu, 0);
    }
    for (i = playerIndex; i < gPlayerCount; i++) {
        (gCurrentMenuCameraObject = &D_801121E0[i])->update();
    }

    updateCallbackTasks();
}

#if 0 /* Superseded without consulting the previous attempt. */
void updateCourseSelectUnlockCourseList(void) {
    u16 new_var3;
    u16 *new_var2;
    s32 buttonsHeld;
    s16 state;
    int new_var;
    s32 buttonsPressed;
    s32 held10800;
    u16 repeat;
    u8 new_var4;
    short divisor;
    s16 *new_var5;

    new_var4 = 1;
    if (gMenuChoicePromptState[0] == 9) {
        D_8010AEA4++;
    } else {
        D_8010AEA4 = 0;
    }

    new_var5 = gMenuChoicePromptState;
    state = *(volatile s16 *)&gMenuChoicePromptState[0];
    if (state < 2) {
        goto outside_menu_rows;
    }
    if (state >= 5) {
        goto outside_menu_rows;
    }

    buttonsHeld = gPlayerInputHeld[0];
    held10800 = buttonsHeld & (STICK_UP | U_JPAD);
    if ((held10800 == 0) && ((buttonsHeld & (STICK_DOWN | D_JPAD)) == 0)) {
        gMenuInputRepeatTimers[0] = 0;
    }

    new_var = 0x19;
    buttonsPressed = gPlayerInputPressed[0];
    repeat = gMenuInputRepeatTimers[0];
    new_var2 = &repeat;
    new_var3 = repeat;
    if ((buttonsPressed & 0x10800) || ((held10800 != 0) && (new_var3 >= 9) && (new_var3 & new_var4))) {
        if (*new_var2 == 0) {
            repeat++;
        }
        gMenuInputRepeatTimers[0] = *new_var2;
        if (state >= 3) {
            gMenuChoicePromptState[0] = state - new_var4;
            enqueueSoundEffect(new_var, 0x32);
            buttonsPressed = gPlayerInputPressed[0];
        }
        state = *new_var5;
    } else {
        if (!(buttonsPressed & 0x20400)) {
            gMenuInputRepeatTimers[0] = *new_var2;
            if (!(buttonsHeld & 0x20400)) {
                goto after_row_change;
            }
            gMenuInputRepeatTimers[0] = *new_var2;
            if (*new_var2 < 9) {
                goto after_row_change;
            }
            gMenuInputRepeatTimers[0] = *new_var2;
            if (!(*new_var2 & new_var4)) {
                goto after_row_change;
            }
        }

        if (new_var4) {
        }
        if (new_var4) {
        }
        if (new_var4) {
        }
        if (new_var4) {
        }
        if (new_var4) {
        }
        if (!*new_var2) {
            repeat++;
        }
        gMenuInputRepeatTimers[0] = *new_var2;
        if (state < gCourseSelectStatus.unk24[0] + new_var4) {
            gMenuChoicePromptState[0] = state + 1;
            enqueueSoundEffect(0x19, 0x32);
            state = *new_var5;
            buttonsPressed = gPlayerInputPressed[0];
        }
    }

after_row_change:
    divisor = 3;

    repeat = gMenuInputRepeatTimers[0];
    if (*new_var2 != 0) {
        repeat++;
        gMenuInputRepeatTimers[0] = *new_var2;
        if (*new_var2 == 0xFFFF) {
            repeat = 10;
            gMenuInputRepeatTimers[0] = *new_var2;
            do {
            } while (0);
        }
    }

    gRacePlayers[0].menuSelection =
        ((state * divisor) + (gRacePlayers[0].menuSelection % divisor) - 6) & 0xFF;
    gMenuChoicePromptState[0] = state;
    if (buttonsPressed & B_BUTTON) {
        enqueueSoundEffect(0x18, 0x32);
        gRacePlayers[0].menuSelection = (gRacePlayers[0].menuSelection % divisor) & 0xFF;
        gMenuInputRepeatTimers[0] = 0;
        gMenuChoicePromptState[0] = *new_var5 + 3;
    } else if ((buttonsPressed & 0x8000) || ((buttonsPressed & 0xFFFFu) & 0x1000)) {
        u8 index;

        enqueueSoundEffect(0x18, 0x32);
        index = ((gMenuChoicePromptState[0] * divisor) + (gRacePlayers[0].menuSelection % divisor) - 6) & 0xFF;
        gCourseSelectSelectedCourseSavedSlot = gGameSaveDataBuffer[0].courseUnlockStates[index];
        gRacePlayers[0].menuSelection = index;
        if (D_8010AECC == 0) {
            gCourseSelectStatus.unk14[0] = 0;
            gCourseSelectStatus.unk4Array[0] = 7;
        } else {
            gCourseSelectStatus.unk1C[0] = 0;
            gCourseSelectStatus.unk8Array[0] = 7;
        }
    }
    goto after_input;

outside_menu_rows:
    if (gMenuChoicePromptState[0] == 0) {
        setCurrentGameTaskCallback(updateCourseSelectCourseList, 0);
    }

after_input:
    if ((gCourseSelectStatus.unk4Array[0] == 7) || (gCourseSelectStatus.unk8Array[0] == 7)) {
        gCurrentGameTask->screenState = 1;
        setCurrentGameTaskCallback(initCourseSelectCourseDetailsMenu, 0);
    }

    {
        RaceCamera *var_s0;
        s32 i;

        i = 0;
        if ((s32) gPlayerCount >
            (((gMenuChoicePromptState[0] * divisor) + (gRacePlayers[0].menuSelection % divisor)) * 0)) {
            var_s0 = D_801121E0;
            do {
                (gCurrentMenuCameraObject = var_s0)->update();
                i++;
                var_s0++;
            } while (i < (s32) gPlayerCount);
        }
    }
    updateCallbackTasks();
}
#endif

#if 0 /* Superseded without consulting the previous attempt. */
void updateCourseSelectUnlockCourseList(void) {
    s32 i;
    int playerIndex;
    s32 held;
    s32 heldUp;
    s32 pressed;
    s32 upMask;
    register s32 divisor;
    u8 *selectionPtr;
    u16 repeatTimer;
    u8 selection;
    u8 cancelSelection;
    RaceCamera *camera;
    s16 initialState;

    playerIndex = 0;
    initialState = gMenuChoicePromptState[playerIndex];
    if (initialState == 9) {
        D_8010AEA4 = (u8)D_8010AEA4 + 1;
    } else {
        D_8010AEA4 = playerIndex;
    }

    if ((gMenuChoicePromptState[playerIndex] >= 2) &&
        (gMenuChoicePromptState[playerIndex] < 5)) {
        upMask = STICK_UP | U_JPAD;
        held = gPlayerInputHeld[playerIndex];
        heldUp = held & upMask;
        if ((heldUp == playerIndex) &&
            !(gPlayerInputHeld[playerIndex] & (STICK_DOWN | D_JPAD))) {
            gMenuInputRepeatTimers[playerIndex] = playerIndex;
        }

        selectionPtr = &gRacePlayers[0].menuSelection;
        pressed = gPlayerInputPressed[playerIndex];
        repeatTimer = gMenuInputRepeatTimers[playerIndex];
        if ((pressed & upMask) ||
            ((heldUp != playerIndex) && (repeatTimer >= 9) && (repeatTimer & 1))) {
            selection = gMenuChoicePromptState[playerIndex];
            if (repeatTimer == playerIndex) {
                repeatTimer++;
            }
            gMenuInputRepeatTimers[playerIndex] = repeatTimer;
            if (selection >= 3) {
                gMenuChoicePromptState[playerIndex]--;
                enqueueSoundEffect(0x19, 0x32);
                pressed = gPlayerInputPressed[playerIndex];
            }
        } else if ((pressed & (STICK_DOWN | D_JPAD)) ||
                   ((*(volatile u16 *)&gMenuInputRepeatTimers[playerIndex] = repeatTimer,
                     held & (STICK_DOWN | D_JPAD)) &&
                    (*(volatile u16 *)&gMenuInputRepeatTimers[playerIndex] = repeatTimer,
                     repeatTimer >= 9) &&
                    (*(volatile u16 *)&gMenuInputRepeatTimers[playerIndex] = repeatTimer,
                     repeatTimer & 1))) {
            if (repeatTimer == playerIndex) {
                repeatTimer++;
            }
            gMenuInputRepeatTimers[playerIndex] = repeatTimer;
            if (gMenuChoicePromptState[playerIndex] < gCourseSelectStatus.unk24[0] + 1) {
                gMenuChoicePromptState[playerIndex]++;
                enqueueSoundEffect(0x19, 0x32);
                pressed = gPlayerInputPressed[playerIndex];
            }
        }

        repeatTimer = gMenuInputRepeatTimers[playerIndex];
        if (repeatTimer != playerIndex) {
            repeatTimer++;
            gMenuInputRepeatTimers[playerIndex] = repeatTimer;
            if (repeatTimer == 0xFFFF) {
                gMenuInputRepeatTimers[playerIndex] = 0xA;
            }
        }

        divisor = 3;
        selection = (gMenuChoicePromptState[playerIndex] * divisor) +
                    (gRacePlayers[0].menuSelection % divisor) - 6;
        *(volatile u8 *)selectionPtr = selection;
        if (pressed & B_BUTTON) {
            enqueueSoundEffect(0x18, 0x32);
            gMenuChoicePromptState[playerIndex] += 3;
            cancelSelection = gRacePlayers[0].menuSelection % divisor;
            *(volatile u8 *)selectionPtr = cancelSelection;
            gMenuInputRepeatTimers[playerIndex] = playerIndex;
        } else if ((pressed & A_BUTTON) || (pressed & START_BUTTON)) {
            enqueueSoundEffect(0x18, 0x32);
            selection = (gMenuChoicePromptState[playerIndex] * divisor) +
                        (gRacePlayers[0].menuSelection % divisor) - 6;
            gRacePlayers[0].selectionUnlockState = gGameSaveDataBuffer[0].courseUnlockStates[selection];
            *(volatile u8 *)selectionPtr = selection;
            if (D_8010AECC == playerIndex) {
                gCourseSelectStatus.unk14[0] = playerIndex;
                if (pressed) {
                }
                gCourseSelectStatus.unk4Array[0] = 7;
            } else {
                gCourseSelectStatus.unk1C[0] = playerIndex;
                gCourseSelectStatus.unk8Array[0] = 7;
            }
        }
    } else if (initialState == playerIndex) {
        setCurrentGameTaskCallback(updateCourseSelectCourseList, playerIndex);
    }

    if ((gCourseSelectStatus.playerOneCourseDecided == 7) ||
        (gCourseSelectStatus.playerTwoCourseDecided == 7)) {
        gCurrentGameTask->screenState = 1;
        setCurrentGameTaskCallback(initCourseSelectCourseDetailsMenu, playerIndex);
    }

    i = playerIndex;
    if (gPlayerCount > playerIndex) {
        camera = D_801121E0;
        do {
            (gCurrentMenuCameraObject = camera)->update();
            i++;
            camera++;
        } while (i < gPlayerCount);
    }
    updateCallbackTasks();
}
#endif

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initCourseSelectCourseDetailsMenu(void) {
    RaceCamera *var_s1;
    s32 var_s0;
    s8 temp_v0;

    if (gCurrentGameTask->screenState == 2) {
        createCallbackTask((CallbackTaskCallback)initCourseDetailsMenu, 0, 0x63);
        temp_v0 = gGameSaveDataBuffer[0].courseUnlockStates[gRacePlayers[0].menuSelection];
        gCourseDetailsMenuSelection = temp_v0 % 7;
        gCourseDetailsPreviewPage = temp_v0 / 7;
        gMenuInputRepeatTimers[0] = 0;
        gMenuExitSelection = 0;
        setCurrentGameTaskCallback(updateCourseSelectCourseDetailsMenu, 0);
    }

 do { var_s0 = 0; if (gPlayerCount > 0) { var_s1 = D_801121E0; do { ; (gCurrentMenuCameraObject = var_s1)->update(); var_s0 += 1; var_s1 += 1; } while (var_s0 < gPlayerCount); } } while (0);
    updateCallbackTasks();
}
// clang-format on

// updateCourseSelectCourseDetailsMenu best match: 99.474%
// (nonmatchings/updateCourseSelectCourseDetailsMenu-4542485759220937537/base_3.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_menu/updateCourseSelectCourseDetailsMenu.s")

#ifdef NON_MATCHING
void updateCourseSelectCourseDetailsMenu(void) {
    s32 i;
    s32 condition;
    u32 soundId;
    u8 selection;
    u16 oldSelection;
    s32 input;
    s32 heldUp;
    s32 tempSelection;
    register u16 repeat;
    s32 divisor;

    soundId = STICK_UP;
    if ((s32)gCourseDetailsMenuState >= 2) {
        selection = gCourseDetailsMenuSelection;
        heldUp = (input = gPlayerInputHeld[0]) & (soundId | U_JPAD);
        oldSelection = selection;
        i = heldUp;
        if (!i && !(gPlayerInputHeld[0] & (STICK_DOWN | D_JPAD))) {
            gMenuInputRepeatTimers[0] = 0;
            if (1) {}
            if (1) {}
            if (1) {}
        }
        divisor = 3;
        if (1) {
            if ((gPlayerInputPressed[0] & (soundId | U_JPAD)) ||
                (heldUp && ((u16)gMenuInputRepeatTimers[0] >= 0xB) &&
                 (((u16)gMenuInputRepeatTimers[0] % divisor) == ((gPlayerInputHeld[0] & (STICK_DOWN | D_JPAD)) * 0)))) {
                repeat = (u16)gMenuInputRepeatTimers[0];
                tempSelection = ((u32)selection) - 1;
                if (!repeat) {
                    gMenuInputRepeatTimers[0] = repeat + 1;
                    repeat = (u16)gMenuInputRepeatTimers[0];
                }
                if (selection > 0) {
                    gCourseDetailsMenuSelection = tempSelection;
                    selection = (u8)tempSelection;
                }
            } else {
                repeat = (u16)gMenuInputRepeatTimers[0];
                if ((gPlayerInputPressed[0] & (STICK_DOWN | D_JPAD)) ||
                    ((gPlayerInputHeld[0] & (STICK_DOWN | D_JPAD)) && (repeat >= 0xB) && ((repeat % divisor) == 0))) {
                    if (!repeat) {
                        gMenuInputRepeatTimers[0] = repeat + 1;
                        repeat = (u16)gMenuInputRepeatTimers[0];
                    }
                    if (1) {
                        tempSelection = selection + 1;
                    }
                    if (selection < 9) {
                        gCourseDetailsMenuSelection = (u16)tempSelection;
                        selection = (u8)tempSelection;
                    }
                } else if (gPlayerInputPressed[0] & (STICK_LEFT | L_JPAD)) {
                    gMenuExitSelection = 1;
                }
            }
        }
        if (repeat) {
            if (1) {
                repeat += 1;
            }
            gMenuInputRepeatTimers[0] = repeat;
            soundId = (repeat & 0xFFFF) & 0xFFFF;
            if ((((((soundId & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) == ((0xFFFF & 0xFFFF) & 0xFFFF)) {
                gMenuInputRepeatTimers[0] = 0xC;
            }
        }
        if (oldSelection != selection) {
            enqueueSoundEffect(0x19, 0x32);
        }
        if (oldSelection = gPlayerInputPressed[0] & B_BUTTON) {
            gMenuExitSelection = 2;
        }
        soundId = 0x18;
        repeat = 1;
        if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON) ||
            ((selection = gMenuExitSelection) != 0)) {
            if (((u8)gCourseDetailsMenuSelection >= 7) || (gMenuExitSelection != 0)) {
                enqueueSoundEffect(soundId, 0x32);
            }
            if ((((u8)gCourseDetailsMenuSelection == 7) || (gMenuExitSelection == 1)) && (gMenuExitSelection != 2)) {
                gCourseDetailsMenuState = divisor;
                if (gMenuExitSelection == repeat) {
                    gCourseDetailsCloseFromBack = repeat;
                }
                gMenuExitSelection = 0;
                setCurrentGameTaskCallback(waitCourseSelectRecordsClose, 0);
            } else {
                input = gMenuExitSelection;
                if (((u8)gCourseDetailsMenuSelection == 8) || (input == 2)) {
                    setCurrentGameTaskCallback(returnToCourseSelectUnlockCourseList, 0);
                    gMenuExitSelection = 0;
                    gCurrentGameTask->screenState = divisor;
                } else if ((u8)gCourseDetailsMenuSelection == 9) {
                    setCurrentGameTaskCallback(returnToCourseSelectModeMenu, 0);
                    gCurrentGameTask->screenState = 9;
                } else {
                    setCurrentGameTaskCallback(initCourseSelectPreview, 0);
                }
            }
        }
    }

    for (i = 0; i < gPlayerCount; i++) {
        (gCurrentMenuCameraObject = &D_801121E0[i])->update();
    }
    updateCallbackTasks();
}
#endif

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void waitCourseSelectRecordsClose(void) {
    RaceCamera *var_s1;
    s32 var_s0;

    if (gCourseSelectStatus.transitionState == 2) {
        gMenuInputRepeatTimers[0] = 0;
        gCourseDetailsCloseFromBack = 0;
        setCurrentGameTaskCallback(updateCourseSelectCourseDetailsMenu, 0);
    }

 do { var_s0 = 0; if (gPlayerCount > 0) { var_s1 = D_801121E0; do { ; (gCurrentMenuCameraObject = var_s1)->update(); var_s0 += 1; var_s1 += 1; } while (var_s0 < gPlayerCount); } } while (0);
    updateCallbackTasks();
}
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void returnToCourseSelectUnlockCourseList(void) {
    RaceCamera *var_s1;
    s32 var_s0;

    if (gCurrentGameTask->screenState == 5) {
        if (D_8010AECC == 0) {
            gCourseSelectStatus.playerOneCourseDecided = 1;
        } else {
            gCourseSelectStatus.playerTwoCourseDecided = 1;
        }
        gCourseSelectStatus.transitionState = 0;
        gCurrentGameTask->screenState = 0;
        gMenuInputRepeatTimers[0] = 0;
        setCurrentGameTaskCallback(updateCourseSelectUnlockCourseList, 0);
    }

 do { var_s0 = 0; if (gPlayerCount > 0) { var_s1 = D_801121E0; do { ; (gCurrentMenuCameraObject = var_s1)->update(); var_s0 += 1; var_s1 += 1; } while (var_s0 < gPlayerCount); } } while (0);
    updateCallbackTasks();
}
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void returnToCourseSelectModeMenu(void) {
    RaceCamera *var_s1;
    s32 var_s0;
    s32 i;

    if (gCurrentGameTask->screenState == 0xB) {
        setCurrentGameTaskCallback(updateCourseSelectModeMenu, 0);
        gCurrentGameTask->fade = 1;
        gCurrentGameTask->timer = 0;
        gCurrentGameTask->screenState = 0;
        gRacePlayers[0].menuState = 0;
        gRacePlayers[0].menuSelection = gRacePlayers[0].menuSelection % 3;
        gMenuChoicePromptState[0] = 0;
        D_8010AECC = 0;
        gMenuInputRepeatTimers[0] = 0;

        for (i = 0; i < 4; i++) {
            gCourseSelectStatus.unk0Array[i] = 0;
            gCourseSelectStatus.unk4Array[i] = 0;
            gCourseSelectStatus.unk8Array[i] = 0;
            gCourseSelectStatus.unkCArray[i] = 0;
            gCourseSelectStatus.unk10Array[i] = 0;
            gCourseSelectStatus.unk14[i] = 0;
            gCourseSelectStatus.unk1C[i] = 0;
            gCourseSelectStatus.unk24[i] = 0;
        }

        gCourseSelectStatus.unk28 = 0;
        gCourseSelectStatus.unk2A = 0;
        gCourseSelectStatus.transitionState = 0;
        gCourseSelectStatus.unk2C = 0;
        gCourseSelectStatus.unk2E = 0;
    }

 do { var_s0 = 0; if (gPlayerCount > 0) { var_s1 = D_801121E0; do { ; (gCurrentMenuCameraObject = var_s1)->update(); var_s0 += 1; var_s1 += 1; } while (var_s0 < gPlayerCount); } } while (0);
    updateCallbackTasks();
}
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initCourseSelectPreview(void) {
    RaceCamera *var_s0;
    s8 *temp;

    gCoursePreviewViewportHeight = 0x78;
    configureViewport(1, 0xE8, 0x78, 0x90, gCoursePreviewViewportHeight, 0xA0, 0xF0, 0.6666666865f);
    enableViewportClear(1);
    temp = &gPrimaryGameSaveRawData.values[gRacePlayers[0].menuSelection];
    D_8010AED0 = temp[0x3F] + 1;
    temp[0x3F] = gCourseDetailsPreviewCourseTiles[(u8) gCourseDetailsPreviewPage * 7 + (u8) gCourseDetailsMenuSelection];
    gCourseSelectStatus.transitionState = 6;
    createCallbackTask((CallbackTaskCallback)&initCoursePreviewCloseSparkles, 0, 0x64);
    setCurrentGameTaskCallback(updateCourseSelectPreviewClose, 0); var_s0 = D_801121E0; do { gCurrentMenuCameraObject = var_s0; var_s0->update();
        var_s0 += 1;
    } while (var_s0 != &D_80112340);
    updateCallbackTasks();
    enqueueSoundEffect(0x17, 0x32);
}
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void updateCourseSelectPreviewClose(void) {
    RaceCamera *var_s0;

    gCoursePreviewViewportHeight -= 4;
    if (gCoursePreviewViewportHeight < 0) {
        gCoursePreviewViewportHeight = (gCoursePreviewViewportHeight < 0) * 0;
    }
    configureViewport(1, 0xE8, 0x78, 0x90, gCoursePreviewViewportHeight, 0xA0, 0xF0, 0.6666666865f);
    if (gCoursePreviewViewportHeight == 0) {
        resetViewport(1);
        setCurrentGameTaskCallback(updateCourseSelectCourseDetailsMenu, 0);
        gCourseSelectStatus.transitionState = 2;
 D_8010AED0 = 0; } var_s0 = D_801121E0; do { (gCurrentMenuCameraObject = var_s0)->update(); var_s0 += 1; } while (var_s0 != (&D_80112340)); updateCallbackTasks();
}
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void exitCourseSelectMenu(void) {
    s8 *ptr;
    s32 count;

    if (gCurrentGameTask->fade != 0xFF) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) gCurrentGameTask->fade, 0x24, 1);
        if (gCurrentGameTask->fade == 0xFF) {
            gFramebufferSwapHold = 1;
        } else {
            updateCallbackTasks();
        }
    } else if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        do { gFramebufferSwapDelay.value = 0; gMenuFlowState = 0; resumeGameTask(2); removeGameTask(4); count = 0; if (gPrimaryCourseUnlockStates.values[0] == (-1)) { count = 1; } ptr = &gCourseUnlockScanStart.value; loop: if (ptr[0x3F] == (-1)) { count++; } if (ptr[0x40] == (-1)) { count++; } if (ptr[0x41] == (-1)) { count++; } if (ptr[0x42] == (-1)) { count++; } } while (0);
        ptr += 4;
        if (ptr != &gCourseUnlockScanEnd.value) {
            goto loop;
        }
        if (count == 0) {
            gGameSaveDataBuffer[0].extraCourseUnlockFlags |= 4;
        }
    }
}
// clang-format on
