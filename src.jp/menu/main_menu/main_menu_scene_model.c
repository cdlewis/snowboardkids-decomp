#include "game/engine/model_asset_symbols.h"
#include "game/race/race_state.h"
#include "assets.h"
#include "game/save_data.h"
#include "game/menu/main_menu/main_menu_scene_model.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/math/fixed_point_math.h"
#include "game/menu/main_menu/main_menu_scene_model_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/race/race_replay.h"
#include "game/race/player/race_player_model_renderer.h"
#include "game/race/scene/race_scene_setup.h"

#define FIXED_MATRIX_ROWS(matrix) ((s16(*)[MAIN_MENU_SCENE_MODEL_MATRIX_AXES])(matrix))
#define MAIN_MENU_SCENE_MODEL_MATRIX_AXES 3
#define MAIN_MENU_CHARACTER_COUNT 6

typedef struct MainMenuAnimationWritePart {
    s32 word0;
    s32 word4;
    s32 word8;
    s32 wordC;
    s32 word10;
} MainMenuAnimationWritePart;

#define ASSET_HANDLE(index) (gAssetHandles[(index)])

Gfx *gMainMenuSceneModelPartDisplayLists[MAIN_MENU_CHARACTER_COUNT * (MAIN_MENU_SCENE_MODEL_PART_COUNT - 1)] = {
    SLASH_GRAPHICS_command_000000, SLASH_GRAPHICS_command_000068, SLASH_GRAPHICS_command_0000D8,
    SLASH_GRAPHICS_command_000168, SLASH_GRAPHICS_command_0001D8, SLASH_GRAPHICS_command_000268,
    SLASH_GRAPHICS_command_000540, SLASH_GRAPHICS_command_000A28, SLASH_GRAPHICS_command_000AA0,
    SLASH_GRAPHICS_command_000C48, SLASH_GRAPHICS_command_000CC0, SLASH_GRAPHICS_command_000E68,
    SLASH_GRAPHICS_command_0010C0, WENDY_GRAPHICS_command_000000, WENDY_GRAPHICS_command_000128,
    WENDY_GRAPHICS_command_000218, WENDY_GRAPHICS_command_000308, WENDY_GRAPHICS_command_0003F8,
    WENDY_GRAPHICS_command_0004F0, WENDY_GRAPHICS_command_000850, WENDY_GRAPHICS_command_000BE8,
    WENDY_GRAPHICS_command_000D10, WENDY_GRAPHICS_command_000FD0, WENDY_GRAPHICS_command_001128,
    WENDY_GRAPHICS_command_0013E8, WENDY_GRAPHICS_command_001630, JAM_GRAPHICS_command_000000,
    JAM_GRAPHICS_command_0000E0,   JAM_GRAPHICS_command_0001D0,   JAM_GRAPHICS_command_000338,
    JAM_GRAPHICS_command_000428,   JAM_GRAPHICS_command_0004F0,   JAM_GRAPHICS_command_0008A0,
    JAM_GRAPHICS_command_000DD8,   JAM_GRAPHICS_command_000F50,   JAM_GRAPHICS_command_001178,
    JAM_GRAPHICS_command_0012F0,   JAM_GRAPHICS_command_001508,   JAM_GRAPHICS_command_001760,
    LINDA_GRAPHICS_command_000000, LINDA_GRAPHICS_command_0001C8, LINDA_GRAPHICS_command_000378,
    LINDA_GRAPHICS_command_000528, LINDA_GRAPHICS_command_0006D8, LINDA_GRAPHICS_command_000888,
    LINDA_GRAPHICS_command_000CD8, LINDA_GRAPHICS_command_001148, LINDA_GRAPHICS_command_001210,
    LINDA_GRAPHICS_command_0014D0, LINDA_GRAPHICS_command_001590, LINDA_GRAPHICS_command_001850,
    LINDA_GRAPHICS_command_001A98, TOMMY_GRAPHICS_command_000000, TOMMY_GRAPHICS_command_0000E0,
    TOMMY_GRAPHICS_command_0002D0, TOMMY_GRAPHICS_command_0004E0, TOMMY_GRAPHICS_command_0006B0,
    TOMMY_GRAPHICS_command_0008C0, TOMMY_GRAPHICS_command_000CE0, TOMMY_GRAPHICS_command_001298,
    TOMMY_GRAPHICS_command_001408, TOMMY_GRAPHICS_command_001608, TOMMY_GRAPHICS_command_001778,
    TOMMY_GRAPHICS_command_001978, TOMMY_GRAPHICS_command_001BA0, NINJA_GRAPHICS_command_000000,
    NINJA_GRAPHICS_command_0002A0, NINJA_GRAPHICS_command_000330, NINJA_GRAPHICS_command_000518,
    NINJA_GRAPHICS_command_0005B8, NINJA_GRAPHICS_command_0007B0, NINJA_GRAPHICS_command_000CF0,
    NINJA_GRAPHICS_command_0010E8, NINJA_GRAPHICS_command_001170, NINJA_GRAPHICS_command_001410,
    NINJA_GRAPHICS_command_001498, NINJA_GRAPHICS_command_001738, NINJA_GRAPHICS_command_0017C8,
};

