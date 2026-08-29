#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/engine/system_runtime.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/engine/relocatable_heap.h"
#include "game/math/spatial_math.h"
#include "game/race/player/race_player_model_renderer.h"

Gfx gRacePlayerShadowRenderSetupDisplayList[] = {
    gsDPPipeSync(),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetRenderMode(G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2),
    gsSPEndDisplayList(),
};

Gfx *gSnowboardDisplayLists[RACE_PLAYER_MODEL_RENDERER_PART_COUNT + 2] = {
    (Gfx *)0x020004B8, (Gfx *)0x02000180, (Gfx *)0x02000828, (Gfx *)0x020004B8, (Gfx *)0x02000180,
    (Gfx *)0x02000828, (Gfx *)0x020004B8, (Gfx *)0x02000180, (Gfx *)0x02000828, (Gfx *)0x02002F68,
    (Gfx *)0x02003408, (Gfx *)0x02003768, (Gfx *)0x02001AE0, (Gfx *)0x02001F80, (Gfx *)0x02002300,
};

Gfx *gGhostSnowboardDisplayLists[RACE_PLAYER_MODEL_RENDERER_PART_COUNT + 2] = {
    (Gfx *)0x02003BC0, (Gfx *)0x02003A48, (Gfx *)0x02003DB0, (Gfx *)0x02003BC0, (Gfx *)0x02003A48,
    (Gfx *)0x02003DB0, (Gfx *)0x02003BC0, (Gfx *)0x02003A48, (Gfx *)0x02003DB0, (Gfx *)0x02004720,
    (Gfx *)0x02004A10, (Gfx *)0x02004B70, (Gfx *)0x02003F68, (Gfx *)0x02004228, (Gfx *)0x020043E8,
};

Gfx *gRacePlayerModelPart0DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02000000, (Gfx *)0x02000000, (Gfx *)0x02000000, (Gfx *)0x02000000, (Gfx *)0x02000000, (Gfx *)0x02000000,
};

Gfx *gRacePlayerModelPart1DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02000068, (Gfx *)0x02000128, (Gfx *)0x020000E0, (Gfx *)0x020001C8, (Gfx *)0x020000E0, (Gfx *)0x020002A0,
};

Gfx *gRacePlayerModelPart2DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x020000D8, (Gfx *)0x02000218, (Gfx *)0x020001D0, (Gfx *)0x02000378, (Gfx *)0x020002D0, (Gfx *)0x02000330,
};

Gfx *gRacePlayerModelPart3DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02000168, (Gfx *)0x02000308, (Gfx *)0x02000338, (Gfx *)0x02000528, (Gfx *)0x020004E0, (Gfx *)0x02000518,
};

Gfx *gRacePlayerModelPart4DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x020001D8, (Gfx *)0x020003F8, (Gfx *)0x02000428, (Gfx *)0x020006D8, (Gfx *)0x020006B0, (Gfx *)0x020005B8,
};

Gfx *gRacePlayerModelPart5DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02000268, (Gfx *)0x020004F0, (Gfx *)0x020004F0, (Gfx *)0x02000888, (Gfx *)0x020008C0, (Gfx *)0x020007B0,
};

Gfx *gRacePlayerModelPart6DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02000540, (Gfx *)0x02000850, (Gfx *)0x020008A0, (Gfx *)0x02000CD8, (Gfx *)0x02000CE0, (Gfx *)0x02000CF0,
};

Gfx *gRacePlayerModelPart7DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02000A28, (Gfx *)0x02000BE8, (Gfx *)0x02000DD8, (Gfx *)0x02001148, (Gfx *)0x02001298, (Gfx *)0x020010E8,
};

Gfx *gRacePlayerModelPart8DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02000AA0, (Gfx *)0x02000D10, (Gfx *)0x02000F50, (Gfx *)0x02001210, (Gfx *)0x02001408, (Gfx *)0x02001170,
};

Gfx *gRacePlayerModelPart9DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02000C48, (Gfx *)0x02000FD0, (Gfx *)0x02001178, (Gfx *)0x020014D0, (Gfx *)0x02001608, (Gfx *)0x02001410,
};

Gfx *gRacePlayerModelPart10DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02000CC0, (Gfx *)0x02001128, (Gfx *)0x020012F0, (Gfx *)0x02001590, (Gfx *)0x02001778, (Gfx *)0x02001498,
};

