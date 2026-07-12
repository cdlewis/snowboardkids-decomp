#include "common.h"
#include "spatial_math.h"

#define RACE_VIEWPORT_CAMERA_COUNT 3
#define RACE_VIEWPORT_CAMERA_CULL_RANGE 0xBA00000
#define NEG_RACE_VIEWPORT_CAMERA_CULL_RANGE (-RACE_VIEWPORT_CAMERA_CULL_RANGE + 1)

typedef struct {
    u8 pad0[0x44];
    s32 transformOffsetX;
    s32 pad48;
    s32 transformOffsetZ;
    u8 pad50[0x60];
} RaceViewportCameraTransform;

extern u8 gCurrentViewportIndex;
extern s16 gFixedArctanAngleTable[];
extern RaceViewportCameraTransform D_801121E0[RACE_VIEWPORT_CAMERA_COUNT];

s32 isPositionNearCurrentRaceViewportCamera(Vec3i *position) {
    s32 deltaX;
    s32 deltaZ;

    deltaX = -D_801121E0[gCurrentViewportIndex].transformOffsetX - position->x;
    deltaZ = -D_801121E0[gCurrentViewportIndex].transformOffsetZ - position->z;

    if ((deltaX < RACE_VIEWPORT_CAMERA_CULL_RANGE) && (deltaX >= NEG_RACE_VIEWPORT_CAMERA_CULL_RANGE) &&
        (deltaZ < RACE_VIEWPORT_CAMERA_CULL_RANGE)) {
        if (deltaZ >= NEG_RACE_VIEWPORT_CAMERA_CULL_RANGE) {
            return 1;
        }
    }
    return 0;
}

s32 calculateFixedAngleFromDeltaXZ(s32 deltaX, s32 deltaZ) {
    s16 angle;

    deltaZ = -deltaZ;

    if ((deltaX == 0) && (deltaZ == 0)) {
        return 0;
    }

    if ((deltaX >= 0) && (deltaZ >= 0)) {
        if (deltaX < deltaZ) {
            angle = gFixedArctanAngleTable[(s16) ((0x280LL * deltaX) / deltaZ)];
            return (s16) -angle;
        }
        angle = 0x400 - gFixedArctanAngleTable[(s16) ((0x280LL * deltaZ) / deltaX)];
        return (s16) -angle;
    }

    if ((deltaX >= 0) && (deltaZ < 0)) {
        deltaZ *= -1;
        if (deltaZ < deltaX) {
            angle = gFixedArctanAngleTable[(s16) ((0x280LL * deltaZ) / deltaX)] + 0x400;
            return (s16) -angle;
        }
        angle = 0x800 - gFixedArctanAngleTable[(s16) ((0x280LL * deltaX) / deltaZ)];
        return (s16) -angle;
    }

    if ((deltaX < 0) && (deltaZ < 0)) {
        deltaX *= -1;
        deltaZ *= -1;
        if (deltaX < deltaZ) {
            angle = gFixedArctanAngleTable[(s16) ((0x280LL * deltaX) / deltaZ)] + 0x800;
            return (s16) -angle;
        }
        angle = 0xC00 - gFixedArctanAngleTable[(s16) ((0x280LL * deltaZ) / deltaX)];
        return (s16) -angle;
    }

    angle = 0;
    if ((deltaX < 0) && (deltaZ >= 0)) {
        deltaX *= -1;
        if (deltaZ < deltaX) {
            angle = gFixedArctanAngleTable[(s16) ((0x280LL * deltaZ) / deltaX)] + 0xC00;
            return (s16) -angle;
        }
        angle = 0x1000 - gFixedArctanAngleTable[(s16) ((0x280LL * deltaX) / deltaZ)];
        return (s16) -angle;
    }

    return angle;
}

s32 calculateFixedAngleBetweenXZPoints(s32 fromX, s32 fromZ, s32 toX, s32 toZ) {
    return calculateFixedAngleFromDeltaXZ(toX - fromX, toZ - fromZ);
}
