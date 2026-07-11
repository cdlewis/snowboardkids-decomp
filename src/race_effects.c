#include "common.h"
#include "race_effects.h"
#include "model_animation.h"
#include "memory_allocator.h"
#include "callback_task_scheduler.h"
#include "race_item_effects.h"
#include "race_player_movement.h"

#define RACE_PLAYER_STATE_SIZE 0x60C

typedef s16 FixedMatrix3s[10];

typedef struct {
    /* 0x00 */ FixedMatrix3s rotation;
    /* 0x14 */ Vec3i translation;
} RaceEffectMatrixSource;

typedef struct {
    /* 0x00 */ RaceEffectMatrixSource source;
    /* 0x20 */ s32 pad20;
} RaceEffectMatrixScratch;

typedef struct {
    s32 pad;
    Vec3i transformed;
    Vec3i offset;
} TransformScratch;

typedef struct {
    /* 0x000 */ u8 pad0[0x13];
    /* 0x013 */ s8 isActive;
    /* 0x014 */ u8 pad14[0x1C - 0x14];
    /* 0x01C */ s32 posX;
    /* 0x020 */ s32 posY;
    /* 0x024 */ s32 posZ;
    /* 0x028 */ u8 pad28[0x44 - 0x28];
    /* 0x044 */ s32 unk44;
    /* 0x048 */ u8 pad48[0x5C - 0x48];
    /* 0x05C */ s32 unk5C;
    /* 0x060 */ u8 pad60[0x94 - 0x60];
    /* 0x094 */ FixedMatrix3s transform;
    union {
        /* 0x0A8 */ Vec3i posA8;
        /* 0x0A8 */ Vec3i velocity;
    };
    /* 0x0B4 */ u8 padB4[0x1C8 - 0xB4];
    /* 0x1C8 */ Vec3i pos;
    /* 0x1D4 */ u8 pad1D4[0x2EC - 0x1D4];
    union {
        /* 0x2EC */ s16 yaw;
        /* 0x2EC */ s16 unk2EC;
    };
    /* 0x2EE */ u8 pad2EE[0x2FC - 0x2EE];
    /* 0x2FC */ s32 flags;
    /* 0x300 */ u8 pad300[0x320 - 0x300];
    /* 0x320 */ s16 actionSoundTimer;
    /* 0x322 */ u8 pad322[0x502 - 0x322];
    /* 0x502 */ s16 surfaceAngle;
    /* 0x504 */ u8 pad504[0x50C - 0x504];
    /* 0x50C */ s16 *unk50C;
    /* 0x510 */ s16 unk510;
    /* 0x512 */ u8 pad512[RACE_PLAYER_STATE_SIZE - 0x512];
} RacePlayerState;

typedef struct {
    /* 0x000 */ s16 surfaceAngle;
    /* 0x002 */ u8 pad2[RACE_PLAYER_STATE_SIZE - 2];
} RacePlayerSurfaceState;

typedef struct {
    /* 0x00 */ s16 value;
    /* 0x02 */ u8 pad2[RACE_PLAYER_STATE_SIZE - 2];
} RacePlayerHalfwordField;

typedef struct {
    /* 0x00 */ u8 value;
    /* 0x01 */ u8 pad1[RACE_PLAYER_STATE_SIZE - 1];
} RacePlayerByteField;

struct RaceEffectActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 playerIndex;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Mtx *matrix;
    /* 0x28 */ s32 velocityY;
    /* 0x2C */ s32 accelerationY;
    /* 0x30 */ void *image;
    /* 0x34 */ void *palette;
    /* 0x38 */ s16 timer;
    /* 0x3A */ s16 spriteIndex;
    /* 0x3C */ s16 targetAngle;
    /* 0x3E */ s16 startAngle;
    union {
        /* 0x40 */ s16 angle;
        /* 0x40 */ s16 *anglePtr;
        /* 0x40 */ s8 matrixDirty;
        struct {
            /* 0x40 */ u8 pad40[2];
            /* 0x42 */ s8 matrixDirty2;
        } matrixFlags;
    };
    /* 0x44 */ Vec3i prevPos;
    /* 0x50 */ s32 radius;
    /* 0x54 */ s8 unk54;
    /* 0x55 */ u8 pad55[0x58 - 0x55];
    /* 0x58 */ s8 matrixDirty2;
};

extern s16 D_8011216C;
extern s16 D_80112168;
extern RaceEffectMatrixSource gIdentityFixedTransform;
extern Gfx D_800D45E0[];
extern Gfx D_800D4620[];
extern Gfx gEffectRenderModeSetupDl[];
extern Gfx gEffectRenderModeCleanupDl[];
extern Gfx *gRegionAllocPtr;
extern u8 gRaceUpdatePaused;
extern u8 D_80156609;
extern s16 gUiBlinkTimer;
extern Mtx *D_80156614;
extern RacePlayerState D_80121D80[];
extern RacePlayerHalfwordField gPlayerHitSource[];
extern RacePlayerSurfaceState gRacePlayerSurfaceAngleByPlayer[];
extern RacePlayerByteField D_8012229A[];
extern s32 D_801248A4;

Mtx *allocFixedTransformMatrix(RaceEffectMatrixSource *);
void addRenderCallback(void *, void *, void *);
void func_80045990(s32, s32, void **, void **);
void func_8006224C(s32, s32, s32, u16);
void enqueuePositionalSoundEffect(s32, void *, s32, s32);
s32 integerSquareRoot64(s64);
s16 fixedSine(s16);
s16 fixedCosine(s16);
void transformVec3iByFixedMatrix(s16 *, Vec3i *, void *);
s64 __ll_mul(s64, s64);

// func_80049440 best match: 99.927% (nonmatchings/func_80049440-7273315160691878794/base_3.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_80049440.s")

