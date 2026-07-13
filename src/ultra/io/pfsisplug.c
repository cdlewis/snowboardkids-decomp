#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PRinternal/controller.h"
#include "PRinternal/siint.h"

OSPifRam __osPfsPifRam;

s32 osPfsIsPlug(OSMesgQueue *mq, u8 *pattern) {
    s32 ret = 0;
    OSMesg msg;
    u8 bitpattern;
    OSContStatus contData[MAXCONTROLLERS];
    s32 channel;
    u8 bits = 0;
    s32 crcErrorCount = 3;

    __osSiGetAccess();

    do {
        __osPfsRequestData(CONT_CMD_REQUEST_STATUS);

        ret = __osSiRawStartDma(OS_WRITE, &__osPfsPifRam);
        osRecvMesg(mq, &msg, OS_MESG_BLOCK);

        ret = __osSiRawStartDma(OS_READ, &__osPfsPifRam);
        osRecvMesg(mq, &msg, OS_MESG_BLOCK);

        __osPfsGetInitData(&bitpattern, &contData[0]);

        for (channel = 0; channel < __osMaxControllers; channel++) {
            if ((contData[channel].status & CONT_ADDR_CRC_ER) == 0) {
                crcErrorCount--;
                break;
            }
        }

        if (channel == __osMaxControllers) {
            crcErrorCount = 0;
        }
    } while (crcErrorCount > 0);

    for (channel = 0; channel < __osMaxControllers; channel++) {
        if ((contData[channel].errno == 0) && ((contData[channel].status & CONT_CARD_ON) != 0)) {
            bits |= (1 << channel);
        }
    }
    __osSiRelAccess();
    *pattern = bits;
    return ret;
}

void __osPfsRequestData(u8 cmd) {
    __OSContRequesFormat *request;
    __OSContRequesFormat requestFormat;
    int i;

    __osContLastCmd = cmd;
    for (i = 0; i < ARRLEN(__osPfsPifRam.ramarray) + 1; i++) {
        ((u32 *)&__osPfsPifRam)[i] = 0;
    }
    __osPfsPifRam.pifstatus = CONT_CMD_EXE;
    request = (__OSContRequesFormat *)__osPfsPifRam.ramarray;

    requestFormat.dummy = CONT_CMD_NOP;
    requestFormat.txsize = CONT_CMD_REQUEST_STATUS_TX;
    requestFormat.rxsize = CONT_CMD_REQUEST_STATUS_RX;
    requestFormat.cmd = cmd;
    requestFormat.typeh = CONT_CMD_NOP;
    requestFormat.typel = CONT_CMD_NOP;
    requestFormat.status = CONT_CMD_NOP;
    requestFormat.dummy1 = CONT_CMD_NOP;

    for (i = 0; i < __osMaxControllers; i++) {
        *request++ = requestFormat;
    }

    *(u8 *)request = CONT_CMD_END;
}

void __osPfsGetInitData(u8 *pattern, OSContStatus *data) {
    u8 *ptr;
    __OSContRequesFormat requestformat;
    int i;
    u8 bits = 0;

    ptr = (u8 *)&__osPfsPifRam;

    for (i = 0; i < __osMaxControllers; i++, ptr += sizeof(requestformat), data++) {
        requestformat = *((__OSContRequesFormat *)ptr);
        data->errno = CHNL_ERR(requestformat);

        if (data->errno != 0) {
            continue;
        }

        data->type = ((requestformat.typel << 8) | requestformat.typeh);
        data->status = requestformat.status;
        bits |= (1 << i);
    }
    *pattern = bits;
}
