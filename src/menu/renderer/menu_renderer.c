#include "common.h"
#include "game/engine/render_callback.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/relocatable_heap.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"

#define FONT_GFX_CMD(pkt, cmd0, cmd1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (cmd0); \
    _g->words.w1 = (cmd1); \
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
typedef struct FontAsset FontAsset;
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

typedef void (*MenuRenderSpriteActorCallback)(MenuRenderSpriteActor *);
typedef void (*MenuRenderCallback)(MenuRenderSprite *);

extern void *allocMenuRenderScratch(s32 size);
s32 drawMenuTilemapSprite(MenuRenderSprite *sprite, s32 arg1, s16 x, s16 y);
void drawMenuSpriteClipped(s16 arg0, s16 arg1, void *arg2, u16 arg3, u16 arg4, u16 arg5, u8 arg6, u8 arg7, s32 arg8, s32 arg9,
                   s32 argA, s32 argB);
void drawMenuSpriteWithAlphaClipped(s16 arg0, s16 arg1, void *arg2, u16 arg3, u16 arg4, u16 arg5, u8 arg6, u16 arg7, u8 arg8,
                   s32 arg9, s32 argA, s32 argB, s32 argC);
void drawMenuGlyph(s16 x, s16 y, u16 glyphIndex, u8 paletteIndex, u16 intensity, u16 fontBank);
void drawMenuColoredGlyph(s16 x, s16 y, u16 glyph, u8 palette, u16 scale, u16 colorMode, s32 arg6);
extern Gfx gMenuRenderModeResetDl[];
extern Gfx *gRegionAllocPtr;
extern s16 gAssetHandles[];
extern u32 gPlayerInputHeld;
extern s16 gMenuSpriteFlipScales[][2];
extern s16 gMenuFadeAlpha;
extern s16 gMenuViewportWidth;
extern s16 gMenuViewportHeight;
extern s16 gMenuViewportCenterX;
extern s16 gMenuViewportCenterY;
extern u16 gMenuTransparentPalette[];

void drawMenuAssetRegion(s16 x, s16 y, void *tableAddress, u16 entryIndex, u16 scaleX, u16 scaleY,
                         u8 startS, u8 startT, u8 width, u8 height) {
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
    texS = startS << 5; texT = startT << 5; minY = (s16)((gMenuViewportCenterY - (gMenuViewportHeight / 2)) << 2); maxY = (gMenuViewportCenterY + (gMenuViewportHeight / 2)) << 2; minX = (s16)((gMenuViewportCenterX - (gMenuViewportWidth / 2)) << 2);
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

        gDPLoadTextureTile_4b(gRegionAllocPtr++, entry->imageOffset + (u8 *)tableAddress + 0x80000000, G_IM_FMT_CI, entry->width,
                              entry->height, 0, 0, entry->width, entry->height, 0, G_TX_CLAMP, G_TX_CLAMP,
                              G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteBase + (entry->textureIndex << 5) + 0x80000000);
        gSPTextureRectangle(gRegionAllocPtr++, left, top, right, bottom, G_TX_RENDERTILE, texS, texT,
                            (u16)(0x8000 / scaleXValue), (u16)(0x8000 / scaleYValue));
    }
}

void drawMenuSprite(s16 arg0, s16 arg1, void *arg2, u16 arg3, u16 arg4, u16 arg5, u8 arg6, u8 arg7) {
    s32 temp_v0;
    s32 temp_v1;

    drawMenuSpriteClipped(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7,
                  temp_v0 = (s16)(gMenuViewportWidth / 2), temp_v1 = (s16)(gMenuViewportHeight / 2), temp_v0, temp_v1);
}

// drawMenuSpriteClipped best match: 80.151% (nonmatchings/drawMenuSpriteClipped-8498672362023432715/base_10.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuSpriteClipped.s")

#ifdef NON_MATCHING
void drawMenuSpriteClipped(s16 x, s16 y, MenuFontAssetTable *table, u16 imageIndex, u16 scaleX, u16 scaleY,
                           u8 flipMode, u8 paletteArg, s32 clipLeft, s32 clipTop, s32 clipRight,
                           s32 clipBottom) {
    MenuFontAssetEntry *texture;
    volatile s32 pad;
    u8 *paletteBase;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 texS;
    s32 texT;
    s16 minX;
    s16 minY;
    s16 maxX;
    s16 maxY;
    s16 flipS;
    s16 flipT;
    s32 texWidth;
    s32 texHeight;
    u16 palette;

    paletteBase = (u8 *)&table->entries[table->entryCount];
    if (scaleX >= 0x201) {
        return;
    }
    if (scaleX <= 0) {
        return;
    }
    if (scaleY >= 0x201) {
        return;
    }
    if (scaleY <= 0) {
        return;
    }
    {
        flipS = gMenuSpriteFlipScales[flipMode & 3][0];
        flipT = gMenuSpriteFlipScales[flipMode & 3][1];
        texture = &table->entries[imageIndex];
        texT = x + gMenuViewportCenterX;
        texWidth = texture->width;
        texHeight = texture->height;
        left = texT << 2;
        top = ((y + gMenuViewportCenterY) << 1) << 1;
        right = (((scaleX * texWidth) << 2) >> 5) + left;
        bottom = (((scaleY * texHeight) << 2) >> 5) + top;
        texS = 0;
        texT = 0;
        texT = 0;
        if (flipS == -1) {
            texS = (texWidth - 1) << 5;
        }
        if (flipT == -1) {
            texT = (texHeight - 1) << 5;
        }

        minY = gMenuViewportCenterY - (s16)clipTop;
        minX = gMenuViewportCenterX - (s16)clipLeft;
        maxY = gMenuViewportCenterY + (s16)clipBottom;
        maxX = gMenuViewportCenterX + (s16)clipRight;
        if (minX < gMenuViewportCenterX - (gMenuViewportWidth / 2)) {
            minX = gMenuViewportCenterX - (gMenuViewportWidth / 2);
        }
        if (maxX > gMenuViewportCenterX + (gMenuViewportWidth / 2)) {
            maxX = gMenuViewportCenterX + (gMenuViewportWidth / 2);
        }
        if (minY < gMenuViewportCenterY - (gMenuViewportHeight / 2)) {
            minY = gMenuViewportCenterY - (gMenuViewportHeight / 2);
        }
        if (maxY > gMenuViewportCenterY + (gMenuViewportHeight / 2)) {
            maxY = gMenuViewportCenterY + (gMenuViewportHeight / 2);
        }

        maxX = maxX << 2;
        maxY = maxY << 2;
        minX = minX << 2;
        minY = minY << 2;
        if ((left < maxX) && (top < maxY) && (right >= minX) && (bottom >= minY)) {
            if (left < minX) {
                texS = (((minX - left) << 3) << 5) / scaleX;
                if (flipS == -1) {
                    texS = ((texWidth - 1) << 5) - texS;
                }
                left = minX;
            }
            if (top < minY) {
                texT = (((minY - top) << 3) << 5) / scaleY;
                if (flipT == -1) {
                    texT = ((texHeight - 1) << 5) - texT;
                }
                top = minY;
            }
            if (right >= maxX) {
                right = maxX - 4;
            }
            if (bottom >= maxY) {
                bottom = maxY - 4;
            }

            if (paletteArg == 0) {
                palette = texture->textureIndex;
            } else {
                palette = paletteArg - 1;
            }

            gDPLoadTextureTile_4b(gRegionAllocPtr++, (u8 *)table + texture->imageOffset,
                                  G_IM_FMT_CI, texture->width, texture->height, 0, 0,
                                  texture->width, texture->height, 0, G_TX_CLAMP, G_TX_CLAMP,
                                  G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            if (palette != 0xFE) {
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteBase + (palette * 0x20));
            } else {
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gMenuTransparentPalette);
            }
            gSPTextureRectangle(gRegionAllocPtr++, left, top, right, bottom, G_TX_RENDERTILE,
                                texS, texT, (u16)((u16)(0x8000 / scaleX) * flipS),
                                (u16)((u16)(0x8000 / scaleY) * flipT));
        }
    }
}
#endif

