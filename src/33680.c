#include "common.h"

extern void func_80033C44(void);
extern void func_80033D64(void);
extern void func_80033504(void);
extern void func_80034BC4(void);
extern s32 D_8010B1C0;

extern void func_800373AC(s32 *arg0);
extern void func_8003C0A4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_80041DD4(s32 arg0, s32 arg1);
extern s32 func_80041FB4(s32 arg0);
extern void func_80042034(s32 arg0);
extern void func_800428C8(s32 arg0);
extern void func_80071824(s32 arg0, void (*arg1)(void));

extern u16 D_8010B1A2;
extern u8 D_8010B1A4;
extern u8 D_8010B1A8;
extern void func_8003CD9C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_8004209C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_8003D068(s16 arg0, s16 arg1);
extern void func_800420FC(s32 arg0, s16 arg1, s16 arg2, s16 arg3);

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u16 unk2A;
} Struct33680;

void func_80032B94(void *arg0);
void func_80032BF0(void *arg0);
void func_80032C74(Struct33680 *arg0);
void func_80032D7C(void);
void func_80032F3C(void);
void func_800342D8(s32 arg0);
void func_80034328(void *arg0);
void func_800346D4(void);
void func_80033688(s32 arg0);
void func_80032FF0(void *arg0);
void func_800330EC(void);
void func_8003329C(void);
void func_80033B20(void);
void func_80034AB0(void);
void func_800350A4(void);
void func_800339B8(void);
void func_80033DE4(void *arg0);
void func_800340D8(void *arg0);
void func_80033758(void);
void func_800338C0(void);
void func_80034014(void);
void func_80032AF0(void);
void func_800335F0(s32 arg0);
void func_80034F2C(void);
void func_8003D218(s32 arg0, s32 arg1, s32 arg2);

void func_80034254(void);
void func_800349A8(s32 arg0);

