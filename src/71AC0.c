#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_80070EC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_8007105C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_800710CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_8007115C.s")

void func_800711C8(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_800711D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_80071408.s")

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_80071664.s")

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_800716A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_800716E4.s")

void func_80071824(void *arg0, void (*arg1)(void)) {
    *(void (**)(void))((u8 *)arg0 + 8) = arg1;
}
