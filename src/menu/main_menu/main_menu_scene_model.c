#include "game/race/race_state.h"
#include "assets.h"
#include "game/save_data.h"
#include "game/menu/main_menu/main_menu_scene_model.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/asset_manager.h"
#include "game/math/fixed_point_math.h"
#include "game/race/race_replay.h"
#include "game/race/scene/race_scene_setup.h"

/* Frame offsets are halfword-relative to the bank start; this form preserves target addu order. */
#define MAIN_MENU_ANIMATION_FRAME_DATA(bank, index) \
    ((s16 *)(((bank)->frameOffsets[(index)] * sizeof(s16)) + (s32)(bank)))
#define FIXED_MATRIX_ONE 0x1000
#define MAIN_MENU_SCENE_MODEL_PART_COUNT 14
#define MAIN_MENU_SCENE_MODEL_MATRIX_AXES 3
#define FIXED_MATRIX_ROWS(matrix) ((s16(*)[MAIN_MENU_SCENE_MODEL_MATRIX_AXES])(matrix))


typedef struct MainMenuAnimationWritePart {
    s32 word0;
    s32 word4;
    s32 word8;
    s32 wordC;
    s32 word10;
} MainMenuAnimationWritePart;

#define ASSET_HANDLE(index) (gAssetHandles[(index)])

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

// saveRaceRecordReplayData best match: 96.493% (nonmatchings/saveRaceRecordReplayData-8498672362023432715/base_18.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/main_menu/main_menu_scene_model/saveRaceRecordReplayData.s")

#ifdef NON_MATCHING
#define REPLAY_SAVE_MAX_NORMAL 0x580
#define REPLAY_SAVE_MIN_NORMAL 0x581
#define REPLAY_SAVE_MAX_EXTRA 0x300
#define REPLAY_SAVE_MIN_EXTRA 0x301

#define ACCUM_SLOT(courseIndex, slotIndex, maxSize, minSize)      \
    if (course != (courseIndex)) {                                \
        count = gGameSaveDataBuffer[0].replaySlots[(slotIndex)].length; \
        if (count != 0) {                                         \
            if (count >= (minSize)) {                             \
                totalLength += count;                             \
            } else {                                              \
                totalLength += (maxSize);                         \
            }                                                     \
        } else {                                                  \
            totalLength += (maxSize);                             \
        }                                                         \
    }

#define COPY_COURSE_DIFF_0 (((course & 0xFFFFu) & 0xFFFFu) != 0)
#define COPY_COURSE_DIFF_1 (course != 1)
#define COPY_COURSE_DIFF_2 (course != 2)
#define COPY_COURSE_DIFF_3 (course != 3)
#define COPY_COURSE_DIFF_4 (course != 4)
#define COPY_COURSE_DIFF_5 (course != 5)
#define COPY_COURSE_DIFF_6 (course != 6)
#define COPY_COURSE_DIFF_8 (course != 8)
#define COPY_COURSE_DIFF_9 (course != 9)
#define COPY_COURSE_DIFF_I(courseIndex) COPY_COURSE_DIFF_##courseIndex
#define COPY_COURSE_DIFF(courseIndex) COPY_COURSE_DIFF_I(courseIndex)

#define AFTER_OLD_COPY_0()
#define AFTER_OLD_COPY_1()
#define AFTER_OLD_COPY_2()
#define AFTER_OLD_COPY_3()
#define AFTER_OLD_COPY_4()
#define AFTER_OLD_COPY_5() \
    if (!count) {          \
    }
#define AFTER_OLD_COPY_6()
#define AFTER_OLD_COPY_7()
#define AFTER_OLD_COPY_8()
#define AFTER_OLD_COPY_I(slotIndex) AFTER_OLD_COPY_##slotIndex()
#define AFTER_OLD_COPY(slotIndex) AFTER_OLD_COPY_I(slotIndex)

