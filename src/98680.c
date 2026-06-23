#include "common.h"

extern void func_80097BAC(void *, s16);
void func_80097C18(s32 arg0, s16 arg1);
extern void func_80097C84(void *, s16);
extern void func_80097CF0(void *, void *, void *);

#pragma GLOBAL_ASM("asm/nonmatchings/98680/func_80097A80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/98680/func_80097AE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/98680/func_80097B48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/98680/func_80097BAC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/98680/func_80097C18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/98680/func_80097C84.s")

#pragma GLOBAL_ASM("asm/nonmatchings/98680/func_80097CF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/98680/func_80097DA4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/98680/func_80097FE4.s")

void func_800980D0(void *arg0, s16 arg1, s16 arg2) {
    s16 sp38[0x10];
    s16 sp18[0x10];

    func_80097C84(sp38, arg2);
    func_80097BAC(sp18, arg1);
    func_80097CF0(sp38, sp18, arg0);
}

void func_80098124(void *arg0, s16 arg1, s16 arg2) {
    s16 sp38[0x10];
    s16 sp18[0x10];

    func_80097BAC(sp38, arg1);
    func_80097C84(sp18, arg2);
    func_80097CF0(sp38, sp18, arg0);
}

void func_80098174(void *arg0, s16 arg1, s16 arg2) {
    s16 sp38[0x10];
    s16 sp18[0x10];

    func_80097C84(sp38, arg2);
    func_80097C18((s32)sp18, arg1);
    func_80097CF0(sp38, sp18, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/98680/func_800981C8.s")

void func_800983E4(void *arg0, s16 arg1, s16 arg2, s16 arg3) {
    s16 sp58[0x10];
    s16 sp38[0x10];
    s16 sp18[0x10];

    func_80097C18(sp58, (s32)arg2);
    func_80097C84(sp38, arg3);
    func_80097CF0(sp58, sp38, sp18);
    func_80097BAC(sp38, arg1);
    func_80097CF0(sp18, sp38, arg0);
}

void func_80098458(void *arg0, s16 arg1, s16 arg2, s16 arg3) {
    s16 sp58[0x10];
    s16 sp38[0x10];
    s16 sp18[0x10];

    func_80097C18(sp58, arg2);
    func_80097C84(sp38, arg3);
    func_80097CF0(sp38, sp58, sp18);
    func_80097BAC(sp38, arg1);
    func_80097CF0(sp18, sp38, arg0);
}

void func_800984CC(void *arg0, s16 arg1, s16 arg2, s16 arg3) {
    s16 sp58[0x10];
    s16 sp38[0x10];
    s16 sp18[0x10];

    func_80097BAC(sp58, arg1);
    func_80097C84(sp38, arg3);
    func_80097CF0(sp58, sp38, sp18);
    func_80097C18(sp38, arg2);
    func_80097CF0(sp18, sp38, arg0);
}

void func_8009853C(void *arg0, s16 arg1, s16 arg2) {
    s16 sp38[0x10];
    s16 sp18[0x10];

    func_80097C18((s32)sp38, arg2);
    func_80097BAC(sp18, arg1);
    func_80097CF0(sp38, sp18, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/98680/func_80098590.s")

#pragma GLOBAL_ASM("asm/nonmatchings/98680/func_800987A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/98680/func_800989EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/98680/func_80098C30.s")
