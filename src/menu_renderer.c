#include "common.h"
#include "callback_task_scheduler.h"
#include "relocatable_heap.h"
#define MENU_RENDERER_C
#include "menu_renderer.h"

#define FONT_GFX_CMD(pkt, cmd0, cmd1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (cmd0); \
    _g->words.w1 = (cmd1); \
}

typedef struct MenuRenderTask MenuRenderTask;
typedef struct RenderCallbackNode RenderCallbackNode;
typedef struct MenuRenderAssetTableHeader MenuRenderAssetTableHeader;
typedef struct MenuRenderAssetTableEntry MenuRenderAssetTableEntry;
typedef struct FontAssetHeader FontAssetHeader;
typedef struct FontTexture FontTexture;
typedef struct FontAsset FontAsset;
typedef struct MenuFontAssetEntry MenuFontAssetEntry;
typedef struct MenuFontAssetTable MenuFontAssetTable;

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

struct MenuRenderAssetTableHeader {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 entryCount;
};

struct MenuRenderAssetTableEntry {
    /* 0x0 */ s32 imageOffset;
    /* 0x4 */ u16 textureIndex;
    /* 0x6 */ u8 width;
    /* 0x7 */ u8 height;
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
extern Gfx gMenuRenderModeResetDl[];
extern s16 D_800B51F0[][2];
extern s16 gMenuFadeAlpha;
extern s16 D_8011213C;
extern s16 gMenuViewportWidth;
extern s16 gMenuViewportHeight;
extern s16 gMenuViewportCenterX;
extern s16 gMenuViewportCenterY;
extern u16 D_800B51D0[];
extern s16 D_800B51F0[][2];

// drawMenuAssetRegion best match: 78.118% (nonmatchings/func_8000EA80-4923837976568703863/base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu_renderer/drawMenuAssetRegion.s")

#ifdef NON_MATCHING
#define MENU_RENDER_EMIT_GFX(cmd0, cmd1) \
    gfx = gRegionAllocPtr;               \
    gRegionAllocPtr = gfx + 1;           \
    gfx->words.w0 = (cmd0);              \
    gfx->words.w1 = (cmd1)

extern Gfx *gRegionAllocPtr;
extern s16 gMenuViewportCenterX;
extern s16 gMenuViewportCenterY;

void drawMenuAssetRegion(s16 x, s16 y, MenuRenderAssetTableHeader *table, u16 entryIndex, u16 scaleX, u16 scaleY,
                   u8 startS, u8 startT, u8 width, u8 height) {
    MenuRenderAssetTableEntry *entry;
    MenuRenderAssetTableHeader *tableBase;
    u8 *paletteBase;
    Gfx *gfx;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 s;
    s32 t;
    s32 clipLeft;
    s32 clipTop;
    s16 clipRight;
    s16 clipBottom;
    volatile u8 pad[0x38];

    tableBase = table;
    paletteBase = (u8 *)tableBase + 8 + (tableBase->entryCount * sizeof(MenuRenderAssetTableEntry));
    x0 = (x + gMenuViewportCenterX) * 4;
    y0 = (y + gMenuViewportCenterY) * 4;
    x1 = x0 + (((width * scaleX) * 4) >> 5);
    y1 = y0 + (((height * scaleY) * 4) >> 5);
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

        entry = (MenuRenderAssetTableEntry *)((u8 *)tableBase + 8 + (entryIndex * sizeof(MenuRenderAssetTableEntry)));

        MENU_RENDER_EMIT_GFX((((entry->width >> 1) - 1) & 0xFFF) | 0xFD480000,
                             entry->imageOffset + (u8 *)tableBase + 0x80000000);
        MENU_RENDER_EMIT_GFX(((((entry->width + 1) >> 1) + 7) >> 3 & 0x1FF) << 9 | 0xF5480000,
                             0x07080200);
        MENU_RENDER_EMIT_GFX(0xE6000000, 0);
        MENU_RENDER_EMIT_GFX(0xF4000000,
                             (((entry->width * 2) & 0xFFF) << 12) | 0x07000000 | ((entry->height * 4) & 0xFFF));
        MENU_RENDER_EMIT_GFX(0xE7000000, 0);
        MENU_RENDER_EMIT_GFX(((((entry->width + 1) >> 1) + 7) >> 3 & 0x1FF) << 9 | 0xF5400000,
                             0x00080200);
        MENU_RENDER_EMIT_GFX(0xF2000000, (((entry->width * 4) & 0xFFF) << 12) | ((entry->height * 4) & 0xFFF));
        MENU_RENDER_EMIT_GFX(0xFD100000, &paletteBase[entry->textureIndex * 0x20] + 0x80000000);
        MENU_RENDER_EMIT_GFX(0xE8000000, 0);
        MENU_RENDER_EMIT_GFX(0xF5000100, 0x07000000);
        MENU_RENDER_EMIT_GFX(0xE6000000, 0);
        MENU_RENDER_EMIT_GFX(0xF0000000, 0x0703C000);
        MENU_RENDER_EMIT_GFX(0xE7000000, 0);
        MENU_RENDER_EMIT_GFX((((x1) & 0xFFF) << 12) | 0xE4000000 | ((y1) & 0xFFF),
                             (((x0) & 0xFFF) << 12) | ((y0) & 0xFFF));
        MENU_RENDER_EMIT_GFX(0xB4000000, (s << 16) | (t & 0xFFFF));
        MENU_RENDER_EMIT_GFX(0xB3000000, ((0x8000 / scaleX) << 16) | ((0x8000 / scaleY) & 0xFFFF));
    }
}

#undef MENU_RENDER_EMIT_GFX
#endif

void drawMenuSprite(s16 arg0, s16 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u8 arg6, u8 arg7) {
    s32 temp_v0;
    s32 temp_v1;

    drawMenuSpriteClipped(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7,
                  temp_v0 = (s16)(gMenuViewportWidth / 2), temp_v1 = (s16)(gMenuViewportHeight / 2), temp_v0, temp_v1);
}

// drawMenuSpriteClipped best match: 57.528% (nonmatchings/drawMenuSpriteClipped-7273315160691878794/base_3.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu_renderer/drawMenuSpriteClipped.s")

