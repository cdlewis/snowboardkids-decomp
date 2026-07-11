#include "common.h"
#include "race_overlay_effects.h"
#include "memory_block_allocator.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "race_input_history.h"
#include "race_player_movement.h"
#include "spatial_math.h"
#include "fixed_point_math.h"
#include "model_animation.h"

#define RACE_MODEL_BUFFER_HANDLE (*(s16 *)&D_80112130[0x48])

typedef struct {
    /* 0x0 */ s16 enabled;
    /* 0x2 */ s16 modelIndex;
    /* 0x4 */ Vec3i transform;
} RaceModelEntry;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
} GfxCommandDest;

typedef union {
    /* 0x00 */ s32 words[8];
    /* 0x00 */ s16 halfwords[0x10];
} GfxCommandSource;

typedef s16 FixedMatrix3sScratch[0x12];

typedef struct {
    s16 rotation[9];
    s16 pad2A;
    Vec3i basePos;
} CourseEffectMatrixSource;

typedef struct {
    /* 0x00 */ FixedTransform source;
    /* 0x20 */ s32 pad20;
} RaceOverlayMatrixScratch;

typedef struct {
    /* 0x00 */ s16 modelIndex;
    char pad2[2];
    /* 0x04 */ Vec3i pos;
    /* 0x10 */ s16 assetIndex;
    char pad12[2];
} RaceOverlayModelEntry;

typedef struct {
    /* 0x00 */ Vec3i pos;
    /* 0x0C */ s16 rotation;
    /* 0x0E */ s16 variant;
} RaceOverlayEffectSpawn;

struct RaceOverlaySpawnActor {
    char pad0[0x10];
    /* 0x10 */ u16 spawnIndex;
    char pad12[6];
    /* 0x18 */ s16 timer;
    /* 0x1A */ s8 lastVariant;
};

struct RaceModelListActor {
    char pad0[0x10];
    /* 0x10 */ u16 modelListIndex;
    char pad12[6];
    /* 0x18 */ GfxCommandDest *modelBuffer;
    /* 0x1C */ s16 modelIndexOffset;
    /* 0x1E */ s16 modelCount;
};

struct RaceOverlayTransformActor {
    char pad0[0x18];
    /* 0x18 */ GfxCommandDest *matrix;
    /* 0x1C */ Vec3i pos;
    /* 0x28 */ s16 pitch;
    /* 0x2A */ s16 yaw;
    char pad2C[2];
    /* 0x2E */ s8 matrixDirty;
};

