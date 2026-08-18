#include "game/race/race_state.h"
#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/engine/system_runtime.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/math/fixed_point_math.h"
#include "game/engine/viewport_manager.h"
#include "game/race/camera/race_camera.h"

#define FONT_GFX_CMD(pkt, cmd0, cmd1) \
    {                                 \
        Gfx *_g = (Gfx *)(pkt);       \
        _g->words.w0 = (cmd0);        \
        _g->words.w1 = (cmd1);        \
    }

typedef struct {
    /* 0x0 */ s32 imageOffset;
    /* 0x4 */ u16 textureIndex;
    /* 0x6 */ u8 width;
    /* 0x7 */ u8 height;
} AssetTableEntry;

typedef struct AssetTable AssetTable;

struct AssetTable {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 entryCount;
    /* 0x8 */ AssetTableEntry entries[1];
};

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 imageOffset;
    /* 0x0C */ u8 padC[2];
    /* 0x0E */ u8 width;
    /* 0x0F */ u8 height;
} FontTexture;

#define PACKED_MTX_WORD(matrix, index) ((matrix)->m[(index) / 4][(index) % 4])

/* Named components preserve IDO's operand scheduling in the matrix packers. */
typedef struct {
    s16 xx;
    s16 xy;
    s16 xz;
    s16 yx;
    s16 yy;
    s16 yz;
    s16 zx;
    s16 zy;
    s16 zz;
    s16 pad12;
    Vec3i translation;
} Transform3DComponents;

typedef struct {
    s32 w0, w1, w2, w3, w4, w5, w6, w7;
    s32 w8, w9, w10, w11, w12, w13, w14, w15;
} PackedMtxWords;

extern RaceCamera D_801121E0[];
extern s16 gMenuAsciiFontPaletteIndex;
extern s16 gMenuViewportWidth;
extern s16 gMenuViewportHeight;
extern s16 gMenuViewportCenterX;
extern s16 gMenuViewportCenterY;
extern s16 gFrameCounter;
extern Gfx *gRegionAllocPtr;
extern void *gMenuAsciiFontPaletteBase;
u16 D_800D40B0[16] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

extern void *allocMenuRenderScratch(s32);

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initMenuAssetHandles(void)
{
    s16 *end;
    s16 *handle;
    end = &gMenuAsciiFontPaletteIndex;
    do { handle = gAssetHandles; do { end = &gMenuAsciiFontPaletteIndex;
        *handle++ = -1;
        *handle++ = -1;
        *handle++ = -1;
        *handle++ = -1;
    }
    while (handle != end);
    }
    while (0);
    end++;
    end--;
}
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void releaseMenuAssetHandles(void)
{
 do { s16 *handle = &gAssetHandles[7]; do { if ((*handle) != (-1)) { *handle = freeRelocatableHeapBlock(*handle); } handle++; } while (handle != (&gMenuAsciiFontPaletteIndex)); } while (0);
}
// clang-format on

void *resolveAssetTableRelativePointer(void *arg0, u32 arg1) {
    return (void *)((u8 *)arg0 + (arg1 & 0xFFFFFF));
}

void getAssetTableImageAndPalette(void *asset, u16 arg1, void **arg2, void **arg3) {
    AssetTableEntry *temp_v1;
    u8 *temp_v0;
    short idx;
    u8 *arg0 = asset;

    temp_v1 = (AssetTableEntry *)((s32)arg0 + (arg1 * sizeof(AssetTableEntry)));
    temp_v0 = arg0 + (((AssetTable *)arg0)->entryCount * sizeof(AssetTableEntry));
    idx = 1;
    *arg2 = (void *)(arg0 + temp_v1[idx].imageOffset);
    temp_v0 += 8;
    *arg3 = (void *)((temp_v1[idx].textureIndex << 5) + temp_v0);
}

void getAssetTableImageAndExplicitPalette(u8 *arg0, u16 arg1, u16 arg2, void **arg3, void **arg4) {
    u8 *temp_v0;
    AssetTableEntry *temp_v1;

    temp_v1 = (AssetTableEntry *)((s32)arg0 + (arg1 * sizeof(AssetTableEntry)));
    temp_v0 = arg0 + (((AssetTable *)arg0)->entryCount * sizeof(AssetTableEntry));
    *arg3 = (void *)(arg0 + temp_v1[1].imageOffset);
    temp_v0 += 8;
    *arg4 = (void *)((arg2 << 5) + temp_v0);
}

void getAssetTableImagePaletteAndSize(u8 *arg0, u16 arg1, void **arg2, void **arg3, s16 *arg4, s16 *arg5) {
    AssetTableEntry *temp_v1;
    u8 *temp_v0;
    short idx;

    temp_v1 = (AssetTableEntry *)((s32)arg0 + (arg1 * sizeof(AssetTableEntry)));
    temp_v0 = arg0 + (((AssetTable *)arg0)->entryCount * sizeof(AssetTableEntry));
    idx = 1;
    *arg2 = (void *)(arg0 + temp_v1[idx].imageOffset);
    temp_v0 += 8;
    *arg3 = (void *)((temp_v1[idx].textureIndex << 5) + temp_v0);
    *arg4 = temp_v1[idx].width;
    *arg5 = temp_v1[idx].height;
}

void drawAssetTableSprite(s16 x, s16 y, AssetTable *table, u16 entryIndex) {
    AssetTableEntry *entry;
    s32 maxX;
    u8 *paletteBase;
    s32 maxY;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 clipS;
    s32 clipT;
    s32 minY;
    s32 x0;
    s32 minX;
    s32 halfHeight;

    paletteBase = (table->entryCount * sizeof(AssetTableEntry)) + (u8 *)table + sizeof(AssetTableEntry);
    entry = &table->entries[entryIndex];
    x0 = x + gMenuViewportCenterX;
    entry += 0;
    y0 = y + gMenuViewportCenterY;
    x1 = x0 + entry->width;
    y1 = y0 + entry->height;
    clipS = 0;
    clipT = 0;

    maxX = gMenuViewportCenterX + (gMenuViewportWidth / 2);
    if (x0 >= maxX) {
        return;
    }

    halfHeight = gMenuViewportHeight / 2;
    maxY = gMenuViewportCenterY + halfHeight;
    minX = gMenuViewportCenterX - (gMenuViewportWidth / 2);
    if (y0 >= maxY) {
        return;
    }
    if (x1 < minX) {
        return;
    }

    minY = gMenuViewportCenterY - halfHeight;
    if (y1 < minY) {
        return;
    }

    if (x0 < minX) {
        clipS = minX - x0;
        x0 = minX;
    }
    if (y0 < minY) {
        clipT = minY - y0;
        y0 = minY;
    }
    if (x1 >= maxX) {
        x1 = maxX;
    }
    if (y1 >= maxY) {
        y1 = maxY;
    }

    gDPLoadTextureTile_4b(
        gRegionAllocPtr++,
        entry->imageOffset + (u8 *)table,
        G_IM_FMT_CI,
        entry->width,
        entry->height,
        0,
        0,
        entry->width,
        entry->height,
        0,
        G_TX_CLAMP,
        G_TX_CLAMP,
        G_TX_NOMASK,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOLOD
    );
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteBase + (entry->textureIndex << 5));
    gSPTextureRectangle(
        gRegionAllocPtr++,
        x0 << 2,
        y0 << 2,
        x1 << 2,
        y1 << 2,
        G_TX_RENDERTILE,
        clipS << 5,
        clipT << 5,
        0x400,
        0x400
    );
}

