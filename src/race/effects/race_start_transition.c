#include "common.h"
#include "assets.h"
#include "game/engine/asset_manager.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/math/fixed_point_math.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/renderer/menu_screen_effects.h"
#include "game/race/camera/race_camera.h"
#include "game/race/effects/race_start_transition.h"
#include "game/engine/viewport_manager.h"

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
    u8 pad0[0x34];
    u8 cupPlacements[0x1A];
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

void updateMenuCameraObjectLookAtOriginCallback(void) {
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

void updateMenuCameraObjectWithTargetOffsetCallback(void) {
    updateMenuCameraObjectWithTargetOffset();
}

// initRaceStartTransition best valid match: 99.898% (nonmatchings/initRaceStartTransition-2188069624939011928/base_55.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/effects/race_start_transition/initRaceStartTransition.s")

#ifdef NON_MATCHING
extern void loadMainMenuSceneModelAnimationBank();
extern u8 D_13F3B0[];
extern u8 D_145380[];
extern u8 D_1467B0[];
extern u8 gRaceSetupOpponentFocusCharacterIds[][4];
extern u8 gPendingEndingCreditsFlow;
extern u8 gRaceSplitscreenMode;
extern RaceSetupSaveData gGameSaveDataBuffer;
extern u8 D_800ECA24;
extern u8 gPlayerCount;
extern u8 D_80121D90;

const f32 D_800E1090 = 1.333333373f;
const f32 D_800E1094 = 1.333333373f;
const f32 D_800E1098 = 1.333333373f;

void initRaceStartTransition(void) {
    s32 effectArg;
    s32 allReady;
    s32 transition;

    transition = 0;
    if (gRaceSplitscreenMode == 0) {
        effectArg = gPlayerCount;
        if (((effectArg ^ 0) == 1) != 0) {
            if ((gGameSaveDataBuffer.cupPlacements[0x18] == 2) && (gGameSaveDataBuffer.cupPlacements[6] == 1)) {
                gGameSaveDataBuffer.cupPlacements[0x18] = 3;
                gPendingEndingCreditsFlow = 1;
                transition = 3;
            }
            if ((gGameSaveDataBuffer.cupPlacements[0x18] == 1) && (gGameSaveDataBuffer.cupPlacements[5] == 1)) {
                transition = 2;
                gGameSaveDataBuffer.cupPlacements[0x18] = 2;
            }
            if (gGameSaveDataBuffer.cupPlacements[0x18] == 0) {
                if (gGameSaveDataBuffer.cupPlacements[9] == 1) {
                    allReady = 1;
                    if (D_800ECA24 != 1) {
                        allReady = 0;
                    }
                    if (1) {
                    }
                    if (1) {
                    }
                    if (gGameSaveDataBuffer.cupPlacements[effectArg] != 1) {
                        allReady = 0;
                    }
                    if (gGameSaveDataBuffer.cupPlacements[effectArg + 1] != 1) {
                        allReady = 0;
                    }
                    if (gGameSaveDataBuffer.cupPlacements[effectArg + 2] != 1) {
                        allReady = 0;
                    }
                    if (gGameSaveDataBuffer.cupPlacements[effectArg + 3] != 1) {
                        allReady = 0;
                    }
                    if (allReady != 0) {
                        transition = 1;
                        gGameSaveDataBuffer.cupPlacements[0x18] = 1;
                    }
                }
            }
        }
    }

    if (transition == 0) {
        if (1) {
        }
        resumeGameTask(2);
        removeGameTask(4);
        return;
    }

    loadMainMenuSceneModelAnimationBank();
    LOAD_ASSET(_5DAF30, 0x2A);
    LOAD_ASSET(_1E0F70, 0x22);
    loadRawRomAsset(D_145380, D_1467B0, 8);
    LOAD_ASSET(_1DCED0, 9);
    loadRawRomAsset(D_13F3B0, D_145380, 0xE);
    LOAD_ASSET(LEVEL_ROOKIE_MOUNTAIN_TEXTURES, 0x12);
    initCallbackTaskScheduler(2);
    if (((!gRaceSetupOpponentFocusCharacterIds) && (!gRaceSetupOpponentFocusCharacterIds)) && (!gRaceSetupOpponentFocusCharacterIds)) {
    }
    resetRaceCameras();
    setRaceCameraModeForced(0, 0x1D);
    setRaceCameraModeForced(1, 0x1D);
    setRaceCameraModeForced(2, 0x1D);
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xC0, 0x140, 0xF0, D_800E1090);
    configureViewport(1, 0xA0, 0x78, 0x120, 0xC0, 0x140, 0xF0, D_800E1094);
    configureViewport(2, 0xA0, 0x78, 0x120, 0xC0, 0x140, 0xF0, D_800E1098);
    enableViewportClear(2);
    gMenuFadeAlpha = 0xFF;
    ;
    gCurrentGameTask->fade = 5;
    effectArg = transition - 1;
    createCallbackTaskWithUserId(initRaceSetupBackdrop, 0, 0x64, effectArg);
    createCallbackTask(initMainMenuModeBoardTransition, 0, 0x64);
    createCallbackTaskWithUserId(initMainMenuModeLabelFadeIn, 0, 0x64, effectArg);
    createCallbackTaskWithUserId(initRaceSetupCharacterFocus, 0, 0x64, D_80121D90);
    createRaceSetupOpponentFocus(1, gRaceSetupOpponentFocusCharacterIds[D_80121D90][0]);
    createRaceSetupOpponentFocus(2, gRaceSetupOpponentFocusCharacterIds[D_80121D90][1]);
    createRaceSetupOpponentFocus(3, gRaceSetupOpponentFocusCharacterIds[D_80121D90][2]);
    effectArg = gRaceSetupOpponentFocusCharacterIds[D_80121D90][3];
    createRaceSetupOpponentFocus(4, effectArg);
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
    createCallbackTaskWithUserId(initFallingMenuSnowflake, 5, 0x64, 0);
    updateRaceCamera(0);
    updateRaceCamera(1);
    updateRaceCamera(2);
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
    createCallbackTaskWithUserId(initFallingMenuSnowflake, 5, 0x64, 0);
    updateRaceCamera(0);
    updateRaceCamera(1);
    updateCallbackTasks();
}

void updateRaceStartTransitionFadeOut(void) {
    gMenuFadeAlpha += 4;
    if (gMenuFadeAlpha >= 0xFF) {
        gCurrentGameTask->fade = 0xFF;
        gFramebufferSwapHold = 1;
        setCurrentGameTaskCallback(finishRaceStartTransition, 0);
    }
    createCallbackTaskWithUserId(initFallingMenuSnowflake, 5, 0x64, 0);
    updateRaceCamera(0);
    updateRaceCamera(1);
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
