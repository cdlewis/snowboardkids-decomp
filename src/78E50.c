#include "common.h"

extern s16 D_801221D2;
extern char D_800E1700[];
extern void sprintf(char *, const char *, ...);
extern void func_80048278(s32, s32, char *, s32);

#pragma GLOBAL_ASM("asm/nonmatchings/78E50/func_80078250.s")

void func_800782B4(s32 arg0) {
    char buf[0x64];
    sprintf(buf, D_800E1700, D_801221D2);
    func_80048278(0x28, 0x28, buf, 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/78E50/func_800782FC.s")