/*
 * Some matched callers were compiled with a pre-prototype s32 entry index.
 * Keep that source-level promotion isolated from the canonical u16 definition.
 */
#ifdef __clang__
void drawAssetTableSpriteWideIndex(s16 x, s16 y, AssetTable *table, s32 entryIndex) {
    drawAssetTableSprite(x, y, table, entryIndex);
}
#else
#pragma weak drawAssetTableSpriteWideIndex = drawAssetTableSprite
extern void drawAssetTableSpriteWideIndex(s16 x, s16 y, AssetTable *table, s32 entryIndex);
#endif

void drawPulsingAssetTableSprite(s16 x, s16 y, AssetTable *table, u16 entryIndex) {
    AssetTableEntry *entry;
    s32 maxX;
    u8 *paletteBase;
    s32 maxY;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 clipS;
    s32 clipT;
    s32 minY;
    s32 x0;
    s32 minX;
    s32 halfHeight;
    s32 pulse;

    paletteBase = (table->entryCount * sizeof(AssetTableEntry)) + (u8 *)table + sizeof(AssetTableEntry);
    entry = &table->entries[entryIndex];
    x0 = x + gMenuViewportCenterX;
    entry += 0;
    y0 = y + gMenuViewportCenterY;
    x1 = x0 + entry->width;
    y1 = y0 + entry->height;
    clipS = 0;
    clipT = 0;

    maxX = gMenuViewportCenterX + (gMenuViewportWidth / 2);
    if (x0 >= maxX) {
        return;
    }

    halfHeight = gMenuViewportHeight / 2;
    maxY = gMenuViewportCenterY + halfHeight;
    // The constant branch preserves IDO's register allocation for the clipping bounds.
    if (1) {
        minX = gMenuViewportCenterX - (gMenuViewportWidth / 2);
        if (y0 >= maxY) {
            return;
        }
        if (x1 < minX) {
            return;
        }
    }

    minY = gMenuViewportCenterY - halfHeight;
    if (y1 < minY) {
        return;
    }

    if (x0 < minX) {
        clipS = minX - x0;
        x0 = minX;
    }
    if (y0 < minY) {
        clipT = minY - y0;
        y0 = minY;
    }
    if (x1 >= maxX) {
        x1 = maxX;
    }
    if (y1 >= maxY) {
        y1 = maxY;
    }

    pulse = gFrameCounter & 0x1F;
    if (pulse >= 0x11) {
        pulse = 0x20 - pulse;
    }
    pulse *= 0x10;
    if (pulse >= 0x100) {
        pulse = 0xFF;
    }

    gDPPipeSync(gRegionAllocPtr++);
    gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, pulse, 0xFF);
    gDPLoadTextureTile_4b(
        gRegionAllocPtr++,
        entry->imageOffset + (u8 *)table,
        G_IM_FMT_CI,
        entry->width,
        entry->height,
        0,
        0,
        entry->width,
        entry->height,
        0,
        G_TX_CLAMP,
        G_TX_CLAMP,
        G_TX_NOMASK,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOLOD
    );
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteBase + (entry->textureIndex << 5));
    gSPTextureRectangle(
        gRegionAllocPtr++,
        x0 << 2,
        y0 << 2,
        x1 << 2,
        y1 << 2,
        G_TX_RENDERTILE,
        clipS << 5,
        clipT << 5,
        0x400,
        0x400
    );
    gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
}

void drawAssetTableSpriteWithDefaultPalette(s16 x, s16 y, AssetTable *table, u16 entryIndex) {
    AssetTableEntry *entry;
    s32 maxX;
    s32 minY;
    s32 maxY;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 clipS;
    s32 clipT;
    s32 x0;
    s32 minX;
    s32 halfHeight;

    entry = &table->entries[entryIndex];
    x0 = x + gMenuViewportCenterX;
    entry += 0;
    y0 = y + gMenuViewportCenterY;
    x1 = x0 + entry->width;
    y1 = y0 + entry->height;
    clipS = 0;
    clipT = 0;

    maxX = gMenuViewportCenterX + (gMenuViewportWidth / 2);
    if (x0 >= maxX) {
        return;
    }

    halfHeight = gMenuViewportHeight / 2;
    maxY = gMenuViewportCenterY + halfHeight;
    minX = gMenuViewportCenterX - (gMenuViewportWidth / 2);
    if (y0 >= maxY) {
        return;
    }
    if (x1 < minX) {
        return;
    }

    minY = gMenuViewportCenterY - halfHeight;
    if (y1 < minY) {
        return;
    }

    if (x0 < minX) {
        clipS = minX - x0;
        x0 = minX;
    }
    if (y0 < minY) {
        clipT = minY - y0;
        y0 = minY;
    }
    if (x1 >= maxX) {
        x1 = maxX;
    }
    if (y1 >= maxY) {
        y1 = maxY;
    }

    gDPLoadTextureTile_4b(
        gRegionAllocPtr++,
        entry->imageOffset + (u8 *)table,
        G_IM_FMT_CI,
        entry->width,
        entry->height,
        0,
        0,
        entry->width,
        entry->height,
        0,
        G_TX_CLAMP,
        G_TX_CLAMP,
        G_TX_NOMASK,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOLOD
    );
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, D_800D40B0);
    gSPTextureRectangle(
        gRegionAllocPtr++,
        x0 << 2,
        y0 << 2,
        x1 << 2,
        y1 << 2,
        G_TX_RENDERTILE,
        clipS << 5,
        clipT << 5,
        0x400,
        0x400
    );
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void drawMenuFillRectangle(s16 x, s16 y, s16 width, s16 height, u8 red, u8 green, u8 blue) {
    volatile char pad[0x18];
    s32 minY;
    s32 maxY;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 x0;
    s32 minX;
    s32 halfHeight;
    s32 halfWidth;
    s32 maxX;

    x0 = x + gMenuViewportCenterX;
    y0 = y + gMenuViewportCenterY;
    x1 = x0 + width;
    y1 = y0 + height;

    halfWidth = gMenuViewportWidth / 2; maxX = gMenuViewportCenterX + halfWidth;
    if (x0 < maxX) {
        do {
            halfHeight = gMenuViewportHeight / 2;
        } while (0);
        maxY = gMenuViewportCenterY + halfHeight;
        minX = gMenuViewportCenterX - halfWidth;
        if (y0 < maxY) {
            minY = gMenuViewportCenterY - halfHeight;
            if ((x1 >= minX) && (y1 >= minY)) {
                if (x0 < minX) {
                    x0 = minX;
                }
                if (y0 < minY) {
                    y0 = minY;
                }
                if (x1 >= maxX) {
                    x1 = maxX;
                }
                if (y1 >= maxY) {
                    y1 = maxY;
                }

                gDPPipeSync(gRegionAllocPtr++);
                gDPSetCycleType(gRegionAllocPtr++, G_CYC_FILL);
                gDPSetRenderMode(gRegionAllocPtr++, G_RM_NOOP, G_RM_NOOP2);
                gDPSetFillColor(gRegionAllocPtr++, (GPACK_RGBA5551(red, green, blue, 1) << 16) |
                                                    GPACK_RGBA5551(red, green, blue, 1));
                gDPFillRectangle(gRegionAllocPtr++, x0, y0, x1 - 1, y1 - 1);
                gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
            }
        }
    }
}
// clang-format on

