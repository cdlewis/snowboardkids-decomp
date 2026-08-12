#include "game/race/race_state.h"
#include "common.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/engine/render_callback.h"
#include "game/engine/system_runtime.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/audio/sound_manager.h"
#include "game/math/spatial_math.h"
#include "game/math/fixed_point_math.h"
#include "game/race/player/race_player_input.h"
#include "game/race/items/race_item_effects.h"

#define ASSET_HANDLE(index) (gAssetHandles[index])
#define RACE_ITEM_GFX_CMD(pkt, cmd0, cmd1) \
    {                                      \
        Gfx *_g = (Gfx *)(pkt);            \
        _g->words.w0 = (cmd0);             \
        _g->words.w1 = (cmd1);             \
    }

typedef union {
    Vec3i vec;
    struct {
        /* 0x18 */ s16 x;
        /* 0x1A */ s16 y;
        /* 0x1C */ u16 frame;
        /* 0x1E */ u8 pad1E[2];
        /* 0x20 */ s8 colorR;
        /* 0x21 */ s8 colorG;
        /* 0x22 */ s8 colorB;
    } sprite;
} RaceItemEffectPayload;

typedef union {
    s32 word;
    void *palette;
    struct {
        /* 0x28 */ s8 unk28;
        /* 0x29 */ s8 phase;
        /* 0x2A */ s8 unk2A;
        /* 0x2B */ s8 unk2B;
    } bytes;
} RaceItemEffectWord28;

typedef union {
    s32 word;
    void *image;
} RaceItemEffectWord2C;

typedef union {
    s32 velocityX;
    s16 timer;
} RaceItemEffectWord24;

typedef union {
    Vec3i vec;
    struct {
        RaceItemEffectWord24 word24;
        RaceItemEffectWord28 word28;
        RaceItemEffectWord2C word2C;
    } fields;
} RaceItemEffectVector24;

typedef union {
    s16 halfword;
    s8 byte;
    u8 ubyte;
    struct {
        /* 0x34 */ s8 byte0;
        /* 0x35 */ s8 matrixDirty;
    } bytes;
} RaceItemEffectState;

typedef union {
    struct {
        /* 0x30 */ s16 x;
        /* 0x32 */ s16 y;
    } screen;
    void *matrix;
    struct {
        /* 0x30 */ s16 drawInitialized;
        /* 0x32 */ s16 delay;
    } particle;
} RaceItemEffectShorts30;

typedef union {
    s16 width;
    s16 alpha;
} RaceItemEffectHalf38;

typedef union {
    s16 halfword;
    s8 byte;
} RaceItemEffectHeight;

typedef union {
    void *matrix;
    struct {
        /* 0x34 */ RaceItemEffectState state;
        /* 0x36 */ RaceItemEffectHeight height;
    } shorts;
} RaceItemEffectWord34;

struct RaceItemEffectActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 playerIndex;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ RaceItemEffectPayload payload;
    /* 0x24 */ RaceItemEffectVector24 vector24;
    /* 0x30 */ RaceItemEffectShorts30 unk30;
    /* 0x34 */ RaceItemEffectWord34 unk34;
    /* 0x38 */ RaceItemEffectHalf38 unk38;
    /* 0x3A */ u8 pad3A[2];
    /* 0x3C */ void *image;
    /* 0x40 */ void *palette;
    /* 0x44 */ u8 pad44[9];
    /* 0x4D */ u8 angleIndex;
    /* 0x4E */ u8 pad4E;
    /* 0x4F */ u8 followPlayerIndex;
    /* 0x50 */ u8 pad50[0x64 - 0x50];
    /* 0x64 */ s16 unk64;
    /* 0x66 */ u8 pad66[2];
    /* 0x68 */ u8 *unk68;
};

struct RaceItemFollowActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 playerIndex;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ Vec3i pos1;
    /* 0x24 */ Vec3i pos2;
    /* 0x30 */ Vec3i offset1;
    /* 0x3C */ Vec3i offset2;
    /* 0x48 */ void *matrix1;
    /* 0x4C */ void *matrix2;
    /* 0x50 */ s8 dirty;
    /* 0x51 */ s8 timer;
};

typedef struct RaceItemDrawNode {
    /* 0x00 */ struct RaceItemDrawNode *next;
    /* 0x04 */ Vec3i *pos;
    /* 0x08 */ u32 displayList;
    /* 0x0C */ void *matrix;
    /* 0x10 */ u8 matrixDirty;
} RaceItemDrawNode;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 playerIndex;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ Vec3i payloads[2];
    /* 0x30 */ u8 pad30[0xC];
    /* 0x3C */ RaceItemDrawNode drawNodes[2];
    /* 0x64 */ s16 timer;
    /* 0x66 */ u8 pad66[2];
    /* 0x68 */ u8 *frameSequence;
} RaceItemSparkBurstActor;

typedef struct {
    /* 0x00 */ RaceItemDrawNode *heads[4];
} RaceItemDrawLists;

struct RaceItemTextureActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ void *images[4];
    /* 0x28 */ void *palettes[4];
};

typedef union {
    s64 value;
    struct {
        /* 0x0 */ s32 high;
        /* 0x4 */ u32 low;
    } word;
} LongLongParts;

extern u8 D_800D4660[4][28];
extern Gfx gRaceItemProjectileQuadVertices[];
extern u32 gAlphaSpriteRenderModeDl[];
extern Gfx gEffectRenderModeSetupDl[];
extern Gfx gEffectRenderModeCleanupDl[];

u8 *gRaceCourseItemEffectTypeTables[10] = {
    D_800D4660[0], D_800D4660[0], D_800D4660[0], D_800D4660[2], D_800D4660[1],
    D_800D4660[3], D_800D4660[0], D_800D4660[0], D_800D4660[0], D_800D4660[0],
};

u16 gRaceItemEffectSpriteIds[8] = { 10, 11, 12, 13, 10, 0, 0, 0 };

