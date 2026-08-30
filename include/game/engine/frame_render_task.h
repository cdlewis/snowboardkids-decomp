#ifndef FRAME_RENDER_TASK_H
#define FRAME_RENDER_TASK_H

#include "common.h"
#include "game/audio/audio_engine.h"
#include "game/engine/system_render.h"

#define FRAME_RENDER_TASK_COUNT 2
#define FRAME_RENDER_DISPLAY_LIST_COUNT 0x3000
#define FRAMEBUFFER_PREPARE_DISPLAY_LIST_COUNT 0x100

typedef struct FrameRenderTask {
    /* 0x000 */ SchedulerTask schedulerTask;
    /* 0x060 */ u16 *framebuffer;
    /* 0x064 */ s16 completionMessage;
    /* 0x066 */ u8 status;
    /* 0x068 */ FrameRenderData renderData;
    /* 0x620 */ Gfx displayList[FRAME_RENDER_DISPLAY_LIST_COUNT];
} FrameRenderTask;

typedef struct FramebufferPrepareTask {
    /* 0x00 */ SchedulerTask schedulerTask;
    /* 0x60 */ Gfx displayList[FRAMEBUFFER_PREPARE_DISPLAY_LIST_COUNT];
} FramebufferPrepareTask;

extern FrameRenderTask gFrameRenderTasks[FRAME_RENDER_TASK_COUNT];
/* Compiler anchor for the status byte at offset 0x66 in each render task. */
extern u8 gFrameRenderTaskStatuses[FRAME_RENDER_TASK_COUNT][sizeof(FrameRenderTask)];
extern FramebufferPrepareTask gFramebufferPrepareTasks[FRAME_RENDER_TASK_COUNT];

#endif
