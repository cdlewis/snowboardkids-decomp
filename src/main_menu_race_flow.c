#include "common.h"
#include "sound_manager.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "race_scene_setup.h"
#include "race_player_input.h"
#include "game_task_scheduler.h"
#include "training_course_ui.h"
#include "menu_screen_effects.h"
#include "main_menu_panel_ui.h"
#include "race_camera.h"
#include "main_menu_race_flow.h"
#include "race_player_update.h"
#include "race_hud.h"
#include "viewport_manager.h"

#define MAIN_MENU_MODE_PREVIEW_RACE_VIEWPORT_ZOOM_FRAMES 0x10

typedef struct {
    /* 0x00 */ char pad[0x18];
    /* 0x18 */ s32 transitionTimer;
    /* 0x1C */ s32 unk1C;
} MainMenuRaceFlowTask;

typedef struct {
    /* 0x0 */ u8 courseIndex;
    /* 0x1 */ u8 pad1[3];
    /* 0x4 */ u8 *romStart;
    /* 0x8 */ u8 *romEnd;
} MainMenuModePreviewRaceCourseAsset;


extern u8 D_1467B0[];
extern u8 D_147910[];
extern u8 D_1DE360[];
extern u8 D_1E0F70[];
extern u8 D_1F1A90[];
extern u8 D_1F2220[];
extern u8 D_245A80[];
extern u8 D_24C8E0[];
extern MainMenuRaceFlowTask *gCurrentGameTask;
extern u8 gMainMenuSelectionResult;
extern s8 gRacePlayerHudStatuses;
extern s16 gRaceCourseIndex;
extern s16 gRaceLapCount;
extern s8 gRacePlayerCount;
extern char gPlayerCount;
extern u8 gRaceUpdatePaused;
extern s16 gMenuFadeAlpha;
extern u8 gFramebufferSwapHold;
extern s32 gMenuFlowState;
extern s8 gRaceCameraModeChangeDisabled;
extern s8 gRaceDemoPlaybackEnabled;
extern u8 gMainMenuModeSelection;
extern s16 gRacePlayerAttackStartTimer;
extern s8 gRaceTypeSelection;
extern s8 gRaceResultState;
extern s16 gMainMenuModePreviewRaceDurationBySelection[];

extern void releaseMenuAssetHandles(void);
extern u8 gPendingFramebufferSwapCount;
extern s8 gFramebufferSwapDelay;
extern u8 gTrainingCourseLesson;
extern s8 gRaceRumbleEnabled;
extern s8 gRaceSplitscreenMode;
extern u8 D_80121D94;
extern u8 D_80121D95;
extern u8 D_80121D96;
extern u8 D_801223A0;
extern u8 D_801223A1;
extern u8 D_801223A2;
extern u8 D_801229AC;
extern u8 D_801229AD;
extern u8 D_801229AE;
extern u8 D_80122FB8;
extern u8 D_80122FB9;
extern u8 D_80122FBA;
extern MainMenuModePreviewRaceCourseAsset gMainMenuModePreviewRaceCourseAssets[];
extern f32 gMainMenuModePreviewRaceAspectRatio;
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_60F1A0[];
extern u8 D_60F990[];
extern s16 gRacePlayerSurfaceAngleByPlayer;

void startMainMenuModePreviewRaceFlow(void) {
    gMainMenuModeSelection = 1;
    setCurrentGameTaskCallback(initMainMenuModePreviewRaceSelectionMenu, 0);
}

void initMainMenuModePreviewRaceSelectionMenu(void) {
    loadCompressedRomAsset(D_1F1A90, D_1F2220, 0x28);
    loadCompressedRomAsset(D_593D10, D_598A70, 0x29);
    loadCompressedRomAsset(D_60F1A0, D_60F990, 0x2A);
    loadCompressedRomAsset(D_245A80, D_24C8E0, 0x1F);
    loadRawRomAsset(D_1467B0, D_147910, 8);
    loadCompressedRomAsset(D_1DE360, D_1E0F70, 9);
    gMainMenuSelectionResult = 0;
    gFramebufferSwapDelay = 0;
    gMenuFadeAlpha = 0xFF;
    resetAllViewports();
    gViewportStates[0].overlayAlpha = 0x80;
    initCallbackTaskScheduler(0);
    configureMenuViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    resetRaceCameras();
    setRaceCameraMode(0, 0x1F);
    createCallbackTaskWithUserId(&initMainMenuBoardModels, 0, 0x64, 0);
    createCallbackTask(&initMainMenuModeSelectGrid, 0, 0x64);
    setCurrentGameTaskCallback(&fadeInMainMenuModePreviewRaceSelectionMenu, 0);
    requestMusicSequenceBank(7);
}

