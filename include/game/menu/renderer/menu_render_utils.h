#ifndef MENU_RENDER_UTILS_H
#define MENU_RENDER_UTILS_H

#include "common.h"

typedef struct AssetTable AssetTable;

void getAssetTableImageAndPalette(void *asset, u16 index, void **image, void **palette);
void drawMenuAsciiChar(s16 x, s16 y, u8 character, u16 palette);
void drawAssetTableSprite(s16 x, s16 y, AssetTable *table, u16 entryIndex);
void drawPulsingAssetTableSprite(s16 x, s16 y, AssetTable *table, u16 entryIndex);
void drawAssetTableSpriteWithDefaultPalette(s16 x, s16 y, AssetTable *table, u16 entryIndex);
void drawAssetTableSpriteWithExplicitPalette(s16 x, s16 y, AssetTable *table, u16 entryIndex, u16 paletteIndex);
void drawScaledAssetTableSprite(s16 x, s16 y, AssetTable *table, u16 entryIndex, u16 scale);
void drawScaledAssetTableSpriteWithExplicitPalette(s16 x, s16 y, AssetTable *table, u16 entryIndex,
                                                   u16 paletteIndex, u16 scale);

#endif
