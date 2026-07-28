#include "game/race/race_state.h"
#include "common.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/engine/render_callback.h"
#include "game/engine/system_runtime.h"
#include "game/race/course/race_course_props_and_pickups.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/race/player/race_player_input.h"
#include "game/race/player/race_player_movement.h"
#include "game/math/spatial_math.h"
#include "game/math/fixed_point_math.h"
#include "game/race/motion/race_motion.h"

#define RACE_MODEL_BUFFER_HANDLE gAssetHandles[0x24]
#define RACE_PICKUP_G_TRI2 0xB1
#define racePickupTriangleWord(v0, v1, v2, flag) \
    (_SHIFTL((flag), 24, 8) | _SHIFTL((v0) * 2, 16, 8) | _SHIFTL((v1) * 2, 8, 8) | _SHIFTL((v2) * 2, 0, 8))
#define gRacePickupQuadrangle(pkt, v0, v1, v2, v3, flag) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(RACE_PICKUP_G_TRI2, 24, 8) | racePickupTriangleWord(v0, v1, v2, flag)); \
    _g->words.w1 = racePickupTriangleWord(v0, v2, v3, flag); \
}

typedef struct {
    /* 0x0 */ s16 enabled;
    /* 0x2 */ s16 modelIndex;
    /* 0x4 */ Vec3i transform;
} CourseCollectibleSpriteEntry;

typedef union {
    /* 0x00 */ s32 words[8];
    /* 0x00 */ s16 halfwords[0x10];
} GfxCommandSource;

typedef struct {
    /* 0x00 */ FixedTransform source;
    /* 0x20 */ s32 pad20;
} RacePickupMatrixScratch;

typedef struct {
    /* 0x00 */ s16 modelIndex;
    char pad2[2];
    /* 0x04 */ Vec3i pos;
    /* 0x10 */ s16 assetIndex;
    char pad12[2];
} RaceCoursePropModelEntry;

typedef struct {
    /* 0x00 */ Vec3i pos;
    /* 0x0C */ s16 rotation;
    /* 0x0E */ s16 variant;
} PickupSpawnEntry;

struct ThrownPickupSpawnerActor {
    char pad0[0x10];
    /* 0x10 */ u16 spawnIndex;
    char pad12[6];
    /* 0x18 */ s16 timer;
    /* 0x1A */ s8 lastVariant;
};

struct CourseEffectModelListActor {
    char pad0[0x10];
    /* 0x10 */ u16 modelListIndex;
    char pad12[6];
    /* 0x18 */ Mtx *modelBuffer;
    /* 0x1C */ s16 modelIndexOffset;
    /* 0x1E */ s16 modelCount;
};

struct ThrownPickupRenderActor {
    char pad0[0x18];
    /* 0x18 */ Mtx *matrix;
    /* 0x1C */ Vec3i pos;
    /* 0x28 */ s16 pitch;
    /* 0x2A */ s16 yaw;
    char pad2C[2];
    /* 0x2E */ s8 matrixDirty;
};

struct ThrownPickupModelActor {
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
};

struct RacePickupActor {
    char pad0[0x10];
    /* 0x10 */ u16 spawnIndex;
    char pad12[6];
    /* 0x18 */ s16 timer;
    /* 0x1A */ s16 matrixDirty;
    /* 0x1C */ Vec3i pos;
    /* 0x28 */ Vec3i drawPos;
    /* 0x34 */ FixedMatrix3s rotationMatrix;
    char pad46[2];
    /* 0x48 */ Vec3i spawnPos;
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
};

struct PickupShardParticleActor {
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
};

extern void packFixedTransformMatrix(FixedTransform *, void *);
extern Mtx *allocFixedTransformMatrix(void *);
extern void setPackedMatrixTranslation(Mtx *, Vec3i *);
extern s32 isPositionNearAnyRaceViewportFocus(void *);
extern void osWritebackDCache(void *, s32);
extern void enqueuePositionalSoundEffect(s32, void *, s32, s32);
extern u8 gItemEffectRollTable[][0x10];
extern u8 gActionEffectRollTable[][0x10];
extern PickupSpawnEntry gRacePickupSpawnEntries[];
extern PickupSpawnEntry *gThrownPickupSpawnLists[];
extern Vec3i gPickupShardInitialVelocities[];
extern RaceCoursePropModelEntry *gRaceCoursePropModelLists[];
extern void *gRaceCourseSceneryDisplayLists[];
extern Gfx *gThrownPickupModelDisplayList;
extern u8 gRaceUpdatePaused;
extern u8 gTrainingCourseLesson;
extern CourseCollectibleSpriteEntry *gCourseCollectibleSpriteListsByCourse[];
extern u32 gCourseCollectibleSpriteVertices[];
extern Vtx gRacePickupBaseVertices[];
extern Vtx gRacePickupTopVertices[];
extern Gfx gEffectRenderModeSetupDl[];
extern Gfx gEffectRenderModeCleanupDl[];
extern Gfx gRaceItemPickupDisplayList[];
extern Gfx gRaceActionPickupDisplayList[];
extern Gfx *gRegionAllocPtr;
extern s16 gFrameCounter;
typedef struct Scratch674B4 {
    FixedTransform transform;
    s32 pad[3];
} Scratch674B4;

