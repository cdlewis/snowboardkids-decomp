#include "common.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/race/effects/race_intro_effects.h"
#include "game/math/spatial_math.h"
#include "game/math/fixed_point_math.h"
#include "game/race/player/race_player_input.h"

#define ASSET_HANDLE(index) (gAssetHandles[(index)])
#define RACE_INTRO_EFFECTS_GFX_CMD(pkt, cmd0, cmd1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (cmd0); \
    _g->words.w1 = (cmd1); \
}

typedef struct {
    /* 0x00 */ s8 textureIndex;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ Vec3i position;
} RaceIntroRenderCommandEntry;

struct RaceIntroMeshActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Mtx *matrices;
};

typedef void (*RaceIntroEffectCallback)(RaceIntroEffectActor *);

typedef union {
    s32 word;
    struct {
        s16 yaw;
        s16 pitch;
    } half;
} PackedAngles;

struct RaceIntroEffectActor {
    /* 0x00 */ char pad0[0x8];
    /* 0x08 */ RaceIntroEffectCallback callback;
    /* 0x0C */ char padC[0x4];
    /* 0x10 */ u16 index;
    /* 0x12 */ char pad12[0x6];
    /* 0x18 */ Vec3i position;
    /* 0x24 */ s32 velocityY;
    /* 0x28 */ s32 radius;
    /* 0x2C */ PackedAngles angle;
    union {
        struct {
            /* 0x30 */ s16 scale;
            /* 0x32 */ s16 tilt;
            /* 0x34 */ s16 pitchVelocity;
            /* 0x36 */ s16 spinVelocity;
        };
        struct {
            /* 0x30 */ void *image;
            /* 0x34 */ void *palette;
        };
    };
    /* 0x38 */ s16 timer;
    /* 0x3A */ s16 stateTimer;
    /* 0x3C */ Gfx *displayList0;
    union {
        /* 0x40 */ Gfx *displayList1;
        /* 0x40 */ s8 displayList0Valid;
    };
    /* 0x44 */ s8 displayListValid;
};

extern void osWritebackDCache(void *, s32);
extern u8 gRenderMatricesDirty;
extern Gfx *gRegionAllocPtr;
extern Gfx D_20028F0[];
extern Gfx D_2002DB8[];
extern Gfx *allocFixedTransformMatrix(FixedTransform *arg0);
extern Vec3i gRaceIntroBillboardPositions[];
extern Vtx *gRaceIntroModelVerticesByCourse[];
extern RaceIntroRenderCommandEntry *gRaceIntroModelCommandsByCourse[];
extern RaceIntroRenderCommandEntry *gRaceIntroAnimatedBillboardCommandsByCourse[];
extern u16 gRaceIntroAnimatedBillboardTextureIds[];
extern Gfx gRaceIntroAnimatedBillboardVertices[];
extern Gfx gRaceIntroBillboardVertices[];
extern Gfx gEffectRenderModeSetupDl[];
extern Gfx gEffectRenderModeCleanupDl[];
extern void setPackedMatrixTranslation(Mtx *, Vec3i *);
extern void getAssetTableImagePaletteAndSize(u8 *, s32, u32 *, u32 *, s16 *, s16 *);

extern s16 gRaceCourseIndex;
extern s16 gFrameCounter;
extern u32 gViewportMatrix;

