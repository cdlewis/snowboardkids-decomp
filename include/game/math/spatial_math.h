#ifndef SPATIAL_MATH_H
#define SPATIAL_MATH_H

#include "common.h"

s32 isPositionNearCurrentRaceViewportCamera(Vec3i *position);
s32 calculateFixedAngleFromDeltaXZ(s32 deltaX, s32 deltaZ);
s32 calculateFixedAngleBetweenXZPoints(s32 fromX, s32 fromZ, s32 toX, s32 toZ);

#endif