void renderRaceCoursePropModels(CourseEffectModelListActor *arg0) {
    RaceCoursePropModelEntry *var_s4;
    s32 var_s5;
    s32 var_s7;
    Gfx *temp_s0;
    Gfx *temp_s2;
    Gfx *temp_s3;

    var_s4 = gRaceCoursePropModelLists[arg0->modelListIndex];
    var_s7 = TRUE;
    var_s5 = 0;
    if (var_s4->modelIndex != -1) {
        do {
            if (isPositionNearCurrentRaceViewportCamera(&var_s4->pos) != 0) {
                if (var_s7 != 0) {
                    gDPPipeSync(gRegionAllocPtr++);
                    temp_s2 = gRegionAllocPtr++;
                    var_s7 = FALSE;
                    gSPSegment(temp_s2, 0x02, getRelocatableHeapBlockBase(gAssetHandles[0xA]));

                    temp_s3 = gRegionAllocPtr++;
                    gSPSegment(temp_s3, 0x03, getRelocatableHeapBlockBase(gAssetHandles[0xB]));
                }

                temp_s0 = gRegionAllocPtr++;
                gDma1p(temp_s0, 1, (u32)arg0->modelBuffer + (var_s5 << 6), 0x40, 2);

                temp_s0 = gRegionAllocPtr++;
                gSPDisplayList(temp_s0, gRaceCourseSceneryDisplayLists[var_s4->modelIndex]);
            }
            var_s4++;
            var_s5++;
        } while (var_s4->modelIndex != -1);
    }
}

void updateRaceCoursePropModels(CourseEffectModelListActor *arg0) {
    RaceCoursePropModelEntry *entry;
    void *pos;

    entry = gRaceCoursePropModelLists[arg0->modelListIndex];
    if (entry->modelIndex != -1) {
        pos = &entry->pos;
        do {
            if (isPositionNearAnyRaceViewportFocus(pos) != 0) {
                pushRacePlayersOutOfCylinderAndApplyItemHit(pos, 0x1C0000, 0x480000, 2);
            }
            entry++;
            pos = &entry->pos;
        } while (entry->modelIndex != -1);
    }
    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)renderRaceCoursePropModels, arg0);
}

void initRaceCoursePropModels(CourseEffectModelListActor *arg0) {
    s32 size;
    RaceCoursePropModelEntry *base;
    RaceCoursePropModelEntry *entry;
    s32 i;
    FixedTransform transform;
    s32 count;

    base = gRaceCoursePropModelLists[arg0->modelListIndex];
    count = 0;
    entry = base;
    if (base->modelIndex != -1) {
        do {
            count++;
            entry++;
        } while (entry->modelIndex != -1);
    }

    if (count != 0) {
        entry = base;
        size = count << 6;
        gAssetHandles[0x23] = allocRelocatableHeapBlock(size);
        arg0->modelBuffer = (void *)getRelocatableHeapBlockBase(gAssetHandles[0x23]);

        for (i = 0; i < count; i++) {
            makeFixedRotationY(transform.rotation, entry->assetIndex);
            transform.translation.x = entry->pos.x;
            transform.translation.y = entry->pos.y;
            transform.translation.z = entry->pos.z;
            packFixedTransformMatrix(&transform, (void *)((u32)arg0->modelBuffer + (i << 6)));
            entry++;
        }

        osWritebackDCache(arg0->modelBuffer, size);
    }
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceCoursePropModels);
}

void renderCourseCollectibleSprites(CourseEffectModelListActor *arg0) {
    CourseCollectibleSpriteEntry *entry;
    s16 modelIndex;
    s32 i;
    CourseEffectModelListActor *actor;
    u8 padding[0x1];
    void *spA0;
    void *sp9C;

    modelIndex = -1;
    actor = arg0;
    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);

    entry = gCourseCollectibleSpriteListsByCourse[gRaceCourseIndex.signedValue];
    i = 0;
    if (entry->modelIndex != -1) {
        do {
            if ((entry->enabled != 0) && (isPositionNearCurrentRaceViewportCamera(&entry->transform) != 0)) {
                if (modelIndex != entry->modelIndex + actor->modelIndexOffset) {
                    modelIndex = entry->modelIndex + actor->modelIndexOffset;
                    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]),
                                                 modelIndex + 4, &spA0, &sp9C);

                    gDPLoadTextureBlock_4b(gRegionAllocPtr++, spA0, G_IM_FMT_CI, 16, 16, 0, G_TX_CLAMP,
                                           G_TX_CLAMP, 0, 0, 0, 0);
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, sp9C);
                }

                gSPMatrix(gRegionAllocPtr++, &actor->modelBuffer[i], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

                gSPVertex(gRegionAllocPtr++, (u32)gCourseCollectibleSpriteVertices, 4, 0);

                gSP2Triangles(gRegionAllocPtr++, 3, 2, 1, 0, 3, 1, 0, 0);
            }
            entry++;
            i++;
        } while (entry->modelIndex != -1);
    }

    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
}

