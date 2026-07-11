#include "common.h"
#include "input_task_scheduler.h"

#define INPUT_TASK_CALLBACK_COUNT 3
#define INPUT_TASK_COUNT 8

typedef struct InputTask {
    struct InputTask *prev;
    struct InputTask *next;
    InputTaskCallback callbacks[INPUT_TASK_CALLBACK_COUNT];
    u8 priority;
    u8 id;
    u16 state;
    u8 pad18[0x10];
} InputTask;

typedef struct InputTaskScheduler {
    u8 pad0[4];
    InputTask *activeTask;
    u8 pad8[0xC];
    u8 unk14;
} InputTaskScheduler;

typedef struct FramebufferState {
    u8 status;
    u8 pad[0x1861F];
} FramebufferState;

typedef struct ControllerInputState {
    u16 buttons;
    s8 stickX;
    s8 stickY;
    u8 pad4[2];
} ControllerInputState;

extern s8 D_800DEED8;
extern u8 gFramebufferSwapDelayTimer;
extern u8 gFramebufferSwapDelay;
extern ControllerInputState D_800E4C18;
extern s16 gFrameCounter;
extern InputTask *gCurrentInputTask;
extern InputTask D_801235C0[INPUT_TASK_COUNT];
extern u8 D_80123700;
extern InputTaskScheduler D_80123708;
extern InputTask *D_8012370C;
extern InputTask *D_80123730[];
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern u8 D_80123752;
extern s32 gPlayerInputHeld;
extern s32 D_8012375C;
extern s32 D_80123760;
extern s32 D_80123764;
extern s32 D_80123768;
extern s32 D_8012376C;
extern s32 D_80123770;
extern s32 D_80123774;
extern s32 gPlayerInputPressed;
extern s32 D_8012377C;
extern s32 D_80123780;
extern s32 D_80123784;
extern s8 D_80123788;
extern s8 D_80123789;
extern s8 D_8012378A;
extern s8 D_8012378B;
extern s8 D_8012378C;
extern s8 D_8012378D;
extern s8 D_8012378E;
extern s8 D_8012378F;
extern s32 D_80123790;
extern u8 D_801237A0;
extern FramebufferState D_8012496E[];

#ifdef NON_MATCHING
void func_8004835C();
#else
void func_8004835C(void *, void *);
#endif
void func_8009B0E8(void);
void func_8009B704(u8);
void clearPendingPositionalSoundRequests(void);
InputTask *func_80099384(s32);
s32 func_80099288(void);
void func_80072B24(void);

void initInputTaskScheduler(void) {
    InputTask **freeTask;
    InputTask *task;
    s32 zero;

    D_80123708.activeTask = NULL;
    D_80123708.unk14 = 0;
    freeTask = D_80123730; task = D_801235C0; do { *freeTask = task; task++; freeTask++; } while (task < &D_801235C0[INPUT_TASK_COUNT]);
    D_80123700 = 0;
    gFrameCounter = 0;
    gPendingFramebufferSwapCount = 2;
    gFramebufferSwapHold = 0;
    zero = 0;
    D_80123752 = zero;
    gPlayerInputHeld = zero;
    D_80123768 = zero;
    gPlayerInputPressed = 0;
    D_80123788 = zero;
    D_8012378C = 0;
    D_8012375C = 0;
    D_8012376C = zero;
    D_8012377C = zero;
    D_80123789 = 0;
    D_8012378D = zero;
    D_80123760 = zero;
    D_80123770 = 0;
    D_80123780 = zero;
    D_8012378A = zero;
    D_8012378E = 0;
    D_80123764 = zero;
    D_80123774 = 0;
    D_80123784 = 0;
    D_8012378B = zero;
    D_8012378F = zero;
    func_8004835C(&D_801235C0[INPUT_TASK_COUNT], &D_80123708);
    func_8009B0E8();
}

// func_80098EAC best match: 92.403% (nonmatchings/func_80098EAC-7273315160691878794/base_11.c)
#pragma GLOBAL_ASM("asm/nonmatchings/input_task_scheduler/func_80098EAC.s")

