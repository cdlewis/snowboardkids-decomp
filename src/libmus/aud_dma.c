#include "game/audio/audio_engine.h"
#include "game/audio/audio_engine_internal.h"

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
    osPiStartDma(
        &gAudioDmaMessages[gPendingAudioDmaCount++],
        0,
        0,
        addr,
        foundBuffer,
        gAudioDmaBufferSize,
        &gAudioDmaQueue
    );
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
