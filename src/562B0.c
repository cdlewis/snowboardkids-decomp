#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_800556B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80055B04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80055B34.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80055C7C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80055FA4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80056070.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_800560F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_8005616C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80056348.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_8005638C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_800563C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80056444.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_800564A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_800565E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_8005663C.s")

extern void func_80056348(void *arg0, s32 arg1);
extern void func_8005638C(void *arg0, s32 arg1);
extern void func_800563C4(void *arg0, s32 arg1);
extern void func_80056444(void *arg0, s32 arg1);
extern void func_800564A8(void *arg0);
extern void func_80071824(void *arg0, void (*arg1)(void));

void func_8005663C(void);

void func_800566C0(void *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, -0x20000);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, -3);
    func_800564A8(arg0);
    *(s16 *)((s32)arg0 + 0x3A) = *(s16 *)((s32)arg0 + 0x3A) - 1;
    if (*(s16 *)((s32)arg0 + 0x3A) == 0) {
        func_80071824(arg0, func_8005663C);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_8005673C.s")

extern void func_8005673C(void);

void func_800567C4(void *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, 0);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, 0);
    func_800564A8(arg0);
    *(s16 *)((s32)arg0 + 0x3A) = *(s16 *)((s32)arg0 + 0x3A) - 1;
    if (*(s16 *)((s32)arg0 + 0x3A) == 0) {
        func_80071824(arg0, func_8005673C);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80056840.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_800568C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80056940.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80056998.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80056A04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80056A58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80056ACC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80056B1C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80056B88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80056BD8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80056C44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80056CA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_8005711C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_8005714C.s")
