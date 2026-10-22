#include "game/race/course/race_course_effects.h"
#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/engine/render_callback.h"
#include "game/engine/system_runtime.h"
#include "game/race/items/race_projectiles.h"
#include "game/audio/sound_manager.h"
#include "game/race/motion/race_motion.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/race/effects/race_visual_effects.h"
#include "game/race/player/race_player_movement.h"
#include "game/race/player/race_player_input.h"
#include "game/race/ui/race_ui_effects.h"
#include "game/math/fixed_point_math.h"
#include "game/math/spatial_math.h"

typedef struct {
    s32 pad;
    Vec3i transformed;
    Vec3i offset;
} RaceItemProjectileTransformScratch;

extern Gfx *gRegionAllocPtr;
extern u8 gRaceUpdatePaused;
extern s16 gUiBlinkTimer;

Vtx gRaceItemProjectileQuadVertices[4] = {
    { { { -6, 6, 0 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 6, 6, 0 }, 0, { 496, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 6, -6, 0 }, 0, { 496, 496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -6, -6, 0 }, 0, { -16, 496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

Vtx gRockItemQuadVertices[4] = {
    { { { -3, 3, 0 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 3, 3, 0 }, 0, { 496, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 3, -3, 0 }, 0, { 496, 496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -3, -3, 0 }, 0, { -16, 496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

u8 gRaceItemEffectTypeTableVariants[4][28] = {
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
            if ((player->invisibilityTimer == 0) && !(player->stateFlags & 0x402000) && (player->isActive != 0)) {
                dx = player->pos.x - pos->x;
                if (dx < 0) {
                    dx = -dx;
                }
                if (dx < radius) {
                    dist = player->collisionBottomY - pos->y;
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

void renderWideHomingShotProjectile(RaceItemProjectileActor *arg0) {
    RaceEffectMatrixScratch sp6C;
    volatile u8 padding[8];

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

        do {
            if (arg0->matrix != NULL) {
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
                gDPLoadTextureBlock_4b(
                    gRegionAllocPtr++,
                    arg0->image,
                    G_IM_FMT_CI,
                    16,
                    16,
                    0,
                    G_TX_CLAMP,
                    G_TX_CLAMP,
                    G_TX_NOMASK,
                    G_TX_NOMASK,
                    G_TX_NOLOD,
                    G_TX_NOLOD
                );
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette);
                gSPMatrix(gRegionAllocPtr++, (Mtx *)arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, (Mtx *)gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPVertex(gRegionAllocPtr++, gRaceItemProjectileQuadVertices, 4, 0);
                gSP1Quadrangle(gRegionAllocPtr++, 2, 1, 0, 3, 3);
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
            }
        } while (0);
    }
}

void updateWideHomingShotProjectile(RaceItemProjectileActor *arg0) {
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
            gRacePlayers[arg0->targetPlayerIndex].isTargetedByAttack = 1;
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
            enqueuePositionalSoundEffect(SFX_PROJECTILE_IMPACT, pos, 0x7F, SFX_PRIORITY_NORMAL);
            spawnRaceItemImpactEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            removeCallbackTask(arg0);
            return;
        }

        arg0->timer--;
        spawnRaceItemProjectileTrailEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 0);
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderWideHomingShotProjectile, arg0);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initWideHomingShotProjectile(RaceItemProjectileActor *arg0) {
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
    if (gRacePlayers[arg0->playerIndex].stateFlags & 0x400) { \
        source.x = -0x1000; \
    } \
    transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].modelPartTransforms[0].rotation, &source, &transformed); \
    product = __ll_mul((s64)transformed.x, (s64)transformed.x); \
    magnitude = integerSquareRoot64(product + __ll_mul((s64)transformed.z, (s64)transformed.z)); \
    if (magnitude != 0) {
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
    transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].modelPartTransforms[0].rotation, &source, &arg0->pos);
    arg0->pos.x += gRacePlayers[arg0->playerIndex].modelPartTransforms[0].translation.x;
    arg0->pos.y += gRacePlayers[arg0->playerIndex].modelPartTransforms[0].translation.y;
    arg0->pos.z += gRacePlayers[arg0->playerIndex].modelPartTransforms[0].translation.z;
    arg0->startAngle = gRacePlayers[arg0->playerIndex].coursePathIndex;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1E]), 0, &arg0->image, &arg0->palette);
    updateWideHomingShotProjectile(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateWideHomingShotProjectile);
}
// clang-format on

void renderCloseRangeHomingShotProjectile(RaceItemProjectileActor *arg0) {
    RaceEffectMatrixScratch sp64;
    volatile u8 padding[8];

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

        do {
            if (arg0->matrix != NULL) {
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
                gDPLoadTextureBlock_4b(
                    gRegionAllocPtr++,
                    arg0->image,
                    G_IM_FMT_CI,
                    16,
                    16,
                    0,
                    G_TX_CLAMP,
                    G_TX_CLAMP,
                    G_TX_NOMASK,
                    G_TX_NOMASK,
                    G_TX_NOLOD,
                    G_TX_NOLOD
                );
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette);
                gSPMatrix(gRegionAllocPtr++, (Mtx *)arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, (Mtx *)gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPVertex(gRegionAllocPtr++, gRaceItemProjectileQuadVertices, 4, 0);
                gSP1Quadrangle(gRegionAllocPtr++, 2, 1, 0, 3, 3);
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
            }
        } while (0);
    }
}

void updateCloseRangeHomingShotProjectile(RaceItemProjectileActor *arg0) {
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
            gRacePlayers[arg0->targetPlayerIndex].isTargetedByAttack = 1;
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
                enqueuePositionalSoundEffect(SFX_PROJECTILE_IMPACT, pos, 0x7F, SFX_PRIORITY_NORMAL);
            }
            spawnRaceItemImpactEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            removeCallbackTask(arg0);
            return;
        }

        arg0->timer--;
        spawnRaceItemProjectileTrailEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderCloseRangeHomingShotProjectile, arg0);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initCloseRangeHomingShotProjectile(RaceItemProjectileActor *arg0) {
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
    if (gRacePlayers[arg0->playerIndex].stateFlags & 0x400) { \
        source.x = -0x1000; \
    } \
    transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].modelPartTransforms[0].rotation, &source, &transformed); \
    product = __ll_mul((s64)transformed.x, (s64)transformed.x); \
    magnitude = integerSquareRoot64(product + __ll_mul((s64)transformed.z, (s64)transformed.z)); \
    if (magnitude != 0) {
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
    transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].modelPartTransforms[0].rotation, &source, &arg0->pos);
    arg0->pos.x += gRacePlayers[arg0->playerIndex].modelPartTransforms[0].translation.x;
    arg0->pos.y += gRacePlayers[arg0->playerIndex].modelPartTransforms[0].translation.y;
    arg0->pos.z += gRacePlayers[arg0->playerIndex].modelPartTransforms[0].translation.z;
    arg0->startAngle = gRacePlayers[arg0->playerIndex].coursePathIndex;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1E]), 2, &arg0->image, &arg0->palette);
    updateCloseRangeHomingShotProjectile(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCloseRangeHomingShotProjectile);
}
// clang-format on

