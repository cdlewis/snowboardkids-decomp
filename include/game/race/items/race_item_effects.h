#ifndef RACE_ITEM_EFFECTS_H
#define RACE_ITEM_EFFECTS_H

#include "common.h"
#include "game/math/spatial_math.h"

typedef struct RaceItemEffectActor RaceItemEffectActor;
typedef struct RaceItemFollowActor RaceItemFollowActor;
typedef struct RaceItemTextureActor RaceItemTextureActor;

s32 getRaceItemEffectType(s32 arg0);
void updateRaceItemSparkBurst(RaceItemEffectActor *arg0);
void initRaceItemSparkBurst(RaceItemEffectActor *arg0);
void spawnRaceItemTrackSparkBurst(Vec3i *arg0, Vec3i *arg1, Vec3i *arg2, Vec3i *arg3, s32 arg4, s16 arg5);
void renderRacePlayerHitEffect(RaceItemEffectActor *arg0);
void updateRacePlayerHitEffect(RaceItemEffectActor *arg0);
void initRacePlayerHitEffect(RaceItemEffectActor *arg0);
void spawnRacePlayerHitEffect(s16 arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4);
void spawnRaceItemImpactEffect(s32 arg0, s32 arg1, s32 arg2, s16 arg3);
void renderRaceItemProjectileTrailEffect(RaceItemEffectActor *arg0);
void updateRaceItemProjectileTrailEffect(RaceItemEffectActor *arg0);
void initRaceItemProjectileTrailEffect(RaceItemEffectActor *arg0);
void spawnRaceItemProjectileTrailEffect(s32 arg0, s32 arg1, s32 arg2, s16 arg3);
void renderRacePlayerShockEffect(RaceItemEffectActor *arg0);
void updateRacePlayerShockEffect(RaceItemEffectActor *arg0);
void initRacePlayerShockEffect(RaceItemEffectActor *arg0);
void renderRaceItemBreakParticle(RaceItemEffectActor *arg0);
void updateRaceItemBreakParticle(RaceItemEffectActor *arg0);
void initRaceItemBreakParticle(RaceItemEffectActor *arg0);
void spawnRaceItemBreakParticles(s16 playerIndex, s16 itemIndex);
void renderRacePlayerRecoverySparkle(RaceItemEffectActor *arg0);
void updateRacePlayerRecoverySparkle(RaceItemEffectActor *arg0);
void initRacePlayerRecoverySparkle(RaceItemEffectActor *arg0);
void renderRacePlayerSnowSpray(RaceItemFollowActor *arg0);
void updateRacePlayerSnowSpray(RaceItemFollowActor *arg0);
void initRacePlayerSnowSpray(RaceItemFollowActor *arg0);
void renderRaceUiSparkle(RaceItemEffectActor *arg0);
void updateRaceUiSparkle(RaceItemEffectActor *arg0);
void initRaceUiSparkle(RaceItemEffectActor *arg0);
void spawnRaceUiSparkle(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4);
void renderRaceItemTextureEffects(RaceItemTextureActor *arg0);
void updateRaceItemTextureEffects(RaceItemTextureActor *arg0);
void initRaceItemTextureEffects(RaceItemTextureActor *arg0);
void renderRacePlayerLandingSnowSpray(RaceItemFollowActor *arg0);
void updateRacePlayerLandingSnowSpray(RaceItemFollowActor *arg0);
void initRacePlayerLandingSnowSpray(RaceItemFollowActor *arg0);

#endif
