#include "common.h"

extern s32 func_80011D74(void *, s32, s16, s16);
extern void func_80071824(void *, void *);
extern void func_80014AA4(void);
extern void func_80014EF0(void);
extern void func_800152D0(void);
extern void func_80015680(void);
extern void func_80015A30(void);
extern void func_80015BD8(void);

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80014600.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80014AA4.s")

void func_80014C7C(void *arg0) {
    *(s16 *)((s32)arg0 + 0x18) = -0x70;
    *(s16 *)((s32)arg0 + 0x1A) = -0x1C;
    *(s16 *)((s32)arg0 + 0x1C) = 0;
    *(s8 *)((s32)arg0 + 0x1E) = 0;
    func_80071824(arg0, func_80014AA4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80014CB8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80014EF0.s")

void func_80015054(void *arg0) {
    *(s16 *)((char *)arg0 + 0x18) = 0x90;
    *(s16 *)((char *)arg0 + 0x1A) = -0x18;
    *(s8 *)((char *)arg0 + 0x1C) = 0;
    func_80071824(arg0, func_80014EF0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_8001508C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_800152D0.s")

void func_80015404(void *arg0) {
    *(s16 *)((char *)arg0 + 0x18) = 0x90;
    *(s16 *)((char *)arg0 + 0x1A) = 8;
    *(s8 *)((char *)arg0 + 0x1C) = 0;
    func_80071824(arg0, func_800152D0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_8001543C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80015680.s")

void func_800157B4(void *arg0) {
    *(s16 *)((char *)arg0 + 0x18) = 0x90;
    *(s16 *)((char *)arg0 + 0x1A) = 0x28;
    *(s8 *)((char *)arg0 + 0x1C) = 0;
    func_80071824(arg0, func_80015680);
}

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_800157EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80015A30.s")

void func_80015B20(void *arg0) {
    *(s16 *)((char *)arg0 + 0x18) = 0x90;
    *(s16 *)((char *)arg0 + 0x1A) = 0x48;
    *(s8 *)((char *)arg0 + 0x1C) = 0;
    func_80071824(arg0, func_80015A30);
}

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80015B58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80015BD8.s")

void func_80015C84(void *arg0) {
    *(s16 *)((char *)arg0 + 0x18) = -0x68;
    *(s16 *)((char *)arg0 + 0x1A) = -0x14;
    *(s16 *)((char *)arg0 + 0x1C) = 0;
    func_80071824(arg0, func_80015BD8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80015CBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80015F4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_8001621C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80016284.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80016560.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_800165F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80016664.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80016948.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80016B54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80016BE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80016E40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80017014.s")

void func_80017078(void *arg0) {
    func_80011D74((void *)((s32)arg0 + 0x1C), 0, *(s16 *)((s32)arg0 + 0x18), *(s16 *)((s32)arg0 + 0x1A));
}

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_800170AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_8001710C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80017168.s")

void n_alSynFreeFX(s32 arg0) {

}