#ifdef NON_MATCHING
void drawMenuSpriteClipped(s16 x, s16 y, MenuFontAssetTable *table, u16 imageIndex, u16 width, u16 height, u8 flip, u8 paletteIndex,
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

    flipS = D_800B51F0[flip & 3][0];
    flipT = D_800B51F0[flip & 3][1];
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

// drawMenuSpriteWithAlphaClipped best match: 82.673% (nonmatchings/drawMenuSpriteWithAlphaClipped-6113366811127043669/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu_renderer/drawMenuSpriteWithAlphaClipped.s")

#ifdef NON_MATCHING
void drawMenuSpriteWithAlphaClipped(s16 x, s16 y, FontAsset *asset, u16 tileIndex, u16 scaleX, u16 scaleY, u8 flipMode, u16 alpha,
                   u8 paletteArg, s16 clipLeft, s16 clipTop, s16 clipRight, s16 clipBottom) {
    FontTexture *texture;
    u8 *textureBase;
    u8 *paletteBase;
    s16 flipS;
    s16 flipT;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 minX;
    s32 minY;
    s32 maxX;
    s32 maxY;
    s32 texS;
    s32 texT;
    s32 texWidth;
    s32 texHeight;
    s32 scaleXValue;
    s32 scaleYValue;
    s32 color;
    s32 palette;

    paletteBase = (u8 *)asset + 8 + (asset->header.entryCount * sizeof(FontTexture));

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
        flipS = D_800B51F0[flipMode & 3][0];
        flipT = D_800B51F0[flipMode & 3][1];
        textureBase = (u8 *)asset + (tileIndex * sizeof(FontTexture));
        texWidth = textureBase[0xE];
        texHeight = textureBase[0xF];
        texture = (FontTexture *)(textureBase + 8);

        left = (x + gMenuViewportCenterX) << 2;
        top = (y + gMenuViewportCenterY) << 2;
        right = (((scaleX * texWidth) << 2) >> 5) + left;
        bottom = (((scaleY * texHeight) << 2) >> 5) + top;
        texS = 0;
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

            if (paletteArg != 0) {
                palette = paletteArg - 1;
            } else {
                palette = texture->paletteIndex;
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
            gSPTextureRectangle(gRegionAllocPtr++, left, top, right, bottom, 0, texS, texT,
                                (u16)((0x8000 / scaleXValue) * flipS), (u16)((0x8000 / scaleYValue) * flipT));
            if (alpha != 0x100) {
                gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
            }
        }
    }
}
#endif

// drawMenuSpriteWithPaletteScale best match: 77.583% (nonmatchings/drawMenuSpriteWithPaletteScale-5802343343535905907/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu_renderer/drawMenuSpriteWithPaletteScale.s")

#ifdef NON_MATCHING
void drawMenuSpriteWithPaletteScale(s16 x, s16 y, FontAsset *asset, u16 index, s32 alpha) {
    FontTexture *texture;
    u8 *textureBase;
    u8 *paletteBase;
    u16 *srcPalette;
    u16 *palette;
    u16 color;
    u16 alphaScale;
    s32 i;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 minX;
    s32 minY;
    s32 maxX;
    s32 maxY;
    s32 srcX;
    s32 srcY;
    s32 red;
    s32 green;
    s32 blue;

    textureBase = (u8 *)asset + (index * sizeof(FontTexture));
    texture = (FontTexture *)(textureBase + 8);
    alphaScale = alpha;
    paletteBase = (u8 *)asset + 8 + (asset->header.entryCount * sizeof(FontTexture));
    left = x + gMenuViewportCenterX;
    top = y + gMenuViewportCenterY;
    right = left + (texture->width >> 1);
    bottom = top + (texture->height >> 1);
    srcX = 0;
    srcY = 0;

    maxX = gMenuViewportCenterX + (gMenuViewportWidth / 2);
    minX = gMenuViewportCenterX - (gMenuViewportWidth / 2);
    if (left < maxX) {
        maxY = gMenuViewportCenterY + (gMenuViewportHeight / 2);
        minY = gMenuViewportCenterY - (gMenuViewportHeight / 2);
        if ((top < maxY) && (right >= minX) && (bottom >= minY)) {
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

            gDPPipeSync(gRegionAllocPtr++);
            FONT_GFX_CMD(gRegionAllocPtr++, 0xBA000C02, 0x3000);

            srcPalette = (u16 *)(paletteBase + (texture->paletteIndex * 0x20));
            palette = allocMenuRenderScratch(0x20);
            for (i = 0; i != 0x10; i++) {
                color = srcPalette[i];
                palette[i] = color;
                if (color & 1) {
                    red = (((color >> 11) & 0x1F) * alphaScale) / 256;
                    green = (((color >> 6) & 0x1F) * alphaScale) / 256;
                    blue = (((color >> 1) & 0x1F) * alphaScale) / 256;
                    palette[i] = (red << 11) | (green << 6) | (blue << 1) | 1;
                }
            }

            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
            gDPLoadTextureTile_4b(gRegionAllocPtr++, (u8 *)asset + texture->imageOffset, G_IM_FMT_CI, texture->width,
                                  texture->height, 0, 0, texture->width, texture->height, 0, G_TX_WRAP, G_TX_WRAP, 0, 0,
                                  0, 0);
            gSPTextureRectangle(gRegionAllocPtr++, left << 2, top << 2, right << 2, bottom << 2, 0,
                                (srcX << 5) + 0x10, (srcY << 5) + 0x10, 0x800, 0x800);
            gDPPipeSync(gRegionAllocPtr++);
            FONT_GFX_CMD(gRegionAllocPtr++, 0xBA000C02, 0);
            gDPPipeSync(gRegionAllocPtr++);
        }
    }
}
#endif

// drawMenuSpriteSubrect best match: 87.440% (nonmatchings/drawMenuSpriteSubrect-5802343343535905907/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu_renderer/drawMenuSpriteSubrect.s")

