#include "PR/os_internal.h"
#include "PR/rcp.h"

#ident "$Revision: 1.17 $"

u32 __osSpGetStatus(void) {
    return IO_READ(SP_STATUS_REG);
}
