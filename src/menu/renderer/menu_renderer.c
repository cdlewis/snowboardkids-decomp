#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/controller_input.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/system_runtime.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"

#define FONT_GFX_CMD(pkt, cmd0, cmd1) \
    {                                 \
        Gfx *_g = (Gfx *)(pkt);       \
        _g->words.w0 = (cmd0);        \
        _g->words.w1 = (cmd1);        \
    }

#define MENU_GLYPH_SCRIPT_NEWLINE 0xFFFD
#define MENU_GLYPH_SCRIPT_SPACE 0xFFFE
#define MENU_GLYPH_SCRIPT_COLOR 0xFFFC
#define MENU_GLYPH_SCRIPT_END 0xFFFF
#define MENU_GLYPH_WIDE_ADVANCE 0x10
#define MENU_GLYPH_NARROW_ADVANCE 8
#define MENU_GLYPH_LINE_HEIGHT 0x10
#define MENU_GLYPH_TALL_HEIGHT 0x10
#define MENU_GLYPH_WIDE_WIDTH 0x10
#define MENU_GLYPH_NARROW_WIDTH 8
#define MENU_GLYPH_DEFAULT_FONT_BANK 0x22
#define MENU_PALETTE_COLOR_COUNT 0x10
#define MENU_PALETTE_SIZE_BYTES 0x20
#define MENU_RGBA5551_ALPHA_BIT 1
#define MENU_RGBA5551_CHANNEL_MASK 0x1F
#define MENU_RGBA5551_SCALE_BASE 0x100
#define MENU_GLYPH_TEXEL_FRAC_BITS 5
#define MENU_GLYPH_RECT_FRAC_BITS 2
#define MENU_GLYPH_TEXTURE_STEP 0x0400
#define MENU_HALF_SCALE_STEP 0x800
#define MENU_CROSSFADE_LOAD_BLOCK_FLAGS 0x07000000
#define MENU_CROSSFADE_TEXTURE_1_TILE 0x01000000
#define MENU_CROSSFADE_SCALE_STEP 0x0400
// Palette selectors are passed as s32 values, but only their low byte is used.
#define MENU_GLYPH_PALETTE_INDEX(palette) (((u8 *)&(palette))[3])

typedef struct MenuRenderTask MenuRenderTask;
typedef struct MenuRenderAssetTableHeader MenuRenderAssetTableHeader;
typedef struct MenuRenderAssetTableEntry MenuRenderAssetTableEntry;
typedef struct FontAssetHeader FontAssetHeader;
typedef struct FontTexture FontTexture;
typedef struct MenuFontAssetEntry MenuFontAssetEntry;
typedef union MenuGlyphPalette MenuGlyphPalette;

struct MenuRenderTask {
    /* 0x00 */ MenuRenderTask *prev;
    /* 0x04 */ MenuRenderTask *next;
    /* 0x08 */ void (*callback)(MenuRenderTask *);
    /* 0x0C */ u16 type;
    /* 0x0E */ u16 priority;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 callbackTimer;
    /* 0x16 */ s16 isActive;
};

typedef struct MenuRenderTileInfo {
    /* 0x0 */ s16 imageIndex;
    /* 0x2 */ u8 paletteIndex;
    /* 0x3 */ u8 flip;
} MenuRenderTileInfo;

struct MenuRenderSpriteActor {
    /* 0x00 */ MenuRenderTask task;
    /* 0x18 */ MenuRenderSprite sprite;
};

struct MenuRenderAssetTableEntry {
    /* 0x0 */ s32 imageOffset;
    /* 0x4 */ u16 textureIndex;
    /* 0x6 */ u8 width;
    /* 0x7 */ u8 height;
};

struct MenuRenderAssetTableHeader {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 entryCount;
    /* 0x8 */ MenuRenderAssetTableEntry entries[1];
};

struct MenuFontAssetEntry {
    /* 0x0 */ s32 imageOffset;
    /* 0x4 */ u16 textureIndex;
    /* 0x6 */ u8 width;
    /* 0x7 */ u8 height;
};

struct FontAssetHeader {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 entryCount;
};

struct FontTexture {
    /* 0x0 */ u32 imageOffset;
    /* 0x4 */ u16 paletteIndex;
    /* 0x6 */ u8 width;
    /* 0x7 */ u8 height;
};

struct FontAsset {
    /* 0x0 */ FontAssetHeader header;
    /* 0x8 */ FontTexture textures[1];
};

struct MenuFontAssetTable {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 entryCount;
    /* 0x8 */ MenuFontAssetEntry entries[1];
};

union MenuGlyphPalette {
    u8 bytes[MENU_PALETTE_SIZE_BYTES];
    u16 colors[MENU_PALETTE_COLOR_COUNT];
};

typedef u16 MenuPalette[MENU_PALETTE_COLOR_COUNT];

typedef void (*MenuRenderSpriteActorCallback)(MenuRenderSpriteActor *);
typedef void (*MenuRenderCallback)(MenuRenderSprite *);

u16 gMenuTransparentPalette[MENU_PALETTE_COLOR_COUNT] = {
    0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};
s16 gMenuSpriteFlipScales[8] = {
    1, 1, -1, 1, 1, -1, -1, -1,
};

extern void *allocMenuRenderScratch(s32 size);
void drawMenuGlyph(s16 x, s16 y, u16 glyphIndex, u8 paletteIndex, u16 intensity, u16 fontBank);
extern Gfx *gRegionAllocPtr;
extern s16 gMenuFadeAlpha;
extern s16 gMenuViewportWidth;
extern s16 gMenuViewportHeight;
extern s16 gMenuViewportCenterX;
extern s16 gMenuViewportCenterY;

void drawMenuAssetRegion(
    s16 x,
    s16 y,
    void *tableAddress,
    u16 entryIndex,
    u16 scaleX,
    u16 scaleY,
    u8 startS,
    u8 startT,
    u8 width,
    u8 height
) {
    MenuRenderAssetTableEntry *entry;
    s32 minX;
    u8 *paletteBase;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 texS;
    s32 texT;
    s32 minY;
    u16 scaleXValue;
    u16 scaleYValue;
    s16 maxX;
    s16 maxY;

    entry = &((MenuRenderAssetTableHeader *)tableAddress)->entries[entryIndex];
    paletteBase = (((MenuRenderAssetTableHeader *)tableAddress)->entryCount * sizeof(MenuRenderAssetTableEntry)) +
                  (u8 *)tableAddress + sizeof(MenuRenderAssetTableEntry);
    scaleXValue = scaleX;
    scaleYValue = scaleY;
    left = (x + gMenuViewportCenterX) << 2;
    top = (y + gMenuViewportCenterY) << 2;
    right = (((width * scaleXValue) << 2) >> 5) + left;
    bottom = height;
    bottom *= scaleYValue;
    bottom = ((bottom << 2) >> 5) + top;
    // Keeping these assignments on one source line preserves IDO's instruction scheduling.
    texS = startS << 5;
    texT = startT << 5;
    minY = (s16)((gMenuViewportCenterY - (gMenuViewportHeight / 2)) << 2);
    maxY = (gMenuViewportCenterY + (gMenuViewportHeight / 2)) << 2;
    minX = (s16)((gMenuViewportCenterX - (gMenuViewportWidth / 2)) << 2);
    maxX = (gMenuViewportCenterX + (gMenuViewportWidth / 2)) << 2;

    if ((left < maxX) && (top < maxY) && (right >= minX) && (bottom >= minY)) {
        if (left < minX) {
            do {
                texS = ((((minX - left) << 3) << 5) / scaleXValue) + texS;
                left = minX;
            } while (0);
        }
        if (top < minY) {
            texT = ((((minY - top) << 3) << 5) / scaleYValue) + texT;
            top = minY;
        }
        if (right >= maxX) {
            right = maxX - 4;
        }
        if (bottom >= maxY) {
            bottom = maxY - 4;
        }

        gDPLoadTextureTile_4b(
            gRegionAllocPtr++,
            entry->imageOffset + (u8 *)tableAddress + 0x80000000,
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
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteBase + (entry->textureIndex << 5) + 0x80000000);
        gSPTextureRectangle(
            gRegionAllocPtr++,
            left,
            top,
            right,
            bottom,
            G_TX_RENDERTILE,
            texS,
            texT,
            (u16)(0x8000 / scaleXValue),
            (u16)(0x8000 / scaleYValue)
        );
    }
}