void updateCourseCollectibleSprites(CourseEffectModelListActor *arg0) {
    CourseCollectibleSpriteEntry *entry;
    Vec3i *pos;
    CourseEffectModelListActor *actor;
    register s32 one;
    register s32 xzSize;
    register s32 ySize;
    register s32 sentinel;

    entry = gCourseCollectibleSpriteListsByCourse[gRaceCourseIndex.signedValue];
    actor = arg0;
    ySize = 1;
    if (gFrameCounter & ySize) {
        actor->modelIndexOffset++;
    }
    one = 1;
    if (actor->modelIndexOffset >= 6) {
        actor->modelIndexOffset = 0;
    }

    sentinel = -ySize;
    ySize = 0xC0000;
    if (entry->modelIndex == sentinel) {
        goto done;
    }
    xzSize = 0x68000;
    pos = &entry->transform;

loop:
    if (isPositionNearAnyRaceViewportFocus(pos) != 0) {
        if (entry->enabled != 0) {
            if (isRacePlayerInsideCylinder(pos, xzSize, ySize, 0) != 0) {
                addRacePlayerScore(&gRacePlayers[0], 0x64);
                entry->enabled = 0;
                enqueuePositionalSoundEffect(0x1A, pos, 0x7F, 0x32);
                goto next;
            }
            if (isRacePlayerInsideCylinder(pos, xzSize, ySize, one) != 0) {
                addRacePlayerScore(&gRacePlayers[1], 0x64);
                entry->enabled = 0;
                enqueuePositionalSoundEffect(0x1A, pos, 0x7F, 0x32);
                goto next;
            }
            if (isRacePlayerInsideCylinder(pos, xzSize, ySize, 2) != 0) {
                addRacePlayerScore(&gRacePlayers[2], 0x64);
                entry->enabled = 0;
                enqueuePositionalSoundEffect(0x1A, pos, 0x7F, 0x32);
                goto next;
            }
            if (isRacePlayerInsideCylinder(pos, xzSize, ySize, 3) != 0) {
                addRacePlayerScore(&gRacePlayers[3], 0x64);
                entry->enabled = 0;
                enqueuePositionalSoundEffect(0x1A, pos, 0x7F, 0x32);
                goto next;
            }
        }
    } else {
        entry->enabled = one;
    }

next:
    entry++;
    pos = &entry->transform;
    if (entry->modelIndex != sentinel) {
        goto loop;
    }

done:
    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)renderCourseCollectibleSprites, actor);
}

void initCourseCollectibleSpriteMatrices(CourseEffectModelListActor *arg0) {
    register CourseEffectModelListActor *actor1;
    register CourseEffectModelListActor *actor2;
    register CourseCollectibleSpriteEntry *script;
    register s32 i;
    register s32 offset;
    register s32 one;

    script = gCourseCollectibleSpriteListsByCourse[gRaceCourseIndex.signedValue];
    actor1 = arg0;
    actor2 = arg0;
    i = 0;
    if (actor1->modelCount > 0) {
        register Mtx *template;

        template = &gIdentityMatrix;
        offset = 0;
        one = 1;
        do {
            script->enabled = one;
            actor1->modelBuffer[i] = *template;
            setPackedMatrixTranslation(&actor1->modelBuffer[i], &script->transform);
            i++;
            offset += sizeof(Mtx);
            script++;
        } while (i < actor2->modelCount);
    }
    osWritebackDCache(actor1->modelBuffer, actor1->modelCount * sizeof(Mtx));
}

void initCourseCollectibleSprites(CourseEffectModelListActor *arg0) {
    CourseEffectModelListActor *new_var;
    CourseCollectibleSpriteEntry *var_v0;

    new_var = arg0;
    new_var->modelCount = 0;
    var_v0 = gCourseCollectibleSpriteListsByCourse[gRaceCourseIndex.signedValue];
    if (var_v0->modelIndex != -1) {
        do {
            new_var->modelCount += 1;
            var_v0++;
        } while (var_v0->modelIndex != -1);
    }
    if (new_var->modelCount != 0) {
        RACE_MODEL_BUFFER_HANDLE = allocRelocatableHeapBlock(new_var->modelCount << 6);
        new_var->modelBuffer = (void *) getRelocatableHeapBlockBase(RACE_MODEL_BUFFER_HANDLE);
        initCourseCollectibleSpriteMatrices(new_var);
        setCallbackTaskCallback(new_var, (CallbackTaskCallback)updateCourseCollectibleSprites);
    }
}

void renderThrownPickupModel(ThrownPickupRenderActor *arg0) {
    FixedMatrix3sWideScratch scratch;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            makeFixedRotationXY(scratch, arg0->pitch, arg0->yaw);
            ((GfxCommandSource *)scratch)->words[5] = arg0->pos.x;
            ((GfxCommandSource *)scratch)->words[6] = arg0->pos.y + 0x190000;
            ((GfxCommandSource *)scratch)->words[7] = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix((GfxCommandSource *)scratch);
            arg0->matrixDirty = 0;
        }

        if (arg0->matrix != NULL) {
            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[0xA]));
            gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[0xB]));
            gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gRegionAllocPtr++, gThrownPickupModelDisplayList);
        }
    }
}

