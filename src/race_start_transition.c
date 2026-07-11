#include "common.h"
#include "asset_manager.h"
#include "sound_manager.h"
#include "callback_task_scheduler.h"
#include "fixed_point_math.h"
#include "game_task_scheduler.h"
#include "main_menu_screen_effects.h"
#include "race_camera.h"
#include "race_start_transition.h"
#include "viewport_manager.h"

typedef struct {
    s32 x;
    s32 y;
} Vec2i;

typedef s16 Matrix4s[0x10];

typedef struct {
    char pad[0x18];
    s32 fade;
} RaceStartTransitionState;

typedef struct {
    u8 pad0[0x35];
    u8 unk35;
    u8 unk36;
    u8 unk37;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    u8 pad3B[2];
    u8 unk3D;
    u8 pad3E[0xE];
    u8 unk4C;
} RaceSetupSaveData;

typedef struct {
    char pad0[0x4];
    s16 pitch;
    s16 yaw;
    char pad8[0x10];
    Vec3i pos;
    s32 depth;
    char pad28[0x8];
    char rotationMtx[0x14];
    Vec3i cameraDelta;
} MenuCameraObject;

typedef struct {
    Matrix4s rotationMtx;
    Vec3i worldPos;
    Vec3i localPos;
} TransformScratch;

extern void releaseMenuAssetHandles(void);
extern s8 gFramebufferSwapDelay;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern s16 gMenuFadeAlpha;

extern RaceStartTransitionState *gCurrentGameTask;
extern MenuCameraObject *gCurrentMenuCameraObject;
extern Vec2i gMenuCameraTargetOffset;

void updateMenuCameraObjectLookAtOrigin(void) {
    TransformScratch scratch;
    MenuCameraObject *obj;

    obj = gCurrentMenuCameraObject;
    makeFixedRotationYX(obj->rotationMtx, -obj->pitch, -obj->yaw);
    obj = gCurrentMenuCameraObject;
    scratch.localPos.x = 0;
    scratch.localPos.y = 0;
    scratch.localPos.z = -obj->depth;
    makeFixedRotationXY(scratch.rotationMtx, obj->pitch, obj->yaw);
    transformVec3iByFixedMatrix(scratch.rotationMtx, &scratch.localPos.x, &scratch.worldPos.x);
    obj = gCurrentMenuCameraObject;
    obj->cameraDelta.x = scratch.worldPos.x - obj->pos.x;
    obj = gCurrentMenuCameraObject;
    obj->cameraDelta.y = scratch.worldPos.y - obj->pos.y;
    obj = gCurrentMenuCameraObject;
    obj->cameraDelta.z = scratch.worldPos.z - obj->pos.z;
}

void updateMenuCameraObject(void) {
    updateMenuCameraObjectLookAtOrigin();
}

void updateMenuCameraObjectWithTargetOffset(void) {
    TransformScratch scratch;
    MenuCameraObject *obj;

    obj = gCurrentMenuCameraObject;
    makeFixedRotationYX(obj->rotationMtx, -obj->pitch, -obj->yaw);
    obj = gCurrentMenuCameraObject;
    scratch.localPos.x = gMenuCameraTargetOffset.x;
    scratch.localPos.y = gMenuCameraTargetOffset.y;
    scratch.localPos.z = -obj->depth;
    makeFixedRotationXY(scratch.rotationMtx, obj->pitch, obj->yaw);
    transformVec3iByFixedMatrix(scratch.rotationMtx, &scratch.localPos.x, &scratch.worldPos.x);
    obj = gCurrentMenuCameraObject;
    obj->cameraDelta.x = scratch.worldPos.x - obj->pos.x;
    obj = gCurrentMenuCameraObject;
    obj->cameraDelta.y = scratch.worldPos.y - obj->pos.y;
    obj = gCurrentMenuCameraObject;
    obj->cameraDelta.z = scratch.worldPos.z - obj->pos.z;
}

void updateMenuCameraObjectFromTargetOffset(void) {
    updateMenuCameraObjectWithTargetOffset();
}

// initRaceStartTransition best match: 83.044% (nonmatchings/initRaceStartTransition-4923837976568703863/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_start_transition/initRaceStartTransition.s")

#ifdef NON_MATCHING
extern void loadMainMenuSceneModelAnimationBank();
extern void initCallbackTaskScheduler(s32);
extern void *createCallbackTask(void (*)(), s32, s32);
extern u8 D_13F3B0[];
extern u8 D_145380[];
extern u8 D_1467B0[];
extern u8 D_1D3070[];
extern u8 D_1D82B0[];
extern u8 D_1DCED0[];
extern u8 D_1DE360[];
extern u8 D_1E0F70[];
extern u8 D_1E19C0[];
extern u8 D_5DAF30[];
extern u8 D_5DB9D0[];
extern u8 D_800BB810[];
extern u8 D_800BB811[];
extern u8 D_800BB812[];
extern u8 D_800BB813[];
extern s8 D_800DC4C0;
extern u8 gRaceSplitscreenMode;
extern RaceSetupSaveData gGameSaveDataBuffer;
extern u8 D_800ECA24;
extern u8 gPlayerCount;
extern u8 D_80121D90;

