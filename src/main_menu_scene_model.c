#include "main_menu_scene_model.h"
#include "relocatable_heap.h"
#include "asset_manager.h"
#include "fixed_point_math.h"

/* Frame offsets are halfword-relative to the bank start; this form preserves target addu order. */
#define MAIN_MENU_ANIMATION_FRAME_DATA(bank, index) \
    ((s16 *)(((bank)->frameOffsets[(index)] * sizeof(s16)) + (s32)(bank)))
#define FIXED_MATRIX_ONE 0x1000
#define MAIN_MENU_MODEL_ASSET_RANGE_WORDS 2
#define MAIN_MENU_MODEL_ASSET_RANGE_START(table, index) ((table)[(index) * MAIN_MENU_MODEL_ASSET_RANGE_WORDS])
#define MAIN_MENU_MODEL_ASSET_RANGE_END(table, index) ((table)[((index) * MAIN_MENU_MODEL_ASSET_RANGE_WORDS) + 1])

extern s16 gMainMenuSceneModelHandles[];
extern s16 D_801121AE;
extern u8 gMainMenuSceneModelAssetStart;
extern u8 D_608560;
extern u8 D_215BE0;
extern u8 D_21D9D0;

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

extern RomAssetAddress gCharacterRawAssetRanges[];
extern RomAssetAddress gCharacterTextureAssetRanges[];
extern MainMenuModelAssetHandles gAssetHandles;

// compressRaceRecordReplayData best match: 98.696% (nonmatchings/compressRaceRecordReplayData-5802343343535905907/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_model/compressRaceRecordReplayData.s")

#ifdef NON_MATCHING
s32 compressRaceRecordReplayData(u8 *src, s32 srcLen, s16 *dst) {
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
        outCount += 1;
        out += 1;
        srcPos += 1;
    } else {
        *out = (bestLength << 10) | bestDistance;
        outCount += 1;
        out += 1;
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
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_model/saveRaceRecordReplayData.s")

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
extern s16 gRaceReplayInputBufferHandle;
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

    history = getRelocatableHeapBlockBase(gRaceReplayInputBufferHandle);
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

// loadCurrentRaceRecordReplayData best match: 80.022% (nonmatchings/loadCurrentRaceRecordReplayData-3357475854818838508/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_model/loadCurrentRaceRecordReplayData.s")

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
extern s16 gCurrentRaceRecordReplayHandle;
extern u16 gRaceCourseIndex;

void loadCurrentRaceRecordReplayData(void) {
    u16 *srcBase;
    u16 *src;
    u8 *dst;
    u8 *model;
    s32 outPos;
    s32 count;
    s32 copied;
    u16 code;
    s32 length;
    s32 offset;
    s32 remainder;
    u8 *copy;
    s32 i;
    u8 *read;
    u8 *write;

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
    copied = 0;
    if (outPos >= count) {
        goto done;
    }
    code = *src;
    length = (code >> 10) & 0x3F;
    if (length != 0) {
        goto compressed;
    }
    dst[outPos] = code;
    outPos++;
    src++;
    goto loop;

compressed:
    offset = outPos - (code & 0x3FF);
    if (length <= 0) {
        goto next;
    }
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
    outPos++;
    copy += 4;
    if (copied != length) {
        goto copy4_loop;
    }
next:
    src++;
    goto loop;

done:
    model = (u8 *)getRelocatableHeapBlockBase(gCurrentRaceRecordReplayHandle);
    *(s32 *)&model[0] = 0;
    *(s32 *)&model[4] = *(s16 *)&dst[0];
    model[8] = 1;
    model[9] = gRaceCourseIndex;
    model[10] = *(s8 *)&dst[2];
    model[12] = 0;
    model[11] = *(s8 *)&dst[3];

    i = 0;
    if (*(s16 *)&dst[0] <= 0) {
        return;
    }
    read = D_8010B200;
    write = model;
write_loop:
    i++;
    write[0xD] = *(s8 *)&read[4];
    write[0x11A1] = *(s8 *)&read[5];
    read += 3;
    write++;
    write[0x2334] = read[3];
    if (i < *(s32 *)&model[4]) {
        goto write_loop;
    }
}
#endif

void loadMainMenuSceneModelAssets(void) {
    loadCompressedRomAsset(&gMainMenuSceneModelAssetStart, &D_608560, 0x3F);
}

void loadMainMenuSceneModelAnimationBank(void) {
    loadCompressedRomAsset(&D_215BE0, &D_21D9D0, 0x3F);
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

    animationBank = (MainMenuModelAnimationBank *)getRelocatableHeapBlockBase(D_801121AE);
    frameData = MAIN_MENU_ANIMATION_FRAME_DATA(animationBank, animationIndex);
    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(gMainMenuSceneModelHandles[modelIndex]);
    frameDuration = *frameData++;
    model->framesRemaining = frameDuration;
    model->animationStart = frameData;
    model->framesRemaining++;
    model->animationCursor = frameData;
    model->frameDuration = frameDuration;
}

MainMenuSceneModel *getMainMenuSceneModel(s32 modelIndex) {
    return (MainMenuSceneModel *)getRelocatableHeapBlockBase(gMainMenuSceneModelHandles[modelIndex]);
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

    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(gMainMenuSceneModelHandles[modelIndex]);
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

    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(gMainMenuSceneModelHandles[modelIndex]);
    model->framesRemaining--;
    if (model->framesRemaining <= 0) {
        model->framesRemaining = model->frameDuration;
        model->animationCursor = model->animationStart;
    }
    applyMainMenuSceneModelAnimationFrame(model);
}

void setMainMenuSceneModelPosition(s32 modelIndex, s32 x, s32 y, s32 z) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(gMainMenuSceneModelHandles[modelIndex]);
    model->pos.x = x;
    model->pos.y = y;
    model->pos.z = z;
}

void setMainMenuSceneModelRotation(s32 modelIndex, s16 x, s16 y, s16 z) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(gMainMenuSceneModelHandles[modelIndex]);
    model->rot.x = x;
    model->rot.y = y;
    model->rot.z = z;
}

// updateMainMenuSceneModelTransforms best match: 83.677% (nonmatchings/func_8004215C-7273315160691878794/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_model/updateMainMenuSceneModelTransforms.s")

#ifdef NON_MATCHING
void updateMainMenuSceneModelTransforms(MainMenuSceneModel *model) {
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
        sineX = fixedSine(*(s16 *)(partCursor + 0x1E));
        cosineX = fixedCosine(*(s16 *)(partCursor + 0x1E));
        sineY = fixedSine(*(s16 *)(partCursor + 0x20));
        cosineY = fixedCosine(*(s16 *)(partCursor + 0x20));
        sineZ = fixedSine(*(s16 *)(partCursor + 0x22));
        cosineZ = fixedCosine(*(s16 *)(partCursor + 0x22));
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

    makeFixedRotationZXY(rootMatrix, model->rot.x, model->rot.y, model->rot.z);
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
