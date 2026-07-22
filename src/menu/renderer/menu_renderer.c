#include "common.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/relocatable_heap.h"
#define MENU_RENDERER_C
#include "game/menu/renderer/menu_renderer.h"

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

typedef struct MenuRenderTask MenuRenderTask;
typedef struct RenderCallbackNode RenderCallbackNode;
typedef struct MenuRenderAssetTableHeader MenuRenderAssetTableHeader;
typedef struct MenuRenderAssetTableEntry MenuRenderAssetTableEntry;
typedef struct FontAssetHeader FontAssetHeader;
typedef struct FontTexture FontTexture;
typedef struct FontAsset FontAsset;
typedef struct MenuFontAssetEntry MenuFontAssetEntry;

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

struct MenuRenderSprite {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 tileSize;
    /* 0x06 */ s16 tileXStep;
    /* 0x08 */ s16 tileYStep;
    /* 0x0A */ s16 padA;
    /* 0x0C */ s16 clipX;
    /* 0x0E */ s16 clipY;
    /* 0x10 */ s16 width;
    /* 0x12 */ s16 height;
    /* 0x14 */ u16 *image;
    /* 0x18 */ u16 *tilemap;
    /* 0x1C */ u8 *tileInfo;
    /* 0x20 */ u16 *palette;
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

typedef void (*MenuRenderSpriteActorCallback)(MenuRenderSpriteActor *);
typedef void (*MenuRenderCallback)(MenuRenderSprite *);

extern void addRenderCallback(RenderCallbackNode **queue, MenuRenderCallback callback, MenuRenderSprite *sprite);
extern void *allocMenuRenderScratch(s32 size);
void getAssetTableImageAndPalette(void *asset, u16 index, void **image, void **palette);
s32 drawMenuTilemapSprite(MenuRenderSprite *sprite, s32 arg1, s16 x, s16 y);
void drawMenuSpriteTileClipped(s16 arg0, s16 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, s32 arg6, s32 arg7);
void drawMenuSpriteClipped(s16 arg0, s16 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u8 arg6, u8 arg7, s32 arg8, s32 arg9,
                   s32 argA, s32 argB);
void drawMenuSpriteWithAlphaClipped(s16 arg0, s16 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u8 arg6, u16 arg7, u8 arg8,
                   s32 arg9, s32 argA, s32 argB, s32 argC);
void drawMenuGlyph(s16 x, s16 y, u16 glyph, u8 palette, u16 scale, u16 arg5);
void drawMenuColoredGlyph(s16 x, s16 y, u16 glyph, u8 palette, u16 scale, u16 colorMode, s32 arg6);
extern Gfx gMenuRenderModeResetDl[];
extern RenderCallbackNode *gMenuRenderCallbackList;
extern Gfx *gRegionAllocPtr;
extern s16 gAssetHandles[];
extern u32 gPlayerInputHeld;
extern s16 gMenuSpriteFlipScales[][2];
extern s16 gMenuFadeAlpha;
extern s16 gMenuViewportWidth;
extern s16 gMenuViewportHeight;
extern s16 gMenuViewportCenterX;
extern s16 gMenuViewportCenterY;
extern u16 D_800B51D0[];

// drawMenuAssetRegion best match: 89.734% (nonmatchings/drawMenuAssetRegion-1219509448159986855/base_88.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuAssetRegion.s")

#ifdef NON_MATCHING
void drawMenuAssetRegion(s16 x, s16 y, MenuRenderAssetTableHeader *table, u16 entryIndex, u16 scaleX, u16 scaleY,
                         u8 startS, u8 startT, u8 width, u8 height) {
    s32 x0;
    s32 y0;
    register s32 x1;
    register s32 y1;
    s32 clipTop;
    s32 clipLeft;
    s16 clipBottom;
    s16 clipRight;
    s32 s;
    s32 t;
    register u16 sy;
    register u16 sx;
    register MenuRenderAssetTableEntry *entry;
    register u8 *paletteBase;

    paletteBase = (table->entryCount * sizeof(MenuRenderAssetTableEntry)) + (u8 *)table +
                  sizeof(MenuRenderAssetTableEntry);
    x0 = (x + gMenuViewportCenterX) * 4;
    y0 = (y + gMenuViewportCenterY) * 4;
    x1 = (((width * scaleX) << 2) >> 5) + x0;
    sx = scaleX;
    sy = scaleY;
    y1 = (((height * scaleY) << 2) >> 5) + y0;
    s = startS << 5;
    t = startT << 5;

    clipTop = (gMenuViewportCenterY - (gMenuViewportHeight / 2)) * 4;
    clipBottom = (gMenuViewportCenterY + (gMenuViewportHeight / 2)) * 4;
    clipLeft = (gMenuViewportCenterX - (gMenuViewportWidth / 2)) * 4;
    clipRight = (gMenuViewportCenterX + (gMenuViewportWidth / 2)) * 4;

    if ((x0 < clipRight) && (y0 < clipBottom) && (x1 >= clipLeft) && (y1 >= clipTop)) {
        if (x0 < clipLeft) {
            s += (((clipLeft - x0) * 8) << 5) / scaleX;
            x0 = clipLeft;
        }
        if (y0 < clipTop) {
            t += (((clipTop - y0) * 8) << 5) / scaleY;
            y0 = clipTop;
        }
        if (x1 >= clipRight) {
            x1 = clipRight - 4;
        }
        if (y1 >= clipBottom) {
            y1 = clipBottom - 4;
        }

        entry = &table->entries[entryIndex];
        entry += 0;
        gDPLoadTextureTile_4b(gRegionAllocPtr++, (u8 *)table + (0, entry->imageOffset) + 0x80000000,
                              G_IM_FMT_CI, entry->width, entry->height, 0, 0, entry->width,
                              entry->height, 0, G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK,
                              G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0,
                          paletteBase + (entry->textureIndex << 5) + 0x80000000);
        gSPTextureRectangle(gRegionAllocPtr++, x0, y0, x1, y1, G_TX_RENDERTILE, s, t,
                            (u16)(0x8000 / sx), (u16)(0x8000 / sy));
    }
}
#endif

void drawMenuSprite(s16 arg0, s16 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u8 arg6, u8 arg7) {
    s32 temp_v0;
    s32 temp_v1;

    drawMenuSpriteClipped(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7,
                  temp_v0 = (s16)(gMenuViewportWidth / 2), temp_v1 = (s16)(gMenuViewportHeight / 2), temp_v0, temp_v1);
}

// drawMenuSpriteClipped best match: 58.587% (nonmatchings/drawMenuSpriteClipped-5802343343535905907/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuSpriteClipped.s")

#ifdef NON_MATCHING
void drawMenuSpriteClipped(s32 x, s16 y, MenuFontAssetTable *table, u16 imageIndex, u16 width, u16 height, u8 flip, u8 paletteIndex,
                   s32 left, s32 top, s32 right, s32 bottom) {
    MenuFontAssetEntry *entry;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 minX;
    s32 minY;
    s32 maxX;
    s32 maxY;
    s32 drawX0;
    s32 drawY0;
    s32 drawX1;
    s32 drawY1;
    s32 dsdx;
    s32 dtdy;
    s32 line;
    s16 flipS;
    s16 flipT;
    s32 palette;
    u16 *paletteBase;
    Gfx *gfx;

    paletteBase = (u16 *)&table->entries[table->entryCount];

    if ((width >= 0x201) || (width <= 0) || (height >= 0x201) || (height <= 0)) {
        return;
    }

    flipS = gMenuSpriteFlipScales[flip & 3][0];
    flipT = gMenuSpriteFlipScales[flip & 3][1];
    entry = &table->entries[imageIndex];

    x0 = (x + gMenuViewportCenterX) << 2;
    y0 = (y + gMenuViewportCenterY) << 2;
    x1 = x0 + ((width * entry->width) << 2 >> 5);
    y1 = y0 + ((height * entry->height) << 2 >> 5);
    dsdx = 0;
    dtdy = 0;

    if (flipS == -1) {
        dsdx = (entry->width - 1) << 5;
    }
    if (flipT == -1) {
        dtdy = (entry->height - 1) << 5;
    }

    minY = gMenuViewportCenterY - (s16)top;
    minX = gMenuViewportCenterX - (s16)left;
    maxY = gMenuViewportCenterY + (s16)bottom;
    maxX = gMenuViewportCenterX + (s16)right;

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

    drawX0 = minX << 2;
    drawY0 = minY << 2;
    drawX1 = maxX << 2;
    drawY1 = maxY << 2;

    if ((x0 >= drawX1) || (y0 >= drawY1) || (x1 < drawX0) || (y1 < drawY0)) {
        return;
    }

    if (x0 < drawX0) {
        dsdx = ((drawX0 - x0) << 8) / width;
        if (flipS == -1) {
            dsdx = ((entry->width - 1) << 5) - dsdx;
        }
        x0 = drawX0;
    }
    if (y0 < drawY0) {
        dtdy = ((drawY0 - y0) << 8) / height;
        if (flipT == -1) {
            dtdy = ((entry->height - 1) << 5) - dtdy;
        }
        y0 = drawY0;
    }
    if (x1 >= drawX1) {
        x1 = drawX1 - 4;
    }
    if (y1 >= drawY1) {
        y1 = drawY1 - 4;
    }

    if (paletteIndex == 0) {
        palette = entry->textureIndex;
    } else {
        palette = (u16)(paletteIndex - 1);
    }

    FONT_GFX_CMD(gRegionAllocPtr++, (((entry->width >> 1) - 1) & 0xFFF) | 0xFD480000,
                 (u32)((u8 *)table + entry->imageOffset));
    line = ((((entry->width + 1) >> 1) + 7) >> 3) & 0x1FF;
    FONT_GFX_CMD(gRegionAllocPtr++, (line << 9) | 0xF5480000, 0x07080200);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000,
                 0x07000000 | (((entry->width << 1) & 0xFFF) << 12) | ((entry->height << 2) & 0xFFF));
    FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
    FONT_GFX_CMD(gRegionAllocPtr++, (line << 9) | 0xF5400000, 0x00080200);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000, (((entry->width << 2) & 0xFFF) << 12) | ((entry->height << 2) & 0xFFF));

    if (palette != 0xFE) {
        FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)(paletteBase + (palette << 4)));
    } else {
        FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)D_800B51D0);
    }
    FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
    FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);

    FONT_GFX_CMD(gRegionAllocPtr++, 0xE4000000 | ((x1 & 0xFFF) << 12) | (y1 & 0xFFF),
                 ((x0 & 0xFFF) << 12) | (y0 & 0xFFF));
    FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000, ((dsdx & 0xFFFF) << 16) | (dtdy & 0xFFFF));
    gfx = gRegionAllocPtr++;
    gfx->words.w0 = 0xB3000000;
    gfx->words.w1 = ((u16)(((0x8000 / width) & 0xFFFF) * flipS) << 16) | (u16)(((0x8000 / height) & 0xFFFF) * flipT);
}
#endif

