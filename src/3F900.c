#include "common.h"

extern u8 D_80123750;
extern s8 D_800EC8B0;
extern u8 D_80121B58;
extern u8 D_80123751;
extern s8 D_800DEED4;
extern s32 D_801235B4;
extern void func_80045914(void);
extern void func_80072260(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);

#pragma GLOBAL_ASM("asm/nonmatchings/3F900/func_8003ED00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3F900/func_8003EF7C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3F900/func_8003F00C.s")

void func_8003F4B4(void) {
    if (D_80123750 == 2) {
        D_800EC8B0 = 0;
        D_80121B58 = 0;
        func_80045914();
        D_80123751 = 0;
        D_800DEED4 = 0;
        func_80072260();
        D_801235B4 = 0;
        func_80099658(3);
        func_8009954C(4);
    }
}
