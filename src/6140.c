#include "common.h"

typedef struct {
    char pad[0x18];
    s32 unk18;
    s32 unk1C;
} Struct801235B8;

typedef struct {
    s8 unk0;
    char pad1[1];
    s16 unk2;
} Struct8010AE70;

extern void func_800720E4(s32);
extern void func_8009956C(void *, s32);
extern void func_8007105C(void);
extern s32 func_80013F88(s32, s32, s32);
extern void func_80045914(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);

void func_80005788(void);
void func_80005B80(void);
extern void func_800062F8(void);

extern Struct801235B8 *D_801235B8;
extern Struct8010AE70 D_8010AE70;
extern s8 D_800EC9C1;
extern u8 D_800EC9C2;
extern u8 D_800EC9DD;
extern u8 D_80121D88;
extern s32 D_8010ADDC;
extern s32 D_801235B4;
extern u8 D_8010ADF8;
extern s16 D_8010ADF0;
extern s16 D_800DEF14;
extern s8 D_800DEED4;
extern u8 D_80123750;
extern u8 D_80123751;

void func_80005540(void) {
    D_801235B8->unk18 = 1;
    func_800720E4(1);
    D_800EC9C1 = 0;
    D_80121D88 = 0;
    D_8010ADDC = 0;
    D_801235B8->unk1C = 0;
    D_801235B4 = 0;
    D_8010ADF8 = 0;
    D_800EC9DD = 0;
    D_8010ADF0 = 0;
    D_800DEF14 = D_801235B8->unk18;
    func_8009956C(&func_80005788, 0);
    D_8010AE70.unk0 = 0;
    D_8010AE70.unk2 = 0;
    func_8007105C();
}

#pragma GLOBAL_ASM("asm/nonmatchings/6140/func_800055EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/6140/func_80005788.s")

void func_80005B14(void) {
    u8 v0;

    v0 = D_800EC9C2;
    if ((v0 == 0 || v0 == 2) && D_8010ADF8 == 0) {
        func_8009956C(&func_800062F8, 0);
        D_801235B4 = 0;
    } else {
        func_8009956C(&func_80005B80, 0);
    }
    func_8007105C();
}

void func_80005B80(void) {
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
            if (D_8010ADF8 == 1) {
                D_801235B4 = 1;
            } else {
                D_801235B4 = 0;
            }
            func_80099658(2);
            func_8009954C(4);
        }
    }
}
