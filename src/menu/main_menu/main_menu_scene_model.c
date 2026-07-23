#include "assets.h"
#include "game/menu/main_menu/main_menu_scene_model.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/asset_manager.h"
#include "game/math/fixed_point_math.h"

/* Frame offsets are halfword-relative to the bank start; this form preserves target addu order. */
#define MAIN_MENU_ANIMATION_FRAME_DATA(bank, index) \
    ((s16 *)(((bank)->frameOffsets[(index)] * sizeof(s16)) + (s32)(bank)))
#define FIXED_MATRIX_ONE 0x1000
#define MAIN_MENU_MODEL_ASSET_RANGE_WORDS 2
#define MAIN_MENU_MODEL_ASSET_RANGE_START(table, index) ((table)[(index) * MAIN_MENU_MODEL_ASSET_RANGE_WORDS])
#define MAIN_MENU_MODEL_ASSET_RANGE_END(table, index) ((table)[((index) * MAIN_MENU_MODEL_ASSET_RANGE_WORDS) + 1])
#define MAIN_MENU_SCENE_MODEL_PART_COUNT 14
#define MAIN_MENU_SCENE_MODEL_MATRIX_AXES 3
#define MAIN_MENU_SCENE_MODEL_MATRIX_ELEMENTS 9


typedef s32 RomAssetAddress;