struct RaceThrownModelActor {
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

struct RaceOverlayModelActor {
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

struct RaceParticleActor {
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

extern void func_80045990(s32, s32, void *, void *);
extern void func_800486BC(CourseEffectMatrixSource *, void *);
extern GfxCommandDest *allocFixedTransformMatrix(void *);
extern void func_80048C90(GfxCommandDest *, Vec3i *);
extern s32 func_80048E60(void *);
extern void osWritebackDCache(void *, s32);
extern void addRenderCallback(void *, void *, void *);
extern void enqueuePositionalSoundEffect(s32, void *, s32, s32);
extern u8 gItemEffectRollTable[][0x10];
extern u8 gActionEffectRollTable[][0x10];
extern RaceOverlayEffectSpawn D_800D9518[];
extern RaceOverlayEffectSpawn *D_800D92D0[];
extern Vec3i D_800D9BD8[];
extern RaceOverlayModelEntry *D_800D7754[];
extern void *D_800DA1C0[];
extern Gfx *D_800DA1F0;
extern s32 D_801248D4;
extern s32 D_801248B0;
extern s16 D_80112144;
extern s16 D_80112146;
extern s16 D_80112168;
extern u8 D_80156609;
extern u8 gRaceUpdatePaused;
extern u8 gTrainingCourseLesson;
extern u8 gRaceSplitscreenMode;
extern s8 D_80121D93;
extern s32 D_80121D9C;
extern s32 D_80121DA4;
extern s8 D_8012239F;
extern s32 D_801223A8;
extern s32 D_801223B0;
extern s8 D_801229AB;
extern s32 D_801229B4;
extern s32 D_801229BC;
extern s8 D_80122FB7;
extern s32 D_80122FC0;
extern s32 D_80122FC8;
extern u8 D_80112130[];
extern RaceModelEntry *D_800D91E8[];
extern u32 D_800D9210[];
extern Vtx D_800D92D8[];
extern Vtx D_800D9358[];
extern Gfx gEffectRenderModeSetupDl[];
extern Gfx gEffectRenderModeCleanupDl[];
extern Gfx D_2003A38[];
extern Gfx D_2003AB8[];
extern FixedTransform gIdentityFixedTransform;
extern GfxCommandDest gIdentityMatrix;
extern FixedTransform gIdentityFixedTransform;
extern Gfx *gRegionAllocPtr;
extern void *D_80156614;
extern s16 gRaceCourseIndex;
extern s16 gFrameCounter;
extern void func_80066E10(RaceModelListActor *);
extern void func_80066ABC(RaceModelListActor *);
extern void func_80067034(RaceModelListActor *);
extern void func_800674B4(RaceThrownModelActor *);
extern void func_800681A4(RaceOverlayModelActor *);
extern void func_80068CD4(RaceOverlayModelActor *);
extern void func_800684E4(RaceOverlayModelActor *);
extern void assignPickupRandomEffect(RaceOverlayModelActor *);
extern void func_8006935C(RaceParticleActor *);
extern void func_80069678(RaceParticleActor *);

typedef struct Scratch674B4 {
    char scratch[0x28];
    s32 pad;
} Scratch674B4;

void func_80066760(RaceModelListActor *arg0) {
    RaceOverlayModelEntry *var_s4;
    s32 var_s5;
    s32 var_s7;
    Gfx *temp_s0;
    Gfx *temp_s2;
    Gfx *temp_s3;

    var_s4 = D_800D7754[arg0->modelListIndex];
    var_s7 = TRUE;
    var_s5 = 0;
    if (var_s4->modelIndex != -1) {
        do {
            if (isPositionNearCurrentViewport(&var_s4->pos) != 0) {
                if (var_s7 != 0) {
                    gDPPipeSync(gRegionAllocPtr++);
                    temp_s2 = gRegionAllocPtr++;
                    var_s7 = FALSE;
                    gSPSegment(temp_s2, 0x02, getMemoryBlockBase(*(s16 *)&D_80112130[0x14]));

                    temp_s3 = gRegionAllocPtr++;
                    gSPSegment(temp_s3, 0x03, getMemoryBlockBase(*(s16 *)&D_80112130[0x16]));
                }

                temp_s0 = gRegionAllocPtr++;
                gDma1p(temp_s0, 1, (u32)arg0->modelBuffer + (var_s5 << 6), 0x40, 2);

                temp_s0 = gRegionAllocPtr++;
                gSPDisplayList(temp_s0, D_800DA1C0[var_s4->modelIndex]);
            }
            var_s4++;
            var_s5++;
        } while (var_s4->modelIndex != -1);
    }
}

void func_800668EC(RaceModelListActor *arg0) {
    RaceOverlayModelEntry *entry;
    void *pos;

    entry = D_800D7754[arg0->modelListIndex];
    if (entry->modelIndex != -1) {
        pos = &entry->pos;
        do {
            if (func_80048E60(pos) != 0) {
                func_80088294(pos, 0x1C0000, 0x480000, 2);
            }
            entry++;
            pos = &entry->pos;
        } while (entry->modelIndex != -1);
    }
    addRenderCallback(&D_801248B0, func_80066760, arg0);
}

void func_800669A0(RaceModelListActor *arg0) {
    s32 size;
    RaceOverlayModelEntry *base;
    RaceOverlayModelEntry *entry;
    s32 i;
    CourseEffectMatrixSource transform;
    s32 count;

    base = D_800D7754[arg0->modelListIndex];
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
        *(s16 *)&D_80112130[0x46] = allocMemoryBlock(size);
        arg0->modelBuffer = (void *)getMemoryBlockBase(*(s16 *)&D_80112130[0x46]);

        for (i = 0; i < count; i++) {
            makeFixedRotationY(&transform, entry->assetIndex);
            transform.basePos.x = entry->pos.x;
            transform.basePos.y = entry->pos.y;
            transform.basePos.z = entry->pos.z;
            func_800486BC(&transform, (void *)((u32)arg0->modelBuffer + (i << 6)));
            entry++;
        }

        osWritebackDCache(arg0->modelBuffer, size);
    }
    setCallbackTaskCallback(arg0, func_800668EC);
}

void func_80066ABC(RaceModelListActor *arg0) {
    RaceModelEntry *entry;
    s16 modelIndex;
    s32 i;
    RaceModelListActor *actor;
    u8 padding[0x1];
    void *spA0;
    void *sp9C;

    modelIndex = -1;
    actor = arg0;
    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);

