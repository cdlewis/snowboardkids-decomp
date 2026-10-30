#include "game/engine/runtime_memory.h"
#include "game/engine/system_render.h"

/* Compile with Clang so explicit NOBITS storage preserves declaration order.
 * The original heap base is only 16-byte aligned; check GPU alignment at link time. */
#define RUNTIME_BSS __attribute__((section(".bss"), used, aligned(16)))

u8 gRelocatableHeapStart[RELOCATABLE_HEAP_SIZE] RUNTIME_BSS = { 0 };
static u8 sInitialBootStack[INITIAL_BOOT_STACK_SIZE] RUNTIME_BSS = { 0 };
u8 gBootThreadStack[BOOT_THREAD_STACK_SIZE] RUNTIME_BSS = { 0 };
u8 gGameThreadStack[GAME_THREAD_STACK_SIZE] RUNTIME_BSS = { 0 };
/* No allocator uses this span. Its regional size keeps the RSP buffers fixed. */
u8 gRuntimeBufferReserved[RUNTIME_BUFFER_RESERVED_SIZE] RUNTIME_BSS = { 0 };
u8 gRspOutputBuffer[RSP_OUTPUT_BUFFER_SIZE] RUNTIME_BSS = { 0 };
u8 gRspYieldBuffer[RSP_YIELD_BUFFER_SIZE] RUNTIME_BSS = { 0 };
u8 gRspDramStack[RSP_DRAM_STACK_SIZE] RUNTIME_BSS = { 0 };
u16 gDepthBuffer[FRAMEBUFFER_PIXEL_COUNT] RUNTIME_BSS = { 0 };
u16 gFramebuffers[FRAMEBUFFER_COUNT][FRAMEBUFFER_PIXEL_COUNT] RUNTIME_BSS = { 0 };
