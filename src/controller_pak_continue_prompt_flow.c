#include "common.h"
#include "sound_manager.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "character_select_course_menu.h"
#include "controller_pak_ui.h"
#include "game_task_scheduler.h"
#include "menu_renderer.h"
#include "controller_pak_continue_prompt_flow.h"
#include "viewport_manager.h"

typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ u8 state;
} ControllerPakContinuePromptTransition;

extern u64 enqueueSoundEffect(s32, s32);
extern void releaseMenuAssetHandles(void);

extern CharacterSelectFlowState *gCurrentGameTask;
extern ControllerPakContinuePromptTransition gControllerPakContinuePromptTransition;
extern s8 gFramebufferSwapDelay;
extern s16 gMenuFadeAlpha;
extern s32 gMenuFlowState;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern s32 gPlayerInputPressed;
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_59AAA0[];
extern u8 D_59DFE0[];
extern u8 D_60F1A0[];
extern u8 D_60F990[];

void initControllerPakContinuePromptFlow(void) {
    requestMusicSequenceBank(3);
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
    gFramebufferSwapDelay = 0;
    gCurrentGameTask->fade = 0xFF;
    gCurrentGameTask->timer = 0;
    gMenuFlowState = 0;
    gControllerPakContinuePromptTransition.state = 0;
    gControllerPakContinuePromptTransition.x = 0x100;
    loadCompressedRomAsset(D_593D10, D_598A70, 0x22);
    loadCompressedRomAsset(D_598A70, D_59AAA0, 0x23);
    loadCompressedRomAsset(D_59AAA0, D_59DFE0, 0x24);
    loadCompressedRomAsset(D_60F1A0, D_60F990, 0x29);
    initCallbackTaskScheduler(0);
    createCallbackTask(initControllerPakContinuePrompt, 0, 0x64);
    gMenuFadeAlpha = gCurrentGameTask->fade;
    setCurrentGameTaskCallback(updateControllerPakContinuePromptFlow, 0);
    updateCallbackTasks();
}

void updateControllerPakContinuePromptFlow(void) {
    s32 temp_a1;
    s32 temp_v0;
    s32 var_v0;

    temp_v0 = gCurrentGameTask->fade;
    if (temp_v0 != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s32) (s16) temp_v0, 0x24, 0);
        if (gCurrentGameTask->fade == 0) {
            gControllerPakContinuePromptTransition.state = 1;
        }
    } else {
        var_v0 = gCurrentGameTask->timer;
        temp_a1 = var_v0;
        if ((gPlayerInputPressed & 0x10800) && (var_v0 != 0)) {
            gCurrentGameTask->timer = var_v0 - 1;
            var_v0 = gCurrentGameTask->timer;
        } else if ((gPlayerInputPressed & 0x20400) && (var_v0 != 1)) {
            gCurrentGameTask->timer = var_v0 + 1;
            var_v0 = gCurrentGameTask->timer;
        }
        if (temp_a1 != var_v0) {
            enqueueSoundEffect(0x19, 0x32);
        }
        if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
            enqueueSoundEffect(0x18, 0x32);
            gMenuFlowState = gCurrentGameTask->timer;
            gControllerPakContinuePromptTransition.state = 2;
            gControllerPakContinuePromptTransition.x = 0x100;
            setCurrentGameTaskCallback(closeControllerPakContinuePromptFlow, 0);
        }
    }
    updateCallbackTasks();
    if (gCurrentGameTask) {
    }
}

void closeControllerPakContinuePromptFlow(void) {
    if (gCurrentGameTask->fade != 0xFF) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) gCurrentGameTask->fade, 0x24, 1);
        if (gCurrentGameTask->fade == 0xFF) {
            gFramebufferSwapHold = 1;
            gControllerPakContinuePromptTransition.state = 3;
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
