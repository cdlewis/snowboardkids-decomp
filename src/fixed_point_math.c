#include "common.h"

#define OBJECT_COUNT 3
#define OBJECT_CULL_RANGE 0xBA00000
#define NEG_OBJECT_CULL_RANGE (-OBJECT_CULL_RANGE + 1)

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i;

typedef struct {
    u8 pad0[0x44];
    s32 offsetX;
    s32 pad48;
    s32 offsetZ;
    u8 pad50[0x60];
} ObjectTransform;

extern u8 D_80156608;
extern s16 D_800D40D0[];
extern ObjectTransform D_801121E0[OBJECT_COUNT];

s32 func_80049000(Vec3i *position);
s32 func_8004908C(s32 deltaX, s32 deltaZ);

#pragma GLOBAL_ASM("asm/nonmatchings/fixed_point_math/func_80049000.s")

s32 func_8004908C(s32 deltaX, s32 deltaZ) {
    s16 angle;

    deltaZ = -deltaZ;

    if ((deltaX == 0) && (deltaZ == 0)) {
        return 0;
    }

    if ((deltaX >= 0) && (deltaZ >= 0)) {
        if (deltaX < deltaZ) {
            angle = D_800D40D0[(s16) ((0x280LL * deltaX) / deltaZ)];
            return (s16) -angle;
        }
        angle = 0x400 - D_800D40D0[(s16) ((0x280LL * deltaZ) / deltaX)];
        return (s16) -angle;
    }

    if ((deltaX >= 0) && (deltaZ < 0)) {
        deltaZ *= -1;
        if (deltaZ < deltaX) {
            angle = D_800D40D0[(s16) ((0x280LL * deltaZ) / deltaX)] + 0x400;
            return (s16) -angle;
        }
        angle = 0x800 - D_800D40D0[(s16) ((0x280LL * deltaX) / deltaZ)];
        return (s16) -angle;
    }

    if ((deltaX < 0) && (deltaZ < 0)) {
        deltaX *= -1;
        deltaZ *= -1;
        if (deltaX < deltaZ) {
            angle = D_800D40D0[(s16) ((0x280LL * deltaX) / deltaZ)] + 0x800;
            return (s16) -angle;
        }
        angle = 0xC00 - D_800D40D0[(s16) ((0x280LL * deltaZ) / deltaX)];
        return (s16) -angle;
    }

    angle = 0;
    if ((deltaX < 0) && (deltaZ >= 0)) {
        deltaX *= -1;
        if (deltaZ < deltaX) {
            angle = D_800D40D0[(s16) ((0x280LL * deltaZ) / deltaX)] + 0xC00;
            return (s16) -angle;
        }
        angle = 0x1000 - D_800D40D0[(s16) ((0x280LL * deltaX) / deltaZ)];
        return (s16) -angle;
    }

    return angle;
}

s32 func_8004940C(s32 fromX, s32 fromZ, s32 toX, s32 toZ) {
    return func_8004908C(toX - fromX, toZ - fromZ);
}