void fadeInMainMenuModePreviewRaceSelectionMenu(void) {
    gMenuFadeAlpha -= 0x10;
    if (gMenuFadeAlpha <= 0) {
        gMenuFadeAlpha = 0;
        setCurrentGameTaskCallback(&updateMainMenuModePreviewRaceSelectionMenu, 0);
    }
    updateRaceCamera(0);
    updateCallbackTasks();
}

void updateMainMenuModePreviewRaceSelectionMenu(void) {
    if (gMainMenuSelectionResult != 0) {
        if (gMainMenuModeSelection == 0xC) {
            requestMusicSequenceStop(0x3C);
        }
        setCurrentGameTaskCallback(&fadeOutMainMenuModePreviewRaceSelectionMenu, 0);
    }
    updateRaceCamera(0);
    updateCallbackTasks();
}

void fadeOutMainMenuModePreviewRaceSelectionMenu(void) {
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0xFF) {
        gMenuFadeAlpha = 0xFF;
        gFramebufferSwapHold = 1;
        setCurrentGameTaskCallback(&exitMainMenuModePreviewRaceSelectionMenu, 0);
    }
    updateRaceCamera(0);
    updateCallbackTasks();
}

void exitMainMenuModePreviewRaceSelectionMenu(void) {
    if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        if (gMainMenuModeSelection != 0xC) {
            setCurrentGameTaskCallback(&initMainMenuModePreviewRace, 0);
        } else {
            gMenuFlowState = 0;
            resumeGameTask(3);
            removeGameTask(4);
        }
    }
}

// initMainMenuModePreviewRace best match: 97.462% (nonmatchings/initMainMenuModePreviewRace-2870645799593382959/base_10.c)

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_race_flow/initMainMenuModePreviewRace.s")

#ifdef NON_MATCHING
void initMainMenuModePreviewRace(void) {
    RaceInputPlayer *players;
    MainMenuModePreviewRaceCourseAsset *courseAsset;
    s32 five0;
    s32 five1;
    s32 five2;
    s32 five3;

    gRaceCourseIndex = (((gMainMenuModePreviewRaceCourseAssets[gMainMenuModeSelection].courseIndex & 0xFF) & 0xFF) & 0xFF) & 0xFF;
    gRaceUpdatePaused = 0;
    gRaceCameraModeChangeDisabled = 0;
    gRaceResultState = 0;
    gRaceDemoPlaybackEnabled = 0;
    gTrainingCourseLesson = 0;
    gRaceSplitscreenMode = 0;
    gRaceTypeSelection = 0;
    resetGameplayRng();

    players = D_80121D80;
    five0 = 5;
    five1 = 5;
    five2 = 5;
    five3 = 5;
    players[0].unk4 = 0;
    players[1].unk4 = 0;
    players[2].unk4 = 0;
    players[3].unk4 = 0;
    players[0].characterId = 0;
    players[1].characterId = 1;
    players[2].characterId = 2;
    players[3].characterId = 3;
    players[0].unk11 = 0;
    players[1].unk11 = 0;
    players[2].unk11 = 0;
    players[3].unk11 = 0;
    players[0].unk12 = 3;
    players[1].unk12 = 5;
    players[2].unk12 = 6;
    players[3].unk12 = 7;
    players[0].unk17 = 0;
    players[1].unk17 = 1;
    players[2].unk17 = 2;
    players[3].unk17 = 3;
    gPlayerCount = 4;
    players[0].isActive = 1;
    players[1].isActive = 1;
    players[2].isActive = 1;
    players[3].isActive = 1;
    gRacePlayerCount = 4;
    gRaceLapCount = 5;
    gRacePlayerAttackStartTimer = 0x64;
    initCallbackTaskScheduler(1);

    D_80121D95 = 0;
    D_80121D94 = 0;
    D_80121D96 = five0;
    D_801223A1 = 0;
    D_801223A0 = 0;
    D_801223A2 = five1;
    D_801229AD = 0;
    D_801229AC = 0;
    D_801229AE = five2;
    D_80122FB9 = 0;
    D_80122FB8 = 0;
    D_80122FBA = five3;

    courseAsset = &gMainMenuModePreviewRaceCourseAssets[gMainMenuModeSelection];
    loadCompressedRomAsset(courseAsset->romStart, courseAsset->romEnd, 0x2B);
    loadRaceCourseAssets();
    loadRaceCharacterAssets();
    resetRaceCameras();
    resetAllViewports();
    gFramebufferSwapDelay = 0;
    initRacePlayers();
    gPlayerCount = 1;
    initRaceHud();
    gPlayerCount = 4;
    configureViewport(0, 0xA0, 0x50, 0x108, 0x78, 0x140, 0x8C, gMainMenuModePreviewRaceAspectRatio);
    gRacePlayerHudStatuses = 1;
    gFramebufferSwapDelay = 0;
    initRaceCourseSceneTasks();
    gMenuFadeAlpha = 0xFF;
    gRaceRumbleEnabled = 0;
    gMenuFlowState = 1;
    loadCompressedRomAsset(D_593D10, D_598A70, 0x29);
    loadCompressedRomAsset(D_60F1A0, D_60F990, 0x2A);
    gMainMenuSelectionResult = 0;
    createCallbackTask(initMainMenuModeDescriptionPanel, 0, 0x64);
    createCallbackTask(initRaceSetupCornerPrompts, 0, 0x64);
    setCurrentGameTaskCallback(fadeInMainMenuModePreviewRace, 0);
    requestMusicSequenceBank(7);
}
#endif

