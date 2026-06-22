#include "common.h"

typedef struct OSMesgQueue_s OSMesgQueue;
typedef struct OSPfs_s OSPfs;

void osRecvMesg(void *, void *, s32);
void osSendMesg(void *, s32, s32);
extern s32 osPfsInitPak(OSMesgQueue *, OSPfs *, int);
extern s32 osPfsFreeBlocks(OSPfs *, s32 *);
extern s32 osPfsNumFiles(OSPfs *, s32 *, s32 *);
extern void func_80045914(void);
extern void func_8009956C(void *, s32);
extern void func_800994F4(s32, void *, s32);
extern void func_80099614(s32);
extern void func_80001C80(void);
extern void func_8003ED00(void);
extern s32 D_800E4B78;
extern s32 D_800E4BB0;
extern OSMesgQueue D_800E4BD0;
extern OSPfs D_800E4C40;
extern u8 D_800B30F0;
extern u8 D_800B318C;
extern u8 D_800DEED4;
extern u8 D_800E4BEE;
extern s16 D_800EC8A8[];
extern u8 D_800EC8B0;
extern u8 D_8010ADFA;
extern s32 D_8010B198;
extern s32 D_8010B19C;
extern u8 D_80123750;
extern u8 D_80123751;

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

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_8000165C.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_800022B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_800024A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_8000262C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80002794.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80002810.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_800028B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80002A1C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80002CE4.s")

void func_80002D50(void) {
    if (D_80123750 == 2) {
        func_80045914();
        D_80123751 = 0;
        D_800DEED4 = 0;
        func_8009956C(func_80001C80, 0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/1050/func_80002DA0.s")
