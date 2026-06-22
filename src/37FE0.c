#include "common.h"

extern void func_80041DD4(s32 arg0, s32 arg1);
extern s32 func_80041FB4(s32 arg0);
extern void func_800428C8(s32 arg0);
extern void func_80071824(s32 arg0, void (*arg1)(void));

void func_800373E0(s32 arg0);
void func_80037770(s32 arg0);
void func_8003905C(s32 arg0);

void func_800373E0(s32 arg0) {
    func_80041FB4(2);
    func_800428C8(2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_8003740C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037484.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800374DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037548.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800375A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037620.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800376AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037708.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037770.s")

void func_800377C8(s32 arg0) {
    if (func_80041FB4(2) == 1) {
        func_80071824(arg0, func_80037770);
        func_80041DD4(2, 0x62);
    }
    func_800428C8(2);
}

void func_80037818(s32 arg0) {
    if (func_80041FB4(2) == 1) {
        func_80071824(arg0, (void (*)())func_800377C8);
        func_80041DD4(2, 0x23);
    }
    func_800428C8(2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037868.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800378E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037998.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037A58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037AA4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037B20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037B6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037C28.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037CAC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037D48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037E3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037EA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037F0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037F74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037FF8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038098.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800380F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038194.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800381E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_8003825C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800382BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038340.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800383AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038424.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038478.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038508.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_8003857C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038628.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038678.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038704.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800387D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038854.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800388C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038954.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800389B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038A54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038AE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038B44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038BBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038C2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038C9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038D38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038DD4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038E90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038F18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038FB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_8003905C.s")

void func_800390E8(s32 arg0) {
    if (func_80041FB4(2) == 1) {
        func_80071824(arg0, func_8003905C);
        func_80041DD4(2, 0);
    }
    func_800428C8(2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80039138.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800391B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80039228.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800392DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80039354.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800393D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80039440.s")
