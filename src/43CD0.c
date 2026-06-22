#include "common.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    char pad4[0x8];
} Node80110928;

extern u8 D_800D3F00[];
extern u16 D_800D4000;
extern u16 D_800D4004;
extern s16 D_800D4010;
extern s16 D_800D4014;
extern s16 D_800D4018;
extern s16 D_801235B0;
extern Node80110928 D_80110928[];

u8 func_800430D0(void) {
    D_800D4000++;
    if (D_801235B0 == 0) {
        D_800D4000++;
    }
    D_800D4000 &= 0xFF;
    return D_800D3F00[D_800D4000];
}

u8 func_80043120(void) {
    D_800D4004++;
    D_800D4004 &= 0xFF;
    return D_800D3F00[D_800D4004];
}

void func_80043154(void) {
    D_800D4004 = 0;
}

u8 func_80043160(u8 *arg0) {
    arg0[0x518]++;
    return D_800D3F00[arg0[0x518]];
}

void func_80043184(void) {
    D_801235B0 = 0;
    D_800D4000 = 0;
    func_80043154();
}

#pragma GLOBAL_ASM("asm/nonmatchings/43CD0/func_800431B0.s")

void func_8004331C(s16 arg0) {
    Node80110928 *v0;
    s16 a0;
    s16 v1;
    s16 a1 = -1;

    if (D_800D4010 != a1) {
        D_800D4018 = D_800D4018 - 1;
        v0 = &D_80110928[arg0];
        v1 = v0->unk2;
        if (a1 != v1) {
            D_80110928[v1].unk0 = v0->unk0;
            a0 = v0->unk0;
            v1 = v0->unk2;
        } else {
            a0 = v0->unk0;
            D_800D4010 = a0;
        }
        if (a1 != a0) {
            D_80110928[a0].unk2 = v1;
        } else {
            D_800D4014 = v1;
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/43CD0/func_800433D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/43CD0/func_800437F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/43CD0/func_800438EC.s")
