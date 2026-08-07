#include "game/race/race_state.h"
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

RaceCourseStartEntry gRaceCourseStartEntries[10] = {
    { 175,
     2, 181,
     { 0, 0 },
     { -110094620, 14417050, -49531060 },
     3632, 1606,
     -1014756656,
     -786436320,
     { -1020269172, -1399455744, -776497348 },
     { -1014623764, -1398308866, -787512540 },
     176,  181,
     102, 181,
     -1015500179,
     -785475956               },
    { 148,
     3, 159,
     { 0, 0 },
     { -124813296, 28559108, 146260502 },
     3952, 3808,
     622943634,   -1530568468,
     { 625979702, -1180041216, -1541519578 },
     { 624572818, -1177194498, -1527416340 },
     149,  159,
     66,  159,
     623554950,   -1532425130 },
    { 145,
     3, 151,
     { 0, 0 },
     { -79949260, 23463182, 43468674 },
     0,    1168,
     390797047,   -917897194,
     { 380296966, -799319726, -913549075 },
     { 392436219, -797597759, -921224574 },
     146,  151,
     69,  196,
     388615591,   -917596570  },
    { 196,
     3, 201,
     { 0, 0 },
     { 87951334, 8865843, -97064827 },
     1808, 976,
     -1262465465,
     -1690699353,
     { -1273777141, -799801344, -1689590096 },
     { -1262216261, -798932992, -1689840375 },
     197,  201,
     128, 292,
     -1265561466,
     -1691386942              },
    { 194,
     3, 200,
     { 0, 0 },
     { -81454671, 26109472, 50923654 },
     4064, 512,
     35999236,    -1547555039,
     { 26709772, -765198336, -1554100760 },
     { 36154208, -761929728, -1545997475 },
     195,  200,
     105, 211,
     34743227,    -1549165737 },
    { 217,
     3, 223,
     { 0, 0 },
     { -56350907, 23705306, -149721464 },
     3008, 1680,
     1195482808,  70087953,
     { 1191131769, -669777920, 80588566 },
     { 1194819864, -667533312, 67911759 },
     218,  223,
     86,  327,
     1194308072,  72588238    },
    { 246,
     3, 252,
     { 0, 0 },
     { -166391126, 73188, -113559623 },
     3264, 2272,
     -1625777969,
     -424230854,
     { -1620177912, -580911104, -414339147 },
     { -1624314406, -578772992, -428294521 },
     247,  252,
     120, 252,
     -1625002876,
     -422023514               },
    { 39,
     1, 999,
     { 0, 0 },
     { 0, 0, 0 },
     0,    0,
     2147483647,  2147483647,
     { 0, 0, 0 },
     { 0, 0, 0 },
     998,  999,
     15,  39,
     0,           0           },
    { 61,
     9, 67,
     { 0, 0 },
     { -80812860, 20932701, 56136634 },
     3968, 1408,
     444079468,   -385020640,
     { 435847815, -218497024, -377184654 },
     { 447545338, -216907776, -385103110 },
     62,   67,
     30,  67,
     442243931,   -384247370  },
    { 66,
     5, 0,
     { 0, 0 },
     { 0, -131625, 241382 },
     0,    2768,
     338630036,   -204627638,
     { 349506429, -288850792, -201324338 },
     { 335783548, -286928629, -203207256 },
     9998, 9999,
     33,  66,
     340434403,   -203448204  },
};

s16 gSineTable[0x1000] = {
#include "race_start_transition_sine_table.inc.c"
};

u8 gRaceSetupOpponentFocusCharacterIds[5][4] = {
    { 1, 2, 3, 4 },
    { 0, 2, 3, 4 },
    { 0, 1, 3, 4 },
    { 0, 2, 1, 4 },
    { 1, 2, 3, 0 },
};

u32 D_800BB824[3] = { 0, 0, 0 };

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

// initRaceStartTransition best valid match: 99.898%
// (nonmatchings/initRaceStartTransition-2188069624939011928/base_55.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/effects/race_start_transition/initRaceStartTransition.s")

#ifdef NON_MATCHING
extern void loadMainMenuSceneModelAnimationBank(void);
extern u8 gPendingEndingCreditsFlow;

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
            if ((gGameSaveDataBuffer[0].cupPlacements[0x18] == 2) && (gGameSaveDataBuffer[0].cupPlacements[6] == 1)) {
                gGameSaveDataBuffer[0].cupPlacements[0x18] = 3;
                gPendingEndingCreditsFlow = 1;
                transition = 3;
            }
            if ((gGameSaveDataBuffer[0].cupPlacements[0x18] == 1) && (gGameSaveDataBuffer[0].cupPlacements[5] == 1)) {
                transition = 2;
                gGameSaveDataBuffer[0].cupPlacements[0x18] = 2;
            }
            if (gGameSaveDataBuffer[0].cupPlacements[0x18] == 0) {
                if (gGameSaveDataBuffer[0].cupPlacements[9] == 1) {
                    allReady = 1;
                    if (gGameSaveDataBuffer[0].cupPlacements[0] != 1) {
                        allReady = 0;
                    }
                    if (1) {}
                    if (1) {}
                    if (gGameSaveDataBuffer[0].cupPlacements[effectArg] != 1) {
                        allReady = 0;
                    }
                    if (gGameSaveDataBuffer[0].cupPlacements[effectArg + 1] != 1) {
                        allReady = 0;
                    }
                    if (gGameSaveDataBuffer[0].cupPlacements[effectArg + 2] != 1) {
                        allReady = 0;
                    }
                    if (gGameSaveDataBuffer[0].cupPlacements[effectArg + 3] != 1) {
                        allReady = 0;
                    }
                    if (allReady != 0) {
                        transition = 1;
                        gGameSaveDataBuffer[0].cupPlacements[0x18] = 1;
                    }
                }
            }
        }
    }

    if (transition == 0) {
        if (1) {}
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
    if (((!gRaceSetupOpponentFocusCharacterIds) && (!gRaceSetupOpponentFocusCharacterIds)) &&
        (!gRaceSetupOpponentFocusCharacterIds)) {}
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
    createCallbackTaskWithUserId(
        (CallbackTaskCallback)initRaceSetupCharacterFocus,
        0,
        0x64,
        gRacePlayers[0].characterId
    );
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
