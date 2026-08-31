#include "common.h"
#include "game/save_data.h"
#include "assets.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/menu/controller_pak/controller_pak_race_record_save_flow.h"
#include "game/menu/controller_pak/controller_pak_menu.h"
#include "game/menu/controller_pak/controller_pak_ui.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/controller_pak/controller_pak_race_record_save_ui.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/race_setup/race_setup_menu.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"

typedef struct RaceRecordSaveCompletion {
    s32 value;
    s32 pad4;
    s32 pad8;
} RaceRecordSaveCompletion;

extern ControllerPakMenuState gControllerPakMenuState;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern u8 gControllerPakMenuCursorState;
extern s32 gMenuFlowState;
extern s32 D_8010ADE0;
extern CallbackTask *D_8010ADE4;
extern s16 gMenuFadeAlpha;

void initControllerPakRaceRecordSaveFlow(void) {
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    gFramebufferRenderInterval.value = 0;
    gControllerPakStatusCodes[0] = 0;
    gMenuChoicePromptState[0] = 0;
    gControllerPakRetryCounts[0] = 0;
    gRacePlayers[0].menuState = 0;
    gMenuSelectionConfirmTimer = 0;
    gCurrentGameTask->callbackData0 = 0xFF;
    gActiveMenuTask = 0;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    D_8010ADE8 = 0;
    gMenuFadeAlpha = gCurrentGameTask->callbackData0;
    gGameSaveDataBuffer[0].money = gRacePlayers[0].money;
    LOAD_ASSET(_59AAA0, 0x21);
    LOAD_ASSET(_59AAA0, 0x24);
    LOAD_ASSET(_593D10, 0x22);
    LOAD_ASSET(_598A70, 0x23);
    LOAD_ASSET(_60F1A0, 0x29);
    initCallbackTaskScheduler(0);
    gActiveMenuTask = createCallbackTask((CallbackTaskCallback)&initControllerPakRaceRecordSaveScorePanel, 0, 0x61);
    D_8010ADE8 = createCallbackTask((CallbackTaskCallback)&initControllerPakRaceRecordSaveStatusChoicePrompt, 0, 0x60);
    gControllerPakRaceRecordSaveStatusTransition.step = 0;
    gControllerPakRaceRecordSaveStatusTransition.alpha = 0;
    gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0;
    gControllerPakRaceRecordSaveStatusTransition.nextStatus = 0;
    gControllerPakMenuState.state = 0;
    gControllerPakMenuState.confirmChoice = 0;
    setCurrentGameTaskCallback(updateControllerPakRaceRecordSaveFlow, 0);
}

#include "updateControllerPakRaceRecordSaveFlow.inc.c"


void fadeOutControllerPakRaceRecordSaveFlow(void) {
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

void updateControllerPakRaceRecordSaveOverwritePrompt(void) {
    if ((gPlayerInputPressed[0] & (STICK_UP | U_JPAD)) && (gControllerPakMenuState.confirmChoice != 0)) {
        gControllerPakMenuState.confirmChoice = 0;
        enqueueSoundEffect(0x19, 0x32);
    } else if ((gPlayerInputPressed[0] & (STICK_DOWN | D_JPAD)) && (gControllerPakMenuState.confirmChoice != 1)) {
        gControllerPakMenuState.confirmChoice = 1;
        enqueueSoundEffect(0x19, 0x32);
    }
    if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
        enqueueSoundEffect(0x18, 0x32);
        if (gControllerPakMenuState.confirmChoice == 0) {
            gControllerPakMenuCursorState = 0;
            setCurrentGameTaskCallback(updateControllerPakRaceRecordSaveFlow, 0);
        } else {
            gMenuFlowState = 1;
            setCurrentGameTaskCallback(fadeOutControllerPakRaceRecordSaveFlow, 0);
        }
    } else if (gPlayerInputPressed[0] & B_BUTTON) {
        enqueueSoundEffect(0x18, 0x32);
        gMenuFlowState = 1;
        setCurrentGameTaskCallback(fadeOutControllerPakRaceRecordSaveFlow, 0);
    }
    updateCallbackTasks();
}
