#ifndef RACE_MOTION_H
#define RACE_MOTION_H

#include "common.h"

typedef struct RaceMotionState RaceMotionState;
typedef struct RaceMotionInitState RaceMotionInitState;

void initRaceCourseSurfaceData(void);
s32 findRaceCourseSurfaceFromPoint(s32 index, s32 x, s32 z);
s32 findRaceCourseSurfaceAtPoint(s32 x, s32 z);
void pushRaceCourseSurfaceBoundaryWithVelocity(s32 *arg0, s32 *arg1, s32 arg2);
void resolveRaceCourseSurfaceCollisionWithNormal(void);
void resolveRaceCourseSurfaceCollisionWithVelocity(s16 arg0, s32 x, s32 z, s32 radius, s32 *pushX, s32 *pushZ, s32 *velocityX,
                  s32 *velocityZ);
void pushRaceCourseSurfaceBoundary(s32 arg0);
void resolveRaceCourseSurfaceCollision(s16 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4, s32 *arg5);
s32 getRaceCourseSurfaceHeight(s32 arg0, s32 arg1, s32 arg2);
s32 getRaceCourseSurfaceType(s32 arg0, s32 arg1, s32 arg2);
u32 projectRaceCourseSurfaceProgress(s32 arg0, s32 arg1, s32 arg2);
void getRaceCourseSurfaceSpawnTransform(s32 arg0, s32 *x, s32 *y, s32 *z, s16 *angle);
void getRaceCourseTargetPositionAhead(s32 arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4, s32 arg5, s32 arg6);
void getRaceCourseProgressPosition(s32 arg0, s32 *arg1, s32 *arg2, s32 arg3);
s16 getRaceCourseNextSurface(s32 arg0);
void setRaceMotionAnimation(RaceMotionState *state, s32 animIndex);
void loadRaceMotionAnimationFrame(RaceMotionState *state);
void loadRaceMotionJointAnimationFrame(RaceMotionState *state);
void interpolateRaceMotionAnimationFrame(RaceMotionState *state, s32 animIndex, s32 frameTimer, s32 frameTimerReset);
void interpolateRaceMotionJointAnimationFrame(RaceMotionState *state, s32 animIndex, s32 frameTimer, s32 frameTimerReset);
void blendRaceMotionJointAnimation(RaceMotionState *state, s32 animIndex, s32 timer, s32 duration);
void stepRaceMotionLoopingAnimation(RaceMotionState *state);
void stepRaceMotionLoopingJointAnimation(RaceMotionState *state);
s32 stepRaceMotionAnimationUntilEnd(RaceMotionState *state);
s32 stepRaceMotionJointAnimationUntilEnd(RaceMotionState *state);
void initRaceMotionModelParts(RaceMotionInitState *state);

#endif
