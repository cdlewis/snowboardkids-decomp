#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "common.h"

typedef struct RandomStateObject RandomStateObject;

s32 randomNextMain(void);
s32 randomNextSecondary(void);
void resetSecondaryRng(void);
u8 randomNextObject(RandomStateObject *arg0);
void resetGameplayRng(void);
void huffmanQueueInsert(s16 arg0);
void huffmanQueueRemove(s16 arg0);
void decompressAssetPayload(u8 arg0, s32 arg1, s32 arg2, s32 arg3);
void loadCompressedAsset(void *arg0, void *arg1, s32 arg2);
void loadRawAsset(void *arg0, void *arg1, s32 arg2);

#endif
