#include "common.h"
#include "game/race/camera/race_camera.h"
#include "game/audio/sound_manager.h"
#include "game/engine/system_runtime.h"
#include "game/engine/system_boot.h"
#include "game/engine/frame_render_task.h"
#include "game/engine/game_task_scheduler.h"
#include "game/engine/render_callback.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/engine/relocatable_heap.h"
#include "game/race/flow/race_flow.h"
#include "game/race/race_state.h"
#include "game/engine/viewport_manager.h"
#include "game/math/fixed_point_math.h"
#include "PR/os_cache.h"
#include "PR/os_thread.h"
#include "PR/sptask.h"
#include "PR/ucode.h"

#define BOOT_THREAD_ID 1
#define MAIN_THREAD_ID 2
#define THREAD_PRIORITY 10
#define BOOT_THREAD_STACK_SIZE 0x2000
#define GAME_THREAD_STACK_SIZE 0x4000
#define PI_MANAGER_PRIORITY 150
#define PI_MANAGER_MSG_COUNT 200
#define DMA_CHUNK_SIZE 0x2000
#define RETRACE_COUNT_NTSC 2
#define RETRACE_COUNT_PAL 30
#define RETRACE_COUNT_MODE 1
#define STARTUP_CALLBACK_PRIORITY 100
#define BOOT_FADE_TIMER 0x3E4
#define TITLE_FADE_TIMER 0x3B6
#define FRAMEBUFFER_TASK_BUSY 1
#define SCHEDULER_SWAPBUFFER_FLAG 0x40
#define SCHEDULER_RETRACE_MASK 0xFFF
#define FRAMEBUFFER_SWAP_RETRACE_DELAY 3
#define OS_MESG_NOBLOCK 0
#define OS_MESG_BLOCK 1
#define OS_READ 0
#define OS_TV_NTSC 1

#include "game/audio/audio_engine.h"

typedef struct {
    RenderCallbackNode queue0;
    RenderCallbackNode queue1;
    RenderCallbackNode queue2;
    RenderCallbackNode queue3;
} RenderCallbackQueueGroup;

u8 gMenuFadeOverlayActive = 0;

s16 gMenuFadeAlpha = 0;

Vp D_800DEF18[] = {
    { { { 640, 480, 511, 0 }, { 640, 480, 511, 0 } } },
};

Gfx D_800DEF28[] = {
    gsDPPipeSync(),
    gsDPSetColorDither(G_CD_BAYER),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPSetDepthSource(G_ZS_PIXEL),
    gsSPEndDisplayList(),
};

