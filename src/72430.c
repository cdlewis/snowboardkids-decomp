#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80071830.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80071A3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80071A8C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80071B74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80071BB0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80071BE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80071C84.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80071CC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80071E80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_800720E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/MusFxBankSetCurrent.s")

extern s32 D_80121850;
extern s32 D_80121974;
extern s32 player_bss_0048;

void func_80072114(s32 arg0) {
    D_80121850 = 2;
    player_bss_0048 = arg0;
    D_80121974 = -1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80072138.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_800721B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80072260.s")

struct OSThread_s;

extern void func_8009DD5C(s32 arg0, s32 arg1);
extern void osStartThread(struct OSThread_s *);
extern void osStopThread(struct OSThread_s *);
extern struct OSThread_s D_8015A6B8;

void func_800722B4(void) {
    osStopThread(&D_8015A6B8);
    func_8009DD5C(3, 0x14);
    osStartThread(&D_8015A6B8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_800722F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80072518.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_8007276C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80072844.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_800728E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80072918.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80072938.s")

extern s32 gzip_data_0000;

void myfree(void) {
    gzip_data_0000 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80072964.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80072A20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80072A74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80072AC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/72430/func_80072B24.s")
