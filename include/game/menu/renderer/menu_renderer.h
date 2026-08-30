#ifndef MENU_RENDERER_H
#define MENU_RENDERER_H

#include "common.h"
#include "game/engine/callback_task_scheduler.h"

#define MENU_PALETTE_COLOR_COUNT 0x10

typedef struct AssetTableEntry {
    /* 0x0 */ u32 imageOffset;
    /* 0x4 */ u16 paletteIndex;
    /* 0x6 */ u8 width;
    /* 0x7 */ u8 height;
} AssetTableEntry;

typedef struct AssetTable {
    /* 0x0 */ u32 unk0;
    /* 0x4 */ u32 entryCount;
    /* 0x8 */ AssetTableEntry entries[1];
} AssetTable;

typedef union MenuPalette {
    u8 bytes[MENU_PALETTE_COLOR_COUNT * sizeof(u16)];
    u16 colors[MENU_PALETTE_COLOR_COUNT];
} MenuPalette;

typedef struct MenuTilemapTile {
    /* 0x0 */ s16 imageIndex;
    /* 0x2 */ u8 paletteIndex;
    /* 0x3 */ u8 flip;
} MenuTilemapTile;

typedef enum MenuTilemapTexelSize {
    MENU_TILEMAP_TEXEL_4B,
    MENU_TILEMAP_TEXEL_8B
} MenuTilemapTexelSize;

typedef struct MenuRenderSprite {
    /* 0x00 */ s16 scrollX;
    /* 0x02 */ s16 scrollY;
    /* 0x04 */ s16 tileWidth;
    /* 0x06 */ s16 tileHeight;
    /* 0x08 */ s16 tilemapWidth;
    /* 0x0A */ s16 tilemapHeight;
    /* 0x0C */ s16 viewportX;
    /* 0x0E */ s16 viewportY;
    /* 0x10 */ s16 viewportWidth;
    /* 0x12 */ s16 viewportHeight;
    /* 0x14 */ u16 *images;
    /* 0x18 */ s16 *tilemap;
    /* 0x1C */ MenuTilemapTile *tiles;
    /* 0x20 */ u8 *paletteData;
} MenuRenderSprite;

typedef struct MenuTilemapSpriteAsset {
    /* 0x00 */ u16 tilemapWidth;
    /* 0x02 */ u16 tilemapHeight;
    /* 0x04 */ u16 tileWidth;
    /* 0x06 */ u16 tileHeight;
    /* 0x08 */ u16 tileCount;
    /* 0x0A */ u16 tilemapOffset;
    /* 0x0C */ u16 paletteOffset;
    /* 0x0E */ u16 imageOffset;
    /* 0x10 */ MenuTilemapTile tiles[1];
} MenuTilemapSpriteAsset;

typedef struct MenuTilemapSprite {
    /* 0x00 */ MenuRenderSprite render;
    /* 0x24 */ s16 unk24;
} MenuTilemapSprite;

typedef struct MenuRenderSpriteActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ MenuRenderSprite sprite;
} MenuRenderSpriteActor;

typedef u16 MenuGlyphScript;

void drawMenuAssetRegion(
    s16 x,
    s16 y,
    AssetTable *table,
    u16 tileIndex,
    u16 scaleX,
    u16 scaleY,
    u8 startS,
    u8 startT,
    u8 width,
    u8 height
);
void drawMenuSprite(s16 x, s16 y, AssetTable *table, u16 tileIndex, u16 width, u16 height, u8 palette, u8 flip);
void drawMenuSpriteClipped(
    s16 x,
    s16 y,
    AssetTable *table,
    u16 imageIndex,
    u16 scaleX,
    u16 scaleY,
    u8 flipMode,
    u8 paletteIndex,
    s16 clipLeft,
    s16 clipTop,
    s16 clipRight,
    s16 clipBottom
);
void drawMenuSpriteWithAlpha(
    s16 x,
    s16 y,
    AssetTable *table,
    u16 tileIndex,
    u16 width,
    u16 height,
    u8 palette,
    u16 alpha,
    u8 flip
);
void drawMenuSpriteWithAlphaWideArgs(
    s32 x,
    s32 y,
    AssetTable *table,
    s32 tileIndex,
    s32 width,
    s32 height,
    s32 palette,
    s32 alpha,
    u32 flip
);
void drawMenuSpriteWithAlphaClipped(
    s16 x,
    s16 y,
    AssetTable *asset,
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
);
void drawMenuSpriteWithPaletteScale(s16 x, s16 y, AssetTable *asset, u16 index, u16 intensity);
void drawMenuSpriteSubrect(
    s16 x,
    s16 y,
    AssetTable *asset,
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
    AssetTable *asset,
    u16 tileIndex,
    u16 scaleX,
    u16 scaleY,
    u8 flip,
    u8 unusedPalette
);
void drawMenuSpriteTile(s16 x, s16 y, AssetTable *table, u16 tileIndex, u16 width, u16 height);
void drawMenuSpriteTileClipped(
    s16 x,
    s16 y,
    AssetTable *table,
    u16 entryIndex,
    u16 unused,
    u16 intensity,
    s16 clipX,
    s16 clipY
);
void noopMenuRenderCallback(void);
void drawMenuTextureByAssetId(s16 x, s16 y, AssetTable *unusedTable, u16 assetId, u16 width, u16 height);
void setMenuSpriteActorDebugUpdate(MenuRenderSpriteActor *actor);
void updateMenuSpriteActorDebugControls(MenuRenderSpriteActor *actor);
void drawMenuTilemapSpriteCallback(MenuRenderSprite *sprite);
void drawMenuTilemapSprite(
    MenuRenderSprite *sprite,
    MenuTilemapTexelSize texelSize,
    s16 tilemapWidth,
    s16 tilemapHeight
);
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
void drawMenuGlyphScript(
    volatile s16 x,
    s16 y,
    MenuGlyphScript *script,
    s32 palette,
    u16 scale,
    volatile u16 colorMode
);
void drawMenuColoredGlyph(s16 x, s16 y, u16 glyph, u8 palette, u16 scale, u16 colorMode, s32 fontBank);
void drawMenuAsciiGlyph(s16 x, s16 y, u16 tileX, s32 tileY, u16 palette, u16 scale);
void drawMenuAsciiText(s16 x, s16 y, u8 *text, u16 palette, u16 scale);
void drawMenuSolidRect(s16 x0, s16 y0, s16 x1, s16 y1, s16 r, s16 g, s16 b);
s32 stepMenuFadeAlpha(s32 value, s16 step, u8 increase);
void drawMenuSpriteCrossfade(s16 x, s16 y, AssetTable *table, u16 imageIndex0, u16 imageIndex1, u8 alpha);

#endif