Gfx D_800DEF90[] = {
    gsDPPipeSync(),
    gsDPSetColorDither(G_CD_BAYER),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPSetGeometryMode(G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPSetDepthSource(G_ZS_PIXEL),
    gsSPEndDisplayList(),
};

Gfx gMenuRenderModeResetDl[] = {
    gsDPPipeSync(),
    gsDPSetColorDither(G_CD_DISABLE),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPSetPrimColor(0, 0, 0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombineMode(G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM),
    gsDPSetRenderMode(G_RM_AA_TEX_EDGE, G_RM_AA_TEX_EDGE2),
    gsDPSetDepthSource(G_ZS_PIXEL),
    gsSPEndDisplayList(),
};

u32 D_800DF078[] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
};

Gfx D_800DF098[] = {
    gsDPPipeSync(),
    gsDPSetColorDither(G_CD_DISABLE),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetCombineMode(G_CC_MODULATEI_PRIM, G_CC_MODULATEI_PRIM),
    gsDPSetRenderMode(G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2),
    gsDPLoadTextureTile_4b(
        D_800DF078,
        G_IM_FMT_I,
        16,
        0,
        0,
        0,
        16,
        4,
        0,
        G_TX_NOMIRROR | G_TX_WRAP,
        G_TX_NOMIRROR | G_TX_WRAP,
        4,
        2,
        G_TX_NOLOD,
        G_TX_NOLOD
    ),
    gsSPEndDisplayList(),
};

s16 gFadeTimer = BOOT_FADE_TIMER;

s32 gClearFramebufferOnNextTask = 1;

extern void osInitialize(void);
extern void osCreatePiManager(OSPri, OSMesgQueue *, OSMesg *, s32);
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern s32 osPiStartDma(OSIoMesg *, s32, s32, u32, void *, u32, OSMesgQueue *);
extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
extern void osViBlack(u8);
extern void osViSetSpecialFeatures(u32);
extern void initMenuAsciiFontTexture(void);

extern s32 osTvType;

extern u8 gFadeColorRed;
extern u8 gFadeColorGreen;
extern u8 gFadeColorBlue;
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
extern u8 gPendingFramebufferSwapCount;
extern u8 gRumblePakConnectedMask;
extern u8 gFramebufferColorBufferIndex;
extern u8 D_800B1CC0[];
extern u8 D_369000[];
extern u8 D_80360000[];
extern u8 D_80368000[];
extern u8 D_80368C00[];
extern u8 gBootThreadStack[BOOT_THREAD_STACK_SIZE];
extern u8 gGameThreadStack[GAME_THREAD_STACK_SIZE];
extern void initMenuAssetHandles(void);
extern void allocRenderCallbackScratchBuffer(void);
extern void allocMenuRenderScratchBuffers(void);
extern void selectMenuRenderScratchBuffer(s32);
extern s32 osSendMesg(void *, void *, s32);
extern void initFramebufferRenderTaskState(void);

void main(void *arg) {
    osInitialize();
    osCreateThread(
        &gBootThread,
        BOOT_THREAD_ID,
        bootThreadMain,
        arg,
        gBootThreadStack + sizeof(gBootThreadStack),
        THREAD_PRIORITY
    );
    osStartThread(&gBootThread);
}

void bootThreadMain(void *arg) {
    osCreatePiManager(PI_MANAGER_PRIORITY, &gPiManagerQueue, gPiManagerMessages, PI_MANAGER_MSG_COUNT);
    osCreateThread(
        &gGameThread,
        MAIN_THREAD_ID,
        gameThreadMain,
        arg,
        gGameThreadStack + sizeof(gGameThreadStack),
        THREAD_PRIORITY
    );
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
                gFrameRenderTasks[0].status = gFrameRenderTaskStatuses[0].status & 0xFFFE;
                break;
            case 6:
                gPendingFramebufferSwapCount++;
                gFrameRenderTasks[1].status = gFrameRenderTaskStatuses[1].status & 0xFFFE;
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

extern s16 gUiBlinkTimer;
extern s16 gMenuViewportWidth;
extern s16 gMenuViewportHeight;
extern s16 gMenuViewportCenterX;
extern s16 gMenuViewportCenterY;
extern u8 gCurrentViewportIndex;
extern RaceCamera D_801121E0[RACE_CAMERA_COUNT];

#define runtimeModelRenderCallbackLists (*(RenderCallbackNode * (*)[24]) & gModelRenderCallbackList)
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
    gDPSetScissor(
        gRegionAllocPtr++, G_SC_NON_INTERLACE, 0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT
    );
    gSPViewport(gRegionAllocPtr++, D_800DEF18);

    if (gMenuOverlayRenderCallbackList != NULL) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
        runRenderCallbacks(&gMenuOverlayRenderCallbackList);
    }

    appendFadeOverlayDisplayList();
    gRenderMatricesDirty = 1;

    upperMask = 0xFFFF0000;
    for (gCurrentViewportIndex = 0; gCurrentViewportIndex < VIEWPORT_COUNT; gCurrentViewportIndex++) {
        if (gViewportStates[gCurrentViewportIndex].screenBoundsValid != 0) {
            gCurrentFrameRenderData->viewports[gCurrentViewportIndex] =
                gViewportStates[gCurrentViewportIndex].viewport;
            gCurrentFrameRenderData->projections[gCurrentViewportIndex] =
                gViewportStates[gCurrentViewportIndex].projectionMatrix;
            gCurrentFrameRenderData->overlayProjections[gCurrentViewportIndex] =
                gViewportStates[gCurrentViewportIndex].overlayProjectionMatrix;

            left = gViewportStates[gCurrentViewportIndex].left;
            top = gViewportStates[gCurrentViewportIndex].top;
            gMenuViewportWidth = gViewportStates[gCurrentViewportIndex].right - left;
            gMenuViewportHeight = gViewportStates[gCurrentViewportIndex].bottom - top;
            gMenuViewportCenterX = left + (gMenuViewportWidth / 2);
            gMenuViewportCenterY = top + (gMenuViewportHeight / 2);

            gCurrentFrameRenderData->viewportMatrices[gCurrentViewportIndex] =
                D_801121E0[gCurrentViewportIndex].packedTransform;
            gCurrentFrameRenderData->viewportMatrices[gCurrentViewportIndex].m[1][2] = 0;
            gCurrentFrameRenderData->viewportMatrices[gCurrentViewportIndex].m[1][3] = 1;
            gCurrentFrameRenderData->viewportMatrices[gCurrentViewportIndex].m[3][2] = 0;
            gCurrentFrameRenderData->viewportMatrices[gCurrentViewportIndex].m[3][3] = 0;
            gViewportMatrix = &gCurrentFrameRenderData->viewportMatrices[gCurrentViewportIndex];

            gDPPipeSync(gRegionAllocPtr++);
            gDPSetScissor(
                gRegionAllocPtr++,
                G_SC_NON_INTERLACE,
                gViewportStates[gCurrentViewportIndex].left,
                gViewportStates[gCurrentViewportIndex].top,
                gViewportStates[gCurrentViewportIndex].right,
                gViewportStates[gCurrentViewportIndex].bottom
            );
            hasModelCallbacks = 0;

            if (gViewportStates[gCurrentViewportIndex].clearFramebuffer != 0) {
                gDPPipeSync(gRegionAllocPtr++);
                gDPSetCycleType(gRegionAllocPtr++, G_CYC_FILL);
                gDPSetRenderMode(gRegionAllocPtr++, G_RM_NOOP, G_RM_NOOP2);
                gDPSetColorImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, gDepthBuffer);
                gDPSetFillColor(gRegionAllocPtr++, 0xFFFCFFFC);
                gDPFillRectangle(
                    gRegionAllocPtr++,
                    gViewportStates[gCurrentViewportIndex].left,
                    gViewportStates[gCurrentViewportIndex].top,
                    gViewportStates[gCurrentViewportIndex].right - 1,
                    gViewportStates[gCurrentViewportIndex].bottom - 1
                );
                gDPPipeSync(gRegionAllocPtr++);
                gDPSetColorImage(
                    gRegionAllocPtr++,
                    G_IM_FMT_RGBA,
                    G_IM_SIZ_16b,
                    320,
                    gFrameRenderTasks[frameIndex].framebuffer
                );
            }

            if (gViewportStates[gCurrentViewportIndex].overlayActive != 0) {
                gDPPipeSync(gRegionAllocPtr++);
                gDPSetCycleType(gRegionAllocPtr++, G_CYC_FILL);
                gDPSetRenderMode(gRegionAllocPtr++, G_RM_NOOP, G_RM_NOOP2);
                gDPSetColorImage(
                    gRegionAllocPtr++,
                    G_IM_FMT_RGBA,
                    G_IM_SIZ_16b,
                    320,
                    gFrameRenderTasks[frameIndex].framebuffer
                );
                gDPSetFillColor(
                    gRegionAllocPtr++,
                    (GPACK_RGBA5551(
                         (u8)gViewportStates[gCurrentViewportIndex].overlayR,
                         (u8)gViewportStates[gCurrentViewportIndex].overlayG,
                         (u8)gViewportStates[gCurrentViewportIndex].overlayB,
                         1
                     )
                     << 16) |
                        GPACK_RGBA5551(
                            (u8)gViewportStates[gCurrentViewportIndex].overlayR,
                            (u8)gViewportStates[gCurrentViewportIndex].overlayG,
                            (u8)gViewportStates[gCurrentViewportIndex].overlayB,
                            1
                        )
                );
                gDPFillRectangle(
                    gRegionAllocPtr++,
                    gViewportStates[gCurrentViewportIndex].left,
                    gViewportStates[gCurrentViewportIndex].top,
                    gViewportStates[gCurrentViewportIndex].right - 1,
                    gViewportStates[gCurrentViewportIndex].bottom - 1
                );
            }

            if (D_80124848 != NULL) {
                gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
                runRenderCallbacks(&D_80124848);
            }

            gCurrentFrameRenderData->rotations[gCurrentViewportIndex].m[0][0] =
                ((D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[0] << 4) & upperMask) |
                ((D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[1] >> 12) & 0xFFFF);
            gCurrentFrameRenderData->rotations[gCurrentViewportIndex].m[0][1] =
                (D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[2] << 4) & upperMask;
            gCurrentFrameRenderData->rotations[gCurrentViewportIndex].m[0][2] =
                ((D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[3] << 4) & upperMask) |
                ((D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[4] >> 12) & 0xFFFF);
            gCurrentFrameRenderData->rotations[gCurrentViewportIndex].m[0][3] =
                (D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[5] << 4) & upperMask;
            gCurrentFrameRenderData->rotations[gCurrentViewportIndex].m[1][0] =
                ((D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[6] << 4) & upperMask) |
                ((D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[7] >> 12) & 0xFFFF);
            gCurrentFrameRenderData->rotations[gCurrentViewportIndex].m[1][1] =
                (D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[8] << 4) & upperMask;
            gCurrentFrameRenderData->translations[gCurrentViewportIndex].m[1][2] =
                (D_801121E0[gCurrentViewportIndex].cameraTransform.translation.x & upperMask) |
                ((D_801121E0[gCurrentViewportIndex].cameraTransform.translation.y >> 16) & 0xFFFF);
            gCurrentFrameRenderData->translations[gCurrentViewportIndex].m[1][3] =
                (D_801121E0[gCurrentViewportIndex].cameraTransform.translation.z & upperMask) | 1;

            gCurrentFrameRenderData->rotations[gCurrentViewportIndex].m[2][0] =
                ((D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[0] << 20) & upperMask) |
                ((D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[1] << 4) & 0xFFFF);
            gCurrentFrameRenderData->rotations[gCurrentViewportIndex].m[2][1] =
                (D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[2] << 20) & upperMask;
            gCurrentFrameRenderData->rotations[gCurrentViewportIndex].m[2][2] =
                ((D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[3] << 20) & upperMask) |
                ((D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[4] << 4) & 0xFFFF);
            gCurrentFrameRenderData->rotations[gCurrentViewportIndex].m[2][3] =
                (D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[5] << 20) & upperMask;
            gCurrentFrameRenderData->rotations[gCurrentViewportIndex].m[3][0] =
                ((D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[6] << 20) & upperMask) |
                ((D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[7] << 4) & 0xFFFF);
            gCurrentFrameRenderData->rotations[gCurrentViewportIndex].m[3][1] =
                (D_801121E0[gCurrentViewportIndex].cameraTransform.rotation[8] << 20) & upperMask;
            gCurrentFrameRenderData->translations[gCurrentViewportIndex].m[3][2] =
                ((D_801121E0[gCurrentViewportIndex].cameraTransform.translation.x << 16) & upperMask) |
                (D_801121E0[gCurrentViewportIndex].cameraTransform.translation.y & 0xFFFF);
            gCurrentFrameRenderData->translations[gCurrentViewportIndex].m[3][3] =
                (D_801121E0[gCurrentViewportIndex].cameraTransform.translation.z << 16) & upperMask;

            if (gBackdropRenderCallbackList != NULL) {
                gSPPerspNormalize(
                    gRegionAllocPtr++,
                    gViewportStates[gCurrentViewportIndex].overlayPerspectiveNorm
                );
                gSPMatrix(
                    gRegionAllocPtr++,
                    &gCurrentFrameRenderData->overlayProjections[gCurrentViewportIndex],
                    G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION
                );
                gSPViewport(gRegionAllocPtr++, &gCurrentFrameRenderData->viewports[gCurrentViewportIndex]);
                gSPMatrix(
                    gRegionAllocPtr++,
                    &gCurrentFrameRenderData->rotations[gCurrentViewportIndex],
                    G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION
                );
                gSPMatrix(
                    gRegionAllocPtr++,
                    &gCurrentFrameRenderData->translations[gCurrentViewportIndex],
                    G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION
                );
                gSPDisplayList(gRegionAllocPtr++, D_800DEF90);
                runRenderCallbacks(&gBackdropRenderCallbackList);
            }

            for (i = 0; i < 24; i += 3) {
                if (runtimeModelRenderCallbackLists[i] != NULL) {
                    hasModelCallbacks = 1;
                }
            }

            if (hasModelCallbacks != 0) {
                gSPPerspNormalize(gRegionAllocPtr++, gViewportStates[gCurrentViewportIndex].perspectiveNorm);
                gSPMatrix(
                    gRegionAllocPtr++,
                    &gCurrentFrameRenderData->projections[gCurrentViewportIndex],
                    G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION
                );
                gSPViewport(gRegionAllocPtr++, &gCurrentFrameRenderData->viewports[gCurrentViewportIndex]);
                gSPMatrix(
                    gRegionAllocPtr++,
                    &gCurrentFrameRenderData->rotations[gCurrentViewportIndex],
                    G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION
                );
                gSPMatrix(
                    gRegionAllocPtr++,
                    &gCurrentFrameRenderData->translations[gCurrentViewportIndex],
                    G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION
                );
                gSPDisplayList(gRegionAllocPtr++, D_800DEF28);

                for (i = 0; i < 24; i += 3) {
                    if (runtimeModelRenderCallbackLists[i] != NULL) {
                        queue = &runtimeModelRenderCallbackLists[i];
                        if (queue == &gEffectRenderCallbackList) {
                            gSPMatrix(
                                gRegionAllocPtr++,
                                &gCurrentFrameRenderData->viewportMatrices[gCurrentViewportIndex],
                                G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW
                            );
                        }
                        runRenderCallbacks(queue);
                    }
                }
            }

            if ((gRaceForegroundRenderCallbackList != NULL) || (gRaceOverlayRenderCallbackList != NULL)) {
                gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
                if (gRaceOverlayRenderCallbackList != NULL) {
                    runRenderCallbacks(&gRaceOverlayRenderCallbackList);
                }
                if (gRaceForegroundRenderCallbackList != NULL) {
                    initMenuAsciiFontTexture();
                    runRenderCallbacks(&gRaceForegroundRenderCallbackList);
                }
            }

            if (gViewportStates[gCurrentViewportIndex].overlayAlpha != 0) {
                gSPDisplayList(gRegionAllocPtr++, D_800DF098);
                gDPSetPrimColor(
                    gRegionAllocPtr++,
                    0,
                    0,
                    0,
                    0,
                    0,
                    gViewportStates[gCurrentViewportIndex].overlayAlpha
                );
                gSPTextureRectangle(
                    gRegionAllocPtr++,
                    (gMenuViewportCenterX - (gMenuViewportWidth / 2)) << 2,
                    (gMenuViewportCenterY - (gMenuViewportHeight / 2)) << 2,
                    (gMenuViewportCenterX + (gMenuViewportWidth / 2)) << 2,
                    (gMenuViewportCenterY + (gMenuViewportHeight / 2)) << 2,
                    G_TX_RENDERTILE,
                    0,
                    0,
                    1 << 10,
                    1 << 10
                );
            }

            gRenderMatricesDirty = 0;
        }
    }

    gMenuViewportWidth = 288;
    gMenuViewportHeight = 208;
    gMenuViewportCenterX = 160;
    gMenuViewportCenterY = 120;

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
            G_TX_RENDERTILE,
            0,
            0,
            1 << 10,
            1 << 10
        );
    }
}

