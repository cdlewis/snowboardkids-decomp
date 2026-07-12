#ifndef RACE_SURFACE_CUES_H
#define RACE_SURFACE_CUES_H

#include "race_player_input.h"

typedef struct {
    /* 0x0 */ s16 surfaceType;
    /* 0x2 */ s16 cueValue;
} RaceSurfaceCue;

s32 advanceRaceSurfaceCueSequence(RaceSurfaceCue *cues, RaceInputPlayer *player, s16 cueIndex);
void resetRacePlayerSurfaceCueState(RaceInputPlayer *player);
s32 updateRacePlayerSurfaceCue(RaceInputPlayer *player);

#endif
