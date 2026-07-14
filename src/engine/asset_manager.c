#include "game/engine/asset_manager.h"
#include "common.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/system_runtime.h"

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

void insertHuffmanQueueNode(s16 arg0) {
    HuffmanNode *iterNode;
    HuffmanNode *node;
    HuffmanNode *curNode;
    s32 head;
    s32 cur;
    s16 oldHead;
    s16 next;
    s16 *count;
    s16 newWeight;
    s16 tailCopy;
    unsigned int end = -1;

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

    head = arg0;
    if (cur >= 0) {
        do {
            curNode = &gHuffmanNodes[cur];
            newWeight = gHuffmanNodes[arg0].weight;
            iterNode = curNode;
            if (iterNode->weight < newWeight) {
                break;
            }
            cur = iterNode->prev;
        } while (cur >= 0);
    }

    node = &gHuffmanNodes[arg0];
    if (cur == end) {
        next = (tailCopy = gHuffmanQueueTail);
        gHuffmanNodes[next].prev = head;
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

// decompressHuffmanAssetPayload best match: 78.507% (nonmatchings/decompressHuffmanAssetPayload-6866765942504228165/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/engine/asset_manager/decompressHuffmanAssetPayload.s")

#ifdef NON_MATCHING
typedef struct {
    /* 0x0 */ u8 *byte;
    /* 0x4 */ s32 bitIndex;
} HuffmanBitReader;

static s32 readHuffmanBit(HuffmanBitReader *reader) {
    s32 bit;

    if (reader->bitIndex == 8) {
        reader->byte++;
        reader->bitIndex = 0;
    }

    bit = *reader->byte & (1 << (7 - reader->bitIndex));
    reader->bitIndex++;
    return bit;
}

static u8 readHuffmanSymbol(HuffmanBitReader *reader) {
    HuffmanNode *node;
    s16 nodeIndex;
    s16 value;

    nodeIndex = gHuffmanNodeCount - 1;
    while (TRUE) {
        node = &gHuffmanNodes[nodeIndex];
        value = node->value;
        if (value != -1) {
            return value & 0xFF;
        }

        if (readHuffmanBit(reader) == 0) {
            nodeIndex = node->left;
        } else {
            nodeIndex = node->right;
        }
    }
}

void decompressHuffmanAssetPayload(u8 flags, s32 compressedPayload, s32 outputBuffer, s32 outputSize) {
    HuffmanBitReader reader;
    HuffmanNode *node;
    s16 leftNode;
    s16 rightNode;
    s32 encodedLength;
    s32 firstValue;
    s32 lastValue;
    s32 nodeIndex;
    s32 outputOffset;
    s32 tableBytesRead;
    s32 value;
    s32 matchLength;
    s32 matchOffset;
    s32 i;
    u8 *dst;
    u8 *output;
    u8 *src;
    u8 *treeData;

    gHuffmanQueueHead = -1;
    gHuffmanQueueTail = -1;
    gHuffmanQueueCount = 0;
    gHuffmanNodeCount = 0;

    tableBytesRead = 0;
    treeData = (u8 *)compressedPayload;
    output = (u8 *)outputBuffer;
    while (TRUE) {
        firstValue = *treeData++;
        tableBytesRead++;
        if ((tableBytesRead != 1) && (firstValue == 0)) {
            break;
        }

        lastValue = *treeData++;
        tableBytesRead++;
        if (firstValue <= lastValue) {
            value = firstValue;
            do {
                nodeIndex = gHuffmanNodeCount;
                encodedLength = *treeData++;
                tableBytesRead++;

                node = &gHuffmanNodes[nodeIndex];
                node->left = -1;
                node->right = -1;
                node->value = value;
                node->weight = encodedLength;
                insertHuffmanQueueNode((s16) nodeIndex);
                value++;
                gHuffmanNodeCount += 1;
            } while ((lastValue + 1) != value);
        }
    }

    if (gHuffmanQueueCount >= 2) {
        do {
            rightNode = gHuffmanQueueTail;
            removeHuffmanQueueNode(rightNode);
            leftNode = gHuffmanQueueTail;
            removeHuffmanQueueNode(leftNode);

            nodeIndex = gHuffmanNodeCount;
            node = &gHuffmanNodes[nodeIndex];
            node->weight = gHuffmanNodes[rightNode].weight + gHuffmanNodes[leftNode].weight;
            node->left = leftNode;
            node->right = rightNode;
            node->value = -1;
            insertHuffmanQueueNode((s16) nodeIndex);
            gHuffmanNodeCount += 1;
        } while (gHuffmanQueueCount >= 2);
    }

    reader.byte = treeData;
    reader.bitIndex = 0;
    outputOffset = 0;

    if (flags == 0) {
        dst = output;
        do {
            *dst++ = readHuffmanSymbol(&reader);
            outputOffset++;
        } while (outputOffset != outputSize);
    } else {
        do {
            encodedLength = readHuffmanSymbol(&reader);
            value = readHuffmanSymbol(&reader);
            if (encodedLength == 0) {
                output[outputOffset] = value;
                outputOffset++;
            } else {
                matchLength = (encodedLength >> 4) & 0xF;
                matchOffset = ((encodedLength << 8) | value) & 0xFFF;
                if (matchLength > 0) {
                    dst = &output[outputOffset];
                    src = &output[outputOffset - matchOffset];
                    i = 0;
                    do {
                        *dst++ = *src++;
                        i++;
                        outputOffset++;
                    } while (i < matchLength);
                }
            }
        } while (outputOffset < outputSize);
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