#define REPLAY_SLOT_OFFSET_0 gGameSaveDataBuffer[0].replaySlots[0].offset
#define REPLAY_SLOT_OFFSET_1 gGameSaveDataBuffer[0].replaySlots[1].offset
#define REPLAY_SLOT_OFFSET_2 gGameSaveDataBuffer[0].replaySlots[2].offset
#define REPLAY_SLOT_OFFSET_3 gGameSaveDataBuffer[0].replaySlots[3].offset
#define REPLAY_SLOT_OFFSET_4 gGameSaveDataBuffer[0].replaySlots[4].offset
#define REPLAY_SLOT_OFFSET_5 gGameSaveDataBuffer[0].replaySlots[5].offset
#define REPLAY_SLOT_OFFSET_6 gGameSaveDataBuffer[0].replaySlots[6].offset
#define REPLAY_SLOT_OFFSET_7 gGameSaveDataBuffer[0].replaySlots[7].offset
#define REPLAY_SLOT_OFFSET_8 gGameSaveDataBuffer[0].replaySlots[8].offset
#define REPLAY_SLOT_OFFSET_I(slotIndex) REPLAY_SLOT_OFFSET_##slotIndex
#define REPLAY_SLOT_OFFSET(slotIndex) REPLAY_SLOT_OFFSET_I(slotIndex)

#define SET_REPLAY_SLOT_OFFSET_0(value) (REPLAY_SLOT_OFFSET_0 = (value))
#define SET_REPLAY_SLOT_OFFSET_1(value) (REPLAY_SLOT_OFFSET_1 = (value))
#define SET_REPLAY_SLOT_OFFSET_2(value) (REPLAY_SLOT_OFFSET_2 = (value))
#define SET_REPLAY_SLOT_OFFSET_3(value) (REPLAY_SLOT_OFFSET_3 = (value))
#define SET_REPLAY_SLOT_OFFSET_4(value) (REPLAY_SLOT_OFFSET_4 = (value))
#define SET_REPLAY_SLOT_OFFSET_5(value) (REPLAY_SLOT_OFFSET_5 = (value))
#define SET_REPLAY_SLOT_OFFSET_6(value) (REPLAY_SLOT_OFFSET_6 = (value))
#define SET_REPLAY_SLOT_OFFSET_7(value) (REPLAY_SLOT_OFFSET_7 = (value))
#define SET_REPLAY_SLOT_OFFSET_8(value) (REPLAY_SLOT_OFFSET_8 = (value))
#define SET_REPLAY_SLOT_OFFSET_I(slotIndex, value) SET_REPLAY_SLOT_OFFSET_##slotIndex(value)
#define SET_REPLAY_SLOT_OFFSET(slotIndex, value) SET_REPLAY_SLOT_OFFSET_I(slotIndex, value)

#define COPY_SLOT(courseIndex, slotIndex)                                      \
    if (COPY_COURSE_DIFF(courseIndex)) {                                       \
        count = gGameSaveDataBuffer[0].replaySlots[(slotIndex)].length;          \
        if (count != 0) {                                                       \
            oldOffset = REPLAY_SLOT_OFFSET(slotIndex);                         \
            SET_REPLAY_SLOT_OFFSET(slotIndex, writeIndex);                     \
            copied = 0;                                                        \
            if (count > 0) {                                                   \
                do {                                                           \
                    gGameSaveDataBuffer[0].replayData[writeIndex] =               \
                        oldData[oldOffset + copied];                            \
                    copied++;                                                  \
                    AFTER_OLD_COPY(slotIndex);                                 \
                    writeIndex++;                                              \
                } while (copied < gGameSaveDataBuffer[0].replaySlots[(slotIndex)].length); \
            }                                                                  \
        }                                                                      \
    } else {                                                                   \
        SET_REPLAY_SLOT_OFFSET(slotIndex, writeIndex);                         \
        gGameSaveDataBuffer[0].replaySlots[(slotIndex)].length = compressedLength; \
        copied = 0;                                                            \
        if (compressedLength > 0) {                                            \
            do {                                                               \
                gGameSaveDataBuffer[0].replayData[writeIndex] =                   \
                    gCompressedRaceRecordReplayBuffer[copied];                    \
                copied++;                                                      \
                writeIndex++;                                                  \
            } while (copied < compressedLength);                              \
        }                                                                      \
    }

