#include "common.h"

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ u8 unk1E;
    /* 0x1F */ u8 unk1F;
    char pad20[2];
    /* 0x22 */ u8 unk22;
    /* 0x23 */ u8 unk23;
} Actor1C970;

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u8 unk1C;
} Actor1C970B;

extern void func_80071824(void *, void *);

void func_8002172C(Actor1C970B *arg0);
void func_800219E4(Actor1C970B *arg0);
void func_80021C98(Actor1C970B *arg0);
void func_80023198(Actor1C970B *arg0);
void func_8002262C(Actor1C970 *arg0);
void func_80023434(Actor1C970 *arg0);
void func_80020DEC(Actor1C970 *arg0);
void func_8002127C(Actor1C970 *arg0);
void func_80021F80(Actor1C970 *arg0);
void func_80022274(Actor1C970 *arg0);
void func_8001D7B8(Actor1C970 *arg0);
void func_8001DD80(Actor1C970 *arg0);
void func_8001E258(Actor1C970 *arg0);
void func_8001E720(Actor1C970 *arg0);
void func_8001EBE8(Actor1C970 *arg0);
void func_8001F0B0(Actor1C970 *arg0);
void func_8001F578(Actor1C970 *arg0);
void func_8001FA40(Actor1C970 *arg0);
void func_8001FF08(Actor1C970 *arg0);
void func_800203D0(Actor1C970 *arg0);
void func_80020818(Actor1C970 *arg0);

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001BD70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001C158.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001C83C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001C96C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001CC10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001D254.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001D2F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001D7B8.s")

void func_8001DACC(Actor1C970 *arg0) {
    arg0->unk18 = 0x96;
    arg0->unk1A = -0x48;
    arg0->unk1C = 0;
    arg0->unk1F = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8001D7B8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001DB0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001DD80.s")

void func_8001DFA0(Actor1C970 *arg0) {
    arg0->unk18 = -0x8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 0x1;
    arg0->unk1F = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8001DD80);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001DFE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001E258.s")

void func_8001E468(Actor1C970 *arg0) {
    arg0->unk18 = -0x8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 0x2;
    arg0->unk1F = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8001E258);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001E4AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001E720.s")

void func_8001E930(Actor1C970 *arg0) {
    arg0->unk18 = -0x8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 0x3;
    arg0->unk1F = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8001E720);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001E974.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001EBE8.s")

void func_8001EDF8(Actor1C970 *arg0) {
    arg0->unk18 = -0x8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 0x4;
    arg0->unk1F = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8001EBE8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001EE3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001F0B0.s")

void func_8001F2C0(Actor1C970 *arg0) {
    arg0->unk18 = -0x8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 0x5;
    arg0->unk1F = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8001F0B0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001F304.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001F578.s")

void func_8001F788(Actor1C970 *arg0) {
    arg0->unk18 = -0x8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 0x6;
    arg0->unk1F = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8001F578);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001F7CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001FA40.s")

void func_8001FC50(Actor1C970 *arg0) {
    arg0->unk18 = -0x8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 0x7;
    arg0->unk1F = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8001FA40);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001FC94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8001FF08.s")

void func_80020118(Actor1C970 *arg0) {
    arg0->unk18 = -0x8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 0x8;
    arg0->unk1F = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8001FF08);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8002015C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_800203D0.s")

void func_800205E0(Actor1C970 *arg0) {
    arg0->unk18 = -0x8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 0x9;
    arg0->unk1F = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_800203D0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80020624.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80020818.s")

void func_80020AA0(Actor1C970 *arg0) {
    arg0->unk18 = -0x8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 0;
    arg0->unk23 = 0;
    arg0->unk22 = 0;
    func_80071824(arg0, func_80020818);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80020AE0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80020B70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80020CEC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80020D88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80020DEC.s")

void func_80020F44(Actor1C970 *arg0) {
    arg0->unk18 = -0x8;
    arg0->unk1A = -0x5C;
    arg0->unk1C = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_80020DEC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80020F80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8002127C.s")

void func_800213D4(Actor1C970 *arg0) {
    arg0->unk18 = 0x48;
    arg0->unk1A = 0x34;
    arg0->unk1C = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8002127C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80021410.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8002172C.s")

void func_8002186C(Actor1C970B *arg0) {
    arg0->unk18 = -0x108;
    arg0->unk1A = -0x50;
    arg0->unk1C = 0;
    func_80071824(arg0, func_8002172C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_800218A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_800219E4.s")

void func_80021B20(Actor1C970B *arg0) {
    arg0->unk18 = -0x108;
    arg0->unk1A = -0x18;
    arg0->unk1C = 0;
    func_80071824(arg0, func_800219E4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80021B58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80021C98.s")

void func_80021E70(Actor1C970B *arg0) {
    arg0->unk18 = -0x128;
    arg0->unk1A = 0x8;
    arg0->unk1C = 0;
    func_80071824(arg0, func_80021C98);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80021EA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80021F80.s")

void func_8002215C(Actor1C970 *arg0) {
    arg0->unk18 = -0x84;
    arg0->unk1A = -0x14;
    arg0->unk1C = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_80021F80);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80022198.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80022274.s")

void func_80022464(Actor1C970 *arg0) {
    arg0->unk18 = -0x84;
    arg0->unk1A = 0xC;
    arg0->unk1C = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_80022274);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_800224A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8002262C.s")

void func_800227A0(Actor1C970 *arg0) {
    arg0->unk18 = -0x74;
    arg0->unk1A = 0x2F;
    arg0->unk1C = 0x2F;
    func_80071824(arg0, func_8002262C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_800227D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80023198.s")

void func_800232F4(Actor1C970B *arg0) {
    arg0->unk18 = -0xF0;
    arg0->unk1A = -0x40;
    arg0->unk1C = 0;
    func_80071824(arg0, func_80023198);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_8002332C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80023434.s")

void func_800235E0(Actor1C970 *arg0) {
    arg0->unk18 = -0x108;
    arg0->unk1A = 0x28;
    arg0->unk1E = 0;
    func_80071824(arg0, func_80023434);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1C970/func_80023618.s")