void drawAssetTableSprite8bpp(s16 x, s16 y, AssetTable *table, u16 entryIndex) {
    AssetTableEntry *entry;
    s32 maxX;
    u8 *paletteBase;
    s32 maxY;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 clipS;
    s32 clipT;
    s32 minY;
    s32 x0;
    s32 minX;
    s32 halfHeight;

    paletteBase = (table->entryCount * sizeof(AssetTableEntry)) + (u8 *)table + sizeof(AssetTableEntry);
    entry = &table->entries[entryIndex];
    x0 = x + gMenuViewportCenterX;
    entry += 0;
    y0 = y + gMenuViewportCenterY;
    x1 = x0 + entry->width;
    y1 = y0 + entry->height;
    clipS = 0;
    clipT = 0;

    maxX = gMenuViewportCenterX + (gMenuViewportWidth / 2);
    if (x0 >= maxX) {
        return;
    }

    halfHeight = gMenuViewportHeight / 2;
    maxY = gMenuViewportCenterY + halfHeight;
    minX = gMenuViewportCenterX - (gMenuViewportWidth / 2);
    if (y0 >= maxY) {
        return;
    }
    if (x1 < minX) {
        return;
    }

    minY = gMenuViewportCenterY - halfHeight;
    if (y1 < minY) {
        return;
    }

    if (x0 < minX) {
        clipS = minX - x0;
        x0 = minX;
    }
    if (y0 < minY) {
        clipT = minY - y0;
        y0 = minY;
    }
    if (x1 >= maxX) {
        x1 = maxX;
    }
    if (y1 >= maxY) {
        y1 = maxY;
    }

    gDPLoadTextureTile(
        gRegionAllocPtr++,
        entry->imageOffset + (u8 *)table,
        G_IM_FMT_CI,
        G_IM_SIZ_8b,
        entry->width,
        entry->height,
        0,
        0,
        entry->width,
        entry->height,
        0,
        G_TX_CLAMP,
        G_TX_CLAMP,
        G_TX_NOMASK,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOLOD
    );
    gDPLoadTLUT_pal256(gRegionAllocPtr++, paletteBase + (entry->textureIndex << 5));
    gSPTextureRectangle(
        gRegionAllocPtr++,
        x0 << 2,
        y0 << 2,
        x1 << 2,
        y1 << 2,
        G_TX_RENDERTILE,
        clipS << 5,
        clipT << 5,
        0x400,
        0x400
    );
}

void drawAssetTableSpriteWithExplicitPalette(s16 x, s16 y, AssetTable *table, u16 entryIndex, u16 paletteIndex) {
    AssetTableEntry *entry;
    s32 maxX;
    u8 *paletteBase;
    s32 maxY;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 clipS;
    s32 clipT;
    s32 minY;
    s32 x0;
    s32 minX;
    s32 halfHeight;

    paletteBase = (table->entryCount * sizeof(AssetTableEntry)) + (u8 *)table + sizeof(AssetTableEntry);
    entry = &table->entries[entryIndex];
    x0 = x + gMenuViewportCenterX;
    entry += 0;
    y0 = y + gMenuViewportCenterY;
    x1 = x0 + entry->width;
    y1 = y0 + entry->height;
    clipS = 0;
    clipT = 0;

    maxX = gMenuViewportCenterX + (gMenuViewportWidth / 2);
    if (x0 >= maxX) {
        return;
    }

    halfHeight = gMenuViewportHeight / 2;
    maxY = gMenuViewportCenterY + halfHeight;
    minX = gMenuViewportCenterX - (gMenuViewportWidth / 2);
    if (y0 >= maxY) {
        return;
    }
    if (x1 < minX) {
        return;
    }

    minY = gMenuViewportCenterY - halfHeight;
    if (y1 < minY) {
        return;
    }

    if (x0 < minX) {
        clipS = minX - x0;
        x0 = minX;
    }
    if (y0 < minY) {
        clipT = minY - y0;
        y0 = minY;
    }
    if (x1 >= maxX) {
        x1 = maxX;
    }
    if (y1 >= maxY) {
        y1 = maxY;
    }

    gDPLoadTextureTile_4b(
        gRegionAllocPtr++,
        entry->imageOffset + (u8 *)table,
        G_IM_FMT_CI,
        entry->width,
        entry->height,
        0,
        0,
        entry->width,
        entry->height,
        0,
        G_TX_CLAMP,
        G_TX_CLAMP,
        G_TX_NOMASK,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOLOD
    );
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteBase + (paletteIndex << 5));
    gSPTextureRectangle(
        gRegionAllocPtr++,
        x0 << 2,
        y0 << 2,
        x1 << 2,
        y1 << 2,
        G_TX_RENDERTILE,
        clipS << 5,
        clipT << 5,
        0x400,
        0x400
    );
}

/*
 * Some matched callers were compiled with a pre-prototype s32 entry index.
 * Keep that source-level promotion isolated from the canonical u16 definition.
 */
#ifdef __clang__
void drawAssetTableSpriteWithExplicitPaletteWideIndex(
    s16 x,
    s16 y,
    AssetTable *table,
    s32 entryIndex,
    u16 paletteIndex
) {
    drawAssetTableSpriteWithExplicitPalette(x, y, table, entryIndex, paletteIndex);
}
#else
#pragma weak drawAssetTableSpriteWithExplicitPaletteWideIndex = drawAssetTableSpriteWithExplicitPalette
extern void
drawAssetTableSpriteWithExplicitPaletteWideIndex(s16 x, s16 y, AssetTable *table, s32 entryIndex, u16 paletteIndex);
#endif

