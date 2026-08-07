#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PR/os_version.h"
#include "PRinternal/piint.h"

typedef struct {
    unsigned int inst1;
    unsigned int inst2;
    unsigned int inst3;
    unsigned int inst4;
} __osExceptionVector;

extern __osExceptionVector __osExceptionPreamble[];

OSTime osClockRate = OS_CLOCK_RATE;
s32 osViClock = VI_NTSC_CLOCK;
u32 __osShutdown = 0;
u32 __OSGlobalIntMask = OS_IM_ALL;
u32 __osFinalrom;

extern void _bzero(void *, int);

void osInitialize(void) {
    u32 pifdata;
    u32 clock = 0;

    __osFinalrom = TRUE;

    __osSetSR(__osGetSR() | SR_CU1);
    __osSetFpcCsr(FPCSR_FS | FPCSR_EV | FPCSR_RM_RN);

    while (__osSiRawReadIo(PIF_RAM_END - 3, &pifdata)) {
        ;
    }
    while (__osSiRawWriteIo(PIF_RAM_END - 3, pifdata | 8)) {
        ;
    }
    *(__osExceptionVector *)UT_VEC = *__osExceptionPreamble;
    *(__osExceptionVector *)XUT_VEC = *__osExceptionPreamble;
    *(__osExceptionVector *)ECC_VEC = *__osExceptionPreamble;
    *(__osExceptionVector *)E_VEC = *__osExceptionPreamble;
    osWritebackDCache((void *)UT_VEC, E_VEC - UT_VEC + sizeof(__osExceptionVector));
    osInvalICache((void *)UT_VEC, E_VEC - UT_VEC + sizeof(__osExceptionVector));
    osMapTLBRdb();
    osPiRawReadIo(4, &clock);
    clock &= ~0xf;
    if (clock != 0) {
        osClockRate = clock;
    }
    osClockRate = osClockRate * 3 / 4;

    if (osResetType == 0) {
        _bzero(osAppNMIBuffer, OS_APP_NMI_BUFSIZE);
    }

    if (osTvType == OS_TV_PAL) {
        osViClock = VI_PAL_CLOCK;
    } else if (osTvType == OS_TV_MPAL) {
        osViClock = VI_MPAL_CLOCK;
    } else {
        osViClock = VI_NTSC_CLOCK;
    }
}
