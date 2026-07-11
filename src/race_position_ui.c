#include "common.h"
#include "relocatable_heap.h"
#include "spatial_math.h"
#include "race_position_ui.h"

#define RACE_POSITION_UI_PART_COUNT 13
#define RACE_POSITION_UI_TEXTURE_VARIANTS 6
#define RACE_POSITION_UI_PLAYER_PART_CAPACITY 14
#define RACE_POSITION_UI_PLAYER_COUNT 4

#define RACE_POSITION_UI_FLAG_SHADOW 0x80
#define RACE_POSITION_UI_FLAG_MARKER_READY 0x100
#define RACE_POSITION_UI_FLAG_HIDE_MESHES 0x400000

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
} RacePositionUiVtx;

typedef struct {
    /* 0x00 */ u32 words[16];
} RacePositionUiMtx;

struct RacePositionUiPlayer {
    /* 0x00 */ u16 playerIndex;
    /* 0x02 */ u8 pad02[0x10 - 0x02];
    /* 0x10 */ u8 textureSet;
    /* 0x11 */ u8 texHeaderIndex;
    /* 0x12 */ u8 textureVariant;
    /* 0x13 */ u8 pad13[0x94 - 0x13];
    /* 0x94 */ u8 partSources[RACE_POSITION_UI_PLAYER_PART_CAPACITY][0x20];
    /* 0x254 */ u8 pad254[0x2FC - 0x254];
    /* 0x2FC */ u32 flags;
    /* 0x300 */ u8 pad300[0x320 - 0x300];
    /* 0x320 */ s16 blinkTimer;
    /* 0x322 */ u8 pad322[0x450 - 0x322];
    /* 0x450 */ s16 partCount;
    /* 0x452 */ u8 pad452[0x468 - 0x452];
    /* 0x468 */ Vec3i markerPoints[RACE_POSITION_UI_PLAYER_COUNT];
    /* 0x498 */ RacePositionUiVtx *markerVtx;
    /* 0x49C */ RacePositionUiMtx *markerMtx;
    /* 0x4A0 */ u8 pad4A0[0x530 - 0x4A0];
    /* 0x530 */ void *partVtx[RACE_POSITION_UI_PLAYER_PART_CAPACITY];
};

extern void *allocMenuRenderScratch(s32 size);
extern s32 allocFixedTransformMatrix(void *source);
extern void getAssetTableImageAndPalette(void *asset, u16 index, void **image, void **palette);

