#include "common.h"

typedef struct {
    char pad[0x1C];
    s16 unk1C;
    s16 unk1E;
    u16 unk20;
    u16 unk22;
    s16 unk24;
    u8 unk26;
} Struct8003BA64;

extern u16 D_8010B1A2;
extern s16 D_80112172;
extern void *D_80124868;
extern s32 func_80043040(s16);
extern void func_8000F8AC(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_80071824(void *arg0, void (*arg1)(void *));
extern void func_800483FC(void *, void *, void *);
extern void func_8003B7C0(void *arg0);
extern void func_8003B9F8(void *arg0);
void func_8003BA64(Struct8003BA64 *arg0);
extern void func_8003BC9C(void *arg0);

#pragma GLOBAL_ASM("asm/nonmatchings/3C3C0/func_8003B7C0.s")

void func_8003B944(Struct8003BA64 *arg0) {
    func_8000F8AC(arg0->unk1C, arg0->unk1E, func_80043040(D_80112172), 0x35, 0x20, 0x20, 0, arg0->unk24, 0);
    func_8000F8AC((s16)(arg0->unk1C + 0x40), arg0->unk1E, func_80043040(D_80112172), 0x36, 0x20, 0x20, 0, arg0->unk24, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3C3C0/func_8003B9F8.s")

void func_8003BA64(Struct8003BA64 *arg0) {
    switch (arg0->unk26) {
    case 0:
        arg0->unk24 += 0xA;
        if (!(arg0->unk24 < 0x100)) {
            arg0->unk24 = 0x100;
            arg0->unk26 = 1;
            arg0->unk22 = 0;
        }
        break;
    case 1:
        arg0->unk22 = arg0->unk22 + 1;
        if (arg0->unk22 == 0x96) {
            arg0->unk22 = 0;
            arg0->unk26 = 2;
        }
        break;
    case 2:
        arg0->unk24 -= 0xA;
        if (!(arg0->unk24 > 0)) {
            arg0->unk24 = 0;
            arg0->unk26 = 3;
            arg0->unk20 = arg0->unk20 + 1;
            if (arg0->unk20 == 0x19) {
                arg0->unk20 = 0;
                func_80071824(arg0, func_8003B9F8);
            }
            if (D_8010B1A2 == 0) {
                D_8010B1A2 = 1;
            }
        }
        break;
    case 3:
        arg0->unk22 = arg0->unk22 + 1;
        if (!(arg0->unk22 < 0x20)) {
            arg0->unk22 = 0;
            arg0->unk26 = 0;
        }
        break;
    }
    func_800483FC(&D_80124868, func_8003B7C0, arg0);
}

void func_8003BBBC(void *arg0) {
    *(s8 *)((s32)arg0 + 0x26) = 3;
    *(s16 *)((s32)arg0 + 0x20) = 0;
    *(s16 *)((s32)arg0 + 0x1C) = -0x40;
    *(s16 *)((s32)arg0 + 0x1E) = 0x10;
    *(s16 *)((s32)arg0 + 0x24) = 0;
    func_80071824(arg0, func_8003BA64);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3C3C0/func_8003BC00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3C3C0/func_8003BC9C.s")

void func_8003BEB4(void *arg0) {
    *(s16 *)((s32)arg0 + 0x18) = 0;
    *(s16 *)((s32)arg0 + 0x1A) = 0;
    *(s16 *)((s32)arg0 + 0x1C) = 0;
    *(s8 *)((s32)arg0 + 0x1E) = 0;
    *(s8 *)((s32)arg0 + 0x1F) = 0;
    func_80071824(arg0, func_8003BC9C);
}