void drawRaceIntroModelMeshes(RaceIntroMeshActor *arg0) {
    volatile u8 pad[0xC];
    u32 image;
    u32 palette;
    s16 width;
    s16 height;
    RaceIntroRenderCommandEntry *entry;
    Vtx *vertices;
    Gfx *gfx;
    s16 textureIndex;
    s32 i;

    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
    entry = gRaceIntroModelCommandsByCourse[gRaceCourseIndex];
    vertices = gRaceIntroModelVerticesByCourse[gRaceCourseIndex];
    textureIndex = -1;
    i = 0;

    if (entry->textureIndex != -1) {
        do {
            if (isPositionNearCurrentRaceViewportCamera(&entry->position) != 0) {
                if (textureIndex != entry->textureIndex) {
                    textureIndex = entry->textureIndex;
                    getAssetTableImagePaletteAndSize((u8 *)getRelocatableHeapBlockBase((s32)ASSET_HANDLE(0x1D)), (u16)textureIndex, &image, &palette,
                                  &width, &height);
                    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, width, height, 0, G_TX_CLAMP,
                                            G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
                }

                gSPMatrix(gRegionAllocPtr++, &arg0->matrices[i], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gfx = gRegionAllocPtr++;
                gfx->words.w0 = 0x0400103F;
                gfx->words.w1 = (u32)&vertices[entry->textureIndex * 4];
                RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
            }
            entry++;
            i++;
        } while (entry->textureIndex != -1);
    }

    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
}

void enqueueDrawRaceIntroModelMeshes(RaceIntroMeshActor *arg0) {
    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)drawRaceIntroModelMeshes, arg0);
}

void initRaceIntroModelMeshes(RaceIntroMeshActor *arg0) {
    RaceIntroRenderCommandEntry *entry;
    s32 count;
    s32 allocSize;
    s32 i;

    entry = gRaceIntroModelCommandsByCourse[gRaceCourseIndex];
    count = 0;
    if (entry->textureIndex != -1) {
        do {
            count++;
            entry++;
        } while (entry->textureIndex != -1);
    }

    if (count != 0) {
        entry = gRaceIntroModelCommandsByCourse[gRaceCourseIndex];
        allocSize = count * sizeof(Mtx);
        gAssetHandles[0x27] = allocRelocatableHeapBlock(allocSize);
        arg0->matrices = getRelocatableHeapBlockBase(gAssetHandles[0x27]);

        i = 0;
        if (count > 0) {
            do {
                arg0->matrices[i] = gIdentityMatrix;
                setPackedMatrixTranslation(&arg0->matrices[i], &entry->position);
                i++;
                entry++;
            } while (i != count);
        }
        osWritebackDCache(arg0->matrices, allocSize);
    }

    setCallbackTaskCallback(arg0, (CallbackTaskCallback)enqueueDrawRaceIntroModelMeshes);
}

void drawRaceIntroBillboard(RaceIntroEffectActor *arg0) {
    FixedTransform sp70;

    if (gRenderMatricesDirty != 0) {
        arg0->displayList0Valid = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->position) != 0) {
        if (arg0->displayList0Valid != 0) {
            sp70 = gIdentityFixedTransform;
            sp70.translation.x = arg0->position.x;
            sp70.translation.y = arg0->position.y;
            sp70.translation.z = arg0->position.z;
            arg0->displayList0 = allocFixedTransformMatrix(&sp70);
        }

        if (arg0->displayList0 != NULL) {
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)gEffectRenderModeSetupDl);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0xFD500000, (u32)arg0->image);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0xF5500000, 0x07080200);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0xF3000000, 0x070FF400);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0xF5400400, 0x00080200);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0xF2000000, 0x0007C07C);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)arg0->palette);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0x01020040, (u32)arg0->displayList0);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0x01000040, gViewportMatrix);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)gRaceIntroBillboardVertices);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x00060200);
            RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)gEffectRenderModeCleanupDl);
        }
    }
}

void updateRaceIntroBillboard(RaceIntroEffectActor *arg0) {
    Vec3i sp44;
    FixedTransform transform;
    RaceIntroEffectActor *temp_s0 = arg0;

    arg0->timer--;
    if (arg0->timer == 0) {
        arg0->timer = randomNextMain() + 0x1E;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitRaceIntroBillboardSpawn);
    }
    makeFixedRotationY(transform.rotation, 0x6D0);
    transformVec3iByFixedMatrix(transform.rotation, (Vec3i *)&temp_s0->velocityY, &sp44);
    temp_s0->position.x += sp44.x;
    temp_s0->position.y += sp44.y;
    temp_s0->position.z += sp44.z;
    temp_s0->radius -= 0x4000;
    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)drawRaceIntroBillboard, (void *)temp_s0);
}