#ifdef NON_MATCHING
void drawMenuSpriteSubrect(s16 x, s16 y, FontAsset *asset, u16 index, u8 srcX, u8 srcY, u8 width, u8 height, s32 scaleX,
                   s32 scaleY) {
    FontTexture *texture;
    u8 *paletteBase;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s16 maxY;
    s16 maxX;
    s32 minY;
    s32 minX;
    s32 texS;
    s32 texT;
    s32 halfY;
    s32 halfX;

    texture = &asset->textures[index];
    paletteBase = (u8 *)asset + (asset->header.entryCount * sizeof(FontTexture)) + 8;
    left = (x + gMenuViewportCenterX) << 2;
    top = (y + gMenuViewportCenterY) << 2;
    right = (((width * scaleX) << 2) >> 5) + left;
    bottom = (((height * scaleY) << 2) >> 5) + top;
    texS = srcX << 5;
    texT = srcY << 5;

    halfY = gMenuViewportHeight / 2;
    minY = (s16)((gMenuViewportCenterY - halfY) << 2);
    maxY = (gMenuViewportCenterY + halfY) << 2;
    halfX = gMenuViewportWidth / 2;
    minX = (s16)((gMenuViewportCenterX - halfX) << 2);
    maxX = (gMenuViewportCenterX + halfX) << 2;

    if ((left < maxX) && (top < maxY) && (right >= minX) && (bottom >= minY)) {
        if (left < minX) {
            texS += (((minX - left) << 8) / scaleX);
            left = minX;
        }
        if (top < minY) {
            texT += (((minY - top) << 8) / scaleY);
            top = minY;
        }
        if (right >= maxX) {
            right = maxX - 4;
        }
        if (bottom >= maxY) {
            bottom = maxY - 4;
        }

        FONT_GFX_CMD(gRegionAllocPtr++, (((texture->width >> 1) - 1) & 0xFFF) | 0xFD480000,
                     (u8 *)asset + texture->imageOffset + 0x80000000);
        FONT_GFX_CMD(gRegionAllocPtr++, (((((texture->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9 | 0xF5480000,
                     0x07080200);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000,
                     0x07000000 | (((texture->width << 1) & 0xFFF) << 12) | ((texture->height << 2) & 0xFFF));
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
        FONT_GFX_CMD(gRegionAllocPtr++, (((((texture->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9 | 0xF5400000,
                     0x00080200);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000,
                     (((texture->width << 2) & 0xFFF) << 12) | ((texture->height << 2) & 0xFFF));
        FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000,
                     paletteBase + (texture->paletteIndex * 0x20) + 0x80000000);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
        FONT_GFX_CMD(gRegionAllocPtr++, (((right) & 0xFFF) << 12) | 0xE4000000 | ((bottom) & 0xFFF),
                     (((left) & 0xFFF) << 12) | ((top) & 0xFFF));
        FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000, (texS << 16) | (texT & 0xFFFF));
        FONT_GFX_CMD(gRegionAllocPtr++, 0xB3000000, ((0x8000 / scaleX) << 16) | ((0x8000 / scaleY) & 0xFFFF));
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/menu_renderer/drawMenuSpriteFixedScale.s")

void drawMenuSpriteTile(s16 arg0, s16 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5) {
    drawMenuSpriteTileClipped(arg0, arg1, arg2, arg3, arg4, arg5, gMenuViewportWidth / 2, gMenuViewportHeight / 2);
}

// drawMenuSpriteTileClipped best match: 80.680% (nonmatchings/drawMenuSpriteTileClipped-6061209858023118177/base_13.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu_renderer/drawMenuSpriteTileClipped.s")

#ifdef NON_MATCHING
void drawMenuSpriteTileClipped(s16 x, s16 y, MenuFontAssetTable *table, u16 entryIndex, u16 unused, u16 alpha, s16 clipRight,
                   s16 clipBottom) {
    MenuFontAssetEntry *entry;
    u8 *paletteBase;
    s16 minX;
    s16 maxX;
    s16 minY;
    s16 maxY;
    s32 x0;
    volatile s32 y0;
    s32 x1;
    s32 y1;
    volatile s32 clipS;
    volatile s32 clipT;
    s32 halfWidth;
    s32 halfHeight;

    entry = &table->entries[entryIndex];
    paletteBase = (u8 *)&table->entries[table->entryCount];
    x0 = x + gMenuViewportCenterX;
    y0 = y + gMenuViewportCenterY;
    x1 = x0 + entry->width;
    y1 = y0 + entry->height;
    minX = gMenuViewportCenterX - clipRight;
    minY = gMenuViewportCenterY - clipBottom;
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

    if ((x0 < maxX) && (y0 < maxY) && (x1 >= minX) && (y1 >= minY)) {
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

        FONT_GFX_CMD(gRegionAllocPtr++, 0xFD480000 | ((entry->width - 1) & 0xFFF),
                     (u32)((u8 *)table + entry->imageOffset));
        FONT_GFX_CMD(gRegionAllocPtr++, ((((entry->width + 8) >> 3) & 0x1FF) << 9) | 0xF5480000, 0x07080200);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000,
                     0x07000000 | (((entry->width << 2) & 0xFFF) << 12) | ((entry->height << 2) & 0xFFF));
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
        FONT_GFX_CMD(gRegionAllocPtr++, ((((entry->width + 8) >> 3) & 0x1FF) << 9) | 0xF5480000, 0x00080200);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000,
                     (((entry->width << 2) & 0xFFF) << 12) | ((entry->height << 2) & 0xFFF));
        if (alpha != 0x100) {
            FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            FONT_GFX_CMD(gRegionAllocPtr++, 0xFC119623, 0xFF2FFFFF);
            FONT_GFX_CMD(gRegionAllocPtr++, 0xFA000000,
                         ((alpha & 0xFF) << 0x18) | ((alpha & 0xFF) << 0x10) | ((alpha & 0xFF) << 8) | 0xFF);
        }
        FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)(paletteBase + (entry->textureIndex << 5)));
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x073FC000);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE4000000 | (((x1 << 2) & 0xFFF) << 12) | ((y1 << 2) & 0xFFF),
                     (((x0 << 2) & 0xFFF) << 12) | ((y0 << 2) & 0xFFF));
        FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000, ((clipS << 21) & 0xFFFF0000) | ((clipT << 5) & 0xFFFF));
        FONT_GFX_CMD(gRegionAllocPtr++, 0xB3000000, 0x04000400);
        if (alpha != 0x100) {
            FONT_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)gMenuRenderModeResetDl);
        }
    }
}
#endif

void func_80011854(void) {
}