extern u8 gRenderMatricesDirty;
extern s16 gUiBlinkTimer;
extern s16 D_80112148;
extern s16 gShopMenuTextureAssetHandle;
extern s16 gAssetHandles[];
extern Gfx *gRegionAllocPtr;
extern Gfx D_800DE070[];
extern u32 D_800DEE90[];
extern u32 D_800DE098[RACE_POSITION_UI_PART_COUNT + 2];
extern u32 D_800DE0D4[RACE_POSITION_UI_PART_COUNT + 2];
extern u32 D_800DE110[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE128[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE140[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE158[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE170[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE188[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE1A0[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE1B8[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE1D0[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE1E8[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE200[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE218[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE230[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE248[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE260[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE278[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE290[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE2A8[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE2C0[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE2D8[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE2F0[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE308[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE320[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE338[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE350[RACE_POSITION_UI_TEXTURE_VARIANTS];
extern u32 D_800DE368[RACE_POSITION_UI_TEXTURE_VARIANTS];

#ifdef NON_MATCHING
static void racePositionUiAppendGfx(u32 w0, u32 w1) {
    Gfx *gfx = gRegionAllocPtr;

    gRegionAllocPtr = gfx + 1;
    gfx->words.w0 = w0;
    gfx->words.w1 = w1;
}

static void racePositionUiLoadAssetTexture(void *asset, u16 textureIndex, u32 segmentAddress) {
    void *image;
    void *palette;

    racePositionUiAppendGfx(0xE7000000, 0);
    racePositionUiAppendGfx(0xBC000806, (u32)getRelocatableHeapBlockBase(D_80112148));
    racePositionUiAppendGfx(0x01020040, (u32)asset);
    getAssetTableImageAndPalette((void *)getRelocatableHeapBlockBase(gShopMenuTextureAssetHandle), textureIndex, &image, &palette);
    racePositionUiAppendGfx(0xFD500000, (u32)image);
    racePositionUiAppendGfx(0xF5500000, 0x07080200);
    racePositionUiAppendGfx(0xE6000000, 0);
    racePositionUiAppendGfx(0xF3000000, 0x073FF200);
    racePositionUiAppendGfx(0xE7000000, 0);
    racePositionUiAppendGfx(0xF5400800, 0x00080200);
    racePositionUiAppendGfx(0xF2000000, 0x000FC0FC);
    racePositionUiAppendGfx(0xFD100000, (u32)palette);
    racePositionUiAppendGfx(0xE8000000, 0);
    racePositionUiAppendGfx(0xF5000100, 0x07000000);
    racePositionUiAppendGfx(0xE6000000, 0);
    racePositionUiAppendGfx(0xF0000000, 0x0703C000);
    racePositionUiAppendGfx(0xE7000000, 0);
    racePositionUiAppendGfx(0x06000000, segmentAddress);
}

static void racePositionUiDrawParts(RacePositionUiPlayer *player, u32 *textures[RACE_POSITION_UI_PART_COUNT]) {
    s32 i;

    racePositionUiAppendGfx(0xE7000000, 0);
    racePositionUiAppendGfx(0xBC000806, (u32)getRelocatableHeapBlockBase(gAssetHandles[player->playerIndex + 0xE]));
    racePositionUiAppendGfx(0xBC000C06, (u32)getRelocatableHeapBlockBase(gAssetHandles[player->playerIndex + 0x12]));

    for (i = 0; i < RACE_POSITION_UI_PART_COUNT; i++) {
        racePositionUiAppendGfx(0x01020040, (u32)player->partVtx[i + 1]);
        racePositionUiAppendGfx(0x06000000, textures[i][player->textureSet]);
    }
}

void func_8007BE80(RacePositionUiPlayer *player) {
    s32 posOffset;
    u8 vtxOffset;
    s32 vtxFlagOffset;
    s32 endOffset;
    u8 *point;
    RacePositionUiVtx *vtx;

    if (gRenderMatricesDirty != 0) {
        player->flags &= ~RACE_POSITION_UI_FLAG_MARKER_READY;
        player->markerVtx = allocMenuRenderScratch(0x40);
        if (player->markerVtx == NULL) {
            return;
        }

        vtxOffset = 0;
        posOffset = 0;
        point = (u8 *) player;
        endOffset = 0x30;
        do {
            ((RacePositionUiVtx *) ((u8 *) player->markerVtx + vtxOffset))->x =
                (((RacePositionUiPlayer *) point)->markerPoints[0].x - player->markerPoints[0].x) >> 14;
            ((RacePositionUiVtx *) ((u8 *) player->markerVtx + vtxOffset))->y =
                (((RacePositionUiPlayer *) point)->markerPoints[0].y - player->markerPoints[0].y) >> 14;
            ((RacePositionUiVtx *) ((u8 *) player->markerVtx + vtxOffset))->z =
                (((RacePositionUiPlayer *) point)->markerPoints[0].z - player->markerPoints[0].z) >> 14;
            posOffset += sizeof(Vec3i);
            vtxFlagOffset = vtxOffset;
            ((RacePositionUiVtx *) ((u8 *) player->markerVtx + vtxFlagOffset))->flag = 0;
            point += sizeof(Vec3i);
            ((RacePositionUiVtx *) ((u8 *) player->markerVtx + vtxOffset))->r = 0;
            ((RacePositionUiVtx *) ((u8 *) player->markerVtx + vtxOffset))->g = 0;
            ((RacePositionUiVtx *) ((u8 *) player->markerVtx + vtxOffset))->b = 0;
            vtx = (RacePositionUiVtx *) ((u8 *) player->markerVtx + vtxOffset);
            vtxOffset += sizeof(RacePositionUiVtx);
            vtx->a = 0x30;
        } while (posOffset != endOffset);

        player->markerMtx = allocMenuRenderScratch(0x100);
        if (player->markerMtx == NULL) {
            return;
        }

        *player->markerMtx = *(RacePositionUiMtx *) D_800DEE90;

        player->markerMtx->words[6] = (player->markerPoints[0].x & 0xFFFF0000)
            | (((player->markerPoints[0].y + 0xA000) >> 16) & 0xFFFF);
        player->markerMtx->words[7] = (player->markerPoints[0].z & 0xFFFF0000) | 1;
        player->markerMtx->words[14] = ((player->markerPoints[0].x << 16) & 0xFFFF0000)
            | ((player->markerPoints[0].y + 0xA000) & 0xFFFF);
        player->markerMtx->words[15] = (player->markerPoints[0].z << 16) & 0xFFFF0000;
        player->flags |= RACE_POSITION_UI_FLAG_MARKER_READY;
    }

    if (isPositionNearCurrentViewport(player->markerPoints) != 0 && (player->flags & RACE_POSITION_UI_FLAG_MARKER_READY) != 0) {
        racePositionUiAppendGfx(0x06000000, (u32)D_800DE070);
        racePositionUiAppendGfx(0x01020040, (u32)player->markerMtx);
        racePositionUiAppendGfx(0x0400103F, (u32)player->markerVtx);
        racePositionUiAppendGfx(0xB1020604, 0x00020400);
        racePositionUiAppendGfx(0xB1040602, 0x00040200);
    }
}
#else
// func_8007BE80 best match: 97.84% (base_8.c, 376 differences)
#pragma GLOBAL_ASM("asm/nonmatchings/race_position_ui/func_8007BE80.s")
#endif

void func_8007C130(void *asset, s16 dlIndex, s16 textureIndex) {
    void *image;
    void *palette;

    gDPPipeSync(gRegionAllocPtr++);
    gSPSegment(gRegionAllocPtr++, 2, getRelocatableHeapBlockBase(D_80112148));
    gSPMatrix(gRegionAllocPtr++, (Mtx *)asset, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    getAssetTableImageAndPalette((void *)getRelocatableHeapBlockBase(gShopMenuTextureAssetHandle), textureIndex, &image, &palette);

    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, 64, 64, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
    gSPDisplayList(gRegionAllocPtr++, D_800DE098[dlIndex]);
}

void func_8007C38C(void *asset, s16 dlIndex, s16 textureIndex) {
    void *image;
    void *palette;

    gDPPipeSync(gRegionAllocPtr++);
    gSPSegment(gRegionAllocPtr++, 2, getRelocatableHeapBlockBase(D_80112148));
    gSPMatrix(gRegionAllocPtr++, (Mtx *)asset, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    getAssetTableImageAndPalette((void *)getRelocatableHeapBlockBase(gShopMenuTextureAssetHandle), textureIndex, &image, &palette);

    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, 64, 64, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
    gSPDisplayList(gRegionAllocPtr++, D_800DE0D4[dlIndex]);
}

void func_8007C5E8(RacePositionUiPlayer *player) {
    RacePositionUiPlayer *countPlayer;
    RacePositionUiPlayer *partVtxPlayer;
    u8 (*partSource)[0x20];
    RacePositionUiPlayer *drawPlayer;
    s32 i;

    countPlayer = player;
    if (gRenderMatricesDirty != 0) {
        player->flags |= RACE_POSITION_UI_FLAG_SHADOW;
        i = 0;
        if (player->partCount > 0) {
            partVtxPlayer = player;
            partSource = player->partSources;
            do {
                partVtxPlayer->partVtx[0] = allocFixedTransformMatrix(partSource);
                if (partVtxPlayer->partVtx[0] == NULL) {
                    player->flags &= ~RACE_POSITION_UI_FLAG_SHADOW;
                }
                i++;
                partVtxPlayer = (RacePositionUiPlayer *)((void **)partVtxPlayer + 1);
                partSource++;
            } while (i < countPlayer->partCount);
        }
    }

    drawPlayer = player;
    if ((drawPlayer->flags & RACE_POSITION_UI_FLAG_SHADOW) == 0) {
        return;
    }

    func_8007C130(drawPlayer->partVtx[0], drawPlayer->texHeaderIndex, drawPlayer->textureVariant);

    if (drawPlayer->blinkTimer != 0) {
        if (drawPlayer->blinkTimer < 0xA5 && drawPlayer->blinkTimer >= 0x10) {
            return;
        }
        if ((gUiBlinkTimer & 1) != 0) {
            return;
        }
    }

    if ((drawPlayer->flags & RACE_POSITION_UI_FLAG_HIDE_MESHES) == 0) {
        gDPPipeSync(gRegionAllocPtr++);
        gSPSegment(gRegionAllocPtr++, 2, getRelocatableHeapBlockBase(gAssetHandles[drawPlayer->playerIndex + 0xE]));
        gSPSegment(gRegionAllocPtr++, 3, getRelocatableHeapBlockBase(gAssetHandles[drawPlayer->playerIndex + 0x12]));

        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[1], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE110[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[2], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE128[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[3], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE140[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[4], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE158[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[5], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE170[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[6], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE188[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[7], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE1A0[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[8], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE1B8[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[9], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE1D0[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[10], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE1E8[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[11], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE200[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[12], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE218[drawPlayer->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)drawPlayer->partVtx[13], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE230[drawPlayer->textureSet]);
    }
}

// func_8007CBC0 best match: 99.003% (base_7.c, 370 differences)
#pragma GLOBAL_ASM("asm/nonmatchings/race_position_ui/func_8007CBC0.s")

#ifdef NON_MATCHING
void func_8007CBC0(RacePositionUiPlayer *player) {
    RacePositionUiPlayer *countPlayer;
    RacePositionUiPlayer *partVtxPlayer;
    u8 (*partSource)[0x20];
    s32 i;
    s32 alphaPulse;
    s16 partCount;
    Gfx *segmentGfx;

    countPlayer = player;
    if (gRenderMatricesDirty != 0) {
        partCount = player->partCount;
        player->flags |= RACE_POSITION_UI_FLAG_SHADOW;
        i = 0;
        if (partCount > 0) {
            partVtxPlayer = player;
            partSource = player->partSources;
            do {
                partVtxPlayer->partVtx[0] = allocFixedTransformMatrix(partSource);
                if (partVtxPlayer->partVtx[0] == NULL) {
                    player->flags &= ~RACE_POSITION_UI_FLAG_SHADOW;
                }
                i++;
                partVtxPlayer = (RacePositionUiPlayer *)((void **)partVtxPlayer + 1);
                partSource++;
            } while (i < countPlayer->partCount);
        }
    }

    if ((player->flags & RACE_POSITION_UI_FLAG_SHADOW) == 0) {
        return;
    }

    alphaPulse = gUiBlinkTimer & 0x1F;
    if (alphaPulse >= 0x10) {
        alphaPulse = 0x1F - alphaPulse;
    }
    alphaPulse = (alphaPulse * 4) + 0x26;
    gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0, 0, 0, alphaPulse & 0xFF);

    func_8007C38C(player->partVtx[0], player->texHeaderIndex, player->textureVariant);

    if ((player->flags & RACE_POSITION_UI_FLAG_HIDE_MESHES) == 0) {
        gDPPipeSync(gRegionAllocPtr++);
        gSPSegment(gRegionAllocPtr++, 2, getRelocatableHeapBlockBase(gAssetHandles[player->playerIndex + 0xE]));
        segmentGfx = gRegionAllocPtr++;
        gSPSegment(segmentGfx, 3, getRelocatableHeapBlockBase(gAssetHandles[player->playerIndex + 0x12]));
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[1], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE248[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[2], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE260[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[3], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE278[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[4], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE290[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[5], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE2A8[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[6], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE2C0[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[7], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE2D8[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[8], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE2F0[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[9], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE308[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[10], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE320[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[11], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE338[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[12], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE350[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[13], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE368[player->textureSet]);
    }
}
#endif
