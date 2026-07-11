#ifndef RACE_SURFACE_CUES_H
#define RACE_SURFACE_CUES_H

#include "race_input_history.h"

typedef struct {
    /* 0x0 */ s16 surfaceType;
    /* 0x2 */ s16 cueValue;
} RaceSurfaceCue;

s32 func_800831C0(RaceSurfaceCue *cues, RaceInputPlayer *player, s16 cueIndex);
void func_80083298(RaceInputPlayer *player);
s32 func_800832CC(RaceInputPlayer *player);

#endif
