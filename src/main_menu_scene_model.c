#include "main_menu_scene_model.h"
#include "memory_allocator.h"
#include "asset_decompression.h"

/* Frame offsets are halfword-relative to the bank start; this form preserves target addu order. */
#define MAIN_MENU_ANIMATION_FRAME_DATA(bank, index) \
    ((s16 *)(((bank)->frameOffsets[(index)] * sizeof(s16)) + (s32)(bank)))
#define MAIN_MENU_MODEL_ASSET_RANGE_WORDS 2
#define MAIN_MENU_MODEL_ASSET_RANGE_START(table, index) ((table)[(index) * MAIN_MENU_MODEL_ASSET_RANGE_WORDS])
#define MAIN_MENU_MODEL_ASSET_RANGE_END(table, index) ((table)[((index) * MAIN_MENU_MODEL_ASSET_RANGE_WORDS) + 1])

extern s16 D_8011218A[];
extern s16 D_801121AE;
extern u8 D_5E34A0;
extern u8 D_608560;
extern u8 D_215BE0;
extern u8 D_21D9D0;

typedef s32 RomAssetAddress;

typedef struct MainMenuModelAssetHandles {
    u8 pad0[0x5A];
    /* 0x5A */ s16 modelInstanceHandles[6];
    /* 0x66 */ s16 modelAssetSlots[6];
    /* 0x72 */ s16 animationAssetSlots[6];
} MainMenuModelAssetHandles;

typedef struct MainMenuAnimationWritePart {
    s32 word0;
    s32 word4;
    s32 word8;
    s32 wordC;
    s32 word10;
} MainMenuAnimationWritePart;

extern RomAssetAddress D_800D4020[];
extern RomAssetAddress D_800D4050[];
extern MainMenuModelAssetHandles D_80112130;
extern void func_80042AB4(MainMenuSceneModel *arg0);

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_model/func_80040C80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_model/func_80040D94.s")

// func_80041A20 best match: 48.898% (nonmatchings/func_80041A20-3236181511606361864/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_model/func_80041A20.s")

#ifdef NON_MATCHING
extern u16 D_800ECC24;
extern u16 D_800ECC28;
extern u16 D_800ECC2C;
extern u16 D_800ECC30;
extern u16 D_800ECC34;
extern u16 D_800ECC38;
extern u16 D_800ECC3C;
extern u16 D_800ECC40;
extern u16 D_800ECC44;
extern u16 D_800ECC46;
extern u8 D_8010B200[];
extern s16 D_8011213E;
extern u16 D_80121B50;

void func_80041A20(void) {
    u16 *srcBase = &D_800ECC46;
    u16 *src = srcBase;
    u8 *dst;
    u8 *model;
    s32 outPos = 0;
    s32 count;
    s32 copied;
    u16 code;
    s32 length;
    s32 offset;
    s32 remainder;

    switch (D_80121B50) {
    case 0:
        src = (u16 *)((u8 *)srcBase + (D_800ECC24 * 2));
        break;
    case 1:
        src = (u16 *)((u8 *)srcBase + (D_800ECC28 * 2));
        break;
    case 2:
        src = (u16 *)((u8 *)srcBase + (D_800ECC2C * 2));
        break;
    case 3:
        src = (u16 *)((u8 *)srcBase + (D_800ECC30 * 2));
        break;
    case 4:
        src = (u16 *)((u8 *)srcBase + (D_800ECC34 * 2));
        break;
    case 5:
        src = (u16 *)((u8 *)srcBase + (D_800ECC38 * 2));
        break;
    case 6:
        src = (u16 *)((u8 *)srcBase + (D_800ECC3C * 2));
        break;
    case 8:
        src = (u16 *)((u8 *)srcBase + (D_800ECC40 * 2));
        break;
    case 9:
        src = (u16 *)((u8 *)srcBase + (D_800ECC44 * 2));
        break;
    }

    dst = D_8010B200;
    count = *src++;
    while (outPos < count) {
        copied = 0;
        code = *src;
        length = (code >> 10) & 0x3F;
        if (length == 0) {
            dst[outPos] = code;
            outPos++;
            src++;
        } else {
            offset = outPos - (code & 0x3FF);
            if (length > 0) {
                remainder = length & 3;
                if (remainder != 0) {
                    u8 *copy = offset + dst;
                    do {
                        dst[outPos] = copy[0];
                        copied++;
                        outPos++;
                        copy++;
                    } while (remainder != copied);
                    if (copied == length) {
                        src++;
                        continue;
                    }
                }
                {
                    u8 *copy = offset + copied + dst;
                    do {
                        dst[outPos] = copy[0];
                        outPos++;
                        dst[outPos] = copy[1];
                        outPos++;
                        dst[outPos] = copy[2];
                        outPos++;
                        dst[outPos] = copy[3];
                        copied += 4;
                        outPos++;
                        copy += 4;
                    } while (copied != length);
                }
            }
            src++;
        }
    }

    model = (u8 *)func_80043040(D_8011213E);
    *(s32 *)&model[0] = 0;
    *(s32 *)&model[4] = *(s16 *)&dst[0];
    model[8] = 1;
    model[9] = D_80121B50;
    model[10] = *(s8 *)&dst[2];
    model[12] = 0;
    model[11] = *(s8 *)&dst[3];

    if (*(s16 *)&dst[0] > 0) {
        s32 i = 0;
        u8 *read = D_8010B200;
        u8 *write = model;

        do {
            i++;
            write[0xD] = *(s8 *)&read[4];
            write[0x11A1] = *(s8 *)&read[5];
            read += 3;
            write++;
            write[0x2334] = read[3];
        } while (i < *(s32 *)&model[4]);
    }
}
#endif

