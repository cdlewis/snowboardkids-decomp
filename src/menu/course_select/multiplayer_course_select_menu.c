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
#include "game/menu/renderer/menu_renderer.h"
#include "game/race/camera/race_camera.h"
#include "game/race/effects/race_start_transition.h"
#include "game/audio/sound_manager.h"
#include "game/engine/system_runtime.h"
#include "game/menu/race_setup/race_setup_ui.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"

extern void releaseMenuAssetHandles(void);
extern void initCourseSelectPlayerPanels(void *);

extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

#define MULTIPLAYER_COURSE_SELECT_PLAYER_RECORD_COUNT 4

s8 gMultiplayerCourseSelectDefaultCourseIds[MULTIPLAYER_COURSE_SELECT_DEFAULT_COURSE_COUNT] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
};
s16 gCourseSelectColumnSoundEffects[MULTIPLAYER_COURSE_SELECT_COLUMN_COUNT] = {
    0x56, 0x55, 0x54, 0x51, 0, 0, 0, 0, 0, 0,
};

extern void n_alSeqpDelete(void);
extern s32 enqueueSoundEffect(s16 soundId, s16 volume);
extern s16 gMenuFadeAlpha;
extern u8 D_800EC9C0;
extern s8 D_800EC9E5;
extern s32 D_8010ADE0;
extern s32 D_8010ADE4;
extern s8 D_8010AE64[];
extern u8 D_8010AEA0[];
extern u8 D_8010AEA4[];
extern u8 D_8010AEA8;
extern s8 D_8010AEAC[];
extern u8 D_8010AEB0;
extern s8 D_8010AEB8[][3];
extern u8 D_8010AEC8[];
extern u8 D_8010AECC[];
extern s16 D_8010AED0;
extern s32 D_8010AED8[];
extern s32 D_8010AEE8[];
extern u8 D_8010AEF8[][4];
extern s8 D_8010AEFB[];
extern u8 gCourseSelectExtraCourseIds[RACE_PLAYER_COUNT][3];
extern u8 gMultiplayerCourseSelectExtraCourseIds[RACE_PLAYER_COUNT][3];
extern u8 gCourseSelectExtraCourseColumnState;
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
        D_8010AEA0[l] = 0;
        D_8010AEAC[l] = 0;
    }
    gFramebufferSwapDelay.value = 0;
    D_800EC9E5 = 0;
    D_8010AEA8 = 0;
    D_8010AEB0 = 0;
    gCurrentGameTask->fade = 0xFF;
    gMenuFadeAlpha = gCurrentGameTask->fade;

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
    D_8010AED0 = 0;
    gCourseSelectModeSelection = 0;
    gCurrentGameTask->timer = 0;
    gCurrentGameTask->unk20 = 0;
    setCurrentGameTaskCallback(updateMultiplayerCourseSelectMenu, 0);

    for (i = 0; i < 4; i++) {
        gRacePlayers[i].menuState = 0;
    }

    for (i = 0; i < gPlayerCount; i++) {
        D_8010AEA4[i] = 0;
        D_8010AEC8[i] = 0;
        D_8010AECC[i] = 0;
        gMenuChoicePromptState[i] = 0;
        gMenuInputRepeatTimers[i] = 0;
        D_8010AED8[i] = 0;
        D_8010AEE8[i] = 0;

        for (j = 0; j < 3; j++) {
            sum = 0;
            for (k = 0; k < 3; k++) {
                sum += gGameSaveDataBuffer[i].courseUnlockStates[k * 3 + j];
            }
            if (sum != -3) {
                D_8010AEB8[i][j] = 1;
            } else {
                D_8010AEB8[i][j] = 0;
            }
        }

        k = 0;
        for (j = 9; j < 12; j++) {
            k += gGameSaveDataBuffer[i].courseUnlockStates[j];
        }
        if (k >= -2) {
            D_8010AEC8[i] = 4;
        }
        if (D_8010AEC8[i] == 4) {
            D_8010AEA0[i] = 1;
        }
        for (j = 0; j < 3; j++) {
            D_8010AEF8[i][j] = j;
        }

        if (D_8010AEA0[i] == 1) {
            for (j = 9; j < 12; j++) {
                if (gGameSaveDataBuffer[i].courseUnlockStates[j] != -1) {
                    D_8010AEFB[i * 4] = j;
                    break;
                }
            }

            k = 0;
            for (j = 9; j < 12; j++) {
                gMultiplayerCourseSelectExtraCourseIds[i][k] = 0;
                if (gGameSaveDataBuffer[i].courseUnlockStates[j] != -1) {
                    gMultiplayerCourseSelectExtraCourseIds[i][k] = j;
                    k++;
                }
            }
        } else {
            D_8010AEFB[i * 4] = 0;
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
            if (gCourseSelectStatus.unk2E == 1) {
                k = 0;
                gCourseSelectStatus.unk2E = 0;
            } else {
                k = gRacePlayers[j].menuSelection % 3;
            }
        }

        D_8010AE64[j] = k;

        if (gCourseSelectStatus.unk2E == 1) {
            k--;
        }

        if (gRacePlayers[j].selectedCharacterId == 5) {
            D_8010AEAC[j] = 0;
        } else if (gRacePlayers[j].menuSelection < 9) {
            D_8010AEAC[j] = gRacePlayers[j].menuSelection / 3;
        } else {
            if (gRacePlayers[j].menuSelection >= 12) {
                D_8010AEAC[j] = 0;
            }

            if ((gRacePlayers[j].menuSelection >= 9) && (gRacePlayers[j].menuSelection < 12)) {
                for (m = 0; m < 3; m++) {
                    if (gMultiplayerCourseSelectExtraCourseIds[j][m] == gRacePlayers[j].menuSelection) {
                        D_8010AEAC[j] = m;
                        break;
                    }
                }
            }
        }

        gRacePlayers[j].menuSelection = D_8010AEF8[j][k];

        gCourseSelectStatus.unk0[j] = 0;
        gCourseSelectStatus.unk4Array[j] = 0;
        gCourseSelectStatus.unk8Array[j] = 0;
        gCourseSelectStatus.unkCArray[j] = 0;
        gCourseSelectStatus.unk10Array[j] = 0;
        gCourseSelectStatus.unk14[j] = 0;
        gCourseSelectStatus.unk1C[j] = 0;
        gCourseSelectStatus.unk24[j] = 0;
    }

    gCourseSelectStatus.unk28 = 0;
    gCourseSelectStatus.unk2A = 0;
    gCourseSelectStatus.unk2C = 0;
}

