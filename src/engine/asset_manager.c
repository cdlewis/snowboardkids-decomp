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

// decompressHuffmanAssetPayload best match: 97.912% (nonmatchings/decompressHuffmanAssetPayload-8498672362023432715/base_22.c)
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

    nodeIndex = gHuffmanNodeCount - 1;
    while (TRUE) {
        node = &gHuffmanNodes[nodeIndex];
        if (node->value != -1) {
            return node->value & 0xFF;
        }

        if (readHuffmanBit(reader) == 0) {
            nodeIndex = node->left;
        } else {
            nodeIndex = node->right;
        }
    }
}

void decompressHuffmanAssetPayload(u8 flags, u8 *compressedPayload, u8 *output, s32 outputSize) {
    HuffmanBitReader reader;
    HuffmanNode *node;
    s16 leftNodeIndex;
    s16 rightNodeIndex;
    s32 rangeStart;
    s32 rangeEnd;
    s32 symbol;
    s32 nodeIndex;
    s32 tableBytesRead;
    s32 outputOffset;
    s32 backreferenceLength;
    s32 backreferenceOffset;
    s32 copied;
    u8 *dst;
    u8 *src;

    gHuffmanQueueHead = -1;
    gHuffmanQueueTail = -1;
    gHuffmanQueueCount = 0;
    gHuffmanNodeCount = 0;

    tableBytesRead = 0;
    reader.byte = compressedPayload;
    while (TRUE) {
        rangeStart = *reader.byte++;
        tableBytesRead++;
        if ((tableBytesRead != 1) && (rangeStart == 0)) {
            break;
        }

        rangeEnd = *reader.byte++;
        tableBytesRead++;
        if (rangeStart <= rangeEnd) {
            symbol = rangeStart;
            do {
                nodeIndex = gHuffmanNodeCount;
                node = &gHuffmanNodes[nodeIndex];
                node->left = -1;
                node->right = -1;
                node->value = symbol;
                node->weight = *reader.byte++;
                tableBytesRead++;

                insertHuffmanQueueNode((s16)nodeIndex);
                symbol++;
                gHuffmanNodeCount++;
            } while (symbol != rangeEnd + 1);
        }
    }

    while (gHuffmanQueueCount >= 2) {
        rightNodeIndex = gHuffmanQueueTail;
        removeHuffmanQueueNode(rightNodeIndex);
        leftNodeIndex = gHuffmanQueueTail;
        removeHuffmanQueueNode(leftNodeIndex);

        nodeIndex = gHuffmanNodeCount;
        node = &gHuffmanNodes[nodeIndex];
        node->weight = gHuffmanNodes[rightNodeIndex].weight + gHuffmanNodes[leftNodeIndex].weight;
        node->left = leftNodeIndex;
        node->right = rightNodeIndex;
        node->value = -1;

        insertHuffmanQueueNode((s16)nodeIndex);
        gHuffmanNodeCount++;
    }

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
            backreferenceLength = readHuffmanSymbol(&reader);
            symbol = readHuffmanSymbol(&reader);
            if (backreferenceLength == 0) {
                output[outputOffset++] = symbol;
            } else {
                backreferenceOffset = ((backreferenceLength << 8) | symbol) & 0xFFF;
                backreferenceLength = (backreferenceLength >> 4) & 0xF;
                copied = 0;
                if (backreferenceLength > 0) {
                    dst = &output[outputOffset];
                    src = &output[outputOffset - backreferenceOffset];
                    do {
                        *dst++ = *src++;
                        copied++;
                        outputOffset++;
                    } while (copied < backreferenceLength);
                }
            }
        } while (outputOffset < outputSize);
    }
}
#endif

void loadCompressedRomAsset(void *arg0, void *arg1, s32 arg2) {
    s16 *sp28;
    u8 *sp30;

    dmaReadRom((u32)arg0, &gCompressedAssetHeader, 8);
    gAssetHandles.assetHandles[arg2] = allocRelocatableHeapBlock(gCompressedAssetHeader.compressedSize);
    gAssetHandles.compressedAssetHandle = allocRelocatableHeapBlock((s32)arg1 - (s32)arg0);
    dmaReadRom((u32)arg0, (void *)getRelocatableHeapBlockBase(gAssetHandles.compressedAssetHandle), (s32)arg1 - (s32)arg0);
    sp30 = (u8 *)getRelocatableHeapBlockBase(gAssetHandles.compressedAssetHandle) + 5;
    sp28 = &gAssetHandles.assetHandles[arg2];
    decompressHuffmanAssetPayload(gCompressedAssetHeader.flags, sp30, getRelocatableHeapBlockBase(*sp28),
                                  gCompressedAssetHeader.compressedSize);
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
