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

extern s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
extern s32 osSetIntMask(s32);
extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern void osCreateThread(SchedulerThread *, s32, void (*)(void *), void *, void *, s32);
extern void osCreateViManager(s32);
extern void osSetEventMesg(s32, OSMesgQueue *, OSMesg);
extern void osStartThread(SchedulerThread *);
extern s32 osSpTaskYielded(void *);
extern void osViBlack(u8);
extern void osViSetEvent(OSMesgQueue *, OSMesg, u32);
extern void osViSetMode(SchedulerViMode *);
extern void *osViGetCurrentFramebuffer(void);
extern void *osViGetNextFramebuffer(void);
extern void osWritebackDCacheAll(void);
extern void osSpTaskLoad(void *);
extern void osSpTaskStartGo(void *);
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
extern s32 gAudioThreadStarted;
extern s32 gAudioCmdListIndex;
extern u32 gAudioFrameCounter;
extern u32 gPendingAudioDmaCount;
extern AudioInfo *gNextAudioInfo;
extern s32 gAudioUnderrunState;
extern SchedulerViMode gSchedulerViModes[];
extern u8 gSchedulerThreadStack[];
extern Acmd *gAudioCmdLists[];
extern Acmd *gAudioCmdListEnd0;
extern Acmd *gAudioCmdListEnd1;
extern SchedulerThread gAudioThread;
extern OSMesgQueue gAudioThreadQueue;
extern OSMesg gAudioThreadMessages[];
extern u64 gAudioTaskDoneQueue[];
extern OSMesg gAudioTaskDoneMessages[];
extern s32 gNextSoundPlayerHandle;
extern s32 gSoundPlayerRandomSeed;
extern s32 gSchedulerRspTaskState;
extern u8 gSchedulerRdpTaskActive;
extern s32 gMinAudioTaskOutputLen;
extern s32 gTargetAudioTaskOutputLen;
extern s32 gMaxAudioTaskOutputLen;
extern s32 gAudioCmdListCapacity;
extern OSMesgQueue gAudioDmaQueue;
extern s32 gAudioSchedulerState;
extern OSIoMesg *gAudioDmaMessages;
extern OSMesg *gAudioDmaMessageBuffer;
extern s32 gAudioDmaBufferSize;
extern s32 *libmus_fxheader_current;
extern ALPlayer gSoundPlayer;
extern ALHeap gSoundPlayerHeap;
extern u8 *gSoundPlayerHeapEnd;
extern u8 *gSoundPlayerVoices;
extern s32 gSoundPlayerCount;
extern PlayerCommandState *gSoundPlayerStates;
extern u8 *gSoundPlayerTuningTable;
extern f32 *gSoundPlayerPitchOffsets;
extern s32 *gSoundPriorityTable;
extern u16 gSoundEffectMasterVolume;
extern u16 gMusicMasterVolume;
extern s32 gAudioTicksPerSecond;
extern s32 gSoundPlayerUpdateCounter;
extern ALSynth gAudioSynthesizer;
extern s32 osTvType;

extern ALDMAproc initAudioDmaCallback(AudioDmaState **);

void initScheduler(SchedulerState *arg0, u8 arg1, u8 arg2) {
    arg0->curRSPTask = 0;
    arg0->curRDPTask = 0;
    arg0->clients = 0;
    arg0->doAudio = 1;
    arg0->doAudio = arg0->doAudio & 0xFFFFFFFFFFFFFFFF;
    arg0->unk0 = 1;
    arg0->unk2 = 3;
    gSchedulerYieldResult = 0;
    gSchedulerYieldRequested = 0;
    gSchedulerRspTaskState = 0;
    gSchedulerRdpTaskActive = 0;
    osCreateMesgQueue(&arg0->retraceQueue, arg0->retraceMsgs, 0x20);
    osCreateMesgQueue(&arg0->queue1A4, arg0->msgs1BC, 0x10);
    osCreateMesgQueue(&arg0->eventQueue, arg0->eventMsgs, 0x10);
    osCreateMesgQueue(&arg0->messageQueue, arg0->messageMsgs, 0x10);
    osCreateMesgQueue(&arg0->framebufferQueue, arg0->framebufferMsgs, 0x10);
    osCreateMesgQueue(&arg0->queue14C, arg0->msgs164, 0x10);
    osCreateViManager(0xFE);
    osViSetMode(&gSchedulerViModes[arg1]);
    osViBlack(1);
    osViSetEvent(&arg0->retraceQueue, (OSMesg)0x29A, arg2);
    osSetEventMesg(4, &arg0->retraceQueue, (OSMesg)0x29B);
    osSetEventMesg(0xE, &arg0->retraceQueue, (OSMesg)0x29D);
    osSetEventMesg(9, &arg0->queue1A4, (OSMesg)0x29C);
    osCreateThread(&arg0->thread258, 6, schedulerThreadMain, arg0, gSchedulerThreadStack, 0x78);
    osStartThread(&arg0->thread258);
    osCreateThread(&arg0->thread408, 5, schedulerSwapBufferThreadMain, arg0, &gSchedulerRspTaskState, 0x64);
    osStartThread(&arg0->thread408);
}

s32 getSchedulerAudioTaskQueue(s32 arg0) {
    return arg0 + 4;
}

s32 getSchedulerGraphicsTaskQueue(s32 arg0) {
    return arg0 + 0x5C;
}

// schedulerThreadMain best match: 98.133% (nonmatchings/schedulerThreadMain-2694253543240320626/base_14.c)

#pragma GLOBAL_ASM("asm/nonmatchings/audio/audio_engine/schedulerThreadMain.s")

#ifdef NON_MATCHING
void schedulerThreadMain(SchedulerState *arg0) {
    s32 zero;
    OSMesgQueue *queue;
    s32 started;
    s32 delayedStart;
    s32 pendingAudio;
    OSMesg msg;

    zero = 0;
    started = zero;
    delayedStart = zero;
    pendingAudio = zero;
    msg = (OSMesg)zero;
    gSchedulerStartupRetraceCount = 0;
    queue = &arg0->retraceQueue;

loop:
    osRecvMesg(queue, &msg, 1);
    switch ((s32)msg) {
        case 0x29A:
            gRetraceCounter = gRetraceCounter + 1;
            gRetraceCounter = gRetraceCounter & 0xFFF;
            if ((started == 0) || (gSchedulerStartupRetraceCount < ((0, 0x16)))) {
                tryStartPendingRdpTask(arg0);
                pendingAudio = pendingAudio * 0;
                if (gSchedulerRdpTaskActive != 0) {
                    pendingAudio = 1;
                } else {
                    notifySchedulerClients(arg0, (s32)arg0);
                }
            } else {
                osViBlack(1);
            }
            if ((started != 0) && (gSchedulerStartupRetraceCount < 0x16)) {
                gSchedulerStartupRetraceCount += 1;
            }
            break;

        case 0x29B:
            if (gSchedulerRspTaskState & 2) {
                gSchedulerRspTaskState &= ~2;
                finishCurrentRdpTask(arg0);
                if (pendingAudio != 0) {
                    pendingAudio = 0;
                    notifySchedulerClients(arg0, (s32)arg0);
                }
                if (delayedStart != 0) {
                    delayedStart = 0;
                    osWritebackDCacheAll();
                    gSchedulerRspTaskState |= 1;
                    osSpTaskLoad(&arg0->curRSPTask->list);
                    osSpTaskStartGo(&arg0->curRSPTask->list);
                }
            } else if (gSchedulerRspTaskState & 1) {
                if (gSchedulerYieldRequested != 0) {
                    startCurrentRdpTask(arg0);
                } else {
                    gSchedulerRspTaskState &= ~1;
                    osSendMesg(&arg0->queue14C, (OSMesg)arg0, 1);
                }
            }
            break;

        case 1:
            if (gSchedulerRdpTaskActive == 0) {
                osWritebackDCacheAll();
                gSchedulerRspTaskState |= 1;
                osSpTaskLoad(&arg0->curRSPTask->list);
                osSpTaskStartGo(&(*arg0).curRSPTask->list);
            } else {
                delayedStart = 1;
            }
            break;

        case 0x29D:
            started = 1;
            notifySchedulerClients(arg0, (s32)&arg0->unk2);
            break;
    }
    goto loop;
}
#endif

void tryStartPendingRdpTask(SchedulerState *arg0) {
    if (gSchedulerRdpTaskActive == 0) {
        if (osRecvMesg(&arg0->messageQueue, (OSMesg *)&arg0->curRDPTask, 0) != -1) {
            osWritebackDCacheAll();
            gSchedulerRdpTaskActive = 1;
            gSchedulerYieldResult = 0;
            *(volatile unsigned int *)&gSchedulerYieldRequested = 0;
            if (gSchedulerRspTaskState & 1) {
                *(volatile unsigned int *)&gSchedulerYieldRequested = 1;
                osSpTaskYield();
            } else {
                startCurrentRdpTask(arg0);
            }
        }
    }
}

void startCurrentRdpTask(SchedulerState *arg0) {
    if (gSchedulerYieldRequested != 0) {
        gSchedulerYieldRequested = 0;
        if (osSpTaskYielded(&arg0->curRSPTask->list) != 0) {
            gSchedulerYieldResult = 1;
        } else {
            gSchedulerYieldResult = 2;
        }
    }
    gSchedulerRspTaskState |= 2;
    osWritebackDCacheAll();
    osSpTaskLoad(&arg0->curRDPTask->list);
    osSpTaskStartGo(&arg0->curRDPTask->list);
}

void finishCurrentRdpTask(SchedulerState *arg0) {
    OSMesg msg;

    msg = 0;
    osWritebackDCacheAll();
    if (gSchedulerYieldResult == 1) {
        osSpTaskLoad(arg0->curRSPTask->list);
        osSpTaskStartGo(arg0->curRSPTask->list);
    } else if (gSchedulerYieldResult == 2) {
        gSchedulerRspTaskState &= ~1;
        osSendMesg(&arg0->queue14C, &msg, 1);
    }
    osSendMesg(arg0->curRDPTask->queue, arg0->curRDPTask->msg, 1);
    gSchedulerRdpTaskActive = 0;
}

// schedulerSwapBufferThreadMain best match: 96.250% (nonmatchings/schedulerSwapBufferThreadMain-1936695454966205676/base_24.c)
#pragma GLOBAL_ASM("asm/nonmatchings/audio/audio_engine/schedulerSwapBufferThreadMain.s")

#ifdef NON_MATCHING
typedef struct SchedulerSwapLocals {
    OSMesgQueue *queue1A4;
    u8 pad4[4];
    OSMesg msg;
} SchedulerSwapLocals;

