#include "common.h"
#include "memory_block_allocator.h"
#include "asset_manager.h"
#include "race_scene_loader.h"
#include "callback_task_scheduler.h"
#include "system_boot.h"
#include "model_animation.h"
#include "race_course_effects.h"
#include "race_course_preview.h"
#include "race_item_effects.h"
#include "race_overlay_effects.h"
#include "race_ui_effects.h"

#define COURSE_PLAYER_COUNT 4
#define COURSE_PLAYER_STRIDE 0x60C

typedef struct {
    /* 0x0 */ u8 *start;
    /* 0x4 */ u8 *end;
} RomAssetRange;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u8 characterId;
    /* 0x11 */ u8 pad11[2];
    /* 0x13 */ s8 isActive;
    /* 0x14 */ u8 pad14[COURSE_PLAYER_STRIDE - 0x14];
} RaceScenePlayer;

extern s16 D_80112130[];
extern s16 D_80112140;
extern s16 D_80112144;
extern u16 gRaceCourseIndex;
extern RaceScenePlayer D_80121D80[];
extern RomAssetRange gCharacterRawAssetRanges[];
extern RomAssetRange gCharacterTextureAssetRanges[];
extern RomAssetRange gCharacterModelAssetRanges[];

extern u8 D_E35C0[];
extern u8 D_EC9A0[];
extern u8 D_F6160[];
extern u8 D_FEDA0[];
extern u8 D_10AAB0[];
extern u8 D_118670[];
extern u8 D_125C90[];
extern u8 D_131A90[];
extern u8 D_1384B0[];
extern u8 D_13F3B0[];
extern u8 D_145380[];
extern u8 D_147910[];
extern u8 D_14B450[];
extern u8 D_1502A0[];
extern u8 D_163410[];
extern u8 D_171480[];
extern u8 D_17D220[];
extern u8 D_186910[];
extern u8 D_197490[];
extern u8 D_1A5620[];
extern u8 D_1B3160[];
extern u8 D_1C24B0[];
extern u8 D_1C9630[];
extern u8 D_1D3070[];
extern u8 D_1D82B0[];
extern u8 D_1DCED0[];
extern u8 D_1E74E0[];
extern u8 D_1EC0F0[];
extern u8 D_1EC4A0[];
extern u8 D_1EC810[];
extern u8 D_1ECD00[];
extern u8 D_1ED910[];
extern u8 D_1EDC80[];
extern u8 D_1EE060[];
extern u8 D_1EEEA0[];
extern u8 D_1EF1D0[];
extern u8 D_1EF530[];
extern u8 D_1F1A90[];
extern u8 D_21D9D0[];
extern u8 D_222500[];
extern u8 D_2274A0[];
extern u8 D_22AE50[];
extern u8 D_2308E0[];
extern u8 D_2349E0[];
extern u8 D_23A7D0[];
extern u8 D_23E9D0[];
extern u8 D_23FCC0[];
extern u8 D_241160[];
extern u8 D_2427D0[];
extern u8 D_243270[];
extern u8 D_245A80[];
extern u8 D_5DAF30[];
extern u8 D_5DB9D0[];

extern u8 gRaceSplitscreenMode;
extern u8 gPlayerCount;
extern u8 D_80121B59;
extern u8 gTrainingCourseLesson;
extern u8 gMainMenuModeSelection;
extern u8 D_80121B5E;
extern u8 gRaceCourseModelEffectsDisabled;
extern u8 gRaceCourseOverlayEffectsDisabled;

