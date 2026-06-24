#include "common.h"

extern void *func_80043040(s16);
extern void func_8004215C(void *);
extern void func_800483FC(void *, void *, void *);
extern s16 D_8011218A[];
extern s32 D_801248B0;
extern void func_80042574(void);
extern void func_8004270C(void);

#pragma GLOBAL_ASM("asm/nonmatchings/43160/func_80042560.s")

#pragma GLOBAL_ASM("asm/nonmatchings/43160/func_80042574.s")

#pragma GLOBAL_ASM("asm/nonmatchings/43160/func_8004270C.s")

void func_800428C8(s32 arg0) {
    void *sp1C;
    void *temp_v0;

    temp_v0 = func_80043040(D_8011218A[arg0]);
    sp1C = temp_v0;
    func_8004215C(temp_v0);
    *(s16 *)((s32)sp1C + 0x8) = 0;
    func_800483FC(&D_801248B0, func_80042574, sp1C);
}

void func_80042920(s32 arg0, s32 arg1, s32 arg2) {
    void *sp1C;
    void *temp_v0;

    temp_v0 = func_80043040(D_8011218A[arg0]);
    sp1C = temp_v0;
    func_8004215C(temp_v0);
    *(s16 *)((s32)sp1C + 0x8) = 0;
    *(s16 *)((s32)sp1C + 0x4) = (s16)arg1;
    *(s16 *)((s32)sp1C + 0x6) = (s16)arg2;
    func_800483FC(&D_801248B0, func_8004270C, sp1C);
}

void func_8004298C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *sp1C;
    void *temp_v0;

    temp_v0 = func_80043040(D_8011218A[arg0]);
    sp1C = temp_v0;
    func_8004215C(temp_v0);
    *(s16 *)((s32)sp1C + 0x8) = 0;
    *(s16 *)((s32)sp1C + 0x4) = (s16)arg1;
    *(s16 *)((s32)sp1C + 0x6) = (s16)arg2;
    func_800483FC(&D_801248B0, func_8004270C, sp1C);
}

void func_80042A00(s32 arg0) {
    void *sp1C;
    void *temp_v0;

    temp_v0 = func_80043040(D_8011218A[arg0]);
    sp1C = temp_v0;
    func_8004215C(temp_v0);
    *(s16 *)((s32)sp1C + 0x8) = 0;
    func_800483FC(&D_801248B0, func_80042574, sp1C);
}

void func_80042A58(s32 arg0, s32 arg1) {
    void *sp1C;
    void *temp_v0;

    temp_v0 = func_80043040(D_8011218A[arg0]);
    sp1C = temp_v0;
    func_8004215C(temp_v0);
    *(s16 *)((s32)sp1C + 0x8) = (s16)arg1;
    func_800483FC(&D_801248B0, func_80042574, sp1C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/43160/func_80042AB4.s")
