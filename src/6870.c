#include "common.h"

typedef struct {
    char pad[0x18];
    s32 unk18;
    s32 unk1C;
} Struct801235B8;

extern void func_8009956C(void *, s32);
extern void func_8007105C(void);
extern s32 func_80013F88(s32, s32, s32);
extern void func_80045914(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);

void func_80006E3C(void);
extern void func_80008620(void);
extern void func_80005540(void);

extern Struct801235B8 *D_801235B8;
extern s8 D_800DEED4;
extern u8 D_80121D88;
extern u8 D_80121B55;
extern s16 D_80121B50;
extern s16 *D_8010AE90;
extern s32 D_801235B4;
extern u8 D_8010ADF8;
extern u8 D_800EC9DD;
extern u8 D_80123750;
extern u8 D_80123751;

#pragma GLOBAL_ASM("asm/nonmatchings/6870/func_80005C70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6870/func_80005F18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6870/func_800062F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6870/func_800066CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6870/func_80006AE8.s")

void func_80006D70(void) {
    if (D_80121D88 == 8) {
        if (D_80121B55 >= 2) {
            func_8009956C(&func_80006E3C, 0);
            D_801235B4 = 1;
            D_8010ADF8 = 1;
        } else {
            if (D_800EC9DD == 0) {
                func_8009956C(&func_80005540, 0);
            } else {
                func_8009956C(&func_80008620, 0);
            }
            D_80121B50 = D_8010AE90[D_80121B50];
            D_801235B4 = 0;
        }
    }
    func_8007105C();
}

void func_80006E3C(void) {
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
            D_80121B50 = D_8010AE90[D_80121B50];
            func_80099658(2);
            func_8009954C(4);
        }
    }
}
