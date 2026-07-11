#include "asset_manager.h"
#include "common.h"
#include "relocatable_heap.h"
#include "system_boot.h"

typedef struct {
    /* 0x0 */ s16 prev;
    /* 0x2 */ s16 next;
    /* 0x4 */ s16 weight;
    /* 0x6 */ s16 left;
    /* 0x8 */ s16 right;
    /* 0xA */ s16 value;
} HuffmanNode;

typedef struct {
    /* 0x0 */ s32 compressedSize;
    /* 0x4 */ u8 flags;
    /* 0x5 */ u8 pad5[3];
} CompressedAssetHeader;

typedef struct {
    /* 0x00 */ s16 assetHandles[0x20];
    /* 0x40 */ s16 compressedAssetHandle;
} AssetHandleTable;

struct RandomStateObject {
    u8 pad0[0x518];
    /* 0x518 */ u8 randomIndex;
};

extern u8 gRandomTable[];
extern u16 gMainRngIndex;
extern u16 gSecondaryRngIndex;
extern s16 gHuffmanQueueHead;
extern s16 gHuffmanQueueTail;
extern s16 gHuffmanQueueCount;
extern s16 gFrameCounter;
extern CompressedAssetHeader gCompressedAssetHeader;
extern HuffmanNode gHuffmanNodes[];
extern AssetHandleTable gAssetHandles;
extern s32 gHuffmanNodeCount;

s32 randomNextMain(void) {
    gMainRngIndex++;
    if (gFrameCounter == 0) {
        gMainRngIndex++;
    }
    gMainRngIndex &= 0xFF;
    return gRandomTable[gMainRngIndex];
}

s32 randomNextSecondary(void) {
    gSecondaryRngIndex++;
    gSecondaryRngIndex &= 0xFF;
    return gRandomTable[gSecondaryRngIndex];
}

void resetSecondaryRng(void) {
    gSecondaryRngIndex = 0;
}

u8 randomNextObject(RandomStateObject *arg0) {
    arg0->randomIndex++;
    return gRandomTable[arg0->randomIndex];
}

void resetGameplayRng(void) {
    gFrameCounter = 0;
    gMainRngIndex = 0;
    resetSecondaryRng();
}

// insertHuffmanQueueNode best match: 99.389%
#pragma GLOBAL_ASM("asm/nonmatchings/asset_manager/insertHuffmanQueueNode.s")

#ifdef NON_MATCHING
void insertHuffmanQueueNode(s16 arg0) {
    HuffmanNode *iterNode;
    HuffmanNode *node;
    HuffmanNode *curNode;
    s32 head;
    s32 cur;
    s16 oldHead;
    s16 next;
    s16 *count;
    s16 tailCopy;
    s32 end = -1;

    count = &gHuffmanQueueCount;
    oldHead = 1;
    head = gHuffmanQueueHead;
    *count += oldHead;
    cur = head;
    if (head == end) {
        oldHead = gHuffmanQueueTail;
        gHuffmanQueueHead = arg0;
        gHuffmanQueueTail = arg0;
        node = &gHuffmanNodes[arg0];
        node->next = head;
        node->prev = 0xFFFFu & oldHead;
        return;
    }

    if (cur >= 0) {
        do {
            curNode = &gHuffmanNodes[cur];
            iterNode = curNode;
            if (iterNode->weight < gHuffmanNodes[arg0].weight) {
                break;
            }
            cur = iterNode->prev;
        } while (cur >= 0);
    }

    node = &gHuffmanNodes[arg0];
    if (cur == end) {
        next = (tailCopy = gHuffmanQueueTail);
        gHuffmanNodes[next].prev = arg0;
        node->prev = end;
        gHuffmanQueueTail = arg0;
        node->next = next;
        return;
    }

    node->prev = cur;
    curNode = &gHuffmanNodes[cur];
    node->next = curNode->next;
    curNode->next = arg0;
    next = node->next;
    if (end == next) {
        gHuffmanQueueHead = arg0;
        return;
    }
    gHuffmanNodes[next].prev = arg0;
}
#endif

