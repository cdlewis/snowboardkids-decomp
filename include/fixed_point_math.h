#ifndef FIXED_POINT_MATH_H
#define FIXED_POINT_MATH_H

#include "spatial_math.h"

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

/* 3x3 fixed-point rotation matrix (row-major). */
typedef s16 FixedMatrix3s[9];

/* Rotation transform: a 3x3 rotation followed by a translation. */
typedef struct {
    /* 0x00 */ FixedMatrix3s rotation;
    /* 0x12 */ s16 pad12;
    /* 0x14 */ Vec3i translation;
} FixedTransform;

void initFixedTransform(FixedTransform *arg0);
s16 fixedSine(s16 arg0);
s16 fixedCosine(s16 arg0);
void makeFixedRotateX(FixedMatrix3s arg0, s16 arg1);
void makeFixedRotateY(FixedMatrix3s arg0, s16 arg1);
void makeFixedRotateZ(FixedMatrix3s arg0, s16 arg1);
void func_80048D60(FixedMatrix3s arg0);
void multiplyFixedMatrices(FixedMatrix3s arg0, FixedMatrix3s arg1, FixedMatrix3s arg2);
void makeFixedRotateXYZ(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3);
void makeFixedRotateXY(FixedMatrix3s arg0, s16 arg1, s16 arg2);
void makeFixedRotateZX(FixedMatrix3s arg0, s16 arg1, s16 arg2);
void makeFixedRotateXZ(FixedMatrix3s arg0, s16 arg1, s16 arg2);
void makeFixedRotateZY(FixedMatrix3s arg0, s16 arg1, s16 arg2);
void makeFixedRotateZXY(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3);
void makeFixedRotateYZX(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3);
void makeFixedRotateZYX(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3);
void makeFixedRotateXZY(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3);
void makeFixedRotateYX(FixedMatrix3s arg0, s16 arg1, s16 arg2);
void transformVec3ByFixedMatrix(FixedMatrix3s arg0, Vec3i *source, Vec3i *dest);
void composeFixedTransforms(FixedTransform *arg0, FixedTransform *arg1, FixedTransform *arg2);
void composeFixedTransformTranslation(FixedTransform *arg0, FixedTransform *arg1, FixedTransform *arg2);
s32 integerSqrt64(s64 arg0);

#endif
