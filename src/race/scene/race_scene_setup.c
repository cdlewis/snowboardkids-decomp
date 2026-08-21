#include "game/race/race_state.h"
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

RomAssetRange gCharacterRawAssetRanges[6] = {
    { (u8 *)&_1502A0_ROM_START, (u8 *)&_1502A0_ROM_END },
    { (u8 *)&_152930_ROM_START, (u8 *)&_152930_ROM_END },
    { (u8 *)&_155A20_ROM_START, (u8 *)&_155A20_ROM_END },
    { (u8 *)&_158D90_ROM_START, (u8 *)&_158D90_ROM_END },
    { (u8 *)&_15C750_ROM_START, (u8 *)&_15C750_ROM_END },
    { (u8 *)&_160360_ROM_START, (u8 *)&_160360_ROM_END },
};

RomAssetRange gCharacterTextureAssetRanges[6] = {
    { (u8 *)&_1E19C0_ROM_START, (u8 *)&_1E19C0_ROM_END },
    { (u8 *)&_1E2380_ROM_START, (u8 *)&_1E2380_ROM_END },
    { (u8 *)&_1E2DE0_ROM_START, (u8 *)&_1E2DE0_ROM_END },
    { (u8 *)&_1E3FE0_ROM_START, (u8 *)&_1E3FE0_ROM_END },
    { (u8 *)&_1E4AB0_ROM_START, (u8 *)&_1E4AB0_ROM_END },
    { (u8 *)&_1E68A0_ROM_START, (u8 *)&_1E68A0_ROM_END },
};

RomAssetRange gCharacterModelAssetRanges[6] = {
    { (u8 *)&_1F2220_ROM_START, (u8 *)&_1F2220_ROM_END },
    { (u8 *)&_1F7D20_ROM_START, (u8 *)&_1F7D20_ROM_END },
    { (u8 *)&_1FE860_ROM_START, (u8 *)&_1FE860_ROM_END },
    { (u8 *)&_2044B0_ROM_START, (u8 *)&_2044B0_ROM_END },
    { (u8 *)&_20A940_ROM_START, (u8 *)&_20A940_ROM_END },
    { (u8 *)&_211470_ROM_START, (u8 *)&_211470_ROM_END },
};

