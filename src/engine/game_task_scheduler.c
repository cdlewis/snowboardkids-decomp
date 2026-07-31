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

u8 gFramebufferSwapDelayTimer[4] = { 0, 0, 0, 0 };
FramebufferSwapDelay gFramebufferSwapDelay = { 0 };
s8 gAnalogStickResponseCurve[56] = {
     0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
     1,  2,  2,  2,  3,  3,  3,  4,  5,  6,
     7,  8,  9, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
    27, 28, 29, 30, 31, 31, 31, 31, 31, 31,
     0,  0,  1,  0,  0,  0,
};
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
    gPlayerInputPressed[0] = 0;
    gPlayerStickX[0] = zero;
    gPlayerStickY[0] = 0;
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

// updateGameTaskScheduler best match: 97.413% with the current scorer
// (nonmatchings/updateGameTaskScheduler-633030068925474062/base_10.c)
#pragma GLOBAL_ASM("asm/nonmatchings/engine/game_task_scheduler/updateGameTaskScheduler.s")

#ifdef NON_MATCHING
void updateGameTaskScheduler(void) {
    s32 *previousInput;
    s32 *input;
    int new_var;
    ControllerInputState *controller;
    s8 *stickXOut;
    s32 newInputValue;
    s8 *stickYOut;
    s32 *newInput;
    u8 *repeatTimer;
    s32 oldInput;
    s32 *repeatInput;
    s8 *responseCurve;
    GameTask *task;
    GameTaskCallback callback;
    GameTaskCallback *callbackArray;
    s32 currentInput;
    u16 new_var2;
    s32 stickHighThreshold;
    s32 stickXTooHigh;
    s8 stickX;
    s32 stickY;
    s8 mappedStickX;
    s8 mappedStickY;
    s32 timer;

    gFrameCounter = (gFrameCounter + 1) & 0xFFF;
    stickYOut = &gAnalogStickResponseCurve;
    newInput = &gPlayerInputRepeat;
    resetRenderScratchAllocator();
    resetRenderCallbackQueues();
    clearPendingPositionalSoundRequests();

    responseCurve = stickYOut;
    repeatInput = newInput;
    repeatTimer = &gPlayerInputRepeatTimer;
    newInput = gPlayerInputPressed;
    stickYOut = gPlayerStickY;
    stickXOut = gPlayerStickX;
    controller = gControllerInputState;
    input = gPlayerInputHeld;
    previousInput = &gPlayerInputPrevious;

    do {
        stickX = controller->stickX;
        stickHighThreshold = 0x2E;
        oldInput = *input;
        *input &= 0xFFFF0000;
        do {
            *input |= controller->buttons;
            *previousInput = oldInput;
            do {
            } while (0);

            if (stickX >= 0x2E) {
                controller->stickX = 0x2D;
                stickX = controller->stickX;
            }
            if (stickX < -0x2D) {
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
        } while (0);

        stickX = controller->stickX;
        controller++;
        if (stickX >= 0) {
            goto positiveStickX;
positiveStickX:
            *stickXOut = (0, responseCurve)[stickX];
        } else {
            *stickXOut = -responseCurve[-stickX];
        }

        if (stickY >= 0) {
            *stickYOut = responseCurve[stickY];
        } else {
            *stickYOut = -responseCurve[-stickY];
        }

        mappedStickX = *stickXOut;
        stickXOut++;
        if (mappedStickX >= 0x1B) {
            *input |= 0x40000;
        }
        if (mappedStickX < -0x1A) {
            *input |= 0x80000;
        }

        mappedStickY = *stickYOut;
        stickYOut++;
        if (mappedStickY >= 0x1B) {
            *input |= 0x10000;
        }
        if (mappedStickY < -0x1A) {
            *input |= 0x20000;
        }
        if (mappedStickX < 8) {
            *input &= 0xFFFBFFFF;
        }
        if (mappedStickX >= -7) {
            *input &= 0xFFF7FFFF;
        }
        if (mappedStickY < 8) {
            *input &= 0xFFFEFFFF;
        }
        if (mappedStickY >= -7) {
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
        if (1) {
            do {
                if (task->state == 2) {
                    task->state = 0;
                    task = gCurrentGameTask;
                }
                task = (gCurrentGameTask = task->next);
            } while (task != NULL);
        }
        gCurrentGameTask = gActiveGameTaskListHead;
    }

    task = gCurrentGameTask;
    if (task != NULL) {
        do {
            new_var2 = task->state;
            if (new_var2 == 0) {
                callback = task->callbacks[0];
                if (callback != NULL) {
                    callback();
                    task = gCurrentGameTask;
                }
                callback = task->callbacks[1];
                if (callback != NULL) {
                    callback();
                    task = gCurrentGameTask;
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

    if (gFramebufferSwapDelayTimer[0] == 0) {
        if (gFramebufferSwapHold == 0) {
            frameIndex = gNextFramebufferRenderTaskIndex;
            if (gFramebufferRenderTask0Statuses[frameIndex].status == 0) {
                if ((s32) gPendingFramebufferSwapCount > 0) {
                    submitFramebufferRenderTask(frameIndex);
                    gFramebufferSwapDelayTimer[0] = gFramebufferSwapDelay.timerValue;
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
    gFramebufferSwapDelayTimer[0]--;

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
