#include "PR/os_internal.h"
#include "PRinternal/siint.h"
#include "PRinternal/controller.h"

s32 osPfsInitPak(OSMesgQueue* queue, OSPfs* pfs, int channel) {
    int k;
    s32 ret = 0;
    u16 sum;
    u16 isum;
    u8 temp[BLOCKSIZE];
    __OSPackId* id;
    __OSPackId newid;

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
    __osIdCheckSum((u16*)temp, &sum, &isum);
    id = (__OSPackId*)temp;

    if ((id->checksum != sum) || (id->inverted_checksum != isum)) {
        ret = __osCheckPackId(pfs, id);

        if (ret != 0) {
            return ret;
        } else if (ret != 0) {
            return ret;
        }
    }

    if (!(id->deviceid & 1)) {
        ret = __osRepairPackId(pfs, id, &newid);

        if (ret != 0) {
            return ret;
        }

        id = &newid;

        if (!(id->deviceid & 1)) {
            return PFS_ERR_DEVICE;
        }
    }

    for (k = 0; k < ARRLEN(pfs->id); k++) {
        pfs->id[k] = ((u8*)id)[k];
    }

    pfs->version = id->version;
    pfs->banks = id->banks;
    pfs->inode_start_page = 1 + DEF_DIR_PAGES + (2 * pfs->banks);
    pfs->dir_size = DEF_DIR_PAGES * PFS_ONE_PAGE;
    pfs->inode_table = 1 * PFS_ONE_PAGE;
    pfs->minode_table = (1 + pfs->banks) * PFS_ONE_PAGE;
    pfs->dir_table = pfs->minode_table + (pfs->banks * PFS_ONE_PAGE);

    ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_LABEL_AREA, pfs->label));

    ret = osPfsChecker(pfs);
    pfs->status |= PFS_INITIALIZED;

    return ret;
}
