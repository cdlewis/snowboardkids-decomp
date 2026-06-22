#include "common.h"

extern u8 D_800D3F00[];
extern u16 D_800D4000;
extern u16 D_800D4004;
extern s16 D_801235B0;

u8 func_800430D0(void) {
    D_800D4000++;
    if (D_801235B0 == 0) {
        D_800D4000++;
    }
    D_800D4000 &= 0xFF;
    return D_800D3F00[D_800D4000];
}

u8 func_80043120(void) {
    D_800D4004++;
    D_800D4004 &= 0xFF;
    return D_800D3F00[D_800D4004];
}

void func_80043154(void) {
    D_800D4004 = 0;
}

u8 func_80043160(u8 *arg0) {
    arg0[0x518]++;
    return D_800D3F00[arg0[0x518]];
}

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
