#include "common.h"

typedef struct {
    char pad[0x18];
    s32 unk18;
    s32 unk1C;
} Struct801235B8;

extern Struct801235B8 *D_801235B8;
extern u8 D_80123750;
extern s8 D_80123751;
extern s8 D_800DEED4;

typedef struct {
    char pad0[0x2];
    u8 unk2;
    u8 unk3;
} Struct8010AF90;

typedef struct {
    s8 unk0;
    char pad1[0x1];
    s16 unk2;
    s16 unk4;
    s16 unk6;
} Struct8010AF60;

typedef struct {
    char pad0[0x8];
    s8 unk8;
    char pad9[0x3];
    s32 unkC;
} Struct80121D80;

extern Struct8010AF90 D_8010AF90;
extern Struct8010AF60 D_8010AF60;
extern Struct80121D80 D_80121D80;
extern u8 D_8010AF92;
extern s8 D_8010AF93;
extern s32 D_801235B4;
extern s32 D_80123778;
extern s16 D_800EC9C8;
extern s16 D_800EC9D0;
extern u8 D_800EC9D8;
extern s8 D_800EC9C1;
extern s32 D_8010ADDC;
extern s32 D_8010ADE0;
extern s32 D_8010ADE4;
extern s32 D_8010ADE8;
extern s16 D_800DEF14;
extern s32 D_800EC9F4;
extern u8 D_593D10;
extern u8 D_598A70;
extern u8 D_59AAA0;
extern u8 D_59DFE0;
extern u8 D_60F1A0;
extern u8 D_60F990;
extern u8 func_8002BA00;
extern u8 func_8002C318;

extern s32 func_80013F88(s32, s32, s32);
extern void func_8007105C(void);
extern void func_80045914(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);
extern void func_80008F2C(void);
extern void func_80072138(s32, s32);
extern void func_8009956C(void *, s32);
extern void func_800704F0(void);
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_800437F0(void *, void *, s32);
extern void func_80070EC0(s32);
extern s32 func_80071408(void *, s32, s32);

void func_80008D60(void) {
    func_800704F0();
    func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    D_800DEED4 = 0;
    D_800EC9C8 = 0;
    D_800EC9D0 = 0;
    D_800EC9D8 = 0;
    D_80121D80.unk8 = 0;
    D_800EC9C1 = 0;
    D_801235B8->unk18 = 0xFF;
    D_8010ADDC = 0;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    D_8010ADE8 = 0;
    D_800DEF14 = D_801235B8->unk18;
    D_800EC9F4 = D_80121D80.unkC;
    func_800437F0(&D_59AAA0, &D_59DFE0, 0x21);
    func_800437F0(&D_59AAA0, &D_59DFE0, 0x24);
    func_800437F0(&D_593D10, &D_598A70, 0x22);
    func_800437F0(&D_598A70, &D_59AAA0, 0x23);
    func_800437F0(&D_60F1A0, &D_60F990, 0x29);
    func_80070EC0(0);
    D_8010ADDC = func_80071408(&func_8002BA00, 0, 0x61);
    D_8010ADE8 = func_80071408(&func_8002C318, 0, 0x60);
    D_8010AF60.unk0 = 0;
    D_8010AF60.unk2 = 0;
    D_8010AF60.unk4 = 0;
    D_8010AF60.unk6 = 0;
    D_8010AF90.unk3 = 0;
    D_8010AF90.unk2 = 0;
    func_8009956C(func_80008F2C, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/9960/func_80008F2C.s")

void func_800095DC(void) {
    s32 temp_v0 = D_801235B8->unk18;
    if (temp_v0 != 0xFF) {
        D_801235B8->unk18 = func_80013F88((s16)temp_v0, 0x20, 1);
        func_8007105C();
        if (D_801235B8->unk18 == 0xFF) {
            D_80123751 = 1;
        }
    } else if (D_80123750 == 2) {
        func_80045914();
        D_80123751 = 0;
        D_800DEED4 = 0;
        func_80099658(2);
        func_8009954C(4);
    }
}

void func_80009690(void) {
    if ((D_80123778 & 0x10800) && (D_8010AF90.unk2 != 0)) {
        D_8010AF90.unk2 = 0;
        func_80072138(0x19, 0x32);
    } else if ((D_80123778 & 0x20400) && (D_8010AF90.unk2 != 1)) {
        D_8010AF90.unk2 = 1;
        func_80072138(0x19, 0x32);
    }
    if ((D_80123778 & 0x8000) || (D_80123778 & 0x1000)) {
        func_80072138(0x18, 0x32);
        if (D_8010AF92 == 0) {
            D_8010AF93 = 0;
            func_8009956C(func_80008F2C, 0);
        } else {
            D_801235B4 = 1;
            func_8009956C(func_800095DC, 0);
        }
    } else if (D_80123778 & 0x4000) {
        func_80072138(0x18, 0x32);
        D_801235B4 = 1;
        func_8009956C(func_800095DC, 0);
    }
    func_8007105C();
}
