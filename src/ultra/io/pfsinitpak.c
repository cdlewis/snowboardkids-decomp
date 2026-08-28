#include "PR/os_internal.h"
#include "PRinternal/siint.h"
#include "PRinternal/controller.h"

s32 osPfsInitPak(OSMesgQueue *queue, OSPfs *pfs, int channel) {
    int byteIndex;
    s32 ret = 0;
    u16 calculatedChecksum;
    u16 calculatedInvertedChecksum;
    u8 candidatePackIdBlock[BLOCKSIZE];
    __OSPackId *packId;
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
    /* The VERSION_I binary omits the primary ID-block read found in later SDK versions. */
    __osIdCheckSum((__OSPackId *)candidatePackIdBlock, &calculatedChecksum, &calculatedInvertedChecksum);
    packId = (__OSPackId *)candidatePackIdBlock;

    /* Recover a valid ID from one of the Controller Pak's redundant ID blocks. */
    if ((packId->checksum != calculatedChecksum) || (packId->inverted_checksum != calculatedInvertedChecksum)) {
        /* ERRCK performs the first result check; the duplicate below is required for the match. */
        ERRCK(__osCheckPackId(pfs, packId));
        if (ret != 0) {
            return ret;
        }
    }

    /* Probe and repair the ID before accepting the accessory as a Controller Pak. */
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

    /* Cache the ID and derived file-system layout for subsequent PFS operations. */
    for (byteIndex = 0; byteIndex < ARRLEN(pfs->id); byteIndex++) {
        pfs->id[byteIndex] = ((u8 *)packId)[byteIndex];
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
