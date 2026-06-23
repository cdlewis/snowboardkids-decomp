#include "common.h"

typedef struct {
    char pad[0x18];
    s32 unk18;
    s32 unk1C;
    char pad20[0x4];
    s32 unk24;
} Struct801235B8;

extern Struct801235B8 *D_801235B8;
extern s16 D_800DEF14;
extern u8 D_800DEF10;
extern u8 D_80123751;
extern s32 D_80123778;
extern void func_800728E0(void);
extern void func_80071408(void *, s32, s32);
extern void func_8009956C(void *, s32);
extern void func_800540EC(void);
extern void func_8008C704(void);
extern void func_800710CC(s32);
extern void func_80096E3C(void);
extern void func_8007115C(void);
extern void func_8006D700(void);
extern void func_8007AA50(void);
extern void func_80072114(s32);
extern void func_8003EC6C(void);
void func_8003EAF0(void);

#pragma GLOBAL_ASM("asm/nonmatchings/3F200/func_8003E600.s")

void func_8003EA78(void) {
    D_801235B8->unk24--;
    if (D_801235B8->unk24 == 0) {
        func_800728E0();
        func_80071408(func_800540EC, 0, 0x64);
        func_8009956C(func_8003EAF0, 0);
    }
}

void func_8003EAF0(void) {
    Struct801235B8 *v0;
    s32 v1;

    if (D_801235B8->unk1C == 0) {
        D_800DEF14 -= 0x10;
        if (D_800DEF14 < 0) {
            D_800DEF14 = 0;
        }
    }
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    func_8007AA50();
    v0 = D_801235B8;
    if (v0->unk18 != 0) {
        v0->unk18 = v0->unk18 - 1;
        v0 = D_801235B8;
        v1 = v0->unk1C;
    } else {
        v1 = v0->unk1C;
        if (v1 == 0) {
            v0->unk1C = 4;
            func_80072114(0x78);
            v0 = D_801235B8;
            v1 = v0->unk1C;
        }
    }
    if ((D_80123778 & 0x1000) && (v1 == 0)) {
        v0->unk1C = 0x10;
        func_80072114(0x1E);
        v0 = D_801235B8;
        v1 = v0->unk1C;
    }
    if (v1 != 0) {
        D_800DEF10 = 1;
        D_800DEF14 += v0->unk1C;
        if (!(D_800DEF14 < 0xFF)) {
            D_800DEF14 = 0xFF;
            D_80123751 = 1;
            func_8009956C(func_8003EC6C, 0);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/3F200/func_8003EC6C.s")
