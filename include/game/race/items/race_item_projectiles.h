#ifndef RACE_ITEM_PROJECTILES_H
#define RACE_ITEM_PROJECTILES_H

#include "game/math/spatial_math.h"

typedef struct RaceItemProjectileActor RaceItemProjectileActor;

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
