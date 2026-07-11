#include "common.h"
#include "memory_allocator.h"
#include "fixed_point_math.h"
#include "model_animation.h"
#include "race_position_tracker.h"

typedef struct ModelAnimCoord {
    s16 x;
    s16 y;
    s16 z;
} ModelAnimCoord;

typedef struct ModelAnimFace {
    u16 coord0;
    u16 coord1;
    u16 coord2;
    s8 faceIndex;
    u8 unk7;
} ModelAnimFace;

typedef struct ModelAnimAssetHeader {
    u16 count;
    u8 data[1];
} ModelAnimAssetHeader;

typedef struct ModelAnimKeyframe {
    s16 nextFaceIndices[2];
    s16 unk4[2];
    s16 coordIndices[4];
    s16 positionIndex;
    s16 angle;
    u16 unk14[4];
} ModelAnimKeyframe;

typedef struct ModelAnimRotation {
    s32 x;
    s32 y;
    s32 z;
} ModelAnimRotation;

typedef struct ModelAnimFrameCursor {
    char pad0[0x33A];
    s16 x;
    s16 y;
    s16 z;
} ModelAnimFrameCursor;

typedef struct ModelAnimStateJoint {
    s32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
} ModelAnimStateJoint;

struct ModelAnimState {
    u16 modelId;
    char pad2[0x332];
    ModelAnimStateJoint joints[12];
    char pad424[6];
    s16 unk42A;
    s16 unk42C;
    s16 unk42E;
    char pad430[0xE];
    s16 unk43E;
    s16 unk440;
    s16 unk442;
    char pad444[0xE];
    s16 animIndex;
    s32 animStartOffset;
    s32 frameDataOffset;
    s16 frameCount;
    s16 framesRemaining;
    s16 unk460;
    s16 unk462;
    s16 frameTimerReset;
    s16 frameTimer;
};

struct ModelAnimInitState {
    u8 pad0[0x10];
    u8 modelId;
    u8 pad11[0x314];
    u8 parentPartId;
    u8 pad326[6];
    s32 x;
    s32 y;
    s32 z;
    u8 partId;
    u8 pad339;
    s16 unk33A;
    s16 unk33C;
    s16 unk33E;
    u8 pad340[0x110];
    s16 partCount;
};

typedef struct CourseSpawnEntry {
    s16 pathIndex;
    char pad2[0x36];
    s16 unk38;
    s16 unk3A;
    char pad3C[2];
    s16 keyframeCount;
    s32 unk40;
    s32 unk44;
} CourseSpawnEntry;

extern ModelAnimCoord *D_80121B90;
extern ModelAnimFace *D_80121B94;
extern ModelAnimKeyframe *D_80121B98;
extern s32 D_80121B9C;
extern s32 D_80121BA0;
extern s32 D_80121BA8;
extern s32 D_80121BB0;
extern s32 D_80121BB4;
extern s32 D_80121BBC;
extern s32 D_80121BC0;
extern s32 D_80121BC8;
extern ModelAnimRotation D_80121C00[];
extern s32 D_80121C48[];
extern s32 D_80121D50;
extern s16 D_8011215C[];
extern s16 D_80112166;
extern s16 D_80121B50;
extern s16 D_800B957E;
extern CourseSpawnEntry D_800B9540[];
extern u8 D_800DE380[];
extern u8 *D_800DE3A8[];
extern u8 *D_800DE3C4[];
extern ModelAnimCoord *D_800DE530[];

extern void func_80081EF4(ModelAnimState *);

void func_8007D190(void) {
    s32 ptr;
    s32 v1;

    ptr = getMemoryBlockBase(D_80112166);
    D_80121B90 = (ModelAnimCoord *)(ptr + 2);
    v1 = (s32)D_80121B90 + *(u16 *)ptr * sizeof(ModelAnimCoord);
    D_80121B94 = (ModelAnimFace *)(v1 + 2);
    v1 = (s32)D_80121B94 + *(u16 *)v1 * sizeof(ModelAnimFace);
    D_80121B98 = (ModelAnimKeyframe *)(v1 + 2);
}

// func_8007D200 best match: 87.524% (base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_8007D200.s")

#ifdef NON_MATCHING
s32 func_8007D200(s32 index, s32 x, s32 z) {
    ModelAnimKeyframe *keyframes;
    ModelAnimKeyframe *keyframe;
    ModelAnimCoord *coords;
    ModelAnimCoord *coord0;
    ModelAnimCoord *coord1;
    ModelAnimCoord *coord2;
    ModelAnimCoord *coord3;
    s32 offset;
    s32 x0;
    s32 x1;
    s32 x2;
    s32 x3;
    s32 z0;
    s32 z1;
    s32 z2;
    s32 z3;
    s16 next;

    keyframes = D_80121B98;

loop:
    offset = index * sizeof(ModelAnimKeyframe);
    keyframe = (ModelAnimKeyframe *)((s32)keyframes + offset);
    coords = D_80121B90;
    coord0 = (ModelAnimCoord *)((s32)coords + keyframe->coordIndices[0] * sizeof(ModelAnimCoord));
    coord1 = (ModelAnimCoord *)((s32)coords + keyframe->coordIndices[1] * sizeof(ModelAnimCoord));
    coord2 = (ModelAnimCoord *)((s32)coords + keyframe->coordIndices[2] * sizeof(ModelAnimCoord));
    coord3 = (ModelAnimCoord *)((s32)coords + keyframe->coordIndices[3] * sizeof(ModelAnimCoord));

    x0 = coord0->x << 0x11;
    x1 = coord1->x << 0x11;
    z0 = coord0->z << 0x11;
    z1 = coord1->z << 0x11;
    x2 = coord2->x << 0x11;
    z2 = coord2->z << 0x11;
    x3 = coord3->x << 0x11;
    z3 = coord3->z << 0x11;

    if ((s64)(x0 - x1) * (z - z1) - (s64)(z0 - z1) * (x - x1) < 0) {
        keyframes = D_80121B98;
        next = ((ModelAnimKeyframe *)((s32)keyframes + offset))->nextFaceIndices[0];
        if (next >= 0) {
            index = next;
            goto loop;
        }
    }
    if ((s64)(x3 - x2) * (z - z2) - (s64)(z3 - z2) * (x - x2) < 0) {
        keyframes = D_80121B98;
        next = ((ModelAnimKeyframe *)((s32)keyframes + offset))->nextFaceIndices[1];
        if (next >= 0) {
            index = next;
            goto loop;
        }
    }
    if ((s64)(x2 - x0) * (z - z0) - (s64)(z2 - z0) * (x - x0) < 0) {
        keyframes = D_80121B98;
        next = ((ModelAnimKeyframe *)((s32)keyframes + offset))->unk4[1];
        if (next >= 0) {
            index = next;
            goto loop;
        }
    }
    if ((s64)(x1 - x3) * (z - z3) - (s64)(z1 - z3) * (x - x3) < 0) {
        keyframes = D_80121B98;
        next = ((ModelAnimKeyframe *)((s32)keyframes + offset))->unk4[0];
        if (next >= 0) {
            index = next;
            goto loop;
        }
    }

    return index;
}
#endif

// func_8007D548 best match: 96.293% (base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_8007D548.s")

