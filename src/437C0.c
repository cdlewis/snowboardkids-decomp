#include "common.h"

extern s32 D_801101A0[][5];
extern void *D_801107D8[];
extern u16 D_80110918;
extern u8 D_80110198[];

#pragma GLOBAL_ASM("asm/nonmatchings/437C0/func_80042BC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/437C0/func_80042C28.s")

#pragma GLOBAL_ASM("asm/nonmatchings/437C0/func_80042CDC.s")

void func_80042D28(void *arg0) {
    D_80110918 = D_80110918 - 1;
    D_801107D8[D_80110918] = arg0;
    *(s8 *)((s32)arg0 + 0x11) = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/437C0/func_80042D58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/437C0/func_80042EE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/437C0/func_80042FA0.s")

s32 func_80043040(s32 arg0) {
    return D_801101A0[arg0][0];
}

void func_80043060(s32 arg0) {
    u8 *temp = &D_80110198[arg0 * 0x14];

    if (temp[0x11] != 0) {
        temp[0x11] = 2;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/437C0/func_80043094.s")
