#include "common.h"
#include "game/save_data.h"
#include "assets.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/menu/controller_pak/controller_pak_race_record_save_flow.h"
#include "game/menu/controller_pak/controller_pak_menu.h"
#include "game/menu/controller_pak/controller_pak_ui.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/controller_pak/controller_pak_race_record_save_ui.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/race_setup/race_setup_menu.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"

typedef struct RaceRecordSaveCompletion {
    s32 value;
    s32 pad4;
    s32 pad8;
} RaceRecordSaveCompletion;

extern ControllerPakMenuState gControllerPakMenuState;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern u8 gControllerPakMenuCursorState;
extern s32 gMenuFlowState;
extern s32 D_8010ADE0;
extern CallbackTask *D_8010ADE4;
extern s16 gMenuFadeAlpha;

void initControllerPakRaceRecordSaveFlow(void) {
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    gFramebufferRenderInterval.value = 0;
    gControllerPakStatusCodes[0] = 0;
    gMenuChoicePromptState[0] = 0;
    gControllerPakRetryCounts[0] = 0;
    gRacePlayers[0].menuState = 0;
    gMenuSelectionConfirmTimer = 0;
    gCurrentGameTask->callbackData0 = 0xFF;
    gActiveMenuTask = 0;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    D_8010ADE8 = 0;
    gMenuFadeAlpha = gCurrentGameTask->callbackData0;
    gGameSaveDataBuffer[0].money = gRacePlayers[0].money;
    LOAD_ASSET(_59AAA0, 0x21);
    LOAD_ASSET(_59AAA0, 0x24);
    LOAD_ASSET(_593D10, 0x22);
    LOAD_ASSET(_598A70, 0x23);
    LOAD_ASSET(_60F1A0, 0x29);
    initCallbackTaskScheduler(0);
    gActiveMenuTask = createCallbackTask((CallbackTaskCallback)&initControllerPakRaceRecordSaveScorePanel, 0, 0x61);
    D_8010ADE8 = createCallbackTask((CallbackTaskCallback)&initControllerPakRaceRecordSaveStatusChoicePrompt, 0, 0x60);
    gControllerPakRaceRecordSaveStatusTransition.step = 0;
    gControllerPakRaceRecordSaveStatusTransition.alpha = 0;
    gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0;
    gControllerPakRaceRecordSaveStatusTransition.nextStatus = 0;
    gControllerPakMenuState.state = 0;
    gControllerPakMenuState.confirmChoice = 0;
    setCurrentGameTaskCallback(updateControllerPakRaceRecordSaveFlow, 0);
}