void schedulerSwapBufferThreadMain(SchedulerState *arg0) {
    OSMesg *msgPtr;
    s16 retrace;
    void *framebuffer;
    OSMesg sentMsg;
    SchedulerTask *task;
    SchedulerSwapLocals locals;

    msgPtr = &locals.msg;
    locals.msg = NULL;
    locals.queue1A4 = &arg0->queue1A4; loop: do { osRecvMesg(&arg0->eventQueue, (OSMesg *)(&arg0->curRSPTask), 1); waitForFramebufferAvailable(arg0, arg0->curRSPTask);
        osSendMesg(&arg0->retraceQueue, (OSMesg)1, 1);
        osRecvMesg(&arg0->queue14C, msgPtr, 1);
        if (framebuffer) {
        }
        osRecvMesg(locals.queue1A4, msgPtr, 1);
        if (!gRetraceCounter) {
        }
        task = arg0->curRSPTask; } while (!(task->flags & 0x40));

    framebuffer = task->framebuffer;
    retrace = task->retrace;
    osSendMesg(task->queue, sentMsg = task->msg, 1);
    if ((0xFFF & (gRetraceCounter - retrace)) >= 0x801) {
        do {
            if ((retrace && retrace) && retrace) {
            }
            waitForNextFramebufferEvent(arg0);
        } while (((gRetraceCounter - retrace) & 0xFFF) >= 0x801);
    }
    if (arg0->doAudio != 0) {
        osViBlack(0);
        arg0->doAudio = 0;
    }
    osViSwapBuffer(framebuffer);
    goto loop;
}
#endif

void addSchedulerClient(SchedulerState *arg0, SchedulerClient *arg1, OSMesgQueue *arg2) {
    s32 prev = osSetIntMask(1);
    arg1->queue = arg2;
    arg1->next = arg0->clients;
    arg0->clients = arg1;
    osSetIntMask(prev);
}

void removeSchedulerClient(SchedulerState *arg0, SchedulerClient *arg1) {
    SchedulerClient *node;
    SchedulerClient *prev;
    s32 mask;

    node = arg0->clients;
    prev = NULL;
    mask = osSetIntMask(1);
    if (node != NULL) {
        do {
            if (node == arg1) {
                if (prev != NULL) {
                    prev->next = arg1->next;
                } else {
                    arg0->clients = arg1->next;
                }
                break;
            }
            prev = node;
            node = node->next;
        } while (node != NULL);
    }
    osSetIntMask(mask);
}

void notifySchedulerClients(SchedulerState *arg0, s32 arg1) {
    SchedulerClient *node = arg0->clients;
    while (node != NULL) {
        osSendMesg(node->queue, (OSMesg)arg1, 0);
        node = node->next;
    }
}

void waitForFramebufferAvailable(SchedulerState *arg0, SchedulerTask *arg1) {
    OSMesg msg;
    SchedulerClient node;
    void *framebuffer;
    OSMesgQueue *queue;

    msg = 0;
    framebuffer = arg1->framebuffer;
    while ((osViGetCurrentFramebuffer() == framebuffer) || (osViGetNextFramebuffer() == framebuffer)) {
        queue = &arg0->framebufferQueue;
        addSchedulerClient(arg0, &node, queue);
        osRecvMesg(queue, &msg, 1);
        removeSchedulerClient(arg0, &node);
    }
}

void waitForNextFramebufferEvent(SchedulerState *arg0) {
    OSMesg msg;
    SchedulerClient node;

    msg = 0;
    addSchedulerClient(arg0, &node, &arg0->framebufferQueue);
    osRecvMesg(&arg0->framebufferQueue, &msg, 1);
    removeSchedulerClient(arg0, &node);
}

s32 soundPlayerCommandEndTrack(PlayerCommandState *arg0, s32 arg1) {
    arg0->unk60 = 0;
    arg0->unk68 = 0;
    arg0->data = 0;
    arg0->soundId = 0;
    arg0->id = 0;
    return 0;
}

void *soundPlayerCommandSetInstrument(PlayerCommandState *arg0, u8 *arg1) {
    u16 v;
    u8 *new_var;

    v = *arg1;
    arg1++;
    if (v & 0x80) {
        v = ((v & 0x7F) << 1) << 7;
        v |= *(new_var = arg1);
        arg1++;
    }
    arg0->instrumentIndex = v;
    return arg1;
}

s32 soundPlayerCommandSetPortamentoTime(PlayerCommandState *arg0, u8 *arg1) {
    arg0->portamentoTime = *arg1;
    return (s32)(arg1 + 1);
}

s32 soundPlayerCommandClearPortamento(PlayerCommandState *arg0, s32 arg1) {
    arg0->portamentoTime = 0;
    return arg1;
}

s32 soundPlayerLoadEnvelope(PlayerCommandState *arg0, u8 *arg1) {
    u32 value;

    value = *arg1++;
    if (value == 0) {
        value = 1;
    }
    arg0->unkF4 = value;
    arg0->unkF5 = arg1[0];
    value = arg1[1];
    arg1 += 2;
    if (value == 0) {
        value = 1;
    }
    arg0->unkFB = value;
    arg0->unk30 = (f32)(1.0 / (f64)(f32)value);

    arg1 += 2;
    arg0->unkF6 = arg1[-2];
    value = arg1[-1];
    if (value == 0) {
        value = 1;
    }
    arg0->unkFC = value;
    arg0->unk34 = (f32)(1.0 / (f64)(f32)value);

    arg1 += 2;
    arg0->unkF7 = arg1[-2];
    value = arg1[-1];
    if (value == 0) {
        value = 1;
    }
    arg0->unkFD = value;
    arg0->unk38 = (f32)(1.0 / (f64)(f32)value);

    return (s32)arg1;
}

s32 soundPlayerCommandSetTempo(PlayerCommandState *arg0, u8 *arg1) {
    s32 i;
    PlayerCommandState *entry;
    s32 value;
    s32 scaled;

    value = (*arg1 * 0x6000) / 120 / gAudioTicksPerSecond;
    scaled = 7;
    scaled = (arg0->unkB4 * value) >> scaled;
    arg1++;
    if (arg0->soundId != 0) {
        do {
            arg0->unkB8 = value;
        } while (0);
    } else {
        i = 0;
        entry = gSoundPlayerStates;
        if (gSoundPlayerCount > 0) {
            do {
                i++;
                if (entry->data == arg0->data) {
                    entry->unkBA = value;
                    entry->unkB8 = scaled;
                }
                entry++;
            } while (i < gSoundPlayerCount);
        }
    }
    return (s32)arg1;
}

s32 soundPlayerCommandSetReleaseOffset(PlayerCommandState *arg0, u8 *arg1) {
    u8 *ret;
    u8 *temp;

    arg0->unkC4 = *arg1;
    ;
    ret = (temp = arg1) + 1;
    arg0->unkC2 = 0;
    arg1++;
    arg1--;
    return (s32)ret;
}

s32 soundPlayerCommandSetReleaseTime(PlayerCommandState *arg0, u8 *arg1) {
    u8 *ret;
    u8 *temp;
    u32 v;
    short new_var;

    new_var = arg1[0] << 8;
    new_var = arg1[1] | ((0, new_var));
    v = new_var;
    ret = (temp = arg1) + 2;
    arg0->unkC2 = v;
    arg0->unkC4 = 0;
    arg1 += 2;
    arg1 -= 2;
    return (s32)ret;
}

s32 soundPlayerCommandSetVibrato(PlayerCommandState *arg0, u8 *arg1) {
    u32 temp_t8;
    f32 var_ft1;

    arg0->unk105 = arg1[0];
    arg0->unkE9 = arg1[1];
    arg1 += 2;
    temp_t8 = arg1[0];
    var_ft1 = (f32)(temp_t8 & 0xFFFF);
    arg0->vibratoDepth = (f32)(((double)var_ft1) / 50.0);
    return (s32)(arg1 + 1);
}

s32 soundPlayerCommandSetNegativeVibrato(PlayerCommandState *arg0, u8 *arg1) {
    u32 temp_t8;
    f32 var_ft1;

    arg0->unk105 = arg1[0];
    arg0->unkE9 = arg1[1];
    arg1 += 2;
    temp_t8 = arg1[0];
    var_ft1 = (f32)(temp_t8 & 0xFFFF);
    arg0->vibratoDepth = (f32)(((double)-var_ft1) / 50.0);
    return (s32)(arg1 + 1);
}

s32 soundPlayerCommandClearVibrato(PlayerCommandState *arg0, s32 arg1) {
    arg0->unkE9 = 0;
    arg0->vibratoPitchOffset = 0.0f;
    return arg1;
}

s32 soundPlayerCommandSetDuration(PlayerCommandState *arg0, u8 *arg1) {
    u8 b;
    unsigned int new_var;
    s16 value;

    b = arg1[0];
    arg1++;
    if (b < 0x80) {
        arg0->unkC0 = b;
    } else {
        value = ((b & 0x7F) << 6) << 2;
        arg0->unkC0 = value;
        new_var = arg1[0];
        b = new_var;
        arg0->unkC0 = value + b;
        arg1++;
    }
    return (s32)arg1;
}

s32 soundPlayerCommandForceDurationRead(PlayerCommandState *arg0, s32 arg1) {
    arg0->flagE6 = 1;
    return arg1;
}

s32 soundPlayerCommandSetPitchOffset(PlayerCommandState *arg0, u8 *arg1) {
    arg0->pitchOffset = *arg1;
    return (s32)(arg1 + 1);
}

s32 soundPlayerCommandClearPitchOffsetOnce(PlayerCommandState *arg0, s32 arg1) {
    arg0->skipPitchOffsetOnce = 1;
    return arg1;
}

s32 soundPlayerCommandAddFinePitch(PlayerCommandState *arg0, u8 *arg1) {
    s32 b;
    f32 f;
    b = *arg1;
    arg1++;
    if (b & 0x80) {
        b |= 0xFFFFFF00;
    }
    f = (f32)b;
    arg0->finePitchOffset = f / 100.0;
    return (s32)arg1;
}

s32 soundPlayerCommandLoadEnvelopePreset(PlayerCommandState *arg0, u8 *arg1) {
    s32 var_v0;

    var_v0 = arg1[0];
    arg1 += 1;
    if (var_v0 & 0x80) {
        var_v0 = arg1[0] | ((0, (var_v0 & 0x7F) << 8));
        arg1 += 1;
    }
    soundPlayerLoadEnvelope(arg0, arg0->data->commands + (var_v0 * 7));
    return (s32)arg1;
}

