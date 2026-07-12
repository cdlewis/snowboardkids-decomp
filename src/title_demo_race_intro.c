#include "race_demo_intro.h"
#include "title_demo_race_intro.h"
#include "sound_manager.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "race_scene_loader.h"
#include "game_task_scheduler.h"
#include "race_camera.h"
#include "race_player_state.h"
#include "race_timer_ui.h"
#include "viewport_manager.h"
#include "menu_screen_effects.h"

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
    u8 bytes[0x60C];
} RaceInputRecord;

extern RaceIntroTransitionState *gCurrentGameTask;
extern RaceIntroPlayer D_80121D80[];
extern s16 gMenuFadeAlpha;
extern s8 gMenuFadeOverlayActive;
extern f32 D_800E10C8;
extern s16 gTitleDemoReplaySegmentFrames[];
extern s16 gTitleDemoCameraModeFrames[];
extern u8 gTitleDemoCameraModes[];
extern u8 gTitleDemoReplayInputs[];
extern u8 gPendingFramebufferSwapCount;
extern u8 gRaceRumbleEnabled;
extern s8 gRaceSplitscreenMode;
extern s8 gFramebufferSwapDelay;
extern u8 gRacePlayerHudStatuses;
extern s8 gTitleDemoRaceIntroViewportHeight;
extern s8 gTitleDemoRaceIntroFadeStep;
extern s16 gRaceCourseIndex;
extern s16 gRaceLapCount;
extern s8 gRacePlayerCount;
extern u8 gPlayerCount;
extern u8 gRaceUpdatePaused;
extern u8 gRaceCameraModeChangeDisabled;
extern u8 gRaceDemoPlaybackEnabled;
extern u8 gTrainingCourseLesson;
extern u8 gMainMenuModeSelection;
extern s16 gRacePlayerAttackStartTimer;
extern u8 gRaceResultState;
extern RaceInputRecord D_8012238C[];
extern s8 D_80121D94;
extern s8 D_80121D95;
extern s8 D_80121D96;
extern RaceInputRecord D_80122998[];
extern s8 D_801223A0;
extern s8 D_801223A1;
extern s8 D_801223A2;
extern RaceInputRecord D_80122FA4[];
extern s8 D_801229AC;
extern s8 D_801229AD;
extern s8 D_801229AE;
extern s8 D_80122FB8;
extern s8 D_80122FB9;
extern s8 D_80122FBA;
extern u8 gFramebufferSwapHold;
extern s32 gMenuFlowState;
extern u8 D_24C8E0;
extern u8 D_24DBE0;
extern void updateCallbackTasksWithMinPriority(s32);
extern void updateRemainingCallbackTasks(void);
extern void requestMusicSequenceStop(s32);
extern void releaseMenuAssetHandles(void);

#define COURSE_REPLAY_OFFSET(course) ((((((((course) << 2) - (course)) << 5) + (course)) << 2) - (course)) << 2)

// initTitleDemoRaceIntro best match: 99.874% (nonmatchings/initTitleDemoRaceIntro-5635509610426229442/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/title_demo_race_intro/initTitleDemoRaceIntro.s")

#ifdef NON_MATCHING
void initTitleDemoRaceIntro(void) {
    RaceIntroPlayer *players;
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

    gRaceCourseIndex = 0;
    gRaceUpdatePaused = 0;
    gRaceCameraModeChangeDisabled = 0;
    gRaceResultState = 0;
    gRaceDemoPlaybackEnabled = one;
    gPlayerCount = four;
    gRaceSplitscreenMode = 0;
    gTrainingCourseLesson = 0;
    gMainMenuModeSelection = 0;

    players = D_80121D80;
    players[0].unk4 = 0;
    players[1].unk4 = 0;
    players[2].unk4 = 0;
    players[3].unk4 = 0;
    players[0].characterId = 0;
    players[1].characterId = one;
    players[2].characterId = two;
    players[3].characterId = three;
    players[0].unk11 = six;
    players[1].unk11 = six;
    players[2].unk11 = six;
    players[3].unk11 = six;
    players[0].unk12 = one;
    players[1].unk12 = two;
    players[2].unk12 = three;
    players[3].unk12 = four;
    players[0].isActive = active;
    players[1].isActive = active;
    players[2].isActive = active;
    players[3].isActive = active;

    gRacePlayerCount = four;
    gRaceLapCount = two;
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
    players = D_80121D80;
    D_80122FBA = five3;

    if (players[0].replayInputSource == 5) {
        loadCompressedRomAsset(&D_24C8E0, &D_24DBE0, 0x2B);
    }
    loadRaceCourseAssets();
    loadRaceCharacterAssets();
    gMenuFlowState = 0;
    resetRaceCameras();
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xB0, 0x140, 0xF0, D_800E10C0);
    gRacePlayerHudStatuses = 1;
    gFramebufferSwapDelay = 0;
    initRacePlayers();
    func_80078430();
    initRaceCourseEffects();
    setCurrentGameTaskCallback(waitForTitleDemoRaceIntroStart, 0);
    gMenuFadeAlpha = 0xFF;
    gRaceRumbleEnabled = 0;
    gCurrentGameTask->fadeDelay = 0x4A1;
    gCurrentGameTask->fadeStep = 0;
    gCurrentGameTask->courseSegment = 0;
    gCurrentGameTask->startDelay = 0x14;
    gTitleDemoRaceIntroFadeStep = 0;
    gTitleDemoRaceIntroViewportHeight = 0;
}
#endif

