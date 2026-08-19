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
extern u8 gMenuExitSelection;
extern u8 gShopMenuDescriptionSeen;
extern u8 gShopMenuShowNewCoursesMessage;
extern s16 gCoursePreviewViewportHeight;
extern RaceCamera D_801121E0[];
extern RaceCamera D_80112340;
extern s8 gCourseSelectSelectedCourseSavedSlot;
extern s32 gMenuFlowState;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern void releaseMenuAssetHandles(void);
extern s32 enqueueSoundEffect(s16, s16);

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
        gCourseSelectHasExtraCourse[i] = 0;
        gCourseSelectSelectedRows[i] = 0;
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
    gCharacterSelectHighlightedRosterIndices[0] = 0;
    gActiveMenuTask = 0;
    gCurrentGameTask->timer = 0;
    gMenuExitSelection = 0;
    showNewCourses = 0;
    gShopMenuDescriptionSeen = showNewCourses;
    gShopMenuShowNewCoursesMessage = showNewCourses;
    gCourseDetailsMenuSelection = 0;
    gCourseDetailsPreviewPage = 0;
    gCourseSelectViewportSyncState = 0;
    gCourseSelectPurchaseFlowActive = 0;
    gCourseSelectSelectionTimers[0] = 0;
    gCourseSelectInputLocked = 0;
    gMenuChoicePromptState[0] = 0;
    gCourseSelectSlideStates[0] = 0;
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
    COURSE_SELECT_STATUS_LAYOUT.submenuState = 0;
    COURSE_SELECT_STATUS_LAYOUT.cursorState = 0;
    COURSE_SELECT_STATUS_LAYOUT.cursorValue = 0;
    COURSE_SELECT_STATUS_LAYOUT.purchaseMessageState = 0;
    COURSE_SELECT_STATUS_LAYOUT.extraCourseColumnState = 0;
}