void updateThrownPickupModel(ThrownPickupModelActor *arg0) {
    Vec3i *pos;
    s32 groundY;
    s32 velocityY;
    s16 timer;

    if (gRaceUpdatePaused == 0) {
        arg0->pos.x += arg0->transformedPos.x;
        velocityY = arg0->transformedPos.y;
        timer = arg0->timer;
        arg0->pos.y += velocityY;
        arg0->pos.z += arg0->transformedPos.z;
        arg0->transformedPos.y = velocityY - 0x5000;
        arg0->pitch -= 0x20;

        if (timer == 0) {
            arg0->unk2C = findRaceCourseSurfaceFromHint(arg0->unk2C, arg0->pos.x, arg0->pos.z);
            groundY = getRaceCourseSurfaceHeight(arg0->unk2C, arg0->pos.x, arg0->pos.z);
            pos = &arg0->pos;
            if (arg0->pos.y < groundY) {
                enqueuePositionalSoundEffect(0x20, pos, 0x7F, 0x32);
                arg0->pos.y = groundY;
                arg0->transformedPos.y = arg0->velocity.y / 2;
                arg0->bounceCount++;
            }

            if (arg0->transformedPos.y <= 0) {
                applyItemHitToRacePlayersInsideSphere(pos, 0x170000, 0x20);
            }
        } else {
            arg0->timer = timer - 1;
        }
    }

    if (arg0->bounceCount == 2) {
        removeCallbackTask(arg0);
        return;
    }

    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)renderThrownPickupModel, arg0);
}

void initThrownPickupModel(ThrownPickupModelActor *arg0) {
    Scratch674B4 sp1C;
    ThrownPickupModelActor *temp_a3 = arg0;

    if (gRaceUpdatePaused == 0) {
        makeFixedRotationY(sp1C.transform.rotation, temp_a3->modelIndex);
        temp_a3->timer = 0x32;
        temp_a3->velocity.x = 0;
        temp_a3->velocity.y = 0xB0000;
        temp_a3->velocity.z = 0xFFF90000;
        transformVec3iByFixedMatrix(sp1C.transform.rotation, &temp_a3->velocity, &temp_a3->transformedPos);
        setCallbackTaskCallback(temp_a3, (CallbackTaskCallback)updateThrownPickupModel);
    }
}

void spawnThrownPickupModel(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4) {
    ThrownPickupModelActor *temp = createCallbackTask((CallbackTaskCallback)initThrownPickupModel, 0, 0x64);

    if (temp != NULL) {
        temp->pos.x = arg0;
        temp->pos.y = arg1;
        temp->pos.z = arg2;
        temp->modelIndex = arg3;
        temp->unk2C = arg4;
    }
}

#define SPAWN_RANGE_MAX 0x14000000
#define SPAWN_RANGE_MIN -0x13FFFFFF

void updateThrownPickupSpawner(ThrownPickupSpawnerActor *arg0) {
    ThrownPickupModelActor *savedSpawned;
    volatile s32 forceStack[6];
    ThrownPickupModelActor *spawned;
    PickupSpawnEntry *entry;
    s32 found;
    s32 diffX;
    s32 diffZ;
    s32 rand;
    s32 i;

    if (gRaceUpdatePaused == 0) {
        if (arg0->timer == 0) {
            arg0->timer = 0x20;
            entry = gThrownPickupSpawnLists[arg0->spawnIndex];
            found = FALSE;
            if (gRaceSplitscreenMode != 2) {
                for (i = 0; i < RACE_PLAYER_COUNT; i++) {
                    if (gRacePlayers[i].isActive != 0) {
                        diffX = gRacePlayers[i].posX - entry->pos.x;
                        diffZ = gRacePlayers[i].posZ - entry->pos.z;
                        if ((((diffX < SPAWN_RANGE_MAX) && (diffX >= SPAWN_RANGE_MIN)) &&
                             (diffZ < SPAWN_RANGE_MAX)) && (diffZ >= SPAWN_RANGE_MIN)) {
                            found = TRUE;
                        }
                    }
                }
            } else {
                found = TRUE;
            }
            if (found != 0) {
                spawned = createCallbackTask((CallbackTaskCallback)initThrownPickupModel, 0, 0x64);
                if (spawned != NULL) {
                    savedSpawned = spawned;
                    rand = randomNextSecondary() & 3;
                    spawned = savedSpawned;
                    if (rand != arg0->lastVariant) {
                    } else {
                        rand = (arg0->lastVariant + 1) & 3;
                    }
                    arg0->lastVariant = rand;
                    savedSpawned->pos.x = (&entry[rand])->pos.x;
                    savedSpawned->pos.y = (&entry[rand])->pos.y;
                    savedSpawned->pos.z = (&entry[rand])->pos.z;
                    savedSpawned->modelIndex = (&entry[rand])->rotation;
                    savedSpawned->unk2C = (&entry[rand])->variant;
                }
            }
        } else {
            arg0->timer--;
        }
    }
}