void fadeInMainMenuModePreviewRace(void) {
    gMenuFadeAlpha -= 0x10;
    if (gMenuFadeAlpha < 0) {
        gMenuFadeAlpha = 0;
        setCurrentGameTaskCallback(&waitForMainMenuModePreviewRaceStart, 0);
    }
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
    gViewportStates[0].overlayAlpha = 0x80;
}

void waitForMainMenuModePreviewRaceStart(void) {
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
    if (gMainMenuSelectionResult != 0) {
        gCurrentGameTask->transitionTimer = 0;
        requestMusicSequenceStop(0x20);
        setCurrentGameTaskCallback(&zoomMainMenuModePreviewRaceViewport, 0);
    }
    gViewportStates[0].overlayAlpha = 0x80;
}

// zoomMainMenuModePreviewRaceViewport best match: 85.661% (nonmatchings/zoomMainMenuModePreviewRaceViewport-5802343343535905907/base_13.c)

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_race_flow/zoomMainMenuModePreviewRaceViewport.s")

#ifdef NON_MATCHING
void zoomMainMenuModePreviewRaceViewport(void) {
    s32 timer;

    gCurrentGameTask->transitionTimer += 1;
    timer = gCurrentGameTask->transitionTimer;

    configureViewport(0, 0xA0, (s32) ((s16) ((((s32) (timer * 0x28)) / 16) + 0x50)),
                      (s16) ((((s32) (timer * 0x18)) / 16) + 0x108),
                      (u16) ((s32) ((s16) ((((s32) (timer * 0x58)) / 16) + 0x78))), 0x140U,
                      (u16) ((s32) ((s16) ((((s32) (timer * 0x64)) / 16) + 0x8C))),
                      (f32) (((((f64) timer) * (-0.9523809523809523)) / 0x10) + 2.2857142857142856));
    if (gCurrentGameTask->transitionTimer == MAIN_MENU_MODE_PREVIEW_RACE_VIEWPORT_ZOOM_FRAMES) {
        requestMusicSequenceBank(0xF);
        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        gCurrentGameTask->transitionTimer = 0;
        setCurrentGameTaskCallback(runMainMenuModePreviewRace, 0);
    }
    if (timer && timer) {}
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
}
#endif

void runMainMenuModePreviewRace(void) {
    gMenuFlowState = 0;
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
    updateRaceHud();
    gCurrentGameTask->transitionTimer += 1;
    if (gCurrentGameTask->transitionTimer == gMainMenuModePreviewRaceDurationBySelection[gMainMenuModeSelection]) {
        setCurrentGameTaskCallback(fadeOutMainMenuModePreviewRace, 0);
        requestMusicSequenceStop(0x40);
    }
}

