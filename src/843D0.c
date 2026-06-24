#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/843D0/func_800837D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/843D0/func_8008393C.s")

void func_80083CFC(u8 *arg0) {
    u8 *var_v0 = arg0 + 0x58C;

    *(s16 *)(arg0 + 0x2DA) = 0xF0;
    if (*(s32 *)(arg0 + 0x2FC) & 0x400) {
        *(s32 *)(var_v0 + 4) = 0x300000;
        *(s32 *)(var_v0 + 8) = 0;
        *(s32 *)(var_v0 + 0xC) = 0;
        *(s16 *)(var_v0 + 0x68) = 0;
        *(arg0 + 0x2DC) = 1;
    } else {
        *(s32 *)(var_v0 + 4) = 0xFFD00000;
        *(s32 *)(var_v0 + 8) = 0;
        *(s32 *)(var_v0 + 0xC) = 0;
        *(s16 *)(var_v0 + 0x68) = 0x800;
        *(s8 *)(arg0 + 0x2DC) = 0;
    }
    *(s16 *)(var_v0 + 0x6A) = 0;
    *(s32 *)(var_v0 + 0x10) = 0xF0000;
    *(s32 *)(var_v0 + 0x14) = 0x230000;
    *(s32 *)(var_v0 + 0x18) = 0;
    *(s16 *)(var_v0 + 0x74) = 1;
    *(s16 *)var_v0 = 1;
}
