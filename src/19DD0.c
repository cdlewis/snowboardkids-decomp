#include "common.h"

typedef struct {
    char pad0[0x18];
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s8 unk1E;
    s8 unk1F;
    s16 unk20;
    s8 unk22;
    s8 unk23;
    s8 unk24;
    s8 unk25;
    s8 unk26;
} Struct19DD0;

extern void func_80071824(void *, void *);
extern void func_800196CC(void);
extern void func_80019CD8(void);
extern void func_8001A270(void);
extern void func_8001A704(void);
extern void func_8001AB98(void);
extern void func_8001B02C(void);
extern void func_8001B2D8(void);
extern void func_8001B520(void);
extern void func_8001B6D8(void);
extern void func_8001B8F0(void);
extern void func_8000F8AC(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_80013D0C(s32, s32, char *, s32, s32);
extern void func_8001BA2C(s32, s32, s32, s32);
extern s32 func_80043040(s16);
extern int sprintf(char *, const char *, ...);
extern u8 D_800EC9C2;
extern s16 D_80112172;
extern s32 D_80121D8C;

const char D_800E0AE0[] = "%6dG";

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_800191D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_80019314.s")

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_8001952C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_8001958C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_800196CC.s")

void func_800197CC(void *arg0) {
    *(s16 *)((s32)arg0 + 0x18) = -0x108;
    *(s16 *)((s32)arg0 + 0x1A) = 8;
    func_80071824(arg0, func_800196CC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_80019800.s")

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_80019CD8.s")

void func_80019FAC(Struct19DD0 *arg0) {
    arg0->unk18 = 0x94;
    arg0->unk1A = -0x48;
    arg0->unk1C = -8;
    *(s16 *)((s8 *)arg0 + 0x1E) = -0x74;
    arg0->unk20 = 0;
    arg0->unk23 = 0;
    arg0->unk22 = 0;
    func_80071824(arg0, func_80019CD8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_80019FFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_8001A270.s")

void func_8001A44C(Struct19DD0 *arg0) {
    arg0->unk18 = -8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 1;
    arg0->unk1F = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8001A270);
}

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_8001A490.s")

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_8001A704.s")

void func_8001A8E0(Struct19DD0 *arg0) {
    arg0->unk18 = -8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 2;
    arg0->unk1F = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8001A704);
}

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_8001A924.s")

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_8001AB98.s")

void func_8001AD74(Struct19DD0 *arg0) {
    arg0->unk18 = -8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 3;
    arg0->unk1F = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8001AB98);
}

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_8001ADB8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_8001B02C.s")

void func_8001B210(Struct19DD0 *arg0) {
    arg0->unk18 = -8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 4;
    arg0->unk1F = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8001B02C);
}

void func_8001B254(Struct19DD0 *arg0) {
    func_8000F8AC(arg0->unk18, (s16)(arg0->unk1A + (D_800EC9C2 * 0x14)), func_80043040(D_80112172), 7, 0x20, 0x20, 0, arg0->unk1C, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_8001B2D8.s")

void func_8001B454(Struct19DD0 *arg0) {
    arg0->unk18 = -0x7C;
    arg0->unk1A = -0x60;
    arg0->unk1C = 0;
    arg0->unk1E = 0;
    arg0->unk1F = 0;
    func_80071824(arg0, func_8001B2D8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_8001B494.s")

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_8001B520.s")

void func_8001B638(Struct19DD0 *arg0) {
    arg0->unk18 = -0x84;
    arg0->unk1A = 0xC;
    arg0->unk1C = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8001B520);
}

void func_8001B674(Struct19DD0 *arg0) {
    func_8000F8AC(arg0->unk18, arg0->unk1A, func_80043040(D_80112172), 0, 0x20, 0x20, 0, arg0->unk1C, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_8001B6D8.s")

void func_8001B7D8(Struct19DD0 *arg0) {
    arg0->unk18 = -8;
    arg0->unk1A = -0x5C;
    arg0->unk1C = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8001B6D8);
}

void func_8001B814(Struct19DD0 *arg0) {
    char sp40[0x18];

    func_8001BA2C(arg0->unk18, arg0->unk1A, 0x5000, 0x4000);
    func_8000F8AC((s16)(arg0->unk18 + 8), (s16)(arg0->unk1A + 4), func_80043040(D_80112172), 0x11, 0x20, 0x20, 0, arg0->unk1C, 0);
    sprintf(sp40, D_800E0AE0, D_80121D8C);
    func_80013D0C((s16)(arg0->unk18 + 0x10), (s16)(arg0->unk1A + 0x10), sp40, 0, arg0->unk1C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_8001B8F0.s")

void func_8001B9F0(Struct19DD0 *arg0) {
    arg0->unk18 = 0x30;
    arg0->unk1A = 0x40;
    arg0->unk1C = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8001B8F0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/19DD0/func_8001BA2C.s")
