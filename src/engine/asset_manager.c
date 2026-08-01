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

// decompressHuffmanAssetPayload best match: 99.313% (nonmatchings/decompressHuffmanAssetPayload-7181144369148334388/base_42.c)
#pragma GLOBAL_ASM("asm/nonmatchings/engine/asset_manager/decompressHuffmanAssetPayload.s")

#ifdef NON_MATCHING
void decompressHuffmanAssetPayload(u8 arg0, u8 *arg1, u8 *arg2, s32 arg3) {
    s32 one;
    u8 **unusedArg1Address;
    HuffmanNode *node;
    HuffmanNode *parentNode;
    s32 symbol;
    s32 length;
    s32 bit;
    s32 nodeIndex;
    s32 rootNodeIndex;
    s32 bitIndex;
    s32 work;
    s32 tableBytesRead;
    s32 outputOffset;
    s32 nodeValue;
    s32 sourceOffset;
    u8 *input;
    u8 *source;
    s32 rangeStart;
    s32 rangeEnd;
    s32 leafNodeIndexCopy;
    u8 weight;
    s32 end;
    u8 copiedByte;
    s32 currentNodeIndex;
    u8 *destination;

    unusedArg1Address = &arg1;
    gHuffmanQueueHead = -1;
    gHuffmanQueueTail = -1;
    gHuffmanQueueCount = 0;
    gHuffmanNodeCount = 0;
    tableBytesRead = 0;
    input = arg1; end = -1; read_range: rangeStart = *input; tableBytesRead += 1; input += 1; if (tableBytesRead != 1) { if (rangeStart == 0) { goto build_tree; } }
    rangeEnd = *input;
    tableBytesRead += 1;
    bit = rangeEnd < rangeStart;
    input += 1;
    currentNodeIndex = rangeStart;
    if (bit == 0) {
        rangeStart = rangeEnd + 1;
        do {
            leafNodeIndexCopy = gHuffmanNodeCount;
            rootNodeIndex = leafNodeIndexCopy;
            weight = *input;
            tableBytesRead += 1;
            input += 1;
            node = &gHuffmanNodes[rootNodeIndex];
            node->left = end;
            node->right = end;
            node->value = (s16)currentNodeIndex;
            node->weight = (s16)weight;
            insertHuffmanQueueNode((s16)rootNodeIndex);
            currentNodeIndex += 1;
            gHuffmanNodeCount += 1;
        } while (rangeStart != currentNodeIndex);
    }
    goto read_range;

build_tree:
    if (gHuffmanQueueCount >= 2) {
        nodeIndex = gHuffmanQueueTail;
        rangeStart = nodeIndex;
        removeHuffmanQueueNode(rangeStart);
        nodeIndex = gHuffmanQueueTail;
        rangeEnd = nodeIndex;
        removeHuffmanQueueNode(rangeEnd);
        nodeIndex = gHuffmanNodeCount;
        parentNode = &gHuffmanNodes[nodeIndex];
        parentNode->weight = gHuffmanNodes[rangeEnd].weight + gHuffmanNodes[rangeStart].weight;
        parentNode->left = rangeEnd;
        parentNode->right = rangeStart;
        parentNode->value = end;
        insertHuffmanQueueNode((s16)gHuffmanNodeCount);
        gHuffmanNodeCount += 1;
        goto build_tree;
    }

    one = 1;
    bitIndex = 0;
    outputOffset = 0;
    if (arg0 == 0) {
        work = (s32)arg2;
        do {
            currentNodeIndex = gHuffmanNodeCount - 1;
decode_raw_symbol:
            nodeValue = gHuffmanNodes[currentNodeIndex].value;
            if (nodeValue != end) {
                symbol = nodeValue & 0xFF;
            } else {
                if (bitIndex == 8) {
                    input += 1;
                    bitIndex = 0;
                }
                bit = *input & (1 << (7 - bitIndex));
                bitIndex += 1;
                if (bit == 0) {
                    currentNodeIndex = gHuffmanNodes[currentNodeIndex].left;
                } else {
                    currentNodeIndex = gHuffmanNodes[currentNodeIndex].right;
                }
                goto decode_raw_symbol;
            }
            *(u8 *)work = (s8)symbol;
            outputOffset += 1;
            work += 1;
        } while (outputOffset != arg3);
    } else {
decode_pair:
        work = (rootNodeIndex = gHuffmanNodeCount - 1);
        currentNodeIndex = work;
decode_first_symbol:
        nodeValue = gHuffmanNodes[currentNodeIndex].value;
        symbol = nodeValue & 0xFF;
        if (nodeValue != end) {
            currentNodeIndex = work;
        } else {
            if (bitIndex == 8) {
                input += 1;
                bitIndex = 0;
            }
            bit = *input & (one << (7 - bitIndex));
            bitIndex += 1;
            bitIndex++;
            bitIndex--;
            if (bit == 0) {
                currentNodeIndex = gHuffmanNodes[currentNodeIndex].left;
            } else {
                currentNodeIndex = gHuffmanNodes[currentNodeIndex].right;
            }
            goto decode_first_symbol;
        }
decode_second_symbol:
        nodeValue = gHuffmanNodes[currentNodeIndex].value;
        if (nodeValue != end) {
            sourceOffset = nodeValue & 0xFF;
        } else {
            if (bitIndex == 8) {
                input += 1;
                bitIndex = 0;
            }
            bit = *input & (1 << (7 - bitIndex));
            bitIndex += 1;
            if (bit == 0) {
                currentNodeIndex = gHuffmanNodes[currentNodeIndex].left;
            } else {
                currentNodeIndex = gHuffmanNodes[currentNodeIndex].right;
            }
            goto decode_second_symbol;
        }
        nodeValue = sourceOffset;
        if (symbol == 0) {
            if (1) {
                arg2[outputOffset] = nodeValue & 0xFFFFFFFF;
                outputOffset += 1;
            }
        } else {
            length = symbol;
            length = (length >> 4) & 0xF;
            sourceOffset = outputOffset - ((((symbol << 3) << 5) | sourceOffset) & 0xFFF);
            nodeValue = 0;
            if (length > 0) {
                destination = arg2 + outputOffset;
                source = arg2 + ((0, sourceOffset));
                do {
                    copiedByte = *source;
                    nodeValue += 1;
                    outputOffset += 1;
                    destination += 1;
                    source += 1;
                    destination[end] = copiedByte & 0xFFu;
                } while (nodeValue < length);
            }
        }
        if (outputOffset < arg3) {
            goto decode_pair;
        }
    }
    if (!symbol) {
    }
}
#endif

