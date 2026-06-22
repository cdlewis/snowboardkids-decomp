#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/49A80/func_80048E80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/49A80/func_80049000.s")

#pragma GLOBAL_ASM("asm/nonmatchings/49A80/func_8004908C.s")

s32 func_8004908C(s32, s32);

s32 func_8004940C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return func_8004908C(arg2 - arg0, arg3 - arg1);
}
