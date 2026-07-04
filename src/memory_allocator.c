#include "common.h"

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

extern MemoryBlockStartAlias D_801101A0[];
extern MemoryBlock *D_801107D8[];
extern u16 D_80110918;
extern u8 D_80160480;

extern MemoryBlock D_80110198[];
extern MemoryBlock *D_80110184;
extern u8 *D_8011091C;

void func_80042BC0(void) {
    MemoryBlock *node = D_80110184;
    MemoryBlock *next;

    if (node == NULL) {
        D_8011091C = &D_80160480;
        return;
    }
    next = node->next;
    if (next != NULL) {
        do {
            node = next;
            next = next->next;
        } while (next != NULL);
    }
    D_8011091C = node->start + node->size;
}

void func_80042C20(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/memory_allocator/func_80042C28.s")

void *func_80042CDC(void) {
    MemoryBlock *block;

    if (D_80110918 >= MEMORY_BLOCK_COUNT) {
        return NULL;
    }
    block = D_801107D8[D_80110918];
    block->status = MEMORY_BLOCK_USED;
    D_80110918++;
    return block;
}

void func_80042D28(MemoryBlock *block) {
    D_80110918 = D_80110918 - 1;
    D_801107D8[D_80110918] = block;
    block->status = MEMORY_BLOCK_FREE;
}

#pragma GLOBAL_ASM("asm/nonmatchings/memory_allocator/func_80042D58.s")

s32 func_80042EE4(s32 arg0) {
    MemoryBlock *temp_a0;
    MemoryBlock *temp_v0;

    if (arg0 == -1) {
        return -1;
    }
    temp_a0 = &D_80110198[arg0];
    if (temp_a0->status != MEMORY_BLOCK_FREE) {
        temp_v0 = temp_a0->next;
        if (temp_v0 == NULL) {
            D_8011091C -= temp_a0->size;
            temp_v0 = temp_a0->next;
        }
        temp_a0->prev->next = temp_v0;
        temp_v0 = temp_a0->next;
        if (temp_v0 != NULL) {
            temp_v0->prev = temp_a0->prev;
        }
        func_80042D28(temp_a0);
        func_80042BC0();
    }
    return -1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/memory_allocator/func_80042FA0.s")

s32 func_80043040(s32 arg0) {
    return (s32) D_801101A0[arg0].start;
}

void func_80043060(s32 arg0) {
    MemoryBlock *block = &D_80110198[arg0];

    if (block->status != MEMORY_BLOCK_FREE) {
        block->status = MEMORY_BLOCK_LOCKED;
    }
}

void func_80043094(s32 arg0) {
    MemoryBlock *block = &D_80110198[arg0];

    if (block->status != MEMORY_BLOCK_FREE) {
        block->status = MEMORY_BLOCK_USED;
    }
}
