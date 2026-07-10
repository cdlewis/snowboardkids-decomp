#include "common.h"
#include "asset_decompression.h"
#include "effect_task_scheduler.h"
#include "game_audio.h"
#include "game_setup_menu.h"
#include "input_task_scheduler.h"

// func_80003140 best match: 98.611% (nonmatchings/func_80003140-5743805732885129799/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/game_setup_menu/func_80003140.s")

#ifdef NON_MATCHING
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 fade;
    /* 0x1C */ s32 timer;
} GameSetupMenuState;

typedef struct {
    /* 0x0 */ u8 state;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ u8 unk4;
} GameSetupMenuSubState;

extern void func_80014C7C(EffectTask *task);
extern void func_800704F0(void);
extern void func_8007066C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, f32 arg7);
extern void func_80072138();
extern f32 D_800E0900;
extern u8 D_800E29C0;
extern s8 D_800DEED4;
extern s16 D_800DEF14;
extern s8 D_800EC8B0;
extern s8 D_800EC8B4;
extern s8 D_800EC8B5;
extern s8 D_800EC8B6;
extern s8 D_800EC8B7;
extern u8 D_800EC9C1;
extern s8 D_800EC9E5;
extern s8 D_800EC9E6;
extern EffectTask *D_8010ADDC;
extern u16 D_8010ADF0;
extern s8 D_8010ADF9;
extern GameSetupMenuSubState D_8010AE00;
extern s16 D_8010AE06;
extern s16 D_8010AE08;
extern s16 D_8010AE0A;
extern s16 D_8010AE0C;
extern s8 D_8010AE0E;
extern s8 D_8010AE0F;
extern s8 D_8010AE10;
extern s8 D_8010AE11;
extern s16 D_8010AE12;
extern s16 D_8010AE14;
extern s16 D_8010AE16;
extern s16 D_8010AE18;
extern s16 D_80121B50;
extern u8 D_80121B55;
extern s8 D_80121D86;
extern s8 D_80122392;
extern s8 D_8012299E;
extern s8 D_80122FAA;
extern GameSetupMenuState *D_801235B8;
extern s32 D_801235B4;
extern s32 D_80123758;
extern s32 D_80123778;
extern u8 D_245A80;
extern u8 D_24C8E0;
extern u8 D_593D10;
extern u8 D_598A70;
extern u8 D_59AAA0;
extern u8 D_59DFE0;
extern u8 D_60F1A0;
extern u8 D_60F990;

void func_80003140(void) {
    func_800720E4(1);
    func_800704F0();
    func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, D_800E0900);

    D_800DEED4 = 0;
    D_801235B8->fade = 0;
    D_801235B8->timer = 0;
    D_800EC9C1 = 0;
    D_800EC9E5 = 1;
    D_800EC9E6 = 0;
    D_8010ADF9 = 0;
    D_800EC8B0 = 0;
    D_80121B50 = 9;
    D_801235B4 = 0;
    D_8010ADF0 = 0;
    D_80121B55 = 1;
    D_800DEF14 = D_801235B8->fade;

    func_800437F0(&D_59AAA0, &D_59DFE0, 0x21);
    func_800437F0(&D_245A80, &D_24C8E0, 0x1F);
    func_800437F0(&D_593D10, &D_598A70, 0x22);
    func_800437F0(&D_598A70, &D_59AAA0, 0x23);
    func_800437F0(&D_60F1A0, &D_60F990, 0x29);

    func_80070EC0(0);
    D_8010ADDC = func_80071408(func_80014C7C, 0, 0x64);

    D_8010AE00.state = 0;
    D_8010AE00.unk1 = 0;
    D_8010AE00.unk2 = 0;
    D_8010AE00.unk4 = 0;
    D_800EC8B4 = 0;
    D_8010AE06 = 0;
    D_8010AE0E = 0;
    D_8010AE12 = 0;
    D_80121D86 = 0;
    D_800EC8B5 = 0;
    D_8010AE08 = 0;
    D_8010AE0F = 0;
    D_8010AE14 = 0;
    D_80122392 = 0;
    D_800EC8B6 = 0;
    D_8010AE0A = 0;
    D_8010AE10 = 0;
    D_8010AE16 = 0;
    D_8012299E = 0;
    D_800EC8B7 = 0;
    D_8010AE0C = 0;
    D_8010AE11 = 0;
    D_8010AE18 = 0;
    D_80122FAA = 0;

    func_8009956C(func_8000337C, 0);
}
#endif

// func_8000337C best match: 88.068% (nonmatchings/func_8000337C-6276316234415602851/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/game_setup_menu/func_8000337C.s")

#ifdef NON_MATCHING

