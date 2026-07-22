#include "common.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/splitscreen_select/race_splitscreen_select_menu.h"
#include "game/menu/splitscreen_select/race_splitscreen_select_ui.h"
#include "game/menu/race_setup/race_setup_ui.h"
#include "game/engine/viewport_manager.h"

typedef struct {
    char pad[0x18];
    s32 fade;
    s32 timer;
} RaceSplitscreenSelectMenuState;

typedef struct {
    u8 state;
    char pad1[1];
    s16 nextState;
    s16 portraitAlpha;
} RaceSplitscreenSelectCursorTarget;

extern void releaseMenuAssetHandles(void);
extern s32 enqueueSoundEffect(s16, s16);

extern u8 D_800EC9DC;
extern u8 D_5A1ED0[];
extern u8 D_5C5320[];
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_59AAA0[];
extern u8 D_59DFE0[];
extern u8 D_59E7F0[];
extern u8 D_245A80[];
extern u8 D_24C8E0[];

extern RaceSplitscreenSelectMenuState *gCurrentGameTask;
extern RaceSplitscreenSelectCursorTarget gRaceSplitscreenSelectCursorTarget;
extern u8 gMenuSelectionConfirmTimer;
extern u8 gRaceSplitscreenMode;
extern u8 gCourseSelectFromRaceTypeMenu;
extern u8 gMenuTransitionState;
extern s32 gActiveMenuTask;
extern s32 gMenuFlowState;
extern u8 gMenuExitSelection;
extern u16 gMenuInputRepeatTimers;
extern s16 gMenuFadeAlpha;
extern s8 gFramebufferSwapDelay;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

void returnToRaceSplitscreenSelectMenu(void) {
    gCurrentGameTask->fade = 1;
    requestMusicSequenceBank(1);
    gMenuSelectionConfirmTimer = 0;
    gMenuTransitionState = 0;
    gActiveMenuTask = 0;
    gCurrentGameTask->timer = 0;
    gMenuFlowState = 0;
    gMenuExitSelection = 0;
    gCourseSelectFromRaceTypeMenu = 0;
    gMenuInputRepeatTimers = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    setCurrentGameTaskCallback(updateRaceSplitscreenSelectMenu, 0);
    gRaceSplitscreenSelectCursorTarget.state = 0;
    gRaceSplitscreenSelectCursorTarget.nextState = 0;
    updateCallbackTasks();
}

void initRaceSplitscreenSelectMenu(void) {
    requestMusicSequenceBank(1);
    D_800EC9DC = 0;
    gCourseSelectFromRaceTypeMenu = 0;
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
    gFramebufferSwapDelay = 0;
    gCurrentGameTask->fade = 0xFF;
    loadCompressedRomAsset(D_5A1ED0, D_5C5320, 0x21);
    loadCompressedRomAsset(D_593D10, D_598A70, 0x22);
    loadCompressedRomAsset(D_598A70, D_59AAA0, 0x23);
    loadCompressedRomAsset(D_59DFE0, D_59E7F0, 0x26);
    loadCompressedRomAsset(D_245A80, D_24C8E0, 0x1F);
    loadCompressedRomAsset(D_59AAA0, D_59DFE0, 0x24);
    initCallbackTaskScheduler(0);
    createCallbackTask((void (*)(CallbackTask *))initMenuIconTilemapSpriteActor, 0, 0x5E);
    gMenuSelectionConfirmTimer = 0;
    gMenuTransitionState = 0;
    gActiveMenuTask = 0;
    gCurrentGameTask->timer = 0;
    gMenuExitSelection = 0;
    gMenuInputRepeatTimers = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    setCurrentGameTaskCallback(updateRaceSplitscreenSelectMenu, 0);
    gRaceSplitscreenSelectCursorTarget.state = 0;
    gRaceSplitscreenSelectCursorTarget.nextState = 0;
    updateCallbackTasks();
}

// updateRaceSplitscreenSelectMenu best match: 98.810% (nonmatchings/updateRaceSplitscreenSelectMenu-2188069624939011928/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/splitscreen_select/race_splitscreen_select_menu/updateRaceSplitscreenSelectMenu.s")