void updateCourseSelectModeMenu(void) {
    u8 oldSelection;

    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) gCurrentGameTask->fade, 0x24, 0);
        if (gCurrentGameTask->fade == 0) {
            createCallbackTask((CallbackTaskCallback)initShopMenuModeChoiceRows, 0, 0x63);
            if (gShopMenuDescriptionSeen == 0) {
                enqueueSoundEffect(0x44, 0x32);
            }
        }
    } else {
        if (gMenuSelectionConfirmTimer == 0) {
            if ((gRacePlayers[0].menuState == 0) && (COURSE_SELECT_STATUS_LAYOUT.cursorState == 1)) {
                oldSelection = gCourseSelectModeSelection;
                if (!(gPlayerInputHeld[0] & 0x10800) && !(gPlayerInputHeld[0] & 0x20400)) {
                    gMenuInputRepeatTimers[0] = 0;
                }
                if (
                    (gPlayerInputPressed[0] & 0x10800) ||
                    ((gPlayerInputHeld[0] & 0x10800) && gMenuInputRepeatTimers[0] >= 0xB && ((gMenuInputRepeatTimers[0] % 3) == 0))
                ) {
                    if (!gMenuInputRepeatTimers[0]) {
                        gMenuInputRepeatTimers[0]++;
                    }
                    if (gCourseSelectModeSelection > 0) {
                        gCourseSelectModeSelection--;
                    }
                } else {
                    if (
                        (gPlayerInputPressed[0] & 0x20400) ||
                        ((gPlayerInputHeld[0] & 0x20400) && gMenuInputRepeatTimers[0] >= 0xB && (gMenuInputRepeatTimers[0] % 3) == 0)
                    ) {
                        if (!gMenuInputRepeatTimers[0]) {
                            gMenuInputRepeatTimers[0]++;
                        }
                        if (gCourseSelectModeSelection < 2) {
                            gCourseSelectModeSelection++;
                        }
                    }
                }
                if (gMenuInputRepeatTimers[0]) {
                    gMenuInputRepeatTimers[0]++;
                    if (gMenuInputRepeatTimers[0] == 0xFFFF) {
                        gMenuInputRepeatTimers[0] = 0xC;
                    }
                }
                if (oldSelection != gCourseSelectModeSelection) {
                    enqueueSoundEffect(0x19, 0x32);
                    if (gShopMenuDescriptionSeen == 0) {
                        gShopMenuDescriptionSeen = 1;
                    }
                    if (gShopMenuShowNewCoursesMessage == 1) {
                        gShopMenuShowNewCoursesMessage = 0;
                    }
                }
                if (((gPlayerInputPressed[0] & 0x1000) || (gPlayerInputPressed[0] & 0x8000)) && gMenuFlowState == 2) {
                    gMenuSelectionConfirmTimer = 1;
                    COURSE_SELECT_STATUS_LAYOUT.cursorState = 2;
                    COURSE_SELECT_STATUS_LAYOUT.cursorValue = 0x100;
                    if (gCourseSelectModeSelection < 2) {
                        gMenuExitSelection = 0;
                        enqueueSoundEffect(0x18, 0x32);
                    } else {
                        gMenuExitSelection = 1;
                        enqueueSoundEffect(0x46, 0x32);
                    }
                } else {
                    if ((gPlayerInputPressed[0] & 0x4000) && gMenuFlowState == 2 && gMenuSelectionConfirmTimer == 0) {
                        gMenuSelectionConfirmTimer = 1;
                        COURSE_SELECT_STATUS_LAYOUT.cursorState = 2;
                        COURSE_SELECT_STATUS_LAYOUT.cursorValue = 0x100;
                        gMenuExitSelection = 1;
                        enqueueSoundEffect(0x46, 0x32);
                    }
                }
            }
        } else {
            gMenuSelectionConfirmTimer++;
        }
        if (gMenuSelectionConfirmTimer == 8) {
            gRacePlayers[0].menuState = 1;
            gMenuSelectionConfirmTimer++;
        }
        if (gRacePlayers[0].menuState == 2) {
            gMenuSelectionConfirmTimer = 0;
            if (gMenuExitSelection == 0) {
                setCurrentGameTaskCallback(initCourseSelectCourseList, 0);
            } else {
                setCurrentGameTaskCallback(exitCourseSelectMenu, COURSE_SELECT_STATUS_LAYOUT.cursorState * 0);
                requestMusicSequenceStop(8);
                gMenuExitSelection = 0;
                gCourseSelectExtraCourseColumnState = 0;
            }
        }
    }
    gMenuFlowState = 0;
    updateCallbackTasks();
}

void initCourseSelectCourseList(void) {
    s32 i;
    s32 listMask;
    s32 column;
    s32 selected;
    s32 one;

    gRacePlayers[0].menuState = 0;
    gCourseSelectHasExtraCourse[0] = 0;
    gMenuSelectionConfirmTimer = 0;
    gMenuInputRepeatTimers[0] = 0;
    createCallbackTask((CallbackTaskCallback)initCourseSelectCourseIconList, 0, 0x63);
    D_8010ADE8 = createCallbackTask((CallbackTaskCallback)initCourseSelectExtraCourseIconList, 0, 0x61);

    if (gGameSaveDataBuffer[0].extraCourseUnlockFlags & 7) {
        gCourseSelectHasExtraCourse[0] = 1;
    }

    for (i = 0; i < 3; i++) {
        gCourseSelectCourseIds[0][i] = i;
    }

    one = 1;
    if (gCourseSelectHasExtraCourse[0] == one) {
        column = one;
        for (i = 9; i < 0xC; i++) {
            if (gGameSaveDataBuffer[0].extraCourseUnlockFlags & column) {
                gCourseSelectCourseIds[0][3] = i;
                break;
            }
            column <<= 1;
        }

        listMask = one;
        for (i = 9, column = 0; i < 0xC; i++) {
            gCourseSelectExtraCourseIds[0][column] = 0;
            if (gGameSaveDataBuffer[0].extraCourseUnlockFlags & listMask) {
                gCourseSelectExtraCourseIds[0][column] = i;
                column++;
            }
            listMask <<= 1;
        }
    } else {
        gCourseSelectCourseIds[0][3] = 0;
    }

    if ((gRacePlayers[0].menuSelection >= 9) && (gRacePlayers[0].menuSelection < 0xC)) {
        selected = gCourseSelectExtraCourseColumnState;
        column = 3;
    } else {
        selected = COURSE_SELECT_STATUS_LAYOUT.extraCourseColumnState;
        column = 0;
        if (one == selected) {
            selected = (COURSE_SELECT_STATUS_LAYOUT.extraCourseColumnState = 0);
        } else {
            column = gRacePlayers[0].menuSelection % 3;
        }
    }

    gCharacterSelectHighlightedRosterIndices[0] = column;
    if (one == selected) {
        column--;
    }

    gRacePlayers[0].menuSelection = gCourseSelectCourseIds[0][column];
    setCurrentGameTaskCallback(updateCourseSelectCourseList, 0);
    updateCallbackTasks();
}

