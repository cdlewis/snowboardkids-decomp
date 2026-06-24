#include "common.h"

typedef struct {
    char pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    u16 unk2A;
} Actor36F80;

extern void func_80036388(s32 arg0);
extern void func_80036418(void);
extern void func_800364B8(void *);
extern void func_800365B4(Actor36F80 *);
extern void func_800367A8(void);
extern void func_800368BC(void *);
extern void func_80036930(Actor36F80 *);
extern void func_80036A4C(Actor36F80 *);
extern void func_80036B54(void);
extern void func_80036DAC(s32);
extern void func_80036C8C(s32);
extern void func_80036E58(s32);
extern void func_8003CD9C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_800373AC(s32 arg0);
extern void func_80041DD4(s32 arg0, s32 arg1);
extern void func_80041D20(s32 arg0, s32 arg1);
extern s32 func_80041FB4(s32 arg0);
extern void func_80042034(s32 arg0);
extern void func_8004209C(s32, s32, s32, s32);
extern void func_800420FC(s32, s16, s16, s16);
extern void func_800428C8(s32 arg0);
extern void func_80071824(s32 arg0, void (*arg1)(void));
extern s32 D_8010B1C0;
extern u16 D_8010B1A2;
extern s8 D_8010B1AC;
extern void func_800483FC(void *, void *, s32);
extern s32 D_80124898;
extern void func_80037070(void);
extern s32 D_8010B1C4;

void func_80036380(void) {
}

void func_80036388(s32 arg0) {
    func_80041FB4(4);
    func_800428C8(4);
}

void func_800363B4(s32 arg0);

