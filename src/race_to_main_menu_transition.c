#include "common.h"
#include "game_audio.h"
#include "effect_task_scheduler.h"
#include "asset_decompression.h"
#include "game_boot.h"
#include "input_task_scheduler.h"
#include "main_menu_debug_ui.h"
#include "main_menu_effects.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 fade;
    /* 0x1C */ s32 timer;
} RaceToMainMenuTransitionState;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s16 yaw;
    /* 0x06 */ u8 pad6[0x1E];
    /* 0x24 */ s32 depth;
    /* 0x28 */ u8 pad28[0x4];
    /* 0x2C */ void (*update)(void);
} MainMenuRaceSetupObject;

typedef struct {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ s32 z;
} Vec3i;

typedef struct {
    /* 0x00 */ s8 value;
} MainMenuFlagByte;

typedef void (*EffectTaskCallback)(void *);

extern s32 func_80013F88(s16, s32, s32);
extern void func_8003DFB0(void);
extern void func_80041CC0(void);
extern s16 func_80042D58(s32);
extern s32 func_80043040(s16);
extern void func_80045914(void);
extern void func_8006D5CC(void);
extern void func_800704F0(void);
extern void func_80070860(s32, s32, s32, s32, s32, s32, s32, f32, s32, s32);

void func_8000D340(void);
void func_8000D590(void);
void func_8000D690(void);
void func_8000D724(void);
extern void func_80035184(void);
extern void func_800362E8(void);
extern void func_80036FB4(void);
extern void func_80039440(void);
extern void func_8003B264(void);

extern u8 D_14B450[];
extern u8 D_1502A0[];
extern u8 D_1EF530[];
extern u8 D_1F1A90[];
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_608560[];
extern u8 D_609AA0[];
extern u8 D_60ECB0[];
extern u8 D_60F1A0[];
extern s8 D_800DEED4;
extern s16 D_800DEF14;
extern f32 D_800E0A70;
extern s16 D_8010B1A0;
extern u16 D_8010B1A2;
extern s8 D_8010B1A4;
extern s8 D_8010B1A5;
extern s16 D_8010B1A6;
extern MainMenuFlagByte D_8010B1A8;
extern s8 D_8010B1A9;
extern s8 D_8010B1AA;
extern s8 D_8010B1AB;
extern s8 D_8010B1AC;
extern Vec3i D_8010B1B0;
extern s16 D_80112130[];
extern MainMenuRaceSetupObject D_801121E0;
extern MainMenuRaceSetupObject *D_800EC9C4;
extern RaceToMainMenuTransitionState *D_801235B8;
extern u8 D_80123750;
extern u8 D_80123751;

// func_8000D340 best match: 93.231% at nonmatchings/func_8000D340-1197934324348345530/base_6.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_to_main_menu_transition/func_8000D340.s")

#ifdef NON_MATCHING
void func_8000D340(void) {
    s32 sp34;
    s32 temp_v0;
    RaceToMainMenuTransitionState *state;

    func_8006D5CC();
    D_801121E0.update = func_8003DFB0;
    D_801121E0.depth = 0x5D24000;
    D_801121E0.yaw = 0xFC0;
    func_800704F0();
    func_80070860(0, 0xA0, 0x38, 0x120, 0x50, 0x140, 0xF0, D_800E0A70, 0x14, 0xAF0);
    D_800DEED4 = 0;
    D_8010B1B0.x = 0;
    D_8010B1B0.y = 0xFFB60000;
    D_8010B1B0.z = 0;
    D_8010B1A0 = 0;
    D_8010B1A2 = 0;
    D_8010B1A4 = 0;
    D_8010B1A6 = 0;
    D_8010B1A5 = 0;
    D_8010B1A8.value = 0;
    (&D_8010B1A8)[2].value = 0;
    (&D_8010B1A8)[3].value = 0;
    (&D_8010B1A8)[4].value = 0;
    (&D_8010B1A8)[1].value = 0;
    func_800437F0(D_608560, D_609AA0, 0x21);
    func_800437F0(D_593D10, D_598A70, 0x22);
    func_800437F0(D_609AA0, D_60ECB0, 0x26);
    func_800437F0(D_60ECB0, D_60F1A0, 0x27);
    temp_v0 = D_1502A0 - D_14B450;
    sp34 = temp_v0;
    D_80112130[0xC] = func_80042D58(temp_v0);
    func_80099C44(D_14B450, func_80043040(D_80112130[0xC]), sp34);
    func_800437F0(D_1EF530, D_1F1A90, 0xD);
    func_80041CC0();
    func_80070EC0(0);
    func_80071408(func_8003D908, 0, 0x64);
    func_80071408(func_8003DD64, 0, 0x64);
    func_80071408(func_8003DB1C, 0, 0x64);
    D_801235B8->fade = 0xFF;
    state = D_801235B8;
    D_800DEF14 = state->fade;
    D_801235B8->timer = 5;
    func_8009956C(func_8000D590, 0);
}
#endif

void func_8000D590(void) {
    if (D_801235B8->timer != 0) {
        D_801235B8->timer--;
    } else {
        if (D_801235B8->fade != 0) {
            D_801235B8->fade = func_80013F88(D_801235B8->fade, 0x10, 0);
        } else {
            func_8009956C(func_8000D690, 0);
            func_80071408((EffectTaskCallback) func_8003BBBC, 0, 0x64);
            func_80071408((EffectTaskCallback) func_80035184, 0, 0x64);
            func_80071408((EffectTaskCallback) func_800362E8, 0, 0x64);
            func_80071408((EffectTaskCallback) func_80036FB4, 0, 0x64);
            func_80071408((EffectTaskCallback) func_80039440, 0, 0x64);
            func_80071408((EffectTaskCallback) func_8003B264, 0, 0x64);
            func_800720E4(0xA);
        }
    }
    func_8007105C();
}

void func_8000D690(void) {
    if (D_8010B1A2 == 0x43) {
        D_801235B8->timer++;
        if (D_801235B8->timer == 0x12C) {
            D_801235B8->timer = 0;
            func_8009956C(func_8000D724, 0);
        }
    }
    func_8007105C();
    D_800EC9C4 = &D_801121E0;
    D_800EC9C4->update();
}

void func_8000D724(void) {
    if (D_801235B8->fade != 0xFF) {
        D_801235B8->fade = func_80013F88(D_801235B8->fade, 0x10, 1);
        if (D_801235B8->fade == 0xFF) {
            D_80123751 = 1;
        } else {
            func_8007105C();
        }
    } else {
        if (D_80123750 == 2) {
            func_80045914();
            D_80123751 = 0;
            D_800DEED4 = 0;
            func_80099658(2);
            func_8009954C(4);
        }
    }
}