s16 gMainMenuSceneModelPartInitDataA[MAIN_MENU_SCENE_MODEL_PART_COUNT * 3] = {
    0, 0,    0, 0,   0x25, 0, -6, -2,  0, 0,   -12, 0, 6, -2,  0, 0, -12, 0,  0, 0,   0,
    0, 0x10, 0, -10, 0xE,  0, -8, -15, 0, 0xA, 0xE, 0, 8, -15, 0, 0, -12, -5, 0, -12, -5,
};

s16 gMainMenuSceneModelPartInitDataB[MAIN_MENU_SCENE_MODEL_PART_COUNT * 3] = {
    0, 0, 0, 0,   0x2B, 0, -6,  -5,  0, 0,   -16, 0, 6,   -5,  0, 0, -16, 0,  0, 0,   0,
    0, 9, 0, -10, 8,    0, -10, -16, 0, 0xA, 8,   0, 0xA, -16, 0, 0, -10, -3, 0, -10, -3,
};

s16 gMainMenuSceneModelPartInitDataC[MAIN_MENU_SCENE_MODEL_PART_COUNT * 3] = {
    0, 0,    0, 0,   0x33, 0, -8,  -3,  0, 0,    -18,  -1, 8,   -3,  0, 1,  -18, -1, 0, 5,   -5,
    0, 0x20, 2, -23, 0x17, 5, -11, -21, 0, 0x17, 0x17, 5,  0xB, -21, 0, -1, -19, -7, 2, -19, -7,
};

s16 *gMainMenuSceneModelPartInitDataByModel[MAIN_MENU_CHARACTER_COUNT + 1] = {
    gMainMenuSceneModelPartInitDataA,
    gMainMenuSceneModelPartInitDataB,
    gMainMenuSceneModelPartInitDataA,
    gMainMenuSceneModelPartInitDataB,
    gMainMenuSceneModelPartInitDataC,
    gMainMenuSceneModelPartInitDataA,
    NULL,
};

extern u8 gCurrentViewportIndex;
extern Gfx *gRegionAllocPtr;

PackedRaceRecordReplay gPackedRaceRecordReplayBuffer;

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
    goto initialize_out;

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
    count = 0;
    cur = src;
    goto after_initialize_out;

initialize_out:
    out = 1 + dst;
    goto loop;

after_initialize_out:
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

#define REPLAY_SAVE_MAX_NORMAL 0x580
#define REPLAY_SAVE_MAX_EXTRA 0x300
#define ACCUM_REPLAY_SLOT(scratch, courseIndex, slotIndex, maxSize)        \
    if (gRaceCourseIndex.signedValue != (courseIndex)) {                   \
        scratch = gGameSaveDataBuffer[0].replaySlots[(slotIndex)].length;  \
        if (gGameSaveDataBuffer[0].replaySlots[(slotIndex)].length != 0) { \
            if (scratch > (maxSize)) {                                     \
                totalLength += scratch;                                    \
            } else {                                                       \
                totalLength += (maxSize);                                  \
            }                                                              \
        } else {                                                           \
            totalLength += (maxSize);                                      \
        }                                                                  \
    }

