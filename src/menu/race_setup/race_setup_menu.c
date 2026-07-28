#include "common.h"
#include "assets.h"
#include "game/engine/asset_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/audio/sound_manager.h"
#include "game/menu/race_setup/race_setup_menu.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/race_setup/race_setup_ui.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"
#include "game/menu/character_select/character_select_menu.h"
#include "game/menu/course_select/multiplayer_course_select_menu.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 fade;
    /* 0x1C */ s32 timer;
} RaceSetupMenuState;

typedef struct {
    /* 0x00 */ u8 state;
    /* 0x01 */ u8 timer;
    /* 0x02 */ s16 alpha;
    /* 0x04 */ u8 forceUpdate;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 pendingStatusCodes[4];
    /* 0x0E */ u8 statusTransitionStates[4];
    /* 0x12 */ s16 nextStatusCodes[4];
    /* 0x1A */ u8 pad1A[0x1E];
} RaceSetupMenuSubState;

typedef struct {
    /* 0x0 */ s16 pad0;
    /* 0x2 */ u8 state;
    /* 0x3 */ u8 pad3;
    /* 0x4 */ u16 targetScale;
    /* 0x6 */ u16 timer;
    /* 0x8 */ u8 selectedOption;
    /* 0x9 */ u8 confirmSelection;
} ControllerPakRumbleCheckPromptTransition;

RaceSetupMenuSubState gRaceSetupMenuSubState;
s16 gRaceSetupSavePanelRects[2][4];

extern u8 gConnectedControllerCount;
extern s32 enqueueSoundEffect(s16 soundId, s16 priority);
extern s8 gFramebufferSwapDelay;
extern s16 gMenuFadeAlpha;
extern u8 gRaceRumbleEnabled;
extern u8 gRumblePakConnectedByController[];
extern u8 gMenuSelectionConfirmTimer;
extern char D_800EC9E5;
extern s8 gCourseSelectModeSelection;
extern u16 gMenuInputRepeatTimers;
extern u8 gHighestUnlockedCourse;
extern s16 gRaceCourseIndex;
extern u8 gPlayerCount;
extern RaceSetupMenuState *gCurrentGameTask;
extern s32 gMenuFlowState;
extern s32 gPlayerInputHeld;
extern s32 gPlayerInputPressed[];

void initRaceSetupMenu(void) {
    s32 i;

    requestMusicSequenceBank(1);
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);

    gFramebufferSwapDelay = 0;
    gCurrentGameTask->fade = 0;
    gCurrentGameTask->timer = 0;
    gMenuSelectionConfirmTimer = 0;
    D_800EC9E5 = 1;
    gCourseSelectModeSelection = 0;
    gHighestUnlockedCourse = 0;
    gRaceRumbleEnabled = 0;
    gRaceCourseIndex = 9;
    gMenuFlowState = 0;
    gMenuInputRepeatTimers = 0;
    gPlayerCount = 1;
    gMenuFadeAlpha = gCurrentGameTask->fade;

    LOAD_ASSET(_59AAA0, 0x21);
    LOAD_ASSET(_245A80, 0x1F);
    LOAD_ASSET(_593D10, 0x22);
    LOAD_ASSET(_598A70, 0x23);
    LOAD_ASSET(_60F1A0, 0x29);

    initCallbackTaskScheduler(0);
    gActiveMenuTask = createCallbackTask((CallbackTaskCallback)initRaceSetupPlayerCountPrompt, 0, 0x64);

    gRaceSetupMenuSubState.state = 0;
    gRaceSetupMenuSubState.timer = 0;
    gRaceSetupMenuSubState.alpha = 0;
    gRaceSetupMenuSubState.forceUpdate = 0;
    for (i = 0; i < 4; i++) {
        gRumblePakConnectedByController[i] = 0;
        gRaceSetupMenuSubState.pendingStatusCodes[i] = 0;
        gRaceSetupMenuSubState.statusTransitionStates[i] = 0;
        gRaceSetupMenuSubState.nextStatusCodes[i] = 0;
        gRacePlayers[i].menuSelection = 0;
    }

    setCurrentGameTaskCallback(updateRaceSetupPlayerCountMenu, 0);
}

void updateRaceSetupPlayerCountMenu(void) {
    s32 confirmationNext;
    s32 temp_a3;
    s32 substateNext;
    s32 temp_v0;
    u8 temp_v1;
    u32 confirmationValue;
    u16 repeatTimer;
    s32 one;

    one = 1;
    substateNext = 2;
    if ((gRaceSetupMenuSubState.state == one) &&
        ((gPlayerInputPressed[0] & 0x8000) || (gPlayerInputPressed[0] & 0x1000))) {
        enqueueSoundEffect(one, 0x32);
        gRaceSetupMenuSubState.state = substateNext;
        gRaceSetupMenuSubState.timer = 0;
    }
    if (gCurrentGameTask->timer == one) {
        temp_v1 = gMenuSelectionConfirmTimer;
        confirmationValue = temp_v1;
        temp_v0 = temp_v1;
        if (temp_v1 == 0) {
            temp_a3 = gPlayerInputPressed[0];
            temp_v0 = gPlayerInputHeld & 0x10800;
            if ((temp_v0 == 0) && !(gPlayerInputHeld & 0x20400)) {
                gMenuInputRepeatTimers = 0;
            }
            if ((temp_a3 & 0x10800) ||
                ((temp_v0 != 0) && ((s32)gMenuInputRepeatTimers >= 9) &&
                 (((s32)gMenuInputRepeatTimers % 3) == 0))) {
                if (gMenuInputRepeatTimers == 0) {
                    gMenuInputRepeatTimers += 1;
                }
                if (gPlayerCount != one) {
                    gPlayerCount -= 1;
                    enqueueSoundEffect(0x19, 0x32);
                }
            } else if ((temp_a3 & 0x20400) ||
                       ((gPlayerInputHeld & 0x20400) && ((s32)gMenuInputRepeatTimers >= 9) &&
                        (((s32)gMenuInputRepeatTimers % 3) == 0))) {
                if (gMenuInputRepeatTimers == 0) {
                    gMenuInputRepeatTimers += 1;
                }
                if (gPlayerCount != gConnectedControllerCount) {
                    gPlayerCount += 1;
                    enqueueSoundEffect(0x19, 0x32);
                }
            }
            repeatTimer = gMenuInputRepeatTimers;
            if (repeatTimer != 0) {
                gMenuInputRepeatTimers = repeatTimer + 1;
                if (gMenuInputRepeatTimers == 0xFFFF) {
                    gMenuInputRepeatTimers = 0xA;
                }
            }
            if ((temp_a3 & 0x8000) || (temp_a3 & 0x1000)) {
                gMenuSelectionConfirmTimer = 1;
                enqueueSoundEffect(0x18, 0x32);
            }
        } else if (temp_v0 < 0x13) {
            gMenuSelectionConfirmTimer = (confirmationNext = confirmationValue + 1);
        }
    }
    if (gRaceSetupMenuSubState.state == 5) {
        setCurrentGameTaskCallback(initRaceSetupSaveMenu, 0);
        gCurrentGameTask->fade = 0;
        gMenuSelectionConfirmTimer = 0;
    }
    updateCallbackTasks();
}