#undef SPAWN_RANGE_MAX
#undef SPAWN_RANGE_MIN

void renderRacePickupIdle(RacePickupActor *arg0) {
    RacePickupMatrixScratch spF4;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            spF4.source = gIdentityFixedTransform;
            spF4.source.translation.x = arg0->drawPos.x;
            spF4.source.translation.y = arg0->drawPos.y;
            spF4.source.translation.z = arg0->drawPos.z;
            arg0->displayList = allocFixedTransformMatrix(&spF4.source);
            arg0->rotationDisplayList = allocFixedTransformMatrix(arg0->rotationMatrix);
            spF4.source = *(FixedTransform *)arg0->rotationMatrix;
            spF4.source.rotation[0] /= 2;
            spF4.source.rotation[1] /= 2;
            spF4.source.rotation[2] /= 2;
            spF4.source.rotation[3] /= 2;
            spF4.source.rotation[4] /= 2;
            spF4.source.rotation[5] /= 2;
            spF4.source.rotation[6] /= 2;
            spF4.source.rotation[7] /= 2;
            spF4.source.rotation[8] /= 2;
            spF4.source.translation.y +=
                (fixedSine((s16)((gFrameCounter << 7) & 0xFFF)) << 7) + 0x300000;
            arg0->scaleDisplayList = allocFixedTransformMatrix(&spF4.source);
        }

        if (arg0->scaleDisplayList != NULL) {
            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[0xA]));
            gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[0xB]));
            gSPMatrix(gRegionAllocPtr++, arg0->scaleDisplayList, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            if (arg0->variant == 0) {
                gSPDisplayList(gRegionAllocPtr++, gRaceItemPickupDisplayList);
            } else {
                gSPDisplayList(gRegionAllocPtr++, gRaceActionPickupDisplayList);
            }
            gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
            gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->image0, G_IM_FMT_CI, 32, 32, 0, G_TX_CLAMP,
                                    G_TX_CLAMP, 0, 0, 0, 0);
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette0);
            gSPMatrix(gRegionAllocPtr++, arg0->displayList, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gDma1p(gRegionAllocPtr++, G_VTX, gRacePickupBaseVertices, 0x207F, 0);
            gRacePickupQuadrangle(gRegionAllocPtr++, 3, 2, 1, 0, 0);
            gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->image1, G_IM_FMT_CI, 32, 32, 0, G_TX_CLAMP,
                                    G_TX_CLAMP, 0, 0, 0, 0);
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette1);
            gSPMatrix(gRegionAllocPtr++, arg0->rotationDisplayList,
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gDma1p(gRegionAllocPtr++, G_VTX, gRacePickupTopVertices, 0x513F, 0);
            gRacePickupQuadrangle(gRegionAllocPtr++, 3, 2, 1, 0, 0);
            gRacePickupQuadrangle(gRegionAllocPtr++, 7, 6, 5, 4, 0);
            gRacePickupQuadrangle(gRegionAllocPtr++, 11, 10, 9, 8, 0);
            gRacePickupQuadrangle(gRegionAllocPtr++, 15, 14, 13, 12, 0);
            gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->image2, G_IM_FMT_CI, 32, 32, 0, G_TX_CLAMP,
                                    G_TX_CLAMP, 0, 0, 0, 0);
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette2);
            gRacePickupQuadrangle(gRegionAllocPtr++, 19, 18, 17, 16, 0);
            gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
        }
    }
}

void renderRacePickupBase(RacePickupActor *arg0) {
    RacePickupMatrixScratch sp64;
    Gfx *temp_v0;
    Gfx *temp_v0_2;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_5;
    Gfx *temp_v0_6;
    Gfx *temp_v0_7;
    Gfx *temp_v0_8;
    Gfx *temp_v0_9;
    Gfx *temp_v0_10;
    Gfx *temp_v0_11;
    Gfx *temp_v0_12;
    Gfx *temp_v0_13;
    Gfx *temp_v0_14;
    Gfx *temp_v0_15;
    Gfx *temp_v0_16;
    Gfx *temp_v0_17;
    Gfx *temp_v0_18;
    Gfx *temp_v0_19;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sp64.source = gIdentityFixedTransform;
            sp64.source.translation.x = arg0->drawPos.x;
            sp64.source.translation.y = arg0->drawPos.y;
            sp64.source.translation.z = arg0->drawPos.z;
            arg0->displayList = allocFixedTransformMatrix(&sp64.source);
        }
        do { if (arg0->displayList != NULL) { temp_v0 = gRegionAllocPtr++; temp_v0->words.w0 = 0x06000000; temp_v0->words.w1 = (u32) gEffectRenderModeSetupDl; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image0; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x070FF400; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400400; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0007C07C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette0; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; temp_v0_15 = gRegionAllocPtr++; temp_v0_15->words.w0 = 0x01020040; temp_v0_15->words.w1 = (u32) arg0->displayList; temp_v0_16 = gRegionAllocPtr++; temp_v0_16->words.w0 = 0x01000040; temp_v0_16->words.w1 = (u32) gViewportMatrix; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400207F; temp_v0_17->words.w1 = (u32) gRacePickupBaseVertices; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; temp_v0_19 = gRegionAllocPtr++; temp_v0_19->words.w0 = 0x06000000; temp_v0_19->words.w1 = (u32) gEffectRenderModeCleanupDl; } } while (0);
    }
}

