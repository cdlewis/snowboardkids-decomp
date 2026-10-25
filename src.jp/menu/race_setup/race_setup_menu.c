#include "common.h"
#include "game/menu/menu_scratch.h"
#include "game/race/race_state.h"
#include "font_encoding.h"
#include "game/save_data.h"
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
#include "game/menu/controller_pak/controller_pak_ui.h"
#include "game/menu/course_select/course_select_menu.h"
#include "game/menu/course_select/multiplayer_course_select_menu.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/engine/system_runtime.h"
#include "game/menu/main_menu/controller_subsystem.h"

#define SAVE_CHOICE_USE_PAK 3
#define SAVE_CHOICE_SKIP_PAK 4
#define SAVE_STATUS_TRANSITION_FADE_OUT 2
#define SAVE_STATUS_TRANSITION_DONE 3
#define SAVE_PANEL_CREATE_DELAY 5
#define SAVE_READY_CONFIRM_DELAY 0xF
/* The transition fields own the storage; the weak view preserves IDO code generation. */

#pragma weak gControllerPakRumbleCheckPromptState

ControllerPakRumbleCheckPromptState gControllerPakRumbleCheckPromptState = { 0 };

RaceSetupMenuSubState gRaceSetupMenuSubState;

s16 gRaceSetupSavePanelRects[2][4];

void initRaceSetupMenu(void) {
    s32 i;

    requestMusicSequenceBank(1);
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);

    gFramebufferRenderInterval.value = 0;
    gCurrentGameTask->callbackData0 = 0;
    gCurrentGameTask->callbackData1 = 0;
    gMenuSelectionConfirmTimer = 0;
    D_800EBD05 = 1;
    gCourseSelectModeSelection = 0;
    gHighestUnlockedCourse = 0;
    gRaceRumbleEnabled = 0;
    gRaceCourseIndex.signedValue = RACE_COURSE_ROOKIE_MOUNTAIN;
    gMenuFlowState = 0;
    gMenuInputRepeatTimers[0] = 0;
    gPlayerCount = 1;
    gMenuFadeAlpha = gCurrentGameTask->callbackData0;

    LOAD_ASSET(_59AAA0, 0x21);
    LOAD_ASSET(SHARED_UI_SPRITES, 0x1F);
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
    s32 pressedInput;
    s32 substateNext;
    s32 temp_v0;
    u8 temp_v1;
    u32 confirmationValue;
    u16 repeatTimer;
    s32 one;

    one = 1;
    substateNext = 2;
    if ((gRaceSetupMenuSubState.state == one) &&
        ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON))) {
        enqueueSoundEffect(one, SFX_PRIORITY_NORMAL);
        gRaceSetupMenuSubState.state = substateNext;
        gRaceSetupMenuSubState.timer = 0;
    }
    if (gCurrentGameTask->callbackData1 == one) {
        temp_v1 = gMenuSelectionConfirmTimer;
        confirmationValue = temp_v1;
        temp_v0 = temp_v1;
        if (temp_v1 == 0) {
            pressedInput = gPlayerInputPressed[0];
            temp_v0 = gPlayerInputHeld[0] & (STICK_UP | U_JPAD);
            if ((temp_v0 == 0) && !(gPlayerInputHeld[0] & (STICK_DOWN | D_JPAD))) {
                gMenuInputRepeatTimers[0] = 0;
            }
            if ((pressedInput & (STICK_UP | U_JPAD)) || ((temp_v0 != 0) && ((s32)gMenuInputRepeatTimers[0] >= 9) &&
                                                         (((s32)gMenuInputRepeatTimers[0] % 3) == 0))) {
                if (gMenuInputRepeatTimers[0] == 0) {
                    gMenuInputRepeatTimers[0] += 1;
                }
                if (gPlayerCount != one) {
                    gPlayerCount -= 1;
                    enqueueSoundEffect(SFX_MENU_CURSOR, SFX_PRIORITY_NORMAL);
                }
            } else if (
                (pressedInput & (STICK_DOWN | D_JPAD)) ||
                ((gPlayerInputHeld[0] & (STICK_DOWN | D_JPAD)) && ((s32)gMenuInputRepeatTimers[0] >= 9) &&
                 (((s32)gMenuInputRepeatTimers[0] % 3) == 0))
            ) {
                if (gMenuInputRepeatTimers[0] == 0) {
                    gMenuInputRepeatTimers[0] += 1;
                }
                if (gPlayerCount != gConnectedControllerCount) {
                    gPlayerCount += 1;
                    enqueueSoundEffect(SFX_MENU_CURSOR, SFX_PRIORITY_NORMAL);
                }
            }
            repeatTimer = gMenuInputRepeatTimers[0];
            if (repeatTimer != 0) {
                gMenuInputRepeatTimers[0] = repeatTimer + 1;
                if (gMenuInputRepeatTimers[0] == 0xFFFF) {
                    gMenuInputRepeatTimers[0] = 0xA;
                }
            }
            if ((pressedInput & A_BUTTON) || (pressedInput & START_BUTTON)) {
                gMenuSelectionConfirmTimer = 1;
                enqueueSoundEffect(SFX_CONFIRM, SFX_PRIORITY_NORMAL);
            }
        } else if (temp_v0 < 0x13) {
            gMenuSelectionConfirmTimer = (confirmationNext = confirmationValue + 1);
        }
    }
    if (gRaceSetupMenuSubState.state == RACE_SETUP_PROMPT_CHECKING) {
        setCurrentGameTaskCallback(initRaceSetupSaveMenu, 0);
        gCurrentGameTask->callbackData0 = 0;
        gMenuSelectionConfirmTimer = 0;
    }
    updateCallbackTasks();
}

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

    gRaceSetupSavePanelCreateTimer = 0;
    // clang-format off
    do { \
        i = 0; \
        connectedControllerCount = gConnectedControllerCount; \
        if (connectedControllerCount > 0) { \
            player = gRacePlayers; \
            do { \
                player++; \
                player[-1].menuState = 0; \
                i++; \
            } while (player < &gRacePlayers[connectedControllerCount]); \
            i = 0; \
        } \
        do { \
            initRaceSetupPlayerSaveData(i); \
            i++; \
        } while (i < 4); \
        gMenuScratch0.task = 0; \
        gMenuScratch1.task = 0; \
    } while (0);
    // clang-format on
    gMenuScratch2.task = 0;
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

