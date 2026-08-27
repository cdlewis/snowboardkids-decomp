#ifndef SYSTEM_RENDER_H
#define SYSTEM_RENDER_H

#include "common.h"

#define FRAMEBUFFER_COUNT 3
#define FRAMEBUFFER_SIZE 0x25800
#define RSP_OUTPUT_BUFFER_SIZE 0x8000
#define RSP_UCODE_DATA_SIZE 0x800
#define RSP_DRAM_STACK_SIZE 0x400
#define RSP_YIELD_BUFFER_SIZE 0xC00

extern Gfx *gRegionAllocPtr;
extern Gfx *gCurrentTaskDisplayListStart;
extern u8 gFramebufferColorBufferIndex;
extern s32 gClearFramebufferOnNextTask;
extern u16 gLastSchedulerRetraceCounter;

extern u8 gCurrentViewportIndex;
extern s16 gUiBlinkTimer;
extern s16 gMenuViewportWidth;
extern s16 gMenuViewportHeight;
extern s16 gMenuViewportCenterX;
extern s16 gMenuViewportCenterY;

void appendFadeOverlayDisplayList(void);
void appendViewportDisplayLists(u8 frameIndex);
void submitFramebufferRenderTask(u8 frameIndex);

#endif
