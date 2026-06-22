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

void func_800088C8(void);
void func_80008C84(void);
extern void func_80005C70(void);

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

#pragma GLOBAL_ASM("asm/nonmatchings/9220/func_800086EC.s")

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