void initRaceStartTransition(s32 arg0, RaceSetupSaveData *unused) {
    RaceSetupSaveData *save;
    s32 transition;
    s32 effectArg;
    s32 allReady;
    u8 state;

    transition = 0;
    if (gRaceSplitscreenMode == 0) {
        arg0 = 1;
        if (gPlayerCount == 1) {
            save = &gGameSaveDataBuffer;
            state = save->unk4C;
            if ((state == 2) && (save->unk3A == 1)) {
                save->unk4C = 3;
                D_800DC4C0 = 1;
                state = save->unk4C;
                transition = 3;
            }
            if ((state == 1) && (save->unk39 == 1)) {
                transition = 2;
                save->unk4C = 2;
                state = 2;
            }
            if (state == 0) {
                if (save->unk3D == 1) {
                    allReady = 1;
                    if (D_800ECA24 != 1) {
                        allReady = 0;
                    }
                    if (save->unk35 != 1) {
                        allReady = 0;
                    }
                    if (save->unk36 != 1) {
                        allReady = 0;
                    }
                    if (save->unk37 != 1) {
                        allReady = 0;
                    }
                    if (save->unk38 != 1) {
                        allReady = 0;
                    }
                    if (allReady != 0) {
                        transition = 1;
                        save->unk4C = 1;
                    }
                }
            }
        }
    }

    if (transition == 0) {
        resumeGameTask(2);
        removeGameTask(4);
        return;
    }

    loadMainMenuSceneModelAnimationBank(1, save, transition);
    loadCompressedRomAsset(D_5DAF30, D_5DB9D0, 0x2A);
    loadCompressedRomAsset(D_1E0F70, D_1E19C0, 0x22);
    loadRawRomAsset(D_145380, D_1467B0, 8);
    loadCompressedRomAsset(D_1DCED0, D_1DE360, 9);
    loadRawRomAsset(D_13F3B0, D_145380, 0xE);
    loadCompressedRomAsset(D_1D3070, D_1D82B0, 0x12);
    initCallbackTaskScheduler(2);
    func_8006D5CC();
    func_8006D580(0, 0x1D);
    func_8006D580(1, 0x1D);
    func_8006D580(2, 0x1D);
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xC0, 0x140, 0xF0, 1.333333373f);
    configureViewport(1, 0xA0, 0x78, 0x120, 0xC0, 0x140, 0xF0, 1.333333373f);
    configureViewport(2, 0xA0, 0x78, 0x120, 0xC0, 0x140, 0xF0, 1.333333373f);
    func_80070E90(2);
    gMenuFadeAlpha = 0xFF;
    gCurrentGameTask->fade = 5;
    effectArg = transition - 1;
    createCallbackTaskWithUserId(initRaceSetupBackdrop, 0, 0x64, effectArg);
    createCallbackTask(initMainMenuModeBoardTransition, 0, 0x64);
    createCallbackTaskWithUserId(initMainMenuModeLabelFadeIn, 0, 0x64, effectArg);
    createCallbackTaskWithUserId(initRaceSetupCharacterFocus, 0, 0x64, D_80121D90);
    createRaceSetupOpponentFocus(1, D_800BB810[D_80121D90 * 4]);
    createRaceSetupOpponentFocus(2, D_800BB811[D_80121D90 * 4]);
    createRaceSetupOpponentFocus(3, D_800BB812[D_80121D90 * 4]);
    createRaceSetupOpponentFocus(4, D_800BB813[D_80121D90 * 4]);
    setCurrentGameTaskCallback(updateRaceStartTransitionIntroDelay, 0);
}
#endif

void updateRaceStartTransitionIntroDelay(void) {
    gCurrentGameTask->fade--;
    if (gCurrentGameTask->fade == 0) {
        requestMusicSequenceBank(4);
        gCurrentGameTask->fade = 0x12C;
        setCurrentGameTaskCallback(updateRaceStartTransitionFadeIn, 0);
    }
    createCallbackTaskWithUserId(initMenuSnowflake, 5, 0x64, 0);
    func_8006D780(0);
    func_8006D780(1);
    func_8006D780(2);
    updateCallbackTasks();
}

void updateRaceStartTransitionFadeIn(void) {
    RaceStartTransitionState **state;
    RaceStartTransitionState *currentState;

    gMenuFadeAlpha -= 8;
    if (gMenuFadeAlpha < 0) {
        gMenuFadeAlpha = 0;
    }
    state = &gCurrentGameTask;
    currentState = *state;
    currentState->fade -= 1;
    if ((*state)->fade == 0) {
        requestMusicSequenceStop(0x7E);
        setCurrentGameTaskCallback(updateRaceStartTransitionFadeOut, 0);
    }
    createCallbackTaskWithUserId(initMenuSnowflake, 5, 0x64, 0);
    func_8006D780(0);
    func_8006D780(1);
    updateCallbackTasks();
}

void updateRaceStartTransitionFadeOut(void) {
    gMenuFadeAlpha += 4;
    if (gMenuFadeAlpha >= 0xFF) {
        gCurrentGameTask->fade = 0xFF;
        gFramebufferSwapHold = 1;
        setCurrentGameTaskCallback(finishRaceStartTransition, 0);
    }
    createCallbackTaskWithUserId(initMenuSnowflake, 5, 0x64, 0);
    func_8006D780(0);
    func_8006D780(1);
    updateCallbackTasks();
}

void finishRaceStartTransition(void) {
    if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        resumeGameTask(2);
        removeGameTask(4);
    }
}
