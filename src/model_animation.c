#include "common.h"

typedef struct ModelAnimCoord {
    s16 x;
    s16 y;
    s16 z;
} ModelAnimCoord;

typedef struct ModelAnimFace {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
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
    s16 unk14[4];
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

extern s32 D_80121B98;
extern s32 D_80121B94;
extern s32 D_80121B90;
extern s16 D_8011215C[];
extern s16 D_80112166;

extern s32 func_80043040(s16);
extern void func_80081EF4(ModelAnimState *);
extern void func_80082070(ModelAnimState *);

void func_8007D190(void) {
    s32 ptr;
    s32 v1;

    ptr = func_80043040(D_80112166);
    D_80121B90 = ptr + 2;
    v1 = D_80121B90 + *(u16 *)ptr * 6;
    D_80121B94 = v1 + 2;
    v1 = D_80121B94 + *(u16 *)v1 * 8;
    D_80121B98 = v1 + 2;
}

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_8007D200.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_8007D548.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_8007D87C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_8007DC38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_8007ECF4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_8007FD88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_8007FF88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80080CC4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80081124.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_800813F8.s")

void func_80081508(s32 arg0, s32 *x, s32 *y, s32 *z, s16 *angle) {
    s32 *coordTablePtr;
    s32 keyframeOffset;

    keyframeOffset = arg0 * sizeof(ModelAnimKeyframe);
    coordTablePtr = &D_80121B90;
    *x = ((ModelAnimCoord *)(*coordTablePtr +
            (((ModelAnimKeyframe *)(D_80121B98 + keyframeOffset))->positionIndex * sizeof(ModelAnimCoord))))->x << 0x11;
    *y = ((ModelAnimCoord *)(*coordTablePtr +
            (((ModelAnimKeyframe *)(D_80121B98 + keyframeOffset))->positionIndex * sizeof(ModelAnimCoord))))->y << 0x11;
    *z = ((ModelAnimCoord *)(*coordTablePtr +
            (((ModelAnimKeyframe *)(D_80121B98 + keyframeOffset))->positionIndex * sizeof(ModelAnimCoord))))->z << 0x11;
    *angle = -((ModelAnimKeyframe *)(D_80121B98 + keyframeOffset))->angle;
}

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_800815D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80081C44.s")

s16 func_80081E1C(s32 arg0) {
    return ((ModelAnimKeyframe *)D_80121B98)[arg0].nextFaceIndices[1];
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

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80082DD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model_animation/func_80082E48.s")

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
