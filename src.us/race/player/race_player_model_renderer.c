#include "common.h"
#include "game/engine/model_asset_symbols.h"
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
    SNOWBOARD_MODELS_ROOT_0004B8, SNOWBOARD_MODELS_ROOT_000180, SNOWBOARD_MODELS_ROOT_000828,
    SNOWBOARD_MODELS_ROOT_0004B8, SNOWBOARD_MODELS_ROOT_000180, SNOWBOARD_MODELS_ROOT_000828,
    SNOWBOARD_MODELS_ROOT_0004B8, SNOWBOARD_MODELS_ROOT_000180, SNOWBOARD_MODELS_ROOT_000828,
    SNOWBOARD_MODELS_ROOT_002F68, SNOWBOARD_MODELS_ROOT_003408, SNOWBOARD_MODELS_ROOT_003768,
    SNOWBOARD_MODELS_ROOT_001AE0, SNOWBOARD_MODELS_ROOT_001F80, SNOWBOARD_MODELS_ROOT_002300,
};

Gfx *gGhostSnowboardDisplayLists[RACE_PLAYER_MODEL_RENDERER_PART_COUNT + 2] = {
    SNOWBOARD_MODELS_ROOT_003BC0, SNOWBOARD_MODELS_ROOT_003A48, SNOWBOARD_MODELS_ROOT_003DB0,
    SNOWBOARD_MODELS_ROOT_003BC0, SNOWBOARD_MODELS_ROOT_003A48, SNOWBOARD_MODELS_ROOT_003DB0,
    SNOWBOARD_MODELS_ROOT_003BC0, SNOWBOARD_MODELS_ROOT_003A48, SNOWBOARD_MODELS_ROOT_003DB0,
    SNOWBOARD_MODELS_ROOT_004720, SNOWBOARD_MODELS_ROOT_004A10, SNOWBOARD_MODELS_ROOT_004B70,
    SNOWBOARD_MODELS_ROOT_003F68, SNOWBOARD_MODELS_ROOT_004228, SNOWBOARD_MODELS_ROOT_0043E8,
};

Gfx *gRacePlayerModelPart0DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_000000, WENDY_GRAPHICS_command_000000, JAM_GRAPHICS_command_000000,
    LINDA_GRAPHICS_command_000000, TOMMY_GRAPHICS_command_000000, NINJA_GRAPHICS_command_000000,
};

Gfx *gRacePlayerModelPart1DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_000068, WENDY_GRAPHICS_command_000128, JAM_GRAPHICS_command_0000E0,
    LINDA_GRAPHICS_command_0001C8, TOMMY_GRAPHICS_command_0000E0, NINJA_GRAPHICS_command_0002A0,
};

Gfx *gRacePlayerModelPart2DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_0000D8, WENDY_GRAPHICS_command_000218, JAM_GRAPHICS_command_0001D0,
    LINDA_GRAPHICS_command_000378, TOMMY_GRAPHICS_command_0002D0, NINJA_GRAPHICS_command_000330,
};

Gfx *gRacePlayerModelPart3DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_000168, WENDY_GRAPHICS_command_000308, JAM_GRAPHICS_command_000338,
    LINDA_GRAPHICS_command_000528, TOMMY_GRAPHICS_command_0004E0, NINJA_GRAPHICS_command_000518,
};

Gfx *gRacePlayerModelPart4DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_0001D8, WENDY_GRAPHICS_command_0003F8, JAM_GRAPHICS_command_000428,
    LINDA_GRAPHICS_command_0006D8, TOMMY_GRAPHICS_command_0006B0, NINJA_GRAPHICS_command_0005B8,
};

Gfx *gRacePlayerModelPart5DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_000268, WENDY_GRAPHICS_command_0004F0, JAM_GRAPHICS_command_0004F0,
    LINDA_GRAPHICS_command_000888, TOMMY_GRAPHICS_command_0008C0, NINJA_GRAPHICS_command_0007B0,
};

Gfx *gRacePlayerModelPart6DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_000540, WENDY_GRAPHICS_command_000850, JAM_GRAPHICS_command_0008A0,
    LINDA_GRAPHICS_command_000CD8, TOMMY_GRAPHICS_command_000CE0, NINJA_GRAPHICS_command_000CF0,
};

Gfx *gRacePlayerModelPart7DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_000A28, WENDY_GRAPHICS_command_000BE8, JAM_GRAPHICS_command_000DD8,
    LINDA_GRAPHICS_command_001148, TOMMY_GRAPHICS_command_001298, NINJA_GRAPHICS_command_0010E8,
};

