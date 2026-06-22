#include "common.h"

typedef struct {
    char pad[0x18];
    s32 unk18;
    s32 unk1C;
} Struct801235B8;

typedef struct {
    u16 unk0;
    u8 unk2;
} Struct8010AF80;

extern void func_800720E4(s32);
extern void func_800704F0(void);
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_800437F0(void *, void *, s32);
extern void func_80070EC0(s32);
extern void func_80071408(void *, s32, s32);
extern void func_8009956C(void *, s32);
extern void func_8007105C(void);
extern s32 func_80013F88(s32, s32, s32);
extern void func_80072138(s32, s32);
extern void func_80045914(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);

void func_8000C3C8(void);
void func_8000C52C(void);
extern void func_80030570(void);

extern Struct801235B8 *D_801235B8;
extern Struct8010AF80 D_8010AF80;
extern s8 D_800DEED4;
extern s16 D_800DEF14;
extern s32 D_801235B4;
extern f32 D_800E0A40;
extern u8 D_80123750;
extern u8 D_80123751;
extern s32 D_80123778;
extern u8 D_593D10;
extern u8 D_598A70;
extern u8 D_59AAA0;
extern u8 D_59DFE0;
extern u8 D_60F1A0;
extern u8 D_60F990;

void func_8000C280(void) {
    func_800720E4(3);
    func_800704F0();
    func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, D_800E0A40);
    D_800DEED4 = 0;
    D_801235B8->unk18 = 0xFF;
    D_801235B8->unk1C = 0;
    D_801235B4 = 0;
    D_8010AF80.unk2 = 0;
    D_8010AF80.unk0 = 0x100;
    func_800437F0(&D_593D10, &D_598A70, 0x22);
    func_800437F0(&D_598A70, &D_59AAA0, 0x23);
    func_800437F0(&D_59AAA0, &D_59DFE0, 0x24);
    func_800437F0(&D_60F1A0, &D_60F990, 0x29);
    func_80070EC0(0);
    func_80071408(&func_80030570, 0, 0x64);
    D_800DEF14 = D_801235B8->unk18;
    func_8009956C(&func_8000C3C8, 0);
    func_8007105C();
}

#pragma GLOBAL_ASM("asm/nonmatchings/CE80/func_8000C3C8.s")

void func_8000C52C(void) {
    if (D_801235B8->unk18 != 0xFF) {
        D_801235B8->unk18 = func_80013F88((s16) D_801235B8->unk18, 0x24, 1);
        if (D_801235B8->unk18 == 0xFF) {
            D_80123751 = 1;
            D_8010AF80.unk2 = 3;
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
