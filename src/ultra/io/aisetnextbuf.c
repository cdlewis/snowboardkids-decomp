#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PRinternal/osint.h"

#define AI_DMA_BOUNDARY_MASK 0x3FFF
#define AI_DMA_CLICK_OFFSET 0x2000

s32 osAiSetNextBuffer(void *buffer, u32 size) {
    static u8 needsDmaAddressWorkaround = FALSE;
    u8 *dmaStart;

    dmaStart = buffer;

    if (needsDmaAddressWorkaround) {
        dmaStart = (u8 *)buffer - AI_DMA_CLICK_OFFSET;
    }

    if ((((u32)buffer + size) & AI_DMA_BOUNDARY_MASK) == AI_DMA_CLICK_OFFSET) {
        needsDmaAddressWorkaround = TRUE;
    } else {
        needsDmaAddressWorkaround = FALSE;
    }

    if (__osAiDeviceBusy()) {
        return -1;
    }

    IO_WRITE(AI_DRAM_ADDR_REG, osVirtualToPhysical(dmaStart));
    IO_WRITE(AI_LEN_REG, size);
    return 0;
}
