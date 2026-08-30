#include "game/race/race_state.h"
#include "assets.h"
#include "game/demo/title_demo_race_intro.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/engine/system_runtime.h"
#include "game/race/scene/race_scene_setup.h"
#include "game/engine/game_task_scheduler.h"
#include "game/race/camera/race_camera.h"
#include "game/race/player/race_player_update.h"
#include "game/race/ui/race_hud.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/menu/renderer/menu_screen_effects.h"

s16 gTitleDemoReplaySegmentFrames[16] = {
    0x0000, 0x0039, 0x006C, 0x00A1, 0x00D2, 0x0107, 0x0138, 0x026C,
    0x02A2, 0x02D3, 0x0307, 0x0339, 0x036E, 0x03A0, 0x03D4, 0x0413,
};

s16 gTitleDemoCameraModeFrames[22] = {
    0x0000, 0x0039, 0x006C, 0x00A1, 0x00D2, 0x0107, 0x0138, 0x015D, 0x01BE, 0x01D2, 0x01FE,
    0x023A, 0x026C, 0x02A2, 0x02D3, 0x0307, 0x0339, 0x036E, 0x03A0, 0x03D4, 0x0413, 0x0000,
};

u8 gTitleDemoCameraModes[24] = {
    0x08, 0x09, 0x0A, 0x0B, 0x10, 0x11, 0x0C, 0x12, 0x0D, 0x0E, 0x0F, 0x13,
    0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x00, 0x00, 0x00,
};

RacePlayerReplaySnapshot gTitleDemoReplayInputs[4][16] = {
#include "title_demo_replay_inputs.inc.c"
};

extern s16 gMenuFadeAlpha;
extern u8 gPendingFramebufferSwapCount;
extern s16 gRaceLapCount;
extern s8 gRacePlayerCount;
extern u8 gRaceUpdatePaused;
extern u8 gTrainingCourseLesson;
extern u8 gMainMenuModeSelection;
extern s16 gRacePlayerAttackStartTimer;
extern u8 gFramebufferSwapHold;
extern s32 gMenuFlowState;

#define RACE_PLAYER_REPLAY_SNAPSHOT(index) (((RacePlayerReplaySnapshot *)gRacePlayers)[index])

void initTitleDemoRaceIntro(void) {
    RacePlayer *players;
    register s32 one = 1;
    s32 active = 1;
    s32 two = 2;
    s32 three = 3;
    s32 four = 4;
    s32 six = 6;
    s32 five0 = 5;
    s32 five1 = 5;
    s32 five2 = 5;
    s32 five3 = 5;
    s32 playerOne;

    gRaceCourseIndex.signedValue = 0;
    playerOne = 1;
    gRaceUpdatePaused = 0;
    gRaceCameraModeChangeDisabled = 0;
    gRaceResultState = 0;
    gRaceDemoPlaybackEnabled = one;
    gPlayerCount = four;
    gRaceSplitscreenMode = 0;
    gTrainingCourseLesson = 0;
    gMainMenuModeSelection = 0;

    players = gRacePlayers;
    players[0].isCpu = 0;
    players[playerOne].isCpu = 0;
    players[2].isCpu = 0;
    players[3].isCpu = 0;
    players[0].characterId = 0;
    players[playerOne].characterId = one;
    players[2].characterId = two;
    players[3].characterId = three;
    players[0].characterVariant = six;
    players[1].characterVariant = six;
    players[2].characterVariant = six;
    players[3].characterVariant = six;
    players[0].snowboardTextureIndex = one;
    players[playerOne].snowboardTextureIndex = two;
    players[2].snowboardTextureIndex = three;
    players[3].snowboardTextureIndex = four;
    players[0].isActive = active;
    players[1].isActive = active;
    players[2].isActive = active;
    players[3].isActive = active;

    gRacePlayerCount = four;
    gRaceLapCount = two;
    gRacePlayerAttackStartTimer = 0x64;
    initCallbackTaskScheduler(1);

    players[0].unk15 = 0;
    players[0].soundDisabled = 0;
    players[0].replayInputSource = five0;
    players[1].unk15 = 0;
    players[1].soundDisabled = 0;
    players[1].replayInputSource = five1;
    players[2].unk15 = 0;
    players[2].soundDisabled = 0;
    players[2].replayInputSource = five2;
    players[3].unk15 = 0;
    players[3].soundDisabled = 0;
    players = gRacePlayers;
    players[3].replayInputSource = five3;

    if (players[0].replayInputSource == 5) {
        LOAD_ASSET(_24C8E0, 0x2B);
    }
    loadRaceCourseAssets();
    loadRaceCharacterAssets();
    gMenuFlowState = 0;
    resetRaceCameras();
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xB0, 0x140, 0xF0, 1.333333373f);
    gRacePlayerHudStatuses[0].active = playerOne;
    gFramebufferRenderInterval.value = 0;
    if ((six && six) != 0) {
        initRacePlayers();
    }
    initRaceHud();
    initRaceCourseSceneTasks();
    setCurrentGameTaskCallback(waitForTitleDemoRaceIntroStart, 0);
    gMenuFadeAlpha = 0xFF;
    gRaceRumbleEnabled = 0;
    gCurrentGameTask->callbackData0 = 0x4A1;
    gCurrentGameTask->callbackData1 = 0;
    gCurrentGameTask->callbackData2 = 0;
    gCurrentGameTask->callbackData3 = 0x14;
    gTitleDemoRaceIntroFadeStep = 0;
    *(s8 *)&gTitleDemoRaceIntroViewportHeight = 0;
}