// updateCourseSelectCourseList best match: 87.710%
// (nonmatchings/updateCourseSelectCourseList-6/base_24.c)
//
// gCourseSelectSelectedCourseId, gCourseSelectHighlightedColumn and gMenuInputHeld are
// linker aliases onto storage this file also reaches through gRacePlayers,
// gCharacterSelectHighlightedRosterIndices and gPlayerInputHeld. The target reloads those
// addresses where a single object would have been value-numbered into one load, so the
// original source saw two declarations for each.
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_menu/updateCourseSelectCourseList.s")

#ifdef NON_MATCHING
void updateCourseSelectCourseList(void) {
    s32 playerIndex;
    RacePlayer *player;
    s32 pressed;
    s32 busyCount;
    s32 firstPromptRow;
    s8 unlockState;
    u16 repeat;
    s32 count;
    u8 lastColumn;
    u8 maxColumn;
    u8 previousColumn;
    s32 transition;
    u8 selectedCourse;
    u8 purchasedCourse;

    busyCount = 0;
    if (gRacePlayers[0].menuState == 9) {
        playerIndex = 0;
        if ((s32)gPlayerCount > 0) {
            while (1) {
                if ((gCourseSelectHorizontalOffsets[playerIndex] != 0) || (gMenuChoicePromptState[playerIndex] == 1) ||
                    (gMenuChoicePromptState[playerIndex] >= 5)) {
                    busyCount += 1;
                }
                playerIndex += 1;
                if (!(playerIndex < (s32)gPlayerCount)) {
                    break;
                }
            }
        }
        if (busyCount == 0) {
            playerIndex = 0;
            if ((s32)gPlayerCount < 3) {
                count = gPlayerCount;
            } else {
                count = 4;
            }
            // IDO schedules the four-way unroll according to this statement's source line.
            // clang-format off
            for (playerIndex = 0; playerIndex < (s32)count; playerIndex++) { gRacePlayers[playerIndex].menuState = 3; }
            // clang-format on
            D_800EC9C0 = 0xF;
            gMenuFlowState = 1;
        }
        transition = (u8)D_800EC9C0;
    } else if ((u8)D_800EC9C0 == 0) {
        playerIndex = 0;
        if (gMenuChoicePromptState[0] == 0) {
            if ((gCourseSelectPreviewModelState0 == 1) && (gRacePlayers[playerIndex].menuState == 0) &&
                !(gCourseSelectSlideStates[0] & 1)) {
                lastColumn = ((gRacePlayers[0].selectedCharacterId == 5) || (*gCourseSelectHasExtraCourse == 0) ||
                              (gCourseSelectModeSelection == 1))
                             ? 3
                             : 4;
                maxColumn = lastColumn;
                if ((s32)gPlayerCount >= 2) {
                    lastColumn -= 1;
                }
                previousColumn = gCourseSelectHighlightedColumn;
                if (((gPlayerInputHeld[0] & 0x10800) == 0) && !(gMenuInputHeld & 0x20400)) {
                    gMenuInputRepeatTimers[0] = 0;
                }
                if ((gPlayerInputPressed[0] & 0x10800) ||
                    (((gPlayerInputHeld[0] & 0x10800) != 0) && ((s32)(u16)gMenuInputRepeatTimers[0] >= 9))) {
                    if ((u16)gMenuInputRepeatTimers[0] == 0) {
                        gMenuInputRepeatTimers[0] = (u16)gMenuInputRepeatTimers[0] + 1;
                    }
                    if (gCharacterSelectHighlightedRosterIndices[0] > 0) {
                        gCharacterSelectHighlightedRosterIndices[0] -= 1;
                        enqueueSoundEffect(0x19, 0x32);
                        playerIndex = 0;
                    }
                } else if ((gPlayerInputPressed[0] & 0x20400) ||
                           ((gPlayerInputHeld[0] & 0x20400) && ((s32)(u16)gMenuInputRepeatTimers[0] >= 9))) {
                    if ((u16)gMenuInputRepeatTimers[0] == 0) {
                        gMenuInputRepeatTimers[0] = (u16)gMenuInputRepeatTimers[0] + 1;
                    }
                    if (gCharacterSelectHighlightedRosterIndices[0] < (s32)lastColumn) {
                        gCharacterSelectHighlightedRosterIndices[0] += 1;
                        enqueueSoundEffect(0x19, 0x32);
                        playerIndex = 0;
                    }
                }
                if ((u16)gMenuInputRepeatTimers[0] != 0) {
                    repeat = (u16)gMenuInputRepeatTimers[0] + 1;
                    gMenuInputRepeatTimers[0] = repeat;
                    if (repeat == 0xFFFF) {
                        gMenuInputRepeatTimers[0] = 0xA;
                    }
                }
                if ((maxColumn != gCharacterSelectHighlightedRosterIndices[0]) && (maxColumn != (u8)previousColumn) && ((u8)previousColumn != gCharacterSelectHighlightedRosterIndices[0])) {
                    gCourseSelectSlideStates[0] += 1;
                    if ((s32)(u8)previousColumn < gCharacterSelectHighlightedRosterIndices[0]) {
                        gCourseSelectHorizontalOffsets[0] = 0xFF800000;
                    } else {
                        gCourseSelectHorizontalOffsets[0] = 0x800000;
                    }
                }
                if (maxColumn == gCharacterSelectHighlightedRosterIndices[0]) {
                    selectedCourse = gCourseSelectCourseIds[0][maxColumn - 1];
                    gCourseSelectExtraCourseColumnState = 2;
                    gCourseSelectSelectedCourseId = selectedCourse;
                } else {
                    selectedCourse = gCourseSelectCourseIds[0][gCharacterSelectHighlightedRosterIndices[0]];
                    gCourseSelectExtraCourseColumnState = 0;
                    gCourseSelectSelectedCourseId = selectedCourse;
                }
                if ((s32)gRacePlayers[playerIndex].menuSelection >= 9) {
                    gRacePlayers[playerIndex].selectionUnlockState = 0;
                } else {
                    unlockState = gGameSaveDataBuffer[0].courseUnlockStates[gRacePlayers[playerIndex].menuSelection];
                    if (unlockState == -1) {
                        gRacePlayers[playerIndex].selectionUnlockState = (u8)((s32)gRacePlayers[playerIndex].menuSelection % 3);
                    } else {
                        gRacePlayers[playerIndex].selectionUnlockState = (u8)unlockState;
                    }
                }
                if ((gCourseSelectHorizontalOffsets[0] == 0) && ((gPlayerInputPressed[0] & 0x1000) || (gPlayerInputPressed[0] & 0x8000))) {
                    enqueueSoundEffect(1, 0x32);
                    playerIndex = 0;
                    if ((gPlayerCount == 1) && (lastColumn == gCharacterSelectHighlightedRosterIndices[0])) {
                        gRacePlayers[0].menuState = 9;
                        gMenuFlowState = 1;
                    } else {
                        gCourseSelectSelectionTimers[0] = 1;
                        gMenuChoicePromptState[0] = 9;
                        gMenuInputRepeatTimers[0] = 0;
                        gRacePlayers[0].menuState = 1;
                        if (gCourseSelectModeSelection == 0) {
                            createCallbackTask((CallbackTaskCallback)initCourseUnlockPricePanel, 0, 0x62);
                            playerIndex = 0;
                        }
                    }
                } else if (gPlayerInputPressed[0] & 0x4000) {
                    enqueueSoundEffect(1, 0x32);
                    gMenuInputRepeatTimers[0] = 0;
                    playerIndex = 0;
                    count = gPlayerCount;
                    if ((s32)count > 0) {
                        player = gRacePlayers;
                        while (1) {
                            player->menuState = 9;
                            player += 1;
                            if (!((u32)player < (u32)(gRacePlayers + count))) {
                                break;
                            }
                        }
                    }
                }
            }
            if ((gRacePlayers[0].menuState == 1) && (gCourseSelectModeSelection == 1)) {
                setCurrentGameTaskCallback(updateCourseSelectUnlockCourseList, 0);
                gCurrentGameTask->screenState = 0;
            }
        } else {
            firstPromptRow = 2;
            if (gMenuChoicePromptState[0] == 9) {
                gCourseSelectSelectionTimers[0] = (u8)gCourseSelectSelectionTimers[0] + 1;
            } else {
                gCourseSelectSelectionTimers[0] = 0;
            }
            if ((
                    gMenuChoicePromptState[0] >=
                    firstPromptRow
                ) &&
                (gMenuChoicePromptState[0] < 5)) {
                if (!(gPlayerInputHeld[0] & 0x10800) && !(gPlayerInputHeld[0] & 0x20400)) {
                    gMenuInputRepeatTimers[0] = 0;
                }
                pressed = gPlayerInputPressed[0];
                if ((pressed & 0x10800) ||
                    ((gMenuInputHeld & 0x10800) && ((s32)(u16)gMenuInputRepeatTimers[0] >= 9) &&
                     ((u16)gMenuInputRepeatTimers[0] & 1))) {
                    repeat = (u16)gMenuInputRepeatTimers[0];
                    if (repeat == 0) {
                        repeat += 1;
                    }
                    gMenuInputRepeatTimers[0] = (s16)repeat;
                    if (gMenuChoicePromptState[0] >= 3) {
                        gMenuChoicePromptState[0] -= 1;
                        enqueueSoundEffect(0x19, 0x32);
                        playerIndex = 0;
                        gCourseSelectPurchaseMessageState = 0;
                    }
                } else {
                    repeat = (u16)gMenuInputRepeatTimers[0];
                    if ((pressed & 0x20400) ||
                        ((gMenuInputHeld & 0x20400) && ((s32)repeat >= 9) && (repeat & 1))) {
                        if (repeat == 0) {
                            repeat += 1;
                        }
                        gMenuInputRepeatTimers[0] = (s16)repeat;
                        if (gMenuChoicePromptState[0] < (gCourseSelectExtraCourseCount0 + 1)) {
                            gMenuChoicePromptState[0] += 1;
                            enqueueSoundEffect(0x19, 0x32);
                            playerIndex = 0;
                            gCourseSelectPurchaseMessageState = 0;
                        }
                    }
                }
                if ((u16)gMenuInputRepeatTimers[0] != 0) {
                    repeat = (u16)gMenuInputRepeatTimers[0] + 1;
                    gMenuInputRepeatTimers[0] = repeat;
                    if (repeat == 0xFFFF) {
                        gMenuInputRepeatTimers[0] = 0xA;
                    }
                }
                gCourseSelectSelectedCourseId =
                    ((*gCourseSelectHasExtraCourse == 1) && (gCharacterSelectHighlightedRosterIndices[0] == 3))
                        ? gCourseSelectExtraCourseIds[0][gMenuChoicePromptState[0] - 2]
                        : ((gMenuChoicePromptState[0] * 3) + ((s32)gCourseSelectSelectedCourseId % 3)) - 6;
                pressed = gPlayerInputPressed[0];
                if (pressed & 0x4000) {
                    enqueueSoundEffect(0x18, 0x32);
                    gCourseSelectPurchaseMessageState = 0;
                    gCourseSelectSelectedCourseId = gCourseSelectCourseIds[0][gCharacterSelectHighlightedRosterIndices[0]];
                    gMenuInputRepeatTimers[0] = 0;
                    playerIndex = 0;
                    gMenuChoicePromptState[0] += 3;
                } else if ((pressed & 0x8000) || (pressed & 0x1000)) {
                    if (gGameSaveDataBuffer[0].courseUnlockStates[gCourseSelectSelectedCourseId] == -1) {
                        if ((u32)gRacePlayers[0].money >= (u32)gCourseUnlockPrices[gCourseSelectSelectedCourseId]) {
                            enqueueSoundEffect(0x49, 0x32);
                            playerIndex = 0;
                            if ((*gCourseSelectHasExtraCourse == 1) && (gCharacterSelectHighlightedRosterIndices[0] == 3)) {
                                purchasedCourse = gCourseSelectExtraCourseIds[0][gMenuChoicePromptState[0] - 2];
                            } else {
                                purchasedCourse =
                                    (((gMenuChoicePromptState[0] * 3) + ((s32)gCourseSelectSelectedCourseId % 3)) - 6) &
                                    0xFF;
                            }
                            gRacePlayers[0].selectionUnlockState =
                                gGameSaveDataBuffer[0].courseUnlockStates[purchasedCourse];
                            gCourseSelectSelectedCourseId = purchasedCourse;
                            if (gCourseSelectSlideStates[0] == 0) {
                                gCourseSelectIncomingModelAngle0 = 0;
                                gCourseSelectIncomingModelState0 = 7;
                            } else {
                                gCourseSelectOutgoingModelAngle0 = 0;
                                gCourseSelectOutgoingModelState0 = 7;
                            }
                        } else if (gCourseSelectPurchaseMessageState == 0) {
                            enqueueSoundEffect(0x47, 0x32);
                            playerIndex = 0;
                            gCourseSelectPurchaseMessageState = 1;
                        }
                    } else if (gCourseSelectPurchaseMessageState == 0) {
                        gCourseSelectPurchaseMessageState = firstPromptRow;
                    }
                }
            }
        }
        if ((gCourseSelectIncomingModelState0 == 7) || (gCourseSelectOutgoingModelState0 == 7)) {
            gCurrentGameTask->timer = 0;
            setCurrentGameTaskCallback(updateCourseSelectPurchasePrompt, 0);
            createCallbackTask((CallbackTaskCallback)initCourseUnlockPurchasePrompt, 0, 0x64);
        }
        transition = (u8)D_800EC9C0;
    } else {
        D_800EC9C0 = (u8)D_800EC9C0 + 1;
        transition = (u8)D_800EC9C0;
        if ((s32)transition >= 0x22) {
            D_800EC9C0 = 0;
            transition = 0;
        }
    }
    if (transition == 0x1B) {
        gCurrentGameTask->fade = 1;
        gRacePlayers[0].menuState = 0;
        setCurrentGameTaskCallback(updateCourseSelectModeMenu, 0);
        gRacePlayers[playerIndex].menuSelection =
            gCourseSelectCourseIds[0][gCharacterSelectHighlightedRosterIndices[0]];
        if (gMenuFlowState == 1) {
            gCourseSelectSelectedCourseId = 0;
        }
        gMenuFlowState = 0;
        D_800EC9C0 = 0;
        gMenuChoicePromptState[0] = 0;
        gCourseSelectSlideStates[0] = 0;
        gCourseSelectPreviewModelState0 = 0;
        gCourseSelectIncomingModelState0 = 0;
        gCourseSelectOutgoingModelState0 = 0;
        gCourseSelectIncomingModelTimer0 = 0;
        gCourseSelectOutgoingModelTimer0 = 0;
        gCourseSelectIncomingModelAngle0 = 0;
        gCourseSelectOutgoingModelAngle0 = 0;
        gCourseSelectExtraCourseCount0 = 0;
        gCourseSelectPreviewModelState1 = 0;
        gCourseSelectIncomingModelState1 = 0;
        gCourseSelectOutgoingModelState1 = 0;
        gCourseSelectIncomingModelTimer1 = 0;
        gCourseSelectOutgoingModelTimer1 = 0;
        gCourseSelectIncomingModelAngle1 = 0;
        gCourseSelectOutgoingModelAngle1 = 0;
        gCourseSelectExtraCourseCount1 = 0;
        gCourseSelectPreviewModelState2 = 0;
        gCourseSelectIncomingModelState2 = 0;
        gCourseSelectOutgoingModelState2 = 0;
        gCourseSelectIncomingModelTimer2 = 0;
        gCourseSelectOutgoingModelTimer2 = 0;
        gCourseSelectIncomingModelAngle2 = 0;
        gCourseSelectOutgoingModelAngle2 = 0;
        gCourseSelectExtraCourseCount2 = 0;
        gCourseSelectPreviewModelState3 = 0;
        gCourseSelectIncomingModelState3 = 0;
        gCourseSelectOutgoingModelState3 = 0;
        gCourseSelectIncomingModelTimer3 = 0;
        gCourseSelectOutgoingModelTimer3 = 0;
        gCourseSelectIncomingModelAngle3 = 0;
        gCourseSelectOutgoingModelAngle3 = 0;
        gCourseSelectExtraCourseCount3 = 0;
        COURSE_SELECT_STATUS_LAYOUT.submenuState = 0;
        COURSE_SELECT_STATUS_LAYOUT.cursorState = 0;
        COURSE_SELECT_STATUS_LAYOUT.cursorValue = 0;
        COURSE_SELECT_STATUS_LAYOUT.purchaseMessageState = 0;
        COURSE_SELECT_STATUS_LAYOUT.extraCourseColumnState = 0;
    }
    for (playerIndex = 0; playerIndex < (s32)gPlayerCount; playerIndex++) {
        (gCurrentMenuCameraObject = &D_801121E0[playerIndex])->update();
    }
    updateCallbackTasks();
}
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
                if (gCourseSelectSlideStates[0] == 0) {
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
            if (gCourseSelectSlideStates[0] == 0) {
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
        gCourseSelectSelectionTimers[0] = ((u8)gCourseSelectSelectionTimers[0]) + 1;
    } else {
        gCourseSelectSelectionTimers[0] = 0;
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
            if (!gCourseSelectSlideStates[0]) {
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
        gCourseSelectSelectionTimers[0]++;
    } else {
        gCourseSelectSelectionTimers[0] = 0;
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
        if (gCourseSelectSlideStates[0] == 0) {
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
        gCourseSelectSelectionTimers[0] = (u8)gCourseSelectSelectionTimers[0] + 1;
    } else {
        gCourseSelectSelectionTimers[0] = playerIndex;
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
            if (gCourseSelectSlideStates[0] == playerIndex) {
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

// Matched by queueram via decomp.me scratch GgqNl.
void updateCourseSelectCourseDetailsMenu(void) {
    s32 i;
    u8 oldSelection;
    s32 heldUp;
    u16 repeat;

    if (gCourseSelectSubmenuState >= 2) {
        heldUp = (gPlayerInputHeld[0]) & (STICK_UP | U_JPAD);
        oldSelection = gCourseDetailsMenuSelection;
        if (!heldUp && !(gPlayerInputHeld[0] & (STICK_DOWN | D_JPAD))) {
            gMenuInputRepeatTimers[0] = 0;
        }
        if ((gPlayerInputPressed[0] & (STICK_UP | U_JPAD)) ||
            (heldUp &&
             (gMenuInputRepeatTimers[0] >= 0xB) &&
            ((gMenuInputRepeatTimers[0] % 3) == 0))) {
            if (gMenuInputRepeatTimers[0] == 0) {
                gMenuInputRepeatTimers[0]++;
            }
            if (gCourseDetailsMenuSelection > 0) {
                gCourseDetailsMenuSelection--;
            }
        } else {
            if ((gPlayerInputPressed[0] & (STICK_DOWN | D_JPAD)) ||
                ((gPlayerInputHeld[0] & (STICK_DOWN | D_JPAD)) &&
                 (gMenuInputRepeatTimers[0] >= 0xB) && ((gMenuInputRepeatTimers[0] % 3) == 0))) {
                if (gMenuInputRepeatTimers[0] == 0) {
                    gMenuInputRepeatTimers[0]++;
                }
                if (gCourseDetailsMenuSelection < 9) {
                    gCourseDetailsMenuSelection++;
                }
            } else if (gPlayerInputPressed[0] & (STICK_LEFT | L_JPAD)) {
                gMenuExitSelection = 1;
            }
        }
        if (gMenuInputRepeatTimers[0]) {
            gMenuInputRepeatTimers[0]++;
            if (gMenuInputRepeatTimers[0] == 0xFFFF) {
                gMenuInputRepeatTimers[0] = 0xC;
            }
        }
        if (oldSelection != gCourseDetailsMenuSelection) {
            enqueueSoundEffect(0x19, 0x32);
        }
        if ((gPlayerInputPressed[0] & B_BUTTON)) {
            gMenuExitSelection = 2;
        }
        repeat = 1;
        if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON) ||
            ((gMenuExitSelection) != 0)) {
            if ((gCourseDetailsMenuSelection >= 7) || (gMenuExitSelection != 0)) {
                enqueueSoundEffect(0x18, 0x32);
            }
            if (((gCourseDetailsMenuSelection == 7) || (gMenuExitSelection == 1)) &&
                (gMenuExitSelection != 2)) {
                gCourseSelectSubmenuState = 3;
                if (gMenuExitSelection == repeat) {
                    gCourseDetailsCloseFromBack = repeat;
                }
                gMenuExitSelection = 0;
                setCurrentGameTaskCallback(waitCourseSelectRecordsClose, 0);
            } else {
                if ((gCourseDetailsMenuSelection == 8) || (gMenuExitSelection == 2)) {
                    setCurrentGameTaskCallback(returnToCourseSelectUnlockCourseList, 0);
                    gMenuExitSelection = 0;
                    gCurrentGameTask->screenState = 3;
                } else if (gCourseDetailsMenuSelection == 9) {
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

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void waitCourseSelectRecordsClose(void) {
    RaceCamera *var_s1;
    s32 var_s0;

    if (gCourseSelectSubmenuState == 2) {
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
        if (gCourseSelectSlideStates[0] == 0) {
            COURSE_SELECT_STATUS_LAYOUT.core.playerOneCourseDecided = 1;
        } else {
            COURSE_SELECT_STATUS_LAYOUT.core.playerTwoCourseDecided = 1;
        }
        COURSE_SELECT_STATUS_LAYOUT.submenuState = 0;
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
        gCourseSelectSlideStates[0] = 0;
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

        COURSE_SELECT_STATUS_LAYOUT.cursorState = 0;
        COURSE_SELECT_STATUS_LAYOUT.cursorValue = 0;
        COURSE_SELECT_STATUS_LAYOUT.submenuState = 0;
        COURSE_SELECT_STATUS_LAYOUT.purchaseMessageState = 0;
        COURSE_SELECT_STATUS_LAYOUT.extraCourseColumnState = 0;
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
    gCourseSelectViewportSyncState = temp[0x3F] + 1;
    temp[0x3F] = gCourseDetailsPreviewCourseTiles[(u8) gCourseDetailsPreviewPage * 7 + (u8) gCourseDetailsMenuSelection];
    gCourseSelectSubmenuState = 6;
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
        gCourseSelectSubmenuState = 2;
 gCourseSelectViewportSyncState = 0; } var_s0 = D_801121E0; do { (gCurrentMenuCameraObject = var_s0)->update(); var_s0 += 1; } while (var_s0 != (&D_80112340)); updateCallbackTasks();
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
