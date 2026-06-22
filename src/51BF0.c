#include "common.h"

extern void *D_80124868;
extern void func_80071824(void *, void *);
extern void func_800483FC(void *, void *, s32);
extern void func_800515F0(void);
extern void func_80052034(void);
extern void func_80052464(void);
extern void func_800524D4(void);
extern void func_80052E00(void);
extern void func_80052E70(void);

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80050FF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80051308.s")

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_800515F0.s")

void func_80051854(void *arg0) {
    func_80071824(arg0, func_800515F0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80051878.s")

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80051E80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80051ED4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80051FDC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80052034.s")

void func_80052364(s32 arg0) {
    func_800483FC(&D_80124868, func_80052034, arg0);
}

void func_80052394(void *arg0) {
    func_80071824(arg0, func_80052364);
}

void func_800523B8(s32 arg0) {
    extern s16 D_8011213C;
    extern s32 func_80043040(s16);
    extern void func_80046358(s32, s32, s32, s32);

    func_80046358(-0x84, -0x64, func_80043040(D_8011213C), 6);
    func_80046358(0x74, -0x64, func_80043040(D_8011213C), 7);
    func_80046358(-0x84, 4, func_80043040(D_8011213C), 8);
    func_80046358(0x74, 4, func_80043040(D_8011213C), 9);
}

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80052464.s")

void func_800524B0(void *arg0) {
    func_80071824(arg0, func_80052464);
}

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_800524D4.s")

void func_80052520(s32 arg0) {
    func_800483FC(&D_80124868, func_800524D4, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80052550.s")

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80052868.s")

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80052E00.s")

void func_80052E4C(void *arg0) {
    func_80071824(arg0, func_80052E00);
}

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80052E70.s")

void func_80053604(s32 arg0) {
    func_800483FC(&D_80124868, func_80052E70, arg0);
}

void func_80053634(void *arg0) {
    func_80071824(arg0, func_80053604);
}
