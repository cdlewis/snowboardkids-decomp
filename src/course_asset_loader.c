#include "common.h"
#include "memory_allocator.h"
#include "asset_decompression.h"
#include "course_asset_loader.h"
#include "effect_task_scheduler.h"
#include "game_boot.h"
#include "model_animation.h"
#include "race_course_effects.h"
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
    /* 0x10 */ u8 courseAssetIndex;
    /* 0x11 */ u8 pad11[2];
    /* 0x13 */ s8 isActive;
    /* 0x14 */ u8 pad14[COURSE_PLAYER_STRIDE - 0x14];
} CoursePlayerState;

extern s16 D_80112130[];
extern s16 D_80112140;
extern s16 D_80112144;
extern u16 D_80121B50;
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

extern u8 D_800EC9C2;
extern u8 D_80121B55;
extern u8 D_80121B59;
extern u8 D_80121B5A;
extern u8 D_80121B5B;
extern u8 D_80121B5E;
extern u8 D_8012482B;
extern u8 D_8012482C;

extern void func_80055B34();
extern void func_800560F4();
extern void func_80056C44();
extern void func_8005714C();
#ifdef NON_MATCHING
// func_80043950 best match: 63.023% at nonmatchings/func_80043950-2225551288923588688/base_4.c.
void func_80043950(void) {
    s32 size;

    switch (D_80121B50) {
    case 0:
        size = D_EC9A0 - D_E35C0;
        D_80112140 = func_80042D58(size);
        func_80043060(D_80112140);
        func_80099C44(D_E35C0, func_80043040(D_80112140), size);
        func_800437F0(D_163410, D_171480, 9);
        func_800437F0(D_21D9D0, D_222500, 0x1B);
        func_8007D190();
        func_800437F0(D_1EC0F0, D_1EC4A0, 0x1D);
        break;
    case 1:
        size = D_F6160 - D_EC9A0;
        D_80112140 = func_80042D58(size);
        func_80043060(D_80112140);
        func_80099C44(D_EC9A0, func_80043040(D_80112140), size);
        func_800437F0(D_171480, D_17D220, 9);
        func_800437F0(D_222500, D_2274A0, 0x1B);
        func_8007D190();
        func_800437F0(D_1EC4A0, D_1EC810, 0x1D);
        break;
    case 2:
        size = D_FEDA0 - D_F6160;
        D_80112140 = func_80042D58(size);
        func_80043060(D_80112140);
        func_80099C44(D_F6160, func_80043040(D_80112140), size);
        func_800437F0(D_17D220, D_186910, 9);
        func_800437F0(D_2274A0, D_22AE50, 0x1B);
        func_8007D190();
        func_800437F0(D_1EC810, D_1ECD00, 0x1D);
        break;
    case 3:
        size = D_10AAB0 - D_FEDA0;
        D_80112130[8] = func_80042D58(size);
        func_80043060(D_80112130[8]);
        func_80099C44(D_FEDA0, func_80043040(D_80112140), size);
        func_800437F0(D_186910, D_197490, 9);
        func_800437F0(D_22AE50, D_2308E0, 0x1B);
        func_8007D190();
        func_800437F0(D_1ECD00, D_1ED910, 0x1D);
        break;
    case 4:
        size = D_118670 - D_10AAB0;
        D_80112130[8] = func_80042D58(size);
        func_80043060(D_80112130[8]);
        func_80099C44(D_10AAB0, func_80043040(D_80112140), size);
        func_800437F0(D_197490, D_1A5620, 9);
        func_800437F0(D_2308E0, D_2349E0, 0x1B);
        func_8007D190();
        func_800437F0(D_1ED910, D_1EDC80, 0x1D);
        break;
    case 5:
        size = D_125C90 - D_118670;
        D_80112130[8] = func_80042D58(size);
        func_80043060(D_80112130[8]);
        func_80099C44(D_118670, func_80043040(D_80112140), size);
        func_800437F0(D_1A5620, D_1B3160, 9);
        func_800437F0(D_2349E0, D_23A7D0, 0x1B);
        func_8007D190();
        func_800437F0(D_1EDC80, D_1EE060, 0x1D);
        break;
    case 6:
        size = D_131A90 - D_125C90;
        D_80112130[8] = func_80042D58(size);
        func_80043060(D_80112130[8]);
        func_80099C44(D_125C90, func_80043040(D_80112140), size);
        func_800437F0(D_1B3160, D_1C24B0, 9);
        func_800437F0(D_23A7D0, D_23E9D0, 0x1B);
        func_8007D190();
        func_800437F0(D_1EE060, D_1EEEA0, 0x1D);
        break;
    case 7:
        size = D_1384B0 - D_131A90;
        D_80112130[8] = func_80042D58(size);
        func_80043060(D_80112130[8]);
        func_80099C44(D_131A90, func_80043040(D_80112140), size);
        func_800437F0(D_1C24B0, D_1C9630, 9);
        func_800437F0(D_23E9D0, D_23FCC0, 0x1B);
        func_8007D190();
        break;
    case 8:
        size = D_13F3B0 - D_1384B0;
        D_80112130[8] = func_80042D58(size);
        func_80043060(D_80112130[8]);
        func_80099C44(D_1384B0, func_80043040(D_80112140), size);
        func_800437F0(D_1C9630, D_1D3070, 9);
        func_800437F0(D_23FCC0, D_241160, 0x1B);
        func_8007D190();
        func_800437F0(D_1EEEA0, D_1EF1D0, 0x1D);
        break;
    case 9:
        size = D_145380 - D_13F3B0;
        D_80112130[8] = func_80042D58(size);
        func_80043060(D_80112130[8]);
        func_80099C44(D_13F3B0, func_80043040(D_80112140), size);
        func_800437F0(D_1D3070, D_1D82B0, 9);
        func_800437F0(D_241160, D_2427D0, 0x1B);
        func_8007D190();
        func_800437F0(D_1EF1D0, D_1EF530, 0x1D);
        break;
    }

    size = D_14B450 - D_147910;
    D_80112130[10] = func_80042D58(size);
    func_80043060(D_80112130[10]);
    func_80099C44(D_147910, func_80043040(D_80112144), size);
    func_800437F0(D_1D82B0, D_1DCED0, 0xB);
    func_800437F0(D_1E74E0, D_1EC0F0, 0x1C);
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

void func_80044294(void) {
    s32 sp2C;
    s32 sp28;

    sp28 = (sp2C = 1);
    if (D_80121B55 != 1) {
        if (D_8012482B != 0) {
            sp28 = 0;
        }
        if (D_8012482C != 0) {
            sp2C = 0;
        }
    }
    switch ((u16) D_80121B50) {
    case 0:
        func_80071408(&func_8006A798, 0, 0x64);
        func_80071408(&func_8006B108, 0, 0x64);
        func_80071408(&func_8006C698, 0, 0x64);
        func_80071664(&func_8006429C, 0, 0x64, 0);
        func_80071664(&func_8006429C, 0, 0x64, 1);
        func_80071664(&func_8006CBBC, 0, 0x64, 0);
        if ((D_800EC9C2 == 0) && (D_80121B59 == 0) && (D_80121B5A == 0) && (D_80121B5B == 0)) {
            if (sp2C != 0) {
                func_80071408(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                func_80071664(&func_800691C8, 0, 0x64, 0);
                func_80071664(&func_800691C8, 0, 0x64, 1);
                func_80071664(&func_800691C8, 0, 0x64, 2);
                func_80071664(&func_800691C8, 0, 0x64, 3);
                func_80071664(&func_800691C8, 0, 0x64, 4);
                func_80071664(&func_800691C8, 0, 0x64, 5);
                func_80071664(&func_800691C8, 0, 0x64, 0x3F);
                func_80071664(&func_800691C8, 0, 0x64, 0x4E);
                func_80071664(&func_800691C8, 0, 0x64, 0x4F);
                func_80071664(&func_800691C8, 0, 0x64, 0x50);
                func_80071664(&func_800691C8, 0, 0x64, 0x51);
                func_80071664(&func_800691C8, 0, 0x64, 0x52);
            }
        }
        if (D_800EC9C2 == 1) {
            if (D_80121B5E == 0) {
                func_80071408(&func_80065764, 0, 0x64);
            }
            if (D_80121B5E == 1) {
                func_80071408(&func_800666B0, 0, 1);
            }
        }
        if (D_800EC9C2 == 1) {
            if (D_80121B5E != 1) {
                goto block_24;
            }
        } else {
block_24:
            func_80071408(&func_8006AE00, 0, 0x64);
        }
        func_8009B58C(0x80U, 0xC0U, 0xFFU);
        break;
    case 1:
        func_80071408(&func_8006A798, 0, 0x64);
        func_80071408(&func_8006AE00, 0, 0x64);
        func_8006B760(0x17, 0xF682AF46, 0xEB74F202, 0xF82BAF46, 0xEA17F202);
        func_8006B760(0x17, 0xF7062F46, 0xE897F202, 0xF5C82F46, 0xEA58F202);
        func_8006B760(0x17, 0xF45B2F46, 0xE876F202, 0xF6502F46, 0xE7927202);
        func_8006B760(0x17, 0xF53F2F46, 0xE5987202, 0xF2E4AF46, 0xE61F7202);
        func_8006B760(0x17, 0xF2BDAF46, 0xE548F202, 0xF3AE2F46, 0xE3AF7202);
        func_8006B760(0x17, 0xF25E41BF, 0xE5FB38EE, 0xF3169B57, 0xE56EEC94);
        func_8006B760(0x17, 0xF50D1255, 0xE7032BEC, 0xF2E89861, 0xE70079E8);
        func_8006B760(0x17, 0xF3EFF851, 0xE8BCCFE0, 0xF5650CCB, 0xE9A4FC34);
        func_8006B760(0x17, 0xF6BEB9D1, 0xEA69C4DE, 0xF6F1B095, 0xEBF8B05E);
        func_80071408(&func_8006B108, 0, 0x64);
        func_80071664(&func_80057600, 0, 0x64, 6);
        func_80071664(&func_80057600, 0, 0x64, 7);
        func_80071664(&func_8006D384, 0, 0x64, 0);
        func_80071664(&func_800669A0, 0, 0x64, 0);
        func_80071408(&func_8006C698, 0, 0x64);
        func_80071664(&func_800675AC, 0, 0x64, 0);
        func_80071664(&func_800675AC, 0, 0x64, 1);
        func_80071664(&func_8006429C, 0, 0x64, 1);
        if ((D_800EC9C2 == 0) && (D_80121B5B == 0)) {
            if (sp2C != 0) {
                func_80071408(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                func_80071664(&func_800691C8, 0, 0x64, 6);
                func_80071664(&func_800691C8, 0, 0x64, 7);
                func_80071664(&func_800691C8, 0, 0x64, 8);
                func_80071664(&func_800691C8, 0, 0x64, 9);
                func_80071664(&func_800691C8, 0, 0x64, 0xA);
                func_80071664(&func_800691C8, 0, 0x64, 0xB);
                func_80071664(&func_800691C8, 0, 0x64, 0xC);
                func_80071664(&func_800691C8, 0, 0x64, 0xD);
                func_80071664(&func_800691C8, 0, 0x64, 0x53);
                func_80071664(&func_800691C8, 0, 0x64, 0x54);
                func_80071664(&func_800691C8, 0, 0x64, 0x55);
                func_80071664(&func_800691C8, 0, 0x64, 0x56);
            }
            func_80061AF4(0x59, (void *)0x2290CDA1, (void *)0xC5F82815, 0xAF0);
        }
        if (D_800EC9C2 == 1) {
            if (D_80121B5E == 0) {
                func_80071408(&func_80065764, 0, 0x64);
            }
            if (D_80121B5E == 1) {
                func_80071408(&func_800666B0, 0, 1);
            }
        }
        func_8009B58C(0xFFU, 0x80U, 0U);
        break;
    case 2:
        func_80071408(&func_8006A798, 0, 0x64);
        func_80071408(&func_8006B108, 0, 0x64);
        func_80071408(&func_8006C698, 0, 0x64);
        func_80071664(&func_80057600, 0, 0x64, 2);
        func_80071664(&func_80063980, 0, 0x64, 0);
        func_80071664(&func_80063980, 0, 0x64, 1);
        if ((D_800EC9C2 == 0) && (D_80121B5B == 0)) {
            if (sp2C != 0) {
                func_80071408(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                func_80071664(&func_800691C8, 0, 0x64, 0xE);
                func_80071664(&func_800691C8, 0, 0x64, 0xF);
                func_80071664(&func_800691C8, 0, 0x64, 0x10);
                func_80071664(&func_800691C8, 0, 0x64, 0x11);
                func_80071664(&func_800691C8, 0, 0x64, 0x12);
                func_80071664(&func_800691C8, 0, 0x64, 0x13);
                func_80071664(&func_800691C8, 0, 0x64, 0x14);
                func_80071664(&func_800691C8, 0, 0x64, 0x42);
                func_80071664(&func_800691C8, 0, 0x64, 0x57);
                func_80071664(&func_800691C8, 0, 0x64, 0x58);
                func_80071664(&func_800691C8, 0, 0x64, 0x59);
            }
        }
        func_8009B58C(0U, 0U, 0x32U);
        break;
    case 3:
        func_80071408(&func_8006A798, 0, 0x64);
        func_80071408(&func_8006B108, 0, 0x64);
        func_80071408(&func_8006C698, 0, 0x64);
        func_80071664(&func_8006CBBC, 0, 0x64, 1);
        func_80071664(&func_8006CBBC, 0, 0x64, 2);
        func_80071664(&func_8006429C, 0, 0x64, 1);
        func_80071408(&func_80055B34, 0, 0x64);
        if ((D_800EC9C2 == 0) && (D_80121B5B == 0)) {
            if (sp2C != 0) {
                func_80071408(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                func_80071664(&func_800691C8, 0, 0x64, 0x15);
                func_80071664(&func_800691C8, 0, 0x64, 0x16);
                func_80071664(&func_800691C8, 0, 0x64, 0x17);
                func_80071664(&func_800691C8, 0, 0x64, 0x18);
                func_80071664(&func_800691C8, 0, 0x64, 0x19);
                func_80071664(&func_800691C8, 0, 0x64, 0x1A);
                func_80071664(&func_800691C8, 0, 0x64, 0x1B);
                func_80071664(&func_800691C8, 0, 0x64, 0x1C);
                func_80071664(&func_800691C8, 0, 0x64, 0x1D);
                func_80071664(&func_800691C8, 0, 0x64, 0x43);
                func_80071664(&func_800691C8, 0, 0x64, 0x44);
                func_80071664(&func_800691C8, 0, 0x64, 0x45);
                func_80071664(&func_800691C8, 0, 0x64, 0x5A);
                func_80071664(&func_800691C8, 0, 0x64, 0x5B);
                func_80071664(&func_800691C8, 0, 0x64, 0x5C);
                func_80071664(&func_800691C8, 0, 0x64, 0x5D);
            }
        }
        if (D_80121B55 == 1) {
            func_80071408(&func_80056C44, 0, 0x64);
        }
        func_8009B58C(0x82U, 0xAFU, 0xFFU);
        break;
    case 4:
        func_80071408(&func_8006A798, 0, 0x64);
        func_80071408(&func_8006B108, 0, 0x64);
        func_80071408(&func_8006C698, 0, 0x64);
        func_80071664(&func_80061428, 0, 0x64, 0);
        func_80071664(&func_800631B0, 0, 0x64, 0);
        func_80071664(&func_80063470, 0, 0x64, 0);
        func_80071664(&func_80057600, 0, 0x64, 4);
        func_80071664(&func_80057600, 0, 0x64, 5);
        func_80071664(&func_8006429C, 0, 0x64, 1);
        if ((D_800EC9C2 == 0) && (D_80121B5B == 0)) {
            if (sp2C != 0) {
                func_80071408(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                func_80071664(&func_800691C8, 0, 0x64, 0x1E);
                func_80071664(&func_800691C8, 0, 0x64, 0x1F);
                func_80071664(&func_800691C8, 0, 0x64, 0x20);
                func_80071664(&func_800691C8, 0, 0x64, 0x21);
                func_80071664(&func_800691C8, 0, 0x64, 0x22);
                func_80071664(&func_800691C8, 0, 0x64, 0x23);
                func_80071664(&func_800691C8, 0, 0x64, 0x24);
                func_80071664(&func_800691C8, 0, 0x64, 0x25);
                func_80071664(&func_800691C8, 0, 0x64, 0x26);
                func_80071664(&func_800691C8, 0, 0x64, 0x5E);
                func_80071664(&func_800691C8, 0, 0x64, 0x5F);
                func_80071664(&func_800691C8, 0, 0x64, 0x60);
            }
        }
        func_80071664(&func_80062ED4, 0, 0x64, 0);
        func_80071664(&func_80062ED4, 0, 0x64, 1);
        func_80071664(&func_80062ED4, 0, 0x64, 2);
        func_80071664(&func_80062ED4, 0, 0x64, 3);
        func_80071664(&func_80062ED4, 0, 0x64, 4);
        func_80071664(&func_80062ED4, 0, 0x64, 5);
        func_80071664(&func_80062ED4, 0, 0x64, 6);
        func_80071664(&func_80062ED4, 0, 0x64, 7);
        func_80071664(&func_80062ED4, 0, 0x64, 8);
        func_80071664(&func_80062ED4, 0, 0x64, 9);
        func_8009B58C(0U, 0U, 0x40U);
        break;
    case 5:
        func_80071408(&func_8006A798, 0, 0x64);
        func_80071664(&func_80057600, 0, 0x64, 0);
        func_80071664(&func_80057600, 0, 0x64, 1);
        func_80071408(&func_8006B108, 0, 0x64);
        func_80071664(&func_8006429C, 0, 0x64, 1);
        func_80071408(&func_8006C698, 0, 0x64);
        if ((D_800EC9C2 == 0) && (D_80121B5B == 0)) {
            if (sp2C != 0) {
                func_80071408(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                func_80071664(&func_800691C8, 0, 0x64, 0x27);
                func_80071664(&func_800691C8, 0, 0x64, 0x28);
                func_80071664(&func_800691C8, 0, 0x64, 0x29);
                func_80071664(&func_800691C8, 0, 0x64, 0x2A);
                func_80071664(&func_800691C8, 0, 0x64, 0x2B);
                func_80071664(&func_800691C8, 0, 0x64, 0x2C);
                func_80071664(&func_800691C8, 0, 0x64, 0x2D);
                func_80071664(&func_800691C8, 0, 0x64, 0x2E);
                func_80071664(&func_800691C8, 0, 0x64, 0x40);
                func_80071664(&func_800691C8, 0, 0x64, 0x41);
                func_80071664(&func_800691C8, 0, 0x64, 0x61);
                func_80071664(&func_800691C8, 0, 0x64, 0x62);
                func_80071664(&func_800691C8, 0, 0x64, 0x63);
                func_80071664(&func_800691C8, 0, 0x64, 0x64);
                func_80071664(&func_800691C8, 0, 0x64, 0x65);
                func_80071664(&func_800691C8, 0, 0x64, 0x66);
            }
        }
        func_8009B58C(0xF0U, 0xE6U, 0xBEU);
        break;
    case 6:
        func_80071408(&func_8006A798, 0, 0x64);
        func_80071408(&func_80063FC0, 0, 1);
        func_80071408(&func_8006B108, 0, 0x64);
        func_80071408(&func_8006C698, 0, 0x64);
        func_80071408(&func_80055B34, 0, 0x64);
        func_80071664(&func_800560F4, 0, 0x64, 0);
        func_80071664(&func_800560F4, 0, 0x64, 1);
        func_80071664(&func_800560F4, 0, 0x64, 2);
        func_80071664(&func_8006429C, 0, 0x64, 1);
        if ((D_800EC9C2 == 0) && (D_80121B5B == 0)) {
            if (sp2C != 0) {
                func_80071408(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                func_80071664(&func_800691C8, 0, 0x64, 0x2F);
                func_80071664(&func_800691C8, 0, 0x64, 0x30);
                func_80071664(&func_800691C8, 0, 0x64, 0x31);
                func_80071664(&func_800691C8, 0, 0x64, 0x32);
                func_80071664(&func_800691C8, 0, 0x64, 0x33);
                func_80071664(&func_800691C8, 0, 0x64, 0x34);
                func_80071664(&func_800691C8, 0, 0x64, 0x35);
                func_80071664(&func_800691C8, 0, 0x64, 0x36);
                func_80071664(&func_800691C8, 0, 0x64, 0x67);
                func_80071664(&func_800691C8, 0, 0x64, 0x68);
                func_80071664(&func_800691C8, 0, 0x64, 0x69);
                func_80071664(&func_800691C8, 0, 0x64, 0x6A);
            }
        }
        if (D_80121B55 == 1) {
            func_80071408(&func_80056C44, 0, 0x64);
        }
        func_8009B5C0(0xFFU, 0xFFU, 0xFFU);
        break;
    case 7:
        func_80071408(&func_8006A798, 0, 0x64);
        func_80071664(&func_80057600, 0, 0x64, 3);
        func_80071664(&func_8006D384, 0, 0x64, 1);
        func_80071408(&func_8006B108, 0, 0x64);
        func_8009B58C(0xFFU, 0xFFU, 0xFFU);
        break;
    case 8:
        func_80071408(&func_8006A798, 0, 0x64);
        func_80071408(&func_8006B108, 0, 0x64);
        func_80071408(&func_8006C698, 0, 0x64);
        func_80071664(&func_8006429C, 0, 0x64, 1);
        if ((D_800EC9C2 == 0) && (D_80121B5B == 0)) {
            if (sp2C != 0) {
                func_80071408(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                func_80071664(&func_800691C8, 0, 0x64, 0x46);
                func_80071664(&func_800691C8, 0, 0x64, 0x47);
                func_80071664(&func_800691C8, 0, 0x64, 0x48);
                func_80071664(&func_800691C8, 0, 0x64, 0x49);
                func_80071664(&func_800691C8, 0, 0x64, 0x4A);
                func_80071664(&func_800691C8, 0, 0x64, 0x4B);
            }
        }
        func_8009B58C(0xFFU, 0xFFU, 0xFFU);
        break;
    case 9:
        func_80071408(&func_8006A798, 0, 0x64);
        func_80071408(&func_8006B108, 0, 0x64);
        func_80071408(&func_8006C698, 0, 0x64);
        func_80071408(&func_8005714C, 0, 0x64);
        func_80071664(&func_8006429C, 0, 0x64, 1);
        if ((D_800EC9C2 == 0) && (D_80121B5B == 0) && ((D_80121B5A == 0) || (D_80121B5A == 7) || (D_80121B5A == 8))) {
            if (sp2C != 0) {
                func_80071408(&func_8006713C, 0, 0x64);
            }
            if (sp28 != 0) {
                func_80071664(&func_800691C8, 0, 0x64, 0x37);
                func_80071664(&func_800691C8, 0, 0x64, 0x38);
                func_80071664(&func_800691C8, 0, 0x64, 0x39);
                func_80071664(&func_800691C8, 0, 0x64, 0x3B);
                func_80071664(&func_800691C8, 0, 0x64, 0x3C);
                func_80071664(&func_800691C8, 0, 0x64, 0x3D);
            }
        }
        if (D_800EC9C2 == 1) {
            if (D_80121B5E == 0) {
                func_80071408(&func_80065764, 0, 0x64);
            }
            if (D_80121B5E == 1) {
                func_80071408(&func_800666B0, 0, 1);
            }
        }
        func_8009B58C(0x20U, 0x40U, 0x50U);
        break;
    }
    func_80071408(&func_800508D0, 5, 0x6E);
}