void func_8000337C(void) {
    s32 sp18;
    s32 temp_a3;
    s32 temp_v0;
    register u16 *var_a2;
    u16 temp_t6;
    u16 temp_v0_2;
    u8 temp_v1;
    s32 one;

    one = 1;
    if ((D_8010AE00.state == one) && ((D_80123778 & 0x8000) || (D_80123778 & 0x1000))) {
        func_80072138((s16) (one & 0xFFFFFFFF), 0x32);
        D_8010AE00.state = 2;
        D_8010AE00.unk1 = 0;
        one = 1;
    }
    if (D_801235B8->timer == one) {
        temp_v1 = D_800EC9C1;
        if (temp_v1 == 0) {
            temp_a3 = D_80123778;
            temp_v0 = D_80123758 & 0x10800;
            if ((temp_v0 == 0) && !(D_80123758 & 0x20400)) {
                D_8010ADF0 = 0;
            }
            var_a2 = &D_8010ADF0;
            if ((temp_a3 & 0x10800) || ((temp_v0 != 0) && (D_8010ADF0 >= 9) && ((D_8010ADF0 % 3) == 0))) {
                if (D_8010ADF0 == 0) {
                    D_8010ADF0 += 1;
                }
                if (D_80121B55 != one) {
                    D_80121B55 -= 1;
                    sp18 = temp_a3;
                    func_80072138(0x19, 0x32, &D_8010ADF0, temp_a3);
                    goto block_25;
                }
            } else if ((temp_a3 & 0x20400) || ((D_80123758 & 0x20400) && (D_8010ADF0 >= 9) && ((D_8010ADF0 % 3) == 0))) {
                if (D_8010ADF0 == 0) {
                    D_8010ADF0 += 1;
                }
                if (D_800E29C0 != D_80121B55) {
                    D_80121B55 += 1;
                    sp18 = temp_a3;
                    func_80072138(0x19, 0x32, &D_8010ADF0, temp_a3);
block_25:
                    var_a2 = &D_8010ADF0;
                }
            }
            temp_v0_2 = *var_a2;
            temp_t6 = temp_v0_2 + 1;
            if (temp_v0_2 != 0) {
                *var_a2 = temp_t6;
                if (temp_t6 == 0xFFFF) {
                    *var_a2 = 0xA;
                }
            }
            if ((temp_a3 & 0x8000) || (temp_a3 & 0x1000)) {
                D_800EC9C1 = 1;
                func_80072138(0x18, 0x32, var_a2, temp_a3);
            }
        } else if (temp_v1 < 0x13) {
            D_800EC9C1 = temp_v1 + 1;
        }
    }
    if (D_8010AE00.state == 5) {
        func_8009956C(func_800035F8, 0);
        D_801235B8->fade = 0;
        D_800EC9C1 = 0;
    }
    func_8007105C();
}
#endif

// func_800035F8 best match: 90.019% (nonmatchings/func_800035F8-2225551288923588688/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/game_setup_menu/func_800035F8.s")

#ifdef NON_MATCHING
extern s16 D_800B31B8;
extern s16 D_800B31BA;
extern s16 D_800B31BC;
extern s16 D_800B31BE;
extern s16 D_800B31C0;
extern s16 D_800B31C2;
extern s16 D_800B31C4;
extern s16 D_800B31C6;
extern s16 D_800EC9C8;
extern s16 D_800EC9D0;
extern u8 D_800EC9D8;
extern u8 D_800EC9E0;
extern u8 D_800EC9E4;
extern s32 D_8010ADE0;
extern s32 D_8010ADE4;
extern s32 D_8010ADE8;
extern s16 D_8010AE38;
extern s16 D_8010AE3A;
extern s16 D_8010AE3C;
extern s16 D_8010AE3E;
extern s16 D_8010AE40;
extern s16 D_8010AE42;
extern s16 D_8010AE44;
extern s16 D_8010AE46;
extern u8 D_80121D80;