    entry = D_800D91E8[gRaceCourseIndex];
    i = 0;
    if (entry->modelIndex != -1) {
        do {
            if ((entry->enabled != 0) && (isPositionNearCurrentViewport(&entry->transform) != 0)) {
                if (modelIndex != entry->modelIndex + actor->modelIndexOffset) {
                    modelIndex = entry->modelIndex + actor->modelIndexOffset;
                    func_80045990(getMemoryBlockBase(D_80112168), (modelIndex + 4) & 0xFFFF, &spA0, &sp9C);

                    gDPLoadTextureBlock_4b(gRegionAllocPtr++, spA0, G_IM_FMT_CI, 16, 16, 0, G_TX_CLAMP,
                                           G_TX_CLAMP, 0, 0, 0, 0);
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, sp9C);
                }

                gSPMatrix(gRegionAllocPtr++, &actor->modelBuffer[i], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, D_80156614, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

                gSPVertex(gRegionAllocPtr++, (u32)D_800D9210, 4, 0);

                gSP2Triangles(gRegionAllocPtr++, 3, 2, 1, 0, 3, 1, 0, 0);
            }
            entry++;
            i++;
        } while (entry->modelIndex != -1);
    }

    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
}

void func_80066E10(RaceModelListActor *arg0) {
    RaceModelEntry *entry;
    Vec3i *pos;
    RaceModelListActor *actor;
    register s32 one;
    register s32 xzSize;
    register s32 ySize;
    register s32 sentinel;

    entry = D_800D91E8[gRaceCourseIndex];
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
    if (func_80048E60(pos) != 0) {
        if (entry->enabled != 0) {
            if (func_80088E98(pos, xzSize, ySize, 0) != 0) {
                func_8008BB5C(D_80121D80, 0x64);
                entry->enabled = 0;
                enqueuePositionalSoundEffect(0x1A, pos, 0x7F, 0x32);
                goto next;
            }
            if (func_80088E98(pos, xzSize, ySize, one) != 0) {
                func_8008BB5C(D_8012238C, 0x64);
                entry->enabled = 0;
                enqueuePositionalSoundEffect(0x1A, pos, 0x7F, 0x32);
                goto next;
            }
            if (func_80088E98(pos, xzSize, ySize, 2) != 0) {
                func_8008BB5C(D_80122998, 0x64);
                entry->enabled = 0;
                enqueuePositionalSoundEffect(0x1A, pos, 0x7F, 0x32);
                goto next;
            }
            if (func_80088E98(pos, xzSize, ySize, 3) != 0) {
                func_8008BB5C(D_80122FA4, 0x64);
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
    addRenderCallback(&D_801248D4, func_80066ABC, actor);
}

void func_80067034(RaceModelListActor *arg0) {
    register RaceModelListActor *actor1;
    register RaceModelListActor *actor2;
    register RaceModelEntry *script;
    register s32 i;
    register s32 offset;
    register s32 one;

    script = D_800D91E8[gRaceCourseIndex];
    actor1 = arg0;
    actor2 = arg0;
    i = 0;
    if (actor1->modelCount > 0) {
        register GfxCommandDest *template;

        template = &gIdentityMatrix;
        offset = 0;
        one = 1;
        do {
            script->enabled = one;
            actor1->modelBuffer[i] = *template;
            func_80048C90(&actor1->modelBuffer[i], &script->transform);
            i++;
            offset += sizeof(GfxCommandDest);
            script++;
        } while (i < actor2->modelCount);
    }
    osWritebackDCache(actor1->modelBuffer, actor1->modelCount * sizeof(GfxCommandDest));
}

void func_8006713C(RaceModelListActor *arg0) {
    RaceModelListActor *new_var;
    RaceModelEntry *var_v0;

    new_var = arg0;
    new_var->modelCount = 0;
    var_v0 = D_800D91E8[gRaceCourseIndex];
    if (var_v0->modelIndex != -1) {
        do {
            new_var->modelCount += 1;
            var_v0++;
        } while (var_v0->modelIndex != -1);
    }
    if (new_var->modelCount != 0) {
        RACE_MODEL_BUFFER_HANDLE = allocMemoryBlock(new_var->modelCount << 6);
        new_var->modelBuffer = (void *) getMemoryBlockBase(RACE_MODEL_BUFFER_HANDLE);
        func_80067034(new_var);
        setCallbackTaskCallback(new_var, func_80066E10);
    }
}

void func_800671F4(RaceOverlayTransformActor *arg0) {
    FixedMatrix3sScratch scratch;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
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
            gSPSegment(gRegionAllocPtr++, 0x02, getMemoryBlockBase(D_80112144));
            gSPSegment(gRegionAllocPtr++, 0x03, getMemoryBlockBase(D_80112146));
            gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gRegionAllocPtr++, D_800DA1F0);
        }
    }
}

void func_80067364(RaceThrownModelActor *arg0) {
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
            arg0->unk2C = func_8007D200(arg0->unk2C, arg0->pos.x, arg0->pos.z);
            groundY = func_80080CC4(arg0->unk2C, arg0->pos.x, arg0->pos.z);
            pos = &arg0->pos;
            if (arg0->pos.y < groundY) {
                enqueuePositionalSoundEffect(0x20, pos, 0x7F, 0x32);
                arg0->pos.y = groundY;
                arg0->transformedPos.y = arg0->velocity.y / 2;
                arg0->bounceCount++;
            }

            if (arg0->transformedPos.y <= 0) {
                func_80089000(pos, 0x170000, 0x20);
            }
        } else {
            arg0->timer = timer - 1;
        }
    }

    if (arg0->bounceCount == 2) {
        removeCallbackTask(arg0);
        return;
    }

    addRenderCallback(&D_801248B0, func_800671F4, arg0);
}

