#ifndef RACE_ITEM_WEAPONS_H
#define RACE_ITEM_WEAPONS_H

#include "spatial_math.h"

typedef struct RaceItemWeaponActor RaceItemWeaponActor;

s32 findRaceItemWeaponTargetPlayer(Vec3i *pos, s32 radius, s16 angle, s16 playerIndex, s16 *outAngle);
void renderWideHomingItemProjectile(RaceItemWeaponActor *arg0);
void updateWideHomingItemProjectile(RaceItemWeaponActor *arg0);
void initWideHomingItemProjectile(RaceItemWeaponActor *arg0);
void renderCloseRangeHomingItemProjectile(RaceItemWeaponActor *arg0);
void updateCloseRangeHomingItemProjectile(RaceItemWeaponActor *arg0);
void initCloseRangeHomingItemProjectile(RaceItemWeaponActor *arg0);
void renderBouncingItemProjectile(RaceItemWeaponActor *arg0);
void updateBouncingItemProjectile(RaceItemWeaponActor *arg0);
void initBouncingItemProjectile(RaceItemWeaponActor *arg0);
void renderThrownTrailImpactProjectile(RaceItemWeaponActor *arg0);
void updateThrownTrailImpactProjectile(RaceItemWeaponActor *arg0);
void initThrownTrailImpactProjectile(RaceItemWeaponActor *arg0);
void createThrownTrailImpactProjectile(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4);
void renderAreaBlastItemProjectile(RaceItemWeaponActor *arg0);
void updateAreaBlastItemProjectile(RaceItemWeaponActor *arg0);
void initAreaBlastItemProjectile(RaceItemWeaponActor *arg0);
void renderLongRangeHomingItemProjectile(RaceItemWeaponActor *arg0);
void updateLongRangeHomingItemProjectile(RaceItemWeaponActor *arg0);
void initLongRangeHomingItemProjectile(RaceItemWeaponActor *arg0);
void renderFallingActionProjectile(RaceItemWeaponActor *arg0);
void updateFallingActionProjectileLanded(RaceItemWeaponActor *arg0);
void updateFallingActionProjectile(RaceItemWeaponActor *arg0);
void initFallingActionProjectile(RaceItemWeaponActor *arg0);
void renderShieldProjectile(RaceItemWeaponActor *arg0);
void updateShieldProjectile(RaceItemWeaponActor *arg0);
void initShieldProjectile(RaceItemWeaponActor *arg0);

#endif
