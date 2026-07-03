#include "common.h"

typedef struct {
    u8 pad0[0x10];
    u16 unk10;
    u8 pad12[6];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[4];
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s16 unk38;
    s16 unk3A;
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
} Obj4A040;

extern s16 D_8011216C;
extern s16 D_80112168;
extern u8 D_80121D80[];
extern u8 D_80122282[];

s32 func_80043040(s16);
void func_80045990(s32, s32, void *, void *);
void func_8004B5F8(Obj4A040 *);
void func_8004D018(Obj4A040 *);
void func_80071824(Obj4A040 *, void *);
void func_80072A74(s32, void *, s32, s32);
void *func_80071408(void *, s32, s32);

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_80049440.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_80049664.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_800499A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_80049CE0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_80049FB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004A2F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004A648.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004A91C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004AC5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004AFE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004B2B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004B5F8.s")

void func_8004B83C(Obj4A040 *arg0) {
    arg0->unk38 = 0x3C;
    arg0->unk3A = -1;
    arg0->unk28 = 0xFFF00000;
    arg0->unk2C = 0;
    func_80045990(func_80043040(D_8011216C), 3, &arg0->unk30, &arg0->unk34);
    func_8004B5F8(arg0);
    func_80071824(arg0, func_8004B5F8);
}

void func_8004B8B4(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4) {
    Obj4A040 *obj = func_80071408(func_8004B83C, 0, 0x1E);

    if (obj != NULL) {
        obj->unk18 = arg0;
        obj->unk1C = arg1;
        obj->unk20 = arg2;
        obj->unk3C = arg4;
        obj->unk3E = arg3;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004B934.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004BC74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004BFA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004C274.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004C5B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004C8F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004CBC4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004CF28.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004D018.s")

void func_8004D184(Obj4A040 *arg0) {
    arg0->unk38 = 0x3C;
    arg0->unk3A = 0x708;
    arg0->unk2C = 0x30000;
    arg0->unk18 = *(s32 *)&D_80121D80[arg0->unk10 * 1548 + 0x1C8];
    arg0->unk1C = *(s32 *)&D_80121D80[arg0->unk10 * 1548 + 0x1CC];
    arg0->unk20 = *(s32 *)&D_80121D80[arg0->unk10 * 1548 + 0x1D0];
    func_80072A74(0x6A, &arg0->unk18, 0x7F, 0x32);
    arg0->unk40 = *(s16 *)&D_80122282[arg0->unk10 * 1548];
    func_80045990(func_80043040(D_80112168), 2, &arg0->unk30, &arg0->unk34);
    func_8004D018(arg0);
    func_80071824(arg0, func_8004D018);
}

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004D280.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004D5C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4A040/func_8004D880.s")
