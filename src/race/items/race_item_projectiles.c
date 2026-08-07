#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/engine/render_callback.h"
#include "game/engine/system_runtime.h"
#define calculateFixedAngleBetweenXZPoints calculateFixedAngleBetweenXZPoints_s32
#include "game/race/items/race_item_projectiles.h"
#undef calculateFixedAngleBetweenXZPoints
#include "game/race/motion/race_motion.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/race/items/race_item_effects.h"
#include "game/race/player/race_player_movement.h"
#include "game/race/player/race_player_input.h"
#include "game/math/fixed_point_math.h"

typedef struct {
    /* 0x00 */ FixedTransform source;
    /* 0x20 */ s32 pad20;
} RaceEffectMatrixScratch;

typedef struct {
    s32 pad;
    Vec3i transformed;
    Vec3i offset;
} TransformScratch;

struct RaceItemProjectileActor {
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
    union {
        /* 0x3A */ s16 targetPlayerIndex;
        /* 0x3A */ s16 blinkTimer;
    };
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

extern Gfx gEffectRenderModeSetupDl[];
extern Gfx gEffectRenderModeCleanupDl[];
extern Gfx *gRegionAllocPtr;
extern u8 gRaceUpdatePaused;
extern s16 gUiBlinkTimer;

Vtx gRaceItemProjectileQuadVertices[4] = {
    { { { -6, 6, 0 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 6, 6, 0 }, 0, { 496, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 6, -6, 0 }, 0, { 496, 496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -6, -6, 0 }, 0, { -16, 496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

Vtx gFallingActionProjectileQuadVertices[4] = {
    { { { -3, 3, 0 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 3, 3, 0 }, 0, { 496, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 3, -3, 0 }, 0, { 496, 496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -3, -3, 0 }, 0, { -16, 496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

u8 D_800D4660[4][28] = {
    {
     0,  3,  1, 3, 0,     0,         0,        1, 3, 3,           1,                           4,                                  1,                          4,         3,              4,                                                                                     4,                                                         3, 3,                                                                                      3,                                                                               3,                                                3,                      3,                                                                    3,                                                                                                                                                                                                            3,                                                                                                                                                                                                                                                                                                                0,                                                                                                                                                                                                                                                              0,                                                                                                                          0,
     },
    {
     0,  3,            1,         3,        0, 0, 0, 1, 3, 3,           1,                                     4,                                                                      1,                                                                                     4,                                                         3, 4, 4, 1, 1,                                                                                      1,                                                                                                                                                                    1,                                                                                                                                                                                                                   1,                                                                                                                                                   1,                                                   1,                                            1,                                                                                                                                                                                                         0,                                                                                                                                                                                                                                                                                                                                                                                                                    0,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  0,
     },
    {
     2, 3, 1,         3,                  0,                   0,                           2,                                  1,                          3,         3, 1, 4, 1, 4, 3,              4,                                                                                                  4,                                                                                                                                                          3,                                                                                                                                                          3,                                                                                                                                                                                                                                               3,                                                                                                                                                                                                                                                                                                                3,                                                                                                                                                                                                                                                              3,                                                                                                                          3,                                    3, 3, 0, 0, 0,
     },
    {
     3, 3,  1,  3, 0,                   0,                                    3,                                        1,                                        3,                                       3,                                                1,                                                                          4,                                                                                         1,                                                                               4,                                                3,                      4,                                                                    4,                                                                             3,    3, 3, 3, 3, 3, 3,                                                                                                                                3,                                                                                                                                                                                                                                                                                                                                                                                                                                            0,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         0,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  0,
     },
};

Mtx *allocFixedTransformMatrix(FixedTransform *);
void spawnRaceUiFadingImpact(s32, s32, s32, u16);
void enqueuePositionalSoundEffect(s32, void *, s32, s32);
s16 calculateFixedAngleBetweenXZPoints(s32, s32, s32, s32);
s16 fixedSine(s16);
s16 fixedCosine(s16);
s64 __ll_mul(s64, s64);

s32 findRaceItemProjectileHomingTarget(Vec3i *pos, s32 radius, s16 angle, s16 playerIndex, s16 *outAngle) {
    RacePlayer *player;
    s32 closest;
    s32 dx;
    s32 dz;
    s32 dist;
    s32 i;
    s32 hit;
    s16 angleDiff;
    s16 playerAngle;

    closest = radius + 0x1000;
    hit = -1;
    i = 0;
    do {
        if (playerIndex != i) {
            player = &gRacePlayers[i];
            if ((player->actionSoundTimer == 0) && !(player->stateFlags & 0x402000) && (player->isActive != 0)) {
                dx = player->pos.x - pos->x;
                if (dx < 0) {
                    dx = -dx;
                }
                if (dx < radius) {
                    dist = player->unk5C - pos->y;
                    if (dist < 0) {
                        dist = -dist;
                    }
                    if (dist < radius) {
                        dz = player->pos.z - pos->z;
                        if (dz < 0) {
                            dz = -dz;
                        }
                        if (dz < radius) {
                            playerAngle =
                                calculateFixedAngleBetweenXZPoints(pos->x, pos->z, player->pos.x, player->pos.z);
                            angleDiff = playerAngle - angle;
                            if ((s16)((angleDiff + 0x380) & 0xFFF) < 0x700) {
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

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void renderWideHomingItemProjectile(RaceItemProjectileActor *arg0) {
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

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sp6C.source = gIdentityFixedTransform;
            sp6C.source.translation.x = arg0->pos.x;
            sp6C.source.translation.y = arg0->pos.y;
            sp6C.source.translation.z = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix(&sp6C.source);
        }

        do { if (arg0->matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeSetupDl; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gViewportMatrix; } ; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) gRaceItemProjectileQuadVertices; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeCleanupDl; } ; } } while (0);
    }
}
// clang-format on

void updateWideHomingItemProjectile(RaceItemProjectileActor *arg0) {
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
        arg0->targetPlayerIndex =
            findRaceItemProjectileHomingTarget(pos, 0x1600000, arg0->targetAngle, arg0->playerIndex, &angleDiff);

        if (arg0->targetPlayerIndex != -1) {
            gRacePlayers[arg0->targetPlayerIndex].itemTargetFlag = 1;
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

        arg0->startAngle = findRaceCourseSurfaceFromHint(arg0->startAngle, arg0->pos.x, arg0->pos.z);
        groundY = getRaceCourseSurfaceHeight(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
        y = arg0->pos.y;
        if (y < groundY) {
            arg0->pos.y = groundY;
            y = groundY;
        }
        arg0->accelerationY = (y - prevY) - 0x20000;

        resolveRaceCourseSurfaceCollision(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ);
        if (pushX != 0 || pushZ != 0) {
            arg0->timer = 0;
            arg0->pos.x += pushX;
            arg0->pos.z += pushZ;
        }

        for (i = 0; i < 4; i++) {
            if (i != arg0->playerIndex && tryApplyRacePlayerItemHit(pos, 0x30000, 8, i)) {
                gRacePlayers[i].hitSourcePlayerIndex = arg0->playerIndex;
                arg0->timer = 0;
                i = 4;
            }
        }

        if (arg0->timer == 0) {
            enqueuePositionalSoundEffect(0xA, pos, 0x7F, 0x32);
            spawnRaceItemImpactEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            removeCallbackTask(arg0);
            return;
        }

        arg0->timer--;
        spawnRaceItemProjectileTrailEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 0);
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderWideHomingItemProjectile, arg0);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initWideHomingItemProjectile(RaceItemProjectileActor *arg0) {
    volatile s32 pad0;
    Vec3i source;
    Vec3i transformed;
    s32 magnitude;
    s32 var_a0;
    s64 product;

    arg0->timer = 0x12C;
    arg0->targetPlayerIndex = -1;
    arg0->velocityY = 0x130000;
    source.z = 0;
    source.y = 0;
    source.x = 0x1000;
    if (gRacePlayers[arg0->playerIndex].stateFlags & 0x400) { source.x = -0x1000; } transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].transform, &source, &transformed); product = __ll_mul((s64) transformed.x, (s64) transformed.x); magnitude = integerSquareRoot64(product + __ll_mul((s64) transformed.z, (s64) transformed.z)); if (magnitude != 0) {
        arg0->accelerationY = (((s64) arg0->velocityY) * transformed.y) / magnitude;
        var_a0 = -arg0->velocityY;
    } else {
        var_a0 = -arg0->velocityY;
        arg0->accelerationY = var_a0;
    }
    arg0->accelerationY += gRacePlayers[arg0->playerIndex].velocity.y;
    arg0->velocityY = var_a0;
    arg0->targetAngle = gRacePlayers[arg0->playerIndex].facingAngle;
    source.z = 0;
    source.y = 0x280000;
    source.x = 0x100000;
    if (gRacePlayers[arg0->playerIndex].stateFlags & 0x400) {
        source.x = 0xFFF00000;
        arg0->targetAngle += 0x800;
    }
    transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].transform, &source, &arg0->pos);
    arg0->pos.x += gRacePlayers[arg0->playerIndex].itemSpawnOffset.x;
    arg0->pos.y += gRacePlayers[arg0->playerIndex].itemSpawnOffset.y;
    arg0->pos.z += gRacePlayers[arg0->playerIndex].itemSpawnOffset.z;
    arg0->startAngle = gRacePlayers[arg0->playerIndex].coursePathIndex;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1E]), 0, &arg0->image, &arg0->palette);
    updateWideHomingItemProjectile(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateWideHomingItemProjectile);
}
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void renderCloseRangeHomingItemProjectile(RaceItemProjectileActor *arg0) {
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

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sp64.source = gIdentityFixedTransform;
            sp64.source.translation.x = arg0->pos.x;
            sp64.source.translation.y = arg0->pos.y;
            sp64.source.translation.z = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix(&sp64.source);
        }

        do { if (arg0->matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeSetupDl; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gViewportMatrix; } ; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) gRaceItemProjectileQuadVertices; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeCleanupDl; } ; } } while (0);
    }
}
// clang-format on

void updateCloseRangeHomingItemProjectile(RaceItemProjectileActor *arg0) {
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
        arg0->targetPlayerIndex =
            findRaceItemProjectileHomingTarget(pos, 0xE00000, arg0->targetAngle, arg0->playerIndex, &angleDiff);

        if (arg0->targetPlayerIndex != -1) {
            gRacePlayers[arg0->targetPlayerIndex].itemTargetFlag = 1;
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

        arg0->startAngle = findRaceCourseSurfaceFromHint(arg0->startAngle, arg0->pos.x, arg0->pos.z);
        groundY = getRaceCourseSurfaceHeight(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
        y = arg0->pos.y;
        if (y < groundY) {
            arg0->pos.y = groundY;
            y = groundY;
        }
        arg0->accelerationY = ((y - prevY) - 0x20000) & 0xFFFFFFFF;

        resolveRaceCourseSurfaceCollision(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ);
        hitPlayer = 0;
        if (pushX != 0 || pushZ != 0) {
            arg0->timer = 0;
            arg0->pos.x += pushX;
            arg0->pos.z += pushZ;
        }

        for (i = 0; i < 4; i++) {
            if (i != arg0->playerIndex && tryApplyRacePlayerItemHit(pos, 0x30000, 0x10, i)) {
                gRacePlayers[i].hitSourcePlayerIndex = arg0->playerIndex;
                hitPlayer = 1;
                arg0->timer = 0;
                i = 4;
            }
        }

        if (arg0->timer == 0) {
            if (hitPlayer == 0) {
                enqueuePositionalSoundEffect(0xA, pos, 0x7F, 0x32);
            }
            spawnRaceItemImpactEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            removeCallbackTask(arg0);
            return;
        }

        arg0->timer--;
        spawnRaceItemProjectileTrailEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderCloseRangeHomingItemProjectile, arg0);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initCloseRangeHomingItemProjectile(RaceItemProjectileActor *arg0) {
    volatile s32 pad0;
    Vec3i source;
    Vec3i transformed;
    s32 magnitude;
    s32 var_a0;
    s64 product;

    arg0->timer = 0x12C;
    arg0->targetPlayerIndex = -1;
    arg0->velocityY = 0x150000;
    source.z = 0;
    source.y = 0;
    source.x = 0x1000;
    if (gRacePlayers[arg0->playerIndex].stateFlags & 0x400) { source.x = -0x1000; } transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].transform, &source, &transformed); product = __ll_mul((s64) transformed.x, (s64) transformed.x); magnitude = integerSquareRoot64(product + __ll_mul((s64) transformed.z, (s64) transformed.z)); if (magnitude != 0) {
        arg0->accelerationY = (((s64) arg0->velocityY) * transformed.y) / magnitude;
        var_a0 = -arg0->velocityY;
    } else {
        var_a0 = -arg0->velocityY;
        arg0->accelerationY = var_a0;
    }
    arg0->accelerationY += gRacePlayers[arg0->playerIndex].velocity.y;
    arg0->velocityY = var_a0;
    arg0->targetAngle = gRacePlayers[arg0->playerIndex].facingAngle;
    source.z = 0;
    source.y = 0x280000;
    source.x = 0x100000;
    if (gRacePlayers[arg0->playerIndex].stateFlags & 0x400) {
        source.x = 0xFFF00000;
        arg0->targetAngle += 0x800;
    }
    transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].transform, &source, &arg0->pos);
    arg0->pos.x += gRacePlayers[arg0->playerIndex].itemSpawnOffset.x;
    arg0->pos.y += gRacePlayers[arg0->playerIndex].itemSpawnOffset.y;
    arg0->pos.z += gRacePlayers[arg0->playerIndex].itemSpawnOffset.z;
    arg0->startAngle = gRacePlayers[arg0->playerIndex].coursePathIndex;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1E]), 2, &arg0->image, &arg0->palette);
    updateCloseRangeHomingItemProjectile(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCloseRangeHomingItemProjectile);
}
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void renderBouncingItemProjectile(RaceItemProjectileActor *arg0) {
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

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sp64.source = gIdentityFixedTransform;
            sp64.source.translation.x = arg0->pos.x;
            sp64.source.translation.y = arg0->pos.y;
            sp64.source.translation.z = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix(&sp64.source);
        }

        do { if (arg0->matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeSetupDl; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gViewportMatrix; } ; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) gRaceItemProjectileQuadVertices; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeCleanupDl; } ; } } while (0);
    }
}
// clang-format on

