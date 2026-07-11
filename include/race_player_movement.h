#ifndef RACE_PLAYER_MOVEMENT_H
#define RACE_PLAYER_MOVEMENT_H

#include "common.h"

struct RaceInputPlayer;
struct RaceVec3i;

void func_80087600(s32 arg0, s32 *arg1, s32 *arg2);
void func_80087AFC(void);
void func_80087E14(struct RaceInputPlayer *player);
void func_80087EFC(void);
void func_80088294(struct RaceVec3i *pos, s32 xzSize, s32 ySize, u16 flag);
void func_80088664(struct RaceVec3i *pos, s32 xzSize, s32 ySize, u16 flag, s16 playerIndex);
void func_80088A1C(struct RaceVec3i *pos, s32 xzSize, s32 ySize, s32 arg3, s16 arg4);
void func_80088C80(struct RaceVec3i *pos, s32 xzSize, s32 ySize, s16 playerIndex);
s32 func_80088E98(struct RaceVec3i *pos, s32 xzSize, s32 ySize, s16 playerIndex);
void func_80089000(struct RaceVec3i *pos, s32 xzSize, s16 flag);
s32 func_800891B8(struct RaceVec3i *pos, s32 xzSize, s16 flag, s16 playerIndex);
s32 func_80089374(void);
void func_8008A940(struct RaceInputPlayer *player);
s32 func_8008B408(struct RaceInputPlayer *player, s32 arg1, s16 arg2);
void func_8008B508(struct RaceVec3i *vec, struct RaceInputPlayer *player);
void func_8008B60C(struct RaceVec3i *vec, struct RaceInputPlayer *player);
void func_8008B73C(struct RaceInputPlayer *player, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_8008BB20(struct RaceInputPlayer *player, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_8008BB5C(struct RaceInputPlayer *player, s32 arg1);
void enqueueRacePlayerVoiceSound(struct RaceInputPlayer *player, s16 soundType);
void func_8008BE1C(struct RaceInputPlayer *player);

#endif