void drawMenuSprite(s16 arg0, s16 arg1, void *arg2, u16 arg3, u16 arg4, u16 arg5, u8 arg6, u8 arg7) {
    s16 temp_v0;
    s16 temp_v1;

    drawMenuSpriteClipped(
        arg0,
        arg1,
        arg2,
        arg3,
        arg4,
        arg5,
        arg6,
        arg7,
        temp_v0 = (s16)(gMenuViewportWidth / 2),
        temp_v1 = (s16)(gMenuViewportHeight / 2),
        temp_v0,
        temp_v1
    );
}

/* Preserve the promoted tile-index argument used by an older caller. */
#ifdef __clang__
void drawMenuSpriteWideIndex(s16 x, s16 y, void *texture, s32 tileIndex, u16 width, u16 height, u8 palette, u8 flip) {
    drawMenuSprite(x, y, texture, tileIndex, width, height, palette, flip);
}
#else
#pragma weak drawMenuSpriteWideIndex = drawMenuSprite
extern void
drawMenuSpriteWideIndex(s16 x, s16 y, void *texture, s32 tileIndex, u16 width, u16 height, u8 palette, u8 flip);
#endif

void drawMenuSpriteClipped(s16 x, s16 y, MenuFontAssetTable *table, u16 imageIndex, u16 scaleX, u16 scaleY,
                           u8 flipMode, u8 paletteIndex, s16 clipLeft, s16 clipTop, s16 clipRight,
                           s16 clipBottom) {
    MenuFontAssetEntry *entry;
    s32 selectedPalette;
    u8 *palette;
    s32 left; // sp90
    s32 top; // sp8C
    s32 right; // sp88
    s32 bottom; // sp84
    s32 texS; // sp80
    s32 texT; // sp7C
    u32 height;
    s32 pad;
    s16 flipS; // sp72
    s16 flipT; // sp70
    s16 pad2;
    s16 minX;
    s16 minY;
    s16 maxX;
    s16 maxY;

    entry = &table->entries[imageIndex];
    palette = table->entryCount * sizeof(MenuFontAssetEntry) + (u8*)table->entries;
    if (scaleX > 0x200) {
        return;
    }
    if (scaleX <= 0) {
        return;
    }
    if (scaleY > 0x200) {
        return;
    }
    if (scaleY <= 0) {
        return;
    }

    flipS = gMenuSpriteFlipScales[(flipMode & 3) * 2 + 0];
    flipT = gMenuSpriteFlipScales[(flipMode & 3) * 2 + 1];

    texS = entry->width;
    texT = entry->height;
    
    left = (x + gMenuViewportCenterX) << 2;
    top = (y + gMenuViewportCenterY) << 2;
    right = left + (((scaleX * texS) << 2) >> 5);
    bottom = top + (((scaleY * texT) << 2) >> 5);

    height = entry->height;
    texS = 0 * height;
    texT = 0;
    if (flipS == -1) {
        texS = ((entry->width - 1) << 5);
    }
    if (flipT == -1) {
        texT = ((entry->height - 1) << 5) - texT;
    }


    clipTop = gMenuViewportCenterY - clipTop;
    clipBottom = gMenuViewportCenterY + clipBottom;
    clipLeft = gMenuViewportCenterX - clipLeft;
    clipRight = gMenuViewportCenterX + clipRight;
    if (clipLeft < gMenuViewportCenterX - (gMenuViewportWidth / 2)) {
        clipLeft = gMenuViewportCenterX - (gMenuViewportWidth / 2);
    }
    if (clipRight > gMenuViewportCenterX + (gMenuViewportWidth / 2)) {
        clipRight = gMenuViewportCenterX + (gMenuViewportWidth / 2);
    }
    if (clipTop < gMenuViewportCenterY - (gMenuViewportHeight / 2)) {
        clipTop = gMenuViewportCenterY - (gMenuViewportHeight / 2);
    }
    if (clipBottom > gMenuViewportCenterY + (gMenuViewportHeight / 2)) {
        clipBottom = gMenuViewportCenterY + (gMenuViewportHeight / 2);
    }

    minX = clipRight << 2;
    minY = clipBottom << 2;
    maxX = clipLeft << 2;
    maxY = clipTop << 2;
    if ((left >= minX) || (top >= minY) || (right < maxX) || (bottom < maxY)) {
        return;
    }

    if (left < maxX) {
        texS = (((maxX - left) << 3) << 5) / scaleX;
        if (flipS == -1) {
            texS = ((entry->width - 1) << 5) - texS;
        }
        left = maxX;
    }
    if (top < maxY) {
        texT = (((maxY - top) << 3) << 5) / scaleY;
        if (flipT == -1) {
            texT = ((entry->height - 1) << 5) - texT;
        }
        top = maxY;
    }
    if (right >= minX) {
        right = minX - 4;
    }
    if (bottom >= minY) {
        bottom = minY - 4;
    }

    if (paletteIndex == 0) {
        selectedPalette = entry->textureIndex;
    } else {
        selectedPalette = (u16)(paletteIndex - 1);
    }

    gDPLoadTextureTile_4b(gRegionAllocPtr++, entry->imageOffset + (u8 *)table,
                          G_IM_FMT_CI, entry->width, entry->height, 0, 0,
                          entry->width, entry->height, 0, G_TX_CLAMP, G_TX_CLAMP,
                          G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    if (selectedPalette != 0xFE) {
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, (selectedPalette << 5) + (u8*)palette);
    } else {
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gMenuTransparentPalette);
    }
    gSPTextureRectangle(gRegionAllocPtr++, left, top, right, bottom, G_TX_RENDERTILE,
                        texS, texT, (u16)((u16)(0x8000 / scaleX) * flipS),
                        (u16)((u16)(0x8000 / scaleY) * flipT));
}

void drawMenuSpriteWithAlpha(s16 arg0, s16 arg1, void *arg2, u16 arg3, u16 arg4, u16 arg5, u8 arg6, u16 arg7, u8 arg8) {
    s32 temp_v0;
    s32 temp_v1;

    drawMenuSpriteWithAlphaClipped(
        arg0,
        arg1,
        arg2,
        arg3,
        arg4,
        arg5,
        arg6,
        arg7,
        arg8,
        temp_v0 = (s16)(gMenuViewportWidth / 2),
        temp_v1 = (s16)(gMenuViewportHeight / 2),
        temp_v0,
        temp_v1
    );
}

/*
 * One matched caller predates this function's narrow prototype. Preserve its
 * original argument promotions without weakening the canonical declaration.
 */
#ifdef __clang__
void drawMenuSpriteWithAlphaWideArgs(
    s32 x,
    s32 y,
    void *texture,
    s32 tileIndex,
    s32 width,
    s32 height,
    s32 palette,
    s32 alpha,
    u32 flip
) {
    drawMenuSpriteWithAlpha(x, y, texture, tileIndex, width, height, palette, alpha, flip);
}
#else
#pragma weak drawMenuSpriteWithAlphaWideArgs = drawMenuSpriteWithAlpha
extern void drawMenuSpriteWithAlphaWideArgs(
    s32 x,
    s32 y,
    void *texture,
    s32 tileIndex,
    s32 width,
    s32 height,
    s32 palette,
    s32 alpha,
    u32 flip
);
#endif

