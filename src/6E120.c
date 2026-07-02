#include "common.h"

typedef struct {
    u16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    char padA[2];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    void (*unk2C)(void);
    char unk30[0x14];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    char unk50[0x44];
    s32 unk94;
    s32 unk98;
    s32 unk9C;
    s16 unkA0;
    char padA2[2];
    s32 unkA4;
    char padA8[8];
} Object6E120;

typedef struct {
    u8 pad0[0x290];
    s32 unk290;
    s32 unk294;
    s32 unk298;
} Player6E120;

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
} PlayerPos6E120;

typedef struct StackD7D4 {
    char sp28[0x20];
    s32 sp48;
    s32 sp4C;
    s32 sp50;
    s32 sp54;
    s32 sp58;
    s32 sp5C;
} StackD7D4;

extern void func_8006D8B4(Player6E120 *, s32);
extern void func_8006ECBC(void);
extern void func_8006EF1C(void);
extern void func_8006F048(void);
extern void func_8006F984(void);
extern void func_8006FA20(void);
extern void func_8006FE88(void);
extern void func_8007022C(void);
extern void func_8007031C(void);
extern void func_800704C4(void);
extern void func_800486BC(void *, void *);
extern void func_80097FE4(void *, s16, s16);
extern void func_8009853C(void *, s16, s16);
extern void func_80098590(void *, s32 *, s32 *);
extern Object6E120 D_801121E0[];
extern Object6E120 *D_801124A0;
extern Player6E120 D_80121D80[];
extern void *D_800DA880[];
extern s32 D_80121B40;
extern s32 D_80121B44;
extern s32 D_80121B48;
extern s8 D_80121B54;
extern u8 D_80121B58;

void func_8006D520(u16 arg0, u16 arg1) {
    Object6E120 *temp;

    if (D_80121B58 == 0) {
        temp = (Object6E120 *)((char *)D_801121E0 + (arg0 * 0xB0));
        temp->unk2 = arg1;
        temp->unk2C = D_800DA880[arg1];
    }
}

void func_8006D580(u16 arg0, u16 arg1) {
    Object6E120 *temp = (Object6E120 *)((char *)D_801121E0 + (arg0 * 0xB0));

    temp->unk2 = arg1;
    temp->unk2C = D_800DA880[arg1];
}

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006D5CC.s")

void func_8006D700(void) {
    Object6E120 *var_s1;
    s32 var_s0;

    var_s0 = 0;
    if (D_80121B54 > 0)
    {
 do { var_s1 = D_801121E0; do { (D_801124A0 = var_s1)->unk2C(); var_s0 += 1; var_s1 += 1; } while (var_s0 < D_80121B54); } while (0);
    }
}

void func_8006D780(s32 arg0) {
    D_801124A0 = (Object6E120 *)((char *)D_801121E0 + (arg0 * 0xB0));
    D_801124A0->unk2C();
}

void func_8006D7D4(void) {
    StackD7D4 stack;

    func_8009853C(D_801124A0->unk30, -D_801124A0->unk4, -D_801124A0->unk6);
    stack.sp54 = 0;
    stack.sp58 = 0;
    stack.sp5C = -D_801124A0->unk24;
    func_80097FE4(stack.sp28, D_801124A0->unk4, D_801124A0->unk6);
    func_80098590(stack.sp28, &stack.sp54, &stack.sp48);
    D_801124A0->unk44 = stack.sp48 - D_801124A0->unk18;
    D_801124A0->unk48 = stack.sp4C - D_801124A0->unk1C;
    D_801124A0->unk4C = stack.sp50 - D_801124A0->unk20;
    func_800486BC(stack.sp28, D_801124A0->unk50);
}

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006D8B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006DDB4.s")

void func_8006E2B4(void) {
}

