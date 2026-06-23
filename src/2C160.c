#include "common.h"

extern void func_80071824(void *, void *);
extern void func_8002C18C(void);

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002B560.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002B8B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002BA00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002BA38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002BB24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002BC60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002BC9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002BDAC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002BF54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002BF9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002C18C.s")

void func_8002C318(void *arg0) {
    *(s16 *)((s32)arg0 + 0x18) = -0x28;
    *(s16 *)((s32)arg0 + 0x1A) = 0xC;
    *(s16 *)((s32)arg0 + 0x1C) = 0xC;
    func_80071824(arg0, func_8002C18C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002C350.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002C390.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002C498.s")