// drawScaledAssetTableSprite best match: 99.907%
// (nonmatchings/drawScaledAssetTableSprite-67/base_1.c)
//
// Measured residual: six instruction words differ only by a v1/a0 register swap in
// the initial width/height calculations. Control flow, stack layout, relocations,
// and the complete display-list command stream match.
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_render_utils/drawScaledAssetTableSprite.s")

#ifdef NON_MATCHING
void drawScaledAssetTableSprite(s16 x, s16 y, AssetTable *asset, u16 entryIndex, u16 scale) {
    s32 x0;
    s32 y0;
    u8 *paletteBase;
    s16 spriteWidth;
    s16 spriteHeight;
    s32 pad;
    s32 x1;
    s32 y1;
    s32 clippedS;
    s32 clippedT;
    AssetTableEntry *sprite;

    clippedS = scale;
    clippedT = scale;
    sprite = &asset->entries[entryIndex];
    if (scale < 0) {
        return;
    }
    paletteBase = asset->entryCount * sizeof(AssetTableEntry) + (u8 *)asset->entries;

    x0 = x + gMenuViewportCenterX;
    y0 = y + gMenuViewportCenterY;
    spriteWidth = sprite->width;
    spriteHeight = sprite->height;
    x1 = spriteWidth >> clippedS;
    y1 = spriteHeight >> clippedT;
    x0 = x0 + ((spriteWidth - x1) / 2);
    y0 = y0 + ((spriteHeight - y1) / 2);
    x1 = x1 + x0;
    y1 = y1 + y0;

    clippedS = 0;
    clippedT = 0;

    if (x0 >= gMenuViewportCenterX + (gMenuViewportWidth / 2)) {
        return;
    }
    if (y0 >= gMenuViewportCenterY + (gMenuViewportHeight / 2)) {
        return;
    }
    if (x1 < gMenuViewportCenterX - (gMenuViewportWidth / 2)) {
        return;
    }
    if (y1 < gMenuViewportCenterY - (gMenuViewportHeight / 2)) {
        return;
    }
    if (x0 < gMenuViewportCenterX - (gMenuViewportWidth / 2)) {
        clippedS = (gMenuViewportCenterX - (gMenuViewportWidth / 2)) - x0;
        x0 = gMenuViewportCenterX - (gMenuViewportWidth / 2);
    }
    if (y0 < gMenuViewportCenterY - (gMenuViewportHeight / 2)) {
        clippedT = (gMenuViewportCenterY - (gMenuViewportHeight / 2)) - y0;
        y0 = gMenuViewportCenterY - (gMenuViewportHeight / 2);
    }
    if (x1 >= gMenuViewportCenterX + (gMenuViewportWidth / 2)) {
        x1 = gMenuViewportCenterX + (gMenuViewportWidth / 2);
    }
    if (y1 >= gMenuViewportCenterY + (gMenuViewportHeight / 2)) {
        y1 = gMenuViewportCenterY + (gMenuViewportHeight / 2);
    }

    gDPPipeSync(gRegionAllocPtr++);
    gDPSetTextureFilter(gRegionAllocPtr++, G_TF_AVERAGE);
    gDPLoadTextureTile_4b(gRegionAllocPtr++, sprite->imageOffset + (u8 *)asset,
                          G_IM_FMT_CI, sprite->width, sprite->height,
                          0, 0, sprite->width, sprite->height, 0,
                          G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                          G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0,
                      (sprite->textureIndex << 5) + paletteBase);
    gSPTextureRectangle(gRegionAllocPtr++, x0 << 2, y0 << 2,
                        x1 << 2, y1 << 2, G_TX_RENDERTILE,
                        (clippedS << 5) + 0x10,
                        (clippedT << 5) + 0x10,
                        1 << (scale + 10),
                        1 << (scale + 10));
    gDPPipeSync(gRegionAllocPtr++);
    gDPSetTextureFilter(gRegionAllocPtr++, G_TF_POINT);
    gDPPipeSync(gRegionAllocPtr++);
}
#endif

// drawScaledAssetTableSpriteWithExplicitPalette best match: 97.391%
// (nonmatchings/drawScaledAssetTableSpriteWithExplicitPalette-7181144369148334388/base_17.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_render_utils/drawScaledAssetTableSpriteWithExplicitPalette.s")

#ifdef NON_MATCHING
void drawScaledAssetTableSpriteWithExplicitPalette(
    s16 x,
    s16 y,
    AssetTable *asset,
    volatile u16 entryIndex,
    u16 paletteIndex,
    u16 scale
) {
    s32 x0;
    s32 y0;
    s32 spriteWidth;
    AssetTableEntry *sprite;
    AssetTableEntry *paletteBase;
    u16 textureScale;
    s32 pad;
    s32 x1;
    s32 y1;
    s32 clippedS;
    s32 clippedT;

    if (paletteIndex && paletteIndex) {}
    x0 = x;
    textureScale = scale;
    if (textureScale >= 0) {
        paletteBase = asset->entryCount + asset->entries;
        sprite = (AssetTableEntry *)asset + (0xFFFFu & entryIndex);
        y0 = y + gMenuViewportCenterY;
        {
            s32 spriteHeight;

            spriteWidth = sprite[1].width;
            x0 = x + gMenuViewportCenterX;
            x1 = spriteWidth >> textureScale;
            spriteHeight = sprite[1].height;
            y1 = spriteHeight >> textureScale;
            sprite++;
            x0 = x0 + ((spriteWidth - x1) / 2);
            y0 = y0 + ((spriteHeight - y1) / 2);
            x1 += x0;
            y1 += y0;
        }
        {
            clippedS = 0;
            clippedT = 0;

            if (x0 >= gMenuViewportCenterX + (gMenuViewportWidth / 2)) {
                return;
            }
            textureScale = 2;
            if (y0 >= gMenuViewportCenterY + (gMenuViewportHeight / textureScale)) {
                return;
            }
            if (x1 < gMenuViewportCenterX - (gMenuViewportWidth / 2)) {
                return;
            }
            if (y1 < gMenuViewportCenterY - (gMenuViewportHeight / textureScale)) {
                return;
            }
            if (x0 < gMenuViewportCenterX - (gMenuViewportWidth / 2)) {
                clippedS = (gMenuViewportCenterX - (gMenuViewportWidth / 2)) - x0;
                x0 = gMenuViewportCenterX - (gMenuViewportWidth / 2);
            }
            if (y0 < gMenuViewportCenterY - (gMenuViewportHeight / textureScale)) {
                clippedT = (gMenuViewportCenterY - (gMenuViewportHeight / textureScale)) - y0;
                y0 = gMenuViewportCenterY - (gMenuViewportHeight / textureScale);
            }
            if (x1 >= gMenuViewportCenterX + (gMenuViewportWidth / 2)) {
                x1 = gMenuViewportCenterX + (gMenuViewportWidth / 2);
                if (1) {}
            }
            if (y1 >= gMenuViewportCenterY + (gMenuViewportHeight / textureScale)) {
                y1 = gMenuViewportCenterY + (gMenuViewportHeight / textureScale);
            }

            gDPPipeSync(gRegionAllocPtr++);
            gDPSetTextureFilter(gRegionAllocPtr++, G_TF_AVERAGE);
            gDPLoadTextureTile_4b(
                gRegionAllocPtr++,
                sprite->imageOffset + (u8 *)asset,
                G_IM_FMT_CI,
                sprite->width,
                sprite->height,
                0,
                0,
                sprite->width,
                sprite->height,
                0,
                G_TX_CLAMP,
                G_TX_CLAMP,
                G_TX_NOMASK,
                G_TX_NOMASK,
                G_TX_NOLOD,
                G_TX_NOLOD
            );
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, (u8 *)paletteBase + ((0, paletteIndex) << 5));
            gSPTextureRectangle(
                gRegionAllocPtr++,
                x0 << textureScale,
                y0 << textureScale,
                x1 << textureScale,
                y1 << textureScale,
                G_TX_RENDERTILE,
                (clippedS << 5) + 0x10,
                (clippedT << 5) + 0x10,
                spriteWidth = 1 << (scale + 10),
                spriteWidth
            );
            gDPPipeSync(gRegionAllocPtr++);
            gDPSetTextureFilter(gRegionAllocPtr++, G_TF_POINT);
            gDPPipeSync(gRegionAllocPtr++);
        }
    }
}
#endif