#ifdef NON_MATCHING
s32 func_80049440(Vec3i *pos, s32 radius, s16 angle, s16 playerIndex, s16 *outAngle) {
    RacePlayerState *player;
    s32 closest;
    s32 dx;
    s32 dz;
    s32 dist;
    s32 i;
    s32 hit;
    s16 playerAngle;

    closest = radius + 0x1000;
    hit = -1;
    i = 0;
    do {
        if (playerIndex != i) {
            player = &D_80121D80[i];
            if ((player->actionSoundTimer == 0) && !(player->flags & 0x402000) && (player->isActive != 0)) {
                dx = player->posX - pos->x;
                if (dx < 0) {
                    dx = -dx;
                }
                if (dx < radius) {
                    dist = player->unk5C - pos->y;
                    if (dist < 0) {
                        dist = -dist;
                    }
                    if (dist < radius) {
                        dz = player->posZ - pos->z;
                        if (dz < 0) {
                            dz = -dz;
                        }
                        if (dz < radius) {
                            playerAngle = calculateAngleBetweenXZPoints(pos->x, pos->z, player->posX, player->posZ);
                            if ((s16)((playerAngle - angle + 0x380) & 0xFFF) < 0x700) {
                                dist = integerSquareRoot64((s64)dx * dx + (s64)dz * dz);
                                if ((dist < radius) && (dist < closest)) {
                                    hit = i;
                                    closest = dist;
                                    *outAngle = playerAngle;
                                }
                            }
                        }
                    }
                }
            }
        }
        i++;
    } while (i != 4);
    return hit;
}
#endif

void func_80049664(RaceEffectActor *arg0) {
    RaceEffectMatrixScratch sp6C;
    volatile u8 padding[8];
    Gfx *temp_v0_2;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_5;
    Gfx *temp_v0_6;
    Gfx *temp_v0_7;
    Gfx *temp_v0_8;
    Gfx *temp_v0_9;
    Gfx *temp_v0_10;
    Gfx *temp_v0_11;
    Gfx *temp_v0_12;
    Gfx *temp_v0_13;
    Gfx *temp_v0_14;
    Gfx *temp_v0_17;
    Gfx *temp_v0_18;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sp6C.source = gIdentityFixedTransform;
            sp6C.source.translation.x = arg0->pos.x;
            sp6C.source.translation.y = arg0->pos.y;
            sp6C.source.translation.z = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix(&sp6C.source);
        }

        do { if (arg0->matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeSetupDl; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) D_80156614; } ; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) D_800D45E0; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeCleanupDl; } ; } } while (0);
    }
}

void func_800499A4(RaceEffectActor *arg0) {
    s32 sin;
    s32 xOffset;
    s32 cos;
    s32 zOffset;
    s32 pushX;
    s32 pushZ;
    s32 prevY;
    s32 y;
    s16 angleDiff;
    s32 groundY;
    Vec3i *pos;
    s32 i;
    volatile u8 padding[0x10];

    if (gRaceUpdatePaused == 0) {
        pos = &arg0->pos;
        arg0->spriteIndex = func_80049440(pos, 0x1600000, arg0->targetAngle, arg0->playerIndex, &angleDiff);

        if (arg0->spriteIndex != -1) {
            D_8012229A[arg0->spriteIndex].value = 1;
            angleDiff = (angleDiff - arg0->targetAngle) & 0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }

            if (angleDiff >= 0x1D) {
                angleDiff = 0x1C;
            }
            if (angleDiff < -0x1C) {
                angleDiff = -0x1C;
            }

            arg0->targetAngle += angleDiff;
        }

        sin = fixedSine(arg0->targetAngle);
        cos = fixedCosine(arg0->targetAngle);
        xOffset = ((s64)sin * arg0->velocityY) / 0x1000;
        zOffset = ((s64)cos * arg0->velocityY) / 0x1000;

        y = arg0->pos.y;
        prevY = y;
        arg0->pos.x += xOffset;
        arg0->pos.y = y;
        arg0->pos.y = arg0->pos.y + arg0->accelerationY;
        arg0->pos.z += zOffset;

        arg0->startAngle = func_8007D200(arg0->startAngle, arg0->pos.x, arg0->pos.z);
        groundY = func_80080CC4(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
        y = arg0->pos.y;
        if (y < groundY) {
            arg0->pos.y = groundY;
            y = groundY;
        }
        arg0->accelerationY = (y - prevY) - 0x20000;

        func_8007FF88(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ);
        if (pushX != 0 || pushZ != 0) {
            arg0->timer = 0;
            arg0->pos.x += pushX;
            arg0->pos.z += pushZ;
        }

        for (i = 0; i < 4; i++) {
            if (i != arg0->playerIndex && func_800891B8(pos, 0x30000, 8, i)) {
                gPlayerHitSource[i].value = arg0->playerIndex;
                arg0->timer = 0;
                i = 4;
            }
        }

        if (arg0->timer == 0) {
            enqueuePositionalSoundEffect(0xA, pos, 0x7F, 0x32);
            func_8004E594(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            removeCallbackTask(arg0);
            return;
        }

        arg0->timer--;
        func_8004EA34(arg0->pos.x, arg0->pos.y, arg0->pos.z, 0);
    }

    addRenderCallback(&D_801248A4, func_80049664, arg0);
}

void func_80049CE0(RaceEffectActor *arg0) {
    volatile s32 pad0;
    Vec3i source;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 magnitude;
    s32 var_a0;
    s64 product;

    arg0->timer = 0x12C;
    arg0->spriteIndex = -1;
    arg0->velocityY = 0x130000;
    source.z = 0;
    source.y = 0;
    source.x = 0x1000;
    if (D_80121D80[arg0->playerIndex].flags & 0x400) { source.x = -0x1000; } transformVec3iByFixedMatrix(D_80121D80[arg0->playerIndex].transform, &source, &sp4C); product = __ll_mul((s64) sp4C, (s64) sp4C); magnitude = integerSquareRoot64(product + __ll_mul((s64) sp54, (s64) sp54)); if (magnitude != 0) {
        arg0->accelerationY = (((s64) arg0->velocityY) * sp50) / magnitude;
        var_a0 = -arg0->velocityY;
    } else {
        var_a0 = -arg0->velocityY;
        arg0->accelerationY = var_a0;
    }
    arg0->accelerationY += D_80121D80[arg0->playerIndex].unk44;
    arg0->velocityY = var_a0;
    arg0->targetAngle = D_80121D80[arg0->playerIndex].unk2EC;
    source.z = 0;
    source.y = 0x280000;
    source.x = 0x100000;
    if (D_80121D80[arg0->playerIndex].flags & 0x400) {
        source.x = 0xFFF00000;
        arg0->targetAngle += 0x800;
    }
    transformVec3iByFixedMatrix(D_80121D80[arg0->playerIndex].transform, &source, &arg0->pos.x);
    arg0->pos.x += D_80121D80[arg0->playerIndex].velocity.x;
    arg0->pos.y += D_80121D80[arg0->playerIndex].velocity.y;
    arg0->pos.z += D_80121D80[arg0->playerIndex].velocity.z;
    arg0->startAngle = D_80121D80[arg0->playerIndex].surfaceAngle;
    func_80045990(getMemoryBlockBase(D_8011216C), 0, &arg0->image, &arg0->palette);
    func_800499A4(arg0);
    setCallbackTaskCallback(arg0, func_800499A4);
}

void func_80049FB4(RaceEffectActor *arg0) {
    RaceEffectMatrixScratch sp64;
    volatile u8 padding[8];
    Gfx *temp_v0_2;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_5;
    Gfx *temp_v0_6;
    Gfx *temp_v0_7;
    Gfx *temp_v0_8;
    Gfx *temp_v0_9;
    Gfx *temp_v0_10;
    Gfx *temp_v0_11;
    Gfx *temp_v0_12;
    Gfx *temp_v0_13;
    Gfx *temp_v0_14;
    Gfx *temp_v0_17;
    Gfx *temp_v0_18;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sp64.source = gIdentityFixedTransform;
            sp64.source.translation.x = arg0->pos.x;
            sp64.source.translation.y = arg0->pos.y;
            sp64.source.translation.z = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix(&sp64.source);
        }

        do { if (arg0->matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeSetupDl; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) D_80156614; } ; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) D_800D45E0; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeCleanupDl; } ; } } while (0);
    }
}

