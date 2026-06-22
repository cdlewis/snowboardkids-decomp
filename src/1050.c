#include "common.h"

typedef struct OSMesgQueue_s OSMesgQueue;
typedef struct OSPfs_s OSPfs;
typedef struct {
    u32 file_size;
    u32 game_code;
    u16 company_code;
    char ext_name[4];
    char game_name[16];
} OSPfsState;
typedef struct {
    char pad[0x18];
    s32 unk18;
    s32 unk1C;
    char pad20[4];
    s32 unk24;
} Struct801235B8;

void osRecvMesg(void *, void *, s32);
void osSendMesg(void *, s32, s32);
extern s32 osPfsInitPak(OSMesgQueue *, OSPfs *, int);
extern s32 osPfsFreeBlocks(OSPfs *, s32 *);
extern s32 osPfsNumFiles(OSPfs *, s32 *, s32 *);
extern s32 osPfsFileState(OSPfs *, s32, OSPfsState *);
extern s32 func_80013F88(s16, s32, s32);
extern void func_80045914(void);
extern void func_8009954C(s32);
extern void func_8009956C(void *, s32);
extern void func_800994F4(s32, void *, s32);
extern void func_80099614(s32);
extern void func_8006D780(s32);
extern void func_8007105C(void);
extern void func_80042034(s32);
extern void func_800428C8(s32);
extern void func_80072114(s32);
extern void func_80072138(s32, s32);
extern void func_800704F0(void);
extern void func_80070C64(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_800437F0(void *, void *, s32);
extern void func_800438EC(void *, void *, s32);
extern void func_8006D5CC(void);
extern void func_8006D520(s32, s32);
extern void func_80070EC0(s32);
extern void func_80071664(void *, s32, s32, s32);
extern void func_8009B58C(s32, s32, s32);
extern void func_800720E4(s32);
extern void func_80002A1C(void);
extern void func_80052E4C(void);
extern void func_80055678(void);
extern void func_80041CF0(void);
extern void func_80041D20(s32, s32);
extern void func_80041DD4(s32, s32);
extern void func_8004209C(s32, s32, s32, s32);
extern void func_800420FC(s32, s32, s32, s32);
extern void func_80052394(void);
extern void func_80002CE4(void);
extern void func_8000262C(void);
extern void func_80001C30(void);
extern void func_80001C80(void);
extern void func_800024A8(void);
extern void func_80002794(void);
extern void func_80002810(void);
extern void func_800028B4(void);
extern void func_80002D50(void);
extern void func_8003E600(void);
extern void func_8003ED00(void);
extern void func_8003F520(void);
extern void func_8003FFD0(void);
extern void func_80073140(void);
extern s32 D_800E4B78;
extern s32 D_800E4BB0;
extern OSMesgQueue D_800E4BD0;
extern OSPfs D_800E4C40;
extern Struct801235B8 *D_801235B8;
extern u8 D_800B30F0;
extern u8 D_800B318C;
extern u8 D_800DEED4;
extern u8 D_800E4BEE;
extern s16 D_800DEF14;
extern s16 D_801124B8;
extern s16 D_800EC8A8[];
extern u8 D_593D10;
extern u8 D_598A70;
extern u8 D_1F1A90;
extern u8 D_1F2220;
extern u8 D_60F1A0;
extern u8 D_60F990;
extern u8 D_1467B0;
extern u8 D_147910;
extern u8 D_1DE360;
extern u8 D_1E0F70;
extern u8 D_800EC8B0;
extern u8 D_8010ADFA;
extern s8 D_8010B1F0;
extern OSPfsState D_8010AF98[];
extern s32 D_8010B198;
extern s32 D_8010B19C;
extern u8 D_80123750;
extern u8 D_80123751;
extern u8 D_8012482A;
extern u8 D_8012482B;
extern u8 D_8012482C;
extern s32 D_801235B4;
extern s32 D_80123778;

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80000450.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_800005E4.s")

void func_80000960(void) {
    if ((D_800E4BEE == 0) && (D_800B30F0 != 0)) {
        osSendMesg(&D_800E4B78, 0x10, 1);
        D_800E4BEE = 1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_800009B0.s")

void func_80000A40(u16 arg0) {
    s32 sp1C;

    sp1C = 0;

    osSendMesg(&D_800E4B78, arg0 + 0x70, 1);
    osRecvMesg(&D_800E4BB0, &sp1C, 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80000A8C.s")

void func_80000C00(u16 arg0) {
    if (D_800EC8B0 != 0) {
        if (D_8010ADFA & (1 << arg0)) {
            D_800EC8A8[arg0] = 1;
        }
    }
}

void func_80000C48(u16 arg0) {
    s32 sp1C;

    sp1C = 0;

    osSendMesg(&D_800E4B78, arg0 + 0x20, 1);
    osRecvMesg(&D_800E4BB0, &sp1C, 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80000C94.s")

void func_80000DB4(u16 arg0) {
    s32 sp1C;

    sp1C = 0;

    osSendMesg(&D_800E4B78, arg0 + 0x30, 1);
    osRecvMesg(&D_800E4BB0, &sp1C, 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80000E00.s")

void func_80001010(u16 arg0) {
    s32 sp1C;

    sp1C = 0;

    osSendMesg(&D_800E4B78, arg0 + 0x40, 1);
    osRecvMesg(&D_800E4BB0, &sp1C, 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_8000105C.s")

void func_800012CC(u16 arg0) {
    s32 sp1C;

    sp1C = 0;

    osSendMesg(&D_800E4B78, arg0 + 0x50, 1);
    osRecvMesg(&D_800E4BB0, &sp1C, 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80001318.s")

void func_80001538(u16 arg0) {
    s32 sp1C;

    sp1C = 0;

    osSendMesg(&D_800E4B78, arg0 + 0x60, 1);
    osRecvMesg(&D_800E4BB0, &sp1C, 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80001584.s")

void func_80001618(void) {
    s32 sp1C;

    sp1C = 0;

    osSendMesg(&D_800E4B78, 0xA0, 1);
    osRecvMesg(&D_800E4BB0, &sp1C, 1);
}

void func_8000165C(void) {
    s32 i;

    osPfsInitPak(&D_800E4BD0, &D_800E4C40, 0);
    for (i = 0; i != 0x10; i++) {
        osPfsFileState(&D_800E4C40, i, &D_8010AF98[i]);
    }
}

void func_800016D8(u16 arg0) {
    s32 sp1C;

    sp1C = 0;

    osSendMesg(&D_800E4B78, arg0 + 0xB0, 1);
    osRecvMesg(&D_800E4BB0, &sp1C, 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80001724.s")

void func_80001858(void) {
    s32 sp1C;

    sp1C = 0;

    osSendMesg(&D_800E4B78, 0xC0, 1);
    osRecvMesg(&D_800E4BB0, &sp1C, 1);
}

void func_8000189C(void) {
    s32 pad;
    s32 maxFiles;
    s32 filesUsed;

    osPfsInitPak(&D_800E4BD0, &D_800E4C40, 0);
    osPfsFreeBlocks(&D_800E4C40, &D_8010B198);
    osPfsNumFiles(&D_800E4C40, &maxFiles, &filesUsed);
    D_8010B19C = maxFiles - filesUsed;
}

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80001904.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80001994.s")

void func_80001C30(void) {
    D_800B318C = 1;
    func_8009956C(func_80001C80, 0);
    func_800994F4(4, func_8003ED00, 0x64);
    func_80099614(3);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80001C80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80002024.s")

void func_800022B8(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = D_801235B8->unk18;
    if (temp_v0 != 0xFF) {
        D_801235B8->unk18 = func_80013F88((s16) temp_v0, 0x28, 1);
        if (D_801235B8->unk18 == 0xFF) {
            D_80123751 = 1;
        } else {
            func_8006D780(0);
            func_80042034(0);
            func_80042034(1);
            func_80042034(2);
            func_80042034(3);
            func_80042034(4);
            func_800428C8(0);
            func_800428C8(1);
            func_800428C8(2);
            func_800428C8(3);
            func_800428C8(4);
            func_8007105C();
        }
    } else if (D_80123750 == 2) {
        func_80045914();
        D_80123751 = 0;
        D_800DEED4 = 0;
        if (D_801235B8->unk24 != 0) {
            temp_v1 = D_801235B8->unk1C;
            if (temp_v1 == 0) {
                func_800994F4(2, func_80073140, 0x64);
                func_8009954C(3);
            } else if (temp_v1 == 1) {
                func_8009956C(func_800024A8, 0);
            } else {
                func_8009956C(func_800028B4, 0);
            }
        } else if (D_800B318C == 0) {
            func_8009956C(func_80001C30, 0);
        } else {
            D_800B318C = 0;
            func_8009956C(func_80001C80, 0);
            func_800994F4(4, func_8003E600, 0x64);
            func_80099614(3);
        }
    }
}

void func_800024A8(void) {
    func_800704F0();
    D_801124B8 = 0x80;
    D_800DEED4 = 0;
    func_80070C64(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    func_800438EC(&D_1467B0, &D_147910, 8);
    func_800437F0(&D_1DE360, &D_1E0F70, 9);
    func_800437F0(&D_1F1A90, &D_1F2220, 0x28);
    func_8006D5CC();
    func_8006D520(0, 0x1F);
    func_80070EC0(0);
    func_80071664(func_80055678, 0, 0x64, 0);
    D_801235B4 = 0;
    D_8010B1F0 = 0;
    func_80071664(&func_80052394, 0, 0x64, 0);
    func_8009B58C(0x20, 0x40, 0x50);
    func_80041CF0();
    func_80041D20(4, 4);
    func_80041DD4(4, 5);
    func_8004209C(4, 0xFFE00000, 0, 0x509000);
    func_800420FC(4, 0, 0x100, 0);
    func_8009956C(func_8000262C, 0);
    func_800720E4(7);
}

void func_8000262C(void) {
    s32 temp_v0;

    D_800DEF14 -= 0x10;
    if (D_800DEF14 < 0) {
        D_800DEF14 = 0;
        temp_v0 = D_80123778;
        if (temp_v0 & 0x10800) {
            if (D_801235B4 != 0) {
                func_80072138(0x19, 0x32);
                D_801235B4 -= 1;
                temp_v0 = D_80123778;
            }
        }
        if ((temp_v0 & 0x20400) && (D_801235B4 != 2)) {
            func_80072138(0x19, 0x32);
            D_801235B4 += 1;
            temp_v0 = D_80123778;
        }
        if (temp_v0 & 0xD000) {
            func_80072138(0x18, 0x32);
            if (D_80123778 & 0x4000) {
                D_801235B4 = 2;
            }
            D_8010B1F0 = 1;
            if (D_801235B4 == 2) {
                func_80072114(0x3C);
            }
            func_8009956C(func_80002794, 0);
        }
    }
    func_8006D780(0);
    func_80042034(4);
    func_800428C8(4);
    func_8007105C();
}

void func_80002794(void) {
    D_800DEF14 += 0x10;
    if (D_800DEF14 >= 0x100) {
        D_800DEF14 = 0xFF;
        D_80123751 = 1;
        func_8009956C(func_80002810, 0);
    }
    func_8006D780(0);
    func_80042034(4);
    func_800428C8(4);
    func_8007105C();
}

void func_80002810(void) {
    if (D_80123750 == 2) {
        func_80045914();
        D_80123751 = 0;
        D_800DEED4 = 0;
        func_8009956C(func_80001C80, 0);
        if (D_801235B4 == 0) {
            func_800994F4(4, func_8003FFD0, 0x64);
            func_80099614(3);
            return;
        }
        if (D_801235B4 == 1) {
            func_800994F4(4, func_8003F520, 0x64);
            func_80099614(3);
        }
    }
}

void func_800028B4(void) {
    func_800704F0();
    func_80070C64(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    D_801124B8 = 0x80;
    D_800DEED4 = 0;
    func_800437F0(&D_593D10, &D_598A70, 0x29);
    func_800437F0(&D_1F1A90, &D_1F2220, 0x28);
    func_800437F0(&D_60F1A0, &D_60F990, 0x2A);
    func_800438EC(&D_1467B0, &D_147910, 8);
    func_800437F0(&D_1DE360, &D_1E0F70, 9);
    func_8006D5CC();
    func_8006D520(0, 0x1F);
    func_80070EC0(0);
    D_801235B4 = 0;
    D_8010B1F0 = 0;
    func_80071664(&func_80052E4C, 0, 0x64, 0);
    func_80071664(&func_80055678, 0, 0x64, 0);
    func_8009B58C(0x20, 0x40, 0x50);
    func_8009956C(&func_80002A1C, 0);
    func_800720E4(7);
}

void func_80002A1C(void) {
    s32 temp_v1;

    D_800DEF14 -= 0x10;
    if (D_800DEF14 < 0) {
        D_800DEF14 = 0;
        temp_v1 = D_80123778;
        if ((temp_v1 & 0x10800) && (D_801235B4 != 0)) {
            D_801235B4 -= 1;
            func_80072138(0x19, 0x32);
            temp_v1 = D_80123778;
        }
        if ((temp_v1 & 0x20400) && (D_801235B4 != 3)) {
            D_801235B4 += 1;
            func_80072138(0x19, 0x32);
            temp_v1 = D_80123778;
        }
        if (temp_v1 & 0x4000) {
            D_801235B4 = 3;
        }
        if (temp_v1 & 0x40100) {
            switch (D_801235B4) {
            case 0:
                if (D_8012482A != 0) {
                    D_8012482A -= 1;
                    func_80072138(0x19, 0x32);
                    temp_v1 = D_80123778;
                }
                break;
            case 1:
                if (D_8012482B != 1) {
                    D_8012482B += 1;
                    func_80072138(0x19, 0x32);
                    temp_v1 = D_80123778;
                }
                break;
            case 2:
                if (D_8012482C != 1) {
                    D_8012482C += 1;
                    func_80072138(0x19, 0x32);
                    temp_v1 = D_80123778;
                }
                break;
            }
        }
        if (temp_v1 & 0x80200) {
            switch (D_801235B4) {
            case 0:
                if (D_8012482A != 1) {
                    D_8012482A += 1;
                    func_80072138(0x19, 0x32);
                    temp_v1 = D_80123778;
                }
                break;
            case 1:
                if (D_8012482B != 0) {
                    D_8012482B -= 1;
                    func_80072138(0x19, 0x32);
                    temp_v1 = D_80123778;
                }
                break;
            case 2:
                if (D_8012482C != 0) {
                    D_8012482C -= 1;
                    func_80072138(0x19, 0x32);
                    temp_v1 = D_80123778;
                }
                break;
            }
        }
        if ((temp_v1 & 0xD000) && (D_801235B4 == 3)) {
            func_80072114(0x3C);
            func_80072138(0x18, 0x32);
            func_8009956C(func_80002CE4, 0);
        }
    }
    func_8006D780(0);
    func_8007105C();
}

void func_80002CE4(void) {
    D_800DEF14 += 0x10;
    if (D_800DEF14 >= 0x100) {
        D_800DEF14 = 0xFF;
        D_80123751 = 1;
        func_8009956C(func_80002D50, 0);
    }
    func_8006D780(0);
    func_8007105C();
}

void func_80002D50(void) {
    if (D_80123750 == 2) {
        func_80045914();
        D_80123751 = 0;
        D_800DEED4 = 0;
        func_8009956C(func_80001C80, 0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80002DA0.s")
