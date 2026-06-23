#include "common.h"

typedef struct {
    s32 word;
    u16 half;
    u8 b6;
    u8 b7;
} Struct57600;

extern Struct57600 D_800D5FF0[];

typedef struct {
    s32 a;
    s32 b;
    s32 c;
} Vec3i;

extern Vec3i D_800D6340[];
extern Vec3i D_800D6220[];
extern Vec3i D_800D6330[];
extern Vec3i D_800D62AC[];
extern s16 D_800D633C[];

extern void *D_80124868;
extern void *D_80124858;
extern void *D_801248B0;
extern void *D_801248BC;
extern void *D_801248C8;
extern u8 D_80121B55;
extern u8 D_80121B56;
extern u8 D_80156608;
extern s16 D_80156612;
extern s16 D_8011216E;
extern s16 D_80112168;
extern s8 D_80122289;
extern s16 D_801235B0;
extern s16 D_8011216C;
extern void *D_80124878;
extern s16 D_800D6050[];
extern void func_80071824(void *, void *);
extern void *func_800711D0(void *, s32, s32);
extern void *func_80071408(void *, s32, s32);
extern void func_800483FC(void *, void *, s32);
extern s32 func_80043040(s16);
extern void func_80045A78(s32, s32, s32, s32);
extern void func_80045990(s32, s32, void *, void *);
extern void func_80097C18(void *, s32);
extern void func_8005F448(void *);
extern s32 func_80080CC4(s16, s32, s32);
extern void func_800716E4(void *);
extern void *func_800716A4(void *, s32, s32);
extern void func_80072A74(s32, void *, s32, s32);
extern s32 func_8007B130(void *, void *, void *, void *);
extern u8 D_80121D80[];
extern u8 D_800EC9F0[];
extern void *D_80121B74;
extern void func_80072138(s32, s32);
extern void func_80048278(s32, s32, void *, s32);
extern void *D_800E1368;
extern void *D_800E136C;
extern void *D_800E1370;
extern void *D_800E1374;
extern void *D_800E1378;
extern void *D_800E14B8;
extern void *D_800E14BC;
extern void *D_800E14C0;
extern void *D_800E14C4;
extern void *D_800E14C8;
extern u8 D_800D5FF4[];
extern s8 D_801124B0[];
extern u16 D_800D6520[];
extern s32 D_80123778;
extern s32 D_801235B4;
extern void func_80057E10(void *);
extern void func_800615BC(void);
extern void func_800640D8(void);
extern void func_80057710(void);
extern void func_80057B60(void *);
extern void func_80057D68(void *);
extern void func_80058C00(void);
extern void func_8005905C(void);
extern void func_80059518(void *);
extern void func_80059950(void *);
extern void func_8005A288(void *);
extern void func_8005E5B4(void);
extern void func_8005F174(void);
extern void func_80061088(void);
extern void func_80062F6C(void);
extern void func_80058B20(void *);
extern void func_800628DC(void);
extern void func_80060FA4(void *);
extern void func_80061CA8(void);
extern void func_800634C8(void);
extern void func_80057AA4(void *);
extern void func_80057CAC(void *);
extern void func_80060E7C(void *);
extern void func_80060D10(void);
extern void func_8006501C(void *);
extern void func_80064F4C(void);
extern void func_80059E5C(void);
extern void func_80059C34(void);
extern void func_80059F6C(void);
extern void func_8005B8E8(void);
extern void func_8005BE68(void);
extern void func_8005C03C(void);
extern void func_80061984(void);
extern void func_80063220(void);
extern void func_80057854(void);
extern void func_800621DC(void *);
extern void func_8005A1FC(void *);
extern void func_8005A31C(void);
extern void func_8005A884(void);
extern void func_8005AAE4(void);
extern void func_8005AE1C(void *);
extern void func_8005A4BC(void);
extern void func_8005AC44(void);
extern void func_8005B9F8(void);
extern void func_8005C14C(void);
extern void func_8005A0E0(void *);
extern void func_80061F38(void);
extern void func_8005F828(void);
extern void func_8005CF60(void);
extern void func_8005D558(void);
extern void func_8005D860(void);
extern void func_8005DB3C(void *);
extern void func_8005D1CC(void);
extern void func_8005D9B4(void);
extern void func_800602BC(void);
extern void func_800589F4(void *);
extern void func_80057E90(void);
extern void func_80058360(void);
extern void func_80065D24(void);
extern void func_80065808(void);
extern void func_80059854(void);
extern void func_8005804C(void);
extern void func_8005812C(void);
extern void func_8005827C(void);
extern void func_80057548(void);
extern void func_8005B6F8(void *);
extern void func_8005CD10(void *);
extern void func_8005E3F8(void *);
extern void func_80064D88(void);
extern void func_80062D34(void);
extern s32 func_80043120(void);
extern void func_8005B61C(void);
extern void func_8005CC54(void);
extern void func_8005E33C(void);
extern void func_8005893C(void *);
extern void func_80060544(void);
extern void func_80060914(void);
extern void func_80058880(void);
extern s16 D_80121B50;

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_800572A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80057548.s")

