#include "common.h"
#include "assets.h"
#include "game/engine/asset_manager.h"
#include "game/math/fixed_point_math.h"
#include "game/engine/relocatable_heap.h"
#include "game/audio/sound_manager.h"
#include "game/engine/system_runtime.h"
#include "libmus/libmus.h"
#include "game/race/player/race_player_input.h"

#define SOUND_MANAGER_FREE_HANDLE_COUNT 13
#define SOUND_MANAGER_QUEUE_CAPACITY 64
#define SOUND_MANAGER_QUEUE_MASK (SOUND_MANAGER_QUEUE_CAPACITY - 1)

typedef struct SoundHandleNode {
    struct SoundHandleNode *prev;
    struct SoundHandleNode *next;
    s32 handle;
    u16 priority;
    u8 stopRequested;
    u8 volume;
} SoundHandleNode;

typedef struct SoundQueueEntry {
    u8 type;
    u8 soundId;
    u8 volume;
    u8 pan;
    u8 priority;
    u8 unk5;
} SoundQueueEntry;

typedef struct PositionalSoundRequest {
    struct PositionalSoundRequest *next;
    Vec3i pos;
    s16 soundId;
    s16 volume;
    s16 minVolume;
    s16 priority;
    s16 mode;
    f32 pitch;
} PositionalSoundRequest;

typedef struct AudioCamera {
    u8 pad0[0x94];
    Vec3i prevPos;
    u8 padA0[0xAC - 0xA0];
    s8 initialized;
    u8 padAD[3];
} AudioCamera;

typedef struct SoundRomRange {
    s32 words[2];
} SoundRomRange;

typedef struct SoundAssetHandles {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
} SoundAssetHandles;

typedef union SoundHalfArg {
    s32 word;
    struct {
        s16 unused;
        s16 value;
    } half;
} SoundHalfArg;

extern SoundAssetHandles gAssetHandles;
extern s16 gRaceCourseIndex;
extern u8 gRaceDemoPlaybackEnabled;
extern u8 D_27E290[];
extern u8 D_800DABB0[];
extern f32 D_800DACAC[];
extern u8 *D_800DB8FC[];
extern s32 D_800DBAAC[];
extern s32 gPendingMusicCommand;
extern s32 gCurrentMusicSequenceHandle;
extern s32 gMusicSequenceStopped;
extern s32 gCurrentMusicSequenceBank;
extern s32 gSoundQueueWriteIndex;
extern s32 gSoundQueueReadIndex;
extern s32 gPlayerPositionalSoundHandle1;
extern s32 gPlayerPositionalSoundHandle2;
extern s32 gPlayerPositionalSoundHandle3;
extern s32 gPlayerLoopingSoundHandle0;
extern s32 gPlayerLoopingSoundHandle1;
extern s32 gPlayerLoopingSoundHandle2;
extern s32 gPlayerLoopingSoundHandle3;
extern s16 gPlayerLoopingSoundId0;
extern s16 gPlayerLoopingSoundId1;
extern s16 gPlayerLoopingSoundId2;
extern s16 gPlayerLoopingSoundId3;
extern s32 gSharedLoopingPositionalSoundHandle;
extern SoundHandleNode gSoundHandleNodes[];
extern SoundHandleNode *gActiveSoundHandleListHead;
extern SoundHandleNode *gActiveSoundHandleListTail;
extern s32 gFreeSoundHandleCount;
extern SoundHandleNode *gFreeSoundHandleStack[];
extern SoundQueueEntry gSoundQueue[];
extern SoundRomRange gMusicSequenceRomRanges[];
extern SoundRomRange gRaceMusicSequenceRomRanges[];
extern s32 gPendingMusicCommandArg;
extern u16 gCourseMusicSequenceBanks[];
extern AudioCamera D_801121E0[];
extern AudioCamera D_801124A0[];
extern s32 gPlayerPositionalSoundHandle0[];
extern u8 gCurrentQueuedSoundType;
extern u8 gCurrentQueuedSoundId;
extern u8 gCurrentQueuedSoundVolume;
extern u8 gCurrentQueuedSoundPan;
extern u8 gCurrentQueuedSoundPriority;
extern u8 gCurrentQueuedSoundAux;
extern SchedulerThread gAudioThread;
extern SchedulerState gSchedulerState;

