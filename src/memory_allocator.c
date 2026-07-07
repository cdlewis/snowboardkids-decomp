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

// func_80042C28 best match: 97.727%
#pragma GLOBAL_ASM("asm/nonmatchings/memory_allocator/func_80042C28.s")

#ifdef NON_MATCHING
extern MemoryBlock D_801101AC[];
extern MemoryBlock D_801101C0[];
extern MemoryBlock D_801101D4[];

void func_80042C28(void) {
    MemoryBlock **freeList = D_801107D8;
    MemoryBlock *block0 = D_80110198;
    MemoryBlock *block1 = D_801101AC;
    MemoryBlock *block2 = D_801101C0;
    MemoryBlock *block3 = D_801101D4;
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

    D_80110918 = 0;
    D_80110184 = NULL;
    func_80042BC0();
}
#endif

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

// func_80042D58 best match: 99.898%
#pragma GLOBAL_ASM("asm/nonmatchings/memory_allocator/func_80042D58.s")

#ifdef NON_MATCHING
extern MemoryBlock D_80110180;

s16 func_80042D58(s32 arg0) {
    MemoryBlock *temp_v1;
    u32 temp_a0;
    MemoryBlock *var_a1;
    MemoryBlock *temp_v0;
    u32 temp_t1;
    u32 available;

    var_a1 = D_80110180.next;
    if (var_a1 != NULL) {
loop_1:
        temp_v1 = var_a1->next;
        if (temp_v1 != NULL) {
            temp_a0 = arg0 + 0xF;
            temp_t1 = (temp_a0 >> 4) * 0x10;
            available = (temp_v1->start - var_a1->start) - var_a1->size;
            if (available >= temp_t1) {
                temp_v0 = func_80042CDC();
                if (temp_v0 == NULL) {
                    return -1;
                }
                temp_v0->prev = var_a1;
                temp_v0->next = var_a1->next;
                temp_v1 = var_a1->next;
                if (temp_v1 != NULL) {
                    temp_v1->prev = temp_v0;
                }
                var_a1->next = temp_v0;
                temp_v0->start = var_a1->start + var_a1->size;
                temp_v0->size = temp_t1;
                return temp_v0->index;
            }
            var_a1 = temp_v1;
            if (temp_v1 != NULL) {
                goto loop_1;
            }
        }
    }

    temp_a0 = ((u32)(arg0 + 0xF) >> 4) * 0x10;
    temp_t1 = (u32)((D_8011091C + temp_a0) - &D_80160480);
    if (temp_t1 >= 0x1C0001U) {
        return -1;
    }
    if (var_a1 == NULL) {
        var_a1 = &D_80110180;
    }
    temp_v0 = func_80042CDC();
    if (temp_v0 == NULL) {
        return -1;
    }
    temp_v0->prev = var_a1;
    temp_v0->next = var_a1->next;
    temp_v1 = var_a1->next;
    if (temp_v1 != NULL) {
        temp_v1->prev = temp_v0;
    }
    var_a1->next = temp_v0;
    temp_v0->start = D_8011091C;
    temp_v0->size = temp_a0;
    func_80042BC0();
    return temp_v0->index;
}
#endif

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