void func_800674B4(RaceThrownModelActor *arg0) {
    Scratch674B4 sp1C;
    RaceThrownModelActor *temp_a3 = arg0;

    if (gRaceUpdatePaused == 0) {
        makeFixedRotationY(sp1C.scratch, temp_a3->modelIndex);
        temp_a3->timer = 0x32;
        temp_a3->velocity.x = 0;
        temp_a3->velocity.y = 0xB0000;
        temp_a3->velocity.z = 0xFFF90000;
        transformVec3iByFixedMatrix(sp1C.scratch, &temp_a3->velocity, &temp_a3->transformedPos);
        setCallbackTaskCallback(temp_a3, func_80067364);
    }
}

void func_8006752C(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4) {
    RaceThrownModelActor *temp = createCallbackTask(func_800674B4, 0, 0x64);

    if (temp != NULL) {
        temp->pos.x = arg0;
        temp->pos.y = arg1;
        temp->pos.z = arg2;
        temp->modelIndex = arg3;
        temp->unk2C = arg4;
    }
}

// func_800675AC best match: 99.625% (nonmatchings/func_800675AC-731940616440357983/base_15.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_overlay_effects/func_800675AC.s")

#ifdef NON_MATCHING
#define SPAWN_RANGE_MAX 0x14000000
#define SPAWN_RANGE_MIN -0x13FFFFFF

void func_800675AC(RaceOverlaySpawnActor *arg0) {
    RaceThrownModelActor *savedSpawned;
    volatile s32 forceStack[6];
    RaceOverlayEffectSpawn *savedEntry;
    RaceOverlayEffectSpawn *newEntry;
    RaceThrownModelActor *spawned;
    RaceOverlayEffectSpawn *entry;
    s32 found;
    s32 diffX;
    s32 diffZ;
    s32 rand;
    s32 prev;

    if (gRaceUpdatePaused == 0) {
        if (arg0->timer == 0) {
            arg0->timer = 0x20;
            newEntry = D_800D92D0[arg0->spawnIndex];
            entry = newEntry;
            found = FALSE;
            if (gRaceSplitscreenMode != 2) {
                if (D_80121D93 != 0) {
                    diffZ = D_80121D9C - entry->pos.x;
                    if ((diffZ < SPAWN_RANGE_MAX) && (diffZ >= SPAWN_RANGE_MIN)) {
                        diffX = D_80121DA4 - entry->pos.z;
                        if ((diffX < SPAWN_RANGE_MAX) && (diffX >= SPAWN_RANGE_MIN)) {
                            found = TRUE;
                        }
                    }
                }
                if (D_8012239F != 0) {
                    diffX = D_801223A8 - entry->pos.x;
                    diffZ = D_801223B0 - entry->pos.z;
                    if ((diffX < SPAWN_RANGE_MAX) && (diffX >= SPAWN_RANGE_MIN) && (diffZ < SPAWN_RANGE_MAX) &&
                        (diffZ >= SPAWN_RANGE_MIN)) {
                        found = TRUE;
                    }
                }
                if (D_801229AB != 0) {
                    diffX = D_801229B4 - entry->pos.x;
                    diffZ = D_801229BC - entry->pos.z;
                    if ((diffX < SPAWN_RANGE_MAX) && (diffX >= SPAWN_RANGE_MIN) && (diffZ < SPAWN_RANGE_MAX) &&
                        (diffZ >= SPAWN_RANGE_MIN)) {
                        found = TRUE;
                    }
                }
                if (D_80122FB7 != 0) {
                    diffX = D_80122FC0 - entry->pos.x;
                    diffZ = D_80122FC8 - entry->pos.z;
                    if ((diffX < SPAWN_RANGE_MAX) && (diffX >= SPAWN_RANGE_MIN) && (diffZ < SPAWN_RANGE_MAX) &&
                        (diffZ >= SPAWN_RANGE_MIN)) {
                        found = TRUE;
                    }
                }
            } else {
                found = TRUE;
            }

            if (found != 0) {
                spawned = createCallbackTask(func_800674B4, 0, 0x64);
                if (spawned != NULL) {
                    savedSpawned = spawned;
                    entry = (savedEntry = entry);
                    rand = func_80043120() & 3;
                    spawned = savedSpawned;
                    prev = arg0->lastVariant;
                    if (rand == prev) {
                        rand = (prev + 1) & 3;
                    }
                    arg0->lastVariant = rand;
                    entry = &entry[rand];
                    spawned->pos.x = entry->pos.x;
                    spawned->pos.y = entry->pos.y;
                    spawned->pos.z = entry->pos.z;
                    spawned->modelIndex = entry->rotation;
                    spawned->unk2C = entry->variant;
                }
            }
        } else {
            arg0->timer--;
        }
    }
}

