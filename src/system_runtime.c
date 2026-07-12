#include "common.h"
#include "sound_manager.h"
#include "system_runtime.h"
#include "game_task_scheduler.h"
#include "controller_main_menu_flow.h"
#include "relocatable_heap.h"
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

#include "audio_engine.h"

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

extern u8 gFadeColorRed;
extern u8 gFadeColorGreen;
extern u8 gFadeColorBlue;
extern s16 gFadeTimer;
extern u16 gRetraceCounter;

extern OSThread gBootThread;
extern OSMesgQueue gPiManagerQueue;
extern OSMesg gPiManagerMessages[];
extern OSThread gGameThread;
extern OSMesgQueue gRomDmaQueue;
extern OSMesg gRomDmaMessages[1];
extern OSMesgQueue gSchedulerClientQueue;
extern OSMesg gSchedulerClientMessages[2];
extern OSMesgQueue gControllerInputUpdateQueue;
extern OSMesg gControllerInputUpdateMessages[8];
extern OSMesgQueue gFramebufferRenderDoneQueue;
extern OSMesg gFramebufferRenderDoneMessages[8];
extern SchedulerState gSchedulerState;
extern SchedulerClient gMainSchedulerClient;
extern u16 gLastSchedulerRetraceCounter;
extern Gfx *gRegionAllocPtr;
extern s8 gRaceRecordSettingsEnabled;
extern s8 gRaceCourseModelEffectsDisabled;
extern s8 gRaceCourseOverlayEffectsDisabled;
extern RenderCallbackNode *gMenuOverlayRenderCallbackList;
extern RenderCallbackNode *D_80124848;
extern RenderCallbackNode *gMenuForegroundRenderCallbackList;
extern RenderCallbackNode *gMenuRenderCallbackList;
extern RenderCallbackNode *gRaceOverlayRenderCallbackList;
extern RenderCallbackNode *gRaceForegroundRenderCallbackList;
extern RenderCallbackNode *gModelRenderCallbackList;
extern RenderCallbackNode *gBackdropRenderCallbackList;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferRenderTask0Statuses;
extern u8 gRaceRumbleEnabled;
extern u8 gRumblePakConnectedMask;
extern u8 gFramebufferRenderTask1Statuses;
extern s32 gClearFramebufferOnNextTask;
extern s8 gMenuFadeOverlayActive;
extern Gfx *gCurrentTaskDisplayListStart;
extern u8 gFramebufferColorBufferIndex;
extern BootTaskHeader gFramebufferRenderTask0;
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
extern BootTaskHeader2 gFramebufferRenderTask1;
extern GfxCommandDest gIdentityMatrix;
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

extern void initMenuAssetHandles(void);
extern void allocRenderCallbackScratchBuffer(void);
extern void allocMenuRenderScratchBuffers(void);
extern void selectMenuRenderScratchBuffer(s32);
extern void appendViewportDisplayLists(u8);
extern s32 osSendMesg(void *, void *, s32);
extern void updateGameTaskScheduler(void);
extern void gameThreadMain(void *);
extern void initFramebufferRenderTaskState(void);

void main(void *arg) {
    osInitialize();
    osCreateThread(&gBootThread, BOOT_THREAD_ID, bootThreadMain, arg, D_80324480, THREAD_PRIORITY);
    osStartThread(&gBootThread);
}

void bootThreadMain(void *arg) {
    osCreatePiManager(PI_MANAGER_PRIORITY, &gPiManagerQueue, gPiManagerMessages, PI_MANAGER_MSG_COUNT);
    osCreateThread(&gGameThread, MAIN_THREAD_ID, gameThreadMain, arg, D_80328480, THREAD_PRIORITY);
    osStartThread(&gGameThread);
    osSetThreadPri(NULL, 0);
    while (1) {
        ;
    }
}

