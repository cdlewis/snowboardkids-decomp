#include "common.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} StructD5CC8;

typedef struct {
    char pad0[0x10];
    u16 unk10;
    char pad12[0x6];
    StructD5CC8 unk18;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    char pad30[0x8];
    s16 unk38;
} Struct56070;

extern void *D_801248D4;
extern void func_800483FC(void *, void *, s32);
extern void func_800556B0(void);
extern void func_80056CA0(void);
extern s16 D_8011216A;
extern s32 func_80043040(s16 arg0);
extern s32 func_800430D0(void);
extern void func_80045990(s32 arg0, s32 arg1, void *arg2, void *arg3);
extern void func_80097C18(void *, s32);
extern void func_80098590(void *, void *, StructD5CC8 *);
extern StructD5CC8 D_800D5CC8[];
void func_80055FA4(Struct56070 *arg0);
void func_80056070(Struct56070 *arg0);
extern void func_80055C7C(void);

extern void func_80056348(Struct56070 *arg0, s16 arg1);
extern void func_800563C4(void *arg0, s32 arg1);
extern void func_80056444(void *arg0, s32 arg1);
extern void func_800564A8(void *arg0);
extern void func_80071824(void *arg0, void (*arg1)(void));
extern s8 D_80122288;
extern s16 D_80122282;
extern s16 D_80121B50;

void func_8005663C(void *);
extern void func_8005673C(void *);
void func_80056998(void *);
void func_80056A58(void *);
void func_80056B1C(void *);
void func_80056BD8(void *);

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_800556B0.s")

void func_80055B04(s32 arg0) {
    func_800483FC(&D_801248D4, func_800556B0, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80055B34.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80055C7C.s")

void func_80055FA4(Struct56070 *arg0) {
    StructD5CC8 sp44;
    char sp24[0x20];
    Struct56070 *temp_s0 = arg0;

    arg0->unk38--;
    if (arg0->unk38 == 0) {
        arg0->unk38 = func_800430D0() + 0x1E;
        func_80071824(arg0, func_80056070);
    }
    func_80097C18(sp24, 0x6D0);
    func_80098590(sp24, &temp_s0->unk24, &sp44);
    temp_s0->unk18.unk0 += sp44.unk0;
    temp_s0->unk18.unk4 += sp44.unk4;
    temp_s0->unk18.unk8 += sp44.unk8;
    temp_s0->unk28 -= 0x4000;
    func_800483FC(&D_801248D4, func_80055C7C, (s32) temp_s0);
}

void func_80056070(Struct56070 *arg0) {
    StructD5CC8 *temp_t3;

    arg0->unk38--;
    if (!arg0->unk38) {
        arg0->unk38 = 0x5A;
        temp_t3 = &D_800D5CC8[arg0->unk10];
        arg0->unk18 = *(&D_800D5CC8[arg0->unk10]);
        arg0->unk24 = 0;
        arg0->unk28 = 0;
        arg0->unk2C = 0xFFF00000;
        func_80071824(arg0, func_80055FA4);
    }
}

void func_800560F4(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x38) = (*(u16 *)((u8 *)arg0 + 0x10) * 0x1E) + 0x1E;
    func_80045990(func_80043040(D_8011216A), (*(u16 *)((u8 *)arg0 + 0x10) + 3) & 0xFFFF, (u8 *)arg0 + 0x30, (u8 *)arg0 + 0x34);
    func_80071824(arg0, func_80056070);
}

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_8005616C.s")

void func_80056348(Struct56070 *arg0, s16 arg1) {
    s16 diff = arg1 - arg0->unk38;

    if (diff >= 5) {
        diff = 4;
    }
    if (diff < -4) {
        diff = -4;
    }
    arg0->unk38 += diff;
}

void func_8005638C(Struct56070 *arg0, s32 arg1) {
    s32 diff = arg1 - arg0->unk24;

    if (diff >= 0x2001) {
        diff = 0x2000;
    }
    if (diff < -0x2000) {
        diff = -0x2000;
    }
    arg0->unk24 += diff;
}

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_800563C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80056444.s")

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_800564A8.s")

void func_800565E8(void *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, 0);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, 0xA);
    func_800564A8(arg0);
}

