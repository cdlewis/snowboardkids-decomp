#include "game/race/race_state.h"
#include "common.h"
#include "assets.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/race_type_select/race_type_select_menu.h"
#include "game/menu/race_type_select/race_type_select_ui.h"
#include "game/menu/race_setup/race_setup_ui.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"


extern u8 gMenuExitSelection;
extern s16 gMenuFadeAlpha;
extern u8 gCourseSelectFromRaceTypeMenu;
extern s32 gMenuFlowState;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

void returnToRaceTypeSelectMenu(void) {
    requestMusicSequenceBank(4);
    gMenuSelectionConfirmTimer = 0;
    gRacePlayers[0].menuState = 0;
    gActiveMenuTask = 0;
    gCurrentGameTask->callbackData0 = 1;
    gCurrentGameTask->callbackData1 = 0;
    gMenuExitSelection = 0;
    gMenuInputRepeatTimers[0] = 0;
    if (gRaceCourseIndex.signedValue == 7) {
        gRaceCourseIndex.signedValue = 9;
    }
    gMenuFadeAlpha = gCurrentGameTask->callbackData0;
    if (gRaceTypeSelection == 3) {
        gRaceTypeSelection = 0;
    }
    setCurrentGameTaskCallback(&updateRaceTypeSelectMenu, 0);
    updateCallbackTasks();
    gRaceTypeSelectCursorTarget.state = 0;
    gRaceTypeSelectCursorTarget.alpha = 0;
}

void initRaceTypeSelectMenu(void) {
    requestMusicSequenceBank(4);
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
    gFramebufferRenderInterval.value = 0;
    gCurrentGameTask->callbackData0 = 0xFF;
    LOAD_ASSET(_5A1ED0, 0x21);
    LOAD_ASSET(_593D10, 0x22);
    LOAD_ASSET(_598A70, 0x23);
    LOAD_ASSET(_59AAA0, 0x24);
    LOAD_ASSET(_59DFE0, 0x26);
    LOAD_ASSET(_5CCD40, 0x25);
    LOAD_ASSET(_245A80, 0x1F);
    initCallbackTaskScheduler(0);
    createCallbackTask((CallbackTaskCallback)initMenuIconTilemapSpriteActor, 0, 0x5E);
    gMenuSelectionConfirmTimer = 0;
    gRacePlayers[0].menuState = 0;
    gActiveMenuTask = 0;
    gCurrentGameTask->callbackData1 = 0;
    gMenuExitSelection = 0;
    gMenuInputRepeatTimers[0] = 0;
    if (gRaceCourseIndex.signedValue == 7) {
        gRaceCourseIndex.signedValue = 9;
    }
    gMenuFadeAlpha = gCurrentGameTask->callbackData0;
    if (gRaceTypeSelection == 3) {
        gRaceTypeSelection = 0;
    }
    setCurrentGameTaskCallback(updateRaceTypeSelectMenu, 0);
    updateCallbackTasks();
    gRaceTypeSelectCursorTarget.state = 0;
    gRaceTypeSelectCursorTarget.alpha = 0;
}

