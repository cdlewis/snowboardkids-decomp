#include "common.h"

typedef struct {
    char pad[0x18];
    s32 unk18;
    char pad1C[4];
    s32 unk20;
} Struct3A0E0;

typedef struct {
    char pad[0x18];
    char unk18[0x20];
    s16 unk38;
    s16 unk3A;
} Struct3A0E0B;

extern s32 func_80041FB4(s32 arg0);
extern void func_800428C8(s32 arg0);
extern void func_80071824(s32 arg0, void (*arg1)(void));
extern void func_80041DD4(s32 arg0, s32 arg1);
extern void func_800483FC(void *, void *, void *);
extern s32 func_8004885C(void *);
extern void func_8007C130(s32, s16, s16);
extern void func_80039A70(void);
extern void func_80039E5C(void);
extern void func_8003B430(void);
extern void func_80039CEC(void);
extern void func_8003A46C(void);
extern void func_80039880(void);
extern void func_8003998C(void);
extern void func_80039B84(void);
extern void func_80039F7C(void);
extern void func_8003A6A8(void);
extern void func_8003AC00(void);
extern void func_8003B134(void);
extern void func_8003A21C(void);
extern void func_8003969C(void);
extern void func_8004209C(s32, s32, s32, s32);
extern void func_8004298C(s32, s32, s32, s32);
extern void func_800420FC(s32, s32, s32, s32);
extern void func_8003B308(s32, s32, s32, s32, s32, s32);
extern void func_80042034(s32);
extern s32 func_80041FB4(s32);
extern u16 D_8010B1A2;
extern s32 D_80124898;

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

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039584.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039610.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003969C.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_800397C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039880.s")

void func_80039930(void *arg0) {
    func_80042034(3);
    func_8004298C(3, *(u16 *)((char *)arg0 + 0x2E), *(u16 *)((char *)arg0 + 0x30), 0xB);
    if (D_8010B1A2 == 0x3A) {
        func_80071824((s32)arg0, func_80039880);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003998C.s")

void func_80039A14(void *arg0) {
    func_80042034(3);
    func_8004298C(3, *(u16 *)((char *)arg0 + 0x2E), *(u16 *)((char *)arg0 + 0x30), 0xB);
    if (D_8010B1A2 == 0x38) {
        func_80071824((s32)arg0, func_8003998C);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039A70.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_80039F7C.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A078.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A108.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A190.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A21C.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A324.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A3E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A46C.s")

void func_8003A538(void *arg0) {
    func_8004298C(3, *(u16 *)((char *)arg0 + 0x2E), *(u16 *)((char *)arg0 + 0x30), 0xB);
    if (D_8010B1A2 == 0x1D) {
        func_80071824((s32)arg0, func_8003A46C);
        func_80041DD4(3, 0x4A);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A598.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A634.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A6A8.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A7EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003A8A4.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003AD40.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003AFC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003B074.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003B134.s")

void func_8003B1F8(void *arg0) {
    if (D_8010B1A2 == 0x13) {
        func_80071824((s32)arg0, func_8003B134);
        func_8003B308(*(s32 *)((char *)arg0 + 0x18) + 0x48000, 0x480000, 0xFFF70000, *(u16 *)((char *)arg0 + 0x2E), *(u16 *)((char *)arg0 + 0x30), 2);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003B264.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003B308.s")

void func_8003B39C(Struct3A0E0B *arg0) {
    s32 temp = func_8004885C(&arg0->unk18);
    if (temp != 0) {
        func_8007C130(temp, arg0->unk38, arg0->unk3A);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/3A0E0/func_8003B3E0.s")

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
