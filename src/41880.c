#include "common.h"

extern s32 func_80043040(s16);
extern s16 D_8011218A[];
extern void func_800437F0(void *, void *, s32);
extern u8 D_5E34A0;
extern u8 D_608560;
extern u8 D_215BE0;
extern u8 D_21D9D0;

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_80040C80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_80040D94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_80041A20.s")

void func_80041CC0(void) {
    func_800437F0(&D_5E34A0, &D_608560, 0x3F);
}

void func_80041CF0(void) {
    func_800437F0(&D_215BE0, &D_21D9D0, 0x3F);
}

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_80041D20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_80041DD4.s")

void func_80041E60(s32 arg0) {
    func_80043040(D_8011218A[arg0]);
}

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_80041E90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_80041FB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_80042034.s")

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_8004209C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_800420FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_8004215C.s")
