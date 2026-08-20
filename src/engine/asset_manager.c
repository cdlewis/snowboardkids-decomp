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

u8 gRandomTable[256] = {
    0xD4, 0xF5, 0xDA, 0x93, 0x49, 0xA0, 0xD3, 0x64, 0x58, 0xC9, 0x31, 0x9A, 0x56, 0x1E, 0x90, 0x34, 0xC7, 0x17, 0x3E,
    0x13, 0x0E, 0x09, 0xE2, 0xDF, 0xF6, 0xA9, 0x3D, 0x59, 0x2A, 0x5D, 0x95, 0x08, 0xFF, 0x91, 0xA7, 0x30, 0x66, 0x27,
    0x52, 0xEE, 0xF4, 0x1A, 0xD9, 0xDB, 0x00, 0xA3, 0x99, 0xB5, 0x3F, 0xCB, 0x6C, 0xB7, 0x94, 0xB1, 0xDD, 0x25, 0x20,
    0x22, 0xE5, 0xFB, 0x9C, 0xCF, 0x0A, 0xB3, 0x1D, 0xED, 0xBC, 0x3B, 0x1F, 0x40, 0x78, 0xF2, 0x5B, 0x37, 0x92, 0x51,
    0xDE, 0x39, 0x8B, 0xBE, 0xE0, 0xAE, 0xAF, 0x18, 0x9B, 0x81, 0x70, 0xB8, 0x29, 0x7E, 0x32, 0x0F, 0x74, 0xD2, 0x1C,
    0xD1, 0xCA, 0x4B, 0xF8, 0x07, 0x55, 0x47, 0x61, 0x45, 0x4D, 0x10, 0xA6, 0xB6, 0xE4, 0x01, 0xC2, 0xA5, 0x54, 0x76,
    0x79, 0x65, 0x42, 0x82, 0x2B, 0xC5, 0xB4, 0x77, 0x16, 0x53, 0xA4, 0x48, 0xFC, 0x6E, 0x4C, 0x88, 0xE1, 0x26, 0x57,
    0xE9, 0x97, 0x0C, 0x87, 0x35, 0x36, 0xBD, 0x7F, 0x5A, 0x41, 0x2E, 0x05, 0xF9, 0x71, 0xEA, 0xFE, 0x68, 0xF1, 0x6D,
    0x7A, 0xAB, 0x96, 0xD6, 0x0D, 0x28, 0xAD, 0x23, 0x3C, 0xBF, 0xCD, 0xAA, 0xB0, 0x2D, 0x38, 0x89, 0xC0, 0xBA, 0xE3,
    0xEB, 0x83, 0x5C, 0x9E, 0xE7, 0x03, 0x9D, 0x14, 0x5F, 0xA8, 0x12, 0x6B, 0xCE, 0xC8, 0x73, 0x3A, 0xEC, 0xC6, 0xB2,
    0x80, 0xF3, 0xD5, 0xFD, 0x06, 0x46, 0xA1, 0x50, 0x43, 0x6F, 0x8D, 0x9F, 0xD7, 0xD8, 0x02, 0xC4, 0x2F, 0x04, 0x0B,
    0x8C, 0x69, 0x86, 0xD0, 0x33, 0x8E, 0x4F, 0x60, 0xFA, 0x24, 0x72, 0x4A, 0xC3, 0xE8, 0x21, 0x15, 0x1B, 0x85, 0x8F,
    0x67, 0xA2, 0x11, 0xE6, 0x2C, 0xDC, 0x63, 0x8A, 0x7C, 0xC1, 0x5E, 0xAC, 0x84, 0x75, 0x44, 0x7D, 0xF7, 0xBB, 0xEF,
    0xCC, 0x6A, 0x62, 0x4E, 0x19, 0x7B, 0xB9, 0x98, 0xF0,
};
u16 gMainRngIndex = 0;
u16 gSecondaryRngIndex = 0;
u32 D_800D4008[2] = { 0, 0 };
s16 gHuffmanQueueHead = -1;
s16 gHuffmanQueueTail = -1;
s16 gHuffmanQueueCount = 0;

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

