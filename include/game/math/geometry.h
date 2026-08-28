#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <PR/ultratypes.h>

typedef struct Vec2s {
    s16 x;
    s16 y;
} Vec2s;

typedef struct Vec3s {
    s16 x;
    s16 y;
    s16 z;
} Vec3s;

typedef struct Vec3i {
    s32 x;
    s32 y;
    s32 z;
} Vec3i;

typedef s16 Mat3x3[9];

/* Rotation transform: a 3x3 fixed-point rotation followed by a translation. */
typedef struct Transform3D {
    /* 0x00 */ union {
        Mat3x3 rotation;
        s16 rotationRows[3][3];
    };
    /* 0x12 */ s16 pad12;
    /* 0x14 */ union {
        Vec3i translation;
        s32 translationElements[3];
    };
} Transform3D;

#endif
