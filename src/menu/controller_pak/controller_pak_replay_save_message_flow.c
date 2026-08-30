#include "common.h"
#include "assets.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/menu/controller_pak/controller_pak_replay_save_message_flow.h"
#include "game/menu/controller_pak/controller_pak_ui.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/engine/viewport_manager.h"

extern s16 gMenuFadeAlpha;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

void initControllerPakReplaySaveMessageFlow(void) {
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
    gFramebufferRenderInterval.value = 0;
    gCurrentGameTask->callbackData0 = 0xFF;
    gCurrentGameTask->callbackData1 = 0;
    gMenuFadeAlpha = gCurrentGameTask->callbackData0;
    LOAD_ASSET(_5E26E0, 0x26);
    initCallbackTaskScheduler(0);
    createCallbackTask((CallbackTaskCallback)initControllerPakMessageIcon, 0, 0x5E);
    setCurrentGameTaskCallback(updateControllerPakReplaySaveMessageFirstPageFadeIn, 0);
    updateCallbackTasks();
}

void updateControllerPakReplaySaveMessageFirstPageFadeIn(void) {
    if (gCurrentGameTask->callbackData0 != 0) {
        gCurrentGameTask->callbackData0 = stepMenuFadeAlpha(gCurrentGameTask->callbackData0, 0xF, 0);
    } else {
        gCurrentGameTask->callbackData1++;
    }
    if (gCurrentGameTask->callbackData1 >= 0x50) {
        gCurrentGameTask->callbackData1 = 0;
        setCurrentGameTaskCallback(updateControllerPakReplaySaveMessageFirstPageFadeOut, 0);
    }
    updateCallbackTasks();
}

void updateControllerPakReplaySaveMessageFirstPageFadeOut(void) {
    if (gCurrentGameTask->callbackData0 != 0xFF) {
        gCurrentGameTask->callbackData0 = stepMenuFadeAlpha(gCurrentGameTask->callbackData0, 0xF, 1);
        if (gCurrentGameTask->callbackData0 == 0xFF) {
            releaseMenuAssetHandles();
            LOAD_ASSET(_5E0E40, 0x26);
        }
    } else {
        setCurrentGameTaskCallback(waitForControllerPakReplaySaveMessageSecondPage, 0);
    }
    updateCallbackTasks();
}

void waitForControllerPakReplaySaveMessageSecondPage(void) {
    gCurrentGameTask->callbackData1++;
    if (gCurrentGameTask->callbackData1 >= 0x14) {
        gCurrentGameTask->callbackData0 = 0xFF;
        gMenuFadeAlpha = gCurrentGameTask->callbackData0;
        gCurrentGameTask->callbackData1 = 0;
        setCurrentGameTaskCallback(updateControllerPakReplaySaveMessageSecondPageFadeIn, 0);
    }
    updateCallbackTasks();
}

void updateControllerPakReplaySaveMessageSecondPageFadeIn(void) {
    if (gCurrentGameTask->callbackData0 != 0) {
        gCurrentGameTask->callbackData0 = stepMenuFadeAlpha(gCurrentGameTask->callbackData0, 0xF, 0);
    } else {
        gCurrentGameTask->callbackData1++;
    }
    if (gCurrentGameTask->callbackData1 >= 0x50) {
        setCurrentGameTaskCallback(fadeOutControllerPakReplaySaveMessageFlow, 0);
    }
    updateCallbackTasks();
}

void fadeOutControllerPakReplaySaveMessageFlow(void) {
    if (gCurrentGameTask->callbackData0 != 0xFF) {
        gCurrentGameTask->callbackData0 = stepMenuFadeAlpha(gCurrentGameTask->callbackData0, 0xF, 1);
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
            resumeGameTask(0);
            removeGameTask(4);
        }
    }
}