void updateControllerPakRaceRecordSaveFlow(void) {
    RaceRecordSaveCompletion sp24;
    RaceRecordSaveCompletion *completion;
    CallbackTask *temp_t0;
    s16 statusCode;
    s32 fade;

    completion = &sp24;
    completion->value = 0;
    fade = gCurrentGameTask->callbackData0;
    temp_t0 = D_8010ADE4;
    if (fade != 0) {
        gCurrentGameTask->callbackData0 = stepMenuFadeAlpha((s32)(s16)fade, 0x24, 0U);
        if (gCurrentGameTask->callbackData0 == 0) {
            gControllerPakMenuState.state = 3;
            gControllerPakMenuState.confirmChoice = 1;
            createCallbackTask((CallbackTaskCallback)initControllerPakDeleteConfirmPrompt, 0, 0x64);
            setCurrentGameTaskCallback(updateControllerPakRaceRecordSaveOverwritePrompt, 0);
        }
    } else {
        if (gMenuSelectionConfirmTimer == 0) {
            if (gControllerPakRaceRecordSaveStatusTransition.step == 1) {
                if (gMenuChoicePromptState[0] != 0) {
                    statusCode = 6;
                } else {
                    statusCode = gControllerPakStatusCodes[0];
                }
                switch (statusCode) {
                    case 0:
                        gRumblePakConnectedByController[0] = 0;
                        requestRumbleMotorInit(0U);
                        if ((gRumbleMotorStatuses[0] != 1) && (gRumbleMotorStatuses[0] != 0xB) &&
                            (gRumbleMotorStatuses[0] != 4)) {
                            gRumblePakConnectedByController[0] = 1;
                        } else {
                            gRumblePakConnectedByController[0] = 0;
                        }
                        requestControllerPakProbe(0U);
                        if (gControllerPakStatusCodes[0] == 0x10) {
                            gControllerPakStatusCodes[0] = 0x11;
                        }
                        break;
                    case 1:
                        requestControllerPakSaveStatus(0U);
                        if ((gControllerPakStatusCodes[0] == 9) || (gControllerPakStatusCodes[0] == 2)) {
                            gControllerPakStatusCodes[0] = 8;
                            gMenuChoicePromptState[0] = 1;
                        }
                        break;
                    case 2:
                        if (gControllerPakRaceRecordSaveStatusTransition.step != 3) {
                            requestControllerPakSaveWrite(0U);
                            if (gControllerPakRetryCounts[0] == 0) {
                                gControllerPakRaceRecordSaveStatusTransition.targetStatus = 5;
                                gControllerPakRaceRecordSaveStatusTransition.step = 3;
                                gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
                            } else if (gControllerPakRetryCounts[0] == 3) {
                                if (temp_t0 != NULL) {
                                    gControllerPakRaceRecordSaveStatusTransition.step = 3;
                                    gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0xD;
                                    gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
                                } else {
                                    gControllerPakStatusCodes[0] = 0xD;
                                }
                                gControllerPakRetryCounts[0] = 0;
                            }
                        }
                        break;
                    case 3:
                        requestControllerPakRepair(0U);
                        if (gControllerPakRetryCounts[0] == 0) {
                            if (temp_t0 != NULL) {
                                gControllerPakRaceRecordSaveStatusTransition.step = 3;
                                gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0x10;
                            } else {
                                gControllerPakStatusCodes[0] = 0x10;
                            }
                        } else if (gControllerPakRetryCounts[0] == 3) {
                            if (temp_t0 != NULL) {
                                gControllerPakRaceRecordSaveStatusTransition.step = 3;
                                gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0xE;
                            } else {
                                gControllerPakStatusCodes[0] = 0xE;
                            }
                            gControllerPakRetryCounts[0] = 0;
                        }
                        break;
                    case 4:
                        if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                            enqueueSoundEffect(1, 0x32);
                            if (temp_t0 != NULL) {
                                gControllerPakRaceRecordSaveStatusTransition.step = 1;
                                gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
                                gControllerPakStatusCodes[0] = 0;
                            } else {
                                gControllerPakStatusCodes[0] = 0;
                            }
                        }
                        break;
                    case 5:
                        if (gRacePlayers[0].menuState == 0) {
                            gRacePlayers[0].menuState = 1;
                        }
                        break;
                    case 6:
                        if ((gMenuChoicePromptState[0] == 3) || (gMenuChoicePromptState[0] == 4)) {
                            if (gPlayerInputPressed[0] & (STICK_UP | U_JPAD)) {
                                if (gMenuChoicePromptState[0] != 3) {
                                    gMenuChoicePromptState[0]--;
                                    enqueueSoundEffect(0x19, 0x32);
                                }
                            }
                            if (gPlayerInputPressed[0] & (STICK_DOWN | D_JPAD)) {
                                if (gMenuChoicePromptState[0] != 4) {
                                    gMenuChoicePromptState[0]++;
                                    enqueueSoundEffect(0x19, 0x32);
                                }
                            }
                            if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                                enqueueSoundEffect(1, 0x32);
                                if (gMenuChoicePromptState[0] == 4) {
                                    if (gControllerPakStatusCodes[0] == 8) {
                                        gControllerPakRaceRecordSaveStatusTransition.nextStatus = 0xF;
                                    } else if (gControllerPakStatusCodes[0] == 7) {
                                        gControllerPakRaceRecordSaveStatusTransition.nextStatus = 3;
                                    } else if (gControllerPakStatusCodes[0] == 0xF) {
                                        gControllerPakRaceRecordSaveStatusTransition.nextStatus = 0;
                                    } else {
                                        gControllerPakRaceRecordSaveStatusTransition.nextStatus = 4;
                                    }
                                } else if (gControllerPakStatusCodes[0] == 8) {
                                    gControllerPakRaceRecordSaveStatusTransition.nextStatus = 2;
                                } else if ((gControllerPakStatusCodes[0] == 0xA) ||
                                           (gControllerPakStatusCodes[0] == 0x11)) {
                                    gControllerPakRaceRecordSaveStatusTransition.nextStatus = 0xF;
                                } else if (gControllerPakStatusCodes[0] == 7) {
                                    gControllerPakRaceRecordSaveStatusTransition.nextStatus = 4;
                                } else {
                                    gControllerPakRaceRecordSaveStatusTransition.nextStatus = 5;
                                }
                                gMenuChoicePromptState[0] += 2;
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
                    case 17:
                        if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                            enqueueSoundEffect(1, 0x32);
                            gMenuChoicePromptState[0] =
                                (s16)gControllerPakRaceRecordSaveStatusChoicePromptStates[gControllerPakStatusCodes[0]];
                        }
                        break;
                    case 15:
                        if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                            enqueueSoundEffect(1, 0x32);
                            gMenuChoicePromptState[0] =
                                (s16)gControllerPakRaceRecordSaveStatusChoicePromptStates[gControllerPakStatusCodes[0]];
                        }
                        break;
                    case 14:
                    case 16:
                        if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                            enqueueSoundEffect(1, 0x32);
                            if (gControllerPakStatusCodes[0] == 0xE) {
                                if (temp_t0 != NULL) {
                                    gControllerPakRaceRecordSaveStatusTransition.step = 3;
                                    gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0x12;
                                } else {
                                    gControllerPakStatusCodes[0] = 7;
                                }
                            } else if (temp_t0 != NULL) {
                                gControllerPakRaceRecordSaveStatusTransition.step = 1;
                                gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
                                gControllerPakStatusCodes[0] = 0;
                            } else {
                                gControllerPakStatusCodes[0] = 0;
                            }
                        }
                        break;
                    case 18:
                        if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                            enqueueSoundEffect(1, 0x32);
                            gControllerPakRaceRecordSaveStatusTransition.step = 3;
                            gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
                            gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0;
                        }
                        break;
                }
            }
            completion->value = gRacePlayers[0].menuState & 1;
        } else {
            gMenuSelectionConfirmTimer++;
        }
    }
    if (completion->value != 0) {
        gMenuSelectionConfirmTimer = 1;
    }
    if (gMenuSelectionConfirmTimer == 0x23) {
        setCurrentGameTaskCallback(fadeOutControllerPakRaceRecordSaveFlow, 0);
    }
    updateCallbackTasks();
}



