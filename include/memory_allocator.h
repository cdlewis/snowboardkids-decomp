#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include "common.h"

/*
 * Fixed-block memory allocator public interface.
 *
 * func_80042C28 initialises the allocator, func_80042D58 allocates a block and
 * returns its handle, getMemoryBlockBase resolves a handle back to its base pointer,
 * func_80042EE4 frees a handle, func_80043060 locks a block, and func_80042C20
 * is the per-frame reset hook. Handles are small non-negative indices; -1
 * indicates "no block".
 */

void func_80042C20(void);
void func_80042C28(void);
s16 func_80042D58(s32 size);
s32 func_80042EE4(s32 arg0);
s32 getMemoryBlockBase(s32 arg0);
void func_80043060(s32 arg0);

#endif
