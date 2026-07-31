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

void startTrainingCourseFlow(void) {
    gTrainingCourseLesson = 1;
    setCurrentGameTaskCallback(&queueTrainingCourseRaceInit, 0);
}

void queueTrainingCourseRaceInit(void) {
    setCurrentGameTaskCallback(initTrainingCourseRace, 0);
}

void initTrainingCourseRace(void) {
    u64 characterId;
    long long three0;
    unsigned int new_var;
    unsigned int new_var2;
    s32 five0;
    s32 six0;
    s32 seven0;
    s32 two0;
    s32 i;

    switch (gTrainingCourseLesson) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            gRaceCourseIndex.signedValue = 0;
            break;
        default:
            gRaceCourseIndex.signedValue = 9;
            break;
    }

    gRaceUpdatePaused = 0;
    gRaceCameraModeChangeDisabled = 0;
    gRaceResultState = 0;
    gRaceDemoPlaybackEnabled = 0;
    gMainMenuModeSelection = 0;
    gRaceSplitscreenMode = 0;
    gRaceTypeSelection = 0;

    characterId = 2;
    five0 = 5;
    six0 = 6;
    seven0 = 7;
    two0 = 2;
    three0 = 3;

    gRacePlayers[0].isCpu = 0;
    gRacePlayers[1].isCpu = 1;
    gRacePlayers[2].isCpu = 1;
    gRacePlayers[3].isCpu = 1;
    new_var2 = 0;
    new_var = (unsigned int)(new_var = (gRacePlayers[new_var2].characterId = new_var2));
    gRacePlayers[1].characterId = 1;
    gRacePlayers[2].characterId = characterId;
    gRacePlayers[3].characterId = 3;
    gRacePlayers[0].characterVariant = 0;
    gRacePlayers[1].characterVariant = 0;
    gRacePlayers[2].characterVariant = 0;
    gRacePlayers[3].characterVariant = 0;
    gRacePlayers[0].unk12 = 3;
    gRacePlayers[1].unk12 = five0;
    gRacePlayers[2].unk12 = six0;
    gRacePlayers[3].unk12 = seven0;
    gRacePlayers[0].unk17 = 0;
    gRacePlayers[1].unk17 = 1;
    gRacePlayers[2].unk17 = two0;
    gRacePlayers[3].unk17 = three0;
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

    for (i = 0; i < RACE_PLAYER_COUNT; i++) {
        gRacePlayers[i].unk15 = 0;
        gRacePlayers[i].replayInputSource = 0;
        gRacePlayers[i].soundDisabled = 0;
    }

    loadRaceCourseAssets();
    loadRaceCharacterAssets();
    resetRaceCameras();
    resetAllViewports();
    gRacePlayerHudStatuses[0].active = 1;
    gFramebufferSwapDelay.value = 0;
    initRacePlayers();
    initRaceHud();
    configureViewport(0, 0xA0, 0x50, 0x108, 0x78, 0x140, 0x8C, 2.285714388f);
    gRacePlayerHudStatuses[0].active = 1;
    gFramebufferSwapDelay.value = 0;
    initRaceCourseSceneTasks();
    gMenuFadeAlpha = 0xFF;
    gRaceRumbleEnabled = 0;
    gMenuFlowState = 1;
    LOAD_ASSET(_593D10, 0x29);
    LOAD_ASSET(_60F1A0, 0x2A);
    gMainMenuSelectionResult = 0;
    createCallbackTask((CallbackTaskCallback)initTrainingCourseOpeningDialog, 0, 0x64);
    createCallbackTask((CallbackTaskCallback)initTrainingCourseLessonTitlePanel, 0, 0x63);
    createCallbackTask((CallbackTaskCallback)initRaceSetupCornerPrompts, 0, 0x64);
    setCurrentGameTaskCallback(fadeInTrainingCourseRace, 0);
    requestMusicSequenceBank(7);
}

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
            if (gRacePlayers[0].coursePathIndex == 0x50) {
                setCurrentGameTaskCallback(fadeInTrainingCourseLessonEndMenu, 0);
                requestMusicSequenceStop(0x40);
                return;
            }
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            if (gRacePlayers[0].coursePathIndex == 0x9C) {
                setCurrentGameTaskCallback(fadeInTrainingCourseLessonEndMenu, 0);
                requestMusicSequenceStop(0x40);
                return;
            }
            break;
        case 6:
            if (gRacePlayers[0].coursePathIndex == 0xB4) {
                setCurrentGameTaskCallback(fadeInTrainingCourseLessonEndMenu, 0);
                requestMusicSequenceStop(0x40);
                return;
            }
            break;
        case 7:
        case 8:
            if (gRacePlayers[0].coursePathIndex == 0x16) {
                setCurrentGameTaskCallback(fadeInTrainingCourseLessonEndMenu, 0);
                requestMusicSequenceStop(0x40);
                return;
            }
            break;
        case 9:
            if (gRacePlayers[0].coursePathIndex == 0x36) {
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
        createCallbackTask((CallbackTaskCallback)initTrainingCourseLessonEndMenu, 0, 0x64);
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
        gFramebufferSwapDelay.value = 0;
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
    gFramebufferSwapDelay.value = 0;
    gMenuFadeAlpha = 0xFF;
    resetAllViewports();
    initCallbackTaskScheduler(0);
    createCallbackTask((CallbackTaskCallback)&initTrainingCourseEndingDialog, 0, 0x64);
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
        gFramebufferSwapDelay.value = 0;
        setCurrentGameTaskCallback(&returnToMainMenuFromTrainingCourse, 0);
    }
}

void returnToMainMenuFromTrainingCourse(void) {
    gMenuFlowState = 0;
    resumeGameTask(3);
    removeGameTask(4);
}
