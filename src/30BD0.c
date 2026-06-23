#include "common.h"

extern void func_800483FC(void *, void *, s32);
extern s32 D_80124868;
extern s32 D_80124838;
extern void func_8003048C(void);
extern void func_80030CC4(void);
extern void func_80031038(void);
extern void func_80031294(void);
extern void func_80031370(void);
extern void func_80031F40(void);
extern void func_80032534(void);

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_8002FFD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_8003048C.s")

void func_80030570(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = -0x70;
    *(s16 *)((u8 *)arg0 + 0x1A) = -0x28;
    *(s32 *)((u8 *)arg0 + 0x20) = 0;
    *(u8 *)((u8 *)arg0 + 0x24) = 0;
    *(s16 *)((u8 *)arg0 + 0x1C) = 0x100;
    *(s16 *)((u8 *)arg0 + 0x1E) = 0;
    func_80071824(arg0, func_8003048C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_800305B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_80030CC4.s")

void func_80030EA8(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = -0x70;
    *(s16 *)((u8 *)arg0 + 0x1A) = -0x1C;
    *(u8 *)((u8 *)arg0 + 0x22) = 6;
    *(s16 *)((u8 *)arg0 + 0x1C) = 0;
    *(s16 *)((u8 *)arg0 + 0x20) = 0;
    *(s16 *)((u8 *)arg0 + 0x24) = 0;
    func_80071824(arg0, func_80030CC4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_80030EF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_80031038.s")

void func_800310EC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = -0x68;
    *(s16 *)((u8 *)arg0 + 0x1A) = -0x44;
    *(s16 *)((u8 *)arg0 + 0x1C) = 0x100;
    *(s16 *)((u8 *)arg0 + 0x1E) = 0;
    func_80071824(arg0, func_80031038);
}

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_8003112C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_80031294.s")

void func_80031330(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = -0x68;
    *(s16 *)((u8 *)arg0 + 0x1A) = 0x48;
    *(s16 *)((u8 *)arg0 + 0x1C) = 0x100;
    *(s16 *)((u8 *)arg0 + 0x1E) = 0;
    func_80071824(arg0, func_80031294);
}

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_80031370.s")

void func_800314E0(s32 arg0) {
    func_800483FC(&D_80124868, func_80031370, arg0);
}

void func_80031510(void *arg0) {
    s32 temp_v0 = 0x34;

    *(s16 *)((u8 *)arg0 + 0x18) = -0x24;
    *(s16 *)((u8 *)arg0 + 0x1A) = temp_v0;
    *(s16 *)((u8 *)arg0 + 0x1C) = 0x5C;
    *(s16 *)((u8 *)arg0 + 0x1E) = temp_v0;
    func_80071824(arg0, func_800314E0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_80031550.s")

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_80031A88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_80031B24.s")

void func_80031BA0(void *arg0) {
    void *temp = arg0;

    func_80011D74((u8 *)arg0 + 0x1C, 0, *(s16 *)((u8 *)temp + 0x18), *(s16 *)((u8 *)temp + 0x1A));
}

void func_80031BD4(s32 arg0) {
    func_800483FC(&D_80124838, func_80031BA0, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_80031C04.s")

void func_80031C6C(void *arg0) {
    void *temp = arg0;

    func_80011D74((u8 *)arg0 + 0x1C, 1, *(s16 *)((u8 *)temp + 0x18), *(s16 *)((u8 *)temp + 0x1A));
}

void func_80031CA0(s32 arg0) {
    func_800483FC(&D_80124838, func_80031C6C, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_80031CD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_80031D3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_80031F40.s")

void func_8003205C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = -0x70;
    *(s16 *)((u8 *)arg0 + 0x1A) = -0x1C;
    *(u8 *)((u8 *)arg0 + 0x20) = 0;
    *(s16 *)((u8 *)arg0 + 0x1C) = 0;
    *(s16 *)((u8 *)arg0 + 0x1E) = 0;
    func_80071824(arg0, func_80031F40);
}

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_8003209C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/30BD0/func_80032534.s")

void func_800325D0(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = -0x54;
    *(s16 *)((u8 *)arg0 + 0x1A) = -0x20;
    *(s16 *)((u8 *)arg0 + 0x1C) = 0x100;
    *(s16 *)((u8 *)arg0 + 0x1E) = 1;
    *(u8 *)((u8 *)arg0 + 0x20) = 0;
    func_80071824(arg0, func_80032534);
}
