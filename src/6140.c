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
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_800437F0(void *, void *, s32);
extern void func_80070EC0(s32);
extern void func_80071408(void *, s32, s32);
extern void func_800704F0(void);

void func_80005788(void);
void func_80005B80(void);
extern void func_800062F8(void);
void func_8001710C(void);

extern u8 D_800EC9DC;
extern f32 D_800E0990;
extern u8 D_5A1ED0;
extern u8 D_5C5320;
extern u8 D_593D10;
extern u8 D_598A70;
extern u8 D_59AAA0;
extern u8 D_59DFE0;
extern u8 D_59E7F0;
extern u8 D_245A80;
extern u8 D_24C8E0;

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

void func_800055EC(void) {
    func_800720E4(1);
    D_800EC9DC = 0;
    D_800EC9DD = 0;
    func_800704F0();
    func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, D_800E0990);
    D_800DEED4 = 0;
    D_801235B8->unk18 = 0xFF;
    func_800437F0(&D_5A1ED0, &D_5C5320, 0x21);
    func_800437F0(&D_593D10, &D_598A70, 0x22);
    func_800437F0(&D_598A70, &D_59AAA0, 0x23);
    func_800437F0(&D_59DFE0, &D_59E7F0, 0x26);
    func_800437F0(&D_245A80, &D_24C8E0, 0x1F);
    func_800437F0(&D_59AAA0, &D_59DFE0, 0x24);
    func_80070EC0(0);
    func_80071408(&func_8001710C, 0, 0x5E);
    D_800EC9C1 = 0;
    D_80121D88 = 0;
    D_8010ADDC = 0;
    D_801235B8->unk1C = 0;
    D_8010ADF8 = 0;
    D_8010ADF0 = 0;
    D_800DEF14 = D_801235B8->unk18;
    func_8009956C(&func_80005788, 0);
    D_8010AE70.unk0 = 0;
    D_8010AE70.unk2 = 0;
    func_8007105C();
}

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
