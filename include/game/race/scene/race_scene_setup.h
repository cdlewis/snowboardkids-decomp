#ifndef RACE_SCENE_SETUP_H
#define RACE_SCENE_SETUP_H

#include "common.h"

#define RACE_CHARACTER_ASSET_COUNT 6

typedef enum RomAssetRangeBound {
    ROM_ASSET_RANGE_START,
    ROM_ASSET_RANGE_END,
    ROM_ASSET_RANGE_BOUND_COUNT
} RomAssetRangeBound;

typedef struct CharacterRomAssetRangeTable {
    u8 *romBoundAddresses[RACE_CHARACTER_ASSET_COUNT * ROM_ASSET_RANGE_BOUND_COUNT];
} CharacterRomAssetRangeTable;

typedef char CharacterRomAssetRangeTableSizeCheck[(sizeof(CharacterRomAssetRangeTable) == 0x30) ? 1 : -1];

extern CharacterRomAssetRangeTable gCharacterRawAssetRanges;
extern CharacterRomAssetRangeTable gCharacterTextureAssetRanges;
extern CharacterRomAssetRangeTable gCharacterModelAssetRanges;

void loadRaceCourseAssets(void);
void loadRaceCharacterAssets(void);
void initRaceCourseSceneTasks(void);

#endif
