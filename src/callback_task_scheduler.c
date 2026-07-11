#include "common.h"
#include "callback_task_scheduler.h"

extern CallbackTask *gCurrentCallbackTask;
extern CallbackTask *gFreeCallbackTaskList[];
extern u16 gFreeCallbackTaskCount;
extern u16 gCallbackTaskType0FreeCount;
extern u16 gCallbackTaskType6FreeCount;
extern u16 gCallbackTaskType5FreeCount;
extern u16 gCallbackTaskType1FreeCount;
extern u16 gCallbackTaskType2FreeCount;
extern u16 gCallbackTaskType3FreeCount;
extern u16 gCallbackTaskType4FreeCount;
extern CallbackTask gCallbackTaskListSentinel;
extern CallbackTask *gCallbackTaskListHead;

#pragma GLOBAL_ASM("asm/nonmatchings/callback_task_scheduler/initCallbackTaskScheduler.s")

// Mirrors the linked list head gCallbackTaskListHead into the global cursor gCurrentCallbackTask,
// then for each node clears callbackTimer and invokes its callback with a
// pointer to itself, advancing via next. Assigning through the captured store
// (`s0 = (gCurrentCallbackTask = gCallbackTaskListHead)`) and clearing callbackTimer via a temp
// steers IDO's register allocator to match the target's temp-register choice.
void updateCallbackTasks(void) {
    CallbackTask *s0;
    s32 new_var;

    s0 = (gCurrentCallbackTask = gCallbackTaskListHead);
    if (gCurrentCallbackTask != NULL) {
        do {
            new_var = 0;
            gCurrentCallbackTask->callbackTimer = new_var;
            gCurrentCallbackTask->callback(gCurrentCallbackTask);
            s0 = gCurrentCallbackTask->next;
            gCurrentCallbackTask = s0;
        } while (gCurrentCallbackTask != NULL);
    }
}

// Like updateCallbackTasks, mirrors the linked list head gCallbackTaskListHead into the global
// cursor gCurrentCallbackTask, then iterates: clearing each node's callbackTimer and
// invoking its callback, advancing via next. Stops early when a node's priority
// is less than minPriority. The compound assignment `s0 = (gCurrentCallbackTask = gCallbackTaskListHead)`
// steers IDO's register allocator to use a separate temp (t6) for the high-half
// load, matching the target.
void updateCallbackTasksWithMinPriority(s32 minPriority) {
    CallbackTask *s0 = (gCurrentCallbackTask = gCallbackTaskListHead);

    if (s0 != NULL) {
        do {
            s0->callbackTimer = 0;
            s0 = gCurrentCallbackTask;
            if ((u16)s0->priority < minPriority) {
                break;
            }
            s0->callback(s0);
            s0 = gCurrentCallbackTask->next;
            gCurrentCallbackTask = s0;
        } while (s0 != NULL);
    }
}

// Drains the global cursor list (gCurrentCallbackTask): for each node, clears
// callbackTimer and invokes its callback with a pointer to itself, then advances
// via next.
// The empty `if ((s0 && s0) && s0) {}` is a dead condition that IDO 5.3 folds to
// zero instructions, but which steers the allocator into keeping the cursor in
// $s0 and emitting `move $a0, $s0` for the callback argument (matching the
// target). Without it IDO folds the reload straight into $a0.
void updateRemainingCallbackTasks(void) {
    CallbackTask *s0 = gCurrentCallbackTask;

    if (s0 != NULL) {
        do {
            s0->callbackTimer = 0;
            s0 = gCurrentCallbackTask;
            if ((s0 && s0) && s0) {}
            s0->callback(s0);
            s0 = gCurrentCallbackTask->next;
            gCurrentCallbackTask = s0;
        } while (s0 != NULL);
    }
}

void noopCallbackTaskScheduler(void) {
}

// createCallbackTaskPreservingArgs best match: 92.071%
#pragma GLOBAL_ASM("asm/nonmatchings/callback_task_scheduler/createCallbackTaskPreservingArgs.s")

