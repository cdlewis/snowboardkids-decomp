#ifndef RACE_COURSE_PROPS_AND_PICKUPS_H
#define RACE_COURSE_PROPS_AND_PICKUPS_H

#include "common.h"

typedef struct ThrownPickupSpawnerActor {
    char pad0[0x10];
    /* 0x10 */ u16 spawnIndex;
    char pad12[6];
    /* 0x18 */ s16 timer;
    /* 0x1A */ s8 lastVariant;
} ThrownPickupSpawnerActor;

typedef struct CourseEffectModelListActor {
    char pad0[0x10];
    /* 0x10 */ u16 modelListIndex;
    char pad12[6];
    /* 0x18 */ Mtx *modelBuffer;
    /* 0x1C */ s16 modelIndexOffset;
    /* 0x1E */ s16 modelCount;
} CourseEffectModelListActor;

typedef struct ThrownPickupRenderActor {
    char pad0[0x18];
    /* 0x18 */ Mtx *matrix;
    /* 0x1C */ Vec3i pos;
    /* 0x28 */ s16 pitch;
    /* 0x2A */ s16 yaw;
    char pad2C[2];
    /* 0x2E */ s8 matrixDirty;
} ThrownPickupRenderActor;

typedef struct ThrownPickupModelActor {
    char pad0[0x1C];
    /* 0x1C */ Vec3i pos;
    /* 0x28 */ s16 pitch;
    /* 0x2A */ s16 modelIndex;
    /* 0x2C */ s16 unk2C;
    char pad2E[2];
    /* 0x30 */ Vec3i transformedPos;
    /* 0x3C */ Vec3i velocity;
    /* 0x48 */ s16 bounceCount;
    /* 0x4A */ s16 timer;
} ThrownPickupModelActor;

typedef struct RacePickupActor {
    char pad0[0x10];
    /* 0x10 */ u16 spawnIndex;
    char pad12[6];
    /* 0x18 */ s16 timer;
    /* 0x1A */ s16 matrixDirty;
    /* 0x1C */ Vec3i pos;
    /* 0x28 */ Vec3i drawPos;
    /* 0x34 */ Transform3D transform;
    /* 0x54 */ void *displayList;
    /* 0x58 */ void *rotationDisplayList;
    /* 0x5C */ void *scaleDisplayList;
    /* 0x60 */ s32 velY;
    /* 0x64 */ void *image0;
    /* 0x68 */ void *palette0;
    char pad6C[8];
    /* 0x74 */ void *image1;
    /* 0x78 */ void *palette1;
    /* 0x7C */ void *image2;
    /* 0x80 */ void *palette2;
    /* 0x84 */ s16 rotation;
    /* 0x86 */ s16 variant;
} RacePickupActor;

typedef struct PickupShardParticleActor {
    char pad0[0x10];
    /* 0x10 */ u16 spawnOffsetIndex;
    char pad12[6];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Vec3i velocity;
    /* 0x30 */ s16 rotX;
    /* 0x32 */ s16 rotY;
    /* 0x34 */ s16 rotZ;
    /* 0x36 */ s16 rotVelX;
    /* 0x38 */ s16 rotVelY;
    /* 0x3A */ s16 rotVelZ;
    /* 0x3C */ s16 timer;
    char pad3E[2];
    /* 0x40 */ void *displayList;
    /* 0x44 */ s8 transformDirty;
    char pad45[3];
    /* 0x48 */ void *image;
    /* 0x4C */ void *palette;
} PickupShardParticleActor;

void renderRaceCoursePropModels(CourseEffectModelListActor *arg0);
void updateRaceCoursePropModels(CourseEffectModelListActor *arg0);
void initRaceCoursePropModels(CourseEffectModelListActor *arg0);
void renderCourseCollectibleSprites(CourseEffectModelListActor *arg0);
void updateCourseCollectibleSprites(CourseEffectModelListActor *arg0);
void initCourseCollectibleSpriteMatrices(CourseEffectModelListActor *arg0);
void initCourseCollectibleSprites(CourseEffectModelListActor *arg0);
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
