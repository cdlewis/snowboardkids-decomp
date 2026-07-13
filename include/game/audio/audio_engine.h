#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include "common.h"
#include <PR/libaudio.h>

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
    s32 *sequenceOffsets;
    s32 *unk8;
    s32 *unkC;
    u8 *commands;
    s32 *jumpTable;
} PlayerCommandData;

typedef struct PlayerCommandClearBlock {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} PlayerCommandClearBlock;

typedef struct PlayerCommandState {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    s32 id;
    s32 fadeTarget;
    s32 fadeTime;
    s32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    PlayerCommandData *data;
    s32 sequencePos;
    s32 restartPos;
    s32 unk60;
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    s32 jumpTarget;
    u8 *returnScripts[5];
    s32 returnUnk60[5];
    s32 returnUnk68[5];
    s16 unkB0;
    s16 unkB2;
    s16 unkB4;
    u16 unkB6;
    s16 unkB8;
    u16 unkBA;
    u16 unkBC;
    u16 soundId;
    s16 unkC0;
    u16 unkC2;
    u16 unkC4;
    u16 unkC6;
    u16 unkC8;
    u16 unkCA;
    u16 unkCC;
    u16 returnUnkC8[5];
    u16 returnUnkCA[5];
    u8 unkE2;
    u8 unkE3;
    u8 unkE4;
    u8 flagE5;
    u8 flagE6;
    u8 flagE7;
    u8 flagE8;
    u8 unkE9;
    u8 unkEA;
    u8 unkEB;
    u8 returnDepth;
    u8 unkED;
    u8 unkEE;
    u8 unkEF;
    u8 unkF0;
    u8 unkF1;
    s8 unkF2;
    u8 unkF3;
    u8 unkF4;
    u8 unkF5;
    u8 unkF6;
    u8 unkF7;
    u8 unkF8;
    u8 unkF9;
    u8 unkFA;
    u8 unkFB;
    u8 unkFC;
    s8 unkFD;
    u8 unkFE;
    u8 unkFF;
    u32 unk100;
    u8 unk104;
    u8 unk105;
    u8 unk106;
    u8 unk107;
    u8 unk108;
    u8 returnUnk109[5];
    u8 returnUnk10E[5];
    u8 returnUnk113[5];
    s8 pitchOffset;
    s8 unk119;
    s8 unk11A;
    u8 pad11B;
} PlayerCommandState;

typedef struct SchedulerTask {
    struct SchedulerTask *next;
    u32 state;
    u32 flags;
    void *framebuffer;
    u8 list[0x40];
    OSMesgQueue *queue;
    OSMesg msg;
    s16 retrace;
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
    s32 count;
    s32 offsets[1];
} PlayerCommandBank;

typedef struct PlayerCommandRelocEntry {
    s32 unk0;
    u8 pad4[4];
    u8 unk8;
    u8 relocated;
    u8 padA[2];
    s32 unkC;
    s32 unk10;
} PlayerCommandRelocEntry;