Gfx *gRacePlayerModelPart11DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02000E68, (Gfx *)0x020013E8, (Gfx *)0x02001508, (Gfx *)0x02001850, (Gfx *)0x02001978, (Gfx *)0x02001738,
};

Gfx *gRacePlayerModelPart12DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x020010C0, (Gfx *)0x02001630, (Gfx *)0x02001760, (Gfx *)0x02001A98, (Gfx *)0x02001BA0, (Gfx *)0x020017C8,
};

Gfx *gRaceGhostPlayerModelPart0DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02001348, (Gfx *)0x02001878, (Gfx *)0x020019B8, (Gfx *)0x02001CE0, (Gfx *)0x02001E08, (Gfx *)0x02001858,
};

Gfx *gRaceGhostPlayerModelPart1DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x020013B0, (Gfx *)0x020019A0, (Gfx *)0x02001A98, (Gfx *)0x02001EA8, (Gfx *)0x02001EE8, (Gfx *)0x02001AF8,
};

Gfx *gRaceGhostPlayerModelPart2DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02001420, (Gfx *)0x02001A90, (Gfx *)0x02001B88, (Gfx *)0x02002058, (Gfx *)0x020020D8, (Gfx *)0x02001B88,
};

Gfx *gRaceGhostPlayerModelPart3DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x020014B0, (Gfx *)0x02001B80, (Gfx *)0x02001CF0, (Gfx *)0x02002208, (Gfx *)0x020022E8, (Gfx *)0x02001D70,
};

Gfx *gRaceGhostPlayerModelPart4DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02001520, (Gfx *)0x02001C70, (Gfx *)0x02001DE0, (Gfx *)0x020023B8, (Gfx *)0x020024B8, (Gfx *)0x02001E10,
};

Gfx *gRaceGhostPlayerModelPart5DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x020015B0, (Gfx *)0x02001D68, (Gfx *)0x02001EA8, (Gfx *)0x02002568, (Gfx *)0x020026C8, (Gfx *)0x02002008,
};

Gfx *gRaceGhostPlayerModelPart6DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02001888, (Gfx *)0x020020C8, (Gfx *)0x02002258, (Gfx *)0x020029B8, (Gfx *)0x02002AE8, (Gfx *)0x02002548,
};

Gfx *gRaceGhostPlayerModelPart7DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02001D70, (Gfx *)0x02002460, (Gfx *)0x02002790, (Gfx *)0x02002E28, (Gfx *)0x020030A0, (Gfx *)0x02002940,
};

Gfx *gRaceGhostPlayerModelPart8DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02001DE8, (Gfx *)0x02002588, (Gfx *)0x02002908, (Gfx *)0x02002EF0, (Gfx *)0x02003210, (Gfx *)0x020029C8,
};

Gfx *gRaceGhostPlayerModelPart9DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02001F90, (Gfx *)0x02002848, (Gfx *)0x02002B30, (Gfx *)0x020031B0, (Gfx *)0x02003410, (Gfx *)0x02002C68,
};

Gfx *gRaceGhostPlayerModelPart10DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02002008, (Gfx *)0x020029A0, (Gfx *)0x02002CA8, (Gfx *)0x02003270, (Gfx *)0x02003580, (Gfx *)0x02002CF0,
};

Gfx *gRaceGhostPlayerModelPart11DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x020021B0, (Gfx *)0x02002C60, (Gfx *)0x02002EC0, (Gfx *)0x02003530, (Gfx *)0x02003780, (Gfx *)0x02002F90,
};

Gfx *gRaceGhostPlayerModelPart12DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    (Gfx *)0x02002408, (Gfx *)0x02002EA8, (Gfx *)0x02003118, (Gfx *)0x02003778, (Gfx *)0x020039A8, (Gfx *)0x02003020,
};

extern void *allocMenuRenderScratch(s32 size);

extern s16 gUiBlinkTimer;
extern Gfx *gRegionAllocPtr;