extern s16 gRaceSetupSavePanelInitialRects[4][2];
extern s16 gMenuChoicePromptState[];
extern u8 gControllerPakOperationCounts[];
extern u8 D_800EC9E4;
extern s32 D_8010ADE0;
extern s32 D_8010ADE4;

void initRaceSetupSaveMenu(void) {
    s32 i;
    s32 connectedControllerCount;
    RacePlayer *player;

    for (i = 0; i < 4; i++) {
        gControllerPakStatusCodes[i] = 0;
        gMenuChoicePromptState[i] = 0;
        gControllerPakRetryCounts[i] = 0;
        gControllerPakOperationCounts[i] = 0;
    }

    D_800EC9E4 = 0;
    do { i = 0; connectedControllerCount = gConnectedControllerCount; if (connectedControllerCount > 0) { player = gRacePlayers; do { player++; player[-1].menuState = 0; i++; } while (player < &gRacePlayers[connectedControllerCount]); i = 0; } do { initRaceSetupPlayerSaveData(i); i++; } while (i < 4); D_8010ADE0 = 0; D_8010ADE4 = 0; } while (0);
    D_8010ADE8 = 0;
    gMenuSelectionConfirmTimer = 0;
    gMenuFlowState = 0;
    gRaceRumbleEnabled = 0;

    for (i = 0; i < 4; i++) {
        gRaceSetupSavePanelRects[0][i] = gRaceSetupSavePanelInitialRects[i][0];
        gRaceSetupSavePanelRects[1][i] = gRaceSetupSavePanelInitialRects[i][1];
    }

    setCurrentGameTaskCallback(updateRaceSetupSaveMenu, 0);
    updateCallbackTasks();
}

// updateRaceSetupSaveMenu best match: 99.498% (nonmatchings/updateRaceSetupSaveMenu-8498672362023432715/base_18.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/race_setup/race_setup_menu/updateRaceSetupSaveMenu.s")

#ifdef NON_MATCHING
#define CONTROLLER_PAK_STATUS_PROBE 0
#define CONTROLLER_PAK_STATUS_SAVE_STATUS 1
#define CONTROLLER_PAK_STATUS_SAVE_READ 2
#define CONTROLLER_PAK_STATUS_REPAIR 3
#define CONTROLLER_PAK_STATUS_RETRY 4
#define CONTROLLER_PAK_STATUS_READY 5
#define CONTROLLER_PAK_STATUS_NO_PAK 7
#define CONTROLLER_PAK_STATUS_SAVE_FOUND 8
#define CONTROLLER_PAK_STATUS_READ_FAILED 0xD
#define CONTROLLER_PAK_STATUS_REPAIR_FAILED 0xE
#define CONTROLLER_PAK_STATUS_REPAIRED 0xF
#define CONTROLLER_PAK_STATUS_REPAIR_CONFIRM 0x11
#define CONTROLLER_PAK_STATUS_USE_EXISTING_SAVE 0x12
#define CONTROLLER_PAK_RETRY_LIMIT 3
#define CONTROLLER_PAK_STATUS_CHOICE_PROMPT 6

#define SAVE_CHOICE_USE_PAK 3
#define SAVE_CHOICE_SKIP_PAK 4
#define SAVE_STATUS_TRANSITION_FADE_OUT 2
#define SAVE_STATUS_TRANSITION_DONE 3
#define SAVE_PANEL_CREATE_DELAY 5
#define SAVE_READY_CONFIRM_DELAY 0xF

typedef struct {
    /* 0x0000 */ u8 pad0[0x4];
    /* 0x0004 */ s32 money;
    /* 0x0008 */ u8 pad8[0x44];
    /* 0x004C */ u8 highestUnlockedCourse;
    /* 0x004D */ u8 pad4D[0x78AB];
} RaceSetupSaveData;

typedef struct {
    /* 0x0000 */ s32 money;
    /* 0x0004 */ u8 pad4[0x78F4];
} RaceSetupSaveMoney;

extern void requestRumbleMotorInitWithContext(u16 controllerIndex, s32 playerCount, s32 choiceValue);
extern void requestControllerPakSaveStatusWithContext(u16 controllerIndex, s32 playerCount, s32 choiceValue);
extern void requestControllerPakSaveReadWithContext(u16 controllerIndex, s32 playerCount, s32 choiceValue);
extern void requestControllerPakRepairWithContext(u16 controllerIndex, s32 playerCount, s32 choiceValue);
extern void initControllerPakRumbleCheckPrompt(CallbackTask *);

extern u8 D_800B3199[];
extern s16 gMenuChoicePromptState[];
extern u8 gControllerPakOperationCounts[];
extern s32 gRumbleMotorStatuses[];
extern u8 D_800EC9E4;
extern RaceSetupSaveData gGameSaveDataBuffer[];
extern RaceSetupSaveMoney D_800EC9F4[];
extern ControllerPakRumbleCheckPromptTransition gControllerPakRumbleCheckPromptTransition;
extern CallbackTask *D_8010ADE0;
extern CallbackTask *D_8010ADE4;

