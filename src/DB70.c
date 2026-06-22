#include "common.h"

typedef struct {
    char pad[0x18];
    s32 unk18;
    s32 unk1C;
} Struct801235B8;

extern void func_800704F0(void);
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_800437F0(void *, void *, s32);
extern void func_80070EC0(s32);
extern void func_80071408(void *, s32, s32);
extern void func_8009956C(void *, s32);
extern void func_8007105C(void);
extern s32 func_80013F88(s32, s32, s32);
extern void func_80045914(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);

void func_8000D048(void);
void func_8000D0D4(void);
void func_8000D17C(void);
void func_8000D1F4(void);
void func_8000D280(void);
extern void func_80031CD0(void);

extern Struct801235B8 *D_801235B8;
extern s8 D_800DEED4;
extern s16 D_800DEF14;
extern f32 D_800E0A60;
extern u8 D_80123750;
extern u8 D_80123751;
extern u8 D_5E26E0;
extern u8 D_5E34A0;
extern u8 D_5E0E40;

void func_8000CF70(void) {
    func_800704F0();
    func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, D_800E0A60);
    D_800DEED4 = 0;
    D_801235B8->unk18 = 0xFF;
    D_801235B8->unk1C = 0;
    D_800DEF14 = D_801235B8->unk18;
    func_800437F0(&D_5E26E0, &D_5E34A0, 0x26);
    func_80070EC0(0);
    func_80071408(&func_80031CD0, 0, 0x5E);
    func_8009956C(&func_8000D048, 0);
    func_8007105C();
}

void func_8000D048(void) {
    if (D_801235B8->unk18 != 0) {
        D_801235B8->unk18 = func_80013F88(D_801235B8->unk18, 0xF, 0);
    } else {
        D_801235B8->unk1C++;
    }
    if (D_801235B8->unk1C >= 0x50) {
        D_801235B8->unk1C = 0;
        func_8009956C(&func_8000D0D4, 0);
    }
    func_8007105C();
}

void func_8000D0D4(void) {
    if (D_801235B8->unk18 != 0xFF) {
        D_801235B8->unk18 = func_80013F88(D_801235B8->unk18, 0xF, 1);
        if (D_801235B8->unk18 == 0xFF) {
            func_80045914();
            func_800437F0(&D_5E0E40, &D_5E26E0, 0x26);
        }
    } else {
        func_8009956C(&func_8000D17C, 0);
    }
    func_8007105C();
}

void func_8000D17C(void) {
    D_801235B8->unk1C++;
    if (D_801235B8->unk1C >= 0x14) {
        D_801235B8->unk18 = 0xFF;
        D_800DEF14 = D_801235B8->unk18;
        D_801235B8->unk1C = 0;
        func_8009956C(&func_8000D1F4, 0);
    }
    func_8007105C();
}

void func_8000D1F4(void) {
    if (D_801235B8->unk18 != 0) {
        D_801235B8->unk18 = func_80013F88(D_801235B8->unk18, 0xF, 0);
    } else {
        D_801235B8->unk1C++;
    }
    if (D_801235B8->unk1C >= 0x50) {
        func_8009956C(&func_8000D280, 0);
    }
    func_8007105C();
}

void func_8000D280(void) {
    if (D_801235B8->unk18 != 0xFF) {
        D_801235B8->unk18 = func_80013F88(D_801235B8->unk18, 0xF, 1);
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
            func_80099658(0);
            func_8009954C(4);
        }
    }
}
