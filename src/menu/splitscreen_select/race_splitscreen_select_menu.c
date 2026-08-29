#include "game/race/race_state.h"
#include "common.h"
#include "assets.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/splitscreen_select/race_splitscreen_select_menu.h"
#include "game/menu/splitscreen_select/race_splitscreen_select_ui.h"
#include "game/menu/race_setup/race_setup_ui.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"

extern void releaseMenuAssetHandles(void);
extern s32 enqueueSoundEffect(s16, s16);

extern u8 D_800EC9DC;

extern u8 gCourseSelectFromRaceTypeMenu;
extern s32 gMenuFlowState;
extern u8 gMenuExitSelection;
extern s16 gMenuFadeAlpha;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

void returnToRaceSplitscreenSelectMenu(void) {
    gCurrentGameTask->fade = 1;
    requestMusicSequenceBank(1);
    gMenuSelectionConfirmTimer = 0;
    gRacePlayers[0].menuState = 0;
    gActiveMenuTask = 0;
    gCurrentGameTask->timer = 0;
    gMenuFlowState = 0;
    gMenuExitSelection = 0;
    gCourseSelectFromRaceTypeMenu = 0;
    gMenuInputRepeatTimers[0] = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    setCurrentGameTaskCallback(updateRaceSplitscreenSelectMenu, 0);
    gRaceSplitscreenSelectCursorTarget.state = 0;
    gRaceSplitscreenSelectCursorTarget.alphaTarget = 0;
    updateCallbackTasks();
}

void initRaceSplitscreenSelectMenu(void) {
    requestMusicSequenceBank(1);
    D_800EC9DC = 0;
    gCourseSelectFromRaceTypeMenu = 0;
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
    gFramebufferRenderInterval.value = 0;
    gCurrentGameTask->fade = 0xFF;
    LOAD_ASSET(_5A1ED0, 0x21);
    LOAD_ASSET(_593D10, 0x22);
    LOAD_ASSET(_598A70, 0x23);
    LOAD_ASSET(_59DFE0, 0x26);
    LOAD_ASSET(_245A80, 0x1F);
    LOAD_ASSET(_59AAA0, 0x24);
    initCallbackTaskScheduler(0);
    createCallbackTask((CallbackTaskCallback)initMenuIconTilemapSpriteActor, 0, 0x5E);
    gMenuSelectionConfirmTimer = 0;
    gRacePlayers[0].menuState = 0;
    gActiveMenuTask = 0;
    gCurrentGameTask->timer = 0;
    gMenuExitSelection = 0;
    gMenuInputRepeatTimers[0] = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    setCurrentGameTaskCallback(updateRaceSplitscreenSelectMenu, 0);
    gRaceSplitscreenSelectCursorTarget.state = 0;
    gRaceSplitscreenSelectCursorTarget.alphaTarget = 0;
    updateCallbackTasks();
}

