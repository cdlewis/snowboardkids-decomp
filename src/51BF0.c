#include "common.h"

extern void *D_80124868;
extern u8 D_8010B1F0;
extern u8 D_800D5598[];
extern void *D_800D5538[];
extern u8 D_80121B5B;
extern void func_80071824(void *, void *);
extern void func_800483FC(void *, void *, s32);
extern void func_800716E4(void *);
extern void func_8001303C(s32, s32, u8 *, s32, s32, s32, s32);
extern void func_800515F0(void);
extern void func_80051878(void *);
extern void func_80051ED4(void *);
extern void func_80052034(void);
extern void func_800523B8(s32);
extern void func_80052464(s32);
extern void func_800524D4(s32);
extern void func_80052550(s32);
extern void func_80052868(s32);
extern void func_80052E00(s32);
extern void func_80052E70(void);

typedef struct {
    char pad[0x18];
    s32 unk18;
    s32 unk1C;
    void *unk20;
    char pad24[4];
    s16 unk28;
    s8 pad2A;
    s8 unk2B;
} Object51BF0;

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80050FF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80051308.s")

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_800515F0.s")

void func_80051854(void *arg0) {
    func_80071824(arg0, func_800515F0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80051878.s")

void func_80051E80(Object51BF0 *arg0) {
    arg0->unk1C += 0x10;
    if (arg0->unk1C >= 0x79) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_80051878, (s32)arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80051ED4.s")

void func_80051FDC(Object51BF0 *arg0) {
    arg0->unk18 = -0x80;
    arg0->unk1C = 4;
    arg0->unk2B = 0;
    arg0->unk28 = 0;
    arg0->unk20 = D_800D5538[D_80121B5B];
    func_80071824(arg0, func_80051ED4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80052034.s")

void func_80052364(s32 arg0) {
    func_800483FC(&D_80124868, func_80052034, arg0);
}

void func_80052394(void *arg0) {
    func_80071824(arg0, func_80052364);
}

void func_800523B8(s32 arg0) {
    extern s16 D_8011213C;
    extern s32 func_80043040(s16);
    extern void func_80046358(s32, s32, s32, s32);

    func_80046358(-0x84, -0x64, func_80043040(D_8011213C), 6);
    func_80046358(0x74, -0x64, func_80043040(D_8011213C), 7);
    func_80046358(-0x84, 4, func_80043040(D_8011213C), 8);
    func_80046358(0x74, 4, func_80043040(D_8011213C), 9);
}

void func_80052464(s32 arg0) {
    if (D_8010B1F0 == 0) {
        func_800483FC(&D_80124868, func_800523B8, arg0);
        return;
    }
    func_800716E4(arg0);
}

void func_800524B0(void *arg0) {
    func_80071824(arg0, func_80052464);
}

void func_800524D4(s32 arg0) {
    func_8001303C(-0x78, -0x58, D_800D5598, 1, 0x100, 5, 0x28);
}

void func_80052520(s32 arg0) {
    func_800483FC(&D_80124868, func_800524D4, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80052550.s")

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80052868.s")

void func_80052E00(s32 arg0) {
    func_800483FC(&D_80124868, func_80052550, arg0);
    func_800483FC(&D_80124868, func_80052868, arg0);
}

void func_80052E4C(void *arg0) {
    func_80071824(arg0, func_80052E00);
}

#pragma GLOBAL_ASM("asm/nonmatchings/51BF0/func_80052E70.s")

void func_80053604(s32 arg0) {
    func_800483FC(&D_80124868, func_80052E70, arg0);
}

void func_80053634(void *arg0) {
    func_80071824(arg0, func_80053604);
}