void removeHuffmanQueueNode(s16 arg0) {
    HuffmanNode *node;
    s16 prev;
    s16 next;
    s16 end = -1;

    if (gHuffmanQueueHead != end) {
        gHuffmanQueueCount--;
        node = &gHuffmanNodes[arg0];
        next = node->next;
        if (end != next) {
            gHuffmanNodes[next].prev = node->prev;
            prev = node->prev;
            next = node->next;
        } else {
            prev = node->prev;
            gHuffmanQueueHead = prev;
        }
        if (end != prev) {
            gHuffmanNodes[prev].next = next;
        } else {
            gHuffmanQueueTail = next;
        }
    }
}

// decompressHuffmanAssetPayload best match: 62.649%
#pragma GLOBAL_ASM("asm/nonmatchings/asset_manager/decompressHuffmanAssetPayload.s")

#ifdef NON_MATCHING
void decompressHuffmanAssetPayload(u8 arg0, s32 arg1, s32 arg2, s32 arg3) {
    HuffmanNode *temp_v0;
    HuffmanNode *temp_v0_2;
    HuffmanNode *temp_v1;
    HuffmanNode *temp_v1_2;
    HuffmanNode *temp_v1_3;
    s16 temp_a1;
    s16 temp_s3_2;
    s16 temp_s6_2;
    s16 temp_v0_3;
    s16 temp_v0_4;
    s16 temp_v0_5;
    s16 var_s0_2;
    s16 var_s0_3;
    s16 var_s0_4;
    s32 temp_a2;
    s32 temp_a2_2;
    s32 temp_at;
    s32 temp_s2;
    s32 var_a0;
    u8 *var_a1;
    u8 *var_s1;
    s32 var_s2;
    s32 var_t2;
    s32 var_v0;
    s8 temp_t3;
    u8 *var_s1_2;
    u8 *var_v1;
    u8 temp_s3;
    u8 temp_s6;
    u8 temp_t7;
    u8 temp_t8;
    u8 var_s0;
    u8 *var_a1_2;

    gHuffmanQueueHead = -1;
    gHuffmanQueueTail = -1;
    gHuffmanQueueCount = 0;
    gHuffmanNodeCount = 0;
    var_s2 = 0;
    var_s1 = (u8 *) arg1;
loop_1:
    temp_s3 = *var_s1;
    temp_s2 = var_s2 + 1;
    var_s1_2 = var_s1 + 1;
    if (temp_s2 == 1) {
block_3:
        temp_s6 = var_s1[1];
        var_s2 = temp_s2 + 1;
        temp_at = (s32) temp_s6 < (s32) temp_s3;
        var_s1 = var_s1_2 + 1;
        var_s0 = temp_s3;
        if (temp_at == 0) {
            do {
                temp_t7 = *var_s1;
                var_s2 += 1;
                var_s1 += 1;
                temp_v0 = &gHuffmanNodes[gHuffmanNodeCount];
                temp_v0->left = -1;
                temp_v0->right = -1;
                temp_v0->value = (s16) var_s0;
                temp_v0->weight = (s16) temp_t7;
                insertHuffmanQueueNode((s16) gHuffmanNodeCount);
                var_s0 += 1;
                gHuffmanNodeCount += 1;
            } while ((temp_s6 + 1) != var_s0);
        }
        goto loop_1;
    }
    if (temp_s3 != 0) {
        goto block_3;
    }
loop_7:
    if (gHuffmanQueueCount >= 2) {
        temp_s3_2 = gHuffmanQueueTail;
        removeHuffmanQueueNode(temp_s3_2);
        temp_s6_2 = gHuffmanQueueTail;
        removeHuffmanQueueNode(temp_s6_2);
        temp_v0_2 = &gHuffmanNodes[gHuffmanNodeCount];
        temp_v0_2->weight = gHuffmanNodes[temp_s3_2].weight + gHuffmanNodes[temp_s6_2].weight;
        temp_v0_2->left = temp_s6_2;
        temp_v0_2->right = temp_s3_2;
        temp_v0_2->value = -1;
        insertHuffmanQueueNode((s16) gHuffmanNodeCount);
        gHuffmanNodeCount += 1;
        goto loop_7;
    }
    var_a0 = 0;
    var_t2 = 0;
    if (arg0 == 0) {
        var_a1 = (u8 *) arg2;
        do {
            var_s0_2 = gHuffmanNodeCount - 1;
loop_13:
            temp_v1 = &gHuffmanNodes[var_s0_2];
            temp_v0_3 = temp_v1->value;
            if (temp_v0_3 == -1) {
                if (var_a0 == 8) {
                    var_s1 += 1;
                    var_a0 = 0;
                }
                var_v0 = *var_s1 & (1 << (7 - var_a0));
                var_a0 += 1;
                if (var_v0 == 0) {
                    var_s0_2 = temp_v1->left;
                } else {
                    var_s0_2 = temp_v1->right;
                }
                goto loop_13;
            }
            *var_a1 = (s8) (temp_v0_3 & 0xFF);
            var_t2 += 1;
            var_a1 += 1;
        } while (var_t2 != arg3);
    } else {
loop_23:
        temp_a1 = gHuffmanNodeCount - 1;
        var_s0_3 = temp_a1;
loop_24:
        temp_v1_2 = &gHuffmanNodes[var_s0_3];
        temp_v0_4 = temp_v1_2->value;
        temp_a2 = temp_v0_4 & 0xFF;
        if (temp_v0_4 == -1) {
            if (var_a0 == 8) {
                var_s1 += 1;
                var_a0 = 0;
            }
            var_v0 = *var_s1 & (1 << (7 - var_a0));
            var_a0 += 1;
            if (var_v0 == 0) {
                var_s0_3 = temp_v1_2->left;
            } else {
                var_s0_3 = temp_v1_2->right;
            }
            goto loop_24;
        }
        var_s0_4 = temp_a1;
loop_31:
        temp_v1_3 = &gHuffmanNodes[var_s0_4];
        temp_v0_5 = temp_v1_3->value;
        if (temp_v0_5 == -1) {
            if (var_a0 == 8) {
                var_s1 += 1;
                var_a0 = 0;
            }
            var_v0 = *var_s1 & (1 << (7 - var_a0));
            var_a0 += 1;
            if (var_v0 == 0) {
                var_s0_4 = temp_v1_3->left;
            } else {
                var_s0_4 = temp_v1_3->right;
            }
            goto loop_31;
        }
        temp_t3 = temp_v0_5 & 0xFF;
        if (temp_a2 == 0) {
            ((u8 *) arg2)[var_t2] = temp_t3;
            var_t2 += 1;
        } else {
            temp_a2_2 = (temp_a2 >> 4) & 0xF;
            var_v0 = 0;
            if (temp_a2_2 > 0) {
                var_a1_2 = (u8 *) arg2 + var_t2;
                var_v1 = (u8 *) arg2 + (var_t2 - (((temp_a2 << 8) | temp_t3) & 0xFFF));
                do {
                    temp_t8 = *var_v1;
                    var_v0 += 1;
                    var_t2 += 1;
                    var_a1_2 += 1;
                    var_v1 += 1;
                    var_a1_2[-1] = temp_t8;
                } while (var_v0 < temp_a2_2);
            }
        }
        if (var_t2 < arg3) {
            goto loop_23;
        }
    }
}
#endif