#if 0
void updateRaceSetupSaveMenu(void) {
    s32 allPlayersReady = 0;
    s16 allControllerPakOpsComplete = 0;
    CallbackTask *savePanelTask = D_8010ADE0;
    CallbackTask *saveStatusTask = D_8010ADE8;
    s32 playerIndex;
    u8 *operationCountPtr;

    if ((gRaceSetupMenuSubState.forceUpdate == 1) && (gRaceSetupMenuSubState.state == CONTROLLER_PAK_STATUS_READY)) {
        gRaceSetupMenuSubState.state = 6;
    }

    if ((savePanelTask == NULL) || (gRaceSetupMenuSubState.forceUpdate != 0)) {
        if ((gRaceSetupMenuSubState.state >= 6) && (gRaceSetupMenuSubState.state < 8)) {
            if (gRaceSetupMenuSubState.state == 6) {
                s32 input = gPlayerInputPressed[0];
                if ((input & A_BUTTON) || (input & START_BUTTON)) {
                    enqueueSoundEffect(1, 0x32);
                    gRaceSetupMenuSubState.state = 7;
                    gRaceSetupMenuSubState.alpha = 0xFF;
                    gRaceSetupMenuSubState.timer = 0;
                }
            }
        } else {
            allPlayersReady = 1;
            allControllerPakOpsComplete = 1;
            playerIndex = 0;
            if (gPlayerCount > 0) {
                operationCountPtr = gControllerPakOperationCounts;
                do {
                    u8 operationCount = *operationCountPtr;

                    if (operationCount != 1) {
                        RacePlayer *player = &gRacePlayers[playerIndex];
                        s32 statusCode;
                        s16 *choiceState;
                        s16 choiceValue;
                        s32 input;

                        if ((saveStatusTask == NULL) || (gRaceSetupMenuSubState.statusTransitionStates[playerIndex] == 0)) {
                            statusCode = playerIndex * 2;
                            choiceState = &gMenuChoicePromptState[playerIndex];
                            choiceValue = *choiceState;
                            if (choiceValue != 0) {
                                statusCode = 6;
                            } else {
                                statusCode = gControllerPakStatusCodes[playerIndex];
                            }

                            switch (statusCode) {
                                case CONTROLLER_PAK_STATUS_PROBE: {
                                    s32 pakState;
                                    u16 controllerIndex = playerIndex;

                                    gRumblePakConnectedByController[playerIndex] = 0;
                                    requestRumbleMotorInitWithContext(controllerIndex, gPlayerCount, choiceValue);
                                    pakState = gRumbleMotorStatuses[playerIndex];
                                    if ((pakState != 1) && (pakState != 0xB) && (pakState != 4)) {
                                        gRumblePakConnectedByController[playerIndex] = 1;
                                    } else {
                                        gRumblePakConnectedByController[playerIndex] = 0;
                                    }
                                    requestControllerPakProbe(controllerIndex);
                                    break;
                                }

                                case CONTROLLER_PAK_STATUS_SAVE_STATUS:
                                    requestControllerPakSaveStatusWithContext((u16)playerIndex, gPlayerCount, choiceValue);
                                    break;

                                case CONTROLLER_PAK_STATUS_SAVE_READ: {
                                    u8 result;

                                    requestControllerPakSaveReadWithContext((u16)playerIndex, gPlayerCount, choiceValue);
                                    result = gControllerPakRetryCounts[playerIndex];
                                    if (result == 0) {
                                        player->unk568 = 0;
                                        player->unkC = gGameSaveDataBuffer[playerIndex].money;
                                        gControllerPakStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_SAVE_FOUND;
                                        *choiceState = 1;
                                    } else if (result == CONTROLLER_PAK_RETRY_LIMIT) {
                                        if (saveStatusTask != NULL) {
                                            gRaceSetupMenuSubState.statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_FADE_OUT;
                                            gRaceSetupMenuSubState.nextStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_READ_FAILED;
                                        } else {
                                            gControllerPakStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_READ_FAILED;
                                        }
                                        gControllerPakRetryCounts[playerIndex] = 0;
                                    }
                                    break;
                                }

                                case CONTROLLER_PAK_STATUS_REPAIR: {
                                    u8 result;

                                    requestControllerPakRepairWithContext((u16)playerIndex, gPlayerCount, choiceValue);
                                    result = gControllerPakRetryCounts[playerIndex];
                                    if (result == 0) {
                                        if (saveStatusTask != NULL) {
                                            gRaceSetupMenuSubState.statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_FADE_OUT;
                                            gRaceSetupMenuSubState.nextStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_REPAIRED;
                                        } else {
                                            gControllerPakStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_REPAIRED;
                                        }
                                    } else if (result == CONTROLLER_PAK_RETRY_LIMIT) {
                                        if (saveStatusTask != NULL) {
                                            gRaceSetupMenuSubState.statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_FADE_OUT;
                                            gRaceSetupMenuSubState.nextStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_REPAIR_FAILED;
                                        } else {
                                            gControllerPakStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_REPAIR_FAILED;
                                        }
                                        gControllerPakRetryCounts[playerIndex] = 0;
                                    }
                                    break;
                                }

                                case CONTROLLER_PAK_STATUS_RETRY:
                                    input = gPlayerInputPressed[playerIndex];
                                    if ((input & A_BUTTON) || (input & START_BUTTON)) {
                                        enqueueSoundEffect(1, 0x32);
                                        if (saveStatusTask != NULL) {
                                            gRaceSetupMenuSubState.statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_FADE_OUT;
                                            gRaceSetupMenuSubState.nextStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_PROBE;
                                        } else {
                                            gControllerPakStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_PROBE;
                                        }
                                    }
                                    break;

                                case CONTROLLER_PAK_STATUS_READY:
                                    if (player->menuState == 0) {
                                        player->menuState = 1;
                                    }
                                    break;

                                case 6:
                                    if ((choiceValue == SAVE_CHOICE_USE_PAK) || (choiceValue == SAVE_CHOICE_SKIP_PAK)) {
                                        input = gPlayerInputPressed[playerIndex];
                                        if ((input & (STICK_UP | U_JPAD)) && (choiceValue != SAVE_CHOICE_USE_PAK)) {
                                            *choiceState = choiceValue - 1;
                                            enqueueSoundEffect(0x19, 0x32);
                                            input = gPlayerInputPressed[playerIndex];
                                        }
                                        if (input & (STICK_DOWN | D_JPAD)) {
                                            choiceValue = *choiceState;
                                            if (choiceValue != SAVE_CHOICE_SKIP_PAK) {
                                                *choiceState = choiceValue + 1;
                                                enqueueSoundEffect(0x19, 0x32);
                                                input = gPlayerInputPressed[playerIndex];
                                            }
                                        }
                                        if ((input & A_BUTTON) || (input & START_BUTTON)) {
                                            s16 state = gControllerPakStatusCodes[playerIndex];

                                            enqueueSoundEffect(1, 0x32);
                                            if (*choiceState == SAVE_CHOICE_SKIP_PAK) {
                                                if (state == CONTROLLER_PAK_STATUS_SAVE_FOUND) {
                                                    RaceSetupSaveData *save = &gGameSaveDataBuffer[playerIndex];

                                                    initRaceSetupPlayerSaveData(playerIndex);
                                                    gRaceSetupMenuSubState.pendingStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_READY;
                                                    player->unkC = save->money;
                                                } else if (state == CONTROLLER_PAK_STATUS_NO_PAK) {
                                                    gRaceSetupMenuSubState.pendingStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_REPAIR;
                                                } else {
                                                    gRaceSetupMenuSubState.pendingStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_RETRY;
                                                }
                                            } else {
                                                if (state == CONTROLLER_PAK_STATUS_SAVE_FOUND) {
                                                    gRaceSetupMenuSubState.pendingStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_USE_EXISTING_SAVE;
                                                } else if (state == CONTROLLER_PAK_STATUS_NO_PAK) {
                                                    gRaceSetupMenuSubState.pendingStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_RETRY;
                                                } else {
                                                    RaceSetupSaveData *save = &gGameSaveDataBuffer[playerIndex];

                                                    gRaceSetupMenuSubState.pendingStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_READY;
                                                    initRaceSetupPlayerSaveData(playerIndex);
                                                    player->unkC = save->money;
                                                }
                                            }
                                            *choiceState += 2;
                                        }
                                    }
                                    break;

                                case 7:
                                case 8:
                                case 9:
                                case 10:
                                case 11:
                                case 12:
                                case 13:
                                case 16:
                                    input = gPlayerInputPressed[playerIndex];
                                    if ((input & A_BUTTON) || (input & START_BUTTON)) {
                                        enqueueSoundEffect(1, 0x32);
                                        *choiceState = D_800B3199[gControllerPakStatusCodes[playerIndex]];
                                    }
                                    break;

                                case 14:
                                case 15:
                                    input = gPlayerInputPressed[playerIndex];
                                    if ((input & A_BUTTON) || (input & START_BUTTON)) {
                                        enqueueSoundEffect(1, 0x32);
                                        if (gControllerPakStatusCodes[playerIndex] == CONTROLLER_PAK_STATUS_REPAIR_FAILED) {
                                            if (saveStatusTask != NULL) {
                                                gRaceSetupMenuSubState.statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_FADE_OUT;
                                                gRaceSetupMenuSubState.nextStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_REPAIR_CONFIRM;
                                            } else {
                                                gControllerPakStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_NO_PAK;
                                            }
                                        } else if (saveStatusTask != NULL) {
                                            gRaceSetupMenuSubState.statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_FADE_OUT;
                                            gRaceSetupMenuSubState.nextStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_PROBE;
                                        } else {
                                            gControllerPakStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_PROBE;
                                        }
                                    }
                                    break;

                                case CONTROLLER_PAK_STATUS_REPAIR_CONFIRM:
                                    input = gPlayerInputPressed[playerIndex];
                                    if ((input & A_BUTTON) || (input & START_BUTTON)) {
                                        enqueueSoundEffect(1, 0x32);
                                        gRaceSetupMenuSubState.statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_FADE_OUT;
                                        gRaceSetupMenuSubState.nextStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_PROBE;
                                    }
                                    break;

                                case CONTROLLER_PAK_STATUS_USE_EXISTING_SAVE:
                                    if (player->menuState == 0) {
                                        player->menuState = 1;
                                    }
                                    break;
                            }
                        }
                        allPlayersReady &= player->menuState;
                    }

                    allControllerPakOpsComplete &= operationCount;
                    playerIndex++;
                    operationCountPtr++;
                } while (playerIndex < gPlayerCount);
            }
        }
    }

    if (allControllerPakOpsComplete != 0) {
        if (savePanelTask == NULL) {
            D_800EC9E4++;
            if (D_800EC9E4 >= SAVE_PANEL_CREATE_DELAY) {
                u8 *operationCount;
                u8 *operationCountEnd;

                D_800EC9E4 = 0;
                D_8010ADE8 = createCallbackTask((CallbackTaskCallback)initRaceSetupSaveStatusWidgets, 0, 0x63);
                createCallbackTask((CallbackTaskCallback)initRaceSetupSavePanelIcons, 0, 0x63);
                D_8010ADE0 = createCallbackTask((CallbackTaskCallback)initRaceSetupSavePanelFrame, 0, 0x63);
                D_8010ADE4 = createCallbackTask((CallbackTaskCallback)initRaceSetupSaveChoicePrompts, 0, 0x63);
                if (gPlayerCount > 0) {
                    operationCount = gControllerPakOperationCounts;
                    operationCountEnd = &gControllerPakOperationCounts[gPlayerCount];
                    do {
                        *operationCount += 1;
                        operationCount++;
                    } while (operationCount < operationCountEnd);
                }
            }
        }
    }

    if (allPlayersReady != 0) {
        gMenuSelectionConfirmTimer++;
        if (gMenuSelectionConfirmTimer == SAVE_READY_CONFIRM_DELAY) {
            u8 *statusTransitionState;
            RaceSetupSaveData *save;
            RaceSetupSaveData *end;

            setCurrentGameTaskCallback(updateRaceSetupRumblePrompt, 0);
            createCallbackTask((CallbackTaskCallback)initControllerPakRumbleCheckPrompt, 0, 0x64);
            gControllerPakRumbleCheckPromptTransition.state = 6;
            gControllerPakRumbleCheckPromptTransition.selectedOption = 0;
            gControllerPakRumbleCheckPromptTransition.targetScale = 2;
            if (gPlayerCount > 0) {
                statusTransitionState = gRaceSetupMenuSubState.statusTransitionStates;
                save = gGameSaveDataBuffer;
                end = &gGameSaveDataBuffer[gPlayerCount];
                do {
                    u8 highestUnlockedCourse = save->highestUnlockedCourse;

                    *statusTransitionState = SAVE_STATUS_TRANSITION_DONE;
                    if (gHighestUnlockedCourse < highestUnlockedCourse) {
                        gHighestUnlockedCourse = highestUnlockedCourse;
                    }
                    save++;
                    statusTransitionState++;
                } while (save < end);
            }
        }
    }

    updateCallbackTasks();
}
#endif

