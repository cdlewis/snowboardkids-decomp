#ifndef RACE_POSITION_H
#define RACE_POSITION_H

#include "common.h"

struct RacePositionPlayer;

void updateRacePlayerRankDisplay(void);
void updateRacePlayerCheckpointEventState(struct RacePositionPlayer *player);
s32 getSmoothedRacePlayerPathOffset(s32 playerIndex, s32 pathIndex, s32 rankSlot);
s32 getRacePlayerPathOffset(s32 playerIndex, s32 pathIndex);

#endif
