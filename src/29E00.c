#include "common.h"

extern void func_80071824(s32 arg0, void (*arg1)(void));
extern void func_800296D8(void);
extern void func_8002A27C(void);
extern void func_8002A710(void);
extern void func_8002AB24(void);
extern void func_8002AE3C(void);
extern void func_8002B05C(void);
extern void func_8002B1FC(void);
extern void func_8002B424(void);

struct Struct29E00 {
    char pad0[0x18];
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    char pad1E[2];
    /* 0x20 */ s8 unk20;
    /* 0x21 */ s8 unk21;
};

typedef struct Struct29E00 Struct29E00;

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_80029200.s")

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_80029344.s")

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_80029548.s")

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_80029598.s")

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_800296D8.s")

void func_800297D8(Struct29E00 *arg0) {
    arg0->unk18 = -0x108;
    arg0->unk1A = 8;
    func_80071824((s32) arg0, func_800296D8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002980C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_80029CE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_80029FB8.s")

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
    arg0->unk21 = 0;
    arg0->unk20 = 0;
    func_80071824((s32) arg0, func_8002AB24);
}

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002ADB8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002AE3C.s")

void func_8002AFB8(Struct29E00 *arg0) {
    arg0->unk18 = -0x7C;
    arg0->unk1A = -0x58;
    arg0->unk1C = 0;
    *((s8 *)arg0 + 0x1E) = 0;
    *((s8 *)arg0 + 0x1F) = 0;
    func_80071824((s32) arg0, func_8002AE3C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002AFF8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002B05C.s")

void func_8002B15C(Struct29E00 *arg0) {
    arg0->unk18 = -0x84;
    arg0->unk1A = 0xC;
    arg0->unk1C = 0;
    *((s8 *)arg0 + 0x1E) = 0;
    func_80071824((s32) arg0, func_8002B05C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002B198.s")

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002B1FC.s")

void func_8002B2FC(Struct29E00 *arg0) {
    arg0->unk18 = -8;
    arg0->unk1A = -0x5C;
    arg0->unk1C = 0;
    *((s8 *)arg0 + 0x1E) = 0;
    func_80071824((s32) arg0, func_8002B1FC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002B338.s")

#pragma GLOBAL_ASM("asm/nonmatchings/29E00/func_8002B424.s")

void func_8002B524(Struct29E00 *arg0) {
    arg0->unk18 = 0x30;
    arg0->unk1A = 0x40;
    arg0->unk1C = 0;
    *((s8 *)arg0 + 0x1E) = 0;
    func_80071824((s32) arg0, func_8002B424);
}
