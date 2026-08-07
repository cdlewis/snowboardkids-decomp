#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PRinternal/osint.h"
#include "PRinternal/piint.h"
#include "PRinternal/macros.h"

extern OSPiHandle *__osDiskHandle;

#define LEO_SECTORS_PER_BLOCK 85
#define LEO_SECTOR_LIMIT 0x54
#define LEO_TRACK_SECTOR_COUNT 88
#define LEO_C2_WORD_COUNT 4
#define LEO_MAX_C1_SECTOR_INDEX 3
#define LEO_SECTOR_C1_ERROR_LIMIT 17
#define LEO_LAST_DATA_SECTOR_BEFORE_C2 0x52

u8 leoDiskStack[OS_PIM_STACKSIZE] ALIGNED(0x10);

static void __osLeoAbnormalResume(void);
static void __osLeoResume(void);

s32 __osLeoInterrupt(void) {
    u32 leoStatus = 0;
    volatile u32 piStatus;
    u32 bmStatus;
    __OSTranxInfo *transferInfo = &__osDiskHandle->transferInfo;
    __OSBlockInfo *blockInfo = &transferInfo->block[transferInfo->blockNum];

    piStatus = IO_READ(PI_STATUS_REG);
    if (piStatus & PI_STATUS_DMA_BUSY) {
        __OSGlobalIntMask = __OSGlobalIntMask & ~SR_IBIT4; // cart interrupt
        blockInfo->errStatus = LEO_ERROR_29;
        __osLeoResume();
        return 1;
    }

    WAIT_ON_IOBUSY(piStatus);
    leoStatus = IO_READ(LEO_STATUS);
    if (leoStatus & LEO_STATUS_MECHANIC_INTERRUPT) {
        WAIT_ON_IOBUSY(piStatus);
        IO_WRITE(LEO_BM_CTL, transferInfo->bmCtlShadow | LEO_BM_CTL_CLR_MECHANIC_INTR);
        blockInfo->errStatus = LEO_ERROR_GOOD;
        return 0;
    }

    if (transferInfo->cmdType == LEO_CMD_TYPE_C2_TRANSFER) {
        return 1;
    }

    if (leoStatus & LEO_STATUS_BUFFER_MANAGER_ERROR) {
        WAIT_ON_IOBUSY(piStatus);
        leoStatus = IO_READ(LEO_STATUS);
        blockInfo->errStatus = LEO_ERROR_22;
        __osLeoResume();
        IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
        __OSGlobalIntMask |= OS_IM_PI;
        return 1;
    }

    if (transferInfo->cmdType == LEO_CMD_TYPE_WRITE) {
        if ((leoStatus & LEO_STATUS_DATA_REQUEST) == 0) {
            if (transferInfo->sectorNum + 1 != transferInfo->transferMode * LEO_SECTORS_PER_BLOCK) {
                blockInfo->errStatus = LEO_ERROR_24;
                __osLeoAbnormalResume();
                return 1;
            }

            IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
            __OSGlobalIntMask |= OS_IM_PI;
            blockInfo->errStatus = LEO_ERROR_GOOD;
            __osLeoResume();
            return 1;
        } else {
            blockInfo->dramAddr = (void *)((u32)blockInfo->dramAddr + blockInfo->sectorSize);
            transferInfo->sectorNum++;
            __osEPiRawStartDma(__osDiskHandle, OS_WRITE, LEO_SECTOR_BUFF, blockInfo->dramAddr, blockInfo->sectorSize);
            return 1;
        }
    } else if (transferInfo->cmdType == LEO_CMD_TYPE_READ) {
        if (transferInfo->transferMode == LEO_SECTOR_MODE) {
            if (transferInfo->sectorNum > (s32)blockInfo->C1ErrNum + LEO_SECTOR_C1_ERROR_LIMIT) {
                blockInfo->errStatus = LEO_ERROR_GOOD;
                __osLeoAbnormalResume();
                return 1;
            }

            if ((leoStatus & LEO_STATUS_DATA_REQUEST) == 0) {
                blockInfo->errStatus = LEO_ERROR_23;
                __osLeoAbnormalResume();
                return 1;
            }
        } else {
            blockInfo->dramAddr = (void *)((u32)blockInfo->dramAddr + blockInfo->sectorSize);
        }

        bmStatus = IO_READ(LEO_BM_STATUS);
        if ((bmStatus & LEO_BM_STATUS_C1SINGLE && bmStatus & LEO_BM_STATUS_C1DOUBLE) ||
            bmStatus & LEO_BM_STATUS_MICRO) {
            if (blockInfo->C1ErrNum > LEO_MAX_C1_SECTOR_INDEX) {
                if (transferInfo->transferMode != LEO_SECTOR_MODE ||
                    transferInfo->sectorNum > LEO_LAST_DATA_SECTOR_BEFORE_C2) {
                    blockInfo->errStatus = LEO_ERROR_23;
                    __osLeoAbnormalResume();
                    return 1;
                }
            } else {
                int errNum = blockInfo->C1ErrNum;
                blockInfo->C1ErrSector[errNum] = transferInfo->sectorNum + 1;
            }

            blockInfo->C1ErrNum++;
        }

        if (leoStatus & LEO_STATUS_C2_TRANSFER) {
            if (transferInfo->sectorNum + 1 != LEO_TRACK_SECTOR_COUNT) {
                blockInfo->errStatus = LEO_ERROR_24;
                __osLeoAbnormalResume();
            }

            if (transferInfo->transferMode == LEO_TRACK_MODE && transferInfo->blockNum == 0) {
                transferInfo->blockNum = 1;
                transferInfo->sectorNum = -1;
                transferInfo->block[1].dramAddr =
                    (void *)((u32)transferInfo->block[1].dramAddr - transferInfo->block[1].sectorSize);

                blockInfo->errStatus = LEO_ERROR_22;
            } else {
                IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
                __OSGlobalIntMask |= OS_IM_PI;
                transferInfo->cmdType = LEO_CMD_TYPE_C2_TRANSFER;
                blockInfo->errStatus = LEO_ERROR_GOOD;
            }

            __osEPiRawStartDma(
                __osDiskHandle,
                OS_READ,
                LEO_C2_BUFF,
                blockInfo->C2Addr,
                blockInfo->sectorSize * LEO_C2_WORD_COUNT
            );
            return 1;
        }

        if (transferInfo->sectorNum == -1 && transferInfo->transferMode == LEO_TRACK_MODE &&
            transferInfo->blockNum == 1) {
            __OSBlockInfo *firstBlockInfo = &transferInfo->block[0];
            if (firstBlockInfo->C1ErrNum == 0) {
                if (firstBlockInfo->C2Addr[0] | firstBlockInfo->C2Addr[1] | firstBlockInfo->C2Addr[2] |
                    firstBlockInfo->C2Addr[3]) {
                    firstBlockInfo->errStatus = LEO_ERROR_24;
                    __osLeoAbnormalResume();
                    return 1;
                }
            }

            firstBlockInfo->errStatus = LEO_ERROR_GOOD;
            __osLeoResume();
        }
        transferInfo->sectorNum++;
        if (leoStatus & LEO_STATUS_DATA_REQUEST) {
            if (transferInfo->sectorNum > LEO_SECTOR_LIMIT) {
                blockInfo->errStatus = LEO_ERROR_24;
                __osLeoAbnormalResume();
                return 1;
            }

            __osEPiRawStartDma(__osDiskHandle, OS_READ, LEO_SECTOR_BUFF, blockInfo->dramAddr, blockInfo->sectorSize);
            blockInfo->errStatus = LEO_ERROR_GOOD;
            return 1;
        } else if (transferInfo->sectorNum <= LEO_SECTOR_LIMIT) {
            blockInfo->errStatus = LEO_ERROR_24;
            __osLeoAbnormalResume();
            return 1;
        }

        return 1;
    } else {
        blockInfo->errStatus = LEO_ERROR_4;
        __osLeoAbnormalResume();
        return 1;
    }
}

static void __osLeoAbnormalResume(void) {
    __OSTranxInfo *transferInfo = &__osDiskHandle->transferInfo;
    u32 piStatus;

    WAIT_ON_IOBUSY(piStatus);
    IO_WRITE(LEO_BM_CTL, transferInfo->bmCtlShadow | LEO_BM_CTL_RESET);
    WAIT_ON_IOBUSY(piStatus);
    IO_WRITE(LEO_BM_CTL, transferInfo->bmCtlShadow);
    __osLeoResume();
    IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
    __OSGlobalIntMask |= OS_IM_PI;
}

static void __osLeoResume(void) {
    __OSEventState *es = &__osEventStateTab[OS_EVENT_PI];
    OSMesgQueue *mq = es->messageQueue;
    s32 last;

    if (mq == NULL || MQ_IS_FULL(mq)) {
        return;
    }

    last = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[last] = es->message;
    mq->validCount++;

    if (mq->mtqueue->next != NULL) {
        __osEnqueueThread(&__osRunQueue, __osPopThread(&mq->mtqueue));
    }
}