typedef struct MainMenuModelAssetHandles {
    u8 pad0[0x40];
    /* 0x40 */ s16 raceRecordReplaySaveBufferHandle;
    u8 pad42[0x5A - 0x42];
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

typedef s16 MainMenuFixedMatrix3x3[MAIN_MENU_SCENE_MODEL_MATRIX_ELEMENTS];

typedef union MainMenuFixedMatrixWithTranslation {
    s16 elements[16];
    struct {
        s16 rotation[10];
        MainMenuModelVec3i translation;
    } parts;
} MainMenuFixedMatrixWithTranslation;

extern RomAssetAddress gCharacterRawAssetRanges[];
extern RomAssetAddress gCharacterTextureAssetRanges[];
extern MainMenuModelAssetHandles gAssetHandles;

#define ASSET_HANDLE(index) (((s16 *)&gAssetHandles)[(index)])

// compressRaceRecordReplayData best match: 99.203% (nonmatchings/compressRaceRecordReplayData-1219509448159986855/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/main_menu/main_menu_scene_model/compressRaceRecordReplayData.s")

#ifdef NON_MATCHING
s32 compressRaceRecordReplayData(u8 *src, s32 srcLen, u16 *dst) {
    s32 count;
    s32 srcPos;
    s32 outCount;
    s32 remaining;
    s32 bestDistance;
    s32 bestLength;
    s32 distance;
    s32 maxLength;
    s32 searchPos;
    s32 length;
    volatile u16 *out;
    u8 *cur;
    u8 *prev;

    srcPos = 0;
    *dst = srcLen;
    outCount = 1;
    remaining = srcLen;
    bestDistance = 0;
    bestLength = 0;
    distance = 1;
    out = 1 + dst;

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
    cur = src;
    count = 0;
    if (maxLength > 0) {
        cur += srcPos;
        prev = src;
        prev = searchPos + prev;
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
        outCount += 1;
        out = out + 1;
        srcPos += 1;
    } else {
        *out = (bestLength << 10) | bestDistance;
        outCount += 1;
        out = out + 1;
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

// saveRaceRecordReplayData best match: 46.979% (nonmatchings/saveRaceRecordReplayData-5802343343535905907/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/main_menu/main_menu_scene_model/saveRaceRecordReplayData.s")

#ifdef NON_MATCHING
#define RACE_INPUT_HISTORY_LENGTH 0x1194
#define REPLAY_SAVE_MAX_NORMAL 0x580
#define REPLAY_SAVE_MIN_NORMAL 0x581
#define REPLAY_SAVE_MAX_EXTRA 0x300
#define REPLAY_SAVE_MIN_EXTRA 0x301

typedef struct {
    /* 0x0000 */ s32 writeIndex;
    /* 0x0004 */ s32 lastWriteIndex;
    /* 0x0008 */ s8 enabled;
    /* 0x0009 */ s8 pad9[4];
    /* 0x000D */ s8 stickX[RACE_INPUT_HISTORY_LENGTH];
    /* 0x11A1 */ s8 stickY[RACE_INPUT_HISTORY_LENGTH];
    /* 0x2335 */ u8 buttons[RACE_INPUT_HISTORY_LENGTH];
} RaceInputHistoryBuffer;

typedef struct {
    /* 0x00 */ u16 length;
    /* 0x02 */ u16 offset;
} RaceRecordReplaySlot;

typedef struct {
    /* 0x0000 */ u8 pad0[0x232];
    /* 0x0232 */ RaceRecordReplaySlot slots[9];
    /* 0x0256 */ u16 data[0x3D55];
} RaceRecordReplaySave;

extern RaceRecordReplaySave gGameSaveDataBuffer;
extern s16 gRaceCourseIndex;
extern u8 D_8010B200[];
extern u16 D_8010E180[];
extern u8 D_800F3EF0[];

extern s32 compressRaceRecordReplayData(u8 *src, s32 srcLen, u16 *dst);

#define ACCUM_SLOT(courseIndex, slotIndex, maxSize, minSize) \
    if (gRaceCourseIndex != (courseIndex)) {          \
        slotLength = save->slots[(slotIndex)].length; \
        if (slotLength != 0) {                        \
            if (slotLength >= (minSize)) {            \
                totalLength += slotLength;            \
            } else {                                  \
                totalLength += (maxSize);             \
            }                                         \
        } else {                                      \
            totalLength += (maxSize);                 \
        }                                             \
    }

#define COPY_OLD_SLOT(courseIndex, slotIndex)                                   \
    if (gRaceCourseIndex != (courseIndex)) {                                    \
        count = save->slots[(slotIndex)].length;                                \
        if (count != 0) {                                                       \
            oldOffset = save->slots[(slotIndex)].offset;                        \
            save->slots[(slotIndex)].offset = writeIndex;                       \
            if (count > 0) {                                                    \
                src = &oldData[oldOffset];                                      \
                dst = &save->data[writeIndex];                                 \
                copied = 0;                                                     \
                do {                                                            \
                    copied++;                                                   \
                    *dst = *src;                                                \
                    src++;                                                      \
                    writeIndex++;                                               \
                    dst++;                                                      \
                } while (copied < save->slots[(slotIndex)].length);             \
            }                                                                   \
        }                                                                       \
    } else {                                                                    \
        save->slots[(slotIndex)].offset = writeIndex;                           \
        save->slots[(slotIndex)].length = compressedLength;                     \
        copied = 0;                                                             \
        if (compressedLength > 0) {                                             \
            src = D_8010E180;                                                   \
            dst = &save->data[writeIndex];                                      \
            do {                                                                \
                copied++;                                                       \
                *dst = *src;                                                    \
                src++;                                                          \
                writeIndex++;                                                   \
                dst++;                                                          \
            } while (copied < compressedLength);                                \
        }                                                                       \
    }                                                                           \
done_##slotIndex:

s32 saveRaceRecordReplayData(void) {
    RaceInputHistoryBuffer *history;
    RaceRecordReplaySave *save;
    u16 *oldData;
    u16 *src;
    u16 *dst;
    u8 *packed;
    s32 i;
    s32 totalLength;
    s32 compressedLength;
    s32 writeIndex;
    s32 slotLength;
    s32 oldOffset;
    s32 copied;
    s32 count;
    u8 buttons;

    history = getRelocatableHeapBlockBase(ASSET_HANDLE(43));
    if (history->lastWriteIndex >= 0xFD5) {
        return 1;
    }

    packed = D_8010B200;
    *(s16 *)packed = history->lastWriteIndex;
    packed[2] = history->pad9[1];
    packed[3] = history->pad9[2];

    i = 0;
    if (history->lastWriteIndex > 0) {
        do {
            packed[4 + (i * 3)] = history->stickX[i];
            packed[5 + (i * 3)] = history->stickY[i];
            buttons = history->buttons[i];
            ((volatile u8 *)packed)[6 + (i * 3)] = buttons;
            packed[6 + (i * 3)] = buttons & ~0x40;
            i++;
        } while (i < history->lastWriteIndex);
    }

    compressedLength = compressRaceRecordReplayData(D_8010B200, (history->lastWriteIndex * 3) + 4, D_8010E180);
    if (compressedLength < 0) {
        return 1;
    }

    save = &gGameSaveDataBuffer;
    totalLength = 0;
    ACCUM_SLOT(0, 0, REPLAY_SAVE_MAX_NORMAL, REPLAY_SAVE_MIN_NORMAL);
    ACCUM_SLOT(1, 1, REPLAY_SAVE_MAX_NORMAL, REPLAY_SAVE_MIN_NORMAL);
    ACCUM_SLOT(2, 2, REPLAY_SAVE_MAX_NORMAL, REPLAY_SAVE_MIN_NORMAL);
    ACCUM_SLOT(3, 3, REPLAY_SAVE_MAX_NORMAL, REPLAY_SAVE_MIN_NORMAL);
    ACCUM_SLOT(4, 4, REPLAY_SAVE_MAX_NORMAL, REPLAY_SAVE_MIN_NORMAL);
    ACCUM_SLOT(5, 5, REPLAY_SAVE_MAX_NORMAL, REPLAY_SAVE_MIN_NORMAL);
    ACCUM_SLOT(6, 6, REPLAY_SAVE_MAX_NORMAL, REPLAY_SAVE_MIN_NORMAL);
    ACCUM_SLOT(8, 7, REPLAY_SAVE_MAX_EXTRA, REPLAY_SAVE_MIN_EXTRA);
    ACCUM_SLOT(9, 8, REPLAY_SAVE_MAX_EXTRA, REPLAY_SAVE_MIN_EXTRA);

    if (compressedLength >= REPLAY_SAVE_MIN_NORMAL) {
        totalLength += compressedLength;
    } else if ((gRaceCourseIndex == 9) || (gRaceCourseIndex == 8)) {
        totalLength += REPLAY_SAVE_MAX_EXTRA;
    } else {
        totalLength += REPLAY_SAVE_MAX_NORMAL;
    }

    if (totalLength >= 0x3A81) {
        return 1;
    }

    gAssetHandles.raceRecordReplaySaveBufferHandle = allocRelocatableHeapBlock(0x7500);
    oldData = getRelocatableHeapBlockBase(gAssetHandles.raceRecordReplaySaveBufferHandle);
    src = save->data;
    dst = oldData;
    do {
        *dst = *src;
        src++;
        dst++;
    } while ((u32)src < (u32)D_800F3EF0);

    writeIndex = 0;
    COPY_OLD_SLOT(0, 0);
    COPY_OLD_SLOT(1, 1);
    COPY_OLD_SLOT(2, 2);
    COPY_OLD_SLOT(3, 3);
    COPY_OLD_SLOT(4, 4);
    COPY_OLD_SLOT(5, 5);
    COPY_OLD_SLOT(6, 6);
    COPY_OLD_SLOT(8, 7);
    COPY_OLD_SLOT(9, 8);

    freeRelocatableHeapBlock(gAssetHandles.raceRecordReplaySaveBufferHandle);
    return 0;
}

#undef COPY_OLD_SLOT
#undef ACCUM_SLOT
#undef REPLAY_SAVE_MIN_EXTRA
#undef REPLAY_SAVE_MAX_EXTRA
#undef REPLAY_SAVE_MIN_NORMAL
#undef REPLAY_SAVE_MAX_NORMAL
#undef RACE_INPUT_HISTORY_LENGTH
#endif

// loadCurrentRaceRecordReplayData best match: 97.083% (nonmatchings/loadCurrentRaceRecordReplayData-6934502587000073416/base_46.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/main_menu/main_menu_scene_model/loadCurrentRaceRecordReplayData.s")

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
extern s16 gRaceCourseIndex;

void loadCurrentRaceRecordReplayData(void) {
    u16 *srcBase;
    u16 *src;
    u8 *dst;
    s32 outPos;
    s32 count;
    s32 copied;
    s32 code;
    s32 length;
    s32 offset;
    s32 remainder;
    u8 *copy;
    RaceInputHistoryBuffer *history;
    s32 i;
    volatile u8 *read;
    s32 copyMore;

    srcBase = &D_800ECC46;
    outPos = 0;
    src = srcBase;
    switch (gRaceCourseIndex) {
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
    count = *src;
    src++;

loop:
    if (outPos >= count) {
        goto done;
    }
    copied = 0;
    code = *src;
    length = (code >> 10) & 0x3F;
    if (length != 0) {
        goto compressed;
    }
    dst[outPos] = (u16)code;
    outPos++;
    goto next;

compressed:
    offset = outPos - (code & 0x3FF);
    if (length > 0) {
        remainder = length & 3;
        if (remainder == 0) {
            goto copy4;
        }
        copy = offset + dst;
copy1:
        dst[outPos] = *copy;
        copied++;
        outPos++;
        copy++;
        if (remainder != copied) {
            goto copy1;
        }
        if (copied == length) {
            goto next;
        }

copy4:
        copy = offset + copied + dst;
copy4_loop:
        dst[outPos] = copy[0];
        outPos++;
        dst[outPos] = copy[1];
        outPos++;
        dst[outPos] = copy[2];
        outPos++;
        dst[outPos] = copy[3];
        copied += 4;
        copyMore = copied != length;
        outPos++;
        copy += 4;
        if (copyMore) {
            goto copy4_loop;
        }
    }
next:
    src++;
    goto loop;

done:
    read = dst;
    history = (RaceInputHistoryBuffer *)getRelocatableHeapBlockBase(ASSET_HANDLE(7));
    history->writeIndex = 0;
    copied = *(volatile s16 *)&read[0];
    history->lastWriteIndex = copied;
    history->enabled = 1;
    history->pad9[0] = gRaceCourseIndex;
    history->pad9[1] = *(volatile s8 *)&read[2];
    history->pad9[2] = *(volatile s8 *)&read[3];
    history->pad9[3] = 0;

    i = 0;
    if (copied <= 0) {
        return;
    }
write_loop:
    history->stickX[i] = *(s8 *)&D_8010B200[4 + (i * 3)];
    history->stickY[i] = *(s8 *)&D_8010B200[5 + (i * 3)];
    history->buttons[i] = D_8010B200[6 + (i * 3)];
    i++;
    if (i < history->lastWriteIndex) {
        goto write_loop;
    }
}
#endif

void loadMainMenuSceneModelAssets(void) {
    LOAD_ASSET(_5E34A0, 0x3F);
}

void loadMainMenuSceneModelAnimationBank(void) {
    LOAD_ASSET(_215BE0, 0x3F);
}

void initMainMenuSceneModel(s32 actorIndex, s32 modelIndex) {
    MainMenuSceneModel *model;

    loadRawRomAsset(MAIN_MENU_MODEL_ASSET_RANGE_START(gCharacterRawAssetRanges, modelIndex),
                  MAIN_MENU_MODEL_ASSET_RANGE_END(gCharacterRawAssetRanges, modelIndex), actorIndex + 0x33);
    loadCompressedRomAsset((void *)MAIN_MENU_MODEL_ASSET_RANGE_START(gCharacterTextureAssetRanges, modelIndex),
                  (void *)MAIN_MENU_MODEL_ASSET_RANGE_END(gCharacterTextureAssetRanges, modelIndex), actorIndex + 0x39);
    gAssetHandles.modelInstanceHandles[actorIndex] = allocRelocatableHeapBlock(sizeof(MainMenuSceneModel));
    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(gAssetHandles.modelInstanceHandles[actorIndex]);
    model->actorIndex = actorIndex;
    model->modelIndex = modelIndex;
    initMainMenuSceneModelParts(model);
}

void setMainMenuSceneModelAnimation(s32 modelIndex, s32 animationIndex) {
    MainMenuModelAnimationBank *animationBank;
    s16 *frameData;
    MainMenuSceneModel *model;
    s16 frameDuration;

    animationBank = (MainMenuModelAnimationBank *)getRelocatableHeapBlockBase(ASSET_HANDLE(63));
    frameData = MAIN_MENU_ANIMATION_FRAME_DATA(animationBank, animationIndex);
    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(ASSET_HANDLE(45 + modelIndex));
    frameDuration = *frameData++;
    model->framesRemaining = frameDuration;
    model->animationStart = frameData;
    model->framesRemaining++;
    model->animationCursor = frameData;
    model->frameDuration = frameDuration;
}

MainMenuSceneModel *getMainMenuSceneModel(s32 modelIndex) {
    return (MainMenuSceneModel *)getRelocatableHeapBlockBase(ASSET_HANDLE(45 + modelIndex));
}

void applyMainMenuSceneModelAnimationFrame(MainMenuSceneModel *model) {
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

s32 stepMainMenuSceneModelAnimation(s32 modelIndex) {
    int new_var2;
    MainMenuSceneModel *new_var;
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(ASSET_HANDLE(45 + modelIndex));
    if (model->framesRemaining == 1) {
        goto ret1_initial;
    }
    new_var = model;
    new_var2 = 0 & 0xFFu;
    model->framesRemaining--;
    applyMainMenuSceneModelAnimationFrame(model);
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

void loopMainMenuSceneModelAnimation(s32 modelIndex) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(ASSET_HANDLE(45 + modelIndex));
    model->framesRemaining--;
    if (model->framesRemaining <= 0) {
        model->framesRemaining = model->frameDuration;
        model->animationCursor = model->animationStart;
    }
    applyMainMenuSceneModelAnimationFrame(model);
}

void setMainMenuSceneModelPosition(s32 modelIndex, s32 x, s32 y, s32 z) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(ASSET_HANDLE(45 + modelIndex));
    model->pos.x = x;
    model->pos.y = y;
    model->pos.z = z;
}

void setMainMenuSceneModelRotation(s32 modelIndex, s16 x, s16 y, s16 z) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(ASSET_HANDLE(45 + modelIndex));
    model->rot.x = x;
    model->rot.y = y;
    model->rot.z = z;
}

// updateMainMenuSceneModelTransforms best match: 92.160% (nonmatchings/updateMainMenuSceneModelTransforms-3379532139742180785/base_37.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/main_menu/main_menu_scene_model/updateMainMenuSceneModelTransforms.s")

#ifdef NON_MATCHING
#if 0
static s32 getMainMenuSceneModelPartOffsetZ(MainMenuSceneModel *model, s32 partIndex) {
    if (partIndex == MAIN_MENU_SCENE_MODEL_PART_COUNT - 1) {
        return model->lastPartOffsetZ;
    }
    return model->parts[partIndex + 1].previousPartOffsetZ;
}

void updateMainMenuSceneModelTransforms(MainMenuSceneModel *model) {
    s16 partLocalMatrices[MAIN_MENU_SCENE_MODEL_PART_COUNT][16];
    MainMenuFixedMatrixWithTranslation rootMatrix;
    MainMenuModelPart *part;
    s16 (*localMatrix)[16];
    s16 (*localMatrixEnd)[16];
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
    MainMenuFixedMatrix3x3 *worldMatrix;
    s16 *worldAxis;
    s16 *localAxis;
    s16 *rootAxis;
    s32 axis;
    s32 partIndex;
    MainMenuModelDisplayObject *displayObject;
    s32 offsetX;
    s32 offsetY;
    s32 offsetZ;
    s32 dot;

    part = model->parts;
    localMatrix = partLocalMatrices;
    localMatrixEnd = &partLocalMatrices[MAIN_MENU_SCENE_MODEL_PART_COUNT];
    do {
        sineX = fixedSine(part->rot.x);
        cosineX = fixedCosine(part->rot.x);
        sineY = fixedSine(part->rot.y);
        cosineY = fixedCosine(part->rot.y);
        sineZ = fixedSine(part->rot.z);
        cosineZ = fixedCosine(part->rot.z);
        negSineY = -sineY;
        negSineZ = -sineZ;

        (*localMatrix)[0] = (cosineY * cosineZ) / FIXED_MATRIX_ONE;
        (*localMatrix)[1] = (cosineY * sineZ) / FIXED_MATRIX_ONE;
        sineXTimesSineY = (sineX * sineY) / FIXED_MATRIX_ONE;
        (*localMatrix)[2] = negSineY;
        (*localMatrix)[3] = ((sineXTimesSineY * cosineZ) / FIXED_MATRIX_ONE) + ((cosineX * negSineZ) / FIXED_MATRIX_ONE);
        (*localMatrix)[4] = ((sineXTimesSineY * sineZ) / FIXED_MATRIX_ONE) + ((cosineX * cosineZ) / FIXED_MATRIX_ONE);
        (*localMatrix)[5] = (sineX * cosineY) / FIXED_MATRIX_ONE;
        cosineXTimesSineY = (cosineX * sineY) / FIXED_MATRIX_ONE;
        (*localMatrix)[6] = ((cosineXTimesSineY * cosineZ) / FIXED_MATRIX_ONE) + ((sineX * sineZ) / FIXED_MATRIX_ONE);
        (*localMatrix)[7] = ((cosineXTimesSineY * sineZ) / FIXED_MATRIX_ONE) + (((-sineX) * cosineZ) / FIXED_MATRIX_ONE);
        (*localMatrix)[8] = (cosineX * cosineY) / FIXED_MATRIX_ONE;
        localMatrix++;
        part++;
    } while (localMatrix != localMatrixEnd);

    makeFixedRotationZXY(rootMatrix.elements, model->rot.x, model->rot.y, model->rot.z);
    rootMatrix.parts.translation = model->pos;

    localMatrix = partLocalMatrices;
    displayObject = model->displayObjects;
    do {
        worldMatrix = (MainMenuFixedMatrix3x3 *)displayObject->pad0;
        worldAxis = *worldMatrix;
        localAxis = *localMatrix;
        axis = 0;
        do {
            rootAxis = rootMatrix.elements;
            do {
                dot = (rootAxis[6] * localAxis[2]) + (rootAxis[0] * localAxis[0]) +
                      (rootAxis[2] * localAxis[1]);
                *worldAxis++ = dot / FIXED_MATRIX_ONE;
                rootAxis++;
            } while (rootAxis != &rootMatrix.elements[MAIN_MENU_SCENE_MODEL_MATRIX_AXES]);
            axis++;
            localAxis += MAIN_MENU_SCENE_MODEL_MATRIX_AXES;
        } while (axis != MAIN_MENU_SCENE_MODEL_MATRIX_AXES);
        localMatrix++;
        displayObject++;
    } while (localMatrix < localMatrixEnd);

    part = model->parts;
    displayObject = model->displayObjects;
    partIndex = 0;
    do {
        localAxis = rootMatrix.elements;
        offsetX = part->offsetX;
        offsetY = part->offsetY;
        offsetZ = getMainMenuSceneModelPartOffsetZ(model, partIndex);

        dot = (((s64)localAxis[3] * offsetY) + ((s64)localAxis[0] * offsetX) +
               ((s64)localAxis[6] * offsetZ)) / FIXED_MATRIX_ONE;
        displayObject->screenX = dot + rootMatrix.parts.translation.x;
        localAxis++;

        dot = (((s64)localAxis[3] * offsetY) + ((s64)localAxis[0] * offsetX) +
               ((s64)localAxis[6] * offsetZ)) / FIXED_MATRIX_ONE;
        displayObject->screenY = dot + rootMatrix.parts.translation.y;
        localAxis++;

        dot = (((s64)localAxis[3] * offsetY) + ((s64)localAxis[0] * offsetX) +
               ((s64)localAxis[6] * offsetZ)) / FIXED_MATRIX_ONE;
        displayObject->screenZ = dot + rootMatrix.parts.translation.z;

        partIndex++;
        part++;
        displayObject++;
    } while (partIndex != MAIN_MENU_SCENE_MODEL_PART_COUNT);
}
#endif

typedef struct MainMenuTransformPartCursor {
    u8 bytes[0x14];
} MainMenuTransformPartCursor;

typedef struct MainMenuTransformMatrixSlot {
    s16 e0;
    s16 e2;
    s16 e4;
    s16 e6;
    s16 e8;
    s16 eA;
    s16 eC;
    s16 eE;
    s16 e10;
    s16 pad12[7];
} MainMenuTransformMatrixSlot;

typedef struct MainMenuTransformProducts {
    u8 pad[0x10];
    s64 first;
    s64 second;
} MainMenuTransformProducts;

#define MAIN_MENU_TRANSFORM_CURSOR_S16(cursor, offset) (*(s16 *)((u8 *)(cursor) + (offset)))
#define MAIN_MENU_TRANSFORM_CURSOR_S32(cursor, offset) (*(s32 *)((u8 *)(cursor) + (offset)))

void updateMainMenuSceneModelTransforms(MainMenuSceneModel *model) {
    u8 partMatrices[0x1B0];
    MainMenuTransformPartCursor *partCursor;
    MainMenuTransformMatrixSlot *matrixSlot;
    MainMenuTransformMatrixSlot *matrixSlotEnd;
    MainMenuTransformMatrixSlot *localSlot;
    s16 *combinedMatrix;
    s16 *localMatrix;
    s16 *rootAxis;
    s16 *modelCursor;
    s16 *displayCursor;
    u8 *displayBase;
    u8 *partBase;
    s16 *rootCursor;
    s16 *positionCursor;
    s32 sineX;
    s32 cosineX;
    s32 sineY;
    s32 cosineY;
    s32 sineZ;
    s32 cosineZ;
    s32 sineXTimesSineY;
    s32 cosineXTimesSineY;
    s32 axisOffset;
    s32 partOffset;
    s32 dot;
    MainMenuTransformProducts products;

    matrixSlot = (MainMenuTransformMatrixSlot *)((u8 *)partMatrices - 0x30);
    partCursor = (MainMenuTransformPartCursor *)model;
    matrixSlotEnd = matrixSlot + MAIN_MENU_SCENE_MODEL_PART_COUNT;
    do {
        sineX = fixedSine(MAIN_MENU_TRANSFORM_CURSOR_S16(partCursor, 0x1E));
        cosineX = fixedCosine(MAIN_MENU_TRANSFORM_CURSOR_S16(partCursor, 0x1E));
        sineY = fixedSine(MAIN_MENU_TRANSFORM_CURSOR_S16(partCursor, 0x20));
        cosineY = fixedCosine(MAIN_MENU_TRANSFORM_CURSOR_S16(partCursor, 0x20));
        sineZ = fixedSine(MAIN_MENU_TRANSFORM_CURSOR_S16(partCursor, 0x22));
        cosineZ = fixedCosine(MAIN_MENU_TRANSFORM_CURSOR_S16(partCursor, 0x22));
        matrixSlot->e4 = -sineY;
        matrixSlot++;
        partCursor++;
        dot = (cosineY * cosineZ) / FIXED_MATRIX_ONE;
        matrixSlot[-1].e0 = dot;
        matrixSlot[-1].e2 = (cosineY * sineZ) / FIXED_MATRIX_ONE;
        sineXTimesSineY = (sineX * sineY) / FIXED_MATRIX_ONE;
        matrixSlot[-1].e6 = ((sineXTimesSineY * cosineZ) / FIXED_MATRIX_ONE) + ((cosineX * -sineZ) / FIXED_MATRIX_ONE);
        matrixSlot[-1].e8 = ((sineXTimesSineY * sineZ) / FIXED_MATRIX_ONE) + ((cosineX * cosineZ) / FIXED_MATRIX_ONE);
        matrixSlot[-1].eA = (sineX * cosineY) / FIXED_MATRIX_ONE;
        cosineXTimesSineY = (cosineX * sineY) / FIXED_MATRIX_ONE;
        matrixSlot[-1].eC = ((cosineXTimesSineY * cosineZ) / FIXED_MATRIX_ONE) + ((sineX * sineZ) / FIXED_MATRIX_ONE);
        matrixSlot[-1].eE = ((cosineXTimesSineY * sineZ) / FIXED_MATRIX_ONE) + (((-sineX) * cosineZ) / FIXED_MATRIX_ONE);
        matrixSlot[-1].e10 = (cosineX * cosineY) / FIXED_MATRIX_ONE;
    } while (matrixSlot != matrixSlotEnd);

    makeFixedRotationZXY((s16 *)matrixSlot, model->rot.x, model->rot.y, model->rot.z);
    MAIN_MENU_TRANSFORM_CURSOR_S32(matrixSlot, 0x14) = model->pos.x;
    MAIN_MENU_TRANSFORM_CURSOR_S32(matrixSlot, 0x18) = model->pos.y;
    MAIN_MENU_TRANSFORM_CURSOR_S32(matrixSlot, 0x1C) = model->pos.z;

    displayBase = (u8 *)model;
    localSlot = (MainMenuTransformMatrixSlot *)((u8 *)partMatrices - 0x30);
    do {
        displayCursor = (s16 *)displayBase;
        axisOffset = 0;
        localMatrix = (s16 *)localSlot;
        do {
            rootAxis = (s16 *)matrixSlot;
            modelCursor = displayCursor;
            do {
                sineXTimesSineY = localMatrix[2] * rootAxis[6];
                cosineXTimesSineY = rootAxis[0] * localMatrix[0];
                dot = rootAxis[2] * localMatrix[1];
                dot = sineXTimesSineY + cosineXTimesSineY + dot;
                rootAxis++;
                modelCursor++;
                MAIN_MENU_TRANSFORM_CURSOR_S16(modelCursor, 0x146) = dot / FIXED_MATRIX_ONE;
            } while (rootAxis != (s16 *)matrixSlot + MAIN_MENU_SCENE_MODEL_MATRIX_AXES);
            axisOffset += 6;
            displayCursor += 3;
            localMatrix += MAIN_MENU_SCENE_MODEL_MATRIX_AXES;
        } while (axisOffset != 0x12);
        localSlot++;
        displayBase += 0x20;
    } while (localSlot < matrixSlot);

    displayBase = (u8 *)model;
    partBase = (u8 *)model;
    partOffset = 0;
    do {
        positionCursor = (s16 *)matrixSlot;
        rootCursor = positionCursor;
        combinedMatrix = (s16 *)displayBase;
        do {
            products.first = (s64)rootCursor[3] * MAIN_MENU_TRANSFORM_CURSOR_S32(partBase, 0x28);
            products.second = (s64)rootCursor[0] * MAIN_MENU_TRANSFORM_CURSOR_S32(partBase, 0x24);
            dot = (products.first +
                   (products.second +
                    ((s64)rootCursor[6] * MAIN_MENU_TRANSFORM_CURSOR_S32(partBase, 0x2C)))) /
                  FIXED_MATRIX_ONE;
            MAIN_MENU_TRANSFORM_CURSOR_S32(combinedMatrix, 0x15C) = dot;
            dot += MAIN_MENU_TRANSFORM_CURSOR_S32(positionCursor, 0x14);
            positionCursor += 2;
            rootCursor++;
            combinedMatrix = (s16 *)((u8 *)combinedMatrix + 4);
            MAIN_MENU_TRANSFORM_CURSOR_S32(combinedMatrix, 0x158) = dot;
        } while (positionCursor != (s16 *)matrixSlot + 6);
        partOffset += 0x14;
        displayBase += 0x20;
        partBase += 0x14;
    } while (partOffset != 0x118);
}

#undef MAIN_MENU_TRANSFORM_CURSOR_S16
#undef MAIN_MENU_TRANSFORM_CURSOR_S32
#endif
