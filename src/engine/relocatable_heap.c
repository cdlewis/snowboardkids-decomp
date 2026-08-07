#include "common.h"
#include "game/engine/relocatable_heap.h"

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

extern RelocatableHeapBlockStartAlias gRelocatableHeapBlockStartAliases[];
extern RelocatableHeapBlock *gRelocatableHeapFreeBlockStack[];
extern u16 gRelocatableHeapUsedBlockCount;
extern u8 gRelocatableHeapStart[];

extern RelocatableHeapBlock gRelocatableHeapBlockPool[];
extern RelocatableHeapBlock *gFirstAllocatedRelocatableHeapBlock;
extern u8 *gRelocatableHeapNextFreeAddress;

void updateRelocatableHeapNextFreeAddress(void) {
    RelocatableHeapBlock *node = gFirstAllocatedRelocatableHeapBlock;
    RelocatableHeapBlock *next;

    if (node == NULL) {
        gRelocatableHeapNextFreeAddress = gRelocatableHeapStart;
        return;
    }
    next = node->next;
    if (next != NULL) {
        do {
            node = next;
            next = next->next;
        } while (next != NULL);
    }
    gRelocatableHeapNextFreeAddress = node->start + node->size;
}

void updateRelocatableHeap(void) {
}

void initRelocatableHeap(void) {
    s32 i;

    for (i = 0; i < RELOCATABLE_HEAP_BLOCK_COUNT; i++) {
        gRelocatableHeapFreeBlockStack[i] = &gRelocatableHeapBlockPool[i];
        gRelocatableHeapBlockPool[i].index = i;
        gRelocatableHeapBlockPool[i].status = RELOCATABLE_HEAP_BLOCK_FREE;
    }

    gRelocatableHeapUsedBlockCount = 0;
    gFirstAllocatedRelocatableHeapBlock = NULL;
    updateRelocatableHeapNextFreeAddress();
}

void *acquireRelocatableHeapBlockMetadata(void) {
    RelocatableHeapBlock *block;

    if (gRelocatableHeapUsedBlockCount >= RELOCATABLE_HEAP_BLOCK_COUNT) {
        return NULL;
    }
    block = gRelocatableHeapFreeBlockStack[gRelocatableHeapUsedBlockCount];
    block->status = RELOCATABLE_HEAP_BLOCK_USED;
    gRelocatableHeapUsedBlockCount++;
    return block;
}

void releaseRelocatableHeapBlockMetadata(RelocatableHeapBlock *block) {
    gRelocatableHeapUsedBlockCount = gRelocatableHeapUsedBlockCount - 1;
    gRelocatableHeapFreeBlockStack[gRelocatableHeapUsedBlockCount] = block;
    block->status = RELOCATABLE_HEAP_BLOCK_FREE;
}

extern RelocatableHeapBlock gRelocatableHeapBlockListHead;

s16 allocRelocatableHeapBlock(s32 size) {
    RelocatableHeapBlock *node;
    RelocatableHeapBlock *newBlock;
    u32 available;
    u32 alignedSize;

    alignedSize = ((u32)(size + 0xF) >> 4) * 0x10;

    /*
     * First try to allocate inside a gap between two existing blocks.
     * gRelocatableHeapBlockListHead is the sentinel/list head.
     */
    node = gRelocatableHeapBlockListHead.next;
    while (node != NULL) {
        if (node->next == NULL) {
            break;
        }

        available = (node->next->start - node->start) - node->size;
        if (available >= alignedSize) {
            newBlock = acquireRelocatableHeapBlockMetadata();
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
    available = (gRelocatableHeapNextFreeAddress + alignedSize) - gRelocatableHeapStart;
    if (available > 0x1C0000) {
        return -1;
    }

    if (node == NULL) {
        node = &gRelocatableHeapBlockListHead;
    }

    newBlock = acquireRelocatableHeapBlockMetadata();
    if (newBlock == NULL) {
        return -1;
    }

    newBlock->prev = node;
    newBlock->next = node->next;

    if (node->next != NULL) {
        node->next->prev = newBlock;
    }

    node->next = newBlock;

    newBlock->start = gRelocatableHeapNextFreeAddress;
    newBlock->size = alignedSize;

    updateRelocatableHeapNextFreeAddress();

    return newBlock->index;
}

s32 freeRelocatableHeapBlock(s32 handle) {
    RelocatableHeapBlock *block;
    RelocatableHeapBlock *next;

    if (handle == -1) {
        return -1;
    }
    block = &gRelocatableHeapBlockPool[handle];
    if (block->status != RELOCATABLE_HEAP_BLOCK_FREE) {
        next = block->next;
        if (next == NULL) {
            gRelocatableHeapNextFreeAddress -= block->size;
            next = block->next;
        }
        block->prev->next = next;
        next = block->next;
        if (next != NULL) {
            next->prev = block->prev;
        }
        releaseRelocatableHeapBlockMetadata(block);
        updateRelocatableHeapNextFreeAddress();
    }
    return -1;
}

void compactRelocatableHeap(void) {
    RelocatableHeapBlock *block;
    s32 size;
    u32 count;
    u8 *dest;
    u8 *nextFree;
    u8 *src;
    u32 blockStart;

    block = gFirstAllocatedRelocatableHeapBlock;
    nextFree = gRelocatableHeapStart;
    if (block != NULL) {
        do {
            blockStart = (u32)block->start;
            src = (u8 *)blockStart;
            count = blockStart - (u32)nextFree;
            blockStart = (u32)nextFree;
            if ((block->status != RELOCATABLE_HEAP_BLOCK_LOCKED) && (count != 0)) {
                count = 0;
                dest = nextFree;
                if (block->size != 0) {
                    do {
                        *dest = *src;
                        count++;
                        src++;
                        dest++;
                    } while (count < (u32)block->size);
                }
                block->start = (u8 *)blockStart;
                src = nextFree;
                if ((!dest) && (!dest)) {}
            }
            size = block->size & 0xFFFFFFFFFFFFFFFF;
            block = block->next;
            nextFree = src + size;
            if ((!blockStart) && (!blockStart)) {}
        } while (block != NULL);
    }
    updateRelocatableHeapNextFreeAddress();
}

void *getRelocatableHeapBlockBase(s32 handle) {
    return gRelocatableHeapBlockStartAliases[handle].start;
}

void lockRelocatableHeapBlock(s32 handle) {
    RelocatableHeapBlock *block = &gRelocatableHeapBlockPool[handle];

    if (block->status != RELOCATABLE_HEAP_BLOCK_FREE) {
        block->status = RELOCATABLE_HEAP_BLOCK_LOCKED;
    }
}

void unlockRelocatableHeapBlock(s32 handle) {
    RelocatableHeapBlock *block = &gRelocatableHeapBlockPool[handle];

    if (block->status != RELOCATABLE_HEAP_BLOCK_FREE) {
        block->status = RELOCATABLE_HEAP_BLOCK_USED;
    }
}
