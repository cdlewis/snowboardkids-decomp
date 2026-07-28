#ifndef RACE_SCENE_SETUP_H
#define RACE_SCENE_SETUP_H

#include "common.h"

typedef struct RomAssetRange {
    /* 0x0 */ u8 *start;
    /* 0x4 */ u8 *end;
} RomAssetRange;

extern RomAssetRange gCharacterRawAssetRanges[];
extern RomAssetRange gCharacterTextureAssetRanges[];
extern RomAssetRange gCharacterModelAssetRanges[];

void loadRaceCourseAssets(void);
void loadRaceCharacterAssets(void);
void initRaceCourseSceneTasks(void);

#endif