#ifdef NON_MATCHING
void func_80098EAC(void) {
    s32 *previousInput;
    s32 *input;
    ControllerInputState *controller;
    s8 *stickXOut;
    s8 *stickYOut;
    s32 *newInput;
    u8 *repeatTimer;
    s32 *repeatInput;
    InputTask *task;
    InputTask *nextTask;
    InputTaskCallback callback;
    s32 oldInput;
    s32 currentInput;
    s32 stickXTooHigh;
    s8 stickX;
    s8 stickY;
    s32 timer;

    gFrameCounter = (gFrameCounter + 1) & 0xFFF;
    func_8004835C();
    func_8009B0E8();
    clearPendingPositionalSoundRequests();

    previousInput = &D_80123768;
    input = &gPlayerInputHeld;
    controller = &D_800E4C18;
    stickXOut = &D_80123788;
    stickYOut = &D_8012378C;
    newInput = &gPlayerInputPressed;
    repeatTimer = &D_801237A0;
    repeatInput = &D_80123790;

    do {
        stickXTooHigh = controller->stickX >= 0x2E;
        oldInput = *input;
        currentInput = oldInput & 0xFFFF0000;
        *input = currentInput;
        *input = currentInput | controller->buttons;
        *previousInput = oldInput;

        if (stickXTooHigh) {
            controller->stickX = 0x2D;
        }
        if (controller->stickX < -0x2D) {
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
        controller++;
        if (stickX >= 0) {
            *stickXOut = *(&D_800DEED8 + stickX);
        } else {
            *stickXOut = -*(&D_800DEED8 - stickX);
        }

        if (stickY >= 0) {
            *stickYOut = *(&D_800DEED8 + stickY);
        } else {
            *stickYOut = -*(&D_800DEED8 - stickY);
        }

        stickX = *stickXOut;
        stickXOut++;
        if (stickX >= 0x1B) {
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
                *repeatTimer = timer + 1;
                *repeatInput = *newInput;
            }
        }

        repeatInput++;
        previousInput++;
        input++;
        newInput++;
        repeatTimer++;
    } while (repeatInput != (s32 *)&D_801237A0);

    task = D_8012370C;
    gCurrentInputTask = task;
    if (task != NULL) {
        do {
            if (task->state == 2) {
                task->state = 0;
                task = gCurrentInputTask;
            }
            nextTask = task->next;
            gCurrentInputTask = nextTask;
            task = nextTask;
        } while (nextTask != NULL);
        gCurrentInputTask = D_8012370C;
    }

    task = gCurrentInputTask;
    if (task != NULL) {
        do {
            if (task->state == 0) {
                callback = task->callbacks[0];
                if (callback != NULL) {
                    callback();
                    task = gCurrentInputTask;
                }
                callback = task->callbacks[1];
                if (callback != NULL) {
                    callback();
                    task = gCurrentInputTask;
                }
                callback = task->callbacks[2];
                if (callback != NULL) {
                    callback();
                    task = gCurrentInputTask;
                }
            }
            nextTask = task->next;
            gCurrentInputTask = nextTask;
            task = nextTask;
        } while (nextTask != NULL);
    }

    func_80099288();
    func_80072B24();
}
#endif

s32 func_80099288(void) {
    u8 frameIndex;

    if (gFramebufferSwapDelayTimer == 0) {
        if (gFramebufferSwapHold == 0) {
            frameIndex = D_80123752;
            if (D_8012496E[frameIndex].status == 0) {
                if ((s32) gPendingFramebufferSwapCount > 0) {
                    func_8009B704(frameIndex);
                    gFramebufferSwapDelayTimer = gFramebufferSwapDelay;
                    gPendingFramebufferSwapCount--;
                    if (D_80123752 != 0) {
                        D_80123752 = 0;
                    } else {
                        D_80123752 = 1;
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

// func_80099384 best match: 95.727% (nonmatchings/func_80099384-180949888360117632/base_16.c)
#pragma GLOBAL_ASM("asm/nonmatchings/input_task_scheduler/func_80099384.s")

#ifdef NON_MATCHING
InputTask *func_80099384(s32 priority) {
    InputTask *task;
    InputTask *next;
    InputTask *prev;
    InputTaskScheduler *scheduler;
    u8 *clear;
    s32 i;

    if (D_80123700 >= INPUT_TASK_COUNT) {
        return NULL;
    }

    task = D_80123730[D_80123700];
    i = 0;
    clear = (u8 *)task;
    do {
        clear[i] = 0;
        i++;
    } while (i != sizeof(InputTask));

    D_80123700++;
    prev = (InputTask *)&D_80123708;
    scheduler = &D_80123708;
    if (D_80123708.activeTask != NULL) {
        next = scheduler->activeTask;
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
#endif

void unlinkInputTask(s32 taskId) {
    InputTask *task;
    InputTask *next;
    s32 freeTaskCount;

    task = D_8012370C;
    while (task != NULL) {
        if (taskId == task->id) {
            task->prev->next = task->next;
            next = task->next;
            if (next != NULL) {
                next->prev = task->prev;
            }
            freeTaskCount = (D_80123700 & 0xFFu) - 1;
            D_80123700 = freeTaskCount;
            D_80123730[(u8) (((((((((((freeTaskCount & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu)] = task;
        }
        task = task->next;
    }
}

void createInputTask(s32 taskId, InputTaskCallback callback, s32 priority) {
    InputTask *task;

    task = func_80099384(priority);
    if (task != NULL) {
        task->id = (u8) taskId;
        task->callbacks[0] = callback;
        task->priority = (u8) priority;
        task->state = 2;
    }
}

void removeInputTask(s32 taskId) {
    unlinkInputTask(taskId);
}

void setCurrentInputTaskCallback(InputTaskCallback callback, s32 callbackIndex) {
    switch (callbackIndex) {
        case 0:
            gCurrentInputTask->callbacks[0] = callback;
            return;
        case 1:
            gCurrentInputTask->callbacks[1] = callback;
            return;
        case 2:
            gCurrentInputTask->callbacks[2] = callback;
            return;
    }
}

void clearCurrentInputTaskCallback(s32 callbackIndex) {
    switch (callbackIndex) {
        case 0:
            gCurrentInputTask->callbacks[0] = NULL;
            return;
        case 1:
            gCurrentInputTask->callbacks[1] = NULL;
            return;
        case 2:
            gCurrentInputTask->callbacks[2] = NULL;
            return;
    }
}

void suspendInputTask(s32 taskId) {
    InputTask *task = D_8012370C;

    while (task != NULL) {
        if (taskId == task->id) {
            task->state = 1;
            return;
        }
        task = task->next;
    }
}

void resumeInputTask(s32 taskId) {
    InputTask *task = D_8012370C;

    while (task != NULL) {
        if (taskId == task->id) {
            task->state = 2;
            return;
        }
        task = task->next;
    }
}