void updateRaceSplitscreenSelectMenu(void) {
    s32 pressed;
    s32 heldUp;
    u8 oldSelection;
    u8 confirmTimer;
    GameTaskCallback callback;

    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16)gCurrentGameTask->fade, 0x24, 0);
        if (gCurrentGameTask->fade == 0) {
            createCallbackTask((CallbackTaskCallback)initRaceSplitscreenSelectPlayerCountIcons, 0, 0x63);
        }
    } else {
        if ((gRaceSplitscreenSelectCursorTarget.portraitAlpha == 0x100) && (gRacePlayers[0].menuState == 0)) {
            if (gMenuSelectionConfirmTimer == 0) {
                if (gRaceSplitscreenSelectCursorTarget.state == 1) {
                    heldUp = gPlayerInputHeld[0] & 0x10800;
                    oldSelection = gRaceSplitscreenMode;
                    if ((heldUp == 0) && !(gPlayerInputHeld[0] & 0x20400)) {
                        gMenuInputRepeatTimers[0] = 0;
                    }
                    pressed = gPlayerInputPressed[0];
                    if ((pressed & 0x10800) || ((heldUp != 0) && (gMenuInputRepeatTimers[0] >= 0xB) &&
                                                ((gMenuInputRepeatTimers[0] % 3) == 0))) {
                        if (!gMenuInputRepeatTimers[0]) {
                            gMenuInputRepeatTimers[0] = (gMenuInputRepeatTimers[0] = gMenuInputRepeatTimers[0] + 1);
                        }
                        if (gRaceSplitscreenMode > 0) {
                            gRaceSplitscreenMode--;
                        }
                    } else {
                        if ((pressed & 0x20400) ||
                            ((gPlayerInputHeld[0] & 0x20400) && (gMenuInputRepeatTimers[0] >= 0xB) &&
                             ((gMenuInputRepeatTimers[0] % 3) == 0))) {
                            if (!gMenuInputRepeatTimers[0]) {
                                gMenuInputRepeatTimers[0] = gMenuInputRepeatTimers[0] + 1;
                            }
                            if (gRaceSplitscreenMode < 4) {
                                gRaceSplitscreenMode++;
                            }
                        }
                    }
                    if (gMenuInputRepeatTimers[0]) {
                        gMenuInputRepeatTimers[0]++;
                        if (gMenuInputRepeatTimers[0] == 0xFFFF) {
                            gMenuInputRepeatTimers[0] = 0xC;
                        }
                    }
                    if (oldSelection != gRaceSplitscreenMode) {
                        enqueueSoundEffect(0x19, 0x32);
                        pressed = gPlayerInputPressed[0];
                    }
                    if ((pressed & 0x1000) || ((pressed & 0x8000) && (gMenuFlowState == 5))) {
                        if ((gRaceSplitscreenMode == 3) && (gRacePlayers[0].selectedCharacterId == 5)) {
                            enqueueSoundEffect(0x46, 0x32);
                        } else {
                            enqueueSoundEffect(0x18, 0x32);
                            gMenuSelectionConfirmTimer = 1;
                            gRaceSplitscreenSelectCursorTarget.state = 2;
                            gRaceSplitscreenSelectCursorTarget.alphaTarget = 0x100;
                            gMenuExitSelection = 0;
                        }
                    }
                }
            } else {
                gMenuSelectionConfirmTimer++;
            }
        }

        confirmTimer = gMenuSelectionConfirmTimer;
        if (((gPlayerInputPressed[0] & 0x4000) && (gMenuFlowState == 5)) && (confirmTimer == 0)) {
            enqueueSoundEffect(1, 0x32);
            gMenuSelectionConfirmTimer = 1;
            gRaceSplitscreenSelectCursorTarget.state = 2;
            gRaceSplitscreenSelectCursorTarget.alphaTarget = 0x100;
            gMenuExitSelection = 1;
        }

        confirmTimer = gMenuSelectionConfirmTimer;
        if (confirmTimer == 7) {
            gRacePlayers[0].menuState = 1;
            gMenuSelectionConfirmTimer = gMenuSelectionConfirmTimer + 1;
        }
        if (gRacePlayers[0].menuState == 2) {
            setCurrentGameTaskCallback(callback = handleRaceSplitscreenSelectMenuSelection, 0);
            if (gMenuExitSelection == 0) {
                requestMusicSequenceStop(4);
            }
        }
    }
    gMenuFlowState = 0;
    updateCallbackTasks();
}

void handleRaceSplitscreenSelectMenuSelection(void) {
    u8 v0;

    v0 = gRaceSplitscreenMode;
    if ((v0 == 0 || v0 == 2) && gMenuExitSelection == 0) {
        setCurrentGameTaskCallback(initCharacterSelectCourseMenuFromPlayerSelect, 0);
        gMenuFlowState = 0;
    } else {
        setCurrentGameTaskCallback(fadeOutRaceSplitscreenSelectMenu, 0);
    }
    updateCallbackTasks();
}

void fadeOutRaceSplitscreenSelectMenu(void) {
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
            gFramebufferRenderInterval.value = 0;
            if (gMenuExitSelection == 1) {
                gMenuFlowState = 1;
            } else {
                gMenuFlowState = 0;
            }
            resumeGameTask(2);
            removeGameTask(4);
        }
    }
}
