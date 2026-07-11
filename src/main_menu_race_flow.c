#include "common.h"
#include "sound_manager.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "race_scene_loader.h"
#include "race_input_history.h"
#include "game_task_scheduler.h"
#include "training_course_ui.h"
#include "main_menu_visual_effects.h"
#include "main_menu_panel_ui.h"
#include "race_camera.h"
#include "main_menu_race_flow.h"
#include "race_player_state.h"
#include "race_timer_ui.h"
#include "viewport_manager.h"

#define MAIN_MENU_DEMO_RACE_VIEWPORT_ZOOM_FRAMES 0x10

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
} MainMenuDemoRaceCourseAsset;


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
extern u8 D_8011228C;
extern s16 D_801124B8;
extern s16 gRaceCourseIndex;
extern s16 gRaceLapCount;
extern s8 gRacePlayerCount;
extern u8 gPlayerCount;
extern u8 gRaceUpdatePaused;
extern s16 gMenuFadeAlpha;
extern u8 gFramebufferSwapHold;
extern s32 D_801235B4;
extern u8 D_80121B58;
extern u8 D_80121B59;
extern u8 gMainMenuModeSelection;
extern s16 D_80121B5C;
extern u8 D_80121B5E;
extern u8 D_80121B5F;
extern s16 gMainMenuDemoRaceDurationBySelection[];

extern void releaseMenuAssetHandles(void);
extern u8 gPendingFramebufferSwapCount;
extern s8 gFramebufferSwapDelay;
extern u8 gTrainingCourseLesson;
extern u8 gRaceRumbleEnabled;
extern u8 gRaceSplitscreenMode;
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
extern MainMenuDemoRaceCourseAsset gMainMenuDemoRaceCourseAssets[];
extern f32 gMainMenuDemoRacePreviewAspectRatio;
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_60F1A0[];
extern u8 D_60F990[];
extern s16 gRacePlayerSurfaceAngleByPlayer;

void startMainMenuDemoRaceSelectionFlow(void) {
    gMainMenuModeSelection = 1;
    setCurrentGameTaskCallback(initMainMenuDemoRaceSelectionMenu, 0);
}

void initMainMenuDemoRaceSelectionMenu(void) {
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
    D_801124B8 = 0x80;
    initCallbackTaskScheduler(0);
    func_80070C64(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    func_8006D5CC();
    func_8006D520(0, 0x1F);
    createCallbackTaskWithUserId(&initTitleMenuBoardModels, 0, 0x64, 0);
    createCallbackTask(&initMainMenuModeSelectGrid, 0, 0x64);
    setCurrentGameTaskCallback(&fadeInMainMenuDemoRaceSelectionMenu, 0);
    requestMusicSequenceBank(7);
}

void fadeInMainMenuDemoRaceSelectionMenu(void) {
    gMenuFadeAlpha -= 0x10;
    if (gMenuFadeAlpha <= 0) {
        gMenuFadeAlpha = 0;
        setCurrentGameTaskCallback(&updateMainMenuDemoRaceSelectionMenu, 0);
    }
    func_8006D780(0);
    updateCallbackTasks();
}

void updateMainMenuDemoRaceSelectionMenu(void) {
    if (gMainMenuSelectionResult != 0) {
        if (gMainMenuModeSelection == 0xC) {
            requestMusicSequenceStop(0x3C);
        }
        setCurrentGameTaskCallback(&fadeOutMainMenuDemoRaceSelectionMenu, 0);
    }
    func_8006D780(0);
    updateCallbackTasks();
}

void fadeOutMainMenuDemoRaceSelectionMenu(void) {
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0xFF) {
        gMenuFadeAlpha = 0xFF;
        gFramebufferSwapHold = 1;
        setCurrentGameTaskCallback(&exitMainMenuDemoRaceSelectionMenu, 0);
    }
    func_8006D780(0);
    updateCallbackTasks();
}

void exitMainMenuDemoRaceSelectionMenu(void) {
    if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        if (gMainMenuModeSelection != 0xC) {
            setCurrentGameTaskCallback(&initMainMenuDemoRace, 0);
        } else {
            D_801235B4 = 0;
            resumeGameTask(3);
            removeGameTask(4);
        }
    }
}

// initMainMenuDemoRace best match: 94.795% (nonmatchings/initMainMenuDemoRace-8662636370764828261/base_5.c)

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_race_flow/initMainMenuDemoRace.s")