void drawMenuSpriteWithAlpha(s16 arg0, s16 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u8 arg6, u16 arg7, u8 arg8) {
    s32 temp_v0;
    s32 temp_v1;

    drawMenuSpriteWithAlphaClipped(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, temp_v0 = (s16)(gMenuViewportWidth / 2),
                  temp_v1 = (s16)(gMenuViewportHeight / 2), temp_v0, temp_v1);
}

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

// drawMenuSpriteWithPaletteScale best match: 80.428% (nonmatchings/drawMenuSpriteWithPaletteScale-1219509448159986855/base_20.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuSpriteWithPaletteScale.s")

#ifdef NON_MATCHING
void drawMenuSpriteWithPaletteScale(s16 x, s16 y, FontAsset *asset, u16 index, u16 intensity) {
    FontTexture *texture;
    u8 *textureEntryBase;
    u8 *paletteBase;
    u16 *srcPalette;
    u16 *scaledPalette;
    u16 paletteColor;
    s32 color;
    s32 i;
    s32 drawLeft;
    s32 drawTop;
    s32 drawRight;
    s32 drawBottom;
    s32 viewportLeft;
    s32 viewportTop;
    s32 viewportRight;
    s32 viewportBottom;
    s32 viewportHalfWidth;
    s32 viewportHalfHeight;
    u16 headerSize;
    s32 sourceX;
    s32 sourceY;
    s32 red;
    u16 green;
    u16 blue;

    headerSize = sizeof(FontAssetHeader);
    textureEntryBase = (u8 *)asset + (index * sizeof(FontTexture));
    texture = (FontTexture *)(textureEntryBase + headerSize);
    paletteBase = (u8 *)asset + headerSize + (asset->header.entryCount * sizeof(FontTexture));
    drawLeft = x + gMenuViewportCenterX;
    drawTop = y + gMenuViewportCenterY;
    drawRight = drawLeft + (texture->width >> 1);
    drawBottom = drawTop + (texture->height >> 1);
    sourceX = 0;
    sourceY = 0;

    viewportHalfWidth = gMenuViewportWidth / 2;
    viewportRight = gMenuViewportCenterX + viewportHalfWidth;
    if (drawLeft >= viewportRight) {
        return;
    }
    viewportHalfHeight = gMenuViewportHeight / 2;
    viewportBottom = gMenuViewportCenterY + viewportHalfHeight;
    viewportLeft = gMenuViewportCenterX - viewportHalfWidth;
    if (drawTop >= viewportBottom) {
        return;
    }
    if (drawRight < viewportLeft) {
        return;
    }
    viewportTop = gMenuViewportCenterY - viewportHalfHeight;
    if (drawBottom < viewportTop) {
        return;
    }
                if (drawLeft < viewportLeft) {
                    sourceX = viewportLeft - drawLeft;
                    drawLeft = viewportLeft;
                }
                if (drawTop < viewportTop) {
                    sourceY = viewportTop - drawTop;
                    drawTop = viewportTop;
                }
                if (drawRight >= viewportRight) {
                    drawRight = viewportRight;
                }
                if (drawBottom >= viewportBottom) {
                    drawBottom = viewportBottom;
                }

                gDPPipeSync(gRegionAllocPtr++);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xBA000C02, 0x3000);

                srcPalette = (u16 *)(paletteBase + (texture->paletteIndex * 0x20));
                scaledPalette = allocMenuRenderScratch(MENU_PALETTE_COLOR_COUNT * sizeof(u16));
                for (i = 0; i != MENU_PALETTE_COLOR_COUNT; i++) {
                    paletteColor = srcPalette[i];
                    color = paletteColor & 0xFFFF;
                    scaledPalette[i] = paletteColor;
                    if (color & MENU_RGBA5551_ALPHA_BIT) {
                        red = (color >> 11) & MENU_RGBA5551_CHANNEL_MASK;
                        green = (color >> 6) & MENU_RGBA5551_CHANNEL_MASK;
                        blue = (color >> 1) & MENU_RGBA5551_CHANNEL_MASK;
                        red = (red * intensity) / 256;
                        green = (green * intensity) / 256;
                        blue = (blue * intensity) / 256;
                        scaledPalette[i] = (red << 11) | (green << 6) | (blue << 1) | MENU_RGBA5551_ALPHA_BIT;
                    }
                }

                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, scaledPalette);
                gDPLoadTextureTile_4b(gRegionAllocPtr++, (u8 *)asset + texture->imageOffset, G_IM_FMT_CI, texture->width,
                                      texture->height, 0, 0, texture->width, texture->height, 0, G_TX_WRAP, G_TX_WRAP, 0, 0,
                                      0, 0);
                gSPTextureRectangle(gRegionAllocPtr++, drawLeft << 2, drawTop << 2, drawRight << 2, drawBottom << 2, 0,
                                    (sourceX << 5) + 0x10, (sourceY << 5) + 0x10, MENU_HALF_SCALE_STEP,
                                    MENU_HALF_SCALE_STEP);
                gDPPipeSync(gRegionAllocPtr++);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xBA000C02, 0);
                gDPPipeSync(gRegionAllocPtr++);
}
#endif

