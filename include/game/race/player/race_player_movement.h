#ifndef RACE_PLAYER_MOVEMENT_H
#define RACE_PLAYER_MOVEMENT_H

#include "common.h"

struct RacePlayer;
struct RaceVec3i;

void getRacePlayerRankingProgress(s32 arg0, s32 *arg1, s32 *arg2);
void updateRacePlayerRankings(void);
void updateRacePlayerFinalLapStatus(struct RacePlayer *player);
void resolveRacePlayerBodyCollisions(void);
void pushRacePlayersOutOfCylinderAndApplyItemHit(struct RaceVec3i *pos, s32 xzSize, s32 ySize, u16 flag);
void pushRacePlayerOutOfCylinderAndApplyItemHit(struct RaceVec3i *pos, s32 xzSize, s32 ySize, u16 flag, s16 playerIndex);
void pushRacePlayersOutOfCylinderOrApplyItemHit(struct RaceVec3i *pos, s32 xzSize, s32 ySize, s32 arg3, s16 arg4);
void pushRacePlayerOutOfCylinder(struct RaceVec3i *pos, s32 xzSize, s32 ySize, s16 playerIndex);
s32 isRacePlayerInsideCylinder(struct RaceVec3i *pos, s32 xzSize, s32 ySize, s16 playerIndex);
void applyItemHitToRacePlayersInsideSphere(struct RaceVec3i *pos, s32 xzSize, s16 flag);
s32 tryApplyRacePlayerItemHit(struct RaceVec3i *pos, s32 xzSize, s16 flag, s16 playerIndex);
s32 updateRacePlayerSurfaceContact(void);
s32 updateRacePlayerGroundAlignment(struct RacePlayer *player);
s32 updateRacePlayerLeanAngle(struct RacePlayer *player, s32 arg1, s16 arg2);
void clampRacePlayerVectorXZSpeed(struct RaceVec3i *vec, struct RacePlayer *player);
void clampRacePlayerVectorXZHalfSpeed(struct RaceVec3i *vec, struct RacePlayer *player);
void updateRacePlayerLocalVelocity(struct RacePlayer *player, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void updateRacePlayerLocalVelocityNoVerticalOffset(struct RacePlayer *player, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void addRacePlayerScore(struct RacePlayer *player, s32 arg1);
void enqueueRacePlayerVoiceSound(struct RacePlayer *player, s16 soundType);
void updateRacePlayerProjectedPosition(struct RacePlayer *player);

#endif
