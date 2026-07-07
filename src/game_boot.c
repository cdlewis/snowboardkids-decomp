#include "common.h"

#define BOOT_THREAD_ID 1
#define MAIN_THREAD_ID 2
#define THREAD_PRIORITY 10
#define PI_MANAGER_PRIORITY 150
#define PI_MANAGER_MSG_COUNT 200
#define DMA_CHUNK_SIZE 0x2000
#define RETRACE_COUNT_NTSC 2
#define RETRACE_COUNT_PAL 30
#define RETRACE_COUNT_MODE 1
#define STARTUP_CALLBACK_PRIORITY 100
#define BOOT_FADE_TIMER 0x3E4
#define TITLE_FADE_TIMER 0x3B6
#define OS_MESG_NOBLOCK 0
#define OS_MESG_BLOCK 1
#define OS_READ 0
#define OS_TV_NTSC 1

typedef s32 OSId;
typedef s32 OSPri;
typedef void *OSMesg;

struct OSThread_s;
struct OSPiHandle_s;

typedef struct OSThread_s OSThread;
typedef struct OSPiHandle_s OSPiHandle;

typedef struct OSMesgQueue_s {
    OSThread *mtqueue;
    OSThread *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;

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
    OSPiHandle *piHandle;
} OSIoMesg;

typedef struct {
    u8 unk0[8];
} MainSchedulerClient;

extern void osInitialize(void);
extern void osCreatePiManager(OSPri, OSMesgQueue *, OSMesg *, s32);
extern void osCreateThread(OSThread *, OSId, void (*)(void *), void *, void *, OSPri);
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern void osStartThread(OSThread *);
extern void osSetThreadPri(OSThread *, OSPri);
extern void osInvalDCache(void *, s32);
extern s32 osPiStartDma(OSIoMesg *, s32, s32, u32, void *, u32, OSMesgQueue *);
extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);

extern s32 osTvType;

extern u8 D_80156618;
extern u8 D_80156619;
extern u8 D_8015661A;
extern s16 D_800DF140;

extern OSThread D_801237B0;
extern OSMesgQueue D_80123CC0;
extern OSMesg D_80123CD8[];
extern OSThread D_80123960;
extern OSMesgQueue D_80123FF8;
extern OSMesg D_80124010[1];
extern OSMesgQueue D_80124050;
extern OSMesg D_80124068[2];
extern OSMesgQueue D_80124070;
extern OSMesg D_80124088[8];
extern OSMesgQueue D_80124018;
extern OSMesg D_80124030[8];
extern u8 D_801240A8[0x778];
extern MainSchedulerClient D_80124820;
extern Gfx *D_80124830;
extern s8 D_8012482A;
extern s8 D_8012482B;
extern s8 D_8012482C;
extern s8 D_800EC8B0;
extern s8 D_8010ADFA;
extern u8 D_80324480[];
extern u8 D_80328480[];

void func_800996FC(void *);
extern void func_80000450(void);
extern void func_80042C28(void);
extern void func_800458E0(void);
extern void func_80048338(void);
extern void func_800484F0(void);
extern void func_800704F0(void);
extern void func_80071830(void);
extern void func_80072C30(void);
extern void func_80098D80(void);
extern void func_800994F4(s32, void (*)(void), s32);
extern void func_800998E4(void *);
extern void func_8009B14C(void);
extern void func_8009C270(void *, s32, s32);
extern void func_8009CA60(void *, MainSchedulerClient *, OSMesgQueue *);

void main(void *arg) {
    osInitialize();
    osCreateThread(&D_801237B0, BOOT_THREAD_ID, func_800996FC, arg, D_80324480, THREAD_PRIORITY);
    osStartThread(&D_801237B0);
}

void func_800996FC(void *arg) {
    osCreatePiManager(PI_MANAGER_PRIORITY, &D_80123CC0, D_80123CD8, PI_MANAGER_MSG_COUNT);
    osCreateThread(&D_80123960, MAIN_THREAD_ID, func_800998E4, arg, D_80328480, THREAD_PRIORITY);
    osStartThread(&D_80123960);
    osSetThreadPri(NULL, 0);
    while (1) {
        ;
    }
}

void func_80099790(void) {
    osCreateMesgQueue(&D_80123FF8, D_80124010, 1);
    osCreateMesgQueue(&D_80124050, D_80124068, 2);
    osCreateMesgQueue(&D_80124070, D_80124088, 8);
    osCreateMesgQueue(&D_80124018, D_80124030, 8);
    if (osTvType == OS_TV_NTSC) {
        func_8009C270(D_801240A8, RETRACE_COUNT_NTSC, RETRACE_COUNT_MODE);
    } else {
        func_8009C270(D_801240A8, RETRACE_COUNT_PAL, RETRACE_COUNT_MODE);
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
    func_800994F4(0, func_80072C30, STARTUP_CALLBACK_PRIORITY);
    D_8012482A = 0;
    D_8012482B = 0;
    D_8012482C = 0;
    D_800EC8B0 = 0;
    D_8010ADFA = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_boot/func_800998E4.s")

void func_80099C44(u32 devAddr, void *dramAddr, s32 size) {
    OSIoMesg mb;
    OSMesg msg;
    s32 chunk;

    while (size != 0) {
        if (size > DMA_CHUNK_SIZE) {
            chunk = DMA_CHUNK_SIZE;
        } else {
            chunk = size;
        }
        osInvalDCache(dramAddr, chunk);
        osPiStartDma(&mb, 0, OS_READ, devAddr, dramAddr, chunk, &D_80123FF8);
        osRecvMesg(&D_80123FF8, &msg, OS_MESG_BLOCK);
        size -= chunk;
        devAddr += chunk;
        dramAddr = (void *)((u8 *)dramAddr + chunk);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_boot/func_80099D10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_boot/func_8009B0E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_boot/func_8009B14C.s")

void func_8009B58C(u8 a0, u8 a1, u8 a2) {
    D_80156618 = a0;
    D_80156619 = a1;
    D_8015661A = a2;
    D_800DF140 = BOOT_FADE_TIMER;
}

void func_8009B5C0(u8 a0, u8 a1, u8 a2) {
    D_80156618 = a0;
    D_80156619 = a1;
    D_8015661A = a2;
    D_800DF140 = TITLE_FADE_TIMER;
}

// func_8009B5F4 best match: 58.191% at nonmatchings/func_8009B5F4-1404502880690620360/base_6.c.
#ifdef NON_MATCHING
Gfx *func_8009B5F4(void) {
    Gfx *gfx;
    Gfx *gfx2;
    s32 fade;
    s32 denom;
    s32 scale;

    gfx = D_80124830;
    D_80124830 = gfx + 1;
    gfx->words.w0 = 0xBC000008;
    fade = D_800DF140;
    scale = 0x1F400;
    denom = 0x3E8 - fade;
    gfx->words.w1 = ((scale / denom) << 16) | ((((-fade << 8) + scale) / denom) & 0xFFFF);

    gfx2 = D_80124830;
    D_80124830 = gfx2 + 1;
    gfx2->words.w0 = 0xF8000000;
    gfx2->words.w1 = (D_80156618 << 24) | (D_80156619 << 16) | (D_8015661A << 8) | 0xFF;

    return gfx;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/game_boot/func_8009B5F4.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/game_boot/func_8009B704.s")