void drawMenuSpriteWithAlpha(s16 arg0, s16 arg1, void *arg2, u16 arg3, u16 arg4, u16 arg5, u8 arg6, u16 arg7, u8 arg8) {
    s32 temp_v0;
    s32 temp_v1;

    drawMenuSpriteWithAlphaClipped(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, temp_v0 = (s16)(gMenuViewportWidth / 2),
                  temp_v1 = (s16)(gMenuViewportHeight / 2), temp_v0, temp_v1);
}

/*
 * One matched caller predates this function's narrow prototype. Preserve its
 * original argument promotions without weakening the canonical declaration.
 */
#ifdef __clang__
void drawMenuSpriteWithAlphaWideArgs(s32 x, s32 y, void *texture, s32 tileIndex, s32 width, s32 height,
                                     s32 palette, s32 alpha, u32 flip) {
    drawMenuSpriteWithAlpha(x, y, texture, tileIndex, width, height, palette, alpha, flip);
}
#else
#pragma weak drawMenuSpriteWithAlphaWideArgs = drawMenuSpriteWithAlpha
extern void drawMenuSpriteWithAlphaWideArgs(s32 x, s32 y, void *texture, s32 tileIndex, s32 width, s32 height,
                                            s32 palette, s32 alpha, u32 flip);
#endif

// drawMenuSpriteWithAlphaClipped best match: 91.212% (nonmatchings/drawMenuSpriteWithAlphaClipped-1219509448159986855/base_35.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuSpriteWithAlphaClipped.s")

#ifdef NON_MATCHING
void drawMenuSpriteWithAlphaClipped(s16 x, s16 y, FontAsset *asset, u16 tileIndex, u16 scaleX, u16 scaleY, u8 flipMode, u16 alpha,
                   u8 paletteArg, s16 clipLeft, s16 clipTop, s16 clipRight, s16 clipBottom) {
    FontTexture *texture;
    volatile s32 pad;
    u8 *paletteBase;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 minX;
    s32 minY;
    s32 maxX;
    s32 maxY;
    s32 texS;
    s16 flipS;
    s16 flipT;
    s32 texT;
    s32 texWidth;
    s32 texHeight;
    s32 scaleXValue;
    s32 scaleYValue;
    s32 color;
    u16 palette;

    paletteBase = (asset->header.entryCount * sizeof(FontTexture)) + (u8 *)asset + 8;

    if (scaleX >= 0x201) {
        return;
    }
    if (scaleX <= 0) {
        return;
    }
    if (scaleY >= 0x201) {
        return;
    }
    if (scaleY <= 0) {
        return;
    }
    {
        flipS = gMenuSpriteFlipScales[flipMode & 3][0];
        flipT = gMenuSpriteFlipScales[flipMode & 3][1];
        texture = &asset->textures[tileIndex];
        texT = x + gMenuViewportCenterX;
        texWidth = texture->width;
        texHeight = texture->height;

        left = texT << 2;
        top = ((y + gMenuViewportCenterY) << 1) << 1;
        right = (((scaleX * texWidth) << 2) >> 5) + left;
        bottom = (((scaleY * texHeight) << 2) >> 5) + (0, top);
        texS = 0;
        texT = 0;
        texT = 0;
        if (flipS == -1) {
            texS = (texWidth - 1) << 5;
        }
        if (flipT == -1) {
            texT = (texHeight - 1) << 5;
        }

        minY = (s16)((gMenuViewportCenterY - clipTop) << 2);
        maxY = (s16)((gMenuViewportCenterY + clipBottom) << 2);
        minX = (s16)((gMenuViewportCenterX - clipLeft) << 2);
        maxX = (s16)((gMenuViewportCenterX + clipRight) << 2);

        scaleYValue = scaleY;
        scaleXValue = scaleX;
        if ((left < maxX) && (top < maxY) && (right >= minX) && (bottom >= minY)) {
            if (left < minX) {
                texS = (((minX - left) << 8) / scaleXValue);
                if (flipS == -1) {
                    texS = ((texWidth - 1) << 5) - texS;
                }
                left = minX;
            }
            if (top < minY) {
                texT = (((minY - top) << 8) / scaleYValue);
                if (flipT == -1) {
                    texT = ((texHeight - 1) << 5) - texT;
                }
                top = minY;
            }
            if (right >= maxX) {
                right = maxX - 4;
            }
            if (bottom >= maxY) {
                bottom = maxY - 4;
            }

            if (paletteArg == (texture->imageOffset * 0)) {
                palette = texture->paletteIndex;
            } else {
                palette = paletteArg - 1;
            }

            if (alpha != 0x100) {
                gDPPipeSync(gRegionAllocPtr++);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xFC119623, 0xFF2FFFFF);
                color = alpha & 0xFF;
                FONT_GFX_CMD(gRegionAllocPtr++, 0xFA000000, (color << 0x18) | (color << 0x10) | (color << 8) | 0xFF);
            }

            gDPLoadTextureTile_4b(gRegionAllocPtr++, (u8 *)asset + texture->imageOffset, G_IM_FMT_CI, texture->width,
                                  texture->height, 0, 0, texture->width, texture->height, 0, G_TX_CLAMP, G_TX_CLAMP, 0,
                                  0, 0, 0);
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteBase + (palette * 0x20));
            palette = 0x8000 / scaleXValue;
            gSPTextureRectangle(gRegionAllocPtr++, left, top, right, bottom, 0, texS, texT,
                                (u16)(palette * flipS), (u16)((u16)(0x8000 / scaleYValue) * flipT));
            if (alpha != 0x100) {
                gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
            }
        }
    }
}
#endif

