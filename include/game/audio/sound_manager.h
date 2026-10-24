#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "common.h"
#include "game/audio/sound_ids.h"

/* Thirteen 16-byte slots, managed by the active list and free stack. */
typedef struct SoundHandleNode {
    struct SoundHandleNode *prev;
    struct SoundHandleNode *next;
    s32 handle;
    u16 priority;
    u8 stopRequested;
    u8 volume;
} SoundHandleNode;

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

typedef struct SoundQueueEntry {
    u8 type;
    u8 soundId;
    u8 volume;
    u8 pan;
    u8 priority;
    u8 unk5;
} SoundQueueEntry;

typedef struct SoundRomRange {
    u32 romStart;
    u32 romEnd;
} SoundRomRange;

extern s32 gPlayerPositionalSoundHandle1;
extern s32 gPlayerPositionalSoundHandle2;
extern s32 gPlayerPositionalSoundHandle3;

#if defined(VERSION_JP)
extern u8 D_800D9EE0[];
extern f32 D_800D9FDC[];
extern u8 *D_800DAC2C[];
extern s32 D_800DADDC[];
extern SoundHandleNode *gActiveSoundHandleListHead;
extern SoundHandleNode *gActiveSoundHandleListTail;
extern u16 gCourseMusicSequenceBanks[];
extern s32 gCurrentMusicSequenceBank;
extern s32 gCurrentMusicSequenceHandle;
extern u8 gCurrentQueuedSoundAux;
extern u8 gCurrentQueuedSoundId;
extern u8 gCurrentQueuedSoundPan;
extern u8 gCurrentQueuedSoundPriority;
extern u8 gCurrentQueuedSoundType;
extern u8 gCurrentQueuedSoundVolume;
extern s32 gFreeSoundHandleCount;
extern SoundHandleNode *gFreeSoundHandleStack[];
extern SoundRomRange gMusicSequenceRomRanges[];
extern s32 gMusicSequenceStopped;
extern s32 gPendingMusicCommand;
extern s32 gPendingMusicCommandArg;
extern PositionalSoundRequest *gPendingPositionalSoundRequests;
extern s32 gPlayerLoopingSoundHandle0;
extern s32 gPlayerLoopingSoundHandle1;
extern s32 gPlayerLoopingSoundHandle2;
extern s32 gPlayerLoopingSoundHandle3;
extern s16 gPlayerLoopingSoundId0;
extern s16 gPlayerLoopingSoundId1;
extern s16 gPlayerLoopingSoundId2;
extern s16 gPlayerLoopingSoundId3;
extern s32 gPlayerPositionalSoundHandle0[];
extern SoundRomRange gRaceMusicSequenceRomRanges[];
extern s32 gSharedLoopingPositionalSoundHandle;
extern SoundHandleNode gSoundHandleNodes[];
extern SoundQueueEntry gSoundQueue[];
extern s32 gSoundQueueReadIndex;
extern s32 gSoundQueueWriteIndex;
#endif

#if defined(VERSION_US)
extern u32 gRaceReplayCourseGridEntries[];
#endif

#if defined(VERSION_JP)
s32 calculatePositionalSoundVolume(Vec3i *pos, s32 volume);
s32 reserveSoundEffectQueueWriteIndex(void);
void stopCurrentMusicSequence(s32 arg0);
#endif

void initSoundManager(void);
void updateSoundManager(void);
void fadeOutAllMusicSequences(void);
void requestMusicSequenceBank(s32 arg0);
void requestMusicSequenceStop(s32 arg0);
s32 enqueueSoundEffect(s16 soundId, s16 priority);
void stopSoundEffects(void);
void requestCourseMusicSequence(void);
s32 countActiveMusicSequences(void);
void clearPendingPositionalSoundRequests(void);
void enqueuePositionalSoundRequest(
    s32 soundId,
    Vec3i *pos,
    s32 volume,
    s32 priority,
    f32 pitch,
    s32 mode,
    s32 minVolume
);
void enqueuePlayerLoopingPositionalSoundRequest(s32 soundId, Vec3i *pos, s32 volume, s32 priority, f32 pitch, s16 mode);
void playPendingPositionalSoundRequests(void);
void enqueuePositionalSoundEffect(s16 soundId, Vec3i *pos, s16 volume, s16 priority);
void enqueuePlayerPositionalSoundEffect(
    s16 soundId,
    Vec3i *pos,
    s16 volume,
    s16 priority,
    s16 playerIndex,
    s16 minVolume
);

#endif
