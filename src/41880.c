#include "common.h"

typedef struct Object41880 {
    char pad0[0xC];
    s32 unkC;
    s32 unk10;
    s16 unk14;
    s16 unk16;
    char pad18[0x11C];
    s32 unk134;
    s32 unk138;
    s32 unk13C;
    s16 unk140;
    s16 unk142;
    s16 unk144;
} Object41880;

extern Object41880 *func_80043040(s16);
extern s16 D_8011218A[];
extern void func_80041E90(Object41880 *arg0);
extern void func_800437F0(void *, void *, s32);
extern u8 D_5E34A0;
extern u8 D_608560;
extern u8 D_215BE0;
extern u8 D_21D9D0;

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_80040C80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_80040D94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_80041A20.s")

void func_80041CC0(void) {
    func_800437F0(&D_5E34A0, &D_608560, 0x3F);
}

void func_80041CF0(void) {
    func_800437F0(&D_215BE0, &D_21D9D0, 0x3F);
}

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_80041D20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_80041DD4.s")

void func_80041E60(s32 arg0) {
    func_80043040(D_8011218A[arg0]);
}

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_80041E90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_80041FB4.s")

void func_80042034(s32 arg0) {
    Object41880 *temp_v0;

    temp_v0 = func_80043040(D_8011218A[arg0]);
    temp_v0->unk14--;
    if (temp_v0->unk14 <= 0) {
        temp_v0->unk14 = temp_v0->unk16;
        temp_v0->unk10 = temp_v0->unkC;
    }
    func_80041E90(temp_v0);
}

void func_8004209C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    Object41880 *temp_v0;

    temp_v0 = func_80043040(D_8011218A[arg0]);
    temp_v0->unk134 = arg1;
    temp_v0->unk138 = arg2;
    temp_v0->unk13C = arg3;
}

void func_800420FC(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    Object41880 *temp_v0;

    temp_v0 = func_80043040(D_8011218A[arg0]);
    temp_v0->unk140 = arg1;
    temp_v0->unk142 = arg2;
    temp_v0->unk144 = arg3;
}

#pragma GLOBAL_ASM("asm/nonmatchings/41880/func_8004215C.s")