#undef runtimeModelRenderCallbackLists
#undef VIEWPORT_COUNT

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void resetRenderCallbackQueues(void) {
    u32 end;
    RenderCallbackQueueGroup *group;

    gMenuForegroundRenderCallbackList = NULL;
    gRaceForegroundRenderCallbackList = NULL;
    do { end = (u32)&gBackdropRenderCallbackList; group = (RenderCallbackQueueGroup *)&gModelRenderCallbackList; loop: group++; group[-1].queue0.next = NULL; group[-1].queue1.next = NULL; } while (0);
    group[-1].queue2.next = NULL;
    group[-1].queue3.next = NULL;
    if ((u32)group != end) {
        goto loop;
    }
    gBackdropRenderCallbackList = NULL;
    gMenuOverlayRenderCallbackList = NULL;
    D_80124848 = NULL;
    gRaceOverlayRenderCallbackList = NULL;
    gMenuRenderCallbackList = NULL;
}
// clang-format on

void initFramebufferRenderTaskState(void) {
    gFrameRenderTasks[0].completionMessage = 5;
    gFrameRenderTasks[0].framebuffer = gFramebuffers[0];
    gFrameRenderTasks[1].completionMessage = 6;
    if (1) {
        gFrameRenderTasks[1].framebuffer = gFramebuffers[1];
    }
    osViSetSpecialFeatures(0x6A);
    gFrameRenderTaskStatuses[0].status = 0;
    gFrameRenderTaskStatuses[1].status = 0;
    gFramebufferColorBufferIndex = 0;
    gFrameRenderTasks[0].viewportData.rotations[0] = gIdentityMatrix;
    gFrameRenderTasks[0].viewportData.rotations[1] = gIdentityMatrix;
    gFrameRenderTasks[0].viewportData.rotations[2] = gIdentityMatrix;
    gFrameRenderTasks[0].viewportData.rotations[3] = gIdentityMatrix;
    gFrameRenderTasks[0].viewportData.translations[0] = gIdentityMatrix;
    gFrameRenderTasks[0].viewportData.translations[1] = gIdentityMatrix;
    gFrameRenderTasks[0].viewportData.translations[2] = gIdentityMatrix;
    gFrameRenderTasks[0].viewportData.translations[3] = gIdentityMatrix;
    gFrameRenderTasks[1].viewportData.rotations[0] = gIdentityMatrix;
    gFrameRenderTasks[1].viewportData.rotations[1] = gIdentityMatrix;
    gFrameRenderTasks[1].viewportData.rotations[2] = gIdentityMatrix;
    gFrameRenderTasks[1].viewportData.rotations[3] = gIdentityMatrix;
    gFrameRenderTasks[1].viewportData.translations[0] = gIdentityMatrix;
    gFrameRenderTasks[1].viewportData.translations[1] = gIdentityMatrix;
    gFrameRenderTasks[1].viewportData.translations[2] = gIdentityMatrix;
    gFrameRenderTasks[1].viewportData.translations[3] = gIdentityMatrix;
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

void submitFramebufferRenderTask(u8 frameIndex) {
    SchedulerTask *schedulerTask;
    s32 nextFramebufferIndex;

    gFramebufferColorBufferIndex++;
    if (gFramebufferColorBufferIndex >= FRAMEBUFFER_COUNT) {
        gFramebufferColorBufferIndex = 0;
    }

    gFrameRenderTasks[frameIndex].framebuffer = gFramebuffers[gFramebufferColorBufferIndex];
    selectMenuRenderScratchBuffer(frameIndex);


    gRegionAllocPtr = gFrameRenderTasks[frameIndex].displayList;
    gCurrentFrameRenderData = &gFrameRenderTasks[frameIndex].viewportData;
    schedulerTask = &gFrameRenderTasks[frameIndex].schedulerTask;

    gSPSegment(gRegionAllocPtr++, 0, 0);
    gDPSetScissor(
        gRegionAllocPtr++, G_SC_NON_INTERLACE, 0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT
    );
    gSPClearGeometryMode(gRegionAllocPtr++,
                         G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_SHADING_SMOOTH);
    gSPClipRatio(gRegionAllocPtr++, FRUSTRATIO_1);
    gDPPipelineMode(gRegionAllocPtr++, G_PM_NPRIMITIVE);
    gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, FRAMEBUFFER_WIDTH, D_369000);

    if (gClearFramebufferOnNextTask != 0) {
        gClearFramebufferOnNextTask = 0;
        gDPSetCycleType(gRegionAllocPtr++, G_CYC_FILL);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_NOOP, G_RM_NOOP2);
        gDPSetColorImage(
            gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, FRAMEBUFFER_WIDTH, gDepthBuffer
        );
        gDPSetFillColor(gRegionAllocPtr++, 0xFFFCFFFC);
        gDPFillRectangle(
            gRegionAllocPtr++, 0, 0, FRAMEBUFFER_WIDTH - 1, FRAMEBUFFER_HEIGHT - 1
        );
        gDPSetDepthImage(gRegionAllocPtr++, gDepthBuffer);
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetColorImage(
            gRegionAllocPtr++,
            G_IM_FMT_RGBA,
            G_IM_SIZ_16b,
            FRAMEBUFFER_WIDTH,
            gFrameRenderTasks[frameIndex].framebuffer
        );
        gDPSetFillColor(gRegionAllocPtr++, 0x10001);
        gDPFillRectangle(
            gRegionAllocPtr++, 0, 0, FRAMEBUFFER_WIDTH - 1, FRAMEBUFFER_HEIGHT - 1
        );
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
        gDPSetTileSize(gRegionAllocPtr++, 0, 0, 0, 0, 0);
        gDPSetTileSize(gRegionAllocPtr++, 1, 0, 0, 0, 0);
        gDPSetTileSize(gRegionAllocPtr++, 2, 0, 0, 0, 0);
        gDPSetTileSize(gRegionAllocPtr++, 3, 0, 0, 0, 0);
        gDPSetTileSize(gRegionAllocPtr++, 4, 0, 0, 0, 0);
        gDPSetTileSize(gRegionAllocPtr++, 5, 0, 0, 0, 0);
        gDPSetTileSize(gRegionAllocPtr++, 6, 0, 0, 0, 0);
        gDPSetTileSize(gRegionAllocPtr++, 7, 0, 0, 0, 0);
        gDPSetTile(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0, 0,
                   0, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPSetTile(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0, 1,
                   0, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPSetTile(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0, 2,
                   0, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPSetTile(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0, 3,
                   0, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPSetTile(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0, 4,
                   0, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPSetTile(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0, 5,
                   0, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPSetTile(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0, 6,
                   0, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPSetTile(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0, 7,
                   0, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    } else {
        gDPSetDepthImage(gRegionAllocPtr++, gDepthBuffer);
        gDPSetColorImage(
            gRegionAllocPtr++,
            G_IM_FMT_RGBA,
            G_IM_SIZ_16b,
            FRAMEBUFFER_WIDTH,
            gFrameRenderTasks[frameIndex].framebuffer
        );
    }

    gDPSetAlphaCompare(gRegionAllocPtr++, G_AC_NONE);
    appendViewportDisplayLists(frameIndex);
    gDPFullSync(gRegionAllocPtr++);
    gSPEndDisplayList(gRegionAllocPtr++);

    schedulerTask->rspTask.t.data_ptr =
        (u64 *)(gCurrentFrameRenderData + 1);
    schedulerTask->rspTask.t.data_size =
        (gRegionAllocPtr - (Gfx *)(gCurrentFrameRenderData + 1)) * sizeof(Gfx);
    schedulerTask->rspTask.t.type = M_GFXTASK;
    schedulerTask->rspTask.t.flags = 0;
    schedulerTask->rspTask.t.ucode_boot = (u64 *)rspbootTextStart;
    schedulerTask->rspTask.t.ucode_boot_size = (unsigned long)aspMainTextStart - (unsigned long)rspbootTextStart;
    schedulerTask->rspTask.t.ucode = (u64 *)D_800B1CC0;
    schedulerTask->rspTask.t.ucode_data = (u64 *)gspF3DLX_fifoDataStart;
    schedulerTask->rspTask.t.ucode_data_size = RSP_UCODE_DATA_SIZE;
    schedulerTask->rspTask.t.dram_stack = (u64 *)D_80368C00;
    schedulerTask->rspTask.t.dram_stack_size = RSP_DRAM_STACK_SIZE;
    schedulerTask->rspTask.t.output_buff = (u64 *)D_80360000;
    schedulerTask->rspTask.t.output_buff_size =
        (u64 *)((unsigned long)D_80360000 + (long long)RSP_OUTPUT_BUFFER_SIZE);
    schedulerTask->rspTask.t.yield_data_ptr = (u64 *)D_80368000;
    schedulerTask->rspTask.t.yield_data_size = RSP_YIELD_BUFFER_SIZE;
    schedulerTask->next = NULL;
    schedulerTask->flags = SCHEDULER_SWAPBUFFER_FLAG;
    schedulerTask->doneQueue = &gFramebufferRenderDoneQueue;
    schedulerTask->doneMsg = &gFrameRenderTasks[frameIndex].completionMessage;
    schedulerTask->framebuffer = gFrameRenderTasks[frameIndex].framebuffer;
    schedulerTask->retrace =
        SCHEDULER_RETRACE_MASK & (gLastSchedulerRetraceCounter + FRAMEBUFFER_SWAP_RETRACE_DELAY);
    gFrameRenderTasks[frameIndex].status |= FRAMEBUFFER_TASK_BUSY;
    osSendMesg(getSchedulerGraphicsTaskQueue(&gSchedulerState), schedulerTask, OS_MESG_BLOCK);

    frameIndex = (frameIndex + 1) & (FRAME_RENDER_TASK_COUNT - 1);
    nextFramebufferIndex = gFramebufferColorBufferIndex;
    nextFramebufferIndex++;
    if (nextFramebufferIndex >= FRAMEBUFFER_COUNT) {
        nextFramebufferIndex = 0;
    }

    schedulerTask = &gFramebufferPrepareTasks[frameIndex].schedulerTask;
    gRegionAllocPtr = gFramebufferPrepareTasks[frameIndex].displayList;
    gSPSegment(gRegionAllocPtr++, 0, 0);
    gDPSetScissor(
        gRegionAllocPtr++, G_SC_NON_INTERLACE, 0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT
    );
    gSPClearGeometryMode(gRegionAllocPtr++,
                         G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_SHADING_SMOOTH);
    gSPClipRatio(gRegionAllocPtr++, FRUSTRATIO_1);
    gDPPipelineMode(gRegionAllocPtr++, G_PM_NPRIMITIVE);
    gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, FRAMEBUFFER_WIDTH, D_369000);
    gDPSetCycleType(gRegionAllocPtr++, G_CYC_FILL);
    gDPSetRenderMode(gRegionAllocPtr++, G_RM_NOOP, G_RM_NOOP2);
    gDPSetColorImage(
        gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, FRAMEBUFFER_WIDTH, gDepthBuffer
    );
    gDPSetFillColor(gRegionAllocPtr++, 0xFFFCFFFC);
    gDPFillRectangle(
        gRegionAllocPtr++, 0, 0, FRAMEBUFFER_WIDTH - 1, FRAMEBUFFER_HEIGHT - 1
    );
    gDPSetDepthImage(gRegionAllocPtr++, gDepthBuffer);
    gDPPipeSync(gRegionAllocPtr++);
    gDPSetColorImage(
        gRegionAllocPtr++,
        G_IM_FMT_RGBA,
        G_IM_SIZ_16b,
        FRAMEBUFFER_WIDTH,
        gFramebuffers[nextFramebufferIndex]
    );
    gDPSetFillColor(gRegionAllocPtr++, 0x10001);
    gDPFillRectangle(
        gRegionAllocPtr++, 0, 0, FRAMEBUFFER_WIDTH - 1, FRAMEBUFFER_HEIGHT - 1
    );
    gDPFullSync(gRegionAllocPtr++);
    gSPEndDisplayList(gRegionAllocPtr++);

    schedulerTask->rspTask.t.data_ptr = (u64 *)gFramebufferPrepareTasks[frameIndex].displayList;
    schedulerTask->rspTask.t.data_size = (gRegionAllocPtr - gFramebufferPrepareTasks[frameIndex].displayList) * sizeof(Gfx);
    schedulerTask->rspTask.t.type = M_GFXTASK;
    schedulerTask->rspTask.t.ucode_boot = (u64 *)rspbootTextStart;
    schedulerTask->rspTask.t.ucode_boot_size = (unsigned long)aspMainTextStart - (unsigned long)rspbootTextStart;
    schedulerTask->rspTask.t.ucode = (u64 *)D_800B1CC0;
    schedulerTask->rspTask.t.ucode_data = (u64 *)gspF3DLX_fifoDataStart;
    schedulerTask->rspTask.t.ucode_data_size = RSP_UCODE_DATA_SIZE;
    schedulerTask->rspTask.t.dram_stack = (u64 *)D_80368C00;
    schedulerTask->rspTask.t.dram_stack_size = RSP_DRAM_STACK_SIZE;
    schedulerTask->rspTask.t.output_buff = (u64 *)D_80360000;
    schedulerTask->rspTask.t.output_buff_size =
        (u64 *)((unsigned long)D_80360000 + (long long)RSP_OUTPUT_BUFFER_SIZE);
    schedulerTask->rspTask.t.yield_data_ptr = (u64 *)D_80368000;
    schedulerTask->rspTask.t.yield_data_size = RSP_YIELD_BUFFER_SIZE;
    schedulerTask->next = NULL;
    schedulerTask->flags = 0;
    schedulerTask->doneQueue = &gFramebufferRenderDoneQueue;
    schedulerTask->doneMsg = NULL;
    schedulerTask->framebuffer = gFramebuffers[nextFramebufferIndex];

    osSendMesg(getSchedulerGraphicsTaskQueue(&gSchedulerState), schedulerTask, OS_MESG_BLOCK);
}