void func_8004A2F4(RaceEffectActor *arg0) {
    s32 sin;
    s32 xOffset;
    s32 cos;
    s32 zOffset;
    s32 pushX;
    s32 pushZ;
    volatile s32 prevY;
    s32 y;
    s16 angleDiff;
    s32 groundY;
    s32 hitPlayer;
    s32 newX;
    s32 newY;
    s32 newZ;
    Vec3i *pos;
    s32 i;

    if (gRaceUpdatePaused == 0) {
        pos = &arg0->pos;
        arg0->spriteIndex = func_80049440(pos, 0xE00000, arg0->targetAngle, arg0->playerIndex, &angleDiff);

        if (arg0->spriteIndex != -1) {
            D_8012229A[arg0->spriteIndex].value = 1;
            angleDiff = (angleDiff - arg0->targetAngle) & 0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }

            if (angleDiff >= 0x19) {
                angleDiff = 0x18;
            }
            if (angleDiff < -0x18) {
                angleDiff = -0x18;
            }

            arg0->targetAngle += angleDiff;
        }

        sin = fixedSine(arg0->targetAngle);
        cos = fixedCosine(arg0->targetAngle);
        xOffset = ((s64)sin * arg0->velocityY) / 0x1000;
        zOffset = ((s64)cos * arg0->velocityY) / 0x1000;

        y = arg0->pos.y;
        prevY = y;
        arg0->pos.x += xOffset;
        arg0->pos.y = y;
        arg0->pos.y = arg0->pos.y + arg0->accelerationY;
        arg0->pos.z += zOffset;

        arg0->startAngle = func_8007D200(arg0->startAngle, arg0->pos.x, arg0->pos.z);
        groundY = func_80080CC4(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
        y = arg0->pos.y;
        if (y < groundY) {
            arg0->pos.y = groundY;
            y = groundY;
        }
        arg0->accelerationY = ((y - prevY) - 0x20000) & 0xFFFFFFFF;

        func_8007FF88(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ);
        hitPlayer = 0;
        if (pushX != 0 || pushZ != 0) {
            arg0->timer = 0;
            arg0->pos.x += pushX;
            arg0->pos.z += pushZ;
        }

        for (i = 0; i < 4; i++) {
            if (i != arg0->playerIndex && func_800891B8(pos, 0x30000, 0x10, i)) {
                gPlayerHitSource[i].value = arg0->playerIndex;
                hitPlayer = 1;
                arg0->timer = 0;
                i = 4;
            }
        }

        if (arg0->timer == 0) {
            if (hitPlayer == 0) {
                enqueuePositionalSoundEffect(0xA, pos, 0x7F, 0x32);
            }
            func_8004E594(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            removeCallbackTask(arg0);
            return;
        }

        arg0->timer--;
        func_8004EA34(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
    }

    addRenderCallback(&D_801248A4, func_80049FB4, arg0);
}

void func_8004A648(RaceEffectActor *arg0) {
    volatile s32 pad0;
    Vec3i source;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 magnitude;
    s32 var_a0;
    s64 product;

    arg0->timer = 0x12C;
    arg0->spriteIndex = -1;
    arg0->velocityY = 0x150000;
    source.z = 0;
    source.y = 0;
    source.x = 0x1000;
    if (D_80121D80[arg0->playerIndex].flags & 0x400) { source.x = -0x1000; } transformVec3iByFixedMatrix(D_80121D80[arg0->playerIndex].transform, &source, &sp4C); product = __ll_mul((s64) sp4C, (s64) sp4C); magnitude = integerSquareRoot64(product + __ll_mul((s64) sp54, (s64) sp54)); if (magnitude != 0) {
        arg0->accelerationY = (((s64) arg0->velocityY) * sp50) / magnitude;
        var_a0 = -arg0->velocityY;
    } else {
        var_a0 = -arg0->velocityY;
        arg0->accelerationY = var_a0;
    }
    arg0->accelerationY += D_80121D80[arg0->playerIndex].unk44;
    arg0->velocityY = var_a0;
    arg0->targetAngle = D_80121D80[arg0->playerIndex].unk2EC;
    source.z = 0;
    source.y = 0x280000;
    source.x = 0x100000;
    if (D_80121D80[arg0->playerIndex].flags & 0x400) {
        source.x = 0xFFF00000;
        arg0->targetAngle += 0x800;
    }
    transformVec3iByFixedMatrix(D_80121D80[arg0->playerIndex].transform, &source, &arg0->pos.x);
    arg0->pos.x += D_80121D80[arg0->playerIndex].velocity.x;
    arg0->pos.y += D_80121D80[arg0->playerIndex].velocity.y;
    arg0->pos.z += D_80121D80[arg0->playerIndex].velocity.z;
    arg0->startAngle = D_80121D80[arg0->playerIndex].surfaceAngle;
    func_80045990(getMemoryBlockBase(D_8011216C), 2, &arg0->image, &arg0->palette);
    func_8004A2F4(arg0);
    setCallbackTaskCallback(arg0, func_8004A2F4);
}

void func_8004A91C(RaceEffectActor *arg0) {
    RaceEffectMatrixScratch sp64;
    Gfx *temp_v0_2;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_5;
    Gfx *temp_v0_6;
    Gfx *temp_v0_7;
    Gfx *temp_v0_8;
    Gfx *temp_v0_9;
    Gfx *temp_v0_10;
    Gfx *temp_v0_11;
    Gfx *temp_v0_12;
    Gfx *temp_v0_13;
    Gfx *temp_v0_14;
    Gfx *temp_v0_17;
    Gfx *temp_v0_18;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sp64.source = gIdentityFixedTransform;
            sp64.source.translation.x = arg0->pos.x;
            sp64.source.translation.y = arg0->pos.y;
            sp64.source.translation.z = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix(&sp64.source);
        }

        do { if (arg0->matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeSetupDl; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) D_80156614; } ; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) D_800D45E0; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeCleanupDl; } ; } } while (0);
    }
}