void func_8005663C(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x18) = 0xB51A13A3;
    *(s32 *)((u8 *)arg0 + 0x1C) = 0xD0E85B43;
    *(s32 *)((u8 *)arg0 + 0x20) = 0x9A645264;
    *(s16 *)((u8 *)arg0 + 0x30) = 0;
    *(s16 *)((u8 *)arg0 + 0x2C) = 0x17A0;
    *(s16 *)((u8 *)arg0 + 0x2E) = 0;
    *(s16 *)((u8 *)arg0 + 0x38) = 0x130;
    *(s16 *)((u8 *)arg0 + 0x34) = 0;
    *(s32 *)((u8 *)arg0 + 0x24) = 0;
    *(s32 *)((u8 *)arg0 + 0x28) = 0x100000;
    *(s16 *)((u8 *)arg0 + 0x3A) = 0;
    if (D_80122288 == 2) {
        func_80071824(arg0, func_800565E8);
    }
}

void func_800566C0(void *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, -0x20000);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, -3);
    func_800564A8(arg0);
    *(s16 *)((s32)arg0 + 0x3A) = *(s16 *)((s32)arg0 + 0x3A) - 1;
    if (*(s16 *)((s32)arg0 + 0x3A) == 0) {
        func_80071824(arg0, func_8005663C);
    }
}

void func_8005673C(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x18) = 0x05CA84CF;
    *(s32 *)((u8 *)arg0 + 0x1C) = 0x00D0C976;
    *(s32 *)((u8 *)arg0 + 0x20) = 0xFAA3DA4A;
    *(s16 *)((u8 *)arg0 + 0x30) = 0;
    *(s16 *)((u8 *)arg0 + 0x2C) = 0x15E0;
    *(s16 *)((u8 *)arg0 + 0x2E) = 0;
    *(s16 *)((u8 *)arg0 + 0x38) = 0x130;
    *(s16 *)((u8 *)arg0 + 0x34) = 0;
    *(s32 *)((u8 *)arg0 + 0x24) = 0;
    *(s32 *)((u8 *)arg0 + 0x28) = 0x100000;
    *(s16 *)((u8 *)arg0 + 0x3A) = 0x154;
    if (D_80122288 == 1) {
        func_80071824(arg0, func_800566C0);
    }
}

void func_800567C4(void *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, 0);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, 0);
    func_800564A8(arg0);
    *(s16 *)((s32)arg0 + 0x3A) = *(s16 *)((s32)arg0 + 0x3A) - 1;
    if (*(s16 *)((s32)arg0 + 0x3A) == 0) {
        func_80071824(arg0, func_8005673C);
    }
}

void func_80056840(void *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, 0);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, 0x30);
    func_800564A8(arg0);
    *(s16 *)((s32)arg0 + 0x3A) = *(s16 *)((s32)arg0 + 0x3A) - 1;
    if (*(s16 *)((s32)arg0 + 0x3A) == 0) {
        *(s16 *)((s32)arg0 + 0x3A) = 0x96;
        func_80071824(arg0, (void (*)()) func_800567C4);
    }
}

void func_800568C0(void *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, 0x20000);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, 0);
    *(s16 *)((s32)arg0 + 0x3A) = *(s16 *)((s32)arg0 + 0x3A) - 1;
    func_800564A8(arg0);
    if (*(s16 *)((s32)arg0 + 0x3A) == 0) {
        *(s16 *)((s32)arg0 + 0x3A) = 0x2A;
        func_80071824(arg0, func_80056840);
    }
}

void func_80056940(void *arg0) {
    func_80056348(arg0, 0x130);
    func_800564A8(arg0);
    if (D_80122282 == 0x35) {
        *(s16 *)((s32)arg0 + 0x3A) = 0x6A;
        func_80071824(arg0, func_800568C0);
    }
}

