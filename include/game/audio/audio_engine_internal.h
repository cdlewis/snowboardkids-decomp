#ifndef GAME_AUDIO_AUDIO_ENGINE_INTERNAL_H
#define GAME_AUDIO_AUDIO_ENGINE_INTERNAL_H

#include "common.h"
#define alSynSetPan alSynSetPan_u8
#include <PR/libaudio.h>
#undef alSynSetPan
#include <PR/os_ai.h>
#include <PR/os_convert.h>
#include <PR/ucode.h>
#include "game/audio/audio_engine.h"

typedef struct AudioInitTask {
    void *outBuf;
    u8 pad4[0x64];
    s16 type;
    u8 pad6A[2];
    OSMesg msg;
} AudioInitTask;

typedef struct AudioWorkBuffers {
    Acmd *commandLists[2];
    AudioInitTask *tasks[3];
} AudioWorkBuffers;

typedef struct SchedulerThreadMainLocals {
    OSMesgQueue *queue;
    u8 pad4[4];
    OSMesg msg;
} SchedulerThreadMainLocals;

extern const char D_800E1A60[16];

extern s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
extern s32 osSetIntMask(s32);
extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern void osCreateThread(SchedulerThread *, s32, void (*)(void *), void *, void *, s32);
extern void osCreateViManager(s32);
extern void osSetEventMesg(s32, OSMesgQueue *, OSMesg);
extern void osStartThread(SchedulerThread *);
extern void osViBlack(u8);
extern void osViSwapBuffer(void *);
extern void osViSetEvent(OSMesgQueue *, OSMesg, u32);
extern void osViSetMode(SchedulerViMode *);
extern void *osViGetCurrentFramebuffer(void);
extern void *osViGetNextFramebuffer(void);
extern void osWritebackDCacheAll(void);
extern void osSpTaskYield(void);
extern u32 osAiGetLength(void);
extern u32 osVirtualToPhysical(void *);
extern s32 osPiStartDma(OSIoMesg *, s32, s32, u32, void *, u32, OSMesgQueue *);
extern void alSynSetPan(ALSynth *, ALVoice *, s32);
extern void rmonPrintf(const char *, ...);
extern AudioDmaState gAudioDmaState;
extern AudioDmaBuffer *gAudioDmaBufferPool;
extern f32 sinf(f32);
extern s32 gSchedulerYieldResult;
extern u16 gRetraceCounter;
extern s32 gSchedulerYieldRequested;
extern s32 gSchedulerStartupRetraceCount;
extern s32 gAudioThreadStarted;
extern s32 gAudioCmdListIndex;
extern u32 gAudioFrameCounter;
extern u32 gPendingAudioDmaCount;
extern AudioInfo *gNextAudioInfo;
extern s32 gAudioUnderrunState;
extern SchedulerViMode gSchedulerViModes[];
extern u8 gSchedulerThreadStack[];
extern AudioWorkBuffers gAudioWorkBuffers;
extern SchedulerThread gAudioThread;
extern OSMesgQueue gAudioThreadQueue;
extern OSMesg gAudioThreadMessages[];
extern u64 gAudioTaskDoneQueue[];
extern OSMesg gAudioTaskDoneMessages[];
extern u8 gAudioThreadStack[0x2000];
extern s32 mus_current_handle;
extern s32 mus_random_seed;
extern s32 gSchedulerRspTaskState;
extern u8 gSchedulerRdpTaskActive;
extern s32 gMinAudioTaskOutputLen;
extern s32 gTargetAudioTaskOutputLen;
extern s32 gMaxAudioTaskOutputLen;
extern s32 gAudioCmdListCapacity;
extern OSMesgQueue gAudioDmaQueue;
extern SchedulerState *gAudioSchedulerState;
extern OSIoMesg *gAudioDmaMessages;
extern OSMesg *gAudioDmaMessageBuffer;
extern s32 gAudioDmaBufferSize;
extern u8 **libmus_fxheader_current;
extern ALPlayer plr_player;
extern ALHeap gSoundPlayerHeap;
extern u8 *gSoundPlayerHeapEnd;
extern ALVoice *mus_voices;
extern s32 max_channels;
extern PlayerCommandState *mus_channels;
extern ALWaveTable **gSoundWaveTable;
extern s32 gSoundBankEntryCount;
extern PlayerCommandBank *gCurrentSoundBank;
extern u8 *gSoundPlayerTuningTable;
extern f32 *gSoundPlayerPitchOffsets;
extern s32 *gSoundPriorityTable;
extern u16 mus_master_volume_effects;
extern u16 mus_master_volume_songs;
extern s32 mus_vsyncs_per_second;
extern s32 mus_next_frame_time;
extern ALSynth gAudioSynthesizer;
extern s32 osTvType;

extern ALDMAproc initAudioDmaCallback(AudioDmaState **);

#endif