void updateBouncingItemProjectile(RaceItemProjectileActor *arg0) {
    s16 *angleDiffOut;
    s32 sin;
    s32 cos;
    RaceItemProjectileActor *projectile;
    s32 pushX;
    s32 pushZ;
    s32 prevY;
    s32 y;
    s16 angleDiff;
    s32 groundY;
    Vec3i velocity;
    Vec3i *pos;
    s32 i;
    volatile u8 padding[4];

    if (gRaceUpdatePaused == 0) {
        projectile = arg0;
        pos = &projectile->pos;
        angleDiffOut = &angleDiff;
        projectile->targetPlayerIndex = findRaceItemProjectileHomingTarget(
            pos,
            0x600000,
            projectile->targetAngle,
            projectile->playerIndex,
            angleDiffOut
        );

        if (projectile->targetPlayerIndex != -1) {
            gRacePlayers[projectile->targetPlayerIndex].itemTargetFlag = 1;
            angleDiff = (angleDiff - projectile->targetAngle) & 0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }

            if (angleDiff >= 0x1D) {
                angleDiff = 0x1C;
            }
            if (angleDiff < -0x1C) {
                angleDiff = -0x1C;
            }

            projectile->targetAngle += angleDiff;
        }

        sin = fixedSine(arg0->targetAngle);
        cos = fixedCosine(arg0->targetAngle);
        velocity.x = ((s64)sin * arg0->velocityY) / 0x1000;
        velocity.z = ((s64)cos * arg0->velocityY) / 0x1000;

        prevY = arg0->pos.y;
        arg0->pos.x += velocity.x;
        arg0->pos.y = prevY + (arg0->accelerationY & 0xFFFFFFFFu);
        arg0->pos.z += velocity.z;

        arg0->startAngle = findRaceCourseSurfaceFromHint(arg0->startAngle, arg0->pos.x, arg0->pos.z);
        groundY = getRaceCourseSurfaceHeight(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
        y = arg0->pos.y;
        if (y < groundY) {
            arg0->pos.y = groundY;
            y = groundY;
        }
        arg0->accelerationY = (y - prevY) - 0x20000;

        resolveRaceCourseSurfaceCollisionWithVelocity(
            projectile->startAngle,
            arg0->pos.x,
            arg0->pos.z,
            0x20000,
            &pushX,
            &pushZ,
            &velocity.x,
            &velocity.z
        );
        if (pushX != 0 || pushZ != 0) {
            arg0->accelerationY = 0;
            arg0->pos.x += pushX;
            arg0->pos.z += pushZ;
            arg0->targetAngle = calculateFixedAngleFromDeltaXZ(velocity.x, velocity.z);
            enqueuePositionalSoundEffect(0x11, pos, 0x7F, 0x32);
        }

        for (i = 0; i < 4; i++) {
            if ((i != arg0->playerIndex || arg0->timer < 0x4B) && tryApplyRacePlayerItemHit(pos, 0x30000, 0x40, i)) {
                gRacePlayers[i].hitSourcePlayerIndex = arg0->playerIndex;
                arg0->timer = 0;
                i = 4;
            }
        }

        if (arg0->timer == 0) {
            enqueuePositionalSoundEffect(0xA, pos, 0x7F, 0x32);
            spawnRaceItemImpactEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            removeCallbackTask(arg0);
            return;
        }

        arg0->timer--;
        spawnRaceItemProjectileTrailEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 3);
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderBouncingItemProjectile, arg0);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initBouncingItemProjectile(RaceItemProjectileActor *arg0) {
    volatile s32 pad0;
    Vec3i source;
    Vec3i transformed;
    s32 magnitude;
    s32 var_a0;
    s64 product;

    arg0->timer = 0xB4;
    arg0->targetPlayerIndex = -1;
    arg0->velocityY = 0x170000;
    source.z = 0;
    source.y = 0;
    source.x = 0x1000;
    if (gRacePlayers[arg0->playerIndex].stateFlags & 0x400) { source.x = -0x1000; } transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].transform, &source, &transformed); product = __ll_mul((s64) transformed.x, (s64) transformed.x); magnitude = integerSquareRoot64(product + __ll_mul((s64) transformed.z, (s64) transformed.z)); if (magnitude != 0) {
        arg0->accelerationY = (((s64) arg0->velocityY) * transformed.y) / magnitude;
        var_a0 = -arg0->velocityY;
    } else {
        var_a0 = -arg0->velocityY;
        arg0->accelerationY = var_a0;
    }
    arg0->accelerationY += gRacePlayers[arg0->playerIndex].velocity.y;
    arg0->velocityY = var_a0;
    arg0->targetAngle = gRacePlayers[arg0->playerIndex].facingAngle;
    source.z = 0;
    source.y = 0x280000;
    source.x = 0x100000;
    if (gRacePlayers[arg0->playerIndex].stateFlags & 0x400) {
        source.x = 0xFFF00000;
        arg0->targetAngle += 0x800;
    }
    transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].transform, &source, &arg0->pos);
    arg0->pos.x += gRacePlayers[arg0->playerIndex].itemSpawnOffset.x;
    arg0->pos.y += gRacePlayers[arg0->playerIndex].itemSpawnOffset.y;
    arg0->pos.z += gRacePlayers[arg0->playerIndex].itemSpawnOffset.z;
    arg0->startAngle = gRacePlayers[arg0->playerIndex].coursePathIndex;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1E]), 3, &arg0->image, &arg0->palette);
    updateBouncingItemProjectile(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateBouncingItemProjectile);
}
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void renderThrownTrailImpactProjectile(RaceItemProjectileActor *arg0) {
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

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sp64.source = gIdentityFixedTransform;
            sp64.source.translation.x = arg0->pos.x;
            sp64.source.translation.y = arg0->pos.y;
            sp64.source.translation.z = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix(&sp64.source);
        }

        do { if (arg0->matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeSetupDl; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gViewportMatrix; } ; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) gRaceItemProjectileQuadVertices; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeCleanupDl; } ; } } while (0);
    }
}
// clang-format on