void updateRaceSetupSaveMenu(void) {
    CallbackTaskCallback saveChoicePromptInitializer;
    s16 *choiceState;
    s32 allPlayersReady = 0;
    s16 allControllerPakOpsComplete = 0;
    s16 statusCode;
    CallbackTask *saveStatusTask = D_8010ADE8;
    CallbackTask * volatile savePanelTask;
    s32 i;
    s32 controllerIndex;

    savePanelTask = D_8010ADE0;

    if ((gRaceSetupMenuSubState.forceUpdate == 1) &&
        (gRaceSetupMenuSubState.state == CONTROLLER_PAK_STATUS_READY)) {
        gRaceSetupMenuSubState.state = 6;
    }

    if ((savePanelTask == NULL) || (gRaceSetupMenuSubState.forceUpdate != 0)) {
        if ((gRaceSetupMenuSubState.state >= 6) && (gRaceSetupMenuSubState.state < 8)) {
            if (gRaceSetupMenuSubState.state == 6) {
                if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                    enqueueSoundEffect(1, 0x32);
                    gRaceSetupMenuSubState.state = 7;
                    gRaceSetupMenuSubState.alpha = 0xFF;
                    gRaceSetupMenuSubState.timer = 0;
                }
            }
        } else {
            allPlayersReady = 1;
            allControllerPakOpsComplete = 1;
            for (i = 0; i < gPlayerCount; i++) {
                choiceState = &gMenuChoicePromptState[i];
                if (gControllerPakOperationCounts[i] != 1) {
                    if ((saveStatusTask == NULL) || (gRaceSetupMenuSubState.statusTransitionStates[i] == 0)) {
                        if (*(&gMenuChoicePromptState[i]) != 0) {
                            statusCode = CONTROLLER_PAK_STATUS_CHOICE_PROMPT;
                        } else {
                            statusCode = gControllerPakStatusCodes[i];
                        }
                        switch (statusCode) {
                            case CONTROLLER_PAK_STATUS_PROBE: {
                                s32 currentControllerIndex = i & 0xFFFF;

                                gRumblePakConnectedByController[i] = 0;
                                requestRumbleMotorInit((u16)currentControllerIndex);
                                if ((gRumbleMotorStatuses[i] != 1) && (gRumbleMotorStatuses[i] != 0xB) &&
                                    (gRumbleMotorStatuses[i] != 4)) {
                                    gRumblePakConnectedByController[i] = 1;
                                } else {
                                    gRumblePakConnectedByController[i] = 0;
                                }
                                requestControllerPakProbe((u16)currentControllerIndex);
                                break;
                            }

                            case CONTROLLER_PAK_STATUS_SAVE_STATUS:
                                requestControllerPakSaveStatus((u16)i);
                                break;

                            case CONTROLLER_PAK_STATUS_SAVE_READ: {
                                requestControllerPakSaveRead((u16)i);
                                if (gControllerPakRetryCounts[i] == 0) {
                                    gRacePlayers[i].unkC = D_800EC9F4[i].money;
                                    gRacePlayers[i].unk568 = 0;
                                    gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_SAVE_FOUND;
                                    *(&gMenuChoicePromptState[i]) = 1;
                                } else if (gControllerPakRetryCounts[i] == CONTROLLER_PAK_RETRY_LIMIT) {
                                    if (saveStatusTask != NULL) {
                                        gRaceSetupMenuSubState.statusTransitionStates[i] =
                                            SAVE_STATUS_TRANSITION_FADE_OUT;
                                        gRaceSetupMenuSubState.nextStatusCodes[i] =
                                            CONTROLLER_PAK_STATUS_READ_FAILED;
                                    } else {
                                        gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_READ_FAILED;
                                    }
                                    gControllerPakRetryCounts[i] = 0;
                                }
                                break;
                            }

                            case CONTROLLER_PAK_STATUS_REPAIR: {
                                requestControllerPakRepair((u16)i);
                                if (gControllerPakRetryCounts[i] == 0) {
                                    if (saveStatusTask != NULL) {
                                        gRaceSetupMenuSubState.statusTransitionStates[i] =
                                            SAVE_STATUS_TRANSITION_FADE_OUT;
                                        gRaceSetupMenuSubState.nextStatusCodes[i] =
                                            CONTROLLER_PAK_STATUS_REPAIRED;
                                    } else {
                                        gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_REPAIRED;
                                    }
                                } else if (gControllerPakRetryCounts[i] == CONTROLLER_PAK_RETRY_LIMIT) {
                                    if (saveStatusTask != NULL) {
                                        gRaceSetupMenuSubState.statusTransitionStates[i] =
                                            SAVE_STATUS_TRANSITION_FADE_OUT;
                                        gRaceSetupMenuSubState.nextStatusCodes[i] =
                                            CONTROLLER_PAK_STATUS_REPAIR_FAILED;
                                    } else {
                                        gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_REPAIR_FAILED;
                                    }
                                    gControllerPakRetryCounts[i] = 0;
                                }
                                break;
                            }

                            case CONTROLLER_PAK_STATUS_RETRY:
                                if ((gPlayerInputPressed[i] & A_BUTTON) || (gPlayerInputPressed[i] & START_BUTTON)) {
                                    enqueueSoundEffect(1, 0x32);
                                    if (saveStatusTask != NULL) {
                                        gRaceSetupMenuSubState.statusTransitionStates[i] =
                                            SAVE_STATUS_TRANSITION_FADE_OUT;
                                        gRaceSetupMenuSubState.nextStatusCodes[i] =
                                            CONTROLLER_PAK_STATUS_PROBE;
                                    } else {
                                        gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_PROBE;
                                    }
                                }
                                break;

                            case CONTROLLER_PAK_STATUS_READY:
                                if ((statusCode && statusCode) && statusCode) {
                                }
                                if (gRacePlayers[i].menuState == 0) {
                                    gRacePlayers[i].menuState = 1;
                                }
                                break;

                            case CONTROLLER_PAK_STATUS_CHOICE_PROMPT:
                                if ((*(&gMenuChoicePromptState[i]) == SAVE_CHOICE_USE_PAK) ||
                                    (*(&gMenuChoicePromptState[i]) == SAVE_CHOICE_SKIP_PAK)) {
                                    if ((gPlayerInputPressed[i] & (STICK_UP | U_JPAD)) &&
                                        (*(&gMenuChoicePromptState[i]) != SAVE_CHOICE_USE_PAK)) {
                                        *(&gMenuChoicePromptState[i]) = *(&gMenuChoicePromptState[i]) - 1;
                                        enqueueSoundEffect(0x19, 0x32);
                                    }
                                    if (gPlayerInputPressed[i] & (STICK_DOWN | D_JPAD)) {
                                        if (*(&gMenuChoicePromptState[i]) != SAVE_CHOICE_SKIP_PAK) {
                                            *(&gMenuChoicePromptState[i]) = *(&gMenuChoicePromptState[i]) - -1;
                                            enqueueSoundEffect(0x19, 0x32);
                                        }
                                    }
                                    if ((gPlayerInputPressed[i] & A_BUTTON) || (gPlayerInputPressed[i] & START_BUTTON)) {
                                        enqueueSoundEffect(1, 0x32);
                                        if (*(&gMenuChoicePromptState[i]) == SAVE_CHOICE_SKIP_PAK) {
                                            if (gControllerPakStatusCodes[i] == CONTROLLER_PAK_STATUS_SAVE_FOUND) {
                                                initRaceSetupPlayerSaveData(i);
                                                gRacePlayers[i].unkC = (&gGameSaveDataBuffer[i])->money;
                                                gRaceSetupMenuSubState.pendingStatusCodes[i] =
                                                    CONTROLLER_PAK_STATUS_READY;
                                            } else if (gControllerPakStatusCodes[i] == CONTROLLER_PAK_STATUS_NO_PAK) {
                                                gRaceSetupMenuSubState.pendingStatusCodes[i] =
                                                    CONTROLLER_PAK_STATUS_REPAIR;
                                            } else {
                                                gRaceSetupMenuSubState.pendingStatusCodes[i] =
                                                    CONTROLLER_PAK_STATUS_RETRY;
                                            }
                                        } else {
                                            if (gControllerPakStatusCodes[i] == CONTROLLER_PAK_STATUS_SAVE_FOUND) {
                                                gRaceSetupMenuSubState.pendingStatusCodes[i] =
                                                    CONTROLLER_PAK_STATUS_USE_EXISTING_SAVE;
                                            } else if (gControllerPakStatusCodes[i] == CONTROLLER_PAK_STATUS_NO_PAK) {
                                                gRaceSetupMenuSubState.pendingStatusCodes[i] =
                                                    CONTROLLER_PAK_STATUS_RETRY;
                                            } else {
                                                gRaceSetupMenuSubState.pendingStatusCodes[i] =
                                                    CONTROLLER_PAK_STATUS_READY;
                                                initRaceSetupPlayerSaveData(i);
                                                gRacePlayers[i].unkC = gGameSaveDataBuffer[i].money;
                                            }
                                        }
                                        *choiceState += 2;
                                    }
                                }
                                break;

                            case CONTROLLER_PAK_STATUS_NO_PAK:
                            case CONTROLLER_PAK_STATUS_SAVE_FOUND:
                            case 9:
                            case 10:
                            case 11:
                            case 12:
                            case CONTROLLER_PAK_STATUS_READ_FAILED:
                            case 16:
                                if ((gPlayerInputPressed[i] & A_BUTTON) || (gPlayerInputPressed[i] & START_BUTTON)) {
                                    enqueueSoundEffect(1, 0x32);
                                    *(&gMenuChoicePromptState[i]) = D_800B3199[gControllerPakStatusCodes[i]];
                                }
                                break;

                            case CONTROLLER_PAK_STATUS_REPAIR_FAILED:
                            case CONTROLLER_PAK_STATUS_REPAIRED:
                                if ((gPlayerInputPressed[i] & A_BUTTON) || (gPlayerInputPressed[i] & START_BUTTON)) {
                                    enqueueSoundEffect(1, 0x32);
                                    if (gControllerPakStatusCodes[i] ==
                                        CONTROLLER_PAK_STATUS_REPAIR_FAILED) {
                                        if (saveStatusTask != NULL) {
                                            gRaceSetupMenuSubState.statusTransitionStates[i] =
                                                SAVE_STATUS_TRANSITION_FADE_OUT;
                                            gRaceSetupMenuSubState.nextStatusCodes[i] =
                                                CONTROLLER_PAK_STATUS_REPAIR_CONFIRM;
                                        } else {
                                            gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_NO_PAK;
                                        }
                                    } else if (saveStatusTask != NULL) {
                                        gRaceSetupMenuSubState.statusTransitionStates[i] =
                                            SAVE_STATUS_TRANSITION_FADE_OUT;
                                        gRaceSetupMenuSubState.nextStatusCodes[i] =
                                            CONTROLLER_PAK_STATUS_PROBE;
                                    } else {
                                        gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_PROBE;
                                        break;
                                    }
                                }
                                break;

                            case CONTROLLER_PAK_STATUS_REPAIR_CONFIRM:
                                if (gPlayerInputPressed[i] & A_BUTTON || gPlayerInputPressed[i] & START_BUTTON) {
                                    enqueueSoundEffect(1, 0x32);
                                    gRaceSetupMenuSubState.statusTransitionStates[i] =
                                        SAVE_STATUS_TRANSITION_FADE_OUT;
                                    gRaceSetupMenuSubState.nextStatusCodes[i] =
                                        CONTROLLER_PAK_STATUS_PROBE;
                                }
                                break;

                            case CONTROLLER_PAK_STATUS_USE_EXISTING_SAVE:
                                if (gRacePlayers[i].menuState == 0) {
                                    gRacePlayers[i].menuState = 1;
                                }
                        }
                    }
                    allPlayersReady &= gRacePlayers[i].menuState;
                }

                allControllerPakOpsComplete &= gControllerPakOperationCounts[i];
            }
        }
    }

    if (allControllerPakOpsComplete) {
        if (savePanelTask == NULL) {
            D_800EC9E4++;
            saveChoicePromptInitializer = (CallbackTaskCallback)initRaceSetupSaveChoicePrompts;
            if (D_800EC9E4 >= SAVE_PANEL_CREATE_DELAY) {
                D_800EC9E4 = 0;
                D_8010ADE8 =
                    createCallbackTask((CallbackTaskCallback)initRaceSetupSaveStatusWidgets, 0, 0x63);
                createCallbackTask((CallbackTaskCallback)initRaceSetupSavePanelIcons, 0, 0x63);
                D_8010ADE0 =
                    createCallbackTask((CallbackTaskCallback)initRaceSetupSavePanelFrame, 0, 0x63);
                D_8010ADE4 = createCallbackTask(saveChoicePromptInitializer, 0, 0x63);

                for (controllerIndex = 0; controllerIndex < gPlayerCount;
                     controllerIndex++) {
                    gControllerPakOperationCounts[controllerIndex]++;
                }
            }
        }
    }

    if (allPlayersReady) {
        gMenuSelectionConfirmTimer++;
        if (gMenuSelectionConfirmTimer == SAVE_READY_CONFIRM_DELAY) {
            setCurrentGameTaskCallback(updateRaceSetupRumblePrompt, 0);
            createCallbackTask((CallbackTaskCallback)initControllerPakRumbleCheckPrompt, 0, 0x64);
            gControllerPakRumbleCheckPromptTransition.state = 6;
            gControllerPakRumbleCheckPromptTransition.selectedOption = 0;
            gControllerPakRumbleCheckPromptTransition.targetScale = 2;

            for (i = 0; i < gPlayerCount; i++) {
                gRaceSetupMenuSubState.statusTransitionStates[i] = SAVE_STATUS_TRANSITION_DONE;
                if (gHighestUnlockedCourse < gGameSaveDataBuffer[i].highestUnlockedCourse) {
                    gHighestUnlockedCourse = gGameSaveDataBuffer[i].highestUnlockedCourse;
                }
            }
        }
    }

    updateCallbackTasks();
}