void func_8006E2BC(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006E2C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006E534.s")

void func_8006EC64(void) {
}

void func_8006EC6C(void) {
}

void func_8006EC74(void) {
    D_801124A0->unk2C = func_8006ECBC;
    D_801124A0->unk2C();
}

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006ECBC.s")

void func_8006EED4(void) {
    D_801124A0->unk2C = func_8006EF1C;
    D_801124A0->unk2C();
}

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006EF1C.s")

void func_8006EFF4(void) {
    D_801124A0->unkA0 = 0x96;
    D_801124A0->unk2C = func_8006F048;
    D_801124A0->unk2C();
}

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006F048.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006F5B0.s")

void func_8006F8AC(void) {
}

void func_8006F8B4(void) {
}

void func_8006F8BC(void) {
    D_801124A0->unk18 = D_80121B40;
    D_801124A0->unk94 = D_801124A0->unk18;
    D_801124A0->unk1C = D_80121B44;
    D_801124A0->unk98 = D_801124A0->unk1C;
    D_801124A0->unk20 = D_80121B48;
    D_801124A0->unk9C = D_801124A0->unk20;
    D_801124A0->unk24 = 0;
    D_801124A0->unk28 = 0;
    D_801124A0->unk2C = func_8006F984;
    D_801124A0->unk2C();
}

void func_8006F984(void) {
    D_801124A0->unkC = ((Player6E120 *)((u8 *)D_80121D80 + (D_801124A0->unk0 * 0x60C)))->unk290;
    D_801124A0->unk10 = ((Player6E120 *)((u8 *)D_80121D80 + (D_801124A0->unk0 * 0x60C)))->unk294;
    D_801124A0->unk14 = ((Player6E120 *)((u8 *)D_80121D80 + (D_801124A0->unk0 * 0x60C)))->unk298;
    func_8006D8B4(D_80121D80, 0x60C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006FA20.s")

void func_8006FDC0(void) {
    D_801124A0->unk8 = 0;
    D_801124A0->unk24 = 0;
    D_801124A0->unk2C = func_8006FA20;
    D_801124A0->unkA0 = 0;
    D_801124A0->unk2C();
}

void func_8006FE24(void) {
    D_801124A0->unk24 = 0;
    D_801124A0->unk28 = 0;
    D_801124A0->unkA0 = 0;
    D_801124A0->unk2C = func_8006FE88;
    D_801124A0->unk2C();
}

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006FE88.s")

void func_80070198(void) {
    D_801124A0->unk4 = 0;
    D_801124A0->unk6 = 0;
    D_801124A0->unk8 = 0;
    D_801124A0->unk18 = 0;
    D_801124A0->unk1C = 0x310000;
    D_801124A0->unk20 = 0x900000;
    D_801124A0->unk24 = 0x670000;
    D_801124A0->unk2C = func_8007022C;
    D_801124A0->unk2C();
}

void func_8007022C(void) {
    func_8006D7D4();
}

void func_8007024C(void) {
    D_801124A0->unk18 = 0;
    D_801124A0->unk1C = 0;
    D_801124A0->unk20 = ((PlayerPos6E120 *)D_80121D80)->unk24 + 0xFE600000;
    D_801124A0->unkC = ((PlayerPos6E120 *)D_80121D80)->unk1C;
    D_801124A0->unk10 = ((PlayerPos6E120 *)D_80121D80)->unk20 + 0xF0000;
    D_801124A0->unk14 = ((PlayerPos6E120 *)D_80121D80)->unk24;
    D_801124A0->unkA0 = 0;
    D_801124A0->unk24 = 0;
    D_801124A0->unk28 = 0;
    D_801124A0->unkA4 = 0x10000;
    D_801124A0->unk2C = func_8007031C;
    D_801124A0->unk2C();
}

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8007031C.s")

void func_8007042C(void) {
    D_801124A0->unk4 = 0x70;
    D_801124A0->unk6 = 0;
    D_801124A0->unk8 = 0;
    D_801124A0->unk18 = 0;
    D_801124A0->unk1C = 0x490000;
    D_801124A0->unk20 = 0x900000;
    D_801124A0->unk24 = 0x500000;
    D_801124A0->unk2C = func_800704C4;
    D_801124A0->unk2C();
}

void func_800704C4(void) {
    func_8006D7D4();
}
