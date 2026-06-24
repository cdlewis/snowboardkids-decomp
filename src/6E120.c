#include "common.h"

typedef struct {
    char pad0[2];
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    char padA[0xE];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    void (*unk2C)(void);
    char pad30[0x70];
    s16 unkA0;
} Object6E120;

extern void func_8006D7D4(void);
extern void func_8006ECBC(void);
extern void func_8006EF1C(void);
extern void func_8006F048(void);
extern void func_8006FA20(void);
extern void func_8006FE88(void);
extern void func_8007022C(void);
extern Object6E120 D_801121E0[];
extern Object6E120 *D_801124A0;
extern void *D_800DA880[];
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

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006D700.s")

void func_8006D780(s32 arg0) {
    D_801124A0 = (Object6E120 *)((char *)D_801121E0 + (arg0 * 0xB0));
    D_801124A0->unk2C();
}

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006D7D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006D8B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006DDB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006E2B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006E2C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006E534.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006EC64.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006F8AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006F8BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8006F984.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8007024C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8007031C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6E120/func_8007042C.s")

void func_800704C4(void) {
    func_8006D7D4();
}