#undef SPAWN_RANGE_MAX
#undef SPAWN_RANGE_MIN
#endif

// func_80067830 best match: display-list command stream matched, remaining differences are stack/local layout.
#pragma GLOBAL_ASM("asm/nonmatchings/race_overlay_effects/func_80067830.s")

#ifdef NON_MATCHING
void func_80067830(RaceOverlayModelActor *arg0) {
    GfxCommandSource spF4;
    Gfx *sp10C;
    Gfx *sp108;
    Gfx *spB0;
    Gfx *spAC;
    Gfx *sp8C;
    Gfx *temp_v0;
    Gfx *temp_t1;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            spF4 = *(GfxCommandSource *)&gIdentityFixedTransform;
            spF4.words[5] = arg0->drawPos.x;
            spF4.words[6] = arg0->drawPos.y;
            spF4.words[7] = arg0->drawPos.z;
            arg0->displayList = allocFixedTransformMatrix(&spF4);
            arg0->rotationDisplayList = allocFixedTransformMatrix((GfxCommandSource *) arg0->rotationMatrix);
            spF4 = *(GfxCommandSource *) arg0->rotationMatrix;
            spF4.halfwords[0] /= 2;
            spF4.halfwords[1] /= 2;
            spF4.halfwords[2] /= 2;
            spF4.halfwords[3] /= 2;
            spF4.halfwords[4] /= 2;
            spF4.halfwords[5] /= 2;
            spF4.halfwords[6] /= 2;
            spF4.halfwords[7] /= 2;
            spF4.halfwords[8] /= 2;
            spF4.words[6] += (fixedSine((s16)((gFrameCounter << 7) & 0xFFF)) << 7) + 0x300000;
            arg0->scaleDisplayList = allocFixedTransformMatrix(&spF4);
        }

        if (arg0->scaleDisplayList != NULL) {
            gDPPipeSync(gRegionAllocPtr++);
            temp_v0 = gRegionAllocPtr++;
            temp_v0->words.w0 = 0xBC000806;
            sp10C = temp_v0;
            sp10C->words.w1 = getMemoryBlockBase(D_80112144);
            temp_v0 = gRegionAllocPtr++;
            temp_v0->words.w0 = 0xBC000C06;
            sp108 = temp_v0;
            sp108->words.w1 = getMemoryBlockBase(D_80112146);
            gSPMatrix(gRegionAllocPtr++, arg0->scaleDisplayList, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            if (arg0->variant == 0) {
                gSPDisplayList(gRegionAllocPtr++, D_2003A38);
            } else {
                gSPDisplayList(gRegionAllocPtr++, D_2003AB8);
            }
            gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
            gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->image0, G_IM_FMT_CI, 32, 32, 0, G_TX_CLAMP,
                                    G_TX_CLAMP, 0, 0, 0, 0);
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette0);
            gSPMatrix(gRegionAllocPtr++, arg0->displayList, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPMatrix(gRegionAllocPtr++, D_80156614, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gDma1p(gRegionAllocPtr++, G_VTX, D_800D92D8, 0x103F, 0);
            temp_v0 = gRegionAllocPtr++;
            temp_v0->words.w1 = 0x60200;
            temp_v0->words.w0 = 0xB1060402;
            gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->image1, G_IM_FMT_CI, 32, 32, 0, G_TX_CLAMP,
                                    G_TX_CLAMP, 0, 0, 0, 0);
            spAC = gRegionAllocPtr;
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette1);
            sp8C = gRegionAllocPtr;
            gSPMatrix(gRegionAllocPtr++, arg0->rotationDisplayList,
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gDma1p(gRegionAllocPtr++, G_VTX, D_800D9358, 0x513F, 0);
            temp_t1 = gRegionAllocPtr++;
            temp_t1->words.w0 = 0xB1060402;
            temp_t1->words.w1 = 0x60200;
            temp_v0 = gRegionAllocPtr++;
            temp_v0->words.w1 = 0xE0A08;
            temp_v0->words.w0 = 0xB10E0C0A;
            temp_v0 = gRegionAllocPtr++;
            temp_v0->words.w1 = 0x161210;
            temp_v0->words.w0 = 0xB1161412;
            temp_v0 = gRegionAllocPtr++;
            temp_v0->words.w1 = 0x1E1A18;
            temp_v0->words.w0 = 0xB11E1C1A;
            spB0 = gRegionAllocPtr;
            gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->image2, G_IM_FMT_CI, 32, 32, 0, G_TX_CLAMP,
                                    G_TX_CLAMP, 0, 0, 0, 0);
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette2);
            temp_v0 = gRegionAllocPtr++;
            temp_v0->words.w1 = 0x262220;
            temp_v0->words.w0 = 0xB1262422;
            gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
        }
    }
}
#endif

