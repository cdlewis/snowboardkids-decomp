#ifndef RELOCATABLE_HEAP_H
#define RELOCATABLE_HEAP_H

#include "common.h"

/*
 * Relocatable heap public interface.
 *
 * Blocks are addressed by small non-negative handles. A handle can be resolved
 * to its current base address, but unlocked blocks may move when the heap is
 * compacted. A handle of -1 indicates "no block".
 */

typedef struct RelocatableHeapBlock {
    struct RelocatableHeapBlock *prev;
    struct RelocatableHeapBlock *next;
    u8 *start;
    s32 size;
    u8 index;
    u8 status;
} RelocatableHeapBlock;

extern RelocatableHeapBlock gRelocatableHeapBlockListHead;
extern RelocatableHeapBlock gRelocatableHeapBlockPool[];
extern RelocatableHeapBlock *gRelocatableHeapFreeBlockStack[];
extern u8 *gRelocatableHeapNextFreeAddress;
extern u16 gRelocatableHeapUsedBlockCount;

void updateRelocatableHeap(void);
void initRelocatableHeap(void);
s16 allocRelocatableHeapBlock(s32 size);
s32 freeRelocatableHeapBlock(s32 handle);
void *getRelocatableHeapBlockBase(s32 handle);
void lockRelocatableHeapBlock(s32 handle);

#endif
