#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "common.h"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} SoundPosition;

void initSoundManager(void);
void updateSoundManager(void);
void fadeOutAllMusicSequences(void);
void requestMusicSequenceBank(s32 arg0);
void requestMusicSequenceStop(s32 arg0);
void stopSoundEffects(void);
void requestCourseMusicSequence(void);
s32 countActiveMusicSequences(void);
void enqueuePositionalSoundEffect(s16 soundId, SoundPosition *pos, s16 volume, s16 distance);
void enqueuePlayerPositionalSoundEffect(s16 soundId, SoundPosition *pos, s16 volume, s16 distance, s16 arg4, s16 priority);

#endif
