#include "PR/os_internal.h"
#include "PRinternal/siint.h"
#include "PRinternal/controller.h"

s32 osPfsInitPak(OSMesgQueue* queue, OSPfs* pfs, int channel) {
    int byteIndex;
    s32 ret = 0;
    u16 checksum;
    u16 invertedChecksum;
    u8 packIdBlock[BLOCKSIZE];
    __OSPackId* packId;
    __OSPackId repairedPackId;

    __osSiGetAccess();

    ret = __osPfsGetStatus(queue, channel);

    __osSiRelAccess();

    if (ret != 0) {
        return ret;
    }

    pfs->queue = queue;
    pfs->channel = channel;
    pfs->status = 0;

    ERRCK(SELECT_BANK(pfs, 0));
    __osIdCheckSum((u16*)packIdBlock, &checksum, &invertedChecksum);
    packId = (__OSPackId*)packIdBlock;

    if ((packId->checksum != checksum) || (packId->inverted_checksum != invertedChecksum)) {
        // The original binary checks the result twice; keep the duplicate to preserve the match.
        ERRCK(__osCheckPackId(pfs, packId));
        if (ret != 0) {
            return ret;
        }
    }

    if (!(packId->deviceid & PFS_ID_DEVICE_ID_BIT)) {
        ret = __osRepairPackId(pfs, packId, &repairedPackId);

        if (ret != 0) {
            return ret;
        }

        packId = &repairedPackId;

        if (!(packId->deviceid & PFS_ID_DEVICE_ID_BIT)) {
            return PFS_ERR_DEVICE;
        }
    }

    for (byteIndex = 0; byteIndex < ARRLEN(pfs->id); byteIndex++) {
        pfs->id[byteIndex] = ((u8*)packId)[byteIndex];
    }

    pfs->version = packId->version;
    pfs->banks = packId->banks;
    pfs->inode_start_page = 1 + DEF_DIR_PAGES + (2 * pfs->banks);
    pfs->dir_size = DEF_DIR_PAGES * PFS_ONE_PAGE;
    pfs->inode_table = PFS_ONE_PAGE;
    pfs->minode_table = (1 + pfs->banks) * PFS_ONE_PAGE;
    pfs->dir_table = pfs->minode_table + (pfs->banks * PFS_ONE_PAGE);

    ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_LABEL_AREA, pfs->label));

    ret = osPfsChecker(pfs);
    pfs->status |= PFS_INITIALIZED;

    return ret;
}