// drawMenuTextureByAssetId best match: 99.328% (nonmatchings/drawMenuTextureByAssetId-6061209858023118177/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu_renderer/drawMenuTextureByAssetId.s")

#ifdef NON_MATCHING
extern s16 D_8011214A;
extern Gfx *gRegionAllocPtr;
extern void getAssetTableImageAndPalette(void *arg0, u16 arg1, void **arg2, void **arg3);

void drawMenuTextureByAssetId(s16 x, s16 y, s32 arg2, u16 assetId, u16 width, u16 height)
{
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
    if ((((width < 0x201) && (width > 0)) && (height < 0x201)) && (height > 0))
    {
        x0 = (x + gMenuViewportCenterX) * 4;
        y0 = (y + gMenuViewportCenterY) * 4;
        x1 = (((width * 64) << 2) >> 5) + x0;
        y1 = (((height * 64) << 2) >> 5) + y0;
        clipY0 = (gMenuViewportCenterY - (gMenuViewportHeight / 2)) * 4;
        clipY1 = (gMenuViewportCenterY + (gMenuViewportHeight / 2)) * 4;
        clipX0 = (gMenuViewportCenterX - (gMenuViewportWidth / 2)) * 4;
        clipX1 = (gMenuViewportCenterX + (gMenuViewportWidth / 2)) * 4;
        if ((((x0 < clipX1) && (y0 < clipY1)) && (x1 >= clipX0)) && (y1 >= clipY0))
        {
            if (x0 < clipX0)
            {
                x0 = clipX0;
            }
            if (y0 < clipY0)
            {
 y0 = clipY0; } if (x1 >= clipX1) { x1 = clipX1 - 4; } if (y1 >= clipY1) { y1 = clipY1 - 4; } getAssetTableImageAndPalette(getRelocatableHeapBlockBase(D_8011214A), assetId, &image, &palette); { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = ((((unsigned int) ((((unsigned int) 0xfd) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 2) & ((0x01 << 3) - 1)) << 21))) | ((unsigned int) ((((unsigned int) 2) & ((0x01 << 2) - 1)) << 19))) | ((unsigned int) ((((unsigned int) (1 - 1)) & ((0x01 << 12) - 1)) << 0)); _g->words.w1 = (unsigned int) (&image); if (((!_g) && (!_g)) && (!_g)) { } } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((((unsigned int) ((((unsigned int) 0xf5) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 2) & ((0x01 << 3) - 1)) << 21))) | ((unsigned int) ((((unsigned int) 2) & ((0x01 << 2) - 1)) << 19))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 9) - 1)) << 9))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 9) - 1)) << 0)); _g->words.w1 = ((((((((unsigned int) ((((unsigned int) 7) & ((0x01 << 3) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 20))) | ((unsigned int) ((((unsigned int) 0x2) & ((0x01 << 2) - 1)) << 18))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 14))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 10))) | ((unsigned int) ((((unsigned int) 0x2) & ((0x01 << 2) - 1)) << 8))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 4))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 0)); } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (unsigned int) ((((unsigned int) 0xe6) & ((0x01 << 8) - 1)) << 24); _g->words.w1 = 0; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((unsigned int) ((((unsigned int) 0xf3) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 12) - 1)) << 12))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 12) - 1)) << 0)); _g->words.w1 = (((unsigned int) ((((unsigned int) 7) & ((0x01 << 3) - 1)) << 24)) | ((unsigned int) ((((unsigned int) MIN((((64 * 64) + 3) >> 2) - 1, 2047)) & ((0x01 << 12) - 1)) << 12))) | ((unsigned int) ((((unsigned int) ((((1 << 11) + MAX(1, 64 / 16)) - 1) / MAX(1, 64 / 16))) & ((0x01 << 12) - 1)) << 0)); } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (unsigned int) ((((unsigned int) 0xe7) & ((0x01 << 8) - 1)) << 24); _g->words.w1 = 0; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((((unsigned int) ((((unsigned int) 0xf5) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 2) & ((0x01 << 3) - 1)) << 21))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 2) - 1)) << 19))) | ((unsigned int) ((((unsigned int) (((64 >> 1) + 7) >> 3)) & ((0x01 << 9) - 1)) << 9))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 9) - 1)) << 0)); _g->words.w1 = ((((((((unsigned int) ((((unsigned int) 0) & ((0x01 << 3) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 20))) | ((unsigned int) ((((unsigned int) 0x2) & ((0x01 << 2) - 1)) << 18))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 14))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 10))) | ((unsigned int) ((((unsigned int) 0x2) & ((0x01 << 2) - 1)) << 8))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 4))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 0)); } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((unsigned int) ((((unsigned int) 0xf2) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 12) - 1)) << 12))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 12) - 1)) << 0)); _g->words.w1 = (((unsigned int) ((((unsigned int) 0) & ((0x01 << 3) - 1)) << 24)) | ((unsigned int) ((((unsigned int) ((64 - 1) << 2)) & ((0x01 << 12) - 1)) << 12))) | ((unsigned int) ((((unsigned int) ((64 - 1) << 2)) & ((0x01 << 12) - 1)) << 0)); } } ; { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = ((((unsigned int) ((((unsigned int) 0xfd) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 3) - 1)) << 21))) | ((unsigned int) ((((unsigned int) 2) & ((0x01 << 2) - 1)) << 19))) | ((unsigned int) ((((unsigned int) (1 - 1)) & ((0x01 << 12) - 1)) << 0)); _g->words.w1 = (unsigned int) palette; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (unsigned int) ((((unsigned int) 0xe8) & ((0x01 << 8) - 1)) << 24); _g->words.w1 = 0; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((((unsigned int) ((((unsigned int) 0xf5) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 3) - 1)) << 21))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 2) - 1)) << 19))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 9) - 1)) << 9))) | ((unsigned int) ((((unsigned int) (256 + ((0 & 0xf) * 16))) & ((0x01 << 9) - 1)) << 0)); _g->words.w1 = ((((((((unsigned int) ((((unsigned int) 7) & ((0x01 << 3) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 20))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 2) - 1)) << 18))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 14))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 10))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 2) - 1)) << 8))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 4))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 0)); } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (unsigned int) ((((unsigned int) 0xe6) & ((0x01 << 8) - 1)) << 24); _g->words.w1 = 0;
                }
                ;
                {
                    Gfx *_g = (Gfx *) (gRegionAllocPtr++);
                    _g->words.w0 = (unsigned int) ((((unsigned int) 0xf0) & ((0x01 << 8) - 1)) << 24);
                    _g->words.w1 = ((unsigned int) ((((unsigned int) 7) & ((0x01 << 3) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 15) & ((0x01 << 10) - 1)) << 14));
                }
                ;
                {
                    Gfx *_g = (Gfx *) (gRegionAllocPtr++);
                    _g->words.w0 = (unsigned int) ((((unsigned int) 0xe7) & ((0x01 << 8) - 1)) << 24);
                    _g->words.w1 = 0;
                }
            }
            ;
            {
                Gfx *_g = (Gfx *) (gRegionAllocPtr++);
                _g->words.w0 = (((unsigned int) ((((unsigned int) 0xe4) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) x1) & ((0x01 << 12) - 1)) << 12))) | ((unsigned int) ((((unsigned int) y1) & ((0x01 << 12) - 1)) << 0));
                _g->words.w1 = (((unsigned int) ((((unsigned int) 0) & ((0x01 << 3) - 1)) << 24)) | ((unsigned int) ((((unsigned int) x0) & ((0x01 << 12) - 1)) << 12))) | ((unsigned int) ((((unsigned int) y0) & ((0x01 << 12) - 1)) << 0));
                {
                    Gfx *_g = (Gfx *) (gRegionAllocPtr++);
                    _g->words.w0 = (unsigned int) ((((unsigned int) ((-65) - 11)) & ((0x01 << 8) - 1)) << 24);
                    _g->words.w1 = (unsigned int) (((unsigned int) ((((unsigned int) 0) & ((0x01 << 16) - 1)) << 16)) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 16) - 1)) << 0)));
                }
                ;
                {
                    Gfx *_g = (Gfx *) (gRegionAllocPtr++);
                    _g->words.w0 = (unsigned int) ((((unsigned int) ((-65) - 12)) & ((0x01 << 8) - 1)) << 24);
                    _g->words.w1 = (unsigned int) (((unsigned int) ((((unsigned int) 0x400) & ((0x01 << 16) - 1)) << 16)) | ((unsigned int) ((((unsigned int) 0x400) & ((0x01 << 16) - 1)) << 0)));
                }
                ;
            }
            ;
        }
    }
}
#endif

