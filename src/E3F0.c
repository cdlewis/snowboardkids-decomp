#include "common.h"

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s32 unk1C;
    char pad20[4];
    /* 0x24 */ u8 unk24;
    char pad25[1];
    /* 0x26 */ s16 unk26;
    char pad28[2];
    /* 0x2A */ u8 unk2A;
    char pad2B[1];
    /* 0x2C */ u8 unk2C;
} ActorE3F0;

extern void func_800483FC(void *, void *, s32);
extern s32 D_80124868;
extern u8 D_8010B1F0;
extern u8 D_800B4FB8[];
extern u8 D_800B3500[];
extern u8 D_800B5038[];
extern u8 D_800B5050[];
extern s16 D_800B51B6[];
extern u8 D_80121B5A;
extern void func_8001303C(s32, s32, u8 *, s32, s32, s32, s32);
extern void func_800716E4(ActorE3F0 *);
void func_8000DF9C(ActorE3F0 *);
extern void func_80071824(ActorE3F0 *, void (*)(ActorE3F0 *));
void func_8000E5A0(ActorE3F0 *);
void func_8000E8CC(ActorE3F0 *);
void func_8000E99C(ActorE3F0 *);
void func_8000E9F4(ActorE3F0 *);

#pragma GLOBAL_ASM("asm/nonmatchings/E3F0/func_8000D7F0.s")

void func_8000DD74(s32 arg0) {
    func_800483FC(&D_80124868, func_8000DD74, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/E3F0/func_8000DDA4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/E3F0/func_8000DF28.s")

#pragma GLOBAL_ASM("asm/nonmatchings/E3F0/func_8000DF9C.s")

void func_8000E548(ActorE3F0 *arg0) {
    arg0->unk1A += 0x10;
    if (arg0->unk1A >= 0x79) {
        func_800716E4(arg0);
    } else {
        func_800483FC(&D_80124868, func_8000DF9C, (s32)arg0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/E3F0/func_8000E5A0.s")

void func_8000E7CC(ActorE3F0 *arg0) {
    arg0->unk18 = -0x80;
    arg0->unk1A = 4;
    arg0->unk2A = 0;
    arg0->unk24 = 1;
    arg0->unk26 = 0;
    arg0->unk1C = (s32)&D_800B3500[(D_80121B5A * 0x2F8) - 0x2F8];
    arg0->unk2C = 0;
    func_80071824(arg0, func_8000E5A0);
}

void func_8000E844(s32 arg0) {
    func_800483FC(&D_80124868, func_8000DF9C, arg0);
}

void func_8000E874(ActorE3F0 *arg0) {
    arg0->unk18 = -0x80;
    arg0->unk1A = -0x24;
    arg0->unk2A = 0;
    arg0->unk24 = 1;
    arg0->unk26 = 7;
    arg0->unk1C = (s32)D_800B4FB8;
    arg0->unk2C = 1;
    func_80071824(arg0, func_8000E5A0);
}

void func_8000E8CC(ActorE3F0 *arg0) {
    *(s16 *)&D_800B5038[0x12] = D_80121B5A;
    func_8001303C((s16)(arg0->unk18 - 0x10), arg0->unk1A, D_800B5038, 0, 0x100, 5, 0x29);
    func_8001303C(D_800B51B6[D_80121B5A], (s16)(arg0->unk1A + 0x18), &D_800B5050[(D_80121B5A * 0x28) - 0x28], 0, 0x100, 4, 0x29);
}

void func_8000E99C(ActorE3F0 *arg0) {
    arg0->unk1A -= 0xA;
    if (arg0->unk1A < -0xC7) {
        func_800716E4(arg0);
    } else {
        func_800483FC(&D_80124868, func_8000E8CC, (s32)arg0);
    }
}

void func_8000E9F4(ActorE3F0 *arg0) {
    if (D_8010B1F0 == 1) {
        func_80071824(arg0, func_8000E99C);
    }
    func_800483FC(&D_80124868, func_8000E8CC, (s32)arg0);
}

void func_8000EA44(ActorE3F0 *arg0) {
    arg0->unk18 = -0x48;
    arg0->unk1A = -0x48;
    arg0->unk1C = 0x78;
    func_80071824(arg0, func_8000E9F4);
}