#ifdef NON_MATCHING
void initMainMenuDemoRace(void) {
    RaceInputPlayer *players;
    MainMenuDemoRaceCourseAsset *courseAsset;

    gRaceCourseIndex = gMainMenuDemoRaceCourseAssets[gMainMenuModeSelection].courseIndex;
    gRaceUpdatePaused = 0;
    D_80121B58 = 0;
    D_80121B5F = 0;
    D_80121B59 = 0;
    gTrainingCourseLesson = 0;
    gRaceSplitscreenMode = 0;
    D_80121B5E = 0;
    resetGameplayRng();

    players = D_80121D80;
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
    D_80121B5C = 0x64;
    initCallbackTaskScheduler(1);

    players[0].unk15 = 0;
    players[0].soundDisabled = 0;
    players[0].replayInputSource = 5;
    players[1].unk15 = 0;
    players[1].soundDisabled = 0;
    players[1].replayInputSource = 5;
    players[2].unk15 = 0;
    players[2].soundDisabled = 0;
    players[2].replayInputSource = 5;
    players[3].unk15 = 0;
    players[3].soundDisabled = 0;
    players[3].replayInputSource = 5;

    courseAsset = &gMainMenuDemoRaceCourseAssets[gMainMenuModeSelection];
    loadCompressedRomAsset(courseAsset->romStart, courseAsset->romEnd, 0x2B);
    loadRaceCourseAssets();
    loadRaceCharacterAssets();
    func_8006D5CC();
    resetAllViewports();
    gFramebufferSwapDelay = 0;
    func_8008BEB0();
    gPlayerCount = 1;
    func_80078430();
    gPlayerCount = 4;
    configureViewport(0, 0xA0, 0x50, 0x108, 0x78, 0x140, 0x8C, gMainMenuDemoRacePreviewAspectRatio);
    D_8011228C = 1;
    gFramebufferSwapDelay = 0;
    initRaceCourseEffects();
    gMenuFadeAlpha = 0xFF;
    gRaceRumbleEnabled = 0;
    D_801235B4 = 1;
    loadCompressedRomAsset(D_593D10, D_598A70, 0x29);
    loadCompressedRomAsset(D_60F1A0, D_60F990, 0x2A);
    gMainMenuSelectionResult = 0;
    createCallbackTask(initMainMenuModeDescriptionPanel, 0, 0x64);
    createCallbackTask(initRaceSetupCornerPrompts, 0, 0x64);
    setCurrentGameTaskCallback(fadeInMainMenuDemoRace, 0);
    requestMusicSequenceBank(7);
}
#endif

void fadeInMainMenuDemoRace(void) {
    gMenuFadeAlpha -= 0x10;
    if (gMenuFadeAlpha < 0) {
        gMenuFadeAlpha = 0;
        setCurrentGameTaskCallback(&waitForMainMenuDemoRaceStart, 0);
    }
    func_8008C704();
    updateCallbackTasksWithMinPriority(0x63);
    func_80096E3C();
    updateRemainingCallbackTasks();
    func_8006D700();
    D_801124B8 = 0x80;
}

void waitForMainMenuDemoRaceStart(void) {
    func_8008C704();
    updateCallbackTasksWithMinPriority(0x63);
    func_80096E3C();
    updateRemainingCallbackTasks();
    func_8006D700();
    if (gMainMenuSelectionResult != 0) {
        gCurrentGameTask->transitionTimer = 0;
        requestMusicSequenceStop(0x20);
        setCurrentGameTaskCallback(&zoomMainMenuDemoRaceViewport, 0);
    }
    D_801124B8 = 0x80;
}

// zoomMainMenuDemoRaceViewport best match: 75.250% (nonmatchings/zoomMainMenuDemoRaceViewport-1197934324348345530/base_3.c)

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_race_flow/zoomMainMenuDemoRaceViewport.s")

#ifdef NON_MATCHING
void zoomMainMenuDemoRaceViewport(void) {
    s32 timer;
    s32 x2;
    s32 x1;
    s32 y1;
    s32 y2;
    f64 denominator = MAIN_MENU_DEMO_RACE_VIEWPORT_ZOOM_FRAMES;
    f32 aspect;

    gCurrentGameTask->transitionTimer += 1;
    timer = gCurrentGameTask->transitionTimer;

    x2 = timer << 2;
    x2 -= timer;
    x2 <<= 3;
    x2 = (s16) ((x2 / MAIN_MENU_DEMO_RACE_VIEWPORT_ZOOM_FRAMES) + 0x108);

    x1 = (s16) (((timer * 0x28) / MAIN_MENU_DEMO_RACE_VIEWPORT_ZOOM_FRAMES) + 0x50);
    y1 = (s16) (((timer * 0x58) / MAIN_MENU_DEMO_RACE_VIEWPORT_ZOOM_FRAMES) + 0x78);
    y2 = (s16) (((timer * 0x64) / MAIN_MENU_DEMO_RACE_VIEWPORT_ZOOM_FRAMES) + 0x8C);
    aspect = (f32) ((((f64) timer * -0.9523809523809523) / denominator) + 2.2857142857142856);

    configureViewport(0, 0xA0, x1, x2 & 0xFFFF, y1, 0x140, y2, aspect);
    if (gCurrentGameTask->transitionTimer == MAIN_MENU_DEMO_RACE_VIEWPORT_ZOOM_FRAMES) {
        requestMusicSequenceBank(0xF);
        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        gCurrentGameTask->transitionTimer = 0;
        setCurrentGameTaskCallback(runMainMenuDemoRace, 0);
    }
    func_8008C704();
    updateCallbackTasksWithMinPriority(0x63);
    func_80096E3C();
    updateRemainingCallbackTasks();
    func_8006D700();
}
#endif

