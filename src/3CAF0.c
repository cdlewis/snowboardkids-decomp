#include "common.h"

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ u8 unk26;
} Struct3CAF0;

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
} Struct3CAF0b;

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ u16 unk24;
    /* 0x26 */ u8 unk26;
} Struct3CAF0c;

extern void func_80071824(void *, void *);
extern void func_800483FC(void *, void *, void *);
extern void func_80011D74(void *, s32, s16, s16);
extern s32 func_80043040(s16);
extern void func_8000F030(s16, s16, s32, s32, s32, s32, s32, s32);
extern s32 D_80124838;
extern s32 D_80124868;
extern s16 D_80112172;
extern u16 D_800B9530[];

void func_8003BF7C(Struct3CAF0 *arg0);
void func_8003C180(Struct3CAF0b *arg0);
void func_8003C634(Struct3CAF0 *arg0);
void func_8003C7E4(Struct3CAF0 *arg0);
void func_8003CE78(Struct3CAF0 *arg0);
void func_8003D2F4(Struct3CAF0 *arg0);
void func_8003D3C0(Struct3CAF0 *arg0);
void func_8003DB84(Struct3CAF0 *arg0);
void func_8003C2EC(Struct3CAF0 *arg0);
void func_8003C93C(Struct3CAF0 *arg0);
void func_8003CAD8(Struct3CAF0 *arg0);
void func_8003CC58(Struct3CAF0 *arg0);
void func_8003CF98(Struct3CAF0 *arg0);
void func_8003D124(Struct3CAF0 *arg0);

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003BEF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003BF7C.s")

void func_8003C078(Struct3CAF0 *arg0) {
    arg0->unk22 = 0;
    arg0->unk20 = 0;
    func_80071824(arg0, func_8003BF7C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003C0A4.s")

void func_8003C118(Struct3CAF0c *arg0) {
    s32 temp = func_80043040(D_80112172);
    Struct3CAF0c *p = (Struct3CAF0c *)((s32)arg0 + arg0->unk24 * 2);
    func_8000F030(p->unk18, p->unk1E, temp, 0xF, 0x20, 0x20, 0, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003C180.s")

void func_8003C208(Struct3CAF0b *arg0) {
    arg0->unk1C = -0x38;
    arg0->unk22 = -0x5A;
    arg0->unk18 = -0x30;
    arg0->unk1E = -0x5D;
    arg0->unk1A = -0x28;
    arg0->unk20 = -0x56;
    arg0->unk26 = 0;
    arg0->unk24 = 0;
    func_80071824(arg0, func_8003C180);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003C264.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003C2EC.s")

void func_8003C3F4(Struct3CAF0 *arg0) {
    arg0->unk22 = 0;
    arg0->unk20 = 0;
    func_80071824(arg0, func_8003C2EC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003C420.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003C484.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003C634.s")

void func_8003C728(Struct3CAF0 *arg0) {
    arg0->unk18 = -0x3D;
    arg0->unk1A = -0x63;
    arg0->unk1C = -0x4E;
    arg0->unk1E = -0x6C;
    arg0->unk22 = 0;
    arg0->unk24 = 0;
    arg0->unk20 = 0;
    arg0->unk26 = 0;
    func_80071824(arg0, func_8003C634);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003C77C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003C7E4.s")

void func_8003C870(Struct3CAF0 *arg0) {
    arg0->unk18 = 0x3B;
    arg0->unk1A = -0x38;
    arg0->unk1E = 0;
    arg0->unk1C = 0;
    func_80071824(arg0, func_8003C7E4);
}

void func_8003C8AC(Struct3CAF0c *arg0) {
    s32 temp = func_80043040(D_80112172);
    func_8000F030(arg0->unk18 - 0xE, arg0->unk1A - 0x50, temp, (u16)(D_800B9530[arg0->unk1C] + 6), 0x20, 0x20, 0, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003C93C.s")

void func_8003CA44(Struct3CAF0 *arg0) {
    arg0->unk1E = 0;
    arg0->unk1C = 0;
    func_80071824(arg0, func_8003C93C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003CA70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003CAD8.s")

void func_8003CB4C(Struct3CAF0 *arg0) {
    arg0->unk1E = 0;
    arg0->unk1C = 0;
    func_80071824(arg0, func_8003CAD8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003CB78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003CBCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003CC58.s")

void func_8003CD70(Struct3CAF0 *arg0) {
    arg0->unk22 = 0;
    arg0->unk20 = 0;
    func_80071824(arg0, func_8003CC58);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003CD9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003CE10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003CE78.s")

void func_8003CF00(Struct3CAF0 *arg0) {
    arg0->unk18 = -0x14;
    arg0->unk1A = -0x59;
    arg0->unk1E = 0;
    arg0->unk1C = 0;
    func_80071824(arg0, func_8003CE78);
}

void func_8003CF3C(Struct3CAF0c *arg0) {
    s32 temp = func_80043040(D_80112172);
    func_8000F030(arg0->unk18, arg0->unk1A, temp, arg0->unk1C, 0x20, 0x20, 0, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003CF98.s")

void func_8003D03C(Struct3CAF0 *arg0) {
    arg0->unk1C = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8003CF98);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D068.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D0BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D124.s")

void func_8003D1EC(Struct3CAF0 *arg0) {
    arg0->unk1C = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8003D124);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D218.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D27C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D2F4.s")

void func_8003D384(Struct3CAF0 *arg0) {
    arg0->unk18 = -0xA7;
    arg0->unk1A = -0x61;
    arg0->unk1E = 0;
    arg0->unk1C = 0;
    func_80071824(arg0, func_8003D2F4);
}

void func_8003D3C0(Struct3CAF0 *arg0) {
    func_80011D74(&arg0->unk20, 1, arg0->unk18, arg0->unk1A);
}

void func_8003D3F4(Struct3CAF0 *arg0) {
    func_800483FC(&D_80124838, func_8003D3C0, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D424.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D4C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D580.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D5F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D68C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D720.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D7C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D88C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D908.s")

void func_8003D974(Struct3CAF0 *arg0) {
    func_80011D74(&arg0->unk20, 1, arg0->unk18, arg0->unk1A);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D9A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003DA24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003DA98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003DB1C.s")

void func_8003DB84(Struct3CAF0 *arg0) {
    func_80011D74(&arg0->unk20, 0, arg0->unk18, arg0->unk1A);
}

void func_8003DBB8(Struct3CAF0 *arg0) {
    func_800483FC(&D_80124868, func_8003DB84, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003DBE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003DC70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003DCCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003DD64.s")
