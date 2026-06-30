#include "common.h"

typedef struct {
    char pad0[0x10];
    /* 0x10 */ s16 unk10;
    /* 0x12 */ char pad12[6];
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ u8 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
} Struct3CAF0;

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
} Struct3CAF0b;

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ u16 unk24;
    /* 0x26 */ u8 unk26;
} Struct3CAF0c;

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
} Struct3CAF0d;

extern void func_80071824(void *, void *);
extern void func_800483FC(void *, void *, void *);
extern void func_80011D74(void *, s32, s16, s16);
extern void func_80017168(void *, s32);
extern void func_800716E4(void *);
extern s32 func_80043040(s16);
extern void func_8000F030(s16, s16, s32, s32, s32, s32, s32, s32);
extern s32 D_80124838;
extern s32 D_80124868;
extern s16 D_80112172;
extern s16 D_8011217C;
extern s16 D_8011217E;
extern u16 D_8010B1A2;
extern u8 D_8010B1A5;
extern s16 D_8010B1A6;
extern u16 D_8010B1D0;
extern u16 D_800B9530[];
extern u16 D_800B9538[];
extern s32 D_8010ADDC;

void func_8003BF7C(Struct3CAF0 *arg0);
void func_8003C180(Struct3CAF0b *arg0);
void func_8003C634(Struct3CAF0 *arg0);
void func_8003C7E4(Struct3CAF0 *arg0);
void func_8003CE78(Struct3CAF0 *arg0);
void func_8003D2F4(Struct3CAF0d *arg0);
void func_8003D5F8(Struct3CAF0 *arg0);
void func_8003D580(Struct3CAF0 *arg0);
void func_8003D4C0(Struct3CAF0 *arg0);
void func_8003D3C0(Struct3CAF0 *arg0);
void func_8003D88C(Struct3CAF0 *arg0);
void func_8003DA98(Struct3CAF0 *arg0);
void func_8003DB84(Struct3CAF0 *arg0);
void func_8003DCCC(Struct3CAF0 *arg0);
void func_8003DBE8(Struct3CAF0 *arg0);
void func_8003C2EC(Struct3CAF0 *arg0);
void func_8003C93C(Struct3CAF0 *arg0);
void func_8003CAD8(Struct3CAF0 *arg0);
void func_8003CC58(Struct3CAF0 *arg0);
void func_8003CF98(Struct3CAF0 *arg0);
void func_8003D124(Struct3CAF0 *arg0);
Struct3CAF0 *func_80071408(void *, s32, s32);