void func_800035F8(void) {
    s16 *var_v1;
    s16 *var_a0;
    u8 *var_a1;
    u8 *var_v0;
    u8 *player;
    s32 i;
    u8 count;

    var_v0 = &D_800EC9E0;
    var_a1 = &D_800EC9D8;
    var_a0 = &D_800EC9D0;
    var_v1 = &D_800EC9C8;
    do {
        var_v0++;
        var_v1++;
        var_a0++;
        var_a1++;
        var_v1[-1] = 0;
        var_a0[-1] = 0;
        var_a1[-1] = 0;
        var_v0[-1] = 0;
    } while (var_v0 < &D_800EC9E4);

    count = D_800E29C0;
    D_800EC9E4 = 0;
    i = 0;
    if (count > 0) {
        player = &D_80121D80;
        do {
            player += 0x60C;
            player[-0x604] = 0;
        } while (player < (&D_80121D80 + (count * 0x60C)));
        i = 0;
    }

    do {
        func_800045D8(i);
        i++;
    } while (i < 4);

    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    D_8010ADE8 = 0;
    D_800EC9C1 = 0;
    D_801235B4 = 0;
    D_800EC8B0 = 0;

    D_8010AE38 = D_800B31B8;
    D_8010AE40 = D_800B31BA;
    D_8010AE3A = D_800B31BC;
    D_8010AE42 = D_800B31BE;
    D_8010AE3C = D_800B31C0;
    D_8010AE44 = D_800B31C2;
    D_8010AE3E = D_800B31C4;
    D_8010AE46 = D_800B31C6;

    func_8009956C(func_80003798, 0);
    func_8007105C();
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/game_setup_menu/func_80003798.s")

void __dummy(void) {
}

// func_80004164 best match: 87.420% (nonmatchings/func_80004164-3236181511606361864/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/game_setup_menu/func_80004164.s")

#ifdef NON_MATCHING
typedef struct {
    /* 0x0 */ s16 pad0;
    /* 0x2 */ u8 state;
    /* 0x3 */ u8 pad3;
    /* 0x4 */ u16 targetScale;
    /* 0x6 */ u16 timer;
    /* 0x8 */ u8 selectedOption;
    /* 0x9 */ u8 confirmSelection;
} ControllerPakConfirmTransition;

extern ControllerPakConfirmTransition D_8010ADD0;
extern u8 D_8010ADD9;
extern void func_80000A40(u16 arg0);
extern void func_80004960(void);

void func_80004164(void) {
    s32 connectedCount;
    s32 i;
    u8 state;

    state = D_8010ADD0.state;
    switch (state) {
        case 1:
            if ((D_80123778 & 0x8000) || (D_80123778 & 0x1000)) {
                func_80072138(1, 0x32);
                D_8010ADD0.state = 2;
                D_8010ADD0.targetScale = 1;
                state = 2;
            }
            break;
        case 2:
            D_8010ADFA = 0;
            i = 0;
            if ((s32)D_80121B55 > 0) {
                do {
                    func_80000A40(i);
                    if (((&D_800EC898)[i] != 1) && ((&D_800EC898)[i] != 0xB) && ((&D_800EC898)[i] != 4)) {
                        (&D_800EC8B4)[i] = 1;
                        D_8010ADFA |= 1 << i;
                    } else {
                        (&D_800EC8B4)[i] = 0;
                    }
                    i++;
                } while (i < (s32)D_80121B55);
            }
            D_8010ADD0.state = 7;
            D_8010ADD0.timer = 0x11;
            state = 7;
            break;
        case 3:
            if ((D_80123778 & 0x8000) || (D_80123778 & 0x1000)) {
                func_80072138(1, 0x32);
                D_8010ADD0.state = 4;
                state = 4;
            }
            break;
        case 6:
            connectedCount = 0;
            D_8010ADFA = 0;
            i = 0;
            if ((s32)D_80121B55 > 0) {
                do {
                    func_80000A40(i);
                    if (((&D_800EC898)[i] != 1) && ((&D_800EC898)[i] != 0xB) && ((&D_800EC898)[i] != 4)) {
                        D_8010ADFA |= 1 << i;
                        (&D_800EC8B4)[i] = 1;
                        connectedCount++;
                    } else {
                        (&D_800EC8B4)[i] = 0;
                    }
                    i++;
                } while (i < (s32)D_80121B55);
            }
            if (connectedCount == D_80121B55) {
                D_8010ADD0.selectedOption = 1;
                D_8010ADD0.targetScale = 2;
            } else {
                D_8010ADD0.selectedOption = 0;
                D_8010ADD0.targetScale = 0;
            }
            D_8010ADD0.state = 0;
            state = 0;
            break;
        case 7:
            D_8010ADD0.timer--;
            if (D_8010ADD0.timer == 0) {
                D_8010ADD0.state = 8;
                D_8010ADD0.targetScale = 2;
                state = 8;
            }
            break;
        case 8:
            if ((D_80123778 & 0x8000) || (D_80123778 & 0x1000)) {
                func_80072138(1, 0x32);
                D_8010ADD0.state = 9;
                D_8010ADD0.targetScale = 3;
                D_8010ADD0.confirmSelection = 1;
                state = 9;
            }
            break;
        case 9:
            if ((D_80123778 & 0x10800) && (D_8010ADD0.confirmSelection != 0)) {
                D_8010ADD0.confirmSelection--;
                func_80072138(0x19, 0x32);
            } else if (D_80123778 & 0x20400) {
                if (D_8010ADD0.confirmSelection != 1) {
                    D_8010ADD0.confirmSelection++;
                    func_80072138(0x19, 0x32);
                }
            }
            if ((D_80123778 & 0x8000) || (D_80123778 & 0x1000)) {
                func_80072138(1, 0x32);
                if (D_8010ADD9 == 1) {
                    D_8010ADD0.state = 1;
                    D_8010ADD0.targetScale = 0;
                } else {
                    connectedCount = 0;
                    i = 0;
                    if ((s32)D_80121B55 > 0) {
                        do {
                            if ((&D_800EC8B4)[i] == 1) {
                                connectedCount++;
                            }
                            i++;
                        } while (i < (s32)D_80121B55);
                    }
                    if (connectedCount > 0) {
                        D_8010ADD0.state = 3;
                        D_8010ADD0.targetScale = 2;
                    } else {
                        D_8010ADD0.state = 4;
                    }
                }
            }
            state = D_8010ADD0.state;
            break;
    }
    if (state == 5) {
        func_8009956C(func_80004960, 0);
    }
    func_8007105C();
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/game_setup_menu/func_800045D8.s")