void func_80056998(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x18) = 0xE6C45F50;
    *(s32 *)((u8 *)arg0 + 0x1C) = 0xED3C9CFB;
    *(s32 *)((u8 *)arg0 + 0x20) = 0xD14CD682;
    *(s16 *)((u8 *)arg0 + 0x30) = 0;
    *(s16 *)((u8 *)arg0 + 0x2C) = 0x1610;
    *(s16 *)((u8 *)arg0 + 0x2E) = 0;
    *(s16 *)((u8 *)arg0 + 0x38) = 0;
    *(s16 *)((u8 *)arg0 + 0x34) = 0;
    *(s32 *)((u8 *)arg0 + 0x24) = 0;
    *(s16 *)((u8 *)arg0 + 0x3A) = 0x1E;
    func_80071824(arg0, func_80056940);
}

void func_80056A04(void *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, 0);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, 0x18);
    func_800564A8(arg0);
}

void func_80056A58(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x18) = 0xF049BD62;
    *(s32 *)((u8 *)arg0 + 0x1C) = 0xF0E87871;
    *(s32 *)((u8 *)arg0 + 0x20) = 0xDA07DE30;
    *(s16 *)((u8 *)arg0 + 0x30) = 0;
    *(s16 *)((u8 *)arg0 + 0x2C) = 0x1070;
    *(s16 *)((u8 *)arg0 + 0x2E) = 0;
    *(s16 *)((u8 *)arg0 + 0x38) = 0;
    *(s16 *)((u8 *)arg0 + 0x34) = 0;
    *(s32 *)((u8 *)arg0 + 0x24) = 0;
    *(s32 *)((u8 *)arg0 + 0x28) = 0x100000;
    *(s16 *)((u8 *)arg0 + 0x3A) = 0x1E;
    func_80071824(arg0, func_80056A04);
}

void func_80056ACC(void *arg0) {
    func_80056348(arg0, 0x130);
    func_800564A8(arg0);
    if (D_80122288 == 2) {
        func_80071824(arg0, func_80056A58);
    }
}

void func_80056B1C(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x18) = 0xAB4FC576;
    *(s32 *)((u8 *)arg0 + 0x1C) = 0xE13FBC73;
    *(s32 *)((u8 *)arg0 + 0x20) = 0xD2B26423;
    *(s16 *)((u8 *)arg0 + 0x30) = 0;
    *(s16 *)((u8 *)arg0 + 0x2C) = 0xDF0;
    *(s16 *)((u8 *)arg0 + 0x2E) = 0;
    *(s16 *)((u8 *)arg0 + 0x38) = 0;
    *(s16 *)((u8 *)arg0 + 0x34) = 0;
    *(s32 *)((u8 *)arg0 + 0x24) = 0;
    *(s16 *)((u8 *)arg0 + 0x3A) = 0x1E;
    func_80071824(arg0, func_80056ACC);
}

void func_80056B88(void *arg0) {
    func_80056348(arg0, 0x30);
    func_800564A8(arg0);
    if (D_80122288 == 1) {
        func_80071824(arg0, func_80056B1C);
    }
}

void func_80056BD8(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x18) = 0xAB4FC576;
    *(s32 *)((u8 *)arg0 + 0x1C) = 0xE0CEDC73;
    *(s32 *)((u8 *)arg0 + 0x20) = 0xD2B26423;
    *(s16 *)((u8 *)arg0 + 0x30) = 0;
    *(s16 *)((u8 *)arg0 + 0x2C) = 0xDF0;
    *(s16 *)((u8 *)arg0 + 0x2E) = 0;
    *(s16 *)((u8 *)arg0 + 0x38) = 0;
    *(s16 *)((u8 *)arg0 + 0x34) = 0;
    *(s32 *)((u8 *)arg0 + 0x24) = 0;
    *(s16 *)((u8 *)arg0 + 0x3A) = 0x1E;
    func_80071824(arg0, func_80056B88);
}

void func_80056C44(void *arg0) {
    s16 temp_v0 = D_80121B50;

    if (temp_v0 == 3) {
        func_80071824(arg0, func_80056998);
        temp_v0 = D_80121B50;
    }
    if (temp_v0 == 6) {
        func_80071824(arg0, func_80056BD8);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_80056CA0.s")

void func_8005711C(s32 arg0) {
    func_800483FC(&D_801248D4, func_80056CA0, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/562B0/func_8005714C.s")
