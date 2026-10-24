#include "assets.h"
#include "game/race/race_state.h"
#include "game/demo/main_menu_demo_race_intro.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/engine/system_runtime.h"
#include "game/race/scene/race_scene_setup.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/menu/renderer/menu_screen_effects.h"
#include "game/race/camera/race_camera.h"
#include "game/race/player/race_player_update.h"
#include "game/race/ui/race_hud.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"
#include "game/race/flow/race_flow.h"

typedef struct {
    /* 0x00 */ u8 courseIndex;
    /* 0x01 */ u8 splitscreen;
    /* 0x02 */ u8 raceType;
    /* 0x03 */ u8 loadCourseAssets;
    /* 0x04 */ void *romStart;
    /* 0x08 */ void *romEnd;
    /* 0x0C */ u8 player0Character;
    /* 0x0D */ u8 player1Character;
    /* 0x0E */ u8 player2Character;
    /* 0x0F */ u8 player3Character;
} MainMenuDemoRaceIntroEntry;

u8 gMainMenuDemoRaceIntroIndex = 0;
MainMenuDemoRaceIntroEntry gMainMenuDemoRaceIntroEntries[] = {
    { RACE_COURSE_ROOKIE_MOUNTAIN,
     0, 0,
     0, (void *)&MAIN_MENU_DEMO_ROOKIE_MOUNTAIN_REPLAY_ROM_START,
     (void *)&MAIN_MENU_DEMO_ROOKIE_MOUNTAIN_REPLAY_ROM_END,
     0, 1,
     2, 3 },
    { RACE_COURSE_BIG_SNOWMAN,
     1, 0,
     0, (void *)&MAIN_MENU_DEMO_BIG_SNOWMAN_REPLAY_ROM_START,
     (void *)&MAIN_MENU_DEMO_BIG_SNOWMAN_REPLAY_ROM_END,
     4, 1,
     2, 3 },
    { RACE_COURSE_SUNSET_ROCK,
     1, 1,
     0, (void *)&MAIN_MENU_DEMO_SUNSET_ROCK_REPLAY_ROM_START,
     (void *)&MAIN_MENU_DEMO_SUNSET_ROCK_REPLAY_ROM_END,
     2, 0,
     1, 3 },
    { RACE_COURSE_ANIMAL_LAND,
     1, 2,
     0, (void *)&MAIN_MENU_DEMO_ANIMAL_LAND_REPLAY_ROM_START,
     (void *)&MAIN_MENU_DEMO_ANIMAL_LAND_REPLAY_ROM_END,
     1, 0,
     1, 3 },
    { RACE_COURSE_GRASS_VALLEY,
     0, 0,
     1, (void *)&MAIN_MENU_DEMO_GRASS_VALLEY_REPLAY_ROM_START,
     (void *)&MAIN_MENU_DEMO_GRASS_VALLEY_REPLAY_ROM_END,
     3, 1,
     4, 2 },
};