void updateThrownTrailImpactProjectile(RaceItemProjectileActor *arg0) {
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

        arg0->startAngle = findRaceCourseSurfaceFromHint(arg0->startAngle, arg0->pos.x, arg0->pos.z);
        groundY = getRaceCourseSurfaceHeight(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
        y = arg0->pos.y;
        if (y < groundY) {
            arg0->pos.y = groundY;
            y = groundY;
        }
        arg0->accelerationY = (y - prevY) - 0x20000;

        resolveRaceCourseSurfaceCollision(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ);
        pos = &arg0->pos;
        if (pushX != 0 || pushZ != 0) {
            arg0->timer = 0;
            arg0->pos.x += pushX;
            arg0->pos.z += pushZ;
        }

        for (i = 0; i < 4; i++) {
            if (tryApplyRacePlayerItemHit(pos, 0x30000, 0x2000, i)) {
                enqueuePositionalSoundEffect(0xA, pos, 0x7F, 0x32);
                spawnRaceItemImpactEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
                arg0->timer = 0;
                i = 4;
            }
        }

        if (arg0->timer == 0) {
            removeCallbackTask(arg0);
            return;
        }

        arg0->timer--;
        spawnRaceItemProjectileTrailEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 3);
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderThrownTrailImpactProjectile, arg0);
}