// drawMenuSpriteWithPaletteScale best match: 98.504% (nonmatchings/drawMenuSpriteWithPaletteScale-8498672362023432715/base_28.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuSpriteWithPaletteScale.s")

#ifdef NON_MATCHING
void drawMenuSpriteWithPaletteScale(s16 x, s16 y, FontAsset *asset, u16 index, u16 intensity) {
    FontTexture *texture;
    u32 loadBlockFlags;
    u8 *textureBase;
    u8 *paletteBase;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 minX;
    s32 minY;
    s32 maxX;
    s32 maxY;
    s32 halfWidth;
    s32 halfHeight;
    u16 headerSize;
    s32 srcX;
    s32 srcY;

    headerSize = sizeof(FontAssetHeader);
    textureBase = (u8 *)asset + (index * sizeof(FontTexture));
    texture = (FontTexture *)(textureBase + headerSize);
    paletteBase = (asset->header.entryCount * sizeof(FontTexture)) + (u8 *)asset + headerSize;
    left = x + gMenuViewportCenterX;
    top = y + gMenuViewportCenterY;
    loadBlockFlags = 0x07000000;
    right = left + (texture->width >> 1);
    bottom = top + (texture->height >> 1);
    srcX = 0;
    srcY = 0;

    halfWidth = gMenuViewportWidth / 2;
    maxX = gMenuViewportCenterX + halfWidth;
    if (left >= maxX) {
        return;
    }
    halfHeight = gMenuViewportHeight / 2;
    maxY = gMenuViewportCenterY + halfHeight;
    minX = gMenuViewportCenterX - halfWidth;
    if (top >= maxY) {
        return;
    }
    if (right < minX) {
        return;
    }
    minY = gMenuViewportCenterY - halfHeight;
    if (bottom < minY) {
        return;
    }
    if (left < minX) {
        srcX = minX - left;
        left = minX;
    }
    if (top < minY) {
        srcY = minY - top;
        top = minY;
    }
    if (right >= maxX) {
        right = maxX;
    }
    if (bottom >= maxY) {
        bottom = maxY;
    }

    {
        u16 *srcPalette;
        u16 *palette;
        u16 paletteColor;
        u16 color;
        s32 i;
        s32 red;
        u16 green;
        u16 blue;

        gDPPipeSync(gRegionAllocPtr++);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xBA000C02, 0x3000);

        srcPalette = (u16 *)(paletteBase + (texture->paletteIndex * MENU_PALETTE_SIZE_BYTES));
        palette = allocMenuRenderScratch(MENU_PALETTE_SIZE_BYTES);
        for (i = 0; i != MENU_PALETTE_COLOR_COUNT; i++) {
            paletteColor = srcPalette[i] ^ 0;
            color = paletteColor & (u64)0xFFFF;
            palette[i] = paletteColor;
            if (color & MENU_RGBA5551_ALPHA_BIT) {
                red = (color >> 11) & MENU_RGBA5551_CHANNEL_MASK;
                green = (color >> 6) & MENU_RGBA5551_CHANNEL_MASK;
                blue = (color >> 1) & MENU_RGBA5551_CHANNEL_MASK;
                // This assignment preserves IDO's palette-loop register allocation.
                color = blue;
                red = (red * intensity) / MENU_RGBA5551_SCALE_BASE;
                green = (green * intensity) / MENU_RGBA5551_SCALE_BASE;
                blue = (blue * intensity) / MENU_RGBA5551_SCALE_BASE;
                palette[i] = (red << 11) | (green << 6) | (blue << 1) | MENU_RGBA5551_ALPHA_BIT;
            }
        }

        FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)palette);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);

        FONT_GFX_CMD(gRegionAllocPtr++, (((texture->width >> 1) - 1) & 0xFFF) | 0xFD480000,
                     (u32)(texture->imageOffset + (u8 *)asset));
        FONT_GFX_CMD(gRegionAllocPtr++,
                     ((((((s32)(texture->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5480000,
                     0x07080200);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000,
                     (((texture->width * 2) & 0xFFF) << 12) | loadBlockFlags |
                         ((texture->height * 4) & 0xFFF));
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
        FONT_GFX_CMD(gRegionAllocPtr++,
                     ((((((s32)(texture->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5400000,
                     0x00080200);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000,
                     (((texture->width * 4) & 0xFFF) << 12) | ((texture->height * 4) & 0xFFF));
        FONT_GFX_CMD(gRegionAllocPtr++,
                     0xE4000000 | (((right << 2) & 0xFFF) << 12) | ((bottom << 2) & 0xFFF),
                     (((left << 2) & 0xFFF) << 12) | ((top << 2) & 0xFFF));
        FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000,
                     ((((srcX << 5) + 0x10) << 14) << 2) | (((srcY << 5) + 0x10) & 0xFFFF));
        FONT_GFX_CMD(gRegionAllocPtr++, 0xB3000000, (MENU_HALF_SCALE_STEP << 16) | MENU_HALF_SCALE_STEP);
        gDPPipeSync(gRegionAllocPtr++);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xBA000C02, 0);
        gDPPipeSync(gRegionAllocPtr++);
    }
}
#endif

void drawMenuSpriteSubrect(s16 x, s16 y, void *assetAddress, u16 index, u8 srcX, u8 srcY, u8 width, u8 height, s32 scaleX,
                           s32 scaleY) {
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
    texS = srcX << 5; texT = srcY << 5; minY = (s16)((gMenuViewportCenterY - (gMenuViewportHeight / 2)) << 2); maxY = (gMenuViewportCenterY + (gMenuViewportHeight / 2)) << 2; minX = (s16)((gMenuViewportCenterX - (gMenuViewportWidth / 2)) << 2);
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

        gDPLoadTextureTile_4b(gRegionAllocPtr++, texture->imageOffset + (u8 *)assetAddress + 0x80000000, G_IM_FMT_CI, texture->width,
                              texture->height, 0, 0, texture->width, texture->height, 0, G_TX_CLAMP, G_TX_CLAMP,
                              G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteBase + (texture->paletteIndex << 5) + 0x80000000);
        gSPTextureRectangle(gRegionAllocPtr++, left, top, right, bottom, G_TX_RENDERTILE, texS, texT,
                            (u16)(0x8000 / scaleXValue), (u16)(0x8000 / scaleYValue));
    }
}

// drawMenuSpriteFixedScale best match: 87.650% (nonmatchings/drawMenuSpriteFixedScale-210831275846872038/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuSpriteFixedScale.s")

#ifdef NON_MATCHING
void drawMenuSpriteFixedScale(s16 x, s16 y, void *assetAddress, u16 tileIndex, u16 scaleX, u16 scaleY, u8 flipMode,
                              u8 unusedPalette) {
    FontTexture *texture;
    FontTexture *textureBase;
    u8 *paletteBase;
    s32 drawLeft;
    s32 drawTop;
    s32 drawRight;
    s32 drawBottom;
    s32 clipLeft;
    s32 clipTop;
    s16 clipRight;
    s16 clipBottom;
    s32 texS;
    s16 sScale;
    s16 tScale;
    s32 texT;
    s32 texWidth;
    s32 texHeight;
    u16 scaleYValue;
    u16 scaleYStep;

    paletteBase = (u8 *)&((FontAsset *)assetAddress)->textures[((FontAsset *)assetAddress)->header.entryCount];
    scaleYValue = scaleY;

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

    sScale = gMenuSpriteFlipScales[flipMode & 3][0];
    tScale = gMenuSpriteFlipScales[flipMode & 3][1];
    textureBase = &((FontTexture *)assetAddress)[tileIndex];
    texWidth = textureBase[1].width;

    drawLeft = (drawRight = x + gMenuViewportCenterX) << 2;
    texS = (y + gMenuViewportCenterY) << 2;
    drawTop = texS;
    drawRight = (((scaleX * texWidth) << 2) / 0x1000) + drawLeft;
    texHeight = textureBase[1].height;
    texture = &textureBase[1];
    drawBottom = (((scaleYValue * texHeight) << 2) / 0x1000) + drawTop;
    texS = 0;
    texT = 0;

    if (sScale == -1) {
        texS = (texWidth - 1) << 5;
    }
    if (tScale == -1) {
        texT = (texHeight - 1) << 5;
    }

    clipTop = (s16)((gMenuViewportCenterY - (gMenuViewportHeight / 2)) << 2);
    clipBottom = (gMenuViewportCenterY + (gMenuViewportHeight / 2)) << 2;
    clipLeft = (s16)((gMenuViewportCenterX - (gMenuViewportWidth / 2)) << 2);
    clipRight = (gMenuViewportCenterX + (gMenuViewportWidth / 2)) << 2;

    if ((drawLeft < clipRight) && (drawTop < clipBottom) && (drawRight >= clipLeft) && (drawBottom >= clipTop)) {
        if (drawLeft < clipLeft) {
            texS = (((clipLeft - drawLeft) << 3) << 12) / scaleX;
            if (sScale == -1) {
                texS = ((texWidth - 1) << 5) - texS;
            }
            drawLeft = clipLeft;
        }
        if (drawTop < clipTop) {
            texT = (((clipTop - drawTop) << 3) << 12) / scaleYValue;
            if (tScale == -1) {
                texT = ((texHeight - 1) << 5) - texT;
            }
            drawTop = clipTop;
        }
        if (drawRight >= clipRight) {
            drawRight = clipRight - 4;
        }
        if (drawBottom >= clipBottom) {
            drawBottom = clipBottom - 4;
        }

        gDPLoadTextureTile_4b(gRegionAllocPtr++, (u8 *)(assetAddress + texture->imageOffset + 0x80000000), G_IM_FMT_CI,
                              texture->width, texture->height, 0, 0, texture->width, texture->height, 0, G_TX_CLAMP,
                              G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0,
                          (u8 *)((u32)paletteBase + (texture->paletteIndex << 5) + 0x80000000));
        scaleYValue = 0x400000 / scaleX;
        scaleYStep = scaleY;
        gSPTextureRectangle(gRegionAllocPtr++, drawLeft, drawTop, drawRight, drawBottom, 0, texS, texT,
                            (u16)(scaleYValue * sScale), (u16)((0x400000 / scaleYStep) * tScale));
    }
}
#endif

void drawMenuSpriteTile(s16 arg0, s16 arg1, void *arg2, u16 arg3, u16 arg4, u16 arg5) {
    drawMenuSpriteTileClipped(arg0, arg1, arg2, arg3, arg4, arg5, gMenuViewportWidth / 2, gMenuViewportHeight / 2);
}

void drawMenuSpriteTileClipped(s16 x, s16 y, MenuFontAssetTable *table, u16 entryIndex, u16 unused, u16 intensity,
                               s16 clipX, s16 clipY) {
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

    gDPLoadTextureTile(gRegionAllocPtr++, entry->imageOffset + (u8 *)table, G_IM_FMT_CI, G_IM_SIZ_8b,
                       entry->width, entry->height, 0, 0, entry->width, entry->height, 0, G_TX_CLAMP,
                       G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    if (intensity != 0x100) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, intensity, intensity, intensity, 0xFF);
    }
    gDPLoadTLUT_pal256(gRegionAllocPtr++, paletteBase + (entry->textureIndex << 5));
    gSPTextureRectangle(gRegionAllocPtr++, x0 << 2, y0 << 2, x1 << 2, y1 << 2, G_TX_RENDERTILE,
                        clipS << 5, clipT << 5, 0x400, 0x400);
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
    FONT_GFX_CMD(gRegionAllocPtr++, 0xE4000000 | ((x1 & 0xFFF) << 12) | (y1 & 0xFFF),
                 ((x0 & 0xFFF) << 12) | (y0 & 0xFFF));
    FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000, 0);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xB3000000, 0x04000400);
}

void func_80011C18(MenuRenderSpriteActor *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateMenuSpriteActorDebugControls);
}

void updateMenuSpriteActorDebugControls(MenuRenderSpriteActor *actor) {
    if (gPlayerInputHeld & U_CBUTTONS) {
        actor->sprite.y++;
    }
    if (gPlayerInputHeld & D_CBUTTONS) {
        actor->sprite.y--;
    }
    if (gPlayerInputHeld & R_CBUTTONS) {
        actor->sprite.x++;
    }
    if (gPlayerInputHeld & L_CBUTTONS) {
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

// drawMenuTilemapSprite best match: 84.412% (nonmatchings/drawMenuTilemapSprite-8498672362023432715/base_36.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuTilemapSprite.s")

#ifdef NON_MATCHING
s32 drawMenuTilemapSprite(MenuRenderSprite *sprite, s32 useLargeTiles, s16 xDivisor, s16 yDivisor) {
    s32 minX;
    s32 minY;
    s16 maxX;
    s16 maxY;
    s16 x;
    s16 y;
    s16 clipX;
    s16 clipY;
    s16 clipRight;
    s16 clipBottom;
    u16 tileMask;
    u16 tileShift;
    s16 cols;
    s16 rows;
    s16 xStart;
    s16 yStart;
    s16 xRemainder;
    s16 yRemainder;
    s16 row;
    s16 col;
    s16 tilemapX;
    s16 tilemapY;
    s16 drawX0;
    s16 drawY0;
    s16 drawX1;
    s16 drawY1;
    s16 texS;
    s16 texT;
    s16 sScale;
    s16 tScale;
    s16 tileIndex;
    s16 imageIndex;
    u8 paletteIndex;
    u8 flip;
    s16 screenX;
    s16 screenY;
    u16 *tilemap;
    MenuRenderTileInfo *tileInfo;
    u16 *palette;

    tilemap = sprite->tilemap;
    tileInfo = (MenuRenderTileInfo *)sprite->tileInfo;
    palette = sprite->palette;

    x = sprite->x;
    clipX = sprite->clipX;
    if (clipX < (minX = -gMenuViewportWidth / 2)) {
        x = x + minX - clipX;
        clipX = minX;
    }

    maxX = sprite->clipX + sprite->width;
    minY = -gMenuViewportHeight / 2;
    y = sprite->y;
    clipY = sprite->clipY;
    if (clipY < minY) {
        y = y + minY - clipY;
        clipY = minY;
    }

    if ((gMenuViewportWidth / 2) < maxX) {
        maxX = gMenuViewportWidth / 2;
    }

    maxY = sprite->clipY + sprite->height;
    if ((gMenuViewportHeight / 2) < maxY) {
        maxY = gMenuViewportHeight / 2;
    }

    clipX += gMenuViewportCenterX;
    maxX += gMenuViewportCenterX;
    clipY += gMenuViewportCenterY;
    maxY += gMenuViewportCenterY;

    tileMask = sprite->tileSize - 1;
    if (sprite->tileSize == 0x10) {
        tileShift = 4;
    } else {
        tileShift = 5;
    }

    xStart = clipX;
    yStart = clipY;
    cols = (maxX - clipX + tileMask - 1) >> tileShift;
    rows = (maxY - clipY + tileMask - 1) >> tileShift;
    if (x & tileMask) {
        cols++;
    }
    if (y & tileMask) {
        rows++;
    }

    clipRight = maxX;
    clipBottom = maxY;
    xStart -= x & tileMask;
    yStart -= y & tileMask;
    xRemainder = (x >> tileShift) % xDivisor;
    yRemainder = tilemapY = (y >> tileShift) % yDivisor;

    row = 0;
    if (rows > 0) {
        do {
            col = 0;
            screenX = xStart;
            tilemapX = xRemainder;
            if (cols > 0) {
                do {
                    tileIndex = tilemap[tilemapX + (tilemapY * sprite->tileYStep)];
                    paletteIndex = tileInfo[tileIndex].paletteIndex;
                    if (tileIndex != 0) {
                        flip = tileInfo[tileIndex].flip;
                        imageIndex = tileInfo[tileIndex].imageIndex;
                        sScale = gMenuSpriteFlipScales[flip][0];
                        tScale = gMenuSpriteFlipScales[flip][1];
                        drawX0 = screenX;
                        drawY0 = screenY = yStart;
                        drawX1 = screenX + sprite->tileSize;
                        drawY1 = yStart + sprite->tileXStep;
                        texS = 0;
                        texT = 0;
                        if (sScale == -1) {
                            texS = sprite->tileSize - 1;
                        }
                        if (tScale == -1) {
                            texT = sprite->tileXStep - 1;
                        }
                        if ((screenX < clipRight) && (yStart < clipBottom) && (drawX1 >= clipX) && (drawY1 >= clipY)) {
                            if (screenX < clipX) {
                                drawX0 = clipX;
                                texS = clipX - screenX;
                                if (sScale == -1) {
                                    s32 flippedTexS;
                                    flippedTexS = sprite->tileSize - (clipX - screenX) - 1;
                                    texS = flippedTexS;
                                }
                            }
                            if (yStart < clipY) {
                                drawY0 = clipY;
                                texT = clipY - yStart;
                                if (tScale == -1) {
                                    texT = sprite->tileXStep - (clipY - yStart) - 1;
                                }
                            }
                            if (drawX1 >= clipRight) {
                                drawX1 = clipRight - 1;
                            }
                            if (drawY1 >= clipBottom) {
                                drawY1 = clipBottom - 1;
                            }

                            if ((u8)useLargeTiles == 0) {
                                gDPLoadTLUT_pal16(gRegionAllocPtr++, paletteIndex,
                                                 sprite->palette + (paletteIndex << 4));
                                gDPLoadTextureTile_4b(
                                    gRegionAllocPtr++,
                                    sprite->image + (((imageIndex - 1) * sprite->tileSize * sprite->tileXStep) / 4),
                                    G_IM_FMT_CI, sprite->tileSize, sprite->tileXStep, 0, 0, sprite->tileSize,
                                    sprite->tileXStep, paletteIndex, G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK,
                                    G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
                            } else {
                                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette + (paletteIndex << 4));
                                gDPLoadTextureTile(
                                    gRegionAllocPtr++,
                                    sprite->image + (((imageIndex - 1) * sprite->tileSize * sprite->tileXStep) / 2),
                                    G_IM_FMT_CI, G_IM_SIZ_8b, sprite->tileSize, sprite->tileXStep, 0, 0,
                                    sprite->tileSize, sprite->tileXStep, 0, G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK,
                                    G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
                            }

                            gSPTextureRectangle(gRegionAllocPtr++, drawX0 * 4, drawY0 * 4, drawX1 * 4, drawY1 * 4,
                                                G_TX_RENDERTILE, texS << 5, texT << 5, sScale << 10, tScale << 10);
                        }
                    }
                    screenX += sprite->tileSize;
                    tilemapX = (tilemapX + 1) % xDivisor;
                    col++;
                } while (col < cols);
            }
            yStart += sprite->tileXStep;
            tilemapY = (tilemapY + 1) % yDivisor;
            row++;
        } while (row < rows);
    }
}

#endif

void drawMenuGlyphScriptWithFontBank(volatile s16 x, s16 y, MenuGlyphScript *script, s32 palette, u16 scale, u16 fontBank) {
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
                    drawMenuGlyph(drawX, drawY, glyphCode & 0xFFFF, MENU_GLYPH_PALETTE_INDEX(palette), scaleValue,
                                  fontBankValue);
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
                    drawMenuGlyph(drawX, drawY, currentGlyph & 0xFFFF, MENU_GLYPH_PALETTE_INDEX(palette), paletteScale,
                                  MENU_GLYPH_DEFAULT_FONT_BANK);
                }
                drawX += glyphAdvance;
            }
            cursor++;
            currentGlyph = *cursor;
        } while ((currentGlyph & 0xFFFF) != MENU_GLYPH_SCRIPT_END);
    }
}