#if 0
void drawScaledAssetTableSpriteWithExplicitPalette(s16 x, s16 y, AssetTable *asset, volatile u16 entryIndex,
                                                   u16 paletteIndex, u16 scale) {
    s32 viewHalfWidth;
    s32 clipLeft;
    s32 clipRight;
    s32 clipTop;
    s32 clipBottom;
    s32 x0;
    s32 y0;
    s32 spriteWidth;
    s32 viewHalfHeight;
    AssetTableEntry *sprite;
    AssetTableEntry *paletteBase;
    u16 textureScale;
    s32 spriteHeight;
    s32 x1;
    s32 y1;
    s32 clippedS;
    s32 clippedT;

    textureScale = scale;
    if (textureScale >= 0) {
        paletteBase = asset->entryCount + asset->entries;
        sprite = (AssetTableEntry *)asset + (0xFFFFu & entryIndex);
        spriteWidth = sprite[1].width;
        x0 = x + gMenuViewportCenterX;
        x1 = spriteWidth >> textureScale;
        spriteHeight = sprite[1].height;
        y0 = y + gMenuViewportCenterY;
        y1 = spriteHeight >> textureScale;
        sprite++;
        x0 = x0 + ((spriteWidth - x1) / 2);
        y0 = y0 + ((spriteHeight - y1) / 2);
        x1 += x0;
        y1 += y0;
        clippedS = 0;
        clippedT = 0;

        viewHalfWidth = gMenuViewportWidth / 2;
        clipRight = gMenuViewportCenterX + viewHalfWidth;
        if (x0 >= clipRight) {
            return;
        }

        viewHalfHeight = (textureScale = 2, gMenuViewportHeight / textureScale);
        clipBottom = gMenuViewportCenterY + viewHalfHeight;
        clipLeft = gMenuViewportCenterX - viewHalfWidth;
        if (y0 >= clipBottom) {
            return;
        }
        if (x1 < clipLeft) {
            return;
        }

        clipTop = gMenuViewportCenterY - viewHalfHeight;
        if (clipTop > y1) {
            return;
        }
        if (x0 < clipLeft) {
            clippedS = clipLeft - x0;
            x0 = clipLeft;
        }
        if (y0 < (gMenuViewportCenterY - viewHalfHeight)) {
            clippedT = clipTop - y0;
            y0 = clipTop;
        }
        if (x1 >= clipRight) {
            x1 = clipRight;
        }
        if (y1 >= clipBottom) {
            y1 = clipBottom;
        }

        {
            s32 textureStep;

            gDPPipeSync(gRegionAllocPtr++);
            gDPSetTextureFilter(gRegionAllocPtr++, G_TF_AVERAGE);
            gDPLoadTextureTile_4b(gRegionAllocPtr++, sprite->imageOffset + (u8 *)asset, G_IM_FMT_CI,
                                  sprite->width, sprite->height, 0, 0, sprite->width, sprite->height, 0,
                                  G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, (u8 *)paletteBase + ((0, paletteIndex) << 5));
            gSPTextureRectangle(gRegionAllocPtr++, x0 << textureScale, y0 << textureScale,
                                x1 << textureScale, y1 << textureScale, G_TX_RENDERTILE,
                                (clippedS << 5) + 0x10, (clippedT << 5) + 0x10,
                                textureStep = 1 << (scale + 10), textureStep);
            gDPPipeSync(gRegionAllocPtr++);
            gDPSetTextureFilter(gRegionAllocPtr++, G_TF_POINT);
            gDPPipeSync(gRegionAllocPtr++);
        }
    }
}
#endif

CLANG_DIAGNOSTIC_PUSH
CLANG_DIAGNOSTIC_IGNORE_DEPRECATED_NON_PROTOTYPE
void drawMenuAsciiFontTile(x, y, s, t, paletteIndex) s16 x;
s16 y;
u16 s;
u16 t;
u16 paletteIndex;
{
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    volatile char pad[0x40];
    s32 maxX;
    s32 maxY;
    s32 minX;
    s32 minY;
    s32 clipS;
    s32 clipT;
    s32 halfHeight;

    x0 = x + gMenuViewportCenterX;
    y0 = y + gMenuViewportCenterY;
    x1 = x0 + 8;
    y1 = y0 + 8;
    clipS = 0;
    clipT = 0;

    maxX = gMenuViewportCenterX + (gMenuViewportWidth / 2);
    if (x0 < maxX) {
        halfHeight = gMenuViewportHeight / 2;
        maxY = gMenuViewportCenterY + halfHeight;
        minX = gMenuViewportCenterX - (gMenuViewportWidth / 2);
        if (y0 < maxY) {
            minY = gMenuViewportCenterY - halfHeight;
            if ((x1 >= minX) && (y1 >= minY)) {
                if (x0 < minX) {
                    clipS = minX - x0;
                    x0 = minX;
                }
                if (y0 < minY) {
                    clipT = minY - y0;
                    y0 = minY;
                }
                if (x1 >= maxX) {
                    x1 = maxX - 1;
                }
                if (y1 >= maxY) {
                    y1 = maxY - 1;
                }
                clipS += s;
                clipT += t;

                if (gMenuAsciiFontPaletteIndex != paletteIndex) {
                    gMenuAsciiFontPaletteIndex = paletteIndex;
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (paletteIndex << 5) + (u32)gMenuAsciiFontPaletteBase);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                }

                gSPTextureRectangle(
                    gRegionAllocPtr++,
                    x0 * 4,
                    y0 * 4,
                    x1 * 4,
                    y1 * 4,
                    0,
                    clipS << 5,
                    clipT << 5,
                    0x400,
                    0x400
                );
            }
        }
    }
}

