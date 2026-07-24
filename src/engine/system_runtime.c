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
extern u8 gMenuFadeOverlayActive;
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

// appendViewportDisplayLists best match: 98.436% (nonmatchings/appendViewportDisplayLists-3379532139742180785/base_18.c)
#pragma GLOBAL_ASM("asm/nonmatchings/engine/system_runtime/appendViewportDisplayLists.s")

#ifdef NON_MATCHING
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
    Mtx projection;
    Mtx overlayProjection;
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
    Mtx transform;
    u8 pad90[0x20];
} RuntimeViewportCamera;

typedef struct {
    u8 pad0[0x78];
    Vp viewports[4];
    Mtx projections[4];
    Mtx overlayProjections[4];
    Mtx rotations[4];
    Mtx translations[4];
    Mtx viewportMatrices[4];
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
extern RenderCallbackNode *gEffectRenderCallbackList;
extern s16 gUiBlinkTimer;
extern s16 gMenuViewportWidth;
extern s16 gMenuViewportHeight;
extern s16 gMenuViewportCenterX;
extern s16 gMenuViewportCenterY;
extern s16 gMenuFadeAlpha;
extern s8 gRenderMatricesDirty;
extern u8 gCurrentViewportIndex;
extern Mtx *gViewportMatrix;

#define runtimeViewportStates ((RuntimeViewportState *)gViewportStates)
#define runtimeDisplayListData ((RuntimeViewportDisplayListData *)gCurrentTaskDisplayListStart)

void appendViewportDisplayLists(u8 frameIndex) {
    RuntimeViewportState *viewport;
    RenderCallbackNode **queue;
    s32 anyModels;
    s32 new_var2;
    s16 left;
    s64 new_var;
    s32 color;
    s16 top;

    gUiBlinkTimer++;
    gMenuViewportWidth = 0x120;
    gMenuViewportHeight = 0xD0;
    gMenuViewportCenterX = 0xA0;
    gMenuViewportCenterY = 0x78;
    gDPPipeSync(gRegionAllocPtr++);
    gDPSetScissor(gRegionAllocPtr++, G_SC_NON_INTERLACE, 0, 0, 320, 240);
    gSPViewport(gRegionAllocPtr++, D_800DEF18);
    if (gMenuOverlayRenderCallbackList != NULL) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
        runRenderCallbacks(&gMenuOverlayRenderCallbackList);
    }
    appendFadeOverlayDisplayList();
    gRenderMatricesDirty = 1;
    gCurrentViewportIndex = 0;
    if (gCurrentViewportIndex < 4) {
        do {
            viewport = &runtimeViewportStates[gCurrentViewportIndex];
            if (viewport->screenBoundsValid != 0) {
                runtimeDisplayListData->viewports[gCurrentViewportIndex] = viewport->viewport;
                runtimeDisplayListData->projections[gCurrentViewportIndex] =
                    runtimeViewportStates[gCurrentViewportIndex].projection;
                runtimeDisplayListData->overlayProjections[gCurrentViewportIndex] =
                    runtimeViewportStates[gCurrentViewportIndex].overlayProjection;
                viewport = &runtimeViewportStates[gCurrentViewportIndex];
                left = viewport->left;
                top = viewport->top;
                gMenuViewportWidth = viewport->right - left;
                gMenuViewportHeight = viewport->bottom - top;
                gMenuViewportCenterX = left + (gMenuViewportWidth / 2);
                gMenuViewportCenterY = top + (gMenuViewportHeight / 2);
                runtimeDisplayListData->viewportMatrices[gCurrentViewportIndex] =
                    D_801121E0[gCurrentViewportIndex].transform;
                runtimeDisplayListData->viewportMatrices[gCurrentViewportIndex].m[1][2] = 0;
                runtimeDisplayListData->viewportMatrices[gCurrentViewportIndex].m[1][3] = 1;
                runtimeDisplayListData->viewportMatrices[gCurrentViewportIndex].m[3][2] = 0;
                runtimeDisplayListData->viewportMatrices[gCurrentViewportIndex].m[3][3] = 0;
                gViewportMatrix = &runtimeDisplayListData->viewportMatrices[gCurrentViewportIndex];
                gDPPipeSync(gRegionAllocPtr++);
                do {
                } while (0);
                gDPSetScissor(gRegionAllocPtr++, G_SC_NON_INTERLACE,
                              runtimeViewportStates[gCurrentViewportIndex].left,
                              runtimeViewportStates[gCurrentViewportIndex].top,
                              runtimeViewportStates[gCurrentViewportIndex].right,
                              runtimeViewportStates[gCurrentViewportIndex].bottom);
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
                    gDPSetCycleType(gRegionAllocPtr++, (3 << 7) << 13);
                    gDPSetRenderMode(gRegionAllocPtr++, G_RM_NOOP, G_RM_NOOP2);
                    gDPSetColorImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320,
                                     D_80124968[frameIndex].colorFramebuffer);
                    gDPSetFillColor(
                        gRegionAllocPtr++,
                        (color = GPACK_RGBA5551(runtimeViewportStates[gCurrentViewportIndex].overlayR,
                                               runtimeViewportStates[gCurrentViewportIndex].overlayG,
                                               runtimeViewportStates[gCurrentViewportIndex].overlayB, 1)) |
                            (color << 16));
                    if (!runtimeViewportStates[gCurrentViewportIndex].bottom) {
                    }
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
                color = D_801121E0[gCurrentViewportIndex].rotation[0] << 4;
                runtimeDisplayListData->rotations[gCurrentViewportIndex].m[0][0] =
                    (color & 0xFFFF0000) |
                    ((D_801121E0[gCurrentViewportIndex].rotation[1] >> 12) & 0xFFFF);
                runtimeDisplayListData->rotations[gCurrentViewportIndex].m[0][1] =
                    (D_801121E0[gCurrentViewportIndex].rotation[2] << 4) & 0xFFFF0000;
                runtimeDisplayListData->rotations[gCurrentViewportIndex].m[0][2] =
                    ((D_801121E0[gCurrentViewportIndex].rotation[3] << 4) & 0xFFFF0000) |
                    ((D_801121E0[gCurrentViewportIndex].rotation[4] >> 12) & 0xFFFF);
                runtimeDisplayListData->rotations[gCurrentViewportIndex].m[0][3] =
                    (D_801121E0[gCurrentViewportIndex].rotation[5] << 4) & 0xFFFF0000;
                runtimeDisplayListData->rotations[gCurrentViewportIndex].m[1][0] =
                    ((D_801121E0[gCurrentViewportIndex].rotation[6] << 4) & 0xFFFF0000) |
                    ((D_801121E0[gCurrentViewportIndex].rotation[7] >> 12) & 0xFFFF);
                runtimeDisplayListData->rotations[gCurrentViewportIndex].m[1][1] =
                    (D_801121E0[gCurrentViewportIndex].rotation[8] << 4) & 0xFFFF0000;
                runtimeDisplayListData->translations[gCurrentViewportIndex].m[1][2] =
                    (D_801121E0[gCurrentViewportIndex].transformOffset[0] & 0xFFFF0000) |
                    ((D_801121E0[gCurrentViewportIndex].transformOffset[1] >> 16) & 0xFFFF);
                runtimeDisplayListData->translations[gCurrentViewportIndex].m[1][3] =
                    (D_801121E0[gCurrentViewportIndex].transformOffset[2] & 0xFFFF0000) | 1;
                anyModels = 20;
                runtimeDisplayListData->rotations[gCurrentViewportIndex].m[2][0] =
                    ((D_801121E0[gCurrentViewportIndex].rotation[0] << anyModels) & 0xFFFF0000) |
                    ((D_801121E0[gCurrentViewportIndex].rotation[1] << 4) & 0xFFFF);
                runtimeDisplayListData->rotations[gCurrentViewportIndex].m[2][1] =
                    (D_801121E0[gCurrentViewportIndex].rotation[2] << anyModels) & 0xFFFF0000;
                runtimeDisplayListData->rotations[gCurrentViewportIndex].m[2][2] =
                    ((D_801121E0[gCurrentViewportIndex].rotation[3] << 20) & 0xFFFF0000) |
                    ((D_801121E0[gCurrentViewportIndex].rotation[4] << 4) & 0xFFFF);
                runtimeDisplayListData->rotations[gCurrentViewportIndex].m[2][3] =
                    (D_801121E0[gCurrentViewportIndex].rotation[5] << anyModels) & 0xFFFF0000;
                runtimeDisplayListData->rotations[gCurrentViewportIndex].m[3][0] =
                    ((D_801121E0[gCurrentViewportIndex].rotation[6] << anyModels) & 0xFFFF0000) |
                    (0xFFFF & (D_801121E0[gCurrentViewportIndex].rotation[7] << 4));
                runtimeDisplayListData->rotations[gCurrentViewportIndex].m[3][1] =
                    (D_801121E0[gCurrentViewportIndex].rotation[8] << anyModels) & 0xFFFF0000;
                runtimeDisplayListData->translations[gCurrentViewportIndex].m[3][2] =
                    ((D_801121E0[gCurrentViewportIndex].transformOffset[0] << 16) & 0xFFFF0000) |
                    (D_801121E0[gCurrentViewportIndex].transformOffset[1] & 0xFFFF);
                (runtimeDisplayListData->translations + gCurrentViewportIndex)->m[3][3] =
                    (D_801121E0[gCurrentViewportIndex].transformOffset[2] << 16) & 0xFFFF0000;
                if (gBackdropRenderCallbackList != NULL) {
                    gSPPerspNormalize(gRegionAllocPtr++,
                                      runtimeViewportStates[gCurrentViewportIndex].overlayPerspectiveNorm);
                    gSPMatrix(gRegionAllocPtr++,
                              &runtimeDisplayListData->overlayProjections[gCurrentViewportIndex],
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
                    gSPViewport(gRegionAllocPtr++,
                                &runtimeDisplayListData->viewports[gCurrentViewportIndex]);
                    gSPMatrix(gRegionAllocPtr++, &runtimeDisplayListData->rotations[gCurrentViewportIndex],
                              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
                    gSPMatrix(gRegionAllocPtr++,
                              &runtimeDisplayListData->translations[gCurrentViewportIndex],
                              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
                    gSPDisplayList(gRegionAllocPtr++, D_800DEF90);
                    runRenderCallbacks(&gBackdropRenderCallbackList);
                }
                anyModels = 0;
                queue = &gModelRenderCallbackList;
                do {
                    if (*queue != NULL) {
                        anyModels = 1;
                    }
                    queue += 3;
                } while (((u32)queue + 1) < ((u32)&gBackdropRenderCallbackList + 1));
                if (anyModels != 0) {
                    gSPPerspNormalize(gRegionAllocPtr++,
                                      runtimeViewportStates[gCurrentViewportIndex].perspectiveNorm);
                    gSPMatrix(gRegionAllocPtr++, &runtimeDisplayListData->projections[gCurrentViewportIndex],
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
                    gSPViewport(gRegionAllocPtr++,
                                &runtimeDisplayListData->viewports[gCurrentViewportIndex]);
                    gSPMatrix(gRegionAllocPtr++, &runtimeDisplayListData->rotations[gCurrentViewportIndex],
                              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
                    gSPMatrix(gRegionAllocPtr++,
                              &runtimeDisplayListData->translations[gCurrentViewportIndex],
                              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
                    gSPDisplayList(gRegionAllocPtr++, D_800DEF28);
                    queue = &gModelRenderCallbackList;
                    do {
                        if (*queue != NULL) {
                            if ((queue == &gEffectRenderCallbackList) != 0) {
                                gSPMatrix(gRegionAllocPtr++,
                                          &runtimeDisplayListData
                                               ->viewportMatrices[gCurrentViewportIndex],
                                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                            }
                            runRenderCallbacks(queue);
                        }
                        queue += 3;
                    } while (queue != &gBackdropRenderCallbackList);
                }
                if ((gRaceForegroundRenderCallbackList != NULL) ||
                    (gRaceOverlayRenderCallbackList != NULL)) {
                    gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
                    if (gRaceOverlayRenderCallbackList != NULL) {
                        runRenderCallbacks(&gRaceOverlayRenderCallbackList);
                    }
                    if ((gRaceForegroundRenderCallbackList != NULL) != 0) {
                        initMenuAsciiFontTexture();
                        runRenderCallbacks(&gRaceForegroundRenderCallbackList);
                    }
                }
                if (runtimeViewportStates[gCurrentViewportIndex].overlayAlpha != 0) {
                    gSPDisplayList(gRegionAllocPtr++, D_800DF098);
                    gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0, 0, 0,
                                    runtimeViewportStates[gCurrentViewportIndex].overlayAlpha & 0xFF);
                    gSPTextureRectangle(gRegionAllocPtr++,
                                        (gMenuViewportCenterX - gMenuViewportWidth / 2) * 4,
                                        (gMenuViewportCenterY - gMenuViewportHeight / 2) * 4,
                                        (gMenuViewportCenterX + gMenuViewportWidth / ((0, 2))) * 4,
                                        (gMenuViewportCenterY + gMenuViewportHeight / 2) * 4,
                                        0, 0, 0, 0x400, 0x400);
                }
                gRenderMatricesDirty = 0;
            }
            gCurrentViewportIndex++;
        } while (gCurrentViewportIndex < 4);
    }
    gMenuViewportWidth = 0x120;
    gMenuViewportHeight = 0xD0;
    gMenuViewportCenterX = 0xA0;
    gMenuViewportCenterY = 0x78;
    if ((gMenuForegroundRenderCallbackList != NULL) || (gMenuRenderCallbackList != NULL)) {
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
        new_var = 0xFF;
        if (gMenuFadeOverlayActive != 0) {
            gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 255, 255, 255,
                            new_var2 = gMenuFadeAlpha & 0xFF);
        } else {
            gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0, 0, 0, gMenuFadeAlpha & new_var);
        }
        gSPTextureRectangle(gRegionAllocPtr++,
                            (gMenuViewportCenterX - gMenuViewportWidth / 2) * 4,
                            (gMenuViewportCenterY - gMenuViewportHeight / 2) * 4,
                            (gMenuViewportCenterX + gMenuViewportWidth / 2) * 4,
                            (gMenuViewportCenterY + gMenuViewportHeight / 2) * 4,
                            0, 0, 0, 0x400, 0x400);
    }
}

#undef runtimeViewportStates
#undef runtimeDisplayListData
#endif

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

// submitFramebufferRenderTask best match: 93.126% at nonmatchings/submitFramebufferRenderTask-7998791169205557824/base_30.c.
#ifdef NON_MATCHING
void submitFramebufferRenderTask(u8 frameIndex) {
    FramebufferRenderTask *renderTask;
    FramebufferPrepTask *prepTask;
    FramebufferRenderTask *renderTaskAlias;
    SchedulerState *schedulerState;
    s32 colorIndex;
    s32 bufferIndex;
    s32 nextColorIndex;
    u64 rspTextSize;
    void *dramStack;
    void *outputBuff;
    Gfx *prepDisplayList;
    void *nextFramebuffer;
    s32 one;
    s32 prepOne;
    s32 allBits;

    colorIndex = gFramebufferColorBufferIndex + 1;
    colorIndex = colorIndex & 0xFF;
    bufferIndex = frameIndex & 0xFF;
    gFramebufferColorBufferIndex = colorIndex;
    if (colorIndex >= FRAMEBUFFER_COUNT) {
        gFramebufferColorBufferIndex = 0;
        colorIndex = 0;
    }

    one = 1;
    allBits = 0xFFFF;
    renderTask = &gFramebufferRenderTask0[bufferIndex];
    nextColorIndex = 1;
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
    one = (u32)D_369000;
    BOOT_GFX_CMD(0xBC001C04, allBits);
    BOOT_GFX_CMD(0xBA001701, 0);
    BOOT_GFX_CMD(0xFD10013F, one);

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
    renderTask->schedulerTask.rspTask.t.type = nextColorIndex;
    rspTextSize = aspMainTextStart - rspbootTextStart;
    renderTaskAlias = renderTask;
    renderTaskAlias->schedulerTask.rspTask.t.dram_stack_size = 0x400;
    outputBuff = D_80360000;
    dramStack = (u8 *)outputBuff + 0x8000;
    renderTaskAlias->schedulerTask.rspTask.t.ucode_data = (u64 *)D_800E21C0;
    renderTaskAlias->schedulerTask.rspTask.t.ucode_data_size = 0x800;
    renderTaskAlias->schedulerTask.rspTask.t.dram_stack = (u64 *)D_80368C00;
    renderTaskAlias->schedulerTask.rspTask.t.output_buff_size = (u64 *)dramStack;
    renderTaskAlias->schedulerTask.rspTask.t.yield_data_size = 0xC00;
    renderTaskAlias->schedulerTask.rspTask.t.yield_data_ptr = (u64 *)D_80368000;
    renderTaskAlias->schedulerTask.flags = 0x40;
    renderTaskAlias->schedulerTask.rspTask.t.flags = 0;
    renderTaskAlias->schedulerTask.rspTask.t.ucode_boot = (u64 *)rspbootTextStart;
    renderTaskAlias->schedulerTask.rspTask.t.ucode_boot_size = rspTextSize;
    renderTaskAlias->schedulerTask.rspTask.t.output_buff = (u64 *)outputBuff;
    renderTaskAlias->schedulerTask.next = NULL;
    renderTaskAlias->schedulerTask.doneQueue = &gFramebufferRenderDoneQueue;
    renderTaskAlias->schedulerTask.doneMsg = &renderTaskAlias->completionMsgType;
    renderTaskAlias->schedulerTask.framebuffer = renderTaskAlias->colorFramebuffer;
    renderTaskAlias->schedulerTask.retrace = 0xFFF & (gLastSchedulerRetraceCounter + 3);
    renderTaskAlias->status |= nextColorIndex;
    schedulerState = &gSchedulerState;
    osSendMesg(getSchedulerGraphicsTaskQueue((s32)schedulerState), renderTaskAlias, nextColorIndex);

    frameIndex = (bufferIndex + 1) & nextColorIndex;
    nextColorIndex = gFramebufferColorBufferIndex + 1;
    if (nextColorIndex >= FRAMEBUFFER_COUNT) {
        nextColorIndex = 0;
    }

    prepTask = &D_80155548[frameIndex];
    prepDisplayList = prepTask->displayList;
    gRegionAllocPtr = prepDisplayList;
    BOOT_GFX_CMD(0xBC000006, 0);
    BOOT_GFX_CMD(0xED000000, 0x5003C0);
    BOOT_GFX_CMD(0xB6000000, 0x33205);
    BOOT_GFX_CMD(0xBC000404, 1);
    BOOT_GFX_CMD(0xBC000C04, prepOne = 1);
    BOOT_GFX_CMD(0xBC001404, 0xFFFF);
    BOOT_GFX_CMD(0xBC001C04, 0xFFFF);
    BOOT_GFX_CMD(0xBA001701, 0);
    BOOT_GFX_CMD(0xFD10013F, one);
    BOOT_GFX_CMD(0xBA001402, 0x300000);
    BOOT_GFX_CMD(0xB900031D, 0);
    BOOT_GFX_CMD(0xFF10013F, D_80369000);
    BOOT_GFX_CMD(0xF7000000, 0xFFFCFFFC);
    BOOT_GFX_CMD(0xF64FC3BC, 0);
    BOOT_GFX_CMD(0xFE000000, D_80369000);
    BOOT_GFX_CMD(0xE7000000, 0);
    nextFramebuffer = D_8038E800 + nextColorIndex * FRAMEBUFFER_SIZE;
    BOOT_GFX_CMD(0xFF10013F, nextFramebuffer);
    BOOT_GFX_CMD(0xF7000000, 0x10001);
    BOOT_GFX_CMD(0xF64FC3BC, 0);
    BOOT_GFX_CMD(0xE9000000, 0);
    BOOT_GFX_CMD(0xB8000000, 0);

    prepTask->schedulerTask.rspTask.t.data_ptr = (u64 *)prepDisplayList;
    prepTask->schedulerTask.rspTask.t.data_size =
        ((((u8 *)gRegionAllocPtr - (u8 *)prepTask) - 0x60) >> 3) * 8;
    prepTask->schedulerTask.rspTask.t.type = 1;
    prepTask->schedulerTask.rspTask.t.ucode_boot = (u64 *)rspbootTextStart;
    prepTask->schedulerTask.rspTask.t.ucode_boot_size = rspTextSize;
    prepTask->schedulerTask.rspTask.t.ucode_data = (u64 *)D_800E21C0;
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
    prepTask->schedulerTask.framebuffer = nextFramebuffer;
    osSendMesg(getSchedulerGraphicsTaskQueue((s32)schedulerState), prepTask, 1);
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/engine/system_runtime/submitFramebufferRenderTask.s")
#endif
