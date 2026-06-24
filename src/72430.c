#include "common.h"

extern s16 D_80121B50;
extern s32 D_80121850;
extern s32 D_80121858;
extern s32 D_8012185C;
extern s32 D_80121974;
extern s32 D_80121AF8;
extern s32 D_80121AFC;
extern s32 player_bss_0048;
extern u16 D_800DBCF4[];

void func_8009DE50(s32 arg0, s32 arg1);
void func_800720E4(s32 arg0);

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80071830.s")

void func_80071A3C(s32 arg0) {
    if (D_8012185C == 0) {
        if (D_80121858 != 0) {
            func_8009DE50(D_80121858, arg0);
        }
        D_8012185C = 1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80071A8C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80071B74.s")

s32 func_80071BB0(void) {
    u32 ret;
    s32 temp_v1;

    temp_v1 = D_80121AFC;
    ret = -1;
    if (temp_v1 == D_80121AF8) {
        return ret;
    }
    (&D_80121AFC)[(temp_v1 == D_80121AF8) * 0] = (temp_v1 + 1) & 0x3F;
    return temp_v1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80071BE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80071C84.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80071CC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80071E80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_800720E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/MusFxBankSetCurrent.s")

void func_80072114(s32 arg0) {
    D_80121850 = 2;
    player_bss_0048 = arg0;
    D_80121974 = -1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80072138.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_800721B8.s")

struct OSThread_s;

extern void func_8009DD5C(s32 arg0, s32 arg1);
extern void func_8009DDE4(s32 arg0);
extern void osStartThread(struct OSThread_s *);
extern void osStopThread(struct OSThread_s *);
extern struct OSThread_s D_8015A6B8;
extern s8 D_80121B00;

void func_80072260(void) {
    osStopThread(&D_8015A6B8);
    D_80121B00 = 0;
    D_80121AF8 = 0;
    D_80121AFC = 0;
    func_8009DD5C(1, 0);
    osStartThread(&D_8015A6B8);
}

void func_800722B4(void) {
    osStopThread(&D_8015A6B8);
    func_8009DD5C(3, 0x14);
    osStartThread(&D_8015A6B8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_800722F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80072518.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_8007276C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80072844.s")

void func_800728E0(void) {
    func_800720E4(D_800DBCF4[D_80121B50]);
}

void func_80072918(void) {
    func_8009DDE4(3);
}

void func_80072938(void) {
    func_8009DDE4(2);
}

extern s32 gzip_data_0000;
extern void func_80072964(s32, s32, s32, s32, f32, s32, s32);

void myfree(void) {
    gzip_data_0000 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80072964.s")

void func_80072A20(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s16 arg5) {
    s32 temp_a0 = arg0 << 16;
    s32 temp_a2 = arg2 << 16;
    s32 temp_a3 = arg3 << 16;

    func_80072964(temp_a0 >> 16, arg1, temp_a2 >> 16, temp_a3 >> 16, arg4, arg5, 0);
}

void func_80072A74(s16 arg0, s32 arg1, s16 arg2, s16 arg3) {
    func_80072964(arg0, arg1, arg2, arg3, 0.0f, -1, 0);
}

void func_80072AC8(s16 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5) {
    func_80072964(arg0, arg1, arg2, arg3, 0.0f, arg4 + 4, arg5);
}

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80072B24.s")
