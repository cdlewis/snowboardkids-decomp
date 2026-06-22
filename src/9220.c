#include "common.h"

typedef struct {
    char pad[0x18];
    s32 unk18;
    s32 unk1C;
} Struct801235B8;

typedef struct {
    s16 unk0;
    s8 unk2;
} Struct8010AF50;

extern void func_800720E4(s32);
extern void func_8009956C(void *, s32);
extern void func_8007105C(void);
extern void func_80072114(s32);
extern s32 func_80013F88(s32, s32, s32);
extern void func_80045914(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);
extern void func_800704F0(void);
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_800437F0(void *, void *, s32);
extern void func_80070EC0(s32);
extern void func_80071408(void *, s32, s32);
extern u8 func_8001710C;

void func_800088C8(void);
void func_80008C84(void);
extern void func_80005C70(void);

extern f32 D_800E09D0;
extern u8 D_245A80;
extern u8 D_24C8E0;
extern u8 D_593D10;
extern u8 D_598A70;
extern u8 D_59AAA0;
extern u8 D_59DFE0;
extern u8 D_59E7F0;
extern u8 D_5A1ED0;
extern u8 D_5C5320;
extern u8 D_5CCD40;
extern u8 D_5D4280;

extern Struct801235B8 *D_801235B8;
extern Struct8010AF50 D_8010AF50;
extern s8 D_800EC9C1;
extern u8 D_80121D88;
extern s32 D_8010ADDC;
extern u8 D_8010ADF8;
extern s16 D_8010ADF0;
extern s16 D_80121B50;
extern s16 D_800DEF14;
extern u8 D_80121B5E;
extern u8 D_800EC9DD;
extern s8 D_800DEED4;
extern s32 D_801235B4;
extern u8 D_80123750;
extern u8 D_80123751;

void func_80008620(void) {
    func_800720E4(4);
    D_800EC9C1 = 0;
    D_80121D88 = 0;
    D_8010ADDC = 0;
    D_801235B8->unk18 = 1;
    D_801235B8->unk1C = 0;
    D_8010ADF8 = 0;
    D_8010ADF0 = 0;
    if (D_80121B50 == 7) {
        D_80121B50 = 9;
    }
    D_800DEF14 = D_801235B8->unk18;
    if (D_80121B5E == 3) {
        D_80121B5E = 0;
    }
    func_8009956C(&func_800088C8, 0);
    func_8007105C();
    D_8010AF50.unk2 = 0;
    D_8010AF50.unk0 = 0;
}

void func_800086EC(void) {
    func_800720E4(4);
    func_800704F0();
    func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, D_800E09D0);
    D_800DEED4 = 0;
    D_801235B8->unk18 = 0xFF;
    func_800437F0(&D_5A1ED0, &D_5C5320, 0x21);
    func_800437F0(&D_593D10, &D_598A70, 0x22);
    func_800437F0(&D_598A70, &D_59AAA0, 0x23);
    func_800437F0(&D_59AAA0, &D_59DFE0, 0x24);
    func_800437F0(&D_59DFE0, &D_59E7F0, 0x26);
    func_800437F0(&D_5CCD40, &D_5D4280, 0x25);
    func_800437F0(&D_245A80, &D_24C8E0, 0x1F);
    func_80070EC0(0);
    func_80071408(&func_8001710C, 0, 0x5E);
    D_800EC9C1 = 0;
    D_80121D88 = 0;
    D_8010ADDC = 0;
    D_801235B8->unk1C = 0;
    D_8010ADF8 = 0;
    D_8010ADF0 = 0;
    if (D_80121B50 == 7) {
        D_80121B50 = 9;
    }
    D_800DEF14 = D_801235B8->unk18;
    if (D_80121B5E == 3) {
        D_80121B5E = 0;
    }
    func_8009956C(func_800088C8, 0);
    func_8007105C();
    D_8010AF50.unk2 = 0;
    D_8010AF50.unk0 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9220/func_800088C8.s")

void func_80008C04(void) {
    if (D_80121B5E < 3 && D_8010ADF8 == 0) {
        func_8009956C(&func_80005C70, 0);
        D_800EC9DD = 1;
    } else {
        func_8009956C(&func_80008C84, 0);
        func_80072114(8);
        D_800EC9DD = 1;
    }
    func_8007105C();
}

void func_80008C84(void) {
    if (D_801235B8->unk18 != 0xFF) {
        D_801235B8->unk18 = func_80013F88((s16) D_801235B8->unk18, 0x24, 1);
        if (D_801235B8->unk18 == 0xFF) {
            D_80123751 = 1;
        } else {
            func_8007105C();
        }
    } else {
        if (D_80123750 == 2) {
            func_80045914();
            D_80123751 = 0;
            D_800DEED4 = 0;
            D_801235B4 = 1;
            func_80099658(2);
            func_8009954C(4);
        }
    }
}
