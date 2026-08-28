#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PR/os_version.h"
#include "PRinternal/controller.h"
#include "PR/rmon.h"

#define PFS_BANK_PROBE_BLOCK 0
#define PFS_BANK_PROBE_MARKER 0x80

#if BUILD_VERSION >= VERSION_J
__OSInode __osPfsInodeCache ALIGNED(0x8);
s32 __osPfsInodeCacheChannel = -1;
u8 __osPfsInodeCacheBank = 250;
#endif

u16 __osSumcalc(u8 *ptr, int length) {
    int i;
    u32 sum = 0;
    u8 *tmp = ptr;

    for (i = 0; i < length; i++) {
        sum += *tmp++;
#if BUILD_VERSION < VERSION_J
        sum = sum & 0xFFFF;
#endif
    }

#if BUILD_VERSION >= VERSION_J
    return sum & 0xFFFF;
#else
    return sum;
#endif
}

s32 __osIdCheckSum(const __OSPackId *id, u16 *csum, u16 *icsum) {
    u16 data = 0;
    u32 j;

    *csum = *icsum = 0;

    for (j = 0; j < ((sizeof(__OSPackId) - sizeof(u32)) / sizeof(u8)); j += 2) {
        data = *(u16 *)((u32)id + j);
        *csum += data;
        *icsum += ~data;
    }

    return 0;
}

s32 __osRepairPackId(OSPfs *pfs, const __OSPackId *sourceId, __OSPackId *repairedId) {
    s32 ret = 0;
    u8 scratchBlock[BLOCKSIZE];
    u8 probeReadback[BLOCKSIZE];
    u8 controllerPakBit = 0;
    int i;
    int detectedBankCount;
    u16 idBlockAddresses[4];

#if BUILD_VERSION >= VERSION_J
    detectedBankCount = 0;
#else
    SET_ACTIVEBANK_TO_ZERO();
#endif

    repairedId->repaired = -1;
    repairedId->random = osGetCount();
    repairedId->serial_mid = sourceId->serial_mid;
    repairedId->serial_low = sourceId->serial_low;

#if BUILD_VERSION >= VERSION_J
    SET_ACTIVEBANK_TO_ZERO();
#else
    detectedBankCount = 0;
#endif

    /*
     * Write a distinct pattern to the first block of each candidate bank. The
     * readback proves the bank is writable; rechecking bank 0 proves later bank
     * selections do not alias it.
     */
    do {
        ERRCK(SELECT_BANK(pfs, detectedBankCount));
        ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_BANK_PROBE_BLOCK, scratchBlock));

        scratchBlock[0] = detectedBankCount | PFS_BANK_PROBE_MARKER;

        for (i = 1; i < ARRLEN(scratchBlock); i++) {
            scratchBlock[i] = ~scratchBlock[i];
        }

        ERRCK(__osContRamWrite(pfs->queue, pfs->channel, PFS_BANK_PROBE_BLOCK, scratchBlock, FALSE));
        ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_BANK_PROBE_BLOCK, probeReadback));

        for (i = 0; i < ARRLEN(probeReadback); i++) {
            if (probeReadback[i] != scratchBlock[i]) {
                break;
            }
        }

        if (i != ARRLEN(probeReadback)) {
            break;
        }

        if (detectedBankCount > 0) {
            ERRCK(SELECT_BANK(pfs, 0));
            ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_BANK_PROBE_BLOCK, scratchBlock));

            if (scratchBlock[0] != PFS_BANK_PROBE_MARKER) {
                break;
            }
        }

        detectedBankCount++;
    } while (detectedBankCount < PFS_MAX_BANKS);

#if BUILD_VERSION >= VERSION_J
    SET_ACTIVEBANK_TO_ZERO();
#else
    ERRCK(SELECT_BANK(pfs, 0));
#endif

    controllerPakBit = (detectedBankCount > 0) ? PFS_ID_DEVICE_ID_BIT : 0;

    repairedId->deviceid = (sourceId->deviceid & (u16)~PFS_ID_DEVICE_ID_BIT) | controllerPakBit;
    repairedId->banks = detectedBankCount;
    repairedId->version = sourceId->version;
    __osIdCheckSum(repairedId, &repairedId->checksum, &repairedId->inverted_checksum);
    idBlockAddresses[0] = PFS_ID_0AREA;
    idBlockAddresses[1] = PFS_ID_1AREA;
    idBlockAddresses[2] = PFS_ID_2AREA;
    idBlockAddresses[3] = PFS_ID_3AREA;

    /* Replace every redundant copy of the ID, then verify the primary copy. */
    for (i = 0; i < ARRLEN(idBlockAddresses); i++) {
        ERRCK(__osContRamWrite(pfs->queue, pfs->channel, idBlockAddresses[i], (u8 *)repairedId, TRUE));
    }

    ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_ID_0AREA, scratchBlock));

    for (i = 0; i < ARRLEN(scratchBlock); i++) {
        if (scratchBlock[i] != ((u8 *)repairedId)[i]) {
#if BUILD_VERSION >= VERSION_J
            return PFS_ERR_DEVICE;
#else
            return PFS_ERR_ID_FATAL;
#endif
        }
    }
    return 0;
}

