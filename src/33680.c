#include "common.h"

extern void func_80033C44(void);
extern void func_80033D64(void);
extern void func_80033504(void);
extern void func_80034188(void);
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
void func_80032AF0(void);
void func_80032E48(void);
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

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80032BF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80032C74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80032CF4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80032D7C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80032E48.s")

void func_80032EF0(s32 arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (D_8010B1A2 == 0x3A) {
        func_80071824(arg0, func_80032E48);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80032F3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80032FF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_8003306C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800330EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800331A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_8003320C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_8003329C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800333A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_8003340C.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800336D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033758.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800337D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033828.s")

void func_800338B8(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800338C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033958.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800339B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033A44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033A94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033B20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033BE0.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033E54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033EA4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80033F5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034014.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800340D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034138.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034188.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034328.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800343A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034418.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800344CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034574.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034600.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_800346D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034754.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034B60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034BC4.s")

void func_80034C50(s32 arg0) {
    if (func_80041FB4(0) == 1) {
        func_80071824(arg0, func_80034BC4);
        func_80041DD4(0, 0x17);
    }
    func_800428C8(0);
    func_800373AC(&D_8010B1C0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80034CAC.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80035150.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33680/func_80035184.s")