void func_800681A4(RaceOverlayModelActor *arg0) {
    RaceOverlayMatrixScratch sp64;
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

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sp64.source = gIdentityFixedTransform;
            sp64.source.translation.x = arg0->drawPos.x;
            sp64.source.translation.y = arg0->drawPos.y;
            sp64.source.translation.z = arg0->drawPos.z;
            arg0->displayList = allocFixedTransformMatrix(&sp64.source);
        }
        do { if (arg0->displayList != NULL) { temp_v0 = gRegionAllocPtr++; temp_v0->words.w0 = 0x06000000; temp_v0->words.w1 = (u32) gEffectRenderModeSetupDl; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image0; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x070FF400; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400400; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0007C07C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette0; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; temp_v0_15 = gRegionAllocPtr++; temp_v0_15->words.w0 = 0x01020040; temp_v0_15->words.w1 = (u32) arg0->displayList; temp_v0_16 = gRegionAllocPtr++; temp_v0_16->words.w0 = 0x01000040; temp_v0_16->words.w1 = (u32) D_80156614; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400207F; temp_v0_17->words.w1 = (u32) D_800D92D8; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; temp_v0_19 = gRegionAllocPtr++; temp_v0_19->words.w0 = 0x06000000; temp_v0_19->words.w1 = (u32) gEffectRenderModeCleanupDl; } } while (0);
    }
}

// func_800684E4 best match: 99.579% (base_22.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_overlay_effects/func_800684E4.s")

#ifdef NON_MATCHING
void func_800684E4(RaceOverlayModelActor *arg0) {
    Gfx *temp_v0;
    GfxCommandSource spF4;
    Gfx *temp_t1;
    Gfx *temp_v0_18;
    Gfx *temp_v0_34;
    Gfx *temp_v0_35;
    Gfx *temp_v0_36;
    Gfx *temp_v0_50;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }
    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            spF4 = *(GfxCommandSource *)&gIdentityFixedTransform;
            spF4.words[5] = arg0->drawPos.x;
            spF4.words[6] = arg0->drawPos.y;
            spF4.words[7] = arg0->drawPos.z;
            arg0->displayList = allocFixedTransformMatrix(&spF4);
            arg0->rotationDisplayList = allocFixedTransformMatrix((GfxCommandSource *) arg0->rotationMatrix);
        }
        if (arg0->displayList != NULL) {
            if (arg0->rotationDisplayList != NULL) {
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
                gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->image0, G_IM_FMT_CI, 32, 32, 0, G_TX_CLAMP,
                                        G_TX_CLAMP, 0, 0, 0, 0);
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette0);
                gSPMatrix(gRegionAllocPtr++, arg0->displayList, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, D_80156614, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gDma1p(gRegionAllocPtr++, G_VTX, D_800D92D8, 0x103F, 0);
                temp_v0_18 = gRegionAllocPtr++;
                temp_v0_18->words.w1 = 0x60200;
                temp_v0_18->words.w0 = 0xB1060402;
                gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->image1, G_IM_FMT_CI, 32, 32, 0, G_TX_CLAMP,
                                        G_TX_CLAMP, 0, 0, 0, 0);
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette1);
                gSPMatrix(gRegionAllocPtr++, arg0->rotationDisplayList,
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gDma1p(gRegionAllocPtr++, G_VTX, D_800D9358, 0x513F, 0);
                temp_t1 = gRegionAllocPtr++;
                temp_t1->words.w0 = 0xB1060402;
                temp_t1->words.w1 = 0x60200;
                temp_v0_34 = gRegionAllocPtr++;
                temp_v0_34->words.w1 = 0xE0A08;
                temp_v0_34->words.w0 = 0xB10E0C0A;
                temp_v0_35 = gRegionAllocPtr++;
                temp_v0_35->words.w1 = 0x161210;
                temp_v0_35->words.w0 = 0xB1161412;
                temp_v0_36 = gRegionAllocPtr++;
                temp_v0_36->words.w1 = 0x1E1A18;
                temp_v0_36->words.w0 = 0xB11E1C1A;
                gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->image2, G_IM_FMT_CI, 32, 32, 0, G_TX_CLAMP,
                                        G_TX_CLAMP, 0, 0, 0, 0);
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette2);
                temp_v0_50 = gRegionAllocPtr++;
                temp_v0_50->words.w1 = 0x262220;
                temp_v0_50->words.w0 = 0xB1262422;
                gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
            }
        }
    }
}
#endif

