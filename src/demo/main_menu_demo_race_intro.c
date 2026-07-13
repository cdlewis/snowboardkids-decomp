#include "game/demo/main_menu_demo_race_intro.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
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

extern RaceIntroTransitionState *gCurrentGameTask;
extern RaceIntroCamera D_801121E0[];
extern MainMenuDemoRaceIntroEntry gMainMenuDemoRaceIntroEntries[];
extern u8 gMainMenuDemoRaceIntroLoadCourseAssetsFlags[];
extern s16 gMenuFadeAlpha;
extern s16 gRaceCourseIndex;
extern s16 gRaceLapCount;
extern s16 gRacePlayerAttackStartTimer;
extern s8 gRacePlayerCount;
extern u8 gMenuFadeOverlayActive;
extern u8 gMainMenuDemoRaceIntroIndex;
extern s8 gFramebufferSwapDelay;
extern u8 gRaceRumbleEnabled;
extern u8 gRaceSplitscreenMode;
extern u8 gRacePlayerHudStatuses;
extern u8 gPlayerCount;
extern u8 gRaceUpdatePaused;
extern u8 gRaceCameraModeChangeDisabled;
extern u8 gRaceDemoPlaybackEnabled;
extern u8 gTrainingCourseLesson;
extern u8 gMainMenuModeSelection;
extern u8 gRaceTypeSelection;
extern u8 gRaceResultState;
extern s32 gMenuFlowState;
extern u8 gFramebufferSwapHold;
extern u8 gPendingFramebufferSwapCount;
extern s32 gPlayerInputPressed;
extern void releaseMenuAssetHandles(void);
// initMainMenuDemoRaceIntro best match: 68.393% (nonmatchings/initMainMenuDemoRaceIntro-8331816093655448999/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/demo/main_menu_demo_race_intro/initMainMenuDemoRaceIntro.s")

#ifdef NON_MATCHING
void initMainMenuDemoRaceIntro(void) {
    RacePlayer *players;
    MainMenuDemoRaceIntroEntry *demoEntry;
    volatile register s32 one;
    s32 character0;
    s32 character1;
    s32 character2;
    s32 character3;
    s32 splitscreenMode;

    demoEntry = &gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex];
    gRaceCourseIndex = demoEntry->courseIndex;
    gRaceUpdatePaused = 0;
    gRaceCameraModeChangeDisabled = 0;
    gRaceResultState = 0;
    gRaceDemoPlaybackEnabled = 0;
    gTrainingCourseLesson = 0;
    gMainMenuModeSelection = 0;
    gRaceSplitscreenMode = demoEntry->splitscreen;
    gRaceTypeSelection = demoEntry->raceType;
    resetGameplayRng();
    players = gRacePlayers;
    players[0].unk4 = 0;
    players[1].unk4 = 0;
    players[2].unk4 = 0;
    players[3].unk4 = 0;
    demoEntry = &gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex];
    character0 = demoEntry->player0Character;
    character1 = demoEntry->player1Character;
    character2 = demoEntry->player2Character;
    character3 = demoEntry->player3Character;
    players[0].unk11 = 6;
    players[1].unk11 = 6;
    players[2].unk11 = 6;
    players[3].unk11 = 6;
    players[0].unk12 = 1;
    players[1].unk12 = 2;
    players[2].unk12 = 3;
    players[3].unk12 = 4;
    players[0].characterId = character0;
    players[1].characterId = character1;
    players[2].characterId = character2;
    players[3].characterId = character3;
    splitscreenMode = gRaceSplitscreenMode;
    one = 1;
    if (splitscreenMode == 0) {
        players[0].isActive = one;
        players[1].isActive = one;
        players[2].isActive = one;
        players[3].isActive = one;
        gRacePlayerCount = 4;
        gPlayerCount = 4;
    } else {
        one = 1;
        players[0].isActive = one;
        players[1].isActive = 0;
        players[2].isActive = 0;
        players[3].isActive = 0;
        gRacePlayerCount = one;
        gPlayerCount = one;
    }
    gRaceLapCount = 2;
    gRacePlayerAttackStartTimer = 0x64;
    if (splitscreenMode == 0) {
        initCallbackTaskScheduler(one);
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
    demoEntry = &gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex];
    loadCompressedRomAsset(demoEntry->romStart, demoEntry->romEnd, 0x2B);
    loadRaceCourseAssets();
    loadRaceCharacterAssets();
    gMenuFlowState = 0;
    resetRaceCameras();
    resetAllViewports();
    gRacePlayerHudStatuses = one;
    gFramebufferSwapDelay = 0;
    initRacePlayers();
    if (gMainMenuDemoRaceIntroLoadCourseAssetsFlags[gMainMenuDemoRaceIntroIndex * sizeof(MainMenuDemoRaceIntroEntry)] == 0) {
        gPlayerCount = one;
    }
    initRaceHud();
    if (gPlayerCount == one) {
        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        gRacePlayerHudStatuses = one;
        gFramebufferSwapDelay = 0;
    } else {
        configureViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        configureViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        configureViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        configureViewport(3, 0xE9, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        D_801121E0[0].active = one;
        D_801121E0[1].active = one;
        D_801121E0[2].active = one;
        D_801121E0[3].active = one;
        gFramebufferSwapDelay = one;
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
#endif

void waitForMainMenuDemoRaceIntroStart(void) {
    gCurrentGameTask->startDelay--;
    if (gCurrentGameTask->startDelay == 0) {
        requestCourseMusicSequence();
        createCallbackTask(updateTitleScreenStartPrompt, 0, 0x64);
        setCurrentGameTaskCallback(updateMainMenuDemoRaceIntro, 0);
    }
}

void updateMainMenuDemoRaceIntro(void) {
    RaceIntroTransitionState *state;
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
    if ((gPlayerInputPressed & START_BUTTON) && (fadeStep == 0)) {
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
        gFramebufferSwapDelay = 0;
        stopSoundEffects();
        gMenuFlowState = 0;
        resumeGameTask(3);
        removeGameTask(4);
    }
}
