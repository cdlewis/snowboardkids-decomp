#ifndef RACE_ITEM_PROJECTILES_H
#define RACE_ITEM_PROJECTILES_H

#include "spatial_math.h"

typedef struct RaceItemProjectileActor RaceItemProjectileActor;

s32 findRaceItemProjectileTargetPlayer(Vec3i *pos, s32 radius, s16 angle, s16 playerIndex, s16 *outAngle);
void renderRaceItemProjectileType1(RaceItemProjectileActor *arg0);
void updateRaceItemProjectileType1(RaceItemProjectileActor *arg0);
void initRaceItemProjectileType1(RaceItemProjectileActor *arg0);
void renderRaceItemProjectileType3(RaceItemProjectileActor *arg0);
void updateRaceItemProjectileType3(RaceItemProjectileActor *arg0);
void initRaceItemProjectileType3(RaceItemProjectileActor *arg0);
void renderRaceItemProjectileType4(RaceItemProjectileActor *arg0);
void updateRaceItemProjectileType4(RaceItemProjectileActor *arg0);
void initRaceItemProjectileType4(RaceItemProjectileActor *arg0);
void renderThrownTrailImpactProjectile(RaceItemProjectileActor *arg0);
void updateThrownTrailImpactProjectile(RaceItemProjectileActor *arg0);
void initThrownTrailImpactProjectile(RaceItemProjectileActor *arg0);
void createThrownTrailImpactProjectile(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4);
void renderRaceItemProjectileType5(RaceItemProjectileActor *arg0);
void updateRaceItemProjectileType5(RaceItemProjectileActor *arg0);
void initRaceItemProjectileType5(RaceItemProjectileActor *arg0);
void renderRaceItemProjectileType2(RaceItemProjectileActor *arg0);
void updateRaceItemProjectileType2(RaceItemProjectileActor *arg0);
void initRaceItemProjectileType2(RaceItemProjectileActor *arg0);
void renderFallingActionProjectile(RaceItemProjectileActor *arg0);
void updateFallingActionProjectileLanded(RaceItemProjectileActor *arg0);
void updateFallingActionProjectile(RaceItemProjectileActor *arg0);
void initFallingActionProjectile(RaceItemProjectileActor *arg0);
void renderShieldProjectile(RaceItemProjectileActor *arg0);
void updateShieldProjectile(RaceItemProjectileActor *arg0);
void initShieldProjectile(RaceItemProjectileActor *arg0);

#endif
