#include "common.h"
#include "sound_manager.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "character_select_course_menu.h"
#include "controller_pak_ui.h"
#include "game_task_scheduler.h"
#include "menu_renderer.h"
#include "shop_menu_flow.h"
#include "viewport_manager.h"

typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ u8 state;
} ControllerPakContinuePromptTransition;

extern void enqueueSoundEffect(s32, s32);
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

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_flow/updateControllerPakContinuePromptFlow.s")

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
