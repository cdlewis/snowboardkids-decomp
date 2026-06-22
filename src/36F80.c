#include "common.h"

extern void func_80036388(void);
extern void func_80036DAC(void);
extern void func_800373AC(s32 *arg0);
extern void func_80041DD4(s32 arg0, s32 arg1);
extern s32 func_80041FB4(s32 arg0);
extern void func_80042034(s32 arg0);
extern void func_800428C8(s32 arg0);
extern void func_80071824(s32 arg0, void (*arg1)(void));
extern s32 D_8010B1C0;
extern u16 D_8010B1A2;

void func_80036380(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036388.s")

void func_800363B4(s32 arg0);

void func_800363B4(s32 arg0) {
    func_80041FB4(4);
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
    if (D_8010B1A2 == 0x41) {
        func_80071824(arg0, func_80036388);
        func_80041DD4(4, 0x61);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036418.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_800364B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036520.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_800365B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036674.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036704.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_800367A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_800368BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036930.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_800369BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036A4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036AC4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036B54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036C14.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036C8C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036D08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036DAC.s")

void func_80036E58(s32 arg0);

void func_80036E58(s32 arg0) {
    func_80042034(4);
    if (D_8010B1A2 == 4) {
        func_80071824(arg0, func_80036DAC);
        func_80041DD4(4, 5);
    }
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036EBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036F6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036FB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80037070.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_800373AC.s")
