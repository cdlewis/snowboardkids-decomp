#include "common.h"
#include "effect_task_scheduler.h"
#include "game_setup_menu.h"

#pragma GLOBAL_ASM("asm/nonmatchings/game_setup_menu/func_80003140.s")

// func_8000337C best match: 87.296% (nonmatchings/func_8000337C-2775475442547365205/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/game_setup_menu/func_8000337C.s")

#ifdef NON_MATCHING
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 fade;
    /* 0x1C */ s32 timer;
} GameSetupMenuState;

extern void func_800035F8(void);
extern void func_80072138();
extern void func_8009956C();
extern u8 D_800E29C0;
extern u8 D_800EC9C1;
extern u16 D_8010ADF0;
extern u8 D_8010AE00[];
extern u8 D_80121B55;
extern GameSetupMenuState *D_801235B8;
extern s32 D_80123758;
extern s32 D_80123778;

void func_8000337C(void) {
    s32 sp18;
    s32 temp_a3;
    s32 temp_v0;
    u16 *var_a2;
    u16 temp_t6;
    u16 temp_v0_2;
    s32 one;

    one = 1;
    if ((D_8010AE00[0] == one) && ((D_80123778 & 0x8000) || (D_80123778 & 0x1000))) {
        func_80072138((s16) (one & 0xFFFFFFFF), 0x32);
        D_8010AE00[0] = 2;
        D_8010AE00[1] = 0;
        one = 1;
    }
    if (D_801235B8->timer == one) {
        if (D_800EC9C1 == 0) {
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
        } else if (D_800EC9C1 < 0x13) {
            D_800EC9C1 += 1;
        }
    }
    if (D_8010AE00[0] == 5) {
        func_8009956C(func_800035F8, 0);
        D_801235B8->fade = 0;
        D_800EC9C1 = 0;
    }
    func_8007105C();
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/game_setup_menu/func_800035F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_setup_menu/func_80003798.s")

void __dummy(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_setup_menu/func_80004164.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_setup_menu/func_800045D8.s")