// drawMenuGlyph best match: 98.589% (nonmatchings/drawMenuGlyph-8498672362023432715/base_22.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuGlyph.s")

#ifdef NON_MATCHING
void drawMenuGlyph(s16 x, s16 y, u16 glyph, u8 palette, u16 scale, u16 fontBank) {
    s32 x0;
    volatile s32 drawY0;
    s32 x1;
    s32 y1;
    s32 clipS;
    s32 clipT;
    s32 i;
    MenuFontAssetEntry *glyphTexture;
    u16 *scaledPalette;
    u16 *dstPalette;
    MenuFontAssetTable *fontAsset;
    u16 *glyphPalette;
    u16 paletteColor;
    int textureIndex;
    s32 color;
    s32 red;
    u16 green;
    u16 blue;

    if (palette == 0) {
        fontAsset = (MenuFontAssetTable *)getRelocatableHeapBlockBase(gAssetHandles[fontBank]);
        color = MENU_GLYPH_WIDE_WIDTH;
    } else {
        fontAsset = (MenuFontAssetTable *)getRelocatableHeapBlockBase(gAssetHandles[fontBank + 1]);
        color = MENU_GLYPH_NARROW_WIDTH;
    }

    glyphPalette = (u16 *)(fontAsset->entryCount + fontAsset->entries);
    x0 = x + gMenuViewportCenterX;
    i = y + gMenuViewportCenterY;
    x1 = color + x0;
    y1 = i + MENU_GLYPH_TALL_HEIGHT;
    clipS = 0;
    clipT = 0;

    if (x0 < gMenuViewportCenterX + (gMenuViewportWidth / 2)) {
        if ((i < gMenuViewportCenterY + (gMenuViewportHeight / 2)) &&
            (x1 >= gMenuViewportCenterX - (gMenuViewportWidth / 2))) {
            if (y1 >= gMenuViewportCenterY - (gMenuViewportHeight / 2)) {
                if (x0 < gMenuViewportCenterX - (gMenuViewportWidth / 2)) {
                    clipS = (gMenuViewportCenterX - (gMenuViewportWidth / 2)) - x0;
                    x0 = gMenuViewportCenterX - (gMenuViewportWidth / 2);
                }
                if (i < gMenuViewportCenterY - (gMenuViewportHeight / 2)) {
                    clipT = (gMenuViewportCenterY - (gMenuViewportHeight / 2)) - i;
                    i = gMenuViewportCenterY - (gMenuViewportHeight / 2);
                }
                if (x1 >= gMenuViewportCenterX + (gMenuViewportWidth / 2)) {
                    x1 = gMenuViewportCenterX + (gMenuViewportWidth / 2);
                    x1 = x1 - 1;
                }
                if (y1 >= gMenuViewportCenterY + (gMenuViewportHeight / 2)) {
                    y1 = (gMenuViewportCenterY + (gMenuViewportHeight / 2)) - 1;
                }
                glyphTexture = fontAsset->entries;
                glyphTexture += glyph;
                textureIndex = (u16)glyphTexture->textureIndex;
                glyphPalette += textureIndex * MENU_PALETTE_COLOR_COUNT;
                drawY0 = i; scaledPalette = allocMenuRenderScratch(MENU_PALETTE_SIZE_BYTES);
                i = 0;
                dstPalette = scaledPalette;
paletteLoop:
                *dstPalette = (paletteColor = *glyphPalette);
                i += sizeof(u16);
                color = paletteColor & 0xFFFF;
                if (color & MENU_RGBA5551_ALPHA_BIT) {
                    red = (color >> 11) & MENU_RGBA5551_CHANNEL_MASK;
                    green = (color >> 6) & MENU_RGBA5551_CHANNEL_MASK;
                    blue = (color >> 1) & MENU_RGBA5551_CHANNEL_MASK;
                    color = paletteColor & 0xFFFF;
                    red = (red * scale) / MENU_RGBA5551_SCALE_BASE;
                    green = (green * scale) / MENU_RGBA5551_SCALE_BASE;
                    blue = (blue * scale) / MENU_RGBA5551_SCALE_BASE;
                    *dstPalette = (red << 11) | (green << 6) | (blue << 1) |
                        MENU_RGBA5551_ALPHA_BIT;
                }
                glyphPalette++;
                dstPalette++;
                if (i != MENU_PALETTE_SIZE_BYTES) {
                    goto paletteLoop;
                }

                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, scaledPalette);
                gDPLoadTextureTile_4b(gRegionAllocPtr++,
                                      glyphTexture->imageOffset + (u8 *)fontAsset,
                                      G_IM_FMT_CI, glyphTexture->width, glyphTexture->height,
                                      0, 0, glyphTexture->width, glyphTexture->height, 0,
                                      G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                                      G_TX_NOLOD, G_TX_NOLOD);
                gSPTextureRectangle(gRegionAllocPtr++,
                                    x0 << MENU_GLYPH_RECT_FRAC_BITS,
                                    drawY0 << MENU_GLYPH_RECT_FRAC_BITS,
                                    x1 << MENU_GLYPH_RECT_FRAC_BITS,
                                    y1 << MENU_GLYPH_RECT_FRAC_BITS,
                                    G_TX_RENDERTILE,
                                    clipS << MENU_GLYPH_TEXEL_FRAC_BITS,
                                    clipT << MENU_GLYPH_TEXEL_FRAC_BITS,
                                    MENU_GLYPH_TEXTURE_STEP, MENU_GLYPH_TEXTURE_STEP);
            }
        }
    }
}
#endif

