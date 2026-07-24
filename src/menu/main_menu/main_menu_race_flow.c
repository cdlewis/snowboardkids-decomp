#include "common.h"
#include "assets.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/race/scene/race_scene_setup.h"
#include "game/race/player/race_player_input.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/training/training_course_ui.h"
#include "game/menu/renderer/menu_screen_effects.h"
#include "game/menu/main_menu/main_menu_panel_ui.h"
#include "game/race/camera/race_camera.h"
#include "game/menu/main_menu/main_menu_race_flow.h"
#include "game/race/player/race_player_update.h"
#include "game/race/ui/race_hud.h"
#include "game/engine/viewport_manager.h"

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
extern s8 D_80121D94;
extern s8 D_80121D95;
extern s8 D_80121D96;
extern s8 D_801223A0;
extern s8 D_801223A1;
extern s8 D_801223A2;
extern s8 D_801229AC;
extern s8 D_801229AD;
extern s8 D_801229AE;
extern s8 D_80122FB8;
extern s8 D_80122FB9;
extern s8 D_80122FBA;

extern void releaseMenuAssetHandles(void);
extern u8 gPendingFramebufferSwapCount;
extern s8 gFramebufferSwapDelay;
extern u8 gTrainingCourseLesson;
extern s8 gRaceRumbleEnabled;
extern s8 gRaceSplitscreenMode;
extern MainMenuModePreviewRaceCourseAsset gMainMenuModePreviewRaceCourseAssets[];
extern s16 gRacePlayerSurfaceAngleByPlayer;

void startMainMenuModePreviewRaceFlow(void) {
    gMainMenuModeSelection = 1;
    setCurrentGameTaskCallback(initMainMenuModePreviewRaceSelectionMenu, 0);
}

void initMainMenuModePreviewRaceSelectionMenu(void) {
    LOAD_ASSET(_1F1A90, 0x28);
    LOAD_ASSET(_593D10, 0x29);
    LOAD_ASSET(_60F1A0, 0x2A);
    LOAD_ASSET(_245A80, 0x1F);
    LOAD_RAW_ASSET(_1467B0, 8);
    LOAD_ASSET(_1DE360, 9);
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

void initMainMenuModePreviewRace(void) {
    MainMenuModePreviewRaceCourseAsset *temp_v0;
    s32 one0;
    s32 one1;
    s32 two0;
    s32 five0;
    unsigned long five1;
    s32 six0;
    s32 seven0;
    s32 two1;
    long long two2;
    long long three0;
    s32 seven1;
    RacePlayer *player;

    gRaceCourseIndex = (s16) gMainMenuModePreviewRaceCourseAssets[gMainMenuModeSelection].courseIndex;
    gRaceUpdatePaused = 0;
    gRaceCameraModeChangeDisabled = 0;
    gRaceResultState = 0;
    gRaceDemoPlaybackEnabled = 0;
    gTrainingCourseLesson = 0;
    gRaceSplitscreenMode = 0;
    gRaceTypeSelection = 0;
    resetGameplayRng();

    one0 = 1;
    two2 = 1;
    one1 = two2;
    two0 = 2;
    five0 = 5;
    six0 = 6;
    seven0 = 7;
    two1 = 2;
    two2 = two1;
    three0 = 3;
    seven1 = 7;
    gRacePlayers->unk4 = 0;
    gRacePlayers[1].unk4 = 0;
    gRacePlayers[2].unk4 = 0;
    gRacePlayers[3].unk4 = 0;
    gRacePlayers->characterId = 0;
    gRacePlayers[1].characterId = one0;
    gRacePlayers[2].characterId = two0;
    gRacePlayers[3].characterId = 3;
    gRacePlayers->unk11 = 0;
    gRacePlayers[1].unk11 = 0;
    gRacePlayers[2].unk11 = 0;
    five1 = 5;
    gRacePlayers[3].unk11 = 0;
    gRacePlayers->unk12 = 3;
    gRacePlayers[1].unk12 = five0;
    player = gRacePlayers;
    gRacePlayers[2].unk12 = six0;
    gRacePlayers[3].unk12 = seven0;
    player->unk17 = 0;
    gRacePlayers[1].unk17 = one1;
    gRacePlayers[2].unk17 = two2;
    gRacePlayers[3].unk17 = three0;
    gPlayerCount = 4;
    player->isActive = one1;
    gRacePlayers[1].isActive = one1;
    gRacePlayers[2].isActive = one1;
    gRacePlayers[3].isActive = one1;
    gRacePlayerCount = 4;
    gRaceLapCount = five1;
    gRacePlayerAttackStartTimer = 0x64;
    initCallbackTaskScheduler(one1);

    D_80121D95 = 0;
    D_80121D94 = 0;
    D_80121D96 = five1;
    D_801223A1 = 0;
    D_801223A0 = 0;
    D_801223A2 = 5;
    D_801229AD = 0;
    D_801229AC = 0;
    D_801229AE = five1;
    D_80122FB9 = 0;
    D_80122FB8 = 0;
    D_80122FBA = five1;

    temp_v0 = &gMainMenuModePreviewRaceCourseAssets[gMainMenuModeSelection];
    loadCompressedRomAsset(temp_v0->romStart, temp_v0->romEnd, 0x2B);
    loadRaceCourseAssets();
    loadRaceCharacterAssets();
    resetRaceCameras();
    resetAllViewports();
    gFramebufferSwapDelay = 0;
    initRacePlayers();
    gPlayerCount = 1;
    initRaceHud();
    gPlayerCount = 4;
    configureViewport(0, 0xA0, 0x50, 0x108, 0x78, 0x140, 0x8C, 2.285714388f);
    gRacePlayerHudStatuses = one1;
    gFramebufferSwapDelay = 0;
    initRaceCourseSceneTasks();
    gMenuFadeAlpha = 0xFF;
    gRaceRumbleEnabled = 0;
    gMenuFlowState = one1;
    LOAD_ASSET(_593D10, 0x29);
    LOAD_ASSET(_60F1A0, 0x2A);
    gMainMenuSelectionResult = 0;
    createCallbackTask(initMainMenuModeDescriptionPanel, 0, 0x64);
    createCallbackTask(initRaceSetupCornerPrompts, 0, 0x64);
    one1++;
    one1--;
    setCurrentGameTaskCallback(fadeInMainMenuModePreviewRace, 0);
    requestMusicSequenceBank(seven1);
}

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

void zoomMainMenuModePreviewRaceViewport(void) {
    s32 timer;
    s32 width;
    s32 centerY;
    s32 scaleY;
    s32 height;
    f32 aspect;

    gCurrentGameTask->transitionTimer += 1;
    timer = gCurrentGameTask->transitionTimer;
    width = (s16) (((timer * 0x18) / 16) + 0x108);
    centerY = (s16) (((timer * 0x28) / 16) + 0x50);
    height = (s16) (((((0, timer)) * 0x58) / 16) + 0x78);
    scaleY = (s16) (((timer * 0x64) / 16) + 0x8C);
    aspect = (f32) ((((f64) timer * -0.9523809523809523) / 0x10) + 2.2857142857142856);
    timer = 0x140;
    configureViewport(width * 0, 0xA0, centerY, width, height, timer, scaleY, aspect);
    if (gCurrentGameTask->transitionTimer == MAIN_MENU_MODE_PREVIEW_RACE_VIEWPORT_ZOOM_FRAMES) {
        requestMusicSequenceBank(0xF);
        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, timer, 0xF0, 1.3333334f);
        gCurrentGameTask->transitionTimer = 0;
        setCurrentGameTaskCallback(runMainMenuModePreviewRace, 0);
    }
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
}

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

