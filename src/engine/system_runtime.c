#include "common.h"
#include "game/audio/sound_manager.h"
#include "game/engine/system_runtime.h"
#include "game/engine/game_task_scheduler.h"
#include "game/engine/render_callback.h"
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
#define FRAMEBUFFER_TASK_COUNT 2
#define FRAMEBUFFER_TASK_INDEX_MASK (FRAMEBUFFER_TASK_COUNT - 1)
#define FRAMEBUFFER_SIZE 0x25800
#define FRAMEBUFFER_CLEAR_TASK_STRIDE 0x860
#define FRAMEBUFFER_DISPLAY_LIST_OFFSET 0x620
#define FRAMEBUFFER_FRAME_DATA_SIZE (FRAMEBUFFER_DISPLAY_LIST_OFFSET - 0x68)
#define FRAMEBUFFER_CLEAR_DL_SIZE (FRAMEBUFFER_CLEAR_TASK_STRIDE - 0x60)
#define FRAMEBUFFER_TASK_BUSY 1
#define SCHEDULER_SWAPBUFFER_FLAG 0x40
#define SCHEDULER_RETRACE_MASK 0xFFF
#define FRAMEBUFFER_SWAP_RETRACE_DELAY 3
#define RSP_OUTPUT_BUFFER_SIZE 0x8000
#define RSP_UCODE_DATA_SIZE 0x800
#define RSP_DRAM_STACK_SIZE 0x400
#define RSP_YIELD_BUFFER_SIZE 0xC00
#define OS_MESG_NOBLOCK 0
#define OS_MESG_BLOCK 1
#define OS_READ 0
#define OS_TV_NTSC 1
typedef s32 OSId;
typedef s32 OSPri;

struct OSThread_s;
struct OSPiHandle_s;

typedef struct OSThread_s OSThread;
typedef struct OSPiHandle_s OSPiHandle;

#include "game/audio/audio_engine.h"

typedef union {
    u32 words[16];
    s64 forceStructureAlignment;
} RuntimeMtx;

typedef struct {
    /* 0x00000 */ SchedulerTask schedulerTask;
    /* 0x00060 */ void *framebuffer;
    /* 0x00064 */ s16 completionMessage;
    /* 0x00066 */ u8 status;
    /* 0x00067 */ u8 pad67;
    /* 0x00068 */ Gfx frameData[FRAMEBUFFER_FRAME_DATA_SIZE / sizeof(Gfx)];
    /* 0x00620 */ Gfx displayList[(FRAMEBUFFER_RENDER_TASK_STRIDE - FRAMEBUFFER_DISPLAY_LIST_OFFSET) / sizeof(Gfx)];
} FramebufferRenderTask;

typedef struct {
    /* 0x000 */ SchedulerTask schedulerTask;
    /* 0x060 */ Gfx displayList[FRAMEBUFFER_CLEAR_DL_SIZE / sizeof(Gfx)];
} FramebufferClearTask;

typedef char FramebufferRenderTaskSizeCheck[(sizeof(FramebufferRenderTask) == FRAMEBUFFER_RENDER_TASK_STRIDE) ? 1 : -1];
typedef char FramebufferClearTaskSizeCheck[(sizeof(FramebufferClearTask) == FRAMEBUFFER_CLEAR_TASK_STRIDE) ? 1 : -1];

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
extern void initMenuAsciiFontTexture(void);

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
extern u8 gPendingFramebufferSwapCount;
extern u8 gRaceRumbleEnabled;
extern u8 gRumblePakConnectedMask;
extern s32 gClearFramebufferOnNextTask;
extern u8 gMenuFadeOverlayActive;
extern Gfx *gCurrentTaskDisplayListStart;
extern u8 gFramebufferColorBufferIndex;
extern FramebufferRenderTask gFramebufferRenderTask0[];
extern FramebufferClearTask gFramebufferClearTasks[];
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
extern s32 osSendMesg(void *, void *, s32);
extern void updateGameTaskScheduler(void);
extern void initFramebufferRenderTaskState(void);

void appendFadeOverlayDisplayList(void);

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
            gFramebufferRenderTask0[0].status = gFramebufferRenderTask0Statuses[0].status & 0xFFFE;
            break;
        case 6:
            gPendingFramebufferSwapCount++;
            gFramebufferRenderTask0[1].status = gFramebufferRenderTask1Statuses[0].status & 0xFFFE;
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

