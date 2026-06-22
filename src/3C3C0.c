#include "common.h"

extern void func_80071824(void *arg0, void (*arg1)(void *));
extern void func_8003BA64(void *arg0);
extern void func_8003BC9C(void *arg0);

#pragma GLOBAL_ASM("asm/nonmatchings/3C3C0/func_8003B7C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3C3C0/func_8003B944.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3C3C0/func_8003B9F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3C3C0/func_8003BA64.s")

void func_8003BBBC(void *arg0) {
    *(s8 *)((s32)arg0 + 0x26) = 3;
    *(s16 *)((s32)arg0 + 0x20) = 0;
    *(s16 *)((s32)arg0 + 0x1C) = -0x40;
    *(s16 *)((s32)arg0 + 0x1E) = 0x10;
    *(s16 *)((s32)arg0 + 0x24) = 0;
    func_80071824(arg0, func_8003BA64);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3C3C0/func_8003BC00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3C3C0/func_8003BC9C.s")

void func_8003BEB4(void *arg0) {
    *(s16 *)((s32)arg0 + 0x18) = 0;
    *(s16 *)((s32)arg0 + 0x1A) = 0;
    *(s16 *)((s32)arg0 + 0x1C) = 0;
    *(s8 *)((s32)arg0 + 0x1E) = 0;
    *(s8 *)((s32)arg0 + 0x1F) = 0;
    func_80071824(arg0, func_8003BC9C);
}
