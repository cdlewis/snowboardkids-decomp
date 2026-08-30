#ifndef FRAME_RENDER_TASK_H
#define FRAME_RENDER_TASK_H

#include "common.h"
#include "game/audio/audio_engine.h"
#include "game/engine/system_render.h"

#define FRAME_RENDER_TASK_COUNT 2
#define FRAME_RENDER_TASK_STRIDE 0x18620
#define FRAME_RENDER_DATA_SIZE 0x5B8
#define FRAME_RENDER_DISPLAY_LIST_SIZE 0x18000
#define FRAMEBUFFER_PREPARE_TASK_STRIDE 0x860
#define FRAMEBUFFER_PREPARE_DISPLAY_LIST_SIZE 0x800

typedef struct FrameRenderTask {
    /* 0x00000 */ SchedulerTask schedulerTask;
    /* 0x00060 */ u16 *framebuffer;
    /* 0x00064 */ s16 completionMessage;
    /* 0x00066 */ u8 status;
    /* 0x00067 */ u8 pad67;
    /* 0x00068 */ RuntimeViewportDisplayListData viewportData;
    /* 0x00620 */ Gfx displayList[FRAME_RENDER_DISPLAY_LIST_SIZE / sizeof(Gfx)];
} FrameRenderTask;

typedef struct FramebufferPrepareTask {
    /* 0x000 */ SchedulerTask schedulerTask;
    /* 0x060 */ Gfx displayList[FRAMEBUFFER_PREPARE_DISPLAY_LIST_SIZE / sizeof(Gfx)];
} FramebufferPrepareTask;

/* Compiler anchor for the status byte at offset 0x66 in each render task. */
typedef struct FrameRenderTaskStatusSlot {
    u8 status;
    u8 pad1[FRAME_RENDER_TASK_STRIDE - 1];
} FrameRenderTaskStatusSlot;

typedef char RuntimeViewportDisplayListDataSizeCheck[
    (sizeof(RuntimeViewportDisplayListData) == FRAME_RENDER_DATA_SIZE) ? 1 : -1
];
typedef char FrameRenderTaskSizeCheck[(sizeof(FrameRenderTask) == FRAME_RENDER_TASK_STRIDE) ? 1 : -1];
typedef char FramebufferPrepareTaskSizeCheck[
    (sizeof(FramebufferPrepareTask) == FRAMEBUFFER_PREPARE_TASK_STRIDE) ? 1 : -1
];

extern FrameRenderTask gFrameRenderTasks[FRAME_RENDER_TASK_COUNT];
extern FrameRenderTaskStatusSlot gFrameRenderTaskStatuses[FRAME_RENDER_TASK_COUNT];
extern FramebufferPrepareTask gFramebufferPrepareTasks[FRAME_RENDER_TASK_COUNT];

#endif