void loadCompressedRomAsset(void *arg0, void *arg1, s32 arg2) {
    s16 *sp28;
    s32 sp30;

    dmaReadRom((u32)arg0, &gCompressedAssetHeader, 8);
    gAssetHandles.assetHandles[arg2] = allocRelocatableHeapBlock(gCompressedAssetHeader.compressedSize);
    gAssetHandles.compressedAssetHandle = allocRelocatableHeapBlock((s32)arg1 - (s32)arg0);
    dmaReadRom((u32)arg0, (void *)getRelocatableHeapBlockBase(gAssetHandles.compressedAssetHandle), (s32)arg1 - (s32)arg0);
    sp30 = getRelocatableHeapBlockBase(gAssetHandles.compressedAssetHandle) + 5;
    sp28 = &gAssetHandles.assetHandles[arg2];
    decompressHuffmanAssetPayload(gCompressedAssetHeader.flags, sp30, getRelocatableHeapBlockBase(*sp28), gCompressedAssetHeader.compressedSize);
    getRelocatableHeapBlockBase(*sp28);
    gAssetHandles.compressedAssetHandle = freeRelocatableHeapBlock(gAssetHandles.compressedAssetHandle);
}

void loadRawRomAsset(void *arg0, void *arg1, s32 arg2) {
    s32 temp_a0 = (s32)arg1 - (s32)arg0;
    s16 *temp_v1;

    temp_v1 = &gAssetHandles.assetHandles[arg2];
    *temp_v1 = allocRelocatableHeapBlock(temp_a0);
    dmaReadRom((u32)arg0, (void *)getRelocatableHeapBlockBase(*temp_v1), temp_a0);
}
