#include "common.h"
#include "memory_block_allocator.h"

#define MEMORY_BLOCK_COUNT 0x50

typedef enum MemoryBlockStatus {
    MEMORY_BLOCK_FREE,
    MEMORY_BLOCK_USED,
    MEMORY_BLOCK_LOCKED
} MemoryBlockStatus;

typedef struct MemoryBlock {
    struct MemoryBlock *prev;
    struct MemoryBlock *next;
    u8 *start;
    s32 size;
    u8 index;
    u8 status;
} MemoryBlock; // size = 0x14

typedef struct MemoryBlockStartAlias {
    u8 *start;
    u8 pad[0x10];
} MemoryBlockStartAlias; // size = 0x14

extern MemoryBlockStartAlias gMemoryBlockStartFields[];
extern MemoryBlock *gFreeMemoryBlocks[];
extern u16 gUsedMemoryBlockCount;
extern u8 gMemoryBlockHeapStart;

extern MemoryBlock gMemoryBlocks[];
extern MemoryBlock *gFirstMemoryBlock;
extern u8 *gMemoryBlockHeapEnd;

void updateMemoryBlockHeapEnd(void) {
    MemoryBlock *node = gFirstMemoryBlock;
    MemoryBlock *next;

    if (node == NULL) {
        gMemoryBlockHeapEnd = &gMemoryBlockHeapStart;
        return;
    }
    next = node->next;
    if (next != NULL) {
        do {
            node = next;
            next = next->next;
        } while (next != NULL);
    }
    gMemoryBlockHeapEnd = node->start + node->size;
}

void updateMemoryBlockAllocator(void) {
}

// initMemoryBlockAllocator best match: 97.727%
#pragma GLOBAL_ASM("asm/nonmatchings/memory_block_allocator/initMemoryBlockAllocator.s")

#ifdef NON_MATCHING
extern MemoryBlock gMemoryBlocks1[];
extern MemoryBlock gMemoryBlocks2[];
extern MemoryBlock gMemoryBlocks3[];

void initMemoryBlockAllocator(void) {
    MemoryBlock **freeList = gFreeMemoryBlocks;
    MemoryBlock *block0 = gMemoryBlocks;
    MemoryBlock *block1 = gMemoryBlocks1;
    MemoryBlock *block2 = gMemoryBlocks2;
    MemoryBlock *block3 = gMemoryBlocks3;
    s32 i = 0;
    s32 next1;
    s32 next2;
    s32 next3;

    do {
        next1 = i + 1;
        next2 = i + 2;
        next3 = i + 3;
        block0->index = i;
        i += 4;
        if ((i && i) && i) {
        }
        freeList[1] = block1;
        freeList[2] = block2;
        freeList[3] = block3;
        freeList[0] = block0;
        block0 += 4;
        block3 += 4;
        block2 += 4;
        block1 += 4;
        block0[-1].index = next3;
        block0[-2].index = next2;
        block0[-3].index = next1;
        block0[-3].status = MEMORY_BLOCK_FREE;
        block0[-2].status = MEMORY_BLOCK_FREE;
        block0[-1].status = MEMORY_BLOCK_FREE;
        freeList += 4;
        block0[-4].status = MEMORY_BLOCK_FREE;
    } while (i != MEMORY_BLOCK_COUNT);

    gUsedMemoryBlockCount = 0;
    gFirstMemoryBlock = NULL;
    updateMemoryBlockHeapEnd();
}
#endif

void *popFreeMemoryBlock(void) {
    MemoryBlock *block;

    if (gUsedMemoryBlockCount >= MEMORY_BLOCK_COUNT) {
        return NULL;
    }
    block = gFreeMemoryBlocks[gUsedMemoryBlockCount];
    block->status = MEMORY_BLOCK_USED;
    gUsedMemoryBlockCount++;
    return block;
}

void pushFreeMemoryBlock(MemoryBlock *block) {
    gUsedMemoryBlockCount = gUsedMemoryBlockCount - 1;
    gFreeMemoryBlocks[gUsedMemoryBlockCount] = block;
    block->status = MEMORY_BLOCK_FREE;
}

extern MemoryBlock gMemoryBlockListHead;

s16 allocMemoryBlock(s32 size) {
    MemoryBlock *node;
    MemoryBlock *newBlock;
    u32 available;
    u32 alignedSize;

    alignedSize = ((u32)(size + 0xF) >> 4) * 0x10;

    /*
     * First try to allocate inside a gap between two existing blocks.
     * gMemoryBlockListHead is the sentinel/list head.
     */
    node = gMemoryBlockListHead.next;
    while (node != NULL) {
        if (node->next == NULL) {
            break;
        }

        available = (node->next->start - node->start) - node->size;
        if (available >= alignedSize) {
            newBlock = popFreeMemoryBlock();
            if (newBlock == NULL) {
                return -1;
            }

            newBlock->prev = node;
            newBlock->next = node->next;
            if (node->next != NULL) {
                node->next->prev = newBlock;
            }
            node->next = newBlock;

            newBlock->start = node->start + node->size;
            newBlock->size = alignedSize;

            return newBlock->index;
        }

        node = node->next;
    }

    /* No suitable gap was found, so append at the current heap end. */
    available = (gMemoryBlockHeapEnd + alignedSize) - &gMemoryBlockHeapStart;
    if (available > 0x1C0000) {
        return -1;
    }

    if (node == NULL) {
        node = &gMemoryBlockListHead;
    }

    newBlock = popFreeMemoryBlock();
    if (newBlock == NULL) {
        return -1;
    }

    newBlock->prev = node;
    newBlock->next = node->next;

    if (node->next != NULL) {
        node->next->prev = newBlock;
    }

    node->next = newBlock;

    newBlock->start = gMemoryBlockHeapEnd;
    newBlock->size = alignedSize;

    updateMemoryBlockHeapEnd();

    return newBlock->index;
}

s32 freeMemoryBlock(s32 handle) {
    MemoryBlock *block;
    MemoryBlock *next;

    if (handle == -1) {
        return -1;
    }
    block = &gMemoryBlocks[handle];
    if (block->status != MEMORY_BLOCK_FREE) {
        next = block->next;
        if (next == NULL) {
            gMemoryBlockHeapEnd -= block->size;
            next = block->next;
        }
        block->prev->next = next;
        next = block->next;
        if (next != NULL) {
            next->prev = block->prev;
        }
        pushFreeMemoryBlock(block);
        updateMemoryBlockHeapEnd();
    }
    return -1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/memory_block_allocator/compactMemoryBlocks.s")

s32 getMemoryBlockBase(s32 handle) {
    return (s32) gMemoryBlockStartFields[handle].start;
}

void lockMemoryBlock(s32 handle) {
    MemoryBlock *block = &gMemoryBlocks[handle];

    if (block->status != MEMORY_BLOCK_FREE) {
        block->status = MEMORY_BLOCK_LOCKED;
    }
}

void unlockMemoryBlock(s32 handle) {
    MemoryBlock *block = &gMemoryBlocks[handle];

    if (block->status != MEMORY_BLOCK_FREE) {
        block->status = MEMORY_BLOCK_USED;
    }
}