void drawRacePlayerGroundShadow(RacePlayer *player) {
    s32 i;

    if (gRenderMatricesDirty != 0) {
        player->stateFlags &= ~RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW_READY;
        player->shadowVtx = allocMenuRenderScratch(0x40);
        if (player->shadowVtx == NULL) {
            return;
        }

        for (i = 0; i < RACE_PLAYER_MODEL_RENDERER_PLAYER_COUNT; i++) {
            player->shadowVtx[i].v.ob[0] = (player->markerPoints[i].x - player->markerPoints[0].x) >> 14;
            player->shadowVtx[i].v.ob[1] = (player->markerPoints[i].y - player->markerPoints[0].y) >> 14;
            player->shadowVtx[i].v.ob[2] = (player->markerPoints[i].z - player->markerPoints[0].z) >> 14;
            player->shadowVtx[i].v.flag = 0;
            player->shadowVtx[i].v.cn[0] = 0;
            player->shadowVtx[i].v.cn[1] = 0;
            player->shadowVtx[i].v.cn[2] = 0;
            player->shadowVtx[i].v.cn[3] = 0x30;
        }

        player->shadowMtx = allocMenuRenderScratch(0x100);
        if (player->shadowMtx == NULL) {
            return;
        }

        *player->shadowMtx = gRacePlayerShadowMatrixTemplate;

        player->shadowMtx->m[1][2] =
            (player->markerPoints[0].x & 0xFFFF0000) | (((player->markerPoints[0].y + 0xA000) >> 16) & 0xFFFF);
        player->shadowMtx->m[1][3] = (player->markerPoints[0].z & 0xFFFF0000) | 1;
        player->shadowMtx->m[3][2] =
            ((player->markerPoints[0].x << 16) & 0xFFFF0000) | ((player->markerPoints[0].y + 0xA000) & 0xFFFF);
        player->shadowMtx->m[3][3] = (player->markerPoints[0].z << 16) & 0xFFFF0000;
        player->stateFlags |= RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW_READY;
    }

    if (isPositionNearCurrentRaceViewportCamera(player->markerPoints) != 0 &&
        (player->stateFlags & RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW_READY) != 0) {
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerShadowRenderSetupDisplayList);
        gSPMatrix(gRegionAllocPtr++, player->shadowMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPVertex(gRegionAllocPtr++, player->shadowVtx, RACE_PLAYER_MODEL_RENDERER_PLAYER_COUNT, 0);
        gSP1Quadrangle(gRegionAllocPtr++, 1, 3, 2, 0, 0);
        gSP1Quadrangle(gRegionAllocPtr++, 2, 3, 1, 0, 0);
    }
}

