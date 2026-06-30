#include "common.h"

typedef struct {
    char pad[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    char pad24[0xA];
    u16 unk2E;
    u16 unk30;
} Struct3A0E0;

typedef struct {
    char pad[0x18];
    char unk18[0x20];
    s16 unk38;
    s16 unk3A;
} Struct3A0E0B;

extern s32 func_80041FB4(s32 arg0);
extern void func_800428C8(s32 arg0);
extern void func_80042920(s32 arg0, u16 arg1, u16 arg2);
extern void func_80071824(s32 arg0, void (*arg1)(void));
extern void func_80041DD4(s32 arg0, s32 arg1);
extern void func_8003A634(void *);
extern void func_800483FC(void *, void *, void *);
extern s32 func_8004885C(void *);
extern void func_8007C130(s32, s16, s16);
extern void func_80039A70(Struct3A0E0 *);
extern void func_80039E5C(void);
extern void func_8003B430(void);
extern void func_80039CEC(void);
extern void func_8003A46C(void);
extern void func_800397C4(void *arg0);
extern void func_80039880(void *);
extern void func_8003998C(void *);
extern void func_80039B84(void);
extern void func_80039F7C(Struct3A0E0 *);
extern void func_8003A6A8(void *arg0);
extern void func_8003A7EC(void *);
extern void func_8003AC00(void);
extern void func_8003AFC0(void *arg0);
extern void func_8003B134(void *);
extern void func_8003A108(void *);
extern void func_8003A21C(void *);
extern void func_8003A190(void *);
extern void func_8003A324(Struct3A0E0 *);
extern void func_8004209C(s32, s32, s32, s32);
extern void func_8003C0A4(s16, s16, u8, u8);
extern void func_8004298C(s32, s32, s32, s32);
extern void func_800420FC(s32, s32, s32, s32);
extern void func_8003D068(s32, s32);
extern void func_8003B308(s32, s32, s32, u16, u16, u8);
extern void func_80041D20(s32, s32);
extern void func_80042034(s32);
extern s32 func_80041FB4(s32);
extern void func_800716E4(void *);
extern void *func_80071664(void (*)(void *), s32, s32, u8);
extern void func_8009853C(void *, s32, s32, void *);
extern void func_8003B740(void *);
extern u16 D_8010B1A2;
extern s32 D_80124898;
extern void *D_8010ADE0;

void func_800394E0(s32 arg0) {
    func_80041FB4(3);
    func_800428C8(3);
}

void func_8003950C(void *arg0) {
    func_80041FB4(3);
    func_800428C8(3);
    if (D_8010B1A2 == 0x41) {
        func_80071824((s32)arg0, (void (*)(void))func_800394E0);
        func_80041DD4(3, 0x5B);
        *(s16 *)((char *)arg0 + 0x26) = 0xC00;
        func_800420FC(3, *(s16 *)((char *)arg0 + 0x24), *(s16 *)((char *)arg0 + 0x26), *(s16 *)((char *)arg0 + 0x28));
    }
}

void func_80039584(void *arg0) {
    u8 *new_var;
    s32 sp18;
    int new_var2;
    u16 temp_t8;

    sp18 = func_80041FB4(3);
    func_800428C8(3);
    new_var2 = 0x2A;
    if (sp18 == 1) {
        *(u16 *)((u8 *)arg0 + new_var2) = (temp_t8 = (*(u16 *)((u8 *)arg0 + 0x2A) & 0xFFFF) + 1);
        if ((temp_t8 & 0xFFFF) == 0x14) {
            new_var = (u8 *)arg0 + 0x2A;
            *(u16 *)new_var = 0;
            func_80071824((s32)arg0, (void (*)(void))func_8003950C);
            func_80041DD4(3, 0x5A);
            D_8010B1A2 = 0x40;
        }
    }
}

void func_80039610(void *arg0) {
    u8 *new_var;
    s32 sp18;
    int new_var2;
    u16 temp_t8;

    sp18 = func_80041FB4(3);
    func_800428C8(3);
    new_var2 = 0x2A;
    if (sp18 == 1) {
        *(u16 *)((u8 *)arg0 + new_var2) = (temp_t8 = (*(u16 *)((u8 *)arg0 + 0x2A) & 0xFFFF) + 1);
        if ((temp_t8 & 0xFFFF) == 0x1B) {
            new_var = (u8 *)arg0 + 0x2A;
            *(u16 *)new_var = 0;
            func_80071824((s32)arg0, (void (*)(void))func_80039584);
            func_80041DD4(3, 0x59);
            func_8003D068(0x30, -0x5B);
        }
    }
}

void func_8003969C(void *arg0) {
    u16 temp_t7;
    u16 *new_var;
    unsigned int new_var2;

    if (func_80041FB4(3) == 1) {
        temp_t7 = (*(u16 *)((u8 *)arg0 + 0x2A) & 0xFFFF) + 1;
        new_var2 = 0xFFFF;
        new_var = (u16 *)((u8 *)arg0 + 0x2A);
        *new_var = temp_t7;
        if ((temp_t7 & new_var2) == 0x14) {
            *(u16 *)((u8 *)arg0 + 0x2A) = 0;
            func_80071824((s32)arg0, func_80039610);
            func_80041DD4(3, 0x58);
        }
    } else {
        *(s32 *)((u8 *)arg0 + 0x20) += 0x80000;
    }
    func_8004209C(3, *(s32 *)((u8 *)arg0 + 0x18), *(s32 *)((u8 *)arg0 + 0x1C), *(s32 *)((u8 *)arg0 + 0x20));
    func_800428C8(3);
}

void func_8003973C(void *arg0) {
    if (func_80041FB4(3) == 0) {
        *(s32 *)((char *)arg0 + 0x18) = *(s32 *)((char *)arg0 + 0x18) + 0x18000;
        func_8004209C(3, *(s32 *)((char *)arg0 + 0x18), *(s32 *)((char *)arg0 + 0x1C), *(s32 *)((char *)arg0 + 0x20));
    }
    if (D_8010B1A2 == 0x3F) {
        *(s16 *)((char *)arg0 + 0x2A) = 0;
        func_80071824((s32)arg0, func_8003969C);
        func_80041DD4(3, 0x57);
    }
    func_800428C8(3);
}

void func_800397C4(void *arg0) {
    s32 unused;
    s32 sp20;
    s32 var_v0;

    sp20 = func_80041FB4(3);
    if (*(u16 *)((u8 *)arg0 + 0x2A) < 5) {
        var_v0 = 1;
    } else {
        var_v0 = -1;
    }
    *(s32 *)((u8 *)arg0 + 0x18) += 0x76000;
    *(s32 *)((u8 *)arg0 + 0x1C) += var_v0 << 19;
    *(s32 *)((u8 *)arg0 + 0x20) += 0xFFFA0000;
    *(u16 *)((u8 *)arg0 + 0x2A) = *(u16 *)((u8 *)arg0 + 0x2A) + 1;
    func_8004209C(3, *(s32 *)((u8 *)arg0 + 0x18), *(s32 *)((u8 *)arg0 + 0x1C), *(s32 *)((u8 *)arg0 + 0x20));
    func_800428C8(3);
    if (sp20 == 1) {
        *(u16 *)((u8 *)arg0 + 0x2A) = 0;
        func_80071824((s32)arg0, func_8003973C);
        func_80041DD4(3, 0x5D);
    }
}

void func_80039880(void *arg0) {
    func_80042034(3);
    *(s32 *)((u8 *)arg0 + 0x18) += 0xFFF58000;
    if (D_8010B1A2 == 0x3C) {
        *(s16 *)((u8 *)arg0 + 0x2A) = 0;
        func_80071824((s32)arg0, (void (*)(void))func_800397C4);
        func_80041DD4(3, 0x5C);
        *(s16 *)((u8 *)arg0 + 0x26) = 0xC00;
        func_800420FC(3, *(s16 *)((u8 *)arg0 + 0x24), *(s16 *)((u8 *)arg0 + 0x26), *(s16 *)((u8 *)arg0 + 0x28));
    }
    func_8004209C(3, *(s32 *)((u8 *)arg0 + 0x18), *(s32 *)((u8 *)arg0 + 0x1C), *(s32 *)((u8 *)arg0 + 0x20));
    func_8004298C(3, *(u16 *)((u8 *)arg0 + 0x2E), *(u16 *)((u8 *)arg0 + 0x30), 0xB);
}

void func_80039930(void *arg0) {
    func_80042034(3);
    func_8004298C(3, *(u16 *)((char *)arg0 + 0x2E), *(u16 *)((char *)arg0 + 0x30), 0xB);
    if (D_8010B1A2 == 0x3A) {
        func_80071824((s32)arg0, func_80039880);
    }
}

void func_8003998C(void *arg0) {
    s32 var_a1;

    func_80042034(3);
    var_a1 = (*(s32 *)(0x18 + (u8 *)arg0) & 0xFFFFFFFFFFFFFFFFu) + 0x24000;
    *(s32 *)((u8 *)arg0 + 0x18) = var_a1;
    if (var_a1 >= 0xE00000) {
        *(s32 *)((u8 *)arg0 + 0x18) = 0xE00000;
        func_80071824((s32)arg0, func_80039930);
        var_a1 = *(s32 *)((u8 *)arg0 + 0x18);
    }
    func_8004209C(3, var_a1, *(s32 *)((u8 *)arg0 + 0x1C), *(s32 *)((u8 *)arg0 + 0x20));
    func_8004298C(3, *(u16 *)((u8 *)arg0 + 0x2E), *(u16 *)((u8 *)arg0 + 0x30), 0xB);
}

void func_80039A14(void *arg0) {
    func_80042034(3);
    func_8004298C(3, *(u16 *)((char *)arg0 + 0x2E), *(u16 *)((char *)arg0 + 0x30), 0xB);
    if (D_8010B1A2 == 0x38) {
        func_80071824((s32)arg0, func_8003998C);
    }
}

void func_80039A70(Struct3A0E0 *arg0) {
    s32 var_a1;
    Struct3A0E0 *new_var;

    func_80042034(3);
    new_var = arg0;
    var_a1 = (new_var->unk18 += 0xFFFB8000);
    if (var_a1 < 0xA00001) {
        new_var->unk18 = 0xA00000;
        func_80071824((s32) new_var, func_80039A14);
        var_a1 = arg0->unk18;
    } else if ((var_a1 < 0x1300001) && (D_8010B1A2 == 0x35)) {
        D_8010B1A2 = 0x36;
        var_a1 = arg0->unk18;
    }
    func_8004209C(3, var_a1, arg0->unk1C, arg0->unk20);
    func_8004298C(3, arg0->unk2E, arg0->unk30, 0xB);
}

void func_80039B38(Struct3A0E0 *arg0) {
    if (D_8010B1A2 == 0x35) {
        func_80071824((s32)arg0, func_80039A70);
        arg0->unk18 = 0x01900000;
        arg0->unk20 = 0xFFF80000;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039B84.s")

void func_80039C84(void *arg0) {
    func_80041FB4(3);
    func_8004298C(3, *(u16 *)((char *)arg0 + 0x2E), *(u16 *)((char *)arg0 + 0x30), 0xB);
    if (D_8010B1A2 == 0x31) {
        func_80071824((s32)arg0, func_80039B84);
        func_80041DD4(3, 0x4B);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039CEC.s")

void func_80039E08(void *arg0) {
    u16 *p = (u16 *)((char *)arg0 + 0x2A);
    (*p)++;
    if (*p == 0xF) {
        *p = 0;
        func_80071824((s32)arg0, func_80039CEC);
        func_80041DD4(3, 0x4A);
    }
    func_800428C8(3);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039E5C.s")

void func_80039F2C(void *arg0) {
    if (D_8010B1A2 == 0x2C) {
        func_80071824((s32)arg0, func_80039E5C);
        func_80041DD4(3, 0x4B);
        *(s16 *)((char *)arg0 + 0x2A) = 0;
    }
}

void func_80039F7C(Struct3A0E0 *arg0) {
    s32 var_a1;

    func_80042034(3);
    var_a1 = arg0->unk18 += 0x28000;
    if (var_a1 >= 0x1900000) {
        arg0->unk18 = 0x1900000;
        func_80071824((s32)arg0, func_80039F2C);
        D_8010B1A2 = 0x28;
        var_a1 = arg0->unk18;
    }
    func_8004209C(3, var_a1, arg0->unk1C, arg0->unk20);
    func_8004298C(3, arg0->unk2E, arg0->unk30, 0xB);
}

void func_8003A010(void *arg0) {
    u16 *p;
    func_80042034(3);
    func_8004298C(3, *(u16 *)((char *)arg0 + 0x2E), *(u16 *)((char *)arg0 + 0x30), 0xB);
    p = (u16 *)((char *)arg0 + 0x2A);
    (*p)++;
    if (*p == 0x41) {
        *p = 0;
        func_80071824((s32)arg0, func_80039F7C);
    }
}

void func_8003A078(Struct3A0E0 *arg0) {
    s32 var_a1;

    func_80042034(3);
    var_a1 = arg0->unk18 += 0xFFFD8000;
    if (var_a1 < 0x800001) {
        arg0->unk18 = 0x800000;
        func_80071824((s32)arg0, func_8003A010);
        var_a1 = arg0->unk18;
    }
    func_8004209C(3, var_a1, arg0->unk1C, arg0->unk20);
    func_8004298C(3, arg0->unk2E, arg0->unk30, 0xB);
}

void func_8003A108(void *arg0) {
    s32 temp_a1;
    u8 *new_var;
    u16 temp_t9;

    func_80042034(3);
    temp_a1 = *(s32 *)((u8 *)arg0 + 0x18) += 0x20000;
    func_8004209C(3, temp_a1, *(s32 *)((u8 *)arg0 + 0x1C), *(s32 *)((u8 *)arg0 + 0x20));
    new_var = (u8 *)arg0 + 0x2A;
    temp_t9 = (*(u16 *)new_var & 0xFFFF) + 1;
    *(u16 *)((u8 *)arg0 + 0x2A) = temp_t9;
    if ((temp_t9 & 0xFFFF) == 0x28) {
        *(u16 *)((u8 *)arg0 + 0x2A) = 0;
        func_80071824((s32)arg0, func_8003A078);
    }
    func_8004298C(3, *(u16 *)((u8 *)arg0 + 0x2E), *(u16 *)((u8 *)arg0 + 0x30), 0xB);
}

void func_8003A190(void *arg0) {
    s32 temp_a1;
    u8 *new_var;
    u16 temp_t9;

    func_80042034(3);
    temp_a1 = *(s32 *)((u8 *)arg0 + 0x18) += 0xFFFD8000;
    func_8004209C(3, temp_a1, *(s32 *)((u8 *)arg0 + 0x1C), *(s32 *)((u8 *)arg0 + 0x20));
    new_var = (u8 *)arg0 + 0x2A;
    temp_t9 = (*(u16 *)new_var & 0xFFFF) + 1;
    *(u16 *)((u8 *)arg0 + 0x2A) = temp_t9;
    if ((temp_t9 & 0xFFFF) == 0x2D) {
        *(u16 *)((u8 *)arg0 + 0x2A) = 0;
        func_80071824((s32)arg0, func_8003A108);
    }
    func_8004298C(3, *(u16 *)((u8 *)arg0 + 0x2E), *(u16 *)((u8 *)arg0 + 0x30), 0xB);
}

void func_8003A21C(void *arg0) {
    s32 temp_a1;
    u8 *new_var;
    u16 temp_t9;

    func_80042034(3);
    temp_a1 = *(s32 *)((u8 *)arg0 + 0x18) += 0x28000;
    func_8004209C(3, temp_a1, *(s32 *)((u8 *)arg0 + 0x1C), *(s32 *)((u8 *)arg0 + 0x20));
    new_var = (u8 *)arg0 + 0x2A;
    temp_t9 = (*(u16 *)new_var & 0xFFFF) + 1;
    *(u16 *)((u8 *)arg0 + 0x2A) = temp_t9;
    if ((temp_t9 & 0xFFFF) == 0x1E) {
        *(u16 *)((u8 *)arg0 + 0x2A) = 0;
        func_80071824((s32)arg0, func_8003A190);
    }
    func_8004298C(3, *(u16 *)((u8 *)arg0 + 0x2E), *(u16 *)((u8 *)arg0 + 0x30), 0xB);
}

void func_8003A2A8(void *arg0) {
    func_80042034(3);
    func_8004298C(3, *(u16 *)((char *)arg0 + 0x2E), *(u16 *)((char *)arg0 + 0x30), 0xB);
    if (D_8010B1A2 == 0x23) {
        u16 *p = (u16 *)((char *)arg0 + 0x2A);
        (*p)++;
        if (*p == 0x2D) {
            *p = 0;
            func_80071824((s32)arg0, func_8003A21C);
        }
    }
}

void func_8003A324(Struct3A0E0 *arg0) {
    s32 var_a1;

    func_80042034(3);
    var_a1 = arg0->unk18 += 0x28000;
    if (var_a1 >= 0x800000) {
        *(u16 *)((char *)arg0 + 0x2A) = 0;
        arg0->unk18 = 0x800000;
        func_80071824((s32)arg0, func_8003A2A8);
        var_a1 = arg0->unk18;
    }
    if ((var_a1 >= 0x200000) && (D_8010B1A2 == 0x20)) {
        D_8010B1A2 = 0x21;
        var_a1 = arg0->unk18;
    }
    func_8004209C(3, var_a1, arg0->unk1C, arg0->unk20);
    func_8004298C(3, arg0->unk2E, arg0->unk30, 0xB);
}

void func_8003A3E0(Struct3A0E0 *arg0) {
    s32 var_a1;

    func_80042034(3);
    var_a1 = arg0->unk18 += 0xFFFA0000;
    if (var_a1 < -0x9FFFFF) {
        arg0->unk18 = 0xFF600000;
        func_80071824((s32)arg0, func_8003A324);
        var_a1 = arg0->unk18;
    }
    func_8004209C(3, var_a1, arg0->unk1C, arg0->unk20);
    func_8004298C(3, arg0->unk2E, arg0->unk30, 0xB);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A46C.s")

void func_8003A538(void *arg0) {
    func_8004298C(3, *(u16 *)((char *)arg0 + 0x2E), *(u16 *)((char *)arg0 + 0x30), 0xB);
    if (D_8010B1A2 == 0x1D) {
        func_80071824((s32)arg0, func_8003A46C);
        func_80041DD4(3, 0x4A);
    }
}

void func_8003A598(Struct3A0E0 *arg0) {
    s32 var_a1;

    func_80042034(3);
    var_a1 = arg0->unk18 += 0xFFFE8000;
    if (var_a1 < 0xC80001) {
        arg0->unk18 = 0xC80000;
        func_80071824((s32)arg0, func_8003A538);
        D_8010B1A2 = 0x1C;
        var_a1 = arg0->unk18;
    }
    func_8004209C(3, var_a1, arg0->unk1C, arg0->unk20);
    func_8004298C(3, arg0->unk2E, arg0->unk30, 0xB);
}

void func_8003A634(void *arg0) {
    s32 pad;
    s32 sp20;

    sp20 = func_80041FB4(3);
    func_80042920(3, *(u16 *)((char *)arg0 + 0x2E), *(u16 *)((char *)arg0 + 0x30));
    if (sp20 == 1) {
        func_80071824((s32)arg0, func_8003A598);
        func_80041DD4(3, 0x49);
        *(s32 *)((char *)arg0 + 0x18) += 0xFFEC0000;
    }
}

void func_8003A6A8(void *arg0) {
    s32 pad;
    s32 sp18;

    sp18 = func_80041FB4(3);
    func_80042920(3, *(u16 *)((char *)arg0 + 0x2E), *(u16 *)((char *)arg0 + 0x30));
    if (sp18 == 1) {
        func_80071824((s32)arg0, func_8003A634);
        func_80041DD4(3, 0x42);
    }
}

void func_8003A70C(void *arg0) {
    u16 *p;
    func_800428C8(3);
    p = (u16 *)((char *)arg0 + 0x2A);
    (*p)++;
    if (*p == 0x1E) {
        *p = 0;
        D_8010B1A2 = 0x1B;
        func_80071824((s32)arg0, func_8003A6A8);
        func_80041DD4(3, 0x41);
    }
}

void func_8003A77C(void *arg0) {
    if (func_80041FB4(3) == 1) {
        func_80071824((s32)arg0, (void (*)(void))func_8003A70C);
    } else {
        *(s32 *)((char *)arg0 + 0x18) = *(s32 *)((char *)arg0 + 0x18) + 0x40000;
        func_8004209C(3, *(s32 *)((char *)arg0 + 0x18), *(s32 *)((char *)arg0 + 0x1C), *(s32 *)((char *)arg0 + 0x20));
    }
    func_800428C8(3);
}

void func_8003A7EC(void *arg0) {
    s32 temp_v0;
    u16 temp_t7;

    temp_v0 = func_80041FB4(3);
    temp_t7 = (*(u16 *)((u8 *)arg0 + 0x2A) & 0xFFFF) + 1;
    *(u16 *)((u8 *)arg0 + 0x2A) = temp_t7;
    *(s32 *)((u8 *)arg0 + 0x18) += 0x70000;
    if ((temp_t7 & 0xFFFF) < 5) {
        *(s32 *)((u8 *)arg0 + 0x1C) += 0x50000;
    } else {
        *(s32 *)((u8 *)arg0 + 0x1C) += 0xFFFB0000;
    }
    if (temp_v0 == 1) {
        *(u16 *)((u8 *)arg0 + 0x2A) = 0;
        func_80071824((s32)arg0, (void (*)(void))func_8003A77C);
        func_80041DD4(3, 0x40);
    }
    func_8004209C(3, *(s32 *)((u8 *)arg0 + 0x18), *(s32 *)((u8 *)arg0 + 0x1C), *(s32 *)((u8 *)arg0 + 0x20));
    func_800428C8(3);
}

void func_8003A8A4(void *arg0) {
    s32 pad;
    s32 sp20;

    *(s32 *)((u8 *)arg0 + 0x18) += 0x100000;
    func_8004209C(3, *(s32 *)((u8 *)arg0 + 0x18), *(s32 *)((u8 *)arg0 + 0x1C), *(s32 *)((u8 *)arg0 + 0x20));
    sp20 = func_80041FB4(3);
    func_800428C8(3);
    *(s32 *)((u8 *)arg0 + 0x1C) += 0x50000;
    if (sp20 == 1) {
        *(u16 *)((u8 *)arg0 + 0x2A) = 0;
        D_8010B1A2 = 0x1A;
        func_80071824((s32)arg0, func_8003A7EC);
        func_80041DD4(3, 0x3F);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A944.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A9E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003AC00.s")

void func_8003ACD0(void *arg0) {
    u16 *p;
    func_800428C8(3);
    p = (u16 *)((char *)arg0 + 0x2A);
    (*p)++;
    if (*p == 0xF) {
        D_8010B1A2 = 0x19;
        func_80071824((s32)arg0, func_8003AC00);
        func_8004209C(3, *(s32 *)((char *)arg0 + 0x18), *(s32 *)((char *)arg0 + 0x1C), *(s32 *)((char *)arg0 + 0x20));
    }
}

void func_8003AD40(void *arg0) {
    void (*new_var)(void *);
    s32 sp18;
    u16 temp_t8;

    sp18 = func_80041FB4(3);
    func_800428C8(3);
    if (sp18 == 1) {
        temp_t8 = (*(u16 *)((u8 *)arg0 + 0x2A) & 0xFFFF) + 1;
        *(u16 *)((u8 *)arg0 - -0x2A) = temp_t8;
        if ((temp_t8 & 0xFFFF) == 0xA) {
            *(u16 *)((u8 *)arg0 + 0x2A) = 0;
            new_var = func_8003ACD0;
            D_8010B1A2 = 0x18;
            func_80071824((s32)arg0, (void (*)(void))new_var);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003ADC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003AE8C.s")

extern void func_80041DD4(s32 arg0, s32 arg1);
extern void func_80071824(s32 arg0, void (*arg1)(void));
extern void func_8003AE8C(void);

void func_8003AF6C(void *arg0) {
    if (func_80041FB4(3) == 1) {
        *(u16 *)((s32)arg0 + 0x2C) = 0;
        func_80071824((s32)arg0, func_8003AE8C);
        func_80041DD4(3, 0x37);
    }
    func_800428C8(3);
}

void func_8003AFC0(void *arg0) {
    u16 temp_t9;

    if (func_80041FB4(3) == 0) {
        *(s32 *)((char *)arg0 + 0x18) = *(s32 *)((char *)arg0 + 0x18) + 0x48000;
        func_8004209C(3, *(s32 *)((char *)arg0 + 0x18), *(s32 *)((char *)arg0 + 0x1C), *(s32 *)((char *)arg0 + 0x20));
        temp_t9 = (*(u16 *)((char *)arg0 + 0x2A) & 0xFFFF) + 1;
        *(u16 *)((char *)arg0 + 0x2A) = temp_t9;
        if ((temp_t9 & 0xFFFF) == 2) {
            func_8003C0A4(-0x14, -0x40, 3, 1);
        }
    }
    if (D_8010B1A2 == 0x16) {
        *(s16 *)((char *)arg0 + 0x2A) = 0;
        func_80071824((s32)arg0, func_8003AF6C);
        func_80041DD4(3, 0x36);
    }
    func_800428C8(3);
}

void func_8003B074(void *arg0) {
    u16 temp_t0;
    s32 sp20;

    sp20 = func_80041FB4(3);
    func_800428C8(3);
    if (D_8010B1A2 == 0x15) {
        *(u16 *)((u8 *)arg0 + 0x2A) = 0;
        func_80071824((s32)arg0, func_8003AFC0);
        *(s16 *)((char *)arg0 + 0x26) = 0x400;
        func_800420FC(3, *(s16 *)((char *)arg0 + 0x24), *(s16 *)((char *)arg0 + 0x26), *(s16 *)((char *)arg0 + 0x28));
        func_80041DD4(3, 0x28);
    } else if (sp20 == 1) {
        temp_t0 = (*(u16 *)((u8 *)arg0 + 0x2A) & 0xFFFF) + 1;
        *(u16 *)((u8 *)arg0 + 0x2A) = temp_t0;
        if ((temp_t0 & 0xFFFF) == 0x10) {
            *(u16 *)((u8 *)arg0 + 0x2A) = 0;
            func_80041DD4(3, 0x27);
        }
    }
}

void func_8003B134(void *arg0) {
    s32 temp_v0;
    u16 temp_t2;

    temp_v0 = func_80041FB4(3);
    *(s32 *)((u8 *)arg0 + 0x18) += 0xFFFB8000;
    if (*(s32 *)((u8 *)arg0 + 0x18) < 0xE00001) {
        *(s32 *)((u8 *)arg0 + 0x18) = 0xE00000;
        func_80071824((s32)arg0, (void (*)(void))func_8003B074);
        D_8010B1A2 = 0x14;
    } else if (temp_v0 == 1) {
        temp_t2 = (*(u16 *)((u8 *)arg0 + 0x2A) & 0xFFFF) + 1;
        *(u16 *)((u8 *)arg0 + 0x2A) = temp_t2;
        if ((temp_t2 & 0xFFFF) == 0x10) {
            *(u16 *)((u8 *)arg0 + 0x2A) = 0;
            func_80041DD4(3, 0x27);
        }
    }
    func_8004209C(3, *(s32 *)((u8 *)arg0 + 0x18), *(s32 *)((u8 *)arg0 + 0x1C), *(s32 *)((u8 *)arg0 + 0x20));
    func_800428C8(3);
}

void func_8003B1F8(void *arg0) {
    if (D_8010B1A2 == 0x13) {
        func_80071824((s32)arg0, func_8003B134);
        func_8003B308(*(s32 *)((char *)arg0 + 0x18) + 0x48000, 0x480000, 0xFFF70000, *(u16 *)((char *)arg0 + 0x2E), *(u16 *)((char *)arg0 + 0x30), 2);
    }
}

void func_8003B264(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x18) = 0x03B48000;
    *(s32 *)((u8 *)arg0 + 0x1C) = 0;
    *(s32 *)((u8 *)arg0 + 0x20) = 0;
    *(u16 *)((u8 *)arg0 + 0x24) = 0;
    *(u16 *)((u8 *)arg0 + 0x26) = 0;
    *(u16 *)((u8 *)arg0 + 0x28) = 0;
    *(u16 *)((u8 *)arg0 + 0x2A) = 0;
    *(u16 *)((u8 *)arg0 + 0x2C) = 0;
    *(u16 *)((u8 *)arg0 + 0x2E) = 0;
    *(u16 *)((u8 *)arg0 + 0x30) = 3;
    func_80041D20(3, 3);
    func_80041DD4(3, 0x27);
    func_8004209C(3, *(s32 *)((u8 *)arg0 + 0x18), *(s32 *)((u8 *)arg0 + 0x1C), *(s32 *)((u8 *)arg0 + 0x20));
    func_800420FC(3, *(s16 *)((u8 *)arg0 + 0x24), *(s16 *)((u8 *)arg0 + 0x26), *(s16 *)((u8 *)arg0 + 0x28));
    func_80071824((s32)arg0, func_8003B1F8);
}

void func_8003B308(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u8 arg5) {
    void *sp1C;
    void *temp_v0;

    temp_v0 = func_80071664((void (*)(void *))func_8003B740, 0, 0x64, arg5);
    D_8010ADE0 = temp_v0;
    *(s32 *)((char *)temp_v0 + 0x2C) = arg0;
    *(s32 *)((char *)temp_v0 + 0x30) = arg1;
    *(s32 *)((char *)temp_v0 + 0x34) = arg2;
    sp1C = temp_v0;
    func_8009853C((char *)temp_v0 + 0x18, 0x400, 0x400, &D_8010ADE0);
    *(u16 *)((char *)sp1C + 0x38) = arg3;
    *(u16 *)((char *)sp1C + 0x3A) = arg4;
}

void func_8003B39C(Struct3A0E0B *arg0) {
    s32 temp = func_8004885C(&arg0->unk18);
    if (temp != 0) {
        func_8007C130(temp, arg0->unk38, arg0->unk3A);
    }
}

void func_8003B3E0(void *arg0) {
    if (D_8010B1A2 == 0x1B) {
        func_800716E4(arg0);
    } else {
        func_800483FC(&D_80124898, func_8003B39C, arg0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003B430.s")

void func_8003B6F0(void *arg0) {
    if (D_8010B1A2 == 0x15) {
        func_80071824((s32)arg0, func_8003B430);
    }
    func_800483FC(&D_80124898, func_8003B39C, arg0);
}

void func_8003B740(void *arg0) {
    u16 *p;
    *(s32 *)((char *)arg0 + 0x2C) = *(s32 *)((char *)arg0 + 0x2C) + 0xFFFB8000;
    p = (u16 *)((char *)arg0 + 0x3C);
    (*p)++;
    if (*p == 0xA2) {
        *p = 0;
        func_80071824((s32)arg0, (void (*)(void))func_8003B6F0);
    }
    func_800483FC(&D_80124898, (void *)func_8003B39C, arg0);
}