void drawMenuSpriteWithAlphaClipped(
    s16 x,
    s16 y,
    FontAsset *asset,
    u16 tileIndex,
    u16 scaleX,
    u16 scaleY,
    u8 flipMode,
    u16 alpha,
    u8 paletteIndex,
    s32 clipLeft,
    s32 clipTop,
    s32 clipRight,
    s32 clipBottom
) {
    FontTexture *texture;
    volatile u8 paddingA[4];
    u8 *paletteBase;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 texS;
    s32 texT;
    u32 height;
    volatile u8 paddingB[4];
    s32 paddingWord;
    s16 flipS;
    s16 flipT;
    s16 paddingHalfword;
    s32 minX;
    s32 minY;
    s32 maxX;
    s32 maxY;
    volatile u16 paddingC;
    u16 selectedPalette;

    texture = &asset->textures[tileIndex];
    paletteBase = (asset->header.entryCount * sizeof(FontTexture)) + (u8 *)asset + sizeof(FontAssetHeader);
    if (scaleX > 0x200) {
        return;
    }
    if (scaleX <= 0) {
        return;
    }
    if (scaleY > 0x200) {
        return;
    }
    if (scaleY <= 0) {
        return;
    }
    flipS = ((s16 *)gMenuSpriteFlipScales)[(flipMode & 3) * 2 + 0];
    flipT = ((s16 *)gMenuSpriteFlipScales)[(flipMode & 3) * 2 + 1];

    texS = texture->width;
    texT = texture->height;
    left = (x + gMenuViewportCenterX) << 2;
    top = (y + gMenuViewportCenterY) << 2;
    right = left + (((scaleX * texS) << 2) >> 5);
    bottom = top + (((scaleY * texT) << 2) >> 5);

    height = texture->height;
    // Retain the cached height's frontend web while initializing the S coordinate.
    texS = height * 0;
    texT = 0;
    if (flipS == -1) {
        texS = (texture->width - 1) << 5;
    }
    if (flipT == -1) {
        texT = ((texture->height - 1) << 5) - texT;
    }

    minY = (s16)((gMenuViewportCenterY - (s16)clipTop) << 2);
    maxY = (s16)((gMenuViewportCenterY + (s16)clipBottom) << 2);
    minX = (s16)((gMenuViewportCenterX - (s16)clipLeft) << 2);
    maxX = (s16)((gMenuViewportCenterX + (s16)clipRight) << 2);

    if ((left >= maxX) || (top >= maxY) || (right < minX) || (bottom < minY)) {
        return;
    }
    if (left < minX) {
        texS = (((minX - left) << 3) << 5) / scaleX;
        if (flipS == -1) {
            texS = ((texture->width - 1) << 5) - texS;
        }
        left = minX;
    }
    if (top < minY) {
        texT = (((minY - top) << 3) << 5) / scaleY;
        if (flipT == -1) {
            texT = ((texture->height - 1) << 5) - texT;
        }
        top = minY;
    }
    if (right >= maxX) {
        right = maxX - 4;
    }
    if (bottom >= maxY) {
        bottom = maxY - 4;
    }

    if (paletteIndex == 0) {
        selectedPalette = texture->paletteIndex;
    } else {
        selectedPalette = paletteIndex - 1;
    }

    if (alpha != 0x100) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, alpha, alpha, alpha, 0xFF);
    }

    gDPLoadTextureTile_4b(
        gRegionAllocPtr++,
        texture->imageOffset + (u8 *)asset,
        G_IM_FMT_CI,
        texture->width,
        texture->height,
        0,
        0,
        texture->width,
        texture->height,
        0,
        G_TX_CLAMP,
        G_TX_CLAMP,
        G_TX_NOMASK,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOLOD
    );
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteBase + (selectedPalette << 5));
    gSPTextureRectangle(
        gRegionAllocPtr++,
        left,
        top,
        right,
        bottom,
        G_TX_RENDERTILE,
        texS,
        texT,
        (u16)((u16)(0x8000 / scaleX) * flipS),
        (u16)((u16)(0x8000 / scaleY) * flipT)
    );
    if (alpha != 0x100) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

void drawMenuSpriteWithPaletteScale(s16 x, s16 y, FontAsset *asset, u16 index, u16 intensity) {
    FontTexture *texture;
    u8 *textureBase;
    u8 *paletteBase;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 srcX;
    s32 srcY;
    u16 headerSize;
    MenuGlyphPalette *scratch;
    MenuGlyphPalette *source;
    u16 red;
    u16 green;
    u16 blue;
    u16 scaledRed;
    s32 i;

    headerSize = sizeof(FontAssetHeader);
    textureBase = (u8 *)asset + (index * sizeof(FontTexture));
    texture = (FontTexture *)(textureBase + headerSize);
    paletteBase = (asset->header.entryCount * sizeof(FontTexture)) + (u8 *)asset + headerSize;
    left = x + gMenuViewportCenterX;
    top = y + gMenuViewportCenterY;
    right = left + (texture->width >> 1);
    bottom = top + (texture->height >> 1);
    srcX = 0;
    srcY = 0;

    if (left >= gMenuViewportCenterX + (gMenuViewportWidth / 2))
        return;
    if (top >= gMenuViewportCenterY + (gMenuViewportHeight / 2))
        return;
    if (right < gMenuViewportCenterX - (gMenuViewportWidth / 2))
        return;
    if (bottom < gMenuViewportCenterY - (gMenuViewportHeight / 2))
        return;
    if (left < gMenuViewportCenterX - (gMenuViewportWidth / 2)) {
        srcX = (gMenuViewportCenterX - (gMenuViewportWidth / 2)) - left;
        left = gMenuViewportCenterX - (gMenuViewportWidth / 2);
    }
    if (top < gMenuViewportCenterY - (gMenuViewportHeight / 2)) {
        srcY = (gMenuViewportCenterY - (gMenuViewportHeight / 2)) - top;
        top = gMenuViewportCenterY - (gMenuViewportHeight / 2);
    }
    if (right >= gMenuViewportCenterX + (gMenuViewportWidth / 2))
        right = gMenuViewportCenterX + (gMenuViewportWidth / 2);
    if (bottom >= gMenuViewportCenterY + (gMenuViewportHeight / 2))
        bottom = gMenuViewportCenterY + (gMenuViewportHeight / 2);

    gDPPipeSync(gRegionAllocPtr++);
    gDPSetTextureFilter(gRegionAllocPtr++, G_TF_AVERAGE);

    source = (MenuGlyphPalette *)(paletteBase + (texture->paletteIndex * MENU_PALETTE_SIZE_BYTES));
    scratch = allocMenuRenderScratch(sizeof(MenuGlyphPalette));
    for (i = 0; i != MENU_PALETTE_COLOR_COUNT; i++) {
        scratch->colors[i] = source->colors[i];
        if (scratch->colors[i] & MENU_RGBA5551_ALPHA_BIT) {
            red = (scratch->colors[i] >> 11) & MENU_RGBA5551_CHANNEL_MASK;
            green = (scratch->colors[i] >> 6) & MENU_RGBA5551_CHANNEL_MASK;
            blue = (scratch->colors[i] >> 1) & MENU_RGBA5551_CHANNEL_MASK;
            red = (red * intensity) / MENU_RGBA5551_SCALE_BASE;
            green = (green * intensity) / MENU_RGBA5551_SCALE_BASE;
            blue = (blue * intensity) / MENU_RGBA5551_SCALE_BASE;

            scratch->colors[i] = (red << 11) | (green << 6) | (blue << 1) | MENU_RGBA5551_ALPHA_BIT;
        }
    }

    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, scratch);
    gDPLoadTextureTile_4b(
        gRegionAllocPtr++,
        texture->imageOffset + (u8 *)asset,
        G_IM_FMT_CI,
        texture->width,
        texture->height,
        0,
        0,
        texture->width,
        texture->height,
        0,
        G_TX_CLAMP,
        G_TX_CLAMP,
        G_TX_NOMASK,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOLOD
    );
    gSPTextureRectangle(
        gRegionAllocPtr++,
        left << 2,
        top << 2,
        right << 2,
        bottom << 2,
        G_TX_RENDERTILE,
        (srcX << 5) + 0x10,
        (srcY << 5) + 0x10,
        MENU_HALF_SCALE_STEP,
        MENU_HALF_SCALE_STEP
    );
    gDPPipeSync(gRegionAllocPtr++);
    gDPSetTextureFilter(gRegionAllocPtr++, G_TF_POINT);
    gDPPipeSync(gRegionAllocPtr++);
}

