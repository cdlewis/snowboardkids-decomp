#include "common.h"

extern s32 func_80043040(s16);
extern void func_8004215C(s32);
extern void func_800483FC(void *, void *, s32);
extern s16 D_8011218A[];
extern s32 D_801248B0;
extern void func_80042574(void);

#pragma GLOBAL_ASM("asm/nonmatchings/43160/func_80042560.s")

#pragma GLOBAL_ASM("asm/nonmatchings/43160/func_80042574.s")

#pragma GLOBAL_ASM("asm/nonmatchings/43160/func_8004270C.s")

void func_800428C8(s32 arg0) {
    s32 temp = func_80043040(D_8011218A[arg0]);

    func_8004215C(temp);
    *(s16 *)((u8 *)temp + 0x8) = 0;
    func_800483FC(&D_801248B0, func_80042574, temp);
}

#pragma GLOBAL_ASM("asm/nonmatchings/43160/func_80042920.s")

#pragma GLOBAL_ASM("asm/nonmatchings/43160/func_8004298C.s")

void func_80042A00(s32 arg0) {
    s32 temp = func_80043040(D_8011218A[arg0]);

    func_8004215C(temp);
    *(s16 *)((u8 *)temp + 0x8) = 0;
    func_800483FC(&D_801248B0, func_80042574, temp);
}

#pragma GLOBAL_ASM("asm/nonmatchings/43160/func_80042A58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/43160/func_80042AB4.s")
