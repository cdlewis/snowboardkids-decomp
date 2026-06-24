#include "common.h"

extern s16 D_801221D2;
extern u8 D_80121D80[];
extern char D_800E1700[];
extern void sprintf(char *, const char *, ...);
extern void func_80048278(s32, s32, char *, s32);
extern void func_8006D520(s32, s32);
extern void func_80082FC8(void *);

void func_80078250(void) {
    void *ptr;
    s16 temp_v0;

    func_8006D520(0, 2);
    ptr = D_80121D80;
    *(s16 *)((u8 *)ptr + 0x2EE) = 0;
    temp_v0 = *(s16 *)((u8 *)ptr + 0x2EE);
    *(s32 *)((u8 *)ptr + 0x1C) = 0;
    *(s32 *)((u8 *)ptr + 0x20) = 0;
    *(s32 *)((u8 *)ptr + 0x24) = 0;
    *(s16 *)((u8 *)ptr + 0x452) = 0;
    *(s32 *)((u8 *)ptr + 0x64) = 0xFFFA0000;
    *(s32 *)((u8 *)ptr + 0x58) = 0;
    *(s16 *)((u8 *)ptr + 0x300) = 0;
    *(s16 *)((u8 *)ptr + 0) = 0;
    *(s16 *)((u8 *)ptr + 0x2EC) = temp_v0;
    *(s16 *)((u8 *)ptr + 0x2EA) = temp_v0;
    func_80082FC8(ptr);
}

void func_800782B4(s32 arg0) {
    char buf[0x64];
    sprintf(buf, D_800E1700, D_801221D2);
    func_80048278(0x28, 0x28, buf, 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/78E50/func_800782FC.s")
