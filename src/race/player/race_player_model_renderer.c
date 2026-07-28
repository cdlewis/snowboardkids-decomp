#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/engine/system_runtime.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/engine/relocatable_heap.h"
#include "game/math/spatial_math.h"
#include "game/race/player/race_player_model_renderer.h"

#define RACE_PLAYER_MODEL_RENDERER_PART_COUNT 13
#define RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS 6
#define RACE_PLAYER_MODEL_RENDERER_PLAYER_PART_CAPACITY 14
#define RACE_PLAYER_MODEL_RENDERER_PLAYER_COUNT 4

#define RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW 0x80
#define RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW_READY 0x100
#define RACE_PLAYER_MODEL_RENDERER_FLAG_HIDE_MESHES 0x400000

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ u16 flag;
    /* 0x08 */ s16 s;
    /* 0x0A */ s16 t;
    /* 0x0C */ u8 r;
    /* 0x0D */ u8 g;
    /* 0x0E */ u8 b;
    /* 0x0F */ u8 a;
} RacePlayerShadowVtx;

typedef struct {
    /* 0x00 */ u32 words[16];
} RacePlayerShadowMtx;

struct RacePlayerModelRenderState {
    /* 0x00 */ u16 playerIndex;
    /* 0x02 */ u8 pad02[0x10 - 0x02];
    /* 0x10 */ u8 textureSet;
    /* 0x11 */ u8 texHeaderIndex;
    /* 0x12 */ u8 textureVariant;
    /* 0x13 */ u8 pad13[0x94 - 0x13];
    /* 0x94 */ u8 partSources[RACE_PLAYER_MODEL_RENDERER_PLAYER_PART_CAPACITY][0x20];
    /* 0x254 */ u8 pad254[0x2FC - 0x254];
    /* 0x2FC */ u32 flags;
    /* 0x300 */ u8 pad300[0x320 - 0x300];
    /* 0x320 */ s16 blinkTimer;
    /* 0x322 */ u8 pad322[0x450 - 0x322];
    /* 0x450 */ s16 partCount;
    /* 0x452 */ u8 pad452[0x468 - 0x452];
    /* 0x468 */ Vec3i shadowPoints[RACE_PLAYER_MODEL_RENDERER_PLAYER_COUNT];
    /* 0x498 */ RacePlayerShadowVtx *shadowVtx;
    /* 0x49C */ RacePlayerShadowMtx *shadowMtx;
    /* 0x4A0 */ u8 pad4A0[0x530 - 0x4A0];
    /* 0x530 */ void *partVtx[RACE_PLAYER_MODEL_RENDERER_PLAYER_PART_CAPACITY];
};

extern void *allocMenuRenderScratch(s32 size);
extern void *allocFixedTransformMatrix(void *source);

extern s16 gUiBlinkTimer;
extern Gfx *gRegionAllocPtr;
extern Gfx gRacePlayerShadowRenderSetupDisplayList[];
extern u32 gRacePlayerShadowMatrixTemplate[];
extern u32 gRacePlayerModelRootPartDisplayLists[RACE_PLAYER_MODEL_RENDERER_PART_COUNT + 2];
extern u32 gRaceGhostPlayerModelRootPartDisplayLists[RACE_PLAYER_MODEL_RENDERER_PART_COUNT + 2];
extern u32 gRacePlayerModelPart0DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRacePlayerModelPart1DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRacePlayerModelPart2DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRacePlayerModelPart3DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRacePlayerModelPart4DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRacePlayerModelPart5DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRacePlayerModelPart6DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRacePlayerModelPart7DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRacePlayerModelPart8DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRacePlayerModelPart9DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRacePlayerModelPart10DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRacePlayerModelPart11DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRacePlayerModelPart12DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRaceGhostPlayerModelPart0DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRaceGhostPlayerModelPart1DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRaceGhostPlayerModelPart2DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRaceGhostPlayerModelPart3DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRaceGhostPlayerModelPart4DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRaceGhostPlayerModelPart5DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRaceGhostPlayerModelPart6DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRaceGhostPlayerModelPart7DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRaceGhostPlayerModelPart8DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRaceGhostPlayerModelPart9DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRaceGhostPlayerModelPart10DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRaceGhostPlayerModelPart11DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];
extern u32 gRaceGhostPlayerModelPart12DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS];

