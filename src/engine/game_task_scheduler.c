#include "common.h"
#include "game/engine/game_task_scheduler.h"
#include "game/engine/system_runtime.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"

#define GAME_TASK_COUNT 8

typedef struct GameTaskScheduler {
    u8 pad0[4];
    GameTask *activeTask;
    u8 pad8[0xC];
    u8 unk14;
} GameTaskScheduler;

extern s8 gAnalogStickResponseCurve;
extern u8 gFramebufferSwapDelayTimer;
extern s16 gFrameCounter;
extern GameTask gGameTaskPool[GAME_TASK_COUNT];
extern u8 gGameTaskCount;
extern GameTaskScheduler gGameTaskScheduler;
extern GameTaskScheduler gGameTaskSchedulerView;
extern GameTask *gActiveGameTaskListHead;
extern GameTask *gFreeGameTaskStack[];
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern u8 gNextFramebufferRenderTaskIndex;
extern s32 gPlayerInputPrevious;
extern s32 gPlayer2InputPrevious;
extern s32 gPlayer3InputPrevious;
extern s32 gPlayer4InputPrevious;
extern s32 gPlayerInputPressed;
extern s32 gPlayer2InputPressed;
extern s32 gPlayer3InputPressed;
extern s32 gPlayer4InputPressed;
extern s8 gPlayerStickX;
extern s8 gPlayer2StickX;
extern s8 gPlayer3StickX;
extern s8 gPlayer4StickX;
extern s8 gPlayerStickY;
extern s8 gPlayer2StickY;
extern s8 gPlayer3StickY;
extern s8 gPlayer4StickY;
extern s32 gPlayerInputRepeat;
extern u8 gPlayerInputRepeatTimer;

void resetRenderScratchAllocator(void *, void *);
void clearPendingPositionalSoundRequests(void);
GameTask *allocateGameTask(s32);
s32 updateFramebufferRenderScheduler(void);
void playPendingPositionalSoundRequests(void);

void initGameTaskScheduler(void) {
    GameTask **freeTask;
    GameTask *task;
    s32 zero;

    gGameTaskScheduler.activeTask = NULL;
    gGameTaskScheduler.unk14 = 0;
    freeTask = gFreeGameTaskStack; task = gGameTaskPool; do { *freeTask = task; task++; freeTask++; } while (task < &gGameTaskPool[GAME_TASK_COUNT]);
    gGameTaskCount = 0;
    gFrameCounter = 0;
    gPendingFramebufferSwapCount = 2;
    gFramebufferSwapHold = 0;
    zero = 0;
    gNextFramebufferRenderTaskIndex = zero;
    gPlayerInputHeld[0] = zero;
    gPlayerInputPrevious = zero;
    gPlayerInputPressed = 0;
    gPlayerStickX = zero;
    gPlayerStickY = 0;
    gPlayer2InputHeld = 0;
    gPlayer2InputPrevious = zero;
    gPlayer2InputPressed = zero;
    gPlayer2StickX = 0;
    gPlayer2StickY = zero;
    gPlayer3InputHeld = zero;
    gPlayer3InputPrevious = 0;
    gPlayer3InputPressed = zero;
    gPlayer3StickX = zero;
    gPlayer3StickY = 0;
    gPlayer4InputHeld = zero;
    gPlayer4InputPrevious = 0;
    gPlayer4InputPressed = 0;
    gPlayer4StickX = zero;
    gPlayer4StickY = zero;
    resetRenderScratchAllocator(&gGameTaskPool[GAME_TASK_COUNT], &gGameTaskScheduler);
    resetRenderCallbackQueues();
}

// updateGameTaskScheduler best match: 94.081% with the current scorer, improved from 93.474%
// (legacy annotation: 95.344%; nonmatchings/updateGameTaskScheduler-8498672362023432715/base_30.c)
#pragma GLOBAL_ASM("asm/nonmatchings/engine/game_task_scheduler/updateGameTaskScheduler.s")