Vtx gRaceItemSparkBurstVertices[16] = {
    { { { -1, 2, 0 }, 0, { -16, -16 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { 1, 2, 0 }, 0, { 496, -16 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { 1, 0, 0 }, 0, { 496, 496 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { -1, 0, 0 }, 0, { -16, 496 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { -2, 4, 0 }, 0, { -16, -16 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { 2, 4, 0 }, 0, { 496, -16 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { 2, 0, 0 }, 0, { 496, 496 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { -2, 0, 0 }, 0, { -16, 496 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { -3, 6, 0 }, 0, { -16, -16 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { 3, 6, 0 }, 0, { 496, -16 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { 3, 0, 0 }, 0, { 496, 496 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { -3, 0, 0 }, 0, { -16, 496 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { -4, 8, 0 }, 0, { -16, -16 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { 4, 8, 0 }, 0, { 496, -16 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { 4, 0, 0 }, 0, { 496, 496 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { -4, 0, 0 }, 0, { -16, 496 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
};

u8 gRaceItemSparkBurstSmallFrameSequence[12] = { 0 };
u8 gRaceItemSparkBurstMediumFrameSequence[12] = { 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0 };
u8 gRaceItemSparkBurstLargeFrameSequence[16] = { 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0 };

Vtx gRacePlayerHitEffectQuadVertices[4] = {
    { { { -14, 17, 5 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 14, 17, 5 }, 0, { 1024, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 14, -11, 5 }, 0, { 1024, 1024 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -14, -11, 5 }, 0, { 0, 1024 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

s16 gRacePlayerHitEffectSpriteOffsets[4] = { 14, 18, 22, 26 };

Gfx gRaceItemEffectTranslucentRenderSetupDl[6] = {
    gsDPPipeSync(),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineMode(G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM),
    gsDPSetRenderMode(G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

Vtx gRacePlayerShockEffectQuadVertices[4] = {
    { { { -12, 12, 0 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 12, 12, 0 }, 0, { 496, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 12, -12, 0 }, 0, { 496, 496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -12, -12, 0 }, 0, { -16, 496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

Vtx gRaceItemBreakParticleQuadVertices[4] = {
    { { { -7, 14, 0 }, 0, { -16, -16 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { 7, 14, 0 }, 0, { 496, -16 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { 7, 0, 0 }, 0, { 496, 496 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
    { { { -7, 0, 0 }, 0, { -16, 496 }, { 0xE6, 0xE6, 0xE6, 0xFF } } },
};

Vec2s gRaceItemBreakParticleAngles[16] = {
    { 0x155, 0x000 },
    { 0x155, 0x200 },
    { 0x155, 0x400 },
    { 0x155, 0x600 },
    { 0x155, 0x800 },
    { 0x155, 0xA00 },
    { 0x155, 0xC00 },
    { 0x155, 0xE00 },
    { 0x2AA, 0x000 },
    { 0x2AA, 0x200 },
    { 0x2AA, 0x400 },
    { 0x2AA, 0x600 },
    { 0x2AA, 0x800 },
    { 0x2AA, 0xA00 },
    { 0x2AA, 0xC00 },
    { 0x2AA, 0xE00 },
};

Vec3i gRaceItemBreakParticleSpawnOffset = { 0, 0, -0x40000 };
Vec3i gRaceItemBreakParticleInitialVelocity = { 0, 0, -0x20000 };

Vtx gRacePlayerRecoverySparkleQuadVertices[4] = {
    { { { -5, 5, 0 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 5, 5, 0 }, 0, { 1008, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 5, -5, 0 }, 0, { 1008, 1008 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -5, -5, 0 }, 0, { -16, 1008 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

Vtx gRacePlayerSnowSprayQuadVertices[4] = {
    { { { -4, 4, 0 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 4, 4, 0 }, 0, { 496, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 4, -4, 0 }, 0, { 496, 496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -4, -4, 0 }, 0, { -16, 496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

Vtx gRacePlayerLandingSnowSprayQuadVertices[4] = {
    { { { -4, 4, 0 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 4, 4, 0 }, 0, { 496, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 4, -4, 0 }, 0, { 496, 496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -4, -4, 0 }, 0, { -16, 496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

RaceItemDrawNode *gRaceItemTextureEffectDrawLists[4];
extern RaceItemDrawLists D_801121E0;
extern u8 gRaceUpdatePaused;
extern u8 gCurrentViewportIndex;
extern Gfx *gRegionAllocPtr;

void getAssetTableImageAndExplicitPalette(u8 *, u16, u16, void **, void **);
/* Local 4-arg declaration; see note in callback_task_scheduler.h. */
RaceItemEffectActor *createCallbackTaskWithUserIdPreservingArgs(void *, s32, s32, s32);

s32 getRaceItemEffectType(s32 arg0) {
    u8 *p = gRaceCourseItemEffectTypeTables[gRaceCourseIndex.signedValue];
    return p[arg0];
}

void updateRaceItemSparkBurst(RaceItemEffectActor *arg0) {
    RaceItemSparkBurstActor *actor;
    RaceItemDrawNode **drawList;
    s32 playerIndex;
    s32 i;

    actor = (RaceItemSparkBurstActor *)arg0;

    if (gRaceUpdatePaused == 0) {
        actor->timer++;
        if (actor->timer == 5) {
            removeCallbackTask(arg0);
            return;
        }
    }

    if (actor->timer == 0) {
        actor->timer++;
    }

    playerIndex = actor->playerIndex;
    if (playerIndex >= 4) {
        playerIndex = 0;
    }
    drawList = &gRaceItemTextureEffectDrawLists[playerIndex];

    for (i = 0; i < 2; i++) {
        actor->drawNodes[i].next = *drawList;
        *drawList = &actor->drawNodes[i];
        actor->drawNodes[i].displayList = (u32)&gRaceItemSparkBurstVertices[actor->frameSequence[actor->timer - 1]];
        actor->drawNodes[i].pos = &actor->payloads[i];
    }
}

void initRaceItemSparkBurst(RaceItemEffectActor *arg0) {
    arg0->unk64 = 0;
    updateRaceItemSparkBurst(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceItemSparkBurst);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void spawnRaceItemTrackSparkBurst(Vec3i *arg0, Vec3i *arg1, Vec3i *arg2, Vec3i *arg3, s32 arg4, s16 arg5) {
    volatile u8 pad[0x10];
    LongLongParts total;
    s32 midAX;
    s32 midAY;
    s32 midAZ;
    s32 midBZ;
    s32 midBY;
    s32 midBX;
    s32 delta;
    s32 i;
    s32 random;
    RaceItemSparkBurstActor *actor;
    s16 itemType;
    s64 distX;
    s64 distY;

    itemType = getRaceItemEffectType(arg5);
    if (itemType != 4) {
        midAX = ((arg0->x - arg1->x) / 2) + arg1->x;
        midAY = ((arg0->y - arg1->y) / 2) + arg1->y;
        midAZ = ((arg0->z - arg1->z) / 2) + arg1->z;
        midBX = ((arg2->x - arg3->x) / 2) + arg3->x;
        midBY = ((arg2->y - arg3->y) / 2) + arg3->y;
        midBZ = ((arg2->z - arg3->z) / 2) + arg3->z;

        delta = midAY - midBY;
        distY = (s64)delta * delta; delta = midAX - midBX; distX = (s64)delta * delta; delta = midAZ - midBZ; total.value = (((s64)delta * delta) + distX) + distY;

        if (total.value >= 0x100000000LL) {
            actor = (RaceItemSparkBurstActor *)createCallbackTaskWithUserIdPreservingArgs(
                initRaceItemSparkBurst, 5, 0x32, itemType);
            if (actor != NULL) {
                actor->frameSequence = gRaceItemSparkBurstLargeFrameSequence;
                if (total.value < 0x6400000000LL) {
                    actor->frameSequence = gRaceItemSparkBurstMediumFrameSequence;
                }
                if (total.value < 0x2400000000LL) {
                    actor->frameSequence = gRaceItemSparkBurstSmallFrameSequence;
                }

                for (i = 0; i < 2; i++) {
                    random = randomNextMain() & 0xF;
                    actor->payloads[i].x = (((arg0->x - arg1->x) * random) / 15) + arg1->x;
                    actor->payloads[i].y = (((arg0->y - arg1->y) * random) / 15) + arg1->y;
                    actor->payloads[i].z = (((arg0->z - arg1->z) * random) / 15) + arg1->z;
                }
            }
        }
    }
}
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void renderRacePlayerHitEffect(RaceItemEffectActor *arg0) {
    Transform3D sp88;
    void *sp84;
    void *sp80;
    s32 frame;

 do { if (gRenderMatricesDirty) { sp88 = gIdentityFixedTransform; sp88.translation.x = arg0->payload.vec.x; sp88.translation.y = arg0->payload.vec.y; sp88.translation.z = arg0->payload.vec.z; arg0->unk30.matrix = allocFixedTransformMatrix(&sp88); } if (arg0->unk30.matrix != NULL) { if (isPositionNearCurrentRaceViewportCamera(&arg0->payload.vec) != 0) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xBB000001; _g->words.w1 = 0xFFFFFFFF; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xFC121824; _g->words.w1 = 0xFF33FFFF; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xB900031D; _g->words.w1 = 0x005049D8; } ; getAssetTableImageAndPalette(getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), (frame = gRacePlayerHitEffectSpriteOffsets[arg0->playerIndex] + (arg0->vector24.fields.word24.timer >> 1), 0xFFFF & frame), &sp84, &sp80); { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xFD500000; _g->words.w1 = (u32) sp84; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xF5500000; _g->words.w1 = 0x07080200; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xE6000000; _g->words.w1 = gRacePlayerHitEffectSpriteOffsets[arg0->playerIndex] * 0; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xF3000000; _g->words.w1 = 0x070FF400; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xE7000000; _g->words.w1 = 0; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xF5400400; _g->words.w1 = 0x80200; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xF2000000; _g->words.w1 = 0x7C07C; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xFD100000; _g->words.w1 = (u32) sp80; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xE8000000; _g->words.w1 = 0; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xF5000100; _g->words.w1 = 0x07000000; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xE6000000; _g->words.w1 = 0; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xF0000000; _g->words.w1 = 0x0703C000; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xE7000000; _g->words.w1 = 0; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0x01020040; _g->words.w1 = (u32) arg0->unk30.matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0x01000040; _g->words.w1 = (u32) gViewportMatrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0x0400103F; _g->words.w1 = (u32) gRacePlayerHitEffectQuadVertices; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xB1060402; _g->words.w1 = 0x60200; } ; } } } while (0);
}
// clang-format on

void updateRacePlayerHitEffect(RaceItemEffectActor *arg0) {
    s16 temp_v0;

    if (gRaceUpdatePaused == 0) {
        arg0->vector24.fields.word24.timer++;
        if (arg0->vector24.fields.word24.timer == 8) {
            removeCallbackTask(arg0);
            return;
        }
    }
    temp_v0 = arg0->vector24.fields.word24.timer;
    if (temp_v0 < 0) {
        arg0->vector24.fields.word24.timer = temp_v0 + 1;
    }
    addRenderCallback(&D_801248E0, (RenderCallback)renderRacePlayerHitEffect, arg0);
}

void initRacePlayerHitEffect(RaceItemEffectActor *arg0) {
    char pad2C[4];
    RacePlayer *sp58;
    char pad50[8];
    Transform3D sp30;
    Vec3i sp24;

    arg0->vector24.fields.word24.timer = -1;
    if (arg0->unk34.shorts.state.halfword == 0) {
        sp58 = &gRacePlayers[arg0->unk38.width];
        makeFixedRotationY(sp30.rotation, arg0->unk34.shorts.height.halfword);
        sp24.x = 0;
        sp24.y = arg0->vector24.fields.word28.word;
        sp24.z = arg0->vector24.fields.word2C.word;
        transformVec3iByFixedMatrix(sp30.rotation, &sp24, &arg0->payload.vec);
        arg0->payload.vec.x += sp58->pos.x;
        arg0->payload.vec.y += sp58->pos.y;
        arg0->payload.vec.z += sp58->pos.z;
    }
    updateRacePlayerHitEffect(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRacePlayerHitEffect);
}

void spawnRacePlayerHitEffect(s16 arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4) {
    RaceItemEffectActor *p = createCallbackTaskWithUserIdPreservingArgs(initRacePlayerHitEffect, 5, 0x32, arg2);

    if (p != NULL) {
        p->unk34.shorts.state.halfword = 0;
        p->unk34.shorts.height.halfword = arg1;
        p->unk38.width = arg0;
        p->vector24.fields.word28.word = arg3;
        p->vector24.fields.word2C.word = arg4;
    }
}

void spawnRaceItemImpactEffect(s32 arg0, s32 arg1, s32 arg2, s16 arg3) {
    RaceItemEffectActor *p = createCallbackTaskWithUserIdPreservingArgs(initRacePlayerHitEffect, 5, 2, arg3);

    if (p != NULL) {
        p->payload.vec.x = arg0;
        p->payload.vec.y = arg1;
        p->payload.vec.z = arg2;
        p->unk34.shorts.state.halfword = 1;
    }
}

void renderRaceItemProjectileTrailEffect(RaceItemEffectActor *arg0) {
    volatile u8 padding[4];
    Transform3D sp64;
    Gfx *temp_v0_2;

    if (gRenderMatricesDirty != 0) {
        arg0->unk34.shorts.state.bytes.matrixDirty = 1;
    }

    if ((u8)arg0->unk34.shorts.state.bytes.matrixDirty != 0) {
        arg0->unk34.shorts.state.bytes.matrixDirty = 0;
        sp64 = gIdentityFixedTransform;
        sp64.rotation[MTX_XX] = arg0->unk30.screen.y << 8;
        sp64.rotation[MTX_YY] = arg0->unk30.screen.y << 8;
        sp64.rotation[MTX_ZZ] = arg0->unk30.screen.y << 8;
        sp64.translation.x = arg0->payload.vec.x;
        sp64.translation.y = arg0->payload.vec.y;
        sp64.translation.z = arg0->payload.vec.z;
        arg0->vector24.fields.word24.velocityX = (s32)allocFixedTransformMatrix(&sp64);
    }

    if (arg0->vector24.fields.word24.velocityX != 0) {
        gSPDisplayList(gRegionAllocPtr++, gRaceItemEffectTranslucentRenderSetupDl);
        temp_v0_2 = gRegionAllocPtr++;
        temp_v0_2->words.w0 = 0xFA000000;
        temp_v0_2->words.w1 = (arg0->unk30.screen.x & 0xFF) | ~0xFF;
        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
            arg0->vector24.fields.word2C.image,
            G_IM_FMT_CI,
            16,
            16,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            0,
            0,
            0,
            0
        );
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->vector24.fields.word28.palette);
        gSPMatrix(
            gRegionAllocPtr++,
            (Mtx *)arg0->vector24.fields.word24.velocityX,
            G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW
        );
        gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPVertex(gRegionAllocPtr++, (Vtx *)gRaceItemProjectileQuadVertices, 4, 0);
        gSP1Quadrangle(gRegionAllocPtr++, 2, 1, 0, 3, 3);
    }
}

void updateRaceItemProjectileTrailEffect(RaceItemEffectActor *arg0) {
    if (gRaceUpdatePaused == 0) {
        arg0->unk30.screen.x -= 0x30;
        arg0->unk30.screen.y += 3;
        if (arg0->unk30.screen.x < 0x21) {
            removeCallbackTask(arg0);
            return;
        }
    }
    addRenderCallback(&D_801248EC, (RenderCallback)renderRaceItemProjectileTrailEffect, arg0);
}

void initRaceItemProjectileTrailEffect(RaceItemEffectActor *arg0) {
    arg0->unk30.screen.x = 0xF0;
    arg0->unk30.screen.y = 0x10;
    getAssetTableImageAndPalette(
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x1E)),
        arg0->unk34.shorts.state.ubyte,
        &arg0->vector24.fields.word2C.image,
        &arg0->vector24.fields.word28.palette
    );
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceItemProjectileTrailEffect);
}

void spawnRaceItemProjectileTrailEffect(s32 arg0, s32 arg1, s32 arg2, s16 arg3) {
    RaceItemEffectActor *p =
        createCallbackTaskPreservingArgs((CallbackTaskCallback)initRaceItemProjectileTrailEffect, 0, 2);

    if (p != NULL) {
        p->unk34.shorts.state.byte = arg3;
        p->payload.vec.x = arg0;
        p->payload.vec.y = arg1;
        p->payload.vec.z = arg2;
    }
}

void renderRacePlayerShockEffect(RaceItemEffectActor *arg0) {
    Transform3D sp80;
    void *sp7C;
    void *sp78;
    volatile s32 pad[2];

    if (gRenderMatricesDirty != 0) {
        arg0->unk34.shorts.state.bytes.matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->payload.vec) != 0) {
        if (arg0->unk34.shorts.state.bytes.matrixDirty != 0) {
            arg0->unk34.shorts.state.bytes.matrixDirty = 0;
            sp80 = gIdentityFixedTransform;
            sp80.translation.x = arg0->payload.vec.x;
            sp80.translation.y = arg0->payload.vec.y;
            sp80.translation.z = arg0->payload.vec.z;
            arg0->unk30.matrix = allocFixedTransformMatrix(&sp80);
        }

        do {
            if (arg0->unk30.matrix != NULL) {
                getAssetTableImageAndPalette(
                    getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)),
                    (u16)(((s8)arg0->unk34.shorts.height.byte >> 1) + 0x36),
                    &sp7C,
                    &sp78
                );

                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)gEffectRenderModeSetupDl);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xFD500000, (u32)sp7C);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5500000, 0x07080200);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF3000000, 0x0703F800);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5400200, 0x80200);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF2000000, 0x3C03C);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)sp78);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01020040, (u32)arg0->unk30.matrix);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01000040, (u32)gViewportMatrix);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)gRacePlayerShockEffectQuadVertices);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)gEffectRenderModeCleanupDl);
            }
        } while (0);
    }
}

void updateRacePlayerShockEffect(RaceItemEffectActor *arg0) {
    RacePlayer *player;
    RaceItemEffectActor *actor = arg0;

    if (gRaceUpdatePaused == 0) {
        arg0->unk34.shorts.height.byte++;
        if (arg0->unk34.shorts.height.byte == 6) {
            removeCallbackTask(arg0);
            return;
        }
    }
    if (actor->unk34.shorts.height.byte < 0) {
        actor->unk34.shorts.height.byte = 0;
    }
    transformVec3iByFixedMatrix(gRacePlayers[actor->playerIndex].transform, &actor->vector24.vec, &actor->payload.vec);
    player = &gRacePlayers[actor->playerIndex];
    actor->payload.vec.x += player->itemSpawnOffset.x;
    actor->payload.vec.y += player->itemSpawnOffset.y;
    actor->payload.vec.z += player->itemSpawnOffset.z;
    addRenderCallback(&D_801248C8, (RenderCallback)renderRacePlayerShockEffect, actor);
}

void initRacePlayerShockEffect(RaceItemEffectActor *arg0) {
    RacePlayer *player;

    arg0->unk34.shorts.height.byte = -1;
    arg0->vector24.fields.word2C.word = 0;
    arg0->vector24.fields.word28.word = 0x280000;
    arg0->vector24.fields.word24.velocityX = 0x400000;
    player = &gRacePlayers[arg0->playerIndex];
    if (player->stateFlags & 0x400) {
        arg0->vector24.fields.word24.velocityX = -arg0->vector24.fields.word24.velocityX;
    }
    player = &gRacePlayers[arg0->playerIndex];
    enqueuePositionalSoundEffect(9, (Vec3i *)&player->pos, 0x7F, 0x32);
    updateRacePlayerShockEffect(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRacePlayerShockEffect);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void renderRaceItemBreakParticle(RaceItemEffectActor *arg0) {
    volatile s32 pad0;
    Transform3D sp74;
    volatile u8 padding[0x14];
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
    Gfx *temp_v0_17;
    Gfx *temp_v0_18;

    if (gRenderMatricesDirty != 0) {
        arg0->pad4E = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->payload.vec) != 0) {
        if (arg0->pad4E != 0) {
            arg0->pad4E = 0;
            sp74 = gIdentityFixedTransform;
            sp74.translation.x = arg0->payload.vec.x;
            sp74.translation.y = arg0->payload.vec.y;
            sp74.translation.z = arg0->payload.vec.z;
            arg0->unk34.matrix = allocFixedTransformMatrix(&sp74);
        }

        do { if (arg0->unk34.matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gRaceItemEffectTranslucentRenderSetupDl; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFA000000; temp_v0_2->words.w1 = (arg0->unk38.width & 0xFF) | ~0xFF; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xFD500000; temp_v0_3->words.w1 = (u32) arg0->image; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w0 = 0xF5500000; temp_v0_4->words.w1 = 0x07080200; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w1 = 0; temp_v0_5->words.w0 = 0xE6000000; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w0 = 0xF3000000; temp_v0_6->words.w1 = 0x0703F800; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w1 = 0; temp_v0_7->words.w0 = 0xE7000000; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF5400200; temp_v0_8->words.w1 = 0x00080200; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xF2000000; temp_v0_9->words.w1 = 0x0003C03C; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w0 = 0xFD100000; temp_v0_10->words.w1 = (u32) arg0->palette; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w1 = 0; temp_v0_11->words.w0 = 0xE8000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w0 = 0xF5000100; temp_v0_12->words.w1 = 0x07000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w1 = 0; temp_v0_13->words.w0 = 0xE6000000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w0 = 0xF0000000; temp_v0_14->words.w1 = 0x0703C000; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w1 = 0; temp_v0_17->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->unk34.matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) gViewportMatrix; } ; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0x0400103F; temp_v0_18->words.w1 = (u32) gRaceItemBreakParticleQuadVertices; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xB1060402; temp_v0_2->words.w1 = 0x00060200; } } while (0);
    }
}
// clang-format on

void updateRaceItemBreakParticle(RaceItemEffectActor *arg0) {
    s16 temp_v0;

    if (gRaceUpdatePaused == 0) {
        temp_v0 = arg0->unk30.particle.delay;
        if (temp_v0 != 0) {
            arg0->unk30.particle.delay = temp_v0 - 1;
            return;
        }

        arg0->payload.vec.x += arg0->vector24.fields.word24.velocityX;
        arg0->payload.vec.y += arg0->vector24.fields.word28.word;
        arg0->payload.vec.z += arg0->vector24.fields.word2C.word;
        arg0->vector24.fields.word28.word -= 0x4000;
        arg0->unk38.alpha -= 0x10;
        if (arg0->unk38.alpha <= 0) {
            removeCallbackTask(arg0);
            return;
        }
    }

    if (arg0->unk30.particle.drawInitialized == 0) {
        arg0->unk30.particle.drawInitialized++;
    }
    addRenderCallback(&D_801248EC, (RenderCallback)renderRaceItemBreakParticle, arg0);
}

void initRaceItemBreakParticle(RaceItemEffectActor *arg0) {
    char padTail[8];
    volatile s32 pad0[1];
    s16 sp3C[0x10];
    Vec3i sp30;
    volatile s32 pad1[1];
    RacePlayer *player;
    Vec2s *angles;

    arg0->unk38.width = 0xFF;
    arg0->unk30.screen.y = randomNextMain() & 3;
    getAssetTableImageAndPalette(
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)),
        gRaceItemEffectSpriteIds[arg0->playerIndex],
        &arg0->image,
        &arg0->palette
    );

    player = &gRacePlayers[arg0->followPlayerIndex];
    arg0->payload.vec.x = player->unk28.x;
    arg0->payload.vec.y = player->unk28.y;
    arg0->payload.vec.z = player->unk28.z;

    angles = &gRaceItemBreakParticleAngles[arg0->angleIndex & 0xFF];
    makeFixedRotationXY(sp3C, angles->x, angles->y);
    transformVec3iByFixedMatrix(sp3C, &gRaceItemBreakParticleSpawnOffset, &sp30);

    arg0->payload.vec.x += sp30.x;
    arg0->payload.vec.y += sp30.y;
    arg0->payload.vec.z += sp30.z;

    transformVec3iByFixedMatrix(sp3C, &gRaceItemBreakParticleInitialVelocity, &arg0->vector24.vec);
    if (arg0->playerIndex == 0) {
        arg0->payload.vec.y += 0x60000;
    }

    updateRaceItemBreakParticle(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceItemBreakParticle);
}

void spawnRaceItemBreakParticles(s16 playerIndex, s16 itemIndex) {
    RaceItemEffectActor *actor;
    void *callback;
    s32 itemType;
    s32 angleIndex;

    itemType = getRaceItemEffectType(itemIndex);
    if (itemType != 4) {
        if (itemType == 1) {
            enqueuePositionalSoundEffect(0x12, (Vec3i *)&gRacePlayers[playerIndex].pos, 0x7F, 0x32);
        } else {
            enqueuePositionalSoundEffect(0x11, (Vec3i *)&gRacePlayers[playerIndex].pos, 0x7F, 0x32);
        }

        callback = initRaceItemBreakParticle;
        for (angleIndex = 0; angleIndex != 0x10; angleIndex++) {
            actor = createCallbackTaskWithUserIdPreservingArgs(callback, 5, 2, itemType);
            if (actor != NULL) {
                actor->followPlayerIndex = playerIndex;
                actor->angleIndex = angleIndex;
            }
        }
    }
}

void renderRacePlayerRecoverySparkle(RaceItemEffectActor *arg0) {
    Transform3D sp78;
    void *sp74;
    void *sp70;

    if (gRenderMatricesDirty != 0) {
        arg0->vector24.fields.word28.bytes.unk28 = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->payload.vec) != 0) {
        if (arg0->vector24.fields.word28.bytes.unk28 != 0) {
            arg0->vector24.fields.word28.bytes.unk28 = 0;
            sp78 = gIdentityFixedTransform;
            sp78.translation.x = arg0->payload.vec.x;
            sp78.translation.y = arg0->payload.vec.y;
            sp78.translation.z = arg0->payload.vec.z;
            arg0->vector24.fields.word24.velocityX = (s32)allocFixedTransformMatrix(&sp78);
        }

        if (arg0->vector24.fields.word24.velocityX != 0) {
            getAssetTableImageAndPalette(
                getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)),
                ((s8)arg0->vector24.fields.word28.bytes.phase >> 1) + 0x2F,
                &sp74,
                &sp70
            );

            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)gAlphaSpriteRenderModeDl);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xFD500000, (u32)sp74);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5500000, 0x07080200);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF3000000, 0x070FF400);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5400400, 0x80200);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF2000000, 0x7C07C);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)sp70);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01020040, arg0->vector24.fields.word24.velocityX);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01000040, (u32)gViewportMatrix);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)gRacePlayerRecoverySparkleQuadVertices);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
        }
    }
}

