#include "PRinternal/piint.h"
#include "PR/ultraerror.h"

#if BUILD_VERSION < VERSION_J
#line 46
#endif

#ident "$Revision: 1.17 $"

s32 __osPiRawReadIo(u32 devAddr, u32 *data) {
    register u32 stat;

#ifdef _DEBUG
    if (devAddr & 0x3) {
        __osError(ERR_OSPIRAWREADIO, 1, devAddr);
        return -1;
    }
#endif

    WAIT_ON_IOBUSY(stat);
    *data = IO_READ((u32)osRomBase | devAddr);

    return 0;
}