void renderRacePickupRespawn(RacePickupActor *arg0) {
    RacePickupMatrixScratch spF4;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }
    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            spF4.source = gIdentityFixedTransform;
            spF4.source.translation.x = arg0->drawPos.x;
            spF4.source.translation.y = arg0->drawPos.y;
            spF4.source.translation.z = arg0->drawPos.z;
            arg0->displayList = allocFixedTransformMatrix(&spF4.source);
            arg0->rotationDisplayList = allocFixedTransformMatrix((GfxCommandSource *) arg0->rotationMatrix);
        }
        if (arg0->displayList != NULL) {
            if (arg0->rotationDisplayList != NULL) {
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
                gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->image0, G_IM_FMT_CI, 32, 32, 0, G_TX_CLAMP,
                                        G_TX_CLAMP, 0, 0, 0, 0);
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette0);
                gSPMatrix(gRegionAllocPtr++, arg0->displayList, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gDma1p(gRegionAllocPtr++, G_VTX, gRacePickupBaseVertices, 0x103F, 0);
                gRacePickupQuadrangle(gRegionAllocPtr++, 3, 2, 1, 0, 0);
                gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->image1, G_IM_FMT_CI, 32, 32, 0, G_TX_CLAMP,
                                        G_TX_CLAMP, 0, 0, 0, 0);
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette1);
                gSPMatrix(gRegionAllocPtr++, arg0->rotationDisplayList,
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gDma1p(gRegionAllocPtr++, G_VTX, gRacePickupTopVertices, 0x513F, 0);
                gRacePickupQuadrangle(gRegionAllocPtr++, 3, 2, 1, 0, 0);
                gRacePickupQuadrangle(gRegionAllocPtr++, 7, 6, 5, 4, 0);
                gRacePickupQuadrangle(gRegionAllocPtr++, 11, 10, 9, 8, 0);
                gRacePickupQuadrangle(gRegionAllocPtr++, 15, 14, 13, 12, 0);
                gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->image2, G_IM_FMT_CI, 32, 32, 0, G_TX_CLAMP,
                                        G_TX_CLAMP, 0, 0, 0, 0);
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette2);
                gRacePickupQuadrangle(gRegionAllocPtr++, 19, 18, 17, 16, 0);
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
            }
        }
    }
}

void updateRacePickupRespawn(RacePickupActor *arg0) {
    s16 temp_v0;
    s32 temp_v1;
    RacePickupActor *temp_s0 = arg0;
    void *temp_s1;

    if (gRaceUpdatePaused == 0) {
        arg0->timer--;
        temp_v0 = arg0->timer;
        temp_v1 = arg0->pos.y - (temp_v0 * 0x14000);
        arg0->drawPos.y = temp_v1 + 0x140000;
        arg0->spawnPos.y = temp_v1;
        if (temp_v0 == 0) {
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRacePickupIdle);
        }
        temp_s1 = &temp_s0->pos;
        pushRacePlayerOutOfCylinder(temp_s1, 0xC0000, 0x180000, 0);
        pushRacePlayerOutOfCylinder(temp_s1, 0xC0000, 0x180000, 1);
        pushRacePlayerOutOfCylinder(temp_s1, 0xC0000, 0x180000, 2);
        pushRacePlayerOutOfCylinder(temp_s1, 0xC0000, 0x180000, 3);
    }
    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)renderRacePickupRespawn, temp_s0);
}

void updateRacePickupBounce(RacePickupActor *arg0) {
    if (gRaceUpdatePaused == 0) {
        arg0->drawPos.y += arg0->velY;
        arg0->velY += 0xFFFF0000;

        if (arg0->drawPos.y < arg0->pos.y) {
            arg0->drawPos.y = arg0->pos.y;
            arg0->timer = 0x10;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRacePickupRespawn);
        }

        pushRacePlayerOutOfCylinder(&arg0->pos, 0xC0000, 0x180000, 0);
        pushRacePlayerOutOfCylinder(&arg0->pos, 0xC0000, 0x180000, 1);
        pushRacePlayerOutOfCylinder(&arg0->pos, 0xC0000, 0x180000, 2);
        pushRacePlayerOutOfCylinder(&arg0->pos, 0xC0000, 0x180000, 3);
    }
    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)renderRacePickupBase, arg0);
}