void renderBouncingShotProjectile(RaceItemProjectileActor *arg0) {
    RaceEffectMatrixScratch sp64;

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

        do {
            if (arg0->matrix != NULL) {
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
                gDPLoadTextureBlock_4b(
                    gRegionAllocPtr++,
                    arg0->image,
                    G_IM_FMT_CI,
                    16,
                    16,
                    0,
                    G_TX_CLAMP,
                    G_TX_CLAMP,
                    G_TX_NOMASK,
                    G_TX_NOMASK,
                    G_TX_NOLOD,
                    G_TX_NOLOD
                );
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette);
                gSPMatrix(gRegionAllocPtr++, (Mtx *)arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, (Mtx *)gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPVertex(gRegionAllocPtr++, gRaceItemProjectileQuadVertices, 4, 0);
                gSP1Quadrangle(gRegionAllocPtr++, 2, 1, 0, 3, 3);
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
            }
        } while (0);
    }
}

void updateBouncingShotProjectile(RaceItemProjectileActor *arg0) {
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
            gRacePlayers[projectile->targetPlayerIndex].isTargetedByAttack = 1;
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
            enqueuePositionalSoundEffect(SFX_ITEM_IMPACT, pos, 0x7F, SFX_PRIORITY_NORMAL);
        }

        for (i = 0; i < 4; i++) {
            if ((i != arg0->playerIndex || arg0->timer < 0x4B) && tryApplyRacePlayerItemHit(pos, 0x30000, 0x40, i)) {
                gRacePlayers[i].hitSourcePlayerIndex = arg0->playerIndex;
                arg0->timer = 0;
                i = 4;
            }
        }

        if (arg0->timer == 0) {
            enqueuePositionalSoundEffect(SFX_PROJECTILE_IMPACT, pos, 0x7F, SFX_PRIORITY_NORMAL);
            spawnRaceItemImpactEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            removeCallbackTask(arg0);
            return;
        }

        arg0->timer--;
        spawnRaceItemProjectileTrailEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 3);
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderBouncingShotProjectile, arg0);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initBouncingShotProjectile(RaceItemProjectileActor *arg0) {
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
    if (gRacePlayers[arg0->playerIndex].stateFlags & 0x400) { \
        source.x = -0x1000; \
    } \
    transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].modelPartTransforms[0].rotation, &source, &transformed); \
    product = __ll_mul((s64)transformed.x, (s64)transformed.x); \
    magnitude = integerSquareRoot64(product + __ll_mul((s64)transformed.z, (s64)transformed.z)); \
    if (magnitude != 0) {
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
    transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].modelPartTransforms[0].rotation, &source, &arg0->pos);
    arg0->pos.x += gRacePlayers[arg0->playerIndex].modelPartTransforms[0].translation.x;
    arg0->pos.y += gRacePlayers[arg0->playerIndex].modelPartTransforms[0].translation.y;
    arg0->pos.z += gRacePlayers[arg0->playerIndex].modelPartTransforms[0].translation.z;
    arg0->startAngle = gRacePlayers[arg0->playerIndex].coursePathIndex;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1E]), 3, &arg0->image, &arg0->palette);
    updateBouncingShotProjectile(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateBouncingShotProjectile);
}
// clang-format on

