#ifndef MENU_RENDERER_H
#define MENU_RENDERER_H

#include "common.h"

typedef struct MenuRenderSprite {
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
} MenuRenderSprite;
typedef struct MenuRenderSpriteActor MenuRenderSpriteActor;
typedef struct MenuFontAssetTable MenuFontAssetTable;
typedef struct FontAsset FontAsset;
typedef u16 MenuGlyphScript;

void drawMenuAssetRegion(
    s16 x,
    s16 y,
    void *texture,
    u16 tileIndex,
    u16 scaleX,
    u16 scaleY,
    u8 startS,
    u8 startT,
    u8 width,
    u8 height
);
void drawMenuSprite(s16 x, s16 y, void *texture, u16 tileIndex, u16 width, u16 height, u8 palette, u8 flip);
void drawMenuSpriteClipped(
    s16 x,
    s16 y,
    MenuFontAssetTable *table,
    volatile u16 imageIndex,
    u16 scaleX,
    u16 scaleY,
    u8 flipMode,
    u8 paletteIndex,
    s32 clipLeft,
    s32 clipTop,
    s32 clipRight,
    s32 clipBottom
);
void drawMenuSpriteWithAlpha(
    s16 x,
    s16 y,
    void *texture,
    u16 tileIndex,
    u16 width,
    u16 height,
    u8 palette,
    u16 alpha,
    u8 flip
);
void drawMenuSpriteWithAlphaClipped(
    s16 x,
    s16 y,
    void *texture,
    u16 tileIndex,
    u16 width,
    u16 height,
    u8 palette,
    u16 alpha,
    u8 flip,
    s32 clipLeft,
    s32 clipTop,
    s32 clipRight,
    s32 clipBottom
);
void drawMenuSpriteWithPaletteScale(s16 x, s16 y, FontAsset *asset, u16 index, u16 intensity);
void drawMenuSpriteSubrect(
    s16 x,
    s16 y,
    void *texture,
    u16 tileIndex,
    u8 srcX,
    u8 srcY,
    u8 width,
    u8 height,
    s32 scaleX,
    s32 scaleY
);
void drawMenuSpriteFixedScale(
    s16 x,
    s16 y,
    FontAsset *asset,
    u16 tileIndex,
    u16 scaleX,
    u16 scaleY,
    u8 flip,
    u8 unusedPalette
);
void drawMenuSpriteTile(s16 x, s16 y, void *texture, u16 tileIndex, u16 width, u16 height);
void drawMenuSpriteTileClipped(
    s16 x,
    s16 y,
    MenuFontAssetTable *table,
    u16 entryIndex,
    u16 unused,
    u16 intensity,
    s16 clipX,
    s16 clipY
);
void noopMenuRenderCallback(void);
void drawMenuTextureByAssetId(s16 x, s16 y, void *texture, u16 assetId, u16 width, u16 height);
void setMenuSpriteActorDebugUpdate(MenuRenderSpriteActor *actor);
void updateMenuSpriteActorDebugControls(MenuRenderSpriteActor *actor);
void drawMenuTilemapSpriteCallback(MenuRenderSprite *sprite);
s32 drawMenuTilemapSprite(MenuRenderSprite *sprite, s32 arg1, s16 x, s16 y);
void noopMenuSpriteCallback(void);
void drawMenuGlyphScriptWithFontBank(
    volatile s16 x,
    s16 y,
    MenuGlyphScript *script,
    s32 palette,
    u16 scale,
    u16 fontBank
);
void drawMenuGlyphScriptDefaultFont(volatile s16 x, s16 y, MenuGlyphScript *script, s32 palette, u16 scale);
void drawMenuGlyph(s16 x, s16 y, u16 glyphIndex, u8 paletteIndex, u16 intensity, u16 fontBank);
void drawMenuColoredGlyphScript(
    volatile s16 x,
    s16 y,
    MenuGlyphScript *script,
    s32 palette,
    u16 scale,
    u16 colorMode,
    u16 fontBank
);
#ifndef MENU_RENDERER_OMIT_DRAW_MENU_GLYPH_SCRIPT
void drawMenuGlyphScript(
    volatile s16 x,
    s16 y,
    MenuGlyphScript *script,
    s32 palette,
    u16 scale,
    volatile u16 colorMode
);
#endif
void drawMenuColoredGlyph(s16 x, s16 y, u16 glyph, u8 palette, u16 scale, u16 colorMode, s32 fontBank);
void drawMenuAsciiGlyph(s16 x, s16 y, u16 tileX, s32 tileY, u16 palette, u16 scale);
void drawMenuAsciiText(s16 x, s16 y, u8 *text, u16 palette, u16 scale);
void drawMenuSolidRect(s16 x0, s16 y0, s16 x1, s16 y1, s16 r, s16 g, s16 b);
s32 stepMenuFadeAlpha(s32 value, s16 step, u8 increase);
void drawMenuSpriteCrossfade(s16 x, s16 y, MenuFontAssetTable *table, u16 imageIndex0, u16 imageIndex1, u8 alpha);

#endif
