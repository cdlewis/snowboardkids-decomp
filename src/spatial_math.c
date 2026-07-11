#include "common.h"
#include "spatial_math.h"

#define OBJECT_COUNT 3
#define OBJECT_CULL_RANGE 0xBA00000
#define NEG_OBJECT_CULL_RANGE (-OBJECT_CULL_RANGE + 1)

typedef struct {
    u8 pad0[0x44];
    s32 offsetX;
    s32 pad48;
    s32 offsetZ;
    u8 pad50[0x60];
} ObjectTransform;

extern u8 gCurrentViewportIndex;
extern s16 gArctanAngleTable[];
extern ObjectTransform D_801121E0[OBJECT_COUNT];

s32 isPositionNearCurrentViewport(Vec3i *position) {
    s32 deltaX;
    s32 deltaZ;

    deltaX = -D_801121E0[gCurrentViewportIndex].offsetX - position->x;
    deltaZ = -D_801121E0[gCurrentViewportIndex].offsetZ - position->z;

    if ((deltaX < OBJECT_CULL_RANGE) && (deltaX >= NEG_OBJECT_CULL_RANGE) && (deltaZ < OBJECT_CULL_RANGE)) {
        if (deltaZ >= NEG_OBJECT_CULL_RANGE) {
            return 1;
        }
    }
    return 0;
}

s32 calculateAngleFromDeltaXZ(s32 deltaX, s32 deltaZ) {
    s16 angle;

    deltaZ = -deltaZ;

    if ((deltaX == 0) && (deltaZ == 0)) {
        return 0;
    }

    if ((deltaX >= 0) && (deltaZ >= 0)) {
        if (deltaX < deltaZ) {
            angle = gArctanAngleTable[(s16) ((0x280LL * deltaX) / deltaZ)];
            return (s16) -angle;
        }
        angle = 0x400 - gArctanAngleTable[(s16) ((0x280LL * deltaZ) / deltaX)];
        return (s16) -angle;
    }

    if ((deltaX >= 0) && (deltaZ < 0)) {
        deltaZ *= -1;
        if (deltaZ < deltaX) {
            angle = gArctanAngleTable[(s16) ((0x280LL * deltaZ) / deltaX)] + 0x400;
            return (s16) -angle;
        }
        angle = 0x800 - gArctanAngleTable[(s16) ((0x280LL * deltaX) / deltaZ)];
        return (s16) -angle;
    }

    if ((deltaX < 0) && (deltaZ < 0)) {
        deltaX *= -1;
        deltaZ *= -1;
        if (deltaX < deltaZ) {
            angle = gArctanAngleTable[(s16) ((0x280LL * deltaX) / deltaZ)] + 0x800;
            return (s16) -angle;
        }
        angle = 0xC00 - gArctanAngleTable[(s16) ((0x280LL * deltaZ) / deltaX)];
        return (s16) -angle;
    }

    angle = 0;
    if ((deltaX < 0) && (deltaZ >= 0)) {
        deltaX *= -1;
        if (deltaZ < deltaX) {
            angle = gArctanAngleTable[(s16) ((0x280LL * deltaZ) / deltaX)] + 0xC00;
            return (s16) -angle;
        }
        angle = 0x1000 - gArctanAngleTable[(s16) ((0x280LL * deltaX) / deltaZ)];
        return (s16) -angle;
    }

    return angle;
}

s32 calculateAngleBetweenXZPoints(s32 fromX, s32 fromZ, s32 toX, s32 toZ) {
    return calculateAngleFromDeltaXZ(toX - fromX, toZ - fromZ);
}
