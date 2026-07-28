#ifndef RACE_START_TRANSITION_H
#define RACE_START_TRANSITION_H

#include "common.h"

extern Vec3i gMenuCameraTargetOffset;

void updateMenuCameraObjectLookAtOrigin(void);
void updateMenuCameraObjectLookAtOriginCallback(void);
void updateMenuCameraObjectWithTargetOffset(void);
void updateMenuCameraObjectWithTargetOffsetCallback(void);
void initRaceStartTransition(void);
void updateRaceStartTransitionIntroDelay(void);
void updateRaceStartTransitionFadeIn(void);
void updateRaceStartTransitionFadeOut(void);
void finishRaceStartTransition(void);

#endif