void func_800363B4(s32 arg0) {
    func_80041FB4(4);
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
    if (D_8010B1A2 == 0x41) {
        func_80071824(arg0, func_80036388);
        func_80041DD4(4, 0x61);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036418.s")

void func_800364B8(void *arg0) {
    func_80042034(4);
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
    if (D_8010B1A2 == 0x3D) {
        *(u16 *)((s32)arg0 + 0x2A) = 0;
        func_80071824(arg0, func_80036418);
        func_80041DD4(4, 0x5F);
    }
}

void func_80036520(Actor36F80 *arg0) {
    s32 limit = (s32)0xFF700000;

    func_80042034(4);
    arg0->unk18 += 0x48000;
    if (arg0->unk18 >= limit) {
        arg0->unk18 = limit;
        D_8010B1A2 = 0x3A;
        func_80071824((s32)arg0, (void (*)(void))func_800364B8);
    }
    func_8004209C(4, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(4);
    func_800373AC((s32)&D_8010B1C0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_800365B4.s")

void func_80036674(Actor36F80 *arg0) {
    arg0->unk18 += (s32)0xFFFE8000;
    if (arg0->unk18 < (s32)0xFE700001) {
        arg0->unk18 = (s32)0xFE700000;
        arg0->unk1C = 0;
        func_80071824((s32)arg0, (void (*)(void))func_800365B4);
    }
    func_8004209C(4, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_80042034(4);
    func_800428C8(4);
    func_800373AC((s32)&D_8010B1C0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036704.s")

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_800367A8.s")

void func_800368BC(void *arg0) {
    u16 temp_v0 = *(u16 *)((u8 *)arg0 + 0x2A);
    void *temp_a2 = arg0;

    if (temp_v0 < 0x1E) {
        *(u16 *)((u8 *)arg0 + 0x2A) = temp_v0 + 1;
    } else if (func_80041FB4(4) == 1) {
        *(u16 *)((u8 *)temp_a2 + 0x2A) = 0;
        func_80071824(temp_a2, func_800367A8);
        func_80041DD4(4, 0x1D);
    }
    func_800428C8(4);
}

void func_80036930(Actor36F80 *arg0) {
    func_80042034(4);
    func_800428C8(4);
    arg0->unk2A++;
    if (D_8010B1A2 == 0xF) {
        arg0->unk2A = 0;
        func_8004209C(4, arg0->unk18, arg0->unk1C, arg0->unk20);
        func_80071824((s32)arg0, (void (*)(void))func_800368BC);
        func_80041DD4(4, 0x1C);
        D_8010B1AC = 1;
    }
}

void func_800369BC(void *arg0) {
    if (func_80041FB4(4) == 1) {
        *(u16 *)((s32)arg0 + 0x2A) = 0;
        func_80071824((s32) arg0, func_80036930);
        func_80041DD4(4, 0xC);
        D_8010B1A2 = 0xC;
        D_8010B1AC = 0;
        func_8003CD9C(-0x24, -0x32, 4, 0);
        func_8003CD9C(0x10, -0x32, 4, 1);
    }
    func_800428C8(4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036A4C.s")

void func_80036AC4(Actor36F80 *arg0) {
    func_80042034(4);
    arg0->unk18 += (s32)0xFFFB8000;
    if (arg0->unk18 < (s32)0xFF600001) {
        arg0->unk18 = (s32)0xFF600000;
        func_80071824((s32)arg0, (void (*)(void))func_80036A4C);
        func_80041DD4(4, 0xA);
    }
    func_8004209C(4, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80036B54.s")

void func_80036C14(s32 arg0) {
    if (D_8010B1A2 < 0xA) {
        func_80042034(4);
    } else if (D_8010B1A2 == 0xB) {
        func_80071824(arg0, func_80036B54);
        func_80041DD4(4, 8);
        D_8010B1AC = 1;
    }
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
}

void func_80036C8C(s32 arg0) {
    if (D_8010B1A2 == 8) {
        func_80071824(arg0, func_80036C14);
        func_80041DD4(4, 7);
        D_8010B1C4 = (s32)0xFFE80000;
        D_8010B1AC = 0;
        func_8003CD9C(-0x24, -0x32, 4, 0);
    }
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
}

void func_80036D08(Actor36F80 *arg0) {
    if (func_80041FB4(4) == 1) {
        arg0->unk2A++;
        if (arg0->unk2A < 6) {
            func_80041DD4(4, 6);
        }
    }
    if (arg0->unk2A == 6) {
        func_80071824((s32)arg0, func_80036C8C);
        arg0->unk2A = 0;
        D_8010B1AC = 1;
        D_8010B1A2 = 7;
    }
    func_800428C8(4);
    func_800373AC((s32)&D_8010B1C0);
}

void func_80036DAC(s32 arg0) {
    if ((func_80041FB4(4) == 1) && (D_8010B1A2 == 4)) {
        D_8010B1A2 = 5;
    }
    if (D_8010B1A2 == 6) {
        func_80071824(arg0, func_80036D08);
        func_80041DD4(4, 6);
        D_8010B1AC = 0;
        func_8003CD9C(-0x1C, -0x3A, 4, 0);
    }
    func_800428C8(4);
    func_800373AC((s32)&D_8010B1C0);
}

void func_80036E58(s32 arg0);

void func_80036E58(s32 arg0) {
    func_80042034(4);
    if (D_8010B1A2 == 4) {
        func_80071824(arg0, func_80036DAC);
        func_80041DD4(4, 5);
    }
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
}

void func_80036EBC(Actor36F80 *arg0) {
    arg0->unk18 += 0x24000;
    if (arg0->unk18 >= 0x100000) {
        arg0->unk18 = 0x100000;
        D_8010B1A2 = 2;
        func_80071824((s32)arg0, func_80036E58);
        func_8004209C(4, arg0->unk18, arg0->unk1C, arg0->unk20);
        func_80041DD4(4, 4);
    } else {
        func_8004209C(4, arg0->unk18, arg0->unk1C, arg0->unk20);
        func_80042034(4);
    }
    func_800428C8(4);
    func_800373AC((s32)&D_8010B1C0);
}

extern void func_80071408(void *arg0, s32 arg1, s32 arg2);
extern void func_8003D384(void);

void func_80036F6C(s32 arg0) {
    if (D_8010B1A2 == 1) {
        func_80071824(arg0, func_80036EBC);
        func_80071408(&func_8003D384, 0, 0x64);
    }
}

void func_80036FB4(Actor36F80 *arg0) {
    arg0->unk18 = (s32)0xFE700000;
    arg0->unk1C = 0;
    arg0->unk20 = 0;
    arg0->unk24 = 0;
    arg0->unk26 = 0x400;
    arg0->unk28 = 0;
    arg0->unk2A = 0;
    func_80041D20(4, 4);
    func_80041DD4(4, 3);
    func_8004209C(4, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800420FC(4, arg0->unk24, arg0->unk26, arg0->unk28);
    *((s8 *)&D_8010B1C0 + 0xD) = 4;
    *((s8 *)&D_8010B1C0 + 0xC) = 9;
    *((s32 *)&D_8010B1C0 + 0) = (s32)0xFFF20000;
    *((s32 *)&D_8010B1C0 + 1) = (s32)0xFFF20000;
    *((s32 *)&D_8010B1C0 + 2) = 0;
    func_80071824((s32)arg0, func_80036F6C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/36F80/func_80037070.s")

void func_800373AC(s32 arg0) {
    func_800483FC(&D_80124898, func_80037070, arg0);
}