// drawMenuSpriteSubrect best match: 99.808% (nonmatchings/drawMenuSpriteSubrect-1219509448159986855/base.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuSpriteSubrect.s")

#ifdef NON_MATCHING
void drawMenuSpriteSubrect(s16 x, s16 y, FontAsset *asset, u16 index, u8 srcX, u8 srcY, u8 width, u8 height, s32 scaleX,
                           s32 scaleY) {
    FontTexture *texture;
    s32 minY;
    s32 minX;
    s32 left;
    u8 *paletteBase;
    s32 top;
    s32 halfY;
    s32 right;
    s32 bottom;
    s32 texS;
    s32 texT;
    s32 halfX;
    u16 scaleXValue;
    u16 scaleYValue;
    s16 maxX;
    s16 maxY;

    texture = &asset->textures[index];
    paletteBase = (asset->header.entryCount * sizeof(FontTexture)) + (u8 *)asset + sizeof(FontAssetHeader);
    scaleXValue = scaleX;
    scaleYValue = scaleY;
    left = (x + gMenuViewportCenterX) << 2;
    top = (y + gMenuViewportCenterY) << 2;
    right = (((width * scaleXValue) << 2) >> 5) + left;
    bottom = height;
    bottom *= scaleYValue;
    bottom = ((bottom << 2) >> 5) + top;
    // Keeping these assignments on one source line preserves IDO's instruction scheduling.
    texS = srcX << 5; texT = srcY << 5; halfY = gMenuViewportHeight / 2; minY = (s16)((gMenuViewportCenterY - halfY) << 2); maxY = (gMenuViewportCenterY + halfY) << 2; halfX = gMenuViewportWidth / 2; minX = (s16)((gMenuViewportCenterX - halfX) << 2);
    maxX = (gMenuViewportCenterX + halfX) << 2;

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

        gDPLoadTextureTile_4b(gRegionAllocPtr++, texture->imageOffset + (u8 *)asset + 0x80000000, G_IM_FMT_CI, texture->width,
                              texture->height, 0, 0, texture->width, texture->height, 0, G_TX_CLAMP, G_TX_CLAMP,
                              G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteBase + (texture->paletteIndex << 5) + 0x80000000);
        gSPTextureRectangle(gRegionAllocPtr++, left, top, right, bottom, G_TX_RENDERTILE, texS, texT,
                            (u16)(0x8000 / scaleXValue), (u16)(0x8000 / scaleYValue));
    }
}
#endif