void initThrownTrailImpactProjectile(RaceItemProjectileActor *arg0) {
    arg0->timer = 0x3C;
    arg0->targetPlayerIndex = -1;
    arg0->velocityY = 0xFFF00000;
    arg0->accelerationY = 0;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1E]), 3, &arg0->image, &arg0->palette);
    updateThrownTrailImpactProjectile(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateThrownTrailImpactProjectile);
}

void createThrownTrailImpactProjectile(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4) {
    RaceItemProjectileActor *obj = createCallbackTask((CallbackTaskCallback)initThrownTrailImpactProjectile, 0, 0x1E);

    if (obj != NULL) {
        obj->pos.x = arg0;
        obj->pos.y = arg1;
        obj->pos.z = arg2;
        obj->targetAngle = arg4;
        obj->startAngle = arg3;
    }
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void renderAreaBlastItemProjectile(RaceItemProjectileActor *arg0) {
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

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sp64.source = gIdentityFixedTransform;
            sp64.source.translation.x = arg0->pos.x;
            sp64.source.translation.y = arg0->pos.y;
            sp64.source.translation.z = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix(&sp64.source);
        }

        do { if (arg0->matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeSetupDl; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gViewportMatrix; } ; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) gRaceItemProjectileQuadVertices; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeCleanupDl; } ; } } while (0);
    }
}
// clang-format on

