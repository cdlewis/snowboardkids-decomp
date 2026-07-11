#include "common.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "character_select_course_menu.h"
#include "controller_pak_replay_save_message_flow.h"
#include "controller_pak_ui.h"
#include "game_task_scheduler.h"
#include "menu_renderer.h"
#include "viewport_manager.h"

extern void releaseMenuAssetHandles(void);

extern CharacterSelectFlowState *gCurrentGameTask;
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
    gCurrentGameTask->fade = 0xFF;
    gCurrentGameTask->timer = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    loadCompressedRomAsset(gControllerPakReplaySaveMessageFirstPageStart,
                           gMainMenuSceneModelAssetStart, 0x26);
    initCallbackTaskScheduler(0);
    createCallbackTask(initControllerPakMessageIcon, 0, 0x5E);
    setCurrentGameTaskCallback(updateControllerPakReplaySaveMessageFirstPageFadeIn, 0);
    updateCallbackTasks();
}

void updateControllerPakReplaySaveMessageFirstPageFadeIn(void) {
    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha(gCurrentGameTask->fade, 0xF, 0);
    } else {
        gCurrentGameTask->timer++;
    }
    if (gCurrentGameTask->timer >= 0x50) {
        gCurrentGameTask->timer = 0;
        setCurrentGameTaskCallback(updateControllerPakReplaySaveMessageFirstPageFadeOut, 0);
    }
    updateCallbackTasks();
}

void updateControllerPakReplaySaveMessageFirstPageFadeOut(void) {
    if (gCurrentGameTask->fade != 0xFF) {
        gCurrentGameTask->fade = stepMenuFadeAlpha(gCurrentGameTask->fade, 0xF, 1);
        if (gCurrentGameTask->fade == 0xFF) {
            releaseMenuAssetHandles();
            loadCompressedRomAsset(gControllerPakReplaySaveMessageSecondPageStart,
                                   gControllerPakReplaySaveMessageFirstPageStart, 0x26);
        }
    } else {
        setCurrentGameTaskCallback(waitForControllerPakReplaySaveMessageSecondPage, 0);
    }
    updateCallbackTasks();
}

void waitForControllerPakReplaySaveMessageSecondPage(void) {
    gCurrentGameTask->timer++;
    if (gCurrentGameTask->timer >= 0x14) {
        gCurrentGameTask->fade = 0xFF;
        gMenuFadeAlpha = gCurrentGameTask->fade;
        gCurrentGameTask->timer = 0;
        setCurrentGameTaskCallback(updateControllerPakReplaySaveMessageSecondPageFadeIn, 0);
    }
    updateCallbackTasks();
}

void updateControllerPakReplaySaveMessageSecondPageFadeIn(void) {
    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha(gCurrentGameTask->fade, 0xF, 0);
    } else {
        gCurrentGameTask->timer++;
    }
    if (gCurrentGameTask->timer >= 0x50) {
        setCurrentGameTaskCallback(fadeOutControllerPakReplaySaveMessageFlow, 0);
    }
    updateCallbackTasks();
}

void fadeOutControllerPakReplaySaveMessageFlow(void) {
    if (gCurrentGameTask->fade != 0xFF) {
        gCurrentGameTask->fade = stepMenuFadeAlpha(gCurrentGameTask->fade, 0xF, 1);
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
            resumeGameTask(0);
            removeGameTask(4);
        }
    }
}
