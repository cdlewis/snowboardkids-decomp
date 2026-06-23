#include "common.h"

struct Struct35E20 {
    char pad0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
};

typedef struct Struct35E20 Struct35E20;

extern void func_80041DD4(s32 arg0, s32 arg1);
extern void func_8003D218(s32 arg0, s32 arg1, s32 arg2);
extern s32 func_80041FB4(s32 arg0);
extern void func_80042034(s32 arg0);
extern void func_800428C8(s32 arg0);
extern void func_80071824(s32 arg0, void (*arg1)(void));
extern void func_80071408(void *arg0, s32 arg1, s32 arg2);
extern void func_800420FC(s32 arg0, s16 arg1, s16 arg2, s16 arg3);
extern void func_8003524C(Struct35E20 *arg0);
extern void func_8003556C(void);
extern void func_80035A2C(void);
extern void func_80035AA4(Struct35E20 *);
extern void func_80035AFC(Struct35E20 *);
extern void func_80035EA8(void);
extern void func_8003C0A4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_8004209C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_8003C208(void);
extern void func_8003600C(void);
extern void func_80036218(void);
extern void func_80035878(s32 arg0);

extern u16 D_8010B1A2;

void func_80035220(s32 arg0) {
    func_80041FB4(1);
    func_800428C8(1);
}

void func_8003524C(Struct35E20 *arg0) {
    func_80041FB4(1);
    func_800428C8(1);
    if (D_8010B1A2 == 0x41) {
        func_80071824((s32) arg0, (void (*)(void)) func_80035220);
        func_80041DD4(1, 0x5B);
        arg0->unk26 = 0xC00;
        func_800420FC(1, arg0->unk24, arg0->unk26, arg0->unk28);
    }
}

void func_800352C4(s32 arg0) {
    func_80041FB4(1);
    func_800428C8(1);
    if (D_8010B1A2 == 0x40) {
        *(u16 *)((s32)arg0 + 0x2A) = 0;
        func_80071824(arg0, func_8003524C);
        func_80041DD4(1, 0x5A);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035320.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_800353AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_8003542C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_800354BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_8003556C.s")

void func_80035600(s32 arg0) {
    func_80042034(1);
    func_800428C8(1);
    if (D_8010B1A2 == 0x3A) {
        func_80071824(arg0, func_8003556C);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_8003564C.s")

void func_8003564C(void);

void func_800356C8(s32 arg0) {
    func_80042034(1);
    func_800428C8(1);
    if (D_8010B1A2 == 0x38) {
        func_80071824(arg0, func_8003564C);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035714.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_800357A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_800357E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035878.s")

void func_8003592C(s32 arg0) {
    func_800428C8(1);
    if (D_8010B1A2 == 0x30) {
        func_80071824(arg0, (void (*)(void)) func_80035878);
        func_80041DD4(1, 0x51);
        func_8003C0A4(6, -0x40, 1, 0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035990.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035A2C.s")

void func_80035AA4(Struct35E20 *arg0) {
    s32 temp;
    s32 sp1C;

    sp1C = func_80041FB4(1);
    temp = (s32)arg0 + 0x2A;
    func_800428C8(1);
    if (sp1C == 1) {
        func_80071824((s32)arg0, func_80035A2C);
        *(u16 *)temp = 0;
    }
}

void func_80035AFC(Struct35E20 *arg0) {
    s32 temp_a1;

    func_80041FB4(1);
    temp_a1 = (arg0->unk18 += 0x2800);
    if (1) {
        func_8004209C(1, temp_a1, arg0->unk1C, arg0->unk20);
        func_800428C8(1);
    }
    if (D_8010B1A2 == 0x2D) {
        func_80071824((s32) arg0, func_80035AA4);
        func_80041DD4(1, 0x4F);
        func_8003C0A4(8, -0x40, 1, 0);
    }
}

void func_80035B88(s32 arg0) {
    func_80042034(1);
    func_800428C8(1);
    if (D_8010B1A2 == 0x2B) {
        func_80071824(arg0, func_80035AFC);
        func_80041DD4(1, 0x4D);
        func_80071408(&func_8003C208, 0, 0x64);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035BF4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035C78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035CC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035D54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035DB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035E28.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035EA8.s")

void func_80035F28(s32 arg0) {
    func_80042034(1);
    func_800428C8(1);
    if (D_8010B1A2 == 0xE) {
        func_80071824(arg0, func_80035EA8);
        func_80041DD4(1, 0x1F);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035F80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_8003600C.s")

void func_800360CC(s32 arg0) {
    if (func_80041FB4(1) == 1) {
        func_80071824(arg0, func_8003600C);
        func_80041DD4(1, 0x13);
    }
    func_800428C8(1);
}

void func_8003611C(Struct35E20 *arg0) {
    u16 temp;

    func_80041FB4(1);
    temp = (*(u16 *)((s32)arg0 + 0x2A) & 0xFFFF) + 1;
    *(u16 *)((s32)arg0 + 0x2A) = temp;
    if ((temp & 0xFFFF) == 0x46) {
        *(u16 *)((s32)arg0 + 0x2A) = 0;
        func_80071824((s32)arg0, func_800360CC);
        func_80041DD4(1, 0x12);
        func_8003D218(-0xB, -0x58, 1);
    }
    func_800428C8(1);
}

void func_80036198(Struct35E20 *arg0) {
    s32 temp_a3;

    if (func_80041FB4(1) == 0) {
        temp_a3 = (arg0->unk20 += (s32)0xFFFA0000);
        func_8004209C(1, arg0->unk18, arg0->unk1C, temp_a3);
    }
    if (D_8010B1A2 == 0xC) {
        func_80071824((s32)arg0, func_8003611C);
        func_80041DD4(1, 0x11);
    }
    func_800428C8(1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80036218.s")

void func_800362B4(s32 arg0) {
    if (D_8010B1A2 == 9) {
        func_80071824(arg0, func_80036218);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_800362E8.s")
