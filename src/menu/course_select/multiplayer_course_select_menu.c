#include "game/race/race_state.h"
#include "common.h"
#include "game/save_data.h"
#include "assets.h"
#include "game/engine/asset_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/character_select/character_select_menu.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/menu/course_select/course_select_menu.h"
#include "game/menu/course_select/course_select_ui.h"
#include "game/menu/course_select/multiplayer_course_select_menu.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/engine/game_task_scheduler.h"
#include "game/engine/relocatable_heap.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/race/camera/race_camera.h"
#include "game/race/effects/race_start_transition.h"
#include "game/audio/sound_manager.h"
#include "game/engine/system_runtime.h"
#include "game/menu/race_setup/race_setup_ui.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"

extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

#define MULTIPLAYER_COURSE_SELECT_PLAYER_RECORD_COUNT 4

s8 gMultiplayerCourseSelectDefaultCourseIds[MULTIPLAYER_COURSE_SELECT_DEFAULT_COURSE_COUNT] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
};
s16 gCourseSelectColumnSoundEffects[MULTIPLAYER_COURSE_SELECT_COLUMN_COUNT] = {
    0x56, 0x55, 0x54, 0x51, 0, 0, 0, 0, 0, 0,
};

extern s16 gMenuFadeAlpha;
extern u8 D_800EC9C0;
extern s8 D_800EC9E5;
extern s32 D_8010ADE0;
extern s32 D_8010ADE4;
extern s32 gPlayerInputHeld[];
extern s32 gPlayerInputPressed[];
extern RaceCamera D_801121E0[];
extern RaceCamera D_801124A0;
extern s32 gMenuFlowState;

