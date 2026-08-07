#include "game/audio/audio_engine_internal.h"

#pragma weak schedulerThreadEntry = schedulerThreadMain

u16 gRetraceCounter = 0;
s32 gSchedulerYieldResult = 0;
s32 gSchedulerYieldRequested = 0;
s32 gSchedulerStartupRetraceCount = 0;

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
    osCreateThread(
        &arg0->thread258,
        6,
        schedulerThreadEntry,
        arg0,
        gSchedulerThreadStack + sizeof(gSchedulerThreadStack),
        0x78
    );
    osStartThread(&arg0->thread258);
    osCreateThread(
        &arg0->thread408,
        5,
        (void (*)(void *))schedulerSwapBufferThreadMain,
        arg0,
        gSchedulerSwapBufferThreadStack + sizeof(gSchedulerSwapBufferThreadStack),
        0x64
    );
    osStartThread(&arg0->thread408);
}

OSMesgQueue *getSchedulerAudioTaskQueue(SchedulerState *scheduler) {
    return &scheduler->messageQueue;
}

OSMesgQueue *getSchedulerGraphicsTaskQueue(SchedulerState *scheduler) {
    return &scheduler->eventQueue;
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void schedulerThreadMain(SchedulerState *arg0) {
    s32 started;
    s32 delayedStart;
    s32 pendingAudio;
    SchedulerThreadMainLocals locals;

    started = 0;
    delayedStart = 0;
    pendingAudio = 0;
    locals.msg = NULL;
    gSchedulerStartupRetraceCount = 0;
    locals.queue = &arg0->retraceQueue; loop: osRecvMesg(locals.queue, &locals.msg, 1); switch ((s32)locals.msg) { case 0x29A: gRetraceCounter = gRetraceCounter + 1; gRetraceCounter = gRetraceCounter & 0xFFF; if ((started == 0) || (gSchedulerStartupRetraceCount < ((0, 0x16)))) { tryStartPendingRdpTask(arg0); pendingAudio = pendingAudio * 0; if (gSchedulerRdpTaskActive != 0) { pendingAudio = 1;
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
                    osSpTaskLoad(&arg0->curRSPTask->rspTask);
                    osSpTaskStartGo(&arg0->curRSPTask->rspTask);
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
                osSpTaskLoad(&arg0->curRSPTask->rspTask);
                osSpTaskStartGo(&(*arg0).curRSPTask->rspTask);
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
// clang-format on

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
        if (osSpTaskYielded(&arg0->curRSPTask->rspTask) != 0) {
            gSchedulerYieldResult = 1;
        } else {
            gSchedulerYieldResult = 2;
        }
    }
    gSchedulerRspTaskState |= 2;
    osWritebackDCacheAll();
    osSpTaskLoad(&arg0->curRDPTask->rspTask);
    osSpTaskStartGo(&arg0->curRDPTask->rspTask);
}

void finishCurrentRdpTask(SchedulerState *arg0) {
    OSMesg msg;

    msg = 0;
    osWritebackDCacheAll();
    if (gSchedulerYieldResult == 1) {
        osSpTaskLoad(&arg0->curRSPTask->rspTask);
        osSpTaskStartGo(&arg0->curRSPTask->rspTask);
    } else if (gSchedulerYieldResult == 2) {
        gSchedulerRspTaskState &= ~1;
        osSendMesg(&arg0->queue14C, &msg, 1);
    }
    osSendMesg(arg0->curRDPTask->doneQueue, arg0->curRDPTask->doneMsg, 1);
    gSchedulerRdpTaskActive = 0;
}

void schedulerSwapBufferThreadMain(SchedulerState *scheduler) {
    OSMesg msg;
    SchedulerTask *task;
    void *framebuffer;
    s16 retrace;

    msg = NULL;
    while (1) {
        osRecvMesg(&scheduler->eventQueue, (OSMesg *)&scheduler->curRSPTask, 1);
        waitForFramebufferAvailable(scheduler, scheduler->curRSPTask);
        osSendMesg(&scheduler->retraceQueue, (OSMesg)1, 1);
        osRecvMesg(&scheduler->queue14C, &msg, 1);
        osRecvMesg(&scheduler->queue1A4, &msg, 1);

        task = scheduler->curRSPTask;
        if (task->flags & 0x40) {
            framebuffer = task->framebuffer;
            retrace = task->retrace;
            osSendMesg(task->doneQueue, task->doneMsg, 1);
            while (((gRetraceCounter - retrace) & 0xFFF) > 0x800) {
                waitForNextFramebufferEvent(scheduler);
            }
            if (scheduler->doAudio != 0) {
                osViBlack(0);
                scheduler->doAudio = 0;
            }
            osViSwapBuffer(framebuffer);
        }
    }
}
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
