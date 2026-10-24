#include "common.h"
#include "game/menu/menu_scratch.h"
#include "assets.h"
#include "game/save_data.h"
#include "game/engine/asset_manager.h"
#include "game/engine/viewport_manager.h"
#include "game/menu/controller_pak/controller_pak_race_record_save_flow.h"
#include "game/menu/controller_pak/controller_pak_race_record_save_ui.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/race/player/race_player_input.h"
#include "game/engine/game_task_scheduler.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"

extern f32 D_800DFD10;

void func_80008EBC(void);

void initControllerPakRaceRecordSaveFlow(void) {
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, D_800DFD10);
    gFramebufferRenderInterval.value = 0;
    gControllerPakStatusCodes[0] = 0;
    gMenuChoicePromptState[0] = 0;
    gControllerPakRetryCounts[0] = 0;
    gRacePlayers[0].menuState = 0;
    gMenuSelectionConfirmTimer = 0;
    gCurrentGameTask->callbackData0 = 0xFF;
    gActiveMenuTask = 0;
    gMenuScratch0.value = 0;
    gMenuScratch1.task = 0;
    gMenuScratch2.task = 0;
    gMenuFadeAlpha = gCurrentGameTask->callbackData0;
    gGameSaveDataBuffer[0].money = gRacePlayers[0].money;
    LOAD_ASSET(_59AAA0, 0x21);
    LOAD_ASSET(_593D10, 0x22);
    LOAD_ASSET(_598A70, 0x23);
    initCallbackTaskScheduler(0);
    gActiveMenuTask = createCallbackTask((CallbackTaskCallback)&initControllerPakRaceRecordSaveScorePanel, 0, 0x61);
    gMenuScratch2.task =
        createCallbackTask((CallbackTaskCallback)&initControllerPakRaceRecordSaveStatusChoicePrompt, 0, 0x60);
    gControllerPakRaceRecordSaveStatusTransition.step = 0;
    gControllerPakRaceRecordSaveStatusTransition.alpha = 0;
    gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0;
    gControllerPakRaceRecordSaveStatusTransition.nextStatus = 0;
    setCurrentGameTaskCallback(func_80008EBC, 0);
}

#pragma GLOBAL_ASM("asm.jp/nonmatchings/menu/controller_pak/controller_pak_race_record_save_flow/func_80008EBC.s")

void func_8000951C(void) {
    s32 temp_v0 = gCurrentGameTask->callbackData0;
    if (temp_v0 != 0xFF) {
        gCurrentGameTask->callbackData0 = stepMenuFadeAlpha((s16)temp_v0, 0x20, 1);
        updateCallbackTasks();
        if (gCurrentGameTask->callbackData0 == 0xFF) {
            gFramebufferSwapHold = 1;
        }
    } else if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferRenderInterval.value = 0;
        resumeGameTask(2);
        removeGameTask(4);
    }
}