s32 soundPlayerCommandDisableEnvelopeReset(PlayerCommandState *arg0, s32 arg1) {
    arg0->flagE8 = 1;
    return arg1;
}

s32 soundPlayerCommandEnableEnvelopeReset(PlayerCommandState *arg0, s32 arg1) {
    arg0->flagE8 = 0;
    return arg1;
}

s32 soundPlayerCommandTieNotes(PlayerCommandState *arg0, s32 arg1) {
    arg0->flagE5 = 1;
    return arg1;
}

s32 soundPlayerCommandUntieNotes(PlayerCommandState *arg0, s32 arg1) {
    arg0->flagE5 = 0;
    return arg1;
}

s32 soundPlayerCommandCallLoop(PlayerCommandState *arg0, u8 *arg1) {
    u8 depth;

    depth = arg0->returnDepth;
    arg0->returnUnk109[depth] = *arg1++;
    arg0->returnScripts[depth] = arg1;
    arg0->returnUnk60[depth] = arg0->unk60;
    arg0->returnUnk68[depth] = arg0->unk68;
    arg0->returnUnk10E[depth] = arg0->unkEF;
    arg0->returnUnk113[depth] = arg0->pitchBendValue;
    arg0->returnUnkC8[depth] = arg0->unkC8;
    arg0->returnUnkCA[depth] = arg0->unkCA;
    arg0->returnDepth++;
    return (s32)arg1;
}

s32 soundPlayerCommandReturnLoop(PlayerCommandState *arg0, s32 arg1) {
    s32 depth;
    u8 *countPtr;
    u8 count;
    u8 nextCount;

    depth = arg0->returnDepth - 1;
    countPtr = &arg0->returnUnk109[depth];
    count = *countPtr;
    nextCount = count - 1;
    if (count != 0xFF) {
        *countPtr = nextCount;
        if ((nextCount & 0xFF) == 0) {
            arg0->returnDepth = depth;
            depth = -1;
        }
    }
    if (depth >= 0) {
        arg1 = (s32)arg0->returnScripts[depth];
        arg0->unk60 = arg0->returnUnk60[depth];
        arg0->unk68 = arg0->returnUnk68[depth];
        arg0->unkEF = arg0->returnUnk10E[depth];
        arg0->pitchBendValue = arg0->returnUnk113[depth];
        arg0->unkC8 = arg0->returnUnkC8[depth];
        arg0->unkCA = arg0->returnUnkCA[depth];
    }
    return arg1;
}

s32 soundPlayerCommandSetTremolo(PlayerCommandState *arg0, u8 *arg1) {
    arg0->unk119 = arg1[0];
    arg0->unkEB = arg1[1];
    arg0->unk106 = arg1[2];
    return (s32)(arg1 + 3);
}

s32 soundPlayerCommandClearTremolo(PlayerCommandState *arg0, s32 arg1) {
    arg0->unkEB = 0;
    return arg1;
}

s32 soundPlayerCommandReadVelocity(PlayerCommandState *arg0, s32 arg1) {
    arg0->unkED = 1;
    return arg1;
}

s32 soundPlayerCommandUseDefaultVelocity(PlayerCommandState *arg0, s32 arg1) {
    arg0->unkED = 0;
    return arg1;
}

s32 soundPlayerCommandSetDefaultVelocity(PlayerCommandState *arg0, u8 *arg1) {
    s32 ret;

    ret = arg1 + 1;
    arg0->unkEE = arg1[0];
    if (ret) {
    }
    arg0->unkED = 0;
    return ret;
}

s32 soundPlayerCommandSetPan(PlayerCommandState *arg0, u8 *arg1) {
    arg0->unkF2 = (s32)(u8)*arg1 / 2;
    return (s32)(arg1 + 1);
}

s32 soundPlayerCommandSkipTwoBytes(s32 arg0, s32 arg1) {
    return arg1 + 2;
}

void *soundPlayerCommandSetJumpTable(void *arg0, u8 *arg1) {
    u8 *temp_a2;
    s32 phi;

    temp_a2 = *(u8 **)((s32)arg0 + 0x54);
    phi = (s32)temp_a2 + (*(s32 **)((s32)(*(u8 **)((s32)arg0 + 0x54)) + 0x14))[*arg1];
    *(s32 *)((s32)arg0 + 0x70) = phi;
    return arg1 + 1;
}

s32 soundPlayerCommandClearJumpTable(PlayerCommandState *arg0, s32 arg1) {
    arg0->jumpTarget = 0;
    return arg1;
}

s32 soundPlayerCommandNoOp(s32 arg0, s32 arg1) {
    return arg1;
}

// soundPlayerCommandJumpRelative best match: 98.2% (nonmatchings/soundPlayerCommandJumpRelative-8757365433159351387/base_10.c)
#pragma GLOBAL_ASM("asm/nonmatchings/audio/audio_engine/soundPlayerCommandJumpRelative.s")

#ifdef NON_MATCHING
s32 soundPlayerCommandJumpRelative(PlayerCommandState *arg0, u8 *arg1) {
    register s32 temp_v0;
    register s32 temp_v1;
    s32 temp_t9;
    PlayerCommandState *state;
    u8 *command;
    s32 temp_t7;

    temp_v0 = arg1[2];
    temp_v1 = arg1[0];
    temp_t9 = (arg1[3] & 0xFF) & 0xFF;
    temp_v0 = temp_v0 << 8;
    arg1 += 5;
    command = arg1 - 5;
    temp_t7 = command[1];
    temp_v0 = temp_v0 + (temp_t9 & 0xFFFFu);
    temp_t9 = arg0->unk64;
    arg0->unk60 = temp_t9 + temp_v0;
    state = arg0;
    state->unkC8 = 1;
    temp_v0 = command[4];
    temp_t9 = command[5];
    temp_v0 = (temp_v0 << 8) + (temp_t9 & 0xFF);
    state->unk68 = arg0->unk6C + temp_v0;
    temp_v1 = (temp_v1 << 8) + temp_t7;
    arg0->unkCA = 1;
    temp_t7 = arg0->restartPos;
    return temp_t7 + temp_v1;
}
#endif

s32 soundPlayerCommandSetFxMix(PlayerCommandState *arg0, u8 *arg1) {
    arg0->unkF3 = *arg1;
    return (s32)(arg1 + 1);
}

s32 soundPlayerCommandRandomizePitchOffset(PlayerCommandState *arg0, u8 *arg1) {
    u8 range;

    range = *arg1;
    arg1++;
    arg0->pitchOffset = soundPlayerRandom(range, arg1, arg0);
    arg0->pitchOffset += *arg1;
    return (s32)(arg1 + 1);
}

s32 soundPlayerCommandRandomizeVolume(PlayerCommandState *arg0, u8 *arg1) {
    u8 temp_a0;
    s32 temp_v0;

    temp_a0 = *arg1;
    arg1++;
    temp_v0 = soundPlayerRandom(temp_a0, arg1, arg0);
    arg0->unkEF = temp_v0;
    arg0->unkEF = *arg1 + (temp_a0 = temp_v0);
    return (s32)(arg1 + 1);
}

s32 soundPlayerCommandRandomizePan(PlayerCommandState *arg0, u8 *arg1) {
    u8 temp_a0;
    s8 temp_v0;

    temp_a0 = *arg1;
    arg1++;
    temp_v0 = soundPlayerRandom(temp_a0, arg1, arg0);
    arg0->unkF2 = temp_v0;
    arg0->unkF2 = *arg1 + (temp_a0 = temp_v0);
    return (s32)(arg1 + 1);
}

s32 soundPlayerCommandSetVolume(PlayerCommandState *arg0, u8 *arg1) {
    arg0->unkEF = *arg1;
    return (s32)(arg1 + 1);
}

s32 soundPlayerCommandSetPanSweep(PlayerCommandState *arg0, u8 *arg1) {
    PlayerCommandState *entry;
    s32 i;
    s32 newId;
    s32 soundId;

    soundId = *arg1;
    arg1++;
    if (soundId >= 0x80) {
        soundId = *arg1 + ((soundId & 0x7F) << 8);
        arg1++;
    }

    newId = startSoundEffect(soundId, arg0->unkEF, (u8)arg0->unkF2, 0, arg0->unk20++);
    arg0->unk20--;
    if (newId == 0) {
        return (s32)arg1;
    }

    i = 0;
    entry = gSoundPlayerStates;
    if (gSoundPlayerCount > 0) {
        do {
            i++;
            if (newId == entry->id) {
                entry->id = arg0->id;
            }
            entry++;
        } while (i < gSoundPlayerCount);
    }
    return (s32)arg1;
}

s32 soundPlayerCommandSetPitchSweepDepth(PlayerCommandState *arg0, u8 *arg1) {
    arg0->pitchBendDepth = *arg1;
    return (s32)(arg1 + 1);
}

s32 initSoundPlayer(PlayerCommandInit *arg0) {
    ALVoiceConfig vc;
    ALSynConfig synConfig;
    AudioSynthInitConfig synthConfig;
    s32 i;

    gSoundPlayerCount = arg0->count;
    gSoundPlayerTuningTable = arg0->tuningTable;
    gSoundPlayerPitchOffsets = arg0->pitchOffsetTable;
    libmus_fxheader_current = arg0->fxHeader;
    gSoundPriorityTable = arg0->unk28;

    if (osTvType == 0) {
        gAudioTicksPerSecond = 50;
    } else {
        gAudioTicksPerSecond = 60;
    }

    audioMemset(arg0->heapBase, 0, arg0->heapLen);
    alHeapInit(&gSoundPlayerHeap, arg0->heapBase, arg0->heapLen);

    gSoundPlayerVoices = alHeapDBAlloc(0, 0, &gSoundPlayerHeap, 1, gSoundPlayerCount * sizeof(ALVoice));
    gSoundPlayerStates = alHeapDBAlloc(0, 0, &gSoundPlayerHeap, 1, gSoundPlayerCount * sizeof(PlayerCommandState));

    audioMemset(gSoundPlayerVoices, 0, gSoundPlayerCount * sizeof(ALVoice));
    audioMemset(gSoundPlayerStates, 0, gSoundPlayerCount * sizeof(PlayerCommandState));

    synConfig.maxVVoices = gSoundPlayerCount;
    synConfig.maxPVoices = gSoundPlayerCount;
    synConfig.maxUpdates = arg0->maxUpdates;
    synConfig.dmaproc = 0;
    synConfig.fxType = 2;
    synConfig.outputRate = 0;
    synConfig.heap = &gSoundPlayerHeap;

    synthConfig.outputRate = arg0->maxFXBusses;
    synthConfig.frameRate = arg0->unk38;
    synthConfig.commandListSize = arg0->unk34;

    initAudioSynthesizer(arg0->unk4, &synConfig, arg0->outputRate, &synthConfig,
                  arg0->unk3C, arg0->unk40, gAudioTicksPerSecond);
    loadSoundBank(arg0->soundBank, arg0->sampleBaseOffset);
    setSoundPlayerMasterVolume(3, 0x7FFF);

    gSoundPlayerUpdateCounter = 0;
    gNextSoundPlayerHandle = 1;
    gSoundPlayerRandomSeed = 0x12345678;

    gSoundPlayer.next = 0;
    gSoundPlayer.handler = soundPlayerUpdate;
    gSoundPlayer.clientData = &gSoundPlayer;
    alSynAddPlayer(&gAudioSynthesizer, &gSoundPlayer);

    for (i = 0; i < gSoundPlayerCount; i++) {
        gSoundPlayerStates[i].unkE4 = 0;
        resetSoundPlayerState(&gSoundPlayerStates[i]);
        vc.unityPitch = 0;
        vc.priority = arg0->outputRate;
        vc.fxBus = 0;
        alSynAllocVoice(&gAudioSynthesizer, &gSoundPlayerVoices[i * 0x1C], &vc);
    }

    return gSoundPlayerHeapEnd - gSoundPlayerHeap.base;
}