void drawMenuColoredGlyphScript(volatile s16 x, s16 y, MenuGlyphScript *script, s32 palette, u16 scale, u16 colorMode, u16 fontBank) {
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
                    drawMenuColoredGlyph(drawX, drawY, glyphCode & 0xFFFF, MENU_GLYPH_PALETTE_INDEX(palette), scaleValue,
                                         colorModeValue, fontBankValue);
                }
                drawX += advance;
            }
            glyphCode = scriptCursor[1];
            scriptCursor++;
        } while (MENU_GLYPH_SCRIPT_END != (glyphCode & 0xFFFF));
    }
}

void drawMenuGlyphScript(volatile s16 x, s16 y, MenuGlyphScript *script,
                         s32 palette, u16 scale, volatile u16 colorMode) {
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
        if (scale) {
        }
        do {
            spaceGlyph = MENU_GLYPH_SCRIPT_SPACE;
            if (MENU_GLYPH_SCRIPT_NEWLINE == (glyphCode & 0xFFFF)) {
                drawX = x;
                drawY += MENU_GLYPH_LINE_HEIGHT;
            } else if (spaceGlyph == (glyphCode & 0xFFFF)) {
                drawX += glyphAdvance;
                if (0) {
                }
            } else if ((glyphCode & 0xFFFF) == MENU_GLYPH_SCRIPT_COLOR) {
                colorModeValue = scriptCursor[1];
                scriptCursor++;
            } else {
                drawMenuColoredGlyph(drawX, drawY, glyphCode & 0xFFFF, MENU_GLYPH_PALETTE_INDEX(palette), scaleValue,
                                     colorModeValue, MENU_GLYPH_DEFAULT_FONT_BANK);
                drawX += (advance = glyphAdvance);
            }
            glyphCode = scriptCursor[1];
            scriptCursor++;
        } while (MENU_GLYPH_SCRIPT_END != (glyphCode & 0xFFFF));
        colorMode = colorModeValue;
    }
}

