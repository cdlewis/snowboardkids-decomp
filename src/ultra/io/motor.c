#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PR/os_version.h"
#include "PRinternal/controller.h"
#include "PRinternal/siint.h"

extern OSPifRam _MotorStopData[MAXCONTROLLERS];
extern OSPifRam _MotorStartData[MAXCONTROLLERS];
extern u8 _motorstopbuf[BLOCKSIZE];
extern u8 _motorstartbuf[BLOCKSIZE];
u32 __osMotorinitialized[MAXCONTROLLERS] = {0, 0, 0, 0};

s32 osMotorStop(OSPfs *pfs) {
    int i;
    s32 ret;
    u8 *ptr;
    __OSContRamReadFormat ramreadformat;

    ptr = (u8 *)&__osPfsPifRam;

    if (!__osMotorinitialized[pfs->channel]) {
        return PFS_ERR_INVALID;
    }

    __osSiGetAccess();

    __osContLastCmd = CONT_CMD_WRITE_PAK;
    __osSiRawStartDma(OS_WRITE, &_MotorStopData[pfs->channel]);
    osRecvMesg(pfs->queue, NULL, OS_MESG_BLOCK);
    ret = __osSiRawStartDma(OS_READ, &__osPfsPifRam);
    osRecvMesg(pfs->queue, NULL, OS_MESG_BLOCK);
    ptr = (u8 *)&__osPfsPifRam;

    if (pfs->channel != 0) {
        for (i = 0; i < pfs->channel; i++) {
            ptr++;
        }
    }

    ramreadformat = *(__OSContRamReadFormat *)ptr;
    ret = CHNL_ERR(ramreadformat);

    if (ret == 0 && __osContDataCrc((u8*)&_motorstopbuf) != ramreadformat.datacrc) {
        ret = PFS_ERR_CONTRFAIL;
    }

    __osSiRelAccess();
    return ret;
}

s32 osMotorStart(OSPfs *pfs) {
    int i;
    s32 ret;
    u8 *ptr;
    __OSContRamReadFormat ramreadformat;

    ptr = (u8 *)&__osPfsPifRam;

    if (!__osMotorinitialized[pfs->channel]) {
        return PFS_ERR_INVALID;
    }

    __osSiGetAccess();

    __osContLastCmd = CONT_CMD_WRITE_PAK;
    __osSiRawStartDma(OS_WRITE, &_MotorStartData[pfs->channel]);
    osRecvMesg(pfs->queue, NULL, OS_MESG_BLOCK);
    ret = __osSiRawStartDma(OS_READ, &__osPfsPifRam);
    osRecvMesg(pfs->queue, NULL, OS_MESG_BLOCK);
    ptr = (u8 *)&__osPfsPifRam;

    if (pfs->channel != 0) {
        for (i = 0; i < pfs->channel; i++) {
            ptr++;
        }
    }

    ramreadformat = *(__OSContRamReadFormat *)ptr;
    ret = CHNL_ERR(ramreadformat);

    if (ret == 0 && __osContDataCrc((u8*)&_motorstartbuf) != ramreadformat.datacrc) {
        ret = PFS_ERR_CONTRFAIL;
    }

    __osSiRelAccess();
    return ret;
}

static void _MakeMotorData(int channel, u16 address, u8 *buffer, OSPifRam *mdata) {
    u8 *ptr = (u8 *)mdata->ramarray;
    __OSContRamReadFormat ramreadformat;
    int i;

    for (i = 0; i < ARRLEN(mdata->ramarray); i++) {
        mdata->ramarray[i] = 0;
    }

    mdata->pifstatus = CONT_CMD_EXE;
    ramreadformat.dummy = CONT_CMD_NOP;
    ramreadformat.txsize = CONT_CMD_WRITE_PAK_TX;
    ramreadformat.rxsize = CONT_CMD_WRITE_PAK_RX;
    ramreadformat.cmd = CONT_CMD_WRITE_PAK;

    ramreadformat.address = (address << 0x5) | __osContAddressCrc(address);
    ramreadformat.datacrc = CONT_CMD_NOP;

    for (i = 0; i < ARRLEN(ramreadformat.data); i++) {
        ramreadformat.data[i] = *buffer++;
    }

    if (channel != 0) {
        for (i = 0; i < channel; i++) {
            *ptr++ = 0;
        }
    }

    *(__OSContRamReadFormat *)ptr = ramreadformat;
    ptr += sizeof(__OSContRamReadFormat);
    ptr[0] = CONT_CMD_END;
}

s32 osMotorInit(OSMesgQueue* mq, OSPfs* pfs, int channel) {
    int i;
    s32 ret;
    u8 temp[32];

    pfs->queue = mq;
    pfs->channel = channel;
    pfs->status = 0;
    pfs->activebank = 128;

    for (i = 0; i < ARRLEN(temp); i++) {
        temp[i] = 254;
    }

    ret = __osContRamWrite(mq, channel, CONT_BLOCK_DETECT, temp, FALSE);

    if (ret == PFS_ERR_NEW_PACK) {
        ret = __osContRamWrite(mq, channel, CONT_BLOCK_DETECT, temp, FALSE);
    }

    if (ret != 0) {
        return ret;
    }

    ret = __osContRamRead(mq, channel, CONT_BLOCK_DETECT, temp);

    if (ret == PFS_ERR_NEW_PACK) {
        ret = PFS_ERR_CONTRFAIL;
    }

    if (ret != 0) {
        return ret;
    }

    if (temp[31] == 254) {
        return PFS_ERR_DEVICE;
    }

    for (i = 0; i < ARRLEN(temp); i++) {
        temp[i] = 128;
    }

    ret = __osContRamWrite(mq, channel, CONT_BLOCK_DETECT, temp, FALSE);

    if (ret == PFS_ERR_NEW_PACK) {
        ret = __osContRamWrite(mq, channel, CONT_BLOCK_DETECT, temp, FALSE);
    }

    if (ret != 0) {
        return ret;
    }

    ret = __osContRamRead(mq, channel, CONT_BLOCK_DETECT, temp);
    if (ret == PFS_ERR_NEW_PACK) {
        ret = PFS_ERR_CONTRFAIL;
    }

    if (ret != 0) {
        return ret;
    }

    if (temp[31] != 0x80) {
        return PFS_ERR_DEVICE;
    }

    if (!__osMotorinitialized[channel]) {
        for (i = 0; i < ARRLEN(_motorstartbuf); i++) {
            _motorstartbuf[i] = 1;
            _motorstopbuf[i] = 0;
        }
        _MakeMotorData(channel, CONT_BLOCK_RUMBLE, _motorstartbuf, &_MotorStartData[channel]);
        _MakeMotorData(channel, CONT_BLOCK_RUMBLE, _motorstopbuf, &_MotorStopData[channel]);
        __osMotorinitialized[channel] = 1;
    }

    return 0;
}