void osStartThread(void *);
void osStopThread(void *);
void *allocRenderCallbackScratch(s32 arg0);
void requestMusicSequenceBank(s32 arg0);
s32 reserveSoundEffectQueueWriteIndex(void);
s32 startCurrentQueuedSoundEffect(void);
s32 calculatePositionalSoundVolume(Vec3i *pos, s32 volume);
void updatePlayerLoopingPositionalSound(s32 soundId, s32 mode, s32 volume, f32 pitch);

extern void enqueuePositionalSoundRequest(s32, Vec3i *, s32, s32, f32, s32, s32);

void initSoundManager(void) {
    PlayerCommandInit init;
    void *nullPtr;
    s32 pad0;
    s32 pad1;
    SoundHandleNode **freeList;
    SoundHandleNode *node;

    gPendingMusicCommand = 0;
    gPendingMusicCommandArg = 0;
    gMusicSequenceStopped = 1;
    nullPtr = NULL;
    gCurrentMusicSequenceHandle = 0;
    gCurrentMusicSequenceBank = -1;

    do { freeList = gFreeSoundHandleStack; node = gSoundHandleNodes; do { *freeList = node; node++; freeList++; } while (node < (SoundHandleNode *)&gActiveSoundHandleListHead); gFreeSoundHandleCount = 13; gCurrentQueuedSoundType = 0; } while (0);

    gSoundQueueWriteIndex = 0;
    gSoundQueueReadIndex = 0;
    gActiveSoundHandleListHead = nullPtr;
    gActiveSoundHandleListTail = nullPtr;
    gPlayerPositionalSoundHandle0[0] = 0;
    gPlayerLoopingSoundHandle0 = 0;
    gPlayerLoopingSoundId0 = 0;
    gPlayerPositionalSoundHandle1 = 0;
    gPlayerLoopingSoundHandle1 = 0;
    gPlayerLoopingSoundId1 = 0;
    gPlayerPositionalSoundHandle2 = 0;
    gPlayerLoopingSoundHandle2 = 0;
    gPlayerLoopingSoundId2 = 0;
    gPlayerPositionalSoundHandle3 = 0;
    gPlayerLoopingSoundHandle3 = 0;
    gPlayerLoopingSoundId3 = 0;
    gSharedLoopingPositionalSoundHandle = 0;

    gAssetHandles.unk6 = allocRelocatableHeapBlock(0x80000);
    LOAD_ASSET(_275A90, 4);
    gAssetHandles.unkA = allocRelocatableHeapBlock(0x10000);

    init.count = 0x18;
    init.unk4 = &gSchedulerState;
    init.outputRate = 0x6E;
    init.heapBase = (u8 *)getRelocatableHeapBlockBase(gAssetHandles.unk6);
    init.heapLen = 0x80000;
    init.soundBank = (PlayerCommandBank *)getRelocatableHeapBlockBase(gAssetHandles.unk8);
    init.sampleBaseOffset = (s32)D_27E290;
    init.tuningTable = D_800DABB0;
    init.pitchOffsetTable = D_800DACAC;
    init.fxHeader = D_800DB8FC;
    init.unk28 = D_800DBAAC;
    init.maxUpdates = 0x100;
    init.maxFXBusses = 0x5622;
    init.unk34 = 0x2000;
    init.unk38 = 1;
    init.unk3C = 0x20;
    init.unk40 = 0x1000;
    MusInitialize(&init);
    MusSetMasterVolume(1, 0x7FFF);
}

