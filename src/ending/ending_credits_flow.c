#include "assets.h"
#include "game/ending/ending_credits_flow.h"
#include "game/engine/relocatable_heap.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/engine/system_runtime.h"
#include "game/engine/game_task_scheduler.h"
#include "game/race/camera/race_camera.h"
#include "game/ending/ending_credits_ui.h"
#include "game/ending/ending_credits_effects.h"
#include "game/race/effects/race_start_transition.h"
#include "game/ending/ending_credits_slash.h"
#include "game/ending/ending_credits_nancy.h"
#include "game/ending/ending_credits_jam.h"
#include "game/ending/ending_credits_linda.h"
#include "game/ending/ending_credits_tommy.h"
#include "game/menu/main_menu/main_menu_scene_model.h"
#include "game/engine/viewport_manager.h"
#include "game/menu/renderer/menu_renderer.h"

/* These globals form the original 0x10-byte ending_credits_flow BSS contribution. */
s16 gEndingCreditsUnusedValue;
u16 gEndingCreditsSequencePhase;
u8 gEndingCreditsHandshakeState;
u8 gEndingCreditsTransitionSnowboardIconSpinStep;
s16 gEndingCreditsTransitionSnowboardIconAngle;
u8 gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_COUNT];

extern void releaseMenuAssetHandles(void);

extern s8 gFramebufferSwapDelay;
extern s16 gMenuFadeAlpha;
extern RaceCamera D_801121E0;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

const f32 gEndingCreditsViewportAspectRatio[4] = { 1.333333373f, 0.0f, 0.0f, 0.0f };

void initEndingCreditsFlow(void) {
    s32 sp34;
    s32 temp_v0;
    volatile GameTask *state;

    resetRaceCameras();
    D_801121E0.update = updateMenuCameraObjectWithTargetOffsetCallback;
    D_801121E0.distance = 0x5D24000;
    D_801121E0.pitch = 0xFC0;
    resetAllViewports();
    configureViewportWithFovAndFarClip(0, 0xA0, 0x38, 0x120, 0x50, 0x140, 0xF0, gEndingCreditsViewportAspectRatio[0], 0x14, 0xAF0);
    gFramebufferSwapDelay = 0;
    gMenuCameraTargetOffset.x = 0;
    if (1) {
        gMenuCameraTargetOffset.y = 0xFFB60000;
        gMenuCameraTargetOffset.z = 0;
        gEndingCreditsUnusedValue = 0;
        gEndingCreditsSequencePhase = 0;
        temp_v0 = 0;
        gEndingCreditsHandshakeState = 0;
        gEndingCreditsTransitionSnowboardIconAngle = 0;
        if (1) {
            gEndingCreditsTransitionSnowboardIconSpinStep = 0;
            gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_SLASH] = temp_v0;
            temp_v0++;
            gEndingCreditsCharacterAuraDoneFlags[temp_v0 + 1] = 0;
            gEndingCreditsCharacterAuraDoneFlags[temp_v0 + 2] = 0;
            gEndingCreditsCharacterAuraDoneFlags[temp_v0 + 3] = 0;
            gEndingCreditsCharacterAuraDoneFlags[temp_v0] = 0;
            LOAD_ASSET(_608560, 0x21);
            LOAD_ASSET(_593D10, 0x22);
            LOAD_ASSET(_609AA0, 0x26);
            LOAD_ASSET(_60ECB0, 0x27);
            temp_v0 = ASSET_SIZE(_14B450);
            sp34 = temp_v0;
            gAssetHandles[0xC] = allocRelocatableHeapBlock((unsigned long) temp_v0);
            DMA_ASSET(_14B450, getRelocatableHeapBlockBase(gAssetHandles[0xC]), sp34);
            LOAD_ASSET(_1EF530, 0xD);
            loadMainMenuSceneModelAssets();
            initCallbackTaskScheduler(0);
            createCallbackTask((CallbackTaskCallback)initEndingCreditsTransitionSnowboardIcon, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)initEndingCreditsTransitionLogoWipe, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)initEndingCreditsTransitionSnowflakeIcon, 0, 0x64);
        }
        gCurrentGameTask->fade = 0xFF;
    }
    state = gCurrentGameTask;
    gMenuFadeAlpha = state->fade;
    gCurrentGameTask->timer = 5;
    setCurrentGameTaskCallback(fadeInEndingCreditsFlow, 0);
}

void fadeInEndingCreditsFlow(void) {
    if (gCurrentGameTask->timer != 0) {
        gCurrentGameTask->timer--;
    } else {
        if (gCurrentGameTask->fade != 0) {
            gCurrentGameTask->fade = stepMenuFadeAlpha((s16)gCurrentGameTask->fade, 0x10, 0);
        } else {
            setCurrentGameTaskCallback(updateEndingCreditsFlow, 0);
            createCallbackTask((CallbackTaskCallback) initEndingCreditsPageTextActor, 0, 0x64);
            createCallbackTask((CallbackTaskCallback) initEndingCreditsSlash, 0, 0x64);
            createCallbackTask((CallbackTaskCallback) initEndingCreditsNancy, 0, 0x64);
            createCallbackTask((CallbackTaskCallback) initEndingCreditsTommy, 0, 0x64);
            createCallbackTask((CallbackTaskCallback) initEndingCreditsJam, 0, 0x64);
            createCallbackTask((CallbackTaskCallback) initEndingCreditsLinda, 0, 0x64);
            requestMusicSequenceBank(0xA);
        }
    }
    updateCallbackTasks();
}

void updateEndingCreditsFlow(void) {
    if (gEndingCreditsSequencePhase == 0x43) {
        gCurrentGameTask->timer++;
        if (gCurrentGameTask->timer == 0x12C) {
            gCurrentGameTask->timer = 0;
            setCurrentGameTaskCallback(fadeOutEndingCreditsFlow, 0);
        }
    }
    updateCallbackTasks();
    gCurrentMenuCameraObject = &D_801121E0;
    gCurrentMenuCameraObject->update();
}

void fadeOutEndingCreditsFlow(void) {
    if (gCurrentGameTask->fade != 0xFF) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16)gCurrentGameTask->fade, 0x10, 1);
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
            resumeGameTask(2);
            removeGameTask(4);
        }
    }
}