void fadeOutMainMenuModePreviewRace(void) {
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
    updateRaceHud();
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0xFF) {
        gFramebufferSwapHold = 1;
        {
            void (*func_ptr)(void) = returnToMainMenuModePreviewRaceSelectionMenu;
            setCurrentGameTaskCallback(func_ptr, 0);
        }
    }
}

void returnToMainMenuModePreviewRaceSelectionMenu(void) {
    if (gPendingFramebufferSwapCount == 2) {
        stopSoundEffects();
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        setCurrentGameTaskCallback(&initMainMenuModePreviewRaceSelectionMenu, 0);
    }
}

void startTrainingCourseFlow(void) {
    gTrainingCourseLesson = 1;
    setCurrentGameTaskCallback(&queueTrainingCourseRaceInit, 0);
}

void queueTrainingCourseRaceInit(void) {
    setCurrentGameTaskCallback(&initTrainingCourseRace, 0);
}

// initTrainingCourseRace best match: 98.186% (nonmatchings/func_8004002C-6061209858023118177/base.c)

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_race_flow/initTrainingCourseRace.s")

#ifdef NON_MATCHING
void initTrainingCourseRace(void) {
    u64 characterId;

    switch (gTrainingCourseLesson) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            gRaceCourseIndex = 0;
            break;
        default:
            gRaceCourseIndex = 9;
            break;
    }

    gRaceUpdatePaused = 0;
    gRaceCameraModeChangeDisabled = 0;
    gRaceResultState = 0;
    gRaceDemoPlaybackEnabled = 0;
    gMainMenuModeSelection = 0;
    gRaceSplitscreenMode = 0;
    gRaceTypeSelection = 0;

    D_80121D80[0].unk4 = 0;
    D_80121D80[1].unk4 = 1;
    D_80121D80[2].unk4 = 1;
    D_80121D80[3].unk4 = 1;
    D_80121D80[0].characterId = (D_80121D80[0].unk11 = 0);
    D_80121D80[1].characterId = 1;
    characterId = 2;
    D_80121D80[2].characterId = characterId;
    D_80121D80[3].characterId = 3;
    D_80121D80[1].unk11 = 0;
    D_80121D80[2].unk11 = 0;
    D_80121D80[3].unk11 = 0;
    D_80121D80[0].unk12 = 3;
    D_80121D80[0].isActive = 1;
    D_80121D80[1].unk12 = 5;
    D_80121D80[2].unk12 = 6;
    if (!gTrainingCourseLesson) {
    }
    D_80121D80[3].unk12 = 7;
    D_80121D80[0].unk17 = 0;
    D_80121D80[1].unk17 = 1;
    D_80121D80[2].unk17 = 2;
    D_80121D80[3].unk17 = 3;
    gPlayerCount = 1;

    switch (gTrainingCourseLesson) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 9:
            D_80121D80[0].isActive = 1;
            D_80121D80[1].isActive = 0;
            D_80121D80[2].isActive = 0;
            D_80121D80[3].isActive = 0;
            gRacePlayerCount = 1;
            break;
        default:
            D_80121D80[0].isActive = 1;
            D_80121D80[1].isActive = 1;
            D_80121D80[2].isActive = 1;
            D_80121D80[3].isActive = 1;
            gRacePlayerCount = 4;
            break;
    }

    gRaceLapCount = 5;
    gRacePlayerAttackStartTimer = 0x64;
    initCallbackTaskScheduler(1);
    D_80121D95 = 0;
    D_80121D96 = 0;
    D_80121D94 = 0;
    D_801223A1 = 0;
    D_801223A2 = 0;
    D_801223A0 = 0;
    D_801229AD = 0;
    D_801229AE = 0;
    D_801229AC = 0;
    D_80122FB9 = 0;
    D_80122FBA = 0;
    D_80122FB8 = 0;
    loadRaceCourseAssets();
    loadRaceCharacterAssets();
    resetRaceCameras();
    resetAllViewports();
    gRacePlayerHudStatuses = 1;
    gFramebufferSwapDelay = 0;
    initRacePlayers();
    initRaceHud();
    configureViewport(0, 0xA0, 0x50, 0x108, 0x78, 0x140, 0x8C, 2.285714388f);
    gRacePlayerHudStatuses = 1;
    gFramebufferSwapDelay = 0;
    initRaceCourseSceneTasks();
    gMenuFadeAlpha = 0xFF;
    gRaceRumbleEnabled = 0;
    gMenuFlowState = 1;
    loadCompressedRomAsset(D_593D10, D_598A70, 0x29);
    loadCompressedRomAsset(D_60F1A0, D_60F990, 0x2A);
    gMainMenuSelectionResult = 0;
    createCallbackTask(initTrainingCourseOpeningDialog, 0, 0x64);
    createCallbackTask(initTrainingCourseLessonTitlePanel, 0, 0x63);
    createCallbackTask(initRaceSetupCornerPrompts, 0, 0x64);
    setCurrentGameTaskCallback(fadeInTrainingCourseRace, 0);
    requestMusicSequenceBank(7);
}
#endif