void waitForTitleDemoRaceIntroStart(void) {
    gCurrentGameTask->callbackData3--;
    if (gCurrentGameTask->callbackData3 == 0) {
        configureViewport(0, 0xA0, 0x78, 0x120, 0, 0x140, 0xF0, 1.333333373f);
        gMenuFadeAlpha = 0;
        requestMusicSequenceBank(0);
        setCurrentGameTaskCallback(updateTitleDemoRaceIntro, 0);
    }
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void updateTitleDemoRaceIntro(void) {
    s32 fadeStep;
    union {
        s32 value;
        u8 padding[12];
    } previousPause;
    s32 cameraIndex;
    s32 fadeDelay;
    s32 nextViewportHeight;
    u32 i;
    u8 *destination;

    previousPause.value = gRaceUpdatePaused;
    configureViewport(0, 0xA0, 0x78, 0x120, (u8) gTitleDemoRaceIntroViewportHeight, 0x140, 0xF0, 1.333333373f);
    {
        u8 *viewportHeight = (u8 *)&gTitleDemoRaceIntroViewportHeight;

        if (*viewportHeight != 0xB0) {
            *viewportHeight += 0x10;
            if (*viewportHeight == 0xB0) {
                createCallbackTask((CallbackTaskCallback)updateTitleScreenStartPrompt, 0, 0x64);
            }
        }
    }
    do { fadeStep = gCurrentGameTask->callbackData1; if (fadeStep == gTitleDemoReplaySegmentFrames[gCurrentGameTask->callbackData2]) { destination = RACE_PLAYER_REPLAY_SNAPSHOT(0).bytes; i = 0; do { destination[i] = gTitleDemoReplayInputs[0][gCurrentGameTask->callbackData2].bytes[i]; i++; } while (i < sizeof(RacePlayerReplaySnapshot)); destination = RACE_PLAYER_REPLAY_SNAPSHOT(1).bytes; i = 0; do { destination[i] = gTitleDemoReplayInputs[1][gCurrentGameTask->callbackData2].bytes[i]; i++; if (1) { } } while (i < sizeof(RacePlayerReplaySnapshot)); destination = RACE_PLAYER_REPLAY_SNAPSHOT(2).bytes; i = 0; do { destination[i] = gTitleDemoReplayInputs[2][gCurrentGameTask->callbackData2].bytes[i]; i++; } while (i < sizeof(RacePlayerReplaySnapshot)); destination = RACE_PLAYER_REPLAY_SNAPSHOT(3).bytes; i = 0; while (i < sizeof(RacePlayerReplaySnapshot)) { destination[i] = gTitleDemoReplayInputs[3][gCurrentGameTask->callbackData2].bytes[i]; i++; } gCurrentGameTask->callbackData2++; fadeStep = gCurrentGameTask->callbackData1; } cameraIndex = gCurrentGameTask->callbackData3; if (fadeStep == gTitleDemoCameraModeFrames[cameraIndex]) { setRaceCameraMode(0, gTitleDemoCameraModes[cameraIndex]); gCurrentGameTask->callbackData3++; gRaceUpdatePaused = 1; } updateRacePlayers(); updateCallbackTasksWithMinPriority(0x63); updateRacePlayersPostUpdate(); updateRemainingCallbackTasks(); gRaceUpdatePaused = previousPause.value; } while (0);
    updateRaceCameras();
    gCurrentGameTask->callbackData1++;
    fadeDelay = gCurrentGameTask->callbackData0;
    if (fadeDelay != 0) {
        gCurrentGameTask->callbackData0 = fadeDelay - 1;
    }
    if (gPlayerInputPressed[0] & START_BUTTON) {
        if ((u8) gTitleDemoRaceIntroFadeStep == 0) {
            gTitleDemoRaceIntroFadeStep = 0x10;
        }
        requestMusicSequenceStop(0x20);
    }
    if (gCurrentGameTask->callbackData0 < 0x41) {
        if ((u8) gTitleDemoRaceIntroFadeStep == 0) {
            gTitleDemoRaceIntroFadeStep = 4;
        }
        requestMusicSequenceStop(0x82);
    }
    if ((u8) gTitleDemoRaceIntroFadeStep != 0) {
        gMenuFadeOverlayActive = 1;
        gMenuFadeAlpha += (u8)gTitleDemoRaceIntroFadeStep;
    }
    if (gMenuFadeAlpha >= 0xFF) {
        gMenuFadeAlpha = 0xFF;
        gFramebufferSwapHold = 1;
        setCurrentGameTaskCallback(finishTitleDemoRaceIntro, 0);
    }
}
// clang-format on

void finishTitleDemoRaceIntro(void) {
    if (gPendingFramebufferSwapCount == 2) {
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