// drawMenuColoredGlyph best match: 98.107% (nonmatchings/drawMenuColoredGlyph-8498672362023432715/base_34.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuColoredGlyph.s")

#ifdef NON_MATCHING
void drawMenuColoredGlyph(s16 x, s16 y, u16 glyph, u8 palette, u16 paletteScale, u16 paletteIndex, s32 fontBank) {
    s32 x0;
    volatile s32 drawY0;
    s32 x1;
    s32 y1;
    s32 clipS;
    s32 clipT;
    s32 glyphWidth;
    s32 i;
    u16 paletteColor;
    u16 *paletteBase;
    MenuFontAssetTable *font;
    u16 *srcPalette;
    u16 *scaledPalette;
    u16 *dstPalette;
    MenuFontAssetEntry *entry;
    s32 color;
    s32 red;
    u16 green;
    u16 blue;
    s32 viewHalfWidth;
    s32 viewHalfHeight;
    s32 minX;
    s32 maxX;
    s32 minY;
    s32 maxY;

    if (palette == 0) {
        font = (MenuFontAssetTable *)getRelocatableHeapBlockBase(gAssetHandles[(u16)fontBank]);
        glyphWidth = MENU_GLYPH_WIDE_WIDTH;
    } else {
        font = (MenuFontAssetTable *)getRelocatableHeapBlockBase(gAssetHandles[(u16)fontBank + 1]);
        glyphWidth = MENU_GLYPH_NARROW_WIDTH;
    }
    paletteBase = (u16 *)(font->entryCount + font->entries);

    x0 = x + gMenuViewportCenterX;
    i = y + gMenuViewportCenterY;
    x1 = glyphWidth + x0;
    y1 = i + MENU_GLYPH_TALL_HEIGHT;
    clipS = 0;
    clipT = 0;

    viewHalfWidth = gMenuViewportWidth / 2;
    maxX = gMenuViewportCenterX + viewHalfWidth;
    if (x0 < maxX) {
        minX = gMenuViewportCenterX - viewHalfWidth;
        viewHalfHeight = gMenuViewportHeight / 2;
        maxY = gMenuViewportCenterY + viewHalfHeight;
        if ((i < maxY) && (x1 >= minX)) {
            minY = gMenuViewportCenterY - viewHalfHeight;
            if (y1 >= minY) {
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
                drawY0 = i;
                scaledPalette = allocMenuRenderScratch(MENU_PALETTE_SIZE_BYTES);
                srcPalette = &paletteBase[(u16)paletteIndex * MENU_PALETTE_COLOR_COUNT];
                i = 0;
                dstPalette = scaledPalette;
paletteLoop:
                *dstPalette = (paletteColor = *(u16 *)&((MenuGlyphPalette *)srcPalette)->bytes[i]);
                i += 2;
                do {
                    color = paletteColor & 0xFFFF;
                    do { if (color & MENU_RGBA5551_ALPHA_BIT) { red = (color >> 11) & MENU_RGBA5551_CHANNEL_MASK; green = (color >> 6) & MENU_RGBA5551_CHANNEL_MASK; blue = (color >> 1) & MENU_RGBA5551_CHANNEL_MASK; red *= paletteScale; red /= MENU_RGBA5551_SCALE_BASE; green = (green * paletteScale) / MENU_RGBA5551_SCALE_BASE; color = green; blue = (blue * paletteScale) / MENU_RGBA5551_SCALE_BASE; if (blue && blue) { } *dstPalette = (red << 11) | (color << 6) | (blue << 1) | MENU_RGBA5551_ALPHA_BIT; } } while (0);
                    dstPalette++;
                    if (i != MENU_PALETTE_SIZE_BYTES) {
                        goto paletteLoop;
                    }
                    entry = &font->entries[glyph];
                } while (0);

                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, scaledPalette);
                gDPLoadTextureTile_4b(gRegionAllocPtr++, (u8 *)font + (*entry).imageOffset, G_IM_FMT_CI,
                                      entry->width, entry->height, 0, 0, entry->width, entry->height, 0,
                                      G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                                      G_TX_NOLOD);
                gSPTextureRectangle(gRegionAllocPtr++, x0 << MENU_GLYPH_RECT_FRAC_BITS,
                                    drawY0 << MENU_GLYPH_RECT_FRAC_BITS, x1 << MENU_GLYPH_RECT_FRAC_BITS,
                                    y1 << MENU_GLYPH_RECT_FRAC_BITS, G_TX_RENDERTILE,
                                    clipS << MENU_GLYPH_TEXEL_FRAC_BITS, clipT << MENU_GLYPH_TEXEL_FRAC_BITS,
                                    MENU_GLYPH_TEXTURE_STEP, MENU_GLYPH_TEXTURE_STEP);
            }
        }
    }
}
#endif

