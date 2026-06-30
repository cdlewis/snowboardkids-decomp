#include "common.h"

typedef struct {
    u8 pad0[0x10];
    s16 unk10;
} Struct71AC0;

void *func_800711D0(void *, s32, s32);
void *func_80071408(void *, s32, s32);

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_80070EC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_8007105C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_800710CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_8007115C.s")

void func_800711C8(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_800711D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_80071408.s")

void *func_80071664(void *arg0, s32 arg1, s32 arg2, s32 arg3){ void *t=func_80071408(arg0,arg1&0xFFFF,arg2); if(t!=NULL){((Struct71AC0 *)t)->unk10=arg3;} return t;}

void *func_800716A4(void *arg0, s32 arg1, s32 arg2, s32 arg3){ void *t=func_800711D0(arg0,arg1&0xFFFF,arg2); if(t!=NULL){((Struct71AC0 *)t)->unk10=arg3;} return t;}

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_800716E4.s")

void func_80071824(void *arg0, void (*arg1)(void)) {
    *(void (**)(void))((u8 *)arg0 + 8) = arg1;
}
