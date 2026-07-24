#ifndef MENU_RENDERER_H
#define MENU_RENDERER_H

#include "common.h"

typedef struct MenuRenderSprite MenuRenderSprite;
typedef struct MenuRenderSpriteActor MenuRenderSpriteActor;
typedef struct MenuFontAssetTable MenuFontAssetTable;
typedef u16 MenuGlyphScript;

#ifdef MENU_RENDERER_C
void drawMenuAssetRegion(s16 x, s16 y, s32 texture, u16 tileIndex, u16 scaleX, u16 scaleY, u8 startS, u8 startT,
                   u8 width, u8 height);
void drawMenuSprite(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, u8 palette, u8 flip);
void drawMenuSpriteClipped(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, u8 flip, u8 palette,
                   s32 clipLeft, s32 clipTop, s32 clipRight, s32 clipBottom);
void drawMenuSpriteWithAlpha(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, u8 palette, u16 alpha, u8 flip);
void drawMenuSpriteWithAlphaClipped(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, u8 palette, u16 alpha, u8 flip,
                   s32 clipLeft, s32 clipTop, s32 clipRight, s32 clipBottom);
void drawMenuSpriteWithPaletteScale(s16 x, s16 y, s32 texture, u16 tileIndex, s32 alpha);
void drawMenuSpriteSubrect(s16 x, s16 y, s32 texture, u16 tileIndex, u8 srcX, u8 srcY, u8 width, u8 height, s32 scaleX,
                   s32 scaleY);
void drawMenuSpriteFixedScale(s16 x, s16 y, s32 texture, u16 tileIndex, u16 scaleX, u16 scaleY, u8 flip, u8 unusedPalette);
void drawMenuSpriteTile(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height);
void drawMenuSpriteTileClipped(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, s32 clipRight, s32 clipBottom);
void noopMenuRenderCallback(void);
void drawMenuTextureByAssetId(s16 x, s16 y, s32 texture, u16 assetId, u16 width, u16 height);
void setMenuSpriteActorDebugUpdate(MenuRenderSpriteActor *actor);
void updateMenuSpriteActorDebugControls(MenuRenderSpriteActor *actor);
void drawMenuTilemapSpriteCallback(MenuRenderSprite *sprite);
s32 drawMenuTilemapSprite(MenuRenderSprite *sprite, s32 arg1, s16 x, s16 y);
void noopMenuSpriteCallback(void);
void drawMenuGlyphScriptWithFontBank(volatile s16 x, s16 y, MenuGlyphScript *script, s32 palette, u16 scale, u16 fontBank);
void drawMenuGlyphScriptDefaultFont(volatile s16 x, s16 y, MenuGlyphScript *script, s32 palette, u16 scale);
void drawMenuGlyph(s16 x, s16 y, u16 glyphIndex, u8 paletteIndex, u16 intensity, u16 fontBank);
void drawMenuColoredGlyphScript(volatile s16 x, s16 y, MenuGlyphScript *script, s32 palette, u16 scale, u16 colorMode, u16 fontBank);
void drawMenuGlyphScript(volatile s16 x, s16 y, MenuGlyphScript *script, s32 palette, u16 scale, volatile u16 colorMode);
void drawMenuColoredGlyph(s16 x, s16 y, u16 glyph, u8 palette, u16 scale, u16 colorMode, s32 fontBank);
void drawMenuAsciiGlyph(s16 x, s16 y, u16 tileX, s32 tileY, u16 palette, u16 scale);
void drawMenuAsciiText(s16 x, s16 y, u8 *text, u16 palette, u16 scale);
void drawMenuSolidRect(s16 x0, s16 y0, s16 x1, s16 y1, s16 r, s16 g, s16 b);
s32 stepMenuFadeAlpha(s32 value, s16 step, u8 increase);
void drawMenuSpriteCrossfade(s16 x, s16 y, MenuFontAssetTable *table, u16 imageIndex0, u16 imageIndex1, u8 alpha);
#elif defined(MENU_RENDERER_BROAD_PROTOTYPES)
void drawMenuAssetRegion(s16 x, s16 y, s32 texture, s32 tileIndex, s32 scaleX, s32 scaleY, s32 startS, s32 startT,
                   s32 width, s32 height);
