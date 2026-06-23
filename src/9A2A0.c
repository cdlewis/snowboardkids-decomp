#include "common.h"

typedef s32 OSId;
typedef s32 OSPri;

struct OSThread_s;

extern void osInitialize(void);
extern void osCreateThread(struct OSThread_s *, OSId, void (*)(void *), void *, void *, OSPri);
extern void osStartThread(struct OSThread_s *);

extern struct OSThread_s D_801237B0;
extern u8 D_80324480[];
extern void func_800996FC(void *);

extern u8 D_80156618;
extern u8 D_80156619;
extern u8 D_8015661A;
extern u16 D_800DF140;

void main(void *arg) {
    osInitialize();
    osCreateThread(&D_801237B0, 1, func_800996FC, arg, D_80324480, 0xA);
    osStartThread(&D_801237B0);
}

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

typedef struct {
    u16 type;
    u8 pri;
    u8 status;
    OSMesgQueue *retQueue;
} OSIoMesgHdr;

typedef struct {
    OSIoMesgHdr hdr;
    void *dramAddr;
    u32 devAddr;
    u32 size;
    void *piHandle;
} OSIoMesg;

extern void osInvalDCache(void *, s32);
extern s32 osPiStartDma(OSIoMesg *, s32, s32, u32, void *, u32, OSMesgQueue *);
extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);

void func_80099C44(u32 devAddr, void *dramAddr, s32 size) {
    OSIoMesg mb;
    OSMesg msg;
    s32 chunk;

    while (size != 0) {
        if (size >= 0x2001) {
            chunk = 0x2000;
        } else {
            chunk = size;
        }
        osInvalDCache(dramAddr, chunk);
        osPiStartDma(&mb, 0, 0, devAddr, dramAddr, chunk, &D_80123FF8);
        osRecvMesg(&D_80123FF8, &msg, 1);
        size -= chunk;
        devAddr += chunk;
        dramAddr = (void *)((u8 *)dramAddr + chunk);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/9A2A0/func_80099D10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9A2A0/func_8009B0E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9A2A0/func_8009B14C.s")

void func_8009B58C(u8 a0, u8 a1, u8 a2) {
    D_80156618 = a0;
    D_80156619 = a1;
    D_8015661A = a2;
    D_800DF140 = 0x3E4;
}

void func_8009B5C0(u8 a0, u8 a1, u8 a2) {
    D_80156618 = a0;
    D_80156619 = a1;
    D_8015661A = a2;
    D_800DF140 = 0x3B6;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9A2A0/func_8009B5F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9A2A0/func_8009B704.s")
