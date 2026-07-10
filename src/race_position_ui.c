#include "common.h"
#include "memory_allocator.h"
#include "fixed_point_math.h"

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

typedef struct {
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
} RacePositionUiPlayer;

extern void *func_80048594(s32 size);
extern s32 func_8004885C(void *source);
extern void func_80045990(void *asset, u16 index, void **image, void **palette);

extern u8 D_80156609;
extern s16 D_80156612;
extern s16 D_80112148;
extern s16 D_8011214A;
extern s16 D_80112130[];
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
    racePositionUiAppendGfx(0xBC000806, (u32)func_80043040(D_80112148));
    racePositionUiAppendGfx(0x01020040, (u32)asset);
    func_80045990((void *)func_80043040(D_8011214A), textureIndex, &image, &palette);
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
    racePositionUiAppendGfx(0xBC000806, (u32)func_80043040(D_80112130[player->playerIndex + 0xE]));
    racePositionUiAppendGfx(0xBC000C06, (u32)func_80043040(D_80112130[player->playerIndex + 0x12]));

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

    if (D_80156609 != 0) {
        player->flags &= ~RACE_POSITION_UI_FLAG_MARKER_READY;
        player->markerVtx = func_80048594(0x40);
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

        player->markerMtx = func_80048594(0x100);
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

    if (func_80049000(player->markerPoints) != 0 && (player->flags & RACE_POSITION_UI_FLAG_MARKER_READY) != 0) {
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

void func_8007C130(void *asset, s16 dlIndex, u16 textureIndex) {
    void *image;
    void *palette;

    gDPPipeSync(gRegionAllocPtr++);
    gSPSegment(gRegionAllocPtr++, 2, func_80043040(D_80112148));
    gSPMatrix(gRegionAllocPtr++, (Mtx *)asset, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    func_80045990((void *)func_80043040(D_8011214A), textureIndex, &image, &palette);

    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, 64, 64, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
    gSPDisplayList(gRegionAllocPtr++, D_800DE098[dlIndex]);
}

void func_8007C38C(void *asset, s16 dlIndex, u16 textureIndex) {
    void *image;
    void *palette;

    gDPPipeSync(gRegionAllocPtr++);
    gSPSegment(gRegionAllocPtr++, 2, func_80043040(D_80112148));
    gSPMatrix(gRegionAllocPtr++, (Mtx *)asset, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    func_80045990((void *)func_80043040(D_8011214A), textureIndex, &image, &palette);

    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, 64, 64, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
    gSPDisplayList(gRegionAllocPtr++, D_800DE0D4[dlIndex]);
}

#ifdef NON_MATCHING
void func_8007C5E8(RacePositionUiPlayer *player) {
    s32 i;

    if (D_80156609 != 0) {
        player->flags |= RACE_POSITION_UI_FLAG_SHADOW;
        for (i = 0; i < player->partCount; i++) {
            player->partVtx[i] = func_8004885C(player->partSources[i]);
            if (player->partVtx[i] == NULL) {
                player->flags &= ~RACE_POSITION_UI_FLAG_SHADOW;
            }
        }
    }

    if ((player->flags & RACE_POSITION_UI_FLAG_SHADOW) == 0) {
        return;
    }

    func_8007C130(player->partVtx[0], player->texHeaderIndex, player->textureVariant);

    if (player->blinkTimer != 0 && (player->blinkTimer >= 0xA5 || player->blinkTimer < 0x10)) {
        if ((D_80156612 & 1) != 0) {
            return;
        }
    }

    if ((player->flags & RACE_POSITION_UI_FLAG_HIDE_MESHES) == 0) {
        gDPPipeSync(gRegionAllocPtr++);
        gSPSegment(gRegionAllocPtr++, 2, func_80043040(D_80112130[player->playerIndex + 0xE]));
        gSPSegment(gRegionAllocPtr++, 3, func_80043040(D_80112130[player->playerIndex + 0x12]));

        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[1], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE110[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[2], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE128[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[3], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE140[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[4], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE158[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[5], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE170[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[6], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE188[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[7], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE1A0[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[8], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE1B8[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[9], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE1D0[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[10], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE1E8[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[11], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE200[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[12], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE218[player->textureSet]);
        gSPMatrix(gRegionAllocPtr++, (Mtx *)player->partVtx[13], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_800DE230[player->textureSet]);
    }
}
#else
// func_8007C5E8 best match: 97.901%
#pragma GLOBAL_ASM("asm/nonmatchings/race_position_ui/func_8007C5E8.s")
#endif

// func_8007CBC0 best match: 88.342%
#pragma GLOBAL_ASM("asm/nonmatchings/race_position_ui/func_8007CBC0.s")

#ifdef NON_MATCHING
void func_8007CBC0(RacePositionUiPlayer *player) {
    Gfx *gfx;
    Gfx *gfx2;
    Gfx *gfx3;
    RacePositionUiPlayer *countPlayer;
    RacePositionUiPlayer *partVtxPlayer;
    u8 (*partSource)[0x20];
    s32 i;
    s32 alphaPulse;

    countPlayer = player;
    if (D_80156609 != 0) {
        player->flags |= RACE_POSITION_UI_FLAG_SHADOW;
        i = 0;
        if (player->partCount > 0) {
            partVtxPlayer = player;
            partSource = player->partSources;
            do {
                partVtxPlayer->partVtx[0] = func_8004885C(partSource);
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

    alphaPulse = D_80156612 & 0x1F;
    if (alphaPulse >= 0x10) {
        alphaPulse = 0x1F - alphaPulse;
    }
    gfx = gRegionAllocPtr;
    alphaPulse = (alphaPulse * 4) + 0x26;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w0 = 0xFA000000;
    gfx->words.w1 = alphaPulse & 0xFF;

    func_8007C38C(player->partVtx[0], player->texHeaderIndex, player->textureVariant);

    if ((player->flags & RACE_POSITION_UI_FLAG_HIDE_MESHES) == 0) {
        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w1 = 0;
        gfx->words.w0 = 0xE7000000;

        gfx2 = gRegionAllocPtr;
        gRegionAllocPtr = gfx2 + 1;
        gfx2->words.w0 = 0xBC000806;
        gfx2->words.w1 = (u32)func_80043040(D_80112130[player->playerIndex + 0xE]);

        gfx3 = gRegionAllocPtr;
        gRegionAllocPtr = gfx3 + 1;
        gfx3->words.w0 = 0xBC000C06;
        gfx3->words.w1 = (u32)func_80043040(D_80112130[player->playerIndex + 0x12]);

        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x01020040;
        gfx->words.w1 = (u32)player->partVtx[1];
        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x06000000;
        gfx->words.w1 = D_800DE248[player->textureSet];

        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x01020040;
        gfx->words.w1 = (u32)player->partVtx[2];
        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x06000000;
        gfx->words.w1 = D_800DE260[player->textureSet];

        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x01020040;
        gfx->words.w1 = (u32)player->partVtx[3];
        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x06000000;
        gfx->words.w1 = D_800DE278[player->textureSet];

        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x01020040;
        gfx->words.w1 = (u32)player->partVtx[4];
        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x06000000;
        gfx->words.w1 = D_800DE290[player->textureSet];

        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x01020040;
        gfx->words.w1 = (u32)player->partVtx[5];
        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x06000000;
        gfx->words.w1 = D_800DE2A8[player->textureSet];

        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x01020040;
        gfx->words.w1 = (u32)player->partVtx[6];
        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x06000000;
        gfx->words.w1 = D_800DE2C0[player->textureSet];

        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x01020040;
        gfx->words.w1 = (u32)player->partVtx[7];
        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x06000000;
        gfx->words.w1 = D_800DE2D8[player->textureSet];

        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x01020040;
        gfx->words.w1 = (u32)player->partVtx[8];
        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x06000000;
        gfx->words.w1 = D_800DE2F0[player->textureSet];

        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x01020040;
        gfx->words.w1 = (u32)player->partVtx[9];
        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x06000000;
        gfx->words.w1 = D_800DE308[player->textureSet];

        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x01020040;
        gfx->words.w1 = (u32)player->partVtx[10];
        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x06000000;
        gfx->words.w1 = D_800DE320[player->textureSet];

        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x01020040;
        gfx->words.w1 = (u32)player->partVtx[11];
        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x06000000;
        gfx->words.w1 = D_800DE338[player->textureSet];

        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x01020040;
        gfx->words.w1 = (u32)player->partVtx[12];
        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x06000000;
        gfx->words.w1 = D_800DE350[player->textureSet];

        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x01020040;
        gfx->words.w1 = (u32)player->partVtx[13];
        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w0 = 0x06000000;
        gfx->words.w1 = D_800DE368[player->textureSet];
    }
}
#endif