void initMainMenuDemoRaceIntro(void) {
    RacePlayer *players;
    gRaceCourseIndex.signedValue = gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex].courseIndex;
    gRaceUpdatePaused = 0;
    gRaceCameraModeChangeDisabled = 0;
    gRaceResultState = 0;
    gRaceDemoPlaybackEnabled = 0;
    gTrainingCourseLesson = 0;
    gMainMenuModeSelection = 0;
    gRaceSplitscreenMode = gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex].splitscreen;
    gRaceTypeSelection = gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex].raceType;
    resetGameplayRng();
    players = gRacePlayers;
    gRacePlayers->isCpu = 0;
    gRacePlayers[1].isCpu = 0;
    gRacePlayers[2].isCpu = 0;
    gRacePlayers[3].isCpu = 0;
    gRacePlayers->characterId = gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex].player0Character;
    gRacePlayers->characterVariant = 6;
    gRacePlayers[1].characterId = gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex].player1Character;
    gRacePlayers[1].characterVariant = 6;
    gRacePlayers[2].characterId = gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex].player2Character;
    gRacePlayers[2].characterVariant = 6;
    gRacePlayers[3].characterId = gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex].player3Character;
    gRacePlayers[3].characterVariant = 6;
    gRacePlayers->snowboardTextureIndex = 1;
    gRacePlayers[1].snowboardTextureIndex = 2;
    gRacePlayers[2].snowboardTextureIndex = 3;
    gRacePlayers[3].snowboardTextureIndex = 4;
    if (gRaceSplitscreenMode == 0) {
        gRacePlayers->isActive = 1;
        gRacePlayers[1].isActive = 1;
        gRacePlayers[2].isActive = 1;
        gRacePlayers[3].isActive = 1;
        gRacePlayerCount = 4;
        gPlayerCount = 4;
    } else {
        gRacePlayers->isActive = 1;
        gRacePlayers[1].isActive = 0;
        gRacePlayers[2].isActive = 0;
        gRacePlayers[3].isActive = 0;
        gRacePlayerCount = 1;
        gPlayerCount = 1;
    }
    gRaceLapCount = 2;
    gRacePlayerAttackStartTimer = 0x64;
    if (gRaceSplitscreenMode == 0) {
        initCallbackTaskScheduler(1);
    } else {
        initCallbackTaskScheduler(2);
    }
    players[0].unk15 = 0;
    players[0].soundDisabled = 0;
    players[0].replayInputSource = RACE_PLAYER_INPUT_REPLAY;
    players[1].unk15 = 0;
    players[1].soundDisabled = 0;
    players[1].replayInputSource = RACE_PLAYER_INPUT_REPLAY;
    players[2].unk15 = 0;
    players[2].soundDisabled = 0;
    players[2].replayInputSource = RACE_PLAYER_INPUT_REPLAY;
    players[3].unk15 = 0;
    players[3].soundDisabled = 0;
    players[3].replayInputSource = RACE_PLAYER_INPUT_REPLAY;
    loadCompressedRomAsset(
        gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex].romStart,
        gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex].romEnd,
        0x2B
    );
    loadRaceCourseAssets();
    loadRaceCharacterAssets();
    gMenuFlowState = 0;
    resetRaceCameras();
    resetAllViewports();
    gRacePlayerHudStatuses[0].active = 1;
    gFramebufferRenderInterval.value = 0;
    initRacePlayers();
    if (!gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex].loadCourseAssets) {
        gPlayerCount = 1;
    }
    initRaceHud();
    if (gPlayerCount == 1) {
        configureViewport(0, 0xA0, 0x78, 0x120U, 0xD0U, 0x140U, 0xF0U, gDemoRaceFullscreenAspectRatio);
        gRacePlayerHudStatuses[0].active = 1;
        gFramebufferRenderInterval.value = 0;
    } else {
        configureViewport(0, 0x57, 0x43, 0x90U, 0x68U, 0xA0U, 0x78U, gDemoRaceTopLeftAspectRatio);
        configureViewport(1, 0x57, 0xAD, 0x90U, 0x68U, 0xA0U, 0x78U, gDemoRaceBottomLeftAspectRatio);
        configureViewport(2, 0xE9, 0x43, 0x90U, 0x68U, 0xA0U, 0x78U, gDemoRaceTopRightAspectRatio);
        configureViewport(3, 0xE9, 0xAD, 0x90U, 0x68U, 0xA0U, 0x78U, gDemoRaceBottomRightAspectRatio);
        gRaceCameras[0].initialized.signedValue = 1;
        gRaceCameras[1].initialized.signedValue = 1;
        gRaceCameras[2].initialized.signedValue = 1;
        gRaceCameras[3].initialized.signedValue = 1;
        if (1) {
            *((u8 *)&gFramebufferRenderInterval.value) = 1;
        }
    }
    if (gRaceSplitscreenMode == 0) {
        gPlayerCount = 4;
    }
    initRaceCourseSceneTasks();
    setCurrentGameTaskCallback(waitForMainMenuDemoRaceIntroStart, 0);
    gMenuFadeAlpha = 0xFF;
    gRaceRumbleEnabled = 0;
    gCurrentGameTask->callbackData0 = 0x384;
    gCurrentGameTask->callbackData1 = 0;
    gCurrentGameTask->callbackData2 = 0;
    gCurrentGameTask->callbackData3 = 0x14;
}

void waitForMainMenuDemoRaceIntroStart(void) {
    gCurrentGameTask->callbackData3--;
    if (gCurrentGameTask->callbackData3 == 0) {
        requestCourseMusicSequence();
        createCallbackTask((CallbackTaskCallback)updateTitleScreenStartPrompt, 0, 0x64);
        setCurrentGameTaskCallback(updateMainMenuDemoRaceIntro, 0);
    }
}

void updateMainMenuDemoRaceIntro(void) {
    GameTask *state;
    s32 fadeStep;

    if (gCurrentGameTask->callbackData1 == 0) {
        gMenuFadeAlpha -= 0x10;
        if (gMenuFadeAlpha < 0) {
            gMenuFadeAlpha = 0;
        }
    }
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
    updateRaceHud();
    state = gCurrentGameTask;
    if (state->callbackData0 != 0) {
        state->callbackData0--;
        state = gCurrentGameTask;
        fadeStep = state->callbackData1;
    } else {
        fadeStep = state->callbackData1;
        if (fadeStep == 0) {
            state->callbackData1 = 4;
            requestMusicSequenceStop(0x78);
            state = gCurrentGameTask;
            fadeStep = state->callbackData1;
        }
    }
    if ((gPlayerInputPressed[0] & START_BUTTON) && (fadeStep == 0)) {
        state->callbackData1 = 0x10;
        requestMusicSequenceStop(0x1E);
        state = gCurrentGameTask;
        fadeStep = state->callbackData1;
    }
    if (fadeStep != 0) {
        gMenuFadeOverlayActive = 1;
        gMenuFadeAlpha += state->callbackData1;
        if (!(gMenuFadeAlpha < 0xFF)) {
            gMenuFadeAlpha = 0xFF;
            gFramebufferSwapHold = 1;
            setCurrentGameTaskCallback(finishMainMenuDemoRaceIntro, 0);
        }
    }
}

void finishMainMenuDemoRaceIntro(void) {
    if (gPendingFramebufferSwapCount == 2) {
        if ((gMainMenuDemoRaceIntroIndex = gMainMenuDemoRaceIntroIndex + 1) >= 5) {
            gMainMenuDemoRaceIntroIndex = 0;
        }
        gRaceRumbleEnabled = 0;
        gRaceCameraModeChangeDisabled = 0;
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferRenderInterval.value = 0;
        stopSoundEffects();
        gMenuFlowState = 0;
        resumeGameTask(3);
        removeGameTask(4);
    }
}
