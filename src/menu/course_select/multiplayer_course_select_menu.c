#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/menu/course_select/multiplayer_course_select_menu.h"
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

extern CharacterSelectFlowState *gCurrentGameTask;
extern s8 gFramebufferSwapDelay;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

#define MULTIPLAYER_COURSE_SELECT_PLAYER_RECORD_COUNT 4

typedef struct {
    char pad0[0x3F];
    s8 characterState[12];
    char pad4B[0x78AD];
} MultiplayerCourseSelectSaveData;

typedef struct {
    u8 unk0[4];
    u8 unk4[4];
    u8 unk8[4];
    u8 unkC[4];
    u8 unk10[4];
    s16 unk14[4];
    s16 unk1C[4];
    u8 unk24[4];
    u8 unk28;
    char pad29;
    s16 unk2A;
    s16 unk2C;
    u8 unk2E;
} MultiplayerCourseSelectStatus;

extern void n_alSeqpDelete(void);
extern void requestMusicSequenceBank(s32);
extern u8 D_14B450[];
extern u8 D_1502A0[];
extern u8 D_1EF530[];
extern u8 D_1F1A90[];
extern u8 D_245A80[];
extern u8 D_24C8E0[];
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_59AAA0[];
extern u8 D_59DFE0[];
extern u8 D_59E7F0[];
extern u8 D_5A1ED0[];
extern u8 D_5C5320[];
extern u8 D_5CBA80[];
extern u8 D_5CCD40[];
extern s16 gMenuFadeAlpha;
extern u8 D_800EC9C0;
extern u8 gRaceSplitscreenMode;
extern s16 gMenuChoicePromptState[];
extern s8 D_800EC9E5;
extern s8 gCourseSelectModeSelection;
extern MultiplayerCourseSelectSaveData gGameSaveDataBuffer[];
extern s32 gActiveMenuTask;
extern s32 D_8010ADE0;
extern s32 D_8010ADE4;
extern u16 gMenuInputRepeatTimers[];
extern s8 D_8010AE64[];
extern u8 D_8010AEA0[];
extern s8 D_8010AEA1;
extern s8 D_8010AEA2;
extern s8 D_8010AEA3;
extern s8 D_8010AEA4[];
extern s8 D_8010AEA8;
extern s8 D_8010AEAC[];
extern s8 D_8010AEAD;
extern s8 D_8010AEAE;
extern s8 D_8010AEAF;
extern s8 D_8010AEB0;
extern s8 D_8010AEB8[][3];
extern u8 D_8010AEC8[];
extern s8 D_8010AECC[];
extern s16 D_8010AED0;
extern s32 D_8010AED8[];
extern s32 D_8010AEE8[];
extern u8 D_8010AEF8[][4];
extern s8 D_8010AEFB[];
extern u8 D_8010AF08[][3];
extern MultiplayerCourseSelectStatus gCourseSelectStatus;
extern s16 gAssetHandles[];
extern s32 D_80112204;
extern void (*D_8011220C)(void);
extern s32 D_801122B4;
extern void (*D_801122BC)(void);
extern s32 D_80112364;
extern void (*D_8011236C)(void);
extern s32 D_80112414;
extern void (*D_8011241C)(void);
extern u8 gPlayerCount;
extern s32 gMenuFlowState;

// initMultiplayerCourseSelectMenu best match: 83.066% (nonmatchings/initMultiplayerCourseSelectMenu-2870645799593382959/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/multiplayer_course_select_menu/initMultiplayerCourseSelectMenu.s")

