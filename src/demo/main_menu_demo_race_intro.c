#include "assets.h"
#include "game/race/race_state.h"
#include "game/demo/main_menu_demo_race_intro.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/engine/system_runtime.h"
#include "game/race/scene/race_scene_setup.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/renderer/menu_screen_effects.h"
#include "game/race/camera/race_camera.h"
#include "game/race/player/race_player_update.h"
#include "game/race/ui/race_hud.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"

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

typedef struct {
    /* 0x00 */ char pad0[0xAC];
    /* 0xAC */ s8 active;
    /* 0xAD */ char padAD[3];
} RaceIntroCamera;

u8 gMainMenuDemoRaceIntroIndex = 0;
MainMenuDemoRaceIntroEntry gMainMenuDemoRaceIntroEntries[] = {
    { 9, 0, 0, 0, (void *)&_24DBE0_ROM_START, (void *)&_24DBE0_ROM_END, 0, 1, 2, 3 },
    { 0, 1, 0, 0, (void *)&_250260_ROM_START, (void *)&_250260_ROM_END, 4, 1, 2, 3 },
    { 1, 1, 1, 0, (void *)&_2521F0_ROM_START, (void *)&_2521F0_ROM_END, 2, 0, 1, 3 },
    { 7, 1, 2, 0, (void *)&_254380_ROM_START, (void *)&_254380_ROM_END, 1, 0, 1, 3 },
    { 3, 0, 0, 1, (void *)&_256420_ROM_START, (void *)&_256420_ROM_END, 3, 1, 4, 2 },
};

extern RaceIntroCamera D_801121E0[];
extern s16 gMenuFadeAlpha;
extern s16 gRaceLapCount;
extern s16 gRacePlayerAttackStartTimer;
extern s8 gRacePlayerCount;
extern u8 gRaceUpdatePaused;
extern u8 gTrainingCourseLesson;
extern u8 gMainMenuModeSelection;
extern s32 gMenuFlowState;
extern u8 gFramebufferSwapHold;
extern u8 gPendingFramebufferSwapCount;
extern f32 D_800E10A0;
extern f32 D_800E10A4;
extern f32 D_800E10A8;
extern f32 D_800E10AC;
extern f32 D_800E10B0;
extern void releaseMenuAssetHandles(void);

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
    gRacePlayers->unk12 = 1;
    gRacePlayers[1].unk12 = 2;
    gRacePlayers[2].unk12 = 3;
    gRacePlayers[3].unk12 = 4;
    if (gRaceSplitscreenMode == 0) { gRacePlayers->isActive = 1;
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
    loadCompressedRomAsset(gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex].romStart, gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex].romEnd, 0x2B);
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
        configureViewport(0, 0xA0, 0x78, 0x120U, 0xD0U, 0x140U, 0xF0U, D_800E10A0);
        gRacePlayerHudStatuses[0].active = 1;
        gFramebufferRenderInterval.value = 0;
    } else {
        configureViewport(0, 0x57, 0x43, 0x90U, 0x68U, 0xA0U, 0x78U, D_800E10A4);
        configureViewport(1, 0x57, 0xAD, 0x90U, 0x68U, 0xA0U, 0x78U, D_800E10A8);
        configureViewport(2, 0xE9, 0x43, 0x90U, 0x68U, 0xA0U, 0x78U, D_800E10AC);
        configureViewport(3, 0xE9, 0xAD, 0x90U, 0x68U, 0xA0U, 0x78U, D_800E10B0);
        D_801121E0[0].active = 1;
        D_801121E0[1].active = 1;
        D_801121E0[2].active = 1;
        D_801121E0[3].active = 1;
        if (1) {
            *((u8*)&gFramebufferRenderInterval.value) = 1;
        }
    }
    if (gRaceSplitscreenMode == 0) {
        gPlayerCount = 4;
    }
    initRaceCourseSceneTasks();
    setCurrentGameTaskCallback(waitForMainMenuDemoRaceIntroStart, 0);
    gMenuFadeAlpha = 0xFF;
    gRaceRumbleEnabled = 0;
    gCurrentGameTask->fadeDelay = 0x384;
    gCurrentGameTask->fadeStep = 0;
    gCurrentGameTask->courseSegment = 0;
    gCurrentGameTask->startDelay = 0x14;
}

void waitForMainMenuDemoRaceIntroStart(void) {
    gCurrentGameTask->startDelay--;
    if (gCurrentGameTask->startDelay == 0) {
        requestCourseMusicSequence();
        createCallbackTask((CallbackTaskCallback)updateTitleScreenStartPrompt, 0, 0x64);
        setCurrentGameTaskCallback(updateMainMenuDemoRaceIntro, 0);
    }
}

void updateMainMenuDemoRaceIntro(void) {
    GameTask *state;
    s32 fadeStep;

    if (gCurrentGameTask->fadeStep == 0) {
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
    if (state->fadeDelay != 0) {
        state->fadeDelay--;
        state = gCurrentGameTask;
        fadeStep = state->fadeStep;
    } else {
        fadeStep = state->fadeStep;
        if (fadeStep == 0) {
            state->fadeStep = 4;
            requestMusicSequenceStop(0x78);
            state = gCurrentGameTask;
            fadeStep = state->fadeStep;
        }
    }
    if ((gPlayerInputPressed[0] & START_BUTTON) && (fadeStep == 0)) {
        state->fadeStep = 0x10;
        requestMusicSequenceStop(0x1E);
        state = gCurrentGameTask;
        fadeStep = state->fadeStep;
    }
    if (fadeStep != 0) {
        gMenuFadeOverlayActive = 1;
        gMenuFadeAlpha += state->fadeStep;
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