void func_80041CC0(void) {
    func_800437F0(&D_5E34A0, &D_608560, 0x3F);
}

void func_80041CF0(void) {
    func_800437F0(&D_215BE0, &D_21D9D0, 0x3F);
}

void func_80041D20(s32 actorIndex, s32 modelIndex) {
    MainMenuSceneModel *model;

    func_800438EC(MAIN_MENU_MODEL_ASSET_RANGE_START(D_800D4020, modelIndex),
                  MAIN_MENU_MODEL_ASSET_RANGE_END(D_800D4020, modelIndex), actorIndex + 0x33);
    func_800437F0((void *)MAIN_MENU_MODEL_ASSET_RANGE_START(D_800D4050, modelIndex),
                  (void *)MAIN_MENU_MODEL_ASSET_RANGE_END(D_800D4050, modelIndex), actorIndex + 0x39);
    D_80112130.modelInstanceHandles[actorIndex] = func_80042D58(sizeof(MainMenuSceneModel));
    model = (MainMenuSceneModel *)func_80043040(D_80112130.modelInstanceHandles[actorIndex]);
    model->actorIndex = actorIndex;
    model->modelIndex = modelIndex;
    func_80042AB4(model);
}

void func_80041DD4(s32 modelIndex, s32 animationIndex) {
    MainMenuModelAnimationBank *animationBank;
    s16 *frameData;
    MainMenuSceneModel *model;
    s16 frameDuration;

    animationBank = (MainMenuModelAnimationBank *)func_80043040(D_801121AE);
    frameData = MAIN_MENU_ANIMATION_FRAME_DATA(animationBank, animationIndex);
    model = (MainMenuSceneModel *)func_80043040(D_8011218A[modelIndex]);
    frameDuration = *frameData++;
    model->framesRemaining = frameDuration;
    model->animationStart = frameData;
    model->framesRemaining++;
    model->animationCursor = frameData;
    model->frameDuration = frameDuration;
}

MainMenuSceneModel *func_80041E60(s32 modelIndex) {
    return (MainMenuSceneModel *)func_80043040(D_8011218A[modelIndex]);
}

void func_80041E90(MainMenuSceneModel *model) {
    s16 *cursor;
    s32 i;
    MainMenuAnimationWritePart *writePart;
    MainMenuModelPart *part;

    cursor = model->animationCursor;
    i = 0;
    writePart = (MainMenuAnimationWritePart *)model;
    do {
        i++;
        writePart[1].word10 = cursor[0] << 11;
        writePart[2].word0 = cursor[1] << 11;
        writePart[2].word4 = cursor[2] << 11;
        writePart++;
        cursor += 3;
    } while (i < 14);

    for (i = 0; i < 14; i++) {
        part = &model->parts[i];
        part->rot.x = *cursor++;
        part->rot.y = *cursor++;
        part->rot.z = *cursor++;
    }

    model->animationCursor = cursor;
}

s32 func_80041FB4(s32 arg0) {
    int new_var2;
    MainMenuSceneModel *new_var;
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)func_80043040(D_8011218A[arg0]);
    if (model->framesRemaining == 1) {
        goto ret1_initial;
    }
    new_var = model;
    new_var2 = 0 & 0xFFu;
    model->framesRemaining--;
    func_80041E90(model);
    if (1 != new_var->framesRemaining) {
        goto ret0;
    }
    goto ret1_after;

ret1_after:
    return 1;

ret1_initial:
    return 1;

ret0:
    return new_var2;
}

void func_80042034(s32 modelIndex) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)func_80043040(D_8011218A[modelIndex]);
    model->framesRemaining--;
    if (model->framesRemaining <= 0) {
        model->framesRemaining = model->frameDuration;
        model->animationCursor = model->animationStart;
    }
    func_80041E90(model);
}

void func_8004209C(s32 modelIndex, s32 x, s32 y, s32 z) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)func_80043040(D_8011218A[modelIndex]);
    model->pos.x = x;
    model->pos.y = y;
    model->pos.z = z;
}

void func_800420FC(s32 modelIndex, s16 x, s16 y, s16 z) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)func_80043040(D_8011218A[modelIndex]);
    model->rot.x = x;
    model->rot.y = y;
    model->rot.z = z;
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_model/func_8004215C.s")
