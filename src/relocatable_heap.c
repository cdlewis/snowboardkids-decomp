#include "common.h"
#include "relocatable_heap.h"

#define RELOCATABLE_HEAP_BLOCK_COUNT 0x50

typedef enum RelocatableHeapBlockStatus {
    RELOCATABLE_HEAP_BLOCK_FREE,
    RELOCATABLE_HEAP_BLOCK_USED,
    RELOCATABLE_HEAP_BLOCK_LOCKED
} RelocatableHeapBlockStatus;

typedef struct RelocatableHeapBlock {
    struct RelocatableHeapBlock *prev;
    struct RelocatableHeapBlock *next;
    u8 *start;
    s32 size;
    u8 index;
    u8 status;
} RelocatableHeapBlock; // size = 0x14

typedef struct RelocatableHeapBlockStartAlias {
    u8 *start;
    u8 pad[0x10];
} RelocatableHeapBlockStartAlias; // size = 0x14

extern RelocatableHeapBlockStartAlias gRelocatableHeapBlockStartFields[];
extern RelocatableHeapBlock *gFreeRelocatableHeapBlocks[];
extern u16 gAllocatedRelocatableHeapBlockCount;
extern u8 gRelocatableHeapStart;

extern RelocatableHeapBlock gRelocatableHeapBlocks[];
extern RelocatableHeapBlock *gFirstRelocatableHeapBlock;
extern u8 *gRelocatableHeapEnd;

void updateRelocatableHeapEnd(void) {
    RelocatableHeapBlock *node = gFirstRelocatableHeapBlock;
    RelocatableHeapBlock *next;

    if (node == NULL) {
        gRelocatableHeapEnd = &gRelocatableHeapStart;
        return;
    }
    next = node->next;
    if (next != NULL) {
        do {
            node = next;
            next = next->next;
        } while (next != NULL);
    }
    gRelocatableHeapEnd = node->start + node->size;
}

void updateRelocatableHeap(void) {
}

// initRelocatableHeap best match: 97.727%
#pragma GLOBAL_ASM("asm/nonmatchings/relocatable_heap/initRelocatableHeap.s")

#ifdef NON_MATCHING
extern RelocatableHeapBlock gRelocatableHeapBlocks1[];
extern RelocatableHeapBlock gRelocatableHeapBlocks2[];
extern RelocatableHeapBlock gRelocatableHeapBlocks3[];

void initRelocatableHeap(void) {
    RelocatableHeapBlock **freeList = gFreeRelocatableHeapBlocks;
    RelocatableHeapBlock *block0 = gRelocatableHeapBlocks;
    RelocatableHeapBlock *block1 = gRelocatableHeapBlocks1;
    RelocatableHeapBlock *block2 = gRelocatableHeapBlocks2;
    RelocatableHeapBlock *block3 = gRelocatableHeapBlocks3;
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
        block0[-3].status = RELOCATABLE_HEAP_BLOCK_FREE;
        block0[-2].status = RELOCATABLE_HEAP_BLOCK_FREE;
        block0[-1].status = RELOCATABLE_HEAP_BLOCK_FREE;
        freeList += 4;
        block0[-4].status = RELOCATABLE_HEAP_BLOCK_FREE;
    } while (i != RELOCATABLE_HEAP_BLOCK_COUNT);

    gAllocatedRelocatableHeapBlockCount = 0;
    gFirstRelocatableHeapBlock = NULL;
    updateRelocatableHeapEnd();
}
#endif

void *popFreeRelocatableHeapBlock(void) {
    RelocatableHeapBlock *block;

    if (gAllocatedRelocatableHeapBlockCount >= RELOCATABLE_HEAP_BLOCK_COUNT) {
        return NULL;
    }
    block = gFreeRelocatableHeapBlocks[gAllocatedRelocatableHeapBlockCount];
    block->status = RELOCATABLE_HEAP_BLOCK_USED;
    gAllocatedRelocatableHeapBlockCount++;
    return block;
}

void pushFreeRelocatableHeapBlock(RelocatableHeapBlock *block) {
    gAllocatedRelocatableHeapBlockCount = gAllocatedRelocatableHeapBlockCount - 1;
    gFreeRelocatableHeapBlocks[gAllocatedRelocatableHeapBlockCount] = block;
    block->status = RELOCATABLE_HEAP_BLOCK_FREE;
}

extern RelocatableHeapBlock gRelocatableHeapListHead;

s16 allocRelocatableHeapBlock(s32 size) {
    RelocatableHeapBlock *node;
    RelocatableHeapBlock *newBlock;
    u32 available;
    u32 alignedSize;

    alignedSize = ((u32)(size + 0xF) >> 4) * 0x10;

    /*
     * First try to allocate inside a gap between two existing blocks.
     * gRelocatableHeapListHead is the sentinel/list head.
     */
    node = gRelocatableHeapListHead.next;
    while (node != NULL) {
        if (node->next == NULL) {
            break;
        }

        available = (node->next->start - node->start) - node->size;
        if (available >= alignedSize) {
            newBlock = popFreeRelocatableHeapBlock();
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
    available = (gRelocatableHeapEnd + alignedSize) - &gRelocatableHeapStart;
    if (available > 0x1C0000) {
        return -1;
    }

    if (node == NULL) {
        node = &gRelocatableHeapListHead;
    }

    newBlock = popFreeRelocatableHeapBlock();
    if (newBlock == NULL) {
        return -1;
    }

    newBlock->prev = node;
    newBlock->next = node->next;

    if (node->next != NULL) {
        node->next->prev = newBlock;
    }

    node->next = newBlock;

    newBlock->start = gRelocatableHeapEnd;
    newBlock->size = alignedSize;

    updateRelocatableHeapEnd();

    return newBlock->index;
}

s32 freeRelocatableHeapBlock(s32 handle) {
    RelocatableHeapBlock *block;
    RelocatableHeapBlock *next;

    if (handle == -1) {
        return -1;
    }
    block = &gRelocatableHeapBlocks[handle];
    if (block->status != RELOCATABLE_HEAP_BLOCK_FREE) {
        next = block->next;
        if (next == NULL) {
            gRelocatableHeapEnd -= block->size;
            next = block->next;
        }
        block->prev->next = next;
        next = block->next;
        if (next != NULL) {
            next->prev = block->prev;
        }
        pushFreeRelocatableHeapBlock(block);
        updateRelocatableHeapEnd();
    }
    return -1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/relocatable_heap/compactRelocatableHeap.s")

s32 getRelocatableHeapBlockBase(s32 handle) {
    return (s32) gRelocatableHeapBlockStartFields[handle].start;
}

void lockRelocatableHeapBlock(s32 handle) {
    RelocatableHeapBlock *block = &gRelocatableHeapBlocks[handle];

    if (block->status != RELOCATABLE_HEAP_BLOCK_FREE) {
        block->status = RELOCATABLE_HEAP_BLOCK_LOCKED;
    }
}

void unlockRelocatableHeapBlock(s32 handle) {
    RelocatableHeapBlock *block = &gRelocatableHeapBlocks[handle];

    if (block->status != RELOCATABLE_HEAP_BLOCK_FREE) {
        block->status = RELOCATABLE_HEAP_BLOCK_USED;
    }
}
