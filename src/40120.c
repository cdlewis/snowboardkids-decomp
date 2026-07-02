#include "common.h"

extern void func_8008C704(void);
extern void func_800710CC(s32);
extern void func_8007115C(void);
extern void func_8006D700(void);
extern void func_80096E3C(void);
extern void func_80072114(s32);
extern void func_800437F0(void *, void *, s32);
extern void func_800438EC(void *, void *, s32);
extern void func_800704F0(void);
extern void func_80070EC0(s32);
extern void func_80070C64(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_8006D5CC(void);
extern void func_8006D520(s32, s32);
extern void func_80071664(void *, s32, s32, s32);
extern void func_800720E4(s32);
extern void func_80071408(void *, s32, s32);
extern void func_80055678(void);
extern void func_80051854(void);

extern u8 D_1467B0;
extern u8 D_147910;
extern u8 D_1DE360;
extern u8 D_1E0F70;
extern u8 D_1F1A90;
extern u8 D_1F2220;
extern u8 D_245A80;
extern u8 D_24C8E0;
extern void func_8009954C(s32);
extern void func_8009956C(void *, s32);
extern void func_80099658(s32);
extern void func_8007AA50(void);
extern void func_800403D8(void);
extern void func_8000DF28(void *);

typedef struct {
    /* 0x00 */ char pad[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
} Struct801235B8;

extern Struct801235B8 *D_801235B8;
extern u8 D_8010B1F0;
extern s16 D_801124B8;
extern s8 D_80121B56;
extern s16 D_800DEF14;
extern u8 D_80123751;
extern s32 D_801235B4;
extern u8 D_80121B5B;
extern s16 D_800D3C90[];

extern void func_8003FEF4(void);
extern void func_8003F554(void);
extern void func_8003F6C0(void);
extern void func_8003F718(void);
extern void func_8003FF78(void);
extern void func_80040450(void);
extern void func_8004086C(void);
extern void func_800408E4(void);
extern void func_8004097C(void);
extern void func_80040A48(void);
extern void func_8003FBE8(void);
extern void func_8003F7E4(void);
extern void func_8003F864(void);
extern void func_80040004(void);
extern void func_80040B90(void);
extern void func_80040B54(void);
extern void func_80040BF4(void);
extern void func_80040C44(void);
extern void func_80040B04(void);
extern void func_8007105C(void);
extern void func_8006D780(s32);
extern void func_80072260(void);
extern void func_80045914(void);
extern u8 D_80123750;
extern s8 D_800DEED4;
extern u8 D_80121B5A;
extern u8 D_593D10;
extern u8 D_598A70;
extern u8 D_60F1A0;
extern u8 D_60F990;
extern void func_8000E874(void);

void func_8003F520(void) {
    D_80121B5B = 1;
    func_8009956C(func_8003F554, 0);
}

void func_8003F554(void) {
    func_800437F0(&D_1F1A90, &D_1F2220, 0x28);
    func_800437F0(&D_593D10, &D_598A70, 0x29);
    func_800437F0(&D_60F1A0, &D_60F990, 0x2A);
    func_800437F0(&D_245A80, &D_24C8E0, 0x1F);
    func_800438EC(&D_1467B0, &D_147910, 8);
    func_800437F0(&D_1DE360, &D_1E0F70, 9);
    D_8010B1F0 = 0;
    D_800DEED4 = 0;
    D_800DEF14 = 0xFF;
    func_800704F0();
    D_801124B8 = 0x80;
    func_80070EC0(0);
    func_80070C64(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    func_8006D5CC();
    func_8006D520(0, 0x1F);
    func_80071664(&func_80055678, 0, 0x64, 0);
    func_80071408(&func_80051854, 0, 0x64);
    func_8009956C(&func_8003F6C0, 0);
    func_800720E4(7);
}

void func_8003F6C0(void) {
    D_800DEF14 -= 0x10;
    if (D_800DEF14 <= 0) {
        D_800DEF14 = 0;
        func_8009956C(&func_8003F718, 0);
    }
    func_8006D780(0);
    func_8007105C();
}

void func_8003F718(void) {
    extern void func_8006D780(s32);
    extern void func_8007105C(void);
    extern void func_8003F778(void);

    if (D_8010B1F0 != 0) {
        if (D_80121B5B == 0xC) {
            func_80072114(0x3C);
        }
        func_8009956C(&func_8003F778, 0);
    }
    func_8006D780(0);
    func_8007105C();
}

void func_8003F778(void) {
    D_800DEF14 += 0x10;
    if (D_800DEF14 >= 0xFF) {
        D_800DEF14 = 0xFF;
        D_80123751 = 1;
        func_8009956C(&func_8003F7E4, 0);
    }
    func_8006D780(0);
    func_8007105C();
}

void func_8003F7E4(void) {
    if (D_80123750 == 2) {
        func_80045914();
        D_80123751 = 0;
        D_800DEED4 = 0;
        if (D_80121B5B != 0xC) {
            func_8009956C(&func_8003F864, 0);
        } else {
            D_801235B4 = 0;
            func_80099658(3);
            func_8009954C(4);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_8003F864.s")

void func_8003FB70(void) {
    D_800DEF14 -= 0x10;
    if (D_800DEF14 < 0) {
        D_800DEF14 = 0;
        func_8009956C(&func_8003FBE8, 0);
    }
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    D_801124B8 = 0x80;
}

extern void func_8003FC60(void);

void func_8003FBE8(void) {
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    if (D_8010B1F0 != 0) {
        D_801235B8->unk18 = 0;
        func_80072114(0x20);
        func_8009956C(&func_8003FC60, 0);
    }
    D_801124B8 = 0x80;
}

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_8003FC60.s")

void func_8003FE54(void) {
    D_801235B4 = 0;
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    func_8007AA50();
    D_801235B8->unk18 += 1;
    if (D_801235B8->unk18 == D_800D3C90[D_80121B5B]) {
        func_8009956C(func_8003FEF4, 0);
        func_80072114(0x40);
    }
}

void func_8003FEF4(void) {
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    func_8007AA50();
    D_800DEF14 += 0x10;
    if (D_800DEF14 >= 0xFF) {
        D_80123751 = 1;
        {
            void (*func_ptr)(void) = func_8003FF78;
            func_8009956C(func_ptr, 0);
        }
    }
}

void func_8003FF78(void) {
    if (D_80123750 == 2) {
        func_80072260();
        func_80045914();
        D_80123751 = 0;
        D_800DEED4 = 0;
        func_8009956C(&func_8003F554, 0);
    }
}

void func_8003FFD0(void) {
    D_80121B5A = 1;
    func_8009956C(&func_80040004, 0);
}

extern void func_8004002C(void);

void func_80040004(void) {
    func_8009956C(&func_8004002C, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_8004002C.s")

void func_80040360(void) {
    D_800DEF14 -= 0x10;
    if (D_800DEF14 < 0) {
        D_800DEF14 = 0;
        func_8009956C(func_800403D8, 0);
    }
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    D_801124B8 = 0x80;
}

void func_800403D8(void) {
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    if (D_8010B1F0 != 0) {
        D_801235B8->unk18 = 0;
        func_80072114(0x20);
        func_8009956C(&func_80040450, 0);
    }
    D_801124B8 = 0x80;
}

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_80040450.s")

#pragma GLOBAL_ASM("asm/nonmatchings/40120/func_80040638.s")

void func_800407AC(void) {
    s32 temp_v1;

    D_80121B56 = 1;
    D_8010B1F0 = 0;
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    func_8007AA50();
    D_801235B8->unk18 += 0x10;
    temp_v1 = D_801235B8->unk18;
    if (temp_v1 == 0x80) {
        func_80071408(func_8000DF28, 0, 0x64);
        func_8009956C(func_8004086C, 0);
    }
    temp_v1 = D_801235B8->unk18;
    D_801124B8 = temp_v1;
}

void func_8004086C(void) {
    D_80121B56 = 1;
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    func_8007AA50();
    D_801124B8 = 0x80;
    if (D_8010B1F0 != 0) {
        func_8009956C(&func_800408E4, 0);
    }
}

void func_800408E4(void) {
    D_80121B56 = 1;
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    func_8007AA50();
    D_801124B8 = 0x80;
    D_800DEF14 += 0x10;
    if (D_800DEF14 >= 0xFF) {
        D_80123751 = 1;
        func_8009956C(&func_8004097C, 0);
    }
}

void func_8004097C(void) {
    if (D_80123750 == 2) {
        func_80072260();
        func_80045914();
        D_80123751 = 0;
        D_800DEED4 = 0;
        if (D_8010B1F0 == 1) {
            if (D_80121B5A < 9) {
                D_80121B5A += 1;
            }
            func_8009956C(func_80040004, 0);
        }
        if (D_8010B1F0 == 2) {
            func_8009956C(func_8004002C, 0);
        }
        if (D_8010B1F0 == 3) {
            func_8009956C(func_80040A48, 0);
        }
    }
}

void func_80040A48(void) {
    if (D_80121B5A != 9) {
        func_8009956C(func_80040C44, 0);
        return;
    }
    func_800437F0(&D_593D10, &D_598A70, 0x29);
    func_800437F0(&D_60F1A0, &D_60F990, 0x2A);
    D_8010B1F0 = 0;
    D_800DEED4 = 0;
    D_800DEF14 = 0xFF;
    func_800704F0();
    func_80070EC0(0);
    func_80071408(&func_8000E874, 0, 0x64);
    func_8009956C(func_80040B04, 0);
}

void func_80040B04(void) {
    D_800DEF14 -= 0x10;
    if (D_800DEF14 <= 0) {
        D_800DEF14 = 0;
        func_8009956C(&func_80040B54, 0);
    }
    func_8007105C();
}

void func_80040B54(void) {
    if (D_8010B1F0 != 0) {
        func_8009956C(&func_80040B90, 0);
    }
    func_8007105C();
}

void func_80040B90(void) {
    D_800DEF14 += 0x10;
    if (D_800DEF14 >= 0xFF) {
        D_800DEF14 = 0xFF;
        D_80123751 = 1;
        func_8009956C(&func_80040BF4, 0);
    }
    func_8007105C();
}

void func_80040BF4(void) {
    if (D_80123750 == 2) {
        func_80045914();
        D_80123751 = 0;
        D_800DEED4 = 0;
        func_8009956C(&func_80040C44, 0);
    }
}

void func_80040C44(void) {
    D_801235B4 = 0;
    func_80099658(3);
    func_8009954C(4);
}
