#ifndef RACE_START_TRANSITION_H
#define RACE_START_TRANSITION_H

#include "common.h"

void updateMenuCameraObjectLookAtOrigin(void);
void updateMenuCameraObject(void);
void updateMenuCameraObjectWithTargetOffset(void);
void updateMenuCameraObjectFromTargetOffset(void);
void initRaceStartTransition(void);
void updateRaceStartTransitionIntroDelay(void);
void updateRaceStartTransitionFadeIn(void);
void updateRaceStartTransitionFadeOut(void);
void finishRaceStartTransition(void);

#endif