extern s16 gMenuAsciiFontTextureNeedsLoad;

void initMenuAsciiFontTexture(void) {
    AssetTable *assetTable = getRelocatableHeapBlockBase(gAssetHandles[6]);

    gMenuAsciiFontPaletteBase =
        (void *)((assetTable->entryCount * sizeof(AssetTableEntry)) + (u8 *)assetTable + sizeof(AssetTableEntry));
    gMenuAsciiFontTextureNeedsLoad = -1;
    gMenuAsciiFontPaletteIndex = -1;
}

void drawMenuAsciiCharImpl(s16 x, s16 y, u8 ch, u16 arg3) {
    char pad[8];
    u32 tile;
    u16 s;
    FontTexture *font;

    if ((ch >= 'a') && (ch <= 'z')) {
        if (gMenuAsciiFontTextureNeedsLoad) {
            font = (FontTexture *)getRelocatableHeapBlockBase(gAssetHandles[6]);

            gDPLoadTextureTile_4b(
                gRegionAllocPtr++,
                font->imageOffset + (u8 *)font,
                G_IM_FMT_CI,
                font->width,
                font->height,
                0,
                0,
                font->width,
                font->height,
                0,
                G_TX_CLAMP,
                G_TX_CLAMP,
                G_TX_NOMASK,
                G_TX_NOMASK,
                G_TX_NOLOD,
                G_TX_NOLOD
            );

            gMenuAsciiFontTextureNeedsLoad = 0;
            gMenuAsciiFontPaletteIndex = -1;
        }
        tile = ch - 0x40;
        s = ((tile & 7) << 3) & 0xFFFF & 0xFFFF;
        drawMenuAsciiFontTile(x, y, s, tile & 0x38, arg3 & 0xFFFF & 0xFFFF & 0xFFFF);
    } else {
        if (gMenuAsciiFontTextureNeedsLoad != 0) {
            font = (FontTexture *)getRelocatableHeapBlockBase(gAssetHandles[6]);

            gDPLoadTextureTile_4b(
                gRegionAllocPtr++,
                font->imageOffset + (u8 *)font,
                G_IM_FMT_CI,
                font->width,
                font->height,
                0,
                0,
                font->width,
                font->height,
                0,
                G_TX_CLAMP,
                G_TX_CLAMP,
                G_TX_NOMASK,
                G_TX_NOMASK,
                G_TX_NOLOD,
                G_TX_NOLOD
            );

            gMenuAsciiFontTextureNeedsLoad = 0;
            gMenuAsciiFontPaletteIndex = -1;
        }
        tile = ch - 0x20;
        if (tile < 0x40) {
            s = ((tile & 7) << 3) & 0xFFFF & 0xFFFF;
            drawMenuAsciiFontTile(x, y, s, tile & 0x38, arg3 & 0xFFFF & 0xFFFF & 0xFFFF);
        }
    }
}
CLANG_DIAGNOSTIC_POP

#pragma weak drawMenuAsciiChar = drawMenuAsciiCharImpl
extern void drawMenuAsciiChar(s16 x, s16 y, u8 ch, u16 arg3);
#ifdef __clang__
void drawMenuAsciiCharLegacy(s16 x, s16 y, volatile s32 ch, u16 arg3) {
    drawMenuAsciiCharImpl(x, y, ch, arg3);
}
#else
#pragma weak drawMenuAsciiCharLegacy = drawMenuAsciiCharImpl
extern void drawMenuAsciiCharLegacy(s16 x, s16 y, volatile s32 ch, u16 arg3);
#endif

CLANG_DIAGNOSTIC_PUSH
CLANG_DIAGNOSTIC_IGNORE_SELF_ASSIGN
void drawMenuAsciiTextDefaultScale(s16 arg0, s16 arg1, u8 *arg2, u16 arg3) {
    s32 var_s0;
    u8 *var_s1;
    char mask;
    s32 var_s2;
    s32 temp_s4;
    s32 temp_t9;
    s32 var_a2;
    s32 newline;

    mask = 0xFFFFFFFFFFFFFFFFu;
    temp_t9 = *arg2 & mask;
    var_s0 = arg0;
    var_s1 = arg2;
    var_s2 = arg1;
    temp_s4 = var_s0;
    if ((0, temp_t9) != 0) {
        var_a2 = temp_t9 & 0xFF;
        arg0 = arg0;
        newline = 0xA;
        do {
            if (newline == var_a2) {
                var_s0 = temp_s4;
                var_s2 += 8;
            } else {
                drawMenuAsciiCharLegacy(var_s0, var_s2, var_a2, arg3);
                var_s0 += 8;
            }
            var_a2 = var_s1[1];
            var_s1++;
        } while (var_a2 != 0);
    }
}
CLANG_DIAGNOSTIC_POP

extern u8 *gRenderCallbackScratchPtr;

void allocRenderCallbackScratchBuffer(void) {
    gAssetHandles[0] = allocRelocatableHeapBlock(0x4000);
}

void resetRenderScratchAllocator(void) {
    gRenderCallbackScratchPtr = getRelocatableHeapBlockBase(gAssetHandles[0]);
}

void *allocRenderCallbackScratch(s32 arg0) {
    s32 *new_var2;
    u32 new_var;
    s32 sp1C;
    s32 base;
    s32 temp_a0;

    sp1C = (s32)gRenderCallbackScratchPtr;
    base = (s32)getRelocatableHeapBlockBase(gAssetHandles[0]);
    new_var2 = &temp_a0;
    temp_a0 = (s32)gRenderCallbackScratchPtr + ((((u32)(arg0 + 3)) >> 2) * 4);
    new_var = (u32)(*new_var2 - base);
    if (new_var >= 0x4001U) {
        return NULL;
    }
    gRenderCallbackScratchPtr = (u8 *)*new_var2;
    return (void *)sp1C;
}

void addRenderCallback(RenderCallbackNode **arg0, RenderCallback arg1, void *arg2) {
    RenderCallbackNode *temp_v0 = allocRenderCallbackScratch(sizeof(RenderCallbackNode));

    if (temp_v0 != NULL) {
        temp_v0->next = *arg0;
        temp_v0->callback = arg1;
        temp_v0->arg = arg2;
        *arg0 = temp_v0;
    }
}

