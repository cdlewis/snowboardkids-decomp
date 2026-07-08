#include "common.h"
#include "memory_allocator.h"
#include "asset_decompression.h"
#include "course_asset_loader.h"
#include "game_boot.h"

#define COURSE_COUNT 10
#define COURSE_PLAYER_COUNT 4
#define COURSE_PLAYER_STRIDE 0x60C

typedef struct {
    /* 0x0 */ u8 *start;
    /* 0x4 */ u8 *end;
} RomAssetRange;

typedef struct {
    /* 0x0 */ RomAssetRange courseGeometry;
    /* 0x8 */ RomAssetRange courseModel;
    /* 0x10 */ RomAssetRange courseTexture;
    /* 0x18 */ RomAssetRange courseAnimation;
} CourseAssetGroup;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u8 courseAssetIndex;
    /* 0x11 */ u8 pad11[2];
    /* 0x13 */ s8 isActive;
    /* 0x14 */ u8 pad14[COURSE_PLAYER_STRIDE - 0x14];
} CoursePlayerState;

extern s16 D_80112130[];
extern s16 D_80112140;
extern s16 D_80112144;
extern s16 D_80121B50;
extern CoursePlayerState D_80121D80[];
extern RomAssetRange D_800D4020[];
extern RomAssetRange D_800D4050[];
extern RomAssetRange D_800D4080[];

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

extern void func_8007D190(void);

#ifdef NON_MATCHING
static CourseAssetGroup sCourseAssets[COURSE_COUNT] = {
    { { D_E35C0, D_EC9A0 }, { D_163410, D_171480 }, { D_21D9D0, D_222500 }, { NULL, NULL } },
    { { D_EC9A0, D_F6160 }, { D_171480, D_17D220 }, { D_222500, D_2274A0 }, { NULL, NULL } },
    { { D_F6160, D_FEDA0 }, { D_17D220, D_186910 }, { D_2274A0, D_22AE50 }, { NULL, NULL } },
    { { D_FEDA0, D_10AAB0 }, { D_186910, D_197490 }, { D_22AE50, D_2308E0 }, { NULL, NULL } },
    { { D_10AAB0, D_118670 }, { D_197490, D_1A5620 }, { D_2308E0, D_2349E0 }, { NULL, NULL } },
    { { D_118670, D_125C90 }, { D_1A5620, D_1B3160 }, { D_2349E0, D_23A7D0 }, { NULL, NULL } },
    { { D_125C90, D_131A90 }, { D_1B3160, D_1C24B0 }, { D_23A7D0, D_23E9D0 }, { NULL, NULL } },
    { { D_131A90, D_1384B0 }, { D_1C24B0, D_1C9630 }, { D_23E9D0, D_23FCC0 }, { NULL, NULL } },
    { { D_1384B0, D_13F3B0 }, { D_1C9630, D_1D3070 }, { D_23FCC0, D_241160 }, { NULL, NULL } },
    { { D_13F3B0, D_145380 }, { D_1D3070, D_1D82B0 }, { D_241160, D_2427D0 }, { NULL, NULL } },
};

static void loadRawCourseAsset(RomAssetRange *range) {
    s32 size;

    size = range->end - range->start;
    D_80112140 = func_80042D58(size);
    func_80043060(D_80112140);
    func_80099C44(range->start, func_80043040(D_80112140), size);
}

// func_80043950 best match: not matched, typed rewrite documents the asset table structure.
void func_80043950(void) {
    CourseAssetGroup *courseAssets;
    s32 size;

    if ((u16)D_80121B50 < COURSE_COUNT) {
        courseAssets = &sCourseAssets[D_80121B50];
        loadRawCourseAsset(&courseAssets->courseGeometry);
        func_800437F0(courseAssets->courseModel.start, courseAssets->courseModel.end, 9);
        func_800437F0(courseAssets->courseTexture.start, courseAssets->courseTexture.end, 0x1B);
        func_8007D190();
    }

    size = D_14B450 - D_147910;
    D_80112144 = func_80042D58(size);
    func_80043060(D_80112144);
    func_80099C44(D_147910, func_80043040(D_80112144), size);
    func_800437F0(D_1D82B0, D_1DCED0, 0xB);
    func_800437F0(D_1E74E0, D_1EF530, 0x1C);
    func_800437F0(D_5DAF30, D_5DB9D0, 0x2A);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/course_asset_loader/func_80043950.s")
#endif

// func_800440F4 best match: 98.654% at nonmatchings/func_800440F4-5635509610426229442/base_6.c.
#ifdef NON_MATCHING
void func_800440F4(void) {
    CoursePlayerState *player;
    RomAssetRange *rawRanges;
    RomAssetRange *textureRanges;
    RomAssetRange *modelRanges;
    s16 *assetHandles;
    s32 i;
    s32 size;

    size = D_1502A0 - D_14B450;
    D_80112130[0xC] = func_80042D58(size);
    func_80099C44(D_14B450, func_80043040(D_80112130[0xC]), size);
    func_800437F0(D_1EF530, D_1F1A90, 0xD);

    rawRanges = D_800D4020;
    textureRanges = D_800D4050;
    modelRanges = D_800D4080;
    for (player = D_80121D80, i = 0; i != COURSE_PLAYER_COUNT; i++, player++) {
        assetHandles = &D_80112130[i];
        if (player->isActive != 0) {
            size = rawRanges[player->courseAssetIndex].end - rawRanges[player->courseAssetIndex].start;
            assetHandles[0xE] = func_80042D58(size);
            func_80099C44(rawRanges[player->courseAssetIndex].start, func_80043040(assetHandles[0xE]), size);
            func_800437F0(textureRanges[player->courseAssetIndex].start, textureRanges[player->courseAssetIndex].end, i + 0x12);
            func_800437F0(modelRanges[player->courseAssetIndex].start, modelRanges[player->courseAssetIndex].end, i + 0x16);
        }
    }

    func_800437F0(D_243270, D_245A80, 0x1E);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/course_asset_loader/func_800440F4.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/course_asset_loader/func_80044294.s")
