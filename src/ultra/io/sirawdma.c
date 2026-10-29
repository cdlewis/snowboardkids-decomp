#include "PR/os_internal.h"
#include "PRinternal/siint.h"

#define PIF_RAM_SIZE (PIF_RAM_END + 1 - PIF_RAM_START)

s32 __osSiRawStartDma(s32 direction, void *dramAddr) {
    if (__osSiDeviceBusy()) {
        return -1;
    }

    if (direction == OS_WRITE) {
        osWritebackDCache(dramAddr, PIF_RAM_SIZE);
    }

    IO_WRITE(SI_DRAM_ADDR_REG, osVirtualToPhysical(dramAddr));

    if (direction == OS_READ) {
        IO_WRITE(SI_PIF_ADDR_RD64B_REG, PIF_RAM_START);
    } else {
        IO_WRITE(SI_PIF_ADDR_WR64B_REG, PIF_RAM_START);
    }

    if (direction == OS_READ) {
        osInvalDCache(dramAddr, PIF_RAM_SIZE);
    }

    return 0;
}