void decompressHuffmanAssetPayload(u8 flags, u8 *compressedPayload, u8 *output, s32 outputSize) {
    u8 *input;
    u8 *outputCopy;
    s32 bit;
    s32 nodeIndex;
    s32 bitIndex;
    s32 firstCodeCopy;
    s32 tableBytesRead;
    s32 outputOffset;
    u8 new_var;
    s32 nodeValue;
    u8 sourceOffset;
    u8 symbol;
    s32 rangeStart;
    s32 rangeEnd;
    s32 end;
    s32 currentNodeIndex;
    u8 *destination;
    u8 *source;
    s32 var_t4;
    s32 temp;

    gHuffmanQueueHead = -1;
    gHuffmanQueueTail = -1;
    gHuffmanQueueCount = 0;
    gHuffmanNodeCount = 0;
    tableBytesRead = 0;
    input = compressedPayload;
    outputCopy = output;

    while (1) {
        rangeStart = input[tableBytesRead++];
        if ((tableBytesRead != 1) && (rangeStart == 0)) {
            break;
        }
        rangeEnd = input[tableBytesRead++];
        for (currentNodeIndex = rangeStart; currentNodeIndex <= rangeEnd; currentNodeIndex++) {
            gHuffmanNodes[gHuffmanNodeCount].weight = input[tableBytesRead++];
            gHuffmanNodes[gHuffmanNodeCount].left = -1;
            gHuffmanNodes[gHuffmanNodeCount].right = -1;
            gHuffmanNodes[gHuffmanNodeCount].value = currentNodeIndex;
            insertHuffmanQueueNode(gHuffmanNodeCount);
            gHuffmanNodeCount++;
        }
    }

    while (1) {
        if (gHuffmanQueueCount < 2) {
            break;
        }
        nodeIndex = gHuffmanQueueTail;
        rangeStart = nodeIndex;
        removeHuffmanQueueNode(rangeStart);
        nodeIndex = gHuffmanQueueTail;
        rangeEnd = nodeIndex;
        removeHuffmanQueueNode(rangeEnd);
        gHuffmanNodes[gHuffmanNodeCount].weight =
            gHuffmanNodes[rangeEnd].weight + gHuffmanNodes[rangeStart].weight;
        gHuffmanNodes[gHuffmanNodeCount].left = rangeEnd;
        gHuffmanNodes[gHuffmanNodeCount].right = rangeStart;
        gHuffmanNodes[gHuffmanNodeCount].value = -1;
        nodeIndex = gHuffmanNodeCount;
        insertHuffmanQueueNode(nodeIndex);
        gHuffmanNodeCount++;
    }

    bitIndex = 0;
    outputOffset = 0;
    if (flags == 0) {
        while (1) {
            currentNodeIndex = gHuffmanNodeCount - 1;
            while (1) {
                if (gHuffmanNodes[currentNodeIndex].value != -1) {
                    symbol = gHuffmanNodes[currentNodeIndex].value;
                    break;
                }
                if (bitIndex == 8) {
                    tableBytesRead++;
                    bitIndex = 0;
                }
                bit = input[tableBytesRead] & (1 << (7 - bitIndex));
                bitIndex++;
                if (bit == 0) {
                    currentNodeIndex = gHuffmanNodes[currentNodeIndex].left;
                } else {
                    currentNodeIndex = gHuffmanNodes[currentNodeIndex].right;
                }
            }

            output[outputOffset++] = symbol;
            if (outputOffset == outputSize) {
                break;
            }
        }
    } else {
        while (1) {
            currentNodeIndex = temp = gHuffmanNodeCount - 1;
            while (1) {
                symbol = gHuffmanNodes[currentNodeIndex].value;
                if (gHuffmanNodes[currentNodeIndex].value != -1) {
                    currentNodeIndex = temp;
                    break;
                }
                if (bitIndex == 8) {
                    tableBytesRead++;
                    bitIndex = 0;
                }
                bit = input[tableBytesRead] & (1 << (7 - bitIndex));
                bitIndex++;
                if (bit == 0) {
                    currentNodeIndex = gHuffmanNodes[currentNodeIndex].left;
                } else {
                    currentNodeIndex = gHuffmanNodes[currentNodeIndex].right;
                }
            }

            while (1) {
                if (gHuffmanNodes[currentNodeIndex].value != -1) {
                    sourceOffset = gHuffmanNodes[currentNodeIndex].value;
                    break;
                }
                if (bitIndex == 8) {
                    tableBytesRead++;
                    bitIndex = 0;
                }
                bit = input[tableBytesRead] & (1 << (7 - bitIndex));
                bitIndex++;
                if (bit == 0) {
                    currentNodeIndex = gHuffmanNodes[currentNodeIndex].left;
                } else {
                    currentNodeIndex = gHuffmanNodes[currentNodeIndex].right;
                }
            }

            if (symbol == 0) {
                outputCopy[outputOffset++] = sourceOffset;
            } else {
                firstCodeCopy = (symbol >> 4) & 0xF;
                var_t4 = outputOffset - (((symbol << 8) | sourceOffset) & 0xFFF);

                for (nodeValue = 0; nodeValue < firstCodeCopy;) {
                    outputCopy[outputOffset++] = output[var_t4 + nodeValue++];
                }
            }
            if (outputOffset >= outputSize) {
                break;
            }
        }
    }
}

void loadCompressedRomAsset(void *arg0, void *arg1, s32 arg2) {
    s16 *sp28;
    u8 *sp30;

    dmaReadRom((u32)arg0, &gCompressedAssetHeader, 8);
    gAssetHandles[arg2] = allocRelocatableHeapBlock(gCompressedAssetHeader.compressedSize);
    gAssetHandles[0x20] = allocRelocatableHeapBlock((s32)arg1 - (s32)arg0);
    dmaReadRom((u32)arg0, (void *)getRelocatableHeapBlockBase(gAssetHandles[0x20]), (s32)arg1 - (s32)arg0);
    sp30 = (u8 *)getRelocatableHeapBlockBase(gAssetHandles[0x20]) + 5;
    sp28 = &gAssetHandles[arg2];
    decompressHuffmanAssetPayload(
        gCompressedAssetHeader.flags,
        sp30,
        getRelocatableHeapBlockBase(*sp28),
        gCompressedAssetHeader.compressedSize
    );
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
