#include "common.h"

void osRecvMesg(void *, void *, s32);
void osSendMesg(void *, s32, s32, u16);
extern s32 D_800E4B78;
extern s32 D_800E4BB0;

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80000450.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_800005E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80000960.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_800009B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80000A40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80000A8C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80000C00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80000C48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80000C94.s")

void func_80000DB4(u16 arg0) {
    s32 sp1C;

    sp1C = 0;

    osSendMesg(&D_800E4B78, arg0 + 0x30, 1, arg0);
    osRecvMesg(&D_800E4BB0, &sp1C, 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80000E00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80001010.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_8000105C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_800012CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80001318.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80001538.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80001584.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80001618.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_8000165C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_800016D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80001724.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80001858.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_8000189C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80001904.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80001994.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80001C30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80001C80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80002024.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_800022B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_800024A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_8000262C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80002794.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80002810.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_800028B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80002A1C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80002CE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80002D50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80002DA0.s")
