#ifndef ASSET_DECOMPRESSION_H
#define ASSET_DECOMPRESSION_H

#include "common.h"

typedef struct RandomStateObject RandomStateObject;

s32 randomNextMain(void);
s32 randomNextSecondary(void);
void resetSecondaryRng(void);
u8 randomNextObject(RandomStateObject *arg0);
void resetGameplayRng(void);
void func_800431B0(s16 arg0);
void func_8004331C(s16 arg0);
void func_800433D8(u8 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_800437F0(void *arg0, void *arg1, s32 arg2);
void func_800438EC(void *arg0, void *arg1, s32 arg2);

#endif
