#include "common.h"

typedef struct {
    char pad[0x1A];
    s16 unk1A;
} Struct6A490;

typedef struct {
    char pad[0x24];
    s32 unk24;
    char pad28[4];
    s32 unk2C;
    s32 unk30;
    char pad34[4];
    s32 unk38;
    s16 unk3C;
} Struct6B760;

extern void func_80071824(void *, void *);
extern void func_800483FC(void *, void *, void *);
extern void func_80072138(s32, s32);
extern void func_80069B60(void *);
extern void func_8006AF48(void);
extern void func_8006A80C(void);
extern void func_80069BEC(void);
extern void func_80069E50(void);
extern void func_80069A78(void *);
extern void func_80069890(void *);
extern s32 func_80043040(s16);
extern void func_80047174(s32, s32, s32, s32, s32);
extern void func_80045A78(s32, s32, s32, s32);
extern s16 D_80112168;
extern void func_8006C5C0(void);
extern void func_8006C1B4(void);
extern void func_800699F0(void);
extern void func_8006C7F4(void);
extern void func_80069AF0(void *);
extern void func_8006B6C8(void);
extern Struct6B760 *func_80071408(void *, s32, s32);
extern u8 D_80121B56;
extern s32 D_80124868;
extern s32 D_801248EC;
extern s32 D_801248B0;
extern s32 D_801248A4;
extern s32 D_801248F8;

void func_80069890(void *arg0) {
    if (*(s16 *)((char *)arg0 + 0x18) != 0) {
        func_80047174(-0x34, -0xC, func_80043040(D_80112168), 0x3F, *(s16 *)((char *)arg0 + 0x18));
    } else {
        func_80045A78(-0x34, -0xC, func_80043040(D_80112168), 0x3F);
    }
}

void func_80069914(void *arg0) {
    if (*(s16 *)((char *)arg0 + 0x18) != 0) {
        func_80047174(-0x20, -0xC, func_80043040(D_80112168), 0x40, *(s16 *)((char *)arg0 + 0x18));
    } else {
        func_80045A78(-0x20, -0xC, func_80043040(D_80112168), 0x40);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_80069998.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_800699F0.s")

void func_80069A78(void *arg0) {
    (*(s16 *)((char *)arg0 + 0x18))++;
    if (*(s16 *)((char *)arg0 + 0x18) == 4) {
        func_80072138(0x4C, 0x5A);
        *(s16 *)((char *)arg0 + 0x1A) = 0x14;
        func_80071824(arg0, func_800699F0);
    }
    func_800483FC(&D_80124868, func_80069890, arg0);
}

void func_80069AF0(void *arg0) {
    if (*(s16 *)((char *)arg0 + 0x18) != 0) {
        (*(s16 *)((char *)arg0 + 0x18))--;
    }
    (*(u16 *)((char *)arg0 + 0x1A))--;
    if (*(u16 *)((char *)arg0 + 0x1A) == 0) {
        func_80071824(arg0, func_80069A78);
    }
    func_800483FC(&D_80124868, func_80069890, arg0);
}

void func_80069B60(void *arg0) {
    if ((--(*(u16 *)((char *)arg0 + 0x1A))) == 0) {
        func_80072138(0x4B, 0x5A);
        *(s16 *)((char *)arg0 + 0x18) = 4;
        *(s16 *)((char *)arg0 + 0x1A) = 0x3C;
        func_80071824(arg0, func_80069AF0);
    }
}

void func_80069BC0(Struct6A490 *arg0) {
    arg0->unk1A = 0x14;
    func_80071824(arg0, func_80069B60);
}

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_80069BEC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_80069E50.s")

void func_8006A74C(void *arg0) {
    func_800483FC(&D_801248A4, func_80069BEC, arg0);
    func_800483FC(&D_801248F8, func_80069E50, arg0);
}

void func_8006A798(void *arg0) {
    func_80071824(arg0, func_8006A74C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006A7BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006A80C.s")

void func_8006A85C(void *arg0) {
    func_80072138(0x52, 0x5A);
    func_80071824(arg0, func_8006A80C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006A894.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006ACE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006AE00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006AF48.s")

void func_8006B0D8(void *arg0) {
    func_800483FC(&D_801248B0, func_8006AF48, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006B108.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006B228.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006B3E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006B6C8.s")

void func_8006B760(s16 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    Struct6B760 *p = func_80071408(func_8006B6C8, 0, 0x64);
    if (p != 0) {
        p->unk24 = arg1;
        p->unk2C = arg2;
        p->unk30 = arg3;
        p->unk38 = arg4;
        p->unk3C = arg0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006B7E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006B988.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006BA50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006BB50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006BC68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006BDE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006BE90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006BFC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006C088.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006C1B4.s")

void func_8006C4AC(void *arg0) {
    if (D_80121B56 == 0) {
        if (*(s16 *)((char *)arg0 + 0x50) != 0) {
            *(s16 *)((char *)arg0 + 0x50) += 0x80;
        } else {
            *(s16 *)((char *)arg0 + 0x56) = 0;
            func_80071824(arg0, func_8006C5C0);
        }
    }
    func_800483FC(&D_801248A4, func_8006C1B4, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006C51C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006C5C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006C698.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006C7F4.s")

void func_8006CB50(void *arg0) {
    *(s16 *)((char *)arg0 + 0x20) -= 0x40;
    *(s16 *)((char *)arg0 + 0x20) = *(s16 *)((char *)arg0 + 0x20) & 0x7FF;
    if (*(s32 *)((char *)arg0 + 0x38) != 0) {
        func_800483FC(&D_801248EC, func_8006C7F4, arg0);
    } else {
        func_800483FC(&D_801248A4, func_8006C7F4, arg0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006CBBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006CCC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006CE68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006D2D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6A490/func_8006D384.s")
