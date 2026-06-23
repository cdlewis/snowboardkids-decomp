#include "common.h"

extern void *D_80124868;
extern void func_80071824(void *, void *);
extern void func_800483FC(void *, void *, s32);
extern void func_80053D8C(s32);
extern void func_80054644(void *);
extern void func_8005537C(void *);

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80053660.s")

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_800536F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80053858.s")

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_8005393C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80053B28.s")

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80053C90.s")

extern void func_80053C90(void);
extern void func_80053B28(void);
extern u8 D_801248F8[];
extern u8 D_801248A4[];

void func_80053D8C(s32 arg0) {
    func_800483FC(D_801248F8, func_80053C90, arg0);
    func_800483FC(D_801248A4, func_80053B28, arg0);
}

void func_80053DD8(void *arg0) {
    func_80071824(arg0, func_80053D8C);
}

extern void func_80042034(s32 arg0);
extern void func_80042A00(s32 arg0);

void func_80053DFC(s32 arg0) {
    func_80042034(0);
    func_80042A00(0);
}

extern void func_800428C8(s32);
extern void func_80072138(s16, s32, void *);
extern void func_80042034(s32);
extern void func_80042A00(s32);
extern void func_80053DFC(s32);
extern s16 D_800D5738[];
extern s16 D_800DEF14;

void func_80053E28(void *arg0) {
    if (D_800DEF14 == 0) {
        if (*(u8 *)((u8 *)arg0 + 0x18) != 0) {
            func_80072138(D_800D5738[*(u16 *)((u8 *)arg0 + 0x10)], 0x32, arg0);
            *(u8 *)((u8 *)arg0 + 0x18) = 0;
        }
        func_80042034(0);
        func_80071824(arg0, func_80053DFC);
        func_80042A00(0);
        return;
    }
    func_800428C8(0);
}

extern void func_80041D20(s32, u16);
extern void func_80041DD4(s32, u8);
extern void func_8004209C(s32, s32, s32, s32);
extern void func_800420FC(s32, s32, s32, s32);
extern u8 D_800D5730[];

void func_80053EBC(void *arg0) {
    *(s8 *)((u8 *)arg0 + 0x18) = 1;
    func_80041D20(0, *(u16 *)((u8 *)arg0 + 0x10));
    func_80041DD4(0, D_800D5730[*(u16 *)((u8 *)arg0 + 0x10)]);
    func_80042034(0);
    func_8004209C(0, 0, 0, 0);
    func_800420FC(0, 0, 0, 0);
    func_80071824(arg0, func_80053E28);
}

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80053F50.s")

extern u8 D_800D5744[];
extern s32 D_800D5748[];
extern void func_80053F50(void);

void func_80053FA0(void *arg0) {
    s32 a = *(s32 *)((u8 *)arg0 + 0x18);
    *(s16 *)((u8 *)arg0 + 0x1C) = a * 0xF;
    func_80041D20(a, *(u16 *)((u8 *)arg0 + 0x10));
    func_80041DD4(*(s32 *)((u8 *)arg0 + 0x18), D_800D5744[*(u16 *)((u8 *)arg0 + 0x10)]);
    func_80042034(*(s32 *)((u8 *)arg0 + 0x18));
    {
        s32 b = *(s32 *)((u8 *)arg0 + 0x18);
        func_8004209C(b, D_800D5748[b], 0xFFB50000, 0xA00000);
    }
    func_800420FC(*(s32 *)((u8 *)arg0 + 0x18), 0, 0x800, 0);
    func_80071824(arg0, func_80053F50);
}

extern void *func_80071664(void *, s32, s32, s32);

