#include "race_intro_transition.h"
#include "game_audio.h"
#include "effect_task_scheduler.h"

extern RaceIntroTransitionState *D_801235B8;
extern s16 D_800DEF14;
extern u8 D_800DEF10;
extern u8 D_800BB830;
extern s8 D_800DEED4;
extern s8 D_800EC8B0;
extern s8 D_80121B58;
extern s32 D_801235B4;
extern u8 D_80123751;
extern u8 D_80123750;
extern s32 D_80123778;
extern void func_80045914(void);
extern void func_8009954C(s32);
extern void func_8009956C(TaskCallback, s32);
extern void func_80099658(s32);
extern void func_800540EC(void *);
extern void func_8008C704(void);
extern void func_80096E3C(void);
extern void func_8006D700(void);
extern void func_8007AA50(void);
extern void func_8003EC6C(void);
void func_8003EAF0(void);

#pragma GLOBAL_ASM("asm/nonmatchings/race_intro_transition/func_8003E600.s")

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
