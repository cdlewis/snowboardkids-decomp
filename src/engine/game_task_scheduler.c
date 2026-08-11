#include "common.h"
#include "game/engine/game_task_scheduler.h"
#include "game/engine/system_runtime.h"
#include "game/engine/frame_render_task.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"

#define GAME_TASK_COUNT 8

u8 gFramebufferSwapDelayTimer[4] = { 0, 0, 0, 0 };
FramebufferSwapDelay gFramebufferSwapDelay = { 0 };
s8 gAnalogStickResponseCurve[56] = {
    0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  2,  2,  2,  3,  3,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 31, 31, 31, 31, 31, 0, 0,  1,  0,  0,  0,
};
extern s16 gFrameCounter;
extern GameTask gGameTaskPool[GAME_TASK_COUNT];
extern u8 gGameTaskCount;
extern GameTask gActiveGameTaskList;
extern volatile GameTask gActiveGameTaskListView;
extern GameTask *gFreeGameTaskStack[];
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern u8 gNextFramebufferRenderTaskIndex;
extern s32 gPlayerInputPrevious[PLAYER_INPUT_COUNT];
extern s32 gPlayer2InputPrevious;
extern s32 gPlayer3InputPrevious;
extern s32 gPlayer4InputPrevious;
extern s32 gPlayerInputRepeat[PLAYER_INPUT_COUNT];
extern u8 gPlayerInputRepeatTimer[PLAYER_INPUT_COUNT];

CLANG_DIAGNOSTIC_PUSH
CLANG_DIAGNOSTIC_IGNORE_DEPRECATED_NON_PROTOTYPE
CLANG_DIAGNOSTIC_IGNORE_STRICT_PROTOTYPES
void resetRenderScratchAllocator();
CLANG_DIAGNOSTIC_POP
void clearPendingPositionalSoundRequests(void);
GameTask *allocateGameTask(s32);
s32 updateFramebufferRenderScheduler(void);
void playPendingPositionalSoundRequests(void);

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initGameTaskScheduler(void) {
    GameTask **freeTask;
    GameTask *task;
    s32 zero;

    gActiveGameTaskList.next = NULL;
    gActiveGameTaskList.priority = 0;
    freeTask = gFreeGameTaskStack; task = gGameTaskPool; do { *freeTask = task; task++; freeTask++; } while (task < &gGameTaskPool[GAME_TASK_COUNT]);
    gGameTaskCount = 0;
    gFrameCounter = 0;
    gPendingFramebufferSwapCount = 2;
    gFramebufferSwapHold = 0;
    zero = 0;
    gNextFramebufferRenderTaskIndex = zero;
    gPlayerInputHeld[0] = zero;
    gPlayerInputPrevious[0] = zero;
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
    resetRenderScratchAllocator(&gGameTaskPool[GAME_TASK_COUNT], &gActiveGameTaskList);
    resetRenderCallbackQueues();
}
// clang-format on