s32 saveRaceRecordReplayData(void) {
    s32 pad[3];
    RaceInputHistoryBuffer *history;
    u16 *oldData;
    s32 i;
    s32 compressedLength;
    s32 totalLength;
    s32 writeIndex;
    s32 count;

    history = getRelocatableHeapBlockBase(gAssetHandles[43]);
    if (history->lastWriteIndex >= 0xFD5) {
        return 1;
    }

    gPackedRaceRecordReplayBuffer.fields.frameCount = history->lastWriteIndex;
    gPackedRaceRecordReplayBuffer.fields.characterId = history->characterId;
    gPackedRaceRecordReplayBuffer.fields.characterVariant = history->characterVariant;

    for (i = 0; i < history->lastWriteIndex; i++) {
        gPackedRaceRecordReplayBuffer.bytes[4 + (i * 3)] = history->stickX[i];
        gPackedRaceRecordReplayBuffer.bytes[5 + (i * 3)] = history->stickY[i];
        gPackedRaceRecordReplayBuffer.bytes[6 + (i * 3)] = history->buttons[i];
        gPackedRaceRecordReplayBuffer.bytes[6 + (i * 3)] &= ~0x40;
    }

    compressedLength = compressRaceRecordReplayData(
        gPackedRaceRecordReplayBuffer.bytes,
        (history->lastWriteIndex * 3) + 4,
        gCompressedRaceRecordReplayBuffer
    );
    if (compressedLength < 0) {
        return 1;
    }

    totalLength = 0;
    /* writeIndex is not live yet, so the extra-course slots borrow it as scratch. */
    ACCUM_REPLAY_SLOT(count, RACE_COURSE_BIG_SNOWMAN, 0, REPLAY_SAVE_MAX_NORMAL);
    ACCUM_REPLAY_SLOT(count, RACE_COURSE_SUNSET_ROCK, 1, REPLAY_SAVE_MAX_NORMAL);
    ACCUM_REPLAY_SLOT(count, RACE_COURSE_NIGHT_HIGHWAY, 2, REPLAY_SAVE_MAX_NORMAL);
    ACCUM_REPLAY_SLOT(count, RACE_COURSE_GRASS_VALLEY, 3, REPLAY_SAVE_MAX_NORMAL);
    ACCUM_REPLAY_SLOT(count, RACE_COURSE_DIZZY_LAND, 4, REPLAY_SAVE_MAX_NORMAL);
    ACCUM_REPLAY_SLOT(count, RACE_COURSE_QUICKSAND_VALLEY, 5, REPLAY_SAVE_MAX_NORMAL);
    ACCUM_REPLAY_SLOT(count, RACE_COURSE_SILVER_MOUNTAIN, 6, REPLAY_SAVE_MAX_NORMAL);
    ACCUM_REPLAY_SLOT(writeIndex, RACE_COURSE_NINJA_LAND, 7, REPLAY_SAVE_MAX_EXTRA);
    ACCUM_REPLAY_SLOT(writeIndex, RACE_COURSE_ROOKIE_MOUNTAIN, 8, REPLAY_SAVE_MAX_EXTRA);

    if (compressedLength > REPLAY_SAVE_MAX_NORMAL) {
        totalLength += compressedLength;
    } else if (
        (gRaceCourseIndex.signedValue == RACE_COURSE_ROOKIE_MOUNTAIN) ||
        (gRaceCourseIndex.signedValue == RACE_COURSE_NINJA_LAND)
    ) {
        totalLength += REPLAY_SAVE_MAX_EXTRA;
    } else {
        totalLength += REPLAY_SAVE_MAX_NORMAL;
    }

    if (totalLength >= 0x3A81) {
        return 1;
    }

    gAssetHandles[0x20] = allocRelocatableHeapBlock(0x7500);
    oldData = getRelocatableHeapBlockBase(gAssetHandles[0x20]);

    for (i = 0; i < 14976; i++) {
        oldData[i] = gGameSaveDataBuffer[0].replayData[i];
    }

    writeIndex = 0;

    if (gRaceCourseIndex.signedValue != RACE_COURSE_BIG_SNOWMAN) {
        count = gGameSaveDataBuffer[0].replaySlots[0].length;
        if (count != 0) {
            count = gGameSaveDataBuffer[0].replaySlots[0].offset;
            gGameSaveDataBuffer[0].replaySlots[0].offset = writeIndex;
            for (i = 0; i < gGameSaveDataBuffer[0].replaySlots[0].length; i++, writeIndex++) {
                gGameSaveDataBuffer[0].replayData[writeIndex] = oldData[count + i];
            }
        }
    } else {
        gGameSaveDataBuffer[0].replaySlots[0].offset = writeIndex;
        gGameSaveDataBuffer[0].replaySlots[0].length = compressedLength;
        for (i = 0; i < compressedLength; writeIndex++, i++) {
            gGameSaveDataBuffer[0].replayData[writeIndex] = gCompressedRaceRecordReplayBuffer[i];
        }
    }

    if (gRaceCourseIndex.signedValue != RACE_COURSE_SUNSET_ROCK) {
        count = gGameSaveDataBuffer[0].replaySlots[1].length;
        if (count != 0) {
            count = gGameSaveDataBuffer[0].replaySlots[1].offset;
            gGameSaveDataBuffer[0].replaySlots[1].offset = writeIndex;
            for (i = 0; i < gGameSaveDataBuffer[0].replaySlots[1].length; i++, writeIndex++) {
                gGameSaveDataBuffer[0].replayData[writeIndex] = oldData[count + i];
            }
        }
    } else {
        gGameSaveDataBuffer[0].replaySlots[1].offset = writeIndex;
        gGameSaveDataBuffer[0].replaySlots[1].length = compressedLength;
        for (i = 0; i < compressedLength; writeIndex++, i++) {
            gGameSaveDataBuffer[0].replayData[writeIndex] = gCompressedRaceRecordReplayBuffer[i];
        }
    }

    if (gRaceCourseIndex.signedValue != RACE_COURSE_NIGHT_HIGHWAY) {
        count = gGameSaveDataBuffer[0].replaySlots[2].length;
        if (count != 0) {
            count = gGameSaveDataBuffer[0].replaySlots[2].offset;
            gGameSaveDataBuffer[0].replaySlots[2].offset = writeIndex;
            for (i = 0; i < gGameSaveDataBuffer[0].replaySlots[2].length; i++, writeIndex++) {
                gGameSaveDataBuffer[0].replayData[writeIndex] = oldData[count + i];
            }
        }
    } else {
        gGameSaveDataBuffer[0].replaySlots[2].offset = writeIndex;
        gGameSaveDataBuffer[0].replaySlots[2].length = compressedLength;
        for (i = 0; i < compressedLength; writeIndex++, i++) {
            gGameSaveDataBuffer[0].replayData[writeIndex] = gCompressedRaceRecordReplayBuffer[i];
        }
    }

    if (gRaceCourseIndex.signedValue != RACE_COURSE_GRASS_VALLEY) {
        count = gGameSaveDataBuffer[0].replaySlots[3].length;
        if (count != 0) {
            count = gGameSaveDataBuffer[0].replaySlots[3].offset;
            gGameSaveDataBuffer[0].replaySlots[3].offset = writeIndex;
            for (i = 0; i < gGameSaveDataBuffer[0].replaySlots[3].length; i++, writeIndex++) {
                gGameSaveDataBuffer[0].replayData[writeIndex] = oldData[count + i];
            }
        }
    } else {
        gGameSaveDataBuffer[0].replaySlots[3].offset = writeIndex;
        gGameSaveDataBuffer[0].replaySlots[3].length = compressedLength;
        for (i = 0; i < compressedLength; writeIndex++, i++) {
            gGameSaveDataBuffer[0].replayData[writeIndex] = gCompressedRaceRecordReplayBuffer[i];
        }
    }

    if (gRaceCourseIndex.signedValue != RACE_COURSE_DIZZY_LAND) {
        count = gGameSaveDataBuffer[0].replaySlots[4].length;
        if (count != 0) {
            count = gGameSaveDataBuffer[0].replaySlots[4].offset;
            gGameSaveDataBuffer[0].replaySlots[4].offset = writeIndex;
            for (i = 0; i < gGameSaveDataBuffer[0].replaySlots[4].length; i++, writeIndex++) {
                gGameSaveDataBuffer[0].replayData[writeIndex] = oldData[count + i];
            }
        }
    } else {
        gGameSaveDataBuffer[0].replaySlots[4].offset = writeIndex;
        gGameSaveDataBuffer[0].replaySlots[4].length = compressedLength;
        for (i = 0; i < compressedLength; writeIndex++, i++) {
            gGameSaveDataBuffer[0].replayData[writeIndex] = gCompressedRaceRecordReplayBuffer[i];
        }
    }

    if (gRaceCourseIndex.signedValue != RACE_COURSE_QUICKSAND_VALLEY) {
        count = gGameSaveDataBuffer[0].replaySlots[5].length;
        if (count != 0) {
            count = gGameSaveDataBuffer[0].replaySlots[5].offset;
            gGameSaveDataBuffer[0].replaySlots[5].offset = writeIndex;
            for (i = 0; i < gGameSaveDataBuffer[0].replaySlots[5].length; i++, writeIndex++) {
                gGameSaveDataBuffer[0].replayData[writeIndex] = oldData[count + i];
            }
        }
    } else {
        gGameSaveDataBuffer[0].replaySlots[5].offset = writeIndex;
        gGameSaveDataBuffer[0].replaySlots[5].length = compressedLength;
        for (i = 0; i < compressedLength; writeIndex++, i++) {
            gGameSaveDataBuffer[0].replayData[writeIndex] = gCompressedRaceRecordReplayBuffer[i];
        }
    }

    if (gRaceCourseIndex.signedValue != RACE_COURSE_SILVER_MOUNTAIN) {
        count = gGameSaveDataBuffer[0].replaySlots[6].length;
        if (count != 0) {
            count = gGameSaveDataBuffer[0].replaySlots[6].offset;
            gGameSaveDataBuffer[0].replaySlots[6].offset = writeIndex;
            for (i = 0; i < gGameSaveDataBuffer[0].replaySlots[6].length; i++, writeIndex++) {
                gGameSaveDataBuffer[0].replayData[writeIndex] = oldData[count + i];
            }
        }
    } else {
        gGameSaveDataBuffer[0].replaySlots[6].offset = writeIndex;
        gGameSaveDataBuffer[0].replaySlots[6].length = compressedLength;
        for (i = 0; i < compressedLength; writeIndex++, i++) {
            gGameSaveDataBuffer[0].replayData[writeIndex] = gCompressedRaceRecordReplayBuffer[i];
        }
    }

    if (gRaceCourseIndex.signedValue != RACE_COURSE_NINJA_LAND) {
        count = gGameSaveDataBuffer[0].replaySlots[7].length;
        if (count != 0) {
            count = gGameSaveDataBuffer[0].replaySlots[7].offset;
            gGameSaveDataBuffer[0].replaySlots[7].offset = writeIndex;
            for (i = 0; i < gGameSaveDataBuffer[0].replaySlots[7].length; i++, writeIndex++) {
                gGameSaveDataBuffer[0].replayData[writeIndex] = oldData[count + i];
            }
        }
    } else {
        gGameSaveDataBuffer[0].replaySlots[7].offset = writeIndex;
        gGameSaveDataBuffer[0].replaySlots[7].length = compressedLength;
        for (i = 0; i < compressedLength; writeIndex++, i++) {
            gGameSaveDataBuffer[0].replayData[writeIndex] = gCompressedRaceRecordReplayBuffer[i];
        }
    }

    if (gRaceCourseIndex.signedValue != RACE_COURSE_ROOKIE_MOUNTAIN) {
        count = gGameSaveDataBuffer[0].replaySlots[8].length;
        if (count != 0) {
            count = gGameSaveDataBuffer[0].replaySlots[8].offset;
            gGameSaveDataBuffer[0].replaySlots[8].offset = writeIndex;
            for (i = 0; i < gGameSaveDataBuffer[0].replaySlots[8].length; i++, writeIndex++) {
                gGameSaveDataBuffer[0].replayData[writeIndex] = oldData[count + i];
            }
        }
    } else {
        gGameSaveDataBuffer[0].replaySlots[8].offset = writeIndex;
        gGameSaveDataBuffer[0].replaySlots[8].length = compressedLength;
        for (i = 0; i < compressedLength; writeIndex++, i++) {
            gGameSaveDataBuffer[0].replayData[writeIndex] = gCompressedRaceRecordReplayBuffer[i];
        }
    }

    freeRelocatableHeapBlock(gAssetHandles[0x20]);
    return 0;
}

