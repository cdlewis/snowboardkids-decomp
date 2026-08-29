#ifndef RACE_PLAYER_PROGRESS_H
#define RACE_PLAYER_PROGRESS_H

#include "common.h"
#include "game/race/player/race_player_input.h"

typedef struct RacePlayerCheckpointEvent {
    /* 0x00 */ s16 pathFrame;
    /* 0x02 */ s16 surfaceCueMask;
} RacePlayerCheckpointEvent;

void updateRacePlayerRankDisplay(void);
s32 updateRacePlayerSmoothedPathOffset(s32 playerIndex, s32 pathIndex, s32 rankSlot);
s32 getRacePlayerPathOffset(s32 playerIndex, s32 pathIndex);
void updateRacePlayerCheckpointEvents(RacePlayer *player);

#endif
