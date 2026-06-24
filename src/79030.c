#include "common.h"

extern s32 func_80043040(s16);
extern void func_80045A78(s32, s32, s32, s32);
extern void func_80048278(s32, s32, char *, s32);
extern int sprintf(char *, const char *, ...);
extern s16 D_8011216E;
extern s16 D_801222F6;

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_80078430.s")

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_80078568.s")

const char D_800E1730[] = "Point";
const char D_800E1738[] = "Time Limit";

void func_80078974(s32 arg0) {
    func_80048278(0x60, -0x61, (char *)D_800E1730, 5);
    func_80048278(0x38, 0x47, (char *)D_800E1738, 5);
}

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_800789C0.s")

const char D_800E1760[] = "Time Limit";
const char D_800E176C[] = "/%d";

void func_80078D3C(s32 arg0) {
    char sp18[0x20];

    func_80048278(0x38, 0x47, (char *)D_800E1760, 5);
    sprintf(sp18, D_800E176C, D_801222F6);
    func_80048278(0x70, -0x48, sp18, 6);
}

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_80078D9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_80079068.s")

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_80079154.s")

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_80079394.s")

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_80079438.s")

void func_80079750(s32 arg0) {

}

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_80079758.s")

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_800799DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_80079E48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_80079F04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_8007A108.s")

void func_8007A278(s32 arg0) {
    func_80045A78(0xC, 0x2C, func_80043040(D_8011216E), 0x90);
}

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_8007A2B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_8007A350.s")

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_8007A3D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_8007A8EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_8007AA50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/79030/func_8007B130.s")