void stopCurrentMusicSequence(s32 arg0) {
    if (gMusicSequenceStopped == 0) {
        if (gCurrentMusicSequenceHandle != 0) {
            MusHandleStop(gCurrentMusicSequenceHandle, arg0);
        }
        gMusicSequenceStopped = 1;
    }
}

s32 loadMusicSequenceBank(s32 arg0) {
    s32 size;
    SoundRomRange *range;

    if (gCurrentMusicSequenceHandle == 0) {
        range = (SoundRomRange *)((arg0 * 2) + (s32 *)gMusicSequenceRomRanges);
        size = range->words[1] - range->words[0];
        dmaReadRom(range->words[0], getRelocatableHeapBlockBase(gAssetHandles.unkA), size);
        if ((gCurrentMusicSequenceHandle = MusStartSong((PlayerCommandData *)getRelocatableHeapBlockBase(gAssetHandles.unkA))) != 0) {
            gCurrentMusicSequenceBank = arg0;
            if (range == gRaceMusicSequenceRomRanges) {
                MusSetMasterVolume(2, 0x7FFF);
            } else {
                MusSetMasterVolume(2, 0x60FF);
            }
            gMusicSequenceStopped = 0;
            return 0;
        }
    } else {
        stopCurrentMusicSequence(4);
    }
    return 1;
}

s32 reserveSoundEffectQueueWriteIndex(void) {
    u32 ret;
    s32 temp_v1;

    temp_v1 = gSoundQueueWriteIndex;
    ret = -1;
    if (gSoundQueueReadIndex == ((temp_v1 + 1) & SOUND_MANAGER_QUEUE_MASK)) {
        return ret;
    }
    (&gSoundQueueWriteIndex)[(gSoundQueueReadIndex == ((temp_v1 + 1) & SOUND_MANAGER_QUEUE_MASK)) * 0] =
        (temp_v1 + 1) & SOUND_MANAGER_QUEUE_MASK;
    return temp_v1;
}

s32 reserveSoundEffectQueueReadIndex(void) {
    u32 ret;
    s32 temp_v1;

    temp_v1 = gSoundQueueReadIndex;
    ret = -1;
    if (temp_v1 == gSoundQueueWriteIndex) {
        return ret;
    }
    (&gSoundQueueReadIndex)[(temp_v1 == gSoundQueueWriteIndex) * 0] = (temp_v1 + 1) & SOUND_MANAGER_QUEUE_MASK;
    return temp_v1;
}

void releaseSoundEffectHandleNode(SoundHandleNode *arg0) {
    SoundHandleNode *temp_v0;
    SoundHandleNode *temp_v1;

    temp_v0 = arg0->prev;
    if (temp_v0 == NULL) {
        gActiveSoundHandleListHead = arg0->next;
        temp_v1 = arg0->next;
        if (temp_v1 != NULL) {
            temp_v1->prev = arg0->prev;
        } else {
            gActiveSoundHandleListTail = NULL;
        }
    } else {
        temp_v0->next = arg0->next;
        temp_v1 = arg0->next;
        if (temp_v1 != NULL) {
            temp_v1->prev = arg0->prev;
        } else {
            gActiveSoundHandleListTail = arg0->prev;
        }
    }
    gFreeSoundHandleStack[gFreeSoundHandleCount] = arg0;
    gFreeSoundHandleCount++;
}

SoundHandleNode *acquireSoundEffectHandleNode(void) {
    s32 count;

    count = gFreeSoundHandleCount;
    if (count == 0) {
        return NULL;
    }
    gFreeSoundHandleCount--;
    return gFreeSoundHandleStack[gFreeSoundHandleCount];
}

