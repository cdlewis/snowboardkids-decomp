#include "common.h"

typedef struct Struct801235B8_99980 {
    char pad0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Struct801235B8_99980;

typedef struct Node99980 {
    struct Node99980 *prev;
    struct Node99980 *next;
    s32 unk8;
    char padC[8];
    u8 unk14;
    u8 unk15;
    s16 unk16;
} Node99980;

extern Struct801235B8_99980 *D_801235B8;
extern Node99980 *D_8012370C;

void func_80099464(s32);

#pragma GLOBAL_ASM("asm/nonmatchings/99980/func_80098D80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/99980/func_80098EAC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/99980/func_80099288.s")

#pragma GLOBAL_ASM("asm/nonmatchings/99980/func_80099384.s")

#pragma GLOBAL_ASM("asm/nonmatchings/99980/func_80099464.s")

#pragma GLOBAL_ASM("asm/nonmatchings/99980/func_800994F4.s")

void func_8009954C(s32 arg0) {
    func_80099464(arg0);
}

void func_8009956C(s32 arg0, s32 arg1) {
    switch (arg1) {
        case 0:
            D_801235B8->unk8 = arg0;
            return;
        case 1:
            D_801235B8->unkC = arg0;
            return;
        case 2:
            D_801235B8->unk10 = arg0;
            return;
    }
}

void func_800995C0(s32 arg0) {
    switch (arg0) {
        case 0:
            D_801235B8->unk8 = 0;
            return;
        case 1:
            D_801235B8->unkC = 0;
            return;
        case 2:
            D_801235B8->unk10 = 0;
            return;
    }
}

void func_80099614(s32 arg0) {
    Node99980 *node = D_8012370C;

    while (node != NULL) {
        if (arg0 == node->unk15) {
            node->unk16 = 1;
            return;
        }
        node = node->next;
    }
}

void func_80099658(s32 arg0) {
    Node99980 *node = D_8012370C;

    while (node != NULL) {
        if (arg0 == node->unk15) {
            node->unk16 = 2;
            return;
        }
        node = node->next;
    }
}