typedef struct {
    s8 active;
    u8 screenBoundsValid;
    u8 clearFramebuffer;
    u8 overlayActive;
    u8 overlayR;
    u8 overlayG;
    u8 overlayB;
    u8 pad7;
    s16 overlayAlpha;
    u8 padA[6];
    Vp viewport;
    u16 perspectiveNorm;
    u16 overlayPerspectiveNorm;
    u8 pad24[4];
    RuntimeMtx projection;
    RuntimeMtx overlayProjection;
    s16 left;
    s16 top;
    s16 right;
    s16 bottom;
} RuntimeViewportState;

typedef struct {
    u8 pad0[0x30];
    s16 rotation[9];
    s16 pad42;
    s32 transformOffset[3];
    RuntimeMtx transform;
    u8 pad90[0x20];
} RuntimeViewportCamera;

typedef struct {
    u8 pad0[0x78];
    Vp viewports[4];
    RuntimeMtx projections[4];
    RuntimeMtx overlayProjections[4];
    RuntimeMtx rotations[4];
    RuntimeMtx translations[4];
    RuntimeMtx viewportMatrices[4];
} RuntimeViewportDisplayListData;

typedef struct {
    void *colorFramebuffer;
    u8 pad4[FRAMEBUFFER_RENDER_TASK_STRIDE - 4];
} RuntimeFramebufferColorTarget;

extern RuntimeViewportCamera D_801121E0[4];
extern RuntimeFramebufferColorTarget D_80124968[];
extern Vp D_800DEF18[];
extern Gfx D_800DEF28[];
extern Gfx D_800DEF90[];
extern Gfx D_800DF098[];
extern Gfx gMenuRenderModeResetDl[];
extern s16 gUiBlinkTimer;
extern s16 gMenuViewportWidth;
extern s16 gMenuViewportHeight;
extern s16 gMenuViewportCenterX;
extern s16 gMenuViewportCenterY;
extern s16 gMenuFadeAlpha;
extern s8 gRenderMatricesDirty;
extern u8 gCurrentViewportIndex;
extern Mtx *gViewportMatrix;

#define runtimeDisplayListData ((RuntimeViewportDisplayListData *)gCurrentTaskDisplayListStart)
#define runtimeViewportStates ((RuntimeViewportState *)gViewportStates)
#define runtimeModelRenderCallbackLists \
    (*(RenderCallbackNode *(*)[24])&gModelRenderCallbackList)
#define VIEWPORT_COUNT 4