s32 __osCheckPackId(OSPfs *pfs, __OSPackId *temp) {
    u16 index[4];
    s32 ret = 0;
    u16 sum;
    u16 isum;
    int i;
    int j;

    SET_ACTIVEBANK_TO_ZERO();
    index[0] = PFS_ID_0AREA;
    index[1] = PFS_ID_1AREA;
    index[2] = PFS_ID_2AREA;
    index[3] = PFS_ID_3AREA;
    for (i = 1; i < ARRLEN(index); i++) {
        ERRCK(__osContRamRead(pfs->queue, pfs->channel, index[i], (u8 *)temp));
        __osIdCheckSum(temp, &sum, &isum);
        if (temp->checksum == sum && temp->inverted_checksum == isum) {
            break;
        }
    }

    if (i == ARRLEN(index)) {
        return PFS_ERR_ID_FATAL;
    }

    for (j = 0; j < ARRLEN(index); j++) {
        if (j != i) {
            ERRCK(__osContRamWrite(pfs->queue, pfs->channel, index[j], (u8 *)temp, TRUE));
        }
    }

    return 0;
}

s32 __osGetId(OSPfs *pfs) {
#if BUILD_VERSION < VERSION_J
    int byteIndex;
#endif
    u16 calculatedChecksum;
    u16 calculatedInvertedChecksum;
    __OSPackId candidatePackId;
    __OSPackId repairedPackId;
    s32 ret;
    __OSPackId *validPackId;

    SET_ACTIVEBANK_TO_ZERO();
    ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_ID_0AREA, (u8 *)&candidatePackId));
    __osIdCheckSum(&candidatePackId, &calculatedChecksum, &calculatedInvertedChecksum);
    validPackId = &candidatePackId;

    /* Recover one of the redundant ID copies, or rebuild the ID if every copy is corrupt. */
    if (validPackId->checksum != calculatedChecksum ||
        validPackId->inverted_checksum != calculatedInvertedChecksum) {
        ret = __osCheckPackId(pfs, validPackId);

        if (ret == PFS_ERR_ID_FATAL) {
            ERRCK(__osRepairPackId(pfs, validPackId, &repairedPackId));
            validPackId = &repairedPackId;
        } else if (ret != 0) {
            return ret;
        }
    }

    /* Probe the accessory's banks before accepting it as a Controller Pak. */
    if (!(validPackId->deviceid & PFS_ID_DEVICE_ID_BIT)) {
        ERRCK(__osRepairPackId(pfs, validPackId, &repairedPackId));
        validPackId = &repairedPackId;

        if (!(validPackId->deviceid & PFS_ID_DEVICE_ID_BIT)) {
            return PFS_ERR_DEVICE;
        }
    }

    /* Cache the ID and derived file-system layout for subsequent PFS operations. */
#if BUILD_VERSION >= VERSION_J
    bcopy(validPackId, pfs->id, BLOCKSIZE);
#else
    for (byteIndex = 0; byteIndex < ARRLEN(pfs->id); byteIndex++) {
        pfs->id[byteIndex] = ((u8 *)validPackId)[byteIndex];
    }
#endif

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

s32 __osCheckId(OSPfs *pfs) {
#if BUILD_VERSION < VERSION_J
    int k;
#endif
    u8 temp[BLOCKSIZE];
    s32 ret;

#if BUILD_VERSION >= VERSION_J
    if (pfs->activebank != 0) {
        ret = __osPfsSelectBank(pfs, 0);

        if (ret == PFS_ERR_NEW_PACK) {
            ret = __osPfsSelectBank(pfs, 0);
        }

        if (ret != 0) {
            return ret;
        }
    }
#else
    SET_ACTIVEBANK_TO_ZERO();
#endif

    ret = __osContRamRead(pfs->queue, pfs->channel, PFS_ID_0AREA, (u8 *)temp);

    if (ret != 0) {
        if (ret != PFS_ERR_NEW_PACK) {
            return ret;
        }
        ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_ID_0AREA, (u8 *)temp));
    }

#if BUILD_VERSION >= VERSION_J
    if (bcmp(pfs->id, temp, BLOCKSIZE) != 0) {
        return PFS_ERR_NEW_PACK;
    }
#else
    for (k = 0; k < ARRLEN(temp); k++) {
        if (pfs->id[k] != temp[k])
            return PFS_ERR_NEW_PACK;
    }
#endif

    return 0;
}