// drawMenuSpriteFixedScale best match: 87.000% (nonmatchings/drawMenuSpriteFixedScale-1219509448159986855/base_54.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuSpriteFixedScale.s")

#ifdef NON_MATCHING
void drawMenuSpriteFixedScale(s16 x, s16 y, s32 assetAddress, u16 tileIndex, u16 scaleX, u16 scaleY, u8 flipMode,
                              u8 unusedPalette) {
    FontTexture *texture;
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
    texture = &((FontAsset *)assetAddress)->textures[tileIndex];
    texWidth = texture->width;
    texHeight = texture->height;

    drawLeft = (drawRight = x + gMenuViewportCenterX) << 2;
    texS = (y + gMenuViewportCenterY) << 2;
    drawTop = texS;
    drawRight = (((scaleX * texWidth) << 2) / 0x1000) + drawLeft;
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

void drawMenuSpriteTile(s16 arg0, s16 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5) {
    drawMenuSpriteTileClipped(arg0, arg1, arg2, arg3, arg4, arg5, gMenuViewportWidth / 2, gMenuViewportHeight / 2);
}

// drawMenuSpriteTileClipped best match: 96.142% (nonmatchings/drawMenuSpriteTileClipped-8909410381742387388/base_34.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuSpriteTileClipped.s")

#ifdef NON_MATCHING
void drawMenuSpriteTileClipped(s16 x, s16 y, MenuFontAssetTable *table, u16 entryIndex, u16 unused, u16 alpha, s16 clipRight,
                   s16 clipBottom) {
    MenuFontAssetEntry *entry;
    volatile s32 padBefore[1];
    u8 *paletteBase;
    volatile s32 padAfter[3];
    s16 minX;
    s16 minY;
    s32 x0;
    volatile s16 clipBottomValue;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 clipS;
    s32 clipT;
    volatile s32 padAfterClip[3];
    s32 halfWidth;
    s16 maxX;
    s16 maxY;
    s32 halfHeight;

    entry = (MenuFontAssetEntry *)table + entryIndex;
    paletteBase = (table->entryCount * sizeof(MenuFontAssetEntry)) + (u8 *)table + sizeof(MenuFontAssetEntry);
    x0 = x + gMenuViewportCenterX; x1 = entry[1].width; x1 = x0 + x1; y0 = y + gMenuViewportCenterY; y1 = entry[1].height; y1 = y0 + y1;
    clipBottomValue = clipBottom;
    minX = gMenuViewportCenterX - clipRight;
    minY = gMenuViewportCenterY - clipBottomValue;
    maxX = gMenuViewportCenterX + clipRight;
    maxY = gMenuViewportCenterY + clipBottom;
    clipS = 0;
    clipT = 0;

    halfWidth = gMenuViewportWidth / 2;
    if (minX < gMenuViewportCenterX - halfWidth) {
        minX = gMenuViewportCenterX - halfWidth;
    }
    if (gMenuViewportCenterX + halfWidth < maxX) {
        maxX = gMenuViewportCenterX + halfWidth;
    }
    halfHeight = gMenuViewportHeight / 2;
    if (minY < gMenuViewportCenterY - halfHeight) {
        minY = gMenuViewportCenterY - halfHeight;
    }
    if (gMenuViewportCenterY + halfHeight < maxY) {
        maxY = gMenuViewportCenterY + halfHeight;
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

    gDPLoadTextureTile(gRegionAllocPtr++, entry->imageOffset + (u8 *)table,
                       G_IM_FMT_CI, G_IM_SIZ_8b, entry->width, entry->height,
                       0, 0, entry->width, entry->height, 0,
                       G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                       G_TX_NOLOD, G_TX_NOLOD);
    if (alpha != 0x100) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, alpha, alpha, alpha, 0xFF);
    }
    gDPLoadTLUT_pal256(gRegionAllocPtr++, paletteBase + (entry->textureIndex << 5));
    gSPTextureRectangle(gRegionAllocPtr++, x0 << 2, y0 << 2, x1 << 2, y1 << 2,
                        G_TX_RENDERTILE, clipS << 5, clipT << 5, 0x400, 0x400);
    if (alpha != 0x100) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}
#endif

void func_80011854(void) {
}

