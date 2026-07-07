#include "common.h"

typedef void (*InputTaskCallback)(void);

#define INPUT_TASK_CALLBACK_COUNT 3

typedef struct InputTask {
    struct InputTask *prev;
    struct InputTask *next;
    InputTaskCallback callbacks[INPUT_TASK_CALLBACK_COUNT];
    u8 priority;
    u8 id;
    u16 state;
    u8 pad18[0x10];
} InputTask;

typedef struct FramebufferState {
    u8 status;
    u8 pad[0x1861F];
} FramebufferState;

extern u8 D_800DEED0;
extern u8 D_800DEED4;
extern InputTask *D_801235B8;
extern u8 D_80123700;
extern InputTask *D_8012370C;
extern InputTask *D_80123730[];
extern u8 D_80123750;
extern u8 D_80123751;
extern u8 D_80123752;
extern FramebufferState D_8012496E[];

void func_8009B704(u8);
InputTask *func_80099384(s32);

#pragma GLOBAL_ASM("asm/nonmatchings/input_task_scheduler/func_80098D80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input_task_scheduler/func_80098EAC.s")

s32 func_80099288(void) {
    u8 frameIndex;

    if (D_800DEED0 == 0) {
        if (D_80123751 == 0) {
            frameIndex = D_80123752;
            if (D_8012496E[frameIndex].status == 0) {
                if ((s32) D_80123750 > 0) {
                    func_8009B704(frameIndex);
                    D_800DEED0 = D_800DEED4;
                    D_80123750--;
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
    D_800DEED0--;

return_one:
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/input_task_scheduler/func_80099384.s")

void func_80099464(s32 taskId) {
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

void func_800994F4(s32 taskId, InputTaskCallback callback, s32 priority) {
    InputTask *task;

    task = func_80099384(priority);
    if (task != NULL) {
        task->id = (u8) taskId;
        task->callbacks[0] = callback;
        task->priority = (u8) priority;
        task->state = 2;
    }
}

void func_8009954C(s32 taskId) {
    func_80099464(taskId);
}

void func_8009956C(InputTaskCallback callback, s32 callbackIndex) {
    switch (callbackIndex) {
        case 0:
            D_801235B8->callbacks[0] = callback;
            return;
        case 1:
            D_801235B8->callbacks[1] = callback;
            return;
        case 2:
            D_801235B8->callbacks[2] = callback;
            return;
    }
}

void func_800995C0(s32 callbackIndex) {
    switch (callbackIndex) {
        case 0:
            D_801235B8->callbacks[0] = NULL;
            return;
        case 1:
            D_801235B8->callbacks[1] = NULL;
            return;
        case 2:
            D_801235B8->callbacks[2] = NULL;
            return;
    }
}

void func_80099614(s32 taskId) {
    InputTask *task = D_8012370C;

    while (task != NULL) {
        if (taskId == task->id) {
            task->state = 1;
            return;
        }
        task = task->next;
    }
}

void func_80099658(s32 taskId) {
    InputTask *task = D_8012370C;

    while (task != NULL) {
        if (taskId == task->id) {
            task->state = 2;
            return;
        }
        task = task->next;
    }
}
