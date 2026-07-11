#include "ending_credits_transition.h"
#include "relocatable_heap.h"
#include "sound_manager.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "system_boot.h"
#include "game_task_scheduler.h"
#include "race_camera.h"
#include "ending_credits_ui.h"
#include "ending_credits_effect_actors.h"
#include "race_start_transition.h"
#include "ending_credits_slash.h"
#include "ending_credits_nancy.h"
#include "ending_credits_jam.h"
#include "ending_credits_linda.h"
#include "ending_credits_tommy.h"
#include "main_menu_scene_model.h"
#include "viewport_manager.h"
#define MENU_RENDERER_S16_STEP_PROTOTYPE
#include "menu_renderer.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 fade;
    /* 0x1C */ s32 timer;
} EndingCreditsTransitionState;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s16 yaw;
    /* 0x06 */ u8 pad6[0x1E];
    /* 0x24 */ s32 depth;
    /* 0x28 */ u8 pad28[0x4];
    /* 0x2C */ void (*update)(void);
} MenuCameraObject;

typedef struct {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ s32 z;
} Vec3i;

typedef struct {
    /* 0x00 */ s8 value;
} MainMenuFlagByte;

typedef void (*CallbackTaskCallback)(void *);

extern void releaseMenuAssetHandles(void);

extern u8 D_14B450[];
extern u8 D_1502A0[];
extern u8 D_1EF530[];
extern u8 D_1F1A90[];
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_608560[];
extern u8 D_609AA0[];
extern u8 D_60ECB0[];
extern u8 D_60F1A0[];
extern s8 gFramebufferSwapDelay;
extern s16 gMenuFadeAlpha;
extern f32 D_800E0A70;
extern s16 D_8010B1A0;
extern u16 gEndingSequencePhase;
extern s8 gEndingActorHandshakeState;
extern s8 gEndingTransitionRotationStep;
extern s16 gEndingTransitionSnowboardAngle;
extern MainMenuFlagByte gEndingCharacterEffectDoneFlags[];
extern s8 gEndingNancyEffectDone;
extern s8 gEndingJamEffectDone;
extern s8 gEndingLindaEffectDone;
extern s8 gEndingTommyEffectDone;
extern Vec3i gMenuCameraTargetOffset;
extern s16 gAssetHandles[];
extern MenuCameraObject D_801121E0;
extern MenuCameraObject *gCurrentMenuCameraObject;
extern EndingCreditsTransitionState *gCurrentGameTask;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

// initEndingCreditsTransition best match: 93.231% at nonmatchings/initEndingCreditsTransition-1197934324348345530/base_6.c.
#pragma GLOBAL_ASM("asm/nonmatchings/ending_credits_transition/initEndingCreditsTransition.s")

#ifdef NON_MATCHING
void initEndingCreditsTransition(void) {
    s32 sp34;
    s32 temp_v0;
    EndingCreditsTransitionState *state;

    resetRaceCameras();
    D_801121E0.update = updateMenuCameraObjectFromTargetOffset;
    D_801121E0.depth = 0x5D24000;
    D_801121E0.yaw = 0xFC0;
    resetAllViewports();
    func_80070860(0, 0xA0, 0x38, 0x120, 0x50, 0x140, 0xF0, D_800E0A70, 0x14, 0xAF0);
    gFramebufferSwapDelay = 0;
    gMenuCameraTargetOffset.x = 0;
    gMenuCameraTargetOffset.y = 0xFFB60000;
    gMenuCameraTargetOffset.z = 0;
    D_8010B1A0 = 0;
    gEndingSequencePhase = 0;
    gEndingActorHandshakeState = 0;
    gEndingTransitionSnowboardAngle = 0;
    gEndingTransitionRotationStep = 0;
    gEndingCharacterEffectDoneFlags[0].value = 0;
    gEndingCharacterEffectDoneFlags[2].value = 0;
    gEndingCharacterEffectDoneFlags[3].value = 0;
    gEndingCharacterEffectDoneFlags[4].value = 0;
    gEndingCharacterEffectDoneFlags[1].value = 0;
    loadCompressedRomAsset(D_608560, D_609AA0, 0x21);
    loadCompressedRomAsset(D_593D10, D_598A70, 0x22);
    loadCompressedRomAsset(D_609AA0, D_60ECB0, 0x26);
    loadCompressedRomAsset(D_60ECB0, D_60F1A0, 0x27);
    temp_v0 = D_1502A0 - D_14B450;
    sp34 = temp_v0;
    gAssetHandles[0xC] = allocRelocatableHeapBlock(temp_v0);
    dmaReadRom(D_14B450, getRelocatableHeapBlockBase(gAssetHandles[0xC]), sp34);
    loadCompressedRomAsset(D_1EF530, D_1F1A90, 0xD);
    loadMainMenuSceneModelAssets();
    initCallbackTaskScheduler(0);
    createCallbackTask(initEndingCreditsTransitionSnowboardIcon, 0, 0x64);
    createCallbackTask(initEndingCreditsTransitionLogoWipe, 0, 0x64);
    createCallbackTask(initEndingCreditsTransitionSnowflakeIcon, 0, 0x64);
    gCurrentGameTask->fade = 0xFF;
    state = gCurrentGameTask;
    gMenuFadeAlpha = state->fade;
    gCurrentGameTask->timer = 5;
    setCurrentGameTaskCallback(updateEndingCreditsTransitionFadeIn, 0);
}
#endif

void updateEndingCreditsTransitionFadeIn(void) {
    if (gCurrentGameTask->timer != 0) {
        gCurrentGameTask->timer--;
    } else {
        if (gCurrentGameTask->fade != 0) {
            gCurrentGameTask->fade = stepMenuFadeAlpha(gCurrentGameTask->fade, 0x10, 0);
        } else {
            setCurrentGameTaskCallback(updateEndingCreditsTransition, 0);
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

void updateEndingCreditsTransition(void) {
    if (gEndingSequencePhase == 0x43) {
        gCurrentGameTask->timer++;
        if (gCurrentGameTask->timer == 0x12C) {
            gCurrentGameTask->timer = 0;
            setCurrentGameTaskCallback(updateEndingCreditsTransitionFadeOut, 0);
        }
    }
    updateCallbackTasks();
    gCurrentMenuCameraObject = &D_801121E0;
    gCurrentMenuCameraObject->update();
}

void updateEndingCreditsTransitionFadeOut(void) {
    if (gCurrentGameTask->fade != 0xFF) {
        gCurrentGameTask->fade = stepMenuFadeAlpha(gCurrentGameTask->fade, 0x10, 1);
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
