#include "common.h"
#include "game_audio.h"
#include "game_boot.h"
#include "input_task_scheduler.h"
#include "main_menu.h"
#include "memory_allocator.h"

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
extern void osViBlack(u8);

extern s32 osTvType;

extern u8 D_80156618;
extern u8 D_80156619;
extern u8 D_8015661A;
extern s16 D_800DF140;
extern u16 D_800DF150;

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
extern u16 D_80124828;
extern Gfx *gRegionAllocPtr;
extern s8 D_8012482A;
extern s8 D_8012482B;
extern s8 D_8012482C;
extern u8 D_80123750;
extern u8 D_8012496E;
extern s8 D_800EC8B0;
extern s8 D_8010ADFA;
extern u8 D_8013CF8E;
extern u8 D_80324480[];
extern u8 D_80328480[];

extern void func_800458E0(void);
extern void func_80048338(void);
extern void func_800484F0(void);
extern void func_800704F0(void);
extern void func_80071E80(void);
extern void func_800722B4(void);
extern void func_80072C30(void);
extern void func_80098EAC(void);
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

// func_800998E4 best match: 82.639% at nonmatchings/func_800998E4-2785870559185086986/base_4.c.
#ifdef NON_MATCHING
void func_800998E4(void *arg0) {
    OSMesg msg;
    s32 done;
    s16 type;
    s32 initialized;

    msg = NULL;
    initialized = 0;
    done = 0;
    func_80099790();
loop_1:
    do {
        if ((osRecvMesg(&D_80124018, &msg, OS_MESG_NOBLOCK) != 0) && (osRecvMesg(&D_80124070, &msg, OS_MESG_NOBLOCK) != 0) && (osRecvMesg(&D_80124050, &msg, OS_MESG_NOBLOCK) != 0)) {
            goto loop_1;
        }
        type = *(s16 *)msg;
        switch (type) {
        case 1:
            D_80124828 = D_800DF150;
            if (initialized == 0) {
                initialized = 1;
                func_80098EAC();
                func_80071E80();
                func_80000A8C(0);
                func_80000A8C(1);
                func_80000A8C(2);
                func_80000A8C(3);
                func_80000960();
            } else {
                initialized = 0;
            }
            break;
        case 5:
            D_80123750 += 1;
            D_8012496E &= 0xFE;
            break;
        case 6:
            D_80123750 += 1;
            D_8013CF8E &= 0xFE;
            break;
        case 3:
            *(volatile s32 *)&done = 1;
            break;
        case 9:
            func_800009B0();
            break;
        }
    } while (*(volatile s32 *)&done == 0);
    func_800722B4();
    osViBlack(1);
    func_80000A40(0);
    func_80000A40(1);
    func_80000A40(2);
    func_80000A40(3);
    func_80000A8C(0);
    func_80000A8C(1);
    func_80000A8C(2);
    func_80000A8C(3);
    func_80000A40(0);
    func_80000A40(1);
    func_80000A40(2);
    func_80000A40(3);
    func_80000A8C(0);
    func_80000A8C(1);
    func_80000A8C(2);
    func_80000A8C(3);
    func_80000A40(0);
    func_80000A40(1);
    func_80000A40(2);
    func_80000A40(3);
    func_80000A8C(0);
    func_80000A8C(1);
    func_80000A8C(2);
    func_80000A8C(3);
loop_16:
    do {
loop_17:
        if ((osRecvMesg(&D_80124018, &msg, OS_MESG_NOBLOCK) != 0) && (osRecvMesg(&D_80124070, &msg, OS_MESG_NOBLOCK) != 0)) {
            if (osRecvMesg(&D_80124050, &msg, OS_MESG_NOBLOCK) != 0) {
                goto loop_17;
            }
        }
    } while (*(s16 *)msg != 1);
    func_80000A40(0);
    func_80000A40(1);
    func_80000A40(2);
    func_80000A40(3);
    func_80000A8C(0);
    func_80000A8C(1);
    func_80000A8C(2);
    func_80000A8C(3);
    goto loop_16;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/game_boot/func_800998E4.s")
#endif

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

// func_8009B5F4 best match: 88.485% at nonmatchings/func_8009B5F4-9017456803007796287/base_19.c.
#ifdef NON_MATCHING
Gfx *func_8009B5F4(void) {
    Gfx *gfx;
    Gfx *ret;
    Gfx *retCopy;
    s32 negFade;
    s32 fade;
    s32 denom;
    s32 scale;

    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w0 = 0xBC000008;
    fade = D_800DF140;
    do {
    } while (0 != 0U);
    retCopy = gfx;
    negFade = -fade;
    scale = 0x1F400;
    denom = 0x3E8 - fade;
    ret = retCopy;
    gfx->words.w1 = (((u16)(scale / (0x3E8 - fade))) << 16) | ((((negFade << 8) + scale) / denom) & 0xFFFF);

    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w0 = 0xF8000000;
    gfx->words.w1 = (D_80156618 << 24) | ((D_80156619 << 8) << 8) | (D_8015661A << 8) | 0xFF;

    return ret;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/game_boot/func_8009B5F4.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/game_boot/func_8009B704.s")