void setSoundPlayerMasterVolume(s32 arg0, s32 arg1) {
    s32 temp_t7 = arg0 & 2;

    if (arg0 & 1) {
        gSoundEffectMasterVolume = arg1;
    }
    if (temp_t7) {
        gMusicMasterVolume = arg1;
    }
}

// startMusicSequence best match: 99.321% (nonmatchings/startMusicSequence-3242520251544044307/base_14.c)
#pragma GLOBAL_ASM("asm/nonmatchings/audio/audio_engine/startMusicSequence.s")

#ifdef NON_MATCHING
s32 startMusicSequence(PlayerCommandData *arg0) {
    s32 i;
    int handle;
    s32 needed;
    s32 value;
    s32 index;
    s32 *ptr;
    PlayerCommandState *state;
    s32 free;
    s32 trackCount;

    trackCount = arg0->trackCount;
    if ((u32)arg0->sequenceOffsets < 0x400U) {
        ptr = (s32 *)&arg0->sequenceOffsets;
        for (i = 0; i < (trackCount * 3) + 5; i++) {
            value = ptr[i];
            if (value != 0) {
                ptr[i] = value;
                ptr[i] = ptr[i] + (s32)arg0;
            }
        }
    }

    arg0++;
    needed = 0;
    arg0--;
    for (i = 0; i < trackCount; i++) {
        if (arg0->sequenceOffsets[i] != 0) {
            needed++;
        }
    }

    free = 0;
    state = gSoundPlayerStates;
    for (i = 0; i < gSoundPlayerCount; i++) {
        if (state->sequencePos == 0) {
            free++;
        }
        state++;
    }

    if (free < needed) {
        return 0;
    }

    value = gNextSoundPlayerHandle;
    handle = value;
    gNextSoundPlayerHandle = handle + 1;
    for (i = 0; i < trackCount; i++) {
        value = (s32)arg0;
        if (arg0->sequenceOffsets[i] != 0) {
            index = findFreeSoundPlayerIndex(value, i);
            if (index == -1) {
                rmonPrintf("NG Channel\n");
            }
            state = &gSoundPlayerStates[index];
            resetSoundPlayerState(state);
            state->data = arg0;
            value = arg0->unk8[i];
            state->unk64 = value;
            state->unk60 = value;
            value = arg0->unkC[i];
            state->unk6C = value;
            state->unk68 = value;
            value = arg0->sequenceOffsets[i];
            state->restartPos = value;
            state->sequencePos = value;
            state->id = handle;
        }
    }

    return handle;
}
#endif

s32 startSoundEffectDefault(s32 arg0) {
    s32 i;
    PlayerCommandState *entry;
    s32 value;

    value = gSoundPriorityTable[arg0];
    entry = gSoundPlayerStates;
    i = 0;
    if (gSoundPlayerCount > 0) {
        do {
            i++;
            if (entry->sequencePos == 0) {
                return startSoundPlayerState(entry, arg0, 0x80, 0x80, value);
            }
            entry++;
        } while (i < gSoundPlayerCount);
    }
    return 0;
}

s32 startSoundEffect(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 i;
    PlayerCommandState *entry;

    if (arg4 == -1) {
        arg4 = gSoundPriorityTable[arg0];
    }

    if (arg3 != 0) {
        entry = gSoundPlayerStates;
        i = 0;
        if (gSoundPlayerCount > 0) {
            do {
                i++;
                if (arg0 == entry->soundId) {
                    return startSoundPlayerState(entry, arg0, arg1, arg2, arg4);
                }
                entry++;
            } while (i < gSoundPlayerCount);
        }
    }

    entry = gSoundPlayerStates;
    i = 0;
    if (gSoundPlayerCount > 0) {
        do {
            i++;
            if (entry->sequencePos == 0) {
                return startSoundPlayerState(entry, arg0, arg1, arg2, arg4);
            }
            entry++;
        } while (i < gSoundPlayerCount);
    }
    return 0;
}

void fadeOutSoundPlayersByType(s32 arg0, s32 arg1) {
    s32 i;
    s32 value;
    PlayerCommandState *entry;

    if (arg1 != 0) {
        value = arg1;
    } else {
        value = 1;
    }
    entry = gSoundPlayerStates;
    i = 0;
    entry = gSoundPlayerStates;
    if (gSoundPlayerCount > 0) {
        do {
            ;
            i++;
            if (((entry->soundId != 0) && (arg0 & 1)) ||
                ((entry->soundId == 0) && (arg0 & 2))) {
                entry->fadeTarget = arg1;
                entry->fadeTime = value;
            }
            entry++;
        } while (i < gSoundPlayerCount);
    }
}

s32 countActiveSoundPlayersByType(s32 arg0) {
    s32 i;
    s32 matches;
    PlayerCommandState *entry;
    volatile char flags;
    u16 temp_a0;

    i = 0;
    entry = gSoundPlayerStates;
    ;
    matches = 0;
    if (gSoundPlayerCount > 0) {
        do {
            i++;
            if (entry->sequencePos != 0) {
                temp_a0 = entry->soundId;
                if (((temp_a0 != 0) && (arg0 & 1)) || ((((unsigned long) temp_a0) == 0) && (arg0 & 2))) {
                    matches++;
                }
            }
            entry++;
        } while (i < gSoundPlayerCount);
    }
    return matches;
}

s32 stopSoundPlayerByHandle(s32 arg0, s32 arg1) {
    s32 count;
    s32 i;
    s32 value;
    PlayerCommandState *entry;

    value = arg0 == 0;
    i = 0;
    if (value) {
        return 0;
    }
    if (arg1 != 0) {
        value = arg1;
    } else {
        value = 1;
    }
    entry = gSoundPlayerStates;
    count = 0;
    if (gSoundPlayerCount > 0) {
        do {
            i++;
            if (arg0 == entry->id) {
                entry->fadeTarget = arg1;
                entry->fadeTime = value;
                count++;
            }
            entry++;
        } while (i < gSoundPlayerCount);
    }
    return count;
}

s32 countSoundPlayersByHandle(s32 arg0) {
    s32 i = 0;
    s32 matches;
    PlayerCommandState *entry;

    if (arg0 == 0) {
        return 0;
    }

    entry = gSoundPlayerStates;
    matches = 0;
    if (gSoundPlayerCount > 0) {
        do {
            i++;
            if (arg0 == entry->id) {
                matches++;
            }
            entry++;
        } while (i < gSoundPlayerCount);
    }
    return matches;
}

s32 setSoundPlayerVolumeByHandle(s32 arg0, s32 arg1) {
    s32 i;
    s32 matches;
    PlayerCommandState *entry;

    if (arg0 == 0) {
        return 0;
    }

    i = 0;
    entry = gSoundPlayerStates;
    matches = 0;
    if (gSoundPlayerCount > 0) {
        do {
            i++;
            if (arg0 == entry->id) {
                entry->unkB0 = arg1;
                matches++;
            }
            entry++;
        } while (i < gSoundPlayerCount);
    }
    return matches;
}

s32 setSoundPlayerPanByHandle(s32 arg0, s32 arg1) {
    s32 i;
    s32 matches;
    PlayerCommandState *entry;

    if (arg0 == 0) {
        return 0;
    }

    i = 0;
    entry = gSoundPlayerStates;
    matches = 0;
    if (gSoundPlayerCount > 0) {
        do {
            i++;
            if (arg0 == entry->id) {
                entry->unkB2 = arg1;
                matches++;
            }
            entry++;
        } while (i < gSoundPlayerCount);
    }
    return matches;
}

s32 setSoundPlayerPitchOffsetByHandle(s32 arg0, f32 arg1) {
    s32 i = 0;
    s32 matches;
    PlayerCommandState *entry;

    if (arg0 == 0) {
        return 0;
    }

    entry = gSoundPlayerStates;
    matches = 0;
    if (gSoundPlayerCount > 0) {
        do {
            i++;
            if (arg0 == entry->id) {
                entry->handlePitchOffset = arg1;
                matches++;
            }
            entry++;
        } while (i < gSoundPlayerCount);
    }
    return matches;
}

s32 setSoundPlayerTempoByHandle(s32 arg0, s32 arg1) {
    s32 i = 0;
    s32 matches;
    PlayerCommandState *entry;

    if (arg0 == 0) {
        return 0;
    }

    if (arg1 <= 0) {
        arg1 = 1;
    } else if (arg1 >= 0x101) {
        arg1 = 0x100;
    }

    entry = gSoundPlayerStates;
    matches = 0;
    if (gSoundPlayerCount > 0) {
        do {
            i++;
            if (arg0 == entry->id) {
                entry->unkB4 = arg1;
                matches++;
                entry->unkB8 = (s32)(entry->unkBA * arg1) >> 7;
            }
            entry++;
        } while (i < gSoundPlayerCount);
    }
    return matches;
}

// soundPlayerUpdate best match: 99.025% (nonmatchings/soundPlayerUpdate-8075865578671233833/base_17.c)

#pragma GLOBAL_ASM("asm/nonmatchings/audio/audio_engine/soundPlayerUpdate.s")

