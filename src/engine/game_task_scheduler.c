#include "common.h"
#include "game/audio/sound_manager.h"
#include "game/engine/game_task_scheduler.h"
#include "game/engine/system_runtime.h"
#include "game/engine/frame_render_task.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"

#define GAME_TASK_COUNT 8
#define RAW_STICK_LIMIT 0x2D
#define STICK_DIRECTION_PRESS_THRESHOLD 0x1B
#define STICK_DIRECTION_RELEASE_THRESHOLD 8
#define PLAYER_INPUT_REPEAT_DELAY 9
#define STICK_DIRECTION_MASK 0xFFFF0000

u8 gFramebufferSubmissionCountdown[4] = { 0, 0, 0, 0 };
FramebufferRenderInterval gFramebufferRenderInterval = { 0 };
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
GameTask *allocateGameTask(s32);
s32 updateFramebufferRenderScheduler(void);

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
    s32 playerIndex;
    s32 *previousInput;
    ControllerInputState *controllerInput;
    s32 previousHeldInput;
    s32 heldInput;
    s32 mappedStickX;
    s32 mappedStickY;
    s8 rawStickX;
    s8 rawStickY;

    gFrameCounter = (gFrameCounter + 1) & 0xFFF;
    resetRenderScratchAllocator();
    resetRenderCallbackQueues();
    clearPendingPositionalSoundRequests();
    for (playerIndex = 0; playerIndex < PLAYER_INPUT_COUNT; playerIndex++) {
        previousInput = &gPlayerInputPrevious[playerIndex];
        previousHeldInput = gPlayerInputHeld[playerIndex];
        *previousInput = previousHeldInput;
        controllerInput = &gControllerInputState[playerIndex];
        heldInput = previousHeldInput & STICK_DIRECTION_MASK;
        *(u32 *)&gPlayerInputHeld[playerIndex] = heldInput;
        gPlayerInputHeld[playerIndex] |= controllerInput->buttons;
        rawStickX = controllerInput->stickX;
        if (rawStickX > RAW_STICK_LIMIT) {
            controllerInput->stickX = RAW_STICK_LIMIT;
            rawStickX = controllerInput->stickX;
        }
        if (rawStickX < -RAW_STICK_LIMIT) {
            controllerInput->stickX = -RAW_STICK_LIMIT;
        }
        rawStickY = controllerInput->stickY;
        if (rawStickY > RAW_STICK_LIMIT) {
            controllerInput->stickY = RAW_STICK_LIMIT;
            rawStickY = controllerInput->stickY;
        }
        if (rawStickY < -RAW_STICK_LIMIT) {
            controllerInput->stickY = -RAW_STICK_LIMIT;
            rawStickY = controllerInput->stickY;
        }
        rawStickX = controllerInput->stickX;
        if (rawStickX >= 0) {
            gPlayerStickX[playerIndex] = gAnalogStickResponseCurve[rawStickX];
        } else {
            gPlayerStickX[playerIndex] = -gAnalogStickResponseCurve[-rawStickX];
        }
        if (rawStickY >= 0) {
            gPlayerStickY[playerIndex] = gAnalogStickResponseCurve[rawStickY];
        } else {
            gPlayerStickY[playerIndex] = -gAnalogStickResponseCurve[-rawStickY];
        }
        mappedStickX = gPlayerStickX[playerIndex];
        if (mappedStickX >= STICK_DIRECTION_PRESS_THRESHOLD) {
            gPlayerInputHeld[playerIndex] |= STICK_RIGHT;
        }
        if (mappedStickX <= -STICK_DIRECTION_PRESS_THRESHOLD) {
            gPlayerInputHeld[playerIndex] |= STICK_LEFT;
        }
        mappedStickY = gPlayerStickY[playerIndex];
        if (mappedStickY >= STICK_DIRECTION_PRESS_THRESHOLD) {
            gPlayerInputHeld[playerIndex] |= STICK_UP;
        }
        if (mappedStickY <= -STICK_DIRECTION_PRESS_THRESHOLD) {
            gPlayerInputHeld[playerIndex] |= STICK_DOWN;
        }
        if (mappedStickX < STICK_DIRECTION_RELEASE_THRESHOLD) {
            gPlayerInputHeld[playerIndex] &= ~STICK_RIGHT;
        }
        if (mappedStickX > -STICK_DIRECTION_RELEASE_THRESHOLD) {
            gPlayerInputHeld[playerIndex] &= ~STICK_LEFT;
        }
        if (mappedStickY < STICK_DIRECTION_RELEASE_THRESHOLD) {
            gPlayerInputHeld[playerIndex] &= ~STICK_UP;
        }
        if (mappedStickY > -STICK_DIRECTION_RELEASE_THRESHOLD) {
            gPlayerInputHeld[playerIndex] &= ~STICK_DOWN;
        }
        heldInput = gPlayerInputHeld[playerIndex];
        gPlayerInputPressed[playerIndex] = ~*previousInput & heldInput;
        if (heldInput == 0) {
            gPlayerInputRepeatTimer[playerIndex] = 0;
            gPlayerInputRepeat[playerIndex] = heldInput;
        } else {
            if (gPlayerInputRepeatTimer[playerIndex] >= PLAYER_INPUT_REPEAT_DELAY) {
                gPlayerInputRepeat[playerIndex] = heldInput;
            } else {
                gPlayerInputRepeatTimer[playerIndex]++;
                gPlayerInputRepeat[playerIndex] = gPlayerInputPressed[playerIndex];
            }
        }
    }

    for (gCurrentGameTask = gActiveGameTaskList.next; gCurrentGameTask != NULL;
         gCurrentGameTask = gCurrentGameTask->next) {
        if (gCurrentGameTask->state == GAME_TASK_STATE_PENDING) {
            gCurrentGameTask->state = GAME_TASK_STATE_ACTIVE;
        }
    }

    for (gCurrentGameTask = gActiveGameTaskList.next; gCurrentGameTask != NULL;
         gCurrentGameTask = gCurrentGameTask->next) {
        if (gCurrentGameTask->state == GAME_TASK_STATE_ACTIVE) {
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

    if (gFramebufferSubmissionCountdown[0] == 0) {
        if (gFramebufferSwapHold == 0) {
            frameIndex = gNextFramebufferRenderTaskIndex;
            if (gFrameRenderTasks[frameIndex].status == 0) {
                if ((s32)gPendingFramebufferSwapCount > 0) {
                    submitFramebufferRenderTask(frameIndex);
                    gFramebufferSubmissionCountdown[0] = gFramebufferRenderInterval.timerValue;
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
    gFramebufferSubmissionCountdown[0]--;

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
        task->state = GAME_TASK_STATE_PENDING;
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
            task->state = GAME_TASK_STATE_SUSPENDED;
            return;
        }
        task = task->next;
    }
}

void resumeGameTask(s32 taskId) {
    GameTask *task = gActiveGameTaskList.next;

    while (task != NULL) {
        if (taskId == task->id) {
            task->state = GAME_TASK_STATE_PENDING;
            return;
        }
        task = task->next;
    }
}