void drawMenuSpriteSubrect(
    s16 x,
    s16 y,
    void *assetAddress,
    u16 index,
    u8 srcX,
    u8 srcY,
    u8 width,
    u8 height,
    s32 scaleX,
    s32 scaleY
) {
    FontTexture *texture;
    s32 minX;
    u8 *paletteBase;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 texS;
    s32 texT;
    s32 minY;
    u16 scaleXValue;
    u16 scaleYValue;
    s16 maxX;
    s16 maxY;

    texture = &((FontAsset *)assetAddress)->textures[index];
    paletteBase = (((FontAsset *)assetAddress)->header.entryCount * sizeof(FontTexture)) + (u8 *)assetAddress +
                  sizeof(FontAssetHeader);
    scaleXValue = scaleX;
    scaleYValue = scaleY;
    left = (x + gMenuViewportCenterX) << 2;
    top = (y + gMenuViewportCenterY) << 2;
    right = (((width * scaleXValue) << 2) >> 5) + left;
    bottom = height;
    bottom *= scaleYValue;
    bottom = ((bottom << 2) >> 5) + top;
    // Keeping these assignments on one source line preserves IDO's instruction scheduling.
    texS = srcX << 5;
    texT = srcY << 5;
    minY = (s16)((gMenuViewportCenterY - (gMenuViewportHeight / 2)) << 2);
    maxY = (gMenuViewportCenterY + (gMenuViewportHeight / 2)) << 2;
    minX = (s16)((gMenuViewportCenterX - (gMenuViewportWidth / 2)) << 2);
    maxX = (gMenuViewportCenterX + (gMenuViewportWidth / 2)) << 2;

    if ((left < maxX) && (top < maxY) && (right >= minX) && (bottom >= minY)) {
        if (left < minX) {
            do {
                texS = ((((minX - left) << 3) << 5) / scaleXValue) + texS;
                left = minX;
            } while (0);
        }
        if (top < minY) {
            texT = ((((minY - top) << 3) << 5) / scaleYValue) + texT;
            top = minY;
        }
        if (right >= maxX) {
            right = maxX - 4;
        }
        if (bottom >= maxY) {
            bottom = maxY - 4;
        }

        gDPLoadTextureTile_4b(
            gRegionAllocPtr++,
            texture->imageOffset + (u8 *)assetAddress + 0x80000000,
            G_IM_FMT_CI,
            texture->width,
            texture->height,
            0,
            0,
            texture->width,
            texture->height,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            G_TX_NOMASK,
            G_TX_NOMASK,
            G_TX_NOLOD,
            G_TX_NOLOD
        );
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteBase + (texture->paletteIndex << 5) + 0x80000000);
        gSPTextureRectangle(
            gRegionAllocPtr++,
            left,
            top,
            right,
            bottom,
            G_TX_RENDERTILE,
            texS,
            texT,
            (u16)(0x8000 / scaleXValue),
            (u16)(0x8000 / scaleYValue)
        );
    }
}

// drawMenuSpriteFixedScale best match: 98.565%
// (nonmatchings/drawMenuSpriteFixedScale-7189982689049576090/base_53.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuSpriteFixedScale.s")

#ifdef NON_MATCHING
/*
 * The empty conditions and single-line clipping block are IDO allocation and
 * frontend-lineage nudges. They do not change the rendering behavior.
 */
void drawMenuSpriteFixedScale(
    s16 x, s16 y, FontAsset *asset, u16 tileIndex,
    u16 scaleX, u16 scaleY, u8 flipMode, u8 unusedPalette
) {
    FontTexture *texture;
    FontTexture *textures;
    u8 *paletteBase;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 texS;
    s32 texT;
    s32 minX;
    s32 minY;
    s16 flipS;
    s16 flipT;
    s16 maxX;
    s16 maxY;

    textures = asset->textures;
    texture = &textures[tileIndex];
    paletteBase = asset->header.entryCount * sizeof(FontTexture) + (u8 *)asset->textures;
    if (scaleX >= 0xF001) {
        return;
    }
    if (scaleX < 0x10) {
        return;
    }
    if (scaleY >= 0xE801) {
        return;
    }
    if (scaleY < 0x10) {
        return;
    }

    flipS = ((s16 *)gMenuSpriteFlipScales)[(flipMode & 3) * 2 + 0];
    flipT = ((s16 *)gMenuSpriteFlipScales)[(flipMode & 3) * 2 + 1];

    texS = texture->width;
    texT = texture->height;
    left = (x + gMenuViewportCenterX) << 2;
    top = (y + gMenuViewportCenterY) << 2;
    right = left + (((scaleX * texS) << 2) / 0x1000);
    bottom = top + (((scaleY * texT) << 2) / 0x1000);

    texS = 0;
    texT = 0;
    if (flipS == -1) {
        texS = (texture->width - 1) << 5;
    }
    if (flipT == -1) {
        texT = (texture->height - 1) << 5;
    }

    minY = (s16)((gMenuViewportCenterY - (gMenuViewportHeight / 2)) << 2);
    maxY = (gMenuViewportCenterY + (gMenuViewportHeight / 2)) << 2;
    minX = (s16)((gMenuViewportCenterX - (gMenuViewportWidth / 2)) << 2);
    maxX = (gMenuViewportCenterX + (gMenuViewportWidth / 2)) << 2;

    if ((left < maxX) && (top < maxY) && (right >= minX) && (bottom >= minY)) {
        if (left < minX) {         if (flipMode && flipMode) {         }             texS = (((minX - left) << 3) << 12) / scaleX;             if (flipS == -1) {                 texS = ((texture->width - 1) << 5) - texS;                 if (top) {                 }             }             left = minX;         }         if (top < minY) {             texT = (((minY - top) << 3) << 12) / scaleY;             if (flipT == -1) {                 texT = ((texture->height - 1) << 5) - texT;             }             top = minY;         }         if (right >= maxX) {             right = maxX - 4;         }         if (bottom >= maxY) {             bottom = maxY - 4;         }          {             u16 scaleYValue;             u16 scaleYStep;              gDPLoadTextureTile_4b(             gRegionAllocPtr++, texture->imageOffset + (u8 *)asset + 0x80000000,             G_IM_FMT_CI, texture->width, texture->height,             0, 0, texture->width, texture->height, 0,             G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,             G_TX_NOLOD, G_TX_NOLOD         );             gDPLoadTLUT_pal16(                 gRegionAllocPtr++, 0,             paletteBase + (texture->paletteIndex << 5) + 0x80000000         );
            scaleYValue = 0x400000 / scaleX;
            scaleYStep = scaleY;
            gSPTextureRectangle(
                gRegionAllocPtr++, left, top, right, bottom, G_TX_RENDERTILE,
                texS, texT,
                (u16)(scaleYValue * flipS),
                (u16)((u16)(0x400000 / scaleYStep) * flipT)
            );
        }
    }
}
#endif

void drawMenuSpriteTile(s16 arg0, s16 arg1, void *arg2, u16 arg3, u16 arg4, u16 arg5) {
    drawMenuSpriteTileClipped(arg0, arg1, arg2, arg3, arg4, arg5, gMenuViewportWidth / 2, gMenuViewportHeight / 2);
}

void drawMenuSpriteTileClipped(
    s16 x,
    s16 y,
    MenuFontAssetTable *table,
    u16 entryIndex,
    u16 unused,
    u16 intensity,
    s16 clipX,
    s16 clipY
) {
    MenuFontAssetEntry *entry;
    volatile s32 padding2;
    u8 *paletteBase;
    volatile u8 padding0[0x18];
    s32 y0;
    s32 x1;
    s32 y1;
    s32 clipS;
    s32 clipT;
    volatile u8 padding1[8];
    s32 x0;
    s16 minX;
    s16 minY;
    s16 maxX;
    s16 maxY;

    paletteBase = (u8 *)(table->entryCount + table->entries);
    entry = &table->entries[entryIndex];
    entry += 0;
    clipS = entry->width;
    x1 = x0 = x + gMenuViewportCenterX;
    y0 = y + gMenuViewportCenterY;
    x1 += clipS;
    y1 = y0 + entry->height;
    clipS = 0;
    clipT = 0;
    minX = gMenuViewportCenterX - clipX;
    maxX = gMenuViewportCenterX + clipX;
    minY = gMenuViewportCenterY - clipY;
    maxY = gMenuViewportCenterY + clipY;

    if (minX < gMenuViewportCenterX - (gMenuViewportWidth / 2)) {
        minX = gMenuViewportCenterX - (gMenuViewportWidth / 2);
    }
    if (gMenuViewportCenterX + (gMenuViewportWidth / 2) < maxX) {
        maxX = gMenuViewportCenterX + (gMenuViewportWidth / 2);
    }
    if (minY < gMenuViewportCenterY - (gMenuViewportHeight / 2)) {
        minY = gMenuViewportCenterY - (gMenuViewportHeight / 2);
    }
    if (gMenuViewportCenterY + (gMenuViewportHeight / 2) < maxY) {
        maxY = gMenuViewportCenterY + (gMenuViewportHeight / 2);
    }

    if (x0 >= maxX) {
        return;
    }
    if (y0 >= maxY) {
        return;
    }
    if (x1 < minX) {
        return;
    }
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
        x1 = maxX - 1;
    }
    if (y1 >= maxY) {
        y1 = maxY - 1;
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
    if (intensity != 0x100) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, intensity, intensity, intensity, 0xFF);
    }
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
    if (intensity != 0x100) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

void func_80011854(void) {
}