void updateRacePlayerRecoverySparkle(RaceItemEffectActor *arg0) {
    if (gRaceUpdatePaused == 0) {
        arg0->vector24.fields.word28.bytes.phase++;
        if (arg0->vector24.fields.word28.bytes.phase == 0xC) {
            removeCallbackTask(arg0);
            return;
        }
    }
    if (arg0->vector24.fields.word28.bytes.phase < 0) {
        arg0->vector24.fields.word28.bytes.phase = 0;
    }
    addRenderCallback(&D_801248C8, (RenderCallback)renderRacePlayerRecoverySparkle, arg0);
}

void initRacePlayerRecoverySparkle(RaceItemEffectActor *arg0) {
    arg0->vector24.fields.word28.bytes.phase = -1;
    arg0->payload.vec.x = gRacePlayers[arg0->playerIndex].effectPos.x + ((randomNextMain() - 0x80) << 10);
    arg0->payload.vec.y = gRacePlayers[arg0->playerIndex].effectPos.y + ((randomNextMain() - 0x80) << 10);
    arg0->payload.vec.z = gRacePlayers[arg0->playerIndex].effectPos.z + ((randomNextMain() - 0x80) << 10);
    updateRacePlayerRecoverySparkle(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRacePlayerRecoverySparkle);
}