void func_80057600(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x20) = 0;
    func_80045990(func_80043040(D_80112168), *(u16 *)&D_800D5FF4[*(u16 *)((u8 *)arg0 + 0x10) * 8], (u8 *)arg0 + 0x18, (u8 *)arg0 + 0x1C);
    *(s32 *)((u8 *)arg0 + 0x24) = D_800D5FF0[*(u16 *)((u8 *)arg0 + 0x10)].word;
    *(s32 *)((u8 *)arg0 + 0x28) = D_800D5FF0[*(u16 *)((u8 *)arg0 + 0x10)].b6;
    *(s32 *)((u8 *)arg0 + 0x2C) = D_800D5FF0[*(u16 *)((u8 *)arg0 + 0x10)].b7;
    func_80071824(arg0, func_80057548);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80057694.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80057710.s")

void func_80057810(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = -0x10;
    if (D_80121B55 != 1) {
        *(s16 *)((u8 *)arg0 + 0x1C) = -8;
    }
    func_80071824(arg0, func_80057710);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80057854.s")

void func_80057AA4(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x1C) += *(s32 *)((u8 *)arg0 + 0x28);
    *(s32 *)((u8 *)arg0 + 0x28) += 4;
    if (*(s32 *)((u8 *)arg0 + 0x28) == 0x38) {
        func_800716E4(arg0);
    } else {
        func_800483FC(&D_80124878, func_80057854, arg0);
    }
}

void func_80057B04(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(u16 *)((u8 *)arg0 + 0x18) - 1;
    if (*(u16 *)((u8 *)arg0 + 0x18) == 0) {
        func_80071824(arg0, func_80057AA4);
    }
    func_800483FC(&D_80124878, func_80057854, arg0);
}

void func_80057B60(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x1C) -= *(s32 *)((u8 *)arg0 + 0x28);
    *(s32 *)((u8 *)arg0 + 0x28) -= 4;
    if (*(s32 *)((u8 *)arg0 + 0x28) == 0) {
        *(s16 *)((u8 *)arg0 + 0x18) = 0x2D;
        func_80071824(arg0, func_80057B04);
    }
    func_800483FC(&D_80124878, func_80057854, arg0);
}

void func_80057BCC(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x20) = -0x2C;
    *(s32 *)((u8 *)arg0 + 0x28) = 0x38;
    *(s32 *)((u8 *)arg0 + 0x1C) = 0x1A4;
    func_80071824(arg0, func_80057B60);
}

void func_80057C08(void *arg0, s16 arg1) {
    void *temp = func_80071408(func_80057BCC, 0, 0x64);
    if (temp != NULL) {
        *(void **)((u8 *)temp + 0x2C) = arg0;
        *(s8 *)((u8 *)temp + 0x30) = arg1;
    }
}

void func_80057C5C(void *arg0) {
    if (D_80156608 == 0) {
        func_80045A78(-0x3C, *(s16 *)((u8 *)arg0 + 0x22), func_80043040(D_8011216E), 0x8F);
    }
}

void func_80057CAC(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x1C) += *(s32 *)((u8 *)arg0 + 0x28);
    *(s32 *)((u8 *)arg0 + 0x28) += 4;
    if (*(s32 *)((u8 *)arg0 + 0x28) == 0x38) {
        func_800716E4(arg0);
    } else {
        func_800483FC(&D_80124878, func_80057C5C, arg0);
    }
}

void func_80057D0C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(u16 *)((u8 *)arg0 + 0x18) - 1;
    if (*(u16 *)((u8 *)arg0 + 0x18) == 0) {
        func_80071824(arg0, func_80057CAC);
    }
    func_800483FC(&D_80124878, func_80057C5C, arg0);
}

void func_80057D68(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x1C) -= *(s32 *)((u8 *)arg0 + 0x28);
    *(s32 *)((u8 *)arg0 + 0x28) -= 4;
    if (*(s32 *)((u8 *)arg0 + 0x28) == 0) {
        *(s16 *)((u8 *)arg0 + 0x18) = 0x2D;
        func_80071824(arg0, func_80057D0C);
    }
    func_800483FC(&D_80124878, func_80057C5C, arg0);
}

void func_80057DD4(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x20) = 0xC;
    *(s32 *)((u8 *)arg0 + 0x28) = 0x38;
    *(s32 *)((u8 *)arg0 + 0x1C) = 0x1A4;
    func_80071824(arg0, func_80057D68);
}

