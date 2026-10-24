#ifndef RACE_PROJECTILES_H
#define RACE_PROJECTILES_H

#include "game/math/spatial_math.h"

typedef struct {
    /* 0x00 */ Transform3D source;
    /* 0x20 */ s32 pad20;
} RaceEffectMatrixScratch;

typedef struct RaceItemProjectileActor {
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
} RaceItemProjectileActor;

extern Vtx gRaceItemProjectileQuadVertices[4];
extern Vtx gRockItemQuadVertices[4];

extern u8 gRaceItemEffectTypeTableVariants[][28];

s32 findRaceItemProjectileHomingTarget(Vec3i *pos, s32 radius, s16 angle, s16 playerIndex, s16 *outAngle);
void renderWideHomingShotProjectile(RaceItemProjectileActor *arg0);
void updateWideHomingShotProjectile(RaceItemProjectileActor *arg0);
void initWideHomingShotProjectile(RaceItemProjectileActor *arg0);
void renderCloseRangeHomingShotProjectile(RaceItemProjectileActor *arg0);
void updateCloseRangeHomingShotProjectile(RaceItemProjectileActor *arg0);
void initCloseRangeHomingShotProjectile(RaceItemProjectileActor *arg0);
void renderBouncingShotProjectile(RaceItemProjectileActor *arg0);
void updateBouncingShotProjectile(RaceItemProjectileActor *arg0);
void initBouncingShotProjectile(RaceItemProjectileActor *arg0);
void renderThrownTrailImpactProjectile(RaceItemProjectileActor *arg0);
void updateThrownTrailImpactProjectile(RaceItemProjectileActor *arg0);
void initThrownTrailImpactProjectile(RaceItemProjectileActor *arg0);
void createThrownTrailImpactProjectile(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4);
void renderAreaBlastShotProjectile(RaceItemProjectileActor *arg0);
void updateAreaBlastShotProjectile(RaceItemProjectileActor *arg0);
void initAreaBlastShotProjectile(RaceItemProjectileActor *arg0);
void renderLongRangeHomingShotProjectile(RaceItemProjectileActor *arg0);
void updateLongRangeHomingShotProjectile(RaceItemProjectileActor *arg0);
void initLongRangeHomingShotProjectile(RaceItemProjectileActor *arg0);
void renderRockItem(RaceItemProjectileActor *arg0);
void updateRockItemLanded(RaceItemProjectileActor *arg0);
void updateRockItem(RaceItemProjectileActor *arg0);
void initRockItem(RaceItemProjectileActor *arg0);
void renderShieldProjectile(RaceItemProjectileActor *arg0);
void updateShieldProjectile(RaceItemProjectileActor *arg0);
void initShieldProjectile(RaceItemProjectileActor *arg0);

#endif