void renderThrownTrailImpactProjectile(RaceItemProjectileActor *arg0) {
    RaceEffectMatrixScratch sp64;

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

        do {
            if (arg0->matrix != NULL) {
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
                gDPLoadTextureBlock_4b(
                    gRegionAllocPtr++,
                    arg0->image,
                    G_IM_FMT_CI,
                    16,
                    16,
                    0,
                    G_TX_CLAMP,
                    G_TX_CLAMP,
                    G_TX_NOMASK,
                    G_TX_NOMASK,
                    G_TX_NOLOD,
                    G_TX_NOLOD
                );
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette);
                gSPMatrix(gRegionAllocPtr++, (Mtx *)arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, (Mtx *)gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPVertex(gRegionAllocPtr++, gRaceItemProjectileQuadVertices, 4, 0);
                gSP1Quadrangle(gRegionAllocPtr++, 2, 1, 0, 3, 3);
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
            }
        } while (0);
    }
}

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
                enqueuePositionalSoundEffect(SFX_PROJECTILE_IMPACT, pos, 0x7F, SFX_PRIORITY_NORMAL);
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

void renderAreaBlastShotProjectile(RaceItemProjectileActor *arg0) {
    RaceEffectMatrixScratch sp64;

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

        do {
            if (arg0->matrix != NULL) {
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
                gDPLoadTextureBlock_4b(
                    gRegionAllocPtr++,
                    arg0->image,
                    G_IM_FMT_CI,
                    16,
                    16,
                    0,
                    G_TX_CLAMP,
                    G_TX_CLAMP,
                    G_TX_NOMASK,
                    G_TX_NOMASK,
                    G_TX_NOLOD,
                    G_TX_NOLOD
                );
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette);
                gSPMatrix(gRegionAllocPtr++, (Mtx *)arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, (Mtx *)gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPVertex(gRegionAllocPtr++, gRaceItemProjectileQuadVertices, 4, 0);
                gSP1Quadrangle(gRegionAllocPtr++, 2, 1, 0, 3, 3);
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
            }
        } while (0);
    }
}