#undef SAVE_READY_CONFIRM_DELAY
#undef SAVE_PANEL_CREATE_DELAY
#undef SAVE_STATUS_TRANSITION_DONE
#undef SAVE_STATUS_TRANSITION_FADE_OUT
#undef SAVE_CHOICE_SKIP_PAK
#undef SAVE_CHOICE_USE_PAK
#undef CONTROLLER_PAK_RETRY_LIMIT
#undef CONTROLLER_PAK_STATUS_USE_EXISTING_SAVE
#undef CONTROLLER_PAK_STATUS_REPAIR_CONFIRM
#undef CONTROLLER_PAK_STATUS_REPAIRED
#undef CONTROLLER_PAK_STATUS_REPAIR_FAILED
#undef CONTROLLER_PAK_STATUS_READ_FAILED
#undef CONTROLLER_PAK_STATUS_SAVE_FOUND
#undef CONTROLLER_PAK_STATUS_NO_PAK
#undef CONTROLLER_PAK_STATUS_READY
#undef CONTROLLER_PAK_STATUS_RETRY
#undef CONTROLLER_PAK_STATUS_REPAIR
#undef CONTROLLER_PAK_STATUS_SAVE_READ
#undef CONTROLLER_PAK_STATUS_SAVE_STATUS
#undef CONTROLLER_PAK_STATUS_PROBE
#endif