extern Gfx *gCurrentTaskDisplayListStart;

void runRenderCallbacks(RenderCallbackNode **arg0) {
    RenderCallbackNode *s0 = *arg0;
    if (s0 != NULL) {
    loop:
        if ((u32)(((u8 *)gRegionAllocPtr - (u8 *)gCurrentTaskDisplayListStart) - 0x5B8) < 0x14181U) {
            s0->callback(s0->arg);
            s0 = s0->next;
            if (s0 != NULL) {
                goto loop;
            }
        }
    }
}

void allocMenuRenderScratchBuffers(void) {
    gAssetHandles[1] = allocRelocatableHeapBlock(0x8000);
    gAssetHandles[2] = allocRelocatableHeapBlock(0x8000);
}

extern u32 gMenuRenderScratchUsedSize;
extern u8 *gMenuRenderScratchStart;
extern u8 *gMenuRenderScratchPtr;

void selectMenuRenderScratchBuffer(s32 arg0) {
    gMenuRenderScratchUsedSize = 0;
    if (arg0 == 0) {
        gMenuRenderScratchPtr = gMenuRenderScratchStart = getRelocatableHeapBlockBase(gAssetHandles[1]);
    } else {
        gMenuRenderScratchPtr = gMenuRenderScratchStart = getRelocatableHeapBlockBase(gAssetHandles[2]);
    }
}

void *allocMenuRenderScratch(s32 size) {
    u8 *oldPtr = gMenuRenderScratchPtr;
    u8 *newPtr = ((0, oldPtr)) + ((((u32)(size + 3)) >> 2) * 4);
    s32 new_var2;

    new_var2 = newPtr - gMenuRenderScratchStart;
    newPtr++;
    newPtr--;

    if (gMenuRenderScratchPtr) {}

    if ((u32)(newPtr - gMenuRenderScratchStart) >= 0x8000) {
        return 0;
    }

    gMenuRenderScratchPtr = newPtr;
    gMenuRenderScratchUsedSize = new_var2;

    return oldPtr;
}

extern void osWritebackDCache(void *, s32);

void writebackMenuRenderScratchBuffer(s32 arg0) {
    if (arg0 == 0) {
        osWritebackDCache((void *)getRelocatableHeapBlockBase(gAssetHandles[1]), gMenuRenderScratchUsedSize);
    } else {
        osWritebackDCache((void *)getRelocatableHeapBlockBase(gAssetHandles[2]), gMenuRenderScratchUsedSize);
    }
}

void *copyGfxCommandBlockToScratch(Mtx *arg0) {
    Mtx *p = allocMenuRenderScratch(sizeof(Mtx));
    if (p == NULL) {
        return NULL;
    }
    *p = *arg0;
    return p;
}

void packFixedTransformMatrix(Transform3D *source, Mtx *dest) {
    Transform3DComponents *components = (Transform3DComponents *)source;

    PACKED_MTX_WORD(dest, 0) = ((components->xy >> 12) & 0xFFFF) | ((components->xx << 4) & 0xFFFF0000);
    PACKED_MTX_WORD(dest, 1) = (components->xz << 4) & 0xFFFF0000;
    PACKED_MTX_WORD(dest, 2) = ((components->yy >> 12) & 0xFFFF) | ((components->yx << 4) & 0xFFFF0000);
    PACKED_MTX_WORD(dest, 3) = (components->yz << 4) & 0xFFFF0000;
    PACKED_MTX_WORD(dest, 4) = ((components->zy >> 12) & 0xFFFF) | ((components->zx << 4) & 0xFFFF0000);
    PACKED_MTX_WORD(dest, 5) = (components->zz << 4) & 0xFFFF0000;
    PACKED_MTX_WORD(dest, 6) = ((components->translation.y >> 16) & 0xFFFF) | (components->translation.x & 0xFFFF0000);
    PACKED_MTX_WORD(dest, 7) = (components->translation.z & 0xFFFF0000) | 1;
    PACKED_MTX_WORD(dest, 8) = ((components->xy << 4) & 0xFFFF) | ((components->xx << 20) & 0xFFFF0000);
    PACKED_MTX_WORD(dest, 9) = (components->xz << 20) & 0xFFFF0000;
    PACKED_MTX_WORD(dest, 10) = ((components->yy << 4) & 0xFFFF) | ((components->yx << 20) & 0xFFFF0000);
    PACKED_MTX_WORD(dest, 11) = (components->yz << 20) & 0xFFFF0000;
    PACKED_MTX_WORD(dest, 12) = ((components->zy << 4) & 0xFFFF) | ((components->zx << 20) & 0xFFFF0000);
    PACKED_MTX_WORD(dest, 13) = (components->zz << 20) & 0xFFFF0000;
    PACKED_MTX_WORD(dest, 14) = (components->translation.y & 0xFFFF) | ((components->translation.x << 16) & 0xFFFF0000);
    PACKED_MTX_WORD(dest, 15) = (components->translation.z << 16) & 0xFFFF0000;
}

Mtx *allocFixedTransformMatrix(Transform3D *source) {
    Mtx *dst = allocMenuRenderScratch(sizeof(Mtx));
    Transform3DComponents *components = (Transform3DComponents *)source;

    if (dst == NULL) {
        return NULL;
    }

    PACKED_MTX_WORD(dst, 0) = ((components->xy >> 12) & 0xFFFF) | ((components->xx << 4) & 0xFFFF0000);
    PACKED_MTX_WORD(dst, 1) = (components->xz << 4) & 0xFFFF0000;
    PACKED_MTX_WORD(dst, 2) = ((components->yy >> 12) & 0xFFFF) | ((components->yx << 4) & 0xFFFF0000);
    PACKED_MTX_WORD(dst, 3) = (components->yz << 4) & 0xFFFF0000;
    PACKED_MTX_WORD(dst, 4) = ((components->zy >> 12) & 0xFFFF) | ((components->zx << 4) & 0xFFFF0000);
    PACKED_MTX_WORD(dst, 5) = (components->zz << 4) & 0xFFFF0000;
    PACKED_MTX_WORD(dst, 6) = ((source->translation.y >> 16) & 0xFFFF) | (source->translation.x & 0xFFFF0000);
    PACKED_MTX_WORD(dst, 7) = (source->translation.z & 0xFFFF0000) | 1;
    PACKED_MTX_WORD(dst, 8) = ((components->xy << 4) & 0xFFFF) | ((components->xx << 20) & 0xFFFF0000);
    PACKED_MTX_WORD(dst, 9) = (components->xz << 20) & 0xFFFF0000;
    PACKED_MTX_WORD(dst, 10) = ((components->yy << 4) & 0xFFFF) | ((components->yx << 20) & 0xFFFF0000);
    PACKED_MTX_WORD(dst, 11) = (components->yz << 20) & 0xFFFF0000;
    PACKED_MTX_WORD(dst, 12) = ((components->zy << 4) & 0xFFFF) | ((components->zx << 20) & 0xFFFF0000);
    PACKED_MTX_WORD(dst, 13) = (components->zz << 20) & 0xFFFF0000;
    PACKED_MTX_WORD(dst, 14) = (source->translation.y & 0xFFFF) | ((source->translation.x << 16) & 0xFFFF0000);
    PACKED_MTX_WORD(dst, 15) = (source->translation.z << 16) & 0xFFFF0000;
    return dst;
}