#ifdef MENU_RENDERER_U32_TILE_INDEX_PROTOTYPE
void drawMenuSprite(s16 x, s16 y, s32 texture, u32 tileIndex, s32 width, s32 height, s32 palette, s32 flip);
#else
void drawMenuSprite(s16 x, s16 y, s32 texture, s32 tileIndex, s32 width, s32 height, s32 palette, s32 flip);
#endif
void drawMenuSpriteClipped(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, u8 flip, u8 palette,
                   s32 clipLeft, s32 clipTop, s32 clipRight, s32 clipBottom);
#ifdef MENU_RENDERER_F8AC_U16_ALPHA_PROTOTYPE
void drawMenuSpriteWithAlpha(s32 x, s32 y, s32 texture, s32 tileIndex, s32 width, s32 height, s32 palette, u16 alpha, s32 flip);
#else
void drawMenuSpriteWithAlpha(s32 x, s32 y, s32 texture, s32 tileIndex, s32 width, s32 height, s32 palette, s32 alpha, s32 flip);
#endif
void drawMenuSpriteWithAlphaClipped(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, u8 palette, u16 alpha, u8 flip,
                   s32 clipLeft, s32 clipTop, s32 clipRight, s32 clipBottom);
void drawMenuSpriteWithPaletteScale(s16 x, s16 y, s32 texture, s32 tileIndex, s32 alpha);
void drawMenuSpriteSubrect(s16 x, s16 y, s32 texture, u16 tileIndex, s32 srcX, s32 srcY, s32 width, s32 height, s32 scaleX,
                   s32 scaleY);
void drawMenuSpriteFixedScale(s16 x, s16 y, s32 texture, s32 tileIndex, s32 scaleX, s32 scaleY, s32 flip, s32 unusedPalette);
void drawMenuSpriteTile(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height);
void drawMenuSpriteTileClipped(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, s32 clipRight, s32 clipBottom);
void noopMenuRenderCallback(void);
void drawMenuTextureByAssetId(s16 x, s16 y, s32 texture, u16 assetId, u16 width, u16 height);
void setMenuSpriteActorDebugUpdate(MenuRenderSpriteActor *actor);
void updateMenuSpriteActorDebugControls(MenuRenderSpriteActor *actor);
void drawMenuTilemapSpriteCallback(MenuRenderSprite *sprite);
s32 drawMenuTilemapSprite(void *sprite, s32 arg1, s16 x, s16 y);
void noopMenuSpriteCallback(void);
void drawMenuGlyphScriptWithFontBank(s16 x, s16 y, MenuGlyphScript *script, s32 palette, s32 scale, s32 fontBank);
void drawMenuGlyphScriptDefaultFont(s16 x, s16 y, void *script, s32 palette, s32 scale);
void drawMenuGlyph(s16 x, s16 y, u16 glyphIndex, u8 paletteIndex, u16 intensity, u16 fontBank);
void drawMenuColoredGlyphScript(s32 x, s32 y, void *script, s32 palette, s32 scale, s32 colorMode, s32 fontBank);
void drawMenuGlyphScript(s32 x, s32 y, u8 *text, s32 palette, s32 scale, s32 colorMode);
void drawMenuColoredGlyph(s16 x, s16 y, u16 glyph, u8 palette, u16 scale, u16 colorMode, s32 fontBank);
void drawMenuAsciiGlyph(s16 x, s16 y, u16 tileX, s32 tileY, u16 palette, u16 scale);
void drawMenuAsciiText(s16 x, s16 y, u8 *text, s32 palette, s32 scale);
void drawMenuSolidRect(s16 x0, s16 y0, s16 x1, s16 y1, s16 r, s16 g, s16 b);
#ifdef MENU_RENDERER_S16_STEP_PROTOTYPE
s32 stepMenuFadeAlpha(s16 value, s32 step, s32 increase);
#else
s32 stepMenuFadeAlpha(s32 value, s32 step, s32 increase);
#endif
void drawMenuSpriteCrossfade(s16 x, s16 y, MenuFontAssetTable *table, s32 imageIndex0, s32 imageIndex1, s32 alpha);
#else
void drawMenuAssetRegion(s16 x, s16 y, s32 texture, s32 tileIndex, s32 scaleX, s32 scaleY, s32 startS, s32 startT,
                   s32 width, s32 height);