// func_8004AC5C best match: 99.823% (nonmatchings/func_8004AC5C-2/output-226-1/source.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004AC5C.s")

#ifdef NON_MATCHING
void func_8004AC5C(RaceEffectActor *arg0) {
    s32 sin;
    s32 xOffset;
    s32 cos;
    s32 zOffset;
    s32 pushX;
    s32 pushZ;
    s32 prevY;
    s32 y;
    s16 angleDiff;
    s32 groundY;
    Vec3i *pos;
    s32 i;
    volatile u8 padding[0x10];

    if (gRaceUpdatePaused == 0) {
        pos = &arg0->pos;
        arg0->spriteIndex = func_80049440(pos, 0x600000, arg0->targetAngle, arg0->playerIndex, &angleDiff);

        if (arg0->spriteIndex != -1) {
            D_8012229A[arg0->spriteIndex].value = 1;
            angleDiff = (angleDiff - arg0->targetAngle) & 0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }

            if (angleDiff >= 0x1D) {
                angleDiff = 0x1C;
            }
            if (angleDiff < -0x1C) {
                angleDiff = -0x1C;
            }

            arg0->targetAngle += angleDiff;
        }

        sin = fixedSine(arg0->targetAngle);
        cos = fixedCosine(arg0->targetAngle);
        xOffset = ((s64) sin * arg0->velocityY) / 0x1000;
        zOffset = ((s64) cos * arg0->velocityY) / 0x1000;

        prevY = arg0->pos.y;
        arg0->pos.x += xOffset;
        arg0->pos.y = prevY + (arg0->accelerationY & 0xFFFFFFFFu);
        arg0->pos.z += zOffset;

        arg0->startAngle = func_8007D200(arg0->startAngle, arg0->pos.x, arg0->pos.z);
        groundY = func_80080CC4(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
        y = arg0->pos.y;
        if (y < groundY) {
            arg0->pos.y = groundY;
            y = groundY;
        }
        arg0->accelerationY = (y - prevY) - 0x20000;

        func_8007ECF4(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ, &xOffset, &zOffset);
        if (pushX != 0 || pushZ != 0) {
            arg0->accelerationY = 0;
            arg0->pos.x += pushX;
            arg0->pos.z += pushZ;
            arg0->targetAngle = calculateAngleFromDeltaXZ(xOffset, zOffset);
            enqueuePositionalSoundEffect(0x11, pos, 0x7F, 0x32);
        }

        for (i = 0; i < 4; i++) {
            if ((i != arg0->playerIndex || arg0->timer < 0x4B) && func_800891B8(pos, 0x30000, 0x40, i)) {
                gPlayerHitSource[i].value = arg0->playerIndex;
                arg0->timer = 0;
                i = 4;
            }
        }

        if (arg0->timer == 0) {
            enqueuePositionalSoundEffect(0xA, pos, 0x7F, 0x32);
            func_8004E594(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            removeCallbackTask(arg0);
            return;
        }

        arg0->timer--;
        func_8004EA34(arg0->pos.x, arg0->pos.y, arg0->pos.z, 3);
    }

    addRenderCallback(&D_801248A4, func_8004A91C, arg0);
}
#endif

void func_8004AFE4(RaceEffectActor *arg0) {
    volatile s32 pad0;
    Vec3i source;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 magnitude;
    s32 var_a0;
    s64 product;

    arg0->timer = 0xB4;
    arg0->spriteIndex = -1;
    arg0->velocityY = 0x170000;
    source.z = 0;
    source.y = 0;
    source.x = 0x1000;
    if (D_80121D80[arg0->playerIndex].flags & 0x400) { source.x = -0x1000; } transformVec3iByFixedMatrix(D_80121D80[arg0->playerIndex].transform, &source, &sp4C); product = __ll_mul((s64) sp4C, (s64) sp4C); magnitude = integerSquareRoot64(product + __ll_mul((s64) sp54, (s64) sp54)); if (magnitude != 0) {
        arg0->accelerationY = (((s64) arg0->velocityY) * sp50) / magnitude;
        var_a0 = -arg0->velocityY;
    } else {
        var_a0 = -arg0->velocityY;
        arg0->accelerationY = var_a0;
    }
    arg0->accelerationY += D_80121D80[arg0->playerIndex].unk44;
    arg0->velocityY = var_a0;
    arg0->targetAngle = D_80121D80[arg0->playerIndex].unk2EC;
    source.z = 0;
    source.y = 0x280000;
    source.x = 0x100000;
    if (D_80121D80[arg0->playerIndex].flags & 0x400) {
        source.x = 0xFFF00000;
        arg0->targetAngle += 0x800;
    }
    transformVec3iByFixedMatrix(D_80121D80[arg0->playerIndex].transform, &source, &arg0->pos.x);
    arg0->pos.x += D_80121D80[arg0->playerIndex].velocity.x;
    arg0->pos.y += D_80121D80[arg0->playerIndex].velocity.y;
    arg0->pos.z += D_80121D80[arg0->playerIndex].velocity.z;
    arg0->startAngle = D_80121D80[arg0->playerIndex].surfaceAngle;
    func_80045990(getMemoryBlockBase(D_8011216C), 3, &arg0->image, &arg0->palette);
    func_8004AC5C(arg0);
    setCallbackTaskCallback(arg0, func_8004AC5C);
}

void func_8004B2B8(RaceEffectActor *arg0) {
    RaceEffectMatrixScratch sp64;
    Gfx *temp_v0_2;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_5;
    Gfx *temp_v0_6;
    Gfx *temp_v0_7;
    Gfx *temp_v0_8;
    Gfx *temp_v0_9;
    Gfx *temp_v0_10;
    Gfx *temp_v0_11;
    Gfx *temp_v0_12;
    Gfx *temp_v0_13;
    Gfx *temp_v0_14;
    Gfx *temp_v0_17;
    Gfx *temp_v0_18;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sp64.source = gIdentityFixedTransform;
            sp64.source.translation.x = arg0->pos.x;
            sp64.source.translation.y = arg0->pos.y;
            sp64.source.translation.z = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix(&sp64.source);
        }

        do { if (arg0->matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeSetupDl; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) D_80156614; } ; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) D_800D45E0; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeCleanupDl; } ; } } while (0);
    }
}