void drawSnowboardModel(void *asset, s16 dlIndex, s16 textureIndex) {
    void *image;
    void *palette;

    gDPPipeSync(gRegionAllocPtr++);
    gSPSegment(gRegionAllocPtr++, 2, getRelocatableHeapBlockBase(gAssetHandles[0xC]));
    gSPMatrix(gRegionAllocPtr++, (Mtx *)asset, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    getAssetTableImageAndPalette(
        (void *)getRelocatableHeapBlockBase(gAssetHandles[0xD]),
        textureIndex,
        &image,
        &palette
    );

    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, 64, 64, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
    gSPDisplayList(gRegionAllocPtr++, gSnowboardDisplayLists[dlIndex]);
}

void drawGhostSnowboardModel(void *asset, s16 dlIndex, s16 textureIndex) {
    void *image;
    void *palette;

    gDPPipeSync(gRegionAllocPtr++);
    gSPSegment(gRegionAllocPtr++, 2, getRelocatableHeapBlockBase(gAssetHandles[0xC]));
    gSPMatrix(gRegionAllocPtr++, (Mtx *)asset, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    getAssetTableImageAndPalette(
        (void *)getRelocatableHeapBlockBase(gAssetHandles[0xD]),
        textureIndex,
        &image,
        &palette
    );

    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, 64, 64, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
    gSPDisplayList(gRegionAllocPtr++, gGhostSnowboardDisplayLists[dlIndex]);
}

void drawRacePlayerModel(RacePlayer *player) {
    RacePlayer *countPlayer;
    RacePlayer *partMatrixPlayer;
    Transform3D *partSource;
    RacePlayer *drawPlayer;
    s32 i;

    countPlayer = player;
    if (gRenderMatricesDirty != 0) {
        player->stateFlags |= RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW;
        i = 0;
        if (player->modelPartCount > 0) {
            partMatrixPlayer = player;
            partSource = player->collisionVolumes;
            do {
                partMatrixPlayer->partMatrices[0] = allocFixedTransformMatrix(partSource);
                if (partMatrixPlayer->partMatrices[0] == NULL) {
                    player->stateFlags &= ~RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW;
                }
                i++;
                // Advance one matrix slot while retaining the source's matching induction-variable shape.
                partMatrixPlayer = (RacePlayer *)((Mtx **)partMatrixPlayer + 1);
                partSource++;
            } while (i < countPlayer->modelPartCount);
        }
    }

    drawPlayer = player;
    if ((drawPlayer->stateFlags & RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW) == 0) {
        return;
    }

    drawSnowboardModel(
        drawPlayer->partMatrices[0],
        drawPlayer->characterVariant,
        drawPlayer->snowboardTextureIndex
    );

    if (drawPlayer->actionSoundTimer != 0) {
        if (drawPlayer->actionSoundTimer < 0xA5 && drawPlayer->actionSoundTimer >= 0x10) {
            return;
        }
        if ((gUiBlinkTimer & 1) != 0) {
            return;
        }
    }

    if ((drawPlayer->stateFlags & RACE_PLAYER_MODEL_RENDERER_FLAG_HIDE_MESHES) == 0) {
        gDPPipeSync(gRegionAllocPtr++);
        gSPSegment(gRegionAllocPtr++, 2, getRelocatableHeapBlockBase(gAssetHandles[drawPlayer->playerIndex + 0xE]));
        gSPSegment(gRegionAllocPtr++, 3, getRelocatableHeapBlockBase(gAssetHandles[drawPlayer->playerIndex + 0x12]));

        gSPMatrix(gRegionAllocPtr++, drawPlayer->partMatrices[1], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart0DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->partMatrices[2], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart1DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->partMatrices[3], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart2DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->partMatrices[4], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart3DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->partMatrices[5], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart4DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->partMatrices[6], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart5DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->partMatrices[7], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart6DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->partMatrices[8], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart7DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->partMatrices[9], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart8DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->partMatrices[10], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart9DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->partMatrices[11], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart10DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->partMatrices[12], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart11DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->partMatrices[13], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart12DisplayLists[drawPlayer->characterId]);
    }
}

void drawRaceGhostPlayerModel(RacePlayer *player) {
    RacePlayer *countPlayer;
    RacePlayer *partMatrixPlayer;
    Transform3D *partSource;
    s32 i;
    s32 alphaPulse;
    Gfx *segmentGfx;

    countPlayer = player;
    if (gRenderMatricesDirty != 0) {
        player->stateFlags |= RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW;
        i = 0;
        if (player->modelPartCount > 0) {
            partMatrixPlayer = player;
            partSource = player->collisionVolumes;
            do {
                // Folded away by IDO, but preserves the target's saved-register allocation.
                if ((partMatrixPlayer && partMatrixPlayer) && partMatrixPlayer) {}
                partMatrixPlayer->partMatrices[0] = allocFixedTransformMatrix(partSource);
                if (partMatrixPlayer->partMatrices[0] == NULL) {
                    player->stateFlags &= ~RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW;
                }
                i++;
                // Advance one matrix slot while retaining the source's matching induction-variable shape.
                partMatrixPlayer = (RacePlayer *)((Mtx **)partMatrixPlayer + 1);
                partSource++;
            } while (i < countPlayer->modelPartCount);
        }
    }

    if ((player->stateFlags & RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW) == 0) {
        return;
    }

    alphaPulse = gUiBlinkTimer & 0x1F;
    if (alphaPulse >= 0x10) {
        alphaPulse = 0x1F - alphaPulse;
    }
    alphaPulse = (alphaPulse * 4) + 0x26;
    gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0, 0, 0, alphaPulse);

    drawGhostSnowboardModel(player->partMatrices[0], player->characterVariant, player->snowboardTextureIndex);

    if ((player->stateFlags & RACE_PLAYER_MODEL_RENDERER_FLAG_HIDE_MESHES) == 0) {
        gDPPipeSync(gRegionAllocPtr++);
        gSPSegment(gRegionAllocPtr++, 2, getRelocatableHeapBlockBase(gAssetHandles[player->playerIndex + 0xE]));
        segmentGfx = gRegionAllocPtr++;
        gSPSegment(segmentGfx, 3, getRelocatableHeapBlockBase(gAssetHandles[player->playerIndex + 0x12]));
        gSPMatrix(gRegionAllocPtr++, player->partMatrices[1], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart0DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->partMatrices[2], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart1DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->partMatrices[3], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart2DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->partMatrices[4], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart3DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->partMatrices[5], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart4DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->partMatrices[6], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart5DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->partMatrices[7], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart6DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->partMatrices[8], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart7DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->partMatrices[9], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart8DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->partMatrices[10], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart9DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->partMatrices[11], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart10DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->partMatrices[12], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart11DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->partMatrices[13], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart12DisplayLists[player->characterId]);
    }
}