#undef ACCUM_REPLAY_SLOT
#undef REPLAY_SAVE_MAX_EXTRA
#undef REPLAY_SAVE_MAX_NORMAL

void loadCurrentRaceRecordReplayData(void) {
    u16 *compressed;
    u8 *decompressed;
    s32 token;
    RaceInputHistoryBuffer *history;
    s32 decompressedLength;
    s32 outputIndex;
    s32 sourceIndex;
    s32 copyIndex;
    s32 i;

    outputIndex = 0;
    compressed = gGameSaveDataBuffer[0].replayData;
    switch (gRaceCourseIndex.unsignedValue) {
        case RACE_COURSE_BIG_SNOWMAN:
            compressed += gGameSaveDataBuffer[0].replaySlots[0].offset;
            break;
        case RACE_COURSE_SUNSET_ROCK:
            compressed += gGameSaveDataBuffer[0].replaySlots[1].offset;
            break;
        case RACE_COURSE_NIGHT_HIGHWAY:
            compressed += gGameSaveDataBuffer[0].replaySlots[2].offset;
            break;
        case RACE_COURSE_GRASS_VALLEY:
            compressed += gGameSaveDataBuffer[0].replaySlots[3].offset;
            break;
        case RACE_COURSE_DIZZY_LAND:
            compressed += gGameSaveDataBuffer[0].replaySlots[4].offset;
            break;
        case RACE_COURSE_QUICKSAND_VALLEY:
            compressed += gGameSaveDataBuffer[0].replaySlots[5].offset;
            break;
        case RACE_COURSE_SILVER_MOUNTAIN:
            compressed += gGameSaveDataBuffer[0].replaySlots[6].offset;
            break;
        case RACE_COURSE_NINJA_LAND:
            compressed += gGameSaveDataBuffer[0].replaySlots[7].offset;
            break;
        case RACE_COURSE_ROOKIE_MOUNTAIN:
            compressed += gGameSaveDataBuffer[0].replaySlots[8].offset;
            break;
    }

    decompressed = gPackedRaceRecordReplayBuffer.bytes;
    decompressedLength = *compressed++;
    for (;;) {
        if (outputIndex >= decompressedLength) {
            break;
        }
        sourceIndex = token = *compressed;
        if (((token >> 10) & 0x3F) == 0) {
            decompressed[outputIndex] = token & 0xFFFF;
            outputIndex++;
        } else {
            sourceIndex &= 0x3FF;
            sourceIndex = outputIndex - sourceIndex;
            for (copyIndex = 0; copyIndex < ((token >> 10) & 0x3F); copyIndex++) {
                decompressed[outputIndex] = decompressed[sourceIndex + copyIndex];
                outputIndex++;
            }
        }
        compressed++;
    }

    history = getRelocatableHeapBlockBase(ASSET_HANDLE(7));
    history->writeIndex = 0;
    history->lastWriteIndex = gPackedRaceRecordReplayBuffer.fields.frameCount;
    history->enabled = 1;
    history->courseId = gRaceCourseIndex.signedValue;
    history->characterId = gPackedRaceRecordReplayBuffer.fields.characterId;
    history->characterVariant = gPackedRaceRecordReplayBuffer.fields.characterVariant;
    history->unkC = 0;

    for (i = 0; i < history->lastWriteIndex; i++) {
        /* Optimized away by IDO; preserves the target loop-counter register allocation. */
        if ((!gGameSaveDataBuffer[0].replaySlots[1].offset) && (!gGameSaveDataBuffer[0].replaySlots[1].offset)) {}
        history->stickX[i] = gPackedRaceRecordReplayBuffer.fields.inputs[i].stickX;
        history->stickY[i] = gPackedRaceRecordReplayBuffer.fields.inputs[i].stickY;
        history->buttons[i] = gPackedRaceRecordReplayBuffer.fields.inputs[i].buttons;
    }
}

