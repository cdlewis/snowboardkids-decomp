#include "common.h"
#include "memory_allocator.h"
#include "fixed_point_matrix.h"

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

typedef struct ModelAnimState {
    u16 modelId;
    char pad2[0x450];
    s16 animIndex;
    s32 animStartOffset;
    s32 frameDataOffset;
    s16 frameCount;
    s16 framesRemaining;
    s16 unk460;
    s16 unk462;
    s16 frameTimerReset;
    s16 frameTimer;
} ModelAnimState;

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
extern s16 D_8011215C[];
extern s16 D_80112166;
extern s16 D_80121B50;
extern s16 D_800B957E;
extern CourseSpawnEntry D_800B9540[];

extern s32 func_8007BDE4(s32, s32);
extern void func_80081EF4(ModelAnimState *);
extern void func_80082070(ModelAnimState *);

void func_8007D190(void) {
    s32 ptr;
    s32 v1;

    ptr = func_80043040(D_80112166);
    D_80121B90 = (ModelAnimCoord *)(ptr + 2);
    v1 = (s32)D_80121B90 + *(u16 *)ptr * sizeof(ModelAnimCoord);
    D_80121B94 = (ModelAnimFace *)(v1 + 2);
    v1 = (s32)D_80121B94 + *(u16 *)v1 * sizeof(ModelAnimFace);
    D_80121B98 = (ModelAnimKeyframe *)(v1 + 2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_8007D200.s")

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

    temp1 = func_80098C30((s64)D_80121BB4 * D_80121BB4 + (s64)D_80121BBC * D_80121BBC);
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

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_8007DC38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_8007ECF4.s")

void func_8007FD88(s32 arg0) {
    s32 temp_v1;
    s32 temp_v0;
    s32 length;
    s32 temp_a1;

    length = func_80098C30((s64)D_80121BB4 * D_80121BB4 + (s64)D_80121BBC * D_80121BBC);
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

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_8007FF88.s")

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

// func_80081124 best match: 93.812% (base_3.c)
#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80081124.s")

#ifdef NON_MATCHING
s32 func_80081124(s32 arg0, s32 arg1, s32 arg2) {
    s32 keyframeOffset;
    s32 faceIndex;
    s32 faceOffset;
    ModelAnimFace *face;
    ModelAnimCoord *coord;
    s64 lhs;
    s64 rhs;

    keyframeOffset = arg0 * sizeof(ModelAnimKeyframe);
    faceIndex = ((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->unk14[0];
    if (faceIndex < ((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->unk14[1]) {
        faceOffset = faceIndex * sizeof(ModelAnimFace);
        do {
            face = (ModelAnimFace *)((s32)D_80121B94 + faceOffset);
            if (face->unk7 == 0) {
                coord = &D_80121B90[face->coord0];
                lhs = (s64)((D_80121B90[face->coord1].x << 0x11) - (coord->x << 0x11)) *
                    (arg2 - (coord->z << 0x11));

                face = (ModelAnimFace *)((s32)D_80121B94 + faceOffset);
                coord = &D_80121B90[face->coord0];
                rhs = (s64)((D_80121B90[face->coord1].z << 0x11) - (coord->z << 0x11)) *
                    (arg1 - (coord->x << 0x11));

                if (lhs - rhs < 0) {
                    goto next;
                }

                face = (ModelAnimFace *)((s32)D_80121B94 + faceOffset);
            }

            coord = &D_80121B90[face->coord1];
            lhs = (s64)((D_80121B90[face->coord2].x << 0x11) - (coord->x << 0x11)) *
                (arg2 - (coord->z << 0x11));

            face = (ModelAnimFace *)((s32)D_80121B94 + faceOffset);
            coord = &D_80121B90[face->coord1];
            rhs = (s64)((D_80121B90[face->coord2].z << 0x11) - (coord->z << 0x11)) *
                (arg1 - (coord->x << 0x11));

            if (lhs - rhs >= 0) {
                return ((ModelAnimFace *)((s32)D_80121B94 + faceOffset))->faceIndex;
            }

next:
            faceIndex++;
            faceOffset += sizeof(ModelAnimFace);
        } while (faceIndex < ((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->unk14[1]);
    }

    return 0;
}
#endif

u32 func_800813F8(s32 arg0, s32 arg1, s32 arg2) {
    ModelAnimKeyframe *keyframe = &D_80121B98[arg0];

    s32 deltaX = arg1 - (D_80121B90[D_80121B98[arg0].positionIndex].x << 0x11);
    s32 deltaZ = arg2 - (D_80121B90[D_80121B98[arg0].positionIndex].z << 0x11);

    D_80121B9C = func_80097AE8(D_80121B98[arg0].angle);
    D_80121BA0 = func_80097B48(D_80121B98[arg0].angle);

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
        D_80121B9C = func_80097AE8(((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->angle);
        D_80121BA0 = func_80097B48(((ModelAnimKeyframe *)((s32)D_80121B98 + keyframeOffset))->angle);

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
        D_80121B9C = func_80097AE8(D_80121B98[pathIndex].angle);
        nextKeyframeOffset = pathIndex * sizeof(ModelAnimKeyframe);
        D_80121BA0 = func_80097B48(((ModelAnimKeyframe *)((s32)D_80121B98 + nextKeyframeOffset))->angle);

        coord = &D_80121B90[((ModelAnimKeyframe *)((s32)D_80121B98 + nextKeyframeOffset))->positionIndex];
        deltaZ = *arg4 - (coord->z << 0x11);
        deltaX = *arg3 - (coord->x << 0x11);
        distance = func_8007BCFC(arg5, pathIndex, arg6, coord);
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
        D_80121B9C = func_80097AE8(D_80121B98[arg0].angle);
        D_80121BA0 = func_80097B48(D_80121B98[arg0].angle);
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

    temp_v0 = func_80043040(D_8011215C[state->modelId]);
    temp_a1 = (s16 *)(temp_v0 + (((u16 *)temp_v0)[animIndex] * 2));
    temp_v1 = *temp_a1;
    temp_a2->framesRemaining = temp_v1;
    temp_a2->framesRemaining++;
    temp_a2->frameCount = temp_v1;
    temp_a1 += 1;
    sp1C = temp_a1;
    temp_v1_2 = (s32)temp_a1 - func_80043040(D_8011215C[temp_a2->modelId]);
    temp_a2->animStartOffset = temp_v1_2;
    temp_a2->frameDataOffset = temp_v1_2;
    temp_a2->frameTimer = 0;
    temp_a2->animIndex = animIndex;
}

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80081EF4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80082070.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80082184.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80082664.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80082B58.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80082FC8.s")
