#include "common.h"

typedef struct {
    /* 0x0 */ s8 minutes;
    /* 0x1 */ s8 seconds;
    /* 0x2 */ s16 fraction;
} RaceTimer;

extern s32 func_80043040(s16);
extern void func_80045A78(s32, s32, s32, s32);
extern void func_80047E88(s32, s16, s32, s32);
extern void func_80048278(s32, s32, char *, s32);
extern int sprintf(char *, const char *, ...);
extern s16 D_8011216E;
extern s16 D_80121B52;
extern s16 D_801222F6;
extern u8 D_80121B56;
extern u8 D_80156608;
extern RaceTimer D_80121B74;
extern RaceTimer D_80121B78;
extern s8 D_80122288[];

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80078430.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80078568.s")

const char D_800E1730[] = "Point";
const char D_800E1738[] = "Time Limit";

void func_80078974(s32 arg0) {
    func_80048278(0x60, -0x61, (char *)D_800E1730, 5);
    func_80048278(0x38, 0x47, (char *)D_800E1738, 5);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_800789C0.s")

const char D_800E1760[] = "Time Limit";
const char D_800E176C[] = "/%d";

void func_80078D3C(s32 arg0) {
    char sp18[0x20];

    func_80048278(0x38, 0x47, (char *)D_800E1760, 5);
    sprintf(sp18, D_800E176C, D_801222F6);
    func_80048278(0x70, -0x48, sp18, 6);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80078D9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80079068.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80079154.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80079394.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80079438.s")

void func_80079750(s32 arg0) {

}

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80079758.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_800799DC.s")

void func_80079E48(s32 arg0) {
    int new_var2;
    int new_var;
    s32 sp1C;
    s32 temp_v0;
    s32 var_v1;

    temp_v0 = D_80156608;
    if (0 == temp_v0) {
        var_v1 = -0x30;
    } else {
        var_v1 = 0x2A;
    } sp1C = (s16)var_v1; new_var = 0xFF; func_80047E88(0x70, (s16)var_v1, (D_80122288[new_var2 = temp_v0 * 0x60C] + 0x31) & new_var, 2); func_80047E88(0x78, *(s16 *)((u8 *)&sp1C + 2), 0x2F, 2);
    func_80047E88(0x80, *(s16 *)((u8 *)&sp1C + 2), (D_80121B52 + 0x30) & new_var, 2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80079F04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_8007A108.s")

void func_8007A278(s32 arg0) {
    func_80045A78(0xC, 0x2C, func_80043040(D_8011216E), 0x90);
}

void func_8007A2B8(void) {
    if (D_80121B56 == 0) {
        D_80121B74.fraction = D_80121B74.fraction + 0x358;
        if (D_80121B74.fraction >= 0x6401) {
            D_80121B74.fraction = 0;
            D_80121B74.seconds = D_80121B74.seconds + 1;
            if (D_80121B74.seconds >= 0x3C) {
                D_80121B74.seconds = 0;
                D_80121B74.minutes = D_80121B74.minutes + 1;
                if (D_80121B74.minutes >= 0x63) {
                    D_80121B74.fraction = 0x6300;
                    D_80121B74.seconds = 0x3B;
                    D_80121B74.minutes = 0x63;
                }
            }
        }
    }
}

void func_8007A350(void) {
    if (D_80121B56 == 0) {
        D_80121B78.fraction = D_80121B78.fraction - 0x358;
        if (D_80121B78.fraction < 0) {
            D_80121B78.fraction = 0x60F8;
            D_80121B78.seconds = D_80121B78.seconds - 1;
            if (D_80121B78.seconds < 0) {
                D_80121B78.seconds = 0x3B;
                D_80121B78.minutes = D_80121B78.minutes - 1;
                if (D_80121B78.minutes < 0) {
                    D_80121B78.fraction = 0;
                    D_80121B78.seconds = 0;
                    D_80121B78.minutes = 0;
                }
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_8007A3D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_8007A8EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_8007AA50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_8007B130.s")