void renderRacePlayerSnowSpray(RaceItemFollowActor *arg0) {
    Transform3D sp90;
    void *sp8C;
    void *sp88;
    volatile s32 pad[2];

    if (gRenderMatricesDirty != 0) {
        arg0->dirty = 1;
    }
    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos1) != 0) {
        if (arg0->dirty != 0) {
            arg0->dirty = 0;
            sp90 = gIdentityFixedTransform;
            sp90.translation.x = arg0->pos1.x;
            sp90.translation.y = arg0->pos1.y;
            sp90.translation.z = arg0->pos1.z;
            arg0->matrix1 = allocFixedTransformMatrix(&sp90);
            sp90.translation.x = arg0->pos2.x;
            sp90.translation.y = arg0->pos2.y;
            sp90.translation.z = arg0->pos2.z;
            arg0->matrix2 = allocFixedTransformMatrix(&sp90);
        }
        if (arg0->matrix2 != NULL) {
            getAssetTableImageAndPalette(
                getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)),
                (u16)((((s8)arg0->timer) >> 2) + 0x39),
                &sp8C,
                &sp88
            );

            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)gAlphaSpriteRenderModeDl);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xFD500000, (u32)sp8C);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5500000, 0x07080200);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF3000000, 0x0703F800);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5400200, 0x80200);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF2000000, 0x3C03C);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)sp88);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01020040, (u32)arg0->matrix1);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01000040, (u32)gViewportMatrix);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)gRacePlayerSnowSprayQuadVertices);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01020040, (u32)arg0->matrix2);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01000040, (u32)gViewportMatrix);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)gRacePlayerSnowSprayQuadVertices);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
        }
    }
}

