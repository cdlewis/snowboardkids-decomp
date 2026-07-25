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

#define ASSET_HANDLE(index) (((s16 *)&gAssetHandles)[(index)])

// compressRaceRecordReplayData best match: 99.203% (nonmatchings/compressRaceRecordReplayData-1219509448159986855/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/main_menu/main_menu_scene_model/compressRaceRecordReplayData.s")

extern s32 compressRaceRecordReplayData(u8 *src, s32 srcLen, u16 *dst);

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

// loadCurrentRaceRecordReplayData best match: 98.946% (nonmatchings/loadCurrentRaceRecordReplayData-8498672362023432715/base_19.c)
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
    u8 *copyStart;
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
    {
        copied = 0;
        remainder = length & 3;
        copyStart = offset + dst;
        if (remainder == 0) {
            goto copy4;
        }
        copy = copyStart;
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
    if ((copied <= 0) != 0) {
        return;
    }
write_loop:
    history->stickX[i] = *(s8 *)&D_8010B200[4 + (i * 3)];
    history->stickY[i] = *(s8 *)&D_8010B200[5 + (i * 3)];
    history->buttons[i] = D_8010B200[6 + (i * 3)];
    i++;
    length = i < history->lastWriteIndex;
    if (length) {
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

// A readable NON_MATCHING implementation is kept below; the production build uses the matching assembly.
#pragma GLOBAL_ASM("asm/nonmatchings/menu/main_menu/main_menu_scene_model/updateMainMenuSceneModelTransforms.s")

#ifdef NON_MATCHING
static s32 getMainMenuSceneModelPartOffsetZ(MainMenuSceneModel *model, s32 partIndex) {
    if (partIndex == MAIN_MENU_SCENE_MODEL_PART_COUNT - 1) {
        return model->lastPartOffsetZ;
    }
    return model->parts[partIndex + 1].previousPartOffsetZ;
}

void updateMainMenuSceneModelTransforms(MainMenuSceneModel *model) {
    MainMenuModelTransform partLocalTransforms[MAIN_MENU_SCENE_MODEL_PART_COUNT];
    MainMenuModelTransform rootTransform;
    MainMenuModelPart *part;
    MainMenuModelTransform *localTransform;
    MainMenuModelTransform *localTransformEnd;
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
    s16 *worldAxis;
    s16 *localAxis;
    s16 *rootAxis;
    s32 axis;
    s32 partIndex;
    MainMenuModelTransform *displayObject;
    s32 offsetX;
    s32 offsetY;
    s32 offsetZ;
    s32 dot;

    part = model->parts;
    localTransform = partLocalTransforms;
    localTransformEnd = &partLocalTransforms[MAIN_MENU_SCENE_MODEL_PART_COUNT];
    do {
        sineX = fixedSine(part->rot.x);
        cosineX = fixedCosine(part->rot.x);
        sineY = fixedSine(part->rot.y);
        cosineY = fixedCosine(part->rot.y);
        sineZ = fixedSine(part->rot.z);
        cosineZ = fixedCosine(part->rot.z);
        negSineY = -sineY;
        negSineZ = -sineZ;

        localTransform->rotation[MTX_XX] = (cosineY * cosineZ) / FIXED_MATRIX_ONE;
        localTransform->rotation[MTX_XY] = (cosineY * sineZ) / FIXED_MATRIX_ONE;
        sineXTimesSineY = (sineX * sineY) / FIXED_MATRIX_ONE;
        localTransform->rotation[MTX_XZ] = negSineY;
        localTransform->rotation[MTX_YX] =
            ((sineXTimesSineY * cosineZ) / FIXED_MATRIX_ONE) +
            ((cosineX * negSineZ) / FIXED_MATRIX_ONE);
        localTransform->rotation[MTX_YY] =
            ((sineXTimesSineY * sineZ) / FIXED_MATRIX_ONE) +
            ((cosineX * cosineZ) / FIXED_MATRIX_ONE);
        localTransform->rotation[MTX_YZ] = (sineX * cosineY) / FIXED_MATRIX_ONE;
        cosineXTimesSineY = (cosineX * sineY) / FIXED_MATRIX_ONE;
        localTransform->rotation[MTX_ZX] =
            ((cosineXTimesSineY * cosineZ) / FIXED_MATRIX_ONE) +
            ((sineX * sineZ) / FIXED_MATRIX_ONE);
        localTransform->rotation[MTX_ZY] =
            ((cosineXTimesSineY * sineZ) / FIXED_MATRIX_ONE) +
            (((-sineX) * cosineZ) / FIXED_MATRIX_ONE);
        localTransform->rotation[MTX_ZZ] = (cosineX * cosineY) / FIXED_MATRIX_ONE;
        localTransform++;
        part++;
    } while (localTransform != localTransformEnd);

    makeFixedRotationZXY(rootTransform.rotation, model->rot.x, model->rot.y, model->rot.z);
    rootTransform.translation = model->pos;

    localTransform = partLocalTransforms;
    displayObject = model->displayObjects;
    do {
        worldAxis = displayObject->rotation;
        localAxis = localTransform->rotation;
        axis = 0;
        do {
            rootAxis = rootTransform.rotation;
            do {
                dot = (rootAxis[6] * localAxis[2]) + (rootAxis[0] * localAxis[0]) +
                      (rootAxis[2] * localAxis[1]);
                *worldAxis++ = dot / FIXED_MATRIX_ONE;
                rootAxis++;
            } while (rootAxis != &rootTransform.rotation[MAIN_MENU_SCENE_MODEL_MATRIX_AXES]);
            axis++;
            localAxis += MAIN_MENU_SCENE_MODEL_MATRIX_AXES;
        } while (axis != MAIN_MENU_SCENE_MODEL_MATRIX_AXES);
        localTransform++;
        displayObject++;
    } while (localTransform < localTransformEnd);

    part = model->parts;
    displayObject = model->displayObjects;
    partIndex = 0;
    do {
        localAxis = rootTransform.rotation;
        offsetX = part->offsetX;
        offsetY = part->offsetY;
        offsetZ = getMainMenuSceneModelPartOffsetZ(model, partIndex);

        dot = (((s64)localAxis[3] * offsetY) + ((s64)localAxis[0] * offsetX) +
               ((s64)localAxis[6] * offsetZ)) / FIXED_MATRIX_ONE;
        displayObject->translation.x = dot + rootTransform.translation.x;
        localAxis++;

        dot = (((s64)localAxis[3] * offsetY) + ((s64)localAxis[0] * offsetX) +
               ((s64)localAxis[6] * offsetZ)) / FIXED_MATRIX_ONE;
        displayObject->translation.y = dot + rootTransform.translation.y;
        localAxis++;

        dot = (((s64)localAxis[3] * offsetY) + ((s64)localAxis[0] * offsetX) +
               ((s64)localAxis[6] * offsetZ)) / FIXED_MATRIX_ONE;
        displayObject->translation.z = dot + rootTransform.translation.z;

        partIndex++;
        part++;
        displayObject++;
    } while (partIndex != MAIN_MENU_SCENE_MODEL_PART_COUNT);
}
#endif