void updateAreaBlastItemProjectile(RaceItemProjectileActor *arg0) {
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
        arg0->targetPlayerIndex =
            findRaceItemProjectileHomingTarget(pos, 0xA00000, arg0->targetAngle, arg0->playerIndex, &angleDiff);

        if (arg0->targetPlayerIndex != -1) {
            gRacePlayers[arg0->targetPlayerIndex].itemTargetFlag = 1;
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

        arg0->startAngle = findRaceCourseSurfaceFromHint(arg0->startAngle, arg0->pos.x, arg0->pos.z);
        groundY = getRaceCourseSurfaceHeight(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
        y = arg0->pos.y;
        if (y < groundY) {
            arg0->pos.y = groundY;
            y = groundY;
        }
        arg0->accelerationY = (y - prevY) - 0x20000;

        resolveRaceCourseSurfaceCollision(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ);
        i = 0;
        if (pushX != 0 || pushZ != 0) {
            arg0->timer = 0;
            arg0->pos.x += pushX;
            if (arg0->targetAngle) {}
            arg0->pos.z += pushZ;
        }

        do {
            if ((i != arg0->playerIndex) && tryApplyRacePlayerItemHit(pos, 0x30000, 0x80, i)) {
                gRacePlayers[i].hitSourcePlayerIndex = arg0->playerIndex;
                arg0->timer = 0;
                i = 4;
            }
            i++;
        } while (i < 4);

        if (arg0->timer == 0) {
            spawnRaceUiFadingImpact(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->playerIndex);
            removeCallbackTask(arg0);
            return;
        }

        arg0->timer--;
        spawnRaceItemProjectileTrailEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 4);
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderAreaBlastItemProjectile, arg0);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initAreaBlastItemProjectile(RaceItemProjectileActor *arg0) {
    volatile s32 pad0;
    Vec3i sp58;
    Vec3i transformed;
    s32 magnitude;
    s32 velocityY;
    RaceItemProjectileActor *actor;
    s64 product;

    arg0->timer = 0x12C;
    arg0->targetPlayerIndex = -1;
    arg0->velocityY = 0x110000;
    sp58.z = (sp58.y = 0);
    sp58.x = 0x1000;

    if (gRacePlayers[arg0->playerIndex].stateFlags & 0x400) {
        sp58.x = -0x1000;
        if (1) {
        }
    }

    actor = arg0; transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].transform, &sp58, &transformed); product = __ll_mul((s64) transformed.x, (s64) transformed.x); magnitude = integerSquareRoot64(product + __ll_mul((s64) transformed.z, (s64) transformed.z)); if (magnitude != 0) { actor->accelerationY = (s64)actor->velocityY * transformed.y / magnitude; velocityY = -actor->velocityY; } else { velocityY = -actor->velocityY; actor->accelerationY = velocityY; } actor->accelerationY += gRacePlayers[actor->playerIndex].velocity.y; actor->velocityY = velocityY; actor->targetAngle = gRacePlayers[actor->playerIndex].facingAngle; sp58.z = 0; sp58.x = 0xFFF00000;
    sp58.y = 0x280000;
    sp58.x = 0x100000;

    if (gRacePlayers[actor->playerIndex].stateFlags & 0x400) {
        sp58.x = 0xFFF00000;
        actor->targetAngle += 0x800;
    }

    transformVec3iByFixedMatrix(gRacePlayers[actor->playerIndex].transform, &sp58, &actor->pos);
    actor->pos.x += ((0, gRacePlayers))[actor->playerIndex].itemSpawnOffset.x;
    actor->pos.y += gRacePlayers[actor->playerIndex].itemSpawnOffset.y;
    actor->pos.z += gRacePlayers[actor->playerIndex].itemSpawnOffset.z;
    actor->startAngle = gRacePlayers[actor->playerIndex].coursePathIndex;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1E]), 4, &actor->image, &actor->palette);
    updateAreaBlastItemProjectile(actor);
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateAreaBlastItemProjectile);
}
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void renderLongRangeHomingItemProjectile(RaceItemProjectileActor *arg0) {
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

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sp64.source = gIdentityFixedTransform;
            sp64.source.translation.x = arg0->pos.x;
            sp64.source.translation.y = arg0->pos.y;
            sp64.source.translation.z = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix(&sp64.source);
        }

        do { if (arg0->matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeSetupDl; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gViewportMatrix; } ; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) gRaceItemProjectileQuadVertices; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeCleanupDl; } ; } } while (0);
    }
}
// clang-format on