Mtx *allocFixedRotationMatrix(Mat3x3 rotation) {
    Mtx *dst = allocMenuRenderScratch(sizeof(Mtx));
    Transform3DComponents *components = (Transform3DComponents *)rotation;

    if (dst == NULL) {
        return NULL;
    }

    PACKED_MTX_WORD(dst, 0) = ((components->xy >> 12) & 0xFFFF) | ((components->xx << 4) & 0xFFFF0000);
    PACKED_MTX_WORD(dst, 1) = (components->xz << 4) & 0xFFFF0000;
    PACKED_MTX_WORD(dst, 2) = ((components->yy >> 12) & 0xFFFF) | ((components->yx << 4) & 0xFFFF0000);
    PACKED_MTX_WORD(dst, 3) = (components->yz << 4) & 0xFFFF0000;
    PACKED_MTX_WORD(dst, 4) = ((components->zy >> 12) & 0xFFFF) | ((components->zx << 4) & 0xFFFF0000);
    PACKED_MTX_WORD(dst, 5) = (components->zz << 4) & 0xFFFF0000;
    PACKED_MTX_WORD(dst, 6) = 0;
    PACKED_MTX_WORD(dst, 7) = 1;
    PACKED_MTX_WORD(dst, 8) = ((components->xy << 4) & 0xFFFF) | ((components->xx << 20) & 0xFFFF0000);
    PACKED_MTX_WORD(dst, 9) = (components->xz << 20) & 0xFFFF0000;
    PACKED_MTX_WORD(dst, 10) = ((components->yy << 4) & 0xFFFF) | ((components->yx << 20) & 0xFFFF0000);
    PACKED_MTX_WORD(dst, 11) = (components->yz << 20) & 0xFFFF0000;
    PACKED_MTX_WORD(dst, 12) = ((components->zy << 4) & 0xFFFF) | ((components->zx << 20) & 0xFFFF0000);
    PACKED_MTX_WORD(dst, 13) = (components->zz << 20) & 0xFFFF0000;
    PACKED_MTX_WORD(dst, 14) = 0;
    PACKED_MTX_WORD(dst, 15) = 0;
    return dst;
}

Mtx *allocTranslationOnlyFixedMatrix(Mtx *source) {
    PackedMtxWords *dst = allocMenuRenderScratch(sizeof(Mtx));
    PackedMtxWords *sourceWords = (PackedMtxWords *)source;

    if (dst == NULL) {
        return NULL;
    }

    dst->w0 = 0x10000;
    dst->w1 = 0;
    dst->w2 = 1;
    dst->w3 = 0;
    dst->w4 = 0;
    dst->w5 = 0x10000;
    dst->w6 = ((sourceWords->w6 >> 16) & 0xFFFF) | (sourceWords->w5 & 0xFFFF0000);
    dst->w7 = (sourceWords->w7 & 0xFFFF0000) | 1;
    dst->w8 = 0;
    dst->w9 = 0;
    dst->w10 = 0;
    dst->w11 = 0;
    dst->w12 = 0;
    dst->w13 = 0;
    dst->w14 = (sourceWords->w6 & 0xFFFF) | ((sourceWords->w5 << 16) & 0xFFFF0000);
    dst->w15 = (sourceWords->w7 << 16) & 0xFFFF0000;
    return (Mtx *)dst;
}

void setPackedMatrixTranslation(Mtx *matrix, Vec3i *translation) {
    PACKED_MTX_WORD(matrix, 6) = (translation->x & 0xFFFF0000) | ((translation->y >> 16) & 0xFFFF);
    PACKED_MTX_WORD(matrix, 7) = (translation->z & 0xFFFF0000) | 1;
    PACKED_MTX_WORD(matrix, 14) = ((translation->x << 16) & 0xFFFF0000) | (translation->y & 0xFFFF);
    PACKED_MTX_WORD(matrix, 15) = (translation->z << 16) & 0xFFFF0000;
}

void copyPackedMatrixTranslation(Mtx *source, Mtx *dest) {
    PackedMtxWords *destWords = (PackedMtxWords *)dest;
    s32 *word6;
    s32 *word5;
    PackedMtxWords *sourceWords = (PackedMtxWords *)source;

    word5 = &sourceWords->w5;
    word6 = &sourceWords->w6;
    destWords->w6 = (*word5 & 0xFFFF0000) | ((*word6 >> 16) & 0xFFFF);
    destWords->w7 = (sourceWords->w7 & 0xFFFF0000) | 1;
    destWords->w14 = ((*word5 << 16) & 0xFFFF0000) | (*word6 & 0xFFFF);
    destWords->w15 = (sourceWords->w7 << 16) & 0xFFFF0000;
}

void scaleFixedMatrix3sByQuarter(Mat3x3 arg0) {
    arg0[0] = arg0[0] / 4;
    arg0[1] = arg0[1] / 4;
    arg0[2] = arg0[2] / 4;
    arg0[3] = arg0[3] / 4;
    arg0[4] = arg0[4] / 4;
    arg0[5] = arg0[5] / 4;
    arg0[6] = arg0[6] / 4;
    arg0[7] = arg0[7] / 4;
    arg0[8] = arg0[8] / 4;
}

void noopThreeArgs(void *arg0, void *arg1, void *arg2) {
}

void noopFourArgs(void *arg0, void *arg1, void *arg2, void *arg3) {
}

s32 isPositionNearAnyRaceViewportFocus(Vec3i *position) {
    s32 i;
    s32 diffX;
    s32 diffZ;

    if (gRaceSplitscreenMode == 2) {
        return 1;
    }

    for (i = 0; i < 4; i++) {
        if (gViewportStates[i].active != 0) {
            diffX = D_801121E0[i].pos.x - position->x;
            diffZ = D_801121E0[i].pos.z - position->z;
            if ((diffX < 0x6000000) && (diffX >= (s32)0xFA000001) && (diffZ < 0x6000000) &&
                (diffZ >= (s32)0xFA000001)) {
                return 1;
            }
        }
    }

    return 0;
}
