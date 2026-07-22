#include "common.h"
#include "assets.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/asset_manager.h"
#include "game/race/scene/race_scene_setup.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/system_runtime.h"
#include "game/race/motion/race_motion.h"
#include "game/race/course/race_course_effects.h"
#include "game/race/effects/race_intro_effects.h"
#include "game/race/items/race_item_effects.h"
#include "game/race/course/race_course_props_and_pickups.h"
#include "game/race/ui/race_ui_effects.h"
#include "game/race/player/race_player_input.h"

typedef struct {
    /* 0x0 */ u8 *start;
    /* 0x4 */ u8 *end;
} RomAssetRange;

extern s16 gAssetHandles[];
extern u16 gRaceCourseIndex;
extern RomAssetRange gCharacterRawAssetRanges[];
extern RomAssetRange gCharacterTextureAssetRanges[];
extern RomAssetRange gCharacterModelAssetRanges[];

extern u8 D_147910[];
extern u8 D_14B450[];
extern u8 D_1502A0[];
extern u8 D_1D82B0[];
extern u8 D_1DCED0[];
extern u8 D_1E74E0[];
extern u8 D_1EC0F0[];
extern u8 D_1EF530[];
extern u8 D_1F1A90[];
extern u8 D_2427D0[];
extern u8 D_243270[];
extern u8 D_245A80[];
extern u8 D_5DAF30[];
extern u8 D_5DB9D0[];

extern u8 gRaceSplitscreenMode;
extern u8 gPlayerCount;
extern u8 gRaceDemoPlaybackEnabled;
extern u8 gTrainingCourseLesson;
extern u8 gMainMenuModeSelection;
extern u8 gRaceTypeSelection;
extern u8 gRaceCourseModelEffectsDisabled;
extern u8 gRaceCourseOverlayEffectsDisabled;

