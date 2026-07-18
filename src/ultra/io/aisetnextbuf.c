#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PRinternal/osint.h"

s32 osAiSetNextBuffer(void *vAddr, u32 size) {
    // Work around an AI DMA hardware bug: when a transfer ends exactly on the
    // 0x2000 boundary of a 16KB (0x4000) region, the next buffer's address must
    // be adjusted by -0x2000 to avoid a glitch.
    static u8 needsWorkaround = FALSE;
    u8 *ptr;

    ptr = vAddr;
    if (needsWorkaround) {
        ptr = (u8 *)vAddr - 0x2000;
    }

    if ((((u32)vAddr + size) & 0x3FFF) == 0x2000) {
        needsWorkaround = TRUE;
    } else {
        needsWorkaround = FALSE;
    }

    if (__osAiDeviceBusy()) {
        return -1;
    }

    IO_WRITE(AI_DRAM_ADDR_REG, osVirtualToPhysical(ptr));
    IO_WRITE(AI_LEN_REG, size);
    return 0;
}