s32 saveRaceRecordReplayData(void) {
    RaceInputHistoryBuffer *history;
    u16 *oldData;
    u16 *src;
    u16 *dst;
    u8 *packed;
    s32 i;
    s32 totalLength;
    s32 compressedLength;
    s32 writeIndex;
    s32 oldOffset;
    s32 copied;
    s32 count;
    s32 course;
    u8 buttons;

    history = getRelocatableHeapBlockBase(ASSET_HANDLE(43));
    if (history->lastWriteIndex >= 0xFD5) {
        return 1;
    }

    gPackedRaceRecordReplayBuffer.fields.frameCount = history->lastWriteIndex;
    gPackedRaceRecordReplayBuffer.fields.characterId = history->characterId;
    packed = gPackedRaceRecordReplayBuffer.bytes;
    gPackedRaceRecordReplayBuffer.fields.characterVariant = history->characterVariant;

    i = 0;
    if (history->lastWriteIndex > 0) {
        do {
            packed[4 + (i * 3)] = history->stickX[i];
            packed[5 + (i * 3)] = history->stickY[i];
            buttons = history->buttons[i];
            ((volatile u8 *)packed)[6 + (i * 3)] = buttons;
            packed[6 + (i * 3)] = buttons & ~0x40;
            i++;
        } while (copied = i < history->lastWriteIndex);
    }

    compressedLength = compressRaceRecordReplayData(gPackedRaceRecordReplayBuffer.bytes,
                                                    (history->lastWriteIndex * 3) + 4,
                                                    gCompressedRaceRecordReplayBuffer);
    if (compressedLength < 0) {
        return 1;
    }

    course = gRaceCourseIndex.signedValue;
    totalLength = 0;
    if (course != 0) {
        totalLength = gGameSaveDataBuffer[0].replaySlots[0].length;
        if (totalLength != 0) {
            if (totalLength >= REPLAY_SAVE_MIN_NORMAL) {
            } else {
                totalLength = REPLAY_SAVE_MAX_NORMAL;
            }
        } else {
            totalLength = REPLAY_SAVE_MAX_NORMAL;
        }
    }
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
    } else if ((course == 9) || (course == 8)) {
        totalLength += REPLAY_SAVE_MAX_EXTRA;
    } else {
        totalLength += REPLAY_SAVE_MAX_NORMAL;
    }

    if (totalLength >= 0x3A81) {
        return 1;
    }

    gAssetHandles[0x20] = allocRelocatableHeapBlock(0x7500);
    oldData = getRelocatableHeapBlockBase(gAssetHandles[0x20]);
    src = gGameSaveDataBuffer[0].replayData;
    dst = oldData;
    do {
        *dst = *src;
        src++;
        dst++;
    } while ((u32)src < (u32)&gGameSaveDataBuffer[0].replayDataTail[0]);

    course = gRaceCourseIndex.signedValue;
    writeIndex = 0;
    COPY_SLOT(0, 0);
    COPY_SLOT(1, 1);
    COPY_SLOT(2, 2);
    COPY_SLOT(3, 3);
    COPY_SLOT(4, 4);
    COPY_SLOT(5, 5);
    COPY_SLOT(6, 6);
    COPY_SLOT(8, 7);
    COPY_SLOT(9, 8);

    freeRelocatableHeapBlock(gAssetHandles[0x20]);
    return 0;
}