void func_80057E10(void *arg0) {
    if (D_80156612 & 8) {
        func_80045A78(0x20, 0x4A, func_80043040(D_8011216E), 0x5B);
    }
}

void func_80057E60(s32 arg0) {
    func_800483FC(&D_80124868, func_80057E10, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80057E90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005804C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005812C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005827C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80058360.s")

void func_800584A0(void *arg0) {
    func_800483FC(&D_80124868, func_80057E90, arg0);
    func_800483FC(&D_80124868, func_8005804C, arg0);
    func_800483FC(&D_80124868, func_8005812C, arg0);
    func_800483FC(&D_80124868, func_8005827C, arg0);
    func_800483FC(&D_80124858, func_80058360, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80058538.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80058610.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80058880.s")

void func_8005893C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        func_80072138(0x1A, 0x32);
        func_80071824(arg0, func_80058880);
    }
    func_800483FC(&D_80124868, func_80057E90, arg0);
    func_800483FC(&D_80124868, func_8005804C, arg0);
    func_800483FC(&D_80124868, func_8005812C, arg0);
    func_800483FC(&D_80124858, func_80058360, arg0);
}

void func_800589F4(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        func_80072138(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        func_80071824(arg0, func_8005893C);
    }
    func_800483FC(&D_80124868, func_80057E90, arg0);
    func_800483FC(&D_80124868, func_8005804C, arg0);
    func_800483FC(&D_80124858, func_80058360, arg0);
}

void func_80058A98(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        func_80072138(0x1A, 0x32);
        func_80071824(arg0, func_800589F4);
    }
    func_800483FC(&D_80124868, func_80057E90, arg0);
    func_800483FC(&D_80124858, func_80058360, arg0);
}

void func_80058B20(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(s16 *)((u8 *)arg0 + 0x18) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x18) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        *(s16 *)((u8 *)arg0 + 0x18) = 0xFF;
        func_80071824(arg0, func_80058A98);
    }
    func_800483FC(&D_80124868, func_80057E90, arg0);
    func_800483FC(&D_80124858, func_80058360, arg0);
}

void func_80058BAC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = 0;
    *(s16 *)((u8 *)arg0 + 0x1C) = D_800D6050[D_80122289 + D_80121B50 * 4];
    func_80071824(arg0, func_80058B20);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80058C00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005905C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80059518.s")

void func_80059804(void *arg0) {
    func_80059518(arg0);
    func_800483FC(&D_80124868, func_80058C00, (s32)arg0);
    func_800483FC(&D_80124858, func_8005905C, (s32)arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80059854.s")

void func_80059950(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(s16 *)((u8 *)arg0 + 0x18) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x18) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x96;
        *(s16 *)((u8 *)arg0 + 0x18) = 0xFF;
        func_80071824(arg0, func_80059854);
    }
    func_80059518(arg0);
    func_800483FC(&D_80124868, func_80058C00, arg0);
    func_800483FC(&D_80124858, func_8005905C, arg0);
}

void func_800599DC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = 0;
    func_80071824(arg0, func_80059950);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80059A04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80059C34.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80059E5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80059F6C.s")

void func_8005A07C(void *arg0) {
    func_800483FC(&D_80124868, func_80059E5C, arg0);
    func_800483FC(&D_80124868, func_80059C34, arg0);
    func_800483FC(&D_80124858, func_80059F6C, arg0);
}

void func_8005A0E0(void *arg0) {
    if (D_80123778 & 0x8000) {
        D_801235B4 |= 0x10;
        func_80072138(0x18, 0x32);
        func_80071824(arg0, func_8005A07C);
    }
    func_800483FC(&D_80124868, func_80059E5C, arg0);
    func_800483FC(&D_80124868, func_80059C34, arg0);
    func_800483FC(&D_80124858, func_80059F6C, arg0);
}

void func_8005A184(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = *(s16 *)((u8 *)arg0 + 0x1C) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1C) == 0) {
        func_80071824(arg0, func_8005A0E0);
    }
    func_800483FC(&D_80124868, func_80059E5C, arg0);
    func_800483FC(&D_80124858, func_80059F6C, arg0);
}

void func_8005A1FC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x1A) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1C) = 0x1E;
        *(s16 *)((u8 *)arg0 + 0x1A) = 0xFF;
        func_80071824(arg0, func_8005A184);
    }
    func_800483FC(&D_80124868, func_80059E5C, arg0);
    func_800483FC(&D_80124858, func_80059F6C, arg0);
}

void func_8005A288(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(s16 *)((u8 *)arg0 + 0x18) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x18) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x18) = 0xFF;
        func_80071824(arg0, func_8005A1FC);
    }
    func_800483FC(&D_80124858, func_80059F6C, arg0);
}