void fadeOutControllerPakRaceRecordSaveFlow(void) {
    s32 temp_v0 = gCurrentGameTask->callbackData0;
    if (temp_v0 != 0xFF) {
        gCurrentGameTask->callbackData0 = stepMenuFadeAlpha((s16)temp_v0, 0x20, 1);
        updateCallbackTasks();
        if (gCurrentGameTask->callbackData0 == 0xFF) {
            gFramebufferSwapHold = 1;
        }
    } else if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferRenderInterval.value = 0;
        resumeGameTask(2);
        removeGameTask(4);
    }
}

void updateControllerPakRaceRecordSaveOverwritePrompt(void) {
    if ((gPlayerInputPressed[0] & (STICK_UP | U_JPAD)) && (gControllerPakMenuState.confirmChoice != 0)) {
        gControllerPakMenuState.confirmChoice = 0;
        enqueueSoundEffect(0x19, 0x32);
    } else if ((gPlayerInputPressed[0] & (STICK_DOWN | D_JPAD)) && (gControllerPakMenuState.confirmChoice != 1)) {
        gControllerPakMenuState.confirmChoice = 1;
        enqueueSoundEffect(0x19, 0x32);
    }
    if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
        enqueueSoundEffect(0x18, 0x32);
        if (gControllerPakMenuState.confirmChoice == 0) {
            gControllerPakMenuCursorState = 0;
            setCurrentGameTaskCallback(updateControllerPakRaceRecordSaveFlow, 0);
        } else {
            gMenuFlowState = 1;
            setCurrentGameTaskCallback(fadeOutControllerPakRaceRecordSaveFlow, 0);
        }
    } else if (gPlayerInputPressed[0] & B_BUTTON) {
        enqueueSoundEffect(0x18, 0x32);
        gMenuFlowState = 1;
        setCurrentGameTaskCallback(fadeOutControllerPakRaceRecordSaveFlow, 0);
    }
    updateCallbackTasks();
}