extern u8 gTrainingCourseLesson;
extern u8 gMainMenuModeSelection;
void loadRaceCourseAssets(void) {
    s32 size;

    switch (gRaceCourseIndex.unsignedValue) {
        case 0:
            size = (u8 *)&BIG_SNOWMAN_COURSE_AUXILIARY_DISPLAY_LISTS_ROM_END - (u8 *)&BIG_SNOWMAN_COURSE_GRAPHICS_ROM_START;
            gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
            lockRelocatableHeapBlock(gAssetHandles[0x8]);
            DMA_ASSET(BIG_SNOWMAN_COURSE_GRAPHICS, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
            LOAD_ASSET(BIG_SNOWMAN_COURSE_TEXTURES, 9);
            LOAD_ASSET(BIG_SNOWMAN_COURSE_SURFACE_DATA, 0x1B);
            initRaceCourseSurfaceData();
            LOAD_ASSET(BIG_SNOWMAN_COURSE_SPRITES, 0x1D);
            break;
        case 1:
            size = (u8 *)&SUNSET_ROCK_COURSE_AUXILIARY_DISPLAY_LISTS_ROM_END - (u8 *)&SUNSET_ROCK_COURSE_GRAPHICS_ROM_START;
            gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
            lockRelocatableHeapBlock(gAssetHandles[0x8]);
            DMA_ASSET(SUNSET_ROCK_COURSE_GRAPHICS, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
            LOAD_ASSET(SUNSET_ROCK_COURSE_TEXTURES, 9);
            LOAD_ASSET(SUNSET_ROCK_COURSE_SURFACE_DATA, 0x1B);
            initRaceCourseSurfaceData();
            LOAD_ASSET(SUNSET_ROCK_COURSE_SPRITES, 0x1D);
            break;
        case 2:
            size = (u8 *)&NIGHT_HIGHWAY_COURSE_AUXILIARY_DISPLAY_LISTS_ROM_END - (u8 *)&NIGHT_HIGHWAY_COURSE_GRAPHICS_ROM_START;
            gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
            lockRelocatableHeapBlock(gAssetHandles[0x8]);
            DMA_ASSET(NIGHT_HIGHWAY_COURSE_GRAPHICS, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
            LOAD_ASSET(NIGHT_HIGHWAY_COURSE_TEXTURES, 9);
            LOAD_ASSET(NIGHT_HIGHWAY_COURSE_SURFACE_DATA, 0x1B);
            initRaceCourseSurfaceData();
            LOAD_ASSET(NIGHT_HIGHWAY_COURSE_SPRITES, 0x1D);
            break;
        case 3:
            size = (u8 *)&GRASS_VALLEY_COURSE_AUXILIARY_DISPLAY_LISTS_ROM_END - (u8 *)&GRASS_VALLEY_COURSE_GRAPHICS_ROM_START;
            gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
            lockRelocatableHeapBlock(gAssetHandles[0x8]);
            DMA_ASSET(GRASS_VALLEY_COURSE_GRAPHICS, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
            LOAD_ASSET(GRASS_VALLEY_COURSE_TEXTURES, 9);
            LOAD_ASSET(GRASS_VALLEY_COURSE_SURFACE_DATA, 0x1B);
            initRaceCourseSurfaceData();
            LOAD_ASSET(GRASS_VALLEY_COURSE_SPRITES, 0x1D);
            break;
        case 4:
            size = (u8 *)&DIZZY_LAND_COURSE_AUXILIARY_DISPLAY_LISTS_ROM_END - (u8 *)&DIZZY_LAND_COURSE_GRAPHICS_ROM_START;
            gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
            lockRelocatableHeapBlock(gAssetHandles[0x8]);
            DMA_ASSET(DIZZY_LAND_COURSE_GRAPHICS, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
            LOAD_ASSET(DIZZY_LAND_COURSE_TEXTURES, 9);
            LOAD_ASSET(DIZZY_LAND_COURSE_SURFACE_DATA, 0x1B);
            initRaceCourseSurfaceData();
            LOAD_ASSET(DIZZY_LAND_COURSE_SPRITES, 0x1D);
            break;
        case 5:
            size = (u8 *)&QUICKSAND_VALLEY_COURSE_AUXILIARY_DISPLAY_LISTS_B_ROM_END - (u8 *)&QUICKSAND_VALLEY_COURSE_GRAPHICS_ROM_START;
            gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
            lockRelocatableHeapBlock(gAssetHandles[0x8]);
            DMA_ASSET(QUICKSAND_VALLEY_COURSE_GRAPHICS, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
            LOAD_ASSET(QUICKSAND_VALLEY_COURSE_TEXTURES, 9);
            LOAD_ASSET(QUICKSAND_VALLEY_COURSE_SURFACE_DATA, 0x1B);
            initRaceCourseSurfaceData();
            LOAD_ASSET(QUICKSAND_VALLEY_COURSE_SPRITES, 0x1D);
            break;
        case 6:
            size = (u8 *)&SILVER_MOUNTAIN_COURSE_AUXILIARY_DISPLAY_LISTS_ROM_END - (u8 *)&SILVER_MOUNTAIN_COURSE_GRAPHICS_ROM_START;
            gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
            lockRelocatableHeapBlock(gAssetHandles[0x8]);
            DMA_ASSET(SILVER_MOUNTAIN_COURSE_GRAPHICS, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
            LOAD_ASSET(SILVER_MOUNTAIN_COURSE_TEXTURES, 9);
            LOAD_ASSET(SILVER_MOUNTAIN_COURSE_SURFACE_DATA, 0x1B);
            initRaceCourseSurfaceData();
            LOAD_ASSET(SILVER_MOUNTAIN_COURSE_SPRITES, 0x1D);
            break;
        case 7:
            size = (u8 *)&ANIMAL_LAND_COURSE_AUXILIARY_DISPLAY_LISTS_ROM_END - (u8 *)&ANIMAL_LAND_COURSE_GRAPHICS_ROM_START;
            gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
            lockRelocatableHeapBlock(gAssetHandles[0x8]);
            DMA_ASSET(ANIMAL_LAND_COURSE_GRAPHICS, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
            LOAD_ASSET(ANIMAL_LAND_COURSE_TEXTURES, 9);
            LOAD_ASSET(ANIMAL_LAND_COURSE_SURFACE_DATA, 0x1B);
            initRaceCourseSurfaceData();
            break;
        case 8:
            size = (u8 *)&NINJA_LAND_COURSE_AUXILIARY_DISPLAY_LISTS_ROM_END - (u8 *)&NINJA_LAND_COURSE_GRAPHICS_ROM_START;
            gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
            lockRelocatableHeapBlock(gAssetHandles[0x8]);
            DMA_ASSET(NINJA_LAND_COURSE_GRAPHICS, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
            LOAD_ASSET(NINJA_LAND_COURSE_TEXTURES, 9);
            LOAD_ASSET(NINJA_LAND_COURSE_SURFACE_DATA, 0x1B);
            initRaceCourseSurfaceData();
            LOAD_ASSET(NINJA_LAND_COURSE_SPRITES, 0x1D);
            break;
        case 9:
            size = (u8 *)&ROOKIE_MOUNTAIN_COURSE_AUXILIARY_DISPLAY_LISTS_ROM_END - (u8 *)&ROOKIE_MOUNTAIN_COURSE_GRAPHICS_ROM_START;
            gAssetHandles[0x8] = allocRelocatableHeapBlock(size);
            lockRelocatableHeapBlock(gAssetHandles[0x8]);
            DMA_ASSET(ROOKIE_MOUNTAIN_COURSE_GRAPHICS, getRelocatableHeapBlockBase(gAssetHandles[0x8]), size);
            LOAD_ASSET(ROOKIE_MOUNTAIN_COURSE_TEXTURES, 9);
            LOAD_ASSET(ROOKIE_MOUNTAIN_COURSE_SURFACE_DATA, 0x1B);
            initRaceCourseSurfaceData();
            LOAD_ASSET(ROOKIE_MOUNTAIN_COURSE_SPRITES, 0x1D);
            break;
    }

    size = (u8 *)&_147910_SUFFIX_ROM_END - (u8 *)&_147910_ROM_START;
    gAssetHandles[10] = allocRelocatableHeapBlock(size);
    lockRelocatableHeapBlock(gAssetHandles[10]);
    DMA_ASSET(_147910, getRelocatableHeapBlockBase(gAssetHandles[0xA]), size);
    LOAD_ASSET(_1D82B0, 0xB);
    LOAD_ASSET(_1E74E0, 0x1C);
    LOAD_ASSET(_5DAF30, 0x2A);
}

void loadRaceCharacterAssets(void) {
    RacePlayer *player;
    RomAssetRange *rawRanges;
    RomAssetRange *textureRanges;
    RomAssetRange *modelRanges;
    s16 *assetHandles;
    s32 i;
    void *heapBase;
    s32 size;

    size = ASSET_SIZE(_14B450);
    gAssetHandles[0xC] = allocRelocatableHeapBlock(size);
    DMA_ASSET(_14B450, getRelocatableHeapBlockBase(gAssetHandles[0xC]), size);
    LOAD_ASSET(_1EF530, 0xD);

    rawRanges = gCharacterRawAssetRanges;
    textureRanges = gCharacterTextureAssetRanges;
    modelRanges = gCharacterModelAssetRanges;
    for (player = gRacePlayers, i = 0; i != RACE_PLAYER_COUNT; i++, player++) {
        assetHandles = &gAssetHandles[i];
        if (player->isActive != 0) {
            size = rawRanges[player->characterId & 0xFFFF].end - rawRanges[player->characterId & 0xFFFF].start;
            assetHandles[0xE] = allocRelocatableHeapBlock(size);
            heapBase = getRelocatableHeapBlockBase(assetHandles[0xE]);
            dmaReadRom((u32)rawRanges[player->characterId & 0xFFFF].start, heapBase, size);
            loadCompressedRomAsset(
                textureRanges[player->characterId & 0xFFFF].start,
                textureRanges[player->characterId & 0xFFFF].end,
                i + 0x12
            );
            loadCompressedRomAsset(
                modelRanges[player->characterId & 0xFFFF].start,
                modelRanges[player->characterId & 0xFFFF].end,
                i + 0x16
            );
        }
    }

    LOAD_ASSET(_243270, 0x1E);
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
    switch (gRaceCourseIndex.unsignedValue) {
        case 0:
            createCallbackTask((CallbackTaskCallback)&initRaceCourseModelRenderTask, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)&initRaceCourseSceneryObjects, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)&initCourseGateObject, 0, 0x64);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseRankModel, 0, 0x64, 0);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseRankModel, 0, 0x64, 1);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initCourseBillboardMarker, 0, 0x64, 0);
            if ((gRaceSplitscreenMode == 0) && (gRaceDemoPlaybackEnabled == 0) && (gTrainingCourseLesson == 0) &&
                (gMainMenuModeSelection == 0)) {
                if (sp2C != 0) {
                    createCallbackTask((CallbackTaskCallback)&initCourseCollectibleSprites, 0, 0x64);
                }
                if (sp28 != 0) {
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 1);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 2);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 3);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 4);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 5);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x3F);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x4E);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x4F);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x50);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x51);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x52);
                }
            }
            if (gRaceSplitscreenMode == 1) {
                if (gRaceTypeSelection == 0) {
                    createCallbackTask((CallbackTaskCallback)&initRaceCourseCoinMarkers, 0, 0x64);
                }
                if (gRaceTypeSelection == 1) {
                    createCallbackTask((CallbackTaskCallback)&initRaceScoreAttackRings, 0, 1);
                }
            }
            if (gRaceSplitscreenMode == 1) {
                if (gRaceTypeSelection != 1) {
                    goto block_24;
                }
            } else {
            block_24:
                createCallbackTask((CallbackTaskCallback)&initCourseTextureMarkers, 0, 0x64);
            }
            setBootFadeColor(0x80U, 0xC0U, 0xFFU);
            break;
        case 1:
            createCallbackTask((CallbackTaskCallback)&initRaceCourseModelRenderTask, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)&initCourseTextureMarkers, 0, 0x64);
            spawnPatrolCourseObject(0x17, 0xF682AF46, 0xEB74F202, 0xF82BAF46, 0xEA17F202);
            spawnPatrolCourseObject(0x17, 0xF7062F46, 0xE897F202, 0xF5C82F46, 0xEA58F202);
            spawnPatrolCourseObject(0x17, 0xF45B2F46, 0xE876F202, 0xF6502F46, 0xE7927202);
            spawnPatrolCourseObject(0x17, 0xF53F2F46, 0xE5987202, 0xF2E4AF46, 0xE61F7202);
            spawnPatrolCourseObject(0x17, 0xF2BDAF46, 0xE548F202, 0xF3AE2F46, 0xE3AF7202);
            spawnPatrolCourseObject(0x17, 0xF25E41BF, 0xE5FB38EE, 0xF3169B57, 0xE56EEC94);
            spawnPatrolCourseObject(0x17, 0xF50D1255, 0xE7032BEC, 0xF2E89861, 0xE70079E8);
            spawnPatrolCourseObject(0x17, 0xF3EFF851, 0xE8BCCFE0, 0xF5650CCB, 0xE9A4FC34);
            spawnPatrolCourseObject(0x17, 0xF6BEB9D1, 0xEA69C4DE, 0xF6F1B095, 0xEBF8B05E);
            createCallbackTask((CallbackTaskCallback)&initRaceCourseSceneryObjects, 0, 0x64);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseSlideSprite, 0, 0x64, 6);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseSlideSprite, 0, 0x64, 7);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initCourseTriggerVolume, 0, 0x64, 0);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCoursePropModels, 0, 0x64, 0);
            createCallbackTask((CallbackTaskCallback)&initCourseGateObject, 0, 0x64);
            createCallbackTaskWithUserId((CallbackTaskCallback)&updateThrownPickupSpawner, 0, 0x64, 0);
            createCallbackTaskWithUserId((CallbackTaskCallback)&updateThrownPickupSpawner, 0, 0x64, 1);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseRankModel, 0, 0x64, 1);
            if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
                if (sp2C != 0) {
                    createCallbackTask((CallbackTaskCallback)&initCourseCollectibleSprites, 0, 0x64);
                }
                if (sp28 != 0) {
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 6);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 7);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 8);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 9);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0xA);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0xB);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0xC);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0xD);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x53);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x54);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x55);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x56);
                }
                func_80061AF4(0x59, (void *)0x2290CDA1, (void *)0xC5F82815, 0xAF0);
            }
            if (gRaceSplitscreenMode == 1) {
                if (gRaceTypeSelection == 0) {
                    createCallbackTask((CallbackTaskCallback)&initRaceCourseCoinMarkers, 0, 0x64);
                }
                if (gRaceTypeSelection == 1) {
                    createCallbackTask((CallbackTaskCallback)&initRaceScoreAttackRings, 0, 1);
                }
            }
            setBootFadeColor(0xFFU, 0x80U, 0U);
            break;
        case 2:
            createCallbackTask((CallbackTaskCallback)&initRaceCourseModelRenderTask, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)&initRaceCourseSceneryObjects, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)&initCourseGateObject, 0, 0x64);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseSlideSprite, 0, 0x64, 2);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initCourseStartFinishSprite, 0, 0x64, 0);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initCourseStartFinishSprite, 0, 0x64, 1);
            if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
                if (sp2C != 0) {
                    createCallbackTask((CallbackTaskCallback)&initCourseCollectibleSprites, 0, 0x64);
                }
                if (sp28 != 0) {
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0xE);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0xF);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x10);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x11);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x12);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x13);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x14);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x42);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x57);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x58);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x59);
                }
            }
            setBootFadeColor(0U, 0U, 0x32U);
            break;
        case 3:
            createCallbackTask((CallbackTaskCallback)&initRaceCourseModelRenderTask, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)&initRaceCourseSceneryObjects, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)&initCourseGateObject, 0, 0x64);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initCourseBillboardMarker, 0, 0x64, 1);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initCourseBillboardMarker, 0, 0x64, 2);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseRankModel, 0, 0x64, 1);
            createCallbackTask((CallbackTaskCallback)&initRaceIntroModelMeshes, 0, 0x64);
            if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
                if (sp2C != 0) {
                    createCallbackTask((CallbackTaskCallback)&initCourseCollectibleSprites, 0, 0x64);
                }
                if (sp28 != 0) {
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x15);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x16);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x17);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x18);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x19);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x1A);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x1B);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x1C);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x1D);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x43);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x44);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x45);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x5A);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x5B);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x5C);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x5D);
                }
            }
            if (gPlayerCount == 1) {
                createCallbackTask((CallbackTaskCallback)&initRaceIntroFlyoverActor, 0, 0x64);
            }
            setBootFadeColor(0x82U, 0xAFU, 0xFFU);
            break;
        case 4:
            createCallbackTask((CallbackTaskCallback)&initRaceCourseModelRenderTask, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)&initRaceCourseSceneryObjects, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)&initCourseGateObject, 0, 0x64);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseTripleParticle, 0, 0x64, 0);
            createCallbackTaskWithUserId((CallbackTaskCallback)&func_800631B0, 0, 0x64, 0);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseSpinningObject, 0, 0x64, 0);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseSlideSprite, 0, 0x64, 4);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseSlideSprite, 0, 0x64, 5);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseRankModel, 0, 0x64, 1);
            if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
                if (sp2C != 0) {
                    createCallbackTask((CallbackTaskCallback)&initCourseCollectibleSprites, 0, 0x64);
                }
                if (sp28 != 0) {
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x1E);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x1F);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x20);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x21);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x22);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x23);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x24);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x25);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x26);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x5E);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x5F);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x60);
                }
            }
            createCallbackTaskWithUserId((CallbackTaskCallback)&initIceCourseBumper, 0, 0x64, 0);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initIceCourseBumper, 0, 0x64, 1);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initIceCourseBumper, 0, 0x64, 2);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initIceCourseBumper, 0, 0x64, 3);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initIceCourseBumper, 0, 0x64, 4);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initIceCourseBumper, 0, 0x64, 5);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initIceCourseBumper, 0, 0x64, 6);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initIceCourseBumper, 0, 0x64, 7);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initIceCourseBumper, 0, 0x64, 8);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initIceCourseBumper, 0, 0x64, 9);
            setBootFadeColor(0U, 0U, 0x40U);
            break;
        case 5:
            createCallbackTask((CallbackTaskCallback)&initRaceCourseModelRenderTask, 0, 0x64);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseSlideSprite, 0, 0x64, 0);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseSlideSprite, 0, 0x64, 1);
            createCallbackTask((CallbackTaskCallback)&initRaceCourseSceneryObjects, 0, 0x64);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseRankModel, 0, 0x64, 1);
            createCallbackTask((CallbackTaskCallback)&initCourseGateObject, 0, 0x64);
            if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
                if (sp2C != 0) {
                    createCallbackTask((CallbackTaskCallback)&initCourseCollectibleSprites, 0, 0x64);
                }
                if (sp28 != 0) {
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x27);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x28);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x29);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x2A);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x2B);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x2C);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x2D);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x2E);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x40);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x41);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x61);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x62);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x63);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x64);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x65);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x66);
                }
            }
            setBootFadeColor(0xF0U, 0xE6U, 0xBEU);
            break;
        case 6:
            createCallbackTask((CallbackTaskCallback)&initRaceCourseModelRenderTask, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)&func_80063FC0, 0, 1);
            createCallbackTask((CallbackTaskCallback)&initRaceCourseSceneryObjects, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)&initCourseGateObject, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)&initRaceIntroModelMeshes, 0, 0x64);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceIntroBillboard, 0, 0x64, 0);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceIntroBillboard, 0, 0x64, 1);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceIntroBillboard, 0, 0x64, 2);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseRankModel, 0, 0x64, 1);
            if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
                if (sp2C != 0) {
                    createCallbackTask((CallbackTaskCallback)&initCourseCollectibleSprites, 0, 0x64);
                }
                if (sp28 != 0) {
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x2F);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x30);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x31);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x32);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x33);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x34);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x35);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x36);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x67);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x68);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x69);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x6A);
                }
            }
            if (gPlayerCount == 1) {
                createCallbackTask((CallbackTaskCallback)&initRaceIntroFlyoverActor, 0, 0x64);
            }
            setTitleFadeColor(0xFFU, 0xFFU, 0xFFU);
            break;
        case 7:
            createCallbackTask((CallbackTaskCallback)&initRaceCourseModelRenderTask, 0, 0x64);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseSlideSprite, 0, 0x64, 3);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initCourseTriggerVolume, 0, 0x64, 1);
            createCallbackTask((CallbackTaskCallback)&initRaceCourseSceneryObjects, 0, 0x64);
            setBootFadeColor(0xFFU, 0xFFU, 0xFFU);
            break;
        case 8:
            createCallbackTask((CallbackTaskCallback)&initRaceCourseModelRenderTask, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)&initRaceCourseSceneryObjects, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)&initCourseGateObject, 0, 0x64);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseRankModel, 0, 0x64, 1);
            if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
                if (sp2C != 0) {
                    createCallbackTask((CallbackTaskCallback)&initCourseCollectibleSprites, 0, 0x64);
                }
                if (sp28 != 0) {
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x46);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x47);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x48);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x49);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x4A);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x4B);
                }
            }
            setBootFadeColor(0xFFU, 0xFFU, 0xFFU);
            break;
        case 9:
            createCallbackTask((CallbackTaskCallback)&initRaceCourseModelRenderTask, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)&initRaceCourseSceneryObjects, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)&initCourseGateObject, 0, 0x64);
            createCallbackTask((CallbackTaskCallback)&initRaceIntroAnimatedBillboards, 0, 0x64);
            createCallbackTaskWithUserId((CallbackTaskCallback)&initRaceCourseRankModel, 0, 0x64, 1);
            if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0) &&
                ((gTrainingCourseLesson == 0) || (gTrainingCourseLesson == 7) || (gTrainingCourseLesson == 8))) {
                if (sp2C != 0) {
                    createCallbackTask((CallbackTaskCallback)&initCourseCollectibleSprites, 0, 0x64);
                }
                if (sp28 != 0) {
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x37);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x38);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x39);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x3B);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x3C);
                    createCallbackTaskWithUserId((CallbackTaskCallback)&initRacePickup, 0, 0x64, 0x3D);
                }
            }
            if (gRaceSplitscreenMode == 1) {
                if (gRaceTypeSelection == 0) {
                    createCallbackTask((CallbackTaskCallback)&initRaceCourseCoinMarkers, 0, 0x64);
                }
                if (gRaceTypeSelection == 1) {
                    createCallbackTask((CallbackTaskCallback)&initRaceScoreAttackRings, 0, 1);
                }
            }
            setBootFadeColor(0x20U, 0x40U, 0x50U);
            break;
    }
    createCallbackTask((CallbackTaskCallback)&initRaceItemTextureEffects, 5, 0x6E);
}
