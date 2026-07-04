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
extern ObjectTransform D_801121E0[OBJECT_COUNT];

s32 func_80049000(Vec3i *position);
s32 func_8004908C(s32 deltaX, s32 deltaZ);

#pragma GLOBAL_ASM("asm/nonmatchings/fixed_point_math/func_80049000.s")
#pragma GLOBAL_ASM("asm/nonmatchings/fixed_point_math/func_8004908C.s")

s32 func_8004940C(s32 fromX, s32 fromZ, s32 toX, s32 toZ) {
    return func_8004908C(toX - fromX, toZ - fromZ);
}
