#include "common.h"

extern void func_80041DD4(s32 arg0, s32 arg1);
extern s32 func_80041FB4(s32 arg0);
extern void func_80042034(s32 arg0);
extern void func_800428C8(s32 arg0);
extern void func_80071824(s32 arg0, void (*arg1)(void));
extern void func_8003C0A4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_8003CB78(s32 arg0, s32 arg1);
extern void func_8003CD9C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_8004209C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_800420FC(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_8003B308(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void func_800382BC(void);
extern void func_80038B44(void *);
extern void func_80039354(void);
extern void func_80037B6C(void);
extern void func_80038DD4(void);
extern u8 D_8010B1A4;
extern void func_800383AC(void);
extern void func_8003857C(void);
extern void func_80037FF8(void);
extern void func_800381E0(void);
extern u16 D_8010B1A2;
extern u8 D_8010B1AA;

void func_800373E0(s32 arg0);
void func_8003740C(void *arg0);
extern void func_80038478(void);
void func_800374DC(void *arg0);
void func_80037620(void);
void func_80037708(void *arg0);
void func_80037770(s32 arg0);
void func_80037998(void);
void func_80037AA4(void);
void func_800380F0(void);
void func_800387D4(void);
void func_800388C0(void);
void func_8003905C(s32 arg0);

void func_800373E0(s32 arg0) {
    func_80041FB4(2);
    func_800428C8(2);
}

void func_8003740C(void *arg0) {
    func_80041FB4(2);
    func_800428C8(2);
    if (D_8010B1A2 == 0x41) {
        func_80071824((s32)arg0, func_800373E0);
        func_80041DD4(2, 0x56);
        *(s16 *)((char *)arg0 + 0x26) = 0xC00;
        func_800420FC(2, *(s16 *)((char *)arg0 + 0x24), *(s16 *)((char *)arg0 + 0x26), *(s16 *)((char *)arg0 + 0x28));
    }
}

void func_80037484(s32 arg0) {
    func_80042034(2);
    func_800428C8(2);
    if (D_8010B1A2 == 0x40) {
        func_80071824(arg0, func_8003740C);
        func_80041DD4(2, 0x68);
    }
}

void func_800374DC(void *arg0) {
    u16 *p;
    func_80042034(2);
    func_800428C8(2);
    p = (u16 *)((char *)arg0 + 0x2A);
    (*p)++;
    if (*p == 0x1E) {
        *p = 0;
        func_80071824((s32)arg0, func_80037484);
        D_8010B1A2 = 0x3F;
    }
}

void func_80037548(s32 arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = func_80041FB4(2);
    func_800428C8(2);
    if (sp18 == 1) {
        func_80071824(arg0, func_800374DC);
        func_80041DD4(2, 0x67);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800375A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037620.s")

void func_800376AC(s32 arg0) {
    func_80042034(2);
    func_800428C8(2);
    if (D_8010B1A2 == 0x3E) {
        *(u16 *)((s32)arg0 + 0x2A) = 0;
        func_80071824(arg0, func_80037620);
        func_80041DD4(2, 0x64);
    }
}

void func_80037708(void *arg0) {
    u16 *p;
    func_80042034(2);
    func_800428C8(2);
    p = (u16 *)((char *)arg0 + 0x2A);
    (*p)++;
    if (*p == 0x32) {
        func_80071824((s32)arg0, func_800376AC);
        D_8010B1A2 = 0x3D;
    }
}

void func_80037770(s32 arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = func_80041FB4(2);
    func_800428C8(2);
    if (sp18 == 1) {
        func_80071824(arg0, func_80037708);
        func_80041DD4(2, 0x63);
    }
}

void func_800377C8(s32 arg0) {
    if (func_80041FB4(2) == 1) {
        func_80071824(arg0, func_80037770);
        func_80041DD4(2, 0x62);
    }
    func_800428C8(2);
}

void func_80037818(s32 arg0) {
    if (func_80041FB4(2) == 1) {
        func_80071824(arg0, (void (*)())func_800377C8);
        func_80041DD4(2, 0x23);
    }
    func_800428C8(2);
}

void func_80037868(void *arg0) {
    if (func_80041FB4(2) == 1) {
        u16 *p = (u16 *)((char *)arg0 + 0x2A);
        (*p)++;
        if (*p == 0xF) {
            *p = 0;
            func_80071824((s32)arg0, (void (*)(void))func_80037818);
            func_80041DD4(2, 0x22);
        }
    }
    func_800428C8(2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800378E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037998.s")

void func_80037A58(s32 arg0) {
    func_80042034(2);
    func_800428C8(2);
    if (D_8010B1A2 == 0x3A) {
        func_80071824(arg0, func_80037998);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037AA4.s")

void func_80037B20(s32 arg0) {
    func_80042034(2);
    func_800428C8(2);
    if (D_8010B1A2 == 0x38) {
        func_80071824(arg0, func_80037AA4);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037B6C.s")

void func_80037C28(void *arg0) {
    if (D_8010B1A2 == 0x33) {
        u16 *p = (u16 *)((char *)arg0 + 0x2A);
        (*p)++;
        if (*p == 0x14) {
            *p = 0;
            func_80071824((s32)arg0, func_80037B6C);
            *(s32 *)((char *)arg0 + 0x18) = 0x1900000;
            *(s32 *)((char *)arg0 + 0x20) = 0x120000;
            D_8010B1AA = 0;
            func_8003CD9C(0x18, -0x36, 2, 1);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037CAC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037D48.s")

void func_80037D48(void);

void func_80037E3C(s32 arg0) {
    func_800428C8(2);
    if (D_8010B1A2 == 0x2F) {
        func_80071824(arg0, func_80037D48);
        func_80041DD4(2, 0x4E);
        func_8003C0A4(8, -0x40, 2, 0);
    }
}

void func_80037EA0(void *arg0) {
    func_80042034(2);
    *(s32 *)((char *)arg0 + 0x18) = *(s32 *)((char *)arg0 + 0x18) + 0x2000;
    func_8004209C(2, *(s32 *)((char *)arg0 + 0x18), *(s32 *)((char *)arg0 + 0x1C), *(s32 *)((char *)arg0 + 0x20));
    func_800428C8(2);
    if (D_8010B1A2 == 0x2D) {
        func_80071824((s32)arg0, func_80037E3C);
    }
}

void func_80037F0C(void *arg0) {
    u16 *p;
    func_80042034(2);
    func_800428C8(2);
    p = (u16 *)((char *)arg0 + 0x2A);
    (*p)++;
    if (*p == 0x19) {
        D_8010B1A2 = 0x2C;
        func_80071824((s32)arg0, func_80037EA0);
        *p = 0;
    }
}

void func_80037F74(void *arg0) {
    if (func_80041FB4(2) == 0) {
        *(s32 *)((char *)arg0 + 0x18) = *(s32 *)((char *)arg0 + 0x18) + 0xFFF60000;
    } else {
        func_80071824((s32)arg0, (void (*)(void))func_80037F0C);
        func_80041DD4(2, 0x44);
        *(s32 *)((char *)arg0 + 0x20) = 0xFFFF0000;
        *(s16 *)((char *)arg0 + 0x2A) = 0;
    }
    func_8004209C(2, *(s32 *)((char *)arg0 + 0x18), *(s32 *)((char *)arg0 + 0x1C), *(s32 *)((char *)arg0 + 0x20));
    func_800428C8(2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80037FF8.s")

void func_80038098(s32 arg0) {
    func_80042034(2);
    func_800428C8(2);
    if (D_8010B1A2 == 0x2A) {
        func_80071824(arg0, func_80037FF8);
        *(s32 *)(arg0 + 0x20) = 0xFFFC0000;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800380F0.s")

void func_80038194(s32 arg0) {
    func_80042034(2);
    func_800428C8(2);
    if (D_8010B1A2 == 0x28) {
        func_80071824(arg0, func_800380F0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800381E0.s")

void func_8003825C(void *arg0) {
    u16 *p;
    func_80042034(2);
    func_800428C8(2);
    p = (u16 *)((char *)arg0 + 0x2A);
    (*p)++;
    if (*p == 0xF) {
        *p = 0;
        func_80071824((s32)arg0, func_800381E0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800382BC.s")

void func_80038340(void *arg0) {
    u16 *p;
    func_80042034(2);
    func_800428C8(2);
    p = (u16 *)((char *)arg0 + 0x2A);
    (*p)++;
    if (*p == 0x19) {
        *p = 0;
        func_80071824((s32)arg0, func_800382BC);
        D_8010B1A2 = 0x27;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800383AC.s")

void func_80038424(s32 arg0) {
    func_80042034(2);
    func_800428C8(2);
    D_8010B1A2 = 0x26;
    if (D_8010B1A2 != 0) {
        func_80071824(arg0, func_800383AC);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038478.s")

void func_80038508(void *arg0) {
    func_80042034(2);
    func_800428C8(2);
    if (D_8010B1A2 == 0x24) {
        u16 *p = (u16 *)((char *)arg0 + 0x2A);
        (*p)++;
        if (*p == 0x12) {
            *p = 0;
            func_80071824((s32)arg0, func_80038478);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_8003857C.s")

void func_80038628(s32 arg0) {
    if (D_8010B1A2 == 0x21) {
        func_80071824(arg0, func_8003857C);
        D_8010B1AA = 0;
        func_8003CD9C(0x18, -0x36, 2, 1);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038678.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038704.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800387D4.s")

void func_80038854(s32 arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = func_80041FB4(2);
    func_800428C8(2);
    if (sp18 == 1) {
        func_80071824(arg0, func_800387D4);
        func_80041DD4(2, 0x47);
        func_8003C0A4(5, -0x46, 2, 0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800388C0.s")

void func_80038954(s32 arg0) {
    func_800428C8(2);
    if (D_8010B1A2 == 0x1E) {
        *(u16 *)((s32)arg0 + 0x2A) = 0;
        func_80071824(arg0, func_800388C0);
        func_80041DD4(2, 0x45);
        func_8003CB78(0x2A, -0x57);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800389B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038A54.s")

void func_80038A54(void);

void func_80038AE8(void *arg0) {
    func_80041FB4(2);
    func_800428C8(2);
    if (D_8010B1A2 == 0x1C) {
        *(u16*)((char*)arg0 + 0x2A) = 0;
        func_80071824((s32)arg0, func_80038A54);
        func_80041DD4(2, 0x34);
    }
}

void func_80038B44(void *arg0) {
    u16 *p;
    func_800428C8(2);
    p = (u16 *)((char *)arg0 + 0x2A);
    (*p)++;
    if (*p == 0x1E) {
        *p = 0;
        func_80071824((s32)arg0, (void (*)(void))func_80038AE8);
        func_80041DD4(2, 0x33);
        *(s32 *)((char *)arg0 + 0x20) = 0xFFFF0000;
        func_8004209C(2, *(s32 *)((char *)arg0 + 0x18), *(s32 *)((char *)arg0 + 0x1C), 0xFFFF0000);
    }
}

void func_80038BBC(void *arg0) {
    if (func_80041FB4(2) == 1) {
        func_80071824((s32)arg0, (void (*)(void))func_80038B44);
    } else {
        *(s32 *)((char *)arg0 + 0x20) = *(s32 *)((char *)arg0 + 0x20) + 0xFFFC0000;
        func_8004209C(2, *(s32 *)((char *)arg0 + 0x18), *(s32 *)((char *)arg0 + 0x1C), *(s32 *)((char *)arg0 + 0x20));
    }
    func_800428C8(2);
}

void func_80038C2C(void *arg0) {
    func_800428C8(2);
    if (D_8010B1A4 == 5) {
        func_80071824((s32)arg0, (void (*)(void))func_80038BBC);
        func_80041DD4(2, 0x32);
        *(s16 *)((char *)arg0 + 0x26) = 0x400;
        func_800420FC(2, *(s16 *)((char *)arg0 + 0x24), *(s16 *)((char *)arg0 + 0x26), *(s16 *)((char *)arg0 + 0x28));
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038C9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038D38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038DD4.s")

void func_80038E90(void *arg0) {
    if (D_8010B1A4 == 2) {
        if (func_80041FB4(2) == 1) {
            D_8010B1A4++;
            func_80071824((s32)arg0, func_80038DD4);
            func_80041DD4(2, 0x2B);
            *(s16 *)((char *)arg0 + 0x26) = 0xC01;
            *(s16 *)((char *)arg0 + 0x2A) = 0;
        }
    }
    func_800428C8(2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038F18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80038FB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_8003905C.s")

void func_800390E8(s32 arg0) {
    if (func_80041FB4(2) == 1) {
        func_80071824(arg0, func_8003905C);
        func_80041DD4(2, 0);
    }
    func_800428C8(2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80039138.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800391B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80039228.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_800392DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80039354.s")

void func_800393D4(void *arg0) {
    if (D_8010B1A2 == 0x13) {
        func_80071824((s32)arg0, func_80039354);
        func_8003B308(*(s32 *)((char *)arg0 + 0x18) + 0x48000, 0x480000, 0xFFF30000, 2, 2, 1);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/37FE0/func_80039440.s")