void func_8004B5F8(RaceEffectActor *arg0) {
    s32 sin;
    s32 cos;
    s32 xOffset;
    s32 zOffset;
    s32 pushX;
    s32 pushZ;
    volatile u8 padding[8];
    s32 prevY;
    s32 y;
    s32 groundY;
    Vec3i *pos;
    s32 i;

    if (gRaceUpdatePaused == 0) {
        sin = fixedSine(arg0->targetAngle);
        cos = fixedCosine(arg0->targetAngle);
        xOffset = ((s64)sin * arg0->velocityY) / 0x1000;
        zOffset = ((s64)cos * arg0->velocityY) / 0x1000;

        prevY = arg0->pos.y;
        arg0->pos.x += xOffset;
        arg0->pos.y = arg0->pos.y + arg0->accelerationY;
        arg0->pos.z += zOffset;

        arg0->startAngle = func_8007D200(arg0->startAngle, arg0->pos.x, arg0->pos.z);
        groundY = func_80080CC4(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
        y = arg0->pos.y;
        if (y < groundY) {
            arg0->pos.y = groundY;
            y = groundY;
        }
        arg0->accelerationY = (y - prevY) - 0x20000;

        func_8007FF88(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ);
        pos = &arg0->pos;
        if (pushX != 0 || pushZ != 0) {
            arg0->timer = 0;
            arg0->pos.x += pushX;
            arg0->pos.z += pushZ;
        }

        for (i = 0; i < 4; i++) {
            if (func_800891B8(pos, 0x30000, 0x2000, i)) {
                enqueuePositionalSoundEffect(0xA, pos, 0x7F, 0x32);
                func_8004E594(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
                arg0->timer = 0;
                i = 4;
            }
        }

        if (arg0->timer == 0) {
            removeCallbackTask(arg0);
            return;
        }

        arg0->timer--;
        func_8004EA34(arg0->pos.x, arg0->pos.y, arg0->pos.z, 3);
    }

    addRenderCallback(&D_801248A4, func_8004B2B8, arg0);
}

void func_8004B83C(RaceEffectActor *arg0) {
    arg0->timer = 0x3C;
    arg0->spriteIndex = -1;
    arg0->velocityY = 0xFFF00000;
    arg0->accelerationY = 0;
    func_80045990(getMemoryBlockBase(D_8011216C), 3, &arg0->image, &arg0->palette);
    func_8004B5F8(arg0);
    setCallbackTaskCallback(arg0, func_8004B5F8);
}

void func_8004B8B4(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4) {
    RaceEffectActor *obj = createCallbackTask(func_8004B83C, 0, 0x1E);

    if (obj != NULL) {
        obj->pos.x = arg0;
        obj->pos.y = arg1;
        obj->pos.z = arg2;
        obj->targetAngle = arg4;
        obj->startAngle = arg3;
    }
}

void func_8004B934(RaceEffectActor *arg0) {
    RaceEffectMatrixScratch sp64;
    Gfx *temp_v0_2;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_5;
    Gfx *temp_v0_6;
    Gfx *temp_v0_7;
    Gfx *temp_v0_8;
    Gfx *temp_v0_9;
    Gfx *temp_v0_10;
    Gfx *temp_v0_11;
    Gfx *temp_v0_12;
    Gfx *temp_v0_13;
    Gfx *temp_v0_14;
    Gfx *temp_v0_17;
    Gfx *temp_v0_18;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sp64.source = gIdentityFixedTransform;
            sp64.source.translation.x = arg0->pos.x;
            sp64.source.translation.y = arg0->pos.y;
            sp64.source.translation.z = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix(&sp64.source);
        }

        do { if (arg0->matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeSetupDl; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) D_80156614; } ; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) D_800D45E0; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeCleanupDl; } ; } } while (0);
    }
}

void func_8004BC74(RaceEffectActor *arg0) {
    s32 sin;
    s32 xOffset;
    volatile s32 cos;
    s32 zOffset;
    s32 pushX;
    s32 pushZ;
    volatile s32 prevY;
    s32 y;
    s16 angleDiff;
    s32 groundY;
    volatile u8 padding[8];
    Vec3i *pos;
    s32 i;

    if (gRaceUpdatePaused == 0) {
        pos = &arg0->pos;
        arg0->spriteIndex = func_80049440(pos, 0xA00000, arg0->targetAngle, arg0->playerIndex, &angleDiff);

        if (arg0->spriteIndex != -1) {
            D_8012229A[arg0->spriteIndex].value = 1;
            angleDiff = (angleDiff - arg0->targetAngle) & 0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }

            if (angleDiff >= 0x11) {
                angleDiff = 0x10;
            }
            if (angleDiff < -0x10) {
                angleDiff = -0x10;
            }

            arg0->targetAngle += angleDiff;
        }

        sin = fixedSine(arg0->targetAngle);
        cos = fixedCosine(arg0->targetAngle);
        xOffset = ((s64)sin * arg0->velocityY) / 0x1000;
        zOffset = ((s64)cos * arg0->velocityY) / 0x1000;

        y = arg0->pos.y;
        arg0->pos.y = (prevY = arg0->pos.y);
        arg0->pos.x += xOffset;
        arg0->pos.y = arg0->pos.y + arg0->accelerationY;
        arg0->pos.z += zOffset;

        arg0->startAngle = func_8007D200(arg0->startAngle, arg0->pos.x, arg0->pos.z);
        groundY = func_80080CC4(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
        y = arg0->pos.y;
        if (y < groundY) {
            arg0->pos.y = groundY;
            y = groundY;
        }
        arg0->accelerationY = (y - prevY) - 0x20000;

        func_8007FF88(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ);
        i = 0;
        if (pushX != 0 || pushZ != 0) {
            arg0->timer = 0;
            arg0->pos.x += pushX;
            if (arg0->targetAngle) {
            }
            arg0->pos.z += pushZ;
        }

        do {
            if ((i != arg0->playerIndex) && func_800891B8(pos, 0x30000, 0x80, i)) {
                gPlayerHitSource[i].value = arg0->playerIndex;
                arg0->timer = 0;
                i = 4;
            }
            i++;
        } while (i < 4);

        if (arg0->timer == 0) {
            func_8006224C(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->playerIndex);
            removeCallbackTask(arg0);
            return;
        }

        arg0->timer--;
        func_8004EA34(arg0->pos.x, arg0->pos.y, arg0->pos.z, 4);
    }

    addRenderCallback(&D_801248A4, func_8004B934, arg0);
}

