#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "common.h"

void initSoundManager(void);
void updateSoundManager(void);
void fadeOutAllMusicSequences(void);
void requestMusicSequenceBank(s32 arg0);
void requestMusicSequenceStop(s32 arg0);
void stopSoundEffects(void);
void requestCourseMusicSequence(void);
s32 countActiveMusicSequences(void);
void enqueuePositionalSoundEffect(s16 soundId, Vec3i *pos, s16 volume, s16 distance);
void enqueuePlayerPositionalSoundEffect(s16 soundId, Vec3i *pos, s16 volume, s16 distance, s16 arg4, s16 priority);

#endif