void drawRacePlayerGroundShadow(RacePlayerModelRenderState *player) {
    s32 i;

    if (gRenderMatricesDirty != 0) {
        player->flags &= ~RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW_READY;
        player->shadowVtx = allocMenuRenderScratch(0x40);
        if (player->shadowVtx == NULL) {
            return;
        }

        for (i = 0; i < RACE_PLAYER_MODEL_RENDERER_PLAYER_COUNT; i++) {
            player->shadowVtx[i].x = (player->shadowPoints[i].x - player->shadowPoints[0].x) >> 14;
            player->shadowVtx[i].y = (player->shadowPoints[i].y - player->shadowPoints[0].y) >> 14;
            player->shadowVtx[i].z = (player->shadowPoints[i].z - player->shadowPoints[0].z) >> 14;
            player->shadowVtx[i].flag = 0;
            player->shadowVtx[i].r = 0;
            player->shadowVtx[i].g = 0;
            player->shadowVtx[i].b = 0;
            player->shadowVtx[i].a = 0x30;
        }

        player->shadowMtx = allocMenuRenderScratch(0x100);
        if (player->shadowMtx == NULL) {
            return;
        }

        *player->shadowMtx = *(RacePlayerShadowMtx *)gRacePlayerShadowMatrixTemplate;

        player->shadowMtx->words[6] = (player->shadowPoints[0].x & 0xFFFF0000)
            | (((player->shadowPoints[0].y + 0xA000) >> 16) & 0xFFFF);
        player->shadowMtx->words[7] = (player->shadowPoints[0].z & 0xFFFF0000) | 1;
        player->shadowMtx->words[14] = ((player->shadowPoints[0].x << 16) & 0xFFFF0000)
            | ((player->shadowPoints[0].y + 0xA000) & 0xFFFF);
        player->shadowMtx->words[15] = (player->shadowPoints[0].z << 16) & 0xFFFF0000;
        player->flags |= RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW_READY;
    }

    if (isPositionNearCurrentRaceViewportCamera(player->shadowPoints) != 0
        && (player->flags & RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW_READY) != 0) {
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerShadowRenderSetupDisplayList);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->shadowMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPVertex(gRegionAllocPtr++, (Vtx *)player->shadowVtx, RACE_PLAYER_MODEL_RENDERER_PLAYER_COUNT, 0);
        gSP1Quadrangle(gRegionAllocPtr++, 1, 3, 2, 0, 0);
        gSP1Quadrangle(gRegionAllocPtr++, 2, 3, 1, 0, 0);
    }
}

void drawRacePlayerModelRootPart(void *asset, s16 dlIndex, s16 textureIndex) {
    void *image;
    void *palette;

    gDPPipeSync(gRegionAllocPtr++);
    gSPSegment(gRegionAllocPtr++, 2, getRelocatableHeapBlockBase(gAssetHandles[0xC]));
    gSPMatrix(gRegionAllocPtr++, (Mtx *)asset, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    getAssetTableImageAndPalette((void *)getRelocatableHeapBlockBase(gAssetHandles[0xD]), textureIndex, &image, &palette);

    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, 64, 64, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
    gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelRootPartDisplayLists[dlIndex]);
}

void drawRaceGhostPlayerModelRootPart(void *asset, s16 dlIndex, s16 textureIndex) {
    void *image;
    void *palette;

    gDPPipeSync(gRegionAllocPtr++);
    gSPSegment(gRegionAllocPtr++, 2, getRelocatableHeapBlockBase(gAssetHandles[0xC]));
    gSPMatrix(gRegionAllocPtr++, (Mtx *)asset, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    getAssetTableImageAndPalette((void *)getRelocatableHeapBlockBase(gAssetHandles[0xD]), textureIndex, &image, &palette);

    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, 64, 64, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
    gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelRootPartDisplayLists[dlIndex]);
}

