#ifndef RACE_MOTION_ANIMATION_DEBUG_VIEWER_H
#define RACE_MOTION_ANIMATION_DEBUG_VIEWER_H

#include "common.h"

#if defined(VERSION_JP)
extern const char gRaceMotionAnimationDebugViewerMotionNumberFormat[];
#endif

void initRaceMotionAnimationDebugViewer(void);
void drawRaceMotionAnimationDebugViewerMotionNumber(void *unused);
void updateRaceMotionAnimationDebugViewer(void);

#endif
