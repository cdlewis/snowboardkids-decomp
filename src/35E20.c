#include "common.h"

struct Struct35E20 {
    char pad0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u16 unk2A;
};

typedef struct Struct35E20 Struct35E20;

extern void func_80041DD4(s32 arg0, s32 arg1);
extern void func_80041D20(s32 arg0, s32 arg1);
extern void func_8003D218(s32 arg0, s32 arg1, s32 arg2);
extern s32 func_80041FB4(s32 arg0);
extern void func_80042034(s32 arg0);
extern void func_800428C8(s32 arg0);
extern void func_80071824(s32 arg0, void (*arg1)(void));
extern void func_80071408(void *arg0, s32 arg1, s32 arg2);
extern void func_800420FC(s32 arg0, s16 arg1, s16 arg2, s16 arg3);
extern void func_8003524C(Struct35E20 *arg0);
extern void func_80035320(Struct35E20 *arg0);
extern void func_800354BC(void);
extern void func_8003556C(Struct35E20 *arg0);
extern void func_800357E8(Struct35E20 *arg0);
extern void func_80035990(Struct35E20 *arg0);
extern void func_80035A2C(Struct35E20 *arg0);
extern void func_80035AA4(Struct35E20 *arg0);
extern void func_80035AFC(Struct35E20 *);
extern void func_80035BF4(Struct35E20 *arg0);
extern void func_80035CC8(Struct35E20 *arg0);
extern void func_80035EA8(Struct35E20 *arg0);
extern void func_8003C0A4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_8004209C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_8003C208(void);
extern void func_8003D218(s32 arg0, s32 arg1, s32 arg2);
extern void func_8003600C(void);
extern void func_800360CC(s32 arg0);
extern void func_80036218(Struct35E20 *arg0);
extern void func_80035878(Struct35E20 *arg0);
extern void func_8003D068(s32 arg0, s32 arg1);
extern void func_80035F28(s32 arg0);
extern void func_8003CA44(void);
extern void func_8003CD9C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

extern u16 D_8010B1A2;
extern s8 D_8010B1A8;
extern s8 D_8010B1A9;

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

void func_80035320(Struct35E20 *arg0) {
    s32 unused;
    s32 sp18;

    sp18 = func_80041FB4(1);
    func_800428C8(1);
    if (sp18 == 1) {
        if (++arg0->unk2A == 0x1A) {
            arg0->unk2A = 0;
            func_80071824((s32) arg0, (void (*)(void)) func_800352C4);
            func_80041DD4(1, 0x59);
            func_8003D068(0x53, -0x64);
        }
    }
}

void func_800353AC(Struct35E20 *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = func_80041FB4(1);
    func_800428C8(1);
    if (sp18 == 1) {
        if (++arg0->unk2A == 0x14) {
            arg0->unk2A = 0;
            func_80071824((s32)arg0, func_80035320);
            func_80041DD4(1, 0x58);
        }
    }
}