void func_80054044(s32 arg0, s32 arg1) {
    void *p = func_80071664(func_80053FA0, 0, 0x64, arg1);
    if (p != NULL) {
        *(s32 *)((u8 *)p + 0x18) = arg0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_8005408C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_800540EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80054130.s")

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80054460.s")

extern void func_80054460(void);
extern u8 D_80121D80[];

void func_800545D0(void *arg0) {
    u8 *e = &D_80121D80[*(u16 *)((u8 *)arg0 + 0x10) * 0x60C];
    if (!(*(s32 *)(e + 0x2FC) & 0x2000)) {
        if (*(u8 *)(e + 0x51A) != 0) {
            *(u8 *)((u8 *)arg0 + 0x2E) = 0xFF;
            func_80071824(arg0, func_80054460);
        }
    }
}

extern s32 func_80043040(s16);
extern void func_80045A78(s16, s16, s32, u8);
extern u8 D_800D57A0[];
extern s16 D_8011213C;
extern s16 D_801235B0;

void func_80054644(void *arg0) {
    if ((D_801235B0 & 0x3E) >= 0x1F) {
        func_80045A78(*(s16 *)((u8 *)arg0 + 0x18), *(s16 *)((u8 *)arg0 + 0x1A), func_80043040(D_8011213C), 1);
    } else {
        func_80045A78(*(s16 *)((u8 *)arg0 + 0x18), *(s16 *)((u8 *)arg0 + 0x1A), func_80043040(D_8011213C), D_800D57A0[(D_801235B0 & 0x1E) >> 1]);
    }
}

void func_800546E4(s32 arg0) {
    func_800483FC(&D_80124868, func_80054644, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80054714.s")

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_8005475C.s")

extern void func_800716E4(void *);
extern void func_8005475C(void);

void func_800548F4(void *arg0) {
    s16 v = *(s16 *)((u8 *)arg0 + 0x18);
    if (v != 0) {
        *(s16 *)((u8 *)arg0 + 0x18) = v - 1;
    } else {
        *(s16 *)((u8 *)arg0 + 0x1A) -= 0x20;
        if (*(s16 *)((u8 *)arg0 + 0x1A) <= 0) {
            func_800716E4(arg0);
            return;
        }
    }
    func_800483FC(&D_80124868, func_8005475C, arg0);
}

void func_80054968(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = 0x3E;
    *(s16 *)((u8 *)arg0 + 0x1A) = 0xFF;
    func_80071824(arg0, func_800548F4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_8005499C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80054A04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80054A64.s")

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80054AC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80054B2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80054B98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80054E70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80054EC4.s")

extern void func_80054EC4(void);

void func_8005502C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x36) = 0xFF;
    *(s32 *)((u8 *)arg0 + 0x30) = 0;
    *(s32 *)((u8 *)arg0 + 0x18) = 0;
    *(s32 *)((u8 *)arg0 + 0x1C) = 0x300000;
    *(s32 *)((u8 *)arg0 + 0x20) = 0xEC0000;
    *(s16 *)((u8 *)arg0 + 0x34) = 0;
    func_80071824(arg0, func_80054EC4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80055074.s")

extern void func_80055074(void);
extern u8 D_80124878[];

void func_800550E0(void *arg0) {
    s16 v;
    *(s16 *)((u8 *)arg0 + 0x18) += 1;
    v = *(s16 *)((u8 *)arg0 + 0x18);
    if (v >= 0x3D) {
        *(s16 *)((u8 *)arg0 + 0x18) = 0;
        v = *(s16 *)((u8 *)arg0 + 0x18);
    }
    if (v < 0xA) {
        *(s16 *)((u8 *)arg0 + 0x1A) = v;
    } else {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0;
    }
    func_800483FC(D_80124878, func_80055074, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80055148.s")

extern void func_80055148(void);

void func_8005537C(void *arg0) {
    s16 v = *(s16 *)((u8 *)arg0 + 0x18);
    if (v != 0) {
        *(s16 *)((u8 *)arg0 + 0x18) = v - 1;
        return;
    }
    *(s16 *)((u8 *)arg0 + 0x1A) += 0x20;
    if (*(s16 *)((u8 *)arg0 + 0x1A) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0xFF;
    }
    func_800483FC(&D_80124868, func_80055148, arg0);
}

void func_800553E0(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = 0x50;
    *(s16 *)((u8 *)arg0 + 0x1A) = 0;
    func_80071824(arg0, func_8005537C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80055410.s")

#pragma GLOBAL_ASM("asm/nonmatchings/54260/func_80055530.s")

extern void func_80055530(void);
extern void func_80055410(void);

void func_8005562C(s32 arg0) {
    func_800483FC(D_801248F8, func_80055530, arg0);
    func_800483FC(D_801248A4, func_80055410, arg0);
}

void func_80055678(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = 0x360;
    func_80071824(arg0, func_8005562C);
}
