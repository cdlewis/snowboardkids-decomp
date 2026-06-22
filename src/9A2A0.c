#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/9A2A0/main.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9A2A0/func_800996FC.s")

extern void func_80000450(void);
extern void func_80042C28(void);
extern void func_800458E0(void);
extern void func_80048338(void);
extern void func_800484F0(void);
extern void func_800704F0(void);
extern void func_80071830(void);
extern void func_80098D80(void);
extern void func_800994F4(s32, void *, s32);
extern void func_8009B14C(void);
extern void func_8009C270(void *, s32, s32);
extern void func_8009CA60(void *, void *, void *);
extern void func_80072C30(void);

typedef struct OSMesgQueue_s OSMesgQueue;
typedef void *OSMesg;

extern OSMesgQueue D_80123FF8;
extern OSMesg D_80124010[1];
extern OSMesgQueue D_80124050;
extern OSMesg D_80124068[2];
extern OSMesgQueue D_80124070;
extern OSMesg D_80124088[8];
extern OSMesgQueue D_80124018;
extern OSMesg D_80124030[8];
extern u8 D_801240A8[0x778];
extern u8 D_80124820[8];
extern s8 D_8012482A;
extern s8 D_8012482B;
extern s8 D_8012482C;
extern s8 D_800EC8B0;
extern s8 D_8010ADFA;

extern s32 osTvType;

void func_80099790(void) {
    osCreateMesgQueue(&D_80123FF8, D_80124010, 1);
    osCreateMesgQueue(&D_80124050, D_80124068, 2);
    osCreateMesgQueue(&D_80124070, D_80124088, 8);
    osCreateMesgQueue(&D_80124018, D_80124030, 8);
    if (osTvType == 1) {
        func_8009C270(D_801240A8, 2, 1);
    } else {
        func_8009C270(D_801240A8, 0x1E, 1);
    }
    func_8009CA60(D_801240A8, &D_80124820, &D_80124050);
    func_80042C28();
    func_800458E0();
    func_80048338();
    func_800484F0();
    func_80098D80();
    func_8009B14C();
    func_80000450();
    func_800704F0();
    func_80071830();
    func_800994F4(0, func_80072C30, 0x64);
    D_8012482A = 0;
    D_8012482B = 0;
    D_8012482C = 0;
    D_800EC8B0 = 0;
    D_8010ADFA = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9A2A0/func_800998E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9A2A0/func_80099C44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9A2A0/func_80099D10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9A2A0/func_8009B0E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9A2A0/func_8009B14C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9A2A0/func_8009B58C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9A2A0/func_8009B5C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9A2A0/func_8009B5F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9A2A0/func_8009B704.s")
