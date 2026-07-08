#include "asset_decompression.h"
#include "common.h"
#include "memory_allocator.h"
#include "game_boot.h"

typedef struct {
    /* 0x0 */ s16 prev;
    /* 0x2 */ s16 next;
    /* 0x4 */ s16 weight;
    /* 0x6 */ s16 left;
    /* 0x8 */ s16 right;
    /* 0xA */ s16 value;
} HuffmanNode;

typedef struct {
    /* 0x0 */ s32 compressedSize;
    /* 0x4 */ u8 flags;
    /* 0x5 */ u8 pad5[3];
} CompressedAssetHeader;

typedef struct {
    /* 0x00 */ s16 assetHandles[0x20];
    /* 0x40 */ s16 compressedAssetHandle;
} AssetHandleTable;

struct RandomStateObject {
    u8 pad0[0x518];
    /* 0x518 */ u8 randomIndex;
};

extern u8 D_800D3F00[];
extern u16 D_800D4000;
extern u16 D_800D4004;
extern s16 D_800D4010;
extern s16 D_800D4014;
extern s16 D_800D4018;
extern s16 D_801235B0;
extern CompressedAssetHeader D_80110920;
extern HuffmanNode D_80110928[];
extern AssetHandleTable D_80112130;
extern s32 D_80112128;

s32 func_800430D0(void) {
    D_800D4000++;
    if (D_801235B0 == 0) {
        D_800D4000++;
    }
    D_800D4000 &= 0xFF;
    return D_800D3F00[D_800D4000];
}

s32 func_80043120(void) {
    D_800D4004++;
    D_800D4004 &= 0xFF;
    return D_800D3F00[D_800D4004];
}

void func_80043154(void) {
    D_800D4004 = 0;
}

u8 func_80043160(RandomStateObject *arg0) {
    arg0->randomIndex++;
    return D_800D3F00[arg0->randomIndex];
}

void func_80043184(void) {
    D_801235B0 = 0;
    D_800D4000 = 0;
    func_80043154();
}

// func_800431B0 best match: 86.433%
#pragma GLOBAL_ASM("asm/nonmatchings/asset_decompression/func_800431B0.s")

#ifdef NON_MATCHING
extern HuffmanNode D_8011092C[];

void func_800431B0(s16 arg0) {
    HuffmanNode *node;
    HuffmanNode *curNode;
    s16 cur;
    s16 oldHead;
    s16 next;
    s16 *count;
    s16 end = -1;

    count = &D_800D4018;
    cur = D_800D4010;
    *count += 1;
    if (cur == end) {
        oldHead = D_800D4014;
        D_800D4010 = arg0;
        D_800D4014 = arg0;
        node = &D_80110928[arg0];
        node->next = cur;
        node->prev = oldHead;
        return;
    }

    if (cur >= 0) {
        do {
            curNode = &D_80110928[cur];
            if (curNode->weight < D_8011092C[arg0].prev) {
                break;
            }
            cur = curNode->prev;
        } while (cur >= 0);
    }

    node = &D_80110928[arg0];
    if (cur == end) {
        next = D_800D4014;
        D_80110928[next].prev = arg0;
        node->prev = end;
        D_800D4014 = arg0;
        node->next = next;
        return;
    }

    node->prev = cur;
    curNode = &D_80110928[cur];
    node->next = curNode->next;
    curNode->next = arg0;
    next = node->next;
    if (end == next) {
        D_800D4010 = arg0;
        return;
    }
    D_80110928[next].prev = arg0;
}
#endif

void func_8004331C(s16 arg0) {
    HuffmanNode *node;
    s16 prev;
    s16 next;
    s16 end = -1;

    if (D_800D4010 != end) {
        D_800D4018--;
        node = &D_80110928[arg0];
        next = node->next;
        if (end != next) {
            D_80110928[next].prev = node->prev;
            prev = node->prev;
            next = node->next;
        } else {
            prev = node->prev;
            D_800D4010 = prev;
        }
        if (end != prev) {
            D_80110928[prev].next = next;
        } else {
            D_800D4014 = next;
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/asset_decompression/func_800433D8.s")

void func_800437F0(void *arg0, void *arg1, s32 arg2) {
    s16 *sp28;
    s32 sp30;

    func_80099C44((u32)arg0, &D_80110920, 8);
    D_80112130.assetHandles[arg2] = func_80042D58(D_80110920.compressedSize);
    D_80112130.compressedAssetHandle = func_80042D58((s32)arg1 - (s32)arg0);
    func_80099C44((u32)arg0, (void *)func_80043040(D_80112130.compressedAssetHandle), (s32)arg1 - (s32)arg0);
    sp30 = func_80043040(D_80112130.compressedAssetHandle) + 5;
    sp28 = &D_80112130.assetHandles[arg2];
    func_800433D8(D_80110920.flags, sp30, func_80043040(*sp28), D_80110920.compressedSize);
    func_80043040(*sp28);
    D_80112130.compressedAssetHandle = func_80042EE4(D_80112130.compressedAssetHandle);
}

void func_800438EC(void *arg0, void *arg1, s32 arg2) {
    s32 temp_a0 = (s32)arg1 - (s32)arg0;
    s16 *temp_v1;

    temp_v1 = &D_80112130.assetHandles[arg2];
    *temp_v1 = func_80042D58(temp_a0);
    func_80099C44((u32)arg0, (void *)func_80043040(*temp_v1), temp_a0);
}
