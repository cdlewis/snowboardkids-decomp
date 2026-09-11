#ifndef RACE_START_TRANSITION_H
#define RACE_START_TRANSITION_H

#include "common.h"

extern Vec3i gMenuCameraTargetOffset;

void updateMenuCameraObjectLookAtOrigin(void);
void updateMenuCameraObjectLookAtOriginCallback(void);
void updateMenuCameraObjectWithTargetOffset(void);
void updateMenuCameraObjectWithTargetOffsetCallback(void);
void initPassAwardCelebration(void);
void updatePassAwardCelebrationIntroDelay(void);
void updatePassAwardCelebrationFadeIn(void);
void updatePassAwardCelebrationFadeOut(void);
void finishPassAwardCelebration(void);

#endif
