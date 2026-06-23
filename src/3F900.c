#include "common.h"

typedef struct {
    char pad[0x24];
    s32 unk24;
} Struct801235B8;

extern Struct801235B8 *D_801235B8;
extern s16 D_800DEF14;
extern f32 D_800E10C4;
extern u8 D_80123750;
extern s8 D_800EC8B0;
extern u8 D_80121B58;
extern u8 D_80123751;
extern s8 D_800DEED4;
extern s32 D_801235B4;
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_800720E4(s32);
extern void func_8009956C(void *, s32);
extern void func_80045914(void);
extern void func_80072260(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);
extern void func_8003F00C(void);

#pragma GLOBAL_ASM("asm/nonmatchings/3F900/func_8003ED00.s")

void func_8003EF7C(void) {
    D_801235B8->unk24--;
    if (D_801235B8->unk24 == 0) {
        func_8007066C(0, 0xA0, 0x78, 0x120, 0, 0x140, 0xF0, D_800E10C4);
        D_800DEF14 = 0;
        func_800720E4(0);
        func_8009956C(func_8003F00C, 0);
    }
}

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
