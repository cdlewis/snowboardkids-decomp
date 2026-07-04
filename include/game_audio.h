#ifndef GAME_AUDIO_H
#define GAME_AUDIO_H

#include "common.h"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} SoundPosition;

void func_80072A74(s16 soundId, SoundPosition *pos, s16 volume, s16 distance);
void func_80072AC8(s16 soundId, SoundPosition *pos, s16 volume, s16 distance, s16 arg4, s16 priority);

#endif
