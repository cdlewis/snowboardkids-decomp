#define _FINALROM
#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PR/ultraerror.h"
#include "PRinternal/osint.h"

__OSEventState __osEventStateTab[OS_NUM_EVENTS] ALIGNED(0x8);

void osSetEventMesg(OSEvent event, OSMesgQueue *mq, OSMesg msg) {
    register u32 saveMask;
    __OSEventState *es;

#ifdef _DEBUG
    if (event >= OS_NUM_EVENTS) {
        __osError(ERR_OSSETEVENTMESG, 1, event);
        return;
    }
#endif

    saveMask = __osDisableInt();

    es = &__osEventStateTab[event];

    es->messageQueue = mq;
    es->message = msg;

    __osRestoreInt(saveMask);
}