void initGameSystems(void) {
    osCreateMesgQueue(&gRomDmaQueue, gRomDmaMessages, 1);
    osCreateMesgQueue(&gSchedulerClientQueue, gSchedulerClientMessages, 2);
    osCreateMesgQueue(&gControllerInputUpdateQueue, gControllerInputUpdateMessages, 8);
    osCreateMesgQueue(&gFramebufferRenderDoneQueue, gFramebufferRenderDoneMessages, 8);
    if (osTvType == OS_TV_NTSC) {
        initScheduler(&gSchedulerState, RETRACE_COUNT_NTSC, RETRACE_COUNT_MODE);
    } else {
        initScheduler(&gSchedulerState, RETRACE_COUNT_PAL, RETRACE_COUNT_MODE);
    }
    addSchedulerClient(&gSchedulerState, &gMainSchedulerClient, &gSchedulerClientQueue);
    initRelocatableHeap();
    initMenuAssetHandles();
    allocRenderCallbackScratchBuffer();
    allocMenuRenderScratchBuffers();
    initGameTaskScheduler();
    initFramebufferRenderTaskState();
    initControllerSubsystem();
    resetAllViewports();
    initSoundManager();
    createGameTask(0, initStartupControllerPakFlow, STARTUP_CALLBACK_PRIORITY);
    gRaceRecordSettingsEnabled = 0;
    gRaceCourseModelEffectsDisabled = 0;
    gRaceCourseOverlayEffectsDisabled = 0;
    gRaceRumbleEnabled = 0;
    gRumblePakConnectedMask = 0;
}

// gameThreadMain best match: 90.409% at nonmatchings/gameThreadMain-5802343343535905907/base_8.c.
#ifdef NON_MATCHING
void gameThreadMain(void *arg0) {
    OSMesg msg;
    s32 done;
    s32 initialized;
    s32 finalType;
    OSMesgQueue *queue18;
    OSMesgQueue *queue70;
    u32 zero;
    OSMesgQueue *queue50;
    volatile u8 *counter;

    msg = NULL;
    zero = 0;
    initialized = (done = 0);
    initGameSystems();
    queue18 = &gFramebufferRenderDoneQueue;
    queue70 = &gControllerInputUpdateQueue;
    queue50 = &gSchedulerClientQueue;
    counter = &gPendingFramebufferSwapCount;
loop_1:
    do {
        if ((osRecvMesg(queue18, &msg, zero) != zero) && (osRecvMesg(queue70, &msg, OS_MESG_NOBLOCK) != 0) && (osRecvMesg(queue50, &msg, OS_MESG_NOBLOCK) != 0)) {
            goto loop_1;
        }
        switch (*(s16 *)msg) {
        case 1:
            gLastSchedulerRetraceCounter = gRetraceCounter;
            if (initialized == 0) {
                initialized = 1;
                updateGameTaskScheduler();
                updateSoundManager();
                serviceRumbleMotorRequest(0);
                serviceRumbleMotorRequest(1);
                serviceRumbleMotorRequest(2);
                serviceRumbleMotorRequest(3);
                requestControllerRead();
            } else {
                initialized = 0;
            }
            break;
        case 5:
            *counter += 1;
            gFramebufferRenderTask0Statuses &= 0xFFFE;
            break;
        case 6:
            *counter += 1;
            gFramebufferRenderTask1Statuses &= 0xFFFE;
            break;
        case 3:
            *(volatile s32 *)&done = 1;
            break;
        case 9:
            updateControllerInputState();
            break;
        }
    } while (*(volatile s32 *)&done == 0);
    fadeOutAllMusicSequences();
    osViBlack(1);
    requestRumbleMotorInit(0);
    requestRumbleMotorInit(1);
    requestRumbleMotorInit(2);
    requestRumbleMotorInit(3);
    serviceRumbleMotorRequest(0);
    serviceRumbleMotorRequest(1);
    serviceRumbleMotorRequest(2);
    serviceRumbleMotorRequest(3);
    requestRumbleMotorInit(0);
    requestRumbleMotorInit(1);
    requestRumbleMotorInit(2);
    requestRumbleMotorInit(3);
    serviceRumbleMotorRequest(0);
    serviceRumbleMotorRequest(1);
    serviceRumbleMotorRequest(2);
    serviceRumbleMotorRequest(3);
    requestRumbleMotorInit(0);
    requestRumbleMotorInit(1);
    requestRumbleMotorInit(2);
    requestRumbleMotorInit(3);
    serviceRumbleMotorRequest(0);
    serviceRumbleMotorRequest(1);
    serviceRumbleMotorRequest(2);
    serviceRumbleMotorRequest(3);
    finalType = 1;
loop_16:
    do {
        zero = 0;
loop_17:
        if ((osRecvMesg(queue18, &msg, zero) != zero) && (osRecvMesg(queue70, &msg, OS_MESG_NOBLOCK) != 0)) {
            if (osRecvMesg(queue50, &msg, OS_MESG_NOBLOCK) != 0) {
                goto loop_17;
            }
        }
    } while (*(s16 *)msg != finalType);
    requestRumbleMotorInit(zero);
    requestRumbleMotorInit(1);
    requestRumbleMotorInit(2);
    requestRumbleMotorInit(3);
    serviceRumbleMotorRequest(0);
    serviceRumbleMotorRequest(1);
    serviceRumbleMotorRequest(2);
    serviceRumbleMotorRequest(3);
    goto loop_16;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/system_runtime/gameThreadMain.s")
#endif

void dmaReadRom(u32 devAddr, void *dramAddr, s32 size) {
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
        osPiStartDma(&mb, 0, OS_READ, devAddr, dramAddr, chunk, &gRomDmaQueue);
        osRecvMesg(&gRomDmaQueue, &msg, OS_MESG_BLOCK);
        size -= chunk;
        devAddr += chunk;
        dramAddr = (void *)((u8 *)dramAddr + chunk);
    }
}