#undef COPY_SLOT
#undef SET_REPLAY_SLOT_OFFSET
#undef SET_REPLAY_SLOT_OFFSET_I
#undef SET_REPLAY_SLOT_OFFSET_8
#undef SET_REPLAY_SLOT_OFFSET_7
#undef SET_REPLAY_SLOT_OFFSET_6
#undef SET_REPLAY_SLOT_OFFSET_5
#undef SET_REPLAY_SLOT_OFFSET_4
#undef SET_REPLAY_SLOT_OFFSET_3
#undef SET_REPLAY_SLOT_OFFSET_2
#undef SET_REPLAY_SLOT_OFFSET_1
#undef SET_REPLAY_SLOT_OFFSET_0
#undef REPLAY_SLOT_OFFSET
#undef REPLAY_SLOT_OFFSET_I
#undef REPLAY_SLOT_OFFSET_8
#undef REPLAY_SLOT_OFFSET_7
#undef REPLAY_SLOT_OFFSET_6
#undef REPLAY_SLOT_OFFSET_5
#undef REPLAY_SLOT_OFFSET_4
#undef REPLAY_SLOT_OFFSET_3
#undef REPLAY_SLOT_OFFSET_2
#undef REPLAY_SLOT_OFFSET_1
#undef REPLAY_SLOT_OFFSET_0
#undef AFTER_OLD_COPY
#undef AFTER_OLD_COPY_I
#undef AFTER_OLD_COPY_8
#undef AFTER_OLD_COPY_7
#undef AFTER_OLD_COPY_6
#undef AFTER_OLD_COPY_5
#undef AFTER_OLD_COPY_4
#undef AFTER_OLD_COPY_3
#undef AFTER_OLD_COPY_2
#undef AFTER_OLD_COPY_1
#undef AFTER_OLD_COPY_0
#undef COPY_COURSE_DIFF
#undef COPY_COURSE_DIFF_I
#undef COPY_COURSE_DIFF_9
#undef COPY_COURSE_DIFF_8
#undef COPY_COURSE_DIFF_6
#undef COPY_COURSE_DIFF_5
#undef COPY_COURSE_DIFF_4
#undef COPY_COURSE_DIFF_3
#undef COPY_COURSE_DIFF_2
#undef COPY_COURSE_DIFF_1
#undef COPY_COURSE_DIFF_0
#undef ACCUM_SLOT
#undef REPLAY_SAVE_MIN_EXTRA
#undef REPLAY_SAVE_MAX_EXTRA
#undef REPLAY_SAVE_MIN_NORMAL
#undef REPLAY_SAVE_MAX_NORMAL
#endif

// loadCurrentRaceRecordReplayData best match: 99.673% (nonmatchings/loadCurrentRaceRecordReplayData-6219302648079029720/base_26.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/main_menu/main_menu_scene_model/loadCurrentRaceRecordReplayData.s")

#ifdef NON_MATCHING
void loadCurrentRaceRecordReplayData(void) {
    u16 *compressed;
    u8 *decompressed;
    u16 backReference;
    RaceInputHistoryBuffer *history;
    s32 decompressedLength;
    s32 outputIndex;
    s32 runLength;
    s32 sourceIndex;
    s32 copyIndex;
    s32 frameCount;
    s32 i;

    outputIndex = 0;
    compressed = gGameSaveDataBuffer[0].replayData;
    switch (gRaceCourseIndex.unsignedValue) {
    case 0:
        compressed += gGameSaveDataBuffer[0].replaySlots[0].offset;
        break;
    case 1:
        compressed += gGameSaveDataBuffer[0].replaySlots[1].offset;
        break;
    case 2:
        compressed += gGameSaveDataBuffer[0].replaySlots[2].offset;
        break;
    case 3:
        compressed += gGameSaveDataBuffer[0].replaySlots[3].offset;
        break;
    case 4:
        compressed += gGameSaveDataBuffer[0].replaySlots[4].offset;
        break;
    case 5:
        compressed += gGameSaveDataBuffer[0].replaySlots[5].offset;
        break;
    case 6:
        compressed += gGameSaveDataBuffer[0].replaySlots[6].offset;
        break;
    case 8:
        compressed += gGameSaveDataBuffer[0].replaySlots[7].offset;
        break;
    case 9:
        compressed += gGameSaveDataBuffer[0].replaySlots[8].offset;
        break;
    }

    decompressed = gPackedRaceRecordReplayBuffer.bytes;
    decompressedLength = *compressed++;
    for (;;) {
        copyIndex = 0;
        if (outputIndex >= decompressedLength) {
            break;
        }
        runLength = 0x3F;
        runLength = (*compressed >> 10) & runLength;
        if (runLength == 0) {
            gPackedRaceRecordReplayBuffer.bytes[outputIndex] = *compressed;
            outputIndex++;
            compressed++;
        } else {
            backReference = *compressed;
            sourceIndex = outputIndex - (backReference & 0x3FF);
            for (copyIndex = 0; copyIndex < runLength; copyIndex++) {
                decompressed[outputIndex] = decompressed[sourceIndex + copyIndex];
                outputIndex++;
            }
            compressed++;
        }
    }

    copyIndex = ASSET_HANDLE(7);
    history = getRelocatableHeapBlockBase(copyIndex);
    history->writeIndex = 0;
    sourceIndex = gPackedRaceRecordReplayBuffer.fields.frameCount;
    frameCount = sourceIndex;
    history->lastWriteIndex = frameCount;
    history->enabled = 1;
    history->courseId = ((s16)gRaceCourseIndex.signedValue) & 0xFFFFu;
    history->characterId = gPackedRaceRecordReplayBuffer.fields.characterId;
    history->characterVariant = gPackedRaceRecordReplayBuffer.fields.characterVariant;
    history->unkC = 0;

    i = 0;
    if (frameCount > 0) {
        do {
            history->stickX[i] = gPackedRaceRecordReplayBuffer.fields.inputs[i].stickX & 0xFF;
            backReference = gPackedRaceRecordReplayBuffer.fields.inputs[i].stickY;
            history->stickY[i] = backReference;
            history->buttons[i] = gPackedRaceRecordReplayBuffer.fields.inputs[i].buttons;
            i++;
        } while (i < history->lastWriteIndex);
    }
}

