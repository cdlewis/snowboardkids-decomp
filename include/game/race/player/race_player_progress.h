#ifndef RACE_PLAYER_PROGRESS_H
#define RACE_PLAYER_PROGRESS_H

#include "common.h"

struct RacePlayerProgressState;

void updateRacePlayerRankDisplay(void);
s32 updateRacePlayerSmoothedPathOffset(s32 playerIndex, s32 pathIndex, s32 rankSlot);
s32 getRacePlayerPathOffset(s32 playerIndex, s32 pathIndex);
void updateRacePlayerCheckpointEvents(struct RacePlayerProgressState *player);

#endif
