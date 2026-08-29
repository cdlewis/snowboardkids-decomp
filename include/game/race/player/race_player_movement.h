#ifndef RACE_PLAYER_MOVEMENT_H
#define RACE_PLAYER_MOVEMENT_H

#include "game/race/player/race_player_input.h"

void getRacePlayerRankingProgress(s32 arg0, s32 *arg1, s32 *arg2);
void updateRacePlayerRankings(void);
void updateRacePlayerFinalLapStatus(RacePlayer *player);
void resolveRacePlayerBodyCollisions(void);
void pushRacePlayersOutOfCylinderAndApplyItemHit(Vec3i *pos, s32 xzSize, s32 ySize, u16 flag);
void pushRacePlayerOutOfCylinderAndApplyItemHit(Vec3i *pos, s32 xzSize, s32 ySize, u16 flag, s16 playerIndex);
void pushRacePlayersOutOfCylinderOrApplyItemHit(Vec3i *pos, s32 xzSize, s32 ySize, s32 arg3, s16 arg4);
void pushRacePlayerOutOfCylinder(Vec3i *pos, s32 xzSize, s32 ySize, s16 playerIndex);
s32 isRacePlayerInsideCylinder(Vec3i *pos, s32 xzSize, s32 ySize, s16 playerIndex);
void applyItemHitToRacePlayersInsideSphere(Vec3i *pos, s32 xzSize, s16 flag);
s32 tryApplyRacePlayerItemHit(Vec3i *pos, s32 xzSize, s16 flag, s16 playerIndex);
s32 updateRacePlayerSurfaceContact(RacePlayer *player);
s32 updateRacePlayerGroundAlignment(RacePlayer *player);
s32 updateRacePlayerLeanAngle(RacePlayer *player, s32 arg1, s16 arg2);
void clampRacePlayerVectorXZSpeed(Vec3i *vec, RacePlayer *player);
void clampRacePlayerVectorXZHalfSpeed(Vec3i *vec, RacePlayer *player);
void updateRacePlayerLocalVelocity(RacePlayer *player, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void updateRacePlayerLocalVelocityNoVerticalOffset(RacePlayer *player, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void addRacePlayerScore(RacePlayer *player, s32 arg1);
void enqueueRacePlayerVoiceSound(RacePlayer *player, s16 soundType);
void updateRacePlayerProjectedPosition(RacePlayer *player);

#endif