void loadMainMenuSceneModelAssets(void) {
    LOAD_ASSET(_5E34A0, 0x3F);
}

void loadMainMenuSceneModelAnimationBank(void) {
    LOAD_ASSET(_215BE0, 0x3F);
}

void initMainMenuSceneModel(s32 sceneModelIndex, s32 characterIndex) {
    MainMenuSceneModel *model;

    loadRawRomAsset(
        gCharacterRawAssetRanges
            .romBoundAddresses[characterIndex * ROM_ASSET_RANGE_BOUND_COUNT + ROM_ASSET_RANGE_START],
        gCharacterRawAssetRanges.romBoundAddresses[characterIndex * ROM_ASSET_RANGE_BOUND_COUNT + ROM_ASSET_RANGE_END],
        MAIN_MENU_SCENE_MODEL_GEOMETRY_HANDLE_BASE + sceneModelIndex
    );
    loadCompressedRomAsset(
        gCharacterTextureAssetRanges
            .romBoundAddresses[characterIndex * ROM_ASSET_RANGE_BOUND_COUNT + ROM_ASSET_RANGE_START],
        gCharacterTextureAssetRanges
            .romBoundAddresses[characterIndex * ROM_ASSET_RANGE_BOUND_COUNT + ROM_ASSET_RANGE_END],
        MAIN_MENU_SCENE_MODEL_TEXTURE_HANDLE_BASE + sceneModelIndex
    );
    ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + sceneModelIndex) =
        allocRelocatableHeapBlock(sizeof(MainMenuSceneModel));
    model = getRelocatableHeapBlockBase(ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + sceneModelIndex));
    model->sceneModelIndex = sceneModelIndex;
    model->characterIndex = characterIndex;
    initMainMenuSceneModelParts(model);
}