void updateAreaBlastShotProjectile(RaceItemProjectileActor *arg0) {
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
            gRacePlayers[arg0->targetPlayerIndex].isTargetedByAttack = 1;
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

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderAreaBlastShotProjectile, arg0);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initAreaBlastShotProjectile(RaceItemProjectileActor *arg0) {
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

    actor = arg0; \
    transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].modelPartTransforms[0].rotation, &sp58, &transformed); \
    product = __ll_mul((s64)transformed.x, (s64)transformed.x); \
    magnitude = integerSquareRoot64(product + __ll_mul((s64)transformed.z, (s64)transformed.z)); \
    if (magnitude != 0) { \
        actor->accelerationY = (s64)actor->velocityY * transformed.y / magnitude; \
        velocityY = -actor->velocityY; \
    } else { \
        velocityY = -actor->velocityY; \
        actor->accelerationY = velocityY; \
    } \
    actor->accelerationY += gRacePlayers[actor->playerIndex].velocity.y; \
    actor->velocityY = velocityY; \
    actor->targetAngle = gRacePlayers[actor->playerIndex].facingAngle; \
    sp58.z = 0; \
    sp58.x = 0xFFF00000;
    sp58.y = 0x280000;
    sp58.x = 0x100000;

    if (gRacePlayers[actor->playerIndex].stateFlags & 0x400) {
        sp58.x = 0xFFF00000;
        actor->targetAngle += 0x800;
    }

    transformVec3iByFixedMatrix(gRacePlayers[actor->playerIndex].modelPartTransforms[0].rotation, &sp58, &actor->pos);
    actor->pos.x += (0, gRacePlayers)[actor->playerIndex].modelPartTransforms[0].translation.x;
    actor->pos.y += gRacePlayers[actor->playerIndex].modelPartTransforms[0].translation.y;
    actor->pos.z += gRacePlayers[actor->playerIndex].modelPartTransforms[0].translation.z;
    actor->startAngle = gRacePlayers[actor->playerIndex].coursePathIndex;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1E]), 4, &actor->image, &actor->palette);
    updateAreaBlastShotProjectile(actor);
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateAreaBlastShotProjectile);
}
// clang-format on

void renderLongRangeHomingShotProjectile(RaceItemProjectileActor *arg0) {
    RaceEffectMatrixScratch sp64;

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

        do {
            if (arg0->matrix != NULL) {
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
                gDPLoadTextureBlock_4b(
                    gRegionAllocPtr++,
                    arg0->image,
                    G_IM_FMT_CI,
                    16,
                    16,
                    0,
                    G_TX_CLAMP,
                    G_TX_CLAMP,
                    G_TX_NOMASK,
                    G_TX_NOMASK,
                    G_TX_NOLOD,
                    G_TX_NOLOD
                );
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette);
                gSPMatrix(gRegionAllocPtr++, (Mtx *)arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, (Mtx *)gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPVertex(gRegionAllocPtr++, gRaceItemProjectileQuadVertices, 4, 0);
                gSP1Quadrangle(gRegionAllocPtr++, 2, 1, 0, 3, 3);
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
            }
        } while (0);
    }
}

