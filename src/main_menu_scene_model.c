#include "main_menu_scene_model.h"
#include "memory_allocator.h"
#include "asset_manager.h"
#include "fixed_point_matrix.h"

/* Frame offsets are halfword-relative to the bank start; this form preserves target addu order. */
#define MAIN_MENU_ANIMATION_FRAME_DATA(bank, index) \
    ((s16 *)(((bank)->frameOffsets[(index)] * sizeof(s16)) + (s32)(bank)))
#define FIXED_MATRIX_ONE 0x1000
#define MAIN_MENU_MODEL_ASSET_RANGE_WORDS 2
#define MAIN_MENU_MODEL_ASSET_RANGE_START(table, index) ((table)[(index) * MAIN_MENU_MODEL_ASSET_RANGE_WORDS])
#define MAIN_MENU_MODEL_ASSET_RANGE_END(table, index) ((table)[((index) * MAIN_MENU_MODEL_ASSET_RANGE_WORDS) + 1])

extern s16 D_8011218A[];
extern s16 D_801121AE;
extern u8 gMainMenuSceneModelAssetStart;
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

extern RomAssetAddress gCharacterRawAssetRanges[];
extern RomAssetAddress gCharacterTextureAssetRanges[];
extern MainMenuModelAssetHandles D_80112130;

// func_80040C80 best match: 98.116% (nonmatchings/func_80040C80-6113366811127043669/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_model/func_80040C80.s")

#ifdef NON_MATCHING
s32 func_80040C80(u8 *src, s32 srcLen, s16 *dst) {
    s16 *out;
    s32 srcPos;
    s32 outCount;
    s32 remaining;
    s32 bestDistance;
    s32 bestLength;
    s32 distance;
    s32 maxLength;
    s32 searchPos;
    s32 length;
    s32 count;
    u8 *cur;
    u8 *prev;

    srcPos = 0;
    *dst = srcLen;
    outCount = 1;
    remaining = srcLen;
    bestDistance = 0;
    bestLength = 0;
    distance = 1;
    out = &dst[1];

loop:
    maxLength = remaining;
    if (remaining >= 0x40) {
        maxLength = 0x3F;
    }

    searchPos = srcPos - 1;
search_loop:
    if (searchPos < 0) {
        goto search_done;
    }

    length = 0;
    if (maxLength > 0) {
        count = 0;
        cur = src;
        cur += srcPos;
        prev = src;
        prev += searchPos;
match_loop:
        count += 1;
        if (*cur != *prev) {
            goto compare_best;
        }
        cur += 1;
        prev += 1;
        length += 1;
        if (count != maxLength) {
            goto match_loop;
        }
    }

compare_best:
    if (bestLength < length) {
        bestDistance = distance;
        bestLength = length;
    }

    distance += 1;
    searchPos -= 1;
    if (distance < 0x400) {
        goto search_loop;
    }

search_done:
    distance = 1;
    if (bestLength <= 0) {
        *out = src[srcPos];
        out += 1;
        outCount += 1;
        srcPos += 1;
    } else {
        *out = (bestLength << 10) | bestDistance;
        out += 1;
        outCount += 1;
        srcPos += bestLength;
    }

    if (srcLen != srcPos) {
        remaining = srcLen - srcPos;
        if (outCount >= 0x1000) {
            return -1;
        }
        bestDistance = 0;
        distance = 1;
        bestLength = 0;
        goto loop;
    }
    return outCount;
}
#endif

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
    loadCompressedRomAsset(&gMainMenuSceneModelAssetStart, &D_608560, 0x3F);
}

void func_80041CF0(void) {
    loadCompressedRomAsset(&D_215BE0, &D_21D9D0, 0x3F);
}

