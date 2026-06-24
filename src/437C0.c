#include "common.h"

extern s32 D_801101A0[][5];
extern void *D_801107D8[];
extern u16 D_80110918;
extern u8 D_80110198[];
extern u8 D_80160480;

typedef struct ListNode437C0 {
    struct ListNode437C0 *prev;
    struct ListNode437C0 *next;
    u8 *start;
    s32 size;
    u8 index;
    u8 used;
} ListNode437C0;

extern ListNode437C0 *D_80110184;
extern u8 *D_8011091C;

void func_80042BC0(void) {
    ListNode437C0 *node = D_80110184;
    ListNode437C0 *next;

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

#pragma GLOBAL_ASM("asm/nonmatchings/437C0/func_80042C28.s")

void *func_80042CDC(void) {
    void *temp_v0;

    if (D_80110918 >= 0x50) {
        return NULL;
    }
    temp_v0 = D_801107D8[D_80110918];
    *(s8 *)((s32)temp_v0 + 0x11) = 1;
    D_80110918++;
    return temp_v0;
}

void func_80042D28(void *arg0) {
    D_80110918 = D_80110918 - 1;
    D_801107D8[D_80110918] = arg0;
    *(s8 *)((s32)arg0 + 0x11) = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/437C0/func_80042D58.s")

s32 func_80042EE4(s32 arg0) {
    ListNode437C0 *temp_a0;
    ListNode437C0 *temp_v0;

    if (arg0 == -1) {
        return -1;
    }
    temp_a0 = (ListNode437C0 *)&D_80110198[arg0 * sizeof(ListNode437C0)];
    if (temp_a0->used != 0) {
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

#pragma GLOBAL_ASM("asm/nonmatchings/437C0/func_80042FA0.s")

s32 func_80043040(s32 arg0) {
    return D_801101A0[arg0][0];
}

void func_80043060(s32 arg0) {
    u8 *temp = &D_80110198[arg0 * 0x14];

    if (temp[0x11] != 0) {
        temp[0x11] = 2;
    }
}

void func_80043094(s32 arg0) {
    u8 *temp = &D_80110198[arg0 * 0x14];

    if (temp[0x11] != 0) {
        temp[0x11] = 1;
    }
}