// updateMultiplayerCourseSelectMenu best match: 97.747% (base_73.c; positionally ranked)
// 273 positional word mismatches, 59 opcode mismatches, and 11 alignment gaps.
// asm-processor requires this pragma to remain on one line.
// clang-format off
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/multiplayer_course_select_menu/updateMultiplayerCourseSelectMenu.s")
// clang-format on

#ifdef NON_MATCHING
void updateMultiplayerCourseSelectMenu(void) {
    s32 input;
    s32 cancelInput;
    u8 finishedPlayerCount;
    s32 count;
    s32 maxVisiblePlayerCount;
    s32 playerIndex;
    u8 maxColumn;
    u8 extraColumn;
    u8 oldColumn;
    s32 selectionOffset;
    s32 *pressedInput;
    RacePlayer *readyPlayer;
    s32 state;
    RaceCamera *camera;

    count = 0;
    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16)gCurrentGameTask->fade, 0x24, 0);
        if (gCurrentGameTask->fade == 0) {
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
        if (gPlayerCount > 0) {
            // Keeping the initializer and loop on one source line reproduces IDO's target scheduling.
            // clang-format off
            readyPlayer = gRacePlayers; while (1)
            {
                // clang-format on
                if (readyPlayer->menuState == 9) {
                    finishedPlayerCount++;
                }
                readyPlayer++;
                if (readyPlayer >= &gRacePlayers[(s32)gPlayerCount]) {
                    break;
                }
            }
        }

        if (finishedPlayerCount == gPlayerCount) {
            playerIndex = 0;
            if (gPlayerCount > 0) {
                while (1) {
                    if ((D_8010AEE8[playerIndex] != 0) || (state = gMenuChoicePromptState[playerIndex], state == 1) ||
                        (state == 9) || (state >= 5)) {
                        count++;
                    }
                    playerIndex++;
                    if (playerIndex >= gPlayerCount) {
                        break;
                    }
                }
            }

            playerIndex = 0;
            if (count == 0) {
                if (gPlayerCount < 3) {
                    maxVisiblePlayerCount = gPlayerCount;
                } else {
                    maxVisiblePlayerCount = 4;
                }
                for (playerIndex = 0; playerIndex < maxVisiblePlayerCount; playerIndex++) {
                    gRacePlayers[playerIndex].menuState = 3;
                }
                D_800EC9C0 = 0xF;
                gMenuFlowState = 1;
            }
        } else if (D_800EC9C0 == 0) {
            for (playerIndex = 0; playerIndex < gPlayerCount; playerIndex++) {
                state = gMenuChoicePromptState[playerIndex];
                if (state == 0) {
                    if ((gCourseSelectStatus.unk0[playerIndex] == 1) && (gRacePlayers[playerIndex].menuState == 0) &&
                        !(D_8010AECC[playerIndex] & 1)) {
                        if (D_8010AEB0 == 1) {
                            gRacePlayers[playerIndex].menuState = 9;
                        } else {
                            if ((gRacePlayers[playerIndex].selectedCharacterId == 5) ||
                                (D_8010AEA0[(u32)playerIndex] == 0)) {
                                maxColumn = 3;
                            } else {
                                maxColumn = 4;
                            }
                            extraColumn = maxColumn;
                            if (gPlayerCount >= 2) {
                                maxColumn--;
                            }

                            oldColumn = gCharacterSelectHudState.highlightedRosterIndices[playerIndex];
                            if (!(gPlayerInputHeld[playerIndex] & 0x10800) &&
                                !(gPlayerInputHeld[playerIndex] & 0x20400)) {
                                gMenuInputRepeatTimers[playerIndex] = 0;
                            }
                            pressedInput = &gPlayerInputPressed[playerIndex];
                            input = *pressedInput;
                            if ((input & 0x10800) || ((gPlayerInputHeld[playerIndex] & 0x10800) &&
                                                      (gMenuInputRepeatTimers[playerIndex] >= 9))) {
                                if (gMenuInputRepeatTimers[playerIndex] == 0) {
                                    gMenuInputRepeatTimers[playerIndex]++;
                                }
                                if (gCharacterSelectHudState.highlightedRosterIndices[playerIndex] > 0) {
                                    gCharacterSelectHudState.highlightedRosterIndices[playerIndex]--;
                                    enqueueSoundEffect(0x19, 0x32);
                                    D_8010AEAC[playerIndex] = 0;
                                    input = *pressedInput;
                                }
                            } else if ((input & 0x20400) || ((gPlayerInputHeld[playerIndex] & 0x20400) &&
                                                             (gMenuInputRepeatTimers[playerIndex] >= 9))) {
                                if (gMenuInputRepeatTimers[playerIndex] == 0) {
                                    gMenuInputRepeatTimers[playerIndex]++;
                                }
                                if (gCharacterSelectHudState.highlightedRosterIndices[playerIndex] < maxColumn) {
                                    gCharacterSelectHudState.highlightedRosterIndices[playerIndex]++;
                                    enqueueSoundEffect(0x19, 0x32);
                                    D_8010AEAC[playerIndex] = 0;
                                    input = *pressedInput;
                                }
                            }

                            if (gMenuInputRepeatTimers[playerIndex] != 0) {
                                gMenuInputRepeatTimers[playerIndex]++;
                                if (gMenuInputRepeatTimers[playerIndex] == 0xFFFF) {
                                    gMenuInputRepeatTimers[playerIndex] = 0xA;
                                }
                            }

                            state = extraColumn;
                            if ((state != gCharacterSelectHudState.highlightedRosterIndices[playerIndex]) &&
                                (state != oldColumn) &&
                                (oldColumn != gCharacterSelectHudState.highlightedRosterIndices[playerIndex])) {
                                D_8010AECC[playerIndex]++;
                                if (oldColumn < gCharacterSelectHudState.highlightedRosterIndices[playerIndex]) {
                                    D_8010AEE8[playerIndex] = -0x800000;
                                } else {
                                    D_8010AEE8[playerIndex] = 0x800000;
                                }
                            }

                            if (state == gCharacterSelectHudState.highlightedRosterIndices[playerIndex]) {
                                gRacePlayers[playerIndex].menuSelection = D_8010AEF8[playerIndex][state - 1];
                                gCourseSelectExtraCourseColumnState = 1;
                            } else {
                                gRacePlayers[playerIndex].menuSelection =
                                    D_8010AEF8[playerIndex]
                                              [gCharacterSelectHudState.highlightedRosterIndices[playerIndex]];
                                gCourseSelectExtraCourseColumnState = 0;
                            }

                            cancelInput = input & 0x4000;
                            if (D_8010AEE8[playerIndex] == 0) {
                                if ((input & 0x1000) || (cancelInput = input & 0x8000, cancelInput != 0)) {
                                    if ((gPlayerCount == 1) &&
                                        (maxColumn == gCharacterSelectHudState.highlightedRosterIndices[0])) {
                                        enqueueSoundEffect(0x18, 0x32);
                                        gRacePlayers[0].menuState = 9;
                                        D_8010AEB0 = 1;
                                        gMenuFlowState = 1;
                                    } else if ((gRacePlayers[playerIndex].menuSelection >= 9) ||
                                               ((gRacePlayers[playerIndex].menuSelection < 9) &&
                                                (D_8010AEB8[playerIndex][gCharacterSelectHudState
                                                                             .highlightedRosterIndices[playerIndex]] !=
                                                 0))) {
                                        enqueueSoundEffect(0x18, 0x32);
                                        gRacePlayers[playerIndex].menuState = 1;
                                        if (gRacePlayers[playerIndex].selectedCharacterId == 5) {
                                            enqueueSoundEffect(0x40, 0x32);
                                        } else {
                                            enqueueSoundEffect(
                                                gCourseSelectColumnSoundEffects
                                                    [gCharacterSelectHudState.highlightedRosterIndices[playerIndex]],
                                                0x32
                                            );
                                        }
                                        gMenuChoicePromptState[playerIndex] = 9;
                                        D_8010AEA4[playerIndex] = 1;
                                        gMenuInputRepeatTimers[playerIndex] = 0;
                                    }
                                } else {
                                    goto handle_column_cancel;
                                }
                            } else {
                            handle_column_cancel:
                                if (cancelInput != 0) {
                                    enqueueSoundEffect(0x18, 0x32);
                                    gRacePlayers[playerIndex].menuState = 9;
                                    D_8010AEB0 = 1;
                                }
                            }
                        }
                    }
                } else {
                    if (state == 9) {
                        D_8010AEA4[playerIndex]++;
                    } else {
                        D_8010AEA4[playerIndex] = 0;
                    }

                    if ((state >= 2) && (state < 5)) {
                        if (D_8010AEB0 == 1) {
                            gRacePlayers[playerIndex].menuState = 9;
                        } else {
                            if (!(gPlayerInputHeld[playerIndex] & 0x10800) &&
                                !(gPlayerInputHeld[playerIndex] & 0x20400)) {
                                gMenuInputRepeatTimers[playerIndex] = 0;
                            }
                            input = gPlayerInputPressed[playerIndex];
                            if ((input & 0x10800) || ((gPlayerInputHeld[playerIndex] & 0x10800) &&
                                                      (gMenuInputRepeatTimers[playerIndex] >= 9) &&
                                                      (gMenuInputRepeatTimers[playerIndex] & 1))) {
                                if (gMenuInputRepeatTimers[playerIndex] == 0) {
                                    gMenuInputRepeatTimers[playerIndex]++;
                                }
                                if (state >= 3) {
                                    gMenuChoicePromptState[playerIndex] = state - 1;
                                    enqueueSoundEffect(0x19, 0x32);
                                    state = gMenuChoicePromptState[playerIndex];
                                    input = gPlayerInputPressed[playerIndex];
                                }
                            } else if ((input & 0x20400) || ((gPlayerInputHeld[playerIndex] & 0x20400) &&
                                                             (gMenuInputRepeatTimers[playerIndex] >= 9) &&
                                                             (gMenuInputRepeatTimers[playerIndex] & 1))) {
                                if (gMenuInputRepeatTimers[playerIndex] == 0) {
                                    gMenuInputRepeatTimers[playerIndex]++;
                                }
                                if (state < (gCourseSelectStatus.unk24[playerIndex] + 1)) {
                                    gMenuChoicePromptState[playerIndex] = state + 1;
                                    enqueueSoundEffect(0x19, 0x32);
                                    state = gMenuChoicePromptState[playerIndex];
                                    input = gPlayerInputPressed[playerIndex];
                                }
                            }

                            if (gMenuInputRepeatTimers[playerIndex] != 0) {
                                gMenuInputRepeatTimers[playerIndex]++;
                                if (gMenuInputRepeatTimers[playerIndex] == 0xFFFF) {
                                    gMenuInputRepeatTimers[playerIndex] = 0xA;
                                }
                            }

                            if ((D_8010AEA0[playerIndex] == 1) &&
                                (gCharacterSelectHudState.highlightedRosterIndices[playerIndex] == 3)) {
                                gRacePlayers[playerIndex].menuSelection =
                                    gCourseSelectExtraCourseIds[playerIndex][gMenuChoicePromptState[playerIndex]];
                            } else {
                                gRacePlayers[playerIndex].menuSelection =
                                    (gRacePlayers[playerIndex].menuSelection % 3) +
                                    (gMenuChoicePromptState[playerIndex] * 3) - 6;
                            }

                            if ((input & 0x8000) || (input & 0x1000)) {
                                enqueueSoundEffect(0x18, 0x32);
                                gRacePlayers[playerIndex].menuState = 2;
                                D_8010AEA8 = 1;
                                if ((D_8010AEA0[playerIndex] == 1) &&
                                    (gCharacterSelectHudState.highlightedRosterIndices[playerIndex] == 3)) {
                                    gRacePlayers[playerIndex].menuSelection =
                                        gCourseSelectExtraCourseIds[playerIndex][gMenuChoicePromptState[playerIndex]];
                                } else {
                                    selectionOffset = (gRacePlayers[playerIndex].selectedCharacterId == 5) ? 0xC : 0;
                                    gRacePlayers[playerIndex].menuSelection =
                                        (gRacePlayers[playerIndex].menuSelection % 3) +
                                        (gMenuChoicePromptState[playerIndex] * 3) + selectionOffset - 6;
                                }

                                if (gRacePlayers[playerIndex].menuSelection >= 9) {
                                    gRacePlayers[playerIndex].selectionUnlockState = 0;
                                } else {
                                    gRacePlayers[playerIndex].selectionUnlockState =
                                        gGameSaveDataBuffer[playerIndex]
                                            .courseUnlockStates[gRacePlayers[playerIndex].menuSelection];
                                }
                                gMenuChoicePromptState[playerIndex] += 3;
                                if (D_8010AECC[playerIndex] == 0) {
                                    gCourseSelectStatus.unkCArray[playerIndex] = 1;
                                    gCourseSelectStatus.unk14[playerIndex] = 0;
                                    gCourseSelectStatus.unk4Array[playerIndex] = 3;
                                } else {
                                    gCourseSelectStatus.unk10Array[playerIndex] = 1;
                                    gCourseSelectStatus.unk1C[playerIndex] = 0;
                                    gCourseSelectStatus.unk8Array[playerIndex] = 3;
                                }
                            } else if (input & 0x4000) {
                                enqueueSoundEffect(0x18, 0x32);
                                gMenuInputRepeatTimers[playerIndex] = 0;
                                gMenuChoicePromptState[playerIndex] += 3;
                                gRacePlayers[playerIndex].menuSelection =
                                    D_8010AEF8[playerIndex]
                                              [gCharacterSelectHudState.highlightedRosterIndices[playerIndex]];
                            }
                        }
                    }
                }

                if (gRacePlayers[playerIndex].menuState == 2) {
                    if (D_8010AEB0 == 1) {
                        gRacePlayers[playerIndex].menuState = 9;
                    } else if (gPlayerInputPressed[playerIndex] & 0x4000) {
                        enqueueSoundEffect(0x18, 0x32);
                        gMenuInputRepeatTimers[playerIndex] = 0;
                        D_8010AEA8 = 0;
                        gRacePlayers[playerIndex].menuState = 1;
                        gMenuChoicePromptState[playerIndex] -= 3;
                        if (D_8010AECC[playerIndex] == 0) {
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

    // Keeping the initializer and loop on one source line reproduces IDO's target scheduling.
    // clang-format off
    camera = D_801121E0; while (1) {
        gCurrentMenuCameraObject = camera;
        (*camera).update();
        camera++;
        if (camera == &D_801124A0) {
            break;
        }
    }
    // clang-format on
    updateCallbackTasks();
}
#endif

void fadeOutMultiplayerCourseSelectMenu(void) {
    if (gCurrentGameTask->fade != 0xFF) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16)gCurrentGameTask->fade, 0x24, 1);
        if (gCurrentGameTask->fade == 0xFF) {
            gFramebufferSwapHold = 1;
        } else {
            updateCallbackTasks();
        }
    } else {
        if (gPendingFramebufferSwapCount == 2) {
            releaseMenuAssetHandles();
            gFramebufferSwapHold = 0;
            gFramebufferSwapDelay.value = 0;
            resumeGameTask(2);
            removeGameTask(4);
        }
    }
}