void fadeInTrainingCourseRace(void) {
    gMenuFadeAlpha -= 0x10;
    if (gMenuFadeAlpha < 0) {
        gMenuFadeAlpha = 0;
        setCurrentGameTaskCallback(waitForTrainingCourseStartSelection, 0);
    }
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
    gViewportStates[0].overlayAlpha = 0x80;
}

void waitForTrainingCourseStartSelection(void) {
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
    if (gMainMenuSelectionResult != 0) {
        gCurrentGameTask->transitionTimer = 0;
        requestMusicSequenceStop(0x20);
        setCurrentGameTaskCallback(&zoomTrainingCourseRaceViewport, 0);
    }
    gViewportStates[0].overlayAlpha = 0x80;
}

// zoomTrainingCourseRaceViewport best match: 92.813% (nonmatchings/zoomTrainingCourseRaceViewport-3242520251544044307/base_10.c)

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_race_flow/zoomTrainingCourseRaceViewport.s")

#ifdef NON_MATCHING
void zoomTrainingCourseRaceViewport(void) {
    volatile u32 new_var;
    s32 temp_v0;

    gCurrentGameTask->transitionTimer += 1;
    temp_v0 = gCurrentGameTask->transitionTimer;
    configureViewport(((s16) ((((s32) (temp_v0 * 0x18)) / 16) - -0x108)) * 0, 0xA0,
                      (s32) ((s16) ((((s32) (temp_v0 * 0x28)) / 16) + 0x50)),
                      (s16) ((((s32) (temp_v0 * 0x18)) / 16) + 0x108),
                      (u16) ((s32) (new_var = (s16) ((((s32) (temp_v0 * 0x58)) / 16) + 0x78))), 0x140U,
                      (u16) ((s32) ((s16) ((((s32) (temp_v0 * 0x64)) / 16) + 0x8C))),
                      (f32) (((((f64) temp_v0) * (-0.9523809523809523)) / 0x10) + 2.2857142857142856));
    if (gCurrentGameTask->transitionTimer == 0x10) {
        requestMusicSequenceBank(0xF);
        configureViewport(0, 0xA0, 0x78, 0x120U, 0xD0U, 0x140U, 0xF0U, 1.3333334f);
        setCurrentGameTaskCallback(runTrainingCourseUntilLessonEnd, 0);
    }
    if (temp_v0 && temp_v0) {}
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
}
#endif

void runTrainingCourseUntilLessonEnd(void) {
    gMenuFlowState = 0;
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
    updateRaceHud();
    gCurrentGameTask->transitionTimer = 0;
    switch (gTrainingCourseLesson) {
        case 1:
            if (gRacePlayerSurfaceAngleByPlayer == 0x50) {
                setCurrentGameTaskCallback(fadeInTrainingCourseLessonEndMenu, 0);
                requestMusicSequenceStop(0x40);
                return;
            }
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            if (gRacePlayerSurfaceAngleByPlayer == 0x9C) {
                setCurrentGameTaskCallback(fadeInTrainingCourseLessonEndMenu, 0);
                requestMusicSequenceStop(0x40);
                return;
            }
            break;
        case 6:
            if (gRacePlayerSurfaceAngleByPlayer == 0xB4) {
                setCurrentGameTaskCallback(fadeInTrainingCourseLessonEndMenu, 0);
                requestMusicSequenceStop(0x40);
                return;
            }
            break;
        case 7:
        case 8:
            if (gRacePlayerSurfaceAngleByPlayer == 0x16) {
                setCurrentGameTaskCallback(fadeInTrainingCourseLessonEndMenu, 0);
                requestMusicSequenceStop(0x40);
                return;
            }
            break;
        case 9:
            if (gRacePlayerSurfaceAngleByPlayer == 0x36) {
                setCurrentGameTaskCallback(fadeInTrainingCourseLessonEndMenu, 0);
                requestMusicSequenceStop(0x40);
            }
            break;
    }
}