void updateGameTaskScheduler(void) {
    s32 i;
    s32 *previousInput;
    ControllerInputState *controller;
    s32 oldInput;
    s32 currentInput;
    s32 mappedStickX;
    s32 mappedStickY;
    s8 stickX;
    s32 stickY;

    gFrameCounter = (gFrameCounter + 1) & 0xFFF;
    resetRenderScratchAllocator();
    resetRenderCallbackQueues();
    clearPendingPositionalSoundRequests();
    for (i = 0; i < PLAYER_INPUT_COUNT; i++) {
        previousInput = &gPlayerInputPrevious[i];
        oldInput = gPlayerInputHeld[i];
        *previousInput = oldInput;
        controller = &gControllerInputState[i];
        currentInput = oldInput & 0xFFFF0000;
        ((u32 *)gPlayerInputHeld)[i] = currentInput;
        gPlayerInputHeld[i] |= controller->buttons;
        stickX = controller->stickX;
        if (stickX >= 0x2E) {
            controller->stickX = 0x2D;
            stickX = controller->stickX;
        }
        if (stickX < -0x2D) {
            controller->stickX = -0x2D;
        }
        stickY = controller->stickY;
        if (stickY >= 0x2E) {
            controller->stickY = 0x2D;
            stickY = controller->stickY;
        }
        if (stickY < -0x2D) {
            controller->stickY = -0x2D;
            stickY = controller->stickY;
        }
        stickX = controller->stickX;
        if (stickX >= 0) {
            gPlayerStickX[i] = gAnalogStickResponseCurve[stickX];
        } else {
            gPlayerStickX[i] = -gAnalogStickResponseCurve[-stickX];
        }
        if (stickY >= 0) {
            gPlayerStickY[i] = gAnalogStickResponseCurve[stickY];
        } else {
            gPlayerStickY[i] = -gAnalogStickResponseCurve[-stickY];
        }
        mappedStickX = gPlayerStickX[i];
        if (mappedStickX >= 0x1B) {
            gPlayerInputHeld[i] |= 0x40000;
        }
        if (mappedStickX < -0x1A) {
            gPlayerInputHeld[i] |= 0x80000;
        }
        mappedStickY = gPlayerStickY[i];
        if (mappedStickY >= 0x1B) {
            gPlayerInputHeld[i] |= 0x10000;
        }
        if (mappedStickY < -0x1A) {
            gPlayerInputHeld[i] |= 0x20000;
        }
        if (mappedStickX < 8) {
            gPlayerInputHeld[i] &= 0xFFFBFFFF;
        }
        if (mappedStickX >= -7) {
            gPlayerInputHeld[i] &= 0xFFF7FFFF;
        }
        if (mappedStickY < 8) {
            gPlayerInputHeld[i] &= 0xFFFEFFFF;
        }
        if (mappedStickY >= -7) {
            gPlayerInputHeld[i] &= 0xFFFDFFFF;
        }
        currentInput = gPlayerInputHeld[i];
        gPlayerInputPressed[i] = ~*previousInput & currentInput;
        if (currentInput == 0) {
            gPlayerInputRepeatTimer[i] = 0;
            gPlayerInputRepeat[i] = currentInput;
        } else {
            if (gPlayerInputRepeatTimer[i] >= 9) {
                gPlayerInputRepeat[i] = currentInput;
            } else {
                gPlayerInputRepeatTimer[i]++;
                gPlayerInputRepeat[i] = gPlayerInputPressed[i];
            }
        }
    }

    for (gCurrentGameTask = gActiveGameTaskList.next; gCurrentGameTask != NULL;
         gCurrentGameTask = gCurrentGameTask->next) {
        if (gCurrentGameTask->state == 2) {
            gCurrentGameTask->state = 0;
        }
    }

    for (gCurrentGameTask = gActiveGameTaskList.next; gCurrentGameTask != NULL;
         gCurrentGameTask = gCurrentGameTask->next) {
        if (gCurrentGameTask->state == 0) {
            if (gCurrentGameTask->callbacks[0] != NULL) {
                gCurrentGameTask->callbacks[0]();
            }
            if (gCurrentGameTask->callbacks[1] != NULL) {
                gCurrentGameTask->callbacks[1]();
            }
            if (gCurrentGameTask->callbacks[2] != NULL) {
                gCurrentGameTask->callbacks[2]();
            }
        }
    }

    updateFramebufferRenderScheduler();
    playPendingPositionalSoundRequests();
}

s32 updateFramebufferRenderScheduler(void) {
    u8 frameIndex;

    if (gFramebufferSwapDelayTimer[0] == 0) {
        if (gFramebufferSwapHold == 0) {
            frameIndex = gNextFramebufferRenderTaskIndex;
            if (gFrameRenderTaskStatuses[frameIndex].status == 0) {
                if ((s32)gPendingFramebufferSwapCount > 0) {
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
    volatile GameTask *sentinel;
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

    prev = &gActiveGameTaskList;
    sentinel = &gActiveGameTaskListView;
    gGameTaskCount++;
    if (prev->next != NULL) {
        next = sentinel->next;
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

    task = gActiveGameTaskList.next;
    while (task != NULL) {
        if (taskId == task->id) {
            task->prev->next = task->next;
            next = task->next;
            if (next != NULL) {
                next->prev = task->prev;
            }
            freeTaskCount = (gGameTaskCount & 0xFFu) - 1;
            gGameTaskCount = freeTaskCount;
            gFreeGameTaskStack
                [(u8)(((((((((((freeTaskCount & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) &
                        0xFFu) &
                       0xFFu) &
                      0xFFu)] = task;
        }
        task = task->next;
    }
}

void createGameTask(s32 taskId, GameTaskCallback callback, s32 priority) {
    GameTask *task;

    task = allocateGameTask(priority);
    if (task != NULL) {
        task->id = (u8)taskId;
        task->callbacks[0] = callback;
        task->priority = (u8)priority;
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
    GameTask *task = gActiveGameTaskList.next;

    while (task != NULL) {
        if (taskId == task->id) {
            task->state = 1;
            return;
        }
        task = task->next;
    }
}

void resumeGameTask(s32 taskId) {
    GameTask *task = gActiveGameTaskList.next;

    while (task != NULL) {
        if (taskId == task->id) {
            task->state = 2;
            return;
        }
        task = task->next;
    }
}
