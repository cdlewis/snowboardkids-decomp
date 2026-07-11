#include "race_intro_transition.h"
#include "race_intro_transition_2.h"
#include "game_audio.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "race_scene_loader.h"
#include "input_task_scheduler.h"
#include "race_camera.h"
#include "race_player_state.h"
#include "race_timer_ui.h"
#include "viewport_manager.h"

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

extern RaceIntroTransitionState *gCurrentInputTask;
extern RaceIntroPlayer D_80121D80[];
extern s16 gMenuFadeAlpha;
extern s8 D_800DEF10;
extern f32 D_800E10C8;
extern s16 D_800BB890[];
extern s16 D_800BB8B0[];
extern u8 D_800BB8DC[];
extern u8 D_800BB8F4[];
extern u8 gPendingFramebufferSwapCount;
extern u8 gRaceRumbleEnabled;
extern s8 D_800EC9C2;
extern s8 gFramebufferSwapDelay;
extern u8 D_8011228C;
extern s8 D_8010B1E0;
extern s8 D_8010B1E1;
extern s16 D_80121B50;
extern s16 D_80121B52;
extern s8 gRacePlayerCount;
extern u8 D_80121B55;
extern u8 gRaceUpdatePaused;
extern u8 D_80121B58;
extern u8 D_80121B59;
extern u8 D_80121B5A;
extern u8 D_80121B5B;
extern s16 D_80121B5C;
extern u8 D_80121B5F;
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
extern s32 D_801235B4;
extern u8 D_24C8E0;
extern u8 D_24DBE0;
extern void updateCallbackTasksWithMinPriority(s32);
extern void updateRemainingCallbackTasks(void);
extern void func_80072114(s32);
extern void releaseMenuAssetHandles(void);
extern void func_800540EC(void *);

#define COURSE_REPLAY_OFFSET(course) ((((((((course) << 2) - (course)) << 5) + (course)) << 2) - (course)) << 2)

// func_8003ED00 best match: 99.874% (nonmatchings/func_8003ED00-5635509610426229442/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_intro_transition_2/func_8003ED00.s")

#ifdef NON_MATCHING
void func_8003ED00(void) {
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

    D_80121B50 = 0;
    gRaceUpdatePaused = 0;
    D_80121B58 = 0;
    D_80121B5F = 0;
    D_80121B59 = one;
    D_80121B55 = four;
    D_800EC9C2 = 0;
    D_80121B5A = 0;
    D_80121B5B = 0;

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
    D_80121B52 = two;
    D_80121B5C = 0x64;
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
    D_801235B4 = 0;
    func_8006D5CC();
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xB0, 0x140, 0xF0, D_800E10C0);
    D_8011228C = 1;
    gFramebufferSwapDelay = 0;
    func_8008BEB0();
    func_80078430();
    initRaceCourseEffects();
    setCurrentInputTaskCallback(func_8003EF7C, 0);
    gMenuFadeAlpha = 0xFF;
    gRaceRumbleEnabled = 0;
    gCurrentInputTask->fadeDelay = 0x4A1;
    gCurrentInputTask->fadeStep = 0;
    gCurrentInputTask->courseSegment = 0;
    gCurrentInputTask->startDelay = 0x14;
    D_8010B1E1 = 0;
    D_8010B1E0 = 0;
}
#endif

void func_8003EF7C(void) {
    gCurrentInputTask->startDelay--;
    if (gCurrentInputTask->startDelay == 0) {
        configureViewport(0, 0xA0, 0x78, 0x120, 0, 0x140, 0xF0, 1.333333373f);
        gMenuFadeAlpha = 0;
        func_800720E4(0);
        setCurrentInputTaskCallback(func_8003F00C, 0);
    }
}

// func_8003F00C best match: 87.815% (nonmatchings/func_8003F00C-8207005055717715604/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_intro_transition_2/func_8003F00C.s")

#ifdef NON_MATCHING
typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
} FourBytes;