void drawMenuTextureByAssetId(s16 x, s16 y, void *texture, u16 assetId, u16 width, u16 height) {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    volatile u8 pad[0x10];
    s16 clipX0;
    s16 clipY0;
    s16 clipX1;
    s16 clipY1;
    void *image;
    void *palette;

    if (width >= 0x201) {
        return;
    }
    if (width <= 0) {
        return;
    }
    if (height >= 0x201) {
        return;
    }
    if (height <= 0) {
        return;
    }

    x0 = (x + gMenuViewportCenterX) << 2;
    y0 = (y + gMenuViewportCenterY) << 2;
    x1 = x0 + (((width * 64) << 2) >> 5);
    y1 = y0 + (((height * 64) << 2) >> 5);

    clipY0 = (gMenuViewportCenterY - (gMenuViewportHeight / 2)) << 2;
    clipY1 = (gMenuViewportCenterY + (gMenuViewportHeight / 2)) << 2;
    clipX0 = (gMenuViewportCenterX - (gMenuViewportWidth / 2)) << 2;
    clipX1 = (gMenuViewportCenterX + (gMenuViewportWidth / 2)) << 2;

    if ((x0 >= clipX1) || (y0 >= clipY1) || (x1 < clipX0) || (y1 < clipY0)) {
        return;
    }
    if (x0 < clipX0) {
        x0 = clipX0;
    }
    if (y0 < clipY0) {
        y0 = clipY0;
    }
    if (x1 >= clipX1) {
        x1 = clipX1 - 4;
    }
    if (y1 >= clipY1) {
        y1 = clipY1 - 4;
    }

    getAssetTableImageAndPalette((void *)getRelocatableHeapBlockBase(gAssetHandles[13]), assetId, &image, &palette);

    FONT_GFX_CMD(gRegionAllocPtr++, 0xFD500000, (u32)&image);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xF5500000, 0x07080200);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xF3000000, 0x073FF200);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xF5400800, 0x00080200);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000, 0x000FC0FC);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)palette);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
    FONT_GFX_CMD(
        gRegionAllocPtr++,
        0xE4000000 | ((x1 & 0xFFF) << 12) | (y1 & 0xFFF),
        ((x0 & 0xFFF) << 12) | (y0 & 0xFFF)
    );
    FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000, 0);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xB3000000, 0x04000400);
}

void func_80011C18(MenuRenderSpriteActor *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateMenuSpriteActorDebugControls);
}

void updateMenuSpriteActorDebugControls(MenuRenderSpriteActor *actor) {
    if (gPlayerInputHeld[0] & U_CBUTTONS) {
        actor->sprite.y++;
    }
    if (gPlayerInputHeld[0] & D_CBUTTONS) {
        actor->sprite.y--;
    }
    if (gPlayerInputHeld[0] & R_CBUTTONS) {
        actor->sprite.x++;
    }
    if (gPlayerInputHeld[0] & L_CBUTTONS) {
        actor->sprite.x--;
    }

    if (actor->sprite.x >= 0x141) {
        actor->sprite.x = 0;
    }
    if (actor->sprite.x < 0) {
        actor->sprite.x = 0x13F;
    }
    if (actor->sprite.y >= 0x9C1) {
        actor->sprite.y = 0;
    }
    if (actor->sprite.y < 0) {
        actor->sprite.y = 0x9BF;
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawMenuTilemapSpriteCallback, &actor->sprite);
}

void drawMenuTilemapSpriteCallback(MenuRenderSprite *arg0) {
    drawMenuTilemapSprite(arg0, 0, 0, 0);
}

void func_80011D6C(void) {
}

void drawMenuTilemapSprite(MenuRenderSprite *sprite, s32 imageSize, s16 tilemapWidth, s16 tilemapHeight) {
    s16 sourceX;
    s16 sourceY;
    s16 drawX;
    s16 drawY;
    s16 clipLeft;
    s16 clipTop;
    s16 clipRight;
    s16 clipBottom;
    u16 tileMask;
    s16 firstDrawX;
    s16 columnCount;
    s16 rowCount;
    s16 column;
    s16 row;
    s16 scaleS;
    s16 scaleT;
    s16 rectLeft;
    s16 rectTop;
    s16 rectRight;
    s16 rectBottom;
    s16 tileId;
    s16 texS;
    s16 texT;
    s16 imageIndex;
    s16 tileX;
    s16 tileY;
    u16 tileShift;
    s16 *tilemap;
    u16 *image;
    s32 sourceOffsetX;
    MenuRenderSprite *render;
    u32 paletteIndex;
    MenuRenderTileInfo *tileInfo;
    u8 *palette;

    tilemap = (s16 *)(render = sprite)->tilemap;
    image = render->image;
    tileInfo = (MenuRenderTileInfo *)render->tileInfo;
    palette = (u8 *)render->palette;

    clipLeft = render->clipX;
    sourceX = render->x;
    if (clipLeft < -gMenuViewportWidth / 2) {
        clipLeft = -gMenuViewportWidth / 2;
        sourceX = (sourceX + clipLeft) - render->clipX;
    }

    clipTop = render->clipY;
    sourceY = render->y;
    if (clipTop < -gMenuViewportHeight / 2) {
        clipTop = -gMenuViewportHeight / 2;
        sourceY = (sourceY + clipTop) - render->clipY;
    }

    clipRight = render->clipX + render->width;
    if (clipRight > gMenuViewportWidth / 2) {
        clipRight = gMenuViewportWidth / 2;
    }

    clipBottom = render->clipY + render->height;
    if (clipBottom > gMenuViewportHeight / 2) {
        clipBottom = gMenuViewportHeight / 2;
    }

    clipLeft += gMenuViewportCenterX;
    clipRight += gMenuViewportCenterX;
    clipTop += gMenuViewportCenterY;
    clipBottom += gMenuViewportCenterY;

    tileMask = render->tileSize - 1;
    if (render->tileSize == 0x10) {
        tileShift = 4;
    } else {
        tileShift = 5;
    }

    sourceOffsetX = sourceX & tileMask;
    columnCount = ((clipRight - clipLeft) + tileMask - 1) >> tileShift;
    rowCount = ((clipBottom - clipTop) + tileMask - 1) >> tileShift;

    if (sourceX & tileMask) {
        columnCount++;
    }
    firstDrawX = clipLeft - sourceOffsetX;

    if (sourceY & tileMask) {
        rowCount++;
    }
    drawY = clipTop - (sourceY & tileMask);
    tileY = (sourceY >> tileShift) % tilemapHeight;

    for (row = 0; row < rowCount; row++) {
        drawX = firstDrawX;
        tileX = (sourceX >> tileShift) % tilemapWidth;

        for (column = 0; column < columnCount; column++) {
            imageIndex = tileX + (tileY * render->tileYStep);
            tileId = tilemap[imageIndex];
            paletteIndex = tileInfo[tileId].paletteIndex;
            imageIndex = tileInfo[tileId].imageIndex;

            if (tileId != 0) {
                scaleS = ((s16 *)gMenuSpriteFlipScales)[tileInfo[tileId].flip * 2 + 0];
                scaleT = ((s16 *)gMenuSpriteFlipScales)[tileInfo[tileId].flip * 2 + 1];
                rectLeft = drawX;
                rectTop = drawY;
                rectRight = drawX + render->tileSize;
                rectBottom = drawY + render->tileXStep;
                texS = 0;
                texT = 0;

                if (scaleS == -1) {
                    texS = render->tileSize - 1;
                }
                if (scaleT == -1) {
                    texT = render->tileXStep - 1;
                }

                if ((drawX < clipRight) && (drawY < clipBottom) && (rectRight >= clipLeft) &&
                    (rectBottom >= clipTop)) {
                    if (drawX < clipLeft) {
                        texS = clipLeft - drawX;
                        if (scaleS == -1) {
                            texS = (render->tileSize - texS) - 1;
                        }
                        rectLeft = clipLeft;
                    }

                    if (drawY < clipTop) {
                        texT = clipTop - drawY;
                        if (scaleT == -1) {
                            texT = (render->tileXStep - texT) - 1;
                        }
                        rectTop = clipTop;
                    }

                    if (rectRight >= clipRight) {
                        rectRight = clipRight - 1;
                    }
                    if (rectBottom >= clipBottom) {
                        rectBottom = clipBottom - 1;
                    }

                    if ((u8)imageSize == 0) {
                        gDPLoadTLUT_pal16(gRegionAllocPtr++, paletteIndex, palette + (paletteIndex << 5));
                        gDPLoadTextureTile_4b(
                            gRegionAllocPtr++,
                            &image[((imageIndex - 1) * render->tileSize * render->tileXStep) / 4],
                            G_IM_FMT_CI,
                            render->tileSize,
                            render->tileXStep,
                            0,
                            0,
                            render->tileSize,
                            render->tileXStep,
                            paletteIndex,
                            G_TX_CLAMP,
                            G_TX_CLAMP,
                            G_TX_NOMASK,
                            G_TX_NOMASK,
                            G_TX_NOLOD,
                            G_TX_NOLOD
                        );
                    } else {
                        if (tilemapHeight) {
                        }
                        gDPLoadTLUT_pal256(gRegionAllocPtr++, palette + (paletteIndex << 5));
                        gDPLoadTextureTile(
                            gRegionAllocPtr++,
                            &image[((imageIndex - 1) * render->tileSize * render->tileXStep) / 2],
                            G_IM_FMT_CI,
                            G_IM_SIZ_8b,
                            render->tileSize,
                            render->tileXStep,
                            0,
                            0,
                            render->tileSize,
                            render->tileXStep,
                            0,
                            G_TX_CLAMP,
                            G_TX_CLAMP,
                            G_TX_NOMASK,
                            G_TX_NOMASK,
                            G_TX_NOLOD,
                            G_TX_NOLOD
                        );
                    }

                    gSPTextureRectangle(
                        gRegionAllocPtr++,
                        rectLeft << 2,
                        rectTop << 2,
                        rectRight << 2,
                        rectBottom << 2,
                        G_TX_RENDERTILE,
                        texS << 5,
                        texT << 5,
                        (u16)(scaleS << 10),
                        (u16)(scaleT << 10)
                    );
                }
            }

            drawX += render->tileSize;
            tileX = (tileX + 1) % tilemapWidth;
        }

        drawY += render->tileXStep;
        tileY = (tileY + 1) % tilemapHeight;
    }
}