void updateRaceSetupSaveMenu(void) {
    CallbackTaskCallback saveChoicePromptInitializer;
    s16 *choiceState;
    s32 allPlayersReady = 0;
    s16 allControllerPakOpsComplete = 0;
    s16 statusCode;
    CallbackTask *saveStatusTask = gMenuScratch2.task;
    CallbackTask *volatile savePanelTask;
    s32 i;
    s32 controllerIndex;

    savePanelTask = gMenuScratch0.task;

    if ((gRaceSetupMenuSubState.forceUpdate == 1) && (gRaceSetupMenuSubState.state == RACE_SETUP_PROMPT_CHECKING)) {
        gRaceSetupMenuSubState.state = RACE_SETUP_PROMPT_CHECK_COMPLETE;
    }

    if ((savePanelTask == NULL) || (gRaceSetupMenuSubState.forceUpdate != 0)) {
        if ((gRaceSetupMenuSubState.state >= RACE_SETUP_PROMPT_CHECK_COMPLETE) &&
            (gRaceSetupMenuSubState.state < RACE_SETUP_PROMPT_DISMISSED)) {
            if (gRaceSetupMenuSubState.state == RACE_SETUP_PROMPT_CHECK_COMPLETE) {
                if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                    enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);
                    gRaceSetupMenuSubState.state = RACE_SETUP_PROMPT_COMPLETION_FADE;
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
                                    gRacePlayers[i].money = gGameSaveDataBuffer[i].money;
                                    gRacePlayers[i].state.score = 0;
                                    gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_SAVE_FOUND;
                                    *(&gMenuChoicePromptState[i]) = 1;
                                } else if (gControllerPakRetryCounts[i] == CONTROLLER_PAK_RETRY_LIMIT) {
                                    if (saveStatusTask != NULL) {
                                        gRaceSetupMenuSubState.statusTransitionStates[i] =
                                            SAVE_STATUS_TRANSITION_FADE_OUT;
                                        gRaceSetupMenuSubState.nextStatusCodes[i] = CONTROLLER_PAK_STATUS_READ_FAILED;
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
                                        gRaceSetupMenuSubState.nextStatusCodes[i] = CONTROLLER_PAK_STATUS_REPAIRED;
                                    } else {
                                        gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_REPAIRED;
                                    }
                                } else if (gControllerPakRetryCounts[i] == CONTROLLER_PAK_RETRY_LIMIT) {
                                    if (saveStatusTask != NULL) {
                                        gRaceSetupMenuSubState.statusTransitionStates[i] =
                                            SAVE_STATUS_TRANSITION_FADE_OUT;
                                        gRaceSetupMenuSubState.nextStatusCodes[i] = CONTROLLER_PAK_STATUS_REPAIR_FAILED;
                                    } else {
                                        gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_REPAIR_FAILED;
                                    }
                                    gControllerPakRetryCounts[i] = 0;
                                }
                                break;
                            }

                            case CONTROLLER_PAK_STATUS_RETRY:
                                if ((gPlayerInputPressed[i] & A_BUTTON) || (gPlayerInputPressed[i] & START_BUTTON)) {
                                    enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);
                                    if (saveStatusTask != NULL) {
                                        gRaceSetupMenuSubState.statusTransitionStates[i] =
                                            SAVE_STATUS_TRANSITION_FADE_OUT;
                                        gRaceSetupMenuSubState.nextStatusCodes[i] = CONTROLLER_PAK_STATUS_PROBE;
                                    } else {
                                        gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_PROBE;
                                    }
                                }
                                break;

                            case CONTROLLER_PAK_STATUS_READY:
                                if ((statusCode && statusCode) && statusCode) {}
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
                                        enqueueSoundEffect(SFX_MENU_CURSOR, SFX_PRIORITY_NORMAL);
                                    }
                                    if (gPlayerInputPressed[i] & (STICK_DOWN | D_JPAD)) {
                                        if (*(&gMenuChoicePromptState[i]) != SAVE_CHOICE_SKIP_PAK) {
                                            *(&gMenuChoicePromptState[i]) = *(&gMenuChoicePromptState[i]) - -1;
                                            enqueueSoundEffect(SFX_MENU_CURSOR, SFX_PRIORITY_NORMAL);
                                        }
                                    }
                                    if ((gPlayerInputPressed[i] & A_BUTTON) ||
                                        (gPlayerInputPressed[i] & START_BUTTON)) {
                                        enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);
                                        if (*(&gMenuChoicePromptState[i]) == SAVE_CHOICE_SKIP_PAK) {
                                            if (gControllerPakStatusCodes[i] == CONTROLLER_PAK_STATUS_SAVE_FOUND) {
                                                initRaceSetupPlayerSaveData(i);
                                                gRacePlayers[i].money = gGameSaveDataBuffer[i].money;
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
                                                gRacePlayers[i].money = gGameSaveDataBuffer[i].money;
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
                                    enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);
                                    *(&gMenuChoicePromptState[i]) =
                                        gRaceSetupMenuPackedData.choicePromptStates[gControllerPakStatusCodes[i]];
                                }
                                break;

                            case CONTROLLER_PAK_STATUS_REPAIR_FAILED:
                            case CONTROLLER_PAK_STATUS_REPAIRED:
                                if ((gPlayerInputPressed[i] & A_BUTTON) || (gPlayerInputPressed[i] & START_BUTTON)) {
                                    enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);
                                    if (gControllerPakStatusCodes[i] == CONTROLLER_PAK_STATUS_REPAIR_FAILED) {
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
                                        gRaceSetupMenuSubState.nextStatusCodes[i] = CONTROLLER_PAK_STATUS_PROBE;
                                    } else {
                                        gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_PROBE;
                                        break;
                                    }
                                }
                                break;

                            case CONTROLLER_PAK_STATUS_REPAIR_CONFIRM:
                                if (gPlayerInputPressed[i] & A_BUTTON || gPlayerInputPressed[i] & START_BUTTON) {
                                    enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);
                                    gRaceSetupMenuSubState.statusTransitionStates[i] = SAVE_STATUS_TRANSITION_FADE_OUT;
                                    gRaceSetupMenuSubState.nextStatusCodes[i] = CONTROLLER_PAK_STATUS_PROBE;
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
            gRaceSetupSavePanelCreateTimer++;
            saveChoicePromptInitializer = (CallbackTaskCallback)initRaceSetupSaveChoicePrompts;
            if (gRaceSetupSavePanelCreateTimer >= SAVE_PANEL_CREATE_DELAY) {
                gRaceSetupSavePanelCreateTimer = 0;
                gMenuScratch2.task = createCallbackTask((CallbackTaskCallback)initRaceSetupSaveStatusWidgets, 0, 0x63);
                createCallbackTask((CallbackTaskCallback)initRaceSetupSavePanelIcons, 0, 0x63);
                gMenuScratch0.task = createCallbackTask((CallbackTaskCallback)initRaceSetupSavePanelFrame, 0, 0x63);
                gMenuScratch1.task = createCallbackTask(saveChoicePromptInitializer, 0, 0x63);

                for (controllerIndex = 0; controllerIndex < gPlayerCount; controllerIndex++) {
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
            gControllerPakRumbleCheckPromptTransition.state = CONTROLLER_PAK_RUMBLE_INITIAL_PROBE;
            gControllerPakRumbleCheckPromptTransition.selectedOption = 0;
            gControllerPakRumbleCheckPromptTransition.messageIndex = CONTROLLER_PAK_RUMBLE_MESSAGE_DO_NOT_REMOVE;

            for (i = 0; i < gPlayerCount; i++) {
                gRaceSetupMenuSubState.statusTransitionStates[i] = SAVE_STATUS_TRANSITION_DONE;
                if (gHighestUnlockedCourse < gGameSaveDataBuffer[i].progressionLevel) {
                    gHighestUnlockedCourse = gGameSaveDataBuffer[i].progressionLevel;
                }
            }
        }
    }

    updateCallbackTasks();
}