// appendViewportDisplayLists best match: 11.115% (nonmatchings/appendViewportDisplayLists-8207005055717715604/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/system_runtime/appendViewportDisplayLists.s")

void resetRenderCallbackQueues(void) {
    u32 end;
    CallbackQueueGroup *group;

    gMenuForegroundRenderCallbackList = NULL;
    gRaceForegroundRenderCallbackList = NULL;
    do { end = (u32)&gBackdropRenderCallbackList; group = (CallbackQueueGroup *)&gModelRenderCallbackList; loop: group++; group[-1].entry0.next = NULL; group[-1].entry1.next = NULL; } while (0);
    group[-1].entry2.next = NULL;
    group[-1].entry3.next = NULL;
    if ((u32)group != end) {
        goto loop;
    }
    gBackdropRenderCallbackList = NULL;
    gMenuOverlayRenderCallbackList = NULL;
    D_80124848 = NULL;
    gRaceOverlayRenderCallbackList = NULL;
    gMenuRenderCallbackList = NULL;
}

void initFramebufferRenderTaskState(void) {
    gFramebufferRenderTask0.msgType = 5;
    gFramebufferRenderTask0.framebuffer = D_8038E800;
    gFramebufferRenderTask1.msgType = 6;
    if (1) {
        gFramebufferRenderTask1.framebuffer = D_803B4000;
    }
    osViSetSpecialFeatures(0x6A);
    gFramebufferRenderTask0Statuses = 0;
    gFramebufferRenderTask1Statuses = 0;
    gFramebufferColorBufferIndex = 0;
    D_80124C28 = gIdentityMatrix;
    D_80124C68 = gIdentityMatrix;
    D_80124CA8 = gIdentityMatrix;
    D_80124CE8 = gIdentityMatrix;
    D_80124D28 = gIdentityMatrix;
    D_80124D68 = gIdentityMatrix;
    D_80124DA8 = gIdentityMatrix;
    D_80124DE8 = gIdentityMatrix;
    D_8013D248 = gIdentityMatrix;
    D_8013D288 = gIdentityMatrix;
    D_8013D2C8 = gIdentityMatrix;
    D_8013D308 = gIdentityMatrix;
    D_8013D348 = gIdentityMatrix;
    D_8013D388 = gIdentityMatrix;
    D_8013D3C8 = gIdentityMatrix;
    D_8013D408 = gIdentityMatrix;
    gMenuFadeOverlayActive = 0;
}

