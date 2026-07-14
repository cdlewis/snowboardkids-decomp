#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PRinternal/controller.h"
#include "PRinternal/siint.h"

#define READFORMAT(ptr) ((__OSContRamReadFormat *)(ptr))
#define PFS_PIF_RAM_WORDS 16
#define PFS_ADDRESS_SHIFT 5

static void __osPackRamWriteData(int channel, u16 address, u8 *buffer);

s32 __osContRamWrite(OSMesgQueue *mq, int channel, u16 address, u8 *buffer, int force) {
    s32 ret = 0;
    s32 i;
    u8 *responsePtr = (u8 *)&__osPfsPifRam;
    __OSContRamReadFormat writeFormat;
    s32 retry = 2;

    if ((force != TRUE) && (address < PFS_LABEL_AREA) && (address != 0)) {
        return 0;
    }

    __osSiGetAccess();
    __osContLastCmd = CONT_CMD_WRITE_PAK;
    __osPackRamWriteData(channel, address, buffer);
    ret = __osSiRawStartDma(OS_WRITE, &__osPfsPifRam);
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);

    do {
        ret = __osSiRawStartDma(OS_READ, &__osPfsPifRam);
        osRecvMesg(mq, NULL, OS_MESG_BLOCK);
        responsePtr = (u8 *)&__osPfsPifRam;

        if (channel != 0) {
            for (i = 0; i < channel; i++) {
                responsePtr++;
            }
        }

        writeFormat = *READFORMAT(responsePtr);

        ret = CHNL_ERR(writeFormat);
        if (ret == 0) {
            if (__osContDataCrc(buffer) != writeFormat.datacrc) {
                ret = __osPfsGetStatus(mq, channel);

                if (ret != 0) {
                    __osSiRelAccess();
                    return ret;
                }

                ret = PFS_ERR_CONTRFAIL;
            }
        } else {
            ret = PFS_ERR_NOPACK;
        }
    } while ((ret == PFS_ERR_CONTRFAIL) && retry-- >= 0);

    __osSiRelAccess();

    return ret;
}

static void __osPackRamWriteData(int channel, u16 address, u8 *buffer) {
    u8 *cmdBufPtr;
    __OSContRamReadFormat writeFormat;
    int i;

    cmdBufPtr = (u8 *)__osPfsPifRam.ramarray;
    for (i = 0; i < PFS_PIF_RAM_WORDS; i++) {
        ((u32 *)&__osPfsPifRam)[i] = 0;
    }
    __osPfsPifRam.pifstatus = CONT_CMD_EXE;
    writeFormat.dummy = CONT_CMD_NOP;
    writeFormat.txsize = CONT_CMD_WRITE_PAK_TX;
    writeFormat.rxsize = CONT_CMD_WRITE_PAK_RX;
    writeFormat.cmd = CONT_CMD_WRITE_PAK;
    writeFormat.address = (address << PFS_ADDRESS_SHIFT) | __osContAddressCrc(address);
    writeFormat.datacrc = CONT_CMD_NOP;

    for (i = 0; i < ARRLEN(writeFormat.data); i++) {
        writeFormat.data[i] = *buffer++;
    }

    if (channel != 0) {
        for (i = 0; i < channel; i++) {
            *cmdBufPtr++ = CONT_CMD_REQUEST_STATUS;
        }
    }

    *(__OSContRamReadFormat *)cmdBufPtr = writeFormat;
    cmdBufPtr += sizeof(__OSContRamReadFormat);
    cmdBufPtr[0] = CONT_CMD_END;
}
