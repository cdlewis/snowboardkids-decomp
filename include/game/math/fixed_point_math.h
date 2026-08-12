#ifndef FIXED_POINT_MATH_H
#define FIXED_POINT_MATH_H

#include "game/math/fixed_matrix_multiply.h"
#include "game/math/spatial_math.h"

extern s16 gSineTable[];

/*
 * Fixed-point (4.12) trig, 3x3 rotation matrix, transform, and integer math
 * helpers used by the race / menu effect and camera systems. FIXED_MATRIX_ONE
 * (0x1000) is the unit value; trig lookups return values scaled by it.
 */

typedef enum { MTX_XX, MTX_XY, MTX_XZ, MTX_YX, MTX_YY, MTX_YZ, MTX_ZX, MTX_ZY, MTX_ZZ } FixedMatrixIndex;

extern Transform3D gIdentityFixedTransform;
extern Mtx gIdentityMatrix;

void initFixedTransform(Transform3D *arg0);
s16 fixedSine(s16 arg0);
s16 fixedCosine(s16 arg0);
void makeFixedRotationX(Mat3x3 arg0, s16 arg1);
void makeFixedRotationY(Mat3x3 arg0, s16 arg1);
void makeFixedRotationZ(Mat3x3 arg0, s16 arg1);
void scaleFixedMatrix3sByQuarter(Mat3x3 arg0);
void makeFixedRotationXYZ(Mat3x3 arg0, s16 arg1, s16 arg2, s16 arg3);
void makeFixedRotationXY(Mat3x3 arg0, s16 arg1, s16 arg2);
void makeFixedRotationZX(Mat3x3 arg0, s16 arg1, s16 arg2);
void makeFixedRotationXZ(Mat3x3 arg0, s16 arg1, s16 arg2);
void makeFixedRotationZY(Mat3x3 arg0, s16 arg1, s16 arg2);
void makeFixedRotationZXY(Mat3x3 arg0, s16 arg1, s16 arg2, s16 arg3);
void makeFixedRotationYZX(Mat3x3 arg0, s16 arg1, s16 arg2, s16 arg3);
void makeFixedRotationZYX(Mat3x3 arg0, s16 arg1, s16 arg2, s16 arg3);
void makeFixedRotationXZY(Mat3x3 arg0, s16 arg1, s16 arg2, s16 arg3);
void makeFixedRotationYX(Mat3x3 arg0, s16 arg1, s16 arg2);
void transformVec3iByFixedMatrix(Mat3x3 arg0, Vec3i *source, Vec3i *dest);
void composeFixedTransforms(Transform3D *arg0, Transform3D *arg1, Transform3D *arg2);
void composeFixedTransformTranslation(Transform3D *arg0, Transform3D *arg1, Transform3D *arg2);
s32 integerSquareRoot64(s64 arg0);

#endif
