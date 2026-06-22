#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/43CD0/func_800430D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/43CD0/func_80043120.s")

extern s16 D_800D4004;

void func_80043154(void) {
    D_800D4004 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/43CD0/func_80043160.s")

extern s16 D_800D4000;
extern s16 D_801235B0;

void func_80043184(void) {
    D_801235B0 = 0;
    D_800D4000 = 0;
    func_80043154();
}

#pragma GLOBAL_ASM("asm/nonmatchings/43CD0/func_800431B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/43CD0/func_8004331C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/43CD0/func_800433D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/43CD0/func_800437F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/43CD0/func_800438EC.s")
