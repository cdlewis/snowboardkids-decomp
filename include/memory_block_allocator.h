#ifndef MEMORY_BLOCK_ALLOCATOR_H
#define MEMORY_BLOCK_ALLOCATOR_H

#include "common.h"

/*
 * Relocatable memory block allocator public interface.
 *
 * initMemoryBlockAllocator initialises the allocator, allocMemoryBlock allocates a block and
 * returns its handle, getMemoryBlockBase resolves a handle back to its base pointer,
 * freeMemoryBlock frees a handle, lockMemoryBlock locks a block, and updateMemoryBlockAllocator
 * is the per-frame reset hook. Handles are small non-negative indices; -1
 * indicates "no block".
 */

void updateMemoryBlockAllocator(void);
void initMemoryBlockAllocator(void);
s16 allocMemoryBlock(s32 size);
s32 freeMemoryBlock(s32 handle);
s32 getMemoryBlockBase(s32 handle);
void lockMemoryBlock(s32 handle);

#endif