void drawMenuTextureByAssetId(s16 x, s16 y, s32 texture, u16 assetId, u16 width, u16 height) {
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
    setCallbackTaskCallback(arg0, updateMenuSpriteActorDebugControls);
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

    addRenderCallback(&gMenuRenderCallbackList, drawMenuTilemapSpriteCallback, &actor->sprite);
}

void drawMenuTilemapSpriteCallback(MenuRenderSprite *arg0) {
    drawMenuTilemapSprite(arg0, 0, 0, 0);
}

void func_80011D6C(void) {
}

// drawMenuTilemapSprite best match: 76.761% (nonmatchings/drawMenuTilemapSprite-5802343343535905907/base_13.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuTilemapSprite.s")

#ifdef NON_MATCHING
#define MENU_RENDER_SPRITE_EMIT_GFX(cmd0, cmd1) \
{ \
    Gfx *_g = gRegionAllocPtr++; \
    _g->words.w0 = (cmd0); \
    _g->words.w1 = (cmd1); \
}

s32 drawMenuTilemapSprite(MenuRenderSprite *sprite, s32 useLargeTiles, s16 xDivisor, s16 yDivisor) {
    s16 minX;
    s16 minY;
    s16 maxX;
    s16 maxY;
    s16 x;
    s16 y;
    s16 clipX;
    s16 clipY;
    s16 clipRight;
    s16 clipBottom;
    s16 tileMask;
    s16 tileShift;
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
    s32 sScale;
    s32 tScale;
    s16 tileIndex;
    s32 imageIndex;
    s32 paletteIndex;
    s32 flip;
    s16 screenX;
    s16 screenY;
    u16 *tilemap;
    MenuRenderTileInfo *tileInfo;
    u16 *palette;

    tilemap = sprite->tilemap;
    tileInfo = (MenuRenderTileInfo *)sprite->tileInfo;
    palette = sprite->palette;

    minX = -gMenuViewportWidth / 2;
    x = sprite->x;
    clipX = sprite->clipX;
    if (clipX < minX) {
        x = x + minX - clipX;
        clipX = minX;
    }

    minY = -gMenuViewportHeight / 2;
    y = sprite->y;
    clipY = sprite->clipY;
    if (clipY < minY) {
        y = y + minY - clipY;
        clipY = minY;
    }

    maxX = sprite->clipX + sprite->width;
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
    yRemainder = (y >> tileShift) % yDivisor;

    row = 0;
    tilemapY = yRemainder;
    if (rows > 0) {
        do {
            col = 0;
            screenX = xStart;
            tilemapX = xRemainder;
            if (cols > 0) {
                do {
                    tileIndex = tilemap[tilemapX + (tilemapY * sprite->tileYStep)];
                    imageIndex = tileInfo[tileIndex].imageIndex;
                    paletteIndex = tileInfo[tileIndex].paletteIndex;
                    if (tileIndex != 0) {
                        flip = tileInfo[tileIndex].flip;
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
                                    texS = sprite->tileSize - (clipX - screenX) - 1;
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

                            if (useLargeTiles == 0) {
                                MENU_RENDER_SPRITE_EMIT_GFX(0xFD100000, (u32)(palette + (paletteIndex << 4)));
                                MENU_RENDER_SPRITE_EMIT_GFX(0xE8000000, 0);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xF5000000 | ((((paletteIndex & 0xF) << 4) + 0x100) & 0x1FF),
                                                            0x07000000);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xE6000000, 0);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xF0000000, 0x0703C000);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xE7000000, 0);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xFD480000 | (((sprite->tileSize >> 1) - 1) & 0xFFF),
                                                            (u32)(sprite->image +
                                                                  ((((imageIndex - 1) * sprite->tileSize *
                                                                     sprite->tileXStep) /
                                                                    4) *
                                                                   2)));
                                MENU_RENDER_SPRITE_EMIT_GFX(0xF5480000 |
                                                                (((((sprite->tileSize + 1) >> 1) + 7) >> 3) & 0x1FF)
                                                                    << 9,
                                                            0x07080200);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xE6000000, 0);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xF4000000,
                                                            0x07000000 | (((sprite->tileSize * 2) & 0xFFF) << 12) |
                                                                ((sprite->tileXStep * 4) & 0xFFF));
                                MENU_RENDER_SPRITE_EMIT_GFX(0xE7000000, 0);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xF5400000 |
                                                                (((((sprite->tileSize + 1) >> 1) + 7) >> 3) & 0x1FF)
                                                                    << 9,
                                                            ((paletteIndex & 0xF) << 20) | 0x00080200);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xF2000000,
                                                            (((sprite->tileSize * 4) & 0xFFF) << 12) |
                                                                ((sprite->tileXStep * 4) & 0xFFF));
                            } else {
                                MENU_RENDER_SPRITE_EMIT_GFX(0xFD100000, (u32)(palette + (paletteIndex << 4)));
                                MENU_RENDER_SPRITE_EMIT_GFX(0xE8000000, 0);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xF5000100, 0x07000000);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xE6000000, 0);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xF0000000, 0x073FC000);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xE7000000, 0);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xFD480000 | ((sprite->tileSize - 1) & 0xFFF),
                                                            (u32)(sprite->image +
                                                                  ((((imageIndex - 1) * sprite->tileSize *
                                                                     sprite->tileXStep) /
                                                                    2) *
                                                                   2)));
                                MENU_RENDER_SPRITE_EMIT_GFX(0xF5480000 |
                                                                ((((sprite->tileSize + 8) >> 3) & 0x1FF) << 9),
                                                            0x07080200);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xE6000000, 0);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xF4000000,
                                                            0x07000000 | (((sprite->tileSize * 4) & 0xFFF) << 12) |
                                                                ((sprite->tileXStep * 4) & 0xFFF));
                                MENU_RENDER_SPRITE_EMIT_GFX(0xE7000000, 0);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xF5480000 |
                                                                ((((sprite->tileSize + 8) >> 3) & 0x1FF) << 9),
                                                            0x00080200);
                                MENU_RENDER_SPRITE_EMIT_GFX(0xF2000000,
                                                            (((sprite->tileSize * 4) & 0xFFF) << 12) |
                                                                ((sprite->tileXStep * 4) & 0xFFF));
                            }

                            MENU_RENDER_SPRITE_EMIT_GFX(0xE4000000 | (((drawX1 * 4) & 0xFFF) << 12) |
                                                            ((drawY1 * 4) & 0xFFF),
                                                        (((drawX0 * 4) & 0xFFF) << 12) | ((drawY0 * 4) & 0xFFF));
                            MENU_RENDER_SPRITE_EMIT_GFX(0xB4000000, ((texS << 21) | ((texT << 5) & 0xFFFF)));
                            MENU_RENDER_SPRITE_EMIT_GFX(0xB3000000, (sScale << 26) | ((tScale << 10) & 0xFFFF));
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