void func_80011C3C(MenuRenderSpriteActor *actor);
void func_80011D44(MenuRenderSprite *sprite);

void func_80011C18(MenuRenderSpriteActor *arg0) {
    setCallbackTaskCallback(arg0, func_80011C3C);
}

void func_80011C3C(MenuRenderSpriteActor *actor) {
    if (gPlayerInputHeld & 8) {
        actor->sprite.y++;
    }
    if (gPlayerInputHeld & 4) {
        actor->sprite.y--;
    }
    if (gPlayerInputHeld & 1) {
        actor->sprite.x++;
    }
    if (gPlayerInputHeld & 2) {
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

    addRenderCallback(&gMenuRenderCallbackList, func_80011D44, &actor->sprite);
}

void func_80011D44(MenuRenderSprite *arg0) {
    drawMenuTilemapSprite(arg0, 0, 0, 0);
}

void func_80011D6C(void) {
}

// drawMenuTilemapSprite best match: 74.828% (nonmatchings/drawMenuTilemapSprite-7123131487808489545/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu_renderer/drawMenuTilemapSprite.s")

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
    s16 sScale;
    s16 tScale;
    s16 tileIndex;
    s16 imageIndex;
    s16 paletteIndex;
    s16 flip;
    s16 tileWidth;
    s16 tileHeight;
    s16 screenX;
    s16 screenY;
    Gfx *gfx;

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
                    tileIndex = sprite->tilemap[tilemapX + (tilemapY * sprite->tileYStep)];
                    imageIndex = ((MenuRenderTileInfo *)sprite->tileInfo)[tileIndex].imageIndex;
                    paletteIndex = ((MenuRenderTileInfo *)sprite->tileInfo)[tileIndex].paletteIndex;
                    if (tileIndex != 0) {
                        flip = ((MenuRenderTileInfo *)sprite->tileInfo)[tileIndex].flip;
                        sScale = D_800B51F0[flip][0];
                        tScale = D_800B51F0[flip][1];
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
                                MENU_RENDER_SPRITE_EMIT_GFX(0xFD100000, (u32)(sprite->palette + (paletteIndex << 4)));
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
                                MENU_RENDER_SPRITE_EMIT_GFX(0xFD100000, (u32)(sprite->palette + (paletteIndex << 4)));
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

void drawMenuGlyphScriptWithFontBank(volatile s16 x, s16 y, u16 *script, s32 palette, u16 scale, u16 arg5) {
    u16 first;
    s32 code;
    u16 *ptr;
    s32 xPos;
    u16 xStep;
    s32 yPos;
    u16 scaleValue;
    register s32 advance;
    u16 arg5Value;

    xPos = x;
    yPos = y;
    if (((u8 *)&palette)[3] == 0) {
        xStep = 0x10;
    } else {
        xStep = 8;
    }

    first = *script ^ 0;
    if (first != 0xFFFF) {
        ptr = script;
        scaleValue = scale;
        arg5Value = arg5;
        code = first;
        do {
            if ((code & 0xFFFF) == 0xFFFD) {
                xPos = x;
                yPos += 0x10;
            } else {
                advance = xStep;
                if ((code & 0xFFFF) != 0xFFFE) {
                    drawMenuGlyph(xPos, yPos, code & 0xFFFF, ((u8 *)&palette)[3], scaleValue, arg5Value);
                }
                xPos += advance;
            }
            code = ptr[1];
            ptr++;
        } while ((code & 0xFFFF) != 0xFFFF);
    }
}

void drawMenuGlyphScriptDefaultFont(volatile s16 x, s16 y, u16 *script, s32 palette, u16 scale) {
    u16 first;
    s32 code;
    u16 *ptr;
    s32 xPos;
    s32 yPos;
    s32 skip;
    register s32 advance;
    u16 xStep;
    u16 scaleValue;

    xPos = x;
    yPos = y;
    if (((u8 *)&palette)[3] == 0) {
        xStep = 0x10;
    } else {
        xStep = 8;
    }

    first = *script ^ 0;
    if (0xFFFF != first) {
        ptr = script;
        scaleValue = scale;
        code = first;
        do {
            skip = 0xFFFE;
            if (0xFFFD == (code & 0xFFFF)) {
                xPos = x;
                yPos += 0x10;
            } else {
                advance = xStep;
                if (skip != (code & 0xFFFF)) {
                    drawMenuGlyph(xPos, yPos, code & 0xFFFF, ((u8 *)&palette)[3], scaleValue, 0x22);
                }
                xPos += advance;
            }
            code = ptr[1];
            ptr++;
        } while (0xFFFF != (code & 0xFFFF));
    }
}

// drawMenuGlyph best match: 79.572% (nonmatchings/drawMenuGlyph-5802343343535905907/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu_renderer/drawMenuGlyph.s")

#ifdef NON_MATCHING
void drawMenuGlyph(s16 x, s16 y, u16 glyph, u8 palette, u16 scale, u16 fontBank) {
    MenuFontAssetTable *font;
    MenuFontAssetEntry *entry;
    u16 *paletteBase;
    u16 *srcPalette;
    u16 *dstPalette;
    s32 glyphWidth;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 viewHalfWidth;
    s32 viewHalfHeight;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    s32 clipS;
    s32 clipT;
    s32 drawX0;
    s32 drawY0;
    s32 drawX1;
    s32 drawY1;
    s32 i;
    u16 color;
    s32 red;
    s32 green;
    s32 blue;

    if (palette == 0) {
        font = (MenuFontAssetTable *)getRelocatableHeapBlockBase(gAssetHandles[fontBank]);
        glyphWidth = 0x10;
    } else {
        font = (MenuFontAssetTable *)getRelocatableHeapBlockBase(gAssetHandles[fontBank + 1]);
        glyphWidth = 8;
    }

    paletteBase = (u16 *)(&font->entries[font->entryCount]);
    x0 = x + gMenuViewportCenterX;
    y0 = y + gMenuViewportCenterY;
    x1 = x0 + glyphWidth;
    y1 = y0 + 0x10;
    clipS = 0;
    clipT = 0;

    viewHalfWidth = gMenuViewportWidth / 2;
    right = gMenuViewportCenterX + viewHalfWidth;
    if (x0 < right) {
        left = gMenuViewportCenterX - viewHalfWidth;
        viewHalfHeight = gMenuViewportHeight / 2;
        bottom = gMenuViewportCenterY + viewHalfHeight;
        if ((y0 < bottom) && (x1 >= left)) {
            top = gMenuViewportCenterY - viewHalfHeight;
            if (y1 >= top) {
                if (x0 < left) {
                    clipS = left - x0;
                    x0 = left;
                }
                if (y0 < top) {
                    clipT = top - y0;
                    y0 = top;
                }
                if (x1 >= right) {
                    x1 = right - 1;
                }
                if (y1 >= bottom) {
                    y1 = bottom - 1;
                }
                drawX0 = x0;
                drawY0 = y0;
                drawX1 = x1;
                drawY1 = y1;

                entry = &font->entries[glyph];
                srcPalette = paletteBase + ((u16)entry->textureIndex * 0x10);
                dstPalette = allocMenuRenderScratch(0x20);
                for (i = 0; i != 0x10; i++) {
                    color = srcPalette[i];
                    dstPalette[i] = color;
                    if (color & 1) {
                        red = (((color >> 11) & 0x1F) * scale) / 0x100;
                        green = (((color >> 6) & 0x1F) * scale) / 0x100;
                        blue = (((color >> 1) & 0x1F) * scale) / 0x100;
                        dstPalette[i] = (red << 11) | (green << 6) | (blue << 1) | 1;
                    }
                }

                FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)dstPalette);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);

                FONT_GFX_CMD(gRegionAllocPtr++, (((entry->width >> 1) - 1) & 0xFFF) | 0xFD480000,
                             (u32)((u8 *)font + entry->imageOffset));
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
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE4000000 | (((drawX1 * 4) & 0xFFF) << 12) |
                                                ((drawY1 * 4) & 0xFFF),
                             (((drawX0 * 4) & 0xFFF) << 12) | ((drawY0 * 4) & 0xFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000, (clipS << 21) | ((clipT << 5) & 0xFFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xB3000000, 0x04000400);
            }
        }
    }
}
#endif