#ifdef NON_MATCHING
ALMicroTime soundPlayerUpdate(void *arg0) {
    PlayerCommandState *var_s0;
    s32 temp_t0;
    s32 temp_v0;
    s32 var_s1;
    u32 temp_t9;
    u32 temp_t9_2;

    var_s0 = gSoundPlayerStates;
    var_s1 = 0;
    if (gSoundPlayerCount > 0) {
        do {
            if (var_s0->sequencePos != 0) {
                temp_t9 = (var_s0->unk0 += (u16)var_s0->unkB8);

                if ((var_s0->unkBC != 0x7FFF) && ((u32)var_s0->unkC < temp_t9) && (var_s0->sequencePos != 0)) {
loop_6:
                    soundPlayerReadNextNote(var_s0, var_s1);
                    if ((int)((u32)var_s0->unkC < (u32)var_s0->unk0)) {
                        if (var_s0->sequencePos != 0) {
                            goto loop_6;
                        }
                    }
                }

                if (0 == var_s0->sequencePos) {

                } else {
                    if (var_s0->unk60 != 0) {
                        soundPlayerUpdateVolumeTrack(var_s0);
                    }
                    if (var_s0) {
                    }
                    if (var_s0->unk68 != 0) {
                        soundPlayerUpdatePanTrack(var_s0);
                    }

                    temp_v0 = var_s0->fadeTarget;
                    temp_t0 = temp_v0 - (1 & 0xFFFFFFFFFFFFFFFF);
                    if (var_s0->fadeTarget != -1) {
                        var_s0->fadeTarget = temp_t0;
                        if ((temp_t0 ^ 0) == -1) {
                            var_s0->sequencePos = soundPlayerCommandEndTrack(var_s0, 0);
                            temp_t9_2 = var_s0->unkE4;
                            if (temp_t9_2 != 0) {
                                var_s0->unkE4 = 0;
                                alSynStopVoice(&gAudioSynthesizer,
                                               (ALVoice *)(gSoundPlayerVoices + (7 * (((0, var_s1)) * 4))));
                            }
                        }
                    }

                    if (var_s0->unkE4 != 0) {
                        soundPlayerUpdateEnvelope(var_s0);
                        soundPlayerUpdateVibrato(var_s0);
                        soundPlayerUpdateTremolo(var_s0);
                        soundPlayerApplyPitch(var_s0, var_s1);
                        soundPlayerApplyVolumeAndPan(var_s0, var_s1);
                    }

                    temp_t9_2 = (u32)(var_s0->unk0 - var_s0->unk10) >> 8;
                    var_s0->noteAgeTicks = (u16)temp_t9_2;
                    var_s0->noteAgeTicksF = (f32)(temp_t9_2 & 0xFFFF);
                }
            }
            var_s1 += 1;
            var_s0 += 1;
        } while (var_s1 < gSoundPlayerCount);
    }

    gSoundPlayerUpdateCounter += 1;
    return 0xF4240 / (s32)gAudioTicksPerSecond;
}
#endif

// soundPlayerReadNextNote best match: 93.328% (nonmatchings/soundPlayerReadNextNote-2694253543240320626/base_11.c)

#pragma GLOBAL_ASM("asm/nonmatchings/audio/audio_engine/soundPlayerReadNextNote.s")

#ifdef NON_MATCHING
typedef u8 *(*PlayerCommandHandler)(PlayerCommandState *, u8 *, s32);

extern PlayerCommandHandler gSoundPlayerCommandHandlers[];
extern u8 gSoundPlayerDefaultVelocities[];
extern u8 *gSoundWaveTable;
extern s32 gSoundBankEntryCount;

void soundPlayerReadNextNote(PlayerCommandState *arg0, s32 arg1) {
    u8 *seq;
    u8 *durationPos;
    u8 cmd;
    u8 fxMix;
    u32 duration;
    s32 soundIndex;

    seq = arg0->sequencePos;
    if (seq != NULL) {
        cmd = seq[0];
        while (cmd >= 0x80) {
            seq = gSoundPlayerCommandHandlers[cmd & 0x7F](arg0, seq + 1, cmd);
            if (seq == NULL) {
                break;
            }
            cmd = seq[0];
        }
    }
    arg0->sequencePos = (s32)seq;

    if (seq != NULL) {
        arg0->portamentoStartPitch = arg0->currentNotePitch;
        cmd = *seq;
        seq++;
        arg0->sequencePos = (s32)seq;
        arg0->unkFE = cmd;

        if (arg0->unkED != 0) {
            arg0->unk108 = gSoundPlayerDefaultVelocities[*seq];
            seq++;
            arg0->sequencePos = (s32)seq;
        } else {
            arg0->unk108 = gSoundPlayerDefaultVelocities[arg0->unkEE];
        }

        duration = (u16)arg0->unkC0;
        if (duration != 0) {
            arg0->unkBC = duration;
            arg0->unk28 = (f32)(duration & 0xFFFF);
        }

        if ((arg0->flagE6 != 0) || (duration == 0)) {
            arg0->flagE6 = 0;
            durationPos = (u8 *)arg0->sequencePos;
            cmd = *durationPos;
            durationPos++;
            arg0->sequencePos = (s32)durationPos;
            if (cmd < 0x80) {
                arg0->unkBC = cmd;
                arg0->unk28 = (f32)cmd;
            } else {
                duration = (cmd & 0x7F) << 8;
                arg0->unkBC = duration;
                duration += *durationPos;
                arg0->unkBC = duration;
                arg0->unk28 = (f32)(duration & 0xFFFF);
                arg0->sequencePos = (s32)(durationPos + 1);
            }
        }

        duration = arg0->unkC;
        arg0->unkC += arg0->unkBC << 8;
        arg0->noteAgeTicks = 0;
        arg0->pitchPulseOffset = 0;
        arg0->unk10 = duration;
        arg0->noteAgeTicksF = 0.0f;
        arg0->unk107 = arg0->unk106;

        if (arg0->unkFE != 0) {
            if (arg0->jumpTarget != 0) {
                u8 *entry = (u8 *)arg0->jumpTarget + (arg0->unkFE * 4);

                arg0->instrumentIndex = entry[-0x30];
                arg0->unkF2 = entry[-0x2E] / 2;
                soundPlayerLoadEnvelope(arg0, arg0->data->commands + (entry[-0x2F] * 7));
                arg0->unkFE = ((u8 *)arg0->jumpTarget + (arg0->unkFE * 4))[-0x2D];
            }

            soundIndex = arg0->instrumentIndex;
            if (gSoundBankEntryCount <= soundIndex) {
                soundIndex = 0;
            }

            if (arg0->flagE5 == 0) {
                if (arg0->unkE4 != 0) {
                    alSynStopVoice(&gAudioSynthesizer, (ALVoice *)(gSoundPlayerVoices + (arg1 * 0x1C)));
                }
                arg0->unkE4 = 1;
                arg0->unkB6 = 0xFFFF;
                arg0->unkE3 = 0xFF;
                alSynStartVoice(&gAudioSynthesizer, (ALVoice *)(gSoundPlayerVoices + (arg1 * 0x1C)),
                                *(ALWaveTable **)(gSoundWaveTable + (soundIndex * 4)));
            }

            arg0->notePitch = gSoundPlayerTuningTable[soundIndex] + arg0->unkFE - 5;
            if (arg0->flagE8 == 0) {
                arg0->padF4[4] = 0;
                arg0->padF4[5] = arg0->padF4[1];
                soundPlayerStartEnvelope(arg0);
            }
            soundPlayerApplyPitch(arg0, arg1);
            soundPlayerApplyVolumeAndPan(arg0, arg1);
            fxMix = arg0->unkF3;
            if (arg0->unkE2 != fxMix) {
                arg0->unkE2 = fxMix;
                alSynSetFXMix(&gAudioSynthesizer, (ALVoice *)(gSoundPlayerVoices + (arg1 * 0x1C)), fxMix);
            }
        } else if (arg0->padF4[4] < 4) {
            arg0->padF4[4] = 4;
            arg0->padF4[6] = 1;
            arg0->unk100 = arg0->unk0;
            arg0->unk104 = arg0->padF4[5];
        }
    } else if (arg0->unkE4 != 0) {
        arg0->unkE4 = 0;
        alSynStopVoice(&gAudioSynthesizer, (ALVoice *)(gSoundPlayerVoices + (arg1 * 0x1C)));
    }
}
#endif

void soundPlayerApplyVolumeAndPan(PlayerCommandState *arg0, s32 arg1) {
    u32 volume;
    s32 stopping;
    int pan;
    u8 oldPan;

    volume = (u32)(arg0->unkF9 * (*arg0).unkEF * arg0->unk108 * arg0->unkB0) >> 13;
    if (volume >= 0x8000U) {
        volume = 0x7FFF;
    }

    if (arg0->soundId == 0) {
        volume *= gMusicMasterVolume;
    } else {
        volume *= gSoundEffectMasterVolume;
    }

    stopping = arg0->fadeTarget;
    volume >>= 15;

    if (stopping != -1) {
        volume = (stopping * volume) / arg0->fadeTime;
    }

    if (volume != arg0->unkB6) {
        arg0->unkB6 = volume;
        alSynSetVol(&gAudioSynthesizer, (ALVoice *)(gSoundPlayerVoices + (14 * (2 * arg1))), (s16)volume, 0xF4240 / gAudioTicksPerSecond);
    }

    stopping = arg0->unkE3;
    oldPan = stopping;
    pan = (((*(u8 *)&arg0->unkF2) * arg0->unkB2) >> 7) & 0x7F;
    if (pan != oldPan) {
        arg0->unkE3 = pan;
        alSynSetPan(&gAudioSynthesizer, (ALVoice *)(gSoundPlayerVoices + (arg1 * 0x1C)), pan & 0xFF);
    }
}

// soundPlayerApplyPitch best match: 97.179% (nonmatchings/soundPlayerApplyPitch-1225020319268080736/base_2.c)

#pragma GLOBAL_ASM("asm/nonmatchings/audio/audio_engine/soundPlayerApplyPitch.s")