// initTrainingCourseRace best match: asm-differ score 200 (nonmatchings/initTrainingCourseRace-3885303446860889946/base_38.c)

#pragma GLOBAL_ASM("asm/nonmatchings/menu/main_menu/main_menu_race_flow/initTrainingCourseRace.s")

#ifdef NON_MATCHING
void initTrainingCourseRace(void) {
    u64 characterId;
    unsigned int new_var;
    unsigned int new_var2;

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
    characterId = 1;
    gRaceDemoPlaybackEnabled = 0;
    gMainMenuModeSelection = 0;
    gRaceSplitscreenMode = 0;
    gRaceTypeSelection = 0;

    gRacePlayers[0].unk4 = 0;
    gRacePlayers[1].unk4 = 1;
    gRacePlayers[2].unk4 = characterId;
    gRacePlayers[3].unk4 = 1;
    new_var2 = 0;
    new_var = (unsigned int) (new_var = (gRacePlayers[new_var2].unk11 = new_var2));
    gRacePlayers[new_var2].characterId = new_var;
    characterId = 2;
    gRacePlayers[2].characterId = characterId;
    gRacePlayers[3].characterId = 3;
    gRacePlayers[1].characterId = 1;
    gRacePlayers[1].unk11 = 0;
    gRacePlayers[2].unk11 = 0;
    gRacePlayers[3].unk11 = 0;
    gRacePlayers[0].unk12 = 3;
    gRacePlayers[0].isActive = 1;
    gRacePlayers[2].unk12 = 6;
    gRacePlayers[1].unk12 = 5;
    if (gTrainingCourseLesson == 0) {
    }
    gRacePlayers[3].unk12 = 7;
    gRacePlayers[1].unk17 = 1;
    gRacePlayers[2].unk17 = 2;
    gRacePlayers[3].unk17 = 3;
    gRacePlayers[0].unk17 = 0;
    gPlayerCount = 1;

    switch (gTrainingCourseLesson) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 9:
            gRacePlayers[0].isActive = 1;
            gRacePlayers[1].isActive = 0;
            gRacePlayers[2].isActive = 0;
            gRacePlayers[3].isActive = 0;
            gRacePlayerCount = 1;
            break;
        default:
            gRacePlayers[0].isActive = 1;
            do {
            } while (0);
            gRacePlayers[1].isActive = 1;
            if (new_var) {
            }
            gRacePlayers[2].isActive = 1;
            gRacePlayers[3].isActive = 1;
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
    LOAD_ASSET(_593D10, 0x29);
    LOAD_ASSET(_60F1A0, 0x2A);
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

void zoomTrainingCourseRaceViewport(void) {
    s32 timer;
    s32 width;
    s32 centerY;
    s32 scaleY;
    s32 height;
    f32 aspect;

    gCurrentGameTask->transitionTimer += 1;
    timer = gCurrentGameTask->transitionTimer;
    width = (s16) (((timer * 0x18) / 16) + 0x108);
    centerY = (s16) (((timer * 0x28) / 16) + 0x50);
    height = (s16) (((((0, timer)) * 0x58) / 16) + 0x78);
    scaleY = (s16) (((timer * 0x64) / 16) + 0x8C);
    aspect = (f32) ((((f64) timer * -0.9523809523809523) / 0x10) + 2.2857142857142856);
    timer = 0x140;
    configureViewport(width * 0, 0xA0, centerY, width, height, timer, scaleY, aspect);
    if (gCurrentGameTask->transitionTimer == 0x10) {
        requestMusicSequenceBank(0xF);
        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, timer, 0xF0, 1.3333334f);
        setCurrentGameTaskCallback(runTrainingCourseUntilLessonEnd, 0);
    }
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
}

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
    LOAD_ASSET(_593D10, 0x29);
    LOAD_ASSET(_60F1A0, 0x2A);
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