Gfx *gRacePlayerModelPart8DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_000AA0, WENDY_GRAPHICS_command_000D10, JAM_GRAPHICS_command_000F50,
    LINDA_GRAPHICS_command_001210, TOMMY_GRAPHICS_command_001408, NINJA_GRAPHICS_command_001170,
};

Gfx *gRacePlayerModelPart9DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_000C48, WENDY_GRAPHICS_command_000FD0, JAM_GRAPHICS_command_001178,
    LINDA_GRAPHICS_command_0014D0, TOMMY_GRAPHICS_command_001608, NINJA_GRAPHICS_command_001410,
};

Gfx *gRacePlayerModelPart10DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_000CC0, WENDY_GRAPHICS_command_001128, JAM_GRAPHICS_command_0012F0,
    LINDA_GRAPHICS_command_001590, TOMMY_GRAPHICS_command_001778, NINJA_GRAPHICS_command_001498,
};

Gfx *gRacePlayerModelPart11DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_000E68, WENDY_GRAPHICS_command_0013E8, JAM_GRAPHICS_command_001508,
    LINDA_GRAPHICS_command_001850, TOMMY_GRAPHICS_command_001978, NINJA_GRAPHICS_command_001738,
};

Gfx *gRacePlayerModelPart12DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_0010C0, WENDY_GRAPHICS_command_001630, JAM_GRAPHICS_command_001760,
    LINDA_GRAPHICS_command_001A98, TOMMY_GRAPHICS_command_001BA0, NINJA_GRAPHICS_command_0017C8,
};

Gfx *gRaceGhostPlayerModelPart0DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_001348, WENDY_GRAPHICS_command_001878, JAM_GRAPHICS_command_0019B8,
    LINDA_GRAPHICS_command_001CE0, TOMMY_GRAPHICS_command_001E08, NINJA_GRAPHICS_command_001858,
};

Gfx *gRaceGhostPlayerModelPart1DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_0013B0, WENDY_GRAPHICS_command_0019A0, JAM_GRAPHICS_command_001A98,
    LINDA_GRAPHICS_command_001EA8, TOMMY_GRAPHICS_command_001EE8, NINJA_GRAPHICS_command_001AF8,
};

Gfx *gRaceGhostPlayerModelPart2DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_001420, WENDY_GRAPHICS_command_001A90, JAM_GRAPHICS_command_001B88,
    LINDA_GRAPHICS_command_002058, TOMMY_GRAPHICS_command_0020D8, NINJA_GRAPHICS_command_001B88,
};

Gfx *gRaceGhostPlayerModelPart3DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_0014B0, WENDY_GRAPHICS_command_001B80, JAM_GRAPHICS_command_001CF0,
    LINDA_GRAPHICS_command_002208, TOMMY_GRAPHICS_command_0022E8, NINJA_GRAPHICS_command_001D70,
};

Gfx *gRaceGhostPlayerModelPart4DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_001520, WENDY_GRAPHICS_command_001C70, JAM_GRAPHICS_command_001DE0,
    LINDA_GRAPHICS_command_0023B8, TOMMY_GRAPHICS_command_0024B8, NINJA_GRAPHICS_command_001E10,
};

Gfx *gRaceGhostPlayerModelPart5DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_0015B0, WENDY_GRAPHICS_command_001D68, JAM_GRAPHICS_command_001EA8,
    LINDA_GRAPHICS_command_002568, TOMMY_GRAPHICS_command_0026C8, NINJA_GRAPHICS_command_002008,
};

Gfx *gRaceGhostPlayerModelPart6DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_001888, WENDY_GRAPHICS_command_0020C8, JAM_GRAPHICS_command_002258,
    LINDA_GRAPHICS_command_0029B8, TOMMY_GRAPHICS_command_002AE8, NINJA_GRAPHICS_command_002548,
};

Gfx *gRaceGhostPlayerModelPart7DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_001D70, WENDY_GRAPHICS_command_002460, JAM_GRAPHICS_command_002790,
    LINDA_GRAPHICS_command_002E28, TOMMY_GRAPHICS_command_0030A0, NINJA_GRAPHICS_command_002940,
};

Gfx *gRaceGhostPlayerModelPart8DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_001DE8, WENDY_GRAPHICS_command_002588, JAM_GRAPHICS_command_002908,
    LINDA_GRAPHICS_command_002EF0, TOMMY_GRAPHICS_command_003210, NINJA_GRAPHICS_command_0029C8,
};

Gfx *gRaceGhostPlayerModelPart9DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_001F90, WENDY_GRAPHICS_command_002848, JAM_GRAPHICS_command_002B30,
    LINDA_GRAPHICS_command_0031B0, TOMMY_GRAPHICS_command_003410, NINJA_GRAPHICS_command_002C68,
};