#ifdef NON_MATCHING
void updateGameTaskScheduler(void) {
    s32 *previousInput;
    s32 *input;
    ControllerInputState *controller;
    s8 *stickXOut;
    s8 *stickYOut;
    s32 *newInput;
    u8 *repeatTimer;
    s32 *repeatInput;
    s8 *responseCurve;
    GameTask *task;
    GameTaskCallback callback;
    GameTaskCallback *callbackArray;
    s32 oldInput;
    s32 currentInput;
    s32 newInputValue;
    s32 stickHighThreshold;
    s32 stickXTooHigh;
    s8 stickX;
    s8 stickY;
    s32 timer;

    gFrameCounter = (gFrameCounter + 1) & 0xFFF;
    resetRenderScratchAllocator();
    resetRenderCallbackQueues();
    clearPendingPositionalSoundRequests();

    responseCurve = &gAnalogStickResponseCurve;
    repeatInput = &gPlayerInputRepeat;
    repeatTimer = &gPlayerInputRepeatTimer;
    newInput = &gPlayerInputPressed;
    stickYOut = &gPlayerStickY;
    stickXOut = &gPlayerStickX;
    controller = gControllerInputState;
    input = gPlayerInputHeld;
    previousInput = &gPlayerInputPrevious;

    do {
        stickXTooHigh = controller->stickX >= 0x2E;
        stickHighThreshold = 0x2E;
        oldInput = *input;
        currentInput = oldInput & 0xFFFF0000;
        *input = currentInput;
        *input = currentInput | controller->buttons;
        *previousInput = oldInput;
        do {
        } while (0);

        if (stickXTooHigh) {
            controller->stickX = 0x2D;
        }
        if (controller->stickX < -0x2D) {
            controller->stickX = -0x2D;
        }

        stickY = controller->stickY;
        if (stickY >= stickHighThreshold) {
            controller->stickY = 0x2D;
            stickY = controller->stickY;
        }
        if (stickY < -0x2D) {
            controller->stickY = -0x2D;
            stickY = controller->stickY;
        }

        stickX = controller->stickX;
        controller++;
        if (stickX >= 0) {
            goto positiveStickX;
positiveStickX:
            *stickXOut = responseCurve[stickX];
        } else {
            *stickXOut = -responseCurve[-stickX];
        }

        if (stickY >= 0) {
            *stickYOut = responseCurve[stickY];
        } else {
            *stickYOut = -responseCurve[-stickY];
        }

        stickX = *stickXOut;
        stickXOut++;
        if (stickX > 0x1B - 1) {
            *input |= 0x40000;
        }
        if (stickX < -0x1A) {
            *input |= 0x80000;
        }

        stickY = *stickYOut;
        stickYOut++;
        if (stickY >= 0x1B) {
            *input |= 0x10000;
        }
        if (stickY < -0x1A) {
            *input |= 0x20000;
        }
        if (stickX < 8) {
            *input &= 0xFFFBFFFF;
        }
        if (stickX >= -7) {
            *input &= 0xFFF7FFFF;
        }
        if (stickY < 8) {
            *input &= 0xFFFEFFFF;
        }
        if (stickY >= -7) {
            *input &= 0xFFFDFFFF;
        }

        currentInput = *input;
        *newInput = ~*previousInput & currentInput;
        if (currentInput == 0) {
            *repeatTimer = 0;
            *repeatInput = currentInput;
        } else {
            timer = *repeatTimer;
            if (timer >= 9) {
                *repeatInput = currentInput;
            } else {
                newInputValue = *newInput;
                *repeatTimer = timer + 1;
                *repeatInput = newInputValue;
            }
        }

        repeatInput++;
        previousInput++;
        input++;
        newInput++;
        repeatTimer++;
    } while (repeatInput != (s32 *)&gPlayerInputRepeatTimer);

    task = gActiveGameTaskListHead;
    gCurrentGameTask = task;
    if (task != NULL) {
        do {
            if (task->state == 2) {
                task->state = 0;
                task = gCurrentGameTask;
            }
            task = (gCurrentGameTask = task->next);
        } while (task != NULL);
        gCurrentGameTask = gActiveGameTaskListHead;
    }

    task = gCurrentGameTask;
    if (task != NULL) {
        do {
            if (task->state == 0) {
                callback = task->callbacks[0];
                if (callback != NULL) {
                    callback();
                    task = gCurrentGameTask;
                }
                callback = task->callbacks[1];
                if (callback != NULL) {
                    callback();
                    task = gCurrentGameTask;
                }
                callbackArray = task->callbacks;
                callback = callbackArray[2];
                if (callback != NULL) {
                    callback();
                    task = gCurrentGameTask;
                }
            }
            task = (gCurrentGameTask = task->next);
        } while (task != NULL);
    }

    updateFramebufferRenderScheduler();
    playPendingPositionalSoundRequests();
}
#endif

