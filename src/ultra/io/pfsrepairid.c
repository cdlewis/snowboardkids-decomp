#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PRinternal/controller.h"

s32 osPfsRepairId(OSPfs *pfs) {
    int byteIndex;
    u16 checksum;
    u16 invertedChecksum;
    __OSPackId initialPackId;
    __OSPackId repairedPackId;
    s32 ret;
    __OSPackId *validPackId;

    SET_ACTIVEBANK_TO_ZERO();
    ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_ID_0AREA, (u8 *)&initialPackId));
    __osIdCheckSum((u16 *)&initialPackId, &checksum, &invertedChecksum);
    validPackId = &initialPackId;

    /* Recover another redundant ID copy, or rebuild the ID if every copy is corrupt. */
    if (validPackId->checksum != checksum || validPackId->inverted_checksum != invertedChecksum) {
        ret = __osCheckPackId(pfs, validPackId);

        if (ret == PFS_ERR_ID_FATAL) {
            ERRCK(__osRepairPackId(pfs, validPackId, &repairedPackId));
            validPackId = &repairedPackId;
        } else if (ret != 0) {
            return ret;
        }
    }

    /* Reprobe the accessory before accepting it as a Controller Pak. */
    if ((validPackId->deviceid & PFS_ID_DEVICE_ID_BIT) == 0) {
        ERRCK(__osRepairPackId(pfs, validPackId, &repairedPackId));
        validPackId = &repairedPackId;

        if ((validPackId->deviceid & PFS_ID_DEVICE_ID_BIT) == 0) {
            return PFS_ERR_DEVICE;
        }
    }

    /* Refresh the file-system geometry cached in the public handle. */
    for (byteIndex = 0; byteIndex < ARRLEN(pfs->id); byteIndex++) {
        pfs->id[byteIndex] = ((u8 *)validPackId)[byteIndex];
    }

    pfs->version = validPackId->version;
    pfs->banks = validPackId->banks;
    pfs->inode_start_page = 1 + DEF_DIR_PAGES + (2 * pfs->banks);
    pfs->dir_size = DEF_DIR_PAGES * PFS_ONE_PAGE;
    pfs->inode_table = PFS_ONE_PAGE;
    pfs->minode_table = (1 + pfs->banks) * PFS_ONE_PAGE;
    pfs->dir_table = pfs->minode_table + pfs->banks * PFS_ONE_PAGE;
    ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_LABEL_AREA, pfs->label));
    return 0;
}