#undef MENU_RENDER_SPRITE_EMIT_GFX
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
    if (((u8 *)&palette)[3] == 0) {
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
                    drawMenuGlyph(drawX, drawY, glyphCode & 0xFFFF, ((u8 *)&palette)[3], scaleValue, fontBankValue);
                }
                drawX += advance;
            }
            glyphCode = scriptCursor[1];
            scriptCursor++;
        } while ((glyphCode & 0xFFFF) != MENU_GLYPH_SCRIPT_END);
    }
}

void drawMenuGlyphScriptDefaultFont(volatile s16 x, s16 y, MenuGlyphScript *script, s32 palette, u16 scale) {
    u16 firstGlyph;
    s32 glyphCode;
    MenuGlyphScript *scriptCursor;
    s32 drawX;
    u16 glyphAdvance;
    s32 drawY;
    u16 scaleValue;
    s32 advance;

    drawX = x;
    drawY = y;
    if (((u8 *)&palette)[3] == 0) {
        glyphAdvance = MENU_GLYPH_WIDE_ADVANCE;
    } else {
        glyphAdvance = MENU_GLYPH_NARROW_ADVANCE;
    }

    firstGlyph = *script ^ 0;
    if (MENU_GLYPH_SCRIPT_END != firstGlyph) {
        scriptCursor = script;
        scaleValue = scale;
        glyphCode = firstGlyph;
        do {
            if (MENU_GLYPH_SCRIPT_NEWLINE == (glyphCode & 0xFFFF)) {
                drawX = x;
                drawY += MENU_GLYPH_LINE_HEIGHT;
            } else {
                advance = glyphAdvance;
                if ((glyphCode & 0xFFFF) != MENU_GLYPH_SCRIPT_SPACE) {
                    drawMenuGlyph(drawX, drawY, glyphCode & 0xFFFF, ((u8 *)&palette)[3], scaleValue,
                                  MENU_GLYPH_DEFAULT_FONT_BANK);
                }
                drawX += advance;
            }
            glyphCode = scriptCursor[1];
            scriptCursor++;
        } while (MENU_GLYPH_SCRIPT_END != (glyphCode & 0xFFFF));
    }
}

// drawMenuGlyph best match: 87.326% (nonmatchings/drawMenuGlyph-1219509448159986855/base_15.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuGlyph.s")