void drawMenuSprite(s16 x, s16 y, s32 texture, s32 tileIndex, s32 width, s32 height, s32 palette, s32 flip);
void drawMenuSpriteClipped(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, u8 flip, u8 palette,
                   s32 clipLeft, s32 clipTop, s32 clipRight, s32 clipBottom);
void drawMenuSpriteWithAlpha(s32 x, s32 y, s32 texture, s32 tileIndex, s32 width, s32 height, s32 palette, s32 alpha, s32 flip);
void drawMenuSpriteWithAlphaClipped(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, u8 palette, u16 alpha, u8 flip,
                   s32 clipLeft, s32 clipTop, s32 clipRight, s32 clipBottom);
void drawMenuSpriteWithPaletteScale(s16 x, s16 y, s32 texture, u16 tileIndex, s32 alpha);
void drawMenuSpriteSubrect(s16 x, s16 y, s32 texture, u16 tileIndex, u8 srcX, u8 srcY, u8 width, u8 height, u16 scaleX,
                   u16 scaleY);
void drawMenuSpriteFixedScale(s16 x, s16 y, s32 texture, u16 tileIndex, u16 scaleX, u16 scaleY, u8 flip, u8 unusedPalette);
void drawMenuSpriteTile(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height);
void drawMenuSpriteTileClipped(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, s32 clipRight, s32 clipBottom);
void noopMenuRenderCallback(void);
void drawMenuTextureByAssetId(s16 x, s16 y, s32 texture, u16 assetId, u16 width, u16 height);
void setMenuSpriteActorDebugUpdate(MenuRenderSpriteActor *actor);
void updateMenuSpriteActorDebugControls(MenuRenderSpriteActor *actor);
void drawMenuTilemapSpriteCallback(MenuRenderSprite *sprite);
s32 drawMenuTilemapSprite(MenuRenderSprite *sprite, s32 arg1, s16 x, s16 y);
void noopMenuSpriteCallback(void);
void drawMenuGlyphScriptWithFontBank(s16 x, s16 y, MenuGlyphScript *script, s32 palette, u16 scale, u16 fontBank);
void drawMenuGlyphScriptDefaultFont(s16 x, s16 y, MenuGlyphScript *script, s32 palette, u16 scale);
void drawMenuGlyph(s16 x, s16 y, u16 glyphIndex, u8 paletteIndex, u16 intensity, u16 fontBank);
void drawMenuColoredGlyphScript(s16 x, s16 y, MenuGlyphScript *script, s32 palette, u16 scale, u16 colorMode, u16 fontBank);
void drawMenuGlyphScript(s32 x, s32 y, u8 *text, s32 palette, s32 scale, s32 colorMode);
void drawMenuColoredGlyph(s16 x, s16 y, u16 glyph, u8 palette, u16 scale, u16 colorMode, s32 fontBank);
void drawMenuAsciiGlyph(s16 x, s16 y, u16 tileX, s32 tileY, u16 palette, u16 scale);
void drawMenuAsciiText(s16 x, s16 y, u8 *text, u16 palette, u16 scale);
void drawMenuSolidRect(s16 x0, s16 y0, s16 x1, s16 y1, s16 r, s16 g, s16 b);
#ifdef MENU_RENDERER_S16_STEP_PROTOTYPE
s32 stepMenuFadeAlpha(s16 value, s32 step, s32 increase);
#else
s32 stepMenuFadeAlpha(s32 value, s32 step, s32 increase);
#endif
void drawMenuSpriteCrossfade(s16 x, s16 y, MenuFontAssetTable *table, u16 imageIndex0, u16 imageIndex1, u8 alpha);
#endif

#endif
