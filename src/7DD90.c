#include "common.h"

extern s32 D_80121B98;

typedef struct Struct7DD90 {
    char pad0[0x45E];
    s16 unk45E;
    char pad460[6];
    s16 unk466;
} Struct7DD90;

extern void func_80081EF4(Struct7DD90 *);
extern void func_80082070(Struct7DD90 *);

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_8007D190.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_8007D200.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_8007D548.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_8007D87C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_8007DC38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_8007ECF4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_8007FD88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_8007FF88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80080CC4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80081124.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_800813F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80081508.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_800815D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80081C44.s")

s16 func_80081E1C(s32 arg0) {
    int idx = arg0 * 0x1C;

    return *(s16 *)((D_80121B98 + idx) + 2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80081E40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80081EF4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80082070.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80082184.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80082664.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80082B58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80082DD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80082E48.s")

s32 func_80082EC0(Struct7DD90 *arg0) {
    s32 temp_v1;
    s32 var_t8;
    s16 temp_v0;

    temp_v1 = arg0->unk466;
    var_t8 = temp_v1 - 1;
    if (temp_v1 == 0) {
        temp_v0 = arg0->unk45E;
        if (temp_v0 != 1) {
            arg0->unk45E = temp_v0 - 1;
            func_80081EF4(arg0);
            if (arg0->unk45E == 1) {
                return 1;
            }
            temp_v1 = arg0->unk466;
            var_t8 = temp_v1 - 1;
        } else {
            return 1;
        }
    }
    arg0->unk466 = var_t8;
    return 0;
}

s32 func_80082F44(Struct7DD90 *arg0) {
    s32 temp_v1;
    s32 var_t8;
    s16 temp_v0;

    temp_v1 = arg0->unk466;
    var_t8 = temp_v1 - 1;
    if (temp_v1 == 0) {
        temp_v0 = arg0->unk45E;
        if (temp_v0 != 1) {
            arg0->unk45E = temp_v0 - 1;
            func_80082070(arg0);
            if (arg0->unk45E == 1) {
                return 1;
            }
            temp_v1 = arg0->unk466;
            var_t8 = temp_v1 - 1;
        } else {
            return 1;
        }
    }
    arg0->unk466 = var_t8;
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80082FC8.s")
