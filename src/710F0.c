#include "common.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    char pad7[1];
    s16 unk8;
    char padA[0xA];
    s16 unk14;
    s16 unk16;
    char pad18[4];
    s16 unk1C;
    s16 unk1E;
    char rest[0x90];
} Struct710F0;

extern Struct710F0 D_801124B0[];
extern u8 D_801124B2;

#pragma GLOBAL_ASM("asm/nonmatchings/710F0/func_800704F0.s")

void func_80070614(s32 arg0) {
    Struct710F0 *p = &D_801124B0[arg0];
    p->unk0 = 0;
    p->unk1 = 0;
    p->unk2 = 0;
    p->unk3 = 0;
    p->unk4 = 0;
    p->unk5 = 0;
    p->unk6 = 0;
    p->unk8 = 0;
    p->unk14 = 0x1FF;
    p->unk16 = 0;
    p->unk1C = 0x1FF;
    p->unk1E = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/710F0/func_8007066C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/710F0/func_80070860.s")

#pragma GLOBAL_ASM("asm/nonmatchings/710F0/func_80070A70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/710F0/func_80070C64.s")

void func_80070E58(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    D_801124B0[arg0].unk3 = 1;
    D_801124B0[arg0].unk4 = arg1;
    D_801124B0[arg0].unk5 = arg2;
    D_801124B0[arg0].unk6 = arg3;
}

void func_80070E90(s32 arg0) {
    D_801124B0[arg0].unk2 = 1;
}