void raceSetupMenuNoop(void) {
}

void updateRaceSetupRumblePrompt(void) {
    s32 statusIndex;
    s32 connectedCount;
    s32 i;
    s32 state;
    s32 one;

    state = gControllerPakRumbleCheckPromptTransition.state;
    switch (state) {
        case CONTROLLER_PAK_RUMBLE_OPEN:
        case CONTROLLER_PAK_RUMBLE_FADE_OUT:
        case CONTROLLER_PAK_RUMBLE_DISMISSED:
            break;

        case CONTROLLER_PAK_RUMBLE_INSERT_ACK:
            if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                state = CONTROLLER_PAK_RUMBLE_CHECK_MOTORS;
                enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);
                gControllerPakRumbleCheckPromptState.state = state;
                gControllerPakRumbleCheckPromptState.messageIndex = CONTROLLER_PAK_RUMBLE_MESSAGE_CHECKING;
                state = CONTROLLER_PAK_RUMBLE_CHECK_MOTORS;
            }
            break;

        case CONTROLLER_PAK_RUMBLE_CHECK_MOTORS:
            gRumblePakConnectedMask = 0;
            for (i = 0; i < (s32)gPlayerCount; i++) {
                requestRumbleMotorInit(i);
                if ((gRumbleMotorStatuses[i] != 1) && (gRumbleMotorStatuses[i] != 0xB) &&
                    (gRumbleMotorStatuses[i] != 4)) {
                    gRumblePakConnectedMask |= 1 << i;
                    gRumblePakConnectedByController[i] = 1;
                } else {
                    gRumblePakConnectedByController[i] = 0;
                }
            }
            gControllerPakRumbleCheckPromptTransition.state = CONTROLLER_PAK_RUMBLE_CHECK_HOLD;
            gControllerPakRumbleCheckPromptTransition.timer = 0x11;
            state = CONTROLLER_PAK_RUMBLE_CHECK_HOLD;
            state = gControllerPakRumbleCheckPromptTransition.state;
            break;

        case CONTROLLER_PAK_RUMBLE_WARNING_ACK:
            if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);
                gControllerPakRumbleCheckPromptState.state = CONTROLLER_PAK_RUMBLE_FADE_OUT;
                state = CONTROLLER_PAK_RUMBLE_FADE_OUT;
            }
            break;

        case CONTROLLER_PAK_RUMBLE_INITIAL_PROBE:
            connectedCount = 0;
            gRumblePakConnectedMask = 0;
            for (i = 0; i < (s32)gPlayerCount; i++) {
                statusIndex = i;
                requestRumbleMotorInit(i);
                if ((gRumbleMotorStatuses[statusIndex] != 1) && (gRumbleMotorStatuses[i] != 0xB) &&
                    (gRumbleMotorStatuses[i] != 4)) {
                    gRumblePakConnectedMask |= 1 << i;
                    gRumblePakConnectedByController[i] = 1;
                    connectedCount++;
                } else {
                    gRumblePakConnectedByController[i] = 0;
                }
            }
            if (connectedCount == gPlayerCount) {
                gControllerPakRumbleCheckPromptTransition.selectedOption = 1;
                gControllerPakRumbleCheckPromptTransition.messageIndex = CONTROLLER_PAK_RUMBLE_MESSAGE_DO_NOT_REMOVE;
            } else {
                gControllerPakRumbleCheckPromptTransition.selectedOption = 0;
                gControllerPakRumbleCheckPromptTransition.messageIndex = CONTROLLER_PAK_RUMBLE_MESSAGE_INSERT;
            }
            state = (gControllerPakRumbleCheckPromptTransition.state = CONTROLLER_PAK_RUMBLE_OPEN);
            break;

        case CONTROLLER_PAK_RUMBLE_CHECK_HOLD:
            gControllerPakRumbleCheckPromptTransition.timer--;
            if (gControllerPakRumbleCheckPromptTransition.timer == 0) {
                gControllerPakRumbleCheckPromptTransition.state = CONTROLLER_PAK_RUMBLE_DEVICE_SUMMARY;
                gControllerPakRumbleCheckPromptTransition.messageIndex = CONTROLLER_PAK_RUMBLE_MESSAGE_DO_NOT_REMOVE;
                state = CONTROLLER_PAK_RUMBLE_DEVICE_SUMMARY;
                state = gControllerPakRumbleCheckPromptTransition.state;
            }
            break;

        case CONTROLLER_PAK_RUMBLE_DEVICE_SUMMARY:
            if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                state = CONTROLLER_PAK_RUMBLE_CONFIRM_DEVICES;
                enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);
                gControllerPakRumbleCheckPromptState.state = state;
                gControllerPakRumbleCheckPromptState.messageIndex = CONTROLLER_PAK_RUMBLE_MESSAGE_CONFIRM;
                gControllerPakRumbleCheckPromptConfirmSelection = 1;
                state = CONTROLLER_PAK_RUMBLE_CONFIRM_DEVICES;
            }
            break;

        case CONTROLLER_PAK_RUMBLE_CONFIRM_DEVICES:
            if ((gPlayerInputPressed[0] & (STICK_UP | U_JPAD)) &&
                (gControllerPakRumbleCheckPromptTransition.confirmSelection != 0)) {
                gControllerPakRumbleCheckPromptTransition.confirmSelection--;
                enqueueSoundEffect(SFX_MENU_CURSOR, SFX_PRIORITY_NORMAL);
            } else if (gPlayerInputPressed[0] & (STICK_DOWN | D_JPAD)) {
                if (gControllerPakRumbleCheckPromptTransition.confirmSelection != 1) {
                    gControllerPakRumbleCheckPromptTransition.confirmSelection++;
                    enqueueSoundEffect(SFX_MENU_CURSOR, SFX_PRIORITY_NORMAL);
                }
            }
            one = 1;
            if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                enqueueSoundEffect(one, SFX_PRIORITY_NORMAL);
                statusIndex = 1;
                if (gControllerPakRumbleCheckPromptConfirmSelection == one) {
                    gControllerPakRumbleCheckPromptState.state = CONTROLLER_PAK_RUMBLE_INSERT_ACK;
                    gControllerPakRumbleCheckPromptState.messageIndex = (statusIndex != 1) * 0;
                } else {
                    connectedCount = 0;
                    for (i = 0; i < (s32)gPlayerCount; i++) {
                        if (gRumblePakConnectedByController[i] == one) {
                            connectedCount++;
                        }
                    }
                    i = 4;
                    statusIndex = 3;
                    if (connectedCount > 0) {
                        gControllerPakRumbleCheckPromptState.state = statusIndex;
                        gControllerPakRumbleCheckPromptState.messageIndex = CONTROLLER_PAK_RUMBLE_MESSAGE_DO_NOT_REMOVE;
                    } else {
                        gControllerPakRumbleCheckPromptState.state = i;
                    }
                }
            }
            state = gControllerPakRumbleCheckPromptState.state;
            break;
    }

    if (state == CONTROLLER_PAK_RUMBLE_DISMISSED) {
        setCurrentGameTaskCallback(initCharacterSelectMenu, 0);
        gControllerPakRumbleCheckPromptTransition.confirmSelection =
            gControllerPakRumbleCheckPromptTransition.confirmSelection;
    }
    updateCallbackTasks();
}

