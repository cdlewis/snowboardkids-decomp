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
} Struct8010AF90;

extern Struct8010AF90 D_8010AF90;
extern u8 D_8010AF92;
extern s8 D_8010AF93;
extern s32 D_801235B4;
extern s32 D_80123778;

extern s32 func_80013F88(s32, s32, s32);
extern void func_8007105C(void);
extern void func_80045914(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);
extern void func_80008F2C(void);
extern void func_80072138(s32, s32);
extern void func_8009956C(void (*)(void), s32);

#pragma GLOBAL_ASM("asm/nonmatchings/9960/func_80008D60.s")

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