void loadRaceCourseAssets(void) {
    s32 size;

    switch (gRaceCourseIndex) {
    case 0:
        size = ASSET_SIZE(LEVEL_BIG_SNOWMAN);
        gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
        lockRelocatableHeapBlock(gAssetHandles[0x8]);
        dmaReadRom((u32)&LEVEL_BIG_SNOWMAN_ROM_START, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
        loadCompressedRomAsset(ASSET_START(LEVEL_BIG_SNOWMAN_TEXTURES), ASSET_END(LEVEL_BIG_SNOWMAN_TEXTURES), 9);
        loadCompressedRomAsset(ASSET_START(LEVEL_BIG_SNOWMAN_SURFACE_DATA), ASSET_END(LEVEL_BIG_SNOWMAN_SURFACE_DATA), 0x1B);
        initRaceCourseSurfaceData();
        loadCompressedRomAsset(ASSET_START(LEVEL_BIG_SNOWMAN_SPRITES), ASSET_END(LEVEL_BIG_SNOWMAN_SPRITES), 0x1D);
        break;
    case 1:
        size = ASSET_SIZE(LEVEL_SUNSET_ROCK);
        gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
        lockRelocatableHeapBlock(gAssetHandles[0x8]);
        dmaReadRom((u32)&LEVEL_SUNSET_ROCK_ROM_START, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
        loadCompressedRomAsset(ASSET_START(LEVEL_SUNSET_ROCK_TEXTURES), ASSET_END(LEVEL_SUNSET_ROCK_TEXTURES), 9);
        loadCompressedRomAsset(ASSET_START(LEVEL_SUNSET_ROCK_SURFACE_DATA), ASSET_END(LEVEL_SUNSET_ROCK_SURFACE_DATA), 0x1B);
        initRaceCourseSurfaceData();
        loadCompressedRomAsset(ASSET_START(LEVEL_SUNSET_ROCK_SPRITES), ASSET_END(LEVEL_SUNSET_ROCK_SPRITES), 0x1D);
        break;
    case 2:
        size = ASSET_SIZE(LEVEL_NIGHT_HIGHWAY);
        gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
        lockRelocatableHeapBlock(gAssetHandles[0x8]);
        dmaReadRom((u32)&LEVEL_NIGHT_HIGHWAY_ROM_START, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
        loadCompressedRomAsset(ASSET_START(LEVEL_NIGHT_HIGHWAY_TEXTURES), ASSET_END(LEVEL_NIGHT_HIGHWAY_TEXTURES), 9);
        loadCompressedRomAsset(ASSET_START(LEVEL_NIGHT_HIGHWAY_SURFACE_DATA), ASSET_END(LEVEL_NIGHT_HIGHWAY_SURFACE_DATA), 0x1B);
        initRaceCourseSurfaceData();
        loadCompressedRomAsset(ASSET_START(LEVEL_NIGHT_HIGHWAY_SPRITES), ASSET_END(LEVEL_NIGHT_HIGHWAY_SPRITES), 0x1D);
        break;
    case 3:
        size = ASSET_SIZE(LEVEL_GRASS_VALLEY);
        gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
        lockRelocatableHeapBlock(gAssetHandles[0x8]);
        dmaReadRom((u32)&LEVEL_GRASS_VALLEY_ROM_START, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
        loadCompressedRomAsset(ASSET_START(LEVEL_GRASS_VALLEY_TEXTURES), ASSET_END(LEVEL_GRASS_VALLEY_TEXTURES), 9);
        loadCompressedRomAsset(ASSET_START(LEVEL_GRASS_VALLEY_SURFACE_DATA), ASSET_END(LEVEL_GRASS_VALLEY_SURFACE_DATA), 0x1B);
        initRaceCourseSurfaceData();
        loadCompressedRomAsset(ASSET_START(LEVEL_GRASS_VALLEY_SPRITES), ASSET_END(LEVEL_GRASS_VALLEY_SPRITES), 0x1D);
        break;
    case 4:
        size = ASSET_SIZE(LEVEL_DIZZY_LAND);
        gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
        lockRelocatableHeapBlock(gAssetHandles[0x8]);
        dmaReadRom((u32)&LEVEL_DIZZY_LAND_ROM_START, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
        loadCompressedRomAsset(ASSET_START(LEVEL_DIZZY_LAND_TEXTURES), ASSET_END(LEVEL_DIZZY_LAND_TEXTURES), 9);
        loadCompressedRomAsset(ASSET_START(LEVEL_DIZZY_LAND_SURFACE_DATA), ASSET_END(LEVEL_DIZZY_LAND_SURFACE_DATA), 0x1B);
        initRaceCourseSurfaceData();
        loadCompressedRomAsset(ASSET_START(LEVEL_DIZZY_LAND_SPRITES), ASSET_END(LEVEL_DIZZY_LAND_SPRITES), 0x1D);
        break;
    case 5:
        size = ASSET_SIZE(LEVEL_QUICKSAND_VALLEY);
        gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
        lockRelocatableHeapBlock(gAssetHandles[0x8]);
        dmaReadRom((u32)&LEVEL_QUICKSAND_VALLEY_ROM_START, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
        loadCompressedRomAsset(ASSET_START(LEVEL_QUICKSAND_VALLEY_TEXTURES), ASSET_END(LEVEL_QUICKSAND_VALLEY_TEXTURES), 9);
        loadCompressedRomAsset(ASSET_START(LEVEL_QUICKSAND_VALLEY_SURFACE_DATA), ASSET_END(LEVEL_QUICKSAND_VALLEY_SURFACE_DATA), 0x1B);
        initRaceCourseSurfaceData();
        loadCompressedRomAsset(ASSET_START(LEVEL_QUICKSAND_VALLEY_SPRITES), ASSET_END(LEVEL_QUICKSAND_VALLEY_SPRITES), 0x1D);
        break;
    case 6:
        size = ASSET_SIZE(LEVEL_SILVER_MOUNTAIN);
        gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
        lockRelocatableHeapBlock(gAssetHandles[0x8]);
        dmaReadRom((u32)&LEVEL_SILVER_MOUNTAIN_ROM_START, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
        loadCompressedRomAsset(ASSET_START(LEVEL_SILVER_MOUNTAIN_TEXTURES), ASSET_END(LEVEL_SILVER_MOUNTAIN_TEXTURES), 9);
        loadCompressedRomAsset(ASSET_START(LEVEL_SILVER_MOUNTAIN_SURFACE_DATA), ASSET_END(LEVEL_SILVER_MOUNTAIN_SURFACE_DATA), 0x1B);
        initRaceCourseSurfaceData();
        loadCompressedRomAsset(ASSET_START(LEVEL_SILVER_MOUNTAIN_SPRITES), ASSET_END(LEVEL_SILVER_MOUNTAIN_SPRITES), 0x1D);
        break;
    case 7:
        size = ASSET_SIZE(LEVEL_ANIMAL_LAND);
        gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
        lockRelocatableHeapBlock(gAssetHandles[0x8]);
        dmaReadRom((u32)&LEVEL_ANIMAL_LAND_ROM_START, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
        loadCompressedRomAsset(ASSET_START(LEVEL_ANIMAL_LAND_TEXTURES), ASSET_END(LEVEL_ANIMAL_LAND_TEXTURES), 9);
        loadCompressedRomAsset(ASSET_START(LEVEL_ANIMAL_LAND_SURFACE_DATA), ASSET_END(LEVEL_ANIMAL_LAND_SURFACE_DATA), 0x1B);
        initRaceCourseSurfaceData();
        break;
    case 8:
        size = ASSET_SIZE(LEVEL_NINJA_LAND);
        gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
        lockRelocatableHeapBlock(gAssetHandles[0x8]);
        dmaReadRom((u32)&LEVEL_NINJA_LAND_ROM_START, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
        loadCompressedRomAsset(ASSET_START(LEVEL_NINJA_LAND_TEXTURES), ASSET_END(LEVEL_NINJA_LAND_TEXTURES), 9);
        loadCompressedRomAsset(ASSET_START(LEVEL_NINJA_LAND_SURFACE_DATA), ASSET_END(LEVEL_NINJA_LAND_SURFACE_DATA), 0x1B);
        initRaceCourseSurfaceData();
        loadCompressedRomAsset(ASSET_START(LEVEL_NINJA_LAND_SPRITES), ASSET_END(LEVEL_NINJA_LAND_SPRITES), 0x1D);
        break;
    case 9:
        size = ASSET_SIZE(LEVEL_ROOKIE_MOUNTAIN);
        gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
        lockRelocatableHeapBlock(gAssetHandles[0x8]);
        dmaReadRom((u32)&LEVEL_ROOKIE_MOUNTAIN_ROM_START, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
        loadCompressedRomAsset(ASSET_START(LEVEL_ROOKIE_MOUNTAIN_TEXTURES), ASSET_END(LEVEL_ROOKIE_MOUNTAIN_TEXTURES), 9);
        loadCompressedRomAsset(ASSET_START(LEVEL_ROOKIE_MOUNTAIN_SURFACE_DATA), ASSET_END(LEVEL_ROOKIE_MOUNTAIN_SURFACE_DATA), 0x1B);
        initRaceCourseSurfaceData();
        loadCompressedRomAsset(ASSET_START(LEVEL_ROOKIE_MOUNTAIN_SPRITES), ASSET_END(LEVEL_ROOKIE_MOUNTAIN_SPRITES), 0x1D);
        break;
    }

    size = D_14B450 - D_147910;
    gAssetHandles[10] = allocRelocatableHeapBlock(size);
    lockRelocatableHeapBlock(gAssetHandles[10]);
    dmaReadRom(D_147910, getRelocatableHeapBlockBase(gAssetHandles[0xA]), size);
    loadCompressedRomAsset(D_1D82B0, D_1DCED0, 0xB);
    loadCompressedRomAsset(D_1E74E0, D_1EC0F0, 0x1C);
    loadCompressedRomAsset(D_5DAF30, D_5DB9D0, 0x2A);
}

void loadRaceCharacterAssets(void) {
    RacePlayer *player;
    RomAssetRange *rawRanges;
    RomAssetRange *textureRanges;
    RomAssetRange *modelRanges;
    s16 *assetHandles;
    s32 i;
    s32 heapBase;
    s32 size;

    size = D_1502A0 - D_14B450;
    gAssetHandles[0xC] = allocRelocatableHeapBlock(size);
    dmaReadRom(D_14B450, getRelocatableHeapBlockBase(gAssetHandles[0xC]), size);
    loadCompressedRomAsset(D_1EF530, D_1F1A90, 0xD);

    rawRanges = gCharacterRawAssetRanges;
    textureRanges = gCharacterTextureAssetRanges;
    modelRanges = gCharacterModelAssetRanges;
    for (player = gRacePlayers, i = 0; i != RACE_PLAYER_COUNT; i++, player++) {
        assetHandles = &gAssetHandles[i];
        if (player->isActive != 0) {
            size = rawRanges[player->characterId & 0xFFFF].end - rawRanges[player->characterId & 0xFFFF].start;
            assetHandles[0xE] = allocRelocatableHeapBlock(size);
            heapBase = getRelocatableHeapBlockBase(assetHandles[0xE]);
            dmaReadRom(rawRanges[player->characterId & 0xFFFF].start, heapBase, size);
            loadCompressedRomAsset(textureRanges[player->characterId & 0xFFFF].start, textureRanges[player->characterId & 0xFFFF].end, i + 0x12);
            loadCompressedRomAsset(modelRanges[player->characterId & 0xFFFF].start, modelRanges[player->characterId & 0xFFFF].end, i + 0x16);
        }
    }

    loadCompressedRomAsset(D_243270, D_245A80, 0x1E);
}

void initRaceCourseSceneTasks(void) {
    s32 sp2C;
    s32 sp28;

    sp28 = (sp2C = 1);
    if (gPlayerCount != 1) {
        if (gRaceCourseModelEffectsDisabled != 0) {
            sp28 = 0;
        }
        if (gRaceCourseOverlayEffectsDisabled != 0) {
            sp2C = 0;
        }
    }
    switch ((u16) gRaceCourseIndex) {
    case 0:
        createCallbackTask(&initRaceCourseModelRenderTask, 0, 0x64);
        createCallbackTask(&initRaceCourseSceneryObjects, 0, 0x64);
        createCallbackTask(&initCourseGateObject, 0, 0x64);
        createCallbackTaskWithUserId(&initRaceCourseRankModel, 0, 0x64, 0);
        createCallbackTaskWithUserId(&initRaceCourseRankModel, 0, 0x64, 1);
        createCallbackTaskWithUserId(&initCourseBillboardMarker, 0, 0x64, 0);
        if ((gRaceSplitscreenMode == 0) && (gRaceDemoPlaybackEnabled == 0) && (gTrainingCourseLesson == 0) && (gMainMenuModeSelection == 0)) {
            if (sp2C != 0) {
                createCallbackTask(&initCourseCollectibleSprites, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 1);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 2);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 3);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 4);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 5);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x3F);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x4E);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x4F);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x50);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x51);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x52);
            }
        }
        if (gRaceSplitscreenMode == 1) {
            if (gRaceTypeSelection == 0) {
                createCallbackTask(&initRaceCourseCoinMarkers, 0, 0x64);
            }
            if (gRaceTypeSelection == 1) {
                createCallbackTask(&initRaceScoreAttackRings, 0, 1);
            }
        }
        if (gRaceSplitscreenMode == 1) {
            if (gRaceTypeSelection != 1) {
                goto block_24;
            }
        } else {
block_24:
            createCallbackTask(&initCourseTextureMarkers, 0, 0x64);
        }
        setBootFadeColor(0x80U, 0xC0U, 0xFFU);
        break;
    case 1:
        createCallbackTask(&initRaceCourseModelRenderTask, 0, 0x64);
        createCallbackTask(&initCourseTextureMarkers, 0, 0x64);
        spawnPatrolCourseObject(0x17, 0xF682AF46, 0xEB74F202, 0xF82BAF46, 0xEA17F202);
        spawnPatrolCourseObject(0x17, 0xF7062F46, 0xE897F202, 0xF5C82F46, 0xEA58F202);
        spawnPatrolCourseObject(0x17, 0xF45B2F46, 0xE876F202, 0xF6502F46, 0xE7927202);
        spawnPatrolCourseObject(0x17, 0xF53F2F46, 0xE5987202, 0xF2E4AF46, 0xE61F7202);
        spawnPatrolCourseObject(0x17, 0xF2BDAF46, 0xE548F202, 0xF3AE2F46, 0xE3AF7202);
        spawnPatrolCourseObject(0x17, 0xF25E41BF, 0xE5FB38EE, 0xF3169B57, 0xE56EEC94);
        spawnPatrolCourseObject(0x17, 0xF50D1255, 0xE7032BEC, 0xF2E89861, 0xE70079E8);
        spawnPatrolCourseObject(0x17, 0xF3EFF851, 0xE8BCCFE0, 0xF5650CCB, 0xE9A4FC34);
        spawnPatrolCourseObject(0x17, 0xF6BEB9D1, 0xEA69C4DE, 0xF6F1B095, 0xEBF8B05E);
        createCallbackTask(&initRaceCourseSceneryObjects, 0, 0x64);
        createCallbackTaskWithUserId(&initRaceCourseSlideSprite, 0, 0x64, 6);
        createCallbackTaskWithUserId(&initRaceCourseSlideSprite, 0, 0x64, 7);
        createCallbackTaskWithUserId(&initCourseTriggerVolume, 0, 0x64, 0);
        createCallbackTaskWithUserId(&initRaceCoursePropModels, 0, 0x64, 0);
        createCallbackTask(&initCourseGateObject, 0, 0x64);
        createCallbackTaskWithUserId(&updateThrownPickupSpawner, 0, 0x64, 0);
        createCallbackTaskWithUserId(&updateThrownPickupSpawner, 0, 0x64, 1);
        createCallbackTaskWithUserId(&initRaceCourseRankModel, 0, 0x64, 1);
        if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
            if (sp2C != 0) {
                createCallbackTask(&initCourseCollectibleSprites, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 6);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 7);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 8);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 9);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0xA);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0xB);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0xC);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0xD);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x53);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x54);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x55);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x56);
            }
            func_80061AF4(0x59, (void *)0x2290CDA1, (void *)0xC5F82815, 0xAF0);
        }
        if (gRaceSplitscreenMode == 1) {
            if (gRaceTypeSelection == 0) {
                createCallbackTask(&initRaceCourseCoinMarkers, 0, 0x64);
            }
            if (gRaceTypeSelection == 1) {
                createCallbackTask(&initRaceScoreAttackRings, 0, 1);
            }
        }
        setBootFadeColor(0xFFU, 0x80U, 0U);
        break;
    case 2:
        createCallbackTask(&initRaceCourseModelRenderTask, 0, 0x64);
        createCallbackTask(&initRaceCourseSceneryObjects, 0, 0x64);
        createCallbackTask(&initCourseGateObject, 0, 0x64);
        createCallbackTaskWithUserId(&initRaceCourseSlideSprite, 0, 0x64, 2);
        createCallbackTaskWithUserId(&initCourseStartFinishSprite, 0, 0x64, 0);
        createCallbackTaskWithUserId(&initCourseStartFinishSprite, 0, 0x64, 1);
        if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
            if (sp2C != 0) {
                createCallbackTask(&initCourseCollectibleSprites, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0xE);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0xF);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x10);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x11);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x12);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x13);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x14);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x42);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x57);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x58);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x59);
            }
        }
        setBootFadeColor(0U, 0U, 0x32U);
        break;
    case 3:
        createCallbackTask(&initRaceCourseModelRenderTask, 0, 0x64);
        createCallbackTask(&initRaceCourseSceneryObjects, 0, 0x64);
        createCallbackTask(&initCourseGateObject, 0, 0x64);
        createCallbackTaskWithUserId(&initCourseBillboardMarker, 0, 0x64, 1);
        createCallbackTaskWithUserId(&initCourseBillboardMarker, 0, 0x64, 2);
        createCallbackTaskWithUserId(&initRaceCourseRankModel, 0, 0x64, 1);
        createCallbackTask(&initRaceIntroModelMeshes, 0, 0x64);
        if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
            if (sp2C != 0) {
                createCallbackTask(&initCourseCollectibleSprites, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x15);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x16);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x17);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x18);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x19);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x1A);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x1B);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x1C);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x1D);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x43);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x44);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x45);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x5A);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x5B);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x5C);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x5D);
            }
        }
        if (gPlayerCount == 1) {
            createCallbackTask(&initRaceIntroFlyoverActor, 0, 0x64);
        }
        setBootFadeColor(0x82U, 0xAFU, 0xFFU);
        break;
    case 4:
        createCallbackTask(&initRaceCourseModelRenderTask, 0, 0x64);
        createCallbackTask(&initRaceCourseSceneryObjects, 0, 0x64);
        createCallbackTask(&initCourseGateObject, 0, 0x64);
        createCallbackTaskWithUserId(&initRaceCourseTripleParticle, 0, 0x64, 0);
        createCallbackTaskWithUserId(&func_800631B0, 0, 0x64, 0);
        createCallbackTaskWithUserId(&initRaceCourseSpinningObject, 0, 0x64, 0);
        createCallbackTaskWithUserId(&initRaceCourseSlideSprite, 0, 0x64, 4);
        createCallbackTaskWithUserId(&initRaceCourseSlideSprite, 0, 0x64, 5);
        createCallbackTaskWithUserId(&initRaceCourseRankModel, 0, 0x64, 1);
        if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
            if (sp2C != 0) {
                createCallbackTask(&initCourseCollectibleSprites, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x1E);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x1F);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x20);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x21);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x22);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x23);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x24);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x25);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x26);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x5E);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x5F);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x60);
            }
        }
        createCallbackTaskWithUserId(&initIceCourseBumper, 0, 0x64, 0);
        createCallbackTaskWithUserId(&initIceCourseBumper, 0, 0x64, 1);
        createCallbackTaskWithUserId(&initIceCourseBumper, 0, 0x64, 2);
        createCallbackTaskWithUserId(&initIceCourseBumper, 0, 0x64, 3);
        createCallbackTaskWithUserId(&initIceCourseBumper, 0, 0x64, 4);
        createCallbackTaskWithUserId(&initIceCourseBumper, 0, 0x64, 5);
        createCallbackTaskWithUserId(&initIceCourseBumper, 0, 0x64, 6);
        createCallbackTaskWithUserId(&initIceCourseBumper, 0, 0x64, 7);
        createCallbackTaskWithUserId(&initIceCourseBumper, 0, 0x64, 8);
        createCallbackTaskWithUserId(&initIceCourseBumper, 0, 0x64, 9);
        setBootFadeColor(0U, 0U, 0x40U);
        break;
    case 5:
        createCallbackTask(&initRaceCourseModelRenderTask, 0, 0x64);
        createCallbackTaskWithUserId(&initRaceCourseSlideSprite, 0, 0x64, 0);
        createCallbackTaskWithUserId(&initRaceCourseSlideSprite, 0, 0x64, 1);
        createCallbackTask(&initRaceCourseSceneryObjects, 0, 0x64);
        createCallbackTaskWithUserId(&initRaceCourseRankModel, 0, 0x64, 1);
        createCallbackTask(&initCourseGateObject, 0, 0x64);
        if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
            if (sp2C != 0) {
                createCallbackTask(&initCourseCollectibleSprites, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x27);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x28);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x29);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x2A);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x2B);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x2C);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x2D);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x2E);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x40);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x41);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x61);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x62);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x63);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x64);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x65);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x66);
            }
        }
        setBootFadeColor(0xF0U, 0xE6U, 0xBEU);
        break;
    case 6:
        createCallbackTask(&initRaceCourseModelRenderTask, 0, 0x64);
        createCallbackTask(&func_80063FC0, 0, 1);
        createCallbackTask(&initRaceCourseSceneryObjects, 0, 0x64);
        createCallbackTask(&initCourseGateObject, 0, 0x64);
        createCallbackTask(&initRaceIntroModelMeshes, 0, 0x64);
        createCallbackTaskWithUserId(&initRaceIntroBillboard, 0, 0x64, 0);
        createCallbackTaskWithUserId(&initRaceIntroBillboard, 0, 0x64, 1);
        createCallbackTaskWithUserId(&initRaceIntroBillboard, 0, 0x64, 2);
        createCallbackTaskWithUserId(&initRaceCourseRankModel, 0, 0x64, 1);
        if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
            if (sp2C != 0) {
                createCallbackTask(&initCourseCollectibleSprites, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x2F);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x30);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x31);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x32);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x33);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x34);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x35);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x36);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x67);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x68);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x69);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x6A);
            }
        }
        if (gPlayerCount == 1) {
            createCallbackTask(&initRaceIntroFlyoverActor, 0, 0x64);
        }
        setTitleFadeColor(0xFFU, 0xFFU, 0xFFU);
        break;
    case 7:
        createCallbackTask(&initRaceCourseModelRenderTask, 0, 0x64);
        createCallbackTaskWithUserId(&initRaceCourseSlideSprite, 0, 0x64, 3);
        createCallbackTaskWithUserId(&initCourseTriggerVolume, 0, 0x64, 1);
        createCallbackTask(&initRaceCourseSceneryObjects, 0, 0x64);
        setBootFadeColor(0xFFU, 0xFFU, 0xFFU);
        break;
    case 8:
        createCallbackTask(&initRaceCourseModelRenderTask, 0, 0x64);
        createCallbackTask(&initRaceCourseSceneryObjects, 0, 0x64);
        createCallbackTask(&initCourseGateObject, 0, 0x64);
        createCallbackTaskWithUserId(&initRaceCourseRankModel, 0, 0x64, 1);
        if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
            if (sp2C != 0) {
                createCallbackTask(&initCourseCollectibleSprites, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x46);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x47);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x48);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x49);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x4A);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x4B);
            }
        }
        setBootFadeColor(0xFFU, 0xFFU, 0xFFU);
        break;
    case 9:
        createCallbackTask(&initRaceCourseModelRenderTask, 0, 0x64);
        createCallbackTask(&initRaceCourseSceneryObjects, 0, 0x64);
        createCallbackTask(&initCourseGateObject, 0, 0x64);
        createCallbackTask(&initRaceIntroAnimatedBillboards, 0, 0x64);
        createCallbackTaskWithUserId(&initRaceCourseRankModel, 0, 0x64, 1);
        if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0) && ((gTrainingCourseLesson == 0) || (gTrainingCourseLesson == 7) || (gTrainingCourseLesson == 8))) {
            if (sp2C != 0) {
                createCallbackTask(&initCourseCollectibleSprites, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x37);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x38);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x39);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x3B);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x3C);
                createCallbackTaskWithUserId(&initRacePickup, 0, 0x64, 0x3D);
            }
        }
        if (gRaceSplitscreenMode == 1) {
            if (gRaceTypeSelection == 0) {
                createCallbackTask(&initRaceCourseCoinMarkers, 0, 0x64);
            }
            if (gRaceTypeSelection == 1) {
                createCallbackTask(&initRaceScoreAttackRings, 0, 1);
            }
        }
        setBootFadeColor(0x20U, 0x40U, 0x50U);
        break;
    }
    createCallbackTask(&initRaceItemTextureEffects, 5, 0x6E);
}
