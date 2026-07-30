#include "game/audio/audio_engine_internal.h"

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

u32 gAudioFrameCounter = 0;
u32 gPendingAudioDmaCount = 0;
s32 gAudioCmdListIndex = 0;
s32 gAudioThreadStarted = 0;
AudioInfo *gNextAudioInfo = NULL;
s32 gAudioUnderrunState = 1;

void initAudioSynthesizer(SchedulerState *scheduler, ALSynConfig *config, s32 threadPriority,
                          AudioSynthInitConfig *initConfig, s32 dmaBufferCount, s32 dmaBufferSize,
                          s32 retraceRate) {
    u32 i;
    f32 targetFrameSamples;

    gAudioSchedulerState = scheduler;
    gAudioDmaBufferSize = dmaBufferSize;
    gAudioDmaState.initialized = 0;
    config->dmaproc = initAudioDmaCallback;
    config->outputRate = osAiSetFrequency(initConfig->outputRate);

    gAudioDmaBufferPool = alHeapDBAlloc(0, 0, config->heap, 1, dmaBufferCount * sizeof(AudioDmaBuffer));
    gAudioDmaMessages = alHeapDBAlloc(0, 0, config->heap, 1, dmaBufferCount * 2 * sizeof(OSIoMesg));
    gAudioDmaMessageBuffer = alHeapDBAlloc(0, 0, config->heap, 1, dmaBufferCount * 2 * sizeof(OSMesg));

    targetFrameSamples = ((f32)(u32)initConfig->frameRate * (f32)config->outputRate) / (f32)retraceRate;
    gTargetAudioTaskOutputLen = (s32)targetFrameSamples;
    if ((f32)(u32)gTargetAudioTaskOutputLen < targetFrameSamples) {
        gTargetAudioTaskOutputLen++;
    }
    if (gTargetAudioTaskOutputLen & 0xF) {
        gTargetAudioTaskOutputLen = (gTargetAudioTaskOutputLen & ~0xF) + 0x10;
    }
    gMinAudioTaskOutputLen = gTargetAudioTaskOutputLen - 0x10;
    gMaxAudioTaskOutputLen = gTargetAudioTaskOutputLen + 0x68;

    alInit((ALGlobals *)&gAudioSynthesizer, config);

    gAudioDmaBufferPool->node.prev = NULL;
    gAudioDmaBufferPool->node.next = NULL;
    for (i = 0; i < dmaBufferCount - 1; i++) {
        alLink((ALLink *)&gAudioDmaBufferPool[i + 1], (ALLink *)&gAudioDmaBufferPool[i]);
        gAudioDmaBufferPool[i].buffer = alHeapDBAlloc(0, 0, config->heap, 1, dmaBufferSize);
    }
    gAudioDmaBufferPool[i].buffer = alHeapDBAlloc(0, 0, config->heap, 1, dmaBufferSize);

    for (i = 0; i < ARRAY_COUNT(gAudioWorkBuffers.commandLists); i++) {
        gAudioWorkBuffers.commandLists[i] =
            alHeapDBAlloc(0, 0, config->heap, 1, initConfig->commandListSize * sizeof(Acmd));
    }

    gAudioCmdListCapacity = initConfig->commandListSize;
    for (i = 0; i < ARRAY_COUNT(gAudioWorkBuffers.tasks); i++) {
        gAudioWorkBuffers.tasks[i] = alHeapDBAlloc(0, 0, config->heap, 1, sizeof(AudioInitTask));
        gAudioWorkBuffers.tasks[i]->type = 2;
        gAudioWorkBuffers.tasks[i]->msg = gAudioWorkBuffers.tasks[i];
        gAudioWorkBuffers.tasks[i]->outBuf =
            alHeapDBAlloc(0, 0, config->heap, 1, gMaxAudioTaskOutputLen * sizeof(s32));
    }

    osCreateMesgQueue((OSMesgQueue *)gAudioTaskDoneQueue, gAudioTaskDoneMessages, 8);
    osCreateMesgQueue(&gAudioThreadQueue, gAudioThreadMessages, 8);
    osCreateMesgQueue(&gAudioDmaQueue, gAudioDmaMessageBuffer, dmaBufferCount * 2);
    if (gAudioThreadStarted == 0) {
        osCreateThread(&gAudioThread, 3, audioThreadMain, NULL,
                       gAudioThreadStack + sizeof(gAudioThreadStack), threadPriority);
    }
    osStartThread(&gAudioThread);
    gAudioThreadStarted = 1;
}

void audioThreadMain(void *arg0) {
    AudioThreadLocals locals;
    u32 done;

    done = 0;
    addSchedulerClient(gAudioSchedulerState, &locals.client, &gAudioThreadQueue);
    do {
        osRecvMesg(&gAudioThreadQueue, &locals.msg, 1);
        switch (((AudioFrameMessage *)locals.msg)->type) {
        case 3:
            break;
        case 1:
            if (buildAudioTask((AudioTask *)gAudioWorkBuffers.tasks[gAudioFrameCounter % 3], gNextAudioInfo) != 0) {
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

    cmdListEnd = alAudioFrame(gAudioWorkBuffers.commandLists[gAudioCmdListIndex], &cmdLen[2], (s16 *)outBuf,
                              task->outLen);
    if (cmdLen[2] == 0) {
        return 0;
    }

    task3 = task;
    task3->unk8 = 0;
    task3->msgQ = (OSMesgQueue *)gAudioTaskDoneQueue;
    task3->msg = (OSMesg)&task3->unk68;
    task3->unk10 = 0;
    task3->dataPtr = gAudioWorkBuffers.commandLists[gAudioCmdListIndex];
    task3->dataSize = (((s32)cmdListEnd - (s32)gAudioWorkBuffers.commandLists[gAudioCmdListIndex]) >> 3) << 3;

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