void waitRaceIntroBillboardSpawn(RaceIntroEffectActor *arg0) {
    Vec3i *temp_t3;

    arg0->timer--;
    if (!arg0->timer) {
        arg0->timer = 0x5A;
        temp_t3 = &gRaceIntroBillboardPositions[arg0->index];
        arg0->position = *(&gRaceIntroBillboardPositions[arg0->index]);
        arg0->velocityY = 0;
        arg0->radius = 0;
        arg0->angle.word = 0xFFF00000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceIntroBillboard);
    }
}

void initRaceIntroBillboard(RaceIntroEffectActor *arg0) {
    arg0->timer = (arg0->index * 0x1E) + 0x1E;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(ASSET_HANDLE(0x1D)),
                                 arg0->index + 3,
                                 &arg0->image, &arg0->palette);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitRaceIntroBillboardSpawn);
}

void drawRaceIntroFlyoverActor(RaceIntroEffectActor *arg0) {
    volatile s32 pad0[1];
    FixedTransform sp84;
    FixedTransform sp64;
    FixedTransform sp44;
    volatile s32 pad1[1];

    if (gRenderMatricesDirty != 0) {
        arg0->displayListValid = 0;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->position) != 0) {
        if (arg0->displayListValid == 0) {
            makeFixedRotationY(sp84.rotation, arg0->spinVelocity);
            sp84.translation.x = 0;
            sp84.translation.y = 0x600000;
            sp84.translation.z = 0;
            makeFixedRotationZXY(sp64.rotation, arg0->scale, arg0->angle.half.yaw + 0x800, arg0->angle.half.pitch);
            sp64.translation.x = arg0->position.x;
            sp64.translation.y = arg0->position.y;
            sp64.translation.z = arg0->position.z;
            composeFixedTransforms(&sp84, &sp64, &sp44);
            arg0->displayList0 = allocFixedTransformMatrix(&sp64);
            arg0->displayList1 = allocFixedTransformMatrix(&sp44);
        }

        if (arg0->displayList1 != NULL) {
            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
            gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
            gSPMatrix(gRegionAllocPtr++, arg0->displayList0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gRegionAllocPtr++, D_20028F0);
            gSPMatrix(gRegionAllocPtr++, arg0->displayList1, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gRegionAllocPtr++, D_2002DB8);
        }
    }
}

void approachRaceIntroFlyoverSpinStep(RaceIntroEffectActor *arg0, s16 arg1) {
    s16 diff = arg1 - arg0->timer;

    if (diff >= 5) {
        diff = 4;
    }
    if (diff < -4) {
        diff = -4;
    }
    arg0->timer += diff;
}

void approachRaceIntroFlyoverVerticalVelocity(RaceIntroEffectActor *arg0, s32 arg1) {
    s32 diff = arg1 - arg0->velocityY;

    if (diff >= 0x2001) {
        diff = 0x2000;
    }
    if (diff < -0x2000) {
        diff = -0x2000;
    }
    arg0->velocityY += diff;
}

void approachRaceIntroFlyoverOrbitRadius(RaceIntroEffectActor *arg0, s32 arg1) {
    s32 diff = arg1 - arg0->radius;

    if (diff >= 0x2001) {
        diff = 0x2000;
    }
    if (diff < -0x2000) {
        diff = -0x2000;
    }
    if (diff >= 0 && arg0->radius != 0) {
        if (arg0->tilt < 0x800) {
            arg0->tilt += 0x28;
        }
    } else {
        if (arg0->tilt != 0) {
            arg0->tilt -= 0x28;
        }
    }
    arg0->radius += diff;
}