#ifdef NON_MATCHING
// loadRaceCourseAssets best match: 63.023% at nonmatchings/loadRaceCourseAssets-2225551288923588688/base_4.c.
void loadRaceCourseAssets(void) {
    s32 size;

    switch (gRaceCourseIndex) {
    case 0:
        size = D_EC9A0 - D_E35C0;
        D_80112140 = allocMemoryBlock(size);
        lockMemoryBlock(D_80112140);
        dmaReadRom(D_E35C0, getMemoryBlockBase(D_80112140), size);
        loadCompressedRomAsset(D_163410, D_171480, 9);
        loadCompressedRomAsset(D_21D9D0, D_222500, 0x1B);
        func_8007D190();
        loadCompressedRomAsset(D_1EC0F0, D_1EC4A0, 0x1D);
        break;
    case 1:
        size = D_F6160 - D_EC9A0;
        D_80112140 = allocMemoryBlock(size);
        lockMemoryBlock(D_80112140);
        dmaReadRom(D_EC9A0, getMemoryBlockBase(D_80112140), size);
        loadCompressedRomAsset(D_171480, D_17D220, 9);
        loadCompressedRomAsset(D_222500, D_2274A0, 0x1B);
        func_8007D190();
        loadCompressedRomAsset(D_1EC4A0, D_1EC810, 0x1D);
        break;
    case 2:
        size = D_FEDA0 - D_F6160;
        D_80112140 = allocMemoryBlock(size);
        lockMemoryBlock(D_80112140);
        dmaReadRom(D_F6160, getMemoryBlockBase(D_80112140), size);
        loadCompressedRomAsset(D_17D220, D_186910, 9);
        loadCompressedRomAsset(D_2274A0, D_22AE50, 0x1B);
        func_8007D190();
        loadCompressedRomAsset(D_1EC810, D_1ECD00, 0x1D);
        break;
    case 3:
        size = D_10AAB0 - D_FEDA0;
        D_80112130[8] = allocMemoryBlock(size);
        lockMemoryBlock(D_80112130[8]);
        dmaReadRom(D_FEDA0, getMemoryBlockBase(D_80112140), size);
        loadCompressedRomAsset(D_186910, D_197490, 9);
        loadCompressedRomAsset(D_22AE50, D_2308E0, 0x1B);
        func_8007D190();
        loadCompressedRomAsset(D_1ECD00, D_1ED910, 0x1D);
        break;
    case 4:
        size = D_118670 - D_10AAB0;
        D_80112130[8] = allocMemoryBlock(size);
        lockMemoryBlock(D_80112130[8]);
        dmaReadRom(D_10AAB0, getMemoryBlockBase(D_80112140), size);
        loadCompressedRomAsset(D_197490, D_1A5620, 9);
        loadCompressedRomAsset(D_2308E0, D_2349E0, 0x1B);
        func_8007D190();
        loadCompressedRomAsset(D_1ED910, D_1EDC80, 0x1D);
        break;
    case 5:
        size = D_125C90 - D_118670;
        D_80112130[8] = allocMemoryBlock(size);
        lockMemoryBlock(D_80112130[8]);
        dmaReadRom(D_118670, getMemoryBlockBase(D_80112140), size);
        loadCompressedRomAsset(D_1A5620, D_1B3160, 9);
        loadCompressedRomAsset(D_2349E0, D_23A7D0, 0x1B);
        func_8007D190();
        loadCompressedRomAsset(D_1EDC80, D_1EE060, 0x1D);
        break;
    case 6:
        size = D_131A90 - D_125C90;
        D_80112130[8] = allocMemoryBlock(size);
        lockMemoryBlock(D_80112130[8]);
        dmaReadRom(D_125C90, getMemoryBlockBase(D_80112140), size);
        loadCompressedRomAsset(D_1B3160, D_1C24B0, 9);
        loadCompressedRomAsset(D_23A7D0, D_23E9D0, 0x1B);
        func_8007D190();
        loadCompressedRomAsset(D_1EE060, D_1EEEA0, 0x1D);
        break;
    case 7:
        size = D_1384B0 - D_131A90;
        D_80112130[8] = allocMemoryBlock(size);
        lockMemoryBlock(D_80112130[8]);
        dmaReadRom(D_131A90, getMemoryBlockBase(D_80112140), size);
        loadCompressedRomAsset(D_1C24B0, D_1C9630, 9);
        loadCompressedRomAsset(D_23E9D0, D_23FCC0, 0x1B);
        func_8007D190();
        break;
    case 8:
        size = D_13F3B0 - D_1384B0;
        D_80112130[8] = allocMemoryBlock(size);
        lockMemoryBlock(D_80112130[8]);
        dmaReadRom(D_1384B0, getMemoryBlockBase(D_80112140), size);
        loadCompressedRomAsset(D_1C9630, D_1D3070, 9);
        loadCompressedRomAsset(D_23FCC0, D_241160, 0x1B);
        func_8007D190();
        loadCompressedRomAsset(D_1EEEA0, D_1EF1D0, 0x1D);
        break;
    case 9:
        size = D_145380 - D_13F3B0;
        D_80112130[8] = allocMemoryBlock(size);
        lockMemoryBlock(D_80112130[8]);
        dmaReadRom(D_13F3B0, getMemoryBlockBase(D_80112140), size);
        loadCompressedRomAsset(D_1D3070, D_1D82B0, 9);
        loadCompressedRomAsset(D_241160, D_2427D0, 0x1B);
        func_8007D190();
        loadCompressedRomAsset(D_1EF1D0, D_1EF530, 0x1D);
        break;
    }

    size = D_14B450 - D_147910;
    D_80112130[10] = allocMemoryBlock(size);
    lockMemoryBlock(D_80112130[10]);
    dmaReadRom(D_147910, getMemoryBlockBase(D_80112144), size);
    loadCompressedRomAsset(D_1D82B0, D_1DCED0, 0xB);
    loadCompressedRomAsset(D_1E74E0, D_1EC0F0, 0x1C);
    loadCompressedRomAsset(D_5DAF30, D_5DB9D0, 0x2A);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/race_scene_loader/loadRaceCourseAssets.s")
#endif

// loadRaceCharacterAssets best match: 98.654% at nonmatchings/loadRaceCharacterAssets-5635509610426229442/base_6.c.
#ifdef NON_MATCHING
void loadRaceCharacterAssets(void) {
    RaceScenePlayer *player;
    RomAssetRange *rawRanges;
    RomAssetRange *textureRanges;
    RomAssetRange *modelRanges;
    s16 *assetHandles;
    s32 i;
    s32 size;

    size = D_1502A0 - D_14B450;
    D_80112130[0xC] = allocMemoryBlock(size);
    dmaReadRom(D_14B450, getMemoryBlockBase(D_80112130[0xC]), size);
    loadCompressedRomAsset(D_1EF530, D_1F1A90, 0xD);

    rawRanges = gCharacterRawAssetRanges;
    textureRanges = gCharacterTextureAssetRanges;
    modelRanges = gCharacterModelAssetRanges;
    for (player = D_80121D80, i = 0; i != COURSE_PLAYER_COUNT; i++, player++) {
        assetHandles = &D_80112130[i];
        if (player->isActive != 0) {
            size = rawRanges[player->characterId].end - rawRanges[player->characterId].start;
            assetHandles[0xE] = allocMemoryBlock(size);
            dmaReadRom(rawRanges[player->characterId].start, getMemoryBlockBase(assetHandles[0xE]), size);
            loadCompressedRomAsset(textureRanges[player->characterId].start, textureRanges[player->characterId].end, i + 0x12);
            loadCompressedRomAsset(modelRanges[player->characterId].start, modelRanges[player->characterId].end, i + 0x16);
        }
    }

    loadCompressedRomAsset(D_243270, D_245A80, 0x1E);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/race_scene_loader/loadRaceCharacterAssets.s")
#endif

void initRaceCourseEffects(void) {
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
        createCallbackTask(&func_8006A798, 0, 0x64);
        createCallbackTask(&func_8006B108, 0, 0x64);
        createCallbackTask(&func_8006C698, 0, 0x64);
        createCallbackTaskWithUserId(&func_8006429C, 0, 0x64, 0);
        createCallbackTaskWithUserId(&func_8006429C, 0, 0x64, 1);
        createCallbackTaskWithUserId(&func_8006CBBC, 0, 0x64, 0);
        if ((gRaceSplitscreenMode == 0) && (D_80121B59 == 0) && (gTrainingCourseLesson == 0) && (gMainMenuModeSelection == 0)) {
            if (sp2C != 0) {
                createCallbackTask(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 1);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 2);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 3);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 4);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 5);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x3F);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x4E);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x4F);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x50);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x51);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x52);
            }
        }
        if (gRaceSplitscreenMode == 1) {
            if (D_80121B5E == 0) {
                createCallbackTask(&func_80065764, 0, 0x64);
            }
            if (D_80121B5E == 1) {
                createCallbackTask(&func_800666B0, 0, 1);
            }
        }
        if (gRaceSplitscreenMode == 1) {
            if (D_80121B5E != 1) {
                goto block_24;
            }
        } else {
block_24:
            createCallbackTask(&func_8006AE00, 0, 0x64);
        }
        setBootFadeColor(0x80U, 0xC0U, 0xFFU);
        break;
    case 1:
        createCallbackTask(&func_8006A798, 0, 0x64);
        createCallbackTask(&func_8006AE00, 0, 0x64);
        func_8006B760(0x17, 0xF682AF46, 0xEB74F202, 0xF82BAF46, 0xEA17F202);
        func_8006B760(0x17, 0xF7062F46, 0xE897F202, 0xF5C82F46, 0xEA58F202);
        func_8006B760(0x17, 0xF45B2F46, 0xE876F202, 0xF6502F46, 0xE7927202);
        func_8006B760(0x17, 0xF53F2F46, 0xE5987202, 0xF2E4AF46, 0xE61F7202);
        func_8006B760(0x17, 0xF2BDAF46, 0xE548F202, 0xF3AE2F46, 0xE3AF7202);
        func_8006B760(0x17, 0xF25E41BF, 0xE5FB38EE, 0xF3169B57, 0xE56EEC94);
        func_8006B760(0x17, 0xF50D1255, 0xE7032BEC, 0xF2E89861, 0xE70079E8);
        func_8006B760(0x17, 0xF3EFF851, 0xE8BCCFE0, 0xF5650CCB, 0xE9A4FC34);
        func_8006B760(0x17, 0xF6BEB9D1, 0xEA69C4DE, 0xF6F1B095, 0xEBF8B05E);
        createCallbackTask(&func_8006B108, 0, 0x64);
        createCallbackTaskWithUserId(&func_80057600, 0, 0x64, 6);
        createCallbackTaskWithUserId(&func_80057600, 0, 0x64, 7);
        createCallbackTaskWithUserId(&func_8006D384, 0, 0x64, 0);
        createCallbackTaskWithUserId(&func_800669A0, 0, 0x64, 0);
        createCallbackTask(&func_8006C698, 0, 0x64);
        createCallbackTaskWithUserId(&func_800675AC, 0, 0x64, 0);
        createCallbackTaskWithUserId(&func_800675AC, 0, 0x64, 1);
        createCallbackTaskWithUserId(&func_8006429C, 0, 0x64, 1);
        if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
            if (sp2C != 0) {
                createCallbackTask(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 6);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 7);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 8);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 9);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0xA);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0xB);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0xC);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0xD);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x53);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x54);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x55);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x56);
            }
            func_80061AF4(0x59, (void *)0x2290CDA1, (void *)0xC5F82815, 0xAF0);
        }
        if (gRaceSplitscreenMode == 1) {
            if (D_80121B5E == 0) {
                createCallbackTask(&func_80065764, 0, 0x64);
            }
            if (D_80121B5E == 1) {
                createCallbackTask(&func_800666B0, 0, 1);
            }
        }
        setBootFadeColor(0xFFU, 0x80U, 0U);
        break;
    case 2:
        createCallbackTask(&func_8006A798, 0, 0x64);
        createCallbackTask(&func_8006B108, 0, 0x64);
        createCallbackTask(&func_8006C698, 0, 0x64);
        createCallbackTaskWithUserId(&func_80057600, 0, 0x64, 2);
        createCallbackTaskWithUserId(&func_80063980, 0, 0x64, 0);
        createCallbackTaskWithUserId(&func_80063980, 0, 0x64, 1);
        if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
            if (sp2C != 0) {
                createCallbackTask(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0xE);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0xF);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x10);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x11);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x12);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x13);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x14);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x42);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x57);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x58);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x59);
            }
        }
        setBootFadeColor(0U, 0U, 0x32U);
        break;
    case 3:
        createCallbackTask(&func_8006A798, 0, 0x64);
        createCallbackTask(&func_8006B108, 0, 0x64);
        createCallbackTask(&func_8006C698, 0, 0x64);
        createCallbackTaskWithUserId(&func_8006CBBC, 0, 0x64, 1);
        createCallbackTaskWithUserId(&func_8006CBBC, 0, 0x64, 2);
        createCallbackTaskWithUserId(&func_8006429C, 0, 0x64, 1);
        createCallbackTask(&initRaceCoursePreviewModelMeshes, 0, 0x64);
        if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
            if (sp2C != 0) {
                createCallbackTask(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x15);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x16);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x17);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x18);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x19);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x1A);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x1B);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x1C);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x1D);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x43);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x44);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x45);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x5A);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x5B);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x5C);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x5D);
            }
        }
        if (gPlayerCount == 1) {
            createCallbackTask(&initRaceCoursePreviewCameraCutscene, 0, 0x64);
        }
        setBootFadeColor(0x82U, 0xAFU, 0xFFU);
        break;
    case 4:
        createCallbackTask(&func_8006A798, 0, 0x64);
        createCallbackTask(&func_8006B108, 0, 0x64);
        createCallbackTask(&func_8006C698, 0, 0x64);
        createCallbackTaskWithUserId(&func_80061428, 0, 0x64, 0);
        createCallbackTaskWithUserId(&func_800631B0, 0, 0x64, 0);
        createCallbackTaskWithUserId(&func_80063470, 0, 0x64, 0);
        createCallbackTaskWithUserId(&func_80057600, 0, 0x64, 4);
        createCallbackTaskWithUserId(&func_80057600, 0, 0x64, 5);
        createCallbackTaskWithUserId(&func_8006429C, 0, 0x64, 1);
        if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
            if (sp2C != 0) {
                createCallbackTask(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x1E);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x1F);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x20);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x21);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x22);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x23);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x24);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x25);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x26);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x5E);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x5F);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x60);
            }
        }
        createCallbackTaskWithUserId(&func_80062ED4, 0, 0x64, 0);
        createCallbackTaskWithUserId(&func_80062ED4, 0, 0x64, 1);
        createCallbackTaskWithUserId(&func_80062ED4, 0, 0x64, 2);
        createCallbackTaskWithUserId(&func_80062ED4, 0, 0x64, 3);
        createCallbackTaskWithUserId(&func_80062ED4, 0, 0x64, 4);
        createCallbackTaskWithUserId(&func_80062ED4, 0, 0x64, 5);
        createCallbackTaskWithUserId(&func_80062ED4, 0, 0x64, 6);
        createCallbackTaskWithUserId(&func_80062ED4, 0, 0x64, 7);
        createCallbackTaskWithUserId(&func_80062ED4, 0, 0x64, 8);
        createCallbackTaskWithUserId(&func_80062ED4, 0, 0x64, 9);
        setBootFadeColor(0U, 0U, 0x40U);
        break;
    case 5:
        createCallbackTask(&func_8006A798, 0, 0x64);
        createCallbackTaskWithUserId(&func_80057600, 0, 0x64, 0);
        createCallbackTaskWithUserId(&func_80057600, 0, 0x64, 1);
        createCallbackTask(&func_8006B108, 0, 0x64);
        createCallbackTaskWithUserId(&func_8006429C, 0, 0x64, 1);
        createCallbackTask(&func_8006C698, 0, 0x64);
        if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
            if (sp2C != 0) {
                createCallbackTask(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x27);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x28);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x29);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x2A);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x2B);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x2C);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x2D);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x2E);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x40);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x41);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x61);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x62);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x63);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x64);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x65);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x66);
            }
        }
        setBootFadeColor(0xF0U, 0xE6U, 0xBEU);
        break;
    case 6:
        createCallbackTask(&func_8006A798, 0, 0x64);
        createCallbackTask(&func_80063FC0, 0, 1);
        createCallbackTask(&func_8006B108, 0, 0x64);
        createCallbackTask(&func_8006C698, 0, 0x64);
        createCallbackTask(&initRaceCoursePreviewModelMeshes, 0, 0x64);
        createCallbackTaskWithUserId(&initRaceCoursePreviewBillboard, 0, 0x64, 0);
        createCallbackTaskWithUserId(&initRaceCoursePreviewBillboard, 0, 0x64, 1);
        createCallbackTaskWithUserId(&initRaceCoursePreviewBillboard, 0, 0x64, 2);
        createCallbackTaskWithUserId(&func_8006429C, 0, 0x64, 1);
        if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
            if (sp2C != 0) {
                createCallbackTask(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x2F);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x30);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x31);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x32);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x33);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x34);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x35);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x36);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x67);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x68);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x69);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x6A);
            }
        }
        if (gPlayerCount == 1) {
            createCallbackTask(&initRaceCoursePreviewCameraCutscene, 0, 0x64);
        }
        setTitleFadeColor(0xFFU, 0xFFU, 0xFFU);
        break;
    case 7:
        createCallbackTask(&func_8006A798, 0, 0x64);
        createCallbackTaskWithUserId(&func_80057600, 0, 0x64, 3);
        createCallbackTaskWithUserId(&func_8006D384, 0, 0x64, 1);
        createCallbackTask(&func_8006B108, 0, 0x64);
        setBootFadeColor(0xFFU, 0xFFU, 0xFFU);
        break;
    case 8:
        createCallbackTask(&func_8006A798, 0, 0x64);
        createCallbackTask(&func_8006B108, 0, 0x64);
        createCallbackTask(&func_8006C698, 0, 0x64);
        createCallbackTaskWithUserId(&func_8006429C, 0, 0x64, 1);
        if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0)) {
            if (sp2C != 0) {
                createCallbackTask(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x46);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x47);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x48);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x49);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x4A);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x4B);
            }
        }
        setBootFadeColor(0xFFU, 0xFFU, 0xFFU);
        break;
    case 9:
        createCallbackTask(&func_8006A798, 0, 0x64);
        createCallbackTask(&func_8006B108, 0, 0x64);
        createCallbackTask(&func_8006C698, 0, 0x64);
        createCallbackTask(&initRaceCoursePreviewAnimatedBillboards, 0, 0x64);
        createCallbackTaskWithUserId(&func_8006429C, 0, 0x64, 1);
        if ((gRaceSplitscreenMode == 0) && (gMainMenuModeSelection == 0) && ((gTrainingCourseLesson == 0) || (gTrainingCourseLesson == 7) || (gTrainingCourseLesson == 8))) {
            if (sp2C != 0) {
                createCallbackTask(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x37);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x38);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x39);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x3B);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x3C);
                createCallbackTaskWithUserId(&func_800691C8, 0, 0x64, 0x3D);
            }
        }
        if (gRaceSplitscreenMode == 1) {
            if (D_80121B5E == 0) {
                createCallbackTask(&func_80065764, 0, 0x64);
            }
            if (D_80121B5E == 1) {
                createCallbackTask(&func_800666B0, 0, 1);
            }
        }
        setBootFadeColor(0x20U, 0x40U, 0x50U);
        break;
    }
    createCallbackTask(&func_800508D0, 5, 0x6E);
}