void setMainMenuSceneModelAnimation(s32 modelIndex, s32 animationIndex) {
    s16 *animationBank;
    s16 *frameData;
    MainMenuSceneModel *model;
    s16 frameDuration;

    animationBank = (s16 *)getRelocatableHeapBlockBase(ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_ANIMATION_BANK_HANDLE));
    frameData = &animationBank[((s32 *)animationBank)[animationIndex]];
    model =
        (MainMenuSceneModel *)getRelocatableHeapBlockBase(ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    frameDuration = *frameData++;
    model->framesRemaining = frameDuration;
    model->animationStart = frameData;
    model->framesRemaining++;
    model->animationCursor = frameData;
    model->frameCount = frameDuration;
}

MainMenuSceneModel *getMainMenuSceneModel(s32 modelIndex) {
    return (MainMenuSceneModel *)getRelocatableHeapBlockBase(
        ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex)
    );
}

void applyMainMenuSceneModelAnimationFrame(MainMenuSceneModel *model) {
    s16 *cursor;
    s32 i;
    MainMenuAnimationWritePart *writePart;
    CharacterModelPart *part;

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
    } while (i < MAIN_MENU_SCENE_MODEL_PART_COUNT);

    for (i = 0; i < MAIN_MENU_SCENE_MODEL_PART_COUNT; i++) {
        part = &model->parts[i];
        part->rotation.x = *cursor++;
        part->rotation.y = *cursor++;
        part->rotation.z = *cursor++;
    }

    model->animationCursor = cursor;
}