void initMultiplayerCourseSelectMenu(void) {
    s32 j;
    s32 k;
    s32 l;
    s32 i;
    s32 m;
    s32 sum;

    if (gRaceSplitscreenMode == 1) {
        requestMusicSequenceBank(2);
    }
    resetRaceCameras();
    resetAllViewports();

    switch (gPlayerCount) {
        case 1:
            configureViewport(0, 0xE8, 0x78, 0x90, 0xD0, 0xA0, 0xF0, 0.6666666865f);
            i = 0xA40000;
            break;
        case 2:
            configureViewport(0, 0xE4, 0x4A, 0x84, 0x54, 0xA8, 0x74, 1.448275805f);
            configureViewport(1, 0xE4, 0xAE, 0x84, 0x54, 0xA8, 0x74, 1.448275805f);
            i = 0x894000;
            break;
        case 3:
        case 4:
            configureViewport(0, 0x78, 0x44, 0x44, 0x40, 0x60, 0x60, 1.333333373f);
            configureViewport(1, 0x78, 0xA8, 0x44, 0x40, 0x60, 0x60, 1.333333373f);
            configureViewport(2, 0x104, 0x44, 0x44, 0x40, 0x60, 0x60, 1.333333373f);
            configureViewport(3, 0x104, 0xA8, 0x44, 0x40, 0x60, 0x60, 1.333333373f);
            i = 0x894000;
            break;
        default:
            break;
    }

    for (l = 0; l < 4; l++) {
        D_801121E0[l].update = updateMenuCameraObjectLookAtOriginCallback;
        D_801121E0[l].distance = i;
        gCourseSelectHasExtraCourse[l] = 0;
        gCourseSelectSelectedRows[l] = 0;
    }
    gFramebufferRenderInterval.value = 0;
    D_800EC9E5 = 0;
    gCourseSelectPurchaseFlowActive = 0;
    gCourseSelectInputLocked = 0;
    gCurrentGameTask->callbackData0 = 0xFF;
    gMenuFadeAlpha = gCurrentGameTask->callbackData0;

    LOAD_ASSET(_5CBA80, 0x21);
    LOAD_ASSET(_593D10, 0x22);
    LOAD_ASSET(_598A70, 0x23);
    LOAD_ASSET(_59AAA0, 0x24);
    LOAD_ASSET(_5A1ED0, 0x25);
    LOAD_ASSET(_59DFE0, 0x26);

    gAssetHandles[0xC] = allocRelocatableHeapBlock(ASSET_SIZE(_14B450));
    DMA_ASSET(_14B450, (void *)getRelocatableHeapBlockBase(gAssetHandles[0xC]), ASSET_SIZE(_14B450));
    LOAD_ASSET(_1EF530, 0xD);
    LOAD_ASSET(_245A80, 0x1F);
    initCallbackTaskScheduler(0);
    createCallbackTask((CallbackTaskCallback)initMenuIconTilemapSpriteActor, 0, 0x63);

    D_800EC9C0 = 0;
    gActiveMenuTask = 0;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    gMenuFlowState = 0;
    gCourseSelectViewportSyncState = 0;
    gCourseSelectModeSelection = 0;
    gCurrentGameTask->callbackData1 = 0;
    gCurrentGameTask->callbackData2 = 0;
    setCurrentGameTaskCallback(updateMultiplayerCourseSelectMenu, 0);

    for (i = 0; i < 4; i++) {
        gRacePlayers[i].menuState = 0;
    }

    for (i = 0; i < gPlayerCount; i++) {
        gCourseSelectSelectionTimers[i] = 0;
        gCourseSelectColumnCounts[i] = 0;
        gCourseSelectSlideStates[i] = 0;
        gMenuChoicePromptState[i] = 0;
        gMenuInputRepeatTimers[i] = 0;
        gCourseSelectVerticalOffsets[i] = 0;
        gCourseSelectHorizontalOffsets[i] = 0;

        for (j = 0; j < 3; j++) {
            sum = 0;
            for (k = 0; k < 3; k++) {
                sum += gGameSaveDataBuffer[i].courseUnlockStates[k * 3 + j];
            }
            if (sum != -3) {
                gCourseSelectColumnUnlocked[i][j] = 1;
            } else {
                gCourseSelectColumnUnlocked[i][j] = 0;
            }
        }

        k = 0;
        for (j = 9; j < 12; j++) {
            k += gGameSaveDataBuffer[i].courseUnlockStates[j];
        }
        if (k >= -2) {
            gCourseSelectColumnCounts[i] = 4;
        }
        if (gCourseSelectColumnCounts[i] == 4) {
            gCourseSelectHasExtraCourse[i] = 1;
        }
        for (j = 0; j < 3; j++) {
            gCourseSelectCourseIds[i][j] = j;
        }

        if (gCourseSelectHasExtraCourse[i] == 1) {
            for (j = 9; j < 12; j++) {
                if (gGameSaveDataBuffer[i].courseUnlockStates[j] != -1) {
                    gCourseSelectCourseIds[i][3] = j;
                    break;
                }
            }

            k = 0;
            for (j = 9; j < 12; j++) {
                gCourseSelectExtraCourseIds[i][k] = 0;
                if (gGameSaveDataBuffer[i].courseUnlockStates[j] != -1) {
                    gCourseSelectExtraCourseIds[i][k] = j;
                    k++;
                }
            }
        } else {
            gCourseSelectCourseIds[i][3] = 0;
        }
    }

    for (j = 0; j < gPlayerCount; j++) {
        if ((gRacePlayers[j].menuSelection >= 9) && (gRacePlayers[j].menuSelection < 12)) {
            if (gRacePlayers[j].selectedCharacterId == 5) {
                k = 0;
            } else {
                k = 3;
            }
        } else {
            if (COURSE_SELECT_STATUS_LAYOUT.extraCourseColumnState == 1) {
                k = 0;
                COURSE_SELECT_STATUS_LAYOUT.extraCourseColumnState = 0;
            } else {
                k = gRacePlayers[j].menuSelection % 3;
            }
        }

        gCharacterSelectHighlightedRosterIndices[j] = k;

        if (COURSE_SELECT_STATUS_LAYOUT.extraCourseColumnState == 1) {
            k--;
        }

        if (gRacePlayers[j].selectedCharacterId == 5) {
            gCourseSelectSelectedRows[j] = 0;
        } else if (gRacePlayers[j].menuSelection < 9) {
            gCourseSelectSelectedRows[j] = gRacePlayers[j].menuSelection / 3;
        } else {
            if (gRacePlayers[j].menuSelection >= 12) {
                gCourseSelectSelectedRows[j] = 0;
            }

            if ((gRacePlayers[j].menuSelection >= 9) && (gRacePlayers[j].menuSelection < 12)) {
                for (m = 0; m < 3; m++) {
                    if (gCourseSelectExtraCourseIds[j][m] == gRacePlayers[j].menuSelection) {
                        gCourseSelectSelectedRows[j] = m;
                        break;
                    }
                }
            }
        }

        gRacePlayers[j].menuSelection = gCourseSelectCourseIds[j][k];

        COURSE_SELECT_STATUS_LAYOUT.core.unk0[j] = 0;
        COURSE_SELECT_STATUS_LAYOUT.core.unk4Array[j] = 0;
        COURSE_SELECT_STATUS_LAYOUT.core.unk8Array[j] = 0;
        COURSE_SELECT_STATUS_LAYOUT.core.unkCArray[j] = 0;
        COURSE_SELECT_STATUS_LAYOUT.core.unk10Array[j] = 0;
        COURSE_SELECT_STATUS_LAYOUT.core.unk14[j] = 0;
        COURSE_SELECT_STATUS_LAYOUT.core.unk1C[j] = 0;
        COURSE_SELECT_STATUS_LAYOUT.core.unk24[j] = 0;
    }

    COURSE_SELECT_STATUS_LAYOUT.cursorState = 0;
    COURSE_SELECT_STATUS_LAYOUT.cursorValue = 0;
    COURSE_SELECT_STATUS_LAYOUT.purchaseMessageState = 0;
}

