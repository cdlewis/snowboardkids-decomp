#ifndef RACE_COURSE_PROPS_AND_PICKUPS_H
#define RACE_COURSE_PROPS_AND_PICKUPS_H

#include "common.h"

typedef struct CourseEffectModelListActor CourseEffectModelListActor;
typedef struct RacePickupActor RacePickupActor;
typedef struct ThrownPickupSpawnerActor ThrownPickupSpawnerActor;
typedef struct ThrownPickupRenderActor ThrownPickupRenderActor;
typedef struct PickupShardParticleActor PickupShardParticleActor;
typedef struct ThrownPickupModelActor ThrownPickupModelActor;

void renderRaceCoursePropModels(CourseEffectModelListActor *arg0);
void updateRaceCoursePropModels(CourseEffectModelListActor *arg0);
void initRaceCoursePropModels(CourseEffectModelListActor *arg0);
void renderCourseOverlaySprites(CourseEffectModelListActor *arg0);
void updateCourseOverlaySprites(CourseEffectModelListActor *arg0);
void initCourseOverlaySpriteMatrices(CourseEffectModelListActor *arg0);
void initCourseOverlaySprites(CourseEffectModelListActor *arg0);
void renderThrownPickupModel(ThrownPickupRenderActor *arg0);
void updateThrownPickupModel(ThrownPickupModelActor *arg0);
void initThrownPickupModel(ThrownPickupModelActor *arg0);
void spawnThrownPickupModel(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4);
void updateThrownPickupSpawner(ThrownPickupSpawnerActor *arg0);
void renderRacePickupIdle(RacePickupActor *arg0);
void renderRacePickupBase(RacePickupActor *arg0);
void renderRacePickupRespawn(RacePickupActor *arg0);
void updateRacePickupRespawn(RacePickupActor *arg0);
void updateRacePickupBounce(RacePickupActor *arg0);
void updateRacePickupCollected(RacePickupActor *arg0);
void updateRacePickupIdle(RacePickupActor *arg0);
void initRacePickup(RacePickupActor *arg0);
void renderPickupShardParticle(PickupShardParticleActor *arg0);
void updatePickupShardParticle(PickupShardParticleActor *arg0);
void initPickupShardParticle(PickupShardParticleActor *arg0);
void spawnPickupShardParticle(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4);

#endif
