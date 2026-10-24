#include "common.h"
#include "game/engine/runtime_memory.h"
#include "game/race/camera/race_camera.h"
#include "game/audio/sound_manager.h"
#include "game/engine/system_runtime.h"
#include "game/engine/system_boot.h"
#include "game/engine/frame_render_task.h"
#include "game/engine/game_task_scheduler.h"
#include "game/engine/render_callback.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/engine/relocatable_heap.h"
#include "game/race/flow/race_flow.h"
#include "game/race/race_state.h"
#include "game/engine/viewport_manager.h"
#include "game/math/fixed_point_math.h"
#include "PR/os_cache.h"
#include "PR/os_thread.h"
#include "PR/sptask.h"
#include "PR/ucode.h"
#include "game/audio/audio_engine.h"
#include "game/menu/race_setup/race_setup_menu.h"

#define OS_MESG_NOBLOCK 0
#define MAIN_THREAD_ID 2
#define PI_MANAGER_PRIORITY 150
#define PI_MANAGER_MSG_COUNT 200
#define BOOT_THREAD_ID 1
#define THREAD_PRIORITY 10
#define DMA_CHUNK_SIZE 0x2000
#define SCHEDULER_VI_MODE_INDEX_NTSC 2
#define SCHEDULER_RETRACE_INTERVAL 1
#define STARTUP_CALLBACK_PRIORITY 100
#define OS_MESG_BLOCK 1
#define OS_READ 0
#define BOOT_FADE_TIMER 0x3E4
#define TITLE_FADE_TIMER 0x3B6

typedef struct {
    RenderCallbackQueueSlot queue0;
    RenderCallbackQueueSlot queue1;
    RenderCallbackQueueSlot queue2;
    RenderCallbackQueueSlot queue3;
} RenderCallbackQueueGroup;

extern OSMesg D_80123330[1];
extern OSMesg D_80123350[8];
extern OSMesg D_80123388[2];
extern OSMesg D_801233A8[8];
extern SchedulerClient D_80123B40;

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
    osCreateMesgQueue(&gRomDmaQueue, D_80123330, 1);
    osCreateMesgQueue(&gSchedulerClientQueue, D_80123388, 2);
    osCreateMesgQueue(&gControllerInputUpdateQueue, D_801233A8, 8);
    osCreateMesgQueue(&gFramebufferRenderDoneQueue, D_80123350, 8);
    initScheduler(&gSchedulerState, SCHEDULER_VI_MODE_INDEX_NTSC, SCHEDULER_RETRACE_INTERVAL);
    addSchedulerClient(&gSchedulerState, &D_80123B40, &gSchedulerClientQueue);
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
                gFrameRenderTasks[0].status = gFrameRenderTaskStatuses[0][0] & 0xFFFE;
                break;
            case 6:
                gPendingFramebufferSwapCount++;
                gFrameRenderTasks[1].status = gFrameRenderTaskStatuses[1][0] & 0xFFFE;
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
    for (;;) {
    loop_17:
        if ((osRecvMesg(&gFramebufferRenderDoneQueue, &msg, OS_MESG_NOBLOCK) != 0) &&
            (osRecvMesg(&gControllerInputUpdateQueue, &msg, OS_MESG_NOBLOCK) != 0) &&
            (osRecvMesg(&gSchedulerClientQueue, &msg, OS_MESG_NOBLOCK) != 0)) {
            goto loop_17;
        }

        if (!(*(s16 *)msg != finalType)) {
            break;
        }
    }

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

#pragma GLOBAL_ASM("asm.jp/nonmatchings/engine/system_runtime/func_80099610.s")

void resetRenderCallbackQueues(void) {
    u32 end;
    RenderCallbackQueueGroup *group;

    gMenuForegroundRenderCallbackList = NULL;
    gRaceForegroundRenderCallbackList = NULL;
    // clang-format off
    do { \
        end = (u32)&gBackdropRenderCallbackList; \
        group = (RenderCallbackQueueGroup *)gModelRenderCallbackQueues; \
    loop: \
        group++; \
        group[-1].queue0.head = NULL; \
        group[-1].queue1.head = NULL; \
    } while (0);
    // clang-format on
    group[-1].queue2.head = NULL;
    group[-1].queue3.head = NULL;
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
    gFrameRenderTasks[0].completionMessage = 5;
    gFrameRenderTasks[0].framebuffer = gFramebuffers[0];
    gFrameRenderTasks[1].completionMessage = 6;
    if (1) {
        gFrameRenderTasks[1].framebuffer = gFramebuffers[1];
    }
    osViSetSpecialFeatures(0x6A);
    gFrameRenderTaskStatuses[0][0] = 0;
    gFrameRenderTaskStatuses[1][0] = 0;
    gFramebufferColorBufferIndex = 0;
    gFrameRenderTasks[0].renderData.viewport.rotations[0] = gIdentityMatrix;
    gFrameRenderTasks[0].renderData.viewport.rotations[1] = gIdentityMatrix;
    gFrameRenderTasks[0].renderData.viewport.rotations[2] = gIdentityMatrix;
    gFrameRenderTasks[0].renderData.viewport.rotations[3] = gIdentityMatrix;
    gFrameRenderTasks[0].renderData.viewport.translations[0] = gIdentityMatrix;
    gFrameRenderTasks[0].renderData.viewport.translations[1] = gIdentityMatrix;
    gFrameRenderTasks[0].renderData.viewport.translations[2] = gIdentityMatrix;
    gFrameRenderTasks[0].renderData.viewport.translations[3] = gIdentityMatrix;
    gFrameRenderTasks[1].renderData.viewport.rotations[0] = gIdentityMatrix;
    gFrameRenderTasks[1].renderData.viewport.rotations[1] = gIdentityMatrix;
    gFrameRenderTasks[1].renderData.viewport.rotations[2] = gIdentityMatrix;
    gFrameRenderTasks[1].renderData.viewport.rotations[3] = gIdentityMatrix;
    gFrameRenderTasks[1].renderData.viewport.translations[0] = gIdentityMatrix;
    gFrameRenderTasks[1].renderData.viewport.translations[1] = gIdentityMatrix;
    gFrameRenderTasks[1].renderData.viewport.translations[2] = gIdentityMatrix;
    gFrameRenderTasks[1].renderData.viewport.translations[3] = gIdentityMatrix;
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

#pragma GLOBAL_ASM("asm.jp/nonmatchings/engine/system_runtime/submitFramebufferRenderTask.s")