s32 __osPfsRWInode(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank) {
    u8 sum;
    int j;
    s32 ret;
    int offset;
    u8 *addr;

#if BUILD_VERSION >= VERSION_J
    if (flag == PFS_READ && bank == __osPfsInodeCacheBank && (pfs->channel == __osPfsInodeCacheChannel)) {
        bcopy(&__osPfsInodeCache, inode, sizeof(__OSInode));
        return 0;
    }
#endif

    SET_ACTIVEBANK_TO_ZERO();

    offset = (bank > 0) ? 1 : pfs->inode_start_page;

    if (flag == PFS_WRITE) {
        inode->inode_page[0].inode_t.page =
            __osSumcalc((u8 *)&inode->inode_page[offset], (PFS_INODE_SIZE_PER_PAGE - offset) * 2);
    }

    for (j = 0; j < PFS_ONE_PAGE; j++) {
        addr = ((u8 *)inode->inode_page + j * BLOCKSIZE);

        if (flag == PFS_WRITE) {
            ret = __osContRamWrite(pfs->queue, pfs->channel, pfs->inode_table + bank * PFS_ONE_PAGE + j, addr, FALSE);
            ret = __osContRamWrite(pfs->queue, pfs->channel, pfs->minode_table + bank * PFS_ONE_PAGE + j, addr, FALSE);
        } else {
            ret = __osContRamRead(pfs->queue, pfs->channel, pfs->inode_table + bank * PFS_ONE_PAGE + j, addr);
        }

        if (ret != 0) {
            return ret;
        }
    }

    if (flag == PFS_READ) {
        sum = __osSumcalc((u8 *)&inode->inode_page[offset], (PFS_INODE_SIZE_PER_PAGE - offset) * 2);
        if (sum != inode->inode_page[0].inode_t.page) {
            for (j = 0; j < PFS_ONE_PAGE; j++) {
                addr = ((u8 *)inode->inode_page + j * BLOCKSIZE);
                ret = __osContRamRead(pfs->queue, pfs->channel, pfs->minode_table + bank * PFS_ONE_PAGE + j, addr);
            }

#if BUILD_VERSION >= VERSION_J
            sum = __osSumcalc((u8 *)&inode->inode_page[offset], (PFS_INODE_SIZE_PER_PAGE - offset) * 2);
#endif

            if (sum != inode->inode_page[0].inode_t.page) {
                return PFS_ERR_INCONSISTENT;
            }

            for (j = 0; j < PFS_ONE_PAGE; j++) {
                addr = ((u8 *)inode->inode_page + j * BLOCKSIZE);
                ret =
                    __osContRamWrite(pfs->queue, pfs->channel, pfs->inode_table + bank * PFS_ONE_PAGE + j, addr, FALSE);
            }
        }
#if BUILD_VERSION < VERSION_J
        else {
            for (j = 0; j < PFS_ONE_PAGE; j++) {
                addr = ((u8 *)inode->inode_page + j * 32);
                ret = __osContRamWrite(
                    pfs->queue,
                    pfs->channel,
                    pfs->minode_table + bank * PFS_ONE_PAGE + j,
                    addr,
                    FALSE
                );
            }
        }
#endif
    }

#if BUILD_VERSION >= VERSION_J
    __osPfsInodeCacheBank = bank;
    bcopy(inode, &__osPfsInodeCache, sizeof(__OSInode));
    __osPfsInodeCacheChannel = pfs->channel;
#endif

    return 0;
}

// This was moved into it's own file in 2.0J
#if BUILD_VERSION < VERSION_J
s32 __osPfsSelectBank(OSPfs *pfs) {
    u8 temp[BLOCKSIZE];
    int i;
    s32 ret = 0;

    for (i = 0; i < BLOCKSIZE; i++) {
        temp[i] = pfs->activebank;
    }

    ret = __osContRamWrite(pfs->queue, pfs->channel, CONT_BLOCK_DETECT, temp, FALSE);
    return ret;
}
#endif

#ifdef _DEBUG
s32 __osDumpId(OSPfs *pfs) {
    u8 id[BLOCKSIZE];
    __OSPackId *temp;
    s32 ret;

    ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_ID_0AREA, id));

    temp = (__OSPackId *)id;
    rmonPrintf("repaired %x\n", temp->repaired);
    rmonPrintf("random %x\n", temp->random);
    rmonPrintf("serial_mid %llu\n", temp->serial_mid);
    rmonPrintf("serial_low %llu\n", temp->serial_low);
    rmonPrintf("deviceid %x\n", temp->deviceid);
    rmonPrintf("banks %x\n", temp->banks);
    rmonPrintf("version %x\n", temp->version);
    rmonPrintf("checksum %x\n", temp->checksum);
    rmonPrintf("inverted_checksum %x\n", temp->inverted_checksum);
    return 0;
}
#endif