typedef struct PlayerCommandInit {
    s32 count;
    void *unk4;
    s32 outputRate;
    u8 *heapBase;
    s32 heapLen;
    PlayerCommandBank *unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 *fxHeader;
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
s32 getSchedulerAudioTaskQueue(s32 arg0);
s32 getSchedulerGraphicsTaskQueue(s32 arg0);
void schedulerThreadMain(SchedulerState *arg0);
void tryStartPendingRdpTask(SchedulerState *arg0);
void startCurrentRdpTask(SchedulerState *arg0);
void finishCurrentRdpTask(SchedulerState *arg0);
void schedulerSwapBufferThreadMain(SchedulerState *arg0);
void addSchedulerClient(SchedulerState *arg0, SchedulerClient *arg1, OSMesgQueue *arg2);
void removeSchedulerClient(SchedulerState *arg0, SchedulerClient *arg1);
void notifySchedulerClients(SchedulerState *arg0, s32 arg1);
void waitForFramebufferAvailable(SchedulerState *arg0, SchedulerTask *arg1);
void waitForNextFramebufferEvent(SchedulerState *arg0);
s32 soundPlayerCommandEndTrack(PlayerCommandState *arg0, s32 arg1);
void *soundPlayerCommandSetInstrument(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandSetPortamentoTime(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandClearPortamento(PlayerCommandState *arg0, s32 arg1);
s32 soundPlayerLoadEnvelope(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandSetTempo(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandSetReleaseOffset(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandSetReleaseTime(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandSetVibrato(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandSetNegativeVibrato(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandClearVibrato(PlayerCommandState *arg0, s32 arg1);
s32 soundPlayerCommandSetDuration(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandForceDurationRead(PlayerCommandState *arg0, s32 arg1);
s32 soundPlayerCommandSetPitchOffset(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandClearPitchOffsetOnce(PlayerCommandState *arg0, s32 arg1);
s32 soundPlayerCommandAddFinePitch(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandLoadEnvelopePreset(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandDisableEnvelopeReset(PlayerCommandState *arg0, s32 arg1);
s32 soundPlayerCommandEnableEnvelopeReset(PlayerCommandState *arg0, s32 arg1);
s32 soundPlayerCommandTieNotes(PlayerCommandState *arg0, s32 arg1);
s32 soundPlayerCommandUntieNotes(PlayerCommandState *arg0, s32 arg1);
s32 soundPlayerCommandCallLoop(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandReturnLoop(PlayerCommandState *arg0, s32 arg1);
s32 soundPlayerCommandSetTremolo(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandClearTremolo(PlayerCommandState *arg0, s32 arg1);
s32 soundPlayerCommandReadVelocity(PlayerCommandState *arg0, s32 arg1);
s32 soundPlayerCommandUseDefaultVelocity(PlayerCommandState *arg0, s32 arg1);
s32 soundPlayerCommandSetDefaultVelocity(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandSetPan(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandSkipTwoBytes(s32 arg0, s32 arg1);
void *soundPlayerCommandSetJumpTable(void *arg0, u8 *arg1);
s32 soundPlayerCommandClearJumpTable(PlayerCommandState *arg0, s32 arg1);
s32 soundPlayerCommandNoOp(s32 arg0, s32 arg1);
s32 soundPlayerCommandJumpRelative(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandSetFxMix(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandRandomizePitchOffset(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandRandomizeVolume(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandRandomizePan(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandSetVolume(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandSetPanSweep(PlayerCommandState *arg0, u8 *arg1);
s32 soundPlayerCommandSetPitchSweepDepth(PlayerCommandState *arg0, u8 *arg1);
s32 initSoundPlayer(PlayerCommandInit *arg0);
void setSoundPlayerMasterVolume(s32 arg0, s32 arg1);
s32 startMusicSequence(PlayerCommandData *arg0);
s32 startSoundEffectDefault(s32 arg0);
s32 startSoundEffect(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void fadeOutSoundPlayersByType(s32 arg0, s32 arg1);
s32 countActiveSoundPlayersByType(s32 arg0);
s32 stopSoundPlayerByHandle(s32 arg0, s32 arg1);
s32 countSoundPlayersByHandle(s32 arg0);
s32 setSoundPlayerVolumeByHandle(s32 arg0, s32 arg1);
s32 setSoundPlayerPanByHandle(s32 arg0, s32 arg1);
s32 setSoundPlayerPitchOffsetByHandle(s32 arg0, f32 arg1);
s32 setSoundPlayerTempoByHandle(s32 arg0, s32 arg1);
ALMicroTime soundPlayerUpdate(void *arg0);
void soundPlayerReadNextNote(PlayerCommandState *arg0, s32 arg1);
void soundPlayerApplyVolumeAndPan(PlayerCommandState *arg0, s32 arg1);
void soundPlayerApplyPitch(PlayerCommandState *arg0, s32 arg1);
void soundPlayerStartEnvelope(PlayerCommandState *arg0);
void soundPlayerUpdateEnvelope(PlayerCommandState *arg0);
void soundPlayerUpdateTremolo(PlayerCommandState *arg0);
void soundPlayerUpdateVibrato(PlayerCommandState *arg0);
void soundPlayerUpdateVolumeTrack(PlayerCommandState *arg0);
void soundPlayerUpdatePanTrack(PlayerCommandState *arg0);
f32 approximatePitchRatio(f32 arg0);
void loadSoundBank(PlayerCommandBank *arg0, s32 arg1);
s32 soundPlayerRandom();
void resetSoundPlayerState(PlayerCommandState *arg0);
s32 findFreeSoundPlayerIndex(s32 arg0, s32 arg1);
void audioMemset(u8 *p, unsigned char c, s32 n);
s32 startSoundPlayerState(PlayerCommandState *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void initAudioSynthesizer(SchedulerState *scheduler, ALSynConfig *config, s32 threadPriority,
                          AudioSynthInitConfig *initConfig, s32 dmaBufferCount, s32 dmaBufferSize,
                          s32 retraceRate);
void audioThreadMain(s32 arg0);
s32 buildAudioTask(AudioTask *task, AudioInfo *info);
void updateAudioUnderrunState(s32 arg0);
s32 audioDmaCallback(s32 addr, s32 len, void *state);
ALDMAproc initAudioDmaCallback(AudioDmaState **arg0);
void reclaimAudioDmaBuffers(void);

#endif