s32 stepMainMenuSceneModelAnimation(s32 modelIndex) {
    int new_var2;
    MainMenuSceneModel *new_var;
    MainMenuSceneModel *model;

    model =
        (MainMenuSceneModel *)getRelocatableHeapBlockBase(ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
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

    model =
        (MainMenuSceneModel *)getRelocatableHeapBlockBase(ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    model->framesRemaining--;
    if (model->framesRemaining <= 0) {
        model->framesRemaining = model->frameCount;
        model->animationCursor = model->animationStart;
    }
    applyMainMenuSceneModelAnimationFrame(model);
}

void setMainMenuSceneModelPosition(s32 modelIndex, s32 x, s32 y, s32 z) {
    MainMenuSceneModel *model;

    model =
        (MainMenuSceneModel *)getRelocatableHeapBlockBase(ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    model->position.x = x;
    model->position.y = y;
    model->position.z = z;
}

void setMainMenuSceneModelRotation(s32 modelIndex, s16 x, s16 y, s16 z) {
    MainMenuSceneModel *model;

    model =
        (MainMenuSceneModel *)getRelocatableHeapBlockBase(ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    model->rotation.x = x;
    model->rotation.y = y;
    model->rotation.z = z;
}

void updateMainMenuSceneModelTransforms(MainMenuSceneModel *model) {
    Transform3D rootTransform;
    Transform3D localPartTransforms[MAIN_MENU_SCENE_MODEL_PART_COUNT];
    {
        s32 sineX;
        s32 cosineX;
        s32 sineY;
        s32 cosineY;
        s32 sineZ;
        s32 cosineZ;
        s32 partIndex;

        partIndex = 0;
        do {
            sineX = fixedSine(model->parts[partIndex].rotation.x);
            cosineX = fixedCosine(model->parts[partIndex].rotation.x);
            sineY = fixedSine(model->parts[partIndex].rotation.y);
            cosineY = fixedCosine(model->parts[partIndex].rotation.y);
            sineZ = fixedSine(model->parts[partIndex].rotation.z);
            cosineZ = fixedCosine(model->parts[partIndex].rotation.z);

            localPartTransforms[partIndex].rotation[MTX_XX] = (cosineY * cosineZ) / FIXED_MATRIX_ONE;
            localPartTransforms[partIndex].rotation[MTX_XY] = (cosineY * sineZ) / FIXED_MATRIX_ONE;
            localPartTransforms[partIndex].rotation[MTX_XZ] = -sineY;
            localPartTransforms[partIndex].rotation[MTX_YX] =
                (((((sineX * sineY) / FIXED_MATRIX_ONE) * cosineZ) / FIXED_MATRIX_ONE) +
                 ((cosineX * -sineZ) / FIXED_MATRIX_ONE));
            localPartTransforms[partIndex].rotation[MTX_YY] =
                (((((sineX * sineY) / FIXED_MATRIX_ONE) * sineZ) / FIXED_MATRIX_ONE) +
                 ((cosineX * cosineZ) / FIXED_MATRIX_ONE));
            localPartTransforms[partIndex].rotation[MTX_YZ] = (sineX * cosineY) / FIXED_MATRIX_ONE;
            localPartTransforms[partIndex].rotation[MTX_ZX] =
                (((((cosineX * sineY) / FIXED_MATRIX_ONE) * cosineZ) / FIXED_MATRIX_ONE) +
                 ((sineX * sineZ) / FIXED_MATRIX_ONE));
            localPartTransforms[partIndex].rotation[MTX_ZY] =
                (((((cosineX * sineY) / FIXED_MATRIX_ONE) * sineZ) / FIXED_MATRIX_ONE) +
                 (((-sineX) * cosineZ) / FIXED_MATRIX_ONE));
            localPartTransforms[partIndex].rotation[MTX_ZZ] = (cosineX * cosineY) / FIXED_MATRIX_ONE;
            partIndex++;
        } while (partIndex != MAIN_MENU_SCENE_MODEL_PART_COUNT);
    }

    makeFixedRotationZXY(rootTransform.rotation, model->rotation.x, model->rotation.y, model->rotation.z);
    rootTransform.translation.x = model->position.x;
    rootTransform.translation.y = model->position.y;
    rootTransform.translation.z = model->position.z;

    {
        s32 partIndex;
        s32 row;
        s32 column;

        for (partIndex = 0; partIndex < MAIN_MENU_SCENE_MODEL_PART_COUNT; partIndex++) {
            for (row = 0; row < MAIN_MENU_SCENE_MODEL_MATRIX_AXES; row++) {
                for (column = 0; column < MAIN_MENU_SCENE_MODEL_MATRIX_AXES; column++) {
                    FIXED_MATRIX_ROWS(model->partTransforms[partIndex].rotation)
                    [row][column] = ((FIXED_MATRIX_ROWS(localPartTransforms[partIndex].rotation)[row][0] *
                                      FIXED_MATRIX_ROWS(rootTransform.rotation)[0][column]) +
                                     (FIXED_MATRIX_ROWS(localPartTransforms[partIndex].rotation)[row][1] *
                                      FIXED_MATRIX_ROWS(rootTransform.rotation)[1][column]) +
                                     (FIXED_MATRIX_ROWS(localPartTransforms[partIndex].rotation)[row][2] *
                                      FIXED_MATRIX_ROWS(rootTransform.rotation)[2][column])) /
                                    FIXED_MATRIX_ONE;
                }
            }
        }

        for (partIndex = 0; partIndex < MAIN_MENU_SCENE_MODEL_PART_COUNT; partIndex++) {
            for (row = 0; row < MAIN_MENU_SCENE_MODEL_MATRIX_AXES; row++) {
                ((s32 *)&model->partTransforms[partIndex].translation)[row] =
                    ((s64)FIXED_MATRIX_ROWS(rootTransform.rotation)[0][row] * model->parts[partIndex].offset.x +
                     (s64)FIXED_MATRIX_ROWS(rootTransform.rotation)[1][row] * model->parts[partIndex].offset.y +
                     (s64)FIXED_MATRIX_ROWS(rootTransform.rotation)[2][row] * model->parts[partIndex].offset.z) /
                    FIXED_MATRIX_ONE;
                ((s32 *)&model->partTransforms[partIndex].translation)[row] += ((s32 *)&rootTransform.translation)[row];
            }
        }
    }
}

void initMainMenuSceneModelRenderer(void) {
}

void initMainMenuSceneModelRenderer_pad(void) {
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void drawMainMenuSceneModel(MainMenuSceneModel *arg0) {
    Transform3D *transform;
    Mtx *matrix;
    s32 partIndex;
    s32 displayListCount;

    if ((u16)arg0->viewportIndex == gCurrentViewportIndex) {
        gDPPipeSync(gRegionAllocPtr++);
        gSPSegment(gRegionAllocPtr++, 0x02,
                   getRelocatableHeapBlockBase(
                       ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_GEOMETRY_HANDLE_BASE +
                                    (u16)arg0->sceneModelIndex)));
        gSPSegment(gRegionAllocPtr++, 0x03,
                   getRelocatableHeapBlockBase(
                       ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_TEXTURE_HANDLE_BASE +
                                    (u16)arg0->sceneModelIndex)));

        displayListCount = MAIN_MENU_SCENE_MODEL_PART_COUNT - 1;
        partIndex = 1; transform = &arg0->partTransforms[1]; do {
            matrix = allocFixedTransformMatrix(transform);
            if (matrix != NULL) {
                gSPMatrix(gRegionAllocPtr++, matrix,
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(
                    gRegionAllocPtr++,
                    gMainMenuSceneModelPartDisplayLists[
                        ((u16)arg0->characterIndex * displayListCount) +
                        partIndex - 1]);
            }
            partIndex++;
            transform++;
        } while (partIndex != MAIN_MENU_SCENE_MODEL_PART_COUNT);

        /* Keep arg0 live through the loop exit for the original register allocation. */
        if (arg0 == NULL) {
        }
    }
}
// clang-format on

void drawTexturedMainMenuSceneModel(MainMenuSceneModel *arg0) {
    MainMenuSceneModel *model;
    Gfx **displayLists;
    Mtx *matrix;
    s32 i;
    s32 stride;

    do {
        if ((u16)arg0->viewportIndex == gCurrentViewportIndex) {
            matrix = allocFixedTransformMatrix(arg0->partTransforms);
            model = arg0;
            if (matrix != NULL) {
                drawSnowboardModel(matrix, model->snowboardDisplayListIndex, model->snowboardTextureIndex);
            }

            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(
                gRegionAllocPtr++,
                0x02,
                getRelocatableHeapBlockBase(
                    gAssetHandles[MAIN_MENU_SCENE_MODEL_GEOMETRY_HANDLE_BASE + (u16)model->sceneModelIndex]
                )
            );
            gSPSegment(
                gRegionAllocPtr++,
                0x03,
                getRelocatableHeapBlockBase(
                    gAssetHandles[MAIN_MENU_SCENE_MODEL_TEXTURE_HANDLE_BASE + (u16)model->sceneModelIndex]
                )
            );

            stride = MAIN_MENU_SCENE_MODEL_PART_COUNT - 1;
            displayLists = gMainMenuSceneModelPartDisplayLists;
            for (i = 1; i < MAIN_MENU_SCENE_MODEL_PART_COUNT; i++) {
                matrix = allocFixedTransformMatrix(&model->partTransforms[i]);
                if (matrix != NULL) {
                    gSPMatrix(gRegionAllocPtr++, matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    gSPDisplayList(gRegionAllocPtr++, displayLists[((u16)model->characterIndex * stride) + i - 1]);
                }
            }
        }
    } while (0);
}

void addMainMenuSceneModelDrawCallback(s32 modelIndex) {
    MainMenuSceneModel *model;

    model =
        (MainMenuSceneModel *)getRelocatableHeapBlockBase(ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    updateMainMenuSceneModelTransforms(model);
    model->viewportIndex = 0;
    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)drawMainMenuSceneModel, model);
}

void addMainMenuSceneModelTexturedDrawCallback(
    s32 modelIndex,
    s32 snowboardDisplayListIndex,
    s32 snowboardTextureIndex
) {
    MainMenuSceneModel *model;

    model =
        (MainMenuSceneModel *)getRelocatableHeapBlockBase(ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    updateMainMenuSceneModelTransforms(model);
    model->viewportIndex = 0;
    model->snowboardDisplayListIndex = (s16)snowboardDisplayListIndex;
    model->snowboardTextureIndex = (s16)snowboardTextureIndex;
    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)drawTexturedMainMenuSceneModel, model);
}

void addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(
    s32 modelIndex,
    s32 snowboardDisplayListIndex,
    s32 snowboardTextureIndex,
    s32 unusedArg
) {
    MainMenuSceneModel *model;

    model =
        (MainMenuSceneModel *)getRelocatableHeapBlockBase(ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    updateMainMenuSceneModelTransforms(model);
    model->viewportIndex = 0;
    model->snowboardDisplayListIndex = (s16)snowboardDisplayListIndex;
    model->snowboardTextureIndex = (s16)snowboardTextureIndex;
    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)drawTexturedMainMenuSceneModel, model);
}

void addMainMenuSceneModelDrawCallbackForViewport0(s32 modelIndex) {
    MainMenuSceneModel *model;

    model =
        (MainMenuSceneModel *)getRelocatableHeapBlockBase(ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    updateMainMenuSceneModelTransforms(model);
    model->viewportIndex = 0;
    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)drawMainMenuSceneModel, model);
}

void addMainMenuSceneModelDrawCallbackForViewport(s32 modelIndex, s32 viewportIndex) {
    MainMenuSceneModel *model;

    model =
        (MainMenuSceneModel *)getRelocatableHeapBlockBase(ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    updateMainMenuSceneModelTransforms(model);
    model->viewportIndex = (s16)viewportIndex;
    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)drawMainMenuSceneModel, model);
}

