#ifndef RACE_MOTION_ANIMATION_DEBUG_VIEWER_H
#define RACE_MOTION_ANIMATION_DEBUG_VIEWER_H

#include "common.h"

extern const char gRaceMotionAnimationDebugViewerMotionNumberFormat[];

void initRaceMotionAnimationDebugViewer(void);
void drawRaceMotionAnimationDebugViewerMotionNumber(void *unused);
void updateRaceMotionAnimationDebugViewer(void);

#endif