#ifdef NON_MATCHING
void soundPlayerApplyPitch(PlayerCommandState *arg0, s32 arg1) {
    register f32 notePitch;
    f32 portamentoStartPitch;
    f32 portamentoPitchStep;
    f32 pitchRatio;
    f32 sequencePitchOffset;
    f64 pitchBendScale;
    s32 signedNotePitch;
    u8 portamentoTime;

    signedNotePitch = arg0->notePitch;
    if (signedNotePitch & 0x80) {
        notePitch = -0x100 - -(s32)signedNotePitch;
    } else {
        notePitch = signedNotePitch;
    }

    portamentoTime = arg0->portamentoTime;
    if ((portamentoTime != (0, 0)) && (portamentoTime >= arg0->noteAgeTicks)) {
        portamentoStartPitch = arg0->portamentoStartPitch;
        if (1) {
            portamentoPitchStep = (notePitch - portamentoStartPitch) / (f32)portamentoTime;
            portamentoPitchStep *= arg0->noteAgeTicksF;
            notePitch = portamentoStartPitch + portamentoPitchStep;
        }
    }

    sequencePitchOffset = (f32)arg0->pitchOffset * (f32)(1 - arg0->skipPitchOffsetOnce);
    arg0->currentNotePitch = notePitch;
    arg0->skipPitchOffsetOnce = 0;
    notePitch += arg0->vibratoPitchOffset + sequencePitchOffset + arg0->finePitchOffset + (f32)arg0->pitchPulseOffset;
    notePitch += gSoundPlayerPitchOffsets[arg0->instrumentIndex];
    notePitch = (f32)((f64)notePitch + ((pitchBendScale = (f64)(f32)arg0->pitchBendDepth * 0.015625) * ((f32)arg0->pitchBendValue - 64.0)));
    if (1) {}
    if (1) {}
    if (1) {}
    if (1) {}
    notePitch += arg0->handlePitchOffset;

    if (notePitch != arg0->cachedPitch) {
        arg0->cachedPitch = notePitch;
        pitchRatio = approximatePitchRatio((f32)((f64)notePitch * 0.083333333333333329));
        if (pitchRatio < 0.0f) {
            pitchRatio = 0.0f;
        }
        if (pitchRatio > 2.0) {
            pitchRatio = 2.0f;
            arg0->unk108 = 0;
        }
        alSynSetPitch(&gAudioSynthesizer, (ALVoice *)(gSoundPlayerVoices + (arg1 * 0x1C)), pitchRatio);
    }
}
#endif

void soundPlayerStartEnvelope(PlayerCommandState *arg0) {
    u16 temp_v0;

    if (arg0->unkBC != 0x7FFF) {
        temp_v0 = arg0->unkC2;
        if (temp_v0 != 0) {
            arg0->unk100 = arg0->unk10 + (temp_v0 << 8);
        } else {
            arg0->unk100 = arg0->unkC - (arg0->unkC4 << 8);
        }
    } else {
        arg0->unk100 = 0x7FFFFFFF;
    }

    arg0->unkF9 = arg0->unkF5;
    arg0->unkFA = arg0->unkF4;
    arg0->unkF8 = 1;
}

// soundPlayerUpdateEnvelope best match: 94.856% (nonmatchings/soundPlayerUpdateEnvelope-8075865578671233833/base_11.c)

#pragma GLOBAL_ASM("asm/nonmatchings/audio/audio_engine/soundPlayerUpdateEnvelope.s")

#ifdef NON_MATCHING
void soundPlayerUpdateEnvelope(PlayerCommandState *arg0) {
    u8 state;
    s32 tick;
    PlayerCommandState *new_var;
    u8 new_var4;
    u8 rate;
    PlayerCommandState *new_var2;
    float new_var3;
    u32 step;
    u8 value;
    f32 temp;

    new_var2 = arg0;
    state = new_var2->unkF8;
    if (state) {
        if (((u32)arg0->unk0 >= (u32)arg0->unk100) && (state < 4)) {
            step = 4;
            arg0->unkF8 = step;
            arg0->unkFA = 1;
            arg0->unk104 = arg0->unkF9;
        }
        tick = arg0->unkFA - 1;
        arg0->unkFA = tick;
        if ((tick & 0xFF) != 0) {
            return;
        }
        rate = arg0->unkF4;
        state = arg0->unkF8;
        arg0->unkFA = rate;
        switch (state) {
            case 1:
                step = ((u32)(arg0->unk0 - arg0->unk10)) >> 8;
                step = step / rate;
                if (step < arg0->unkFB) {
                    value = arg0->unkF5 ^ 0;
                    arg0->unkF9 = (s32)((f32)value + ((arg0->unkF6 - value) * arg0->unk30 * (f32)step));
                    return;
                }
                arg0->unkF8 = state;
                arg0->unkF8 = arg0->unkF8 + 1;
                arg0->unkF9 = arg0->unkF6;
                return;
            case 2:
                step = (((u32)(arg0->unk0 - arg0->unk10) >> 8) - arg0->unkFB) / rate;
                new_var4 = arg0->unkFC;
                if (step < new_var4) {
                    value = arg0->unkF6;
                    arg0->unkF9 = (s32)((f32)value + ((arg0->unkF7 - value) * arg0->unk34 * (f32)step));
                    return;
                }
                arg0->unkF8 = state + 1;
                arg0->unkF9 = arg0->unkF7;
                return;
            case 3:
                return;
            case 4:
                step = ((u32)(arg0->unk0 - arg0->unk100) >> 8) / rate;
                new_var = arg0;
                if (step < arg0->unkFD) {
                    value = new_var->unk104;
                    temp = value;
                    new_var3 = temp * new_var->unk38;
                    new_var->unkF9 = (s32)(temp - (new_var3 * (f32)step));
                    return;
                }
                new_var->unkF8 = state + 1;
                arg0->unkF9 = (((((u32)(arg0->unk0 - arg0->unk10)) >> 8) - new_var->unkFB) / rate) * 0;
                break;
        }
    }
}
#endif

void soundPlayerUpdateTremolo(PlayerCommandState *arg0) {
    u8 temp_t7;
    u8 temp_v0;

    if (1) {
        temp_t7 = (arg0->unk107 & 0xFFFFU) - 1;
    }
    arg0->unk107 = temp_t7;
    if ((temp_t7 & 0xFF) == 0) {
        temp_v0 = arg0->unkEB;
        if (temp_v0 != 0) {
            if (arg0->pitchPulseOffset == 0) {
                arg0->unk107 = temp_v0;
                arg0->pitchPulseOffset = arg0->unk119;
                return;
            }
            arg0->pitchPulseOffset = 0;
            arg0->unk107 = arg0->unk106;
        }
    }
}

void soundPlayerUpdateVibrato(PlayerCommandState *arg0) {
    s32 temp_v1;
    register f32 temp_fv1;
    u8 temp_v0;

    temp_v0 = arg0->unkE9;
    if (temp_v0 != 0) {
        temp_v1 = arg0->noteAgeTicks - arg0->unk105;
        if (temp_v1 > 0) {
            temp_fv1 = sinf((f32)((f64)((temp_v1 / (f32)temp_v0) * ((float)2.0)) * 3.1415926000000001)) * arg0->vibratoDepth;
            arg0->vibratoPitchOffset = temp_fv1;
        }
    }
}

// soundPlayerUpdateVolumeTrack best match: 99.020% (nonmatchings/soundPlayerUpdateVolumeTrack-3242520251544044307/base_15.c)
#pragma GLOBAL_ASM("asm/nonmatchings/audio/audio_engine/soundPlayerUpdateVolumeTrack.s")

#ifdef NON_MATCHING
void soundPlayerUpdateVolumeTrack(PlayerCommandState *arg0) {
    short high_mask;
    u16 temp_c8;
    s32 one = 1;
    register u8 *temp_v1;
    int new_var;
    register u8 temp_v0;
    register u8 *temp_t1;
    short temp_s16;
    register u8 temp_v0_2;
    register u8 *temp_t3;
    register u16 temp_t6;
    register u16 temp_t9;
    register u16 temp_t0;

    if ((u32)arg0->unk4 < (u32)arg0->unk0) {
        do {
            temp_t9 = (temp_c8 = arg0->unkC8) - one;
            arg0->unk4 = arg0->unk4 + 0x100;
            arg0->unkC8 = temp_t9;
            temp_t0 = (temp_t9 ^ (temp_v0 = 0)) & 0xFFFF;
            if (temp_t0 == 0) {
                temp_v1 = (u8 *)arg0->unk60;
                temp_v0 = *temp_v1;
                temp_t1 = temp_v1 + one;
                arg0->unk60 = (s32)temp_t1;
                if ((s32)temp_v0 >= 0x80) {
                    temp_t6 = temp_v0 & 0x7F;
                    arg0->unkEF = temp_t6;
                    temp_v0_2 = *temp_t1;
                    do {
                        ;
                        temp_t3 = (temp_t1 = temp_t1 + one);
                        arg0->unk60 = (s32)temp_t3;
                    } while (0);
                    high_mask = temp_v0_2 & 0x7F;
                    temp_t0 = one;
                    if ((s32)temp_v0_2 >= 0x80) {
                        temp_s16 = (temp_t6 = high_mask << 8);
                        arg0->unkC8 = temp_t6;
                        arg0->unkC8 = temp_t6 + *temp_t3 + (new_var = 2);
                        arg0->unk60 = (s32)(temp_t3 + temp_t0);
                    } else {
                        arg0->unkC8 = temp_v0_2 + 2;
                    }
                } else {
                    arg0->unkEF = temp_v0;
                    arg0->unkC8 = one;
                }
            }
        } while ((u32)arg0->unk4 < (u32)arg0->unk0);
    }
}
#endif

// soundPlayerUpdatePanTrack best match: 98.824% (nonmatchings/soundPlayerUpdatePanTrack-8808947407184708385/base_11.c)
#pragma GLOBAL_ASM("asm/nonmatchings/audio/audio_engine/soundPlayerUpdatePanTrack.s")

#ifdef NON_MATCHING
void soundPlayerUpdatePanTrack(PlayerCommandState *arg0) {
    u16 temp_ca;
    s32 one = 1;
    int new_var;
    register u8 *temp_v1;
    register u8 temp_v0;
    register u8 *temp_t1;
    short temp_s16;
    register u8 *temp_t3;
    register u16 temp_t6;
    register u16 temp_t9;
    register u16 temp_t0;

    if ((u32)arg0->unk8 < (u32)arg0->unk0) {
        do {
            temp_t9 = (temp_ca = arg0->unkCA) - one;
            arg0->unk8 = arg0->unk8 + 0x100;
            arg0->unkCA = temp_t9;
            temp_t0 = (temp_t9 ^ (temp_v0 = 0)) & 0xFFFF;
            if (!temp_t0) {
                temp_v1 = (u8 *)arg0->unk68;
                temp_v0 = *temp_v1;
                temp_t1 = temp_v1 + one;
                arg0->unk68 = (s32)temp_t1;
                if ((s32)temp_v0 >= 0x80) {
                    temp_t6 = temp_v0 & 0x7F;
                    arg0->pitchBendValue = temp_t6;
                    temp_v0 = *temp_t1;
                    do {
                        ;
                        temp_t3 = temp_t1 + one;
                        arg0->unk68 = (s32)temp_t3;
                    } while (0);
                    temp_t0 = one;
                    if ((s32)temp_v0 >= 0x80) {
                        temp_s16 = (((temp_v0 & 0x7F) & 0xFFFF) & 0xFFu) << 8;
                        temp_t6 = temp_s16;
                        arg0->unkCA = (short) temp_t6;
                        arg0->unkCA = temp_t6 + *temp_t3 + (new_var = 2);
                        arg0->unk68 = (s32)(temp_t3 + temp_t0);
                    } else {
                        arg0->unkCA = temp_v0 + 2;
                    }
                } else {
                    arg0->pitchBendValue = temp_v0;
                    arg0->unkCA = one;
                }
            }
        } while ((u32)arg0->unk8 < (u32)arg0->unk0);
    }
}
#endif

