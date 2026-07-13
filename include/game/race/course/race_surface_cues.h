#ifndef RACE_SURFACE_CUES_H
#define RACE_SURFACE_CUES_H

#include "game/race/player/race_player_input.h"

typedef struct {
    /* 0x0 */ s16 surfaceType;
    /* 0x2 */ s16 cueValue;
} RaceSurfaceCue;

s32 advanceRaceSurfaceCueSequence(RaceSurfaceCue *cues, RacePlayer *player, s16 cueIndex);
void resetRacePlayerSurfaceCueState(RacePlayer *player);
s32 updateRacePlayerSurfaceCue(RacePlayer *player);

#endif
