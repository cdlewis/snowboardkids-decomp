#include "common.h"
#include "game/audio/sound_manager.h"
#include "game/engine/system_runtime.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/engine/relocatable_heap.h"
#include "game/race/flow/race_flow.h"
#include "game/engine/viewport_manager.h"
#include "PR/sptask.h"

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
#define FRAMEBUFFER_COUNT 3
#define FRAMEBUFFER_SIZE 0x25800
#define FRAMEBUFFER_RENDER_TASK_STRIDE 0x18620
#define FRAMEBUFFER_PREP_TASK_STRIDE 0x860
#define FRAMEBUFFER_TASK_DATA_OFFSET 0x620
#define FRAMEBUFFER_RENDER_DL_SIZE (FRAMEBUFFER_TASK_DATA_OFFSET - 0x68)
#define FRAMEBUFFER_PREP_DL_SIZE (FRAMEBUFFER_PREP_TASK_STRIDE - 0x60)
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

#include "game/audio/audio_engine.h"

typedef struct {
    /* 0x00 */ void *next;
    /* 0x04 */ s32 state;
    /* 0x08 */ s32 flags;
    /* 0x0C */ void *framebuffer;
    /* 0x10 */ OSTask rspTask;
    /* 0x50 */ OSMesgQueue *doneQueue;
    /* 0x54 */ OSMesg doneMsg;
    /* 0x58 */ s16 retrace;
    /* 0x5A */ u8 pad5A[6];
} FramebufferSchedulerTask;

typedef struct {
    /* 0x00000 */ FramebufferSchedulerTask schedulerTask;
    /* 0x00060 */ void *colorFramebuffer;
    /* 0x00064 */ s16 completionMsgType;
    /* 0x00066 */ u8 status;
    /* 0x67 */ u8 pad67;
    /* 0x00068 */ Gfx displayList[FRAMEBUFFER_RENDER_DL_SIZE / sizeof(Gfx)];
    /* 0x00620 */ u8 taskData[FRAMEBUFFER_RENDER_TASK_STRIDE - FRAMEBUFFER_TASK_DATA_OFFSET];
} FramebufferRenderTask;

typedef struct {
    /* 0x000 */ FramebufferSchedulerTask schedulerTask;
    /* 0x060 */ Gfx displayList[FRAMEBUFFER_PREP_DL_SIZE / sizeof(Gfx)];
} FramebufferPrepTask;

typedef char FramebufferRenderTaskSizeCheck[(sizeof(FramebufferRenderTask) == FRAMEBUFFER_RENDER_TASK_STRIDE) ? 1 : -1];
typedef char FramebufferPrepTaskSizeCheck[(sizeof(FramebufferPrepTask) == FRAMEBUFFER_PREP_TASK_STRIDE) ? 1 : -1];

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
extern FramebufferRenderTask gFramebufferRenderTask0[];
extern FramebufferPrepTask D_80155548[];
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

void gameThreadMain(void *arg0) {
    OSMesg msg;
    s32 initialized;
    s32 finalType;
    volatile s32 done;

    msg = NULL;
    initialized = 0;
    done = 0;
    initGameSystems();

    while ((u64)1) {
        if ((osRecvMesg(&gFramebufferRenderDoneQueue, &msg, OS_MESG_NOBLOCK) != 0) &&
            (osRecvMesg(&gControllerInputUpdateQueue, &msg, OS_MESG_NOBLOCK) != 0) &&
            (osRecvMesg(&gSchedulerClientQueue, &msg, OS_MESG_NOBLOCK) != 0)) {
            continue;
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
            gPendingFramebufferSwapCount++;
            gFramebufferRenderTask0[0].status = gFramebufferRenderTask0Statuses & 0xFFFE;
            break;
        case 6:
            gPendingFramebufferSwapCount++;
            gFramebufferRenderTask0[1].status = gFramebufferRenderTask1Statuses & 0xFFFE;
            break;
        case 3:
            done = 1;
            break;
        case 9:
            updateControllerInputState();
            break;
        }
        if (done != 0) {
            break;
        }
    }

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
loop_17:
        if ((osRecvMesg(&gFramebufferRenderDoneQueue, &msg, OS_MESG_NOBLOCK) != 0) &&
            (osRecvMesg(&gControllerInputUpdateQueue, &msg, OS_MESG_NOBLOCK) != 0) &&
            (osRecvMesg(&gSchedulerClientQueue, &msg, OS_MESG_NOBLOCK) != 0)) {
            goto loop_17;
        }
    } while (*(s16 *)msg != finalType);

    requestRumbleMotorInit(0);
    requestRumbleMotorInit(1);
    requestRumbleMotorInit(2);
    requestRumbleMotorInit(3);
    serviceRumbleMotorRequest(0);
    serviceRumbleMotorRequest(1);
    serviceRumbleMotorRequest(2);
    serviceRumbleMotorRequest(3);
    goto loop_16;
}

