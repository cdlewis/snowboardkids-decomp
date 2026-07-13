#ifndef RACE_PLAYER_COLLISION_H
#define RACE_PLAYER_COLLISION_H

#include "race_player_input.h"

s32 tryQueueRacePlayerHitCueSpinout(RacePlayer *player);
s32 tryQueueRacePlayerHitCueGhostSpinout(RacePlayer *player);
s32 tryQueueRacePlayerHitCueLaunch(RacePlayer *player);
s32 tryQueueRacePlayerHitCueStun(RacePlayer *player);
s32 tryQueueRacePlayerHitCueTrip(RacePlayer *player);
s32 tryQueueRacePlayerHitCueTumble(RacePlayer *player);
s32 tryQueueRacePlayerHitCueSlide(RacePlayer *player);
s32 tryQueueRacePlayerHitCuePushForward(RacePlayer *player);
s32 tryQueueRacePlayerHitCuePushBack(RacePlayer *player);
s32 tryQueueRacePlayerHitCueNudge(RacePlayer *player);
s32 tryQueueRacePlayerHitCueSlowdown(RacePlayer *player);
s32 tryQueueRacePlayerHitCueObstacle(RacePlayer *player);
s32 tryQueueRacePlayerHitCueHeavyKnockdown(RacePlayer *player);
s32 tryQueueRacePlayerHitCueItemSteal(RacePlayer *player);
s32 tryQueueRacePlayerHitCueGhostSlowdown(RacePlayer *player);
s32 tryQueueRacePlayerHitCueSpin(RacePlayer *player);
s32 tryQueueRacePlayerHitCueReverseSpin(RacePlayer *player);
s32 tryQueueRacePlayerHitCueTerrainFall(RacePlayer *player);
s32 tryQueueRacePlayerHitCueTerrainCrash(RacePlayer *player);
void resolveRacePlayerHitReactions(RacePlayer *player);
s32 isRacePlayerRespawnSurfaceValid(RacePlayer *player);
void resolveRacePlayerCollisionVolumes(RacePlayer *player);

#endif