void waitForTitleDemoRaceIntroStart(void) {
    gCurrentGameTask->startDelay--;
    if (gCurrentGameTask->startDelay == 0) {
        configureViewport(0, 0xA0, 0x78, 0x120, 0, 0x140, 0xF0, 1.333333373f);
        gMenuFadeAlpha = 0;
        requestMusicSequenceBank(0);
        setCurrentGameTaskCallback(updateTitleDemoRaceIntro, 0);
    }
}

// updateTitleDemoRaceIntro best match: 87.815% (nonmatchings/updateTitleDemoRaceIntro-8207005055717715604/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/title_demo_race_intro/updateTitleDemoRaceIntro.s")

#ifdef NON_MATCHING
typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
} FourBytes;

void updateTitleDemoRaceIntro(void) {
    RaceIntroTransitionState *state;
    s32 prevOpen;
    s32 i;
    s32 fadeStep;
    s32 temp;
    s32 offset;
    u8 *dst;
    u8 *src;
    u8 value;
    FourBytes *fourDst;

    prevOpen = gRaceUpdatePaused;
    configureViewport(0, 0xA0, 0x78, 0x120, gTitleDemoRaceIntroViewportHeight, 0x140, 0xF0, D_800E10C8);

    temp = gTitleDemoRaceIntroViewportHeight;
    if (temp != 0xB0) {
        temp += 0x10;
        gTitleDemoRaceIntroViewportHeight = temp;
        if ((temp & 0xFF) == 0xB0) {
            createCallbackTask(updateTitleScreenStartPrompt, 0, 0x64);
        }
    }

    state = gCurrentGameTask;
    fadeStep = state->fadeStep;
    if (fadeStep == gTitleDemoReplaySegmentFrames[state->courseSegment]) {
        i = 0;
        do {
            dst = (u8 *)D_80121D80 + i;
            offset = COURSE_REPLAY_OFFSET(gCurrentGameTask->courseSegment);
            src = gTitleDemoReplayInputs + offset + i;
            value = *src;
            i++;
            *dst = value;
        } while ((u32)i < 0x60C);

        i = 0;
        do {
            dst = (u8 *)D_8012238C + i;
            offset = COURSE_REPLAY_OFFSET(gCurrentGameTask->courseSegment);
            src = gTitleDemoReplayInputs + offset + 0x60C0 + i;
            value = *src;
            i++;
            *dst = value;
        } while ((u32)i < 0x60C);

        i = 0;
        do {
            dst = (u8 *)D_80122998 + i;
            offset = COURSE_REPLAY_OFFSET(gCurrentGameTask->courseSegment);
            src = gTitleDemoReplayInputs + offset + 0xC180 + i;
            value = *src;
            i++;
            *dst = value;
        } while ((u32)i < 0x60C);

        i = 0;
copy_player3:
        fourDst = (FourBytes *)((u8 *)D_80122FA4 + i);
        offset = COURSE_REPLAY_OFFSET(gCurrentGameTask->courseSegment);
        fourDst->b0 = gTitleDemoReplayInputs[offset + 0x12240 + i];
        offset = COURSE_REPLAY_OFFSET(gCurrentGameTask->courseSegment);
        fourDst->b1 = gTitleDemoReplayInputs[offset + 0x12241 + i];
        offset = COURSE_REPLAY_OFFSET(gCurrentGameTask->courseSegment);
        fourDst->b2 = gTitleDemoReplayInputs[offset + 0x12242 + i];
        offset = COURSE_REPLAY_OFFSET(gCurrentGameTask->courseSegment);
        fourDst->b3 = gTitleDemoReplayInputs[offset + 0x12243 + i];
        i += 4;
        if (i != 0x60C) {
            goto copy_player3;
        }

        gCurrentGameTask->courseSegment++;
        state = gCurrentGameTask;
        fadeStep = state->fadeStep;
    }

    temp = state->startDelay;
    if (fadeStep == gTitleDemoCameraModeFrames[temp]) {
        setRaceCameraMode(0, gTitleDemoCameraModes[temp]);
        gCurrentGameTask->startDelay++;
        gRaceUpdatePaused = 1;
    }

    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    gRaceUpdatePaused = prevOpen;
    updateRaceCameras();

    gCurrentGameTask->fadeStep++;
    state = gCurrentGameTask;
    temp = state->fadeDelay;
    if (temp != 0) {
        state->fadeDelay = temp - 1;
    }

    if (gPlayerInputPressed & 0x1000) {
        if (gTitleDemoRaceIntroFadeStep == 0) {
            gTitleDemoRaceIntroFadeStep = 0x10;
        }
        requestMusicSequenceStop(0x20);
    }

    if (gCurrentGameTask->fadeDelay < 0x41) {
        if (gTitleDemoRaceIntroFadeStep == 0) {
            gTitleDemoRaceIntroFadeStep = 4;
        }
        requestMusicSequenceStop(0x82);
    }

    temp = gTitleDemoRaceIntroFadeStep;
    if (temp != 0) {
        gMenuFadeOverlayActive = 1;
        gMenuFadeAlpha += temp;
    }

    if (gMenuFadeAlpha >= 0xFF) {
        gMenuFadeAlpha = 0xFF;
        gFramebufferSwapHold = 1;
        setCurrentGameTaskCallback(finishTitleDemoRaceIntro, 0);
    }
}
#endif

void finishTitleDemoRaceIntro(void) {
    if (gPendingFramebufferSwapCount == 2) {
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