void func_8003542C(Struct35E20 *arg0) {
    s32 unused;
    s32 sp20;
    s32 temp_a1;

    sp20 = func_80041FB4(1);
    func_800428C8(1);
    if (sp20 == 0) {
        temp_a1 = (arg0->unk18 += 0x18000);
        func_8004209C(1, temp_a1, arg0->unk1C, arg0->unk20);
    }
    if (D_8010B1A2 == 0x3F) {
        arg0->unk2A = 0;
        func_80071824((s32)arg0, (void (*)(void))func_800353AC);
        func_80041DD4(1, 0x57);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_800354BC.s")

void func_8003556C(Struct35E20 *arg0) {
    func_80042034(1);
    arg0->unk18 += 0xFFF58000;
    if (D_8010B1A2 == 0x3C) {
        arg0->unk2A = 0;
        func_80071824((s32)arg0, func_800354BC);
        func_80041DD4(1, 0x5C);
        D_8010B1A9 = 1;
    }
    func_8004209C(1, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(1);
}

void func_80035600(s32 arg0) {
    func_80042034(1);
    func_800428C8(1);
    if (D_8010B1A2 == 0x3A) {
        func_80071824(arg0, func_8003556C);
    }
}

void func_8003564C(Struct35E20 *arg0) {
    s32 var_a1;

    func_80042034(1);
    arg0->unk18 += 0x24000;
    var_a1 = arg0->unk18;
    if (var_a1 >= 0x1200000) {
        arg0->unk18 = 0x1200000;
        func_80071824((s32)arg0, (void (*)(void)) func_80035600);
        var_a1 = arg0->unk18;
    }
    func_8004209C(1, var_a1, arg0->unk1C, arg0->unk20);
    func_800428C8(1);
}

void func_800356C8(s32 arg0) {
    func_80042034(1);
    func_800428C8(1);
    if (D_8010B1A2 == 0x38) {
        func_80071824(arg0, func_8003564C);
    }
}

void func_80035714(Struct35E20 *arg0) {
    s32 var_a1;

    func_80042034(1);
    arg0->unk18 += 0xFFFB8000;
    var_a1 = arg0->unk18;
    if (var_a1 < 0x01000001) {
        arg0->unk18 = 0x01000000;
        func_80071824((s32)arg0, func_800356C8);
        D_8010B1A2 = 0x37;
        var_a1 = arg0->unk18;
    }
    func_8004209C(1, var_a1, arg0->unk1C, arg0->unk20);
    func_800428C8(1);
}

void func_800357A4(Struct35E20 *arg0) {
    if (D_8010B1A2 == 0x36) {
        func_80071824((s32)arg0, func_80035714);
        arg0->unk18 = 0x1900000;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_800357E8.s")

void func_80035878(Struct35E20 *arg0) {
    if (func_80041FB4(1) == 0) {
        arg0->unk20 = 0;
    }
    func_8004209C(1, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(1);
    if (D_8010B1A2 == 0x32) {
        func_80071824((s32)arg0, (void (*)(void))func_800357E8);
        func_80041DD4(1, 0x52);
        arg0->unk26 = 0xC00;
        func_800420FC(1, arg0->unk24, arg0->unk26, arg0->unk28);
        D_8010B1A8 = 0;
        func_8003CD9C(0x13, -0x3A, 1, 1);
    }
}

void func_8003592C(s32 arg0) {
    func_800428C8(1);
    if (D_8010B1A2 == 0x30) {
        func_80071824(arg0, (void (*)(void)) func_80035878);
        func_80041DD4(1, 0x51);
        func_8003C0A4(6, -0x40, 1, 0);
    }
}

void func_80035990(Struct35E20 *arg0) {
    if (func_80041FB4(1) == 1) {
        arg0->unk2A++;
        if (arg0->unk2A == 0xA) {
            arg0->unk2A = 0;
            func_80071824((s32)arg0, (void (*)(void))func_8003592C);
            D_8010B1A2 = 0x2F;
        }
    } else {
        arg0->unk18 += 0x20000;
    }
    func_8004209C(1, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(1);
}

void func_80035A2C(Struct35E20 *arg0) {
    func_800428C8(1);
    if (D_8010B1A2 == 0x2E) {
        if (++arg0->unk2A == 0x14) {
            arg0->unk2A = 0;
            func_80071824((s32)arg0, func_80035990);
            func_80041DD4(1, 0x50);
        }
    }
}

void func_80035AA4(Struct35E20 *arg0) {
    s32 sp1c;
    s32 sp18;

    sp18 = func_80041FB4(1);
    func_800428C8(1);
    sp1c = (sp18 == 1);
    if (sp1c) {
        func_80071824((s32) arg0, func_80035A2C);
        arg0->unk2A = 0;
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

void func_80035BF4(Struct35E20 *arg0) {
    s32 var_a1;

    func_80042034(1);
    arg0->unk18 += 0x120000;
    var_a1 = arg0->unk18;
    if (var_a1 >= -0x600000) {
        arg0->unk18 = -0x600000;
        func_80071824((s32)arg0, (void (*)(void)) func_80035B88);
        D_8010B1A2 = 0x2A;
        var_a1 = arg0->unk18;
    }
    func_8004209C(1, var_a1, arg0->unk1C, arg0->unk20);
    func_800428C8(1);
}

void func_80035C78(Struct35E20 *arg0) {
    if (D_8010B1A2 == 0x29) {
        func_80071824((s32)arg0, func_80035BF4);
        func_80041DD4(1, 0x4C);
        arg0->unk20 = 0xA0000;
    }
}

void func_80035CC8(Struct35E20 *arg0) {
    arg0->unk18 += 0xFFFE0000;
    func_8004209C(1, arg0->unk18, arg0->unk1C, arg0->unk20);
    if (arg0->unk18 < -0x18FFFFF) {
        D_8010B1A2 = 0x12;
        arg0->unk18 = -0x1900000;
        func_80071824((s32)arg0, (void (*)(void)) func_80035C78);
    }
    func_80042034(1);
    func_800428C8(1);
}

void func_80035D54(Struct35E20 *arg0) {
    func_80042034(1);
    func_800428C8(1);
    if (++arg0->unk2A == 0x32) {
        arg0->unk2A = 0;
        func_80071824((s32)arg0, func_80035CC8);
    }
}

void func_80035DB4(Struct35E20 *arg0) {
    func_80042034(1);
    arg0->unk18 += 0x60000;
    func_8004209C(1, arg0->unk18, arg0->unk1C, arg0->unk20);
    if (arg0->unk18 >= -0x580000) {
        arg0->unk18 = -0x580000;
        func_80071824((s32)arg0, (void (*)(void)) func_80035D54);
    }
    func_800428C8(1);
}

void func_80035E28(Struct35E20 *arg0) {
    if (D_8010B1A2 == 0x11) {
        arg0->unk18 = 0xFE700000;
        arg0->unk26 = 0x400;
        func_8004209C(1, -0x1900000, arg0->unk1C, arg0->unk20);
        func_800420FC(1, arg0->unk24, arg0->unk26, arg0->unk28);
        func_80071824((s32)arg0, (void (*)(void)) func_80035DB4);
        func_80041DD4(1, 0x1F);
    }
}

void func_80035EA8(Struct35E20 *arg0) {
    func_80042034(1);
    arg0->unk18 += 0x60000;
    func_8004209C(1, arg0->unk18, arg0->unk1C, arg0->unk20);
    if (arg0->unk18 >= 0x1900000) {
        arg0->unk18 = 0x1900000;
        func_80071824((s32)arg0, (void (*)(void)) func_80035E28);
        D_8010B1A2 = 0xF;
    }
    func_800428C8(1);
}

void func_80035F28(s32 arg0) {
    func_80042034(1);
    func_800428C8(1);
    if (D_8010B1A2 == 0xE) {
        func_80071824(arg0, func_80035EA8);
        func_80041DD4(1, 0x1F);
    }
}

void func_80035F80(Struct35E20 *arg0) {
    if (func_80041FB4(1) == 1) {
        if (++arg0->unk2A == 0xC) {
            arg0->unk2A = 0;
            func_80071824((s32) arg0, (void (*)(void)) func_80035F28);
            func_80041DD4(1, 0x15);
            func_80071408(&func_8003CA44, 0, 0x64);
        }
    }
    func_800428C8(1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_8003600C.s")

void func_800360CC(s32 arg0) {
    if (func_80041FB4(1) == 1) {
        func_80071824(arg0, func_8003600C);
        func_80041DD4(1, 0x13);
    }
    func_800428C8(1);
}

void func_8003611C(Struct35E20 *arg0) {
    func_80041FB4(1);
    arg0->unk2A += 1;
    if (arg0->unk2A == 0x46) {
        arg0->unk2A = 0;
        func_80071824((s32) arg0, (void (*)(void)) func_800360CC);
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

void func_80036218(Struct35E20 *arg0) {
    arg0->unk18 += 0x39435;
    if (D_8010B1A2 == 0xB) {
        func_80071824((s32)arg0, (void (*)(void))func_80036198);
        func_80041DD4(1, 0x10);
        func_8003C0A4(0xA, -0x40, 1, 0);
    } else {
        func_8004209C(1, arg0->unk18, arg0->unk1C, arg0->unk20);
        func_80042034(1);
    }
    func_800428C8(1);
}

void func_800362B4(s32 arg0) {
    if (D_8010B1A2 == 9) {
        func_80071824(arg0, func_80036218);
    }
}

void func_800362E8(Struct35E20 *arg0) {
    arg0->unk18 = 0xFE700000;
    arg0->unk1C = 0;
    arg0->unk20 = 0;
    arg0->unk24 = 0;
    arg0->unk26 = 0x400;
    arg0->unk28 = 0;
    func_80041D20(1, 1);
    func_80041DD4(1, 0xF);
    func_8004209C(1, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800420FC(1, arg0->unk24, arg0->unk26, arg0->unk28);
    func_80071824((s32)arg0, (void (*)(void))func_800362B4);
}