void fadeInTrainingCourseLessonEndMenu(void) {
    s32 temp_v1;

    gRaceUpdatePaused = 1;
    gMainMenuSelectionResult = 0;
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
    updateRaceHud();
    gCurrentGameTask->transitionTimer += 0x10;
    temp_v1 = gCurrentGameTask->transitionTimer;
    if (temp_v1 == 0x80) {
        createCallbackTask(initTrainingCourseLessonEndMenu, 0, 0x64);
        setCurrentGameTaskCallback(waitForTrainingCourseLessonEndMenuSelection, 0);
    }
    temp_v1 = gCurrentGameTask->transitionTimer;
    gViewportStates[0].overlayAlpha = temp_v1;
}

void waitForTrainingCourseLessonEndMenuSelection(void) {
    gRaceUpdatePaused = 1;
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
    updateRaceHud();
    gViewportStates[0].overlayAlpha = 0x80;
    if (gMainMenuSelectionResult != 0) {
        setCurrentGameTaskCallback(&fadeOutTrainingCourseLessonEndMenu, 0);
    }
}

void fadeOutTrainingCourseLessonEndMenu(void) {
    gRaceUpdatePaused = 1;
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
    updateRaceHud();
    gViewportStates[0].overlayAlpha = 0x80;
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0xFF) {
        gFramebufferSwapHold = 1;
        setCurrentGameTaskCallback(&handleTrainingCourseLessonEndMenuSelection, 0);
    }
}

void handleTrainingCourseLessonEndMenuSelection(void) {
    if (gPendingFramebufferSwapCount == 2) {
        stopSoundEffects();
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        if (gMainMenuSelectionResult == 1) {
            if (gTrainingCourseLesson < 9) {
                gTrainingCourseLesson += 1;
            }
            setCurrentGameTaskCallback(queueTrainingCourseRaceInit, 0);
        }
        if (gMainMenuSelectionResult == 2) {
            setCurrentGameTaskCallback(initTrainingCourseRace, 0);
        }
        if (gMainMenuSelectionResult == 3) {
            setCurrentGameTaskCallback(finishTrainingCourse, 0);
        }
    }
}

void finishTrainingCourse(void) {
    if (gTrainingCourseLesson != 9) {
        setCurrentGameTaskCallback(returnToMainMenuFromTrainingCourse, 0);
        return;
    }
    loadCompressedRomAsset(D_593D10, D_598A70, 0x29);
    loadCompressedRomAsset(D_60F1A0, D_60F990, 0x2A);
    gMainMenuSelectionResult = 0;
    gFramebufferSwapDelay = 0;
    gMenuFadeAlpha = 0xFF;
    resetAllViewports();
    initCallbackTaskScheduler(0);
    createCallbackTask(&initTrainingCourseEndingDialog, 0, 0x64);
    setCurrentGameTaskCallback(fadeInTrainingCourseEndingDialog, 0);
}

void fadeInTrainingCourseEndingDialog(void) {
    gMenuFadeAlpha -= 0x10;
    if (gMenuFadeAlpha <= 0) {
        gMenuFadeAlpha = 0;
        setCurrentGameTaskCallback(&waitForTrainingCourseEndingDialog, 0);
    }
    updateCallbackTasks();
}

void waitForTrainingCourseEndingDialog(void) {
    if (gMainMenuSelectionResult != 0) {
        setCurrentGameTaskCallback(&fadeOutTrainingCourseEndingDialog, 0);
    }
    updateCallbackTasks();
}

void fadeOutTrainingCourseEndingDialog(void) {
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0xFF) {
        gMenuFadeAlpha = 0xFF;
        gFramebufferSwapHold = 1;
        setCurrentGameTaskCallback(&exitTrainingCourseEndingDialog, 0);
    }
    updateCallbackTasks();
}

void exitTrainingCourseEndingDialog(void) {
    if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        setCurrentGameTaskCallback(&returnToMainMenuFromTrainingCourse, 0);
    }
}

void returnToMainMenuFromTrainingCourse(void) {
    gMenuFlowState = 0;
    resumeGameTask(3);
    removeGameTask(4);
}
