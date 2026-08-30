#include "common.h"
#include "game/race/effects/snowboard_trail_effects.h"
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/engine/system_runtime.h"
#include "game/engine/relocatable_heap.h"
#include "game/race/player/race_player_input.h"
#include "game/race/ui/race_ui_effects.h"
#include "game/math/spatial_math.h"
#include "game/math/fixed_point_math.h"
#include "game/menu/renderer/menu_render_utils.h"

#define SNOWBOARD_TRAIL_TIMER 0xF0
#define SNOWBOARD_TRAIL_FLAG_FACING_BACKWARD 0x400
#define SNOWBOARD_TRAIL_FLAG_CANCEL 0x3040

extern u8 gRaceUpdatePaused;
extern u32 gSnowboardTrailFrontDisplayList[];
extern u32 gSnowboardTrailBackDisplayList[];
extern Gfx *gRegionAllocPtr;

void renderSnowboardTrailEffect(SnowboardTrailState *trail) {
    Gfx *unused;

    if (gRenderMatricesDirty != 0) {
        trail->displayListsDirty = 1;
    }

    if (trail->displayListsDirty != 0) {
        trail->displayListsDirty = 0;
        trail->frontDisplayList = allocFixedTransformMatrix(&trail->frontTransform);
        trail->backDisplayList = allocFixedTransformMatrix(&trail->backTransform);
    }

    if (trail->frontDisplayList != NULL) {
        gDPPipeSync(gRegionAllocPtr++);
        gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[0xA]));
        gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[0xB]));
        gSPMatrix(gRegionAllocPtr++, trail->frontDisplayList, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gSnowboardTrailFrontDisplayList);
        gSPMatrix(gRegionAllocPtr++, trail->backDisplayList, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gSnowboardTrailBackDisplayList);
    }
}

void updateSnowboardTrailEffect(RacePlayer *player) {
    SnowboardTrailState *trail;
    Transform3D scratch;

    trail = &player->snowboardTrail;
    switch (player->snowboardTrail.state) {
        case 0:
            break;
        case 1:
            trail->spinYaw += 0x240;
            transformVec3iByFixedMatrix(player->modelPartTransforms[0].rotation, &trail->localOffset, &trail->worldPos);
            trail->worldPos.x += player->modelPartTransforms[0].translation.x;
            trail->worldPos.y += player->modelPartTransforms[0].translation.y;
            trail->worldPos.z += player->modelPartTransforms[0].translation.z;
            makeFixedRotationY(scratch.rotation, trail->modelYaw);
            multiplyFixedMatrix3s(scratch.rotation, player->modelPartTransforms[0].rotation, trail->frontTransform.rotation);
            trail->frontTransform.translation.x = trail->worldPos.x;
            trail->frontTransform.translation.y = trail->worldPos.y;
            trail->frontTransform.translation.z = trail->worldPos.z;
            trail->frontTransform.rotation[0] = (trail->frontTransform.rotation[0] * trail->scaleStep) / 16;
            trail->frontTransform.rotation[1] = (trail->frontTransform.rotation[1] * trail->scaleStep) / 16;
            trail->frontTransform.rotation[2] = (trail->frontTransform.rotation[2] * trail->scaleStep) / 16;
            trail->frontTransform.rotation[3] = (trail->frontTransform.rotation[3] * trail->scaleStep) / 16;
            trail->frontTransform.rotation[4] = (trail->frontTransform.rotation[4] * trail->scaleStep) / 16;
            trail->frontTransform.rotation[5] = (trail->frontTransform.rotation[5] * trail->scaleStep) / 16;
            trail->frontTransform.rotation[6] = (trail->frontTransform.rotation[6] * trail->scaleStep) / 16;
            trail->frontTransform.rotation[7] = (trail->frontTransform.rotation[7] * trail->scaleStep) / 16;
            trail->frontTransform.rotation[8] = (trail->frontTransform.rotation[8] * trail->scaleStep) / 16;
            makeFixedRotationX(scratch.rotation, trail->spinYaw);
            scratch.translation.x = trail->scale.x;
            scratch.translation.y = trail->scale.y;
            scratch.translation.z = trail->scale.z;
            composeFixedTransforms(&scratch, &trail->frontTransform, &trail->backTransform);
            if (gRaceUpdatePaused == 0) {
                trail->scaleStep++;
            }
            if (trail->scaleStep == 0x10) {
                trail->state = 2;
            }
            if (player->soundDisabled == 0) {
                addRenderCallback(
                    &gRaceModelEffectRenderCallbackList,
                    (RenderCallback)renderSnowboardTrailEffect,
                    trail
                );
            }
            return;
        case 2:
            trail->spinYaw += 0x240;
            transformVec3iByFixedMatrix(player->modelPartTransforms[0].rotation, &trail->localOffset, &trail->worldPos);
            trail->worldPos.x += player->modelPartTransforms[0].translation.x;
            trail->worldPos.y += player->modelPartTransforms[0].translation.y;
            trail->worldPos.z += player->modelPartTransforms[0].translation.z;
            makeFixedRotationY(scratch.rotation, trail->modelYaw);
            multiplyFixedMatrix3s(scratch.rotation, player->modelPartTransforms[0].rotation, trail->frontTransform.rotation);
            trail->frontTransform.translation.x = trail->worldPos.x;
            trail->frontTransform.translation.y = trail->worldPos.y;
            trail->frontTransform.translation.z = trail->worldPos.z;
            makeFixedRotationX(scratch.rotation, trail->spinYaw);
            scratch.translation.x = trail->scale.x;
            scratch.translation.y = trail->scale.y;
            scratch.translation.z = trail->scale.z;
            composeFixedTransforms(&scratch, &trail->frontTransform, &trail->backTransform);
            if (player->stateFlags & SNOWBOARD_TRAIL_FLAG_CANCEL) {
                player->trailEffectTimer = 0;
            }
            if (gRaceUpdatePaused == 0) {
                if (player->trailEffectTimer != 0) {
                    player->trailEffectTimer--;
                }
            }
            if (player->trailEffectTimer == 0) {
                trail->state = 0;
                spawnRaceUiSnowboardTrailEffect(player);
                return;
            }
            if (player->soundDisabled == 0) {
                addRenderCallback(
                    &gRaceModelEffectRenderCallbackList,
                    (RenderCallback)renderSnowboardTrailEffect,
                    trail
                );
            }
            return;
        default:
            return;
    }
}

void startSnowboardTrailEffect(RacePlayer *player) {
    SnowboardTrailState *trail = &player->snowboardTrail;

    player->trailEffectTimer = SNOWBOARD_TRAIL_TIMER;
    if (player->stateFlags & SNOWBOARD_TRAIL_FLAG_FACING_BACKWARD) {
        trail->localOffset.x = 0x300000;
        trail->localOffset.y = 0;
        trail->localOffset.z = 0;
        trail->modelYaw = 0;
        player->snowboardTrailSide = 1;
    } else {
        trail->localOffset.x = -0x300000;
        trail->localOffset.y = 0;
        trail->localOffset.z = 0;
        trail->modelYaw = 0x800;
        player->snowboardTrailSide = 0;
    }
    trail->spinYaw = 0;
    trail->scale.x = 0xF0000;
    trail->scale.y = 0x230000;
    trail->scale.z = 0;
    trail->scaleStep = 1;
    trail->state = 1;
}
