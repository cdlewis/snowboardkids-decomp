#include "common.h"

extern void func_80071824(s32 arg0, void (*arg1)(void));
extern void func_80029344(void);
extern void func_800296D8(void);
extern void func_80029CE4(void);
extern void func_8002A27C(void);
extern void func_8002A710(void);
extern void func_8002AB24(void);
extern void func_8002AE3C(void);
extern void func_8002B05C(void);
extern void func_8002B1FC(void);
extern void func_8002B424(void);
extern void func_8000F8AC(s16, s16, s32, s32, s32, s32, s32, s32, s32);
extern void func_80013154(s32, s32, u8 *, s32, s32, s32);
extern void func_8001BA2C(s32, s32, s32, s32);
extern void func_80013D0C(s32, s32, char *, s32, s32);
extern int sprintf(char *, const char *, ...);
extern s32 func_80043040(s16);
extern s16 D_80112172;
extern u8 D_80121B5E;
extern s32 D_80121D8C;
extern u8 D_800B7198[];

const char D_800E0EA0[] = "%6dG";

struct Struct29E00 {
    char pad0[0x18];
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s8 unk24;
    /* 0x25 */ s8 unk25;
    /* 0x26 */ s8 unk26;
};

typedef struct Struct29E00 Struct29E00;

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_80029200.s")

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_80029344.s")

void func_80029548(Struct29E00 *arg0) {
    s32 temp_v1 = -0x104;
    long long zero;
    s32 temp_t6 = -0x58;
    s32 temp_t7 = 1;

    arg0->unk18 = temp_v1;
    arg0->unk1A = temp_v1;
    arg0->unk1C = temp_v1;
    arg0->unk1E = temp_v1;
    arg0->unk22 = temp_t6;
    zero = 0;
    arg0->unk25 = zero;
    arg0->unk26 = temp_t7;
    arg0->unk24 = zero;
    func_80071824((s32) arg0, func_80029344);
}

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_80029598.s")

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_800296D8.s")

void func_800297D8(Struct29E00 *arg0) {
    arg0->unk18 = -0x108;
    arg0->unk1A = 8;
    func_80071824((s32) arg0, func_800296D8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002980C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_80029CE4.s")

void func_80029FB8(Struct29E00 *arg0) {
    arg0->unk18 = 0x94;
    arg0->unk1A = -0x48;
    arg0->unk1C = -8;
    arg0->unk1E = -0x74;
    arg0->unk20 = 0;
    *((s8 *)arg0 + 0x23) = 0;
    *((s8 *)arg0 + 0x22) = 0;
    func_80071824((s32) arg0, func_80029CE4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002A008.s")

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002A27C.s")

void func_8002A458(Struct29E00 *arg0) {
    arg0->unk18 = -8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 1;
    *((s8 *)arg0 + 0x1F) = 0;
    *((s8 *)arg0 + 0x1E) = 0;
    func_80071824((s32) arg0, func_8002A27C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002A49C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002A710.s")

void func_8002A8EC(Struct29E00 *arg0) {
    arg0->unk18 = -8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 2;
    *((s8 *)arg0 + 0x1F) = 0;
    *((s8 *)arg0 + 0x1E) = 0;
    func_80071824((s32) arg0, func_8002A710);
}

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002A930.s")

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002AB24.s")

void func_8002AD74(Struct29E00 *arg0) {
    arg0->unk18 = -8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 2;
    *((s8 *)arg0 + 0x21) = 0;
    *((s8 *)arg0 + 0x20) = 0;
    func_80071824((s32) arg0, func_8002AB24);
}

void func_8002ADB8(Struct29E00 *arg0) {
    func_8000F8AC(arg0->unk18, arg0->unk1A + (D_80121B5E * 0x18), func_80043040(D_80112172), 7, 0x20, 0x20, 0, arg0->unk1C, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002AE3C.s")

void func_8002AFB8(Struct29E00 *arg0) {
    arg0->unk18 = -0x7C;
    arg0->unk1A = -0x58;
    arg0->unk1C = 0;
    *((s8 *)arg0 + 0x1E) = 0;
    *((s8 *)arg0 + 0x1F) = 0;
    func_80071824((s32) arg0, func_8002AE3C);
}

void func_8002AFF8(Struct29E00 *arg0) {
    u8 *temp = &D_800B7198[D_80121B5E * 0x8C];

    func_80013154(arg0->unk18, arg0->unk1A, temp, 1, arg0->unk1C, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002B05C.s")

void func_8002B15C(Struct29E00 *arg0) {
    arg0->unk18 = -0x84;
    arg0->unk1A = 0xC;
    arg0->unk1C = 0;
    *((s8 *)arg0 + 0x1E) = 0;
    func_80071824((s32) arg0, func_8002B05C);
}

void func_8002B198(Struct29E00 *arg0) {
    func_8000F8AC(arg0->unk18, arg0->unk1A, func_80043040(D_80112172), 2, 0x20, 0x20, 0, arg0->unk1C, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002B1FC.s")

void func_8002B2FC(Struct29E00 *arg0) {
    arg0->unk18 = -8;
    arg0->unk1A = -0x5C;
    arg0->unk1C = 0;
    *((s8 *)arg0 + 0x1E) = 0;
    func_80071824((s32) arg0, func_8002B1FC);
}

void func_8002B338(Struct29E00 *arg0) {
    char sp40[0x18];

    if (D_80121B5E != 3) {
        func_8001BA2C(arg0->unk18, arg0->unk1A, 0x5000, 0x4000);
        func_8000F8AC((s16)(arg0->unk18 + 8), (s16)(arg0->unk1A + 4), func_80043040(D_80112172), 0x11, 0x20, 0x20, 0, arg0->unk1C, 0);
        sprintf(sp40, D_800E0EA0, D_80121D8C);
        func_80013D0C((s16)(arg0->unk18 + 0x10), (s16)(arg0->unk1A + 0x10), sp40, 0, arg0->unk1C);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002B424.s")

void func_8002B524(Struct29E00 *arg0) {
    arg0->unk18 = 0x30;
    arg0->unk1A = 0x40;
    arg0->unk1C = 0;
    *((s8 *)arg0 + 0x1E) = 0;
    func_80071824((s32) arg0, func_8002B424);
}
