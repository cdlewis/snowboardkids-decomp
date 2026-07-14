#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PRinternal/controller.h"

s32 osPfsRepairId(OSPfs *pfs) {
    int byteIndex;
    u16 checksum;
    u16 invertedChecksum;
    __OSPackId packId;
    __OSPackId repairedPackId;
    s32 ret;
    __OSPackId *validId;

    SET_ACTIVEBANK_TO_ZERO();
    ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_ID_0AREA, (u8*)&packId));
    __osIdCheckSum((u16*)&packId, &checksum, &invertedChecksum);
    validId = &packId;

    if (validId->checksum != checksum || validId->inverted_checksum != invertedChecksum) {
        ret = __osCheckPackId(pfs, validId);

        if (ret == PFS_ERR_ID_FATAL) {
            ERRCK(__osRepairPackId(pfs, validId, &repairedPackId));
            validId = &repairedPackId;
        } else if (ret != 0) {
            return ret;
        }
    }

    if ((validId->deviceid & 1) == 0) {
        ERRCK(__osRepairPackId(pfs, validId, &repairedPackId));
        validId = &repairedPackId;

        if ((validId->deviceid & 1) == 0) {
            return PFS_ERR_DEVICE;
        }
    }

    for (byteIndex = 0; byteIndex < ARRLEN(pfs->id); byteIndex++) {
        pfs->id[byteIndex] = ((u8*)validId)[byteIndex];
    }

    pfs->version = validId->version;
    pfs->banks = validId->banks;
    pfs->inode_start_page = 1 + DEF_DIR_PAGES + (2 * pfs->banks);
    pfs->dir_size = DEF_DIR_PAGES * PFS_ONE_PAGE;
    pfs->inode_table = PFS_ONE_PAGE;
    pfs->minode_table = (1 + pfs->banks) * PFS_ONE_PAGE;
    pfs->dir_table = pfs->minode_table + pfs->banks * PFS_ONE_PAGE;
    ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_LABEL_AREA, pfs->label));
    return 0;
}
