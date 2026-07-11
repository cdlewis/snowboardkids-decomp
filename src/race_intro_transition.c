#include "race_intro_transition.h"
#include "game_audio.h"
#include "effect_task_scheduler.h"
#include "asset_decompression.h"
#include "course_asset_loader.h"
#include "input_task_scheduler.h"
#include "main_menu_overlay_effects.h"
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
} RaceIntroCourseEntry;

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

extern RaceIntroTransitionState *D_801235B8;
extern RaceIntroCamera D_801121E0[];
extern RaceIntroPlayer D_80121D80[];
extern RaceIntroCourseEntry D_800BB834[];
extern u8 D_800BB837[];
extern s16 D_800DEF14;
extern s16 D_80121B50;
extern s16 D_80121B52;
extern s16 D_80121B5C;
extern u8 D_800DEF10;
extern u8 D_800BB830;
extern s8 D_800DEED4;
extern s8 D_800EC8B0;
extern u8 D_800EC9C2;
extern u8 D_8011228C;
extern u8 D_80121B54;
extern u8 D_80121B55;
extern u8 gRaceUpdatePaused;
extern s8 D_80121B58;
extern u8 D_80121B59;
extern u8 D_80121B5A;
extern u8 D_80121B5B;
extern u8 D_80121B5E;
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
extern s32 D_801235B4;
extern u8 D_80123751;
extern u8 D_80123750;
extern s32 D_80123778;
extern void func_80045914(void);
// func_8003E600 best match: 56.497% (nonmatchings/func_8003E600-7273315160691878794/base.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_intro_transition/func_8003E600.s")

#ifdef NON_MATCHING
void func_8003E600(void) {
    RaceIntroPlayer *players;
    RaceIntroCourseEntry *courseEntry;
    register s32 one;

    courseEntry = &D_800BB834[D_800BB830];
    D_80121B50 = courseEntry->courseIndex;
    gRaceUpdatePaused = 0;
    D_80121B58 = 0;
    D_80121B5F = 0;
    D_80121B59 = 0;
    D_80121B5A = 0;
    D_80121B5B = 0;
    D_800EC9C2 = courseEntry->splitscreen;
    D_80121B5E = courseEntry->unk2;
    func_80043184();
    players = D_80121D80;
    players[0].unk4 = 0;
    players[1].unk4 = 0;
    players[2].unk4 = 0;
    players[3].unk4 = 0;
    courseEntry = &D_800BB834[D_800BB830];
    players[0].unk11 = 6;
    players[1].unk11 = 6;
    players[2].unk11 = 6;
    players[3].unk11 = 6;
    players[0].unk12 = 1;
    players[1].unk12 = 2;
    players[2].unk12 = 3;
    players[3].unk12 = 4;
    players[0].characterId = courseEntry->player0Character;
    players[1].characterId = courseEntry->player1Character;
    players[2].characterId = courseEntry->player2Character;
    players[3].characterId = courseEntry->player3Character;
    one = 1;
    if (D_800EC9C2 == 0) {
        players[0].isActive = one;
        players[1].isActive = one;
        players[2].isActive = one;
        players[3].isActive = one;
        D_80121B54 = 4;
        D_80121B55 = 4;
    } else {
        players[0].isActive = one;
        players[1].isActive = 0;
        players[2].isActive = 0;
        players[3].isActive = 0;
        D_80121B54 = one;
        D_80121B55 = one;
    }
    D_80121B52 = 2;
    D_80121B5C = 0x64;
    if (D_800EC9C2 == 0) {
        func_80070EC0(one);
    } else {
        func_80070EC0(2);
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
    courseEntry = &D_800BB834[D_800BB830];
    func_800437F0(courseEntry->romStart, courseEntry->romEnd, 0x2B);
    func_80043950();
    func_800440F4();
    D_801235B4 = 0;
    func_8006D5CC();
    func_800704F0();
    D_8011228C = one;
    D_800DEED4 = 0;
    func_8008BEB0();
    if (D_800BB837[D_800BB830 * sizeof(RaceIntroCourseEntry)] == 0) {
        D_80121B55 = one;
    }
    func_80078430();
    if (D_80121B55 == one) {
        func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        D_8011228C = one;
        D_800DEED4 = 0;
    } else {
        func_8007066C(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        func_8007066C(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        func_8007066C(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        func_8007066C(3, 0xE9, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        D_801121E0[0].active = one;
        D_801121E0[1].active = one;
        D_801121E0[2].active = one;
        D_801121E0[3].active = one;
        D_800DEED4 = one;
    }
    if (D_800EC9C2 == 0) {
        D_80121B55 = 4;
    }
    func_80044294();
    func_8009956C(func_8003EA78, 0);
    D_800DEF14 = 0xFF;
    D_800EC8B0 = 0;
    D_801235B8->fadeDelay = 0x384;
    D_801235B8->fadeStep = 0;
    D_801235B8->courseSegment = 0;
    D_801235B8->startDelay = 0x14;
}
#endif

void func_8003EA78(void) {
    D_801235B8->startDelay--;
    if (D_801235B8->startDelay == 0) {
        func_800728E0();
        func_80071408(func_800540EC, 0, 0x64);
        func_8009956C(func_8003EAF0, 0);
    }
}

void func_8003EAF0(void) {
    RaceIntroTransitionState *state;
    s32 fadeStep;

    if (D_801235B8->fadeStep == 0) {
        D_800DEF14 -= 0x10;
        if (D_800DEF14 < 0) {
            D_800DEF14 = 0;
        }
    }
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    func_8007AA50();
    state = D_801235B8;
    if (state->fadeDelay != 0) {
        state->fadeDelay--;
        state = D_801235B8;
        fadeStep = state->fadeStep;
    } else {
        fadeStep = state->fadeStep;
        if (fadeStep == 0) {
            state->fadeStep = 4;
            func_80072114(0x78);
            state = D_801235B8;
            fadeStep = state->fadeStep;
        }
    }
    if ((D_80123778 & 0x1000) && (fadeStep == 0)) {
        state->fadeStep = 0x10;
        func_80072114(0x1E);
        state = D_801235B8;
        fadeStep = state->fadeStep;
    }
    if (fadeStep != 0) {
        D_800DEF10 = 1;
        D_800DEF14 += state->fadeStep;
        if (!(D_800DEF14 < 0xFF)) {
            D_800DEF14 = 0xFF;
            D_80123751 = 1;
            func_8009956C(func_8003EC6C, 0);
        }
    }
}

void func_8003EC6C(void) {
    if (D_80123750 == 2) {
        if ((D_800BB830 = D_800BB830 + 1) >= 5) {
            D_800BB830 = 0;
        }
        D_800EC8B0 = 0;
        D_80121B58 = 0;
        func_80045914();
        D_80123751 = 0;
        D_800DEED4 = 0;
        func_80072260();
        D_801235B4 = 0;
        func_80099658(3);
        func_8009954C(4);
    }
}
