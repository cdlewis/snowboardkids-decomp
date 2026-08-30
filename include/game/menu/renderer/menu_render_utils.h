#ifndef MENU_RENDER_UTILS_H
#define MENU_RENDER_UTILS_H

#include "common.h"
#include "game/menu/renderer/menu_renderer.h"

void initMenuAssetHandles(void);
void releaseMenuAssetHandles(void);
void *resolveAssetTableRelativePointer(void *asset, u32 relativeAddress);
void initMenuAsciiFontTexture(void);
void getAssetTableImageAndPalette(void *asset, u16 index, void **image, void **palette);
void getAssetTableImagePaletteAndSize(
    u8 *asset,
    u16 index,
    void **image,
    void **palette,
    s16 *width,
    s16 *height
);
void drawMenuAsciiChar(s16 x, s16 y, u8 character, u16 palette);
void drawMenuAsciiCharImpl(s16 x, s16 y, u8 character, u16 palette);
void drawMenuAsciiCharLegacy(s16 x, s16 y, volatile s32 character, u16 palette);
void drawMenuAsciiTextDefaultScale(s16 x, s16 y, const char *text, u16 palette);
void drawAssetTableSprite(s16 x, s16 y, AssetTable *table, u16 entryIndex);
void drawAssetTableSprite8bpp(s16 x, s16 y, AssetTable *table, u16 entryIndex);
void drawAssetTableSpriteWideIndex(s16 x, s16 y, AssetTable *table, s32 entryIndex);
void drawMenuFillRectangle(s16 x, s16 y, s16 width, s16 height, u8 red, u8 green, u8 blue);
void drawPulsingAssetTableSprite(s16 x, s16 y, AssetTable *table, u16 entryIndex);
void drawAssetTableSpriteWithDefaultPalette(s16 x, s16 y, AssetTable *table, u16 entryIndex);
void drawAssetTableSpriteWithExplicitPalette(s16 x, s16 y, AssetTable *table, u16 entryIndex, u16 paletteIndex);
void drawAssetTableSpriteWithExplicitPaletteWideIndex(
    s16 x,
    s16 y,
    AssetTable *table,
    s32 entryIndex,
    u16 paletteIndex
);
void drawScaledAssetTableSprite(s16 x, s16 y, AssetTable *table, u16 entryIndex, u16 scale);
void drawScaledAssetTableSpriteWithExplicitPalette(
    s16 x,
    s16 y,
    AssetTable *table,
    u16 entryIndex,
    u16 paletteIndex,
    u16 scale
);
void packFixedTransformMatrix(Transform3D *source, Mtx *dest);
Mtx *allocFixedTransformMatrix(Transform3D *source);
Mtx *allocFixedRotationMatrix(Mat3x3 rotation);
Mtx *allocTranslationOnlyFixedMatrix(Mtx *source);
void setPackedMatrixTranslation(Mtx *matrix, Vec3i *translation);
void copyPackedMatrixTranslation(Mtx *source, Mtx *dest);
s32 isPositionNearAnyRaceViewportFocus(Vec3i *position);
void allocRenderCallbackScratchBuffer(void);
void allocMenuRenderScratchBuffers(void);
void selectMenuRenderScratchBuffer(s32 bufferIndex);
void *allocMenuRenderScratch(s32 size);

#endif
