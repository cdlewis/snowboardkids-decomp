#include "common.h"

extern s32 func_80041FB4(s32 arg0);
extern void func_800428C8(s32 arg0);

void func_800394E0(s32 arg0) {
    func_80041FB4(3);
    func_800428C8(3);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003950C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039584.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039610.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003969C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003973C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_800397C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039880.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039930.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003998C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039A14.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039A70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039B38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039B84.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039C84.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039CEC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039E08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039E5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039F2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039F7C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A010.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A078.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A108.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A190.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A21C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A2A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A324.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A3E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A46C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A538.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A598.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A634.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A6A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A70C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A77C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A7EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A8A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A944.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A9E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003AC00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003ACD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003AD40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003ADC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003AE8C.s")

extern void func_80041DD4(s32 arg0, s32 arg1);
extern void func_80071824(s32 arg0, void (*arg1)(void));
extern void func_8003AE8C(void);

void func_8003AF6C(void *arg0) {
    if (func_80041FB4(3) == 1) {
        *(u16 *)((s32)arg0 + 0x2C) = 0;
        func_80071824((s32)arg0, func_8003AE8C);
        func_80041DD4(3, 0x37);
    }
    func_800428C8(3);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003AFC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003B074.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003B134.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003B1F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003B264.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003B308.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003B39C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003B3E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003B430.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003B6F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003B740.s")