void func_8003BEF0(Struct3CAF0 *arg0) {
    func_8000F030(arg0->unk18 + arg0->unk1C, arg0->unk1A + arg0->unk1E, func_80043040(D_80112172), ((u16) arg0->unk20 + 0x32) & 0xFFFF, 0x20, 0x20, ((u8 *) &arg0->unk24)[1], 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003BF7C.s")

void func_8003C078(Struct3CAF0 *arg0) {
    arg0->unk22 = 0;
    arg0->unk20 = 0;
    func_80071824(arg0, func_8003BF7C);
}

void func_8003C0A4(s16 arg0, s16 arg1, u8 arg2, u8 arg3) {
    Struct3CAF0 *temp_v0 = (Struct3CAF0 *)(D_8010ADDC = (s32)func_80071408(func_8003C078, 0, 0x63));

    temp_v0->unk1C = arg0;
    temp_v0->unk1E = arg1;
    temp_v0->unk26 = arg2;
    ((u8 *) &temp_v0->unk24)[1] = arg3;
}

void func_8003C118(Struct3CAF0c *arg0) {
    s32 temp = func_80043040(D_80112172);
    Struct3CAF0c *p = (Struct3CAF0c *)((s32)arg0 + arg0->unk24 * 2);
    func_8000F030(p->unk18, p->unk1E, temp, 0xF, 0x20, 0x20, 0, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003C180.s")

void func_8003C208(Struct3CAF0b *arg0) {
    arg0->unk1C = -0x38;
    arg0->unk22 = -0x5A;
    arg0->unk18 = -0x30;
    arg0->unk1E = -0x5D;
    arg0->unk1A = -0x28;
    arg0->unk20 = -0x56;
    arg0->unk26 = 0;
    arg0->unk24 = 0;
    func_80071824(arg0, func_8003C180);
}

void func_8003C264(Struct3CAF0 *arg0) {
    func_8000F030(arg0->unk18 + arg0->unk1C, arg0->unk1A + arg0->unk1E, func_80043040(D_80112172), ((u16) arg0->unk20 + 0x10) & 0xFFFF, 0x20, 0x20, 0, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003C2EC.s")

void func_8003C3F4(Struct3CAF0 *arg0) {
    arg0->unk22 = 0;
    arg0->unk20 = 0;
    func_80071824(arg0, func_8003C2EC);
}

void func_8003C420(s16 arg0, s16 arg1, u8 arg2) {
    Struct3CAF0 *temp_v0 = (Struct3CAF0 *)(D_8010ADDC = (s32)func_80071408(func_8003C3F4, 0, 0x63));

    temp_v0->unk1C = arg0;
    temp_v0->unk1E = arg1;
    temp_v0->unk26 = arg2;
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003C484.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003C634.s")

void func_8003C728(Struct3CAF0 *arg0) {
    arg0->unk18 = -0x3D;
    arg0->unk1A = -0x63;
    arg0->unk1C = -0x4E;
    arg0->unk1E = -0x6C;
    arg0->unk22 = 0;
    arg0->unk24 = 0;
    arg0->unk20 = 0;
    arg0->unk26 = 0;
    func_80071824(arg0, func_8003C634);
}

void func_8003C77C(Struct3CAF0c *arg0) {
    s32 temp;

    temp = func_80043040(D_80112172);
    func_8000F030(arg0->unk18, arg0->unk1A, temp, (arg0->unk1C + 9) & 0xFFFF, 0x20, 0x20, 0, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003C7E4.s")

void func_8003C870(Struct3CAF0 *arg0) {
    arg0->unk18 = 0x3B;
    arg0->unk1A = -0x38;
    arg0->unk1E = 0;
    arg0->unk1C = 0;
    func_80071824(arg0, func_8003C7E4);
}

void func_8003C8AC(Struct3CAF0c *arg0) {
    s32 temp = func_80043040(D_80112172);
    func_8000F030(arg0->unk18 - 0xE, arg0->unk1A - 0x50, temp, (u16)(D_800B9530[arg0->unk1C] + 6), 0x20, 0x20, 0, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003C93C.s")

void func_8003CA44(Struct3CAF0 *arg0) {
    arg0->unk1E = 0;
    arg0->unk1C = 0;
    func_80071824(arg0, func_8003C93C);
}

void func_8003CA70(Struct3CAF0c *arg0) {
    s32 temp;

    temp = func_80043040(D_80112172);
    func_8000F030(arg0->unk18, arg0->unk1A, temp, (arg0->unk1C + 0x14) & 0xFFFF, 0x20, 0x20, 0, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003CAD8.s")

void func_8003CB4C(Struct3CAF0 *arg0) {
    arg0->unk1E = 0;
    arg0->unk1C = 0;
    func_80071824(arg0, func_8003CAD8);
}

void func_8003CB78(s16 arg0, s16 arg1) {
    Struct3CAF0 *temp_v0 = (Struct3CAF0 *)(D_8010ADDC = (s32)func_80071408(func_8003CB4C, 0, 0x64));

    temp_v0->unk18 = arg0;
    temp_v0->unk1A = arg1;
}

void func_8003CBCC(Struct3CAF0 *arg0) {
    func_8000F030(arg0->unk18 + arg0->unk1C, arg0->unk1A + arg0->unk1E, func_80043040(D_80112172), ((u16) arg0->unk20 + 0x20) & 0xFFFF, 0x20, 0x20, ((u8 *) &arg0->unk24)[1], 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003CC58.s")

void func_8003CD70(Struct3CAF0 *arg0) {
    arg0->unk22 = 0;
    arg0->unk20 = 0;
    func_80071824(arg0, func_8003CC58);
}

void func_8003CD9C(s16 arg0, s16 arg1, u8 arg2, u8 arg3) {
    Struct3CAF0 *temp_v0 = (Struct3CAF0 *)(D_8010ADDC = (s32)func_80071408(func_8003CD70, 0, 0x64));

    temp_v0->unk1C = arg0;
    temp_v0->unk1E = arg1;
    temp_v0->unk26 = arg2;
    ((u8 *) &temp_v0->unk24)[1] = arg3;
}

void func_8003CE10(Struct3CAF0c *arg0) {
    s32 temp;

    temp = func_80043040(D_80112172);
    func_8000F030(arg0->unk18, arg0->unk1A, temp, (arg0->unk1C + 0x20) & 0xFFFF, 0x20, 0x20, 0, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003CE78.s")

void func_8003CF00(Struct3CAF0 *arg0) {
    arg0->unk18 = -0x14;
    arg0->unk1A = -0x59;
    arg0->unk1E = 0;
    arg0->unk1C = 0;
    func_80071824(arg0, func_8003CE78);
}

void func_8003CF3C(Struct3CAF0c *arg0) {
    s32 temp = func_80043040(D_80112172);
    func_8000F030(arg0->unk18, arg0->unk1A, temp, arg0->unk1C, 0x20, 0x20, 0, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003CF98.s")

void func_8003D03C(Struct3CAF0 *arg0) {
    arg0->unk1C = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8003CF98);
}

void func_8003D068(s16 arg0, s16 arg1) {
    Struct3CAF0 *temp = (Struct3CAF0 *)(D_8010ADDC = (s32)func_80071408(func_8003D03C, 0, 0x64));

    temp->unk18 = arg0;
    temp->unk1A = arg1;
}

void func_8003D0BC(Struct3CAF0c *arg0) {
    s32 temp;

    temp = func_80043040(D_80112172);
    func_8000F030(arg0->unk18, arg0->unk1A, temp, (arg0->unk1C + 0x1C) & 0xFFFF, 0x20, 0x20, 0, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D124.s")

void func_8003D1EC(Struct3CAF0 *arg0) {
    arg0->unk1C = 0;
    arg0->unk1E = 0;
    func_80071824(arg0, func_8003D124);
}

void func_8003D218(s16 arg0, s16 arg1, u8 arg2) {
    Struct3CAF0 *temp_v0 = (Struct3CAF0 *)(D_8010ADDC = (s32)func_80071408(func_8003D1EC, 0, 0x64));

    temp_v0->unk18 = arg0;
    temp_v0->unk1A = arg1;
    temp_v0->unk10 = arg2;
}

void func_8003D27C(Struct3CAF0c *arg0) {
    s32 temp;

    temp = func_80043040(D_80112172);
    func_8000F030(arg0->unk18, arg0->unk1A, temp, (D_800B9538[arg0->unk1C] + 0x19) & 0xFFFF, 0x20, 0x20, 0, 0);
}

void func_8003D2F4(Struct3CAF0d *arg0) {
    Struct3CAF0d *temp_a2 = arg0;

    arg0->unk1E++;
    if (arg0->unk1E == 7) {
        arg0->unk1E = 0;
        arg0->unk1C = (arg0->unk1C + 1) & 3;
    }
    if (temp_a2->unk18 < 0x12) {
        temp_a2->unk18++;
    }
    if (D_8010B1A2 == 4) {
        func_800716E4(temp_a2);
    } else {
        func_800483FC(&D_80124868, func_8003D27C, temp_a2);
    }
}

void func_8003D384(Struct3CAF0 *arg0) {
    arg0->unk18 = -0xA7;
    arg0->unk1A = -0x61;
    arg0->unk1E = 0;
    arg0->unk1C = 0;
    func_80071824(arg0, func_8003D2F4);
}

void func_8003D3C0(Struct3CAF0 *arg0) {
    func_80011D74(&arg0->unk20, 1, arg0->unk18, arg0->unk1A);
}

void func_8003D3F4(Struct3CAF0 *arg0) {
    func_800483FC(&D_80124838, func_8003D3C0, arg0);
}

void func_8003D424(Struct3CAF0 *arg0) {
    s16 *temp_v0;
    s16 temp_v1;
    Struct3CAF0 *temp_a2 = arg0;

    temp_v0 = &arg0->unk20;
    if (*temp_v0 > 0) {
        *temp_v0 -= D_8010B1A5;
        D_8010B1A6 = *temp_v0;
        temp_v1 = *temp_v0;
        if (temp_v1 <= 0) {
            *temp_v0 = 0x140 - temp_v1;
        }
    }
    func_800483FC(&D_80124838, func_8003D3C0, temp_a2);
    if (D_8010B1A2 == 0x3A) {
        func_80071824(temp_a2, func_8003D3F4);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D4C0.s")

void func_8003D580(Struct3CAF0 *arg0) {
    s16 *temp_v0;

    func_800483FC(&D_80124838, func_8003D3C0, arg0);
    temp_v0 = &arg0->unk20;
    if (D_8010B1A2 == 0x33) {
        func_80071824(arg0, func_8003D4C0);
        D_8010B1A5 = 8;
        if (*temp_v0 == 0) {
            *temp_v0 = 0x140;
        }
    }
}

void func_8003D5F8(Struct3CAF0 *arg0) {
    s16 *temp_v0;
    Struct3CAF0 *temp_a2 = arg0;

    temp_v0 = &arg0->unk20;
    if (*temp_v0 > 0) {
        *temp_v0 -= 8;
        D_8010B1A6 = *temp_v0;
        if (*temp_v0 == 0) {
            *temp_v0 = 0x140;
        }
    }
    func_800483FC(&D_80124838, func_8003D3C0, temp_a2);
    if (D_8010B1A2 == 0x2A) {
        func_80071824(temp_a2, func_8003D580);
    }
}

void func_8003D68C(Struct3CAF0 *arg0) {
    s16 *sp1C;
    Struct3CAF0 *temp_a2 = arg0;

    sp1C = &arg0->unk20;
    if (*sp1C > 0) {
        *sp1C -= 8;
        D_8010B1A6 = *sp1C;
    }
    func_800483FC(&D_80124838, func_8003D3C0, temp_a2);
    if (*sp1C == 0) {
        temp_a2->unk18 = 0x14;
        *sp1C = 0x140;
        func_80071824(temp_a2, func_8003D5F8);
    }
}

void func_8003D720(Struct3CAF0 *arg0) {
    s16 *temp_v0;
    u16 *temp_v1;
    Struct3CAF0 *temp_a2 = arg0;

    temp_v0 = &arg0->unk20;
    if (*temp_v0 < 0x140) {
        *temp_v0 += D_8010B1A5;
        D_8010B1A6 = *temp_v0;
        temp_v1 = &D_8010B1D0;
        *temp_v1 = *temp_v1 + 1;
    }
    func_800483FC(&D_80124838, func_8003D3C0, temp_a2);
    if (D_8010B1A2 == 0x20) {
        func_80071824(temp_a2, func_8003D68C);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D7C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003D88C.s")

void func_8003D908(Struct3CAF0 *arg0) {
    func_80017168((u8 *)arg0 + 0x20, func_80043040(D_8011217C));
    arg0->unk22 = 0x10;
    arg0->unk18 = arg0->unk28;
    arg0->unk1A = arg0->unk2A;
    D_8010B1D0 = 0;
    func_80071824(arg0, func_8003D88C);
}

void func_8003D974(Struct3CAF0 *arg0) {
    func_80011D74(&arg0->unk20, 1, arg0->unk18, arg0->unk1A);
}

void func_8003D9A8(Struct3CAF0 *arg0) {
    arg0->unk20--;
    if (arg0->unk20 < 0) {
        arg0->unk20 = 0x140;
    }
    func_800483FC(&D_80124838, func_8003D974, arg0);
    if ((D_8010B1A2 == 0x2A) || (D_8010B1A2 == 0x3A)) {
        func_80071824(arg0, func_8003DA98);
    }
}

void func_8003DA24(Struct3CAF0 *arg0) {
    arg0->unk20++;
    if (arg0->unk20 == 0x140) {
        arg0->unk20 = 0;
    }
    func_800483FC(&D_80124838, func_8003D974, arg0);
    if (D_8010B1A2 == 0x14) {
        func_80071824(arg0, func_8003DA98);
    }
}

void func_8003DA98(Struct3CAF0 *arg0) {
    func_800483FC(&D_80124838, func_8003D974, arg0);
    if (D_8010B1A2 == 0x10) {
        func_80071824(arg0, func_8003DA24);
    }
    if ((D_8010B1A2 == 0x20) || (D_8010B1A2 == 0x33)) {
        func_80071824(arg0, func_8003D9A8);
    }
}

void func_8003DB1C(Struct3CAF0 *arg0) {
    func_80017168((u8 *)arg0 + 0x20, func_80043040(D_8011217C));
    arg0->unk20 = 0;
    arg0->unk22 = 0x1F0;
    arg0->unk18 = 0x14;
    arg0->unk1A = arg0->unk2A;
    func_80071824(arg0, func_8003DA98);
}

void func_8003DB84(Struct3CAF0 *arg0) {
    func_80011D74(&arg0->unk20, 0, arg0->unk18, arg0->unk1A);
}

void func_8003DBB8(Struct3CAF0 *arg0) {
    func_800483FC(&D_80124868, func_8003DB84, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003DBE8.s")

void func_8003DC70(Struct3CAF0 *arg0) {
    func_800483FC(&D_80124868, func_8003DB84, arg0);
    if (D_8010B1A2 == 0x42) {
        arg0->unk1C = 0;
        func_80071824(arg0, func_8003DBE8);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/3CAF0/func_8003DCCC.s")

void func_8003DD64(Struct3CAF0 *arg0) {
    func_80017168((u8 *)arg0 + 0x20, func_80043040(D_8011217E));
    arg0->unk20 = 0;
    arg0->unk22 = 0x10;
    arg0->unk18 = arg0->unk28;
    arg0->unk1A = arg0->unk2A;
    func_80071824(arg0, func_8003DCCC);
}