void drawRacePlayerModel(RacePlayerModelRenderState *player) {
    RacePlayerModelRenderState *countPlayer;
    RacePlayerModelRenderState *partVtxPlayer;
    u8 (*partSource)[0x20];
    RacePlayerModelRenderState *drawPlayer;
    s32 i;

    countPlayer = player;
    if (gRenderMatricesDirty != 0) {
        player->flags |= RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW;
        i = 0;
        if (player->partCount > 0) {
            partVtxPlayer = player;
            partSource = player->partSources;
            do {
                partVtxPlayer->partVtx[0] = allocFixedTransformMatrix(partSource);
                if (partVtxPlayer->partVtx[0] == NULL) {
                    player->flags &= ~RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW;
                }
                i++;
                partVtxPlayer = (RacePlayerModelRenderState *)((void **)partVtxPlayer + 1);
                partSource++;
            } while (i < countPlayer->partCount);
        }
    }

    drawPlayer = player;
    if ((drawPlayer->flags & RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW) == 0) {
        return;
    }

    drawRacePlayerModelRootPart(drawPlayer->partVtx[0], drawPlayer->texHeaderIndex, drawPlayer->textureVariant);

    if (drawPlayer->blinkTimer != 0) {
        if (drawPlayer->blinkTimer < 0xA5 && drawPlayer->blinkTimer >= 0x10) {
            return;
        }
        if ((gUiBlinkTimer & 1) != 0) {
            return;
        }
    }

    if ((drawPlayer->flags & RACE_PLAYER_MODEL_RENDERER_FLAG_HIDE_MESHES) == 0) {
        gDPPipeSync(gRegionAllocPtr++);
        gSPSegment(gRegionAllocPtr++, 2, getRelocatableHeapBlockBase(gAssetHandles[drawPlayer->playerIndex + 0xE]));
        gSPSegment(gRegionAllocPtr++, 3, getRelocatableHeapBlockBase(gAssetHandles[drawPlayer->playerIndex + 0x12]));

        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[1], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart0DisplayLists[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[2], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart1DisplayLists[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[3], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart2DisplayLists[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[4], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart3DisplayLists[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[5], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart4DisplayLists[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[6], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart5DisplayLists[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[7], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart6DisplayLists[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[8], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart7DisplayLists[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[9], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart8DisplayLists[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[10], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart9DisplayLists[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[11], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart10DisplayLists[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[12], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart11DisplayLists[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[13], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart12DisplayLists[drawPlayer->textureSet]);
    }
}

void drawRaceGhostPlayerModel(RacePlayerModelRenderState *player) {
    RacePlayerModelRenderState *countPlayer;
    RacePlayerModelRenderState *partVtxPlayer;
    u8 (*partSource)[0x20];
    s32 i;
    s32 alphaPulse;
    Gfx *segmentGfx;

    countPlayer = player;
    if (gRenderMatricesDirty != 0) {
        player->flags |= RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW;
        i = 0;
        if (player->partCount > 0) {
            partVtxPlayer = player;
            partSource = player->partSources;
            do {
                // Folded away by IDO, but preserves the target's saved-register allocation.
                if ((partVtxPlayer && partVtxPlayer) && partVtxPlayer) {
                }
                partVtxPlayer->partVtx[0] = allocFixedTransformMatrix(partSource);
                if (partVtxPlayer->partVtx[0] == NULL) {
                    player->flags &= ~RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW;
                }
                i++;
                partVtxPlayer = (RacePlayerModelRenderState *)((void **)partVtxPlayer + 1);
                partSource++;
            } while (i < countPlayer->partCount);
        }
    }

    if ((player->flags & RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW) == 0) {
        return;
    }

    alphaPulse = gUiBlinkTimer & 0x1F;
    if (alphaPulse >= 0x10) {
        alphaPulse = 0x1F - alphaPulse;
    }
    alphaPulse = (alphaPulse * 4) + 0x26;
    gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0, 0, 0, alphaPulse);

    drawRaceGhostPlayerModelRootPart(player->partVtx[0], player->texHeaderIndex, player->textureVariant);

    if ((player->flags & RACE_PLAYER_MODEL_RENDERER_FLAG_HIDE_MESHES) == 0) {
        gDPPipeSync(gRegionAllocPtr++);
        gSPSegment(gRegionAllocPtr++, 2, getRelocatableHeapBlockBase(gAssetHandles[player->playerIndex + 0xE]));
        segmentGfx = gRegionAllocPtr++;
        gSPSegment(segmentGfx, 3, getRelocatableHeapBlockBase(gAssetHandles[player->playerIndex + 0x12]));
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[1], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart0DisplayLists[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[2], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart1DisplayLists[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[3], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart2DisplayLists[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[4], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart3DisplayLists[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[5], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart4DisplayLists[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[6], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart5DisplayLists[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[7], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart6DisplayLists[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[8], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart7DisplayLists[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[9], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart8DisplayLists[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[10], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart9DisplayLists[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[11], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart10DisplayLists[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[12], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart11DisplayLists[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[13], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart12DisplayLists[player->textureSet]);
    }
}