#ifdef NON_MATCHING
void initMultiplayerCourseSelectMenu(void) {
    s32 size;
    s32 i;
    s32 j;
    s32 k;
    s32 sum;
    s32 selected;
    s32 screenBase;
    RacePlayer *player;

    if (gRaceSplitscreenMode == 1) {
        requestMusicSequenceBank(2);
    }
    resetRaceCameras();
    resetAllViewports();

    switch (gPlayerCount) {
    case 1:
        configureViewport(0, 0xE8, 0x78, 0x90, 0xD0, 0xA0, 0xF0, 0.6666666865f);
        screenBase = 0xA40000;
        break;
    case 2:
        configureViewport(0, 0xE4, 0x4A, 0x84, 0x54, 0xA8, 0x74, 1.448275805f);
        configureViewport(1, 0xE4, 0xAE, 0x84, 0x54, 0xA8, 0x74, 1.448275805f);
        screenBase = 0x894000;
        break;
    case 3:
    case 4:
        configureViewport(0, 0x78, 0x44, 0x44, 0x40, 0x60, 0x60, 1.333333373f);
        configureViewport(1, 0x78, 0xA8, 0x44, 0x40, 0x60, 0x60, 1.333333373f);
        configureViewport(2, 0x104, 0x44, 0x44, 0x40, 0x60, 0x60, 1.333333373f);
        configureViewport(3, 0x104, 0xA8, 0x44, 0x40, 0x60, 0x60, 1.333333373f);
        screenBase = 0x894000;
        break;
    default:
        break;
    }

    D_8011220C = updateMenuCameraObjectLookAtOriginCallback;
    D_80112204 = screenBase;
    D_8010AEA0[0] = 0;
    D_8010AEAC[0] = 0;
    D_801122BC = updateMenuCameraObjectLookAtOriginCallback;
    D_801122B4 = screenBase;
    D_8010AEA1 = 0;
    D_8010AEAD = 0;
    D_8011236C = updateMenuCameraObjectLookAtOriginCallback;
    D_80112364 = screenBase;
    D_8010AEA2 = 0;
    D_8010AEAE = 0;
    D_8011241C = updateMenuCameraObjectLookAtOriginCallback;
    D_80112414 = screenBase;
    D_8010AEA3 = 0;
    D_8010AEAF = 0;
    gFramebufferSwapDelay = 0;
    D_800EC9E5 = 0;
    D_8010AEA8 = 0;
    D_8010AEB0 = 0;
    gCurrentGameTask->fade = 0xFF;
    gMenuFadeAlpha = gCurrentGameTask->fade;

    loadCompressedRomAsset(D_5CBA80, D_5CCD40, 0x21);
    loadCompressedRomAsset(D_593D10, D_598A70, 0x22);
    loadCompressedRomAsset(D_598A70, D_59AAA0, 0x23);
    loadCompressedRomAsset(D_59AAA0, D_59DFE0, 0x24);
    loadCompressedRomAsset(D_5A1ED0, D_5C5320, 0x25);
    loadCompressedRomAsset(D_59DFE0, D_59E7F0, 0x26);

    size = D_1502A0 - D_14B450;
    gAssetHandles[0xC] = allocRelocatableHeapBlock(size);
    dmaReadRom((u32)D_14B450, (void *)getRelocatableHeapBlockBase(gAssetHandles[0xC]), size);
    loadCompressedRomAsset(D_1EF530, D_1F1A90, 0xD);
    loadCompressedRomAsset(D_245A80, D_24C8E0, 0x1F);
    initCallbackTaskScheduler(0);
    createCallbackTask((void (*)(CallbackTask *))initMenuIconTilemapSpriteActor, 0, 0x63);

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

    player = gRacePlayers;
    do {
        player++;
        player[-1].menuState = 0;
    } while ((u32) player < (u32) gRacePlayersEnd);

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
                sum += gGameSaveDataBuffer[i].characterState[j + k * 3];
            }
            D_8010AEB8[i][j] = (sum != -3);
        }

        sum = 0;
        for (j = 9; j < 12; j++) {
            sum += gGameSaveDataBuffer[i].characterState[j];
        }
        if (sum >= -2) {
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
                if (gGameSaveDataBuffer[i].characterState[j] != -1) {
                    D_8010AEFB[i * 4] = j;
                    break;
                }
            }

            k = 0;
            for (j = 9; j < 12; j++) {
                D_8010AF08[i][k] = 0;
                if (gGameSaveDataBuffer[i].characterState[j] != -1) {
                    D_8010AF08[i][k] = j;
                    k++;
                }
            }
        } else {
            D_8010AEFB[i * 4] = 0;
        }
    }

    for (i = 0; i < gPlayerCount; i++) {
        selected = gRacePlayers[i].menuSelection;
        if ((selected >= 9) && (selected < 12)) {
            j = 3;
            if (gRacePlayers[i].selectedCharacterId == 5) {
                j = 0;
            }
            k = gCourseSelectStatus.unk2E;
        } else {
            k = gCourseSelectStatus.unk2E;
            if (k == 1) {
                j = 0;
                gCourseSelectStatus.unk2E = 0;
                k = 0;
            } else {
                j = selected % 3;
            }
        }

        D_8010AE64[i] = j;
        if (k == 1) {
            j--;
        }

        if (gRacePlayers[i].selectedCharacterId == 5) {
            D_8010AEAC[i] = 0;
        } else if (selected < 9) {
            D_8010AEAC[i] = selected / 3;
        } else {
            if (selected >= 12) {
                D_8010AEAC[i] = 0;
            }
            if (selected >= 9 && selected < 12) {
                for (k = 0; k < 3; k++) {
                    if (D_8010AF08[i][k] == selected) {
                        D_8010AEAC[i] = k;
                        break;
                    }
                }
            }
        }

        gRacePlayers[i].menuSelection = D_8010AEF8[i][j];
        gCourseSelectStatus.unk0[i] = 0;
        gCourseSelectStatus.unk4[i] = 0;
        gCourseSelectStatus.unk8[i] = 0;
        gCourseSelectStatus.unkC[i] = 0;
        gCourseSelectStatus.unk10[i] = 0;
        gCourseSelectStatus.unk14[i] = 0;
        gCourseSelectStatus.unk1C[i] = 0;
        gCourseSelectStatus.unk24[i] = 0;
    }

    gCourseSelectStatus.unk28 = 0;
    gCourseSelectStatus.unk2A = 0;
    gCourseSelectStatus.unk2C = 0;
}
#endif

