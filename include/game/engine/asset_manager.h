#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "common.h"

typedef struct RandomStateObject RandomStateObject;

extern s16 gAssetHandles[0x40];

s32 randomNextMain(void);
s32 randomNextSecondary(void);
void resetSecondaryRng(void);
u8 randomNextObject(RandomStateObject *arg0);
void resetGameplayRng(void);
void insertHuffmanQueueNode(s16 arg0);
void removeHuffmanQueueNode(s16 arg0);
void decompressHuffmanAssetPayload(u8 flags, u8 *compressedPayload, u8 *output, s32 outputSize);
void loadCompressedRomAsset(void *arg0, void *arg1, s32 arg2);
void loadRawRomAsset(void *arg0, void *arg1, s32 arg2);

#endif