s32 updateFramebufferRenderScheduler(void) {
    u8 frameIndex;

    if (gFramebufferSwapDelayTimer == 0) {
        if (gFramebufferSwapHold == 0) {
            frameIndex = gNextFramebufferRenderTaskIndex;
            if (gFramebufferRenderTask0Statuses[frameIndex].status == 0) {
                if ((s32) gPendingFramebufferSwapCount > 0) {
                    submitFramebufferRenderTask(frameIndex);
                    gFramebufferSwapDelayTimer = gFramebufferSwapDelay.timerValue;
                    gPendingFramebufferSwapCount--;
                    if (gNextFramebufferRenderTaskIndex != 0) {
                        gNextFramebufferRenderTaskIndex = 0;
                    } else {
                        gNextFramebufferRenderTaskIndex = 1;
                    }
                    goto return_one;
                }
                return 0;
            }
            return 0;
        }
        goto return_one;
    }
    gFramebufferSwapDelayTimer--;

return_one:
    return 1;
}

GameTask *allocateGameTask(s32 priority) {
    GameTask *task;
    GameTask *next;
    GameTask *prev;
    volatile GameTaskScheduler *sentinel;
    u8 *clear;
    s32 i;

    if (gGameTaskCount >= GAME_TASK_COUNT) {
        return NULL;
    }

    task = gFreeGameTaskStack[gGameTaskCount];
    i = 0;
    clear = (u8 *)task;
    do {
        clear[i] = 0;
        i++;
    } while (i != sizeof(GameTask));

    prev = (GameTask *)&gGameTaskScheduler;
    sentinel = &gGameTaskSchedulerView;
    gGameTaskCount++;
    if (prev->next != NULL) {
        next = sentinel->activeTask;
        do {
            if (next->priority < priority) {
                break;
            }
            prev = next;
            next = next->next;
        } while (next != NULL);
    }

    task->prev = prev;
    task->next = prev->next;
    next = prev->next;
    if (next != NULL) {
        next->prev = task;
    }
    prev->next = task;
    return task;
}

void releaseGameTaskById(s32 taskId) {
    GameTask *task;
    GameTask *next;
    s32 freeTaskCount;

    task = gActiveGameTaskListHead;
    while (task != NULL) {
        if (taskId == task->id) {
            task->prev->next = task->next;
            next = task->next;
            if (next != NULL) {
                next->prev = task->prev;
            }
            freeTaskCount = (gGameTaskCount & 0xFFu) - 1;
            gGameTaskCount = freeTaskCount;
            gFreeGameTaskStack[(u8) (((((((((((freeTaskCount & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu)] = task;
        }
        task = task->next;
    }
}

void createGameTask(s32 taskId, GameTaskCallback callback, s32 priority) {
    GameTask *task;

    task = allocateGameTask(priority);
    if (task != NULL) {
        task->id = (u8) taskId;
        task->callbacks[0] = callback;
        task->priority = (u8) priority;
        task->state = 2;
    }
}

void removeGameTask(s32 taskId) {
    releaseGameTaskById(taskId);
}

void setCurrentGameTaskCallback(GameTaskCallback callback, s32 callbackIndex) {
    switch (callbackIndex) {
        case 0:
            gCurrentGameTask->callbacks[0] = callback;
            return;
        case 1:
            gCurrentGameTask->callbacks[1] = callback;
            return;
        case 2:
            gCurrentGameTask->callbacks[2] = callback;
            return;
    }
}

void clearCurrentGameTaskCallback(s32 callbackIndex) {
    switch (callbackIndex) {
        case 0:
            gCurrentGameTask->callbacks[0] = NULL;
            return;
        case 1:
            gCurrentGameTask->callbacks[1] = NULL;
            return;
        case 2:
            gCurrentGameTask->callbacks[2] = NULL;
            return;
    }
}

void suspendGameTask(s32 taskId) {
    GameTask *task = gActiveGameTaskListHead;

    while (task != NULL) {
        if (taskId == task->id) {
            task->state = 1;
            return;
        }
        task = task->next;
    }
}

void resumeGameTask(s32 taskId) {
    GameTask *task = gActiveGameTaskListHead;

    while (task != NULL) {
        if (taskId == task->id) {
            task->state = 2;
            return;
        }
        task = task->next;
    }
}
