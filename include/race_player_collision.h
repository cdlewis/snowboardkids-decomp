#ifndef RACE_PLAYER_COLLISION_H
#define RACE_PLAYER_COLLISION_H

#include "race_player_input.h"

s32 tryQueueRacePlayerHitCueSpinout(RaceInputPlayer *player);
s32 tryQueueRacePlayerHitCueGhostSpinout(RaceInputPlayer *player);
s32 tryQueueRacePlayerHitCueLaunch(RaceInputPlayer *player);
s32 tryQueueRacePlayerHitCueStun(RaceInputPlayer *player);
s32 tryQueueRacePlayerHitCueTrip(RaceInputPlayer *player);
s32 tryQueueRacePlayerHitCueTumble(RaceInputPlayer *player);
s32 tryQueueRacePlayerHitCueSlide(RaceInputPlayer *player);
s32 tryQueueRacePlayerHitCuePushForward(RaceInputPlayer *player);
s32 tryQueueRacePlayerHitCuePushBack(RaceInputPlayer *player);
s32 tryQueueRacePlayerHitCueNudge(RaceInputPlayer *player);
s32 tryQueueRacePlayerHitCueSlowdown(RaceInputPlayer *player);
s32 tryQueueRacePlayerHitCueObstacle(RaceInputPlayer *player);
s32 tryQueueRacePlayerHitCueHeavyKnockdown(RaceInputPlayer *player);
s32 tryQueueRacePlayerHitCueItemSteal(RaceInputPlayer *player);
s32 tryQueueRacePlayerHitCueGhostSlowdown(RaceInputPlayer *player);
s32 tryQueueRacePlayerHitCueSpin(RaceInputPlayer *player);
s32 tryQueueRacePlayerHitCueReverseSpin(RaceInputPlayer *player);
s32 tryQueueRacePlayerTerrainFallCue(RaceInputPlayer *player);
s32 tryQueueRacePlayerTerrainCrashCue(RaceInputPlayer *player);
void resolveRacePlayerItemHitReactions(RaceInputPlayer *player);
s32 isRacePlayerRespawnSurfaceValid(RaceInputPlayer *player);
void resolveRacePlayerCollisionVolumes(RaceInputPlayer *player);

#endif