// updateMultiplayerCourseSelectMenu best match: 69.119% (not yet matching)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/multiplayer_course_select_menu/updateMultiplayerCourseSelectMenu.s")

#ifdef NON_MATCHING
typedef struct MultiplayerCourseSelectObject {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ void (*update)(void);
    /* 0x30 */ u8 pad30[0x80];
} MultiplayerCourseSelectObject;

extern void initCourseSelectPlayerPanels(CallbackTask *);
extern void initCourseSelectCompletePanels(CallbackTask *);
extern s32 enqueueSoundEffect(s16, s16);

extern void *D_8010ADE8;
extern u8 D_8010AF06[];
extern u8 gCourseSelectExtraCourseColumnState;
extern s16 gCourseSelectColumnSoundEffects[];
extern s8 gCourseUnlockSaveSlots[][0x78F8];
extern s32 gPlayerInputHeld[];
extern s32 gPlayerInputPressed[];
extern s32 D_8010AEE8[];
extern MultiplayerCourseSelectObject *gCurrentMenuCameraObject;
extern MultiplayerCourseSelectObject D_801121E0[];
extern MultiplayerCourseSelectObject D_801124A0;

void updateMultiplayerCourseSelectMenu(void) {
    s32 readyCount;
    s32 blockingCount;
    s32 activeCount;
    s32 playerCount;
    s32 i;
    s32 row;
    u8 maxColumnOriginal;
    s8 oldColumn;
    s32 originalChoice;
    u8 maxColumn;
    s32 held;
    s32 pressed;
    s32 heldHorizontal;
    u16 *repeatTimer;
    s8 *column;
    s8 *confirmHold;
    u8 *unlockColumn;
    s8 *rowLock;
    s32 *momentum;
    u8 *selections;
    RacePlayer *player;
    MultiplayerCourseSelectObject *obj;

    readyCount = 0;
    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) gCurrentGameTask->fade, 0x24, 0);
        if (gCurrentGameTask->fade == 0) {
            if (gPlayerCount == 1) {
                createCallbackTask(initCourseSelectCourseIconList, 0, 0x63);
            } else {
                createCallbackTask(initCourseSelectPlayerPanels, 0, 0x62);
                createCallbackTask(initCourseSelectCompletePanels, 0, 0x63);
            }
            D_8010ADE8 = createCallbackTask(initCourseSelectExtraCourseIconList, 0, 0x61);
        }
    } else {
        playerCount = gPlayerCount;
        readyCount = 0;
        if (playerCount > 0) {
            player = gRacePlayers;
            do {
                if (player->menuState == 9) {
                    readyCount = (readyCount + 1) & 0xFF;
                }
                player++;
            } while (player < &gRacePlayers[playerCount]);
        }

        if (playerCount == readyCount) {
            blockingCount = 0;
            for (i = 0; i < playerCount; i++) {
                row = gMenuChoicePromptState[i];
                if ((D_8010AEE8[i] != 0) || (row == 1) || (row == 9) || (row >= 5)) {
                    blockingCount++;
                }
            }

            if (blockingCount == 0) {
                row = (playerCount < 3) ? playerCount : 4;
                for (i = 0; i < row; i++) {
                    gRacePlayers[i].menuState = 3;
                }
                D_800EC9C0 = 0xF;
                gMenuFlowState = 1;
            }
        } else if (D_800EC9C0 == 0) {
            activeCount = 0;
            player = gRacePlayers;
            for (i = 0; i < playerCount; i++, player++) {
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
                                selections = D_8010AEF8[i];
                                repeatTimer = &gMenuInputRepeatTimers[i];
                                maxColumn = 4;
                                if ((player->selectedCharacterId == 5) || (D_8010AEA0[i] == 0)) {
                                    maxColumn = 3;
                                }
                                maxColumnOriginal = maxColumn;
                                if (playerCount >= 2) {
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

                                if ((maxColumnOriginal != *column) && (maxColumnOriginal != originalChoice) && (originalChoice != *column)) {
                                    *confirmHold = *confirmHold + 1;
                                    if (originalChoice < *column) {
                                        *momentum = 0x800000;
                                    } else {
                                        *momentum = -0x800000;
                                    }
                                }
                                if (maxColumnOriginal == *column) {
                                    player->menuSelection = selections[maxColumnOriginal - 1];
                                    gCourseSelectExtraCourseColumnState = 1;
                                } else {
                                    player->menuSelection = selections[*column];
                                    gCourseSelectExtraCourseColumnState = 0;
                                }

                                if (*momentum == 0) {
                                    if ((pressed & START_BUTTON) || (pressed & A_BUTTON)) {
                                        if ((gPlayerCount == 1) && (maxColumn == D_8010AE64[0])) {
                                            enqueueSoundEffect(0x18, 0x32);
                                            gRacePlayers[0].menuState = 9;
                                            D_8010AEB0 = 1;
                                            gMenuFlowState = 1;
                                        } else if ((player->menuSelection >= 9) || (D_8010AEB8[i][*column] != 0)) {
                                            rowLock = &D_8010AEA4[i];
                                            enqueueSoundEffect(0x18, 0x32);
                                            player->menuState = 1;
                                            if (player->selectedCharacterId == 5) {
                                                enqueueSoundEffect(0x40, 0x32);
                                            } else {
                                                enqueueSoundEffect(gCourseSelectColumnSoundEffects[*column], 0x32);
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
                                player->menuSelection = D_8010AF06[(i * 3) + row];
                            } else {
                                player->menuSelection = (row * 3) + (player->menuSelection % 3) - 6;
                            }

                            if ((pressed & A_BUTTON) || (pressed & START_BUTTON)) {
                                enqueueSoundEffect(0x18, 0x32);
                                player->menuState = 2;
                                D_8010AEA8 = 1;
                                if ((*unlockColumn == 1) && (D_8010AE64[i] == 3)) {
                                    player->menuSelection = D_8010AF06[(i * 3) + row];
                                } else {
                                    player->menuSelection = (row * 3) + (player->menuSelection % 3) + ((player->selectedCharacterId == 5) ? 0xC : 0) - 6;
                                }
                                if (player->menuSelection >= 9) {
                                    player->selectionUnlockState = 0;
                                } else {
                                    player->selectionUnlockState = gCourseUnlockSaveSlots[i][player->menuSelection];
                                }
                                gMenuChoicePromptState[i] += 3;
                                if ((u8) D_8010AECC[i] == 0) {
                                    gCourseSelectStatus.unkC[i] = 1;
                                    gCourseSelectStatus.unk14[i] = 0;
                                    gCourseSelectStatus.unk4[i] = 3;
                                } else {
                                    gCourseSelectStatus.unk10[i] = 1;
                                    gCourseSelectStatus.unk1C[i] = 0;
                                    gCourseSelectStatus.unk8[i] = 3;
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
                            gCourseSelectStatus.unk4[i] = 1;
                        } else {
                            gCourseSelectStatus.unk8[i] = 1;
                        }
                    } else {
                        activeCount++;
                    }
                }
                if (activeCount == gPlayerCount) {
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

    obj = D_801121E0;
    do {
        gCurrentMenuCameraObject = obj;
        obj->update();
        obj++;
    } while (obj != &D_801124A0);
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
            gFramebufferSwapDelay = 0;
            resumeGameTask(2);
            removeGameTask(4);
        }
    }
}