void func_8004BFA0(RaceEffectActor *arg0) {
    volatile s32 pad0;
    Vec3i sp58;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 magnitude;
    s32 velocityY;
    RaceEffectActor *actor;
    s64 product;

    arg0->timer = 0x12C;
    arg0->spriteIndex = -1;
    arg0->velocityY = 0x110000;
    sp58.z = (sp58.y = 0);
    sp58.x = 0x1000;

    if (D_80121D80[arg0->playerIndex].flags & 0x400) {
        sp58.x = -0x1000;
        if (1) {
        }
    }

    actor = arg0; transformVec3iByFixedMatrix(D_80121D80[arg0->playerIndex].transform, &sp58, &sp4C); product = __ll_mul((s64) sp4C, (s64) sp4C); magnitude = integerSquareRoot64(product + __ll_mul((s64) sp54, (s64) sp54)); if (magnitude != 0) { actor->accelerationY = (s64)actor->velocityY * sp50 / magnitude; velocityY = -actor->velocityY; } else { velocityY = -actor->velocityY; actor->accelerationY = velocityY; } actor->accelerationY += D_80121D80[actor->playerIndex].unk44; actor->velocityY = velocityY; actor->targetAngle = D_80121D80[actor->playerIndex].yaw; sp58.z = 0; sp58.x = 0xFFF00000;
    sp58.y = 0x280000;
    sp58.x = 0x100000;

    if (D_80121D80[actor->playerIndex].flags & 0x400) {
        sp58.x = 0xFFF00000;
        actor->targetAngle += 0x800;
    }

    transformVec3iByFixedMatrix(D_80121D80[actor->playerIndex].transform, &sp58, &actor->pos);
    actor->pos.x += ((0, D_80121D80))[actor->playerIndex].posA8.x;
    actor->pos.y += D_80121D80[actor->playerIndex].posA8.y;
    actor->pos.z += D_80121D80[actor->playerIndex].posA8.z;
    actor->startAngle = D_80121D80[actor->playerIndex].surfaceAngle;
    func_80045990(getMemoryBlockBase(D_8011216C), 4, &actor->image, &actor->palette);
    func_8004BC74(actor);
    setCallbackTaskCallback(actor, func_8004BC74);
}

void func_8004C274(RaceEffectActor *arg0) {
    RaceEffectMatrixScratch sp64;
    Gfx *temp_v0_2;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_5;
    Gfx *temp_v0_6;
    Gfx *temp_v0_7;
    Gfx *temp_v0_8;
    Gfx *temp_v0_9;
    Gfx *temp_v0_10;
    Gfx *temp_v0_11;
    Gfx *temp_v0_12;
    Gfx *temp_v0_13;
    Gfx *temp_v0_14;
    Gfx *temp_v0_17;
    Gfx *temp_v0_18;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sp64.source = gIdentityFixedTransform;
            sp64.source.translation.x = arg0->pos.x;
            sp64.source.translation.y = arg0->pos.y;
            sp64.source.translation.z = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix(&sp64.source);
        }

        do { if (arg0->matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeSetupDl; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) D_80156614; } ; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) D_800D45E0; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeCleanupDl; } ; } } while (0);
    }
}