// drawMenuAsciiGlyph best match: 98.369% (nonmatchings/drawMenuAsciiGlyph-3885303446860889946/base_13.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuAsciiGlyph.s")

#ifdef NON_MATCHING
void drawMenuAsciiGlyph(s16 x, s16 y, u16 tileS, s32 tileT, u16 paletteIndex, u16 paletteScale) {
    s32 drawX0;
    s32 drawY0;
    s32 x1;
    s32 y1;
    s32 clipS;
    s32 clipT;
    s32 x0;
    s32 y0;
    FontTexture *atlasTexture;
    u16 *paletteBase;
    FontAsset *asset;
    u16 *sourcePalette;
    u16 *dstPalette;
    s16 selectedPalette;
    s32 maxX;
    s32 maxY;
    s32 minY;
    s32 minX;
    s32 halfHeight;
    u16 paletteColor;
    u16 *scaledPalette;
    s32 color;
    s32 red;
    u16 green;
    u16 blue;

    asset = (FontAsset *)getRelocatableHeapBlockBase(gAssetHandles[6]);
    paletteBase = (u16 *)((asset->header.entryCount * sizeof(FontTexture)) + (u8 *)asset + sizeof(FontAssetHeader));
    atlasTexture = &asset->textures[0];
    selectedPalette = atlasTexture->paletteIndex;
    x0 = x + gMenuViewportCenterX;
    y0 = y + gMenuViewportCenterY;
    x1 = x0 + 8;
    y1 = y0 + 8;
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
        x1 = maxX - 1;
    }
    if (y1 >= maxY) {
        y1 = maxY - 1;
    }

    clipS += tileS;
    clipT += (u16)tileT;
    if (selectedPalette != paletteIndex) {
        selectedPalette = paletteIndex;
    }

    drawX0 = x0;
    drawY0 = y0;
    sourcePalette = paletteBase + (selectedPalette * 16);
    scaledPalette = allocMenuRenderScratch(0x20);
    y0 = 0;
    dstPalette = scaledPalette;
