#include "common.h"
#include "sound_manager.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "character_select_course_menu.h"
#include "game_task_scheduler.h"
#include "menu_renderer.h"
#include "race_type_select_menu.h"
#include "race_type_select_ui.h"
#include "title_menu.h"
#include "viewport_manager.h"

typedef struct {
    s16 alpha;
    s8 state;
} RaceTypeSelectCursorState;

extern void releaseMenuAssetHandles(void);
extern s32 enqueueSoundEffect(s16, s16);

extern u8 D_245A80;
extern u8 D_24C8E0;
extern u8 D_593D10;
extern u8 D_598A70;
extern u8 D_59AAA0;
extern u8 D_59DFE0;
extern u8 D_59E7F0;
extern u8 D_5A1ED0;
extern u8 D_5C5320;
extern u8 D_5CCD40;
extern u8 D_5D4280;

extern CharacterSelectFlowState *gCurrentGameTask;
extern RaceTypeSelectCursorState gRaceTypeSelectCursorTarget;
extern s8 D_800EC9C1;
extern u8 D_80121D88;
extern s32 gActiveMenuTask;
extern u8 D_8010ADF8;
extern u16 D_8010ADF0;
extern u8 gRaceTypeSelectCursorAnimState;
extern s16 gRaceCourseIndex;
extern s16 gMenuFadeAlpha;
extern u8 gRaceTypeSelection;
extern u8 D_800EC9DD;
extern s8 gFramebufferSwapDelay;
extern s32 gMenuFlowState;
extern s32 gPlayerInputHeld;
extern s32 gPlayerInputPressed;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

void returnToRaceTypeSelectMenu(void) {
    requestMusicSequenceBank(4);
    D_800EC9C1 = 0;
    D_80121D88 = 0;
    gActiveMenuTask = 0;
    gCurrentGameTask->fade = 1;
    gCurrentGameTask->timer = 0;
    D_8010ADF8 = 0;
    D_8010ADF0 = 0;
    if (gRaceCourseIndex == 7) {
        gRaceCourseIndex = 9;
    }
    gMenuFadeAlpha = gCurrentGameTask->fade;
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
    gFramebufferSwapDelay = 0;
    gCurrentGameTask->fade = 0xFF;
    loadCompressedRomAsset(&D_5A1ED0, &D_5C5320, 0x21);
    loadCompressedRomAsset(&D_593D10, &D_598A70, 0x22);
    loadCompressedRomAsset(&D_598A70, &D_59AAA0, 0x23);
    loadCompressedRomAsset(&D_59AAA0, &D_59DFE0, 0x24);
    loadCompressedRomAsset(&D_59DFE0, &D_59E7F0, 0x26);
    loadCompressedRomAsset(&D_5CCD40, &D_5D4280, 0x25);
    loadCompressedRomAsset(&D_245A80, &D_24C8E0, 0x1F);
    initCallbackTaskScheduler(0);
    createCallbackTask((void (*)(CallbackTask *))func_8001710C, 0, 0x5E);
    D_800EC9C1 = 0;
    D_80121D88 = 0;
    gActiveMenuTask = 0;
    gCurrentGameTask->timer = 0;
    D_8010ADF8 = 0;
    D_8010ADF0 = 0;
    if (gRaceCourseIndex == 7) {
        gRaceCourseIndex = 9;
    }
    gMenuFadeAlpha = gCurrentGameTask->fade;
    if (gRaceTypeSelection == 3) {
        gRaceTypeSelection = 0;
    }
    setCurrentGameTaskCallback(updateRaceTypeSelectMenu, 0);
    updateCallbackTasks();
    gRaceTypeSelectCursorTarget.state = 0;
    gRaceTypeSelectCursorTarget.alpha = 0;
}

// updateRaceTypeSelectMenu best match: 90.782% (nonmatchings/updateRaceTypeSelectMenu-7273315160691878794/base_12.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_type_select_menu/updateRaceTypeSelectMenu.s")

