/* The GPU consumes a physical address, derived from the allocated depth buffer. */
gDepthBufferPhysicalAddress = gDepthBuffer - 0x80000000;

/* ORI uses an unsigned low half, rather than a carry-adjusted HI16. */
main_BSS_CLEAR_HI = main_BSS_SIZE >> 16;
main_BSS_CLEAR_LO = main_BSS_SIZE & 0xFFFF;

ASSERT(main_BSS_SIZE > 0 && (main_BSS_SIZE & 7) == 0, "boot BSS clear requires nonempty 8-byte units")
ASSERT(buffers_BSS_START >= main_BSS_END, "runtime buffers overlap main BSS")
ASSERT(buffers_BSS_END <= 0x80400000, "runtime layout exceeds original 4 MB RAM")
ASSERT((buffers_BSS_START & 15) == 0, "runtime buffers require 16-byte alignment")
ASSERT(gRelocatableHeapStart == buffers_BSS_START, "heap must begin the runtime buffer section")
ASSERT(gBootThreadStack == gRelocatableHeapStart + 0x1C2000, "heap or initial boot stack size changed")
ASSERT(gGameThreadStack == gBootThreadStack + 0x2000, "boot-thread stack size changed")
ASSERT(gRuntimeBufferReserved == gGameThreadStack + 0x4000, "game-thread stack size changed")
ASSERT(gRspYieldBuffer == gRspOutputBuffer + 0x8000, "RSP output buffer size changed")
ASSERT(gRspDramStack == gRspYieldBuffer + 0xC00, "RSP yield buffer size changed")
ASSERT(gDepthBuffer == gRspDramStack + 0x400, "RSP DRAM stack size changed")
ASSERT(gFramebuffers == gDepthBuffer + 0x25800, "depth buffer size changed")
ASSERT(gFramebuffers + 0x70800 == buffers_BSS_END, "framebuffer storage must end the runtime section")
ASSERT((gDepthBuffer & 63) == 0, "depth buffer requires 64-byte alignment")
ASSERT((gFramebuffers & 63) == 0, "framebuffers require 64-byte alignment")

ASSERT(entry_ROM_START == 0x1000 && entry_VRAM == 0x80000400, "hardware boot placement changed")
/* Original IPL3 copies one MB from ROM 0x1000 to the header entry address. */
ASSERT(main_VRAM - main_ROM_START == entry_VRAM - entry_ROM_START, "main violates IPL3 ROM-to-RAM mapping")
ASSERT(main_ROM_END <= entry_ROM_START + 0x100000, "main exceeds IPL3 1 MB boot transfer")