void func_80041D20(s32 actorIndex, s32 modelIndex) {
    MainMenuSceneModel *model;

    loadRawRomAsset(MAIN_MENU_MODEL_ASSET_RANGE_START(gCharacterRawAssetRanges, modelIndex),
                  MAIN_MENU_MODEL_ASSET_RANGE_END(gCharacterRawAssetRanges, modelIndex), actorIndex + 0x33);
    loadCompressedRomAsset((void *)MAIN_MENU_MODEL_ASSET_RANGE_START(gCharacterTextureAssetRanges, modelIndex),
                  (void *)MAIN_MENU_MODEL_ASSET_RANGE_END(gCharacterTextureAssetRanges, modelIndex), actorIndex + 0x39);
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

// func_8004215C best match: 83.677% (nonmatchings/func_8004215C-7273315160691878794/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_model/func_8004215C.s")

#ifdef NON_MATCHING
void func_8004215C(MainMenuSceneModel *model) {
    s16 partMatrices[14][16];
    s16 rootMatrix[16];
    MainMenuModelPart *part;
    u8 *partCursor;
    s16 (*partMatrix)[16];
    s16 (*partMatrixEnd)[16];
    s32 sineX;
    s32 cosineX;
    s32 sineY;
    s32 cosineY;
    s32 sineZ;
    s32 cosineZ;
    s32 negSineY;
    s32 negSineZ;
    s32 sineXTimesSineY;
    s32 cosineXTimesSineY;
    s16 *combinedMatrixBase;
    s16 *combinedMatrix;
    s16 *localMatrix;
    s16 *rootMatrixRow;
    s32 rowOffset;
    s32 localIndex;
    s16 *localAxis;
    s16 *combinedAxis;
    s16 *rootAxis;
    s32 dot;

    partCursor = (u8 *)model;
    partMatrix = partMatrices;
    partMatrixEnd = &partMatrices[14];
    do {
        sineX = func_80097AE8(*(s16 *)(partCursor + 0x1E));
        cosineX = func_80097B48(*(s16 *)(partCursor + 0x1E));
        sineY = func_80097AE8(*(s16 *)(partCursor + 0x20));
        cosineY = func_80097B48(*(s16 *)(partCursor + 0x20));
        sineZ = func_80097AE8(*(s16 *)(partCursor + 0x22));
        cosineZ = func_80097B48(*(s16 *)(partCursor + 0x22));
        negSineY = -sineY;
        negSineZ = -sineZ;

        (*partMatrix)[0] = (cosineY * cosineZ) / FIXED_MATRIX_ONE;
        (*partMatrix)[1] = (cosineY * sineZ) / FIXED_MATRIX_ONE;
        sineXTimesSineY = (sineX * sineY) / FIXED_MATRIX_ONE;
        (*partMatrix)[2] = negSineY;
        (*partMatrix)[3] = ((sineXTimesSineY * cosineZ) / FIXED_MATRIX_ONE) + ((cosineX * negSineZ) / FIXED_MATRIX_ONE);
        (*partMatrix)[4] = ((sineXTimesSineY * sineZ) / FIXED_MATRIX_ONE) + ((cosineX * cosineZ) / FIXED_MATRIX_ONE);
        (*partMatrix)[5] = (sineX * cosineY) / FIXED_MATRIX_ONE;
        cosineXTimesSineY = (cosineX * sineY) / FIXED_MATRIX_ONE;
        (*partMatrix)[6] = ((cosineXTimesSineY * cosineZ) / FIXED_MATRIX_ONE) + ((sineX * sineZ) / FIXED_MATRIX_ONE);
        (*partMatrix)[7] = ((cosineXTimesSineY * sineZ) / FIXED_MATRIX_ONE) + (((-sineX) * cosineZ) / FIXED_MATRIX_ONE);
        (*partMatrix)[8] = (cosineX * cosineY) / FIXED_MATRIX_ONE;
        partMatrix++;
        partCursor += 0x14;
    } while (partMatrix != partMatrixEnd);

    func_800981C8(rootMatrix, model->rot.x, model->rot.y, model->rot.z);
    *(s32 *)&rootMatrix[10] = model->pos.x;
    *(s32 *)&rootMatrix[12] = model->pos.y;
    *(s32 *)&rootMatrix[14] = model->pos.z;

    partMatrix = partMatrices;
    combinedMatrixBase = &model->unk146;
    do {
        combinedMatrix = combinedMatrixBase;
        rowOffset = 0;
        localMatrix = *partMatrix;
        do {
            rootMatrixRow = rootMatrix;
            do {
                dot = (rootMatrixRow[6] * localMatrix[2]) + (rootMatrixRow[0] * localMatrix[0]) +
                      (rootMatrixRow[2] * localMatrix[1]);
                *combinedMatrix++ = dot / FIXED_MATRIX_ONE;
                rootMatrixRow++;
            } while (rootMatrixRow != &rootMatrix[3]);
            rowOffset += 6;
            localMatrix += 3;
        } while (rowOffset != 0x12);
        partMatrix++;
        combinedMatrixBase += 0x10;
    } while (partMatrix < partMatrixEnd);

    part = model->parts;
    partCursor = (u8 *)model;
    combinedMatrixBase = &model->unk146;
    localIndex = 0;
    do {
        rootAxis = &rootMatrix[10];
        localAxis = rootMatrix;
        combinedAxis = combinedMatrixBase;
        do {
            dot = (((s64)localAxis[3] * *(s32 *)(partCursor + 0x28)) +
                   ((s64)localAxis[0] * *(s32 *)(partCursor + 0x24)) +
                   ((s64)localAxis[6] * *(s32 *)(partCursor + 0x2C))) / FIXED_MATRIX_ONE;
            *(s32 *)&combinedAxis[11] = dot;
            *(s32 *)&combinedAxis[11] += *(s32 *)rootAxis;
            rootAxis += 2;
            localAxis++;
            combinedAxis += 2;
        } while (rootAxis != &rootMatrix[16]);
        localIndex += 0x14;
        combinedMatrixBase += 0x10;
        partCursor += 0x14;
        part++;
    } while (localIndex != 0x118);
}
#endif