#ifdef NON_MATCHING
void updateRaceSplitscreenSelectMenu(void) {
    s32 newInput;
    s32 heldInput;
    s32 pressedUp;
    s32 repeatTimer;
    s32 selection;
    s32 newInputCopy;
    s32 previousSelection;
    s32 pressedUpCopy;
    s32 repeatTimerCopy;
    u8 waitTimer;

    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) gCurrentGameTask->fade, 0x24, 0);
        if (gCurrentGameTask->fade == 0) {
            createCallbackTask(initRaceSplitscreenSelectPlayerCountIcons, 0, 0x63);
        }
    } else {
        if ((gRaceSplitscreenSelectCursorTarget.portraitAlpha == 0x100) && (gMenuTransitionState == 0)) {
            if (gMenuSelectionConfirmTimer == 0) {
                if (gRaceSplitscreenSelectCursorTarget.state == 1) {
                    selection = gRaceSplitscreenMode;
                    newInputCopy = gPlayerInputHeld;
                    newInput = newInputCopy;
                    pressedUp = newInput & (STICK_UP | U_JPAD);
                    previousSelection = selection;

                    if ((pressedUp == 0) && ((newInput & (STICK_DOWN | D_JPAD)) == 0)) {
                        gMenuInputRepeatTimers = 0;
                    }

                    heldInput = gPlayerInputPressed;
                    pressedUpCopy = pressedUp;
                    if ((heldInput & (STICK_UP | U_JPAD)) ||
                        ((pressedUpCopy != 0) && (gMenuInputRepeatTimers >= 0xB) &&
                         ((gMenuInputRepeatTimers % 3) == 0))) {
                        repeatTimer = gMenuInputRepeatTimers;
                        if (repeatTimer == 0) {
                            gMenuInputRepeatTimers = repeatTimer + 1;
                            repeatTimer = gMenuInputRepeatTimers;
                        }
                        if (selection > 0) {
                            gRaceSplitscreenMode = selection - 1;
                            selection = (((selection - 1) & 0xFF) & 0xFF) & 0xFF;
                        }
                    } else {
                        repeatTimer = gMenuInputRepeatTimers;
                        repeatTimerCopy = repeatTimer;
                        if ((heldInput & (STICK_DOWN | D_JPAD)) ||
                            ((newInput & (STICK_DOWN | D_JPAD)) && (repeatTimer >= 0xB) && ((repeatTimer % 3) == 0))) {
                            if (repeatTimerCopy == 0) {
                                gMenuInputRepeatTimers = repeatTimerCopy + 1;
                                repeatTimer = gMenuInputRepeatTimers;
                            }
                            if (selection < 4) {
                                gRaceSplitscreenMode = selection + 1;
                                selection = (u8) (selection + 1);
                            }
                        }
                    }

                    if (repeatTimer != 0) {
                        gMenuInputRepeatTimers = repeatTimer + 1;
                        if (gMenuInputRepeatTimers == 0xFFFF) {
                            gMenuInputRepeatTimers = 0xC;
                        }
                    }

                    if (selection != previousSelection) {
                        enqueueSoundEffect(0x19, 0x32);
                        heldInput = gPlayerInputPressed;
                    }

                    if ((heldInput & START_BUTTON) || ((heldInput & A_BUTTON) && (gMenuFlowState == 5))) {
                        if ((gRaceSplitscreenMode == 3) && (gMenuSelectionVariant == 5)) {
                            enqueueSoundEffect(0x46, 0x32);
                        } else {
                            enqueueSoundEffect(0x18, 0x32);
                            gMenuSelectionConfirmTimer = 1;
                            if (gMenuInputRepeatTimers && gMenuInputRepeatTimers) {
                            }
                            gRaceSplitscreenSelectCursorTarget.state = 2;
                            gRaceSplitscreenSelectCursorTarget.nextState = 0x100;
                            gMenuExitSelection = 0;
                        }
                    }
                }
            } else {
                gMenuSelectionConfirmTimer++;
            }
        }

        waitTimer = gMenuSelectionConfirmTimer;
        if ((gPlayerInputPressed & B_BUTTON) && (gMenuFlowState == 5) && (waitTimer == 0)) {
            enqueueSoundEffect(1, 0x32);
            gMenuSelectionConfirmTimer = 1;
            gRaceSplitscreenSelectCursorTarget.state = 2;
            gRaceSplitscreenSelectCursorTarget.nextState = 0x100;
            gMenuExitSelection = 1;
        }

        waitTimer = gMenuSelectionConfirmTimer;
        if (waitTimer == 7) {
            gMenuTransitionState = 1;
            gMenuSelectionConfirmTimer++;
        }

        if (gMenuTransitionState == 2) {
            setCurrentGameTaskCallback(handleRaceSplitscreenSelectMenuSelection, 0);
            if (gMenuExitSelection == 0) {
                requestMusicSequenceStop(4);
            }
        }
    }
    gMenuFlowState = 0;
    updateCallbackTasks();
}
#endif

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