void approachRaceIntroFlyoverPitchVelocity(RaceIntroEffectActor *arg0, s16 arg1) {
    s16 diff = arg1 - arg0->pitchVelocity;

    if (diff >= 5) {
        diff = 4;
    }
    if (diff < -4) {
        diff = -4;
    }

    arg0->pitchVelocity += diff;
    arg0->angle.half.pitch += (((-arg0->pitchVelocity * 2) - arg0->angle.half.pitch) >> 3);
}

void updateRaceIntroFlyoverActor(RaceIntroEffectActor *arg0) {
    s32 sine;
    s32 cosine;

    arg0->spinVelocity += arg0->timer;
    arg0->angle.half.yaw += arg0->pitchVelocity;
    arg0->position.y += arg0->velocityY;

    sine = fixedSine(arg0->angle.half.yaw);
    cosine = fixedCosine(arg0->angle.half.yaw);

    arg0->position.x += ((s64) -arg0->radius * sine) / 0x1000;
    arg0->position.z += ((s64) -arg0->radius * cosine) / 0x1000;
    arg0->scale = 0x80 - ((fixedSine(arg0->tilt + 0x400) + 0x1000) / 0x40);

    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)drawRaceIntroFlyoverActor, (void *)arg0);
}

void updateRaceIntroFlyoverIdle(RaceIntroEffectActor *arg0) {
    approachRaceIntroFlyoverSpinStep(arg0, 0x130);
    approachRaceIntroFlyoverVerticalVelocity(arg0, 0);
    approachRaceIntroFlyoverOrbitRadius(arg0, 0x100000);
    approachRaceIntroFlyoverPitchVelocity(arg0, 0xA);
    updateRaceIntroFlyoverActor(arg0);
}

void initRaceIntroFlyoverIdle(RaceIntroEffectActor *arg0) {
    arg0->position.x = 0xB51A13A3;
    arg0->position.y = 0xD0E85B43;
    arg0->position.z = 0x9A645264;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0x17A0;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0x130;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->radius = 0x100000;
    arg0->stateTimer = 0;
    if (gRacePlayers[0].unk508 == 2) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceIntroFlyoverIdle);
    }
}

void updateRaceIntroFlyoverLongPanReturn(RaceIntroEffectActor *arg0) {
    approachRaceIntroFlyoverSpinStep(arg0, 0x130);
    approachRaceIntroFlyoverVerticalVelocity(arg0, -0x20000);
    approachRaceIntroFlyoverOrbitRadius(arg0, 0x100000);
    approachRaceIntroFlyoverPitchVelocity(arg0, -3);
    updateRaceIntroFlyoverActor(arg0);
    arg0->stateTimer--;
    if (arg0->stateTimer == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)initRaceIntroFlyoverIdle);
    }
}

void initRaceIntroFlyoverLongPanReturn(RaceIntroEffectActor *arg0) {
    arg0->position.x = 0x05CA84CF;
    arg0->position.y = 0x00D0C976;
    arg0->position.z = 0xFAA3DA4A;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0x15E0;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0x130;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->radius = 0x100000;
    arg0->stateTimer = 0x154;
    if (gRacePlayers[0].unk508 == 1) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceIntroFlyoverLongPanReturn);
    }
}

void updateRaceIntroFlyoverLongPanHold(RaceIntroEffectActor *arg0) {
    approachRaceIntroFlyoverSpinStep(arg0, 0x130);
    approachRaceIntroFlyoverVerticalVelocity(arg0, 0);
    approachRaceIntroFlyoverOrbitRadius(arg0, 0x100000);
    approachRaceIntroFlyoverPitchVelocity(arg0, 0);
    updateRaceIntroFlyoverActor(arg0);
    arg0->stateTimer--;
    if (arg0->stateTimer == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)initRaceIntroFlyoverLongPanReturn);
    }
}