void appendViewportDisplayLists(u8 frameIndex) {
    RenderCallbackNode **queue;
    s32 hasModelCallbacks;
    u32 upperMask;
    s16 left;
    s16 top;
    s32 i;

    gUiBlinkTimer++;
    gMenuViewportWidth = 288;
    gMenuViewportHeight = 208;
    gMenuViewportCenterX = 160;
    gMenuViewportCenterY = 120;

    gDPPipeSync(gRegionAllocPtr++);
    gDPSetScissor(gRegionAllocPtr++, G_SC_NON_INTERLACE, 0, 0, 320, 240);
    gSPViewport(gRegionAllocPtr++, D_800DEF18);

    if (gMenuOverlayRenderCallbackList != NULL) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
        runRenderCallbacks(&gMenuOverlayRenderCallbackList);
    }

    appendFadeOverlayDisplayList();
    gRenderMatricesDirty = 1;

    upperMask = 0xFFFF0000;
    for (gCurrentViewportIndex = 0; gCurrentViewportIndex < VIEWPORT_COUNT; gCurrentViewportIndex++) {
        if (runtimeViewportStates[gCurrentViewportIndex].screenBoundsValid != 0) {
        runtimeDisplayListData->viewports[gCurrentViewportIndex] =
            runtimeViewportStates[gCurrentViewportIndex].viewport;
        runtimeDisplayListData->projections[gCurrentViewportIndex] =
            runtimeViewportStates[gCurrentViewportIndex].projection;
        runtimeDisplayListData->overlayProjections[gCurrentViewportIndex] =
            runtimeViewportStates[gCurrentViewportIndex].overlayProjection;

        left = runtimeViewportStates[gCurrentViewportIndex].left;
        top = runtimeViewportStates[gCurrentViewportIndex].top;
        gMenuViewportWidth = runtimeViewportStates[gCurrentViewportIndex].right - left;
        gMenuViewportHeight = runtimeViewportStates[gCurrentViewportIndex].bottom - top;
        gMenuViewportCenterX = left + (gMenuViewportWidth / 2);
        gMenuViewportCenterY = top + (gMenuViewportHeight / 2);

        runtimeDisplayListData->viewportMatrices[gCurrentViewportIndex] =
            D_801121E0[gCurrentViewportIndex].transform;
        runtimeDisplayListData->viewportMatrices[gCurrentViewportIndex].words[6] = 0;
        runtimeDisplayListData->viewportMatrices[gCurrentViewportIndex].words[7] = 1;
        runtimeDisplayListData->viewportMatrices[gCurrentViewportIndex].words[14] = 0;
        runtimeDisplayListData->viewportMatrices[gCurrentViewportIndex].words[15] = 0;
        gViewportMatrix = (Mtx *)&runtimeDisplayListData->viewportMatrices[gCurrentViewportIndex];

        gDPPipeSync(gRegionAllocPtr++);
        gDPSetScissor(gRegionAllocPtr++, G_SC_NON_INTERLACE,
                      runtimeViewportStates[gCurrentViewportIndex].left,
                      runtimeViewportStates[gCurrentViewportIndex].top,
                      runtimeViewportStates[gCurrentViewportIndex].right,
                      runtimeViewportStates[gCurrentViewportIndex].bottom);
        hasModelCallbacks = 0;

        if (runtimeViewportStates[gCurrentViewportIndex].clearFramebuffer != 0) {
            gDPPipeSync(gRegionAllocPtr++);
            gDPSetCycleType(gRegionAllocPtr++, G_CYC_FILL);
            gDPSetRenderMode(gRegionAllocPtr++, G_RM_NOOP, G_RM_NOOP2);
            gDPSetColorImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, D_80369000);
            gDPSetFillColor(gRegionAllocPtr++, 0xFFFCFFFC);
            gDPFillRectangle(gRegionAllocPtr++,
                             runtimeViewportStates[gCurrentViewportIndex].left,
                             runtimeViewportStates[gCurrentViewportIndex].top,
                             runtimeViewportStates[gCurrentViewportIndex].right - 1,
                             runtimeViewportStates[gCurrentViewportIndex].bottom - 1);
            gDPPipeSync(gRegionAllocPtr++);
            gDPSetColorImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320,
                             D_80124968[frameIndex].colorFramebuffer);
        }

        if (runtimeViewportStates[gCurrentViewportIndex].overlayActive != 0) {
            gDPPipeSync(gRegionAllocPtr++);
            gDPSetCycleType(gRegionAllocPtr++, G_CYC_FILL);
            gDPSetRenderMode(gRegionAllocPtr++, G_RM_NOOP, G_RM_NOOP2);
            gDPSetColorImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320,
                             D_80124968[frameIndex].colorFramebuffer);
            gDPSetFillColor(
                gRegionAllocPtr++,
                (GPACK_RGBA5551(
                    (u8)runtimeViewportStates[gCurrentViewportIndex].overlayR,
                    (u8)runtimeViewportStates[gCurrentViewportIndex].overlayG,
                    (u8)runtimeViewportStates[gCurrentViewportIndex].overlayB, 1) << 16) |
                GPACK_RGBA5551(
                    (u8)runtimeViewportStates[gCurrentViewportIndex].overlayR,
                    (u8)runtimeViewportStates[gCurrentViewportIndex].overlayG,
                    (u8)runtimeViewportStates[gCurrentViewportIndex].overlayB, 1));
            gDPFillRectangle(gRegionAllocPtr++,
                             runtimeViewportStates[gCurrentViewportIndex].left,
                             runtimeViewportStates[gCurrentViewportIndex].top,
                             runtimeViewportStates[gCurrentViewportIndex].right - 1,
                             runtimeViewportStates[gCurrentViewportIndex].bottom - 1);
        }

        if (D_80124848 != NULL) {
            gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
            runRenderCallbacks(&D_80124848);
        }

        runtimeDisplayListData->rotations[gCurrentViewportIndex].words[0] =
            ((D_801121E0[gCurrentViewportIndex].rotation[0] << 4) & upperMask) |
            ((D_801121E0[gCurrentViewportIndex].rotation[1] >> 12) & 0xFFFF);
        runtimeDisplayListData->rotations[gCurrentViewportIndex].words[1] =
            (D_801121E0[gCurrentViewportIndex].rotation[2] << 4) & upperMask;
        runtimeDisplayListData->rotations[gCurrentViewportIndex].words[2] =
            ((D_801121E0[gCurrentViewportIndex].rotation[3] << 4) & upperMask) |
            ((D_801121E0[gCurrentViewportIndex].rotation[4] >> 12) & 0xFFFF);
        runtimeDisplayListData->rotations[gCurrentViewportIndex].words[3] =
            (D_801121E0[gCurrentViewportIndex].rotation[5] << 4) & upperMask;
        runtimeDisplayListData->rotations[gCurrentViewportIndex].words[4] =
            ((D_801121E0[gCurrentViewportIndex].rotation[6] << 4) & upperMask) |
            ((D_801121E0[gCurrentViewportIndex].rotation[7] >> 12) & 0xFFFF);
        runtimeDisplayListData->rotations[gCurrentViewportIndex].words[5] =
            (D_801121E0[gCurrentViewportIndex].rotation[8] << 4) & upperMask;
        runtimeDisplayListData->translations[gCurrentViewportIndex].words[6] =
            (D_801121E0[gCurrentViewportIndex].transformOffset[0] & upperMask) |
            ((D_801121E0[gCurrentViewportIndex].transformOffset[1] >> 16) & 0xFFFF);
        runtimeDisplayListData->translations[gCurrentViewportIndex].words[7] =
            (D_801121E0[gCurrentViewportIndex].transformOffset[2] & upperMask) | 1;

        runtimeDisplayListData->rotations[gCurrentViewportIndex].words[8] =
            ((D_801121E0[gCurrentViewportIndex].rotation[0] << 20) & upperMask) |
            ((D_801121E0[gCurrentViewportIndex].rotation[1] << 4) & 0xFFFF);
        runtimeDisplayListData->rotations[gCurrentViewportIndex].words[9] =
            (D_801121E0[gCurrentViewportIndex].rotation[2] << 20) & upperMask;
        runtimeDisplayListData->rotations[gCurrentViewportIndex].words[10] =
            ((D_801121E0[gCurrentViewportIndex].rotation[3] << 20) & upperMask) |
            ((D_801121E0[gCurrentViewportIndex].rotation[4] << 4) & 0xFFFF);
        runtimeDisplayListData->rotations[gCurrentViewportIndex].words[11] =
            (D_801121E0[gCurrentViewportIndex].rotation[5] << 20) & upperMask;
        runtimeDisplayListData->rotations[gCurrentViewportIndex].words[12] =
            ((D_801121E0[gCurrentViewportIndex].rotation[6] << 20) & upperMask) |
            ((D_801121E0[gCurrentViewportIndex].rotation[7] << 4) & 0xFFFF);
        runtimeDisplayListData->rotations[gCurrentViewportIndex].words[13] =
            (D_801121E0[gCurrentViewportIndex].rotation[8] << 20) & upperMask;
        runtimeDisplayListData->translations[gCurrentViewportIndex].words[14] =
            ((D_801121E0[gCurrentViewportIndex].transformOffset[0] << 16) & upperMask) |
            (D_801121E0[gCurrentViewportIndex].transformOffset[1] & 0xFFFF);
        runtimeDisplayListData->translations[gCurrentViewportIndex].words[15] =
            (D_801121E0[gCurrentViewportIndex].transformOffset[2] << 16) & upperMask;

        if (gBackdropRenderCallbackList != NULL) {
            gSPPerspNormalize(gRegionAllocPtr++,
                              runtimeViewportStates[gCurrentViewportIndex].overlayPerspectiveNorm);
            gSPMatrix(gRegionAllocPtr++,
                      &runtimeDisplayListData->overlayProjections[gCurrentViewportIndex],
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
            gSPViewport(gRegionAllocPtr++,
                        &runtimeDisplayListData->viewports[gCurrentViewportIndex]);
            gSPMatrix(gRegionAllocPtr++,
                      &runtimeDisplayListData->rotations[gCurrentViewportIndex],
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
            gSPMatrix(gRegionAllocPtr++,
                      &runtimeDisplayListData->translations[gCurrentViewportIndex],
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
            gSPDisplayList(gRegionAllocPtr++, D_800DEF90);
            runRenderCallbacks(&gBackdropRenderCallbackList);
        }

        for (i = 0; i < 24; i += 3) {
            if (runtimeModelRenderCallbackLists[i] != NULL) {
                hasModelCallbacks = 1;
            }
        }

        if (hasModelCallbacks != 0) {
            gSPPerspNormalize(gRegionAllocPtr++,
                              runtimeViewportStates[gCurrentViewportIndex].perspectiveNorm);
            gSPMatrix(gRegionAllocPtr++,
                      &runtimeDisplayListData->projections[gCurrentViewportIndex],
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
            gSPViewport(gRegionAllocPtr++,
                        &runtimeDisplayListData->viewports[gCurrentViewportIndex]);
            gSPMatrix(gRegionAllocPtr++,
                      &runtimeDisplayListData->rotations[gCurrentViewportIndex],
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
            gSPMatrix(gRegionAllocPtr++,
                      &runtimeDisplayListData->translations[gCurrentViewportIndex],
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
            gSPDisplayList(gRegionAllocPtr++, D_800DEF28);

            for (i = 0; i < 24; i += 3) {
                if (runtimeModelRenderCallbackLists[i] != NULL) {
                    queue = &runtimeModelRenderCallbackLists[i];
                    if (queue == &gEffectRenderCallbackList) {
                        gSPMatrix(gRegionAllocPtr++,
                                  &runtimeDisplayListData->viewportMatrices[gCurrentViewportIndex],
                                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    }
                    runRenderCallbacks(queue);
                }
            }
        }

        if ((gRaceForegroundRenderCallbackList != NULL) ||
            (gRaceOverlayRenderCallbackList != NULL)) {
            gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
            if (gRaceOverlayRenderCallbackList != NULL) {
                runRenderCallbacks(&gRaceOverlayRenderCallbackList);
            }
            if (gRaceForegroundRenderCallbackList != NULL) {
                initMenuAsciiFontTexture();
                runRenderCallbacks(&gRaceForegroundRenderCallbackList);
            }
        }

        if (runtimeViewportStates[gCurrentViewportIndex].overlayAlpha != 0) {
            gSPDisplayList(gRegionAllocPtr++, D_800DF098);
            gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0, 0, 0,
                            runtimeViewportStates[gCurrentViewportIndex].overlayAlpha);
            gSPTextureRectangle(
                gRegionAllocPtr++,
                (gMenuViewportCenterX - (gMenuViewportWidth / 2)) << 2,
                (gMenuViewportCenterY - (gMenuViewportHeight / 2)) << 2,
                (gMenuViewportCenterX + (gMenuViewportWidth / 2)) << 2,
                (gMenuViewportCenterY + (gMenuViewportHeight / 2)) << 2,
                G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
        }

            gRenderMatricesDirty = 0;
        }
    }

    gMenuViewportWidth = 288;
    gMenuViewportHeight = 208;
    gMenuViewportCenterX = 160;
    gMenuViewportCenterY = 120;

    if ((gMenuForegroundRenderCallbackList != NULL) ||
        (gMenuRenderCallbackList != NULL)) {
        gDPSetScissor(gRegionAllocPtr++, G_SC_NON_INTERLACE, 0, 0, 320, 240);
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
        if (gMenuRenderCallbackList != NULL) {
            runRenderCallbacks(&gMenuRenderCallbackList);
        }
        if (gMenuForegroundRenderCallbackList != NULL) {
            initMenuAsciiFontTexture();
            runRenderCallbacks(&gMenuForegroundRenderCallbackList);
        }
    }

    gDPPipeSync(gRegionAllocPtr++);
    gDPSetScissor(gRegionAllocPtr++, G_SC_NON_INTERLACE, 0, 0, 320, 240);

    if (gMenuFadeAlpha != 0) {
        gSPDisplayList(gRegionAllocPtr++, D_800DF098);
        if (gMenuFadeOverlayActive != 0) {
            gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 255, 255, 255, gMenuFadeAlpha);
        } else {
            gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0, 0, 0, gMenuFadeAlpha);
        }
        gSPTextureRectangle(
            gRegionAllocPtr++,
            (gMenuViewportCenterX - (gMenuViewportWidth / 2)) << 2,
            (gMenuViewportCenterY - (gMenuViewportHeight / 2)) << 2,
            (gMenuViewportCenterX + (gMenuViewportWidth / 2)) << 2,
            (gMenuViewportCenterY + (gMenuViewportHeight / 2)) << 2,
            G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
    }
}

#undef runtimeDisplayListData
#undef runtimeViewportStates
#undef runtimeModelRenderCallbackLists
#undef VIEWPORT_COUNT

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
    gFramebufferRenderTask0[0].completionMessage = 5;
    gFramebufferRenderTask0[0].framebuffer = D_8038E800;
    gFramebufferRenderTask0[1].completionMessage = 6;
    if (1) {
        gFramebufferRenderTask0[1].framebuffer = D_803B4000;
    }
    osViSetSpecialFeatures(0x6A);
    gFramebufferRenderTask0Statuses[0].status = 0;
    gFramebufferRenderTask1Statuses[0].status = 0;
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

// submitFramebufferRenderTask best match: 93.170% at nonmatchings/submitFramebufferRenderTask-210831275846872038/base_2.c.
#ifdef NON_MATCHING
void submitFramebufferRenderTask(u8 frameIndex) {
    FramebufferRenderTask *renderTask;
    FramebufferClearTask *clearTask;
    s32 framebufferIndex;
    s32 taskIndex;
    s32 clearFramebufferIndex;
    u64 ucodeBootSize;
    void *rdpOutputBufferEnd;
    void *rdpOutputBuffer;
    Gfx *clearDisplayList;
    void *clearFramebuffer;

    gFramebufferColorBufferIndex++;
    framebufferIndex = gFramebufferColorBufferIndex;
    taskIndex = frameIndex;
    if (framebufferIndex >= FRAMEBUFFER_COUNT) {
        gFramebufferColorBufferIndex = 0;
        framebufferIndex = 0;
    }

    renderTask = &gFramebufferRenderTask0[taskIndex];
    renderTask->framebuffer = D_8038E800 + framebufferIndex * FRAMEBUFFER_SIZE;
    selectMenuRenderScratchBuffer(taskIndex);

    gRegionAllocPtr = renderTask->displayList;
    gCurrentTaskDisplayListStart = renderTask->frameData;

    gSPSegment(gRegionAllocPtr++, 0, 0);
    gDPSetScissor(gRegionAllocPtr++, G_SC_NON_INTERLACE, 0, 0, 320, 240);
    gSPClearGeometryMode(gRegionAllocPtr++,
                         G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_SHADING_SMOOTH);
    gMoveWd(gRegionAllocPtr++, G_MW_CLIP, G_MWO_CLIP_RNX, 1);
    gMoveWd(gRegionAllocPtr++, G_MW_CLIP, G_MWO_CLIP_RNY, 1);
    gMoveWd(gRegionAllocPtr++, G_MW_CLIP, G_MWO_CLIP_RPX, 0xFFFF);
    gMoveWd(gRegionAllocPtr++, G_MW_CLIP, G_MWO_CLIP_RPY, 0xFFFF);
    gDPPipelineMode(gRegionAllocPtr++, G_PM_NPRIMITIVE);
    gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, D_369000);

    if (gClearFramebufferOnNextTask != 0) {
        gClearFramebufferOnNextTask = 0;
        gDPSetCycleType(gRegionAllocPtr++, G_CYC_FILL);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_NOOP, G_RM_NOOP2);
        gDPSetColorImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, D_80369000);
        gDPSetFillColor(gRegionAllocPtr++, 0xFFFCFFFC);
        gDPFillRectangle(gRegionAllocPtr++, 0, 0, 319, 239);
        gDPSetDepthImage(gRegionAllocPtr++, D_80369000);
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetColorImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, renderTask->framebuffer);
        gDPSetFillColor(gRegionAllocPtr++, 0x10001);
        gDPFillRectangle(gRegionAllocPtr++, 0, 0, 319, 239);
        gDPSetEnvColor(gRegionAllocPtr++, 0, 0, 0, 0);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0, 0, 0, 0);
        gDPSetBlendColor(gRegionAllocPtr++, 0, 0, 0, 0);
        gDPSetFogColor(gRegionAllocPtr++, 0, 0, 0, 0);
        gDPSetFillColor(gRegionAllocPtr++, 0);
        gDPSetPrimDepth(gRegionAllocPtr++, 0, 0);
        gDPSetConvert(gRegionAllocPtr++, 0, 0, 0, 0, 0, 0);
        gDPSetKeyR(gRegionAllocPtr++, 0, 0, 0);
        gDPSetKeyGB(gRegionAllocPtr++, 0, 0, 0, 0, 0, 0);
        gDPSetCombineKey(gRegionAllocPtr++, G_CK_NONE);
        gDPNoOp(gRegionAllocPtr++);
        gDPSetTileSize(gRegionAllocPtr++, G_TX_RENDERTILE, 0, 0, 0, 0);
        gDPSetTileSize(gRegionAllocPtr++, 1, 0, 0, 0, 0);
        gDPSetTileSize(gRegionAllocPtr++, 2, 0, 0, 0, 0);
        gDPSetTileSize(gRegionAllocPtr++, 3, 0, 0, 0, 0);
        gDPSetTileSize(gRegionAllocPtr++, 4, 0, 0, 0, 0);
        gDPSetTileSize(gRegionAllocPtr++, 5, 0, 0, 0, 0);
        gDPSetTileSize(gRegionAllocPtr++, 6, 0, 0, 0, 0);
        gDPSetTileSize(gRegionAllocPtr++, 7, 0, 0, 0, 0);
        gDPSetTile(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0, G_TX_RENDERTILE,
                   0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD,
                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPSetTile(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0, 1,
                   0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD,
                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPSetTile(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0, 2,
                   0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD,
                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPSetTile(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0, 3,
                   0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD,
                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPSetTile(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0, 4,
                   0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD,
                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPSetTile(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0, 5,
                   0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD,
                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPSetTile(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0, 6,
                   0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD,
                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPSetTile(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0, 7,
                   0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD,
                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    } else {
        gDPSetDepthImage(gRegionAllocPtr++, D_80369000);
        gDPSetColorImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, renderTask->framebuffer);
    }

    gDPSetAlphaCompare(gRegionAllocPtr++, G_AC_NONE);
    appendViewportDisplayLists(taskIndex);
    gDPFullSync(gRegionAllocPtr++);
    gSPEndDisplayList(gRegionAllocPtr++);

    renderTask->schedulerTask.rspTask.t.data_ptr = (u64 *)renderTask->displayList;
    renderTask->schedulerTask.rspTask.t.data_size = (u8 *)gRegionAllocPtr - (u8 *)renderTask->displayList;
    renderTask->schedulerTask.rspTask.t.ucode = (u64 *)D_800B1CC0;
    renderTask->schedulerTask.rspTask.t.type = M_GFXTASK;
    ucodeBootSize = aspMainTextStart - rspbootTextStart;
    rdpOutputBuffer = D_80360000;
    rdpOutputBufferEnd = (u8 *)rdpOutputBuffer + RSP_OUTPUT_BUFFER_SIZE;
    renderTask->schedulerTask.rspTask.t.ucode_data = (u64 *)D_800E21C0;
    renderTask->schedulerTask.rspTask.t.ucode_data_size = RSP_UCODE_DATA_SIZE;
    renderTask->schedulerTask.rspTask.t.dram_stack = (u64 *)D_80368C00;
    renderTask->schedulerTask.rspTask.t.dram_stack_size = RSP_DRAM_STACK_SIZE;
    renderTask->schedulerTask.rspTask.t.output_buff = (u64 *)rdpOutputBuffer;
    renderTask->schedulerTask.rspTask.t.output_buff_size = (u64 *)rdpOutputBufferEnd;
    renderTask->schedulerTask.rspTask.t.yield_data_ptr = (u64 *)D_80368000;
    renderTask->schedulerTask.rspTask.t.yield_data_size = RSP_YIELD_BUFFER_SIZE;
    renderTask->schedulerTask.rspTask.t.flags = 0;
    renderTask->schedulerTask.rspTask.t.ucode_boot = (u64 *)rspbootTextStart;
    renderTask->schedulerTask.rspTask.t.ucode_boot_size = ucodeBootSize;
    renderTask->schedulerTask.next = NULL;
    renderTask->schedulerTask.flags = SCHEDULER_SWAPBUFFER_FLAG;
    renderTask->schedulerTask.doneQueue = &gFramebufferRenderDoneQueue;
    renderTask->schedulerTask.doneMsg = &renderTask->completionMessage;
    renderTask->schedulerTask.framebuffer = renderTask->framebuffer;
    renderTask->schedulerTask.retrace =
        (gLastSchedulerRetraceCounter + FRAMEBUFFER_SWAP_RETRACE_DELAY) & SCHEDULER_RETRACE_MASK;
    renderTask->status |= FRAMEBUFFER_TASK_BUSY;
    osSendMesg(getSchedulerGraphicsTaskQueue(&gSchedulerState), renderTask, OS_MESG_BLOCK);

    taskIndex = (taskIndex + 1) & FRAMEBUFFER_TASK_INDEX_MASK;
    clearFramebufferIndex = gFramebufferColorBufferIndex + 1;
    if (clearFramebufferIndex >= FRAMEBUFFER_COUNT) {
        clearFramebufferIndex = 0;
    }

    clearTask = &gFramebufferClearTasks[taskIndex];
    clearDisplayList = clearTask->displayList;
    gRegionAllocPtr = clearDisplayList;
    gSPSegment(gRegionAllocPtr++, 0, 0);
    gDPSetScissor(gRegionAllocPtr++, G_SC_NON_INTERLACE, 0, 0, 320, 240);
    gSPClearGeometryMode(gRegionAllocPtr++,
                         G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_SHADING_SMOOTH);
    gMoveWd(gRegionAllocPtr++, G_MW_CLIP, G_MWO_CLIP_RNX, 1);
    gMoveWd(gRegionAllocPtr++, G_MW_CLIP, G_MWO_CLIP_RNY, 1);
    gMoveWd(gRegionAllocPtr++, G_MW_CLIP, G_MWO_CLIP_RPX, 0xFFFF);
    gMoveWd(gRegionAllocPtr++, G_MW_CLIP, G_MWO_CLIP_RPY, 0xFFFF);
    gDPPipelineMode(gRegionAllocPtr++, G_PM_NPRIMITIVE);
    gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, D_369000);
    gDPSetCycleType(gRegionAllocPtr++, G_CYC_FILL);
    gDPSetRenderMode(gRegionAllocPtr++, G_RM_NOOP, G_RM_NOOP2);
    gDPSetColorImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, D_80369000);
    gDPSetFillColor(gRegionAllocPtr++, 0xFFFCFFFC);
    gDPFillRectangle(gRegionAllocPtr++, 0, 0, 319, 239);
    gDPSetDepthImage(gRegionAllocPtr++, D_80369000);
    gDPPipeSync(gRegionAllocPtr++);
    clearFramebuffer = D_8038E800 + clearFramebufferIndex * FRAMEBUFFER_SIZE;
    gDPSetColorImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, clearFramebuffer);
    gDPSetFillColor(gRegionAllocPtr++, 0x10001);
    gDPFillRectangle(gRegionAllocPtr++, 0, 0, 319, 239);
    gDPFullSync(gRegionAllocPtr++);
    gSPEndDisplayList(gRegionAllocPtr++);

    clearTask->schedulerTask.rspTask.t.data_ptr = (u64 *)clearDisplayList;
    clearTask->schedulerTask.rspTask.t.data_size = (u8 *)gRegionAllocPtr - (u8 *)clearDisplayList;
    clearTask->schedulerTask.rspTask.t.type = M_GFXTASK;
    clearTask->schedulerTask.rspTask.t.ucode_boot = (u64 *)rspbootTextStart;
    clearTask->schedulerTask.rspTask.t.ucode_boot_size = ucodeBootSize;
    clearTask->schedulerTask.rspTask.t.ucode_data = (u64 *)D_800E21C0;
    clearTask->schedulerTask.rspTask.t.ucode = (u64 *)D_800B1CC0;
    clearTask->schedulerTask.rspTask.t.ucode_data_size = RSP_UCODE_DATA_SIZE;
    clearTask->schedulerTask.rspTask.t.dram_stack = (u64 *)D_80368C00;
    clearTask->schedulerTask.rspTask.t.dram_stack_size = RSP_DRAM_STACK_SIZE;
    clearTask->schedulerTask.rspTask.t.output_buff = (u64 *)rdpOutputBuffer;
    clearTask->schedulerTask.rspTask.t.output_buff_size = (u64 *)rdpOutputBufferEnd;
    clearTask->schedulerTask.rspTask.t.yield_data_ptr = (u64 *)D_80368000;
    clearTask->schedulerTask.rspTask.t.yield_data_size = RSP_YIELD_BUFFER_SIZE;
    clearTask->schedulerTask.next = NULL;
    clearTask->schedulerTask.flags = 0;
    clearTask->schedulerTask.doneQueue = &gFramebufferRenderDoneQueue;
    clearTask->schedulerTask.doneMsg = NULL;
    clearTask->schedulerTask.framebuffer = clearFramebuffer;
    osSendMesg(getSchedulerGraphicsTaskQueue(&gSchedulerState), clearTask, OS_MESG_BLOCK);
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/engine/system_runtime/submitFramebufferRenderTask.s")
#endif