#endif

void loadMainMenuSceneModelAssets(void) {
    LOAD_ASSET(_5E34A0, 0x3F);
}

void loadMainMenuSceneModelAnimationBank(void) {
    LOAD_ASSET(_215BE0, 0x3F);
}

INCLUDE_ASM("asm/matchings/menu/main_menu/main_menu_scene_model", initMainMenuSceneModel);

#ifdef NON_MATCHING
void initMainMenuSceneModel(s32 sceneModelIndex, s32 characterIndex) {
    MainMenuSceneModel *model;

    loadRawRomAsset(gCharacterRawAssetRanges[characterIndex].start,
                    gCharacterRawAssetRanges[characterIndex].end,
                    MAIN_MENU_SCENE_MODEL_GEOMETRY_HANDLE_BASE + sceneModelIndex);
    loadCompressedRomAsset(gCharacterTextureAssetRanges[characterIndex].start,
                           gCharacterTextureAssetRanges[characterIndex].end,
                           MAIN_MENU_SCENE_MODEL_TEXTURE_HANDLE_BASE + sceneModelIndex);
    ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + sceneModelIndex) =
        allocRelocatableHeapBlock(sizeof(MainMenuSceneModel));
    model = getRelocatableHeapBlockBase(
        ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + sceneModelIndex));
    model->sceneModelIndex = sceneModelIndex;
    model->characterIndex = characterIndex;
    initMainMenuSceneModelParts(model);
}
#endif

void setMainMenuSceneModelAnimation(s32 modelIndex, s32 animationIndex) {
    MainMenuModelAnimationBank *animationBank;
    s16 *frameData;
    MainMenuSceneModel *model;
    s16 frameDuration;

    animationBank = (MainMenuModelAnimationBank *)getRelocatableHeapBlockBase(
        ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_ANIMATION_BANK_HANDLE));
    frameData = MAIN_MENU_ANIMATION_FRAME_DATA(animationBank, animationIndex);
    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(
        ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    frameDuration = *frameData++;
    model->framesRemaining = frameDuration;
    model->animationStart = frameData;
    model->framesRemaining++;
    model->animationCursor = frameData;
    model->frameDuration = frameDuration;
}