void setBootFadeColor(u8 a0, u8 a1, u8 a2) {
    gFadeColorRed = a0;
    gFadeColorGreen = a1;
    gFadeColorBlue = a2;
    gFadeTimer = BOOT_FADE_TIMER;
}

void setTitleFadeColor(u8 a0, u8 a1, u8 a2) {
    gFadeColorRed = a0;
    gFadeColorGreen = a1;
    gFadeColorBlue = a2;
    gFadeTimer = TITLE_FADE_TIMER;
}

// appendFadeOverlayDisplayList best match: 88.485% at nonmatchings/appendFadeOverlayDisplayList-9017456803007796287/base_19.c.
#ifdef NON_MATCHING
Gfx *appendFadeOverlayDisplayList(void) {
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
    fade = gFadeTimer;
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
    gfx->words.w1 = (gFadeColorRed << 24) | ((gFadeColorGreen << 8) << 8) | (gFadeColorBlue << 8) | 0xFF;

    return ret;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/system_runtime/appendFadeOverlayDisplayList.s")
#endif

// submitFramebufferRenderTask best match: 80.089% at nonmatchings/submitFramebufferRenderTask-8207005055717715604/base_5.c.
#ifdef NON_MATCHING
void submitFramebufferRenderTask(u8 arg0) {
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

    colorIndex = gFramebufferColorBufferIndex + 1;
    colorIndex &= 0xFF;
    bufferIndex = arg0 & 0xFF;
    gFramebufferColorBufferIndex = colorIndex;
    if (colorIndex >= 3) {
        gFramebufferColorBufferIndex = 0;
        colorIndex = 0;
    }

    one = 1;
    allBits = 0xFFFF;
    task = (BootSchedulerTask *)((u8 *)&gFramebufferRenderTask0 + bufferIndex * 0x18620);
    task->unk60 = D_8038E800 + colorIndex * 0x25800;
    selectMenuRenderScratchBuffer(bufferIndex);

    gRegionAllocPtr = (Gfx *)((u8 *)task + 0x620);
    gCurrentTaskDisplayListStart = task->dlStart;

    BOOT_GFX_CMD(0xBC000006, 0);
    BOOT_GFX_CMD(0xED000000, 0x5003C0);
    BOOT_GFX_CMD(0xB6000000, 0x33205);
    BOOT_GFX_CMD(0xBC000404, one);
    BOOT_GFX_CMD(0xBC000C04, one);
    BOOT_GFX_CMD(0xBC001404, allBits);
    BOOT_GFX_CMD(0xBC001C04, allBits);
    BOOT_GFX_CMD(0xBA001701, 0);
    BOOT_GFX_CMD(0xFD10013F, D_369000);

    if (gClearFramebufferOnNextTask != 0) {
        gClearFramebufferOnNextTask = 0;
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
    appendViewportDisplayLists(arg0);
    BOOT_GFX_CMD(0xE9000000, 0);
    BOOT_GFX_CMD(0xB8000000, 0);

    task->unk40 = (u8 *)gCurrentTaskDisplayListStart + 0x5B8;
    task->unk44 = ((((u8 *)gRegionAllocPtr - (u8 *)gCurrentTaskDisplayListStart) - 0x5B8) >> 3) * 8;
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
    task->unk50 = &gFramebufferRenderDoneQueue;
    task->unk54 = task->msg;
    task->unkC = task->unk60;
    task->unk58 = (gLastSchedulerRetraceCounter + 3) & 0xFFF;
    task->unk66 |= 1;
    osSendMesg(getSchedulerGraphicsTaskQueue((s32)&gSchedulerState), task, 1);

    nextColorIndex = gFramebufferColorBufferIndex + 1;
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
    nextTask->unk50 = &gFramebufferRenderDoneQueue;
    nextTask->unk54 = 0;
    nextTask->unk3C = dramStack;
    osSendMesg(getSchedulerGraphicsTaskQueue((s32)&gSchedulerState), nextTask, 1);
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/system_runtime/submitFramebufferRenderTask.s")
#endif