void initRaceSetupPlayerSaveData(s32 playerIndex) {
    s32 course;
    s32 record;

    gGameSaveDataBuffer[playerIndex].checksum = 0;
    gGameSaveDataBuffer[playerIndex].highScores[0] = 0;
    for (record = 0; record < 11; record++) {
        gGameSaveDataBuffer[playerIndex].highScores[record + 1] = 0;
        gGameSaveDataBuffer[playerIndex].cupPlacements[record] = 0;
    }
    for (record = 0; record < 3; record++) {
        gGameSaveDataBuffer[playerIndex].courseUnlockStates[record] = record;
    }
    for (record = 3; record < 12; record++) {
        gGameSaveDataBuffer[playerIndex].courseUnlockStates[record] = -1;
    }
    gGameSaveDataBuffer[playerIndex].characterFlags = 0;
    gGameSaveDataBuffer[playerIndex].progressionLevel = 0;
    gGameSaveDataBuffer[playerIndex].extraCourseUnlockFlags = 0;

    for (course = 0; course < 11; course++) {
        for (record = 0; record < 5; record++) {
            gGameSaveDataBuffer[playerIndex].timeTrialRecords[course][record].minutes =
                gDefaultTimeTrialRecords[course][record].minutes;
            gGameSaveDataBuffer[playerIndex].timeTrialRecords[course][record].seconds =
                gDefaultTimeTrialRecords[course][record].seconds;
            gGameSaveDataBuffer[playerIndex].timeTrialRecords[course][record].fraction =
                gDefaultTimeTrialRecords[course][record].fraction << 8;
            gGameSaveDataBuffer[playerIndex].timeTrialCharacterIds[course][record] = 0x10;
            gGameSaveDataBuffer[playerIndex].timeTrialCharacterIds[course][record] += record;

            if (course == RACE_COURSE_ROOKIE_MOUNTAIN) {
                gGameSaveDataBuffer[playerIndex].raceRecords[course][record].minutes =
                    gDefaultRookieMountainRaceRecords[0].records[record].minutes;
                gGameSaveDataBuffer[playerIndex].raceRecords[course][record].seconds =
                    gDefaultRookieMountainRaceRecords[0].records[record].seconds;
                gGameSaveDataBuffer[playerIndex].raceRecords[course][record].fraction =
                    gDefaultRookieMountainRaceRecords[0].records[record].fraction << 8;
            } else {
                gGameSaveDataBuffer[playerIndex].raceRecords[course][record].minutes =
                    (&gDefaultRaceRecordsByCourseParity
                          .bytes[record * 3])[(course & 1) * sizeof(RaceSetupSaveDefaultTimeRow)];
                gGameSaveDataBuffer[playerIndex].raceRecords[course][record].seconds =
                    (&gDefaultRaceRecordsByCourseParity
                          .bytes[record * 3])[(course & 1) * sizeof(RaceSetupSaveDefaultTimeRow) + 1];
                gGameSaveDataBuffer[playerIndex].raceRecords[course][record].fraction =
                    (&gDefaultRaceRecordsByCourseParity
                          .bytes[record * 3])[(course & 1) * sizeof(RaceSetupSaveDefaultTimeRow) + 2]
                    << 8;
            }
            gGameSaveDataBuffer[playerIndex].raceRecordCharacterIds[course][record] = 0x10;
            gGameSaveDataBuffer[playerIndex].raceRecordCharacterIds[course][record] += record;
            if (course == RACE_COURSE_ROOKIE_MOUNTAIN) {
                gGameSaveDataBuffer[playerIndex].scoreAttackScores[course][record] = gDefaultScoreAttackScores[record];
            } else {
                gGameSaveDataBuffer[playerIndex].scoreAttackScores[course][record] =
                    gDefaultScoreAttackScores[record + 5];
            }
            gGameSaveDataBuffer[playerIndex].scoreAttackCharacterIds[course][record] = record;
            gGameSaveDataBuffer[playerIndex].trickAttackScores[course][record] =
                gDefaultSaveRecordData.trickAttackScores[record];
            gGameSaveDataBuffer[playerIndex].trickAttackCharacterIds[course][record] = record;
        }
        gGameSaveDataBuffer[playerIndex].bestLapRecords[course].minutes = gDefaultBestLapRecords[course].minutes;
        gGameSaveDataBuffer[playerIndex].bestLapRecords[course].seconds = gDefaultBestLapRecords[course].seconds;
        gGameSaveDataBuffer[playerIndex].bestLapRecords[course].fraction = gDefaultBestLapRecords[course].fraction;
    }

    gGameSaveDataBuffer[playerIndex].replaySlots[0].length = 0;
    gGameSaveDataBuffer[playerIndex].replaySlots[1].length = 0;
    gGameSaveDataBuffer[playerIndex].replaySlots[2].length = 0;
    gGameSaveDataBuffer[playerIndex].replaySlots[3].length = 0;
    gGameSaveDataBuffer[playerIndex].replaySlots[4].length = 0;
    gGameSaveDataBuffer[playerIndex].replaySlots[5].length = 0;
    gGameSaveDataBuffer[playerIndex].replaySlots[6].length = 0;
    gGameSaveDataBuffer[playerIndex].replaySlots[7].length = 0;
    gGameSaveDataBuffer[playerIndex].replaySlots[8].length = 0;

    for (record = 0; record < (s32)sizeof(gGameSaveDataBuffer[playerIndex].reserved); record++) {
        gGameSaveDataBuffer[playerIndex].reserved[record] = 0;
    }

    if (gMainMenuSecretCodeUnlocked == 1) {
        record = 0;
        while (record < 11) {
            gGameSaveDataBuffer[playerIndex].cupPlacements[record] = 1;
            record++;
        }
        record = 0;
        for (;;) {
            gGameSaveDataBuffer[playerIndex].courseUnlockStates[record] =
                gMultiplayerCourseSelectDefaultCourseIds[record];
            gGameSaveDataBuffer[playerIndex].courseUnlockStates[record + 1] =
                gMultiplayerCourseSelectDefaultCourseIds[record + 1];
            gGameSaveDataBuffer[playerIndex].courseUnlockStates[record + 2] =
                gMultiplayerCourseSelectDefaultCourseIds[record + 2];
            gGameSaveDataBuffer[playerIndex].courseUnlockStates[record + 3] =
                gMultiplayerCourseSelectDefaultCourseIds[record + 3];
            record += 4;
            if ((s8 *)gCourseSelectColumnSoundEffects == &gMultiplayerCourseSelectDefaultCourseIds[record]) {
                break;
            }
        }
        gGameSaveDataBuffer[playerIndex].characterFlags = 1;
        gGameSaveDataBuffer[playerIndex].progressionLevel = 3;
        gGameSaveDataBuffer[playerIndex].extraCourseUnlockFlags = 0x3F;
    }
}