#ifdef NON_MATCHING
void *createCallbackTaskPreservingArgs(void (*callback)(), s32 type, s32 priority) {
    CallbackTask *task;
    CallbackTask *prev;
    CallbackTask *next;
    s32 index;

    type &= 0xFFFF;
    switch (type & 0xFF) {
    case 0:
        if (gCallbackTaskType0FreeCount == 0) {
            return NULL;
        }
        gCallbackTaskType0FreeCount--;
        break;
    case 1:
        if (gCallbackTaskType1FreeCount == 0) {
            return NULL;
        }
        gCallbackTaskType1FreeCount--;
        break;
    case 2:
        if (gCallbackTaskType2FreeCount == 0) {
            return NULL;
        }
        gCallbackTaskType2FreeCount--;
        break;
    case 3:
        if (gCallbackTaskType3FreeCount == 0) {
            return NULL;
        }
        gCallbackTaskType3FreeCount--;
        break;
    case 4:
        if (gCallbackTaskType4FreeCount == 0) {
            return NULL;
        }
        gCallbackTaskType4FreeCount--;
        break;
    case 5:
        if (gCallbackTaskType5FreeCount == 0) {
            return NULL;
        }
        gCallbackTaskType5FreeCount--;
        break;
    case 6:
        if (gCallbackTaskType6FreeCount == 0) {
            return NULL;
        }
        gCallbackTaskType6FreeCount--;
        break;
    default:
        return NULL;
    }

    if (gFreeCallbackTaskCount == 0) {
        return NULL;
    }
    index = gFreeCallbackTaskCount - 1;
    gFreeCallbackTaskCount = index;

    task = gFreeCallbackTaskList[index];
    prev = &gCallbackTaskListSentinel;
    if (gCallbackTaskListSentinel.next != NULL) {
        next = gCallbackTaskListSentinel.next;
        do {
            if ((u16)next->priority < priority) {
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
    task->isActive = 1;
    task->callback = (void (*)(CallbackTask *))callback;
    task->type = type;
    task->priority = priority;
    return task;
}
#endif

// createCallbackTask best match: 96.233%
#pragma GLOBAL_ASM("asm/nonmatchings/callback_task_scheduler/createCallbackTask.s")

#ifdef NON_MATCHING
void *createCallbackTask(void (*callback)(), u16 type, s32 priority) {
    CallbackTask *task;
    CallbackTask *prev;
    CallbackTask *next;
    CallbackTask *sentinel;
    s32 index;
    u32 i;

    switch (type & 0xFF) {
    case 0:
        if (gCallbackTaskType0FreeCount == 0) {
            return NULL;
        }
        gCallbackTaskType0FreeCount--;
        break;
    case 1:
        if (gCallbackTaskType1FreeCount == 0) {
            return NULL;
        }
        gCallbackTaskType1FreeCount--;
        break;
    case 2:
        if (gCallbackTaskType2FreeCount == 0) {
            return NULL;
        }
        gCallbackTaskType2FreeCount--;
        break;
    case 3:
        if (gCallbackTaskType3FreeCount == 0) {
            return NULL;
        }
        gCallbackTaskType3FreeCount--;
        break;
    case 4:
        if (gCallbackTaskType4FreeCount == 0) {
            return NULL;
        }
        gCallbackTaskType4FreeCount--;
        break;
    case 5:
        if (gCallbackTaskType5FreeCount == 0) {
            return NULL;
        }
        gCallbackTaskType5FreeCount--;
        break;
    case 6:
        if (gCallbackTaskType6FreeCount == 0) {
            return NULL;
        }
        gCallbackTaskType6FreeCount--;
        break;
    default:
        return NULL;
    }

    if ((gFreeCallbackTaskCount & 0xFFFFu) == 0) {
        return NULL;
    }
    index = gFreeCallbackTaskCount - 1;
    sentinel = &gCallbackTaskListSentinel;
    prev = sentinel;
    gFreeCallbackTaskCount = index;
    task = gFreeCallbackTaskList[index & 0xFFFF];
    if (prev->next != NULL) {
        next = sentinel->next;
        do {
            if ((u16) next->priority < priority) {
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
    task->callback = (void (*)(CallbackTask *))callback;
    task->type = type;
    task->priority = priority;
    task->isActive = 1;

    i = 0;
    do {
        task->args[i] = NULL;
        i = (i + 1) & 0xFFFF;
    } while (i < 0x40U);

    return task;
}
#endif

void *createCallbackTaskWithUserId(void (*callback)(), s32 type, s32 priority, s32 userId){ CallbackTask *t=createCallbackTask(callback,type&0xFFFF,priority); if(t!=NULL){t->userId=userId;} return t;}

void *createCallbackTaskWithUserIdPreservingArgs(void (*callback)(), s32 type, s32 priority, s32 userId){ CallbackTask *t=createCallbackTaskPreservingArgs(callback,type&0xFFFF,priority); if(t!=NULL){t->userId=userId;} return t;}

void removeCallbackTask(void *taskPtr) {
    CallbackTask *task = taskPtr;
    CallbackTask *next;
    u16 *counter;
    unsigned char type;

    task->isActive = 0;
    task->prev->next = task->next;
    next = task->next;
    if (next != NULL) {
        next->prev = task->prev;
    }

    gFreeCallbackTaskList[gFreeCallbackTaskCount] = task;
    gFreeCallbackTaskCount++;

    type = task->type & 0xFF;
    switch (type) {
    case 0:
        counter = &gCallbackTaskType0FreeCount;
        (*counter)++;
        return;
    case 1:
        counter = &gCallbackTaskType1FreeCount;
        (*counter)++;
        return;
    case 2:
        counter = &gCallbackTaskType2FreeCount;
        (*counter)++;
        return;
    case 3:
        counter = &gCallbackTaskType3FreeCount;
        (*counter)++;
        return;
    case 4:
        counter = &gCallbackTaskType4FreeCount;
        (*counter)++;
        return;
    case 5:
        counter = &gCallbackTaskType5FreeCount;
        (*counter)++;
        return;
    case 6:
        counter = &gCallbackTaskType6FreeCount;
        (*counter)++;
        return;
    }
}

void setCallbackTaskCallback(void *task, void (*callback)()) {
    ((CallbackTask *)task)->callback = (void (*)(CallbackTask *))callback;
}