void drawMenuColoredGlyphScript(volatile s16 x, s16 y, u16 *script, s32 palette, u16 scale, u16 red, u16 blue) {
    u16 first;
    s32 code;
    u16 *ptr;
    s32 xPos;
    u16 xStep;
    s32 yPos;
    s32 skip;
    register s32 advance;
    u16 scaleValue;
    u16 redValue;
    u16 blueValue;

    xPos = x;
    yPos = y;
    if (!((u8 *)&palette)[3]) {
        xStep = 0x10;
    } else {
        xStep = 8;
    }

    first = *script ^ 0;
    if (0xFFFF != first) {
        ptr = script;
        scaleValue = scale;
        code = first;
        redValue = red;
        blueValue = blue;
        do {
            skip = 0xFFFE;
            if (0xFFFD == (code & 0xFFFF)) {
                xPos = x;
                yPos += 0x10;
            } else {
                advance = xStep;
                if (skip != (code & 0xFFFF)) {
                    drawMenuColoredGlyph(xPos, yPos, code & 0xFFFF, ((u8 *)&palette)[3], scaleValue, redValue, blueValue);
                }
                xPos += advance;
            }
            code = ptr[1];
            ptr++;
        } while (0xFFFF != (code & 0xFFFF));
    }
}

// drawMenuGlyphScript best match: 99.737%
#pragma GLOBAL_ASM("asm/nonmatchings/menu_renderer/drawMenuGlyphScript.s")

