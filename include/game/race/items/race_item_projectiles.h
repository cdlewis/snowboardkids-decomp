#ifndef RACE_ITEM_PROJECTILES_H
#define RACE_ITEM_PROJECTILES_H

#include "game/math/spatial_math.h"

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

s32 findRaceItemProjectileHomingTarget(Vec3i *pos, s32 radius, s16 angle, s16 playerIndex, s16 *outAngle);
void renderWideHomingItemProjectile(RaceItemProjectileActor *arg0);
void updateWideHomingItemProjectile(RaceItemProjectileActor *arg0);
void initWideHomingItemProjectile(RaceItemProjectileActor *arg0);
void renderCloseRangeHomingItemProjectile(RaceItemProjectileActor *arg0);
void updateCloseRangeHomingItemProjectile(RaceItemProjectileActor *arg0);
void initCloseRangeHomingItemProjectile(RaceItemProjectileActor *arg0);
void renderBouncingItemProjectile(RaceItemProjectileActor *arg0);
void updateBouncingItemProjectile(RaceItemProjectileActor *arg0);
void initBouncingItemProjectile(RaceItemProjectileActor *arg0);
void renderThrownTrailImpactProjectile(RaceItemProjectileActor *arg0);
void updateThrownTrailImpactProjectile(RaceItemProjectileActor *arg0);
void initThrownTrailImpactProjectile(RaceItemProjectileActor *arg0);
void createThrownTrailImpactProjectile(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4);
void renderAreaBlastItemProjectile(RaceItemProjectileActor *arg0);
void updateAreaBlastItemProjectile(RaceItemProjectileActor *arg0);
void initAreaBlastItemProjectile(RaceItemProjectileActor *arg0);
void renderLongRangeHomingItemProjectile(RaceItemProjectileActor *arg0);
void updateLongRangeHomingItemProjectile(RaceItemProjectileActor *arg0);
void initLongRangeHomingItemProjectile(RaceItemProjectileActor *arg0);
void renderFallingActionProjectile(RaceItemProjectileActor *arg0);
void updateFallingActionProjectileLanded(RaceItemProjectileActor *arg0);
void updateFallingActionProjectile(RaceItemProjectileActor *arg0);
void initFallingActionProjectile(RaceItemProjectileActor *arg0);
void renderShieldProjectile(RaceItemProjectileActor *arg0);
void updateShieldProjectile(RaceItemProjectileActor *arg0);
void initShieldProjectile(RaceItemProjectileActor *arg0);

#endif
