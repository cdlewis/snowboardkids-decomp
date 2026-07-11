#include "main_menu_demo_race_intro.h"
#include "sound_manager.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "race_scene_loader.h"
#include "game_task_scheduler.h"
#include "menu_screen_effects.h"
#include "race_camera.h"
#include "race_player_state.h"
#include "race_timer_ui.h"
#include "viewport_manager.h"

typedef struct {
    /* 0x00 */ u8 courseIndex;
    /* 0x01 */ u8 splitscreen;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 loadCourseAssets;
    /* 0x04 */ void *romStart;
    /* 0x08 */ void *romEnd;
    /* 0x0C */ u8 player0Character;
    /* 0x0D */ u8 player1Character;
    /* 0x0E */ u8 player2Character;
    /* 0x0F */ u8 player3Character;
} MainMenuDemoRaceIntroEntry;

typedef struct {
    /* 0x000 */ s16 playerIndex;
    /* 0x002 */ char pad2[2];
    /* 0x004 */ u8 unk4;
    /* 0x005 */ char pad5[0xB];
    /* 0x010 */ u8 characterId;
    /* 0x011 */ u8 unk11;
    /* 0x012 */ u8 unk12;
    /* 0x013 */ s8 isActive;
    /* 0x014 */ s8 soundDisabled;
    /* 0x015 */ s8 unk15;
    /* 0x016 */ s8 replayInputSource;
    /* 0x017 */ char pad17[0x5F5];
} RaceIntroPlayer;

typedef struct {
    /* 0x00 */ char pad0[0xAC];
    /* 0xAC */ s8 active;
    /* 0xAD */ char padAD[3];
} RaceIntroCamera;

extern RaceIntroTransitionState *gCurrentGameTask;
extern RaceIntroCamera D_801121E0[];
extern RaceIntroPlayer D_80121D80[];
extern MainMenuDemoRaceIntroEntry gMainMenuDemoRaceIntroEntries[];
extern u8 gMainMenuDemoRaceIntroLoadCourseAssetsFlags[];
extern s16 gMenuFadeAlpha;
extern s16 gRaceCourseIndex;
extern s16 gRaceLapCount;
extern s16 D_80121B5C;
extern u8 gMenuFadeOverlayActive;
extern u8 gMainMenuDemoRaceIntroIndex;
extern s8 gFramebufferSwapDelay;
extern u8 gRaceRumbleEnabled;
extern u8 gRaceSplitscreenMode;
extern u8 D_8011228C;
extern u8 D_80121B54;
extern u8 gPlayerCount;
extern u8 gRaceUpdatePaused;
extern u8 gRaceCameraModeChangeDisabled;
extern u8 D_80121B59;
extern u8 gTrainingCourseLesson;
extern u8 gMainMenuModeSelection;
extern u8 gRaceTypeSelection;
extern u8 D_80121B5F;
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
extern s32 gMenuFlowState;
extern u8 gFramebufferSwapHold;
extern u8 gPendingFramebufferSwapCount;
extern s32 gPlayerInputPressed;
extern void releaseMenuAssetHandles(void);
// initMainMenuDemoRaceIntro best match: 65.668% (nonmatchings/func_8003E600-731940616440357983/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_demo_race_intro/initMainMenuDemoRaceIntro.s")

#ifdef NON_MATCHING
void initMainMenuDemoRaceIntro(void) {
    RaceIntroPlayer *players;
    MainMenuDemoRaceIntroEntry *demoEntry;
    register s32 one;

    demoEntry = &gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex];
    gRaceCourseIndex = demoEntry->courseIndex;
    gRaceUpdatePaused = 0;
    gRaceCameraModeChangeDisabled = 0;
    D_80121B5F = 0;
    D_80121B59 = 0;
    gTrainingCourseLesson = 0;
    gMainMenuModeSelection = 0;
    gRaceSplitscreenMode = demoEntry->splitscreen;
    gRaceTypeSelection = demoEntry->unk2;
    func_80043184();
    players = D_80121D80;
    players[0].unk4 = 0;
    players[1].unk4 = 0;
    players[2].unk4 = 0;
    players[3].unk4 = 0;
    demoEntry = &gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex];
    players[0].unk11 = 6;
    players[1].unk11 = 6;
    players[2].unk11 = 6;
    players[3].unk11 = 6;
    players[0].unk12 = 1;
    players[1].unk12 = 2;
    players[2].unk12 = 3;
    players[3].unk12 = 4;
    players[0].characterId = demoEntry->player0Character;
    players[1].characterId = demoEntry->player1Character;
    players[2].characterId = demoEntry->player2Character;
    players[3].characterId = demoEntry->player3Character;
    one = 1;
    if (gRaceSplitscreenMode == 0) {
        players[0].isActive = one;
        players[1].isActive = one;
        players[2].isActive = one;
        players[3].isActive = one;
        D_80121B54 = 4;
        gPlayerCount = 4;
    } else {
        one = 1;
        players[0].isActive = one;
        players[1].isActive = 0;
        players[2].isActive = 0;
        players[3].isActive = 0;
        D_80121B54 = one;
        gPlayerCount = one;
    }
    gRaceLapCount = 2;
    D_80121B5C = 0x64;
    if (gRaceSplitscreenMode == 0) {
        initCallbackTaskScheduler(one);
    } else {
        initCallbackTaskScheduler(2);
    }
    D_80121D95 = 0;
    D_80121D94 = 0;
    D_80121D96 = 5;
    D_801223A1 = 0;
    D_801223A0 = 0;
    D_801223A2 = 5;
    D_801229AD = 0;
    D_801229AC = 0;
    D_801229AE = 5;
    D_80122FB9 = 0;
    D_80122FB8 = 0;
    D_80122FBA = 5;
    demoEntry = &gMainMenuDemoRaceIntroEntries[gMainMenuDemoRaceIntroIndex];
    loadCompressedRomAsset(demoEntry->romStart, demoEntry->romEnd, 0x2B);
    loadRaceCourseAssets();
    loadRaceCharacterAssets();
    gMenuFlowState = 0;
    resetRaceCameras();
    resetAllViewports();
    D_8011228C = one;
    gFramebufferSwapDelay = 0;
    func_8008BEB0();
    if (gMainMenuDemoRaceIntroLoadCourseAssetsFlags[gMainMenuDemoRaceIntroIndex * sizeof(MainMenuDemoRaceIntroEntry)] == 0) {
        gPlayerCount = one;
    }
    func_80078430();
    if (gPlayerCount == one) {
        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        D_8011228C = one;
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
    initRaceCourseEffects();
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
    func_8008C704();
    updateCallbackTasksWithMinPriority(0x63);
    func_80096E3C();
    updateRemainingCallbackTasks();
    updateRaceCameras();
    func_8007AA50();
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
    if ((gPlayerInputPressed & 0x1000) && (fadeStep == 0)) {
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