void raceSetupMenuNoop(void) {
}

// updateRaceSetupRumblePrompt best match: 99.684% (nonmatchings/updateRaceSetupRumblePrompt-1645024839200431810/base_24.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/race_setup/race_setup_menu/updateRaceSetupRumblePrompt.s")

#ifdef NON_MATCHING
typedef struct {
    /* 0x0 */ u8 state;
    /* 0x1 */ u8 pad1;
    /* 0x2 */ u16 targetScale;
} ControllerPakRumbleCheckPromptState;

ControllerPakRumbleCheckPromptState gRaceSetupRumblePromptStateCase1;
ControllerPakRumbleCheckPromptState gRaceSetupRumblePromptStateCase8;
ControllerPakRumbleCheckPromptState gRaceSetupRumblePromptStateCase9Yes;
ControllerPakRumbleCheckPromptState gRaceSetupRumblePromptStateCase9Connected;

extern ControllerPakRumbleCheckPromptTransition gControllerPakRumbleCheckPromptTransition;
extern u8 gControllerPakRumbleCheckPromptState;
extern u8 gControllerPakRumbleCheckPromptConfirmSelection;
extern void enqueueSoundEffect(s16 arg0, s32 arg1);
extern void requestRumbleMotorInit(u16 arg0);
extern s32 gRumbleMotorStatuses[];

