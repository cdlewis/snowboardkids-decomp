#ifndef RACE_POSITION_TRACKER_H
#define RACE_POSITION_TRACKER_H

#include "common.h"

struct RacePositionPlayer;

#define updateRacePositionTracker func_8007B250

void updateRacePositionTracker(void);
void func_8007BB08(struct RacePositionPlayer *player);
s32 getSmoothedRacePlayerPathOffset(s32 playerIndex, s32 pathIndex, s32 rankSlot);
s32 getRacePlayerPathOffset(s32 playerIndex, s32 pathIndex);

#endif