#ifdef NON_MATCHING
s32 func_8007D548(s32 x, s32 z) {
    s32 index;

    index = 0;
    if (*(s16 *)((u8 *)&D_800B957E + D_80121B50 * sizeof(CourseSpawnEntry)) >= 0) {
        do {
            ModelAnimKeyframe *keyframe;
            ModelAnimCoord *coord0;
            ModelAnimCoord *coord1;
            ModelAnimCoord *coord2;
            ModelAnimCoord *coord3;
            s32 outside;
            s32 x0;
            s32 x1;
            s32 z0;
            s32 x2;
            s32 x3;
            s32 z1;
            s32 z2;
            s32 z3;

            keyframe = &D_80121B98[index];
            outside = FALSE;
            coord0 = &D_80121B90[keyframe->coordIndices[0]];
            coord1 = &D_80121B90[keyframe->coordIndices[1]];
            coord2 = &D_80121B90[keyframe->coordIndices[2]];
            coord3 = &D_80121B90[keyframe->coordIndices[3]];

            x0 = coord0->x << 0x11;
            x1 = coord1->x << 0x11;
            coord2 = &D_80121B90[keyframe->coordIndices[2]];
            z0 = coord0->z << 0x11;
            x2 = coord2->x << 0x11;
            x3 = coord3->x << 0x11;
            z1 = coord1->z << 0x11;
            z2 = coord2->z << 0x11;
            z3 = coord3->z << 0x11;

            if ((s64)(x0 - x1) * (z - z1) - (s64)(z0 - z1) * (x - x1) < 0) {
                outside = TRUE;
            }
            if ((s64)(x3 - x2) * (z - z2) - (s64)(z3 - z2) * (x - x2) < 0) {
                outside = TRUE;
            }
            if ((s64)(x2 - x0) * (z - z0) - (s64)(z2 - z0) * (x - x0) < 0) {
                outside = TRUE;
            }
            if ((s64)(x1 - x3) * (z - z3) - (s64)(z1 - z3) * (x - x3) < 0) {
                outside = TRUE;
            }

            if (outside == FALSE) {
                return index;
            }
            index++;
        } while (index <= *(s16 *)((u8 *)&D_800B957E + D_80121B50 * sizeof(CourseSpawnEntry)));
    }

    return 0;
}
#endif

void func_8007D87C(s32 *arg0, s32 *arg1, s32 arg2) {
    s32 temp5;
    s32 temp6;
    s32 temp2;
    s32 temp3;
    s32 temp1;
    s32 temp4;

    temp1 = integerSquareRoot64((s64)D_80121BB4 * D_80121BB4 + (s64)D_80121BBC * D_80121BBC);
    temp2 = ((s64)D_80121BBC * 0x1000) / temp1;
    temp3 = ((s64)D_80121BB4 * 0x1000) / temp1;

    temp4 = ((s64)-temp2 * D_80121BA8 + (s64)temp3 * D_80121BB0) / 0x1000;
    if (-arg2 < (D_80121BB0 = temp4)) {
        temp5 = ((s64)temp3 * *arg0 + (s64)temp2 * *arg1) / 0x1000;
        temp6 = ((s64)-temp2 * *arg0 + (s64)temp3 * *arg1) / 0x1000;
        if (temp6 > 0) {
            temp6 = -temp6;
        }

        *arg0 = ((s64)temp3 * temp5 - (s64)temp2 * temp6) / 0x1000;
        *arg1 = ((s64)temp2 * temp5 + (s64)temp3 * temp6) / 0x1000;

        D_80121BBC = -arg2 - D_80121BB0;
        D_80121BC0 += ((s64)-temp2 * D_80121BBC) / 0x1000;
        D_80121BC8 += ((s64)temp3 * D_80121BBC) / 0x1000;
    }
}

// func_8007DC38 best match: 52.431% (base.c)
#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_8007DC38.s")

// func_8007ECF4 best match: 86.996% (base_3.c)
#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_8007ECF4.s")

#ifdef NON_MATCHING
void func_8007ECF4(s16 arg0, s32 x, s32 z, s32 radius, s32 *pushX, s32 *pushZ, s32 *velocityX,
                  s32 *velocityZ) {
    ModelAnimKeyframe *keyframe;
    ModelAnimCoord *coord0;
    ModelAnimCoord *coord1;
    ModelAnimCoord *coord2;
    ModelAnimCoord *coord3;
    s32 keyframeOffset;
    s32 x0;
    s32 x1;
    s32 x2;
    s32 x3;
    s32 z0;
    s32 z1;
    s32 z2;
    s32 z3;
    s32 length;
    s32 normalX;
    s32 tangentX;
    s32 tangentZ;
    s32 alongEdge;
    s32 distanceFromEdge;
    s32 edgeLength;

    keyframeOffset = arg0 * sizeof(ModelAnimKeyframe);
    keyframe = (ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset);
    coord0 = &D_80121B90[keyframe->coordIndices[0]];
    coord1 = &D_80121B90[keyframe->coordIndices[1]];
    coord2 = &D_80121B90[keyframe->coordIndices[2]];
    coord3 = &D_80121B90[keyframe->coordIndices[3]];

    x0 = coord0->x << 0x11;
    x1 = coord1->x << 0x11;
    x2 = coord2->x << 0x11;
    x3 = coord3->x << 0x11;
    z0 = coord0->z << 0x11;
    z1 = coord1->z << 0x11;
    z2 = coord2->z << 0x11;
    z3 = coord3->z << 0x11;

    D_80121BC0 = x;
    D_80121BC8 = z;

    if (keyframe->nextFaceIndices[0] < 0) {
        D_80121BB4 = x1 - x0;
        D_80121BBC = z1 - z0;
        D_80121BA8 = x - x0;
        D_80121BB0 = z - z0;
        func_8007D87C(velocityX, velocityZ, radius);
        keyframe = (ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset);
    }

    if (keyframe->nextFaceIndices[1] < 0) {
        D_80121BB4 = x2 - x3;
        D_80121BBC = z2 - z3;
        D_80121BA8 = D_80121BC0 - x3;
        D_80121BB0 = D_80121BC8 - z3;
        func_8007D87C(velocityX, velocityZ, radius);
        keyframe = (ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset);
    }

    if (keyframe->unk4[0] < 0) {
        D_80121BB4 = x3 - x1;
        D_80121BBC = z3 - z1;
        D_80121BA8 = D_80121BC0 - x1;
        D_80121BB0 = D_80121BC8 - z1;

        length = integerSquareRoot64((s64)D_80121BB4 * D_80121BB4 + (s64)D_80121BBC * D_80121BBC);
        normalX = ((s64)D_80121BBC * 0x1000) / length;
        tangentX = ((s64)D_80121BB4 * 0x1000) / length;
        alongEdge = ((s64)tangentX * D_80121BA8 + (s64)normalX * D_80121BB0) / 0x1000;

        tangentZ = -normalX;
        distanceFromEdge = ((s64)tangentZ * D_80121BA8 + (s64)tangentX * D_80121BB0) / 0x1000;
        edgeLength = ((s64)tangentX * D_80121BB4 + (s64)normalX * D_80121BBC) / 0x1000;
        if ((keyframe->unk14[3] & 2) && (alongEdge < 0)) {
            s32 dx;
            s32 dz;
            s32 distance;

            dx = D_80121BC0 - x1;
            dz = D_80121BC8 - z1;
            distance = integerSquareRoot64((s64)dx * dx + (s64)dz * dz);
            if (distance < radius) {
                D_80121BC0 = ((s64)dx * radius) / distance + x1;
                D_80121BC8 = ((s64)dz * radius) / distance + z1;
            }
        } else if ((keyframe->unk14[3] & 8) && (edgeLength < alongEdge)) {
            s32 dx;
            s32 dz;
            s32 distance;

            dx = D_80121BC0 - x3;
            dz = D_80121BC8 - z3;
            distance = integerSquareRoot64((s64)dx * dx + (s64)dz * dz);
            if (distance < radius) {
                D_80121BC0 = ((s64)dx * radius) / distance + x3;
                D_80121BC8 = ((s64)dz * radius) / distance + z3;
            }
        } else {
            if (-radius < distanceFromEdge) {
                s32 projectedVelocityX;
                s32 projectedVelocityZ;

                projectedVelocityX = ((s64)tangentX * *velocityX + (s64)normalX * *velocityZ) / 0x1000;
                projectedVelocityZ = ((s64)tangentZ * *velocityX + (s64)tangentX * *velocityZ) / 0x1000;
                if (projectedVelocityZ > 0) {
                    projectedVelocityZ = -projectedVelocityZ;
                }

                *velocityX = ((s64)tangentX * projectedVelocityX - (s64)normalX * projectedVelocityZ) / 0x1000;
                *velocityZ = ((s64)normalX * projectedVelocityX + (s64)tangentX * projectedVelocityZ) / 0x1000;

                D_80121BBC = -radius - distanceFromEdge;
                D_80121BC0 += ((s64)tangentZ * D_80121BBC) / 0x1000;
                D_80121BC8 += ((s64)tangentX * D_80121BBC) / 0x1000;
            }
        }

        keyframe = (ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset);
    }

    if (keyframe->unk4[1] < 0) {
        D_80121BB4 = x0 - x2;
        D_80121BBC = z0 - z2;
        D_80121BA8 = D_80121BC0 - x2;
        D_80121BB0 = D_80121BC8 - z2;

        length = integerSquareRoot64((s64)D_80121BB4 * D_80121BB4 + (s64)D_80121BBC * D_80121BBC);
        normalX = ((s64)D_80121BBC * 0x1000) / length;
        tangentX = ((s64)D_80121BB4 * 0x1000) / length;
        alongEdge = ((s64)tangentX * D_80121BA8 + (s64)normalX * D_80121BB0) / 0x1000;

        tangentZ = -normalX;
        distanceFromEdge = ((s64)tangentZ * D_80121BA8 + (s64)tangentX * D_80121BB0) / 0x1000;
        edgeLength = ((s64)tangentX * D_80121BB4 + (s64)normalX * D_80121BBC) / 0x1000;
        if ((keyframe->unk14[3] & 4) && (alongEdge < 0)) {
            s32 dx;
            s32 dz;
            s32 distance;

            dx = D_80121BC0 - x2;
            dz = D_80121BC8 - z2;
            distance = integerSquareRoot64((s64)dx * dx + (s64)dz * dz);
            if (distance < radius) {
                D_80121BC0 = ((s64)dx * radius) / distance + x2;
                D_80121BC8 = ((s64)dz * radius) / distance + z2;
            }
        } else if ((keyframe->unk14[3] & 1) && (edgeLength < alongEdge)) {
            s32 dx;
            s32 dz;
            s32 distance;

            dx = D_80121BC0 - x0;
            dz = D_80121BC8 - z0;
            distance = integerSquareRoot64((s64)dx * dx + (s64)dz * dz);
            if (distance < radius) {
                D_80121BC0 = ((s64)dx * radius) / distance + x0;
                D_80121BC8 = ((s64)dz * radius) / distance + z0;
            }
        } else {
            if (-radius < distanceFromEdge) {
                s32 projectedVelocityX;
                s32 projectedVelocityZ;

                projectedVelocityX = ((s64)tangentX * *velocityX + (s64)normalX * *velocityZ) / 0x1000;
                projectedVelocityZ = ((s64)tangentZ * *velocityX + (s64)tangentX * *velocityZ) / 0x1000;
                if (projectedVelocityZ > 0) {
                    projectedVelocityZ = -projectedVelocityZ;
                }

                *velocityX = ((s64)tangentX * projectedVelocityX - (s64)normalX * projectedVelocityZ) / 0x1000;
                *velocityZ = ((s64)normalX * projectedVelocityX + (s64)tangentX * projectedVelocityZ) / 0x1000;

                D_80121BBC = -radius - distanceFromEdge;
                D_80121BC0 += ((s64)tangentZ * D_80121BBC) / 0x1000;
                D_80121BC8 += ((s64)tangentX * D_80121BBC) / 0x1000;
            }
        }
    }

    *pushX = D_80121BC0 - x;
    *pushZ = D_80121BC8 - z;
}
#endif

