#ifndef RACE_COURSE_PROPS_AND_PICKUPS_H
#define RACE_COURSE_PROPS_AND_PICKUPS_H

#include "common.h"
#include "game/engine/callback_task_scheduler.h"

typedef struct FallingRockSpawnerActor {
    /* 0x00 */ CallbackTaskHeader task; /* userId selects the spawn list. */
    /* 0x18 */ s16 timer;
    /* 0x1A */ s8 lastVariant;
} FallingRockSpawnerActor;

typedef struct CourseEffectModelListActor {
    char pad0[0x10];
    /* 0x10 */ u16 modelListIndex;
    char pad12[6];
    /* 0x18 */ Mtx *modelBuffer;
    /* 0x1C */ s16 modelIndexOffset;
    /* 0x1E */ s16 modelCount;
} CourseEffectModelListActor;

/* One scheduler task shared by the renderer and simulation; userId is unused. */
typedef struct FallingRockActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Mtx *matrix;
    /* 0x1C */ Vec3i pos;
    /* 0x28 */ s16 pitch;
    /* 0x2A */ s16 yaw;
    /* 0x2C */ s16 surfaceIndex;
    /* 0x2E */ s8 matrixDirty;
    /* 0x2F */ char pad2F;
    /* 0x30 */ Vec3i transformedPos;
    /* 0x3C */ Vec3i velocity;
    /* 0x48 */ s16 bounceCount;
    /* 0x4A */ s16 timer;
} FallingRockActor;

typedef struct RacePickupActor {
    char pad0[0x10];
    /* 0x10 */ u16 spawnIndex;
    char pad12[6];
    /* 0x18 */ s16 timer;
    /* 0x1A */ s16 matrixDirty;
    /* 0x1C */ Vec3i pos;
    /* 0x28 */ Vec3i drawPos;
    /* 0x34 */ Transform3D transform;
    /* 0x54 */ Mtx *baseMatrix; /* Camera-facing base, multiplied by gViewportMatrix. */
    /* 0x58 */ Mtx *shellMatrix;
    /* 0x5C */ Mtx *symbolMatrix; /* Floating item/action symbol. */
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

/* Eight shards per collection. task.userId selects geometry/initial velocity
 * (0..7), not a unique identity: scheduler slots can be reused between bursts. */
typedef struct PickupShardParticleActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Vec3i velocity;
    /* 0x30 */ s16 rotX;
    /* 0x32 */ s16 rotY;
    /* 0x34 */ s16 rotZ;
    /* 0x36 */ s16 rotVelX;
    /* 0x38 */ s16 rotVelY;
    /* 0x3A */ s16 rotVelZ;
    /* 0x3C */ s16 timer;
    /* 0x3E */ char pad3E[2]; /* Unused by the original game; downstream recomp
                               uses these bytes for a per-spawn interpolation serial. */
    /* 0x40 */ Mtx *matrix;
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
void renderFallingRock(FallingRockActor *arg0);
void updateFallingRock(FallingRockActor *arg0);
void initFallingRock(FallingRockActor *arg0);
void spawnFallingRock(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4);
void updateFallingRockSpawner(FallingRockSpawnerActor *arg0);
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
