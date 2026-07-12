#ifndef RACE_PLAYER_POSITION_H
#define RACE_PLAYER_POSITION_H

#include "common.h"

struct RacePlayerPositionState;

void updateRacePlayerRankDisplay(void);
s32 updateRacePlayerSmoothedPathOffset(s32 playerIndex, s32 pathIndex, s32 rankSlot);
s32 getRacePlayerPathOffset(s32 playerIndex, s32 pathIndex);
void updateRacePlayerCheckpointEventState(struct RacePlayerPositionState *player);

#endif
