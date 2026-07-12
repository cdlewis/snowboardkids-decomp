#ifndef RACE_PLAYER_MOVEMENT_H
#define RACE_PLAYER_MOVEMENT_H

#include "common.h"

struct RaceInputPlayer;
struct RaceVec3i;

void getRacePlayerRankingProgress(s32 arg0, s32 *arg1, s32 *arg2);
void updateRacePlayerRankings(void);
void updateRacePlayerFinalLapStatus(struct RaceInputPlayer *player);
void resolveRacePlayerBodyCollisions(void);
void pushRacePlayersOutOfCylinderAndApplyItemHit(struct RaceVec3i *pos, s32 xzSize, s32 ySize, u16 flag);
void pushRacePlayerOutOfCylinderAndApplyItemHit(struct RaceVec3i *pos, s32 xzSize, s32 ySize, u16 flag, s16 playerIndex);
void pushRacePlayersOutOfCylinderOrApplyItemHit(struct RaceVec3i *pos, s32 xzSize, s32 ySize, s32 arg3, s16 arg4);
void pushRacePlayerOutOfCylinder(struct RaceVec3i *pos, s32 xzSize, s32 ySize, s16 playerIndex);
s32 isRacePlayerInsideCylinder(struct RaceVec3i *pos, s32 xzSize, s32 ySize, s16 playerIndex);
void applyItemHitToRacePlayersInsideSphere(struct RaceVec3i *pos, s32 xzSize, s16 flag);
s32 tryApplyRacePlayerItemHit(struct RaceVec3i *pos, s32 xzSize, s16 flag, s16 playerIndex);
s32 updateRacePlayerSurfaceContact(void);
void updateRacePlayerGroundAlignment(struct RaceInputPlayer *player);
s32 updateRacePlayerLeanAngle(struct RaceInputPlayer *player, s32 arg1, s16 arg2);
void clampRacePlayerVectorXZSpeed(struct RaceVec3i *vec, struct RaceInputPlayer *player);
void clampRacePlayerVectorXZHalfSpeed(struct RaceVec3i *vec, struct RaceInputPlayer *player);
void updateRacePlayerLocalVelocity(struct RaceInputPlayer *player, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void updateRacePlayerLocalVelocityNoVerticalOffset(struct RaceInputPlayer *player, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void addRacePlayerScore(struct RaceInputPlayer *player, s32 arg1);
void enqueueRacePlayerVoiceSound(struct RaceInputPlayer *player, s16 soundType);
void updateRacePlayerProjectedPosition(struct RaceInputPlayer *player);

#endif