void updateRacePlayerSnowSpray(RaceItemFollowActor *arg0) {
    RacePlayer *player;
    RaceItemFollowActor *temp_a2 = arg0;

    if (gRaceUpdatePaused == 0) {
        arg0->timer++;
        player = &gRacePlayers[arg0->playerIndex];
        arg0->pos1.x = arg0->offset1.x + player->unk28.x;
        arg0->pos1.y = arg0->offset1.y + player->unk28.y;
        arg0->pos1.z = arg0->offset1.z + player->unk28.z;
        arg0->pos2.x = arg0->offset2.x + player->unk28.x;
        arg0->pos2.y = arg0->offset2.y + player->unk28.y;
        arg0->pos2.z = arg0->offset2.z + player->unk28.z;
        if (arg0->timer == 0x18) {
            removeCallbackTask(arg0);
            return;
        }
    }
    if (temp_a2->timer < 0) {
        temp_a2->timer = 0;
    }
    addRenderCallback(&D_801248C8, (RenderCallback)renderRacePlayerSnowSpray, temp_a2);
}

void initRacePlayerSnowSpray(RaceItemFollowActor *arg0) {
    RacePlayer *player;

    arg0->timer = -1;
    player = &gRacePlayers[arg0->playerIndex];
    if (player->stateFlags & 0x400) {
        arg0->offset1.x = player->groundMarkerSources[0].x - player->unk28.x;
        arg0->offset1.y = player->groundMarkerSources[0].y - player->unk28.y;
        arg0->offset1.z = player->groundMarkerSources[0].z - player->unk28.z;
        arg0->offset2.x = player->groundMarkerSources[2].x - player->unk28.x;
        arg0->offset2.y = player->groundMarkerSources[2].y - player->unk28.y;
        arg0->offset2.z = player->groundMarkerSources[2].z - player->unk28.z;
    } else {
        arg0->offset1.x = player->groundMarkerSources[1].x - player->unk28.x;
        arg0->offset1.y = player->groundMarkerSources[1].y - player->unk28.y;
        arg0->offset1.z = player->groundMarkerSources[1].z - player->unk28.z;
        arg0->offset2.x = player->groundMarkerSources[3].x - player->unk28.x;
        arg0->offset2.y = player->groundMarkerSources[3].y - player->unk28.y;
        arg0->offset2.z = player->groundMarkerSources[3].z - player->unk28.z;
    }
    updateRacePlayerSnowSpray(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRacePlayerSnowSpray);
}

