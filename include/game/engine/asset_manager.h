#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "common.h"
#include "game/race/player/race_player_input.h"

#define ASSET_HANDLE_COUNT 0x40

typedef s16 AssetHandle;
typedef AssetHandle AssetHandles[ASSET_HANDLE_COUNT];

typedef struct {
    /* 0x0 */ s16 prev;
    /* 0x2 */ s16 next;
    /* 0x4 */ s16 weight;
    /* 0x6 */ s16 left;
    /* 0x8 */ s16 right;
    /* 0xA */ s16 value;
} HuffmanNode;

typedef struct {
    /* 0x0 */ s32 decodedSize; /* Output allocation and decompression length. */
    /* 0x4 */ u8 flags;
    /* 0x5 */ u8 pad5[3]; /* DMA read-ahead: encoded payload starts at +5, not +8. */
} CompressedAssetHeader;

extern AssetHandles gAssetHandles;

#if defined(VERSION_JP)
extern CompressedAssetHeader gCompressedAssetHeader;
extern s32 gHuffmanNodeCount;
extern HuffmanNode gHuffmanNodes[];
#endif

s32 randomNextMain(void);
s32 randomNextSecondary(void);
void resetSecondaryRng(void);
u8 randomNextObject(RacePlayer *player);
void resetGameplayRng(void);
void insertHuffmanQueueNode(s16 arg0);
void removeHuffmanQueueNode(s16 arg0);
void decompressHuffmanAssetPayload(u8 flags, u8 *compressedPayload, u8 *output, s32 outputSize);
void loadCompressedRomAsset(void *arg0, void *arg1, s32 arg2);
void loadRawRomAsset(void *arg0, void *arg1, s32 arg2);

#endif