void updateRacePickupCollected(RacePickupActor *arg0) {
    s32 temp_a2;
    s32 temp_v0;
    s32 var_v1;
    void *temp_s1;

    if (gRaceUpdatePaused == 0) {
        temp_v0 = arg0->velY;
        temp_a2 = arg0->pos.y;
        var_v1 = (arg0->drawPos.y += temp_v0);
        arg0->velY = temp_v0 + 0xFFFF0000;
        if (var_v1 < temp_a2) {
            arg0->drawPos.y = temp_a2;
            arg0->velY = 0x30000;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRacePickupBounce);
            var_v1 = arg0->drawPos.y;
        }
        temp_s1 = &arg0->drawPos;
        if (var_v1 < 0) {
            pushRacePlayerOutOfCylinder(temp_s1, 0xC0000, 0x180000, 0);
            pushRacePlayerOutOfCylinder(temp_s1, 0xC0000, 0x180000, 1);
            pushRacePlayerOutOfCylinder(temp_s1, 0xC0000, 0x180000, 2);
            pushRacePlayerOutOfCylinder(temp_s1, 0xC0000, 0x180000, 3);
        }
    }
    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)renderRacePickupBase, arg0);
}

void updateRacePickupIdle(RacePickupActor *arg0) {
    RacePlayer *player;
    s32 i;
    s32 playerInsideCylinder;

    if (gRaceUpdatePaused == 0) {
        playerInsideCylinder = 0;

        for (i = 0; i < 4; i++) {
            if (!isRacePlayerInsideCylinder(&arg0->pos, 0xBF000, 0x170000, i)) {
                continue;
            }

            player = &gRacePlayers[i];
            playerInsideCylinder = 1;

            if ((player->unk568 < 0x64) && (player->isCpu == 0)) {
                continue;
            }

            arg0->velY = 0x60000;
            if (player->unk568 >= 0x64) {
                player->unk568 -= 0x64;
            } else {
                player->unk568 = 0;
            }

            // arg0->variant distinguishes the two item box colors: one branch resolves a held
            // item (itemEffectType), the other an immediate action/ability (actionEffectType).
            // Both roll tables are indexed by [racePosition][rollIndex], where rollIndex comes
            // from randomNextObject walking a per-player counter through the shared byte table.
            // Each player advances their own counter by 1 every time they draw an item/action,
            // regardless of box color. Lower racePosition values are closer to 1st place and
            // skew toward common/weaker tier values; higher values skew rarer/stronger.
            if (arg0->variant == 0) {
                player->itemEffectType = gItemEffectRollTable[player->racePosition][randomNextObject((RandomStateObject *)player) & 0xF];
                if (gTrainingCourseLesson != 0) {
                    player->itemEffectType = 1;
                }
                player->itemEffectCount = 3;
                player->itemEffectPalette = 4;
            } else {
                player->actionEffectType = gActionEffectRollTable[player->racePosition][randomNextObject((RandomStateObject *)player) & 0xF];
                if (gTrainingCourseLesson != 0) {
                    player->actionEffectType = 1;
                }
                // Course index 8 special case, CPU opponents only. When a CPU's action roll
                // lands on the common tier-4 result, there is a 255/256 chance to upgrade it
                // to the rarer tier 6.
                if ((gRaceCourseIndex.signedValue == 8) && (player->isCpu != 0) && (player->actionEffectType == 4)) {
                    if (randomNextMain() != 0) {
                        player->actionEffectType = 6;
                    }
                }
                player->actionEffectPalette = 4;
            }

            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRacePickupCollected);
            spawnPickupShardParticle(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->rotation, 0);
            spawnPickupShardParticle(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->rotation, 1);
            spawnPickupShardParticle(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->rotation, 2);
            spawnPickupShardParticle(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->rotation, 3);
            spawnPickupShardParticle(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->rotation, 4);
            spawnPickupShardParticle(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->rotation, 5);
            spawnPickupShardParticle(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->rotation, 6);
            spawnPickupShardParticle(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->rotation, 7);
            enqueuePositionalSoundEffect(0x1F, &arg0->pos, 0x7F, 0x32);

            goto done;
        }

        if (playerInsideCylinder) {
            pushRacePlayersOutOfCylinderAndApplyItemHit(&arg0->pos, 0xBF000, 0x170000, 0x800);
        }
    }

done:
    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)renderRacePickupIdle, arg0);
}

void initRacePickup(RacePickupActor *arg0) {
    PickupSpawnEntry *entry = &gRacePickupSpawnEntries[arg0->spawnIndex];

    arg0->pos.x = entry->pos.x;
    arg0->pos.y = entry->pos.y;
    arg0->pos.z = entry->pos.z;
    arg0->rotation = entry->rotation;
    arg0->variant = entry->variant;
    if (gTrainingCourseLesson == 7) {
        arg0->variant = 1;
    }
    if (gTrainingCourseLesson == 8) {
        arg0->variant = 0;
    }

    arg0->drawPos = arg0->pos;
    arg0->drawPos.y += 0x140000;
    makeFixedRotationY(arg0->rotationMatrix, arg0->rotation);

    arg0->spawnPos.x = arg0->pos.x;
    arg0->spawnPos.y = arg0->pos.y;
    arg0->spawnPos.z = arg0->pos.z;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x1E, &arg0->image0, &arg0->palette0);
    if (arg0->variant == 0) {
        getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x20, &arg0->image1, &arg0->palette1);
    } else {
        getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x21, &arg0->image1, &arg0->palette1);
    }
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x22, &arg0->image2, &arg0->palette2);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRacePickupIdle);
}