void updateRaceIntroFlyoverLongPanPitchUp(RaceIntroEffectActor *arg0) {
    approachRaceIntroFlyoverSpinStep(arg0, 0x130);
    approachRaceIntroFlyoverVerticalVelocity(arg0, 0);
    approachRaceIntroFlyoverOrbitRadius(arg0, 0x100000);
    approachRaceIntroFlyoverPitchVelocity(arg0, 0x30);
    updateRaceIntroFlyoverActor(arg0);
    arg0->stateTimer--;
    if (arg0->stateTimer == 0) {
        arg0->stateTimer = 0x96;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceIntroFlyoverLongPanHold);
    }
}

void updateRaceIntroFlyoverLongPanRise(RaceIntroEffectActor *arg0) {
    approachRaceIntroFlyoverSpinStep(arg0, 0x130);
    approachRaceIntroFlyoverVerticalVelocity(arg0, 0x20000);
    approachRaceIntroFlyoverOrbitRadius(arg0, 0x100000);
    approachRaceIntroFlyoverPitchVelocity(arg0, 0);
    arg0->stateTimer--;
    updateRaceIntroFlyoverActor(arg0);
    if (arg0->stateTimer == 0) {
        arg0->stateTimer = 0x2A;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceIntroFlyoverLongPanPitchUp);
    }
}

void waitRaceIntroFlyoverLongPanTrigger(RaceIntroEffectActor *arg0) {
    approachRaceIntroFlyoverSpinStep(arg0, 0x130);
    updateRaceIntroFlyoverActor(arg0);
    if (gRacePlayers[0].surfaceAngle == 0x35) {
        arg0->stateTimer = 0x6A;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceIntroFlyoverLongPanRise);
    }
}

void initRaceIntroFlyoverLongPan(RaceIntroEffectActor *arg0) {
    arg0->position.x = 0xE6C45F50;
    arg0->position.y = 0xED3C9CFB;
    arg0->position.z = 0xD14CD682;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0x1610;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->stateTimer = 0x1E;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitRaceIntroFlyoverLongPanTrigger);
}

void updateRaceIntroFlyoverShortPanFinal(RaceIntroEffectActor *arg0) {
    approachRaceIntroFlyoverSpinStep(arg0, 0x130);
    approachRaceIntroFlyoverVerticalVelocity(arg0, 0);
    approachRaceIntroFlyoverOrbitRadius(arg0, 0x100000);
    approachRaceIntroFlyoverPitchVelocity(arg0, 0x18);
    updateRaceIntroFlyoverActor(arg0);
}

void initRaceIntroFlyoverShortPanFinal(RaceIntroEffectActor *arg0) {
    arg0->position.x = 0xF049BD62;
    arg0->position.y = 0xF0E87871;
    arg0->position.z = 0xDA07DE30;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0x1070;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->radius = 0x100000;
    arg0->stateTimer = 0x1E;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceIntroFlyoverShortPanFinal);
}

void waitRaceIntroFlyoverShortPanFinal(RaceIntroEffectActor *arg0) {
    approachRaceIntroFlyoverSpinStep(arg0, 0x130);
    updateRaceIntroFlyoverActor(arg0);
    if (gRacePlayers[0].unk508 == 2) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)initRaceIntroFlyoverShortPanFinal);
    }
}

void initRaceIntroFlyoverShortPanSecond(RaceIntroEffectActor *arg0) {
    arg0->position.x = 0xAB4FC576;
    arg0->position.y = 0xE13FBC73;
    arg0->position.z = 0xD2B26423;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0xDF0;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->stateTimer = 0x1E;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitRaceIntroFlyoverShortPanFinal);
}

void waitRaceIntroFlyoverShortPanSecond(RaceIntroEffectActor *arg0) {
    approachRaceIntroFlyoverSpinStep(arg0, 0x30);
    updateRaceIntroFlyoverActor(arg0);
    if (gRacePlayers[0].unk508 == 1) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)initRaceIntroFlyoverShortPanSecond);
    }
}

