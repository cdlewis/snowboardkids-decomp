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

extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

#define MULTIPLAYER_COURSE_SELECT_PLAYER_RECORD_COUNT 4

s8 D_800B3490[MULTIPLAYER_COURSE_SELECT_DEFAULT_COURSE_COUNT] = {
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
extern s8 D_8010AEA8;
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
extern u8 D_8010AF06[][3];
extern u8 gMultiplayerCourseSelectExtraCourseIds[RACE_PLAYER_COUNT][3];
extern s8 gCourseSelectExtraCourseColumnState;
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

#if 0 /* Superseded without consulting the previous attempt. */
// updateMultiplayerCourseSelectMenu best match: 89.714% (nonmatchings/updateMultiplayerCourseSelectMenu-8498672362023432715/base_52.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/multiplayer_course_select_menu/updateMultiplayerCourseSelectMenu.s")

#ifdef NON_MATCHING
extern void initCourseSelectPlayerPanels(CallbackTask *);
extern void initCourseSelectCompletePanels(CallbackTask *);
extern s32 enqueueSoundEffect(s16, s16);

extern u8 D_8010AF06[];
extern u8 gCourseSelectExtraCourseColumnState;
extern s32 D_8010AEE8[];
extern RaceCamera D_801121E0[];
extern RaceCamera D_801124A0;

void updateMultiplayerCourseSelectMenu(void) {
    s32 readyCount;
    s32 count;
    u8 playerCount;
    s32 i;
    s32 row;
    s32 selectionOffset;
    u8 maxColumnOriginal;
    s8 oldColumn;
    u8 originalChoice;
    u8 maxColumn;
    s32 held;
    s32 pressed;
    s32 heldHorizontal;
    u16 *repeatTimer;
    s8 *column;
    u8 *confirmHold;
    u8 *unlockColumn;
    u8 *rowLock;
    s32 *momentum;
    RacePlayer *readyPlayer;
    RacePlayer *player;
    RaceCamera *obj;

    count = 0;
    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) gCurrentGameTask->fade, 0x24, 0);
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
        playerCount = gPlayerCount;
        readyCount = 0;
        if ((s32) playerCount > 0) {
            readyPlayer = gRacePlayers;
            do {
                if (readyPlayer->menuState == 9) {
                    readyCount = (readyCount + 1) & 0xFF;
                }
                readyPlayer++;
            } while ((u32) readyPlayer < (u32) &gRacePlayers[playerCount]);
        }

        if (playerCount == readyCount) {
            i = 0;
            if ((s32) playerCount > 0) {
                momentum = D_8010AEE8;
                do {
                    if ((*momentum != 0) ||
                        (row = gMenuChoicePromptState[i], (row == 1)) ||
                        (row == 9) ||
                        (row >= 5)) {
                        count++;
                    }
                    i++;
                    momentum++;
                } while (i < (s32) playerCount);
                i = 0;
            }

            if (count == 0) {
                row = (playerCount < 3) ? playerCount : 4;
                for (i = 0; i < row; i++) {
                    gRacePlayers[i].menuState = 3;
                }
                D_800EC9C0 = 0xF;
                gMenuFlowState = 1;
            }
        } else if (D_800EC9C0 == 0) {
            i = 0;
            if ((s32) playerCount > 0) {
                player = gRacePlayers;
                do {
                row = gMenuChoicePromptState[i];
                if (row == 0) {
                    if ((gCourseSelectStatus.unk0[i] == 1) && (player->menuState == 0)) {
                        confirmHold = &D_8010AECC[i];
                        if (((u8) *confirmHold & 1) == 0) {
                            if (D_8010AEB0 == 1) {
                                player->menuState = 9;
                            } else {
                                column = &D_8010AE64[i];
                                momentum = &D_8010AEE8[i];
                                repeatTimer = &gMenuInputRepeatTimers[i];
                                maxColumn = 4;
                                if ((player->selectedCharacterId == 5) || (D_8010AEA0[i] == 0)) {
                                    maxColumn = 3;
                                }
                                maxColumnOriginal = maxColumn;
                                if (gPlayerCount >= 2) {
                                    maxColumn = (maxColumn - 1) & 0xFF;
                                }

                                held = gPlayerInputHeld[i];
                                oldColumn = *column;
                                originalChoice = oldColumn;
                                heldHorizontal = held & (STICK_UP | U_JPAD);
                                if ((heldHorizontal == 0) && !(held & (STICK_DOWN | D_JPAD))) {
                                    *repeatTimer = 0;
                                }
                                pressed = gPlayerInputPressed[i];
                                if ((pressed & (STICK_UP | U_JPAD)) || ((heldHorizontal != 0) && (*repeatTimer >= 9))) {
                                    if (*repeatTimer == 0) {
                                        *repeatTimer = *repeatTimer + 1;
                                    }
                                    if (oldColumn > 0) {
                                        *column = oldColumn - 1;
                                        enqueueSoundEffect(0x19, 0x32);
                                        D_8010AEAC[i] = 0;
                                        oldColumn = *column;
                                        pressed = gPlayerInputPressed[i];
                                    }
                                } else if ((pressed & (STICK_DOWN | D_JPAD)) || ((held & (STICK_DOWN | D_JPAD)) && (*repeatTimer >= 9))) {
                                    if (*repeatTimer == 0) {
                                        *repeatTimer = *repeatTimer + 1;
                                    }
                                    if (oldColumn < maxColumn) {
                                        *column = oldColumn + 1;
                                        enqueueSoundEffect(0x19, 0x32);
                                        D_8010AEAC[i] = 0;
                                        oldColumn = *column;
                                        pressed = gPlayerInputPressed[i];
                                    }
                                }
                                if (*repeatTimer != 0) {
                                    *repeatTimer = *repeatTimer + 1;
                                    if ((u16) *repeatTimer == 0xFFFF) {
                                        *repeatTimer = 0xA;
                                    }
                                }

                                if ((maxColumnOriginal != oldColumn) && (maxColumnOriginal != originalChoice) && (originalChoice != oldColumn)) {
                                    *confirmHold = *confirmHold + 1;
                                    if (originalChoice < oldColumn) {
                                        *momentum = -0x800000;
                                    } else {
                                        *momentum = 0x800000;
                                    }
                                }
                                if (maxColumnOriginal == oldColumn) {
                                    player->menuSelection = D_8010AEF8[i][maxColumnOriginal - 1];
                                    gCourseSelectExtraCourseColumnState = 1;
                                } else {
                                    player->menuSelection = D_8010AEF8[i][oldColumn];
                                    gCourseSelectExtraCourseColumnState = 0;
                                }

                                if ((*momentum == 0) &&
                                    ((pressed & START_BUTTON) || (pressed & A_BUTTON))) {
                                        if ((gPlayerCount == 1) && (maxColumn == D_8010AE64[0])) {
                                            enqueueSoundEffect(0x18, 0x32);
                                            gRacePlayers[0].menuState = 9;
                                            D_8010AEB0 = 1;
                                            gMenuFlowState = 1;
                                        } else if ((player->menuSelection >= 9) ||
                                                   ((player->menuSelection < 9) &&
                                                    (D_8010AEB8[i][oldColumn] != 0))) {
                                            rowLock = &D_8010AEA4[i];
                                            enqueueSoundEffect(0x18, 0x32);
                                            player->menuState = 1;
                                            if (player->selectedCharacterId == 5) {
                                                enqueueSoundEffect(0x40, 0x32);
                                            } else {
                                                enqueueSoundEffect(gCourseSelectColumnSoundEffects[oldColumn], 0x32);
                                            }
                                            gMenuChoicePromptState[i] = 9;
                                            *rowLock = 1;
                                            *repeatTimer = 0;
                                        }
                                } else if (pressed & B_BUTTON) {
                                    enqueueSoundEffect(0x18, 0x32);
                                    player->menuState = 9;
                                    D_8010AEB0 = 1;
                                }
                            }
                        }
                    }
                } else {
                    rowLock = &D_8010AEA4[i];
                    if (row == 9) {
                        *rowLock = *rowLock + 1;
                    } else {
                        *rowLock = 0;
                    }

                    if ((row >= 2) && (row < 5)) {
                        if (D_8010AEB0 == 1) {
                            player->menuState = 9;
                        } else {
                            held = gPlayerInputHeld[i];
                            heldHorizontal = held & (STICK_UP | U_JPAD);
                            unlockColumn = &D_8010AEA0[i];
                            repeatTimer = &gMenuInputRepeatTimers[i];
                            if ((heldHorizontal == 0) && !(held & (STICK_DOWN | D_JPAD))) {
                                *repeatTimer = 0;
                            }
                            pressed = gPlayerInputPressed[i];
                            if ((pressed & (STICK_UP | U_JPAD)) || ((heldHorizontal != 0) && (*repeatTimer >= 9) && (*repeatTimer & 1))) {
                                if (*repeatTimer == 0) {
                                    *repeatTimer = *repeatTimer + 1;
                                }
                                if (row >= 3) {
                                    gMenuChoicePromptState[i] = row - 1;
                                    enqueueSoundEffect(0x19, 0x32);
                                    row = gMenuChoicePromptState[i];
                                    pressed = gPlayerInputPressed[i];
                                }
                            } else if ((pressed & (STICK_DOWN | D_JPAD)) || ((held & (STICK_DOWN | D_JPAD)) && (*repeatTimer >= 9) && (*repeatTimer & 1))) {
                                if (*repeatTimer == 0) {
                                    *repeatTimer = *repeatTimer + 1;
                                }
                                if (row < gCourseSelectStatus.unk24[i] + 1) {
                                    gMenuChoicePromptState[i] = row + 1;
                                    enqueueSoundEffect(0x19, 0x32);
                                    row = gMenuChoicePromptState[i];
                                    pressed = gPlayerInputPressed[i];
                                }
                            }
                            if (*repeatTimer != 0) {
                                *repeatTimer = *repeatTimer + 1;
                                if ((u16) *repeatTimer == 0xFFFF) {
                                    *repeatTimer = 0xA;
                                }
                            }

                            if ((*unlockColumn == 1) && (D_8010AE64[i] == 3)) {
                                selectionOffset = (i << 2) - i;
                                player->menuSelection = D_8010AF06[selectionOffset + row];
                            } else {
                                player->menuSelection = ((row * 4) - row) + (player->menuSelection % 3) - 6;
                            }

                            if ((pressed & A_BUTTON) || (pressed & START_BUTTON)) {
                                enqueueSoundEffect(0x18, 0x32);
                                player->menuState = 2;
                                D_8010AEA8 = 1;
                                if ((*unlockColumn == 1) && (D_8010AE64[i] == 3)) {
                                    selectionOffset = ((i << 1) << 1) - i;
                                    row = gMenuChoicePromptState[i];
                                    player->menuSelection = D_8010AF06[selectionOffset + row];
                                } else {
                                    row = gMenuChoicePromptState[i];
                                    player->menuSelection = ((row * 4) - row) + (player->menuSelection % 3) + ((player->selectedCharacterId == 5) ? 0xC : 0) - 6;
                                }
                                if (player->menuSelection >= 9) {
                                    player->selectionUnlockState = 0;
                                } else {
                                    player->selectionUnlockState =
                                        gGameSaveDataBuffer[i].courseUnlockStates[player->menuSelection];
                                }
                                gMenuChoicePromptState[i] = row + 3;
                                if ((u8) D_8010AECC[i] == 0) {
                                    gCourseSelectStatus.unkCArray[i] = 1;
                                    gCourseSelectStatus.unk14[i] = 0;
                                    gCourseSelectStatus.unk4Array[i] = 3;
                                } else {
                                    gCourseSelectStatus.unk10Array[i] = 1;
                                    gCourseSelectStatus.unk1C[i] = 0;
                                    gCourseSelectStatus.unk8Array[i] = 3;
                                }
                            } else if (pressed & B_BUTTON) {
                                enqueueSoundEffect(0x18, 0x32);
                                *repeatTimer = 0;
                                gMenuChoicePromptState[i] += 3;
                                player->menuSelection = D_8010AEF8[i][D_8010AE64[i]];
                            }
                        }
                    }
                }

                if (player->menuState == 2) {
                    if (D_8010AEB0 == 1) {
                        player->menuState = 9;
                    } else if (gPlayerInputPressed[i] & B_BUTTON) {
                        enqueueSoundEffect(0x18, 0x32);
                        gMenuInputRepeatTimers[i] = 0;
                        D_8010AEA8 = 0;
                        player->menuState = 1;
                        gMenuChoicePromptState[i] -= 3;
                        if ((u8) D_8010AECC[i] == 0) {
                            gCourseSelectStatus.unk4Array[i] = 1;
                        } else {
                            gCourseSelectStatus.unk8Array[i] = 1;
                        }
                    } else {
                        count++;
                    }
                }
                if (count == gPlayerCount) {
                    D_800EC9C0 = 1;
                }
                i++;
                player++;
                } while (i < (s32) gPlayerCount);
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

    obj = D_801121E0;
    do {
        gCurrentMenuCameraObject = obj;
        obj->update();
        obj++;
    } while (obj != &D_801124A0);
    updateCallbackTasks();
}

#endif
#endif

// updateMultiplayerCourseSelectMenu best match: 91.537%
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/multiplayer_course_select_menu/updateMultiplayerCourseSelectMenu.s")

#ifdef NON_MATCHING
void updateMultiplayerCourseSelectMenu(void) {
    s32 input;
    s32 finishedPlayerCount;
    s32 playerIndex;
    s32 selectedPlayerCount;
    u8 maxColumn;
    s32 maxColumnOriginal;
    s32 pressed;
    s32 heldUp;
    s32 menuSelection;
    s32 characterOffset;
    s32 held;
    s32 transitionMomentum;
    s8 oldColumn;
    s8 column;
    u16 repeatTimer;
    RacePlayer *player;
    RaceCamera *camera;
    s32 busyPlayerCount;

    busyPlayerCount = 0;
    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16)gCurrentGameTask->fade, 0x24, 0);
        if (gCurrentGameTask->fade == 0) {
            if (gPlayerCount == 1) {
                createCallbackTask((CallbackTaskCallback)initCourseSelectCourseIconList, 0, 0x63);
            } else {
                createCallbackTask((CallbackTaskCallback)initCourseSelectPlayerPanels, 0, 0x62);
                createCallbackTask((CallbackTaskCallback)initCourseSelectCompletePanels, 0, 0x63);
            }
            D_8010ADE8 =
                createCallbackTask((CallbackTaskCallback)initCourseSelectExtraCourseIconList, 0, 0x61);
        }
    } else {
        selectedPlayerCount = 0;
        if ((s32)gPlayerCount > 0) {
            RacePlayer *readyPlayer;

            readyPlayer = gRacePlayers;
            do {
                if (readyPlayer->menuState == 9) {
                    selectedPlayerCount = (selectedPlayerCount + 1) & 0xFF;
                }
                readyPlayer++;
            } while ((u32)readyPlayer < (u32)&gRacePlayers[(long)gPlayerCount]);
        }

        if (gPlayerCount == selectedPlayerCount) {
            playerIndex = 0;
            if ((s32)gPlayerCount > 0) {
                do {
                    if ((D_8010AEE8[playerIndex] != 0) ||
                        (gMenuChoicePromptState[playerIndex] == 1) ||
                        (gMenuChoicePromptState[playerIndex] == 9) ||
                        (gMenuChoicePromptState[playerIndex] >= 5)) {
                        busyPlayerCount++;
                    }
                    playerIndex++;
                } while (playerIndex < (s32)gPlayerCount);
                playerIndex = 0;
            }
            if (busyPlayerCount == 0) {
                if ((s32)gPlayerCount < 3) {
                    selectedPlayerCount = gPlayerCount;
                } else {
                    selectedPlayerCount = 4;
                }
                for (playerIndex = 0; playerIndex < selectedPlayerCount; playerIndex++) {
                    gRacePlayers[playerIndex].menuState = 3;
                }
                D_800EC9C0 = 0xF;
                gMenuFlowState = 1;
            }
        } else if (D_800EC9C0 == 0) {
            playerIndex = 0;
            if ((s32)gPlayerCount > 0) {
                player = gRacePlayers;
                finishedPlayerCount = 0;
                do {
                    if (gMenuChoicePromptState[playerIndex] == 0) {
                        if ((gCourseSelectStatus.unk0[playerIndex] == 1) &&
                            (player->menuState == 0) &&
                            !(D_8010AECC[playerIndex] & 1)) {
                            if (D_8010AEB0 == 1) {
                                player->menuState = 9;
                            } else {
                                if ((player->selectedCharacterId == 5) ||
                                    (D_8010AEA0[playerIndex] == 0)) {
                                    maxColumn = 3;
                                } else {
                                    maxColumn = 4;
                                }
                                maxColumnOriginal = maxColumn;
                                if ((s32)gPlayerCount >= 2) {
                                    maxColumn = maxColumn - 1;
                                }

                                held = gPlayerInputHeld[playerIndex];
                                column = gCharacterSelectHudState.highlightedRosterIndices[playerIndex];
                                oldColumn = column;
                                heldUp = held & 0x10800;
                                if ((heldUp == 0) && !(held & 0x20400)) {
                                    gMenuInputRepeatTimers[playerIndex] = 0;
                                }

                                input = gPlayerInputPressed[playerIndex];
                                pressed = input;
                                if ((pressed & 0x10800) ||
                                    ((heldUp != 0) &&
                                     ((s32)(u16)gMenuInputRepeatTimers[playerIndex] >= 9))) {
                                    repeatTimer = gMenuInputRepeatTimers[playerIndex];
                                    if (repeatTimer == 0) {
                                        gMenuInputRepeatTimers[playerIndex] = repeatTimer + 1;
                                    }
                                    if (column > 0) {
                                        gCharacterSelectHudState.highlightedRosterIndices[playerIndex] = column - 1;
                                        enqueueSoundEffect(0x19, 0x32);
                                        D_8010AEAC[playerIndex] = 0;
                                    }
                                } else if ((pressed & 0x20400) ||
                                           ((held & 0x20400) &&
                                            ((s32)(u16)gMenuInputRepeatTimers[playerIndex] >= 9))) {
                                    repeatTimer = gMenuInputRepeatTimers[playerIndex];
                                    if (repeatTimer == 0) {
                                        gMenuInputRepeatTimers[playerIndex] = repeatTimer + 1;
                                    }
                                    if (column < (s32)maxColumn) {
                                        gCharacterSelectHudState.highlightedRosterIndices[playerIndex] = column + 1;
                                        enqueueSoundEffect(0x19, 0x32);
                                        D_8010AEAC[playerIndex] = 0;
                                    }
                                }

                                repeatTimer = gMenuInputRepeatTimers[playerIndex];
                                transitionMomentum = D_8010AEE8[playerIndex];
                                if (repeatTimer != 0) {
                                    repeatTimer++;
                                    gMenuInputRepeatTimers[playerIndex] = repeatTimer;
                                    if (repeatTimer == 0xFFFF) {
                                        gMenuInputRepeatTimers[playerIndex] = 0xA;
                                    }
                                }

                                column = gCharacterSelectHudState.highlightedRosterIndices[playerIndex];
                                if ((maxColumnOriginal != column) &&
                                    (maxColumnOriginal != (u8)oldColumn) &&
                                    ((u8)oldColumn != column)) {
                                    D_8010AECC[playerIndex]++;
                                    if ((s32)(u8)oldColumn < column) {
                                        D_8010AEE8[playerIndex] = -0x800000;
                                    } else {
                                        D_8010AEE8[playerIndex] = 0x800000;
                                    }
                                }

                                if (maxColumnOriginal == column) {
                                    player->menuSelection =
                                        D_8010AEF8[playerIndex][maxColumnOriginal - 1];
                                    gCourseSelectExtraCourseColumnState = 1;
                                } else {
                                    player->menuSelection = D_8010AEF8[playerIndex][column];
                                    gCourseSelectExtraCourseColumnState = 0;
                                }

                                pressed = gPlayerInputPressed[playerIndex];
                                if ((transitionMomentum == 0) &&
                                    ((pressed & 0x1000) || (pressed & 0x8000))) {
                                    if ((gPlayerCount == 1) &&
                                        (maxColumn ==
                                         gCharacterSelectHudState.highlightedRosterIndices[0])) {
                                        enqueueSoundEffect(0x18, 0x32);
                                        gRacePlayers[0].menuState = 9;
                                        D_8010AEB0 = 1;
                                        gMenuFlowState = 1;
                                    } else {
                                        menuSelection = player->menuSelection;
                                        if ((menuSelection >= 9) ||
                                            ((menuSelection < 9) &&
                                             (D_8010AEB8[playerIndex][column] != 0))) {
                                            enqueueSoundEffect(0x18, 0x32);
                                            player->menuState = 1;
                                            if (player->selectedCharacterId == 5) {
                                                enqueueSoundEffect(0x40, 0x32);
                                            } else {
                                                enqueueSoundEffect(
                                                    gCourseSelectColumnSoundEffects
                                                        [gCharacterSelectHudState
                                                             .highlightedRosterIndices[playerIndex]],
                                                    0x32);
                                            }
                                            gMenuChoicePromptState[playerIndex] = 9;
                                            D_8010AEA4[playerIndex] = 1;
                                            gMenuInputRepeatTimers[playerIndex] = 0;
                                        }
                                    }
                                } else if (pressed & 0x4000) {
                                    enqueueSoundEffect(0x18, 0x32);
                                    player->menuState = 9;
                                    D_8010AEB0 = 1;
                                }
                            }
                        }
                    } else {
                        if (gMenuChoicePromptState[playerIndex] == 9) {
                            D_8010AEA4[playerIndex]++;
                        } else {
                            D_8010AEA4[playerIndex] = 0;
                        }

                        if ((gMenuChoicePromptState[playerIndex] >= 2) &&
                            (gMenuChoicePromptState[playerIndex] < 5)) {
                            if (D_8010AEB0 == 1) {
                                player->menuState = 9;
                            } else {
                                held = gPlayerInputHeld[playerIndex];
                                heldUp = held & 0x10800;
                                if ((heldUp == 0) && !(held & 0x20400)) {
                                    gMenuInputRepeatTimers[playerIndex] = 0;
                                }

                                pressed = gPlayerInputPressed[playerIndex];
                                if ((pressed & 0x10800) ||
                                    ((heldUp != 0) &&
                                     ((s32)(u16)gMenuInputRepeatTimers[playerIndex] >= 9) &&
                                     ((u16)gMenuInputRepeatTimers[playerIndex] & 1))) {
                                    repeatTimer = gMenuInputRepeatTimers[playerIndex];
                                    if (repeatTimer == 0) {
                                        gMenuInputRepeatTimers[playerIndex] = repeatTimer + 1;
                                    }
                                    if (gMenuChoicePromptState[playerIndex] >= 3) {
                                        gMenuChoicePromptState[playerIndex]--;
                                        enqueueSoundEffect(0x19, 0x32);
                                    }
                                } else if ((pressed & 0x20400) ||
                                           ((held & 0x20400) &&
                                            ((s32)(u16)gMenuInputRepeatTimers[playerIndex] >= 9) &&
                                            ((u16)gMenuInputRepeatTimers[playerIndex] & 1))) {
                                    repeatTimer = gMenuInputRepeatTimers[playerIndex];
                                    if (repeatTimer == 0) {
                                        gMenuInputRepeatTimers[playerIndex] = repeatTimer + 1;
                                    }
                                    if (gMenuChoicePromptState[playerIndex] <
                                        (gCourseSelectStatus.unk24[playerIndex] + 1)) {
                                        gMenuChoicePromptState[playerIndex]++;
                                        enqueueSoundEffect(0x19, 0x32);
                                    }
                                }

                                repeatTimer = gMenuInputRepeatTimers[playerIndex];
                                if (repeatTimer != 0) {
                                    repeatTimer++;
                                    gMenuInputRepeatTimers[playerIndex] = repeatTimer;
                                    if (repeatTimer == 0xFFFF) {
                                        gMenuInputRepeatTimers[playerIndex] = 0xA;
                                    }
                                }

                                if ((D_8010AEA0[playerIndex] == 1) &&
                                    (gCharacterSelectHudState
                                         .highlightedRosterIndices[playerIndex] == 3)) {
                                    player->menuSelection =
                                        D_8010AF06[playerIndex]
                                                     [gMenuChoicePromptState[playerIndex]];
                                } else {
                                    player->menuSelection =
                                        ((gMenuChoicePromptState[playerIndex] * 4) -
                                         gMenuChoicePromptState[playerIndex]) +
                                        ((s32)player->menuSelection % 3) - 6;
                                }

                                pressed = gPlayerInputPressed[playerIndex];
                                if ((pressed & 0x8000) || (pressed & 0x1000)) {
                                    enqueueSoundEffect(0x18, 0x32);
                                    player->menuState = 2;
                                    D_8010AEA8 = 1;
                                    if ((D_8010AEA0[playerIndex] == 1) &&
                                        (gCharacterSelectHudState
                                             .highlightedRosterIndices[playerIndex] == 3)) {
                                        player->menuSelection =
                                            D_8010AF06[playerIndex]
                                                         [gMenuChoicePromptState[playerIndex]];
                                    } else {
                                        characterOffset = 0;
                                        if (player->selectedCharacterId == 5) {
                                            characterOffset = 0xC;
                                        }
                                        player->menuSelection =
                                            ((gMenuChoicePromptState[playerIndex] * 4) -
                                             gMenuChoicePromptState[playerIndex]) +
                                            ((s32)player->menuSelection % 3) +
                                            characterOffset - 6;
                                    }

                                    if ((s32)player->menuSelection >= 9) {
                                        player->selectionUnlockState = 0;
                                    } else {
                                        player->selectionUnlockState =
                                            gGameSaveDataBuffer[playerIndex].courseUnlockStates[player->menuSelection];
                                    }

                                    gMenuChoicePromptState[playerIndex] += 3;
                                    if (D_8010AECC[playerIndex] == 0) {
                                        gCourseSelectStatus.unkC[playerIndex] = 1;
                                        gCourseSelectStatus.unk14[playerIndex] = 0;
                                        gCourseSelectStatus.unk4[playerIndex] = 3;
                                    } else {
                                        gCourseSelectStatus.unk10[playerIndex] = 1;
                                        gCourseSelectStatus.unk1C[playerIndex] = 0;
                                        gCourseSelectStatus.unk8[playerIndex] = 3;
                                    }
                                } else if (pressed & 0x4000) {
                                    enqueueSoundEffect(0x18, 0x32);
                                    gMenuInputRepeatTimers[playerIndex] = 0;
                                    gMenuChoicePromptState[playerIndex] += 3;
                                    player->menuSelection =
                                        D_8010AEF8[playerIndex]
                                                     [gCharacterSelectHudState
                                                          .highlightedRosterIndices[playerIndex]];
                                }
                            }
                        }
                    }

                    if (player->menuState == 2) {
                        if (D_8010AEB0 == 1) {
                            player->menuState = 9;
                        } else if (gPlayerInputPressed[playerIndex] & 0x4000) {
                            enqueueSoundEffect(0x18, 0x32);
                            gMenuInputRepeatTimers[playerIndex] = 0;
                            D_8010AEA8 = 0;
                            player->menuState = 1;
                            gMenuChoicePromptState[playerIndex] -= 3;
                            if (D_8010AECC[playerIndex] == 0) {
                                gCourseSelectStatus.unk4[playerIndex] = 1;
                            } else {
                                gCourseSelectStatus.unk8[playerIndex] = 1;
                            }
                        } else {
                            finishedPlayerCount++;
                        }
                    }

                    if (finishedPlayerCount == gPlayerCount) {
                        D_800EC9C0 = 1;
                    }
                    player++;
                    playerIndex++;
                } while (playerIndex < (s32)gPlayerCount);
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

    camera = D_801121E0;
    do {
        gCurrentMenuCameraObject = camera;
        camera->update();
        camera++;
    } while (camera != &D_801124A0);
    updateCallbackTasks();
}
#endif

void fadeOutMultiplayerCourseSelectMenu(void) {
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
            gFramebufferSwapDelay.value = 0;
            resumeGameTask(2);
            removeGameTask(4);
        }
    }
}
