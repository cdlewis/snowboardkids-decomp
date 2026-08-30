#ifndef SYSTEM_RENDER_H
#define SYSTEM_RENDER_H

#include "common.h"

#define FRAMEBUFFER_COUNT 3
#define FRAMEBUFFER_WIDTH 320
#define FRAMEBUFFER_HEIGHT 240
#define FRAMEBUFFER_PIXEL_COUNT (FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT)
#define FRAMEBUFFER_SIZE (FRAMEBUFFER_PIXEL_COUNT * sizeof(u16))
#define RSP_OUTPUT_BUFFER_SIZE 0x8000
#define RSP_UCODE_DATA_SIZE 0x800
#define RSP_DRAM_STACK_SIZE 0x400
#define RSP_YIELD_BUFFER_SIZE 0xC00

typedef struct RuntimeViewportData {
    /* 0x000 */ Vp viewports[4];
    /* 0x040 */ Mtx projections[4];
    /* 0x140 */ Mtx overlayProjections[4];
    /* 0x240 */ Mtx rotations[4];
    /* 0x340 */ Mtx translations[4];
    /* 0x440 */ Mtx viewportMatrices[4];
} RuntimeViewportData;

typedef struct FrameRenderData {
    /* 0x000 */ u8 reserved[0x78];
    /* 0x078 */ RuntimeViewportData viewport;
} FrameRenderData;

extern Gfx *gRegionAllocPtr;
extern FrameRenderData *gCurrentFrameRenderData;
extern u16 gDepthBuffer[FRAMEBUFFER_PIXEL_COUNT];
extern u16 gFramebuffers[FRAMEBUFFER_COUNT][FRAMEBUFFER_PIXEL_COUNT];
extern u8 gFramebufferColorBufferIndex;
extern s32 gClearFramebufferOnNextTask;
extern u16 gLastSchedulerRetraceCounter;

extern u8 gCurrentViewportIndex;
extern s16 gUiBlinkTimer;
extern s16 gMenuViewportWidth;
extern s16 gMenuViewportHeight;
extern s16 gMenuViewportCenterX;
extern s16 gMenuViewportCenterY;

#endif
