#include "common.h"

extern void func_80071824(void *, void *);
extern void func_800129DC(s16, s16, u8 *, s32, s32);
extern u8 D_800B79AC[];
extern s16 D_8010AF62;

typedef struct {
    char pad[0x18];
    s16 unk18;
    s16 unk1A;
    union {
        s8 s8;
        s16 s16;
    } unk1C;
    s8 unk1E;
    s8 unk1F;
    s8 unk20;
} Object2C160;

extern void func_8002B8B4(Object2C160 *);
extern void func_8002BB24(Object2C160 *);
extern void func_8002BDAC(Object2C160 *);
extern void func_8002C18C(void);
extern void func_8002C390(Object2C160 *);

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002B560.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002B8B4.s")

void func_8002BA00(Object2C160 *arg0) {
    arg0->unk18 = 0x90;
    arg0->unk1A = -0x20;
    arg0->unk1C.s8 = 0;
    func_80071824(arg0, func_8002B8B4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002BA38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002BB24.s")

void func_8002BC60(Object2C160 *arg0) {
    arg0->unk18 = -0x44;
    arg0->unk1A = -0x1E;
    arg0->unk1C.s16 = 0;
    arg0->unk1F = 0;
    func_80071824(arg0, func_8002BB24);
}

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002BC9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002BDAC.s")

void func_8002BF54(Object2C160 *arg0) {
    arg0->unk18 = -0x42;
    arg0->unk1A = -0xE;
    arg0->unk1C.s16 = 0x100;
    D_8010AF62 = 0x100;
    arg0->unk20 = 0;
    func_80071824(arg0, func_8002BDAC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002BF9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002C18C.s")

void func_8002C318(void *arg0) {
    *(s16 *)((s32)arg0 + 0x18) = -0x28;
    *(s16 *)((s32)arg0 + 0x1A) = 0xC;
    *(s16 *)((s32)arg0 + 0x1C) = 0xC;
    func_80071824(arg0, func_8002C18C);
}

void func_8002C350(Object2C160 *arg0) {
    func_800129DC(arg0->unk18, arg0->unk1A, D_800B79AC, 1, arg0->unk1C.s16);
}

#pragma GLOBAL_ASM("asm/nonmatchings/2C160/func_8002C390.s")

void func_8002C498(Object2C160 *arg0) {
    arg0->unk18 = -0x24;
    arg0->unk1A = -0x38;
    arg0->unk1C.s16 = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8002C390);
}