void loadCompressedRomAsset(void *arg0, void *arg1, s32 arg2) {
    s16 *sp28;
    u8 *sp30;

    dmaReadRom((u32)arg0, &gCompressedAssetHeader, 8);
    gAssetHandles[arg2] = allocRelocatableHeapBlock(gCompressedAssetHeader.compressedSize);
    gAssetHandles[0x20] = allocRelocatableHeapBlock((s32)arg1 - (s32)arg0);
    dmaReadRom((u32)arg0, (void *)getRelocatableHeapBlockBase(gAssetHandles[0x20]), (s32)arg1 - (s32)arg0);
    sp30 = (u8 *)getRelocatableHeapBlockBase(gAssetHandles[0x20]) + 5;
    sp28 = &gAssetHandles[arg2];
    decompressHuffmanAssetPayload(gCompressedAssetHeader.flags, sp30, getRelocatableHeapBlockBase(*sp28),
                                  gCompressedAssetHeader.compressedSize);
    getRelocatableHeapBlockBase(*sp28);
    gAssetHandles[0x20] = freeRelocatableHeapBlock(gAssetHandles[0x20]);
}

void loadRawRomAsset(void *arg0, void *arg1, s32 arg2) {
    s32 temp_a0 = (s32)arg1 - (s32)arg0;
    s16 *temp_v1;

    temp_v1 = &gAssetHandles[arg2];
    *temp_v1 = allocRelocatableHeapBlock(temp_a0);
    dmaReadRom((u32)arg0, (void *)getRelocatableHeapBlockBase(*temp_v1), temp_a0);
}
