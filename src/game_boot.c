#include "common.h"
#include "game_audio.h"
#include "game_boot.h"
#include "input_task_scheduler.h"
#include "main_menu.h"
#include "memory_allocator.h"
#include "race_flow.h"
#include "viewport_manager.h"

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
#define BOOT_GFX_CMD(cmd0, cmd1) \
{ \
    Gfx *_g = gRegionAllocPtr++; \
    _g->words.w0 = (cmd0); \
    _g->words.w1 = (u32)(cmd1); \
}

typedef s32 OSId;
typedef s32 OSPri;

struct OSThread_s;
struct OSPiHandle_s;

typedef struct OSThread_s OSThread;
typedef struct OSPiHandle_s OSPiHandle;

#include "player_commands.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 pad4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ void *unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ void *unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ void *unk20;
    /* 0x24 */ s32 pad24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ void *unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ void *unk38;
    /* 0x3C */ void *unk3C;
    /* 0x40 */ void *unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ void *unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ void *unk50;
    /* 0x54 */ void *unk54;
    /* 0x58 */ s16 unk58;
    /* 0x5A */ u8 pad5A[6];
    /* 0x60 */ void *unk60;
    /* 0x64 */ u8 msg[2];
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 pad67;
    /* 0x68 */ Gfx dlStart[1];
} BootSchedulerTask;

typedef struct {
    /* 0x00 */ u8 pad0[0x60];
    /* 0x60 */ void *framebuffer;
    /* 0x64 */ s16 msgType;
} BootTaskHeader;

typedef struct {
    /* 0x000 */ u8 pad0[0x680];
    /* 0x680 */ void *framebuffer;
    /* 0x684 */ s16 msgType;
} BootTaskHeader2;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
} GfxCommandDest;

typedef struct RenderCallbackNode {
    struct RenderCallbackNode *next;
    void (*callback)(s32);
    s32 arg;
} RenderCallbackNode;

typedef struct {
    RenderCallbackNode entry0;
    RenderCallbackNode entry1;
    RenderCallbackNode entry2;
    RenderCallbackNode entry3;
} CallbackQueueGroup;

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
extern void osViSetSpecialFeatures(u32);

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
extern SchedulerClient D_80124820;
extern u16 D_80124828;
extern Gfx *gRegionAllocPtr;
extern s8 D_8012482A;
extern s8 D_8012482B;
extern s8 D_8012482C;
extern RenderCallbackNode *D_80124838;
extern RenderCallbackNode *D_80124848;
extern RenderCallbackNode *D_80124858;
extern RenderCallbackNode *D_80124868;
extern RenderCallbackNode *D_80124878;
extern RenderCallbackNode *D_80124888;
extern RenderCallbackNode *D_80124898;
extern RenderCallbackNode *D_801248F8;
extern u8 D_80123750;
extern u8 D_8012496E;
extern s8 D_800EC8B0;
extern s8 D_8010ADFA;
extern u8 D_8013CF8E;
extern s32 D_800DF144;
extern s8 D_800DEF10;
extern Gfx *D_80124904;
extern u8 D_80124834;
extern BootTaskHeader D_80124908;
extern u8 D_80155548[];
extern u8 D_369000[];
extern u8 D_800B1CC0[];
extern u8 D_800E21C0[];
extern u8 D_80360000[];
extern u8 D_80368000[];
extern u8 D_80368C00[];
extern u8 D_80369000[];
extern u8 D_8038E800[];
extern u8 D_803B4000[];
extern u8 aspMainTextStart[];
extern u8 rspbootTextStart[];
extern u8 D_80324480[];
extern u8 D_80328480[];
extern BootTaskHeader2 D_8013C908;
extern GfxCommandDest D_800DEE50;
extern GfxCommandDest D_80124C28;
extern GfxCommandDest D_80124C68;
extern GfxCommandDest D_80124CA8;
extern GfxCommandDest D_80124CE8;
extern GfxCommandDest D_80124D28;
extern GfxCommandDest D_80124D68;
extern GfxCommandDest D_80124DA8;
extern GfxCommandDest D_80124DE8;
extern GfxCommandDest D_8013D248;
extern GfxCommandDest D_8013D288;
extern GfxCommandDest D_8013D2C8;
extern GfxCommandDest D_8013D308;
extern GfxCommandDest D_8013D348;
extern GfxCommandDest D_8013D388;
extern GfxCommandDest D_8013D3C8;
extern GfxCommandDest D_8013D408;

