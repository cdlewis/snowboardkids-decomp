#ifndef FIXED_POINT_MATH_H
#define FIXED_POINT_MATH_H

#include "game/math/fixed_matrix_multiply.h"
#include "game/math/spatial_math.h"

/*
 * Fixed-point (4.12) trig, 3x3 rotation matrix, transform, and integer math
 * helpers used by the race / menu effect and camera systems. FIXED_MATRIX_ONE
 * (0x1000) is the unit value; trig lookups return values scaled by it.
 */

typedef enum {
    MTX_XX,
    MTX_XY,
    MTX_XZ,
    MTX_YX,
    MTX_YY,
    MTX_YZ,
    MTX_ZX,
    MTX_ZY,
    MTX_ZZ
} FixedMatrixIndex;

extern FixedTransform gIdentityFixedTransform;

void initFixedTransform(FixedTransform *arg0);
s16 fixedSine(s16 arg0);
s16 fixedCosine(s16 arg0);
void makeFixedRotationX(FixedMatrix3s arg0, s16 arg1);
void makeFixedRotationY(FixedMatrix3s arg0, s16 arg1);
void makeFixedRotationZ(FixedMatrix3s arg0, s16 arg1);
void scaleFixedMatrix3sByQuarter(FixedMatrix3s arg0);
void makeFixedRotationXYZ(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3);
void makeFixedRotationXY(FixedMatrix3s arg0, s16 arg1, s16 arg2);
void makeFixedRotationZX(FixedMatrix3s arg0, s16 arg1, s16 arg2);
void makeFixedRotationXZ(FixedMatrix3s arg0, s16 arg1, s16 arg2);
void makeFixedRotationZY(FixedMatrix3s arg0, s16 arg1, s16 arg2);
void makeFixedRotationZXY(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3);
void makeFixedRotationYZX(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3);
void makeFixedRotationZYX(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3);
void makeFixedRotationXZY(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3);
void makeFixedRotationYX(FixedMatrix3s arg0, s16 arg1, s16 arg2);
void transformVec3iByFixedMatrix(FixedMatrix3s arg0, Vec3i *source, Vec3i *dest);
void composeFixedTransforms(FixedTransform *arg0, FixedTransform *arg1, FixedTransform *arg2);
void composeFixedTransformTranslation(FixedTransform *arg0, FixedTransform *arg1, FixedTransform *arg2);
s32 integerSquareRoot64(s64 arg0);

#endif
