#include "common.h"

extern void func_80041DD4(s32 arg0, s32 arg1);
extern s32 func_80041FB4(s32 arg0);
extern void func_800428C8(s32 arg0);
extern void func_80071824(s32 arg0, void (*arg1)(void));
extern void func_8003600C(void);

void func_80035220(s32 arg0) {
    func_80041FB4(1);
    func_800428C8(1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_8003524C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_800352C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035320.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_800353AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_8003542C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_800354BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_8003556C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035600.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_8003564C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_800356C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035714.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_800357A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_800357E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035878.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_8003592C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035990.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035A2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035AA4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035AFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035B88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035BF4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035C78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035CC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035D54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035DB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035E28.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035EA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035F28.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80035F80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_8003600C.s")

void func_800360CC(s32 arg0) {
    if (func_80041FB4(1) == 1) {
        func_80071824(arg0, func_8003600C);
        func_80041DD4(1, 0x13);
    }
    func_800428C8(1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_8003611C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80036198.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_80036218.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_800362B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/35E20/func_800362E8.s")