void updateLongRangeHomingItemProjectile(RaceItemProjectileActor *arg0) {
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
        arg0->targetPlayerIndex =
            findRaceItemProjectileHomingTarget(pos, 0x1200000, arg0->targetAngle, arg0->playerIndex, &angleDiff);

        if (arg0->targetPlayerIndex != -1) {
            gRacePlayers[arg0->targetPlayerIndex].itemTargetFlag = 1;
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

        arg0->startAngle = findRaceCourseSurfaceFromHint(arg0->startAngle, arg0->pos.x, arg0->pos.z);
        groundY = getRaceCourseSurfaceHeight(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
        y = arg0->pos.y;
        if (y < groundY) {
            arg0->pos.y = groundY;
            y = groundY;
        }
        arg0->accelerationY = (y - prevY) - 0x20000;

        resolveRaceCourseSurfaceCollision(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ);
        if (pushX != 0 || pushZ != 0) {
            arg0->timer = 0;
            arg0->pos.x += pushX;
            arg0->pos.z += pushZ;
        }

        for (i = 0; i < 4; i++) {
            if ((i != arg0->playerIndex) && tryApplyRacePlayerItemHit(pos, 0x30000, 0x100, i)) {
                gRacePlayers[i].hitSourcePlayerIndex = arg0->playerIndex;
                arg0->timer = 0;
                i = 4;
            }
        }

        if (arg0->timer == 0) {
            enqueuePositionalSoundEffect(0xA, pos, 0x7F, 0x32);
            spawnRaceItemImpactEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            removeCallbackTask(arg0);
            return;
        }

        arg0->timer--;
        spawnRaceItemProjectileTrailEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 1);
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderLongRangeHomingItemProjectile, arg0);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initLongRangeHomingItemProjectile(RaceItemProjectileActor *arg0) {
    Vec3i *new_var;
    Vec3i sp58;
    Vec3i transformed;
    s32 magnitude;
    s32 velocityY;
    RaceItemProjectileActor *actor;
    s64 product;

    arg0->timer = 0x12C;
    arg0->targetPlayerIndex = -1;
    arg0->velocityY = 0x130000;
    sp58.z = (sp58.y = 0);
    sp58.x = 0x1000;

    if (gRacePlayers[arg0->playerIndex].stateFlags & 0x400) {
        sp58.x = -0x1000;
        if (1) {
        }
    }

    actor = arg0; transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].transform, &sp58, &transformed); product = __ll_mul((s64) transformed.x, (s64) transformed.x); magnitude = integerSquareRoot64(product + __ll_mul((s64) transformed.z, (s64) transformed.z)); if (magnitude != 0) { actor->accelerationY = (s64)actor->velocityY * transformed.y / magnitude; velocityY = -actor->velocityY; } else { velocityY = -actor->velocityY; actor->accelerationY = velocityY; } actor->accelerationY += gRacePlayers[actor->playerIndex].velocity.y; actor->velocityY = velocityY; actor->targetAngle = gRacePlayers[actor->playerIndex].facingAngle; sp58.z = 0;
    new_var = &sp58;
    sp58.y = 0x280000;
    sp58.x = 0x100000;

    if (gRacePlayers[actor->playerIndex].stateFlags & 0x400) {
        sp58.x = 0xFFF00000;
        actor->targetAngle += 0x800;
    }

    transformVec3iByFixedMatrix(gRacePlayers[actor->playerIndex].transform, new_var, &actor->pos);
    actor->pos.x += gRacePlayers[actor->playerIndex].itemSpawnOffset.x;
    actor->pos.y += gRacePlayers[actor->playerIndex].itemSpawnOffset.y;
    actor->pos.z += gRacePlayers[actor->playerIndex].itemSpawnOffset.z;
    actor->startAngle = gRacePlayers[actor->playerIndex].coursePathIndex;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1E]), 1, &actor->image, &actor->palette);
    updateLongRangeHomingItemProjectile(actor);
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateLongRangeHomingItemProjectile);
}
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void renderFallingActionProjectile(RaceItemProjectileActor *arg0) {
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

    if (gRenderMatricesDirty != 0) {
        arg0->matrixFlags.matrixDirty2 = 1;
    }

    if ((arg0->blinkTimer < 0x1F) && !(gUiBlinkTimer & 1)) {
        return;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixFlags.matrixDirty2 != 0) {
            arg0->matrixFlags.matrixDirty2 = 0;
            sp64.source = gIdentityFixedTransform;
            sp64.source.translation.x = arg0->pos.x;
            sp64.source.translation.y = arg0->pos.y;
            sp64.source.translation.z = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix(&sp64.source);
        }

        do { if (arg0->matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeSetupDl; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gViewportMatrix; } ; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) gFallingActionProjectileQuadVertices; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeCleanupDl; } ; } } while (0);
    }
}
// clang-format on

