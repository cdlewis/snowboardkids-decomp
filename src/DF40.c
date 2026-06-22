#include "common.h"

typedef struct {
    char pad[0x18];
    s32 unk18;
    s32 unk1C;
} Struct801235B8;

typedef struct {
    char pad0[0x2C];
    void (*unk2C)(void);
} Struct801121E0;

extern s32 func_80013F88(s32, s32, s32);
extern void func_8009956C(void *, s32);
extern void func_80071408(void *, s32, s32);
extern void func_800720E4(s32);
extern void func_8007105C(void);
extern void func_80045914(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);

void func_8000D690(void);
void func_8000D724(void);
extern void func_8003BBBC(void);
extern void func_80035184(void);
extern void func_800362E8(void);
extern void func_80036FB4(void);
extern void func_80039440(void);
extern void func_8003B264(void);

extern Struct801235B8 *D_801235B8;
extern Struct801121E0 D_801121E0;
extern Struct801121E0 *D_800EC9C4;
extern u16 D_8010B1A2;
extern s8 D_800DEED4;
extern u8 D_80123750;
extern u8 D_80123751;

#pragma GLOBAL_ASM("asm/nonmatchings/DF40/func_8000D340.s")

void func_8000D590(void) {
    if (D_801235B8->unk1C != 0) {
        D_801235B8->unk1C = D_801235B8->unk1C - 1;
    } else {
        if (D_801235B8->unk18 != 0) {
            D_801235B8->unk18 = func_80013F88((s16) D_801235B8->unk18, 0x10, 0);
        } else {
            func_8009956C(&func_8000D690, 0);
            func_80071408(&func_8003BBBC, 0, 0x64);
            func_80071408(&func_80035184, 0, 0x64);
            func_80071408(&func_800362E8, 0, 0x64);
            func_80071408(&func_80036FB4, 0, 0x64);
            func_80071408(&func_80039440, 0, 0x64);
            func_80071408(&func_8003B264, 0, 0x64);
            func_800720E4(0xA);
        }
    }
    func_8007105C();
}

void func_8000D690(void) {
    if (D_8010B1A2 == 0x43) {
        D_801235B8->unk1C++;
        if (D_801235B8->unk1C == 0x12C) {
            D_801235B8->unk1C = 0;
            func_8009956C(&func_8000D724, 0);
        }
    }
    func_8007105C();
    D_800EC9C4 = &D_801121E0;
    D_800EC9C4->unk2C();
}

void func_8000D724(void) {
    if (D_801235B8->unk18 != 0xFF) {
        D_801235B8->unk18 = func_80013F88((s16) D_801235B8->unk18, 0x10, 1);
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
            func_80099658(2);
            func_8009954C(4);
        }
    }
}
