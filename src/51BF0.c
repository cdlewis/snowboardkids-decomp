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

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_800523B8.s")

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
