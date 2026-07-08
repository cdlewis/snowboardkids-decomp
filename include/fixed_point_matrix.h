#ifndef FIXED_POINT_MATRIX_H
#define FIXED_POINT_MATRIX_H

#include "fixed_point_math.h"

/*
 * Fixed-point (4.12) 3x3 rotation matrix and transform helpers used by the
 * race / menu effect and camera systems. FIXED_MATRIX_ONE (0x1000) is the
 * unit value; trig lookups return values scaled by it.
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

void func_80097A80(FixedTransform *arg0);
s16 func_80097AE8(s16 arg0);
s16 func_80097B48(s16 arg0);
void func_80097BAC(FixedMatrix3s arg0, s16 arg1);
void func_80097C18(FixedMatrix3s arg0, s16 arg1);
void func_80097C84(FixedMatrix3s arg0, s16 arg1);
void func_80097CF0(FixedMatrix3s arg0, FixedMatrix3s arg1, FixedMatrix3s arg2);
void func_80097DA4(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3);
void func_80097FE4(FixedMatrix3s arg0, s16 arg1, s16 arg2);
void func_800980D0(FixedMatrix3s arg0, s16 arg1, s16 arg2);
void func_80098124(FixedMatrix3s arg0, s16 arg1, s16 arg2);
void func_80098174(FixedMatrix3s arg0, s16 arg1, s16 arg2);
void func_800981C8(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3);
void func_800983E4(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3);
void func_80098458(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3);
void func_800984CC(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3);
void func_8009853C(FixedMatrix3s arg0, s16 arg1, s16 arg2);
void func_80098590(FixedMatrix3s arg0, Vec3i *source, Vec3i *dest);
void func_800987A0(FixedTransform *arg0, FixedTransform *arg1, FixedTransform *arg2);
void func_800989EC(FixedTransform *arg0, FixedTransform *arg1, FixedTransform *arg2);
s32 func_80098C30(s64 arg0);

#endif