f32 approximatePitchRatio(f32 arg0) {
    f32 square;
    f32 fourth;

    if (arg0 == 0.0f) {
        return 1.0f;
    }

    if (arg0 > 0.0f) {
        square = arg0 * arg0;
        fourth = square * square;
        return (f32)(((f64)arg0 * 0.69314718055994495) + 1.0 +
            ((f64)square * 0.240226506959101) +
            ((f64)(square * arg0) * 0.055504108664821597) +
            ((f64)fourth * 0.0096181291076284803) +
            ((f64)(fourth * arg0) * 0.00133335581464284) +
            ((f64)(fourth * square) * 0.00015403530393381601));
    }

    arg0 = -arg0;
    square = arg0 * arg0;
    fourth = square * square;
    return (f32)(1.0 / (((f64)arg0 * 0.69314718055994495) + 1.0 +
        ((f64)square * 0.240226506959101) +
        ((f64)(square * arg0) * 0.055504108664821597) +
        ((f64)fourth * 0.0096181291076284803) +
        ((f64)(fourth * arg0) * 0.00133335581464284) +
        ((f64)(fourth * square) * 0.00015403530393381601)));
}

// loadSoundBank best match: 99.845%

#pragma GLOBAL_ASM("asm/nonmatchings/audio/audio_engine/loadSoundBank.s")

#ifdef NON_MATCHING
extern u8 *gSoundWaveTable;
extern s32 gSoundBankEntryCount;
extern PlayerCommandBank *gCurrentSoundBank;

void loadSoundBank(PlayerCommandBank *bank, s32 sampleBaseOffset) {
    s32 count;
    s32 i;
    s32 offset;
    ALWaveTable *wave;

    gCurrentSoundBank = bank;
    count = bank->waveCount;
    gSoundBankEntryCount = ((count & 0xFFFFFFFFFFFFFFFF) & 0xFFFFFFFFFFFFFFFF) & 0xFFFFFFFFFFFFFFFF;
    gSoundWaveTable = alHeapDBAlloc(0, 0, &gSoundPlayerHeap, 1, count * 4);

    i = 0;
    if (gSoundBankEntryCount > 0) {
        offset = 0;
        do {
            *(ALWaveTable **)(gSoundWaveTable + offset) =
                (ALWaveTable *)(*(s32 *)((u8 *)gCurrentSoundBank->waveTableOffsets + offset) + (s32)gCurrentSoundBank);
            wave = *(ALWaveTable **)(gSoundWaveTable + offset);
            if (wave->flags == 0) {
                do {
                    wave->base += sampleBaseOffset;
                    (*(ALWaveTable **)(gSoundWaveTable + offset))->flags = 1;
                    wave = *(ALWaveTable **)(gSoundWaveTable + offset);
                } while (0);
                if (wave->waveInfo.adpcmWave.loop != 0) {
                    wave->waveInfo.adpcmWave.loop =
                        (ALADPCMloop *)((s32)wave->waveInfo.adpcmWave.loop + (s32)gCurrentSoundBank);
                    wave = *(ALWaveTable **)(gSoundWaveTable + offset);
                }
                if (wave->type == AL_ADPCM_WAVE) {
                    wave->waveInfo.adpcmWave.book =
                        (ALADPCMBook *)((s32)wave->waveInfo.adpcmWave.book + (s32)bank);
                }
            }
            i++;
            offset += 4;
        } while (i < gSoundBankEntryCount);
    }
    osWritebackDCacheAll();
}
#endif

s32 soundPlayerRandom(s32 arg0) {
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 var_v0;
    f32 temp_f0;
    f32 temp_f10;

    for (var_v0 = 0; var_v0 != 8;) {
        var_v0 += 4;
        temp_v1 = gSoundPlayerRandomSeed & 0x48000000;
        gSoundPlayerRandomSeed *= 2;
        if ((temp_v1 == 0x48000000) || (temp_v1 == 0x08000000)) {
            gSoundPlayerRandomSeed |= 1;
        }
        temp_v1_2 = gSoundPlayerRandomSeed & 0x48000000;
        gSoundPlayerRandomSeed *= 2;
        if ((temp_v1_2 == 0x48000000) || (temp_v1_2 == 0x08000000)) {
            gSoundPlayerRandomSeed |= 1;
        }
        temp_v1_3 = gSoundPlayerRandomSeed & 0x48000000;
        gSoundPlayerRandomSeed *= 2;
        if ((temp_v1_3 == 0x48000000) || (temp_v1_3 == 0x08000000)) {
            gSoundPlayerRandomSeed |= 1;
        }
        temp_v1_4 = gSoundPlayerRandomSeed & 0x48000000;
        gSoundPlayerRandomSeed *= 2;
        if ((temp_v1_4 == 0x48000000) || (temp_v1_4 == 0x08000000)) {
            gSoundPlayerRandomSeed |= 1;
        }
    }

    temp_f0 = (f32)gSoundPlayerRandomSeed / 65536.0f;
    temp_f10 = (f32)arg0;
    temp_f0 = temp_f0 / 65536.0f;
    return (s32)(temp_f10 * temp_f0);
}

void resetSoundPlayerState(PlayerCommandState *arg0) {
    u8 temp_v0;
    PlayerCommandClearBlock *var_v1;
    s32 var_a1;
    u16 temp_t9;

    temp_v0 = arg0->unkE4;
    arg0->sequencePos = 0;

    var_v1 = (PlayerCommandClearBlock *)arg0;
    var_a1 = 0;
loop:
    var_a1 += 4;
    var_v1->unk1 = 0;
    var_v1->unk2 = 0;
    var_v1->unk3 = 0;
    var_v1++;
    var_v1[-1].unk0 = 0;
    if (var_a1 != sizeof(PlayerCommandState)) {
        goto loop;
    }

    arg0->unkB6 = 0xFFFF;
    arg0->unkE2 = 0xFF;
    arg0->unkE3 = 0xFF;
    arg0->cachedPitch = 99.9f;
    temp_t9 = 0x6000 / gAudioTicksPerSecond;
    arg0->unkBC = 1;
    arg0->unkED = 0;
    arg0->unkEE = 0x7F;
    arg0->unkEF = 0x7F;
    arg0->pitchBendValue = 0x40;
    arg0->pitchBendDepth = 2;
    *(u8 *)&arg0->unkF2 = 0x40;
    arg0->unkC8 = 1;
    arg0->unkCA = 1;
    arg0->fadeTarget = -1;
    arg0->unkB0 = 0x80;
    arg0->unkBA = temp_t9;
    arg0->unkB8 = temp_t9;
    arg0->unkB2 = 0x80;
    arg0->unkB4 = 0x80;
    arg0->unkE4 = temp_v0;
    arg0->soundId = 0;
}

s32 findFreeSoundPlayerIndex(s32 arg0, s32 arg1) {
    s32 i;
    PlayerCommandState *entry;

    entry = gSoundPlayerStates;
    i = 0;
    if (gSoundPlayerCount > 0) {
        do {
            if (entry->sequencePos == 0) {
                return i;
            }
            i++;
            entry++;
        } while (i < gSoundPlayerCount);
    }
    return -1;
}

void audioMemset(u8 *p, unsigned char c, s32 n) {
    u8 *q = p;

    while (n--) {
        *(q++) = c;
    }
}

s32 startSoundPlayerState(PlayerCommandState *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 temp_v1;

    resetSoundPlayerState(arg0);
    arg0->soundId = arg1;
    arg0->unkB0 = arg2;
    arg0->unkB2 = arg3;
    arg0->id = gNextSoundPlayerHandle;
    gNextSoundPlayerHandle += 1;
    arg0->unk20 = arg4;
    temp_v1 = libmus_fxheader_current[arg1];
    arg0->restartPos = temp_v1;
    arg0->sequencePos = temp_v1;
    return arg0->id;
}

// initAudioSynthesizer best match: 87.624% (nonmatchings/initAudioSynthesizer-8460208293698481450/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/audio/audio_engine/initAudioSynthesizer.s")