s32 startCurrentQueuedSoundEffect(void) {
    SoundHandleNode *temp_v0;
    SoundHandleNode *temp_v0_2;
    SoundHandleNode *var_v1;
    s32 var_a0;

    temp_v0 = acquireSoundEffectHandleNode();
    if (temp_v0 == NULL) {
        if (gCurrentQueuedSoundPriority < gActiveSoundHandleListTail->priority) {
            return 1;
        }
        if (gActiveSoundHandleListTail->stopRequested != 0) {
            gActiveSoundHandleListTail->stopRequested = 0;
            MusHandleStop(gActiveSoundHandleListTail->handle, 0);
        }
        return 0;
    }

    var_a0 = gCurrentQueuedSoundId;
    if ((var_a0 >= 0x70) || (var_a0 < 0)) {
        var_a0 = (gCurrentQueuedSoundId = 0);
    }

    temp_v0->handle = MusStartEffect2(var_a0, gCurrentQueuedSoundVolume, gCurrentQueuedSoundPan, 0, gCurrentQueuedSoundPriority);
    temp_v0->volume = gCurrentQueuedSoundAux;
    temp_v0->priority = gCurrentQueuedSoundPriority;
    temp_v0->stopRequested = 1;

    if (gActiveSoundHandleListHead == NULL) {
        temp_v0->prev = gActiveSoundHandleListHead;
        temp_v0->next = NULL;
        gActiveSoundHandleListHead = (gActiveSoundHandleListTail = temp_v0);
        return 1;
    }

    var_v1 = gActiveSoundHandleListHead;
    if (gActiveSoundHandleListHead != NULL) {
        do {
            if (temp_v0->priority >=
                (s32)((u64)((((var_v1->priority & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFFULL)) {
                temp_v0_2 = var_v1->prev;
                temp_v0->next = var_v1;
                temp_v0->prev = temp_v0_2;
                if (temp_v0_2 == NULL) {
                    gActiveSoundHandleListHead = temp_v0;
                } else {
                    temp_v0_2->next = temp_v0;
                }
                var_v1->prev = temp_v0;
                return 1;
            }
            var_v1 = var_v1->next;
        } while (var_v1 != NULL);
    }

    temp_v0->next = gActiveSoundHandleListTail->next;
    temp_v0->prev = gActiveSoundHandleListTail;
    gActiveSoundHandleListTail->next = temp_v0;
    gActiveSoundHandleListTail = temp_v0;
    return 1;
}

void updateSoundManager(void) {
    SoundHandleNode *node;
    s32 *left;
    s32 *right;
    SoundQueueEntry *entry;
    s32 index;

    osStopThread(&gAudioThread);

    node = gActiveSoundHandleListHead;
    while (node != NULL) {
        if (node->handle == 0) {
            releaseSoundEffectHandleNode(node);
            goto next_node;
        } else {
            if (MusHandleAsk(node->handle) == 0) {
                releaseSoundEffectHandleNode(node);
            }
        }
    next_node:
        node = node->next;
    }

    right = &gPlayerLoopingSoundHandle0, left = gPlayerPositionalSoundHandle0;
    do {
        if ((*right != 0) && (MusHandleAsk(*right) == 0)) {
            *right = 0;
        }
        if ((*left != 0) && (MusHandleAsk(*left) == 0)) {
            *left = 0;
        }
        right++;
    } while (&gPlayerLoopingSoundHandle0 > ++left);

    if ((gSharedLoopingPositionalSoundHandle != 0) && (MusHandleAsk(gSharedLoopingPositionalSoundHandle) == 0)) {
        gSharedLoopingPositionalSoundHandle = 0;
    }

    if (gCurrentQueuedSoundType == 0) {
        index = reserveSoundEffectQueueReadIndex();
        if (index != -1) {
            entry = &gSoundQueue[index];
            gCurrentQueuedSoundType = entry->type;
            gCurrentQueuedSoundId = entry->soundId;
            gCurrentQueuedSoundVolume = entry->volume;
            gCurrentQueuedSoundPan = entry->pan;
            gCurrentQueuedSoundPriority = entry->priority;
            gCurrentQueuedSoundAux = entry->unk5;
        }
    }

    if ((gCurrentQueuedSoundType == 1) && (startCurrentQueuedSoundEffect() != 0)) {
        gCurrentQueuedSoundType = 0;
    }

    if ((gCurrentMusicSequenceHandle != 0) && (MusHandleAsk(gCurrentMusicSequenceHandle) == 0)) {
        gCurrentMusicSequenceHandle = 0;
        gCurrentMusicSequenceBank = -1;
    }

    if ((gPendingMusicCommand == 1) && (loadMusicSequenceBank(gPendingMusicCommandArg) == 0)) {
        gPendingMusicCommand = 0;
    }

    if (gPendingMusicCommand == 2) {
        stopCurrentMusicSequence(gPendingMusicCommandArg);
        gPendingMusicCommand = 0;
    }

    osStartThread(&gAudioThread);
}

void requestMusicSequenceBank(s32 arg0) {
    if (arg0 != gCurrentMusicSequenceBank) {
        gCurrentMusicSequenceBank = arg0;
        gPendingMusicCommand = 1;
        gPendingMusicCommandArg = arg0;
    }
}

void requestMusicSequenceStop(s32 arg0) {
    gPendingMusicCommand = 2;
    gPendingMusicCommandArg = arg0;
    gCurrentMusicSequenceBank = -1;
}

s32 enqueueSoundEffect(s16 arg0, s16 arg1) {
    s32 temp_v0 = reserveSoundEffectQueueWriteIndex();
    SoundQueueEntry *temp_v1;

    if (temp_v0 == -1) {
        return 1;
    }
    temp_v1 = &gSoundQueue[temp_v0];
    temp_v1->type = 1;
    temp_v1->soundId = arg0;
    temp_v1->volume = 0xFF;
    temp_v1->pan = 0x80;
    temp_v1->unk5 = 0;
    temp_v1->priority = arg1;
    return 0;
}

s32 enqueueSoundEffectWithVolume(s16 arg0, s16 arg1, s16 arg2) {
    s32 temp_v0;
    SoundQueueEntry *temp_v1;

    if (arg1 <= 0) {
        return 0;
    }
    temp_v0 = reserveSoundEffectQueueWriteIndex();
    if (temp_v0 == -1) {
        return 1;
    }
    if (arg1 >= 0x100) {
        arg1 = 0xFF;
    }
    temp_v1 = &gSoundQueue[temp_v0];
    temp_v1->type = 1;
    temp_v1->soundId = (u8)arg0;
    temp_v1->volume = (u8)arg1;
    temp_v1->pan = 0x80;
    temp_v1->unk5 = 0;
    temp_v1->priority = (u8)arg2;
    return 0;
}

void stopSoundEffects(void) {
    osStopThread(&gAudioThread);
    gCurrentQueuedSoundType = 0;
    gSoundQueueWriteIndex = 0;
    gSoundQueueReadIndex = 0;
    MusStop(1, 0);
    osStartThread(&gAudioThread);
}

void fadeOutAllMusicSequences(void) {
    osStopThread(&gAudioThread);
    MusStop(3, 0x14);
    osStartThread(&gAudioThread);
}

s32 calculatePositionalSoundVolume(Vec3i *pos, s32 volume) {
    AudioCamera *camera;
    s32 distance;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 attenuation;
    s32 adjustedVolume;

    distance = 0x4000000;
    if (volume <= 0) {
        return 0;
    }

    if (volume >= 0x80) {
        volume = 0x7F;
    }

    camera = D_801121E0;
    do {
        if (camera->initialized == 1) {
            dx = camera->prevPos.x - pos->x;
            if ((dx >= -0x4000000) && (dx < 0x4000001)) {
                dy = camera->prevPos.y - pos->y;
                if (((camera->prevPos.y - pos->y) >= -0x4000000) && (dy < 0x4000001)) {
                    dz = camera->prevPos.z - pos->z;
                    if ((dz >= -0x4000000) && (dz < 0x4000001)) {
                        adjustedVolume = integerSquareRoot64((s64)dx * dx + (s64)dy * dy + (s64)dz * dz);
                        if (adjustedVolume < distance) {
                            distance = adjustedVolume;
                        }
                    }
                }
            }
        }
        camera++;
    } while (camera != D_801124A0);

    attenuation = 0x1000 - fixedSine(distance / 0x10000);
    adjustedVolume = (volume * attenuation) / 0x1000;
    if (adjustedVolume < 0) {
        adjustedVolume = 0;
    }
    return adjustedVolume;
}

void updatePlayerLoopingPositionalSound(s32 soundId, s32 mode, s32 volume, f32 pitch) {
    s32 adjustedVolume;
    s32 activeCameras;

    if ((f64)pitch > 6.0) {
        pitch = 6.0f;
    }
    if ((f64)pitch < -6.0) {
        pitch = -6.0f;
    }

    adjustedVolume = calculatePositionalSoundVolume(&gRacePlayers[mode].pos, volume);

    activeCameras = 0;
    if (D_801121E0[0].initialized != 0) {
        activeCameras = 1;
    }
    if (D_801121E0[1].initialized != 0) {
        activeCameras += 1;
    }
    if (D_801121E0[2].initialized != 0) {
        activeCameras += 1;
    }
    if (D_801121E0[3].initialized != 0) {
        activeCameras += 1;
    }
    if (gRaceDemoPlaybackEnabled != 0) {
        activeCameras = 4;
    }

    adjustedVolume = adjustedVolume - ((((activeCameras * 10) - 1) * adjustedVolume) / 100);

    if (adjustedVolume == 0) {
        if (*(&gPlayerLoopingSoundHandle0 + mode) != 0) {
            MusHandleStop(*(&gPlayerLoopingSoundHandle0 + mode), 0);
            *(&gPlayerLoopingSoundHandle0 + mode) = 0;
        }
    } else {
        if ((*(&gPlayerLoopingSoundHandle0 + mode) != 0) && (soundId != *(&gPlayerLoopingSoundId0 + mode))) {
            MusHandleStop(*(&gPlayerLoopingSoundHandle0 + mode), 0);
            *(&gPlayerLoopingSoundHandle0 + mode) = 0;
        }

        if (*(&gPlayerLoopingSoundHandle0 + mode) == 0) {
            *(&gPlayerLoopingSoundId0 + mode) = soundId;
            *(&gPlayerLoopingSoundHandle0 + mode) = MusStartEffect2(soundId, adjustedVolume, 0x80, 0, 0x46);
            MusHandleSetFreqOffset(*(&gPlayerLoopingSoundHandle0 + mode), pitch);
        } else {
            MusHandleSetVolume(*(&gPlayerLoopingSoundHandle0 + mode), adjustedVolume);
            MusHandleSetFreqOffset(*(&gPlayerLoopingSoundHandle0 + mode), pitch);
        }
    }
}

void playPlayerPositionalSound(s32 soundId, s32 playerIndex, s32 volume, s32 minVolume) {
    s32 adjustedVolume;

    adjustedVolume = calculatePositionalSoundVolume(&gRacePlayers[playerIndex].pos, volume);
    if (adjustedVolume < minVolume) {
        adjustedVolume = minVolume;
    }
    if (adjustedVolume != 0) {
        if (gPlayerPositionalSoundHandle0[playerIndex] != 0) {
            MusHandleStop(gPlayerPositionalSoundHandle0[playerIndex], 0);
            gPlayerPositionalSoundHandle0[playerIndex] = 0;
        }
        gPlayerPositionalSoundHandle0[playerIndex] = MusStartEffect2(soundId, adjustedVolume, 0x80, 0, 0x5A);
    }
}

void updateSingleLoopingPositionalSound(s16 soundId, Vec3i *pos, s16 volume) {
    s32 adjustedVolume;

    adjustedVolume = calculatePositionalSoundVolume(pos, volume);
    if (adjustedVolume == 0) {
        if (gSharedLoopingPositionalSoundHandle != 0) {
            MusHandleStop(gSharedLoopingPositionalSoundHandle, 0);
            gSharedLoopingPositionalSoundHandle = 0;
        }
    } else if (gSharedLoopingPositionalSoundHandle == 0) {
        gSharedLoopingPositionalSoundHandle = MusStartEffect2(soundId, adjustedVolume, 0x80, 0, 0x32);
    } else {
        MusHandleSetVolume(gSharedLoopingPositionalSoundHandle, adjustedVolume);
    }
}

void requestCourseMusicSequence(void) {
    requestMusicSequenceBank(gCourseMusicSequenceBanks[gRaceCourseIndex]);
}

void countActiveSoundPlayers(void) {
    MusAsk(3);
}

s32 countActiveMusicSequences(void) {
    return MusAsk(2);
}

extern PositionalSoundRequest *gPendingPositionalSoundRequests;

void clearPendingPositionalSoundRequests(void) {
    gPendingPositionalSoundRequests = NULL;
}

void enqueuePositionalSoundRequest(s32 soundId, Vec3i *pos, s32 volume, s32 priority, f32 pitch, s32 mode,
                                   s32 minVolume) {
    PositionalSoundRequest *node;

    node = allocRenderCallbackScratch(sizeof(PositionalSoundRequest));
    if (node != NULL) {
        node->next = gPendingPositionalSoundRequests;
        node->pos = *pos;
        node->soundId = ((SoundHalfArg *)&soundId)->half.value;
        node->volume = ((SoundHalfArg *)&volume)->half.value;
        node->minVolume = ((SoundHalfArg *)&minVolume)->half.value;
        node->priority = ((SoundHalfArg *)&priority)->half.value;
        node->mode = ((SoundHalfArg *)&mode)->half.value;
        node->pitch = pitch;
        gPendingPositionalSoundRequests = node;
    }
}

void enqueuePlayerLoopingPositionalSoundRequest(s32 arg0, Vec3i *arg1, s32 arg2, s32 arg3, f32 arg4, s16 mode) {
    s32 temp_a0 = arg0 << 16;
    s32 temp_a2 = arg2 << 16;
    s32 temp_a3 = arg3 << 16;

    enqueuePositionalSoundRequest(temp_a0 >> 16, arg1, temp_a2 >> 16, temp_a3 >> 16, arg4, mode, 0);
}

void enqueuePositionalSoundEffect(s16 arg0, Vec3i *arg1, s16 arg2, s16 arg3) {
    enqueuePositionalSoundRequest(arg0, arg1, arg2, arg3, 0.0f, -1, 0);
}

void enqueuePlayerPositionalSoundEffect(s16 arg0, Vec3i *arg1, s16 arg2, s16 arg3, s16 arg4, s16 minVolume) {
    enqueuePositionalSoundRequest(arg0, arg1, arg2, arg3, 0.0f, arg4 + 4, minVolume);
}

void playPendingPositionalSoundRequests(void) {
    PositionalSoundRequest *node;
    s16 mode;

    osStopThread(&gAudioThread);
    node = gPendingPositionalSoundRequests;

    while (node != NULL) {
        mode = node->mode;
        switch (mode) {
            case -1:
                enqueueSoundEffectWithVolume(node->soundId, calculatePositionalSoundVolume(&node->pos, node->volume), node->priority);
                break;

            case 0:
            case 1:
            case 2:
            case 3:
                updatePlayerLoopingPositionalSound(node->soundId, mode, node->volume, node->pitch);
                break;

            case 4:
            case 5:
            case 6:
            case 7:
                playPlayerPositionalSound(node->soundId, mode - 4, node->volume, node->minVolume);
                break;

            case 10:
                updateSingleLoopingPositionalSound(node->soundId, &node->pos, node->volume);
                break;
        }
        node = node->next;
    }

    osStartThread(&gAudioThread);
}