void updateLongRangeHomingShotProjectile(RaceItemProjectileActor *arg0) {
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
            gRacePlayers[arg0->targetPlayerIndex].isTargetedByAttack = 1;
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
            enqueuePositionalSoundEffect(SFX_PROJECTILE_IMPACT, pos, 0x7F, SFX_PRIORITY_NORMAL);
            spawnRaceItemImpactEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            removeCallbackTask(arg0);
            return;
        }

        arg0->timer--;
        spawnRaceItemProjectileTrailEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 1);
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderLongRangeHomingShotProjectile, arg0);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initLongRangeHomingShotProjectile(RaceItemProjectileActor *arg0) {
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

    actor = arg0; \
    transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].modelPartTransforms[0].rotation, &sp58, &transformed); \
    product = __ll_mul((s64)transformed.x, (s64)transformed.x); \
    magnitude = integerSquareRoot64(product + __ll_mul((s64)transformed.z, (s64)transformed.z)); \
    if (magnitude != 0) { \
        actor->accelerationY = (s64)actor->velocityY * transformed.y / magnitude; \
        velocityY = -actor->velocityY; \
    } else { \
        velocityY = -actor->velocityY; \
        actor->accelerationY = velocityY; \
    } \
    actor->accelerationY += gRacePlayers[actor->playerIndex].velocity.y; \
    actor->velocityY = velocityY; \
    actor->targetAngle = gRacePlayers[actor->playerIndex].facingAngle; \
    sp58.z = 0;
    new_var = &sp58;
    sp58.y = 0x280000;
    sp58.x = 0x100000;

    if (gRacePlayers[actor->playerIndex].stateFlags & 0x400) {
        sp58.x = 0xFFF00000;
        actor->targetAngle += 0x800;
    }

    transformVec3iByFixedMatrix(gRacePlayers[actor->playerIndex].modelPartTransforms[0].rotation, new_var, &actor->pos);
    actor->pos.x += gRacePlayers[actor->playerIndex].modelPartTransforms[0].translation.x;
    actor->pos.y += gRacePlayers[actor->playerIndex].modelPartTransforms[0].translation.y;
    actor->pos.z += gRacePlayers[actor->playerIndex].modelPartTransforms[0].translation.z;
    actor->startAngle = gRacePlayers[actor->playerIndex].coursePathIndex;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1E]), 1, &actor->image, &actor->palette);
    updateLongRangeHomingShotProjectile(actor);
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateLongRangeHomingShotProjectile);
}
// clang-format on

void renderRockItem(RaceItemProjectileActor *arg0) {
    RaceEffectMatrixScratch sp64;

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

        do {
            if (arg0->matrix != NULL) {
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
                gDPLoadTextureBlock_4b(
                    gRegionAllocPtr++,
                    arg0->image,
                    G_IM_FMT_CI,
                    16,
                    16,
                    0,
                    G_TX_CLAMP,
                    G_TX_CLAMP,
                    G_TX_NOMASK,
                    G_TX_NOMASK,
                    G_TX_NOLOD,
                    G_TX_NOLOD
                );
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette);
                gSPMatrix(gRegionAllocPtr++, (Mtx *)arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, (Mtx *)gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPVertex(gRegionAllocPtr++, gRockItemQuadVertices, 4, 0);
                gSP1Quadrangle(gRegionAllocPtr++, 2, 1, 0, 3, 3);
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
            }
        } while (0);
    }
}

void updateRockItemLanded(RaceItemProjectileActor *arg0) {
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
            enqueuePositionalSoundEffect(SFX_COLLISION_IMPACT, pos, 0x7F, SFX_PRIORITY_NORMAL);
            spawnRaceItemImpactEffect(arg0->pos.x, actor->pos.y, actor->pos.z, 1);
            removeCallbackTask(arg0);
            return;
        }
        i++;
    } while (i != 4);

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderRockItem, actor);
}

