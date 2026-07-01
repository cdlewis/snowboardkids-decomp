#include "common.h"

extern s16 func_80042D58(s32);
extern s32 func_80043040(s16);
extern s32 func_800430D0(void);
extern void func_80045990(s32, s32, void *, void *);
extern void func_80071824(void *, void *);
extern void func_80097C18(void *, s16);
extern void func_80098590(void *, void *, void *);
extern void func_80088C80(void *, s32, s32, s32);
extern void *func_800711D0(void *, s32, s32);
extern void *func_80071408(void *, s32, s32);
extern void func_800483FC(void *, void *, void *);
extern char D_800D9BD8[];
extern s32 D_801248D4;
extern s16 D_80112168;
extern u8 D_80121B56;
extern u8 D_80112130[];
extern s16 *D_800D91E8[];
extern s16 D_80121B50;
extern void func_80066E10(void);
extern void func_80067034(void *);
extern void func_80067364(void *);
extern void func_800674B4(void *);

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 unk18;
    char pad1A[2];
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    char pad24[8];
    /* 0x2C */ s32 unk2C;
    char pad30[0x1C];
    /* 0x4C */ s32 unk4C;
} Struct68BF0;

typedef struct {
    char pad0[0x20];
    /* 0x20 */ s32 unk20;
    char pad24[4];
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    char pad30[0x30];
    /* 0x60 */ s32 unk60;
} Struct68DB4;

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 unk3A;
    /* 0x3C */ s16 unk3C;
} Struct69678;

extern void func_800681A4(Struct68DB4 *);
extern void func_80068CD4(Struct68DB4 *);
extern void func_800684E4(Struct68BF0 *);
extern void func_80068EA0(Struct68BF0 *);
extern void func_8006935C(Struct69678 *);
extern void func_80069678(Struct69678 *);

typedef struct Scratch674B4 {
    char scratch[0x28];
    s32 pad;
} Scratch674B4;

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80066760.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_800668EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_800669A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80066ABC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80066E10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80067034.s")

