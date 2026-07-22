#include "game/audio/audio_engine_internal.h"

// initAudioSynthesizer best match: 99.961% (nonmatchings/initAudioSynthesizer-8699393380584516020/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/libmus/aud_thread/initAudioSynthesizer.s")

#ifdef NON_MATCHING
void initAudioSynthesizer(SchedulerState *scheduler, ALSynConfig *config, s32 threadPriority,
                          AudioSynthInitConfig *initConfig, s32 dmaBufferCount, s32 dmaBufferSize,
                          s32 retraceRate) {
    s32 dmaMessageCount;
    u32 i;
    f32 targetFrameSamples;

    gAudioSchedulerState = (s32)scheduler;
    gAudioDmaBufferSize = dmaBufferSize;
    gAudioDmaState.initialized = 0;
    config->dmaproc = initAudioDmaCallback;
    config->outputRate = osAiSetFrequency(initConfig->outputRate);

    gAudioDmaBufferPool = alHeapDBAlloc(0, 0, config->heap, 1, dmaBufferCount * sizeof(AudioDmaBuffer));
    dmaMessageCount = dmaBufferCount * 2;
    i = dmaMessageCount * sizeof(OSMesg);
    gAudioDmaMessages = alHeapDBAlloc(0, 0, config->heap, 1, dmaMessageCount * sizeof(OSIoMesg));
    gAudioDmaMessageBuffer = alHeapDBAlloc(0, 0, config->heap, 1, i);

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

    i = 0;
    do {
        gAudioCmdLists[i] = alHeapDBAlloc(0, 0, config->heap, 1, initConfig->commandListSize * sizeof(Acmd));
        i++;
    } while (&gAudioCmdLists[i] < &gAudioCmdListEnd0);

    gAudioCmdListCapacity = initConfig->commandListSize;
    i = 0;
    do {
        ((AudioInitTask **)gAudioCmdLists)[i + 2] = alHeapDBAlloc(0, 0, config->heap, 1, sizeof(AudioInitTask));
        ((AudioInitTask **)gAudioCmdLists)[i + 2]->type = 2;
        ((AudioInitTask **)gAudioCmdLists)[i + 2]->msg = ((AudioInitTask **)gAudioCmdLists)[i + 2];
        ((AudioInitTask **)gAudioCmdLists)[i + 2]->outBuf =
            alHeapDBAlloc(0, 0, config->heap, 1, gMaxAudioTaskOutputLen * sizeof(s32));
        i++;
    } while (&gAudioCmdListEnd1 != &gAudioCmdLists[i]);

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
