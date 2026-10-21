#include "game/engine/runtime_memory.h"
#include "game/engine/system_render.h"

u8 gRelocatableHeapStart[RELOCATABLE_HEAP_SIZE];
static u8 sInitialBootStack[INITIAL_BOOT_STACK_SIZE];
u8 gBootThreadStack[BOOT_THREAD_STACK_SIZE];
u8 gGameThreadStack[GAME_THREAD_STACK_SIZE];
u8 D_80328480[REGION_ALLOCATION_BUFFER_SIZE];
u8 gRspOutputBuffer[RSP_OUTPUT_BUFFER_SIZE];
u8 gRspYieldBuffer[RSP_YIELD_BUFFER_SIZE];
u8 gRspDramStack[RSP_DRAM_STACK_SIZE];
u16 gDepthBuffer[FRAMEBUFFER_PIXEL_COUNT];
u16 gFramebuffers[FRAMEBUFFER_COUNT][FRAMEBUFFER_PIXEL_COUNT];