Gfx *gRaceGhostPlayerModelPart10DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_002008, WENDY_GRAPHICS_command_0029A0, JAM_GRAPHICS_command_002CA8,
    LINDA_GRAPHICS_command_003270, TOMMY_GRAPHICS_command_003580, NINJA_GRAPHICS_command_002CF0,
};

Gfx *gRaceGhostPlayerModelPart11DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_0021B0, WENDY_GRAPHICS_command_002C60, JAM_GRAPHICS_command_002EC0,
    LINDA_GRAPHICS_command_003530, TOMMY_GRAPHICS_command_003780, NINJA_GRAPHICS_command_002F90,
};

Gfx *gRaceGhostPlayerModelPart12DisplayLists[RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS] = {
    SLASH_GRAPHICS_command_002408, WENDY_GRAPHICS_command_002EA8, JAM_GRAPHICS_command_003118,
    LINDA_GRAPHICS_command_003778, TOMMY_GRAPHICS_command_0039A8, NINJA_GRAPHICS_command_003020,
};

void drawRacePlayerGroundShadow(RacePlayer *player) {
    s32 i;

    if (gRenderMatricesDirty != 0) {
        player->stateFlags &= ~RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW_READY;
        player->shadowVtx = allocMenuRenderScratch(0x40);
        if (player->shadowVtx == NULL) {
            return;
        }

        for (i = 0; i < RACE_PLAYER_SHADOW_VERTEX_COUNT; i++) {
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
        gSPVertex(gRegionAllocPtr++, player->shadowVtx, RACE_PLAYER_SHADOW_VERTEX_COUNT, 0);
        gSP1Quadrangle(gRegionAllocPtr++, 1, 3, 2, 0, 0);
        gSP1Quadrangle(gRegionAllocPtr++, 2, 3, 1, 0, 0);
    }
}

/* SNOWBOARD_TEXTURES declares 64x57 images (e.g. entry 9/palette 9 at
 * +0x4258). The matching renderer loads 64x64 below: the seven extra sampled
 * rows are outside the declared image and must not be treated as authored art.
 * The ghost renderer uses the same load contract. */
void drawSnowboardModel(Mtx *matrix, s16 dlIndex, s16 textureIndex) {
    void *image;
    void *palette;
#ifdef SBK_SNOWBOARD_TEXTURE_SIZE
    s16 width;
    s16 height;
#endif

    gDPPipeSync(gRegionAllocPtr++);
    gSPSegment(gRegionAllocPtr++, 2, getRelocatableHeapBlockBase(gAssetHandles[0xC]));
    gSPMatrix(gRegionAllocPtr++, matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

#ifdef SBK_SNOWBOARD_TEXTURE_SIZE
    getAssetTableImagePaletteAndSize(
        getRelocatableHeapBlockBase(gAssetHandles[0xD]),
        textureIndex,
        &image,
        &palette,
        &width,
        &height
    );
    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, width, height, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
#else
    getAssetTableImageAndPalette(
        (void *)getRelocatableHeapBlockBase(gAssetHandles[0xD]),
        textureIndex,
        &image,
        &palette
    );

    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, 64, 64, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
#endif
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
    gSPDisplayList(gRegionAllocPtr++, gSnowboardDisplayLists[dlIndex]);
}

void drawGhostSnowboardModel(Mtx *matrix, s16 dlIndex, s16 textureIndex) {
    void *image;
    void *palette;
#ifdef SBK_SNOWBOARD_TEXTURE_SIZE
    s16 width;
    s16 height;
#endif

    gDPPipeSync(gRegionAllocPtr++);
    gSPSegment(gRegionAllocPtr++, 2, getRelocatableHeapBlockBase(gAssetHandles[0xC]));
    gSPMatrix(gRegionAllocPtr++, matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
#ifdef SBK_SNOWBOARD_TEXTURE_SIZE
    getAssetTableImagePaletteAndSize(
        getRelocatableHeapBlockBase(gAssetHandles[0xD]),
        textureIndex,
        &image,
        &palette,
        &width,
        &height
    );
    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, width, height, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
#else
    getAssetTableImageAndPalette(
        (void *)getRelocatableHeapBlockBase(gAssetHandles[0xD]),
        textureIndex,
        &image,
        &palette
    );

    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, 64, 64, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
#endif
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
        player->stateFlags |= RACE_PLAYER_MODEL_RENDERER_FLAG_MODEL_MATRICES_READY;
        i = 0;
        if (player->modelPartCount > 0) {
            partMatrixPlayer = player;
            partSource = player->modelPartTransforms;
            do {
                partMatrixPlayer->modelPartMatrices[0] = allocFixedTransformMatrix(partSource);
                if (partMatrixPlayer->modelPartMatrices[0] == NULL) {
                    player->stateFlags &= ~RACE_PLAYER_MODEL_RENDERER_FLAG_MODEL_MATRICES_READY;
                }
                i++;
                // Advance one matrix slot while retaining the source's matching induction-variable shape.
                partMatrixPlayer = (RacePlayer *)((Mtx **)partMatrixPlayer + 1);
                partSource++;
            } while (i < countPlayer->modelPartCount);
        }
    }

    drawPlayer = player;
    if ((drawPlayer->stateFlags & RACE_PLAYER_MODEL_RENDERER_FLAG_MODEL_MATRICES_READY) == 0) {
        return;
    }

    drawSnowboardModel(
        drawPlayer->modelPartMatrices[0],
        drawPlayer->characterVariant,
        drawPlayer->snowboardTextureIndex
    );

    if (drawPlayer->invisibilityTimer != 0) {
        if (drawPlayer->invisibilityTimer < 0xA5 && drawPlayer->invisibilityTimer >= 0x10) {
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

        gSPMatrix(gRegionAllocPtr++, drawPlayer->modelPartMatrices[1], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart0DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->modelPartMatrices[2], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart1DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->modelPartMatrices[3], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart2DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->modelPartMatrices[4], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart3DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->modelPartMatrices[5], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart4DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->modelPartMatrices[6], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart5DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->modelPartMatrices[7], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart6DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->modelPartMatrices[8], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart7DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->modelPartMatrices[9], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart8DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->modelPartMatrices[10], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart9DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->modelPartMatrices[11], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart10DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->modelPartMatrices[12], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRacePlayerModelPart11DisplayLists[drawPlayer->characterId]);
        gSPMatrix(gRegionAllocPtr++, drawPlayer->modelPartMatrices[13], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
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
        player->stateFlags |= RACE_PLAYER_MODEL_RENDERER_FLAG_MODEL_MATRICES_READY;
        i = 0;
        if (player->modelPartCount > 0) {
            partMatrixPlayer = player;
            partSource = player->modelPartTransforms;
            do {
                // Folded away by IDO, but preserves the target's saved-register allocation.
                if ((partMatrixPlayer && partMatrixPlayer) && partMatrixPlayer) {}
                partMatrixPlayer->modelPartMatrices[0] = allocFixedTransformMatrix(partSource);
                if (partMatrixPlayer->modelPartMatrices[0] == NULL) {
                    player->stateFlags &= ~RACE_PLAYER_MODEL_RENDERER_FLAG_MODEL_MATRICES_READY;
                }
                i++;
                // Advance one matrix slot while retaining the source's matching induction-variable shape.
                partMatrixPlayer = (RacePlayer *)((Mtx **)partMatrixPlayer + 1);
                partSource++;
            } while (i < countPlayer->modelPartCount);
        }
    }

    if ((player->stateFlags & RACE_PLAYER_MODEL_RENDERER_FLAG_MODEL_MATRICES_READY) == 0) {
        return;
    }

    alphaPulse = gUiBlinkTimer & 0x1F;
    if (alphaPulse >= 0x10) {
        alphaPulse = 0x1F - alphaPulse;
    }
    alphaPulse = (alphaPulse * 4) + 0x26;
    gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0, 0, 0, alphaPulse);

    drawGhostSnowboardModel(player->modelPartMatrices[0], player->characterVariant, player->snowboardTextureIndex);

    if ((player->stateFlags & RACE_PLAYER_MODEL_RENDERER_FLAG_HIDE_MESHES) == 0) {
        gDPPipeSync(gRegionAllocPtr++);
        gSPSegment(gRegionAllocPtr++, 2, getRelocatableHeapBlockBase(gAssetHandles[player->playerIndex + 0xE]));
        segmentGfx = gRegionAllocPtr++;
        gSPSegment(segmentGfx, 3, getRelocatableHeapBlockBase(gAssetHandles[player->playerIndex + 0x12]));
        gSPMatrix(gRegionAllocPtr++, player->modelPartMatrices[1], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart0DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->modelPartMatrices[2], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart1DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->modelPartMatrices[3], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart2DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->modelPartMatrices[4], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart3DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->modelPartMatrices[5], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart4DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->modelPartMatrices[6], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart5DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->modelPartMatrices[7], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart6DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->modelPartMatrices[8], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart7DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->modelPartMatrices[9], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart8DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->modelPartMatrices[10], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart9DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->modelPartMatrices[11], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart10DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->modelPartMatrices[12], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart11DisplayLists[player->characterId]);
        gSPMatrix(gRegionAllocPtr++, player->modelPartMatrices[13], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, gRaceGhostPlayerModelPart12DisplayLists[player->characterId]);
    }
}
