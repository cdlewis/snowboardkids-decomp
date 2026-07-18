#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PRinternal/controller.h"
#include "PRinternal/siint.h"

#define READFORMAT(ptr) ((__OSContRamReadFormat *)(ptr))
#define PFS_PIF_RAM_WORDS 16
#define PFS_ADDRESS_SHIFT 5

static void __osPackRamReadData(int channel, u16 address);

s32 __osContRamRead(OSMesgQueue *mq, int channel, u16 address, u8 *buffer) {
    s32 ret = 0;
    int i;
    u8 *ptr = (u8 *)&__osPfsPifRam;
    __OSContRamReadFormat ramreadformat;
    int retry = 2;

    __osSiGetAccess();
    __osContLastCmd = CONT_CMD_READ_PAK;
    __osPackRamReadData(channel, address);
    ret = __osSiRawStartDma(OS_WRITE, &__osPfsPifRam);
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);

    do {
        ret = __osSiRawStartDma(OS_READ, &__osPfsPifRam);
        osRecvMesg(mq, NULL, OS_MESG_BLOCK);
        ptr = (u8 *)&__osPfsPifRam;

        if (channel != 0) {
            for (i = 0; i < channel; i++) {
                ptr++;
            }
        }

        ramreadformat = *READFORMAT(ptr);

        ret = CHNL_ERR(ramreadformat);
        if (ret == 0) {
            u8 c = __osContDataCrc((u8 *)&ramreadformat.data);
            if (c != ramreadformat.datacrc) {
                ret = __osPfsGetStatus(mq, channel);

                if (ret != 0) {
                    __osSiRelAccess();
                    return ret;
                }

                ret = PFS_ERR_CONTRFAIL;
            } else {
                for (i = 0; i < ARRLEN(ramreadformat.data); i++) {
                    *buffer++ = ramreadformat.data[i];
                }
            }
        } else {
            ret = PFS_ERR_NOPACK;
        }
    } while ((ret == PFS_ERR_CONTRFAIL) && retry-- >= 0);

    __osSiRelAccess();

    return ret;
}

static void __osPackRamReadData(int channel, u16 address) {
    u8 *cmdBufPtr;
    __OSContRamReadFormat readFormat;
    int i;

    cmdBufPtr = (u8 *)__osPfsPifRam.ramarray;
    for (i = 0; i < PFS_PIF_RAM_WORDS; i++) {
        __osPfsPifRam.words[i] = 0;
    }
    __osPfsPifRam.pifstatus = CONT_CMD_EXE;
    readFormat.dummy = CONT_CMD_NOP;
    readFormat.txsize = CONT_CMD_READ_PAK_TX;
    readFormat.rxsize = CONT_CMD_READ_PAK_RX;
    readFormat.cmd = CONT_CMD_READ_PAK;
    readFormat.address = (address << PFS_ADDRESS_SHIFT) | __osContAddressCrc(address);
    readFormat.datacrc = CONT_CMD_NOP;

    for (i = 0; i < ARRLEN(readFormat.data); i++) {
        readFormat.data[i] = CONT_CMD_NOP;
    }

    if (channel != 0) {
        for (i = 0; i < channel; i++) {
            *cmdBufPtr++ = CONT_CMD_REQUEST_STATUS;
        }
    }

    *(__OSContRamReadFormat *)cmdBufPtr = readFormat;
    cmdBufPtr += sizeof(__OSContRamReadFormat);
    cmdBufPtr[0] = CONT_CMD_END;
}