void drawMenuGlyphScriptWithFontBank(
    volatile s16 x,
    s16 y,
    MenuGlyphScript *script,
    s32 palette,
    u16 scale,
    u16 fontBank
) {
    u16 firstGlyph;
    s32 glyphCode;
    MenuGlyphScript *scriptCursor;
    s32 drawX;
    u16 glyphAdvance;
    s32 drawY;
    u16 scaleValue;
    s32 advance;
    u16 fontBankValue;

    drawX = x;
    drawY = y;
    if (MENU_GLYPH_PALETTE_INDEX(palette) == 0) {
        glyphAdvance = MENU_GLYPH_WIDE_ADVANCE;
    } else {
        glyphAdvance = MENU_GLYPH_NARROW_ADVANCE;
    }

    firstGlyph = *script ^ 0;
    if (firstGlyph != MENU_GLYPH_SCRIPT_END) {
        scriptCursor = script;
        scaleValue = scale;
        fontBankValue = fontBank;
        glyphCode = firstGlyph;
        do {
            if ((glyphCode & 0xFFFF) == MENU_GLYPH_SCRIPT_NEWLINE) {
                drawX = x;
                drawY += MENU_GLYPH_LINE_HEIGHT;
            } else {
                advance = glyphAdvance;
                if ((glyphCode & 0xFFFF) != MENU_GLYPH_SCRIPT_SPACE) {
                    drawMenuGlyph(
                        drawX,
                        drawY,
                        glyphCode & 0xFFFF,
                        MENU_GLYPH_PALETTE_INDEX(palette),
                        scaleValue,
                        fontBankValue
                    );
                }
                drawX += advance;
            }
            glyphCode = scriptCursor[1];
            scriptCursor++;
        } while ((glyphCode & 0xFFFF) != MENU_GLYPH_SCRIPT_END);
    }
}

void drawMenuGlyphScriptDefaultFont(volatile s16 x, s16 y, MenuGlyphScript *script, s32 palette, u16 scale) {
    u16 initialGlyph;
    s32 currentGlyph;
    MenuGlyphScript *cursor;
    s32 drawX;
    u16 glyphAdvance;
    s32 drawY;
    u16 paletteScale;

    drawX = x;
    drawY = y;
    if (MENU_GLYPH_PALETTE_INDEX(palette) == 0) {
        glyphAdvance = MENU_GLYPH_WIDE_ADVANCE;
    } else {
        glyphAdvance = MENU_GLYPH_NARROW_ADVANCE;
    }

    // The no-op preserves IDO's initial-glyph register allocation.
    initialGlyph = *script ^ 0;
    if (initialGlyph != MENU_GLYPH_SCRIPT_END) {
        cursor = script;
        paletteScale = scale;
        currentGlyph = initialGlyph;
        do {
            if ((currentGlyph & 0xFFFF) == MENU_GLYPH_SCRIPT_NEWLINE) {
                drawX = x;
                drawY += MENU_GLYPH_LINE_HEIGHT;
            } else {
                if ((currentGlyph & 0xFFFF) != MENU_GLYPH_SCRIPT_SPACE) {
                    drawMenuGlyph(
                        drawX,
                        drawY,
                        currentGlyph & 0xFFFF,
                        MENU_GLYPH_PALETTE_INDEX(palette),
                        paletteScale,
                        MENU_GLYPH_DEFAULT_FONT_BANK
                    );
                }
                drawX += glyphAdvance;
            }
            cursor++;
            currentGlyph = *cursor;
        } while ((currentGlyph & 0xFFFF) != MENU_GLYPH_SCRIPT_END);
    }
}

