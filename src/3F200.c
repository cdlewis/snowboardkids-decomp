#include "common.h"

typedef struct {
    char pad[0x24];
    s32 unk24;
} Struct801235B8;

extern Struct801235B8 *D_801235B8;
extern void func_800728E0(void);
extern void func_80071408(void *, s32, s32);
extern void func_8009956C(void *, s32);
extern void func_800540EC(void);
extern void func_8003EAF0(void);

#pragma GLOBAL_ASM("asm/nonmatchings/3F200/func_8003E600.s")

void func_8003EA78(void) {
    D_801235B8->unk24--;
    if (D_801235B8->unk24 == 0) {
        func_800728E0();
        func_80071408(func_800540EC, 0, 0x64);
        func_8009956C(func_8003EAF0, 0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/3F200/func_8003EAF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3F200/func_8003EC6C.s")
