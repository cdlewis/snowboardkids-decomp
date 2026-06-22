#include "common.h"

extern void func_8008C704(void);
extern void func_800710CC(s32);
extern void func_8007115C(void);
extern void func_8006D700(void);
extern void func_80096E3C(void);
extern void func_80072114(s32);
extern void func_8009956C(void *, s32);
extern void func_8007AA50(void);
extern void func_800403D8(void);

typedef struct {
    /* 0x00 */ char pad[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
} Struct801235B8;

extern Struct801235B8 *D_801235B8;
extern u8 D_8010B1F0;
extern s16 D_801124B8;
extern s8 D_80121B56;
extern s16 D_800DEF14;
extern u8 D_80123751;
extern s32 D_801235B4;
extern u8 D_80121B5B;
extern s16 D_800D3C90[];

extern void func_8003FEF4(void);
extern void func_8003FF78(void);
extern void func_80040450(void);
extern void func_800408E4(void);

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_8003F520.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_8003F554.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_8003F6C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_8003F718.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_8003F778.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_8003F7E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_8003F864.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_8003FB70.s")

extern void func_8003FC60(void);

void func_8003FBE8(void) {
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    if (D_8010B1F0 != 0) {
        D_801235B8->unk18 = 0;
        func_80072114(0x20);
        func_8009956C(&func_8003FC60, 0);
    }
    D_801124B8 = 0x80;
}

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_8003FC60.s")

void func_8003FE54(void) {
    D_801235B4 = 0;
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    func_8007AA50();
    D_801235B8->unk18 += 1;
    if (D_801235B8->unk18 == D_800D3C90[D_80121B5B]) {
        func_8009956C(func_8003FEF4, 0);
        func_80072114(0x40);
    }
}

void func_8003FEF4(void) {
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    func_8007AA50();
    D_800DEF14 += 0x10;
    if (D_800DEF14 >= 0xFF) {
        D_80123751 = 1;
        {
            void (*func_ptr)(void) = func_8003FF78;
            func_8009956C(func_ptr, 0);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_8003FF78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_8003FFD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_80040004.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_8004002C.s")

void func_80040360(void) {
    D_800DEF14 -= 0x10;
    if (D_800DEF14 < 0) {
        D_800DEF14 = 0;
        func_8009956C(func_800403D8, 0);
    }
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    D_801124B8 = 0x80;
}

void func_800403D8(void) {
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    if (D_8010B1F0 != 0) {
        D_801235B8->unk18 = 0;
        func_80072114(0x20);
        func_8009956C(&func_80040450, 0);
    }
    D_801124B8 = 0x80;
}

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_80040450.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_80040638.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_800407AC.s")

void func_8004086C(void) {
    D_80121B56 = 1;
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    func_8007AA50();
    D_801124B8 = 0x80;
    if (D_8010B1F0 != 0) {
        func_8009956C(&func_800408E4, 0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_800408E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_8004097C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_80040A48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_80040B04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_80040B54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_80040B90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_80040BF4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_80040C44.s")