void func_8003F00C(void) {
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
    configureViewport(0, 0xA0, 0x78, 0x120, D_8010B1E0, 0x140, 0xF0, D_800E10C8);

    temp = D_8010B1E0;
    if (temp != 0xB0) {
        temp += 0x10;
        D_8010B1E0 = temp;
        if ((temp & 0xFF) == 0xB0) {
            createCallbackTask(func_800540EC, 0, 0x64);
        }
    }

    state = gCurrentInputTask;
    fadeStep = state->fadeStep;
    if (fadeStep == D_800BB890[state->courseSegment]) {
        i = 0;
        do {
            dst = (u8 *)D_80121D80 + i;
            offset = COURSE_REPLAY_OFFSET(gCurrentInputTask->courseSegment);
            src = D_800BB8F4 + offset + i;
            value = *src;
            i++;
            *dst = value;
        } while ((u32)i < 0x60C);

        i = 0;
        do {
            dst = (u8 *)D_8012238C + i;
            offset = COURSE_REPLAY_OFFSET(gCurrentInputTask->courseSegment);
            src = D_800BB8F4 + offset + 0x60C0 + i;
            value = *src;
            i++;
            *dst = value;
        } while ((u32)i < 0x60C);

        i = 0;
        do {
            dst = (u8 *)D_80122998 + i;
            offset = COURSE_REPLAY_OFFSET(gCurrentInputTask->courseSegment);
            src = D_800BB8F4 + offset + 0xC180 + i;
            value = *src;
            i++;
            *dst = value;
        } while ((u32)i < 0x60C);

        i = 0;
copy_player3:
        fourDst = (FourBytes *)((u8 *)D_80122FA4 + i);
        offset = COURSE_REPLAY_OFFSET(gCurrentInputTask->courseSegment);
        fourDst->b0 = D_800BB8F4[offset + 0x12240 + i];
        offset = COURSE_REPLAY_OFFSET(gCurrentInputTask->courseSegment);
        fourDst->b1 = D_800BB8F4[offset + 0x12241 + i];
        offset = COURSE_REPLAY_OFFSET(gCurrentInputTask->courseSegment);
        fourDst->b2 = D_800BB8F4[offset + 0x12242 + i];
        offset = COURSE_REPLAY_OFFSET(gCurrentInputTask->courseSegment);
        fourDst->b3 = D_800BB8F4[offset + 0x12243 + i];
        i += 4;
        if (i != 0x60C) {
            goto copy_player3;
        }

        gCurrentInputTask->courseSegment++;
        state = gCurrentInputTask;
        fadeStep = state->fadeStep;
    }

    temp = state->startDelay;
    if (fadeStep == D_800BB8B0[temp]) {
        func_8006D520(0, D_800BB8DC[temp]);
        gCurrentInputTask->startDelay++;
        gRaceUpdatePaused = 1;
    }

    func_8008C704();
    updateCallbackTasksWithMinPriority(0x63);
    func_80096E3C();
    updateRemainingCallbackTasks();
    gRaceUpdatePaused = prevOpen;
    func_8006D700();

    gCurrentInputTask->fadeStep++;
    state = gCurrentInputTask;
    temp = state->fadeDelay;
    if (temp != 0) {
        state->fadeDelay = temp - 1;
    }

    if (gPlayerInputPressed & 0x1000) {
        if (D_8010B1E1 == 0) {
            D_8010B1E1 = 0x10;
        }
        func_80072114(0x20);
    }

    if (gCurrentInputTask->fadeDelay < 0x41) {
        if (D_8010B1E1 == 0) {
            D_8010B1E1 = 4;
        }
        func_80072114(0x82);
    }

    temp = D_8010B1E1;
    if (temp != 0) {
        D_800DEF10 = 1;
        gMenuFadeAlpha += temp;
    }

    if (gMenuFadeAlpha >= 0xFF) {
        gMenuFadeAlpha = 0xFF;
        gFramebufferSwapHold = 1;
        setCurrentInputTaskCallback(func_8003F4B4, 0);
    }
}
#endif

void func_8003F4B4(void) {
    if (gPendingFramebufferSwapCount == 2) {
        gRaceRumbleEnabled = 0;
        D_80121B58 = 0;
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        func_80072260();
        D_801235B4 = 0;
        resumeInputTask(3);
        removeInputTask(4);
    }
}
