#include "common.h"
#include "memory_allocator.h"
#include "fixed_point_math.h"
#include "race_ui_effects.h"
#include "snowboard_trail_effects.h"

#define SNOWBOARD_TRAIL_TIMER 0xF0
#define SNOWBOARD_TRAIL_FLAG_FACING_BACKWARD 0x400
#define SNOWBOARD_TRAIL_FLAG_CANCEL 0x3040

struct SnowboardTrailState {
    /* 0x00 */ s16 state;
    /* 0x02 */ u8 pad02[0x04 - 0x02];
    /* 0x04 */ Vec3i localOffset;
    /* 0x10 */ Vec3i scale;
    /* 0x1C */ Vec3i worldPos;
    /* 0x28 */ s16 rotation[9];
    /* 0x3A */ u8 pad3A[0x3C - 0x3A];
    /* 0x3C */ Vec3i drawPos;
    /* 0x48 */ s16 transform[16];
    /* 0x68 */ s16 modelYaw;
    /* 0x6A */ s16 spinYaw;
    /* 0x6C */ void *frontDisplayList;
    /* 0x70 */ void *backDisplayList;
    /* 0x74 */ s16 scaleStep;
    /* 0x76 */ u8 displayListsDirty;
};

typedef struct {
    /* 0x000 */ u8 pad000[0x014 - 0x000];
    /* 0x014 */ s8 disabled;
    /* 0x015 */ u8 pad015[0x028 - 0x015];
    /* 0x028 */ u8 trailFrontSource[0x20];
    /* 0x048 */ u8 trailBackSource[0x20];
    /* 0x068 */ u8 pad068[0x06C - 0x068];
    /* 0x06C */ void *trailFrontDisplayList;
    /* 0x070 */ void *trailBackDisplayList;
    /* 0x074 */ u8 pad074[0x076 - 0x074];
    /* 0x076 */ u8 trailDisplayListsDirty;
    /* 0x077 */ u8 pad077[0x094 - 0x077];
    /* 0x094 */ s16 modelTransform[10];
    /* 0x0A8 */ Vec3i velocity;
    /* 0x0B4 */ u8 pad0B4[0x2DA - 0x0B4];
    /* 0x2DA */ s16 trailTimer;
    /* 0x2DC */ u8 trailSide;
    /* 0x2DD */ u8 pad2DD[0x2FC - 0x2DD];
    /* 0x2FC */ s32 flags;
    /* 0x300 */ u8 pad300[0x58C - 0x300];
    /* 0x58C */ SnowboardTrailState trail;
} SnowboardTrailPlayer;

extern void func_800483FC(void *queue, void (*callback)(SnowboardTrailState *), SnowboardTrailState *trail);
extern void *func_8004885C(u8 *source);

extern u8 gRaceUpdatePaused;
extern s16 D_80112144;
extern s16 D_80112146;
extern u32 D_2002208[];
extern u32 D_20023A8[];
extern Gfx *gRegionAllocPtr;
extern void *D_801248BC;
extern u8 D_80156609;

void func_800837D0(SnowboardTrailState *trail) {
    Gfx *unused;

    if (D_80156609 != 0) {
        trail->displayListsDirty = 1;
    }

    if (trail->displayListsDirty != 0) {
        trail->displayListsDirty = 0;
        trail->frontDisplayList = func_8004885C((u8 *)trail->rotation);
        trail->backDisplayList = func_8004885C((u8 *)trail->transform);
    }

    if (trail->frontDisplayList != NULL) {
        gDPPipeSync(gRegionAllocPtr++);
        gSPSegment(gRegionAllocPtr++, 0x02, func_80043040(D_80112144));
        gSPSegment(gRegionAllocPtr++, 0x03, func_80043040(D_80112146));
        gSPMatrix(gRegionAllocPtr++, trail->frontDisplayList, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_2002208);
        gSPMatrix(gRegionAllocPtr++, trail->backDisplayList, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_20023A8);
    }
}

// func_8008393C best match: 99.958%
#pragma GLOBAL_ASM("asm/nonmatchings/snowboard_trail_effects/func_8008393C.s")