void drawMenuGlyph(s16 x, s16 y, u16 glyphIndex, u8 paletteIndex, u16 intensity, u16 fontBank) {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 clipS;
    s32 clipT;
    s32 color;
    s32 i;
    u16 paletteColor;
    u8 new_var;
    MenuGlyphPalette *paletteBase;
    FontAsset *font;
    MenuGlyphPalette *srcPalette;
    u16 *scaledPalette;
    FontTexture *entry;
    u16 red;
    u16 green;
    u16 blue;

    if (paletteIndex == 0) {
        font = getRelocatableHeapBlockBase(gAssetHandles[fontBank]);
        color = 0x10;
    } else {
        font = getRelocatableHeapBlockBase(gAssetHandles[fontBank + 1]);
        color = 8;
    }

    paletteBase = (MenuGlyphPalette *)(font->header.entryCount + font->textures);
    x1 = x0 = x + gMenuViewportCenterX;
    y0 = y + gMenuViewportCenterY;
    x1 = color + x0;
    y1 = y0 + 0x10;
    clipS = 0;
    clipT = 0;

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
        clipS = (gMenuViewportCenterX - (gMenuViewportWidth / 2)) - x0;
        x0 = gMenuViewportCenterX - (gMenuViewportWidth / 2);
    }
    if (y0 < gMenuViewportCenterY - (gMenuViewportHeight / 2)) {
        clipT = (gMenuViewportCenterY - (gMenuViewportHeight / 2)) - y0;
        y0 = gMenuViewportCenterY - (gMenuViewportHeight / 2);
    }
    if (x1 >= gMenuViewportCenterX + (gMenuViewportWidth / 2)) {
        x1 = (gMenuViewportCenterX + (gMenuViewportWidth / 2)) - 1;
    }
    if (y1 >= gMenuViewportCenterY + (gMenuViewportHeight / 2)) {
        y1 = (gMenuViewportCenterY + (gMenuViewportHeight / 2)) - 1;
    }

    entry = font->textures;
    entry += glyphIndex;
    srcPalette = &paletteBase[entry->paletteIndex];
    scaledPalette = allocMenuRenderScratch(0x20);
    for (i = 0; i < 0x10; i++) {
        scaledPalette[i] = srcPalette->colors[i];

        if (scaledPalette[i] & 1) {
            red = (scaledPalette[i] >> 11) & 0x1F;
            green = (scaledPalette[i] >> 6) & 0x1F;
            blue = (scaledPalette[i] >> 1) & 0x1F;
            red = (red * intensity) / 0x100;
            green = (green * intensity) / 0x100;
            blue = (blue * intensity) / 0x100;
            scaledPalette[i] = (((red << 11) | (green << 6)) | (blue << 1)) | 1;
        }
    }

    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, scaledPalette);
    gDPLoadTextureTile_4b(
        gRegionAllocPtr++,
        entry->imageOffset + (u8 *)font,
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

void drawMenuColoredGlyphScript(
    volatile s16 x,
    s16 y,
    MenuGlyphScript *script,
    s32 palette,
    u16 scale,
    u16 colorMode,
    u16 fontBank
) {
    u16 firstGlyph;
    s32 glyphCode;
    MenuGlyphScript *scriptCursor;
    s32 drawX;
    u16 glyphAdvance;
    s32 drawY;
    s32 spaceGlyph;
    s32 advance;
    u16 scaleValue;
    u16 colorModeValue;
    u16 fontBankValue;

    drawX = x;
    drawY = y;
    if (MENU_GLYPH_PALETTE_INDEX(palette) == 0) {
        glyphAdvance = MENU_GLYPH_WIDE_ADVANCE;
    } else {
        glyphAdvance = MENU_GLYPH_NARROW_ADVANCE;
    }

    firstGlyph = *script ^ 0;
    if (MENU_GLYPH_SCRIPT_END != firstGlyph) {
        scriptCursor = script;
        scaleValue = scale;
        glyphCode = firstGlyph;
        colorModeValue = colorMode;
        fontBankValue = fontBank;
        do {
            spaceGlyph = MENU_GLYPH_SCRIPT_SPACE;
            if (MENU_GLYPH_SCRIPT_NEWLINE == (glyphCode & 0xFFFF)) {
                drawX = x;
                drawY += MENU_GLYPH_LINE_HEIGHT;
            } else {
                advance = glyphAdvance;
                if (spaceGlyph != (glyphCode & 0xFFFF)) {
                    drawMenuColoredGlyph(
                        drawX,
                        drawY,
                        glyphCode & 0xFFFF,
                        MENU_GLYPH_PALETTE_INDEX(palette),
                        scaleValue,
                        colorModeValue,
                        fontBankValue
                    );
                }
                drawX += advance;
            }
            glyphCode = scriptCursor[1];
            scriptCursor++;
        } while (MENU_GLYPH_SCRIPT_END != (glyphCode & 0xFFFF));
    }
}

void drawMenuGlyphScript(
    volatile s16 x,
    s16 y,
    MenuGlyphScript *script,
    s32 palette,
    u16 scale,
    volatile u16 colorMode
) {
    u16 firstGlyph;
    s32 glyphCode;
    MenuGlyphScript *scriptCursor;
    s32 drawX;
    s32 drawY;
    s32 spaceGlyph;
    register s32 advance;
    u16 glyphAdvance;
    u16 scaleValue;
    u16 colorModeValue;

    drawX = x;
    drawY = y;
    if (MENU_GLYPH_PALETTE_INDEX(palette) == 0) {
        glyphAdvance = MENU_GLYPH_WIDE_ADVANCE;
    } else {
        glyphAdvance = MENU_GLYPH_NARROW_ADVANCE;
    }

    firstGlyph = *script ^ 0;
    if (MENU_GLYPH_SCRIPT_END != firstGlyph) {
        scriptCursor = script;
        scaleValue = scale;
        glyphCode = firstGlyph;
        colorModeValue = colorMode;
        if (scale) {}
        do {
            spaceGlyph = MENU_GLYPH_SCRIPT_SPACE;
            if (MENU_GLYPH_SCRIPT_NEWLINE == (glyphCode & 0xFFFF)) {
                drawX = x;
                drawY += MENU_GLYPH_LINE_HEIGHT;
            } else if (spaceGlyph == (glyphCode & 0xFFFF)) {
                drawX += glyphAdvance;
                if (0) {}
            } else if ((glyphCode & 0xFFFF) == MENU_GLYPH_SCRIPT_COLOR) {
                colorModeValue = scriptCursor[1];
                scriptCursor++;
            } else {
                drawMenuColoredGlyph(
                    drawX,
                    drawY,
                    glyphCode & 0xFFFF,
                    MENU_GLYPH_PALETTE_INDEX(palette),
                    scaleValue,
                    colorModeValue,
                    MENU_GLYPH_DEFAULT_FONT_BANK
                );
                drawX += (advance = glyphAdvance);
            }
            glyphCode = scriptCursor[1];
            scriptCursor++;
        } while (MENU_GLYPH_SCRIPT_END != (glyphCode & 0xFFFF));
        colorMode = colorModeValue;
    }
}

void drawMenuColoredGlyph(s16 x, s16 y, u16 glyph, u8 palette, u16 paletteScale, u16 paletteIndex, s32 fontBank) {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 clipS;
    s32 clipT;
    s32 glyphWidth;
    s32 i;
    u16 paletteColor;
    u8 new_var;
    MenuGlyphPalette *paletteBase;
    MenuFontAssetTable *font;
    MenuGlyphPalette *srcPalette;
    u16 *scaledPalette;
    MenuFontAssetEntry *entry;
    s32 color;
    u16 red;
    u16 green;
    u16 blue;
    u16 paletteScaleValue;
    s32 viewHalfWidth;
    s32 viewHalfHeight;
    s32 minX;
    s32 maxX;
    s32 minY;
    s32 viewHalfHeightValue;
    s32 maxY;
    u16 paletteIndexValue;

    if (palette == 0) {
        font = getRelocatableHeapBlockBase(gAssetHandles[(u16)fontBank]);
        glyphWidth = 0x10;
    } else {
        font = getRelocatableHeapBlockBase(gAssetHandles[((u16)fontBank) + 1]);
        glyphWidth = 8;
    }
    paletteBase = (MenuGlyphPalette *)(font->entryCount + font->entries);
    srcPalette = &paletteBase[paletteIndex];
    x0 = x + gMenuViewportCenterX;
    y0 = y + gMenuViewportCenterY;
    x1 = glyphWidth + x0;
    y1 = y0 + 0x10;
    clipS = 0;
    clipT = 0;
    viewHalfWidth = gMenuViewportWidth / 2;
    maxX = gMenuViewportCenterX + viewHalfWidth;
    if (x0 < maxX) {
        viewHalfHeightValue = gMenuViewportHeight / 2;
        minX = gMenuViewportCenterX - viewHalfWidth;
        maxY = gMenuViewportCenterY + viewHalfHeightValue;
        if ((y0 < maxY) && (x1 >= minX)) {
            minY = gMenuViewportCenterY - viewHalfHeightValue;
            if (y1 >= minY) {
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
                scaledPalette = allocMenuRenderScratch(0x20);
                for (i = 0; i < 0x10; i++) {
                    scaledPalette[i] = srcPalette->colors[i];

                    if (scaledPalette[i] & 1) {
                        red = (scaledPalette[i] >> 11) & 0x1F;
                        green = (scaledPalette[i] >> 6) & 0x1F;
                        blue = (scaledPalette[i] >> 1) & 0x1F;
                        red = (red * paletteScale) / 0x100;
                        green = (green * paletteScale) / 0x100;
                        blue = (blue * paletteScale) / 0x100;
                        scaledPalette[i] = (((red << 11) | (green << 6)) | (blue << 1)) | 1;
                    }
                }

                entry = &font->entries[glyph];
                if (1) {}

                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, scaledPalette);
                gDPLoadTextureTile_4b(
                    gRegionAllocPtr++,
                    entry->imageOffset + (u8 *)font,
                    2,
                    entry->width,
                    entry->height,
                    0,
                    0,
                    entry->width,
                    entry->height,
                    0,
                    0x2,
                    0x2,
                    0,
                    0,
                    0,
                    0
                );
                gSPTextureRectangle(
                    gRegionAllocPtr++,
                    x0 << 2,
                    y0 << 2,
                    x1 << 2,
                    y1 << 2,
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

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void drawMenuAsciiGlyph(s16 x, s16 y, u16 tileS, s32 tileT, u16 palette, u16 paletteScale) {
    s32 x0;
    s32 storedY;
    s32 x1;
    s32 y1;
    s32 clipS;
    s32 clipT;
    s32 maxX;
    volatile u16 *dst;
    FontTexture *texture;
    MenuGlyphPalette *palettes;
    FontAsset *font;
    s32 maxY;
    s32 i;
    s16 paletteIndex;

    font = getRelocatableHeapBlockBase(gAssetHandles[6]);
    palettes = (MenuGlyphPalette *)(font->header.entryCount + font->textures);
    paletteIndex = *(s16 *)&font->textures[0].paletteIndex;
    x0 = x + gMenuViewportCenterX;
    i = y + gMenuViewportCenterY;
    texture = &font->textures[0];
    x1 = x0 + 8;
    y1 = i + 8;
    clipS = 0;
    clipT = 0;

    {
        s32 minX;
        s32 minY;
        s32 halfHeight;

        maxX = gMenuViewportCenterX + (gMenuViewportWidth / 2);
        if (x0 < maxX) {
            halfHeight = gMenuViewportHeight / 2;
            maxY = gMenuViewportCenterY + halfHeight;
            minX = gMenuViewportCenterX - (gMenuViewportWidth / 2);
            if (i < maxY) {
                minY = gMenuViewportCenterY - halfHeight;
                if ((x1 >= minX) && (y1 >= minY)) {
                    s32 colorValue;
                    MenuGlyphPalette *scratch;
                    MenuGlyphPalette *source;
                    s32 color;
                    s32 red;
                    u16 green;
                    u16 blue;
                    s32 scaleValue;
                    u16 scaledRed;

                    if (x0 < minX) {
                        clipS = minX - x0;
                        x0 = minX;
                    }
                    if (i < minY) {
                        clipT = minY - i;
                        i = minY;
                    }
                    if (x1 >= maxX) {
                        x1 = maxX - 1;
                    }
                    if (y1 >= maxY) {
                        y1 = maxY - 1;
                    }
                    clipS += tileS;
                    clipT += (u16)tileT;

                    if (paletteIndex != palette) {
                        paletteIndex = palette;
                    }
                    storedY = i;
                    if (1) {
                    scratch = allocMenuRenderScratch(sizeof(MenuGlyphPalette)); i = 0; source = &palettes[paletteIndex]; dst = scratch->colors; palette_loop: *dst = (color = (*(u16 *)&source->bytes[i]) & 0xFFFFu); i += sizeof(u16); if ((colorValue = color & 0xFFFF) & 1) { red = ((colorValue >> 11) & 0x1F) & 0xFFFF;
                        green = (colorValue >> 6) & 0x1F;
                        colorValue = (blue = (colorValue >> 1) & 0x1F);
                        scaleValue = paletteScale;
                        red = red * scaleValue;
                        scaledRed = red / 0x100;
                        red = scaledRed;
                        green = (green * paletteScale) / 0x100;
                        colorValue = green;
                        blue = (blue * paletteScale) / 0x100;
                        *dst = (red << 11) | (colorValue << 6) | (blue << 1) | 1;
                    }
                    dst += 2;
                    dst--;
                    if (i != sizeof(MenuGlyphPalette)) {
                        goto palette_loop;
                    }

                    gDPLoadTextureTile_4b(gRegionAllocPtr++, texture->imageOffset + (u8 *)font,
                                          G_IM_FMT_CI, texture->width, texture->height,
                                          0, 0, texture->width, texture->height, 0,
                                          G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                                          G_TX_NOLOD, G_TX_NOLOD);
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, scratch);
                    gSPTextureRectangle(gRegionAllocPtr++, x0 << 2, storedY << 2, x1 << 2, y1 << 2,
                                        G_TX_RENDERTILE, clipS << 5, clipT << 5, 0x400, 0x400);
                    }
                }
            }
        }
    }
}
// clang-format on

void drawMenuAsciiText(s16 arg0, s16 arg1, u8 *arg2, u16 arg3, u16 arg4) {
    s32 var_s0;
    u8 *var_s1;
    char mask;
    s32 var_s2;
    s32 temp_s3;
    s32 temp_t9;
    long var_v0;
    s32 var_a2;
    s32 newline;
    s32 var_v1;

    mask = 0xFFFFFFFFFFFFFFFFu;
    temp_t9 = *arg2 & mask;
    var_s0 = arg0;
    var_s1 = arg2;
    var_s2 = arg1;
    temp_s3 = var_s0;
    if ((0, temp_t9) != 0) {
        var_v0 = temp_t9 & 0xFF;
        newline = 0xA;
        do {
            var_v1 = var_v0;
            if (newline == var_v0) {
                var_s0 = temp_s3;
                var_s2 += 8;
            } else {
                var_v0 = var_v1 - 0x20;
                if ((u32)var_v0 < 0x40) {
                    var_a2 = var_v0 & 7;
                    drawMenuAsciiGlyph(var_s0, var_s2, ((var_a2 << 3) & 0xFFFF) & 0xFFFF, var_v0 & 0x38, arg3, arg4);
                }
                var_s0 += 8;
            }
            var_v0 = var_s1[1];
            var_s1++;
        } while (var_v0 != 0);
    }
}

void drawMenuSolidRect(s16 x0, s16 y0, s16 x1, s16 y1, s16 r, s16 g, s16 b) {
    if ((x0 >= 0) && (x0 < 321) && (x1 >= 0) && (x1 < 321) && (y0 >= 0) && (y0 < 241) && (y1 >= 0) && (y1 < 241)) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCycleType(gRegionAllocPtr++, G_CYC_FILL);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_NOOP, G_RM_NOOP2);
        gDPSetFillColor(gRegionAllocPtr++, (GPACK_RGBA5551(r, g, b, 1) << 16) | GPACK_RGBA5551(r, g, b, 1));
        gDPFillRectangle(gRegionAllocPtr++, x0, y0, x1, y1);
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

s32 stepMenuFadeAlpha(s32 value, s16 step, u8 increase) {
    if (increase) {
    } else {
        step = step * -1;
    }

    value += step;
    if (increase) {
        if (value >= 0x100) {
            value = 0xFF;
        }
    } else if (value < 0) {
        value = 0;
    }

    gMenuFadeAlpha = value;
    return value;
}

void drawMenuSpriteCrossfade(s16 x, s16 y, MenuFontAssetTable *table, u16 imageIndex0, u16 imageIndex1, u8 alpha) {
    MenuFontAssetEntry *entry0;
    MenuFontAssetEntry *entry1;
    MenuFontAssetEntry *paletteBase;
    s32 minX;
    s32 maxX;
    s32 minY;
    s32 maxY;
    s32 x0;
    s32 y0;
    s32 halfWidth;
    s32 x1;
    s32 y1;
    s32 clipS;
    s32 clipT;
    s32 halfHeight;
    s32 pad;
    s32 pad2;

    paletteBase = table->entries + (0, table->entryCount);
    entry0 = &table->entries[imageIndex0++];
    entry1 = &table->entries[imageIndex1];

    clipS = entry0->width;
    x1 = x0 = x + gMenuViewportCenterX;
    y0 = y + gMenuViewportCenterY;
    x1 += clipS;
    y1 = y0 + entry0->height;

    clipS = 0;
    clipT = 0;
    halfWidth = gMenuViewportWidth / 2;
    maxX = gMenuViewportCenterX + halfWidth;
    if (x0 >= maxX) {
        return;
    }

    halfHeight = gMenuViewportHeight / 2;
    maxY = gMenuViewportCenterY + halfHeight;
    minX = gMenuViewportCenterX - halfWidth;
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
        do {
            clipS = minX - x0;
        } while (0);
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

    gDPPipeSync(gRegionAllocPtr++);
    gDPSetCycleType(gRegionAllocPtr++, G_CYC_2CYCLE);
    gDPSetTextureLOD(gRegionAllocPtr++, G_TL_TILE);
    gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0, 0, 0, alpha);
    gDPSetCombineLERP(
        gRegionAllocPtr++,
        TEXEL1,
        TEXEL0,
        PRIMITIVE_ALPHA,
        TEXEL0,
        TEXEL1,
        TEXEL0,
        PRIMITIVE,
        TEXEL0,
        0,
        0,
        0,
        COMBINED,
        0,
        0,
        0,
        COMBINED
    );

    gDPLoadTextureTile(
        gRegionAllocPtr++,
        (u8 *)table + (0, entry0->imageOffset),
        G_IM_FMT_RGBA,
        G_IM_SIZ_8b,
        entry0->width,
        entry0->height,
        0,
        0,
        entry0->width,
        entry0->height,
        0,
        G_TX_WRAP,
        G_TX_WRAP,
        G_TX_NOMASK,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOLOD
    );
    gDPLoadMultiTile(
        gRegionAllocPtr++,
        (u8 *)table + (0, entry1->imageOffset),
        0x100,
        G_TX_RENDERTILE + 1,
        G_IM_FMT_RGBA,
        G_IM_SIZ_8b,
        entry1->width,
        entry1->height,
        0,
        0,
        entry1->width,
        entry1->height,
        0,
        G_TX_WRAP,
        G_TX_WRAP,
        G_TX_NOMASK,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOLOD
    );
    gDPLoadTLUT_pal256(gRegionAllocPtr++, (entry1->textureIndex << 5) + (u8 *)paletteBase);
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
