#include "common.h"

typedef struct ObjectA3E0 {
    char pad0[0x2C];
    void (*unk2C)(void);
    char pad30[0x80];
} ObjectA3E0;

extern ObjectA3E0 *D_800EC9C4;
extern s16 D_8010ADF0;
extern u8 D_8010AF41;
extern s8 D_8010AF74;
extern ObjectA3E0 D_801121E0[];
extern u8 D_80121B55;
extern void func_8000B7B8(void);
extern void func_8007105C(void);
extern void func_8009956C(void *, s32);

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_800097E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_80009C48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000A048.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000A214.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000AFE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000B220.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000B690.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000B7B8.s")

void func_8000BAFC(void) {
    ObjectA3E0 *var_s1;
    s32 var_s0;

    if (D_8010AF41 == 2) {
        D_8010ADF0 = 0;
        D_8010AF74 = 0;
        func_8009956C(func_8000B7B8, 0);
    }
 do { var_s0 = 0; if (D_80121B55 > 0) { var_s1 = D_801121E0; do { ; (D_800EC9C4 = var_s1)->unk2C(); var_s0 += 1; var_s1 += 1; } while (var_s0 < D_80121B55); } } while (0);
    func_8007105C();
}

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000BBB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000BCA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000BEC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000C010.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000C114.s")
