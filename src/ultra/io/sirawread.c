#include "PR/os_internal.h"
#include "PRinternal/siint.h"

#if BUILD_VERSION < VERSION_J
#line 45
#endif

#ident "$Revision: 1.17 $"

s32 __osSiRawReadIo(u32 devAddr, u32 *data) {
    if (__osSiDeviceBusy()) {
        return -1;
    }

    *data = IO_READ(devAddr);
    return 0;
}
