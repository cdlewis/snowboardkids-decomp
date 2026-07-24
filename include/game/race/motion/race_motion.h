#ifndef RACE_MOTION_H
#define RACE_MOTION_H

#include "common.h"

typedef struct RaceMotionState RaceMotionState;
typedef struct RaceMotionInitState RaceMotionInitState;

void initRaceCourseSurfaceData(void);
s32 findRaceCourseSurfaceFromHint(s32 surfaceIndex, s32 x, s32 z);
s32 findRaceCourseSurfaceAtPoint(s32 x, s32 z);
void pushRaceCourseSurfaceBoundaryWithVelocity(s32 *velocityX, s32 *velocityZ, s32 radius);
void resolveRaceCourseSurfaceCollisionWithNormal(s32 surfaceIndex, s32 x, s32 z, s32 radius, s8 normalIndex, s32 *pushX,
                  s32 *pushZ, s32 *velocityX, s32 *velocityZ);
void resolveRaceCourseSurfaceCollisionWithVelocity(s32 surfaceIndex, s32 x, s32 z, s32 radius, s32 *pushX,
                  s32 *pushZ, s32 *velocityX, s32 *velocityZ);
void pushRaceCourseSurfaceBoundary(s32 radius);
void resolveRaceCourseSurfaceCollision(s32 surfaceIndex, s32 x, s32 z, s32 radius, s32 *pushX, s32 *pushZ);
s32 getRaceCourseSurfaceHeight(s32 surfaceIndex, s32 x, s32 z);
s32 getRaceCourseSurfaceType(s32 surfaceIndex, s32 x, s32 z);
u32 projectRaceCourseSurfaceProgress(s32 surfaceIndex, s32 x, s32 z);
void getRaceCourseSurfaceSpawnTransform(s32 surfaceIndex, s32 *x, s32 *y, s32 *z, s16 *angle);
void getRaceCourseTargetPositionAhead(s32 surfaceIndex, s32 x, s32 z, s32 *targetX, s32 *targetZ, s32 playerIndex,
                  s32 lookahead);
void getRaceCourseProgressPosition(s32 surfaceIndex, s32 *x, s32 *z, s32 playerIndex);
s16 getRaceCourseNextSurface(s32 surfaceIndex);
void setRaceMotionAnimation(RaceMotionState *state, s32 animationIndex);
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