void renderPickupShardParticle(PickupShardParticleActor *arg0) {
    volatile s32 pad;
    FixedTransform transform;
    Gfx *temp_v0;
    Gfx *temp_v0_10;
    Gfx *temp_v0_11;
    Gfx *temp_v0_12;
    Gfx *temp_v0_13;
    Gfx *temp_v0_14;
    Gfx *temp_v0_15;
    Gfx *temp_v0_16;
    Gfx *temp_v0_17;
    Gfx *temp_v0_18;
    Gfx *temp_v0_19;
    Gfx *temp_v0_2;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_5;
    Gfx *temp_v0_6;
    Gfx *temp_v0_7;
    Gfx *temp_v0_8;
    Gfx *temp_v0_9;
    Gfx *var_v0;

    if (gRenderMatricesDirty != 0) {
        arg0->transformDirty = 1;
    }
    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->transformDirty != 0) {
            arg0->transformDirty = 0;
            makeFixedRotationXYZ(transform.rotation, arg0->rotX, arg0->rotY, arg0->rotZ);
            transform.translation.x = arg0->pos.x;
            transform.translation.y = arg0->pos.y;
            transform.translation.z = arg0->pos.z;
            arg0->displayList = allocFixedTransformMatrix(&transform);
        }
        if (arg0->displayList != NULL) {
            temp_v0 = gRegionAllocPtr++;
            temp_v0->words.w1 = (u32)gEffectRenderModeSetupDl;
            temp_v0->words.w0 = 0x06000000;
            temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32)arg0->palette; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x070FF400; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400400; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0007C07C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32)arg0->image; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; temp_v0_15 = gRegionAllocPtr++; temp_v0_15->words.w0 = 0x01020040; temp_v0_15->words.w1 = (u32)arg0->displayList; temp_v0_16 = gRegionAllocPtr++; temp_v0_16->words.w0 = 0x0400103F; temp_v0_16->words.w1 = (u32)&gRacePickupBaseVertices[((((u16)arg0->spawnOffsetIndex) >> 1) * 4) + 8]; if (arg0->spawnOffsetIndex & 1) { temp_v0_17 = gRegionAllocPtr++;
                temp_v0_17->words.w1 = 0x604;
                temp_v0_17->words.w0 = 0xBF000000;
                var_v0 = gRegionAllocPtr++;
                var_v0->words.w1 = 0x406;
                var_v0->words.w0 = 0xBF000000;
            } else {
                temp_v0_18 = gRegionAllocPtr++;
                temp_v0_18->words.w1 = 0x402;
                temp_v0_18->words.w0 = 0xBF000000;
                var_v0 = gRegionAllocPtr++;
                var_v0->words.w1 = 0x204;
                var_v0->words.w0 = 0xBF000000;
            }
            temp_v0_19 = gRegionAllocPtr++;
            temp_v0_19->words.w1 = (u32)gEffectRenderModeCleanupDl;
            temp_v0_19->words.w0 = 0x06000000;
        }
    }
}

void updatePickupShardParticle(PickupShardParticleActor *arg0) {
    s32 temp_v0;
    PickupShardParticleActor *temp_a2 = arg0;

    if (gRaceUpdatePaused == 0) {
        arg0->timer--;
    }
    if (temp_a2->timer != 0) {
        if (gRaceUpdatePaused == 0) {
            temp_v0 = temp_a2->velocity.y;
            temp_a2->pos.x += temp_a2->velocity.x;
            temp_a2->pos.y += temp_v0;
            temp_a2->pos.z += temp_a2->velocity.z;
            temp_a2->velocity.y = temp_v0 + 0xFFFF0000;
            temp_a2->rotX += temp_a2->rotVelX;
            temp_a2->rotY += temp_a2->rotVelY;
            temp_a2->rotZ += temp_a2->rotVelZ;
        }
        addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)renderPickupShardParticle, temp_a2);
        return;
    }
    removeCallbackTask(temp_a2);
}

void initPickupShardParticle(PickupShardParticleActor *arg0) {
    FixedTransform transform;

    arg0->timer = 0xA;
    arg0->rotVelX = randomNextMain() - 0x80;
    arg0->rotVelY = randomNextMain() - 0x80;
    arg0->rotVelZ = randomNextMain() - 0x80;
    makeFixedRotationY(transform.rotation, arg0->rotY);
    transformVec3iByFixedMatrix(transform.rotation, &gPickupShardInitialVelocities[arg0->spawnOffsetIndex],
                               &arg0->velocity);
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x22, &arg0->palette, &arg0->image);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updatePickupShardParticle);
}

void spawnPickupShardParticle(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4) {
    PickupShardParticleActor *temp = createCallbackTaskPreservingArgs((CallbackTaskCallback)initPickupShardParticle, 5, 0x3B);

    if (temp != NULL) {
        temp->spawnOffsetIndex = arg4;
        temp->pos.x = arg0;
        temp->pos.y = arg1;
        temp->pos.z = arg2;
        temp->rotY = arg3;
    }
}
