#include "common.h"

extern u8 *D_800D46D0[];
extern s16 D_80121B50;

u8 func_8004DB60(s32 arg0) {
    u8 *p = D_800D46D0[D_80121B50];
    return p[arg0];
}

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004DB8C.s")

extern void func_8004DB8C(void *);
extern void func_80071824(void *, void (*)());

void func_8004DC6C(void *arg0) {
    *(u16*)((u8*)arg0 + 0x64) = 0;
    func_8004DB8C(arg0);
    func_80071824(arg0, func_8004DB8C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004DCA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004E02C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004E3BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004E438.s")

extern void *func_800716A4(void *, s32, s32, s16);
extern void func_8004E438(void);

void func_8004E518(s16 arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4) {
    void *p = func_800716A4(func_8004E438, 5, 0x32, arg2);
    if (p != NULL) {
        *(s16 *)((u8 *)p + 0x34) = 0;
        *(s16 *)((u8 *)p + 0x36) = arg1;
        *(s16 *)((u8 *)p + 0x38) = arg0;
        *(s32 *)((u8 *)p + 0x28) = arg3;
        *(s32 *)((u8 *)p + 0x2C) = arg4;
    }
}

void func_8004E594(s32 arg0, s32 arg1, s32 arg2, s16 arg3) {
    void *p = func_800716A4(func_8004E438, 5, 2, arg3);
    if (p != NULL) {
        *(s32 *)((u8 *)p + 0x18) = arg0;
        *(s32 *)((u8 *)p + 0x1C) = arg1;
        *(s32 *)((u8 *)p + 0x20) = arg2;
        *(s16 *)((u8 *)p + 0x34) = 1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004E604.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004E960.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004E9D0.s")

extern void *func_800711D0(void *, s32, s32);
extern void func_8004E9D0(void);

void func_8004EA34(s32 arg0, s32 arg1, s32 arg2, s16 arg3) {
    void *p = func_800711D0(func_8004E9D0, 0, 2);
    if (p != NULL) {
        *(s8 *)((u8 *)p + 0x34) = arg3;
        *(s32 *)((u8 *)p + 0x18) = arg0;
        *(s32 *)((u8 *)p + 0x1C) = arg1;
        *(s32 *)((u8 *)p + 0x20) = arg2;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004EAA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004EE0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004EF24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004EFF8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004F33C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004F3FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004F55C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004F68C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004F9CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004FA44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004FB44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8004FF34.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_80050030.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8005019C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_80050340.s")

extern void func_80050340(void *);

void func_80050398(void *arg0) {
    *(u16*)((u8*)arg0 + 0x1C) = 0xFFFF;
    func_80050340(arg0);
    func_80071824(arg0, func_80050340);
}

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_800503D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_80050458.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_80050888.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_800508D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_8005098C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_80050D84.s")

#pragma GLOBAL_ASM("asm/nonmatchings/4E760/func_80050E80.s")