void func_8004C5B4(RaceEffectActor *arg0) {
    s32 sin;
    s32 xOffset;
    s32 cos;
    s32 zOffset;
    s32 pushX;
    s32 pushZ;
    s32 prevY;
    s32 y;
    s16 angleDiff;
    s32 groundY;
    Vec3i *pos;
    s32 i;
    volatile u8 padding[0x10];

    if (gRaceUpdatePaused == 0) {
        pos = &arg0->pos;
        arg0->spriteIndex = func_80049440(pos, 0x1200000, arg0->targetAngle, arg0->playerIndex, &angleDiff);

        if (arg0->spriteIndex != -1) {
            D_8012229A[arg0->spriteIndex].value = 1;
            angleDiff = (angleDiff - arg0->targetAngle) & 0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }

            if (angleDiff >= 0x13) {
                angleDiff = 0x12;
            }
            if (angleDiff < -0x12) {
                angleDiff = -0x12;
            }

            arg0->targetAngle += angleDiff;
        }

        sin = fixedSine(arg0->targetAngle);
        cos = fixedCosine(arg0->targetAngle);
        xOffset = ((s64)sin * arg0->velocityY) / 0x1000;
        zOffset = ((s64)cos * arg0->velocityY) / 0x1000;

        y = arg0->pos.y;
        prevY = y;
        arg0->pos.x += xOffset;
        arg0->pos.y = y;
        arg0->pos.y = arg0->pos.y + arg0->accelerationY;
        arg0->pos.z += zOffset;

        arg0->startAngle = func_8007D200(arg0->startAngle, arg0->pos.x, arg0->pos.z);
        groundY = func_80080CC4(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
        y = arg0->pos.y;
        if (y < groundY) {
            arg0->pos.y = groundY;
            y = groundY;
        }
        arg0->accelerationY = (y - prevY) - 0x20000;

        func_8007FF88(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ);
        if (pushX != 0 || pushZ != 0) {
            arg0->timer = 0;
            arg0->pos.x += pushX;
            arg0->pos.z += pushZ;
        }

        for (i = 0; i < 4; i++) {
            if ((i != arg0->playerIndex) && func_800891B8(pos, 0x30000, 0x100, i)) {
                gPlayerHitSource[i].value = arg0->playerIndex;
                arg0->timer = 0;
                i = 4;
            }
        }

        if (arg0->timer == 0) {
            enqueuePositionalSoundEffect(0xA, pos, 0x7F, 0x32);
            func_8004E594(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            removeCallbackTask(arg0);
            return;
        }

        arg0->timer--;
        func_8004EA34(arg0->pos.x, arg0->pos.y, arg0->pos.z, 1);
    }

    addRenderCallback(&D_801248A4, func_8004C274, arg0);
}

void func_8004C8F0(RaceEffectActor *arg0) {
    Vec3i *new_var;
    Vec3i sp58;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 magnitude;
    s32 velocityY;
    RaceEffectActor *actor;
    s64 product;

    arg0->timer = 0x12C;
    arg0->spriteIndex = -1;
    arg0->velocityY = 0x130000;
    sp58.z = (sp58.y = 0);
    sp58.x = 0x1000;

    if (D_80121D80[arg0->playerIndex].flags & 0x400) {
        sp58.x = -0x1000;
        if (1) {
        }
    }

    actor = arg0; transformVec3iByFixedMatrix(D_80121D80[arg0->playerIndex].transform, &sp58, &sp4C); product = __ll_mul((s64) sp4C, (s64) sp4C); magnitude = integerSquareRoot64(product + __ll_mul((s64) sp54, (s64) sp54)); if (magnitude != 0) { actor->accelerationY = (s64)actor->velocityY * sp50 / magnitude; velocityY = -actor->velocityY; } else { velocityY = -actor->velocityY; actor->accelerationY = velocityY; } actor->accelerationY += D_80121D80[actor->playerIndex].unk44; actor->velocityY = velocityY; actor->targetAngle = D_80121D80[actor->playerIndex].yaw; sp58.z = 0;
    new_var = &sp58;
    sp58.y = 0x280000;
    sp58.x = 0x100000;

    if (D_80121D80[actor->playerIndex].flags & 0x400) {
        sp58.x = 0xFFF00000;
        actor->targetAngle += 0x800;
    }

    transformVec3iByFixedMatrix(D_80121D80[actor->playerIndex].transform, new_var, &actor->pos);
    actor->pos.x += D_80121D80[actor->playerIndex].posA8.x;
    actor->pos.y += D_80121D80[actor->playerIndex].posA8.y;
    actor->pos.z += D_80121D80[actor->playerIndex].posA8.z;
    actor->startAngle = D_80121D80[actor->playerIndex].surfaceAngle;
    func_80045990(getMemoryBlockBase(D_8011216C), 1, &actor->image, &actor->palette);
    func_8004C5B4(actor);
    setCallbackTaskCallback(actor, func_8004C5B4);
}

void func_8004CBC4(RaceEffectActor *arg0) {
    RaceEffectMatrixScratch sp64;
    Gfx *temp_v0_2;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_5;
    Gfx *temp_v0_6;
    Gfx *temp_v0_7;
    Gfx *temp_v0_8;
    Gfx *temp_v0_9;
    Gfx *temp_v0_10;
    Gfx *temp_v0_11;
    Gfx *temp_v0_12;
    Gfx *temp_v0_13;
    Gfx *temp_v0_14;
    Gfx *temp_v0_17;
    Gfx *temp_v0_18;

    if (D_80156609 != 0) {
        arg0->matrixFlags.matrixDirty2 = 1;
    }

    if ((arg0->spriteIndex < 0x1F) && !(gUiBlinkTimer & 1)) {
        return;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
        if (arg0->matrixFlags.matrixDirty2 != 0) {
            arg0->matrixFlags.matrixDirty2 = 0;
            sp64.source = gIdentityFixedTransform;
            sp64.source.translation.x = arg0->pos.x;
            sp64.source.translation.y = arg0->pos.y;
            sp64.source.translation.z = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix(&sp64.source);
        }

        do { if (arg0->matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeSetupDl; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) D_80156614; } ; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) D_800D4620; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeCleanupDl; } ; } } while (0);
    }
}

void func_8004CF28(RaceEffectActor *arg0) {
    Vec3i *pos;
    RaceEffectActor *actor;
    s32 i;
    s32 radius;
    s16 spriteIndex;

    spriteIndex = arg0->spriteIndex;
    actor = arg0;
    if (spriteIndex != 0) {
        actor->spriteIndex = spriteIndex - 1;
    } else {
        removeCallbackTask(arg0);
        return;
    }

    i = 0;
    pos = &actor->pos;
    radius = 0x30000;
    do {
        if (func_800891B8(pos, radius, 0x400, i)) {
            enqueuePositionalSoundEffect(0x14, pos, 0x7F, 0x32);
            func_8004E594(arg0->pos.x, actor->pos.y, actor->pos.z, 1);
            removeCallbackTask(arg0);
            return;
        }
        i++;
    } while (i != 4);

    addRenderCallback(&D_801248A4, func_8004CBC4, actor);
}

void func_8004D018(RaceEffectActor *arg0) {
    Vec3i *pos;
    s32 i;
    s32 accelerationY;
    s32 groundY;

    if (gRaceUpdatePaused == 0) {
        accelerationY = arg0->accelerationY;
        arg0->pos.y += accelerationY;
        arg0->accelerationY = accelerationY - 0x6000;

        arg0->angle = func_8007D200(arg0->angle, arg0->pos.x, arg0->pos.z);
        groundY = func_80080CC4(arg0->angle, arg0->pos.x, arg0->pos.z);
        if (arg0->pos.y < groundY + 0x30000) {
            arg0->pos.y = groundY + 0x30000;
            setCallbackTaskCallback(arg0, func_8004CF28);
        }

        for (i = 0; i != 4; i++) {
            pos = &arg0->pos;
            if ((i != arg0->playerIndex || arg0->timer == 0) && func_800891B8(pos, 0x30000, 0x400, i)) {
                enqueuePositionalSoundEffect(0x14, pos, 0x7F, 0x32);
                func_8004E594(arg0->pos.x, arg0->pos.y, arg0->pos.z, 1);
                removeCallbackTask(arg0);
                return;
            }
        }

        if (arg0->timer != 0) {
            arg0->timer--;
        }
    }

    addRenderCallback(&D_801248A4, func_8004CBC4, arg0);
}