#ifdef NON_MATCHING
void drawMenuGlyphScript(volatile s16 x, s16 y, u16 *script, s32 palette, u16 scale, volatile u16 colorMode) {
    u16 first;
    s32 code;
    u16 *ptr;
    s32 xPos;
    s32 yPos;
    s32 skip;
    register s32 advance;
    u16 xStep;
    u16 scaleValue;
    u16 colorModeValue;

    xPos = x;
    yPos = y;
    if (((u8 *)&palette)[3] == 0) {
        xStep = 0x10;
    } else {
        xStep = 8;
    }

    first = *script ^ 0;
    if (0xFFFF != first) {
        ptr = script;
        scaleValue = scale;
        code = first;
        colorModeValue = colorMode;
        do {
            skip = 0xFFFE;
            if (0xFFFD == (code & 0xFFFF)) {
                xPos = x;
                yPos += 0x10;
            } else if (skip == (code & 0xFFFF)) {
                xPos += xStep;
            } else if ((code & 0xFFFF) == 0xFFFC) {
                colorModeValue = ptr[1];
                ptr++;
            } else {
                drawMenuColoredGlyph(xPos, yPos, code & 0xFFFF, ((u8 *)&palette)[3], scaleValue, colorModeValue, 0x22);
                xPos += (advance = xStep);
            }
            code = ptr[1];
            ptr++;
        } while (0xFFFF != (code & 0xFFFF));
        colorMode = colorModeValue;
    }
}
#endif

// drawMenuColoredGlyph best match: 79.241% (nonmatchings/func_80013284-731940616440357983/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu_renderer/drawMenuColoredGlyph.s")

#ifdef NON_MATCHING
void drawMenuColoredGlyph(s16 x, s16 y, u16 glyph, u8 palette, u16 paletteScale, u16 paletteIndex, s32 fontBank) {
    MenuFontAssetTable *font;
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
    s32 line;

    if (palette == 0) {
        font = (MenuFontAssetTable *)getRelocatableHeapBlockBase(gAssetHandles[(u16)fontBank]);
        glyphWidth = 0x10;
    } else {
        font = (MenuFontAssetTable *)getRelocatableHeapBlockBase(gAssetHandles[(u16)fontBank + 1]);
        glyphWidth = 8;
    }
    paletteBase = (u16 *)(&font->entries[font->entryCount]);

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

                FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)dstPalette);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);

                entry = &font->entries[glyph];
                FONT_GFX_CMD(gRegionAllocPtr++, (((entry->width >> 1) - 1) & 0xFFF) | 0xFD480000,
                             (u32)((u8 *)font + entry->imageOffset));
                line = ((((entry->width + 1) >> 1) + 7) >> 3) & 0x1FF;
                FONT_GFX_CMD(gRegionAllocPtr++, (line << 9) | 0xF5480000, 0x07080200);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000,
                             (((entry->width * 2) & 0xFFF) << 12) | 0x07000000 | ((entry->height * 4) & 0xFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, (line << 9) | 0xF5400000, 0x00080200);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000,
                             (((entry->width * 4) & 0xFFF) << 12) | ((entry->height * 4) & 0xFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE4000000 | (((drawX1 * 4) & 0xFFF) << 12) | ((drawY1 * 4) & 0xFFF),
                             (((drawX0 * 4) & 0xFFF) << 12) | ((drawY0 * 4) & 0xFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000, ((clipS << 21) & 0xFFFF0000) | ((clipT << 5) & 0xFFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xB3000000, 0x04000400);
            }
        }
    }
}
#endif

// drawMenuAsciiGlyph best match: 73.956% (nonmatchings/func_800137C8-2225551288923588688/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu_renderer/drawMenuAsciiGlyph.s")