extern void func_800458E0(void);
extern void func_80048338(void);
extern void func_800484F0(void);
extern void func_80071E80(void);
extern void func_800722B4(void);
extern void func_80048524(s32);
extern void func_80099D10(u8);
extern s32 osSendMesg(void *, void *, s32);
extern void func_80098EAC(void);
extern void func_800998E4(void *);
extern void func_8009B14C(void);

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
        func_8009C270((SchedulerState *)D_801240A8, RETRACE_COUNT_NTSC, RETRACE_COUNT_MODE);
    } else {
        func_8009C270((SchedulerState *)D_801240A8, RETRACE_COUNT_PAL, RETRACE_COUNT_MODE);
    }
    func_8009CA60((SchedulerState *)D_801240A8, &D_80124820, &D_80124050);
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

// func_800998E4 best match: 87.864% at nonmatchings/func_800998E4-7273315160691878794/base_21.c.
#ifdef NON_MATCHING
void func_800998E4(void *arg0) {
    OSMesg msg;
    s32 done;
    s32 initialized;
    s32 finalType;
    OSMesgQueue *queue18;
    OSMesgQueue *queue70;
    OSMesgQueue *queue50;
    u8 *counter;

    msg = NULL;
    initialized = 0;
    func_80099790();
    done = 0;
    queue18 = &D_80124018;
    queue70 = &D_80124070;
    queue50 = &D_80124050;
    counter = &D_80123750;
loop_1:
    do {
        if ((osRecvMesg(queue18, &msg, OS_MESG_NOBLOCK) != 0) && (osRecvMesg(queue70, &msg, OS_MESG_NOBLOCK) != 0) && (osRecvMesg(queue50, &msg, OS_MESG_NOBLOCK) != 0)) {
            goto loop_1;
        }
        switch (*(s16 *)msg) {
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
            *counter += 1;
            D_8012496E &= 0xFFFE;
            break;
        case 6:
            *counter += 1;
            D_8013CF8E &= 0xFFFE;
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
    finalType = 1;
loop_16:
    do {
loop_17:
        if ((osRecvMesg(queue18, &msg, OS_MESG_NOBLOCK) != 0) && (osRecvMesg(queue70, &msg, OS_MESG_NOBLOCK) != 0)) {
            if (osRecvMesg(queue50, &msg, OS_MESG_NOBLOCK) != 0) {
                goto loop_17;
            }
        }
    } while (*(s16 *)msg != finalType);
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

// func_80099D10 best match: 11.115% (nonmatchings/func_80099D10-8207005055717715604/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/game_boot/func_80099D10.s")

void func_8009B0E8(void) {
    u32 end;
    CallbackQueueGroup *group;

    D_80124858 = NULL;
    D_80124888 = NULL;
    do { end = (u32)&D_801248F8; group = (CallbackQueueGroup *)&D_80124898; loop: group++; group[-1].entry0.next = NULL; group[-1].entry1.next = NULL; } while (0);
    group[-1].entry2.next = NULL;
    group[-1].entry3.next = NULL;
    if ((u32)group != end) {
        goto loop;
    }
    D_801248F8 = NULL;
    D_80124838 = NULL;
    D_80124848 = NULL;
    D_80124878 = NULL;
    D_80124868 = NULL;
}

void func_8009B14C(void) {
    D_80124908.msgType = 5;
    D_80124908.framebuffer = D_8038E800;
    D_8013C908.msgType = 6;
    if (1) {
        D_8013C908.framebuffer = D_803B4000;
    }
    osViSetSpecialFeatures(0x6A);
    D_8012496E = 0;
    D_8013CF8E = 0;
    D_80124834 = 0;
    D_80124C28 = D_800DEE50;
    D_80124C68 = D_800DEE50;
    D_80124CA8 = D_800DEE50;
    D_80124CE8 = D_800DEE50;
    D_80124D28 = D_800DEE50;
    D_80124D68 = D_800DEE50;
    D_80124DA8 = D_800DEE50;
    D_80124DE8 = D_800DEE50;
    D_8013D248 = D_800DEE50;
    D_8013D288 = D_800DEE50;
    D_8013D2C8 = D_800DEE50;
    D_8013D308 = D_800DEE50;
    D_8013D348 = D_800DEE50;
    D_8013D388 = D_800DEE50;
    D_8013D3C8 = D_800DEE50;
    D_8013D408 = D_800DEE50;
    D_800DEF10 = 0;
}

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

// func_8009B704 best match: 80.089% at nonmatchings/func_8009B704-8207005055717715604/base_5.c.
#ifdef NON_MATCHING
void func_8009B704(u8 arg0) {
    BootSchedulerTask *task;
    BootSchedulerTask *nextTask;
    s32 colorIndex;
    s32 bufferIndex;
    s32 nextColorIndex;
    s32 textSize;
    void *dramStack;
    u8 nextBufferIndex;
    s32 one;
    s32 allBits;

    colorIndex = D_80124834 + 1;
    colorIndex &= 0xFF;
    bufferIndex = arg0 & 0xFF;
    D_80124834 = colorIndex;
    if (colorIndex >= 3) {
        D_80124834 = 0;
        colorIndex = 0;
    }

    one = 1;
    allBits = 0xFFFF;
    task = (BootSchedulerTask *)((u8 *)&D_80124908 + bufferIndex * 0x18620);
    task->unk60 = D_8038E800 + colorIndex * 0x25800;
    func_80048524(bufferIndex);

    gRegionAllocPtr = (Gfx *)((u8 *)task + 0x620);
    D_80124904 = task->dlStart;

    BOOT_GFX_CMD(0xBC000006, 0);
    BOOT_GFX_CMD(0xED000000, 0x5003C0);
    BOOT_GFX_CMD(0xB6000000, 0x33205);
    BOOT_GFX_CMD(0xBC000404, one);
    BOOT_GFX_CMD(0xBC000C04, one);
    BOOT_GFX_CMD(0xBC001404, allBits);
    BOOT_GFX_CMD(0xBC001C04, allBits);
    BOOT_GFX_CMD(0xBA001701, 0);
    BOOT_GFX_CMD(0xFD10013F, D_369000);

    if (D_800DF144 != 0) {
        D_800DF144 = 0;
        BOOT_GFX_CMD(0xBA001402, 0x300000);
        BOOT_GFX_CMD(0xB900031D, 0);
        BOOT_GFX_CMD(0xFF10013F, D_80369000);
        BOOT_GFX_CMD(0xF7000000, 0xFFFCFFFC);
        BOOT_GFX_CMD(0xF64FC3BC, 0);
        BOOT_GFX_CMD(0xFE000000, D_80369000);
        BOOT_GFX_CMD(0xE7000000, 0);
        BOOT_GFX_CMD(0xFF10013F, task->unk60);
        BOOT_GFX_CMD(0xF7000000, 0x10001);
        BOOT_GFX_CMD(0xF64FC3BC, 0);
        BOOT_GFX_CMD(0xFB000000, 0);
        BOOT_GFX_CMD(0xFA000000, 0);
        BOOT_GFX_CMD(0xF9000000, 0);
        BOOT_GFX_CMD(0xF8000000, 0);
        BOOT_GFX_CMD(0xF7000000, 0);
        BOOT_GFX_CMD(0xEE000000, 0);
        BOOT_GFX_CMD(0xEC000000, 0);
        BOOT_GFX_CMD(0xEB000000, 0);
        BOOT_GFX_CMD(0xEA000000, 0);
        BOOT_GFX_CMD(0xBA000801, 0);
        BOOT_GFX_CMD(0xC0000000, 0);
        BOOT_GFX_CMD(0xF2000000, 0);
        BOOT_GFX_CMD(0xF2000000, 0x01000000);
        BOOT_GFX_CMD(0xF2000000, 0x02000000);
        BOOT_GFX_CMD(0xF2000000, 0x03000000);
        BOOT_GFX_CMD(0xF2000000, 0x04000000);
        BOOT_GFX_CMD(0xF2000000, 0x05000000);
        BOOT_GFX_CMD(0xF2000000, 0x06000000);
        BOOT_GFX_CMD(0xF2000000, 0x07000000);
        BOOT_GFX_CMD(0xF5000000, 0);
        BOOT_GFX_CMD(0xF5000000, 0x01000000);
        BOOT_GFX_CMD(0xF5000000, 0x02000000);
        BOOT_GFX_CMD(0xF5000000, 0x03000000);
        BOOT_GFX_CMD(0xF5000000, 0x04000000);
        BOOT_GFX_CMD(0xF5000000, 0x05000000);
        BOOT_GFX_CMD(0xF5000000, 0x06000000);
        BOOT_GFX_CMD(0xF5000000, 0x07000000);
    } else {
        BOOT_GFX_CMD(0xFE000000, D_80369000);
        BOOT_GFX_CMD(0xFF10013F, task->unk60);
    }

    BOOT_GFX_CMD(0xB9000002, 0);
    func_80099D10(arg0);
    BOOT_GFX_CMD(0xE9000000, 0);
    BOOT_GFX_CMD(0xB8000000, 0);

    task->unk40 = (u8 *)D_80124904 + 0x5B8;
    task->unk44 = ((((u8 *)gRegionAllocPtr - (u8 *)D_80124904) - 0x5B8) >> 3) * 8;
    task->unk20 = D_800B1CC0;
    task->unk10 = 1;
    textSize = aspMainTextStart - rspbootTextStart;
    task->unk34 = 0x400;
    dramStack = D_80360000 + 0x8000;
    task->unk28 = D_800E21C0;
    task->unk2C = 0x800;
    task->unk30 = D_80368C00;
    task->unk3C = dramStack;
    task->unk4C = 0xC00;
    task->unk48 = D_80368000;
    task->unk8 = 0x40;
    task->unk14 = 0;
    task->unk18 = rspbootTextStart;
    task->unk1C = textSize;
    task->unk38 = D_80360000;
    task->unk0 = 0;
    task->unk50 = &D_80124018;
    task->unk54 = task->msg;
    task->unkC = task->unk60;
    task->unk58 = (D_80124828 + 3) & 0xFFF;
    task->unk66 |= 1;
    osSendMesg(func_8009C43C((s32)D_801240A8), task, 1);

    nextColorIndex = D_80124834 + 1;
    nextBufferIndex = (bufferIndex + 1) & 1;
    if (nextColorIndex >= 3) {
        nextColorIndex = 0;
    }

    nextTask = (BootSchedulerTask *)(D_80155548 + nextBufferIndex * 0x860);
    gRegionAllocPtr = (Gfx *)&nextTask->unk60;
    BOOT_GFX_CMD(0xBC000006, 0);
    BOOT_GFX_CMD(0xED000000, 0x5003C0);
    BOOT_GFX_CMD(0xB6000000, 0x33205);
    BOOT_GFX_CMD(0xBC000404, one);
    BOOT_GFX_CMD(0xBC000C04, one);
    BOOT_GFX_CMD(0xBC001404, allBits);
    BOOT_GFX_CMD(0xBC001C04, allBits);
    BOOT_GFX_CMD(0xBA001701, 0);
    BOOT_GFX_CMD(0xFD10013F, D_369000);
    BOOT_GFX_CMD(0xBA001402, 0x300000);
    BOOT_GFX_CMD(0xB900031D, 0);
    BOOT_GFX_CMD(0xFF10013F, D_80369000);
    BOOT_GFX_CMD(0xF7000000, 0xFFFCFFFC);
    BOOT_GFX_CMD(0xF64FC3BC, 0);
    BOOT_GFX_CMD(0xFE000000, D_80369000);
    BOOT_GFX_CMD(0xE7000000, 0);
    nextTask->unkC = D_8038E800 + nextColorIndex * 0x25800;
    BOOT_GFX_CMD(0xFF10013F, nextTask->unkC);
    BOOT_GFX_CMD(0xF7000000, 0x10001);
    BOOT_GFX_CMD(0xF64FC3BC, 0);
    BOOT_GFX_CMD(0xE9000000, 0);
    BOOT_GFX_CMD(0xB8000000, 0);

    nextTask->unk40 = &nextTask->unk60;
    nextTask->unk44 = ((((u8 *)gRegionAllocPtr - (u8 *)nextTask) - 0x60) >> 3) * 8;
    nextTask->unk10 = 1;
    nextTask->unk18 = rspbootTextStart;
    nextTask->unk28 = D_800E21C0;
    nextTask->unk1C = textSize;
    nextTask->unk20 = D_800B1CC0;
    nextTask->unk2C = 0x800;
    nextTask->unk30 = D_80368C00;
    nextTask->unk34 = 0x400;
    nextTask->unk38 = D_80360000;
    nextTask->unk48 = D_80368000;
    nextTask->unk4C = 0xC00;
    nextTask->unk0 = 0;
    nextTask->unk8 = 0;
    nextTask->unk50 = &D_80124018;
    nextTask->unk54 = 0;
    nextTask->unk3C = dramStack;
    osSendMesg(func_8009C43C((s32)D_801240A8), nextTask, 1);
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/game_boot/func_8009B704.s")
#endif