void func_8004D184(RaceEffectActor *arg0) {
    RacePlayerState *player;

    arg0->timer = 0x3C;
    arg0->spriteIndex = 0x708;
    arg0->accelerationY = 0x30000;
    player = &D_80121D80[arg0->playerIndex];
    arg0->pos.x = player->pos.x;
    arg0->pos.y = player->pos.y;
    arg0->pos.z = player->pos.z;
    enqueuePositionalSoundEffect(0x6A, &arg0->pos, 0x7F, 0x32);
    arg0->angle = gRacePlayerSurfaceAngleByPlayer[arg0->playerIndex].surfaceAngle;
    func_80045990(getMemoryBlockBase(D_80112168), 2, &arg0->image, &arg0->palette);
    func_8004D018(arg0);
    setCallbackTaskCallback(arg0, func_8004D018);
}

void func_8004D280(RaceEffectActor *arg0) {
    RaceEffectMatrixScratch sp64;
    volatile u8 padding[8];
    Gfx *temp_v0_2;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_5;
    Gfx *temp_v0_6;
    Gfx *temp_v0_7;
    Gfx *temp_v0_8;
    Gfx *temp_v0_9;
    Gfx *temp_v0_10;
    Gfx *temp_v0_11;
    Gfx *temp_v0_12;
    Gfx *temp_v0_13;
    Gfx *temp_v0_14;
    Gfx *temp_v0_17;
    Gfx *temp_v0_18;

    if (D_80156609 != 0) {
        arg0->matrixDirty2 = 1;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
        if (arg0->matrixDirty2 != 0) {
            arg0->matrixDirty2 = 0;
            sp64.source = gIdentityFixedTransform;
            sp64.source.translation.x = arg0->pos.x;
            sp64.source.translation.y = arg0->pos.y;
            sp64.source.translation.z = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix(&sp64.source);
        }

        do { if (arg0->matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeSetupDl; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) D_80156614; } ; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) D_800D45E0; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeCleanupDl; } ; } } while (0);
    }
}

// func_8004D5C0 best match: 97.915% (nonmatchings/func_8004D5C0-6688367443449623229/base_14.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004D5C0.s")

#ifdef NON_MATCHING
void func_8004D5C0(RaceEffectActor *arg0) {
    s32 sin;
    s32 cos;
    s32 xOffset;
    s32 zOffset;
    s32 pushX;
    s32 pushZ;
    s32 prevY;
    volatile u8 padding[8];
    s32 y;
    s32 groundY;
    Vec3i *pos;
    RacePlayerState *player;

    if (gRaceUpdatePaused == 0) {
        if (arg0->unk54 == 0) {
            sin = fixedSine(arg0->targetAngle);
            cos = fixedCosine(arg0->targetAngle);
            xOffset = ((s64)sin * arg0->velocityY) / 0x1000;
            zOffset = ((s64)cos * arg0->velocityY) / 0x1000;
            y = arg0->pos.y;
            prevY = y;
            arg0->pos.x += xOffset;
            arg0->pos.y = y + arg0->accelerationY;
            arg0->pos.z += zOffset;

            arg0->startAngle = func_8007D200(arg0->startAngle, arg0->pos.x, arg0->pos.z);
            groundY = func_80080CC4(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
            y = arg0->pos.y;
            if (y < groundY) {
                arg0->pos.y = groundY;
                y = groundY;
            }
            arg0->accelerationY = (y - prevY) - 0x20000;

            func_8007FF88(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ);
            if (pushX != 0 || pushZ != 0) {
                arg0->timer = 0;
                arg0->pos.x += pushX;
                arg0->pos.z += pushZ;
                enqueuePositionalSoundEffect(0xA, &arg0->pos, 0x7F, 0x32);
            }
        } else {
            arg0->timer = 0;
            enqueuePositionalSoundEffect(0x11, &arg0->pos, 0x7F, 0x32);
        }

        if (arg0->timer == 0) {
            func_8004E594(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            removeCallbackTask((RaceEffectActor *)arg0);
            player = &D_80121D80[arg0->playerIndex];
            player->unk510++;
            return;
        }

        arg0->timer--;
        pos = &arg0->pos;
        func_8004EA34(arg0->pos.x, arg0->pos.y, arg0->pos.z, 5);
        arg0->prevPos = *pos;
        arg0->radius = 0x30000;
        player = &D_80121D80[arg0->playerIndex];
        arg0->anglePtr = player->unk50C;
        player->unk50C = (s16 *)&arg0->anglePtr;
    }

    addRenderCallback(&D_801248A4, func_8004D280, arg0);
}
#endif

void func_8004D880(RaceEffectActor *arg0) {
    volatile s32 pad;
    Vec3i source;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 magnitude;
    s32 newVelocity;
    s64 product;

    arg0->timer = 0x12C;
    arg0->spriteIndex = -1;
    arg0->velocityY = 0x120000;

    source.z = 0;
    source.y = 0;
    source.x = 0x1000;
    if (D_80121D80[arg0->playerIndex].flags & 0x400) { source.x = -0x1000; } transformVec3iByFixedMatrix(D_80121D80[arg0->playerIndex].transform, &source, &sp4C); product = __ll_mul((s64) sp4C, (s64) sp4C); magnitude = integerSquareRoot64(product + __ll_mul((s64) sp54, (s64) sp54)); if (magnitude != 0) {
        arg0->accelerationY = (s64)arg0->velocityY * sp50 / magnitude;
        newVelocity = -arg0->velocityY;
    } else {
        newVelocity = -arg0->velocityY;
        arg0->accelerationY = newVelocity;
    }

    arg0->accelerationY += D_80121D80[arg0->playerIndex].unk44;
    arg0->velocityY = newVelocity;
    arg0->targetAngle = D_80121D80[arg0->playerIndex].yaw;

    source.z = 0;
    source.y = 0x280000;
    source.x = 0x100000;

    if (D_80121D80[arg0->playerIndex].flags & 0x400) {
        source.x = -0x100000;
        arg0->targetAngle += 0x800;
    }

    transformVec3iByFixedMatrix(D_80121D80[arg0->playerIndex].transform, &source, &arg0->pos);

    arg0->pos.x += D_80121D80[arg0->playerIndex].posA8.x;
    arg0->pos.y += D_80121D80[arg0->playerIndex].posA8.y;
    arg0->pos.z += D_80121D80[arg0->playerIndex].posA8.z;
    arg0->startAngle = D_80121D80[arg0->playerIndex].surfaceAngle;
    func_80045990(getMemoryBlockBase(D_8011216C), 5, &arg0->image, &arg0->palette);
    arg0->unk54 = 0;
    func_8004D5C0(arg0);
    setCallbackTaskCallback(arg0, func_8004D5C0);
}