void updateRaceSetupRumblePrompt(void) {
    s32 statusIndex;
    s32 connectedCount;
    s32 i;
    s32 state;

    state = gControllerPakRumbleCheckPromptTransition.state;
    switch (state) {
        case 0:
        case 4:
        case 5:
            break;
        case 1:
            if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                state = 2;
                enqueueSoundEffect(1, 0x32);
                gRaceSetupRumblePromptStateCase1.state = state;
                gRaceSetupRumblePromptStateCase1.targetScale = 1;
                state = 2;
            }
            break;
        case 2:
            gRumblePakConnectedMask = 0;
            i = 0;
            if ((s32)gPlayerCount > 0) {
                do {
                    requestRumbleMotorInit(i);
                    if ((gRumbleMotorStatuses[i] != 1) && (gRumbleMotorStatuses[i] != 0xB) && (gRumbleMotorStatuses[i] != 4)) {
                        gRumblePakConnectedByController[i] = 1;
                        gRumblePakConnectedMask |= 1 << i;
                    } else {
                        gRumblePakConnectedByController[i] = 0;
                    }
                    i++;
                } while (i < (s32)gPlayerCount);
            }
            gControllerPakRumbleCheckPromptTransition.state = 7;
            gControllerPakRumbleCheckPromptTransition.timer = 0x11;
            state = 7;
            state = gControllerPakRumbleCheckPromptTransition.state;
            break;
        case 3:
            if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                enqueueSoundEffect(1, 0x32);
                gControllerPakRumbleCheckPromptState = 4;
                state = 4;
            }
            break;
        case 6:
            connectedCount = 0;
            gRumblePakConnectedMask = 0;
            i = 0;
            if ((s32)gPlayerCount > 0) {
                do {
                    statusIndex = i;
                    requestRumbleMotorInit(i);
                    if ((gRumbleMotorStatuses[statusIndex] != 1) && (gRumbleMotorStatuses[i] != 0xB) && (gRumbleMotorStatuses[i] != 4)) {
                        gRumblePakConnectedMask |= 1 << i;
                        gRumblePakConnectedByController[i] = 1;
                        connectedCount++;
                    } else {
                        gRumblePakConnectedByController[i] = 0;
                    }
                    i++;
                } while (i < (s32)gPlayerCount);
            }
            if (connectedCount == gPlayerCount) {
                gControllerPakRumbleCheckPromptTransition.selectedOption = 1;
                gControllerPakRumbleCheckPromptTransition.targetScale = 2;
            } else {
                gControllerPakRumbleCheckPromptTransition.selectedOption = 0;
                gControllerPakRumbleCheckPromptTransition.targetScale = 0;
            }
            state = (gControllerPakRumbleCheckPromptTransition.state = 0);
            break;
        case 7:
            gControllerPakRumbleCheckPromptTransition.timer--;
            if (gControllerPakRumbleCheckPromptTransition.timer == 0) {
                gControllerPakRumbleCheckPromptTransition.state = 8;
                gControllerPakRumbleCheckPromptTransition.targetScale = 2;
                state = 8;
                state = gControllerPakRumbleCheckPromptTransition.state;
            }
            break;
        case 8:
            if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                state = 9;
                enqueueSoundEffect(1, 0x32);
                gRaceSetupRumblePromptStateCase8.state = state;
                gRaceSetupRumblePromptStateCase8.targetScale = 3;
                gControllerPakRumbleCheckPromptConfirmSelection = 1;
                state = 9;
            }
            break;
        case 9:
            if ((gPlayerInputPressed[0] & (STICK_UP | U_JPAD)) && (gControllerPakRumbleCheckPromptTransition.confirmSelection != 0)) {
                gControllerPakRumbleCheckPromptTransition.confirmSelection--;
                enqueueSoundEffect(0x19, 0x32);
            } else if (gPlayerInputPressed[0] & (STICK_DOWN | D_JPAD)) {
                if (gControllerPakRumbleCheckPromptTransition.confirmSelection != 1) {
                    gControllerPakRumbleCheckPromptTransition.confirmSelection++;
                    enqueueSoundEffect(0x19, 0x32);
                }
            }
            if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                enqueueSoundEffect(1, 0x32);
                statusIndex = 1;
                if (((((((((((gControllerPakRumbleCheckPromptConfirmSelection & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) &
                             0xFFu) &
                            0xFFu) &
                           0xFFFF) &
                          0xFFu) &
                         0xFFu) &
                        0xFFu) == 1) {
                    gRaceSetupRumblePromptStateCase9Yes.state = statusIndex;
                    gRaceSetupRumblePromptStateCase9Yes.targetScale =
                        (gControllerPakRumbleCheckPromptTransition.confirmSelection != 1) * 0;
                } else {
                    connectedCount = 0;
                    i = 0;
                    if ((s32)gPlayerCount > 0) {
                        do {
                            if (gRumblePakConnectedByController[i] == 1) {
                                connectedCount++;
                            }
                            i++;
                        } while (i < (s32)gPlayerCount);
                    }
                    i = 4;
                    statusIndex = 3;
                    if (connectedCount > 0) {
                        gRaceSetupRumblePromptStateCase9Connected.state = statusIndex;
                        gRaceSetupRumblePromptStateCase9Connected.targetScale = 2;
                    } else {
                        gControllerPakRumbleCheckPromptState = i;
                    }
                }
            }
            state = gControllerPakRumbleCheckPromptState;
            break;
    }
    if (state == 5) {
        setCurrentGameTaskCallback(initCharacterSelectMenu, 0);
        gControllerPakRumbleCheckPromptTransition.confirmSelection = gControllerPakRumbleCheckPromptTransition.confirmSelection;
    }
    updateCallbackTasks();
}
#endif

// initRaceSetupPlayerSaveData best match: 78.654% (nonmatchings/initRaceSetupPlayerSaveData-8498672362023432715/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/race_setup/race_setup_menu/initRaceSetupPlayerSaveData.s")

#ifdef NON_MATCHING
typedef struct {
    s8 unk0;
    s8 unk1;
    s8 unk2;
} RaceSetupSaveTriplet045D8;

extern RaceSetupSaveTriplet045D8 D_800B31C8[];
extern RaceSetupSaveTriplet045D8 D_800B3270[];
extern RaceSetupSaveTriplet045D8 D_800B3294[];
extern RaceSetupSaveTriplet045D8 D_800B32A4[];
extern u8 D_800B32C4[];
extern u16 D_800B32D0[];
extern u8 gMainMenuSecretCodeUnlocked;

#define RACE_SETUP_SAVE_WORD_045D8(ptr, offset) (*(s32 *)((ptr) + (offset)))
#define RACE_SETUP_SAVE_HALF_045D8(ptr, offset) (*(s16 *)((ptr) + (offset)))
#define RACE_SETUP_SAVE_BYTE_045D8(ptr, offset) (*((s8 *)(ptr) + (offset)))
#define RACE_SETUP_SAVE_UBYTE_045D8(ptr, offset) (*((u8 *)(ptr) + (offset)))

