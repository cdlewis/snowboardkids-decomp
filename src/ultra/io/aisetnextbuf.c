#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PRinternal/osint.h"

/**
 * Queues an audio DMA from RDRAM in the AI's two-entry FIFO.
 *
 * The buffer must be 8-byte aligned and size must be a multiple of 8. Returns
 * -1 when both FIFO entries are occupied, or 0 when the DMA was queued.
 */
s32 osAiSetNextBuffer(void *buffer, u32 size) {
    static u8 applyAddressWorkaround = FALSE;
    u8 *dmaAddress;

    dmaAddress = buffer;
    if (applyAddressWorkaround) {
        // The AI would otherwise read this DMA from buffer + 0x2000 when the
        // preceding buffer ended at offset 0x2000 within a 16 KiB region.
        dmaAddress = (u8 *)buffer - 0x2000;
    }

    if ((((u32)buffer + size) & 0x3FFF) == 0x2000) {
        applyAddressWorkaround = TRUE;
    } else {
        applyAddressWorkaround = FALSE;
    }

    // VERSION_I updates the workaround state before checking the FIFO. A
    // rejected buffer can therefore affect the next call.
    if (__osAiDeviceBusy()) {
        return -1;
    }

    IO_WRITE(AI_DRAM_ADDR_REG, osVirtualToPhysical(dmaAddress));
    IO_WRITE(AI_LEN_REG, size);
    return 0;
}
