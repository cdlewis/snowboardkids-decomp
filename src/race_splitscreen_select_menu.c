#include "common.h"
#include "sound_manager.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "character_select_course_menu.h"
#include "game_task_scheduler.h"
#include "menu_renderer.h"
#include "race_splitscreen_select_menu.h"
#include "race_splitscreen_select_ui.h"
#include "title_menu.h"
#include "viewport_manager.h"

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
extern u8 D_800EC9C1;
extern u8 gRaceSplitscreenMode;
extern u8 D_800EC9DD;
extern u8 D_80121D88;
extern s32 gActiveMenuTask;
extern s32 gMenuFlowState;
extern u8 D_8010ADF8;
extern u16 D_8010ADF0;
extern s16 gMenuFadeAlpha;
extern s8 gFramebufferSwapDelay;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

void returnToRaceSplitscreenSelectMenu(void) {
    gCurrentGameTask->fade = 1;
    requestMusicSequenceBank(1);
    D_800EC9C1 = 0;
    D_80121D88 = 0;
    gActiveMenuTask = 0;
    gCurrentGameTask->timer = 0;
    gMenuFlowState = 0;
    D_8010ADF8 = 0;
    D_800EC9DD = 0;
    D_8010ADF0 = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    setCurrentGameTaskCallback(updateRaceSplitscreenSelectMenu, 0);
    gRaceSplitscreenSelectCursorTarget.state = 0;
    gRaceSplitscreenSelectCursorTarget.nextState = 0;
    updateCallbackTasks();
}

void initRaceSplitscreenSelectMenu(void) {
    requestMusicSequenceBank(1);
    D_800EC9DC = 0;
    D_800EC9DD = 0;
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
    createCallbackTask((void (*)(CallbackTask *))func_8001710C, 0, 0x5E);
    D_800EC9C1 = 0;
    D_80121D88 = 0;
    gActiveMenuTask = 0;
    gCurrentGameTask->timer = 0;
    D_8010ADF8 = 0;
    D_8010ADF0 = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    setCurrentGameTaskCallback(updateRaceSplitscreenSelectMenu, 0);
    gRaceSplitscreenSelectCursorTarget.state = 0;
    gRaceSplitscreenSelectCursorTarget.nextState = 0;
    updateCallbackTasks();
}

// updateRaceSplitscreenSelectMenu best match: 98.535% (nonmatchings/func_80005788-7273315160691878794/base_15.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_splitscreen_select_menu/updateRaceSplitscreenSelectMenu.s")

#ifdef NON_MATCHING
void updateRaceSplitscreenSelectMenu(void) {
    s32 newInput;
    s32 heldInput;
    s32 pressedUp;
    s32 repeatTimer;
    s32 selection;
    s32 newInputCopy;
    s32 previousSelection;
    s32 tempSelection;
    u8 waitTimer;

    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) gCurrentGameTask->fade, 0x24, 0);
        if (gCurrentGameTask->fade == 0) {
            createCallbackTask(initRaceSplitscreenSelectPlayerCountIcons, 0, 0x63);
        }
    } else {
        if ((gRaceSplitscreenSelectCursorTarget.portraitAlpha == 0x100) && (D_80121D88 == 0)) {
            if (D_800EC9C1 == 0) {
                if (gRaceSplitscreenSelectCursorTarget.state == 1) {
                    selection = gRaceSplitscreenMode;
                    newInputCopy = gPlayerInputHeld;
                    newInput = newInputCopy;
                    pressedUp = newInput & 0x10800;
                    previousSelection = selection;

                    if ((pressedUp == 0) && ((newInput & 0x20400) == 0)) {
                        D_8010ADF0 = 0;
                    }

                    heldInput = gPlayerInputPressed;
                    if ((heldInput & 0x10800) ||
                        ((pressedUp != 0) && (D_8010ADF0 >= 0xB) && ((D_8010ADF0 % 3) == 0))) {
                        repeatTimer = D_8010ADF0;
                        tempSelection = selection - 1;
                        if (repeatTimer == 0) {
                            D_8010ADF0 = repeatTimer + 1;
                            repeatTimer = D_8010ADF0;
                        }
                        if (selection > 0) {
                            gRaceSplitscreenMode = tempSelection;
                            selection = ((tempSelection & 0xFF) & 0xFF) & 0xFF;
                        }
                    } else {
                        repeatTimer = D_8010ADF0;
                        if ((heldInput & 0x20400) ||
                            ((newInput & 0x20400) && (repeatTimer >= 0xB) && ((repeatTimer % 3) == 0))) {
                            if (repeatTimer == 0) {
                                D_8010ADF0 = repeatTimer + 1;
                                repeatTimer = D_8010ADF0;
                            }
                            if (selection < 4) {
                                gRaceSplitscreenMode = selection + 1;
                                selection = (u8) (selection + 1);
                            }
                        }
                    }

                    if (repeatTimer != 0) {
                        D_8010ADF0 = repeatTimer + 1;
                        if (D_8010ADF0 == 0xFFFF) {
                            D_8010ADF0 = 0xC;
                        }
                    }

                    if (selection != previousSelection) {
                        enqueueSoundEffect(0x19, 0x32);
                        heldInput = gPlayerInputPressed;
                    }

                    if ((heldInput & 0x1000) || ((heldInput & 0x8000) && (gMenuFlowState == 5))) {
                        if ((gRaceSplitscreenMode == 3) && (D_80121D85 == 5)) {
                            enqueueSoundEffect(0x46, 0x32);
                        } else {
                            enqueueSoundEffect(0x18, 0x32);
                            D_800EC9C1 = 1;
                            if (D_8010ADF0 && D_8010ADF0) {
                            }
                            gRaceSplitscreenSelectCursorTarget.state = 2;
                            gRaceSplitscreenSelectCursorTarget.nextState = 0x100;
                            D_8010ADF8 = 0;
                        }
                    }
                }
            } else {
                D_800EC9C1++;
            }
        }

        waitTimer = D_800EC9C1;
        if ((gPlayerInputPressed & 0x4000) && (gMenuFlowState == 5) && (waitTimer == 0)) {
            enqueueSoundEffect(1, 0x32);
            D_800EC9C1 = 1;
            gRaceSplitscreenSelectCursorTarget.state = 2;
            gRaceSplitscreenSelectCursorTarget.nextState = 0x100;
            D_8010ADF8 = 1;
        }

        waitTimer = D_800EC9C1;
        if (waitTimer == 7) {
            D_80121D88 = 1;
            D_800EC9C1++;
        }

        if (D_80121D88 == 2) {
            setCurrentGameTaskCallback(handleRaceSplitscreenSelectMenuSelection, 0);
            if (D_8010ADF8 == 0) {
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
    if ((v0 == 0 || v0 == 2) && D_8010ADF8 == 0) {
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
            if (D_8010ADF8 == 1) {
                gMenuFlowState = 1;
            } else {
                gMenuFlowState = 0;
            }
            resumeGameTask(2);
            removeGameTask(4);
        }
    }
}