#ifdef NON_MATCHING
void func_8008393C(struct RaceInputPlayer *input) {
    SnowboardTrailPlayer *player = (SnowboardTrailPlayer *)input;
    SnowboardTrailState *trail;
    FixedMatrix3s *playerTransform;
    FixedMatrix3s *rotation;
    FixedTransform scratch;

    trail = &player->trail;
    switch (player->trail.state) {
    case 0:
        return;
    case 1:
        break;
    case 2:
        goto state_2;
    default:
        return;
    }

    {
        trail->spinYaw += 0x240;
        playerTransform = (FixedMatrix3s *)player->modelTransform;
        transformVec3ByFixedMatrix(*playerTransform, &trail->localOffset, &trail->worldPos);
        trail->worldPos.x += player->velocity.x;
        trail->worldPos.y += player->velocity.y;
        trail->worldPos.z += player->velocity.z;
        makeFixedRotateY(scratch.rotation, trail->modelYaw);
        rotation = (FixedMatrix3s *)trail->rotation;
        multiplyFixedMatrices(scratch.rotation, *playerTransform, *rotation);
        trail->drawPos.x = trail->worldPos.x;
        trail->drawPos.y = trail->worldPos.y;
        trail->drawPos.z = trail->worldPos.z;
        trail->rotation[0] = (trail->rotation[0] * trail->scaleStep) / 16;
        trail->rotation[1] = (trail->rotation[1] * trail->scaleStep) / 16;
        trail->rotation[2] = (trail->rotation[2] * trail->scaleStep) / 16;
        trail->rotation[3] = (trail->rotation[3] * trail->scaleStep) / 16;
        trail->rotation[4] = (trail->rotation[4] * trail->scaleStep) / 16;
        trail->rotation[5] = (trail->rotation[5] * trail->scaleStep) / 16;
        trail->rotation[6] = (trail->rotation[6] * trail->scaleStep) / 16;
        trail->rotation[7] = (trail->rotation[7] * trail->scaleStep) / 16;
        trail->rotation[8] = (trail->rotation[8] * trail->scaleStep) / 16;
        makeFixedRotateX(scratch.rotation, trail->spinYaw);
        scratch.translation.x = trail->scale.x;
        scratch.translation.y = trail->scale.y;
        scratch.translation.z = trail->scale.z;
        composeFixedTransforms(&scratch, (FixedTransform *)rotation, (FixedTransform *)trail->transform);
        if (gRaceUpdatePaused == 0) {
            trail->scaleStep++;
        }
        if (trail->scaleStep == 0x10) {
            trail->state = 2;
        }
        if (player->disabled == 0) {
            func_800483FC(&D_801248BC, func_800837D0, trail);
        }
    }
    return;

state_2:
    {
        trail->spinYaw += 0x240;
        playerTransform = (FixedMatrix3s *)player->modelTransform;
        transformVec3ByFixedMatrix(*playerTransform, &trail->localOffset, &trail->worldPos);
        trail->worldPos.x += player->velocity.x;
        trail->worldPos.y += player->velocity.y;
        trail->worldPos.z += player->velocity.z;
        makeFixedRotateY(scratch.rotation, trail->modelYaw);
        rotation = (FixedMatrix3s *)trail->rotation;
        multiplyFixedMatrices(scratch.rotation, *playerTransform, *rotation);
        trail->drawPos.x = trail->worldPos.x;
        trail->drawPos.y = trail->worldPos.y;
        trail->drawPos.z = trail->worldPos.z;
        makeFixedRotateX(scratch.rotation, trail->spinYaw);
        scratch.translation.x = trail->scale.x;
        scratch.translation.y = trail->scale.y;
        scratch.translation.z = trail->scale.z;
        composeFixedTransforms(&scratch, (FixedTransform *)rotation, (FixedTransform *)trail->transform);
        if (player->flags & SNOWBOARD_TRAIL_FLAG_CANCEL) {
            player->trailTimer = 0;
        }
        if (gRaceUpdatePaused == 0) {
            if (player->trailTimer != 0) {
                player->trailTimer--;
            }
        }
        if (player->trailTimer == 0) {
            trail->state = 0;
            func_8005F5C8((struct RaceUiSnowboardTrailPlayer *)player);
            return;
        }
        if (player->disabled == 0) {
            func_800483FC(&D_801248BC, func_800837D0, trail);
        }
    }
}
#endif

void func_80083CFC(struct RaceInputPlayer *input) {
    SnowboardTrailPlayer *player = (SnowboardTrailPlayer *)input;
    SnowboardTrailState *trail = &player->trail;

    player->trailTimer = SNOWBOARD_TRAIL_TIMER;
    if (player->flags & SNOWBOARD_TRAIL_FLAG_FACING_BACKWARD) {
        trail->localOffset.x = 0x300000;
        trail->localOffset.y = 0;
        trail->localOffset.z = 0;
        trail->modelYaw = 0;
        player->trailSide = 1;
    } else {
        trail->localOffset.x = -0x300000;
        trail->localOffset.y = 0;
        trail->localOffset.z = 0;
        trail->modelYaw = 0x800;
        player->trailSide = 0;
    }
    trail->spinYaw = 0;
    trail->scale.x = 0xF0000;
    trail->scale.y = 0x230000;
    trail->scale.z = 0;
    trail->scaleStep = 1;
    trail->state = 1;
}