void func_80068BF0(RaceOverlayModelActor *arg0) {
    s16 temp_v0;
    s32 temp_v1;
    RaceOverlayModelActor *temp_s0 = arg0;
    void *temp_s1;

    if (gRaceUpdatePaused == 0) {
        arg0->timer--;
        temp_v0 = arg0->timer;
        temp_v1 = arg0->pos.y - (temp_v0 * 0x14000);
        arg0->drawPos.y = temp_v1 + 0x140000;
        arg0->spawnPos.y = temp_v1;
        if (temp_v0 == 0) {
            setCallbackTaskCallback(arg0, assignPickupRandomEffect);
        }
        temp_s1 = &temp_s0->pos;
        func_80088C80(temp_s1, 0xC0000, 0x180000, 0);
        func_80088C80(temp_s1, 0xC0000, 0x180000, 1);
        func_80088C80(temp_s1, 0xC0000, 0x180000, 2);
        func_80088C80(temp_s1, 0xC0000, 0x180000, 3);
    }
    addRenderCallback(&D_801248D4, func_800684E4, temp_s0);
}

void func_80068CD4(RaceOverlayModelActor *arg0) {
    if (gRaceUpdatePaused == 0) {
        arg0->drawPos.y += arg0->velY;
        arg0->velY += 0xFFFF0000;

        if (arg0->drawPos.y < arg0->pos.y) {
            arg0->drawPos.y = arg0->pos.y;
            arg0->timer = 0x10;
            setCallbackTaskCallback(arg0, func_80068BF0);
        }

        func_80088C80(&arg0->pos, 0xC0000, 0x180000, 0);
        func_80088C80(&arg0->pos, 0xC0000, 0x180000, 1);
        func_80088C80(&arg0->pos, 0xC0000, 0x180000, 2);
        func_80088C80(&arg0->pos, 0xC0000, 0x180000, 3);
    }
    addRenderCallback(&D_801248D4, func_800681A4, arg0);
}

void func_80068DB4(RaceOverlayModelActor *arg0) {
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
            setCallbackTaskCallback(arg0, func_80068CD4);
            var_v1 = arg0->drawPos.y;
        }
        temp_s1 = &arg0->drawPos;
        if (var_v1 < 0) {
            func_80088C80(temp_s1, 0xC0000, 0x180000, 0);
            func_80088C80(temp_s1, 0xC0000, 0x180000, 1);
            func_80088C80(temp_s1, 0xC0000, 0x180000, 2);
            func_80088C80(temp_s1, 0xC0000, 0x180000, 3);
        }
    }
    addRenderCallback(&D_801248D4, func_800681A4, arg0);
}

// assignPickupRandomEffect best match: 99.901%
#pragma GLOBAL_ASM("asm/nonmatchings/race_overlay_effects/assignPickupRandomEffect.s")

#ifdef NON_MATCHING
void assignPickupRandomEffect(RaceOverlayModelActor *arg0) {
    RaceInputPlayer *player;
    Vec3i *pos;
    s32 found;
    s32 i;
    s32 maxPlayers;

    if (gRaceUpdatePaused == 0) {
        found = 0;
        i = 0;
        pos = &arg0->pos;
        maxPlayers = 4;

loop:
        if (func_80088E98(pos, 0xBF000, 0x170000, i) == 0) {
            goto next;
        }

        player = &D_80121D80[i];
        found = 1;
        if ((player->unk568 < 0x64) && (player->unk4 == 0)) {
            goto next;
        }

        arg0->velY = 0x60000;
        if (player->unk568 >= 0x64) {
            player->unk568 -= 0x64;
        } else {
            player->unk568 = 0;
        }

        if (arg0->variant == 0) {
            player->itemEffectType = gItemEffectRollTable[player->rankIndex][randomNextObject((RandomStateObject *)player) & 0xF];
            if (gTrainingCourseLesson != 0) {
                player->itemEffectType = 1;
            }
            player->itemEffectCount = 3;
            player->pad513[0] = maxPlayers;
        } else {
            player->actionEffectType = gActionEffectRollTable[player->rankIndex][randomNextObject((RandomStateObject *)player) & 0xF];
            if (gTrainingCourseLesson != 0) {
                player->actionEffectType = 1;
            }
            if ((gRaceCourseIndex == 8) && (player->unk4 != 0) && (player->actionEffectType == maxPlayers)) {
                if (randomNextMain() != 0) {
                    player->actionEffectType = 6;
                }
            }
            player->pad516[0] = maxPlayers;
        }

        setCallbackTaskCallback(arg0, func_80068DB4);
        func_80069808(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->rotation, 0);
        func_80069808(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->rotation, 1);
        func_80069808(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->rotation, 2);
        func_80069808(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->rotation, 3);
        func_80069808(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->rotation, 4);
        func_80069808(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->rotation, 5);
        func_80069808(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->rotation, 6);
        func_80069808(arg0->pos.x, arg0->pos.y, arg0->pos.z, arg0->rotation, 7);
        enqueuePositionalSoundEffect(0x1F, pos, 0x7F, 0x32);
        goto done;

next:
        i++;
        if (i != maxPlayers) {
            goto loop;
        }
        if (found != 0) {
            func_80088294(pos, 0xBF000, 0x170000, 0x800);
        }
    }

done:
    addRenderCallback(&D_801248D4, func_80067830, arg0);
}
#endif