paletteLoop:
    *dstPalette = (paletteColor = *(u16 *)((u8 *)sourcePalette + y0));
    y0 += 2;
    color = paletteColor & 0xFFFF;
    if (color && color) {
    }
    if (color & 1) {
        red = (color >> 11) & 0x1F;
        green = (color >> 6) & 0x1F;
 blue = (color >> 1) & 0x1F; red = (paletteScale * red) / 256; green = (paletteScale * green) / 256; color = green;
        if (red && red) {
        }
        blue = (blue * paletteScale) / 256;
        if (red && red) {
        }
        // This empty condition preserves IDO's palette-loop register allocation.
        if (blue && blue) {
        }
        *dstPalette = (((red << 11) | (color << 6)) | (blue << 1)) | 1;
        if (dstPalette && dstPalette) {
        }
    }
    dstPalette++;
    if (y0 != 0x20) {
        goto paletteLoop;
    }

    gDPLoadTextureTile_4b(gRegionAllocPtr++, atlasTexture->imageOffset + (u8 *)asset, G_IM_FMT_CI,
                          atlasTexture->width, atlasTexture->height, 0, 0, atlasTexture->width,
                          atlasTexture->height, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, G_TX_NOLOD,
                          G_TX_NOLOD);
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, scaledPalette);
    gSPTextureRectangle(gRegionAllocPtr++, drawX0 * 4, drawY0 * 4, x1 * 4, y1 * 4, 0, clipS << 5,
                        clipT << 5, 0x0400, 0x0400);
}
#endif

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
                    drawMenuAsciiGlyph(var_s0, var_s2, ((var_a2 << 3) & 0xFFFF) & 0xFFFF, var_v0 & 0x38, arg3,
                                  arg4);
                }
                var_s0 += 8;
            }
            var_v0 = var_s1[1];
            var_s1++;
        } while (var_v0 != 0);
    }
}

void drawMenuSolidRect(s16 x0, s16 y0, s16 x1, s16 y1, s16 r, s16 g, s16 b) {
    if ((x0 >= 0) && (x0 < 321) && (x1 >= 0) && (x1 < 321) &&
        (y0 >= 0) && (y0 < 241) && (y1 >= 0) && (y1 < 241)) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCycleType(gRegionAllocPtr++, G_CYC_FILL);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_NOOP, G_RM_NOOP2);
        gDPSetFillColor(gRegionAllocPtr++,
                        (GPACK_RGBA5551(r, g, b, 1) << 16) | GPACK_RGBA5551(r, g, b, 1));
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
        do { clipS = minX - x0; } while (0);
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
    gDPSetCombineLERP(gRegionAllocPtr++, TEXEL1, TEXEL0, PRIMITIVE_ALPHA, TEXEL0, TEXEL1, TEXEL0,
                      PRIMITIVE, TEXEL0, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED);

    gDPLoadTextureTile(gRegionAllocPtr++, (u8 *)table + (0, entry0->imageOffset), G_IM_FMT_RGBA,
                       G_IM_SIZ_8b, entry0->width, entry0->height, 0, 0, entry0->width,
                       entry0->height, 0, G_TX_WRAP, G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                       G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadMultiTile(gRegionAllocPtr++, (u8 *)table + (0, entry1->imageOffset), 0x100,
                     G_TX_RENDERTILE + 1, G_IM_FMT_RGBA, G_IM_SIZ_8b, entry1->width, entry1->height,
                     0, 0, entry1->width, entry1->height, 0, G_TX_WRAP, G_TX_WRAP, G_TX_NOMASK,
                     G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadTLUT_pal256(gRegionAllocPtr++, (entry1->textureIndex << 5) + (u8*)paletteBase);
    gSPTextureRectangle(gRegionAllocPtr++, x0 << 2, y0 << 2, x1 << 2, y1 << 2,
                        G_TX_RENDERTILE, clipS << 5, clipT << 5, 0x400, 0x400);
    gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
}