void func_8005A2F0(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = 0;
    *(s16 *)((u8 *)arg0 + 0x1A) = 0;
    func_80071824(arg0, func_8005A288);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005A31C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005A4BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005A884.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005AAE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005AC44.s")

void func_8005AE1C(void *arg0) {
    func_80048278(0x20, -0x48, &D_800E1368, 6);
    func_80048278(0x20, -0x28, &D_800E136C, 6);
    func_80048278(0x20, -8, &D_800E1370, 6);
    func_80048278(0x20, 0x18, &D_800E1374, 4);
    func_80048278(0x20, 0x38, &D_800E1378, 4);
}

void func_8005AEB0(void *arg0) {
    func_800483FC(&D_80124868, func_8005A31C, arg0);
    func_800483FC(&D_80124868, func_8005A884, arg0);
    func_800483FC(&D_80124858, func_8005AAE4, arg0);
    func_800483FC(&D_80124858, func_8005AE1C, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005AF2C.s")

void func_8005AFEC(void *arg0) {
    func_800483FC(&D_80124868, func_8005A31C, arg0);
    func_800483FC(&D_80124868, func_8005A4BC, arg0);
    func_800483FC(&D_80124858, func_8005AAE4, arg0);
    func_800483FC(&D_80124858, func_8005AC44, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005B068.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005B14C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005B344.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005B3EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005B49C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005B55C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005B61C.s")

void func_8005B6F8(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        func_80072138(0x1A, 0x32);
        func_80071824(arg0, func_8005B61C);
    }
    func_800483FC(&D_80124868, func_8005A31C, arg0);
    func_800483FC(&D_80124858, func_8005AAE4, arg0);
    func_800483FC(&D_80124858, func_8005AC44, arg0);
}

void func_8005B798(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(s16 *)((u8 *)arg0 + 0x18) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x18) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        *(s16 *)((u8 *)arg0 + 0x18) = 0xFF;
        func_80071824(arg0, func_8005B6F8);
    }
    func_800483FC(&D_80124868, func_8005A31C, arg0);
    func_800483FC(&D_80124858, func_8005AAE4, arg0);
    func_800483FC(&D_80124858, func_8005AC44, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005B834.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005B8E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005B9F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005BE68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005C03C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005C14C.s")

void func_8005C3E4(void *arg0) {
    func_800483FC(&D_80124868, func_8005B8E8, arg0);
    func_800483FC(&D_80124868, func_8005BE68, arg0);
    func_800483FC(&D_80124858, func_8005C03C, arg0);
}

void func_8005C448(void *arg0) {
    if (D_80123778 & 0x8000) {
        func_80072138(0x18, 0x32);
        D_801235B4 |= 0x10;
        func_80071824(arg0, func_8005C3E4);
    }
    func_800483FC(&D_80124868, func_8005B8E8, arg0);
    func_800483FC(&D_80124868, func_8005BE68, arg0);
    func_800483FC(&D_80124858, func_8005C03C, arg0);
}

void func_8005C4EC(void *arg0) {
    func_800483FC(&D_80124868, func_8005B8E8, arg0);
    func_800483FC(&D_80124868, func_8005B9F8, arg0);
    func_800483FC(&D_80124858, func_8005C03C, arg0);
    func_800483FC(&D_80124858, func_8005C14C, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005C568.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005C64C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005C89C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005C944.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005C9F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005CAB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005CB74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005CC54.s")

void func_8005CD10(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1E) = *(s16 *)((u8 *)arg0 + 0x1E) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1E) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1E) = 0x14;
        func_80072138(0x1A, 0x32);
        func_80071824(arg0, func_8005CC54);
    }
    func_800483FC(&D_80124868, func_8005B8E8, arg0);
    func_800483FC(&D_80124858, func_8005C03C, arg0);
    func_800483FC(&D_80124858, func_8005C14C, arg0);
}

void func_8005CDB0(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = *(s16 *)((u8 *)arg0 + 0x1C) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x1C) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1E) = 0x14;
        *(s16 *)((u8 *)arg0 + 0x1C) = 0xFF;
        func_80071824(arg0, func_8005CD10);
    }
    func_800483FC(&D_80124868, func_8005B8E8, arg0);
    func_800483FC(&D_80124858, func_8005C03C, arg0);
    func_800483FC(&D_80124858, func_8005C14C, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005CE4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005CF60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005D1CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005D558.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005D860.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005D9B4.s")

void func_8005DB3C(void *arg0) {
    func_80048278(0x30, -0x48, &D_800E14B8, 6);
    func_80048278(0x30, -0x28, &D_800E14BC, 6);
    func_80048278(0x30, -8, &D_800E14C0, 6);
    func_80048278(0x30, 0x18, &D_800E14C4, 4);
    func_80048278(0x30, 0x38, &D_800E14C8, 4);
}

void func_8005DBD0(void *arg0) {
    func_800483FC(&D_80124868, func_8005CF60, arg0);
    func_800483FC(&D_80124868, func_8005D558, arg0);
    func_800483FC(&D_80124858, func_8005D860, arg0);
    func_800483FC(&D_80124858, func_8005DB3C, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005DC4C.s")

void func_8005DD0C(void *arg0) {
    func_800483FC(&D_80124868, func_8005CF60, arg0);
    func_800483FC(&D_80124868, func_8005D1CC, arg0);
    func_800483FC(&D_80124858, func_8005D860, arg0);
    func_800483FC(&D_80124858, func_8005D9B4, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005DD88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005DE6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005E064.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005E10C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005E1BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005E27C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005E33C.s")

void func_8005E3F8(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        func_80072138(0x1A, 0x32);
        func_80071824(arg0, func_8005E33C);
    }
    func_800483FC(&D_80124868, func_8005CF60, arg0);
    func_800483FC(&D_80124858, func_8005D860, arg0);
    func_800483FC(&D_80124858, func_8005D9B4, arg0);
}

void func_8005E498(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(s16 *)((u8 *)arg0 + 0x18) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x18) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        *(s16 *)((u8 *)arg0 + 0x18) = 0xFF;
        func_80071824(arg0, func_8005E3F8);
    }
    func_800483FC(&D_80124868, func_8005CF60, arg0);
    func_800483FC(&D_80124858, func_8005D860, arg0);
    func_800483FC(&D_80124858, func_8005D9B4, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005E534.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005E5B4.s")

void func_8005E68C(void *arg0) {
    if (*(u16 *)((u8 *)arg0 + 0x10) != 0) {
        *(s16 *)((u8 *)arg0 + 0x10) = *(u16 *)((u8 *)arg0 + 0x10) - 1;
    } else {
        func_800483FC(&D_80124868, func_8005E5B4, (s32)arg0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005E6D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005EA4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005ECA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005EFFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005F174.s")

void func_8005F298(s16 arg0) {
    void *temp = func_800711D0(func_8005F174, 0, 0x62);
    if (temp != NULL) {
        *(s8 *)((u8 *)temp + 0x59) = arg0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005F2DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005F448.s")

void func_8005F56C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x90) = 1;
    *(s32 *)((u8 *)arg0 + 0x24) = 0xF0000;
    *(s32 *)((u8 *)arg0 + 0x28) = 0x230000;
    *(s32 *)((u8 *)arg0 + 0x2C) = 0;
    *(s16 *)((u8 *)arg0 + 0x92) = 0xF;
    *(s32 *)((u8 *)arg0 + 0x3C) = 0x20000;
    func_8005F448(arg0);
    func_80071824(arg0, func_8005F448);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005F5C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005F6A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005F828.s")

void func_8005FB30(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x48) = 2;
    func_80072A74(0xF, &D_80121D80[*(u16 *)((u8 *)arg0 + 0x10) * 1548 + 0x1C], 0x7F, 0x32);
    func_80071824(arg0, func_8005F828);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005FBA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8005FED0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_800601F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_800602BC.s")

void func_80060454(void *arg0, void *arg1, void *arg2, s16 arg3) {
    void *temp = func_800711D0(func_800602BC, 5, 0x1E);
    if (temp != NULL) {
        *(s8 *)((u8 *)temp + 0x57) = 0;
        *(s16 *)((u8 *)temp + 0x10) = arg3;
        *(void **)((u8 *)temp + 0x18) = arg0;
        *(void **)((u8 *)temp + 0x1C) = arg1;
        *(void **)((u8 *)temp + 0x20) = arg2;
    }
}

void func_800604CC(void *arg0, void *arg1, void *arg2, s16 arg3) {
    void *temp = func_800711D0(func_800602BC, 5, 0x1E);
    if (temp != NULL) {
        *(s8 *)((u8 *)temp + 0x57) = 1;
        *(s16 *)((u8 *)temp + 0x10) = arg3;
        *(void **)((u8 *)temp + 0x18) = arg0;
        *(void **)((u8 *)temp + 0x1C) = arg1;
        *(void **)((u8 *)temp + 0x20) = arg2;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80060544.s")

void func_8006069C(void *arg0) {
    if (D_80121B56 == 0) {
        *(s32 *)((u8 *)arg0 + 0x4C) += -0x10000;
        *(s32 *)((u8 *)arg0 + 0x18) += 0x20000;
        *(s32 *)((u8 *)arg0 + 0x1C) += *(s32 *)((u8 *)arg0 + 0x4C);
        *(s32 *)((u8 *)arg0 + 0x20) += 0x10000;
        *(s16 *)((u8 *)arg0 + 0x56) = *(s16 *)((u8 *)arg0 + 0x56) - 1;
    }
    if (*(s16 *)((u8 *)arg0 + 0x56) == 0) {
        func_800716E4(arg0);
    } else {
        func_800483FC(&D_801248B0, func_80060544, arg0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80060738.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80060914.s")

void func_80060BC4(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x54) = 1;
    *(s32 *)((u8 *)arg0 + 0x48) = 0;
    *(s32 *)((u8 *)arg0 + 0x4C) = 0;
    *(u8 *)((u8 *)arg0 + 0x59) = 1;
    func_80072A74(0xD, &D_80121D80[*(s16 *)((u8 *)arg0 + 0x50) * 1548 + 0x1C], 0x7F, 0x32);
    func_80071824(arg0, func_80060914);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80060C4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80060D10.s")

void func_80060E7C(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x1C) += *(s32 *)((u8 *)arg0 + 0x28);
    *(s32 *)((u8 *)arg0 + 0x28) += 4;
    if (*(s32 *)((u8 *)arg0 + 0x28) == 0x38) {
        func_800716E4(arg0);
    } else {
        func_800483FC(&D_80124878, func_80060D10, arg0);
    }
}

void func_80060EDC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(u16 *)((u8 *)arg0 + 0x18) - 1;
    if (*(u16 *)((u8 *)arg0 + 0x18) == 0) {
        func_80071824(arg0, func_80060E7C);
    }
    func_800483FC(&D_80124878, func_80060D10, arg0);
}

void func_80060F38(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x1C) -= *(s32 *)((u8 *)arg0 + 0x28);
    *(s32 *)((u8 *)arg0 + 0x28) -= 4;
    if (*(s32 *)((u8 *)arg0 + 0x28) == 0) {
        *(s16 *)((u8 *)arg0 + 0x18) = 0x2D;
        func_80071824(arg0, func_80060EDC);
    }
    func_800483FC(&D_80124878, func_80060D10, arg0);
}

void func_80060FA4(void *arg0) {
    if (D_801124B0[*(s8 *)((u8 *)arg0 + 0x30) * 176] != 0) {
        if (D_80121B55 == 1) {
            *(s32 *)((u8 *)arg0 + 0x20) = -0x24;
        } else {
            *(s32 *)((u8 *)arg0 + 0x20) = -0x16;
        }
        *(s32 *)((u8 *)arg0 + 0x28) = 0x38;
        *(s32 *)((u8 *)arg0 + 0x1C) = 0x1A4;
        func_80071824(arg0, func_80060F38);
    } else {
        func_800716E4(arg0);
    }
}

void func_80061034(void *arg0, s16 arg1) {
    void *temp = func_80071408(func_80060FA4, 0, 0x64);
    if (temp != NULL) {
        *(void **)((u8 *)temp + 0x2C) = arg0;
        *(s8 *)((u8 *)temp + 0x30) = arg1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80061088.s")

void func_800613EC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x30) = *(s16 *)((u8 *)arg0 + 0x30) + 4;
    func_800483FC(&D_801248B0, func_80061088, (s32)arg0);
}

void func_80061428(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x30) = 0;
    *(Vec3i *)((u8 *)arg0 + 0x18) = D_800D6220[*(u16 *)((u8 *)arg0 + 0x10)];
    func_80071824(arg0, func_800613EC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80061484.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_800615BC.s")

void func_800617C8(void *arg0) {
    func_80071824(arg0, func_800615BC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_800617EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80061984.s")

void func_80061A98(void *arg0) {
    s32 temp;
    *(s16 *)((u8 *)arg0 + 0x34) = 0xF;
    temp = func_80080CC4(*(s16 *)((u8 *)arg0 + 0x24), *(s32 *)((u8 *)arg0 + 0x18), *(s32 *)((u8 *)arg0 + 0x20));
    *(s16 *)((u8 *)arg0 + 0x2A) = 8;
    *(s32 *)((u8 *)arg0 + 0x1C) = temp;
    *(s16 *)((u8 *)arg0 + 0x28) = *(s16 *)((u8 *)arg0 + 0x2A);
    func_80071824(arg0, func_80061984);
}

void func_80061AF4(s16 arg0, void *arg1, void *arg2, s16 arg3) {
    void *temp = func_80071408(func_80061A98, 0, 0x64);
    if (temp != NULL) {
        *(void **)((u8 *)temp + 0x18) = arg1;
        *(void **)((u8 *)temp + 0x20) = arg2;
        *(s16 *)((u8 *)temp + 0x26) = (arg3 + 0x800) & 0xFFF;
        *(s16 *)((u8 *)temp + 0x24) = arg0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80061B70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80061CA8.s")

void func_80061D90(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x68) = 0x3C;
    func_80097C18((u8 *)arg0 + 0x44, 0x800);
    *(s32 *)((u8 *)arg0 + 0x58) = 0;
    *(s32 *)((u8 *)arg0 + 0x5C) = 0x100000;
    *(s32 *)((u8 *)arg0 + 0x60) = 0xFFE00000;
    func_80071824(arg0, func_80061CA8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80061DE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80061F38.s")

void func_800621DC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x4C) = 4;
    *(s16 *)((u8 *)arg0 + 0x4A) = 0xFF;
    *(s16 *)((u8 *)arg0 + 0x48) = 0;
    *(s32 *)((u8 *)arg0 + 0x2C) = *(s32 *)((u8 *)arg0 + 0x38);
    *(s32 *)((u8 *)arg0 + 0x30) = *(s32 *)((u8 *)arg0 + 0x3C);
    *(s32 *)((u8 *)arg0 + 0x34) = *(s32 *)((u8 *)arg0 + 0x40);
    func_80072A74(0x13, (u8 *)arg0 + 0x38, 0x7F, 0x32);
    func_80071824(arg0, func_80061F38);
}

void func_8006224C(void *arg0, void *arg1, void *arg2) {
    void *temp = func_800716A4(func_800621DC, 0, 3);
    if (temp != NULL) {
        *(void **)((u8 *)temp + 0x38) = arg0;
        *(void **)((u8 *)temp + 0x3C) = arg1;
        *(void **)((u8 *)temp + 0x40) = arg2;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_800622B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_800623E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80062530.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_800625D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_800628DC.s")

void func_80062A10(void *arg0) {
    func_80045990(func_80043040(D_80112168), 0x35, (u8 *)arg0 + 0x30, (u8 *)arg0 + 0x2C);
    func_80071824(arg0, func_800628DC);
}

void func_80062A64(s16 arg0) {
    void *temp;
    temp = func_800711D0(func_80062A10, 0, 4);
    if (temp != NULL) {
        *(s8 *)((u8 *)temp + 0x35) = arg0;
        *(s16 *)((u8 *)temp + 0x28) = 0;
    }
    temp = func_800711D0(func_80062A10, 0, 4);
    if (temp != NULL) {
        *(s8 *)((u8 *)temp + 0x35) = arg0;
        *(s16 *)((u8 *)temp + 0x28) = 0x555;
    }
    temp = func_800711D0(func_80062A10, 0, 4);
    if (temp != NULL) {
        *(s8 *)((u8 *)temp + 0x35) = arg0;
        *(s16 *)((u8 *)temp + 0x28) = 0xAAA;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80062AF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80062D34.s")

void func_80062ED4(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x2C) = func_80043120() << 4;
    if (func_80043120() & 1) {
        *(s16 *)((u8 *)arg0 + 0x2E) = 0x40;
    } else {
        *(s16 *)((u8 *)arg0 + 0x2E) = -0x40;
    }
    *(s16 *)((u8 *)arg0 + 0x24) = 0x1000;
    *(Vec3i *)((u8 *)arg0 + 0x18) = D_800D62AC[*(u16 *)((u8 *)arg0 + 0x10)];
    func_80071824(arg0, func_80062D34);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80062F6C.s")

void func_80063164(void *arg0) {
    if (D_80121B56 == 0) {
        *(s16 *)((u8 *)arg0 + 0x2E) = *(s16 *)((u8 *)arg0 + 0x2E) + 8;
    }
    func_800483FC(&D_801248BC, func_80062F6C, (s32)arg0);
}

void func_800631B0(void *arg0) {
    *(Vec3i *)((u8 *)arg0 + 0x18) = D_800D6330[*(u16 *)((u8 *)arg0 + 0x10)];
    *(s16 *)((u8 *)arg0 + 0x2C) = D_800D633C[*(u16 *)((u8 *)arg0 + 0x10)];
    func_80071824(arg0, func_80063164);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80063220.s")

void func_80063410(void *arg0) {
    if (D_80121B56 == 0) {
        *(s16 *)((u8 *)arg0 + 0x2E) = *(s16 *)((u8 *)arg0 + 0x2E) + 0x60;
        *(s16 *)((u8 *)arg0 + 0x30) = *(s16 *)((u8 *)arg0 + 0x30) + 0x10;
        *(s16 *)((u8 *)arg0 + 0x32) = *(s16 *)((u8 *)arg0 + 0x32) + 4;
    }
    func_800483FC(&D_801248BC, func_80063220, arg0);
}

void func_80063470(void *arg0) {
    *(Vec3i *)((u8 *)arg0 + 0x18) = D_800D6340[*(u16 *)((u8 *)arg0 + 0x10)];
    func_80071824(arg0, func_80063410);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_800634C8.s")

void func_8006392C(void *arg0) {
    if ((D_801235B0 & 7) == 0) {
        *(s16 *)((u8 *)arg0 + 0x4C) = (*(s16 *)((u8 *)arg0 + 0x4C) + 4) & 0x3F;
    }
    func_800483FC(&D_801248C8, func_800634C8, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80063980.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80063A9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80063E70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80063FC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_800640D8.s")

void func_8006426C(s32 arg0) {
    func_800483FC(&D_801248C8, func_800640D8, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8006429C.s")

void func_800643B4(void *arg0, u16 *arg1) {
    *(u16 **)((u8 *)arg0 + 0x30) = arg1;
    *(s16 *)((u8 *)arg0 + 0x34) = arg1[0];
    *(s16 *)((u8 *)arg0 + 0x36) = arg1[1];
    func_80045990(func_80043040(D_8011216C), arg1[2], (u8 *)arg0 + 0x44, (u8 *)arg0 + 0x40);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80064414.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80064470.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_800647E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80064914.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80064B28.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80064C68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80064D88.s")

void func_80064EAC(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x24) = 0;
    *(s32 *)((u8 *)arg0 + 0x28) = 0x280000;
    *(s32 *)((u8 *)arg0 + 0x2C) = 0;
    *(s16 *)((u8 *)arg0 + 0x3C) = 0;
    *(s32 *)((u8 *)arg0 + 0x4C) = 0;
    *(s32 *)((u8 *)arg0 + 0x50) = 0x300000;
    *(s32 *)((u8 *)arg0 + 0x54) = 0;
    func_800643B4(arg0, D_800D6520);
    func_80072A74(0x66, &D_80121D80[*(u16 *)((u8 *)arg0 + 0x10) * 1548 + 0x28], 0x7F, 0x32);
    func_80071824(arg0, func_80064D88);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80064F4C.s")

void func_8006501C(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x1C) -= *(s32 *)((u8 *)arg0 + 0x28);
    *(s32 *)((u8 *)arg0 + 0x28) += 4;
    if (*(s32 *)((u8 *)arg0 + 0x28) == 0x38) {
        func_800716E4(arg0);
    } else {
        func_800483FC(&D_80124878, func_80064F4C, arg0);
    }
}

void func_8006507C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(u16 *)((u8 *)arg0 + 0x18) - 1;
    if (*(u16 *)((u8 *)arg0 + 0x18) == 0) {
        func_80071824(arg0, func_8006501C);
    }
    func_800483FC(&D_80124878, func_80064F4C, arg0);
}

void func_800650D8(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x1C) -= *(s32 *)((u8 *)arg0 + 0x28);
    *(s32 *)((u8 *)arg0 + 0x28) -= 4;
    if (*(s32 *)((u8 *)arg0 + 0x28) == 0) {
        *(s16 *)((u8 *)arg0 + 0x18) = 0x5A;
        func_80071824(arg0, func_8006507C);
    }
    func_800483FC(&D_80124878, func_80064F4C, arg0);
}

void func_80065144(void *arg0) {
    s32 v0;
    *(s32 *)((u8 *)arg0 + 0x20) = -0x2C;
    *(s32 *)((u8 *)arg0 + 0x1C) = 0x1A4;
    *(s32 *)((u8 *)arg0 + 0x28) = 0x38;
    v0 = func_8007B130(&D_80121B74, &D_800EC9F0[D_80121B50 * 4 + 0x12A], (u8 *)arg0 + 0x2C, arg0);
    *(s8 *)((u8 *)arg0 + 0x30) = v0;
    func_80071824(arg0, func_800650D8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_800651BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80065508.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_8006565C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80065764.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80065808.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80065CB8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80065D24.s")

void func_80065E0C(void *arg0) {
    s16 v1 = *(s16 *)((u8 *)arg0 + 0x30);
    if (v1 != 0) {
        *(s32 *)((u8 *)arg0 + 0x1C) -= *(s32 *)((u8 *)arg0 + 0x28);
        *(s32 *)((u8 *)arg0 + 0x28) -= 0x2000;
        *(s16 *)((u8 *)arg0 + 0x30) = v1 - 1;
    } else {
        *(s16 *)((u8 *)arg0 + 0x30) = 0x14;
        *(s16 *)((u8 *)arg0 + 0x32) = 0;
        func_80071824(arg0, func_80065D24);
    }
    func_800483FC(&D_801248BC, func_80065808, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80065E90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80065FD8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_80066158.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_800663C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/57EA0/func_800666B0.s")
