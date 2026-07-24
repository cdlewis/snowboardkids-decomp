#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PRinternal/osint.h"

/**
 * Queues an audio buffer in the AI's two-entry DMA FIFO.
 *
 * Returns -1 when both FIFO entries are occupied, or 0 when the buffer was
 * queued successfully.
 */
s32 osAiSetNextBuffer(void *buffer, u32 size) {
    static u8 previousDmaEndedAtBoundary = FALSE;
    u8 *dmaAddress;

    dmaAddress = buffer;
    if (previousDmaEndedAtBoundary) {
        // Compensate for an AI hardware bug triggered when the preceding DMA
        // ends at offset 0x2000 within a 16 KiB region.
        dmaAddress = (u8 *)buffer - 0x2000;
    }

    if ((((u32)buffer + size) & 0x3FFF) == 0x2000) {
        previousDmaEndedAtBoundary = TRUE;
    } else {
        previousDmaEndedAtBoundary = FALSE;
    }

    // This check intentionally follows the workaround state update to match
    // the original libultra implementation. A rejected buffer can therefore
    // leave the state describing a DMA that was never queued.
    if (__osAiDeviceBusy()) {
        return -1;
    }

    IO_WRITE(AI_DRAM_ADDR_REG, osVirtualToPhysical(dmaAddress));
    IO_WRITE(AI_LEN_REG, size);
    return 0;
}
