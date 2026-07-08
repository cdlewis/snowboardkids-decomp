#ifndef FIXED_POINT_MATH_H
#define FIXED_POINT_MATH_H

#include "common.h"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i;

s32 func_80049000(Vec3i *position);
s32 func_8004908C(s32 deltaX, s32 deltaZ);
s32 func_8004940C(s32 fromX, s32 fromZ, s32 toX, s32 toZ);

#endif