void renderRaceUiSparkle(RaceItemEffectActor *arg0) {
    if ((u8)arg0->payload.sprite.colorR == gCurrentViewportIndex) {
        if ((u8)arg0->payload.sprite.colorG == 0) {
            if ((u8)arg0->payload.sprite.colorB == 0) {
                drawAssetTableSpriteWithExplicitPalette(
                    arg0->payload.sprite.x,
                    arg0->payload.sprite.y,
                    getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)),
                    (arg0->payload.sprite.frame >> 1) + 0x5C,
                    0x1D
                );
            } else {
                drawAssetTableSpriteWithExplicitPalette(
                    arg0->payload.sprite.x,
                    arg0->payload.sprite.y,
                    getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)),
                    (arg0->payload.sprite.frame >> 1) + 0x5C,
                    0x1E
                );
            }
        } else if ((u8)arg0->payload.sprite.colorB == 0) {
            drawScaledAssetTableSpriteWithExplicitPalette(
                (s16)(arg0->payload.sprite.x - 8),
                (s16)(arg0->payload.sprite.y - 8),
                getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)),
                (arg0->payload.sprite.frame >> 1) + 0x5C,
                0x1D,
                1
            );
        } else {
            drawScaledAssetTableSpriteWithExplicitPalette(
                (s16)(arg0->payload.sprite.x - 8),
                (s16)(arg0->payload.sprite.y - 8),
                getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)),
                (arg0->payload.sprite.frame >> 1) + 0x5C,
                0x1E,
                1
            );
        }
    }
}