void updateRaceTypeSelectMenu(void) {
    u16 sp18[4];
    u16 nextSelection;
    s32 newInput;
    s32 heldInput;
    s32 pressedUp;
    s32 repeatTimer;
    s32 selection;
    s32 previousSelection;
    s32 pressedUpCopy;
    s32 repeatTimerCopy;
    u8 waitTimer;
    RaceTypeSelectCursorState *cursorTarget;

    if (gCurrentGameTask->callbackData0 != 0) {
        gCurrentGameTask->callbackData0 = stepMenuFadeAlpha((s16)gCurrentGameTask->callbackData0, 0x24, 0);
        if (gCurrentGameTask->callbackData0 == 0) {
            createCallbackTask((CallbackTaskCallback)initRaceTypeSelectOptionIcons, 0, 0x62);
        }
    } else {
        if (gRacePlayers[0].menuState == 0) {
            if (gMenuSelectionConfirmTimer == 0) {
                if (gRaceTypeSelectCursorAnimState == 1) {
                    selection = gRaceTypeSelection;
                    newInput = gPlayerInputHeld[0];
                    pressedUp = newInput & (STICK_UP | U_JPAD);
                    sp18[0] = 3;
                    previousSelection = selection;

                    if ((pressedUp == 0) && ((newInput & (STICK_DOWN | D_JPAD)) == 0)) {
                        gMenuInputRepeatTimers[0] = 0;
                    }

                    heldInput = gPlayerInputPressed[0];
                    pressedUpCopy = pressedUp;
                    if ((heldInput & (STICK_UP | U_JPAD)) ||
                        ((pressedUpCopy != 0) && (gMenuInputRepeatTimers[0] >= 9) &&
                         ((gMenuInputRepeatTimers[0] % 3) == 0))) {
                        repeatTimer = gMenuInputRepeatTimers[0];
                        if ((repeatTimer & 0xFFFF) == 0) {
                            gMenuInputRepeatTimers[0] = repeatTimer + 1;
                            repeatTimer =
                                (((((gMenuInputRepeatTimers[0] & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) &
                                0xFFFFu;
                        }
                        if (selection > 0) {
                            gRaceTypeSelection = selection - 1;
                            selection =
                                (u8)(((((((((((((selection - 1) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) &
                                           0xFFFF) &
                                          0xFFFF) &
                                         0xFFFF) &
                                        0xFFFF) &
                                       0xFFFF) &
                                      0xFFFF) &
                                     0xFFFF);
                        }
                    } else {
                        repeatTimer = gMenuInputRepeatTimers[0];
                        if ((heldInput & (STICK_DOWN | D_JPAD)) ||
                            ((newInput & (STICK_DOWN | D_JPAD)) && ((repeatTimerCopy = repeatTimer ^ 0) >= 9) &&
                             ((repeatTimerCopy % 3) == 0))) {
                            if (repeatTimer == 0) {
                                gMenuInputRepeatTimers[0] = repeatTimer + 1;
                                repeatTimer = gMenuInputRepeatTimers[0];
                            }
                            if (selection < 3) {
                                nextSelection = selection + 1;
                                gRaceTypeSelection = nextSelection;
                                selection = (u8)(selection + 1);
                            }
                        }
                    }

                    repeatTimerCopy = selection ^ 0;
                    if (repeatTimer != 0) {
                        gMenuInputRepeatTimers[0] = repeatTimer + 1;
                        if (gMenuInputRepeatTimers[0 ^ 0] == 0xFFFF) {
                            if (&gRaceTypeSelectCursorTarget) {}
                            selection = 0;
                            gMenuInputRepeatTimers[selection] = 0xC;
                        }
                    }

                    if (repeatTimerCopy != previousSelection) {
                        enqueueSoundEffect(0x19, 0x32);
                        heldInput = gPlayerInputPressed[0];
                        gRaceCourseIndex.signedValue = 9;
                    }

                    if ((heldInput & START_BUTTON) || ((heldInput & A_BUTTON) && (gMenuFlowState == 4))) {
                        enqueueSoundEffect(0x18, 0x32);
                        gMenuSelectionConfirmTimer = 1;
                        (&gRaceTypeSelectCursorTarget)->state = 2;
                        (&gRaceTypeSelectCursorTarget)->alpha = 0x100;
                        gMenuExitSelection = 0;
                    }
                }
            } else {
                gMenuSelectionConfirmTimer++;
            }
        }

        waitTimer = gMenuSelectionConfirmTimer;
        if (gMenuSelectionConfirmTimer == 8) {
            gRacePlayers[0].menuState = 1;
            gMenuSelectionConfirmTimer++;
            waitTimer = gMenuSelectionConfirmTimer;
        }

        if ((waitTimer == 0) && (gPlayerInputPressed[0] & B_BUTTON) && (gMenuFlowState == (sp18[0] + 1))) {
            enqueueSoundEffect(0x18, 0x32);
            cursorTarget = &gRaceTypeSelectCursorTarget;
            cursorTarget->state = 2;
            cursorTarget->alpha = 0x100;
            gMenuSelectionConfirmTimer = 1;
            gMenuExitSelection = 1;
        }

        if (gRacePlayers[0].menuState == 2) {
            setCurrentGameTaskCallback(handleRaceTypeSelectMenuSelection, 0);
            requestMusicSequenceStop(4);
        }
    }
    gMenuFlowState = 0;
    updateCallbackTasks();
}

void handleRaceTypeSelectMenuSelection(void) {
    if (gRaceTypeSelection < 3 && gMenuExitSelection == 0) {
        setCurrentGameTaskCallback(&initCharacterSelectCourseMenuFromRaceTypeSelect, 0);
        gCourseSelectFromRaceTypeMenu = 1;
    } else {
        setCurrentGameTaskCallback(&fadeOutRaceTypeSelectMenu, 0);
        requestMusicSequenceStop(8);
        gCourseSelectFromRaceTypeMenu = 1;
    }
    updateCallbackTasks();
}

void fadeOutRaceTypeSelectMenu(void) {
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
            gMenuFlowState = 1;
            resumeGameTask(2);
            removeGameTask(4);
        }
    }
}