void initRaceIntroFlyoverShortPan(RaceIntroEffectActor *arg0) {
    arg0->position.x = 0xAB4FC576;
    arg0->position.y = 0xE0CEDC73;
    arg0->position.z = 0xD2B26423;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0xDF0;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->stateTimer = 0x1E;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitRaceIntroFlyoverShortPanSecond);
}

void initRaceIntroFlyoverActor(RaceIntroEffectActor *arg0) {
    s16 temp_v0 = gRaceCourseIndex;

    if (temp_v0 == 3) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)initRaceIntroFlyoverLongPan);
        temp_v0 = gRaceCourseIndex;
    }
    if (temp_v0 == 6) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)initRaceIntroFlyoverShortPan);
    }
}

void drawRaceIntroAnimatedBillboards(RaceIntroMeshActor *arg0) {
    volatile u8 pad[0xC];
    u32 image;
    u32 palette;
    s16 width;
    s16 height;
    RaceIntroRenderCommandEntry *entry;
    Gfx *gfx;
    s16 textureIndex;
    s16 loadedTextureIndex;
    s32 i;

    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
    entry = gRaceIntroAnimatedBillboardCommandsByCourse[gRaceCourseIndex];
    loadedTextureIndex = -1;
    i = 0;
    if (entry->textureIndex != -1) {
        do {
            if (isPositionNearCurrentRaceViewportCamera(&entry->position) != 0) {
                textureIndex = gRaceIntroAnimatedBillboardTextureIds[entry->textureIndex] + ((s32)(gFrameCounter & 4) / 4);
                if (textureIndex != loadedTextureIndex) {
                    loadedTextureIndex = textureIndex;
                    getAssetTableImagePaletteAndSize((u8 *)getRelocatableHeapBlockBase((s32)ASSET_HANDLE(0x1C)), textureIndex & 0xFFFF, &image, &palette,
                                  &width, &height);
                    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, width, height, 0, G_TX_CLAMP,
                                            G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
                }
                gSPMatrix(gRegionAllocPtr++, &arg0->matrices[i], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gfx = gRegionAllocPtr++;
                gfx->words.w1 = (u32)gRaceIntroAnimatedBillboardVertices;
                gfx->words.w0 = 0x0400103F;
                RACE_INTRO_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
            }
            entry++;
            i++;
        } while (entry->textureIndex != -1);
    }
    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
}

void enqueueDrawRaceIntroAnimatedBillboards(RaceIntroMeshActor *arg0) {
    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)drawRaceIntroAnimatedBillboards, arg0);
}

void initRaceIntroAnimatedBillboards(RaceIntroMeshActor *arg0) {
    RaceIntroRenderCommandEntry *entry;
    s32 count;
    s32 allocSize;
    s32 i;

    entry = gRaceIntroAnimatedBillboardCommandsByCourse[gRaceCourseIndex];
    count = 0;
    if (entry->textureIndex != -1) {
        do {
            count++;
            entry++;
        } while (entry->textureIndex != -1);
    }

    if (count != 0) {
        entry = gRaceIntroAnimatedBillboardCommandsByCourse[gRaceCourseIndex];
        allocSize = count * sizeof(Mtx);
        gAssetHandles[0x28] = allocRelocatableHeapBlock(allocSize);
        arg0->matrices = getRelocatableHeapBlockBase(gAssetHandles[0x28]);

        i = 0;
        if (count > 0) {
            do {
                arg0->matrices[i] = gIdentityMatrix;
                setPackedMatrixTranslation(&arg0->matrices[i], &entry->position);
                i++;
                entry++;
            } while (i != count);
        }
        osWritebackDCache(arg0->matrices, allocSize);
    }

    setCallbackTaskCallback(arg0, (CallbackTaskCallback)enqueueDrawRaceIntroAnimatedBillboards);
}