void dmaReadRom(u32 romOffset, void *ramAddress, s32 size) {
    OSIoMesg dmaRequest;
    OSMesg dmaDoneMsg;
    s32 transferSize;

    while (size != 0) {
        if (size > DMA_CHUNK_SIZE) {
            transferSize = DMA_CHUNK_SIZE;
        } else {
            transferSize = size;
        }
        osInvalDCache(ramAddress, transferSize);
        osPiStartDma(&dmaRequest, 0, OS_READ, romOffset, ramAddress, transferSize, &gRomDmaQueue);
        osRecvMesg(&gRomDmaQueue, &dmaDoneMsg, OS_MESG_BLOCK);
        size -= transferSize;
        romOffset += transferSize;
        ramAddress = (void *)((u8 *)ramAddress + transferSize);
    }
}

// appendViewportDisplayLists best match: 11.115% (nonmatchings/appendViewportDisplayLists-8207005055717715604/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/engine/system_runtime/appendViewportDisplayLists.s")

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
    gFramebufferRenderTask0[0].completionMsgType = 5;
    gFramebufferRenderTask0[0].colorFramebuffer = D_8038E800;
    gFramebufferRenderTask0[1].completionMsgType = 6;
    if (1) {
        gFramebufferRenderTask0[1].colorFramebuffer = D_803B4000;
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

void appendFadeOverlayDisplayList(void) {
    gSPFogPosition(gRegionAllocPtr++, gFadeTimer, 1000);
    gDPSetFogColor(gRegionAllocPtr++, gFadeColorRed, gFadeColorGreen, gFadeColorBlue, 255);
}

// submitFramebufferRenderTask best match: 81.299% at nonmatchings/submitFramebufferRenderTask-8808947407184708385/base_4.c.
#ifdef NON_MATCHING
void submitFramebufferRenderTask(u8 frameIndex) {
    FramebufferRenderTask *renderTask;
    FramebufferPrepTask *prepTask;
    s32 colorIndex;
    s32 bufferIndex;
    s32 nextColorIndex;
    s32 rspTextSize;
    void *dramStack;
    void *outputBuff;
    Gfx *prepDisplayList;
    u8 nextBufferIndex;
    s32 one;
    s32 allBits;

    colorIndex = gFramebufferColorBufferIndex + 1;
    colorIndex &= 0xFF;
    bufferIndex = frameIndex & 0xFF;
    gFramebufferColorBufferIndex = colorIndex;
    if (colorIndex >= FRAMEBUFFER_COUNT) {
        gFramebufferColorBufferIndex = 0;
        colorIndex = 0;
    }

    one = 1;
    allBits = 0xFFFF;
    renderTask = &gFramebufferRenderTask0[bufferIndex];
    renderTask->colorFramebuffer = D_8038E800 + colorIndex * FRAMEBUFFER_SIZE;
    frameIndex = bufferIndex;
    selectMenuRenderScratchBuffer(bufferIndex);

    gRegionAllocPtr = (Gfx *)renderTask->taskData;
    gCurrentTaskDisplayListStart = renderTask->displayList;

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
        BOOT_GFX_CMD(0xFF10013F, renderTask->colorFramebuffer);
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
        BOOT_GFX_CMD(0xFF10013F, renderTask->colorFramebuffer);
    }

    BOOT_GFX_CMD(0xB9000002, 0);
    appendViewportDisplayLists(frameIndex);
    BOOT_GFX_CMD(0xE9000000, 0);
    BOOT_GFX_CMD(0xB8000000, 0);

    renderTask->schedulerTask.rspTask.t.data_ptr = (u64 *)((u8 *)gCurrentTaskDisplayListStart + FRAMEBUFFER_RENDER_DL_SIZE);
    renderTask->schedulerTask.rspTask.t.data_size =
        ((((u8 *)gRegionAllocPtr - (u8 *)gCurrentTaskDisplayListStart) - FRAMEBUFFER_RENDER_DL_SIZE) >> 3) * 8;
    renderTask->schedulerTask.rspTask.t.ucode = (u64 *)D_800B1CC0;
    renderTask->schedulerTask.rspTask.t.type = 1;
    rspTextSize = aspMainTextStart - rspbootTextStart;
    renderTask->schedulerTask.rspTask.t.dram_stack_size = 0x400;
    outputBuff = D_80360000;
    dramStack = (u8 *)outputBuff + 0x8000;
    renderTask->schedulerTask.rspTask.t.ucode_data = (u64 *)D_800E21C0;
    renderTask->schedulerTask.rspTask.t.ucode_data_size = 0x800;
    renderTask->schedulerTask.rspTask.t.dram_stack = (u64 *)D_80368C00;
    renderTask->schedulerTask.rspTask.t.output_buff_size = (u64 *)dramStack;
    renderTask->schedulerTask.rspTask.t.yield_data_size = 0xC00;
    renderTask->schedulerTask.rspTask.t.yield_data_ptr = (u64 *)D_80368000;
    renderTask->schedulerTask.flags = 0x40;
    renderTask->schedulerTask.rspTask.t.flags = 0;
    renderTask->schedulerTask.rspTask.t.ucode_boot = (u64 *)rspbootTextStart;
    renderTask->schedulerTask.rspTask.t.ucode_boot_size = rspTextSize;
    renderTask->schedulerTask.rspTask.t.output_buff = (u64 *)outputBuff;
    renderTask->schedulerTask.next = NULL;
    renderTask->schedulerTask.doneQueue = &gFramebufferRenderDoneQueue;
    renderTask->schedulerTask.doneMsg = &renderTask->completionMsgType;
    renderTask->schedulerTask.framebuffer = renderTask->colorFramebuffer;
    renderTask->schedulerTask.retrace = (gLastSchedulerRetraceCounter + 3) & 0xFFF;
    renderTask->status |= 1;
    osSendMesg(getSchedulerGraphicsTaskQueue((s32)&gSchedulerState), renderTask, 1);

    nextColorIndex = gFramebufferColorBufferIndex + 1;
    nextBufferIndex = (bufferIndex + 1) & 1;
    if (nextColorIndex >= FRAMEBUFFER_COUNT) {
        nextColorIndex = 0;
    }

    prepTask = &D_80155548[nextBufferIndex];
    prepDisplayList = prepTask->displayList;
    gRegionAllocPtr = prepDisplayList + 1;
    prepDisplayList->words.w0 = 0xBC000006;
    prepDisplayList->words.w1 = 0;
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
    prepTask->schedulerTask.framebuffer = D_8038E800 + nextColorIndex * FRAMEBUFFER_SIZE;
    BOOT_GFX_CMD(0xFF10013F, prepTask->schedulerTask.framebuffer);
    BOOT_GFX_CMD(0xF7000000, 0x10001);
    BOOT_GFX_CMD(0xF64FC3BC, 0);
    BOOT_GFX_CMD(0xE9000000, 0);
    BOOT_GFX_CMD(0xB8000000, 0);

    prepTask->schedulerTask.rspTask.t.data_ptr = (u64 *)prepDisplayList;
    prepTask->schedulerTask.rspTask.t.data_size =
        ((((u8 *)gRegionAllocPtr - (u8 *)prepTask) - 0x60) >> 3) * 8;
    prepTask->schedulerTask.rspTask.t.type = 1;
    prepTask->schedulerTask.rspTask.t.ucode_boot = (u64 *)rspbootTextStart;
    prepTask->schedulerTask.rspTask.t.ucode_data = (u64 *)D_800E21C0;
    prepTask->schedulerTask.rspTask.t.ucode_boot_size = rspTextSize;
    prepTask->schedulerTask.rspTask.t.ucode = (u64 *)D_800B1CC0;
    prepTask->schedulerTask.rspTask.t.ucode_data_size = 0x800;
    prepTask->schedulerTask.rspTask.t.dram_stack = (u64 *)D_80368C00;
    prepTask->schedulerTask.rspTask.t.dram_stack_size = 0x400;
    prepTask->schedulerTask.rspTask.t.output_buff = (u64 *)outputBuff;
    prepTask->schedulerTask.rspTask.t.yield_data_ptr = (u64 *)D_80368000;
    prepTask->schedulerTask.rspTask.t.yield_data_size = 0xC00;
    prepTask->schedulerTask.next = NULL;
    prepTask->schedulerTask.flags = 0;
    prepTask->schedulerTask.doneQueue = &gFramebufferRenderDoneQueue;
    prepTask->schedulerTask.doneMsg = NULL;
    prepTask->schedulerTask.rspTask.t.output_buff_size = (u64 *)dramStack;
    osSendMesg(getSchedulerGraphicsTaskQueue((s32)&gSchedulerState), prepTask, 1);
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/engine/system_runtime/submitFramebufferRenderTask.s")
#endif