void func_8007FD88(s32 arg0) {
    s32 temp_v1;
    s32 temp_v0;
    s32 length;
    s32 temp_a1;

    length = integerSquareRoot64((s64)D_80121BB4 * D_80121BB4 + (s64)D_80121BBC * D_80121BBC);
    temp_v1 = ((s64)D_80121BBC * 0x1000) / length;
    temp_v0 = ((s64)D_80121BB4 * 0x1000) / length;

    temp_a1 = -temp_v1;
    D_80121BB0 = ((s64)temp_a1 * D_80121BA8 + (s64)temp_v0 * D_80121BB0) / 0x1000;

    if (-arg0 < D_80121BB0) {
        D_80121BBC = -arg0 - D_80121BB0;
        D_80121BC0 += ((s64)temp_a1 * D_80121BBC) / 0x1000;
        D_80121BC8 += ((s64)temp_v0 * D_80121BBC) / 0x1000;
    }
}

// func_8007FF88 best match: 74.835% (base_3.c)
#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_8007FF88.s")

#ifdef NON_MATCHING
void func_8007FF88(s16 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4, s32 *arg5) {
    ModelAnimKeyframe *keyframe;
    s32 keyframeOffset;
    s32 x0;
    s32 x1;
    s32 x2;
    s32 x3;
    s32 z0;
    s32 z1;
    s32 z2;
    s32 z3;
    s32 length;
    s32 nx;
    s32 nz;
    s32 along;
    s32 side;
    s32 edgeLength;
    s32 deltaX;
    s32 deltaZ;
    s32 dist;
    u16 flags;

    keyframe = &D_80121B98[arg0];
    x0 = D_80121B90[keyframe->coordIndices[0]].x << 0x11;
    x1 = D_80121B90[keyframe->coordIndices[1]].x << 0x11;
    x2 = D_80121B90[keyframe->coordIndices[2]].x << 0x11;
    x3 = D_80121B90[keyframe->coordIndices[3]].x << 0x11;
    z0 = D_80121B90[keyframe->coordIndices[0]].z << 0x11;
    z1 = D_80121B90[keyframe->coordIndices[1]].z << 0x11;
    z2 = D_80121B90[keyframe->coordIndices[2]].z << 0x11;
    z3 = D_80121B90[keyframe->coordIndices[3]].z << 0x11;

    D_80121BC0 = arg1;
    D_80121BC8 = arg2;
    keyframeOffset = arg0 * sizeof(ModelAnimKeyframe);

    if (keyframe->nextFaceIndices[0] < 0) {
        D_80121BB4 = x1 - x0;
        D_80121BBC = z1 - z0;
        D_80121BA8 = arg1 - x0;
        D_80121BB0 = arg2 - z0;
        func_8007FD88(arg3);
        keyframe = (ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset);
    }

    if (keyframe->nextFaceIndices[1] < 0) {
        D_80121BB4 = x2 - x3;
        D_80121BBC = z2 - z3;
        D_80121BA8 = D_80121BC0 - x3;
        D_80121BB0 = D_80121BC8 - z3;
        func_8007FD88(arg3);
        keyframe = (ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset);
    }

    if (keyframe->unk4[0] < 0) {
        D_80121BB4 = x3 - x1;
        D_80121BBC = z3 - z1;
        D_80121BA8 = D_80121BC0 - x1;
        D_80121BB0 = D_80121BC8 - z1;

        length = integerSquareRoot64((s64)D_80121BB4 * D_80121BB4 + (s64)D_80121BBC * D_80121BBC);
        nx = ((s64)D_80121BBC * 0x1000) / length;
        nz = ((s64)D_80121BB4 * 0x1000) / length;
        along = ((s64)nz * D_80121BA8 + (s64)nx * D_80121BB0) / 0x1000;
        side = ((s64)-nx * D_80121BA8 + (s64)nz * D_80121BB0) / 0x1000;
        edgeLength = ((s64)nz * D_80121BB4 + (s64)nx * D_80121BBC) / 0x1000;
        flags = ((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->unk14[3];

        if ((flags & 2) && (along < 0)) {
            deltaX = D_80121BC0 - x1;
            deltaZ = D_80121BC8 - z1;
            dist = integerSquareRoot64((s64)deltaX * deltaX + (s64)deltaZ * deltaZ);
            if (dist < arg3) {
                D_80121BC0 = (((s64)deltaX * arg3) / dist) + x1;
                D_80121BC8 = (((s64)deltaZ * arg3) / dist) + z1;
            }
            keyframe = (ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset);
        } else {
            if (flags & 8) {
                if (edgeLength < along) {
                    deltaX = D_80121BC0 - x3;
                    deltaZ = D_80121BC8 - z3;
                    dist = integerSquareRoot64((s64)deltaX * deltaX + (s64)deltaZ * deltaZ);
                    if (dist < arg3) {
                        D_80121BC0 = (((s64)deltaX * arg3) / dist) + x3;
                        D_80121BC8 = (((s64)deltaZ * arg3) / dist) + z3;
                    }
                    keyframe = (ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset);
                } else if (-arg3 < side) {
                    D_80121BBC = -arg3 - side;
                    D_80121BC0 += ((s64)-nx * D_80121BBC) / 0x1000;
                    D_80121BC8 += ((s64)nz * D_80121BBC) / 0x1000;
                    keyframe = (ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset);
                }
            } else if (-arg3 < side) {
                D_80121BBC = -arg3 - side;
                D_80121BC0 += ((s64)-nx * D_80121BBC) / 0x1000;
                D_80121BC8 += ((s64)nz * D_80121BBC) / 0x1000;
                keyframe = (ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset);
            }
        }
    }

    if (keyframe->unk4[1] < 0) {
        D_80121BB4 = x0 - x2;
        D_80121BBC = z0 - z2;
        D_80121BA8 = D_80121BC0 - x2;
        D_80121BB0 = D_80121BC8 - z2;

        length = integerSquareRoot64((s64)D_80121BB4 * D_80121BB4 + (s64)D_80121BBC * D_80121BBC);
        nx = ((s64)D_80121BBC * 0x1000) / length;
        nz = ((s64)D_80121BB4 * 0x1000) / length;
        along = ((s64)nz * D_80121BA8 + (s64)nx * D_80121BB0) / 0x1000;
        side = ((s64)-nx * D_80121BA8 + (s64)nz * D_80121BB0) / 0x1000;
        edgeLength = ((s64)nz * D_80121BB4 + (s64)nx * D_80121BBC) / 0x1000;
        flags = ((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->unk14[3];

        if ((flags & 4) && (along < 0)) {
            deltaX = D_80121BC0 - x2;
            deltaZ = D_80121BC8 - z2;
            dist = integerSquareRoot64((s64)deltaX * deltaX + (s64)deltaZ * deltaZ);
            if (dist < arg3) {
                D_80121BC0 = (((s64)deltaX * arg3) / dist) + x2;
                D_80121BC8 = (((s64)deltaZ * arg3) / dist) + z2;
            }
        } else {
            if (flags & 1) {
                if (edgeLength < along) {
                    deltaX = D_80121BC0 - x0;
                    deltaZ = D_80121BC8 - z0;
                    dist = integerSquareRoot64((s64)deltaX * deltaX + (s64)deltaZ * deltaZ);
                    if (dist < arg3) {
                        D_80121BC0 = (((s64)deltaX * arg3) / dist) + x0;
                        D_80121BC8 = (((s64)deltaZ * arg3) / dist) + z0;
                    }
                } else if (-arg3 < side) {
                    D_80121BBC = -arg3 - side;
                    D_80121BC0 += ((s64)-nx * D_80121BBC) / 0x1000;
                    D_80121BC8 += ((s64)nz * D_80121BBC) / 0x1000;
                }
            } else if (-arg3 < side) {
                D_80121BBC = -arg3 - side;
                D_80121BC0 += ((s64)-nx * D_80121BBC) / 0x1000;
                D_80121BC8 += ((s64)nz * D_80121BBC) / 0x1000;
            }
        }
    }

    *arg4 = D_80121BC0 - arg1;
    *arg5 = D_80121BC8 - arg2;
}
#endif

// func_80080CC4 best match: 82.954% (base_10.c)
#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80080CC4.s")

#ifdef NON_MATCHING
s32 func_80080CC4(s32 arg0, s32 arg1, s32 arg2) {
    s32 keyframeOffset;
    s32 faceIndex;
    s32 faceOffset;
    ModelAnimFace *face;
    ModelAnimCoord *coord0;
    ModelAnimCoord *coord1;
    ModelAnimCoord *coord2;
    ModelAnimCoord *coords;
    s64 lhs;
    s64 rhs;
    s16 x20;
    s16 z20;
    s16 y20;
    s16 x10;
    s16 z10;
    s16 y10;
    s32 xDelta;
    s32 zDelta;
    s32 numeratorA;
    s32 numeratorB;
    s32 denominator;

    keyframeOffset = arg0 * sizeof(ModelAnimKeyframe);
    faceIndex = ((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->unk14[0];
    if (faceIndex < ((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->unk14[1]) {
        faceOffset = faceIndex * sizeof(ModelAnimFace);
        do {
            face = (ModelAnimFace *)((s32)D_80121B94 + faceOffset);
            if (face->unk7 == 0) {
                coords = D_80121B90;
                coord0 = (ModelAnimCoord *)((s32)coords + face->coord0 * sizeof(ModelAnimCoord));
                lhs = (s64)((coords[face->coord1].x << 0x11) - (coord0->x << 0x11)) *
                    (arg2 - (coord0->z << 0x11));

                face = (ModelAnimFace *)((s32)D_80121B94 + faceOffset);
                coords = D_80121B90;
                coord0 = (ModelAnimCoord *)((s32)coords + face->coord0 * sizeof(ModelAnimCoord));
                rhs = (s64)((coords[face->coord1].z << 0x11) - (coord0->z << 0x11)) *
                    (arg1 - (coord0->x << 0x11));

                if (lhs - rhs < 0) {
                    goto next;
                }

                face = (ModelAnimFace *)((s32)D_80121B94 + faceOffset);
            }

            coords = D_80121B90;
            coord1 = (ModelAnimCoord *)((s32)coords + face->coord1 * sizeof(ModelAnimCoord));
            lhs = (s64)((coords[face->coord2].x << 0x11) - (coord1->x << 0x11)) *
                (arg2 - (coord1->z << 0x11));

            face = (ModelAnimFace *)((s32)D_80121B94 + faceOffset);
            coords = D_80121B90;
            coord1 = (ModelAnimCoord *)((s32)coords + face->coord1 * sizeof(ModelAnimCoord));
            rhs = (s64)((coords[face->coord2].z << 0x11) - (coord1->z << 0x11)) *
                (arg1 - (coord1->x << 0x11));

            if (lhs - rhs >= 0) {
                face = (ModelAnimFace *)((s32)D_80121B94 + faceOffset);
                coords = D_80121B90;
                coord2 = (ModelAnimCoord *)((s32)coords + face->coord2 * sizeof(ModelAnimCoord));
                coord0 = (ModelAnimCoord *)((s32)coords + face->coord0 * sizeof(ModelAnimCoord));
                coord1 = (ModelAnimCoord *)((s32)coords + face->coord1 * sizeof(ModelAnimCoord));

                x20 = coord2->x - coord0->x;
                z20 = coord2->z - coord0->z;
                y20 = coord2->y - coord0->y;
                x10 = coord1->x - coord0->x;
                z10 = coord1->z - coord0->z;
                y10 = coord1->y - coord0->y;
                xDelta = arg1 - (coord0->x << 0x11);
                zDelta = arg2 - (coord0->z << 0x11);

                numeratorA = (y20 * z10) - (z20 * y10);
                numeratorB = (x20 * y10) - (y20 * x10);
                denominator = (z20 * x10) - (x20 * z10);

                return (-((s64)numeratorA * xDelta) - ((s64)numeratorB * zDelta)) / denominator
                    + (D_80121B90[((ModelAnimFace *)((s32)D_80121B94 + faceOffset))->coord0].y << 0x11);
            }

next:
            faceIndex++;
            faceOffset += sizeof(ModelAnimFace);
        } while (faceIndex < ((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->unk14[1]);
    }

    return 0xC0000000;
}
#endif

s32 func_80081124(s32 arg0, s32 arg1, s32 arg2) {
    s32 keyframeOffset;
    s32 faceIndex;
    s32 faceOffset;
    ModelAnimFace *face;
    ModelAnimCoord *coord;
    ModelAnimCoord *coords;
    s64 pad;
    s64 rhs;
    s64 lhs;

    keyframeOffset = arg0 * sizeof(ModelAnimKeyframe);
    faceIndex = ((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->unk14[0];
    if (faceIndex < ((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->unk14[1]) {
        faceOffset = faceIndex * sizeof(ModelAnimFace);
        do {
            face = (ModelAnimFace *)((s32)D_80121B94 + faceOffset);
            if (face->unk7 == 0) {
                coords = D_80121B90;
                coord = (ModelAnimCoord *)((s32)coords + face->coord0 * sizeof(ModelAnimCoord));
                lhs = (s64)((coords[face->coord1].x << 0x11) - (coord->x << 0x11)) *
                    (arg2 - (coord->z << 0x11));

                face = (ModelAnimFace *)((s32)D_80121B94 + faceOffset);
                coords = D_80121B90;
                coord = (ModelAnimCoord *)((s32)coords + face->coord0 * sizeof(ModelAnimCoord));
                rhs = (s64)((coords[face->coord1].z << 0x11) - (coord->z << 0x11)) *
                    (arg1 - (coord->x << 0x11));

                if (lhs - rhs < 0) {
                    goto next;
                }

                face = (ModelAnimFace *)((s32)D_80121B94 + faceOffset);
            }

            coords = D_80121B90;
            coord = (ModelAnimCoord *)((s32)coords + face->coord1 * sizeof(ModelAnimCoord));
            lhs = (s64)((coords[face->coord2].x << 0x11) - (coord->x << 0x11)) *
                (arg2 - (coord->z << 0x11));

            face = (ModelAnimFace *)((s32)D_80121B94 + faceOffset);
            coords = D_80121B90;
            coord = (ModelAnimCoord *)((s32)coords + face->coord1 * sizeof(ModelAnimCoord));
            rhs = (s64)((coords[face->coord2].z << 0x11) - (coord->z << 0x11)) *
                (arg1 - (coord->x << 0x11));

            if (lhs - rhs >= 0) {
                if ((!coord->x) && (!coord->x)) {
                }
                return ((ModelAnimFace *)((s32)D_80121B94 + faceOffset))->faceIndex;
            }

next:
            faceIndex++;
            faceOffset += sizeof(ModelAnimFace);
        } while (faceIndex < ((ModelAnimKeyframe *)((s32)D_80121B98 + (arg0 * sizeof(ModelAnimKeyframe))))->unk14[1]);
    }

    return 0;
}

u32 func_800813F8(s32 arg0, s32 arg1, s32 arg2) {
    ModelAnimKeyframe *keyframe = &D_80121B98[arg0];

    s32 deltaX = arg1 - (D_80121B90[D_80121B98[arg0].positionIndex].x << 0x11);
    s32 deltaZ = arg2 - (D_80121B90[D_80121B98[arg0].positionIndex].z << 0x11);

    D_80121B9C = fixedSine(D_80121B98[arg0].angle);
    D_80121BA0 = fixedCosine(D_80121B98[arg0].angle);

    return ((s64)(-D_80121B9C) * deltaX + (s64)D_80121BA0 * deltaZ) / 0x1000;
}

void func_80081508(s32 arg0, s32 *x, s32 *y, s32 *z, s16 *angle) {
    ModelAnimCoord **coordTablePtr;
    s32 keyframeOffset;

    keyframeOffset = arg0 * sizeof(ModelAnimKeyframe);
    coordTablePtr = &D_80121B90;
    *x = ((ModelAnimCoord *)((s32)*coordTablePtr +
            (((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->positionIndex * sizeof(ModelAnimCoord))))->x << 0x11;
    *y = ((ModelAnimCoord *)((s32)*coordTablePtr +
            (((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->positionIndex * sizeof(ModelAnimCoord))))->y << 0x11;
    *z = ((ModelAnimCoord *)((s32)*coordTablePtr +
            (((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->positionIndex * sizeof(ModelAnimCoord))))->z << 0x11;
    *angle = -((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->angle;
}

// func_800815D4 best match: 93.956% (base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_800815D4.s")

#ifdef NON_MATCHING
void func_800815D4(s32 arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4, s32 arg5, s32 arg6) {
    s32 keyframeOffset;
    s32 deltaX;
    s32 deltaZ;
    s32 projected;
    s32 distance;
    s16 pathIndex;
    s32 nextKeyframeOffset;
    ModelAnimCoord *coord;

    if (arg0 != D_800B9540[D_80121B50].pathIndex) {
        keyframeOffset = arg0 * sizeof(ModelAnimKeyframe);
        D_80121B9C = fixedSine(((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->angle);
        D_80121BA0 = fixedCosine(((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->angle);

        coord = &D_80121B90[((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->positionIndex];
        deltaX = arg1 - (coord->x << 0x11);
        deltaZ = arg2 - (coord->z << 0x11);
        projected = ((s64)-D_80121B9C * deltaX + (s64)D_80121BA0 * deltaZ) / 0x1000;

        if ((arg0 >= D_800B9540[D_80121B50].unk38) && (D_800B9540[D_80121B50].unk3A >= arg0)) {
            distance = projected + 0xC00000;
            *arg3 = ((s64)-D_80121B9C * distance) / 0x1000;
            *arg4 = ((s64)D_80121BA0 * distance) / 0x1000;
        } else if ((D_80121B50 == 3) && ((arg0 == 0x11D) || (arg0 == 0x11E))) {
            distance = projected + 0xC00000;
            *arg3 = ((s64)-D_80121B9C * distance) / 0x1000;
            *arg4 = ((s64)D_80121BA0 * distance) / 0x1000;
        } else {
            distance = projected + 0xFF400000;
            *arg3 = ((s64)-D_80121B9C * distance) / 0x1000;
            *arg4 = ((s64)D_80121BA0 * distance) / 0x1000;
        }

        *arg3 += D_80121B90[((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->positionIndex].x << 0x11;
        *arg4 += D_80121B90[((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->positionIndex].z << 0x11;

        pathIndex = func_8007D200(arg0, *arg3, *arg4);
        D_80121B9C = fixedSine(D_80121B98[pathIndex].angle);
        nextKeyframeOffset = pathIndex * sizeof(ModelAnimKeyframe);
        D_80121BA0 = fixedCosine(((ModelAnimKeyframe *)((s32)D_80121B98 + nextKeyframeOffset))->angle);

        coord = &D_80121B90[((ModelAnimKeyframe *)((s32)D_80121B98 + nextKeyframeOffset))->positionIndex];
        deltaZ = *arg4 - (coord->z << 0x11);
        deltaX = *arg3 - (coord->x << 0x11);
        distance = func_8007BCFC(arg5, pathIndex, arg6);
        projected = ((s64)-D_80121B9C * deltaX + (s64)D_80121BA0 * deltaZ) / 0x1000;

        *arg3 = ((s64)D_80121BA0 * distance + (s64)-D_80121B9C * projected) / 0x1000;
        *arg4 = ((s64)D_80121B9C * distance + (s64)D_80121BA0 * projected) / 0x1000;
        *arg3 += D_80121B90[((ModelAnimKeyframe *)((s32)D_80121B98 + nextKeyframeOffset))->positionIndex].x << 0x11;
        *arg4 += D_80121B90[((ModelAnimKeyframe *)((s32)D_80121B98 + nextKeyframeOffset))->positionIndex].z << 0x11;
    } else {
        *arg3 = D_800B9540[D_80121B50].unk18;
        *arg4 = D_800B9540[D_80121B50].unk1C;
    }
}
#endif

void func_80081C44(s32 arg0, s32 *arg1, s32 *arg2, s32 arg3) {
    s32 unused[4];
    s32 temp_v0;

    if (arg0 != D_800B9540[D_80121B50].pathIndex) {
        D_80121B9C = fixedSine(D_80121B98[arg0].angle);
        D_80121BA0 = fixedCosine(D_80121B98[arg0].angle);
        temp_v0 = func_8007BDE4(arg3, arg0);
        *arg1 = ((s64)D_80121BA0 * temp_v0) / 0x1000;
        *arg2 = ((s64)D_80121B9C * temp_v0) / 0x1000;
        *arg1 += D_80121B90[D_80121B98[arg0].positionIndex].x << 0x11;
        *arg2 += D_80121B90[D_80121B98[arg0].positionIndex].z << 0x11;
    } else {
        *arg1 = D_800B9540[D_80121B50].unk40;
        *arg2 = D_800B9540[D_80121B50].unk44;
    }
}

s16 func_80081E1C(s32 arg0) {
    return D_80121B98[arg0].nextFaceIndices[1];
}

void func_80081E40(ModelAnimState *state, s32 animIndex) {
    s16 *temp_a1;
    s16 temp_v1;
    s32 temp_v1_2;
    s16 *sp1C;
    s32 temp_v0;
    ModelAnimState *temp_a2 = state;

    temp_v0 = getMemoryBlockBase(D_8011215C[state->modelId]);
    temp_a1 = (s16 *)(temp_v0 + (((u16 *)temp_v0)[animIndex] * 2));
    temp_v1 = *temp_a1;
    temp_a2->framesRemaining = temp_v1;
    temp_a2->framesRemaining++;
    temp_a2->frameCount = temp_v1;
    temp_a1 += 1;
    sp1C = temp_a1;
    temp_v1_2 = (s32)temp_a1 - getMemoryBlockBase(D_8011215C[temp_a2->modelId]);
    temp_a2->animStartOffset = temp_v1_2;
    temp_a2->frameDataOffset = temp_v1_2;
    temp_a2->frameTimer = 0;
    temp_a2->animIndex = animIndex;
}

// func_80081EF4 best match: 99.309% (base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80081EF4.s")

#ifdef NON_MATCHING
void func_80081EF4(ModelAnimState *state) {
    s16 *frameData;
    ModelAnimState *cursor;
    s32 i;
    s32 base;
    s16 value0;
    s16 value1;
    s16 angle;

    base = getMemoryBlockBase(D_8011215C[state->modelId]);
    state->frameTimerReset = 1;
    frameData = (s16 *)(base + state->frameDataOffset);
    state->frameTimer = state->frameTimerReset;

    cursor = state;
    for (i = 0; i < 2; i++) {
        cursor->joints[0].unkC = frameData[0] << 14;
        cursor->joints[0].unk10 = frameData[1] << 14;
        cursor = (ModelAnimState *)((s32)cursor + sizeof(ModelAnimStateJoint));
        cursor->joints[0].unk0 = frameData[2] << 14;
        frameData += 3;
    }

    cursor = state;
    for (i = 0; i != sizeof(ModelAnimStateJoint) * 12; i += sizeof(ModelAnimStateJoint)) {
        value0 = frameData[0];
        value1 = frameData[1];
        frameData += 2;
        cursor->joints[0].unk6 = (value0 >> 4) & 0xFF0;
        cursor->joints[0].unk8 = (value0 << 4) & 0xFF0;
        cursor->joints[0].unkA = (value1 >> 4) & 0xFF0;
        if (value1 & 1) {
            cursor->joints[0].unk6 += 8;
        }
        if (value1 & 2) {
            cursor->joints[0].unk8 += 8;
        }
        if (value1 & 4) {
            cursor->joints[0].unkA += 8;
        }
        cursor = (ModelAnimState *)((s32)cursor + sizeof(ModelAnimStateJoint));
    }

    state->frameDataOffset = (s32)frameData - getMemoryBlockBase(D_8011215C[state->modelId]);
    angle = state->joints[0].unk6;
    state->unk43E = angle;
    state->unk42A = angle;
    angle = state->joints[0].unk8;
    state->unk440 = angle;
    state->unk42C = angle;
    angle = state->joints[0].unkA;
    state->unk442 = angle;
    state->unk42E = angle;
}
#endif

void func_80082070(ModelAnimState *state) {
    s16 *data;
    s32 base;
    ModelAnimFrameCursor *cursor;
    s32 one;
    s32 offset;

    base = getMemoryBlockBase(D_8011215C[state->modelId]);
    data = (s16 *)(((one = base) + state->frameDataOffset) + 0x24);
    state->frameTimerReset = 1;
    offset = 0x78;
    cursor = (ModelAnimFrameCursor *)((u8 *)state + 0x78);
    state->frameTimer = state->frameTimerReset;
    one = 1;

    do {
        s16 packed0 = data[0];
        s16 packed1 = data[one];

        data += 2;
        cursor->x = (packed0 >> 4) & 0xFF0;
        cursor->y = (packed0 << 4) & 0xFF0;
        cursor->z = (packed1 >> 4) & 0xFF0;

        if (packed1 & one) {
            cursor->x += 8;
        }
        if (packed1 & 2) {
            cursor->y += 8;
        }
        offset += 0x14;
        if (packed1 & 4) {
            cursor->z += 8;
        }
        cursor = (ModelAnimFrameCursor *)((u8 *)cursor + 0x14);
    } while (offset != 0xF0);

    state->frameDataOffset = (s32)data - getMemoryBlockBase(D_8011215C[state->modelId]);
}

// func_80082184 best match: 78.465% (base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80082184.s")

#ifdef NON_MATCHING
extern s32 D_80121BD0[];
extern s32 D_80121C00[];
extern s32 D_80121D50;

void func_80082184(ModelAnimState *state, s32 animIndex, s32 frameTimer, s32 frameTimerReset) {
    s16 *data;
    s32 base;
    s32 count;
    s32 offset;
    s32 *rotFrame;
    s32 *rot;
    s32 *posFrame;
    s32 *pos;
    ModelAnimState *cursor;
    s32 start;
    s32 delta;
    s32 x;
    s32 y;
    s32 z;
    s16 packed0;
    s16 packed1;
    s32 stride;
    s32 rotBase;

    base = getMemoryBlockBase(D_8011215C[state->modelId]);
    data = (s16 *)(base + (((u16 *)base)[animIndex] * 2) + 2);

    stride = 0xC;
    rotFrame = D_80121BD0;
    posFrame = D_80121C00;
    do {
        count = 0;
        rot = rotFrame;
loop_rot:
        rot[0] = data[0] << 14;
        rot[1] = data[1] << 14;
        rot[2] = data[2] << 14;
        count++;
        rot += 3;
        data += 3;
        if (count < 2) {
            goto loop_rot;
        }

        offset = 0;
        pos = posFrame;
loop_pos:
        packed0 = data[0];
        packed1 = data[1];
        x = (packed0 >> 4) & 0xFF0;
        y = (packed0 << 4) & 0xFF0;
        z = (packed1 >> 4) & 0xFF0;
        data += 2;
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        if (packed1 & 1) {
            pos[0] = x + 8;
        }
        if (packed1 & 2) {
            pos[1] = y + 8;
        }
        offset += stride;
        if (packed1 & 4) {
            pos[2] = z + 8;
        }
        pos += 3;
        if (offset < 0x90) {
            goto loop_pos;
        }

        posFrame += 0x2A;
        rotFrame += 6;
    } while ((u32)posFrame < (u32)&D_80121D50);

    pos = D_80121C00;
    count = 0;
    cursor = state;
loop_interp:
    start = pos[0];
    delta = (pos[0x2A] - start) & 0xFFF;
    if (delta >= 0x801) {
        delta -= 0x1000;
    }
    *(s16 *)((u8 *)cursor + 0x33A) = start + ((delta * frameTimer) / frameTimerReset);

    start = pos[1];
    delta = (pos[0x2B] - start) & 0xFFF;
    if (delta >= 0x801) {
        delta -= 0x1000;
    }
    *(s16 *)((u8 *)cursor + 0x33C) = start + ((delta * frameTimer) / frameTimerReset);

    start = pos[2];
    delta = (pos[0x2C] - start) & 0xFFF;
    if (delta >= 0x801) {
        delta -= 0x1000;
    }
    count++;
    pos += 3;
    cursor = (ModelAnimState *)((u8 *)cursor + 0x14);
    *(s16 *)((u8 *)cursor + 0x32A) = start + ((delta * frameTimer) / frameTimerReset);
    if (count != 12) {
        goto loop_interp;
    }

    rotBase = (s32)D_80121BD0;
    *(s32 *)((u8 *)state + 0x340) =
        *(s32 *)(rotBase + 0x0) + (((*(s32 *)(rotBase + 0x18) - *(s32 *)(rotBase + 0x0)) * frameTimer) / frameTimerReset);
    *(s32 *)((u8 *)state + 0x344) =
        *(s32 *)(rotBase + 0x4) + (((*(s32 *)(rotBase + 0x1C) - *(s32 *)(rotBase + 0x4)) * frameTimer) / frameTimerReset);
    *(s32 *)((u8 *)state + 0x348) =
        *(s32 *)(rotBase + 0x8) + (((*(s32 *)(rotBase + 0x20) - *(s32 *)(rotBase + 0x8)) * frameTimer) / frameTimerReset);
    *(s32 *)((u8 *)state + 0x354) =
        *(s32 *)(rotBase + 0xC) + (((*(s32 *)(rotBase + 0x24) - *(s32 *)(rotBase + 0xC)) * frameTimer) / frameTimerReset);
    *(s32 *)((u8 *)state + 0x358) =
        *(s32 *)(rotBase + 0x10) + (((*(s32 *)(rotBase + 0x28) - *(s32 *)(rotBase + 0x10)) * frameTimer) / frameTimerReset);

    *(s16 *)((u8 *)state + 0x42A) = *(s16 *)((u8 *)state + 0x33A);
    *(s16 *)((u8 *)state + 0x43E) = *(s16 *)((u8 *)state + 0x33A);
    *(s16 *)((u8 *)state + 0x440) = *(s16 *)((u8 *)state + 0x33C);
    *(s16 *)((u8 *)state + 0x42C) = *(s16 *)((u8 *)state + 0x33C);
    *(s16 *)((u8 *)state + 0x442) = *(s16 *)((u8 *)state + 0x33E);
    *(s16 *)((u8 *)state + 0x42E) = *(s16 *)((u8 *)state + 0x33E);

    *(s32 *)((u8 *)state + 0x35C) =
        *(s32 *)(rotBase + 0x14) + (((*(s32 *)(rotBase + 0x2C) - *(s32 *)(rotBase + 0x14)) * frameTimer) / frameTimerReset);
}
#endif

// func_80082664 best match: 78.710% (base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80082664.s")

#ifdef NON_MATCHING
extern s32 D_80121BD0[];
extern s32 D_80121C00[];
extern s32 D_80121D50;

void func_80082664(ModelAnimState *state, s32 animIndex, s32 frameTimer, s32 frameTimerReset) {
    s16 *data;
    s32 base;
    s32 count;
    s32 offset;
    s32 *rotFrame;
    s32 *rot;
    s32 *posFrame;
    s32 *pos;
    ModelAnimState *cursor;
    s32 start;
    s32 delta;
    s32 x;
    s32 y;
    s32 z;
    s16 packed0;
    s16 packed1;
    s32 stride;
    s32 rotBase;

    base = getMemoryBlockBase(D_8011215C[state->modelId]);
    data = (s16 *)(base + (((u16 *)base)[animIndex] * 2) + 2);

    stride = 0xC;
    rotFrame = D_80121BD0;
    posFrame = D_80121C00;
    do {
        count = 0;
        rot = rotFrame;
loop_rot:
        rot[0] = data[0] << 14;
        rot[1] = data[1] << 14;
        rot[2] = data[2] << 14;
        count++;
        rot += 3;
        data += 3;
        if (count < 2) {
            goto loop_rot;
        }

        offset = 0;
        pos = posFrame;
loop_pos:
        packed0 = data[0];
        packed1 = data[1];
        x = (packed0 >> 4) & 0xFF0;
        y = (packed0 << 4) & 0xFF0;
        z = (packed1 >> 4) & 0xFF0;
        data += 2;
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        if (packed1 & 1) {
            pos[0] = x + 8;
        }
        if (packed1 & 2) {
            pos[1] = y + 8;
        }
        offset += stride;
        if (packed1 & 4) {
            pos[2] = z + 8;
        }
        pos += 3;
        if (offset < 0x3C) {
            goto loop_pos;
        }

        count = 5;
loop_skip:
        count++;
        data += 2;
        if (count < 12) {
            goto loop_skip;
        }

        posFrame += 0x2A;
        rotFrame += 6;
    } while ((u32)posFrame < (u32)&D_80121D50);

    pos = D_80121C00;
    count = 0;
    cursor = state;
loop_interp:
    start = pos[0];
    delta = (pos[0x2A] - start) & 0xFFF;
    if (delta >= 0x801) {
        delta -= 0x1000;
    }
    *(s16 *)((u8 *)cursor + 0x33A) = start + ((delta * frameTimer) / frameTimerReset);

    start = pos[1];
    delta = (pos[0x2B] - start) & 0xFFF;
    if (delta >= 0x801) {
        delta -= 0x1000;
    }
    *(s16 *)((u8 *)cursor + 0x33C) = start + ((delta * frameTimer) / frameTimerReset);

    start = pos[2];
    delta = (pos[0x2C] - start) & 0xFFF;
    if (delta >= 0x801) {
        delta -= 0x1000;
    }
    count++;
    pos += 3;
    cursor = (ModelAnimState *)((u8 *)cursor + 0x14);
    *(s16 *)((u8 *)cursor + 0x32A) = start + ((delta * frameTimer) / frameTimerReset);
    if (count != 5) {
        goto loop_interp;
    }

    rotBase = (s32)D_80121BD0;
    *(s32 *)((u8 *)state + 0x340) =
        *(s32 *)(rotBase + 0x0) + (((*(s32 *)(rotBase + 0x18) - *(s32 *)(rotBase + 0x0)) * frameTimer) / frameTimerReset);
    *(s32 *)((u8 *)state + 0x344) =
        *(s32 *)(rotBase + 0x4) + (((*(s32 *)(rotBase + 0x1C) - *(s32 *)(rotBase + 0x4)) * frameTimer) / frameTimerReset);
    *(s32 *)((u8 *)state + 0x348) =
        *(s32 *)(rotBase + 0x8) + (((*(s32 *)(rotBase + 0x20) - *(s32 *)(rotBase + 0x8)) * frameTimer) / frameTimerReset);
    *(s32 *)((u8 *)state + 0x354) =
        *(s32 *)(rotBase + 0xC) + (((*(s32 *)(rotBase + 0x24) - *(s32 *)(rotBase + 0xC)) * frameTimer) / frameTimerReset);
    *(s32 *)((u8 *)state + 0x358) =
        *(s32 *)(rotBase + 0x10) + (((*(s32 *)(rotBase + 0x28) - *(s32 *)(rotBase + 0x10)) * frameTimer) / frameTimerReset);

    *(s16 *)((u8 *)state + 0x42A) = *(s16 *)((u8 *)state + 0x33A);
    *(s16 *)((u8 *)state + 0x43E) = *(s16 *)((u8 *)state + 0x33A);
    *(s16 *)((u8 *)state + 0x440) = *(s16 *)((u8 *)state + 0x33C);
    *(s16 *)((u8 *)state + 0x42C) = *(s16 *)((u8 *)state + 0x33C);
    *(s16 *)((u8 *)state + 0x442) = *(s16 *)((u8 *)state + 0x33E);
    *(s16 *)((u8 *)state + 0x42E) = *(s16 *)((u8 *)state + 0x33E);

    *(s32 *)((u8 *)state + 0x35C) =
        *(s32 *)(rotBase + 0x14) + (((*(s32 *)(rotBase + 0x2C) - *(s32 *)(rotBase + 0x14)) * frameTimer) / frameTimerReset);
}
#endif

// func_80082B58 best match: 98.133% (base_12.c)
#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80082B58.s")

#ifdef NON_MATCHING
void func_80082B58(ModelAnimState *state, s32 animIndex, s32 timer, s32 duration) {
    s32 frameBase;
    s16 *packed;
    ModelAnimRotation *row;
    ModelAnimRotation *out;
    ModelAnimRotation *end;
    s32 *interp;
    ModelAnimRotation *outAlias;
    void *joint;
    s32 i;
    s16 xy;
    s16 shiftedX;
    s16 zAndFlags;
    s32 jointStride;
    s32 x;
    u16 frameOffset;
    s32 y;
    s32 z;
    s32 delta;

    frameBase = getMemoryBlockBase(D_8011215C[state->modelId]);
    frameOffset = ((u16 *)frameBase)[animIndex];
    packed = (s16 *)(frameBase + (frameOffset * 2));
    packed++;
    packed = (s16 *)((u8 *)packed + 0x24);

    row = D_80121C00;
    end = (ModelAnimRotation *)&D_80121D50;
    do {
        i = 0x48;
        out = row + 6;
        do {
            xy = packed[0];
            zAndFlags = packed[1];
            shiftedX = xy >> 4;
            x = shiftedX & 0xFF0;
            y = (xy << 4) & 0xFF0;
            z = (zAndFlags >> 4) & 0xFF0;
            packed += 2;
            out->x = x;
            out->y = y;
            out->z = z;
            if (zAndFlags & 1) {
                out->x = x + 8;
            }
            if ((zAndFlags & 2) & 0xFFFF) {
                out->y += 8;
            }
            i += 0xC;
            outAlias = out;
            if (zAndFlags & 4) {
                outAlias->z += 8;
            }
            out++;
        } while (i < 0x90);
        row += 14;
        packed = (s16 *)((u8 *)packed + 0x24);
    } while (row < end);

    interp = D_80121C48;
    i = 6;
    joint = (u8 *)state + 0x78;
loop:
    x = interp[0];
    jointStride = 0x14;
    delta = (interp[42] - x) & 0xFFF;
    if (delta >= 0x801) {
        delta -= 0x1000;
    }
    *(s16 *)((u8 *)joint + 0x33A) = x + ((delta * timer) / duration);

    y = interp[1];
    delta = (interp[43] - y) & 0xFFF;
    if (delta >= 0x801) {
        delta -= 0x1000;
    }
    *(s16 *)((u8 *)joint + 0x33C) = y + ((delta * timer) / duration);

    z = interp[2];
    delta = (interp[44] - z) & 0xFFF;
    if (delta >= 0x801) {
        delta -= 0x1000;
    }
    i++;
    interp += 3;
    joint = (u8 *)joint + jointStride;
    *(s16 *)((u8 *)joint + 0x32A) = z + ((delta * timer) / duration);
    if (i != 12) {
        goto loop;
    }
}
#endif

void func_80082DD0(ModelAnimState *state) {
    s32 frameTimer;
    s32 nextFrameTimer;

    frameTimer = state->frameTimer;
    nextFrameTimer = frameTimer - 1;
    if (frameTimer == 0) {
        state->framesRemaining--;
        if (state->framesRemaining <= 0) {
            state->framesRemaining = state->frameCount;
            state->frameDataOffset = state->animStartOffset;
        }
        func_80081EF4(state);
        frameTimer = state->frameTimer;
        nextFrameTimer = frameTimer - 1;
    }
    state->frameTimer = nextFrameTimer;
}

void func_80082E48(ModelAnimState *state) {
    s32 frameTimer;
    s32 nextFrameTimer;

    frameTimer = state->frameTimer;
    nextFrameTimer = frameTimer - 1;
    if (frameTimer == 0) {
        state->framesRemaining--;
        if (state->framesRemaining <= 0) {
            state->framesRemaining = state->frameCount;
            state->frameDataOffset = state->animStartOffset;
        }
        func_80082070(state);
        frameTimer = state->frameTimer;
        nextFrameTimer = frameTimer - 1;
    }
    state->frameTimer = nextFrameTimer;
}

s32 func_80082EC0(ModelAnimState *state) {
    s32 frameTimer;
    s32 nextFrameTimer;
    s16 framesRemaining;

    frameTimer = state->frameTimer;
    nextFrameTimer = frameTimer - 1;
    if (frameTimer == 0) {
        framesRemaining = state->framesRemaining;
        if (framesRemaining != 1) {
            state->framesRemaining = framesRemaining - 1;
            func_80081EF4(state);
            if (state->framesRemaining == 1) {
                return 1;
            }
            frameTimer = state->frameTimer;
            nextFrameTimer = frameTimer - 1;
        } else {
            return 1;
        }
    }
    state->frameTimer = nextFrameTimer;
    return 0;
}

s32 func_80082F44(ModelAnimState *state) {
    s32 frameTimer;
    s32 nextFrameTimer;
    s16 framesRemaining;

    frameTimer = state->frameTimer;
    nextFrameTimer = frameTimer - 1;
    if (frameTimer == 0) {
        framesRemaining = state->framesRemaining;
        if (framesRemaining != 1) {
            state->framesRemaining = framesRemaining - 1;
            func_80082070(state);
            if (state->framesRemaining == 1) {
                return 1;
            }
            frameTimer = state->frameTimer;
            nextFrameTimer = frameTimer - 1;
        } else {
            return 1;
        }
    }
    state->frameTimer = nextFrameTimer;
    return 0;
}

// func_80082FC8 best match: 98.000% (base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80082FC8.s")

#ifdef NON_MATCHING
void func_80082FC8(ModelAnimInitState *state) {
    s32 tableOffset;
    s32 i;
    u8 *parentIds;
    u8 *partIds;
    u8 modelId;
    ModelAnimInitState *part;
    ModelAnimCoord *coords;

    modelId = state->modelId;
    tableOffset = modelId * 4;
    state->partCount = D_800DE380[modelId];
    partIds = D_800DE3A8[modelId];
    parentIds = D_800DE3C4[modelId];
    i = 0;
    if (state->partCount > 0) {
        part = state;
        do {
            part->partId = *partIds;
            i++;
            part = (ModelAnimInitState *)((u8 *)part + 0x14);
            part->parentPartId = *parentIds;
            partIds++;
            parentIds++;
        } while (i < state->partCount);
        modelId = state->modelId;
        i = 0;
        tableOffset = modelId * 4;
    }

    coords = *(ModelAnimCoord **)((u8 *)D_800DE530 + tableOffset);
    if (state->partCount > 0) {
        part = state;
        do {
            part->unk33E = 0;
            part->unk33C = part->unk33E;
            i++;
            part->unk33A = part->unk33E;
            part = (ModelAnimInitState *)((u8 *)part + 0x14);
            part->x = coords->x << 0xE;
            part->y = coords->y << 0xE;
            part->z = coords->z << 0xE;
            coords++;
        } while (i < state->partCount);
    }
}
#endif