void updateRockItem(RaceItemProjectileActor *arg0) {
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
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRockItemLanded);
        }

        for (i = 0; i != 4; i++) {
            pos = &arg0->pos;
            if ((i != arg0->playerIndex || arg0->timer == 0) && tryApplyRacePlayerItemHit(pos, 0x30000, 0x400, i)) {
                enqueuePositionalSoundEffect(SFX_COLLISION_IMPACT, pos, 0x7F, SFX_PRIORITY_NORMAL);
                spawnRaceItemImpactEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 1);
                removeCallbackTask(arg0);
                return;
            }
        }

        if (arg0->timer != 0) {
            arg0->timer--;
        }
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderRockItem, arg0);
}

void initRockItem(RaceItemProjectileActor *arg0) {
    RacePlayer *player;

    arg0->timer = 0x3C;
    arg0->blinkTimer = 0x708;
    arg0->accelerationY = 0x30000;
    player = &gRacePlayers[arg0->playerIndex];
    arg0->pos.x = player->modelPartTransforms[9].translation.x;
    arg0->pos.y = player->modelPartTransforms[9].translation.y;
    arg0->pos.z = player->modelPartTransforms[9].translation.z;
    enqueuePositionalSoundEffect(SFX_PROJECTILE_LAUNCH, &arg0->pos, 0x7F, SFX_PRIORITY_NORMAL);
    arg0->angle = gRacePlayers[arg0->playerIndex].coursePathIndex;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 2, &arg0->image, &arg0->palette);
    updateRockItem(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRockItem);
}

void renderShieldProjectile(RaceItemProjectileActor *arg0) {
    RaceEffectMatrixScratch sp64;
    volatile u8 padding[8];

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

        do {
            if (arg0->matrix != NULL) {
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
                gDPLoadTextureBlock_4b(
                    gRegionAllocPtr++,
                    arg0->image,
                    G_IM_FMT_CI,
                    16,
                    16,
                    0,
                    G_TX_CLAMP,
                    G_TX_CLAMP,
                    G_TX_NOMASK,
                    G_TX_NOMASK,
                    G_TX_NOLOD,
                    G_TX_NOLOD
                );
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette);
                gSPMatrix(gRegionAllocPtr++, (Mtx *)arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, (Mtx *)gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPVertex(gRegionAllocPtr++, gRaceItemProjectileQuadVertices, 4, 0);
                gSP1Quadrangle(gRegionAllocPtr++, 2, 1, 0, 3, 3);
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
            }
        } while (0);
    }
}

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
                enqueuePositionalSoundEffect(SFX_PROJECTILE_IMPACT, &arg0->pos, 0x7F, SFX_PRIORITY_NORMAL);
            }
        } else {
            arg0->timer = 0;
            enqueuePositionalSoundEffect(SFX_ITEM_IMPACT, &arg0->pos, 0x7F, SFX_PRIORITY_NORMAL);
        }

        if (arg0->timer == 0) {
            spawnRaceItemImpactEffect(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            removeCallbackTask((RaceItemProjectileActor *)arg0);
            gRacePlayers[arg0->playerIndex].shieldShotCount++;
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
    if (gRacePlayers[arg0->playerIndex].stateFlags & 0x400) { \
        source.x = -0x1000; \
    } \
    transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].modelPartTransforms[0].rotation, &source, &transformed); \
    product = __ll_mul((s64)transformed.x, (s64)transformed.x); \
    magnitude = integerSquareRoot64(product + __ll_mul((s64)transformed.z, (s64)transformed.z)); \
    if (magnitude != 0) {
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

    transformVec3iByFixedMatrix(gRacePlayers[arg0->playerIndex].modelPartTransforms[0].rotation, &source, &arg0->pos);

    arg0->pos.x += gRacePlayers[arg0->playerIndex].modelPartTransforms[0].translation.x;
    arg0->pos.y += gRacePlayers[arg0->playerIndex].modelPartTransforms[0].translation.y;
    arg0->pos.z += gRacePlayers[arg0->playerIndex].modelPartTransforms[0].translation.z;
    arg0->startAngle = gRacePlayers[arg0->playerIndex].coursePathIndex;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1E]), 5, &arg0->image, &arg0->palette);
    arg0->unk54 = 0;
    updateShieldProjectile(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateShieldProjectile);
}
// clang-format on
