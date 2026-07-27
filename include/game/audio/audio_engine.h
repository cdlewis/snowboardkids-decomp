#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include "common.h"
#include <PR/libaudio.h>
#include <PR/sptask.h>

typedef void *OSMesg;

typedef struct OSMesgQueue {
    void *mtqueue;
    void *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;

typedef struct SchedulerThread {
    u8 pad[0x1B0];
} SchedulerThread;

typedef struct SchedulerViMode {
    u8 pad[0x50];
} SchedulerViMode;

typedef struct SchedulerClient {
    struct SchedulerClient *next;
    OSMesgQueue *queue;
} SchedulerClient;

typedef struct AudioThreadLocals {
    SchedulerClient client;
    OSMesg msg;
    s32 pad;
} AudioThreadLocals;

typedef struct PlayerCommandData {
    s32 trackCount;
    u8 **sequenceOffsets;
    u8 **unk8;
    u8 **unkC;
    u8 *commands;
    s32 *jumpTable;
} PlayerCommandData;

typedef struct PlayerCommandClearBlock {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} PlayerCommandClearBlock;

typedef struct DrumEntry {
    u8 wave;
    u8 envelope;
    u8 pan;
    u8 pitch;
} DrumEntry;

typedef struct PlayerCommandState {
    u32 channel_frame;
    u32 unk4;
    u32 unk8;
    u32 note_end_frame;
    u32 note_start_frame;
    s32 id;
    s32 fadeTarget;
    s32 fadeTime;
    s32 unk20;
    f32 cachedPitch;
    f32 length_float;
    f32 finePitchOffset;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 last_note;
    f32 count_float;
    f32 vibratoDepth;
    f32 vibratoPitchOffset;
    f32 port_base;
    f32 handlePitchOffset;
    PlayerCommandData *song_addr;
    u8 *pdata;
    u8 *restartPos;
    u8 *unk60;
    u8 *unk64;
    u8 *unk68;
    u8 *unk6C;
    DrumEntry *pdrums;
    u8 *returnScripts[5];
    u8 *returnUnk60[5];
    u8 *returnUnk68[5];
    s16 unkB0;
    s16 unkB2;
    s16 unkB4;
    u16 old_volume;
    s16 unkB8;
    u16 unkBA;
    u16 length;
    u16 soundId;
    u16 fixed_length;
    u16 unkC2;
    u16 unkC4;
    u16 count;
    u16 unkC8;
    u16 unkCA;
    u16 wave;
    u16 returnUnkC8[5];
    u16 returnUnkCA[5];
    u8 old_reverb;
    u8 old_pan;
    u8 playing;
    u8 trigger_off;
    u8 ignore;
    u8 skipPitchOffsetOnce;
    u8 env_trigger_off;
    u8 unkE9;
    u8 portamentoTime;
    u8 unkEB;
    u8 returnDepth;
    u8 velocity_on;
    u8 default_velocity;
    u8 unkEF;
    u8 pitchBendValue;
    u8 pitchBendDepth;
    s8 pan;
    u8 reverb;
    u8 unkF4;
    u8 env_init_vol;
    u8 unkF6;
    u8 unkF7;
    u8 env_phase;
    u8 env_current;
    u8 env_count;
    u8 unkFB;
    u8 unkFC;
    u8 unkFD;
    u8 note;
    u8 base_note;
    u32 release_frame;
    u8 release_start_vol;
    u8 unk105;
    u8 wobble_off_speed;
    u8 wobble_count;
    u8 velocity;
    u8 returnUnk109[5];
    u8 returnUnk10E[5];
    u8 returnUnk113[5];
    s8 pitchOffset;
    s8 unk119;
    s8 wobble_current;
    u8 pad11B;
} PlayerCommandState;

typedef struct SchedulerTask {
    struct SchedulerTask *next;
    u32 state;
    u32 flags;
    void *framebuffer;
    OSTask rspTask;
    OSMesgQueue *doneQueue;
    OSMesg doneMsg;
    s16 retrace;
    u8 pad5A[6];
} SchedulerTask;

typedef struct SchedulerState {
    s16 unk0;
    s16 unk2;
    OSMesgQueue messageQueue;
    OSMesg messageMsgs[16];
    OSMesgQueue eventQueue;
    OSMesg eventMsgs[16];
    OSMesgQueue retraceQueue;
    OSMesg retraceMsgs[32];
    OSMesgQueue queue14C;
    OSMesg msgs164[16];
    OSMesgQueue queue1A4;
    OSMesg msgs1BC[16];
    OSMesgQueue framebufferQueue;
    OSMesg framebufferMsgs[16];
    u8 pad254[4];
    SchedulerThread thread258;
    SchedulerThread thread408;
    u8 pad5B8[0x768 - 0x5B8];
    SchedulerClient *clients;
    SchedulerTask *curRSPTask;
    SchedulerTask *curRDPTask;
    s32 doAudio;
} SchedulerState;

typedef struct AudioDmaState {
    u8 initialized;
    u8 pad1[3];
    ALLink *activeList;
    ALLink *readyList;
} AudioDmaState;

typedef struct AudioDmaBuffer {
    ALLink node;
    s32 addr;
    u32 counter;
    void *buffer;
} AudioDmaBuffer;

typedef struct AudioSynthInitConfig {
    s32 outputRate;
    s32 frameRate;
    s32 commandListSize;
} AudioSynthInitConfig;

typedef struct OSIoMesg {
    u8 pad[0x18];
} OSIoMesg;

typedef struct PlayerCommandBank {
    u8 pad0[0x10];
    s32 waveCount;
    s32 waveTableOffsets[1];
} PlayerCommandBank;

typedef struct PlayerCommandInit {
    s32 count;
    void *unk4;
    s32 outputRate;
    u8 *heapBase;
    s32 heapLen;
    PlayerCommandBank *soundBank;
    s32 sampleBaseOffset;
    u8 *tuningTable;
    f32 *pitchOffsetTable;
    u8 **fxHeader;
    s32 *unk28;
    s32 maxUpdates;
    s32 maxFXBusses;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
} PlayerCommandInit;

typedef struct AudioInfo {
    void *buf;
    s16 len;
} AudioInfo;

typedef struct AudioFrameMessage {
    s16 type;
    s16 unk2;
    AudioInfo *info;
} AudioFrameMessage;

typedef struct AudioTask {
    void *outBuf;
    s16 outLen;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u32 type;
    u32 flags;
    void *ucodeBoot;
    u32 ucodeBootSize;
    void *ucode;
    u32 ucodeSize;
    void *ucodeData;
    u32 ucodeDataSize;
    void *dramStack;
    u32 dramStackSize;
    void *outputBuff;
    void *outputBuffSize;
    void *dataPtr;
    u32 dataSize;
    void *yieldDataPtr;
    u32 yieldDataSize;
    OSMesgQueue *msgQ;
    OSMesg msg;
    u8 pad60[8];
    u8 unk68;
} AudioTask;

void initScheduler(SchedulerState *arg0, u8 arg1, u8 arg2);
OSMesgQueue *getSchedulerAudioTaskQueue(SchedulerState *scheduler);
OSMesgQueue *getSchedulerGraphicsTaskQueue(SchedulerState *scheduler);
void schedulerThreadMain(SchedulerState *scheduler);
void schedulerThreadEntry(void *arg);
void tryStartPendingRdpTask(SchedulerState *arg0);
void startCurrentRdpTask(SchedulerState *arg0);
void finishCurrentRdpTask(SchedulerState *arg0);
void schedulerSwapBufferThreadMain(SchedulerState *scheduler);
void addSchedulerClient(SchedulerState *arg0, SchedulerClient *arg1, OSMesgQueue *arg2);
void removeSchedulerClient(SchedulerState *arg0, SchedulerClient *arg1);
void notifySchedulerClients(SchedulerState *arg0, s32 arg1);
void waitForFramebufferAvailable(SchedulerState *arg0, SchedulerTask *arg1);
void waitForNextFramebufferEvent(SchedulerState *arg0);
u8 *Fstop(PlayerCommandState *arg0, u8 *arg1);
void *Fwave(PlayerCommandState *arg0, u8 *arg1);
s32 Fport(PlayerCommandState *arg0, u8 *arg1);
s32 Fportoff(PlayerCommandState *arg0, s32 arg1);
s32 Fdefa(PlayerCommandState *arg0, u8 *arg1);
s32 Ftempo(PlayerCommandState *arg0, u8 *arg1);
s32 Fendit(PlayerCommandState *arg0, u8 *arg1);
s32 Fcutoff(PlayerCommandState *arg0, u8 *arg1);
s32 Fvibup(PlayerCommandState *arg0, u8 *arg1);
s32 Fvibdown(PlayerCommandState *arg0, u8 *arg1);
s32 Fviboff(PlayerCommandState *arg0, s32 arg1);
s32 Flength(PlayerCommandState *arg0, u8 *arg1);
s32 Fignore(PlayerCommandState *arg0, s32 arg1);
s32 Ftrans(PlayerCommandState *arg0, u8 *arg1);
s32 Fignore_trans(PlayerCommandState *arg0, s32 arg1);
s32 Fdistort(PlayerCommandState *arg0, u8 *arg1);
s32 Fenvelope(PlayerCommandState *arg0, u8 *arg1);
s32 Fenvoff(PlayerCommandState *arg0, s32 arg1);
s32 Fenvon(PlayerCommandState *arg0, s32 arg1);
s32 Ftroff(PlayerCommandState *arg0, s32 arg1);
s32 Ftron(PlayerCommandState *arg0, s32 arg1);
s32 Ffor(PlayerCommandState *arg0, u8 *arg1);
s32 Fnext(PlayerCommandState *arg0, s32 arg1);
s32 Fwobble(PlayerCommandState *arg0, u8 *arg1);
s32 Fwobbleoff(PlayerCommandState *arg0, s32 arg1);
s32 Fvelon(PlayerCommandState *arg0, s32 arg1);
s32 Fveloff(PlayerCommandState *arg0, s32 arg1);
s32 Fvelocity(PlayerCommandState *arg0, u8 *arg1);
s32 Fpan(PlayerCommandState *arg0, u8 *arg1);
s32 Fstereo(s32 arg0, s32 arg1);
void *Fdrums(void *arg0, u8 *arg1);
s32 Fdrumsoff(PlayerCommandState *arg0, s32 arg1);
s32 Fprint(s32 arg0, s32 arg1);
u8 *Fgoto(PlayerCommandState *arg0, u8 *arg1);
s32 Freverb(PlayerCommandState *arg0, u8 *arg1);
s32 FrandNote(PlayerCommandState *arg0, u8 *arg1);
u8 *FrandVolume(PlayerCommandState *arg0, u8 *arg1);
u8 *FrandPan(PlayerCommandState *arg0, u8 *arg1);
s32 Fvolume(PlayerCommandState *arg0, u8 *arg1);
s32 Fstartfx(PlayerCommandState *arg0, u8 *arg1);
s32 Fbendrange(PlayerCommandState *arg0, u8 *arg1);
s32 MusInitialize(PlayerCommandInit *arg0);
void MusSetMasterVolume(s32 arg0, s32 arg1);
s32 MusStartSong(PlayerCommandData *arg0);
s32 MusStartEffect(s32 arg0);
s32 MusStartEffect2(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void MusStop(s32 arg0, s32 arg1);
s32 MusAsk(s32 arg0);
s32 MusHandleStop(s32 arg0, s32 arg1);
s32 MusHandleAsk(s32 arg0);
s32 MusHandleSetVolume(s32 arg0, s32 arg1);
s32 MusHandleSetPan(s32 arg0, s32 arg1);
s32 MusHandleSetFreqOffset(s32 arg0, f32 arg1);
s32 MusHandleSetTempo(s32 arg0, s32 arg1);
ALMicroTime __MusIntMain(void *arg0);
void __MusIntGetNewNote(PlayerCommandState *arg0, s32 arg1);
void __MusIntSetVolumeAndPan(PlayerCommandState *arg0, s32 arg1);
void __MusIntSetPitch(PlayerCommandState *arg0, s32 arg1);
void __MusIntInitEnvelope(PlayerCommandState *arg0);
void __MusIntProcessEnvelope(PlayerCommandState *arg0);
void __MusIntProcessWobble(PlayerCommandState *arg0);
void __MusIntProcessVibrato(PlayerCommandState *arg0);
void __MusIntProcessContinuousVolume(PlayerCommandState *arg0);
void __MusIntProcessContinuousPitchBend(PlayerCommandState *arg0);
f32 __MusIntPowerOf2(f32 arg0);
void __MusIntRemapPtrBank(PlayerCommandBank *bank, s32 sampleBaseOffset);
s32 __MusIntRandom(s32 range);
s32 __MusIntRandomWithContext(s32 range, u8 *sequencePos, PlayerCommandState *state);
void __MusIntInitialiseChannel(PlayerCommandState *arg0);
s32 __MusIntFindChannel(s32 arg0, s32 arg1);
void __MusIntMemSet(void *p, unsigned char c, s32 n);
s32 __MusIntStartEffect(PlayerCommandState *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void initAudioSynthesizer(SchedulerState *scheduler, ALSynConfig *config, s32 threadPriority,
                          AudioSynthInitConfig *initConfig, s32 dmaBufferCount, s32 dmaBufferSize,
                          s32 retraceRate);
void audioThreadMain(void *arg0);
s32 buildAudioTask(AudioTask *task, AudioInfo *info);
void updateAudioUnderrunState(s32 arg0);
s32 audioDmaCallback(s32 addr, s32 len, void *state);
ALDMAproc initAudioDmaCallback(AudioDmaState **arg0);
void reclaimAudioDmaBuffers(void);

#endif
