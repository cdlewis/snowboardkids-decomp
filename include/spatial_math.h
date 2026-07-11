#ifndef SPATIAL_MATH_H
#define SPATIAL_MATH_H

#include "common.h"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i;

s32 isPositionNearCurrentViewport(Vec3i *position);
s32 calculateAngleFromDeltaXZ(s32 deltaX, s32 deltaZ);
s32 calculateAngleBetweenXZPoints(s32 fromX, s32 fromZ, s32 toX, s32 toZ);

#endif