void updateRaceUiSparkle(RaceItemEffectActor *arg0) {
    u16 temp = arg0->payload.sprite.frame + 1;

    arg0->payload.sprite.frame++;
    if ((((arg0->payload.sprite.frame) + 1) - 1) >= 0x10) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)renderRaceUiSparkle, arg0);
    }
}

void initRaceUiSparkle(RaceItemEffectActor *arg0) {
    arg0->payload.sprite.frame = 0xFFFF;
    updateRaceUiSparkle(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiSparkle);
}

void spawnRaceUiSparkle(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4) {
    RaceItemEffectActor *temp_v0;

    temp_v0 = createCallbackTaskPreservingArgs((CallbackTaskCallback)initRaceUiSparkle, 5, 3);
    if (temp_v0 != NULL) {
        temp_v0->payload.sprite.x = arg0 - 8;
        temp_v0->payload.sprite.y = arg1 - 8;
        temp_v0->payload.sprite.colorR = arg2;
        temp_v0->payload.sprite.colorG = arg3;
        temp_v0->payload.sprite.colorB = arg4;
    }
}

void renderRaceItemTextureEffects(RaceItemTextureActor *arg0) {
    register RaceItemTextureActor *actor;
    s32 i;
    RaceItemDrawNode *node;
    Transform3D transform;

    actor = arg0;
    transform = gIdentityFixedTransform;
    do {
        if (gRenderMatricesDirty != 0) {
            i = 0;
            do {
                node = gRaceItemTextureEffectDrawLists[i++];
                if (node != NULL) {
                    do {
                        node->matrixDirty = 1;
                        node = node->next;
                    } while (node != NULL);
                }
            } while (D_801121E0.heads != &gRaceItemTextureEffectDrawLists[i]);
        }
        gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
        i = 0;
        do {
            node = gRaceItemTextureEffectDrawLists[i];
            if (node != NULL) {
                gDPLoadTextureBlock_4b(
                    gRegionAllocPtr++,
                    actor->images[i],
                    G_IM_FMT_CI,
                    16,
                    16,
                    0,
                    G_TX_CLAMP,
                    G_TX_CLAMP,
                    G_TX_NOMASK,
                    G_TX_NOMASK,
                    G_TX_NOLOD,
                    G_TX_NOLOD
                );
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, actor->palettes[i]);
            }
            if (node != NULL) {
                do {
                    if (isPositionNearCurrentRaceViewportCamera(node->pos) != 0) {
                        if (node->matrixDirty != 0) {
                            node->matrixDirty = 0;
                            transform.translation.x = node->pos->x;
                            transform.translation.y = node->pos->y;
                            transform.translation.z = node->pos->z;
                            node->matrix = allocFixedTransformMatrix(&transform);
                        }
                        gSPMatrix(gRegionAllocPtr++, node->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                        gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                        gSPVertex(gRegionAllocPtr++, node->displayList, 4, 0);
                        gSP2Triangles(gRegionAllocPtr++, 3, 2, 1, 0, 3, 1, 0, 0);
                    }
                    node = node->next;
                } while (node != NULL);
            }
            i++;
        } while (D_801121E0.heads != &gRaceItemTextureEffectDrawLists[i]);
        gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
    } while (0);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void updateRaceItemTextureEffects(RaceItemTextureActor *arg0) {
    s32 i;

    for (i = 0; i < 4; i++) gRaceItemTextureEffectDrawLists[i] = NULL;

    addRenderCallback(&D_801248E0, (RenderCallback)renderRaceItemTextureEffects, arg0);
}
// clang-format on

CLANG_DIAGNOSTIC_PUSH
CLANG_DIAGNOSTIC_IGNORE_SELF_ASSIGN
// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initRaceItemTextureEffects(RaceItemTextureActor *arg0) {
    u16 *var_s0;
    RaceItemTextureActor *callbackActor;
    s32 var_s1;
    volatile short pad;
    void **var_s2;
    volatile s16 *new_var2;
    void **var_s3;
    volatile s16 *var_s4;
    s32 var_s5;
    RaceItemTextureActor *actor;
    void (*new_var)(RaceItemTextureActor *);
    RaceItemTextureActor *var_s6;

    var_s4 = gAssetHandles;
    if (1 != 0) {
    }
    do { var_s0 = gRaceItemEffectSpriteIds; if (1) { } var_s6 = arg0; actor = var_s6; { } var_s1 = 0; var_s2 = actor->images; var_s3 = actor->palettes; if (1) { } arg0 = arg0; if (arg0 && arg0) { } var_s5 = 0x10; if (1) { } new_var = updateRaceItemTextureEffects; callbackActor = actor; do { getAssetTableImageAndPalette((0, getRelocatableHeapBlockBase((new_var2 = var_s4)[0x1C])), *var_s0, var_s2, var_s3); var_s1 += 4; var_s0++; var_s2++; var_s3++; } while (var_s1 != var_s5); } while (0);

    updateRaceItemTextureEffects(callbackActor);
    setCallbackTaskCallback(callbackActor, (CallbackTaskCallback)new_var);
}
// clang-format on
CLANG_DIAGNOSTIC_POP

