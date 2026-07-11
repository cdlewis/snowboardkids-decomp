#include "race_to_main_menu_transition.h"
#include "memory_allocator.h"
#include "game_audio.h"
#include "effect_task_scheduler.h"
#include "asset_manager.h"
#include "game_boot.h"
#include "input_task_scheduler.h"
#include "race_camera.h"
#include "main_menu_debug_ui.h"
#include "main_menu_effects.h"
#include "main_menu_race_setup.h"
#include "main_menu_scene_actor_0.h"
#include "main_menu_scene_actor_1.h"
#include "main_menu_scene_actor_2.h"
#include "main_menu_scene_actor_3.h"
#include "main_menu_scene_actor_4.h"
#include "main_menu_scene_model.h"
#include "viewport_manager.h"
#define MENU_RENDERING_S16_STEP_PROTOTYPE
#include "menu_rendering.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 fade;
    /* 0x1C */ s32 timer;
} RaceToMainMenuTransitionState;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s16 yaw;
    /* 0x06 */ u8 pad6[0x1E];
    /* 0x24 */ s32 depth;
    /* 0x28 */ u8 pad28[0x4];
    /* 0x2C */ void (*update)(void);
} MainMenuRaceSetupObject;

typedef struct {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ s32 z;
} Vec3i;

typedef struct {
    /* 0x00 */ s8 value;
} MainMenuFlagByte;

typedef void (*EffectTaskCallback)(void *);

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
extern u16 D_8010B1A2;
extern s8 D_8010B1A4;
extern s8 D_8010B1A5;
extern s16 D_8010B1A6;
extern MainMenuFlagByte D_8010B1A8;
extern s8 D_8010B1A9;
extern s8 D_8010B1AA;
extern s8 D_8010B1AB;
extern s8 D_8010B1AC;
extern Vec3i D_8010B1B0;
extern s16 D_80112130[];
extern MainMenuRaceSetupObject D_801121E0;
extern MainMenuRaceSetupObject *D_800EC9C4;
extern RaceToMainMenuTransitionState *gCurrentInputTask;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

// func_8000D340 best match: 93.231% at nonmatchings/func_8000D340-1197934324348345530/base_6.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_to_main_menu_transition/func_8000D340.s")

#ifdef NON_MATCHING
void func_8000D340(void) {
    s32 sp34;
    s32 temp_v0;
    RaceToMainMenuTransitionState *state;

    func_8006D5CC();
    D_801121E0.update = func_8003DFB0;
    D_801121E0.depth = 0x5D24000;
    D_801121E0.yaw = 0xFC0;
    resetAllViewports();
    func_80070860(0, 0xA0, 0x38, 0x120, 0x50, 0x140, 0xF0, D_800E0A70, 0x14, 0xAF0);
    gFramebufferSwapDelay = 0;
    D_8010B1B0.x = 0;
    D_8010B1B0.y = 0xFFB60000;
    D_8010B1B0.z = 0;
    D_8010B1A0 = 0;
    D_8010B1A2 = 0;
    D_8010B1A4 = 0;
    D_8010B1A6 = 0;
    D_8010B1A5 = 0;
    D_8010B1A8.value = 0;
    (&D_8010B1A8)[2].value = 0;
    (&D_8010B1A8)[3].value = 0;
    (&D_8010B1A8)[4].value = 0;
    (&D_8010B1A8)[1].value = 0;
    loadCompressedRomAsset(D_608560, D_609AA0, 0x21);
    loadCompressedRomAsset(D_593D10, D_598A70, 0x22);
    loadCompressedRomAsset(D_609AA0, D_60ECB0, 0x26);
    loadCompressedRomAsset(D_60ECB0, D_60F1A0, 0x27);
    temp_v0 = D_1502A0 - D_14B450;
    sp34 = temp_v0;
    D_80112130[0xC] = func_80042D58(temp_v0);
    func_80099C44(D_14B450, func_80043040(D_80112130[0xC]), sp34);
    loadCompressedRomAsset(D_1EF530, D_1F1A90, 0xD);
    func_80041CC0();
    func_80070EC0(0);
    createEffectTask(func_8003D908, 0, 0x64);
    createEffectTask(func_8003DD64, 0, 0x64);
    createEffectTask(func_8003DB1C, 0, 0x64);
    gCurrentInputTask->fade = 0xFF;
    state = gCurrentInputTask;
    gMenuFadeAlpha = state->fade;
    gCurrentInputTask->timer = 5;
    setCurrentInputTaskCallback(func_8000D590, 0);
}
#endif

void func_8000D590(void) {
    if (gCurrentInputTask->timer != 0) {
        gCurrentInputTask->timer--;
    } else {
        if (gCurrentInputTask->fade != 0) {
            gCurrentInputTask->fade = stepMenuFadeAlpha(gCurrentInputTask->fade, 0x10, 0);
        } else {
            setCurrentInputTaskCallback(func_8000D690, 0);
            createEffectTask((EffectTaskCallback) func_8003BBBC, 0, 0x64);
            createEffectTask((EffectTaskCallback) func_80035184, 0, 0x64);
            createEffectTask((EffectTaskCallback) func_800362E8, 0, 0x64);
            createEffectTask((EffectTaskCallback) func_80036FB4, 0, 0x64);
            createEffectTask((EffectTaskCallback) func_80039440, 0, 0x64);
            createEffectTask((EffectTaskCallback) func_8003B264, 0, 0x64);
            func_800720E4(0xA);
        }
    }
    updateEffectTasks();
}

void func_8000D690(void) {
    if (D_8010B1A2 == 0x43) {
        gCurrentInputTask->timer++;
        if (gCurrentInputTask->timer == 0x12C) {
            gCurrentInputTask->timer = 0;
            setCurrentInputTaskCallback(func_8000D724, 0);
        }
    }
    updateEffectTasks();
    D_800EC9C4 = &D_801121E0;
    D_800EC9C4->update();
}

void func_8000D724(void) {
    if (gCurrentInputTask->fade != 0xFF) {
        gCurrentInputTask->fade = stepMenuFadeAlpha(gCurrentInputTask->fade, 0x10, 1);
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
            resumeInputTask(2);
            removeInputTask(4);
        }
    }
}
