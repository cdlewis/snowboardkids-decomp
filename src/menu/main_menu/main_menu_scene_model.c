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

/* Frame offsets are halfword-relative to the bank start; this form preserves target addu order. */
#define MAIN_MENU_ANIMATION_FRAME_DATA(bank, index) \
    ((s16 *)(((bank)->frameOffsets[(index)] * sizeof(s16)) + (s32)(bank)))
#define FIXED_MATRIX_ONE 0x1000
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
    (Gfx *)0x02000000, (Gfx *)0x02000068, (Gfx *)0x020000D8, (Gfx *)0x02000168, (Gfx *)0x020001D8, (Gfx *)0x02000268,
    (Gfx *)0x02000540, (Gfx *)0x02000A28, (Gfx *)0x02000AA0, (Gfx *)0x02000C48, (Gfx *)0x02000CC0, (Gfx *)0x02000E68,
    (Gfx *)0x020010C0, (Gfx *)0x02000000, (Gfx *)0x02000128, (Gfx *)0x02000218, (Gfx *)0x02000308, (Gfx *)0x020003F8,
    (Gfx *)0x020004F0, (Gfx *)0x02000850, (Gfx *)0x02000BE8, (Gfx *)0x02000D10, (Gfx *)0x02000FD0, (Gfx *)0x02001128,
    (Gfx *)0x020013E8, (Gfx *)0x02001630, (Gfx *)0x02000000, (Gfx *)0x020000E0, (Gfx *)0x020001D0, (Gfx *)0x02000338,
    (Gfx *)0x02000428, (Gfx *)0x020004F0, (Gfx *)0x020008A0, (Gfx *)0x02000DD8, (Gfx *)0x02000F50, (Gfx *)0x02001178,
    (Gfx *)0x020012F0, (Gfx *)0x02001508, (Gfx *)0x02001760, (Gfx *)0x02000000, (Gfx *)0x020001C8, (Gfx *)0x02000378,
    (Gfx *)0x02000528, (Gfx *)0x020006D8, (Gfx *)0x02000888, (Gfx *)0x02000CD8, (Gfx *)0x02001148, (Gfx *)0x02001210,
    (Gfx *)0x020014D0, (Gfx *)0x02001590, (Gfx *)0x02001850, (Gfx *)0x02001A98, (Gfx *)0x02000000, (Gfx *)0x020000E0,
    (Gfx *)0x020002D0, (Gfx *)0x020004E0, (Gfx *)0x020006B0, (Gfx *)0x020008C0, (Gfx *)0x02000CE0, (Gfx *)0x02001298,
    (Gfx *)0x02001408, (Gfx *)0x02001608, (Gfx *)0x02001778, (Gfx *)0x02001978, (Gfx *)0x02001BA0, (Gfx *)0x02000000,
    (Gfx *)0x020002A0, (Gfx *)0x02000330, (Gfx *)0x02000518, (Gfx *)0x020005B8, (Gfx *)0x020007B0, (Gfx *)0x02000CF0,
    (Gfx *)0x020010E8, (Gfx *)0x02001170, (Gfx *)0x02001410, (Gfx *)0x02001498, (Gfx *)0x02001738, (Gfx *)0x020017C8,
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
#define ACCUM_REPLAY_SLOT(scratch, courseIndex, slotIndex, maxSize)                \
    if (gRaceCourseIndex.signedValue != (courseIndex)) {                           \
        scratch = gGameSaveDataBuffer[0].replaySlots[(slotIndex)].length;          \
        if (gGameSaveDataBuffer[0].replaySlots[(slotIndex)].length != 0) {         \
            if (scratch > (maxSize)) {                                             \
                totalLength += scratch;                                            \
            } else {                                                               \
                totalLength += (maxSize);                                          \
            }                                                                      \
        } else {                                                                   \
            totalLength += (maxSize);                                              \
        }                                                                          \
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
    ACCUM_REPLAY_SLOT(count, 0, 0, REPLAY_SAVE_MAX_NORMAL);
    ACCUM_REPLAY_SLOT(count, 1, 1, REPLAY_SAVE_MAX_NORMAL);
    ACCUM_REPLAY_SLOT(count, 2, 2, REPLAY_SAVE_MAX_NORMAL);
    ACCUM_REPLAY_SLOT(count, 3, 3, REPLAY_SAVE_MAX_NORMAL);
    ACCUM_REPLAY_SLOT(count, 4, 4, REPLAY_SAVE_MAX_NORMAL);
    ACCUM_REPLAY_SLOT(count, 5, 5, REPLAY_SAVE_MAX_NORMAL);
    ACCUM_REPLAY_SLOT(count, 6, 6, REPLAY_SAVE_MAX_NORMAL);
    ACCUM_REPLAY_SLOT(writeIndex, 8, 7, REPLAY_SAVE_MAX_EXTRA);
    ACCUM_REPLAY_SLOT(writeIndex, 9, 8, REPLAY_SAVE_MAX_EXTRA);

    if (compressedLength > REPLAY_SAVE_MAX_NORMAL) {
        totalLength += compressedLength;
    } else if ((gRaceCourseIndex.signedValue == 9) || (gRaceCourseIndex.signedValue == 8)) {
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

    if (gRaceCourseIndex.signedValue != 0) {
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

    if (gRaceCourseIndex.signedValue != 1) {
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

    if (gRaceCourseIndex.signedValue != 2) {
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

    if (gRaceCourseIndex.signedValue != 3) {
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

    if (gRaceCourseIndex.signedValue != 4) {
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

    if (gRaceCourseIndex.signedValue != 5) {
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

    if (gRaceCourseIndex.signedValue != 6) {
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

    if (gRaceCourseIndex.signedValue != 8) {
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

    if (gRaceCourseIndex.signedValue != 9) {
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
        gCharacterRawAssetRanges
            .romBoundAddresses[characterIndex * ROM_ASSET_RANGE_BOUND_COUNT + ROM_ASSET_RANGE_END],
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
    MainMenuAnimationBank *animationBank;
    s16 *frameData;
    MainMenuSceneModel *model;
    s16 frameDuration;

    animationBank = (MainMenuAnimationBank *)getRelocatableHeapBlockBase(
        ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_ANIMATION_BANK_HANDLE)
    );
    frameData = MAIN_MENU_ANIMATION_FRAME_DATA(animationBank, animationIndex);
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
                    model->partTransforms[partIndex].rotationRows[row][column] =
                        ((localPartTransforms[partIndex].rotationRows[row][0] *
                          rootTransform.rotationRows[0][column]) +
                         (localPartTransforms[partIndex].rotationRows[row][1] *
                          rootTransform.rotationRows[1][column]) +
                         (localPartTransforms[partIndex].rotationRows[row][2] *
                          rootTransform.rotationRows[2][column])) /
                        FIXED_MATRIX_ONE;
                }
            }
        }

        for (partIndex = 0; partIndex < MAIN_MENU_SCENE_MODEL_PART_COUNT; partIndex++) {
            for (row = 0; row < MAIN_MENU_SCENE_MODEL_MATRIX_AXES; row++) {
                model->partTransforms[partIndex].translationElements[row] =
                    ((s64)rootTransform.rotationRows[0][row] * model->parts[partIndex].offset.x +
                     (s64)rootTransform.rotationRows[1][row] * model->parts[partIndex].offset.y +
                     (s64)rootTransform.rotationRows[2][row] * model->parts[partIndex].offset.z) /
                    FIXED_MATRIX_ONE;
                model->partTransforms[partIndex].translationElements[row] += rootTransform.translationElements[row];
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