void initRaceSetupPlayerSaveData(s32 arg0) {
    u8 *sp30;
    s32 i;
    s32 course;
    s32 recordOffset;
    u8 *save = (u8 *)&gGameSaveDataBuffer[arg0 & 0xFFFFFFFFFFFFFFFF];
    u8 *base = (u8 *)&gGameSaveDataBuffer[arg0];
    s8 *wordCursor;
    u8 *byteCursor;
    u8 *courseCursor0;
    u8 *courseCursor1;
    u8 *courseCursor2;
    u8 *courseCursor3;
    u8 *scoreCursor;
    RaceSetupSaveTriplet045D8 *courseTimes;
    RaceSetupSaveTriplet045D8 *courseRows;
    RaceSetupSaveTriplet045D8 *specialRecords;
    RaceSetupSaveTriplet045D8 *records;

    RACE_SETUP_SAVE_WORD_045D8(save, 0) = 0;
    RACE_SETUP_SAVE_WORD_045D8(save, 4) = 0;

    wordCursor = base;
    byteCursor = base;
    i = 0;
    do {
        i++;
        RACE_SETUP_SAVE_WORD_045D8(wordCursor, 8) = 0;
        wordCursor += 4;
        byteCursor++;
        RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x33) = 0;
    } while (i < 0xB);

    i = 0;
    byteCursor = base;
    do {
        RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x3F) = i;
        i++;
        byteCursor++;
    } while (i < 3);

    RACE_SETUP_SAVE_BYTE_045D8(base, 0x42) = -1;

    i = 4;
    byteCursor = base + 4;
    do {
        i += 4;
        RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x40) = -1;
        RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x41) = -1;
        RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x42) = -1;
        byteCursor += 4;
        RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x3B) = -1;
    } while (i != 0xC);

    RACE_SETUP_SAVE_BYTE_045D8(save, 0x4B) = 0;
    RACE_SETUP_SAVE_BYTE_045D8(save, 0x4C) = 0;
    RACE_SETUP_SAVE_BYTE_045D8(save, 0x78D7) = 0;

    specialRecords = D_800B3294;
    records = D_800B32A4;
    course = 0;
    courseCursor0 = base;
    courseTimes = D_800B31C8;
    courseCursor1 = base;
    courseCursor2 = base;
    courseCursor3 = base;
    courseRows = D_800B3270;
    sp30 = base;

    do {
        u8 *rankIcons = D_800B32C4;
        u16 *recordText = D_800B32D0;
        s8 row0;
        s8 row1;
        s8 row2;

        i = 0;
        byteCursor = courseCursor0;
        recordOffset = 0;
        wordCursor = (u8 *)courseTimes;
        base = courseCursor1;
        scoreCursor = courseCursor2;
        if (1) {
        do {
            RaceSetupSaveTriplet045D8 *record;

            RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x4F) = ((RaceSetupSaveTriplet045D8 *)wordCursor)->unk1;
            RACE_SETUP_SAVE_HALF_045D8(byteCursor, 0x50) = ((RaceSetupSaveTriplet045D8 *)wordCursor)->unk2 << 8;
            RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x4E) = ((RaceSetupSaveTriplet045D8 *)wordCursor)->unk0;
            RACE_SETUP_SAVE_BYTE_045D8(base, 0x77FB) = 0x10;
            RACE_SETUP_SAVE_UBYTE_045D8(base, 0x77FB) += i;
            if (course == 9) {
                record = (RaceSetupSaveTriplet045D8 *)((u8 *)specialRecords + recordOffset);
                RACE_SETUP_SAVE_HALF_045D8(byteCursor, 0x158) = record->unk2 << 8;
                RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x156) = record->unk0;
                RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x157) = record->unk1;
            } else {
                record = (RaceSetupSaveTriplet045D8 *)((u8 *)records + recordOffset + ((course & 1) * 0xF));
                RACE_SETUP_SAVE_HALF_045D8(byteCursor, 0x158) = record->unk2 << 8;
                RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x156) = record->unk0;
                RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x157) = record->unk1;
            }
            RACE_SETUP_SAVE_BYTE_045D8(base, 0x78A0) = 0x10;
            RACE_SETUP_SAVE_UBYTE_045D8(base, 0x78A0) += i;
            if (course == 9) {
                RACE_SETUP_SAVE_BYTE_045D8(base, 0x7832) = rankIcons[0];
            } else {
                RACE_SETUP_SAVE_BYTE_045D8(base, 0x7832) = rankIcons[5];
            }
            RACE_SETUP_SAVE_BYTE_045D8(base, 0x7869) = i;
            RACE_SETUP_SAVE_HALF_045D8(scoreCursor, 0x7756) = *recordText;
            RACE_SETUP_SAVE_BYTE_045D8(base, 0x77C4) = i;
            i++;
            byteCursor += 4;
            recordOffset += 3;
            wordCursor += 3;
            base++;
            rankIcons++;
            scoreCursor += 2;
            recordText++;
        } while (i < 5);
        }

        row0 = courseRows->unk0;
        row1 = courseRows->unk1;
        row2 = courseRows->unk2;
        course++;
        courseCursor0 += 0x14;
        courseTimes += 5;
        courseCursor1 += 5;
        courseCursor2 += 0xA;
        courseCursor3 += 4;
        courseRows++;
        RACE_SETUP_SAVE_BYTE_045D8(courseCursor3, 0x126) = row0;
        RACE_SETUP_SAVE_BYTE_045D8(courseCursor3, 0x127) = row1;
        RACE_SETUP_SAVE_HALF_045D8(courseCursor3, 0x128) = row2;
    } while (course < 0xB);

    RACE_SETUP_SAVE_HALF_045D8(save, 0x232) = 0;
    RACE_SETUP_SAVE_HALF_045D8(save, 0x236) = 0;
    RACE_SETUP_SAVE_HALF_045D8(save, 0x23A) = 0;
    RACE_SETUP_SAVE_HALF_045D8(save, 0x23E) = 0;
    RACE_SETUP_SAVE_HALF_045D8(save, 0x242) = 0;
    RACE_SETUP_SAVE_HALF_045D8(save, 0x246) = 0;
    RACE_SETUP_SAVE_HALF_045D8(save, 0x24A) = 0;
    RACE_SETUP_SAVE_HALF_045D8(save, 0x24E) = 0;
    RACE_SETUP_SAVE_HALF_045D8(save, 0x252) = 0;

    i = 0;
    byteCursor = sp30;
    do {
        i++;
        byteCursor++;
        RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x78D7) = 0;
    } while (i < 0x20);

    i = 0;
    if (gMainMenuSecretCodeUnlocked == 1) {
        byteCursor = sp30;
secret_unlock_loop:
            i++;
            RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x34) = 1;
            byteCursor++;
            if (i < 0xB) {
                goto secret_unlock_loop;
            }

        byteCursor = sp30;
        wordCursor = D_800B3490;
secret_copy_loop:
        {
            s8 temp0 = wordCursor[0];
            s8 temp1 = wordCursor[1];
            s8 temp2 = wordCursor[2];
            s8 temp3 = wordCursor[3];

            wordCursor += 4;
            byteCursor += 4;
            RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x3B) = temp0;
            RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x3C) = temp1;
            RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x3D) = temp2;
            RACE_SETUP_SAVE_BYTE_045D8(byteCursor, 0x3E) = temp3;
            if (wordCursor != &D_800B3490[MULTIPLAYER_COURSE_SELECT_DEFAULT_COURSE_COUNT]) {
                goto secret_copy_loop;
            }
        }

        RACE_SETUP_SAVE_BYTE_045D8(save, 0x4B) = 1;
        RACE_SETUP_SAVE_BYTE_045D8(save, 0x4C) = 3;
        RACE_SETUP_SAVE_BYTE_045D8(save, 0x78D7) = 0x3F;
    }
}

#undef RACE_SETUP_SAVE_WORD_045D8
#undef RACE_SETUP_SAVE_HALF_045D8
#undef RACE_SETUP_SAVE_BYTE_045D8
#undef RACE_SETUP_SAVE_UBYTE_045D8
#endif