#ifdef NON_MATCHING
void updateRaceTypeSelectMenu(void) {
    u16 sp18;
    s32 newInput;
    s32 heldInput;
    s32 pressedUp;
    s32 repeatTimer;
    s32 selection;
    s32 previousSelection;
    s32 tempSelection;
    u8 waitTimer;

    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) gCurrentGameTask->fade, 0x24, 0);
        if (gCurrentGameTask->fade == 0) {
            createCallbackTask(initRaceTypeSelectOptionIcons, 0, 0x62);
        }
    } else {
        if (D_80121D88 == 0) {
            if (D_800EC9C1 == 0) {
                if (gRaceTypeSelectCursorAnimState == 1) {
                    selection = gRaceTypeSelection;
                    newInput = gPlayerInputHeld;
                    pressedUp = newInput & 0x10800;
                    sp18 = 3;
                    previousSelection = selection;

                    if ((pressedUp == 0) && ((newInput & 0x20400) == 0)) {
                        D_8010ADF0 = 0;
                    }

                    heldInput = gPlayerInputPressed;
                    if ((heldInput & 0x10800) ||
                        ((pressedUp != 0) && (D_8010ADF0 >= 9) && ((D_8010ADF0 % 3) == 0))) {
                        repeatTimer = D_8010ADF0;
                        tempSelection = selection - 1;
                        if (repeatTimer == 0) {
                            D_8010ADF0 = repeatTimer + 1;
                            repeatTimer = D_8010ADF0;
                        }
                        if (selection > 0) {
                            gRaceTypeSelection = tempSelection;
                            selection = tempSelection;
                        }
                    } else {
                        repeatTimer = D_8010ADF0;
                        if ((heldInput & 0x20400) ||
                            ((newInput & 0x20400) && (repeatTimer >= 9) && ((repeatTimer % 3) == 0))) {
                            if (repeatTimer == 0) {
                                D_8010ADF0 = repeatTimer + 1;
                                repeatTimer = D_8010ADF0;
                            }
                            if (selection < 3) {
                                gRaceTypeSelection = selection + 1;
                                selection = selection + 1;
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
                        gRaceCourseIndex = 9;
                    }

                    if ((heldInput & 0x1000) || ((heldInput & 0x8000) && (gMenuFlowState == 4))) {
                        enqueueSoundEffect(0x18, 0x32);
                        D_800EC9C1 = 1;
                        gRaceTypeSelectCursorTarget.state = 2;
                        gRaceTypeSelectCursorTarget.alpha = 0x100;
                        D_8010ADF8 = 0;
                    }
                }
            } else {
                D_800EC9C1++;
            }
        }

        waitTimer = D_800EC9C1;
        if (D_800EC9C1 == 8) {
            D_80121D88 = 1;
            D_800EC9C1++;
            waitTimer = D_800EC9C1;
        }

        if ((waitTimer == 0) && (gPlayerInputPressed & 0x4000) && (gMenuFlowState == (sp18 + 1))) {
            enqueueSoundEffect(0x18, 0x32);
            gRaceTypeSelectCursorTarget.state = 2;
            gRaceTypeSelectCursorTarget.alpha = 0x100;
            D_800EC9C1 = 1;
            D_8010ADF8 = 1;
        }

        if (D_80121D88 == 2) {
            setCurrentGameTaskCallback(handleRaceTypeSelectMenuSelection, 0);
            requestMusicSequenceStop(4);
        }
    }
    gMenuFlowState = 0;
    updateCallbackTasks();
}
#endif

void handleRaceTypeSelectMenuSelection(void) {
    if (gRaceTypeSelection < 3 && D_8010ADF8 == 0) {
        setCurrentGameTaskCallback(&initCharacterSelectCourseMenuFromRaceTypeSelect, 0);
        D_800EC9DD = 1;
    } else {
        setCurrentGameTaskCallback(&fadeOutRaceTypeSelectMenu, 0);
        requestMusicSequenceStop(8);
        D_800EC9DD = 1;
    }
    updateCallbackTasks();
}

void fadeOutRaceTypeSelectMenu(void) {
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
            gMenuFlowState = 1;
            resumeGameTask(2);
            removeGameTask(4);
        }
    }
}
