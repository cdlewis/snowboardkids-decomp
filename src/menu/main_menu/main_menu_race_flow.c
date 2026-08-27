#include "game/race/race_state.h"
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
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/main_menu/main_menu_panel_ui.h"
#include "game/race/camera/race_camera.h"
#include "game/menu/main_menu/main_menu_race_flow.h"
#include "game/race/player/race_player_update.h"
#include "game/race/ui/race_hud.h"
#include "game/engine/viewport_manager.h"

#define MAIN_MENU_MODE_PREVIEW_RACE_VIEWPORT_ZOOM_FRAMES 0x10

typedef struct {
    /* 0x0 */ u8 courseIndex;
    /* 0x1 */ u8 pad1[3];
    /* 0x4 */ u8 *romStart;
    /* 0x8 */ u8 *romEnd;
} MainMenuModePreviewRaceCourseAsset;

MainMenuModePreviewRaceCourseAsset gMainMenuModePreviewRaceCourseAssets[] = {
    { 0, { 0, 0, 0 }, NULL,           NULL           },
    { 0, { 0, 0, 0 }, (u8 *)0x258C80, (u8 *)0x25B410 },
    { 0, { 0, 0, 0 }, (u8 *)0x25B410, (u8 *)0x25DF80 },
    { 9, { 0, 0, 0 }, (u8 *)0x25DF80, (u8 *)0x260A30 },
    { 0, { 0, 0, 0 }, (u8 *)0x260A30, (u8 *)0x263520 },
    { 0, { 0, 0, 0 }, (u8 *)0x263520, (u8 *)0x265D30 },
    { 4, { 0, 0, 0 }, (u8 *)0x265D30, (u8 *)0x2688C0 },
    { 1, { 0, 0, 0 }, (u8 *)0x2688C0, (u8 *)0x26B1D0 },
    { 3, { 0, 0, 0 }, (u8 *)0x26B1D0, (u8 *)0x26DE60 },
    { 2, { 0, 0, 0 }, (u8 *)0x26DE60, (u8 *)0x270730 },
    { 9, { 0, 0, 0 }, (u8 *)0x270730, (u8 *)0x2732E0 },
    { 0, { 0, 0, 0 }, (u8 *)0x2732E0, (u8 *)0x275A90 },
};
s16 gMainMenuModePreviewRaceDurationBySelection[] = {
    0x384, 0x287, 0x2BB, 0x33B, 0x23F, 0x2B2, 0x2C7, 0x138, 0x1AF, 0x1FD, 0x16A, 0x1E2, 0, 0, 0, 0,
};

extern s16 gRaceLapCount;
extern s8 gRacePlayerCount;
extern u8 gRaceUpdatePaused;
extern s16 gMenuFadeAlpha;
extern u8 gFramebufferSwapHold;
extern s32 gMenuFlowState;
extern u8 gMainMenuModeSelection;
extern s16 gRacePlayerAttackStartTimer;

extern void releaseMenuAssetHandles(void);
extern u8 gPendingFramebufferSwapCount;
extern u8 gTrainingCourseLesson;

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
    gFramebufferRenderInterval.value = 0;
    gMenuFadeAlpha = 0xFF;
    resetAllViewports();
    gViewportStates[0].overlayAlpha = 0x80;
    initCallbackTaskScheduler(0);
    configureMenuViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    resetRaceCameras();
    setRaceCameraMode(0, 0x1F);
    createCallbackTaskWithUserId((CallbackTaskCallback)&initMainMenuBoardModels, 0, 0x64, 0);
    createCallbackTask((CallbackTaskCallback)&initMainMenuModeSelectGrid, 0, 0x64);
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
        gFramebufferRenderInterval.value = 0;
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
    s32 i;
    RacePlayer *player;

    gRaceCourseIndex.signedValue = (s16)gMainMenuModePreviewRaceCourseAssets[gMainMenuModeSelection].courseIndex;
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
    gRacePlayers->isCpu = 0;
    gRacePlayers[1].isCpu = 0;
    gRacePlayers[2].isCpu = 0;
    gRacePlayers[3].isCpu = 0;
    gRacePlayers->characterId = 0;
    gRacePlayers[1].characterId = one0;
    gRacePlayers[2].characterId = two0;
    gRacePlayers[3].characterId = 3;
    gRacePlayers->characterVariant = 0;
    gRacePlayers[1].characterVariant = 0;
    gRacePlayers[2].characterVariant = 0;
    five1 = 5;
    gRacePlayers[3].characterVariant = 0;
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

    for (i = 0; i < RACE_PLAYER_COUNT; i++) {
        gRacePlayers[i].unk15 = 0;
        gRacePlayers[i].soundDisabled = 0;
        gRacePlayers[i].replayInputSource = five1;
    }

    temp_v0 = &gMainMenuModePreviewRaceCourseAssets[gMainMenuModeSelection];
    loadCompressedRomAsset(temp_v0->romStart, temp_v0->romEnd, 0x2B);
    loadRaceCourseAssets();
    loadRaceCharacterAssets();
    resetRaceCameras();
    resetAllViewports();
    gFramebufferRenderInterval.value = 0;
    initRacePlayers();
    gPlayerCount = 1;
    initRaceHud();
    gPlayerCount = 4;
    configureViewport(0, 0xA0, 0x50, 0x108, 0x78, 0x140, 0x8C, 2.285714388f);
    gRacePlayerHudStatuses[0].active = one1;
    gFramebufferRenderInterval.value = 0;
    initRaceCourseSceneTasks();
    gMenuFadeAlpha = 0xFF;
    gRaceRumbleEnabled = 0;
    gMenuFlowState = one1;
    LOAD_ASSET(_593D10, 0x29);
    LOAD_ASSET(_60F1A0, 0x2A);
    gMainMenuSelectionResult = 0;
    createCallbackTask((CallbackTaskCallback)initMainMenuModeDescriptionPanel, 0, 0x64);
    createCallbackTask((CallbackTaskCallback)initRaceSetupCornerPrompts, 0, 0x64);
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
    width = (s16)(((timer * 0x18) / 16) + 0x108);
    centerY = (s16)(((timer * 0x28) / 16) + 0x50);
    height = (s16)(((((0, timer)) * 0x58) / 16) + 0x78);
    scaleY = (s16)(((timer * 0x64) / 16) + 0x8C);
    aspect = (f32)((((f64)timer * -0.9523809523809523) / 0x10) + 2.2857142857142856);
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
        gFramebufferRenderInterval.value = 0;
        setCurrentGameTaskCallback(&initMainMenuModePreviewRaceSelectionMenu, 0);
    }
}