void updateFallingActionProjectileLanded(RaceItemProjectileActor *arg0) {
    Vec3i *pos;
    RaceItemProjectileActor *actor;
    s32 i;
    s32 radius;
    s16 blinkTimer;

    blinkTimer = arg0->blinkTimer;
    actor = arg0;
    if (blinkTimer != 0) {
        actor->blinkTimer = blinkTimer - 1;
    } else {
        removeCallbackTask(arg0);
        return;
    }

    i = 0;
    pos = &actor->pos;
    radius = 0x30000;
    do {
        if (tryApplyRacePlayerItemHit(pos, radius, 0x400, i)) {
            enqueuePositionalSoundEffect(0x14, pos, 0x7F, 0x32);
            spawnRaceItemImpactEffect(arg0->pos.x, actor->pos.y, actor->pos.z, 1);
            removeCallbackTask(arg0);
            return;
        }
        i++;
    } while (i != 4);

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderFallingActionProjectile, actor);
}

void updateFallingActionProjectile(RaceItemProjectileActor *arg0) {
    Vec3i *pos;
    s32 i;
    s32 accelerationY;
    s32 groundY;

    if (gRaceUpdatePaused == 0) {
        accelerationY = arg0->accelerationY;
        arg0->pos.y += accelerationY;
        arg0->accelerationY = accelerationY - 0x6000;

        arg0->angle = findRaceCourseSurfaceFromHint(arg0->angle, arg0->pos.x, arg0->pos.z);
        groundY = getRaceCourseSurfaceHeight(arg0->angle, arg0->pos.x, arg0->pos.z);
        if (arg0->pos.y < groundY + 0x30000) {
            arg0->pos.y = groundY + 0x30000;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateFallingActionProjectileLanded);
        }

        for (i = 0; i != 4; i++) {
            pos = &arg0->pos;
            if ((i != arg0->playerIndex || arg0->timer == 0) && tryApplyRacePlayerItemHit(pos, 0x30000, 0x400, i)) {
                enqueuePositionalSoundEffect(0x14, pos, 0x7F, 0x32);
                spawnRaceItemImpactEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 1);
                removeCallbackTask(arg0);
                return;
            }
        }

        if (arg0->timer != 0) {
            arg0->timer--;
        }
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderFallingActionProjectile, arg0);
}

