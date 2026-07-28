#ifndef FIXED_MATRIX_TYPES_H
#define FIXED_MATRIX_TYPES_H

#include "common.h"

typedef s16 FixedMatrix3s[9];
typedef s16 FixedMatrix3sPadded[10];
typedef s16 FixedMatrix3sScratch[0x10];
typedef s16 FixedMatrix3sWideScratch[0x12];

/* Rotation transform: a 3x3 rotation followed by a translation. */
typedef struct {
    /* 0x00 */ FixedMatrix3s rotation;
    /* 0x12 */ s16 pad12;
    /* 0x14 */ Vec3i translation;
} FixedTransform;

#endif
