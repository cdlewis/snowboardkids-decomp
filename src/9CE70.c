#include "common.h"

typedef struct Node9CE70 {
    struct Node9CE70 *next;
    void *mq;
} Node9CE70;

extern void osSendMesg(void *, s32, s32);
extern s32 osSetIntMask(s32);

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009C270.s")

s32 func_8009C434(s32 arg0) {
    return arg0 + 4;
}

s32 func_8009C43C(s32 arg0) {
    return arg0 + 0x5C;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009C444.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009C6DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009C77C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009C81C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009C8DC.s")

void func_8009CA60(void *arg0, Node9CE70 *arg1, void *arg2) {
    s32 prev = osSetIntMask(1);
    arg1->mq = arg2;
    arg1->next = *(Node9CE70 **)((u8 *)arg0 + 0x768);
    *(Node9CE70 **)((u8 *)arg0 + 0x768) = arg1;
    osSetIntMask(prev);
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009CAB4.s")

void func_8009CB44(void *arg0, s32 arg1) {
    Node9CE70 *node = *(Node9CE70 **)((u8 *)arg0 + 0x768);
    while (node != NULL) {
        osSendMesg(node->mq, arg1, 0);
        node = node->next;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009CB98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009CC50.s")

s32 func_8009CCA0(void *arg0, s32 arg1) {
    *(s32 *)((u8 *)arg0 + 0x60) = 0;
    *(s32 *)((u8 *)arg0 + 0x68) = 0;
    *(s32 *)((u8 *)arg0 + 0x54) = 0;
    *(u16 *)((u8 *)arg0 + 0xBE) = 0;
    *(s32 *)((u8 *)arg0 + 0x14) = 0;
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009CCC0.s")

s32 func_8009CCFC(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0xEA) = *arg1;
    return (s32)(arg1 + 1);
}

s32 func_8009CD0C(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xEA) = 0;
    return arg1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009CD18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009CE3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009CF1C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009CF30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009CF50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009CFAC.s")

s32 func_8009D00C(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xE9) = 0;
    *(f32 *)((u8 *)arg0 + 0x48) = 0.0f;
    return arg1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D020.s")

s32 func_8009D064(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xE6) = 1;
    return arg1;
}

s32 func_8009D074(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0x118) = *arg1;
    return (s32)(arg1 + 1);
}

s32 func_8009D084(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xE7) = 1;
    return arg1;
}

s32 func_8009D094(void *arg0, u8 *arg1) {
    s32 b;
    f32 f;
    b = *arg1;
    arg1++;
    if (b & 0x80) {
        b |= 0xFFFFFF00;
    }
    f = (f32)b;
    *(f32 *)((u8 *)arg0 + 0x2C) = f / 100.0;
    return (s32)arg1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D0E0.s")

s32 func_8009D138(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xE8) = 1;
    return arg1;
}

s32 func_8009D148(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xE8) = 0;
    return arg1;
}

s32 func_8009D154(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xE5) = 1;
    return arg1;
}

s32 func_8009D164(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xE5) = 0;
    return arg1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D170.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D1EC.s")

s32 func_8009D27C(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0x119) = arg1[0];
    *(u8 *)((u8 *)arg0 + 0xEB) = arg1[1];
    *(u8 *)((u8 *)arg0 + 0x106) = arg1[2];
    return (s32)(arg1 + 3);
}

s32 func_8009D2A0(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xEB) = 0;
    return arg1;
}

s32 func_8009D2AC(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xED) = 1;
    return arg1;
}

s32 func_8009D2BC(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xED) = 0;
    return arg1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D2C8.s")

s32 func_8009D2DC(void *arg0, u8 *arg1) {
    *(s8 *)((u8 *)arg0 + 0xF2) = (s32)(u8)*arg1 / 2;
    return (s32)(arg1 + 1);
}

s32 func_8009D2FC(s32 arg0, s32 arg1) {
    return arg1 + 2;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D308.s")

s32 func_8009D330(void *arg0, s32 arg1) {
    *(s32 *)((s32)arg0 + 0x70) = 0;
    return arg1;
}

s32 func_8009D33C(s32 arg0, s32 arg1) {
    return arg1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D348.s")

s32 func_8009D3AC(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0xF3) = *arg1;
    return (s32)(arg1 + 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D3BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D40C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D45C.s")

s32 func_8009D4AC(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0xEF) = *arg1;
    return (s32)(arg1 + 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D4BC.s")

s32 func_8009D598(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0xF1) = *arg1;
    return (s32)(arg1 + 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D5A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D8B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D8D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009DBE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009DC68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009DD5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009DDE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009DE50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009DEC4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009DF14.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009DF78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009DFDC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009E040.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009E0D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009E354.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009E76C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009E938.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009EB6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009EBDC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009EEE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009EF44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009EFF4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009F0C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009F194.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009F344.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009F4C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009F604.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009F6F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009F748.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009F780.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009F810.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009FC0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009FD74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009FF40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009FF80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_800A0138.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_800A0170.s")