void initFallingActionProjectile(RaceItemProjectileActor *arg0) {
    RacePlayer *player;

    arg0->timer = 0x3C;
    arg0->blinkTimer = 0x708;
    arg0->accelerationY = 0x30000;
    player = &gRacePlayers[arg0->playerIndex];
    arg0->pos.x = player->projectilePos.x;
    arg0->pos.y = player->projectilePos.y;
    arg0->pos.z = player->projectilePos.z;
    enqueuePositionalSoundEffect(0x6A, &arg0->pos, 0x7F, 0x32);
    arg0->angle = gRacePlayers[arg0->playerIndex].coursePathIndex;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 2, &arg0->image, &arg0->palette);
    updateFallingActionProjectile(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateFallingActionProjectile);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void renderShieldProjectile(RaceItemProjectileActor *arg0) {
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

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty2 = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty2 != 0) {
            arg0->matrixDirty2 = 0;
            sp64.source = gIdentityFixedTransform;
            sp64.source.translation.x = arg0->pos.x;
            sp64.source.translation.y = arg0->pos.y;
            sp64.source.translation.z = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix(&sp64.source);
        }

        do { if (arg0->matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeSetupDl; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gViewportMatrix; } ; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) gRaceItemProjectileQuadVertices; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gEffectRenderModeCleanupDl; } ; } } while (0);
    }
}
// clang-format on

void updateShieldProjectile(RaceItemProjectileActor *arg0) {
    s32 zOffset;
    s32 sin;
    s32 cos;
    Vec3i *pos;
    s32 pushX;
    s32 pushZ;
    s32 prevY;
    u8 padding[8];
    s32 y;
    s32 groundY;
    RacePlayer *player;
    s32 xOffset;
    s16 startAngle;

    if (gRaceUpdatePaused == 0) {
        if (arg0->unk54 == 0) {
            sin = fixedSine(arg0->targetAngle);
            cos = fixedCosine(arg0->targetAngle);
            xOffset = ((s64)sin * arg0->velocityY) / 0x1000;
            zOffset = ((s64)cos * arg0->velocityY) / 0x1000;
            y = arg0->pos.y;
            prevY = y;
            arg0->pos.x += xOffset;
            arg0->pos.y = (0, y) + arg0->accelerationY;
            arg0->pos.z += zOffset;

            arg0->startAngle = findRaceCourseSurfaceFromHint(arg0->startAngle, arg0->pos.x, arg0->pos.z);
            groundY = getRaceCourseSurfaceHeight(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
            y = arg0->pos.y;
            if (y < groundY) {
                arg0->pos.y = groundY;
                y = groundY;
            }
            arg0->accelerationY = (y - prevY) - 0x20000;

            resolveRaceCourseSurfaceCollision(
                startAngle = arg0->startAngle,
                arg0->pos.x,
                arg0->pos.z,
                0x20000,
                &pushX,
                &pushZ
            );
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
            spawnRaceItemImpactEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            removeCallbackTask((RaceItemProjectileActor *)arg0);
            gRacePlayers[arg0->playerIndex].shieldEffectTimer++;
            return;
        }

        arg0->timer--;
        spawnRaceItemProjectileTrailEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 5);
        arg0->prevPos = arg0->pos;
        arg0->radius = 0x30000;
        player = &gRacePlayers[arg0->playerIndex];
        arg0->anglePtr = player->anglePtr;
        player->anglePtr = (s16 *)&arg0->anglePtr;
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderShieldProjectile, arg0);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initShieldProjectile(RaceItemProjectileActor *arg0) {
    volatile s32 pad;
    Vec3i source;
    Vec3i transformed;
    s32 magnitude;
    s32 newVelocity;
    s64 product;

    arg0->timer = 0x12C;
    arg0->targetPlayerIndex = -1;
    arg0->velocityY = 0x120000;

    source.z = 0;
    source.y = 0;
    source.x = 0x1000;
    if (gRacePlayers[arg0->playerIndex].stateFlags & 0x400) { source.x = -0x1000; } transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].transform, &source, &transformed); product = __ll_mul((s64) transformed.x, (s64) transformed.x); magnitude = integerSquareRoot64(product + __ll_mul((s64) transformed.z, (s64) transformed.z)); if (magnitude != 0) {
        arg0->accelerationY = (s64)arg0->velocityY * transformed.y / magnitude;
        newVelocity = -arg0->velocityY;
    } else {
        newVelocity = -arg0->velocityY;
        arg0->accelerationY = newVelocity;
    }

    arg0->accelerationY += gRacePlayers[arg0->playerIndex].velocity.y;
    arg0->velocityY = newVelocity;
    arg0->targetAngle = gRacePlayers[arg0->playerIndex].facingAngle;

    source.z = 0;
    source.y = 0x280000;
    source.x = 0x100000;

    if (gRacePlayers[arg0->playerIndex].stateFlags & 0x400) {
        source.x = -0x100000;
        arg0->targetAngle += 0x800;
    }

    transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].transform, &source, &arg0->pos);

    arg0->pos.x += gRacePlayers[arg0->playerIndex].itemSpawnOffset.x;
    arg0->pos.y += gRacePlayers[arg0->playerIndex].itemSpawnOffset.y;
    arg0->pos.z += gRacePlayers[arg0->playerIndex].itemSpawnOffset.z;
    arg0->startAngle = gRacePlayers[arg0->playerIndex].coursePathIndex;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1E]), 5, &arg0->image, &arg0->palette);
    arg0->unk54 = 0;
    updateShieldProjectile(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateShieldProjectile);
}
// clang-format on