void func_8006713C(void *arg0) {
    void *new_var;
    s16 *var_v0;

    new_var = arg0;
    *(s16 *)((u8 *)new_var + 0x1E) = 0;
    var_v0 = D_800D91E8[D_80121B50];
    if (var_v0[1] != -1) {
        do {
            *(s16 *)((u8 *)new_var + 0x1E) += 1;
            var_v0 += 8;
        } while (var_v0[1] != -1);
    }
    if (*(s16 *)((u8 *)new_var + 0x1E) != 0) {
        *(s16 *)&D_80112130[0x48] = func_80042D58(*(s16 *)((u8 *)new_var + 0x1E) << 6);
        *(s32 *)((u8 *)new_var + 0x18) = func_80043040(*(s16 *)&D_80112130[0x48]);
        func_80067034(new_var);
        func_80071824(new_var, func_80066E10);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_800671F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80067364.s")

void func_800674B4(void *arg0) {
    Scratch674B4 sp1C;
    void *temp_a3 = arg0;

    if (D_80121B56 == 0) {
        func_80097C18(sp1C.scratch, *(s16 *)((u8 *)temp_a3 + 0x2A));
        *(s16 *)((u8 *)temp_a3 + 0x4A) = 0x32;
        *(s32 *)((u8 *)temp_a3 + 0x3C) = 0;
        *(s32 *)((u8 *)temp_a3 + 0x40) = 0xB0000;
        *(s32 *)((u8 *)temp_a3 + 0x44) = 0xFFF90000;
        func_80098590(sp1C.scratch, (u8 *)temp_a3 + 0x3C, (u8 *)temp_a3 + 0x30);
        func_80071824(temp_a3, func_80067364);
    }
}

void func_8006752C(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4) {
    void *temp = func_80071408(func_800674B4, 0, 0x64);

    if (temp != NULL) {
        *(s32 *)((u8 *)temp + 0x1C) = arg0;
        *(s32 *)((u8 *)temp + 0x20) = arg1;
        *(s32 *)((u8 *)temp + 0x24) = arg2;
        *(s16 *)((u8 *)temp + 0x2A) = arg3;
        *(s16 *)((u8 *)temp + 0x2C) = arg4;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_800675AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80067830.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_800681A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_800684E4.s")

void func_80068BF0(Struct68BF0 *arg0) {
    s16 temp_v0;
    s32 temp_v1;
    Struct68BF0 *temp_s0 = arg0;
    void *temp_s1;

    if (D_80121B56 == 0) {
        arg0->unk18--;
        temp_v0 = arg0->unk18;
        temp_v1 = arg0->unk20 - (temp_v0 * 0x14000);
        arg0->unk2C = temp_v1 + 0x140000;
        arg0->unk4C = temp_v1;
        if (temp_v0 == 0) {
            func_80071824(arg0, func_80068EA0);
        }
        temp_s1 = &temp_s0->unk1C;
        func_80088C80(temp_s1, 0xC0000, 0x180000, 0);
        func_80088C80(temp_s1, 0xC0000, 0x180000, 1);
        func_80088C80(temp_s1, 0xC0000, 0x180000, 2);
        func_80088C80(temp_s1, 0xC0000, 0x180000, 3);
    }
    func_800483FC(&D_801248D4, func_800684E4, temp_s0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80068CD4.s")

void func_80068DB4(Struct68DB4 *arg0) {
    s32 temp_a2;
    s32 temp_v0;
    s32 var_v1;
    void *temp_s1;

    if (D_80121B56 == 0) {
        temp_v0 = arg0->unk60;
        temp_a2 = arg0->unk20;
        var_v1 = (arg0->unk2C += temp_v0);
        arg0->unk60 = temp_v0 + 0xFFFF0000;
        if (var_v1 < temp_a2) {
            arg0->unk2C = temp_a2;
            arg0->unk60 = 0x30000;
            func_80071824(arg0, func_80068CD4);
            var_v1 = arg0->unk2C;
        }
        temp_s1 = &arg0->unk28;
        if (var_v1 < 0) {
            func_80088C80(temp_s1, 0xC0000, 0x180000, 0);
            func_80088C80(temp_s1, 0xC0000, 0x180000, 1);
            func_80088C80(temp_s1, 0xC0000, 0x180000, 2);
            func_80088C80(temp_s1, 0xC0000, 0x180000, 3);
        }
    }
    func_800483FC(&D_801248D4, func_800681A4, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80068EA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_800691C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_8006935C.s")

void func_80069678(Struct69678 *arg0) {
    s32 temp_v0;
    Struct69678 *temp_a2 = arg0;

    if (D_80121B56 == 0) {
        arg0->unk3C--;
    }
    if (temp_a2->unk3C != 0) {
        if (D_80121B56 == 0) {
            temp_v0 = temp_a2->unk28;
            temp_a2->unk18 += temp_a2->unk24;
            temp_a2->unk1C += temp_v0;
            temp_a2->unk20 += temp_a2->unk2C;
            temp_a2->unk28 = temp_v0 + 0xFFFF0000;
            temp_a2->unk30 += temp_a2->unk36;
            temp_a2->unk32 += temp_a2->unk38;
            temp_a2->unk34 += temp_a2->unk3A;
        }
        func_800483FC(&D_801248D4, func_8006935C, temp_a2);
        return;
    }
    func_800716E4(temp_a2);
}

void func_80069754(void *arg0) {
    char sp28[0x20];

    *(s16 *)((char *)arg0 + 0x3C) = 0xA;
    *(s16 *)((char *)arg0 + 0x36) = func_800430D0() - 0x80;
    *(s16 *)((char *)arg0 + 0x38) = func_800430D0() - 0x80;
    *(s16 *)((char *)arg0 + 0x3A) = func_800430D0() - 0x80;
    func_80097C18(sp28, *(s16 *)((char *)arg0 + 0x32));
    func_80098590(sp28, (char *)&D_800D9BD8 + (*(u16 *)((char *)arg0 + 0x10) * 0xC), (char *)arg0 + 0x24);
    func_80045990(func_80043040(D_80112168), 0x22, (char *)arg0 + 0x4C, (char *)arg0 + 0x48);
    func_80071824(arg0, func_80069678);
}

void func_80069808(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4) {
    void *temp = func_800711D0(func_80069754, 5, 0x3B);

    if (temp != NULL) {
        *(s16 *)((u8 *)temp + 0x10) = arg4;
        *(s32 *)((u8 *)temp + 0x18) = arg0;
        *(s32 *)((u8 *)temp + 0x1C) = arg1;
        *(s32 *)((u8 *)temp + 0x20) = arg2;
        *(s16 *)((u8 *)temp + 0x32) = arg3;
    }
}
