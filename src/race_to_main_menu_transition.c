#include "common.h"

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

typedef void (*InputTaskCallback)(void);
typedef void (*EffectTaskCallback)(void *);

extern s32 func_80013F88(s16, s32, s32);
extern void func_80045914(void);
extern void func_8007105C(void);
extern void func_80071408(EffectTaskCallback, s32, s32);
extern void func_800720E4(s32);
extern void func_80099658(s32);
extern void func_8009954C(s32);
extern void func_8009956C(InputTaskCallback, s32);

void func_8000D340(void);
void func_8000D590(void);
void func_8000D690(void);
void func_8000D724(void);
extern void func_8003BBBC(void);
extern void func_80035184(void);
extern void func_800362E8(void);
extern void func_80036FB4(void);
extern void func_80039440(void);
extern void func_8003B264(void);

extern s8 D_800DEED4;
extern u16 D_8010B1A2;
extern MainMenuRaceSetupObject D_801121E0;
extern MainMenuRaceSetupObject *D_800EC9C4;
extern RaceToMainMenuTransitionState *D_801235B8;
extern u8 D_80123750;
extern u8 D_80123751;

#pragma GLOBAL_ASM("asm/nonmatchings/race_to_main_menu_transition/func_8000D340.s")

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