#ifdef NON_MATCHING
void drawMenuGlyph(s16 x, s16 y, u16 glyph, u8 palette, u16 scale, u16 fontBank) {
    MenuFontAssetTable *fontAsset;
    MenuFontAssetEntry *glyphTexture;
    u16 *fontPalettes;
    u16 *glyphPalette;
    u16 *scaledPalette;
    s32 glyphWidth;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 viewHalfWidth;
    s32 viewHalfHeight;
    s32 viewportLeft;
    s32 viewportRight;
    s32 viewportTop;
    s32 viewportBottom;
    s32 clipS;
    s32 clipT;
    s32 drawX0;
    volatile s32 drawY0;
    s32 drawX1;
    s32 drawY1;
    s32 i;
    u16 color;
    s32 red;
    u8 green;
    u16 blue;

    if (palette == 0) {
        fontAsset = (MenuFontAssetTable *)getRelocatableHeapBlockBase(gAssetHandles[fontBank]);
        glyphWidth = MENU_GLYPH_WIDE_WIDTH;
    } else {
        fontAsset = (MenuFontAssetTable *)getRelocatableHeapBlockBase(gAssetHandles[fontBank + 1]);
        glyphWidth = MENU_GLYPH_NARROW_WIDTH;
    }

    fontPalettes = (u16 *)(&fontAsset->entries[fontAsset->entryCount]);
    x0 = x + gMenuViewportCenterX;
    y0 = y + gMenuViewportCenterY;
    x1 = x0 + glyphWidth;
    y1 = y0 + MENU_GLYPH_TALL_HEIGHT;
    clipS = 0;
    clipT = 0;

    viewHalfWidth = gMenuViewportWidth / 2;
    viewportRight = gMenuViewportCenterX + viewHalfWidth;
    if (x0 < viewportRight) {
        viewportLeft = gMenuViewportCenterX - viewHalfWidth;
        viewHalfHeight = gMenuViewportHeight / 2;
        viewportBottom = gMenuViewportCenterY + viewHalfHeight;
        if ((y0 < viewportBottom) && (x1 >= viewportLeft)) {
            viewportTop = gMenuViewportCenterY - viewHalfHeight;
            if (y1 >= viewportTop) {
                if (x0 < viewportLeft) {
                    clipS = viewportLeft - x0;
                    x0 = viewportLeft;
                }
                if (y0 < viewportTop) {
                    clipT = viewportTop - y0;
                    y0 = viewportTop;
                }
                if (x1 >= viewportRight) {
                    x1 = viewportRight - 1;
                }
                if (y1 >= viewportBottom) {
                    y1 = viewportBottom - 1;
                }
                drawX0 = x0;
                drawY0 = y0;
                drawX1 = x1;
                drawY1 = y1;

                glyphTexture = fontAsset->entries;
                glyphTexture += glyph;
                glyphPalette = fontPalettes + ((u16)glyphTexture->textureIndex * MENU_PALETTE_COLOR_COUNT);
                scaledPalette = allocMenuRenderScratch(MENU_PALETTE_SIZE_BYTES);
                for (i = 0; i != MENU_PALETTE_COLOR_COUNT; i++) {
                    color = glyphPalette[i];
                    scaledPalette[i] = color;
                    if (color & MENU_RGBA5551_ALPHA_BIT) {
                        red = (((color >> 11) & MENU_RGBA5551_CHANNEL_MASK) * scale) / MENU_RGBA5551_SCALE_BASE;
                        green = (((color >> 6) & MENU_RGBA5551_CHANNEL_MASK) * scale) / MENU_RGBA5551_SCALE_BASE;
                        blue = (((color >> 1) & MENU_RGBA5551_CHANNEL_MASK) * scale) / MENU_RGBA5551_SCALE_BASE;
                        scaledPalette[i] = (red << 11) | (green << 6) | (blue << 1) | MENU_RGBA5551_ALPHA_BIT;
                    }
                }

                FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)scaledPalette);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);

                FONT_GFX_CMD(gRegionAllocPtr++, (((glyphTexture->width >> 1) - 1) & 0xFFF) | 0xFD480000,
                             (u32)((u8 *)fontAsset + glyphTexture->imageOffset));
                FONT_GFX_CMD(gRegionAllocPtr++,
                             ((((((s32)(glyphTexture->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5480000,
                             0x07080200);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000,
                             (((glyphTexture->width * 2) & 0xFFF) << 12) | 0x07000000 |
                                 ((glyphTexture->height * 4) & 0xFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++,
                             ((((((s32)(glyphTexture->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5400000,
                             0x00080200);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000,
                             (((glyphTexture->width * 4) & 0xFFF) << 12) | ((glyphTexture->height * 4) & 0xFFF));
                FONT_GFX_CMD(gRegionAllocPtr++,
                             0xE4000000 | ((((drawX1) << MENU_GLYPH_RECT_FRAC_BITS) & 0xFFF) << 12) |
                                 (((drawY1) << MENU_GLYPH_RECT_FRAC_BITS) & 0xFFF),
                             ((((drawX0) << MENU_GLYPH_RECT_FRAC_BITS) & 0xFFF) << 12) |
                                 (((drawY0) << MENU_GLYPH_RECT_FRAC_BITS) & 0xFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000,
                             (clipS << (16 + MENU_GLYPH_TEXEL_FRAC_BITS)) |
                                 ((clipT << MENU_GLYPH_TEXEL_FRAC_BITS) & 0xFFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xB3000000,
                             (MENU_GLYPH_TEXTURE_STEP << 16) | MENU_GLYPH_TEXTURE_STEP);
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
    if (((u8 *)&palette)[3] == 0) {
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
                    drawMenuColoredGlyph(drawX, drawY, glyphCode & 0xFFFF, ((u8 *)&palette)[3], scaleValue, colorModeValue,
                                         fontBankValue);
                }
                drawX += advance;
            }
            glyphCode = scriptCursor[1];
            scriptCursor++;
        } while (MENU_GLYPH_SCRIPT_END != (glyphCode & 0xFFFF));
    }
}

void drawMenuGlyphScript(volatile s16 x, s16 y, MenuGlyphScript *script, s32 palette, u16 scale, volatile u16 colorMode) {
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
    if (((u8 *)&palette)[3] == 0) {
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
                drawMenuColoredGlyph(drawX, drawY, glyphCode & 0xFFFF, ((u8 *)&palette)[3], scaleValue, colorModeValue,
                                     MENU_GLYPH_DEFAULT_FONT_BANK);
                drawX += (advance = glyphAdvance);
            }
            glyphCode = scriptCursor[1];
            scriptCursor++;
        } while (MENU_GLYPH_SCRIPT_END != (glyphCode & 0xFFFF));
        colorMode = colorModeValue;
    }
}

// drawMenuColoredGlyph best match: 84.271%
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuColoredGlyph.s")

#ifdef NON_MATCHING
void drawMenuColoredGlyph(s16 x, s16 y, u16 glyph, u8 palette, u16 paletteScale, u16 paletteIndex, s32 fontBank) {
    u8 *font;
    s32 glyphWidth;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 viewHalfWidth;
    s32 viewHalfHeight;
    s32 minX;
    s32 maxX;
    s32 minY;
    s32 maxY;
    s32 clipS;
    s32 clipT;
    volatile s32 drawX0;
    volatile s32 drawY0;
    volatile s32 drawX1;
    volatile s32 drawY1;
    u16 *srcPalette;
    u16 *paletteBase;
    u16 *dstPalette;
    MenuFontAssetEntry *entry;
    s32 i;
    u16 color;
    s32 red;
    s32 green;
    s32 blue;

    if (palette == 0) {
        font = (u8 *)getRelocatableHeapBlockBase(gAssetHandles[(u16)fontBank]);
        glyphWidth = 0x10;
    } else {
        font = (u8 *)getRelocatableHeapBlockBase(gAssetHandles[(u16)fontBank + 1]);
        glyphWidth = 8;
    }
    paletteBase = (u16 *)(&((MenuFontAssetTable *)font)->entries[((MenuFontAssetTable *)font)->entryCount]);

    x0 = x + gMenuViewportCenterX;
    y0 = y + gMenuViewportCenterY;
    x1 = x0 + glyphWidth;
    y1 = y0 + 0x10;
    clipS = 0;
    clipT = 0;

    viewHalfWidth = gMenuViewportWidth / 2;
    maxX = gMenuViewportCenterX + viewHalfWidth;
    if (x0 < maxX) {
        minX = gMenuViewportCenterX - viewHalfWidth;
        viewHalfHeight = gMenuViewportHeight / 2;
        maxY = gMenuViewportCenterY + viewHalfHeight;
        if ((y0 < maxY) && (x1 >= minX)) {
            minY = gMenuViewportCenterY - viewHalfHeight;
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
                drawX0 = x0;
                drawY0 = y0;
                drawX1 = x1;
                drawY1 = y1;

                dstPalette = allocMenuRenderScratch(0x20);
                srcPalette = paletteBase + ((u16)paletteIndex * 0x10);
                for (i = 0; i != 0x10; i++) {
                    color = srcPalette[i];
                    dstPalette[i] = color;
                    if (color & 1) {
                        red = (((color >> 11) & 0x1F) * paletteScale) / 0x100;
                        green = (((color >> 6) & 0x1F) * paletteScale) / 0x100;
                        blue = (((color >> 1) & 0x1F) * paletteScale) / 0x100;
                        dstPalette[i] = (red << 11) | (green << 6) | (blue << 1) | 1;
                    }
                }

                entry = &((MenuFontAssetTable *)font)->entries[glyph];
                FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)dstPalette);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);

                FONT_GFX_CMD(gRegionAllocPtr++, (((entry->width >> 1) - 1) & 0xFFF) | 0xFD480000,
                             (u32)(font + entry->imageOffset));
                FONT_GFX_CMD(gRegionAllocPtr++,
                             ((((((s32)(entry->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5480000,
                             0x07080200);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000,
                             (((entry->width * 2) & 0xFFF) << 12) | 0x07000000 |
                                 ((entry->height * 4) & 0xFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++,
                             ((((((s32)(entry->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5400000,
                             0x00080200);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000,
                             (((entry->width * 4) & 0xFFF) << 12) | ((entry->height * 4) & 0xFFF));
                FONT_GFX_CMD(gRegionAllocPtr++,
                             0xE4000000 | (((drawX1 << 2) & 0xFFF) << 12) | ((drawY1 << 2) & 0xFFF),
                             (((drawX0 << 2) & 0xFFF) << 12) | ((drawY0 << 2) & 0xFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000,
                             (clipS << 21) | ((clipT << 5) & 0xFFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xB3000000, 0x04000400);
            }
        }
    }
}
#endif

// drawMenuAsciiGlyph best match: 97.336% (nonmatchings/drawMenuAsciiGlyph-8699393380584516020/base_16.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_renderer/drawMenuAsciiGlyph.s")

#ifdef NON_MATCHING
void drawMenuAsciiGlyph(s16 x, s16 y, u16 tileS, s32 tileT, u16 paletteIndex, u16 paletteScale) {
    FontAsset *asset;
    FontTexture *atlasTexture;
    s32 x0;
    u16 *scaledPalette;
    s16 selectedPalette;
    s32 y0;
    s32 x1;
    s32 y1;
    u16 *sourcePalette;
    s32 halfWidth;
    s32 halfHeight;
    s32 maxX;
    s32 maxY;
    s32 minX;
    s32 minY;
    s32 clipS;
    s32 clipT;
    u16 *paletteBase;
    s32 i;
    u16 paletteColor;
    s32 color;
    s32 red;
    u16 green;
    u16 blue;

    asset = (FontAsset *)getRelocatableHeapBlockBase(gAssetHandles[6]);
    paletteBase = (u16 *)&asset->textures[asset->header.entryCount];
    atlasTexture = &asset->textures[0];
    selectedPalette = atlasTexture->paletteIndex;
    x0 = x + gMenuViewportCenterX;
    y0 = y + gMenuViewportCenterY;
    x1 = x0 + 8;
    y1 = y0 + 8;
    clipS = 0;
    clipT = 0;

    halfWidth = gMenuViewportWidth / 2;
    maxX = gMenuViewportCenterX + halfWidth;
    if (x0 >= maxX) {
        return;
    }
    minX = gMenuViewportCenterX - halfWidth;
    halfHeight = gMenuViewportHeight / 2;
    maxY = gMenuViewportCenterY + halfHeight;
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

    sourcePalette = paletteBase + (selectedPalette * 16);
    scaledPalette = allocMenuRenderScratch(0x20);
 for (i = 0; i != 16; i++) { scaledPalette[i] = (paletteColor = sourcePalette[i]); color = paletteColor & 0xFFFF; if (color & 1) { red = (color >> 11) & 0x1F; green = (color >> 6) & 0x1F; blue = (color >> 1) & 0x1F; red = (red * paletteScale) / 256; green = (green * paletteScale) / 256; color = green; blue = (blue * paletteScale) / 256; scaledPalette[i] = (((red << 11) | (color << 6)) | (blue << 1)) | 1; } } gDPLoadTextureTile_4b(gRegionAllocPtr++, (u8 *)asset + atlasTexture->imageOffset, G_IM_FMT_CI, atlasTexture->width, atlasTexture->height, 0, 0, atlasTexture->width, atlasTexture->height, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, scaledPalette);
    gSPTextureRectangle(gRegionAllocPtr++, x0 * 4, y0 * 4, x1 * 4, y1 * 4, 0, clipS << 5,
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

s32 stepMenuFadeAlpha(value, step, increase)
s32 value;
s16 step;
u8 increase;
{
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