void runMainMenuDemoRace(void) {
    D_801235B4 = 0;
    func_8008C704();
    updateCallbackTasksWithMinPriority(0x63);
    func_80096E3C();
    updateRemainingCallbackTasks();
    func_8006D700();
    func_8007AA50();
    gCurrentGameTask->transitionTimer += 1;
    if (gCurrentGameTask->transitionTimer == gMainMenuDemoRaceDurationBySelection[gMainMenuModeSelection]) {
        setCurrentGameTaskCallback(fadeOutMainMenuDemoRace, 0);
        requestMusicSequenceStop(0x40);
    }
}

void fadeOutMainMenuDemoRace(void) {
    func_8008C704();
    updateCallbackTasksWithMinPriority(0x63);
    func_80096E3C();
    updateRemainingCallbackTasks();
    func_8006D700();
    func_8007AA50();
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0xFF) {
        gFramebufferSwapHold = 1;
        {
            void (*func_ptr)(void) = returnToMainMenuDemoRaceSelectionMenu;
            setCurrentGameTaskCallback(func_ptr, 0);
        }
    }
}

void returnToMainMenuDemoRaceSelectionMenu(void) {
    if (gPendingFramebufferSwapCount == 2) {
        stopSoundEffects();
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        setCurrentGameTaskCallback(&initMainMenuDemoRaceSelectionMenu, 0);
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
    D_80121B58 = 0;
    D_80121B5F = 0;
    D_80121B59 = 0;
    gMainMenuModeSelection = 0;
    gRaceSplitscreenMode = 0;
    D_80121B5E = 0;

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
    D_80121B5C = 0x64;
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
    func_8006D5CC();
    resetAllViewports();
    D_8011228C = 1;
    gFramebufferSwapDelay = 0;
    func_8008BEB0();
    func_80078430();
    configureViewport(0, 0xA0, 0x50, 0x108, 0x78, 0x140, 0x8C, 2.285714388f);
    D_8011228C = 1;
    gFramebufferSwapDelay = 0;
    initRaceCourseEffects();
    gMenuFadeAlpha = 0xFF;
    gRaceRumbleEnabled = 0;
    D_801235B4 = 1;
    loadCompressedRomAsset(D_593D10, D_598A70, 0x29);
    loadCompressedRomAsset(D_60F1A0, D_60F990, 0x2A);
    gMainMenuSelectionResult = 0;
    createCallbackTask(initTrainingCourseOpeningDialog, 0, 0x64);
    createCallbackTask(initTrainingCourseTitlePanel, 0, 0x63);
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
    func_8008C704();
    updateCallbackTasksWithMinPriority(0x63);
    func_80096E3C();
    updateRemainingCallbackTasks();
    func_8006D700();
    D_801124B8 = 0x80;
}

void waitForTrainingCourseStartSelection(void) {
    func_8008C704();
    updateCallbackTasksWithMinPriority(0x63);
    func_80096E3C();
    updateRemainingCallbackTasks();
    func_8006D700();
    if (gMainMenuSelectionResult != 0) {
        gCurrentGameTask->transitionTimer = 0;
        requestMusicSequenceStop(0x20);
        setCurrentGameTaskCallback(&zoomTrainingCourseRaceViewport, 0);
    }
    D_801124B8 = 0x80;
}

// zoomTrainingCourseRaceViewport best match: 74.802% (nonmatchings/zoomTrainingCourseRaceViewport-1197934324348345530/base_8.c)

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_race_flow/zoomTrainingCourseRaceViewport.s")

#ifdef NON_MATCHING
void zoomTrainingCourseRaceViewport(void) {
    s32 timer;
    s32 x2;
    s32 x1;
    s32 y1;
    s32 y2;
    f64 denominator = MAIN_MENU_DEMO_RACE_VIEWPORT_ZOOM_FRAMES;
    f32 aspect;

    gCurrentGameTask->transitionTimer += 1;
    timer = gCurrentGameTask->transitionTimer;

    x2 = timer << 2;
    x2 -= timer;
    x2 <<= 3;
    x2 = (s16) ((x2 / MAIN_MENU_DEMO_RACE_VIEWPORT_ZOOM_FRAMES) + 0x108);

    x1 = (s16) (((timer * 0x28) / MAIN_MENU_DEMO_RACE_VIEWPORT_ZOOM_FRAMES) + 0x50);
    y1 = (s16) (((timer * 0x58) / MAIN_MENU_DEMO_RACE_VIEWPORT_ZOOM_FRAMES) + 0x78);
    y2 = (s16) (((timer * 0x64) / MAIN_MENU_DEMO_RACE_VIEWPORT_ZOOM_FRAMES) + 0x8C);
    aspect = (f32) ((((f64) timer * -0.9523809523809523) / denominator) + 2.2857142857142856);

    configureViewport(0, 0xA0, x1, x2 & 0xFFFF, y1, 0x140, y2, aspect);
    if (gCurrentGameTask->transitionTimer == MAIN_MENU_DEMO_RACE_VIEWPORT_ZOOM_FRAMES) {
        requestMusicSequenceBank(0xF);
        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        setCurrentGameTaskCallback(runTrainingCourseUntilActionPrompt, 0);
    }
    func_8008C704();
    updateCallbackTasksWithMinPriority(0x63);
    func_80096E3C();
    updateRemainingCallbackTasks();
    func_8006D700();
}
#endif

void runTrainingCourseUntilActionPrompt(void) {
    D_801235B4 = 0;
    func_8008C704();
    updateCallbackTasksWithMinPriority(0x63);
    func_80096E3C();
    updateRemainingCallbackTasks();
    func_8006D700();
    func_8007AA50();
    gCurrentGameTask->transitionTimer = 0;
    switch (gTrainingCourseLesson) {
        case 1:
            if (gRacePlayerSurfaceAngleByPlayer == 0x50) {
                setCurrentGameTaskCallback(fadeInTrainingCourseActionMenu, 0);
                requestMusicSequenceStop(0x40);
                return;
            }
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            if (gRacePlayerSurfaceAngleByPlayer == 0x9C) {
                setCurrentGameTaskCallback(fadeInTrainingCourseActionMenu, 0);
                requestMusicSequenceStop(0x40);
                return;
            }
            break;
        case 6:
            if (gRacePlayerSurfaceAngleByPlayer == 0xB4) {
                setCurrentGameTaskCallback(fadeInTrainingCourseActionMenu, 0);
                requestMusicSequenceStop(0x40);
                return;
            }
            break;
        case 7:
        case 8:
            if (gRacePlayerSurfaceAngleByPlayer == 0x16) {
                setCurrentGameTaskCallback(fadeInTrainingCourseActionMenu, 0);
                requestMusicSequenceStop(0x40);
                return;
            }
            break;
        case 9:
            if (gRacePlayerSurfaceAngleByPlayer == 0x36) {
                setCurrentGameTaskCallback(fadeInTrainingCourseActionMenu, 0);
                requestMusicSequenceStop(0x40);
            }
            break;
    }
}

void fadeInTrainingCourseActionMenu(void) {
    s32 temp_v1;

    gRaceUpdatePaused = 1;
    gMainMenuSelectionResult = 0;
    func_8008C704();
    updateCallbackTasksWithMinPriority(0x63);
    func_80096E3C();
    updateRemainingCallbackTasks();
    func_8006D700();
    func_8007AA50();
    gCurrentGameTask->transitionTimer += 0x10;
    temp_v1 = gCurrentGameTask->transitionTimer;
    if (temp_v1 == 0x80) {
        createCallbackTask(initTrainingCourseActionMenu, 0, 0x64);
        setCurrentGameTaskCallback(waitForTrainingCourseActionMenuSelection, 0);
    }
    temp_v1 = gCurrentGameTask->transitionTimer;
    D_801124B8 = temp_v1;
}

void waitForTrainingCourseActionMenuSelection(void) {
    gRaceUpdatePaused = 1;
    func_8008C704();
    updateCallbackTasksWithMinPriority(0x63);
    func_80096E3C();
    updateRemainingCallbackTasks();
    func_8006D700();
    func_8007AA50();
    D_801124B8 = 0x80;
    if (gMainMenuSelectionResult != 0) {
        setCurrentGameTaskCallback(&fadeOutTrainingCourseActionMenu, 0);
    }
}

void fadeOutTrainingCourseActionMenu(void) {
    gRaceUpdatePaused = 1;
    func_8008C704();
    updateCallbackTasksWithMinPriority(0x63);
    func_80096E3C();
    updateRemainingCallbackTasks();
    func_8006D700();
    func_8007AA50();
    D_801124B8 = 0x80;
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0xFF) {
        gFramebufferSwapHold = 1;
        setCurrentGameTaskCallback(&handleTrainingCourseActionMenuSelection, 0);
    }
}

void handleTrainingCourseActionMenuSelection(void) {
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
    D_801235B4 = 0;
    resumeGameTask(3);
    removeGameTask(4);
}