#ifdef NON_MATCHING
void drawMenuAsciiGlyph(s16 x, s16 y, u16 tileX, s32 tileY, u16 palette, u16 scale) {
    FontAsset *asset;
    FontTexture *font;
    u16 *paletteBase;
    u16 *palettePtr;
    u16 *dst;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 halfW;
    s32 halfH;
    s32 clipS;
    s32 clipT;
    s32 offset;
    u16 color;
    s32 red;
    s32 green;
    s32 blue;
    s32 scaleValue;
    s16 paletteValue;

    asset = (FontAsset *)getRelocatableHeapBlockBase(D_8011213C);
    paletteBase = (u16 *)&asset->textures[asset->header.entryCount];
    font = &asset->textures[0];
    x0 = x + gMenuViewportCenterX;
    y0 = y + gMenuViewportCenterY;
    x1 = x0 + 8;
    y1 = y0 + 8;
    clipS = 0;
    clipT = 0;
    paletteValue = *(s16 *)((u8 *)asset + 0xC);

    halfW = gMenuViewportWidth / 2;
    right = gMenuViewportCenterX + halfW;
    if (x0 < right) {
        left = gMenuViewportCenterX - halfW;
        halfH = gMenuViewportHeight / 2;
        bottom = gMenuViewportCenterY + halfH;
        if (y0 < bottom && x1 >= left) {
            top = gMenuViewportCenterY - halfH;
            if (y1 >= top) {
                if (x0 < left) {
                    clipS = left - x0;
                    x0 = left;
                }
                if (y0 < top) {
                    clipT = top - y0;
                    y0 = top;
                }
                if (x1 >= right) {
                    x1 = right - 1;
                }
                if (y1 >= bottom) {
                    y1 = bottom - 1;
                }

                clipS += tileX;
                clipT += (u16)tileY;
                if (paletteValue != palette) {
                    paletteValue = palette;
                }

                dst = allocMenuRenderScratch(0x20);
                palettePtr = &paletteBase[paletteValue * 16];
                scaleValue = scale;
                offset = 0;
                do {
                    color = *(u16 *)((u8 *)palettePtr + offset);
                    offset += 2;
                    *dst = color;
                    if (color & 1) {
                        red = (((color >> 11) & 0x1F) * scaleValue) / 256;
                        green = (((color >> 6) & 0x1F) * scaleValue) / 256;
                        blue = (((color >> 1) & 0x1F) * scaleValue) / 256;
                        *dst = (red << 11) | (green << 6) | (blue << 1) | 1;
                    }
                    dst++;
                } while (offset != 0x20);

                FONT_GFX_CMD(gRegionAllocPtr++, (((font->width >> 1) - 1) & 0xFFF) | 0xFD480000,
                             (u32)(font->imageOffset + (u8 *)asset));
                FONT_GFX_CMD(gRegionAllocPtr++,
                             ((((((s32)(font->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5480000,
                             0x07080200);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000,
                             (((font->width * 2) & 0xFFF) << 12) | 0x07000000 | ((font->height * 4) & 0xFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++,
                             ((((((s32)(font->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5400000,
                             0x00080200);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000,
                             (((font->width * 4) & 0xFFF) << 12) | ((font->height * 4) & 0xFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)(dst - 16));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, (((x1 * 4) & 0xFFF) << 12) | 0xE4000000 | ((y1 * 4) & 0xFFF),
                             (((x0 * 4) & 0xFFF) << 12) | ((y0 * 4) & 0xFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000, (clipS << 21) | ((clipT << 5) & 0xFFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xB3000000, 0x04000400);
            }
        }
    }
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

// drawMenuSolidRect best match: 70.220%
#pragma GLOBAL_ASM("asm/nonmatchings/menu_renderer/drawMenuSolidRect.s")

#ifdef NON_MATCHING
void drawMenuSolidRect(s16 x0, s16 y0, s16 x1, s16 y1, s16 r, s16 g, s16 b) {
    Gfx *gfx;
    s32 color;
    extern u32 gMenuRenderModeResetDl[];

    if (x0 >= 0 && x0 < 321 && x1 >= 0 && x1 < 321 && y0 >= 0 && y0 < 241 && y1 >= 0 && y1 < 241) {
        gfx = gRegionAllocPtr++;
        gfx->words.w0 = 0xE7000000;
        gfx->words.w1 = 0;

        gfx = gRegionAllocPtr++;
        gfx->words.w0 = 0xBA001402;
        gfx->words.w1 = 0x00300000;

        gfx = gRegionAllocPtr++;
        gfx->words.w0 = 0xB900031D;
        gfx->words.w1 = 0;

        gfx = gRegionAllocPtr++;
        gfx->words.w0 = 0xF7000000;
        color = (((r << 8) & 0xF800) | ((g << 3) & 0x7C0) | ((b >> 2) & 0x3E)) | 1;
        gfx->words.w1 = (color << 16) | color;

        gfx = gRegionAllocPtr++;
        gfx->words.w0 = 0xF6000000 | ((x1 & 0x3FF) << 14) | ((y1 & 0x3FF) << 2);
        gfx->words.w1 = ((x0 & 0x3FF) << 14) | ((y0 & 0x3FF) << 2);

        gfx = gRegionAllocPtr++;
        gfx->words.w0 = 0x06000000;
        gfx->words.w1 = (u32)gMenuRenderModeResetDl;
    }
}
#endif

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

// drawMenuSpriteCrossfade best match: 82.448% (nonmatchings/func_80013FEC-7273315160691878794/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu_renderer/drawMenuSpriteCrossfade.s")

#ifdef NON_MATCHING
void drawMenuSpriteCrossfade(s16 x, s16 y, MenuFontAssetTable *table, u16 imageIndex0, u16 imageIndex1, u8 alpha) {
    MenuFontAssetEntry *entry0;
    MenuFontAssetEntry *entry1;
    s32 minX;
    s32 maxX;
    s32 minY;
    s32 maxY;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 drawX0;
    s32 drawY0;
    s32 drawX1;
    s32 drawY1;
    s32 clipS;
    s32 clipT;
    u8 *paletteBase;
    s32 halfWidth;
    s32 halfHeight;

    entry0 = &table->entries[imageIndex0];
    entry1 = &table->entries[imageIndex1];
    paletteBase = (u8 *)&table->entries[table->entryCount];

    x0 = x + gMenuViewportCenterX;
    y0 = y + gMenuViewportCenterY;
    x1 = x0 + entry0->width;
    y1 = y0 + entry0->height;

    clipS = 0;
    clipT = 0;
    halfWidth = gMenuViewportWidth / 2;
    maxX = gMenuViewportCenterX + halfWidth;
    if (x0 < maxX) {
        halfHeight = gMenuViewportHeight / 2;
        maxY = gMenuViewportCenterY + halfHeight;
        if (y0 < maxY) {
            minX = gMenuViewportCenterX - halfWidth;
            if (x1 >= minX) {
                minY = gMenuViewportCenterY - halfHeight;
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

                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xBA001402, 0x00100000);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xBA001001, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xFA000000, alpha);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xFC2527FF, 0x1FFC9238);

                    FONT_GFX_CMD(gRegionAllocPtr++, 0xFD080000 | ((entry0->width - 1) & 0xFFF),
                                 (u32)((u8 *)table + entry0->imageOffset));
                    FONT_GFX_CMD(gRegionAllocPtr++, ((((entry0->width + 8) >> 3) & 0x1FF) << 9) | 0xF5080000,
                                 0x07000000);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000,
                                 0x07000000 | (((entry0->width << 2) & 0xFFF) << 12) |
                                     ((entry0->height << 2) & 0xFFF));
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, ((((entry0->width + 8) >> 3) & 0x1FF) << 9) | 0xF5080000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000,
                                 (((entry0->width << 2) & 0xFFF) << 12) | ((entry0->height << 2) & 0xFFF));

                    FONT_GFX_CMD(gRegionAllocPtr++, 0xFD080000 | ((entry1->width - 1) & 0xFFF),
                                 (u32)((u8 *)table + entry1->imageOffset));
                    FONT_GFX_CMD(gRegionAllocPtr++, ((((entry1->width + 8) >> 3) & 0x1FF) << 9) | 0xF5080100,
                                 0x07000000);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000,
                                 0x07000000 | (((entry1->width << 2) & 0xFFF) << 12) |
                                     ((entry1->height << 2) & 0xFFF));
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, ((((entry1->width + 8) >> 3) & 0x1FF) << 9) | 0xF5080100,
                                 0x01000000);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000,
                                 0x01000000 | (((entry1->width << 2) & 0xFFF) << 12) |
                                     ((entry1->height << 2) & 0xFFF));

                    FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)(paletteBase + (entry1->textureIndex << 5)));
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x073FC000);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE4000000 | (((drawX1 << 2) & 0xFFF) << 12) |
                                                    ((drawY1 << 2) & 0xFFF),
                                 (((drawX0 << 2) & 0xFFF) << 12) | ((drawY0 << 2) & 0xFFF));
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000,
                                 ((clipS << 21) & 0xFFFF0000) | ((clipT << 5) & 0xFFFF));
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xB3000000, 0x04000400);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)gMenuRenderModeResetDl);
                }
            }
        }
    }
}
#endif