void func_80032A80(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80032A88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80032AF0.s")

void func_80032B94(void *arg0) {
    func_80041FB4(0);
    func_800428C8(0);
    if (D_8010B1A2 == 0x40) {
        *(u16 *)((s32)arg0 + 0x2A) = 0;
        func_80071824((s32) arg0, func_80032AF0);
        func_80041DD4(0, 0x55);
    }
}

void func_80032BF0(void *arg0) {
    Struct33680 *temp = arg0;
    if (func_80041FB4(0) == 1) {
        temp->unk2A += 1;
        if (temp->unk2A == 0x1E) {
            temp->unk2A = 0;
            func_80071824((s32) temp, func_80032B94);
            func_80041DD4(0, 0x54);
            func_8003D068(0xA, -0x64);
        }
    }
    func_800428C8(0);
}

void func_80032C74(Struct33680 *arg0) {
    struct { s32 ret; s32 pad; } l;
    l.ret = func_80041FB4(0);
    func_800428C8(0);
    if (l.ret == 1) {
        arg0->unk2A += 1;
        if (arg0->unk2A == 0x14) {
            arg0->unk2A = 0;
            func_80071824((s32) arg0, func_80032BF0);
            func_80041DD4(0, 0x53);
        }
    }
}

void func_80032CF4(Struct33680 *arg0) {
    if (func_80041FB4(0) == 0) {
        arg0->unk18 += 0x18000;
        func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    }
    if (D_8010B1A2 == 0x3F) {
        arg0->unk2A = 0;
        func_80071824((s32) arg0, func_80032C74);
        func_80041DD4(0, 0x22);
    }
    func_800428C8(0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80032D7C.s")

void func_80032E48(Struct33680 *arg0) {
    func_80042034(0);
    arg0->unk18 += 0xFFF70000;
    if (D_8010B1A2 == 0x3B) {
        func_80071824((s32) arg0, func_80032D7C);
        func_80041DD4(0, 0x20);
        arg0->unk26 = 0xC00;
        func_800420FC(0, arg0->unk24, arg0->unk26, arg0->unk28);
        D_8010B1A8 = 1;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void func_80032EF0(s32 arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (D_8010B1A2 == 0x3A) {
        func_80071824(arg0, func_80032E48);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80032F3C.s")

void func_80032FF0(void *arg0) {
    Struct33680 *temp = arg0;
    func_80042034(0);
    temp->unk18 += 0x24000;
    if (temp->unk18 >= 0x500000) {
        temp->unk18 = 0x500000;
        func_80071824((s32) temp, func_80032F3C);
    }
    func_8004209C(0, temp->unk18, temp->unk1C, temp->unk20);
    func_800428C8(0);
}

void func_8003306C(Struct33680 *arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (D_8010B1A2 == 0x37) {
        arg0->unk2A += 1;
        if (arg0->unk2A == 0x1E) {
            arg0->unk2A = 0;
            func_80071824((s32) arg0, func_80032FF0);
            D_8010B1A2 = 0x38;
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800330EC.s")

void func_800331A8(void *arg0) {
    if (D_8010B1A2 == 0x34) {
        func_80071824((s32) arg0, func_800330EC);
        *(s32 *)((s32)arg0 + 0x18) = 0x1900000;
        *(s32 *)((s32)arg0 + 0x20) = 0;
        D_8010B1A8 = 0;
        func_8003CD9C(0x1C, -0x30, 0, 1);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_8003320C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_8003329C.s")

void func_800333A8(void *arg0) {
    func_800428C8(0);
    if (D_8010B1A2 == 0x2F) {
        func_80071824((s32) arg0, func_8003329C);
        func_80041DD4(0, 0x4E);
        func_8003C0A4(8, -0x40, 0, 0);
    }
}

void func_8003340C(Struct33680 *arg0) {
    func_80042034(0);
    arg0->unk18 += 0x2000;
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
    if (D_8010B1A2 == 0x2D) {
        func_80071824((s32) arg0, func_800333A8);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033478.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033504.s")

void func_800335A4(s32 arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (D_8010B1A2 == 0x2A) {
        func_80071824(arg0, func_80033504);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800335F0.s")

void func_80033688(s32 arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (D_8010B1A2 == 0x28) {
        func_80071824(arg0, func_800335F0);
    }
}

void func_800336D4(Struct33680 *arg0) {
    func_80042034(0);
    arg0->unk18 += 0xFFFDA000;
    if (arg0->unk18 < (s32)0xFF700001) {
        arg0->unk18 = 0xFF700000;
        func_80071824((s32) arg0, func_80033688);
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033758.s")

void func_800337D4(void *arg0) {
    func_80042034(0);
    func_800428C8(0);
    if ((D_8010B1A2 = 0x27) != 0) {
        func_80071824((s32) arg0, func_80033758);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033828.s")

void func_800338B8(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800338C0.s")

void func_80033958(void *arg0) {
    func_80042034(0);
    func_800428C8(0);
    *(u16 *)((s32)arg0 + 0x2A) += 1;
    if (*(u16 *)((s32)arg0 + 0x2A) == 0x32) {
        *(u16 *)((s32)arg0 + 0x2A) = 0;
        func_80071824((s32) arg0, func_800338C0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800339B8.s")

void func_80033A44(void *arg0) {
    if (D_8010B1A2 == 0x22) {
        func_80071824((s32) arg0, func_800339B8);
        D_8010B1A8 = 0;
        func_8003CD9C(0x1C, -0x30, 0, 1);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033A94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033B20.s")

void func_80033BE0(void *arg0) {
    func_800428C8(0);
    if (D_8010B1A2 == 0x1E) {
        func_80071824((s32) arg0, func_80033B20);
        func_80041DD4(0, 0x2D);
        func_8003C0A4(8, -0x40, 0, 0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033C44.s")

void func_80033D0C(s32 arg0) {
    struct {
        s32 ret;
        s32 pad;
    } locals;

    locals.ret = func_80041FB4(0);
    func_800428C8(0);
    if (locals.ret == 1) {
        func_80071824(arg0, func_80033C44);
        func_80041DD4(0, 0x2F);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033D64.s")

void func_80033DE4(void *arg0) {
    func_80041FB4(0);
    func_800428C8(0);
    if (D_8010B1A2 == 0x1B) {
        *(u16 *)((s32)arg0 + 0x2A) = 0;
        func_80071824((s32) arg0, func_80033D64);
        func_80041DD4(0, 0x2D);
        func_8003C0A4(8, -0x40, 0, 0);
    }
}

void func_80033E54(void *arg0) {
    func_800428C8(0);
    if (D_8010B1A2 == 0x1A) {
        func_80071824((s32) arg0, func_80033DE4);
        func_80041DD4(0, 0x2C);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033EA4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033F5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034014.s")

void func_800340D8(void *arg0) {
    struct {
        s32 ret;
        s32 pad;
    } locals;

    locals.ret = func_80041FB4(0);
    func_800428C8(0);
    if (locals.ret == 1) {
        D_8010B1A4++;
        func_80071824((s32) arg0, func_80034014);
    }
}

void func_80034138(void *arg0) {
    func_800428C8(0);
    if (D_8010B1A2 == 0x19) {
        func_80071824((s32) arg0, func_800340D8);
        func_80041DD4(0, 0x2A);
    }
}

void func_80034188(Struct33680 *arg0) {
    func_80041FB4(0);
    func_800428C8(0);
    arg0->unk18 += 0xFFFD0000;
    if (D_8010B1A2 == 0x18) {
        func_80071824((s32) arg0, func_80034138);
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
}

void func_800341FC(s32 arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (D_8010B1A2 == 0x17) {
        func_80071824(arg0, func_80034188);
        func_80041DD4(0, 0x29);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034254.s")

void func_800342D8(s32 arg0) {
    if (func_80041FB4(0) == 1) {
        func_80071824(arg0, func_80034254);
        func_80041DD4(0, 0);
    }
    func_800428C8(0);
}

void func_80034328(void *arg0) {
    Struct33680 *temp = arg0;
    if (func_80041FB4(0) == 1) {
        temp->unk2A += 1;
        if (temp->unk2A == 0xF) {
            temp->unk2A = 0;
            func_80071824((s32) temp, func_800342D8);
            func_80041DD4(0, 0x23);
        }
    }
    func_800428C8(0);
}

void func_800343A0(void *arg0) {
    Struct33680 *temp = arg0;
    if (func_80041FB4(0) == 1) {
        temp->unk2A += 1;
        if (temp->unk2A == 0xF) {
            temp->unk2A = 0;
            func_80071824((s32) temp, func_80034328);
            func_80041DD4(0, 0x22);
        }
    }
    func_800428C8(0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034418.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800344CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034574.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034600.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800346D4.s")

void func_80034754(void *arg0) {
    Struct33680 *temp = arg0;
    func_80042034(0);
    func_800428C8(0);
    if (D_8010B1A2 == 0x12) {
        temp->unk2A += 1;
        if (temp->unk2A == 0x1E) {
            temp->unk2A = 0;
            func_80071824((s32) temp, func_800346D4);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800347C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034864.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034900.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800349A8.s")

void func_80034A60(s32 arg0) {
    if (func_80041FB4(0) == 1) {
        func_80071824(arg0, func_800349A8);
        func_80041DD4(0, 0x1A);
    }
    func_800428C8(0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034AB0.s")

void func_80034B60(void *arg0) {
    func_800428C8(0);
    *(u16 *)((s32)arg0 + 0x2A) += 1;
    if (*(u16 *)((s32)arg0 + 0x2A) == 0x19) {
        *(u16 *)((s32)arg0 + 0x2A) = 0;
        func_80071824((s32) arg0, func_80034AB0);
        func_80041DD4(0, 0x18);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034BC4.s")

void func_80034C50(s32 arg0) {
    if (func_80041FB4(0) == 1) {
        func_80071824(arg0, func_80034BC4);
        func_80041DD4(0, 0x17);
    }
    func_800428C8(0);
    func_800373AC(&D_8010B1C0);
}

void func_80034CAC(void *arg0) {
    if (D_8010B1A2 == 0xD) {
        func_80071824((s32) arg0, func_80034C50);
        func_80041DD4(0, 0x16);
        *(s16 *)((s32)arg0 + 0x2C) = 0x16;
    }
    func_800428C8(0);
    func_800373AC(&D_8010B1C0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034D10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034DA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034E60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034F2C.s")

void func_80034FC8(s32 arg0) {
    func_80041FB4(0);
    if (D_8010B1A2 == 5) {
        func_80071824(arg0, func_80034F2C);
        func_80041DD4(0, 2);
        func_8003D218(0x55, -0x62, 0);
    }
    func_800428C8(0);
}

void func_80035030(s32 arg0) {
    func_80041FB4(0);
    if (D_8010B1A2 == 3) {
        func_80071824(arg0, (void (*)()) func_80034FC8);
        func_80041DD4(0, 2);
        func_8003D218(0x55, -0x62, 0);
        *(u16 *)(((s32)arg0) + 0x2A) = 0;
    }
    func_800428C8(0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800350A4.s")

void func_80035150(void *arg0) {
    if (D_8010B1A2 == 2) {
        func_80071824((s32) arg0, func_800350A4);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80035184.s")
