#ifndef RACE_PROGRESS_H
#define RACE_PROGRESS_H

#include "common.h"

struct RaceProgressPlayer;

void updateRacePositionTracker(void);
void updateRacePlayerCheckpointEvent(struct RaceProgressPlayer *player);
s32 getSmoothedRacePlayerPathOffset(s32 playerIndex, s32 pathIndex, s32 rankSlot);
s32 getRacePlayerPathOffset(s32 playerIndex, s32 pathIndex);

#endif
