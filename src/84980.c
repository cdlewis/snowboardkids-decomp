#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/84980/func_80083D80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/84980/func_8008409C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/84980/func_8008431C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/84980/func_80084510.s")

#pragma GLOBAL_ASM("asm/nonmatchings/84980/func_80084730.s")

typedef struct {
    char pad0[4];
    u8 unk4;
    char pad5[0x7F];
    s32 unk84;
    char pad88[4];
    s32 unk8C;
} Struct84958;

extern s32 func_800430D0(void);

s32 func_80084958(Struct84958 *arg0) {
    s32 sp18;
    s32 var_v1;
    s32 temp_v0;

    var_v1 = 0;
    if (arg0->unk4 != 0) {
        sp18 = 0;
        var_v1 = sp18;
        if (!(func_800430D0() & 3)) {
            return 1;
        }
    }
    temp_v0 = arg0->unk84 & ~arg0->unk8C;
    if (temp_v0 & 0xF0000) {
        var_v1 = 1;
    }
    if (temp_v0 & 0xC000) {
        var_v1++;
    }
    return var_v1;
}