// Matched by inspectredc via decomp.me scratch J668c.
void updateMultiplayerCourseSelectMenu(void) {
    s32 input;
    s32 tempTimer;
    u8 finishedPlayerCount;
    s32 count;
    s32 maxVisiblePlayerCount;
    s32 playerIndex;
    u8 maxColumn;
    u8 extraColumn;
    u8 oldColumn;
    RaceCamera *camera;
    s32 pad[3];
    s32 i;

    count = 0;
    if (gCurrentGameTask->callbackData0 != 0) {
        gCurrentGameTask->callbackData0 = stepMenuFadeAlpha((s16)gCurrentGameTask->callbackData0, 0x24, 0);
        if (gCurrentGameTask->callbackData0 == 0) {
            if (gPlayerCount == 1) {
                createCallbackTask((CallbackTaskCallback)initCourseSelectCourseIconList, 0, 0x63);
            } else {
                createCallbackTask((CallbackTaskCallback)initCourseSelectPlayerPanels, 0, 0x62);
                createCallbackTask((CallbackTaskCallback)initCourseSelectCompletePanels, 0, 0x63);
            }
            D_8010ADE8 = createCallbackTask((CallbackTaskCallback)initCourseSelectExtraCourseIconList, 0, 0x61);
        }
    } else {
        finishedPlayerCount = 0;
        for (i = 0; i < gPlayerCount; i++) {
            if (gRacePlayers[i].menuState == 9) {
                finishedPlayerCount++;
            }
        }
        if (finishedPlayerCount == gPlayerCount) {
            for (playerIndex = 0; playerIndex < gPlayerCount; playerIndex++) {
                if (gCourseSelectHorizontalOffsets[playerIndex] != 0 || gMenuChoicePromptState[playerIndex] == 1 ||
                    gMenuChoicePromptState[playerIndex] == 9 || gMenuChoicePromptState[playerIndex] >= 5) {
                    count++;
                }
            }
            if (count == 0) {
                if (gPlayerCount < 3) {
                    maxVisiblePlayerCount = gPlayerCount;
                } else {
                    maxVisiblePlayerCount = 4;
                }
                playerIndex = 0;
                while (playerIndex < maxVisiblePlayerCount) {
                    gRacePlayers[playerIndex].menuState = 3;
                    playerIndex++;
                }
                D_800EC9C0 = 0xF;
                gMenuFlowState = 1;
            }
        } else if (D_800EC9C0 == 0) {
            for (playerIndex = 0; playerIndex < gPlayerCount; playerIndex++) {
                if (gMenuChoicePromptState[playerIndex] == 0) {
                    if (((gCourseSelectStatus.unk0[playerIndex] == 1) && (gRacePlayers[playerIndex].menuState == 0)) &&
                        (!(gCourseSelectSlideStates[playerIndex] & 1))) {
                        if (gCourseSelectInputLocked == 1) {
                            gRacePlayers[playerIndex].menuState = 9;
                        } else {
                            if ((gRacePlayers[playerIndex].selectedCharacterId == 5) ||
                                (gCourseSelectHasExtraCourse[playerIndex] == 0)) {
                                maxColumn = 3;
                            } else {
                                maxColumn = 4;
                            }
                            extraColumn = maxColumn;
                            if (gPlayerCount >= 2) {
                                maxColumn--;
                            }
                            oldColumn = gCharacterSelectHighlightedRosterIndices[playerIndex];
                            if ((!(gPlayerInputHeld[playerIndex] & 0x10800)) &&
                                (!(gPlayerInputHeld[playerIndex] & 0x20400))) {
                                // FAKE
                                if (playerIndex)
                                    ;
                                gMenuInputRepeatTimers[playerIndex] = 0;
                            }
                            if ((gPlayerInputPressed[playerIndex] & 0x10800) ||
                                ((gPlayerInputHeld[playerIndex] & 0x10800) &&
                                 ((tempTimer = gMenuInputRepeatTimers[playerIndex]) >= 9))) {
                                if (gMenuInputRepeatTimers[playerIndex] == 0) {
                                    gMenuInputRepeatTimers[playerIndex]++;
                                }
                                if (gCharacterSelectHighlightedRosterIndices[playerIndex] > 0) {
                                    gCharacterSelectHighlightedRosterIndices[playerIndex]--;
                                    enqueueSoundEffect(0x19, 0x32);
                                    gCourseSelectSelectedRows[playerIndex] = 0;
                                }
                            } else if ((gPlayerInputPressed[playerIndex] & 0x20400) ||
                                       ((gPlayerInputHeld[playerIndex] & 0x20400) &&
                                        ((tempTimer = gMenuInputRepeatTimers[playerIndex]) >= 9))) {
                                if (gMenuInputRepeatTimers[playerIndex] == 0) {
                                    gMenuInputRepeatTimers[playerIndex]++;
                                }
                                if (gCharacterSelectHighlightedRosterIndices[playerIndex] < maxColumn) {
                                    gCharacterSelectHighlightedRosterIndices[playerIndex]++;
                                    enqueueSoundEffect(0x19, 0x32);
                                    gCourseSelectSelectedRows[playerIndex] = 0;
                                }
                            }
                            if (gMenuInputRepeatTimers[playerIndex] != 0) {
                                gMenuInputRepeatTimers[playerIndex]++;
                                if (gMenuInputRepeatTimers[playerIndex] == 0xFFFF) {
                                    gMenuInputRepeatTimers[playerIndex] = 0xA;
                                }
                            }
                            if (((extraColumn != gCharacterSelectHighlightedRosterIndices[playerIndex]) &&
                                 (oldColumn != extraColumn)) &&
                                (oldColumn != gCharacterSelectHighlightedRosterIndices[playerIndex])) {
                                gCourseSelectSlideStates[playerIndex]++;
                                if (oldColumn < gCharacterSelectHighlightedRosterIndices[playerIndex]) {
                                    gCourseSelectHorizontalOffsets[playerIndex] = -0x800000;
                                } else {
                                    gCourseSelectHorizontalOffsets[playerIndex] = 0x800000;
                                }
                            }
                            if (extraColumn == gCharacterSelectHighlightedRosterIndices[playerIndex]) {
                                gRacePlayers[playerIndex].menuSelection =
                                    gCourseSelectCourseIds[playerIndex][extraColumn - 1];
                                gCourseSelectExtraCourseColumnState = 1;
                            } else {
                                gRacePlayers[playerIndex].menuSelection =
                                    gCourseSelectCourseIds[playerIndex]
                                                          [gCharacterSelectHighlightedRosterIndices[playerIndex]];
                                gCourseSelectExtraCourseColumnState = 0;
                            }
                            if (gCourseSelectHorizontalOffsets[playerIndex] == 0 &&
                                ((gPlayerInputPressed[playerIndex] & 0x1000) ||
                                 ((gPlayerInputPressed[playerIndex] & 0x8000)))) {
                                if ((gPlayerCount == 1) && (maxColumn == gCharacterSelectHighlightedRosterIndices[0])) {
                                    enqueueSoundEffect(0x18, 0x32);
                                    gRacePlayers[0].menuState = 9;
                                    gCourseSelectInputLocked = 1;
                                    gMenuFlowState = 1;
                                } else if ((gRacePlayers[playerIndex].menuSelection >= 9) ||
                                           ((gRacePlayers[playerIndex].menuSelection < 9) &&
                                            (gCourseSelectColumnUnlocked
                                                 [playerIndex][gCharacterSelectHighlightedRosterIndices[playerIndex]] !=
                                             0))) {
                                    enqueueSoundEffect(0x18, 0x32);
                                    gRacePlayers[playerIndex].menuState = 1;
                                    if (gRacePlayers[playerIndex].selectedCharacterId == 5) {
                                        enqueueSoundEffect(0x40, 0x32);
                                    } else {
                                        enqueueSoundEffect(
                                            gCourseSelectColumnSoundEffects
                                                [gCharacterSelectHighlightedRosterIndices[playerIndex]],
                                            0x32
                                        );
                                    }
                                    gMenuChoicePromptState[playerIndex] = 9;
                                    gCourseSelectSelectionTimers[playerIndex] = 1;
                                    gMenuInputRepeatTimers[playerIndex] = 0;
                                }
                            } else {
                                if (gPlayerInputPressed[playerIndex] & 0x4000) {
                                    enqueueSoundEffect(0x18, 0x32);
                                    gRacePlayers[playerIndex].menuState = 9;
                                    gCourseSelectInputLocked = 1;
                                }
                            }
                        }
                    }
                } else {
                    if (gMenuChoicePromptState[playerIndex] == 9) {
                        gCourseSelectSelectionTimers[playerIndex]++;
                    } else {
                        gCourseSelectSelectionTimers[playerIndex] = 0;
                    }
                    if ((gMenuChoicePromptState[playerIndex] >= 2) && (gMenuChoicePromptState[playerIndex] < 5)) {
                        if (gCourseSelectInputLocked == 1) {
                            gRacePlayers[playerIndex].menuState = 9;
                        } else {
                            if ((!(gPlayerInputHeld[playerIndex] & 0x10800)) &&
                                (!(gPlayerInputHeld[playerIndex] & 0x20400))) {
                                gMenuInputRepeatTimers[playerIndex] = 0;
                            }
                            if ((gPlayerInputPressed[playerIndex] & 0x10800) ||
                                (((gPlayerInputHeld[playerIndex] & 0x10800) &&
                                  (gMenuInputRepeatTimers[playerIndex] >= 9)) &&
                                 (gMenuInputRepeatTimers[playerIndex] & 1))) {
                                if (gMenuInputRepeatTimers[playerIndex] == 0) {
                                    gMenuInputRepeatTimers[playerIndex]++;
                                }
                                if (gMenuChoicePromptState[playerIndex] >= 3) {
                                    gMenuChoicePromptState[playerIndex]--;
                                    enqueueSoundEffect(0x19, 0x32);
                                }
                            } else if ((gPlayerInputPressed[playerIndex] & 0x20400) ||
                                       (((gPlayerInputHeld[playerIndex] & 0x20400) &&
                                         (gMenuInputRepeatTimers[playerIndex] >= 9)) &&
                                        (gMenuInputRepeatTimers[playerIndex] & 1))) {
                                if (gMenuInputRepeatTimers[playerIndex] == 0) {
                                    gMenuInputRepeatTimers[playerIndex]++;
                                }
                                if (gMenuChoicePromptState[playerIndex] <
                                    (gCourseSelectStatus.unk24[playerIndex] + 1)) {
                                    gMenuChoicePromptState[playerIndex]++;
                                    enqueueSoundEffect(0x19, 0x32);
                                }
                            }
                            if (gMenuInputRepeatTimers[playerIndex] != 0) {
                                gMenuInputRepeatTimers[playerIndex]++;
                                if (gMenuInputRepeatTimers[playerIndex] == 0xFFFF) {
                                    gMenuInputRepeatTimers[playerIndex] = 0xA;
                                }
                            }
                            if ((gCourseSelectHasExtraCourse[playerIndex] == 1) &&
                                (gCharacterSelectHighlightedRosterIndices[playerIndex] == 3)) {
                                gRacePlayers[playerIndex].menuSelection =
                                    gCourseSelectExtraCourseIds[playerIndex][gMenuChoicePromptState[playerIndex] - 2];
                            } else {
                                gRacePlayers[playerIndex].menuSelection =
                                    ((gRacePlayers[playerIndex].menuSelection % 3) +
                                     (gMenuChoicePromptState[playerIndex] * 3)) -
                                    6;
                            }
                            if ((gPlayerInputPressed[playerIndex] & 0x8000) ||
                                (gPlayerInputPressed[playerIndex] & 0x1000)) {
                                enqueueSoundEffect(0x18, 0x32);
                                gRacePlayers[playerIndex].menuState = 2;
                                gCourseSelectPurchaseFlowActive = 1;
                                if ((gCourseSelectHasExtraCourse[playerIndex] == 1) &&
                                    (gCharacterSelectHighlightedRosterIndices[playerIndex] == 3)) {
                                    gRacePlayers[playerIndex].menuSelection =
                                        gCourseSelectExtraCourseIds[playerIndex]
                                                                   [gMenuChoicePromptState[playerIndex] - 2];
                                } else {
                                    input = (gRacePlayers[playerIndex].selectedCharacterId == 5) ? (0xC) : (0);
                                    gRacePlayers[playerIndex].menuSelection =
                                        (((gRacePlayers[playerIndex].menuSelection % 3) +
                                          (gMenuChoicePromptState[playerIndex] * 3)) +
                                         input) -
                                        6;
                                }
                                if (gRacePlayers[playerIndex].menuSelection >= 9) {
                                    gRacePlayers[playerIndex].selectionUnlockState = 0;
                                } else {
                                    gRacePlayers[playerIndex].selectionUnlockState =
                                        gGameSaveDataBuffer[playerIndex]
                                            .courseUnlockStates[gRacePlayers[playerIndex].menuSelection];
                                }
                                gMenuChoicePromptState[playerIndex] += 3;
                                if (gCourseSelectSlideStates[playerIndex] == 0) {
                                    gCourseSelectStatus.unkCArray[playerIndex] = 1;
                                    gCourseSelectStatus.unk14[playerIndex] = 0;
                                    gCourseSelectStatus.unk4Array[playerIndex] = 3;
                                } else {
                                    gCourseSelectStatus.unk10Array[playerIndex] = 1;
                                    gCourseSelectStatus.unk1C[playerIndex] = 0;
                                    gCourseSelectStatus.unk8Array[playerIndex] = 3;
                                }
                            } else if (gPlayerInputPressed[playerIndex] & 0x4000) {
                                enqueueSoundEffect(0x18, 0x32);
                                gMenuInputRepeatTimers[playerIndex] = 0;
                                gMenuChoicePromptState[playerIndex] += 3;
                                gRacePlayers[playerIndex].menuSelection =
                                    gCourseSelectCourseIds[playerIndex]
                                                          [gCharacterSelectHighlightedRosterIndices[playerIndex]];
                            }
                        }
                    }
                }
                if (gRacePlayers[playerIndex].menuState == 2) {
                    if (gCourseSelectInputLocked == 1) {
                        gRacePlayers[playerIndex].menuState = 9;
                    } else if (gPlayerInputPressed[playerIndex] & 0x4000) {
                        enqueueSoundEffect(0x18, 0x32);
                        gMenuInputRepeatTimers[playerIndex] = 0;
                        gCourseSelectPurchaseFlowActive = 0;
                        gRacePlayers[playerIndex].menuState = 1;
                        gMenuChoicePromptState[playerIndex] -= 3;
                        if (gCourseSelectSlideStates[playerIndex] == 0) {
                            gCourseSelectStatus.unk4Array[playerIndex] = 1;
                        } else {
                            gCourseSelectStatus.unk8Array[playerIndex] = 1;
                        }
                    } else {
                        count++;
                    }
                }
                if (count == gPlayerCount) {
                    D_800EC9C0 = 1;
                }
            }
        } else {
            D_800EC9C0++;
        }
    }
    if (D_800EC9C0 == 0x19) {
        setCurrentGameTaskCallback(fadeOutMultiplayerCourseSelectMenu, 0);
        if (gMenuFlowState == 0) {
            requestMusicSequenceStop(8);
        }
    }

    for (playerIndex = 0; playerIndex < 4; playerIndex++) {
        gCurrentMenuCameraObject = &D_801121E0[playerIndex];
        gCurrentMenuCameraObject->update();
    }

    updateCallbackTasks();
}

void fadeOutMultiplayerCourseSelectMenu(void) {
    if (gCurrentGameTask->callbackData0 != 0xFF) {
        gCurrentGameTask->callbackData0 = stepMenuFadeAlpha((s16)gCurrentGameTask->callbackData0, 0x24, 1);
        if (gCurrentGameTask->callbackData0 == 0xFF) {
            gFramebufferSwapHold = 1;
        } else {
            updateCallbackTasks();
        }
    } else {
        if (gPendingFramebufferSwapCount == 2) {
            releaseMenuAssetHandles();
            gFramebufferSwapHold = 0;
            gFramebufferRenderInterval.value = 0;
            resumeGameTask(2);
            removeGameTask(4);
        }
    }
}
