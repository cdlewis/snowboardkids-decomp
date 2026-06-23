#include "common.h"

typedef struct {
    char pad[0x18];
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s8 unk1E;
} Struct2D0E0;

extern void func_80071824(Struct2D0E0 *, void *);
extern void func_8002C9A0(void);
extern void func_8002FAB8(void);
extern void func_8002DF40(void);
extern void func_8002E114(void);
extern void func_8002E32C(void);
extern void func_8002E468(void);
extern void func_8002CFAC(void);
extern void func_8002D558(void);
extern void func_8002D9EC(void);
extern void func_8002DCE8(void);
extern void func_8002EF14(void);
extern void func_8002E9E4(void);
extern void func_8000F8AC(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 func_80043040(s16);
extern s32 D_800B34B0[];
extern u8 D_80121D86;
extern s16 D_8011217E;

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002C4E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002C624.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002C800.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002C860.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002C9A0.s")

void func_8002CAA0(Struct2D0E0 *arg0) {
    arg0->unk18 = -0x108;
    arg0->unk1A = 8;
    func_80071824(arg0, func_8002C9A0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002CAD4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002CFAC.s")

void func_8002D294(Struct2D0E0 *arg0) {
    arg0->unk18 = 0x94;
    arg0->unk1A = -0x48;
    arg0->unk1C = -8;
    *(s16 *)&arg0->unk1E = -0x74;
    *(s16 *)((char *)arg0 + 0x20) = 0;
    *(s8 *)((char *)arg0 + 0x23) = 0;
    *(s8 *)((char *)arg0 + 0x22) = 0;
    func_80071824(arg0, func_8002CFAC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002D2E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002D558.s")

void func_8002D734(Struct2D0E0 *arg0) {
    arg0->unk18 = -8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 1;
    *(s8 *)((char *)arg0 + 0x21) = 0;
    *(s8 *)((char *)arg0 + 0x20) = 0;
    func_80071824(arg0, func_8002D558);
}

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002D778.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002D9EC.s")

void func_8002DC14(Struct2D0E0 *arg0) {
    arg0->unk18 = -8;
    arg0->unk1A = -0x140;
    arg0->unk1C = 2;
    *(s8 *)((char *)arg0 + 0x21) = 0;
    *(s8 *)((char *)arg0 + 0x20) = 0;
    func_80071824(arg0, func_8002D9EC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002DC58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002DCE8.s")

void func_8002DE6C(Struct2D0E0 *arg0) {
    arg0->unk18 = -0x7C;
    arg0->unk1A = -0x50;
    arg0->unk1C = 0;
    arg0->unk1E = 0;
    *(s8 *)((char *)arg0 + 0x1F) = 0;
    func_80071824(arg0, func_8002DCE8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002DEAC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002DF40.s")

void func_8002E074(Struct2D0E0 *arg0) {
    arg0->unk18 = -0x84;
    arg0->unk1A = 0xC;
    arg0->unk1C = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8002DF40);
}

void func_8002E0B0(void *arg0) {
    func_8000F8AC(*(s16 *)((char *)arg0 + 0x18), *(s16 *)((char *)arg0 + 0x1A), func_80043040(D_8011217E), 0, 0x20, 0x20, 0, *(s16 *)((char *)arg0 + 0x1C), 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002E114.s")

void func_8002E214(Struct2D0E0 *arg0) {
    arg0->unk18 = -8;
    arg0->unk1A = -0x5C;
    arg0->unk1C = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8002E114);
}

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002E250.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002E32C.s")

void func_8002E42C(Struct2D0E0 *arg0) {
    arg0->unk18 = 0x30;
    arg0->unk1A = 0x40;
    arg0->unk1C = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8002E32C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002E468.s")

void func_8002E568(Struct2D0E0 *arg0) {
    arg0->unk18 = 0x30;
    arg0->unk1A = 0x40;
    arg0->unk1C = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8002E468);
}

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002E5A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002E6E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002E798.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002E810.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002E9E4.s")

void func_8002EC04(Struct2D0E0 *arg0) {
    arg0->unk18 = 0x20;
    arg0->unk1A = -8;
    *(s32 *)((char *)arg0 + 0x20) = D_800B34B0[D_80121D86];
    arg0->unk1C = 0;
    *(s8 *)((char *)arg0 + 0x24) = 0;
    func_80071824(arg0, func_8002E9E4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002EC5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002EF14.s")

void func_8002EFB8(Struct2D0E0 *arg0) {
    arg0->unk18 = -0x76;
    arg0->unk1A = -0x40;
    *(s32 *)((char *)arg0 + 0x20) = 0;
    arg0->unk1C = 0x100;
    *(s16 *)&arg0->unk1E = 0;
    func_80071824(arg0, func_8002EF14);
}

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002EFFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002F2C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002F854.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002F8DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002FAB8.s")

void func_8002FBC8(Struct2D0E0 *arg0) {
    arg0->unk18 = 0x90;
    arg0->unk1A = 0x44;
    *(s8 *)&arg0->unk1C = 0;
    func_80071824(arg0, func_8002FAB8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002FC00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002FD70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/2D0E0/func_8002FEF8.s")
