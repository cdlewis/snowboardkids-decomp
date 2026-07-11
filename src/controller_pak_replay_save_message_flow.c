#include "common.h"
#include "effect_task_scheduler.h"
#include "asset_manager.h"
#include "character_select_course_menu.h"
#include "controller_pak_replay_save_message_flow.h"
#include "controller_pak_ui.h"
#include "input_task_scheduler.h"
#include "menu_rendering.h"
#include "viewport_manager.h"

extern void releaseMenuAssetHandles(void);

extern CharacterSelectFlowState *gCurrentInputTask;
extern s8 gFramebufferSwapDelay;
extern s16 gMenuFadeAlpha;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern u8 gControllerPakReplaySaveMessageSecondPageStart[];
extern u8 gControllerPakReplaySaveMessageFirstPageStart[];
extern u8 gMainMenuSceneModelAssetStart[];

void initControllerPakReplaySaveMessageFlow(void) {
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
    gFramebufferSwapDelay = 0;
    gCurrentInputTask->fade = 0xFF;
    gCurrentInputTask->timer = 0;
    gMenuFadeAlpha = gCurrentInputTask->fade;
    loadCompressedRomAsset(gControllerPakReplaySaveMessageFirstPageStart,
                           gMainMenuSceneModelAssetStart, 0x26);
    func_80070EC0(0);
    createEffectTask(initControllerPakMessageIcon, 0, 0x5E);
    setCurrentInputTaskCallback(updateControllerPakReplaySaveMessageFirstPageFadeIn, 0);
    updateEffectTasks();
}

void updateControllerPakReplaySaveMessageFirstPageFadeIn(void) {
    if (gCurrentInputTask->fade != 0) {
        gCurrentInputTask->fade = stepMenuFadeAlpha(gCurrentInputTask->fade, 0xF, 0);
    } else {
        gCurrentInputTask->timer++;
    }
    if (gCurrentInputTask->timer >= 0x50) {
        gCurrentInputTask->timer = 0;
        setCurrentInputTaskCallback(updateControllerPakReplaySaveMessageFirstPageFadeOut, 0);
    }
    updateEffectTasks();
}

void updateControllerPakReplaySaveMessageFirstPageFadeOut(void) {
    if (gCurrentInputTask->fade != 0xFF) {
        gCurrentInputTask->fade = stepMenuFadeAlpha(gCurrentInputTask->fade, 0xF, 1);
        if (gCurrentInputTask->fade == 0xFF) {
            releaseMenuAssetHandles();
            loadCompressedRomAsset(gControllerPakReplaySaveMessageSecondPageStart,
                                   gControllerPakReplaySaveMessageFirstPageStart, 0x26);
        }
    } else {
        setCurrentInputTaskCallback(waitForControllerPakReplaySaveMessageSecondPage, 0);
    }
    updateEffectTasks();
}

void waitForControllerPakReplaySaveMessageSecondPage(void) {
    gCurrentInputTask->timer++;
    if (gCurrentInputTask->timer >= 0x14) {
        gCurrentInputTask->fade = 0xFF;
        gMenuFadeAlpha = gCurrentInputTask->fade;
        gCurrentInputTask->timer = 0;
        setCurrentInputTaskCallback(updateControllerPakReplaySaveMessageSecondPageFadeIn, 0);
    }
    updateEffectTasks();
}

void updateControllerPakReplaySaveMessageSecondPageFadeIn(void) {
    if (gCurrentInputTask->fade != 0) {
        gCurrentInputTask->fade = stepMenuFadeAlpha(gCurrentInputTask->fade, 0xF, 0);
    } else {
        gCurrentInputTask->timer++;
    }
    if (gCurrentInputTask->timer >= 0x50) {
        setCurrentInputTaskCallback(fadeOutControllerPakReplaySaveMessageFlow, 0);
    }
    updateEffectTasks();
}

void fadeOutControllerPakReplaySaveMessageFlow(void) {
    if (gCurrentInputTask->fade != 0xFF) {
        gCurrentInputTask->fade = stepMenuFadeAlpha(gCurrentInputTask->fade, 0xF, 1);
        if (gCurrentInputTask->fade == 0xFF) {
            gFramebufferSwapHold = 1;
        } else {
            updateEffectTasks();
        }
    } else {
        if (gPendingFramebufferSwapCount == 2) {
            releaseMenuAssetHandles();
            gFramebufferSwapHold = 0;
            gFramebufferSwapDelay = 0;
            resumeInputTask(0);
            removeInputTask(4);
        }
    }
}
