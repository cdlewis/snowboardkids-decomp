#include "common.h"

s16 func_80097B48(s16 arg0);

extern s16 D_800B9810[];
extern void func_80097BAC(s16 *, s16);
void func_80097C18(s16 *arg0, s16 arg1);
extern void func_80097C84(s16 *, s16);
extern void func_80097CF0(void *, void *, void *);

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} MatCopy80097A80;

extern MatCopy80097A80 D_800DEE30;

void func_80097A80(MatCopy80097A80 *arg0) {
    *arg0 = D_800DEE30;
}

s16 func_80097AE8(s16 arg0) {
    s16 temp;

    arg0 &= 0xFFF;
    if (arg0 == 0x400) {
        return 0x1000;
    }
    if (arg0 == 0xC00) {
        return -0x1000;
    }
    temp = D_800B9810[arg0] >> 3;
    return temp;
}

#pragma GLOBAL_ASM("asm/nonmatchings/98680/func_80097B48.s")

void func_80097BAC(s16 *arg0, s16 arg1) {
    s32 sine = func_80097AE8(arg1);
    s16 cosine = func_80097B48(arg1);

    arg0[0] = 0x1000;
    arg0[1] = 0;
    arg0[2] = 0;
    arg0[3] = 0;
    arg0[4] = cosine;
    arg0[5] = sine;
    arg0[6] = 0;
    arg0[7] = -sine;
    arg0[8] = cosine;
}

void func_80097C18(s16 *arg0, s16 arg1) {
    s32 sine = func_80097AE8(arg1);
    s16 cosine = func_80097B48(arg1);

    arg0[0] = cosine;
    arg0[1] = 0;
    arg0[2] = -sine;
    arg0[3] = 0;
    arg0[4] = 0x1000;
    arg0[5] = 0;
    arg0[6] = sine;
    arg0[7] = 0;
    arg0[8] = cosine;
}

void func_80097C84(s16 *arg0, s16 arg1) {
    s32 sine = func_80097AE8(arg1);
    s16 cosine = func_80097B48(arg1);

    arg0[0] = cosine;
    arg0[1] = sine;
    arg0[2] = 0;
    arg0[3] = -sine;
    arg0[4] = cosine;
    arg0[5] = 0;
    arg0[6] = 0;
    arg0[7] = 0;
    arg0[8] = 0x1000;
}

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