void renderRacePlayerLandingSnowSpray(RaceItemFollowActor *arg0) {
    Transform3D sp98;
    void *sp94;
    void *sp90;
    volatile s32 pad[2];

    if (gRenderMatricesDirty != 0) {
        arg0->dirty = 1;
    }
    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos1) != 0) {
        if (arg0->dirty != 0) {
            arg0->dirty = 0;
            sp98 = gIdentityFixedTransform;
            sp98.translation.x = arg0->pos1.x;
            sp98.translation.y = arg0->pos1.y;
            sp98.translation.z = arg0->pos1.z;
            arg0->matrix1 = allocFixedTransformMatrix(&sp98);
            sp98.translation.x = arg0->pos2.x;
            sp98.translation.y = arg0->pos2.y;
            sp98.translation.z = arg0->pos2.z;
            arg0->matrix2 = allocFixedTransformMatrix(&sp98);
        }
        if (arg0->matrix2 != NULL) {
            getAssetTableImageAndExplicitPalette(
                (u8 *)getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)),
                (u16)((((s8)arg0->timer) >> 2) + 0x39),
                0x12,
                &sp94,
                &sp90
            );

            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)gAlphaSpriteRenderModeDl);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xFD500000, (u32)sp94);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5500000, 0x07080200);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF3000000, 0x0703F800);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5400200, 0x80200);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF2000000, 0x3C03C);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)sp90);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01020040, (u32)arg0->matrix1);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01000040, (u32)gViewportMatrix);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)gRacePlayerLandingSnowSprayQuadVertices);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01020040, (u32)arg0->matrix2);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01000040, (u32)gViewportMatrix);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)gRacePlayerLandingSnowSprayQuadVertices);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
        }
    }
}

void updateRacePlayerLandingSnowSpray(RaceItemFollowActor *arg0) {
    RacePlayer *player;
    RaceItemFollowActor *temp_a2 = arg0;

    if (gRaceUpdatePaused == 0) {
        arg0->timer++;
        player = &gRacePlayers[arg0->playerIndex];
        arg0->pos1.x = arg0->offset1.x + player->unk28.x;
        arg0->pos1.y = arg0->offset1.y + player->unk28.y;
        arg0->pos1.z = arg0->offset1.z + player->unk28.z;
        arg0->pos2.x = arg0->offset2.x + player->unk28.x;
        arg0->pos2.y = arg0->offset2.y + player->unk28.y;
        arg0->pos2.z = arg0->offset2.z + player->unk28.z;
        if (arg0->timer == 0x18) {
            removeCallbackTask(arg0);
            return;
        }
    }
    if (temp_a2->timer < 0) {
        temp_a2->timer = 0;
    }
    addRenderCallback(&D_801248C8, (RenderCallback)renderRacePlayerLandingSnowSpray, temp_a2);
}

void initRacePlayerLandingSnowSpray(RaceItemFollowActor *arg0) {
    RacePlayer *player;

    arg0->timer = -1;
    player = &gRacePlayers[arg0->playerIndex];
    if (player->stateFlags & 0x400) {
        arg0->offset1.x = player->groundMarkerSources[0].x - player->unk28.x;
        arg0->offset1.y = player->groundMarkerSources[0].y - player->unk28.y;
        arg0->offset1.z = player->groundMarkerSources[0].z - player->unk28.z;
        arg0->offset2.x = player->groundMarkerSources[2].x - player->unk28.x;
        arg0->offset2.y = player->groundMarkerSources[2].y - player->unk28.y;
        arg0->offset2.z = player->groundMarkerSources[2].z - player->unk28.z;
    } else {
        arg0->offset1.x = player->groundMarkerSources[1].x - player->unk28.x;
        arg0->offset1.y = player->groundMarkerSources[1].y - player->unk28.y;
        arg0->offset1.z = player->groundMarkerSources[1].z - player->unk28.z;
        arg0->offset2.x = player->groundMarkerSources[3].x - player->unk28.x;
        arg0->offset2.y = player->groundMarkerSources[3].y - player->unk28.y;
        arg0->offset2.z = player->groundMarkerSources[3].z - player->unk28.z;
    }
    updateRacePlayerLandingSnowSpray(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRacePlayerLandingSnowSpray);
}
