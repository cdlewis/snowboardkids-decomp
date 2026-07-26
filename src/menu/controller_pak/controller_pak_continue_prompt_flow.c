#include "common.h"
#include "assets.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/menu/controller_pak/controller_pak_ui.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/controller_pak/controller_pak_continue_prompt_flow.h"
#include "game/engine/viewport_manager.h"

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
    LOAD_ASSET(_593D10, 0x22);
    LOAD_ASSET(_598A70, 0x23);
    LOAD_ASSET(_59AAA0, 0x24);
    LOAD_ASSET(_60F1A0, 0x29);
    initCallbackTaskScheduler(0);
    createCallbackTask((CallbackTaskCallback)initControllerPakContinuePrompt, 0, 0x64);
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
        if ((gPlayerInputPressed & (STICK_UP | U_JPAD)) && (var_v0 != 0)) {
            gCurrentGameTask->timer = var_v0 - 1;
            var_v0 = gCurrentGameTask->timer;
        } else if ((gPlayerInputPressed & (STICK_DOWN | D_JPAD)) && (var_v0 != 1)) {
            gCurrentGameTask->timer = var_v0 + 1;
            var_v0 = gCurrentGameTask->timer;
        }
        if (temp_a1 != var_v0) {
            enqueueSoundEffect(0x19, 0x32);
        }
        if ((gPlayerInputPressed & A_BUTTON) || (gPlayerInputPressed & START_BUTTON)) {
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