void initMainMenuSceneModelParts(MainMenuSceneModel *model) {
    s16 *cursor;
    s32 i;
    s8 rootParentIndex = -1;

    model->parts[0].parentPartIndex = rootParentIndex;
    model->parts[1].parentPartIndex = rootParentIndex;
    model->parts[2].parentPartIndex = 1;
    model->parts[3].parentPartIndex = 2;
    model->parts[4].parentPartIndex = 1;
    model->parts[5].parentPartIndex = 4;
    model->parts[6].parentPartIndex = 1;
    model->parts[7].parentPartIndex = 6;
    model->parts[8].parentPartIndex = 6;
    model->parts[9].parentPartIndex = 8;
    model->parts[10].parentPartIndex = 6;
    model->parts[11].parentPartIndex = 10;
    model->parts[12].parentPartIndex = 3;
    model->parts[13].parentPartIndex = 5;

    cursor = gMainMenuSceneModelPartInitDataByModel[(u16)model->characterIndex];
    for (i = 0; i < MAIN_MENU_SCENE_MODEL_PART_COUNT; i++) {
        model->parts[i].rotation.x = model->parts[i].rotation.y = model->parts[i].rotation.z = 0;
        model->parts[i].offset.x = cursor[0] << 16;
        model->parts[i].offset.y = cursor[1] << 16;
        model->parts[i].offset.z = cursor[2] << 16;
        cursor += 3;
    }
}