void func_800691C8(RaceOverlayModelActor *arg0) {
    RaceOverlayEffectSpawn *entry = &D_800D9518[arg0->spawnIndex];

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
    func_80045990(getMemoryBlockBase(D_80112168), 0x1E, &arg0->image0, &arg0->palette0);
    if (arg0->variant == 0) {
        func_80045990(getMemoryBlockBase(D_80112168), 0x20, &arg0->image1, &arg0->palette1);
    } else {
        func_80045990(getMemoryBlockBase(D_80112168), 0x21, &arg0->image1, &arg0->palette1);
    }
    func_80045990(getMemoryBlockBase(D_80112168), 0x22, &arg0->image2, &arg0->palette2);
    setCallbackTaskCallback(arg0, assignPickupRandomEffect);
}

void func_8006935C(RaceParticleActor *arg0) {
    volatile s32 pad;
    CourseEffectMatrixSource transform;
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

    if (D_80156609 != 0) {
        arg0->transformDirty = 1;
    }
    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
        if (arg0->transformDirty != 0) {
            arg0->transformDirty = 0;
            makeFixedRotationXYZ(transform.rotation, arg0->rotX, arg0->rotY, arg0->rotZ);
            transform.basePos.x = arg0->pos.x;
            transform.basePos.y = arg0->pos.y;
            transform.basePos.z = arg0->pos.z;
            arg0->displayList = allocFixedTransformMatrix((GfxCommandSource *)&transform);
        }
        if (arg0->displayList != NULL) {
            temp_v0 = gRegionAllocPtr++;
            temp_v0->words.w1 = (u32)gEffectRenderModeSetupDl;
            temp_v0->words.w0 = 0x06000000;
            temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32)arg0->palette; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x070FF400; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400400; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0007C07C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32)arg0->image; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; temp_v0_15 = gRegionAllocPtr++; temp_v0_15->words.w0 = 0x01020040; temp_v0_15->words.w1 = (u32)arg0->displayList; temp_v0_16 = gRegionAllocPtr++; temp_v0_16->words.w0 = 0x0400103F; temp_v0_16->words.w1 = (u32)&D_800D92D8[((((u16)arg0->spawnOffsetIndex) >> 1) * 4) + 8]; if (arg0->spawnOffsetIndex & 1) { temp_v0_17 = gRegionAllocPtr++;
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

void func_80069678(RaceParticleActor *arg0) {
    s32 temp_v0;
    RaceParticleActor *temp_a2 = arg0;

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
        addRenderCallback(&D_801248D4, func_8006935C, temp_a2);
        return;
    }
    removeCallbackTask(temp_a2);
}

void func_80069754(RaceParticleActor *arg0) {
    char sp28[0x20];

    arg0->timer = 0xA;
    arg0->rotVelX = randomNextMain() - 0x80;
    arg0->rotVelY = randomNextMain() - 0x80;
    arg0->rotVelZ = randomNextMain() - 0x80;
    makeFixedRotationY(sp28, arg0->rotY);
    transformVec3iByFixedMatrix(sp28, &D_800D9BD8[arg0->spawnOffsetIndex], &arg0->velocity);
    func_80045990(getMemoryBlockBase(D_80112168), 0x22, &arg0->palette, &arg0->image);
    setCallbackTaskCallback(arg0, func_80069678);
}

void func_80069808(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4) {
    RaceParticleActor *temp = createCallbackTaskPreservingArgs(func_80069754, 5, 0x3B);

    if (temp != NULL) {
        temp->spawnOffsetIndex = arg4;
        temp->pos.x = arg0;
        temp->pos.y = arg1;
        temp->pos.z = arg2;
        temp->rotY = arg3;
    }
}
