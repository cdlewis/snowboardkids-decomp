#include "common.h"
#include "game/save_data.h"
#include "game/race/player/race_player_input.h"
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
    FixedTransform transform;
    Vec3i worldPos;
    Vec3i localPos;
} TransformScratch;

extern void releaseMenuAssetHandles(void);
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern s16 gMenuFadeAlpha;

void updateMenuCameraObjectLookAtOrigin(void) {
    TransformScratch scratch;
    RaceCamera *obj;

    obj = gCurrentMenuCameraObject;
    makeFixedRotationYX(obj->rotationMatrix, -obj->pitch, -obj->yaw);
    obj = gCurrentMenuCameraObject;
    scratch.localPos.x = 0;
    scratch.localPos.y = 0;
    scratch.localPos.z = -obj->distance;
    makeFixedRotationXY(scratch.transform.rotation, obj->pitch, obj->yaw);
    transformVec3iByFixedMatrix(scratch.transform.rotation, &scratch.localPos, &scratch.worldPos);
    obj = gCurrentMenuCameraObject;
    obj->transformOffset.x = scratch.worldPos.x - obj->pos.x;
    obj = gCurrentMenuCameraObject;
    obj->transformOffset.y = scratch.worldPos.y - obj->pos.y;
    obj = gCurrentMenuCameraObject;
    obj->transformOffset.z = scratch.worldPos.z - obj->pos.z;
}

void updateMenuCameraObjectLookAtOriginCallback(void) {
    updateMenuCameraObjectLookAtOrigin();
}

void updateMenuCameraObjectWithTargetOffset(void) {
    TransformScratch scratch;
    RaceCamera *obj;

    obj = gCurrentMenuCameraObject;
    makeFixedRotationYX(obj->rotationMatrix, -obj->pitch, -obj->yaw);
    obj = gCurrentMenuCameraObject;
    scratch.localPos.x = gMenuCameraTargetOffset.x;
    scratch.localPos.y = gMenuCameraTargetOffset.y;
    scratch.localPos.z = -obj->distance;
    makeFixedRotationXY(scratch.transform.rotation, obj->pitch, obj->yaw);
    transformVec3iByFixedMatrix(scratch.transform.rotation, &scratch.localPos, &scratch.worldPos);
    obj = gCurrentMenuCameraObject;
    obj->transformOffset.x = scratch.worldPos.x - obj->pos.x;
    obj = gCurrentMenuCameraObject;
    obj->transformOffset.y = scratch.worldPos.y - obj->pos.y;
    obj = gCurrentMenuCameraObject;
    obj->transformOffset.z = scratch.worldPos.z - obj->pos.z;
}

void updateMenuCameraObjectWithTargetOffsetCallback(void) {
    updateMenuCameraObjectWithTargetOffset();
}

// initRaceStartTransition best valid match: 99.898% (nonmatchings/initRaceStartTransition-2188069624939011928/base_55.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/effects/race_start_transition/initRaceStartTransition.s")

#ifdef NON_MATCHING
extern void loadMainMenuSceneModelAnimationBank(void);
extern u8 gRaceSetupOpponentFocusCharacterIds[][4];
extern u8 gPendingEndingCreditsFlow;
extern u8 gRaceSplitscreenMode;
extern u8 D_800ECA24;
extern u8 gPlayerCount;

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
    LOAD_RAW_ASSET(_145380, 8);
    LOAD_ASSET(_1DCED0, 9);
    LOAD_RAW_ASSET(LEVEL_ROOKIE_MOUNTAIN, 0xE);
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
    createCallbackTaskWithUserId((CallbackTaskCallback)initRaceSetupBackdrop, 0, 0x64, effectArg);
    createCallbackTask((CallbackTaskCallback)initMainMenuModeBoardTransition, 0, 0x64);
    createCallbackTaskWithUserId((CallbackTaskCallback)initMainMenuModeLabelFadeIn, 0, 0x64, effectArg);
    createCallbackTaskWithUserId((CallbackTaskCallback)initRaceSetupCharacterFocus, 0, 0x64, gRacePlayers[0].characterId);
    createRaceSetupOpponentFocus(1, gRaceSetupOpponentFocusCharacterIds[gRacePlayers[0].characterId][0]);
    createRaceSetupOpponentFocus(2, gRaceSetupOpponentFocusCharacterIds[gRacePlayers[0].characterId][1]);
    createRaceSetupOpponentFocus(3, gRaceSetupOpponentFocusCharacterIds[gRacePlayers[0].characterId][2]);
    effectArg = gRaceSetupOpponentFocusCharacterIds[gRacePlayers[0].characterId][3];
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
    createCallbackTaskWithUserId((CallbackTaskCallback)initFallingMenuSnowflake, 5, 0x64, 0);
    updateRaceCamera(0);
    updateRaceCamera(1);
    updateRaceCamera(2);
    updateCallbackTasks();
}

void updateRaceStartTransitionFadeIn(void) {
    GameTask **state;
    GameTask *currentState;

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
    createCallbackTaskWithUserId((CallbackTaskCallback)initFallingMenuSnowflake, 5, 0x64, 0);
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
    createCallbackTaskWithUserId((CallbackTaskCallback)initFallingMenuSnowflake, 5, 0x64, 0);
    updateRaceCamera(0);
    updateRaceCamera(1);
    updateCallbackTasks();
}

void finishRaceStartTransition(void) {
    if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay.value = 0;
        resumeGameTask(2);
        removeGameTask(4);
    }
}