#ifdef NON_MATCHING
void initAudioSynthesizer(SchedulerState *scheduler, ALSynConfig *config, s32 threadPriority,
                          AudioSynthInitConfig *initConfig, s32 dmaBufferCount, s32 dmaBufferSize,
                          s32 retraceRate) {
    s32 dmaMessageCount;
    s32 roundedFrameSamples;
    s32 dmaBufferIndex;
    f32 targetFrameSamples;
    Acmd **cmdList;
    AudioInitTask **task;

    gAudioSchedulerState = (s32)scheduler;
    gAudioDmaBufferSize = dmaBufferSize;
    gAudioDmaState.initialized = 0;
    config->dmaproc = initAudioDmaCallback;
    config->outputRate = osAiSetFrequency(initConfig->outputRate);

    gAudioDmaBufferPool = alHeapDBAlloc(0, 0, config->heap, 1, dmaBufferCount * sizeof(AudioDmaBuffer));
    dmaMessageCount = dmaBufferCount * 2;
    gAudioDmaMessages = alHeapDBAlloc(0, 0, config->heap, 1, dmaMessageCount * sizeof(OSIoMesg));
    gAudioDmaMessageBuffer = alHeapDBAlloc(0, 0, config->heap, 1, dmaMessageCount * sizeof(OSMesg));

    targetFrameSamples = ((f32)(u32)initConfig->frameRate * (f32)config->outputRate) / (f32)retraceRate;
    roundedFrameSamples = (s32)targetFrameSamples;
    gTargetAudioTaskOutputLen = roundedFrameSamples;
    if ((f32)(u32)roundedFrameSamples < targetFrameSamples) {
        roundedFrameSamples++;
        gTargetAudioTaskOutputLen = roundedFrameSamples;
    }
    if (roundedFrameSamples & 0xF) {
        roundedFrameSamples = (roundedFrameSamples & ~0xF) + 0x10;
        gTargetAudioTaskOutputLen = roundedFrameSamples;
    }
    gMinAudioTaskOutputLen = roundedFrameSamples - 0x10;
    gMaxAudioTaskOutputLen = roundedFrameSamples + 0x68;

    alInit((ALGlobals *)&gAudioSynthesizer, config);

    gAudioDmaBufferPool->node.prev = NULL;
    gAudioDmaBufferPool->node.next = NULL;
    dmaBufferCount--;
    dmaBufferIndex = 0;
    if (dmaBufferCount != 0) {
        do {
            alLink((ALLink *)&gAudioDmaBufferPool[dmaBufferIndex + 1], (ALLink *)&gAudioDmaBufferPool[dmaBufferIndex]);
            gAudioDmaBufferPool[dmaBufferIndex].buffer =
                alHeapDBAlloc(0, 0, config->heap, 1, dmaBufferSize);
            dmaBufferIndex++;
        } while (dmaBufferIndex != dmaBufferCount);
    }
    gAudioDmaBufferPool[dmaBufferIndex].buffer =
        alHeapDBAlloc(0, 0, config->heap, 1, dmaBufferSize);

    cmdList = gAudioCmdLists;
    do {
        *cmdList = alHeapDBAlloc(0, 0, config->heap, 1, initConfig->commandListSize * sizeof(Acmd));
        cmdList++;
    } while ((u32)cmdList < (u32)&gAudioCmdListEnd0);

    gAudioCmdListCapacity = initConfig->commandListSize;
    task = (AudioInitTask **)gAudioCmdLists;
    do {
        task[2] = alHeapDBAlloc(0, 0, config->heap, 1, sizeof(AudioInitTask));
        task[2]->type = 2;
        task[2]->msg = task[2];
        task[2]->outBuf =
            alHeapDBAlloc(0, 0, config->heap, 1, gMaxAudioTaskOutputLen * sizeof(s32));
        task++;
    } while (task != (AudioInitTask **)&gAudioCmdListEnd1);

    osCreateMesgQueue((OSMesgQueue *)gAudioTaskDoneQueue, gAudioTaskDoneMessages, 8);
    osCreateMesgQueue(&gAudioThreadQueue, gAudioThreadMessages, 8);
    osCreateMesgQueue(&gAudioDmaQueue, gAudioDmaMessageBuffer, dmaMessageCount);
    if (gAudioThreadStarted == 0) {
        osCreateThread(&gAudioThread, 3, audioThreadMain, NULL, &gAudioDmaState, threadPriority);
    }
    osStartThread(&gAudioThread);
    gAudioThreadStarted = 1;
}
#endif

void audioThreadMain(s32 arg0) {
    AudioThreadLocals locals;
    u32 done;

    done = 0;
    addSchedulerClient((SchedulerState *)gAudioSchedulerState, &locals.client, &gAudioThreadQueue);
    do {
        osRecvMesg(&gAudioThreadQueue, &locals.msg, 1);
        switch (((AudioFrameMessage *)locals.msg)->type) {
        case 3:
            break;
        case 1:
            if (buildAudioTask((AudioTask *)gAudioCmdLists[(gAudioFrameCounter % 3) + 2], gNextAudioInfo) != 0) {
                osRecvMesg((OSMesgQueue *)gAudioTaskDoneQueue, &locals.msg, 1);
                updateAudioUnderrunState((s32)((AudioFrameMessage *)locals.msg)->info);
                gNextAudioInfo = ((AudioFrameMessage *)locals.msg)->info;
            }
            break;
        case 10:
            done = 1;
            break;
        }
    } while (done == 0);
    alClose((ALGlobals *)&gAudioSynthesizer);
}

s32 buildAudioTask(AudioTask *task, AudioInfo *info) {
    u32 outBuf;
    AudioTask *task3;
    s32 cmdLen[3];
    AudioTask *task2;
    Acmd *cmdListEnd;

    reclaimAudioDmaBuffers();
    outBuf = osVirtualToPhysical(task->outBuf);

    if (info != NULL) {
        if (!aspMainTextStart) {
        }
        osAiSetNextBuffer(info->buf, info->len * 4);
    }

    task->outLen = ((gTargetAudioTaskOutputLen - (osAiGetLength() >> 2)) + 0x68) & 0xFFF0;
    if ((u32)task->outLen < (u32)gMinAudioTaskOutputLen) {
        task->outLen = gMinAudioTaskOutputLen;
    }

    cmdListEnd = alAudioFrame(gAudioCmdLists[gAudioCmdListIndex], &cmdLen[2], (s16 *)outBuf, task->outLen);
    if (cmdLen[2] == 0) {
        return 0;
    }

    task3 = task;
    task3->unk8 = 0;
    task3->msgQ = (OSMesgQueue *)gAudioTaskDoneQueue;
    task3->msg = (OSMesg)&task3->unk68;
    task3->unk10 = 0;
    task3->dataPtr = gAudioCmdLists[gAudioCmdListIndex];
    task3->dataSize = (((s32)cmdListEnd - (s32)gAudioCmdLists[gAudioCmdListIndex]) >> 3) << 3;

    task3->type = 2;
    task3->ucodeBoot = rspbootTextStart;
    task2 = task3;
    task2->ucodeBootSize = (u8 *)aspMainTextStart - (u8 *)rspbootTextStart;
    task3->flags = 0;
    task3->ucode = aspMainTextStart;
    task3->ucodeData = aspMainDataStart;
    task3->ucodeDataSize = 0x800;
    task3->dramStack = NULL;
    task3->dramStackSize = 0;
    task3->outputBuff = NULL;
    task3->outputBuffSize = NULL;
    task3->yieldDataPtr = NULL;
    task3->yieldDataSize = 0;

    osSendMesg(getSchedulerAudioTaskQueue(gAudioSchedulerState), &task3->unk8, 1);
    gAudioCmdListIndex ^= 1;
    return 1;
}

void updateAudioUnderrunState(s32 arg0) {
    s32 temp;

    if ((osAiGetLength() >> 2) == 0) {
        temp = 0;
        if (gAudioUnderrunState != temp) {
            return;
        }
        gAudioUnderrunState = temp;
    }
}

s32 audioDmaCallback(s32 addr, s32 len, void *state) {
    void *foundBuffer;
    s32 delta;
    s32 addrEnd;
    s32 buffEnd;
    AudioDmaBuffer *dmaPtr;
    AudioDmaBuffer *lastDmaPtr;
    ALLink *first;
    s32 dmaLen;

    lastDmaPtr = NULL;
    first = gAudioDmaState.activeList;
    dmaPtr = (AudioDmaBuffer *)first;
    if (dmaPtr != NULL) {
        dmaLen = gAudioDmaBufferSize;
        do {
            if ((u32)addr < (u32)dmaPtr->addr) {
                break;
            }
            lastDmaPtr = dmaPtr;
            addrEnd = addr + len;
            delta = dmaLen;
            buffEnd = dmaPtr->addr + delta;
            if (addrEnd <= buffEnd) {
                dmaPtr->counter = gAudioFrameCounter;
                buffEnd = dmaPtr->addr;
                foundBuffer = (u8 *)dmaPtr->buffer + addr - buffEnd;
                return osVirtualToPhysical(foundBuffer);
            }
            dmaPtr = (AudioDmaBuffer *)dmaPtr->node.next;
        } while (dmaPtr != NULL);
    }

    dmaPtr = (AudioDmaBuffer *)gAudioDmaState.readyList;
    if (dmaPtr == NULL) {
        return osVirtualToPhysical(first);
    }

    gAudioDmaState.readyList = dmaPtr->node.next;
    alUnlink((ALLink *)dmaPtr);
    if (lastDmaPtr != NULL) {
        alLink((ALLink *)dmaPtr, (ALLink *)lastDmaPtr);
    } else {
        first = gAudioDmaState.activeList;
        if (first != NULL) {
            gAudioDmaState.activeList = (ALLink *)dmaPtr;
            dmaPtr->node.next = first;
            dmaPtr->node.prev = NULL;
            first->prev = (ALLink *)dmaPtr;
        } else {
            gAudioDmaState.activeList = (ALLink *)dmaPtr;
            dmaPtr->node.next = NULL;
            dmaPtr->node.prev = NULL;
        }
    }

    delta = addr & 1;
    addr -= delta;
    dmaPtr->addr = addr;
    dmaPtr->counter = gAudioFrameCounter;
    foundBuffer = dmaPtr->buffer;
    osPiStartDma(&gAudioDmaMessages[gPendingAudioDmaCount++], 0, 0, addr, foundBuffer, gAudioDmaBufferSize,
                 &gAudioDmaQueue);
    return osVirtualToPhysical(foundBuffer) + delta;
}

ALDMAproc initAudioDmaCallback(AudioDmaState **arg0) {
    if (gAudioDmaState.initialized == 0) {
        gAudioDmaState.activeList = 0;
        gAudioDmaState.readyList = &gAudioDmaBufferPool->node;
        gAudioDmaState.initialized = 1;
    }
    *arg0 = &gAudioDmaState;
    return audioDmaCallback;
}

void reclaimAudioDmaBuffers(void) {
    OSMesg msg[2];
    u32 i;
    AudioDmaBuffer *node;
    AudioDmaBuffer *next;

    i = 0;
    if (gPendingAudioDmaCount != 0) {
        do {
            osRecvMesg(&gAudioDmaQueue, msg, 0);
            i++;
        } while (i < gPendingAudioDmaCount);
    }

    node = (AudioDmaBuffer *)gAudioDmaState.activeList;
    if (node != NULL) {
        do {
            next = (AudioDmaBuffer *)node->node.next;
            if ((node->counter + 1) < gAudioFrameCounter) {
                if ((ALLink *)node == gAudioDmaState.activeList) {
                    gAudioDmaState.activeList = &((AudioDmaBuffer *)node->node.next)->node;
                }
                alUnlink(&node->node);
                if (gAudioDmaState.readyList != NULL) {
                    alLink(&node->node, gAudioDmaState.readyList);
                } else {
                    gAudioDmaState.readyList = &node->node;
                    node->node.next = (node->node.prev = NULL);
                }
            }
            node = next;
            i = 0;
        } while (next != NULL);
    }

    gPendingAudioDmaCount = 0;
    gAudioFrameCounter++;
}
