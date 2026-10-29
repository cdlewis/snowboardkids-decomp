#include "PR/os_internal.h"
#include "PRinternal/siint.h"

#if BUILD_VERSION < VERSION_J
#line 46
#endif

#ident "$Revision: 1.17 $"

s32 __osSiRawWriteIo(u32 devAddr, u32 data) {
    if (__osSiDeviceBusy()) {
        return -1;
    }

    IO_WRITE(devAddr, data);
    return 0;
}