MainMenuSceneModel *getMainMenuSceneModel(s32 modelIndex) {
    return (MainMenuSceneModel *)getRelocatableHeapBlockBase(
        ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
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

    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(
        ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
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

    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(
        ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    model->framesRemaining--;
    if (model->framesRemaining <= 0) {
        model->framesRemaining = model->frameDuration;
        model->animationCursor = model->animationStart;
    }
    applyMainMenuSceneModelAnimationFrame(model);
}

void setMainMenuSceneModelPosition(s32 modelIndex, s32 x, s32 y, s32 z) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(
        ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    model->pos.x = x;
    model->pos.y = y;
    model->pos.z = z;
}

void setMainMenuSceneModelRotation(s32 modelIndex, s16 x, s16 y, s16 z) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(
        ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    model->rot.x = x;
    model->rot.y = y;
    model->rot.z = z;
}

void updateMainMenuSceneModelTransforms(MainMenuSceneModel *model) {
    MainMenuModelTransform modelTransform;
    MainMenuModelTransform partTransforms[MAIN_MENU_SCENE_MODEL_PART_COUNT];
    {
        s32 sineX;
        s32 cosineX;
        s32 sineY;
        s32 cosineY;
        s32 sineZ;
        s32 cosineZ;
        s32 i;

        i = 0;
        do {
            sineX = fixedSine(model->parts[i].rot.x);
            cosineX = fixedCosine(model->parts[i].rot.x);
            sineY = fixedSine(model->parts[i].rot.y);
            cosineY = fixedCosine(model->parts[i].rot.y);
            sineZ = fixedSine(model->parts[i].rot.z);
            cosineZ = fixedCosine(model->parts[i].rot.z);

            partTransforms[i].rotation[MTX_XX] = (cosineY * cosineZ) / FIXED_MATRIX_ONE;
            partTransforms[i].rotation[MTX_XY] = (cosineY * sineZ) / FIXED_MATRIX_ONE;
            partTransforms[i].rotation[MTX_XZ] = -sineY;
            partTransforms[i].rotation[MTX_YX] =
                (((((sineX * sineY) / FIXED_MATRIX_ONE) * cosineZ) / FIXED_MATRIX_ONE) +
                 ((cosineX * -sineZ) / FIXED_MATRIX_ONE));
            partTransforms[i].rotation[MTX_YY] =
                (((((sineX * sineY) / FIXED_MATRIX_ONE) * sineZ) / FIXED_MATRIX_ONE) +
                 ((cosineX * cosineZ) / FIXED_MATRIX_ONE));
            partTransforms[i].rotation[MTX_YZ] = (sineX * cosineY) / FIXED_MATRIX_ONE;
            partTransforms[i].rotation[MTX_ZX] =
                (((((cosineX * sineY) / FIXED_MATRIX_ONE) * cosineZ) / FIXED_MATRIX_ONE) +
                 ((sineX * sineZ) / FIXED_MATRIX_ONE));
            partTransforms[i].rotation[MTX_ZY] =
                (((((cosineX * sineY) / FIXED_MATRIX_ONE) * sineZ) / FIXED_MATRIX_ONE) +
                 (((-sineX) * cosineZ) / FIXED_MATRIX_ONE));
            partTransforms[i].rotation[MTX_ZZ] = (cosineX * cosineY) / FIXED_MATRIX_ONE;
            i++;
        } while (i != MAIN_MENU_SCENE_MODEL_PART_COUNT);
    }

    makeFixedRotationZXY(modelTransform.rotation, model->rot.x, model->rot.y, model->rot.z);
    modelTransform.translation[0] = model->pos.x;
    modelTransform.translation[1] = model->pos.y;
    modelTransform.translation[2] = model->pos.z;

    {
        s32 i;
        s32 j;
        s32 k;

        for (i = 0; i < MAIN_MENU_SCENE_MODEL_PART_COUNT; i++) {
            for (j = 0; j < MAIN_MENU_SCENE_MODEL_MATRIX_AXES; j++) {
                for (k = 0; k < MAIN_MENU_SCENE_MODEL_MATRIX_AXES; k++) {
                    FIXED_MATRIX_ROWS(model->displayObjects[i].rotation)[j][k] =
                        ((FIXED_MATRIX_ROWS(partTransforms[i].rotation)[j][0] *
                          FIXED_MATRIX_ROWS(modelTransform.rotation)[0][k]) +
                         (FIXED_MATRIX_ROWS(partTransforms[i].rotation)[j][1] *
                          FIXED_MATRIX_ROWS(modelTransform.rotation)[1][k]) +
                         (FIXED_MATRIX_ROWS(partTransforms[i].rotation)[j][2] *
                          FIXED_MATRIX_ROWS(modelTransform.rotation)[2][k])) /
                        FIXED_MATRIX_ONE;
                }
            }
        }

        for (i = 0; i < MAIN_MENU_SCENE_MODEL_PART_COUNT; i++) {
            for (j = 0; j < MAIN_MENU_SCENE_MODEL_MATRIX_AXES; j++) {
                model->displayObjects[i].translation[j] =
                    ((s64)modelTransform.rotation[j] * model->parts[i].offsetX +
                     (s64)modelTransform.rotation[j + MAIN_MENU_SCENE_MODEL_MATRIX_AXES] *
                         model->parts[i].offsetY +
                     (s64)modelTransform.rotation[j + (MAIN_MENU_SCENE_MODEL_MATRIX_AXES * 2)] *
                         model->parts[i + 1].previousPartOffsetZ) /
                    FIXED_MATRIX_ONE;
                model->displayObjects[i].translation[j] += modelTransform.translation[j];
            }
        }
    }
}
