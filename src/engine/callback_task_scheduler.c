#include "common.h"
#define CALLBACK_TASK_SCHEDULER_IMPLEMENTATION
#include "game/engine/callback_task_scheduler.h"
#undef CALLBACK_TASK_SCHEDULER_IMPLEMENTATION
#include "game/engine/system_runtime.h"

/*
 * This overlapping view is required to preserve IDO's register allocation in
 * the two task creation functions. D_80112770.activeListSentinel aliases
 * gCallbackTaskActiveListSentinel at offset 0x10.
 */
typedef struct CallbackTaskSchedulerBss {
    u8 padding[0x10];
    CallbackTask activeListSentinel;
} CallbackTaskSchedulerBss;

extern CallbackTask *gCurrentCallbackTask;
extern CallbackTask *gFreeCallbackTaskPool[];
extern u16 gFreeCallbackTaskCount;
extern u16 gFreeCallbackTaskType0Count;
extern u16 gFreeCallbackTaskType6Count;
extern u16 gFreeCallbackTaskType5Count;
extern u16 gFreeCallbackTaskType1Count;
extern u16 gFreeCallbackTaskType2Count;
extern u16 gFreeCallbackTaskType3Count;
extern u16 gFreeCallbackTaskType4Count;
extern CallbackTask gCallbackTaskActiveListSentinel;
extern CallbackTask *gCallbackTaskActiveListHead;
extern CallbackTaskSchedulerBss D_80112770;
extern CallbackTaskGroup D_80112898;
extern CallbackTaskGroup D_801129B0;
extern CallbackTaskGroup D_80112AC8;
extern CallbackTaskGroup D_80112BE0;
extern CallbackTaskGroup D_80121820;
void *createCallbackTaskPreservingArgsS32(CallbackTaskCallback callback, s32 type, s32 priority);
void *createCallbackTaskS32(CallbackTaskCallback callback, s32 type, s32 priority);
#pragma weak createCallbackTaskPreservingArgsS32 = createCallbackTaskPreservingArgs
#pragma weak createCallbackTaskS32 = createCallbackTask
// The empty condition steers IDO's instruction scheduler to advance task3
// immediately after storing it while leaving task0's advance after its store.
// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initCallbackTaskScheduler(s32 arg0) {
    CallbackTask **pool;
    CallbackTaskGroup *task0;
    CallbackTaskGroup *task1;
    CallbackTaskGroup *task2;
    CallbackTaskGroup *task3;
    CallbackTaskGroup *end;

    gCallbackTaskActiveListHead = NULL; resetRenderCallbackQueues(); pool = gFreeCallbackTaskPool; task0 = &D_80112898; task1 = &D_801129B0; task2 = &D_80112AC8; task3 = &D_80112BE0; end = &D_80121820; loop: pool[3] = &task3->tasks[0]; task3++; pool[1] = &task1->tasks[0]; pool[2] = &task2->tasks[0]; if (((!task0) && (!task0)) && (!task0)) { } task2++; task1++; pool[0] = &task0->tasks[0]; task0++;
    task0[-1].tasks[1].header.isActive = 0;
    task0[-1].tasks[1].header.callbackTimer = 0;
    task0[-1].tasks[2].header.isActive = 0;
    task0[-1].tasks[2].header.callbackTimer = 0;
    task0[-1].tasks[3].header.isActive = 0;
    task0[-1].tasks[3].header.callbackTimer = 0;
    pool += 4;
    task0[-1].tasks[0].header.isActive = 0;
    task0[-1].tasks[0].header.callbackTimer = 0;
    if (task3 != end) {
        goto loop;
    }

    gFreeCallbackTaskCount = 0xD8;
    switch (arg0) {
    case 0:
        gFreeCallbackTaskType1Count = 0;
        gFreeCallbackTaskType2Count = 0;
        gFreeCallbackTaskType3Count = 0;
        gFreeCallbackTaskType4Count = 0;
        gFreeCallbackTaskType5Count = 0;
        gFreeCallbackTaskType6Count = 0;
        gFreeCallbackTaskType0Count = 0xD8;
        break;
    case 1:
        gFreeCallbackTaskType1Count = 6;
        gFreeCallbackTaskType2Count = 6;
        gFreeCallbackTaskType3Count = 6;
        gFreeCallbackTaskType4Count = 6;
        gFreeCallbackTaskType0Count = 0x64;
        gFreeCallbackTaskType5Count = 0x4C;
        gFreeCallbackTaskType6Count = 0x10;
        break;
    case 2:
        gFreeCallbackTaskType1Count = 0x18;
        gFreeCallbackTaskType2Count = 0;
        gFreeCallbackTaskType3Count = 0;
        gFreeCallbackTaskType4Count = 0;
        gFreeCallbackTaskType0Count = 0x64;
        gFreeCallbackTaskType5Count = 0x4C;
        gFreeCallbackTaskType6Count = 0x10;
        break;
    }
}
// clang-format on

// Mirrors the linked list head gCallbackTaskActiveListHead into the global cursor gCurrentCallbackTask,
// then for each node clears callbackTimer and invokes its callback with a
// pointer to itself, advancing via next. Assigning through the captured store
// (`s0 = (gCurrentCallbackTask = gCallbackTaskActiveListHead)`) and clearing callbackTimer via a temp
// steers IDO's register allocator to match the target's temp-register choice.
void updateCallbackTasks(void) {
    CallbackTask *s0;
    s32 new_var;

    s0 = (gCurrentCallbackTask = gCallbackTaskActiveListHead);
    if (gCurrentCallbackTask != NULL) {
        do {
            new_var = 0;
            gCurrentCallbackTask->header.callbackTimer = new_var;
            gCurrentCallbackTask->header.callback(gCurrentCallbackTask);
            s0 = gCurrentCallbackTask->header.next;
            gCurrentCallbackTask = s0;
        } while (gCurrentCallbackTask != NULL);
    }
}

// Like updateCallbackTasks, mirrors the linked list head gCallbackTaskActiveListHead into the global
// cursor gCurrentCallbackTask, then iterates: clearing each node's callbackTimer and
// invoking its callback, advancing via next. Stops early when a node's priority
// is less than minPriority. The compound assignment `s0 = (gCurrentCallbackTask = gCallbackTaskActiveListHead)`
// steers IDO's register allocator to use a separate temp (t6) for the high-half
// load, matching the target.
void updateCallbackTasksWithMinPriority(s32 minPriority) {
    CallbackTask *s0 = (gCurrentCallbackTask = gCallbackTaskActiveListHead);

    if (s0 != NULL) {
        do {
            s0->header.callbackTimer = 0;
            s0 = gCurrentCallbackTask;
            if ((u16)s0->header.priority < minPriority) {
                break;
            }
            s0->header.callback(s0);
            s0 = gCurrentCallbackTask->header.next;
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
            s0->header.callbackTimer = 0;
            s0 = gCurrentCallbackTask;
            if ((s0 && s0) && s0) {}
            s0->header.callback(s0);
            s0 = gCurrentCallbackTask->header.next;
            gCurrentCallbackTask = s0;
        } while (s0 != NULL);
    }
}

void noopCallbackTask(void) {
}

void *createCallbackTaskPreservingArgs(CallbackTaskCallback callback, u16 type, s32 priority) {
    CallbackTask *task;
    CallbackTask *prev;
    CallbackTask *next;
    volatile CallbackTask *sentinel;
    s32 index;

    task = NULL;
    next = NULL;
    switch ((type & 0xFFFFu) & 0xFF) {
        case 0:
            if (gFreeCallbackTaskType0Count == 0) {
                return NULL;
            }
            gFreeCallbackTaskType0Count--;
            break;
        case 1:
            if (gFreeCallbackTaskType1Count == 0) {
                return NULL;
            }
            gFreeCallbackTaskType1Count--;
            break;
        case 2:
            if (gFreeCallbackTaskType2Count == 0) {
                return NULL;
            }
            gFreeCallbackTaskType2Count--;
            prev = next;
            break;
        case 3:
            if (gFreeCallbackTaskType3Count == 0) {
                return NULL;
            }
            gFreeCallbackTaskType3Count--;
            break;
        case 4:
            if (gFreeCallbackTaskType4Count == 0) {
                return NULL;
            }
            gFreeCallbackTaskType4Count--;
            break;
        case 5:
            if (gFreeCallbackTaskType5Count == 0) {
                return NULL;
            }
            gFreeCallbackTaskType5Count--;
            break;
        case 6:
            if (gFreeCallbackTaskType6Count == 0) {
                return NULL;
            }
            gFreeCallbackTaskType6Count--;
            break;
        default:
            return NULL;
    }

    index = gFreeCallbackTaskCount;
    if ((index & 0xFFFFu) == 0) {
        return NULL;
    }
    index = (index & 0xFFFF) - 1;
    prev = &gCallbackTaskActiveListSentinel;
    sentinel = &D_80112770.activeListSentinel;
    gFreeCallbackTaskCount = index;
    task = gFreeCallbackTaskPool
        [(((((((index & 0xFFFF) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu];
    if (prev->header.next != NULL) {
        next = sentinel->header.next;
        do {
            if ((u16)next->header.priority < priority) {
                break;
            }
            prev = next;
            next = next->header.next;
        } while (next != NULL);
    }

    task->header.prev = prev;
    task->header.next = prev->header.next;
    next = prev->header.next;
    if (next != NULL) {
        next->header.prev = task;
    }
    prev->header.next = task;
    task->header.isActive = 1;
    task->header.callback = callback;
    task->header.type = type;
    task->header.priority = priority;
    return task;
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void *createCallbackTask(CallbackTaskCallback callback, u16 type, s32 priority) {
    CallbackTask *task;
    CallbackTask *prev;
    CallbackTask *next;
    volatile CallbackTask *sentinel;
    s32 index;
    u32 i;
    u32 nextIndex;

    task = NULL;
    next = NULL;
    switch ((type & 0xFFFFu) & 0xFF) {
    case 0:
        if (gFreeCallbackTaskType0Count == 0) {
            return NULL;
        }
        gFreeCallbackTaskType0Count--;
        break;
    case 1:
        if (gFreeCallbackTaskType1Count == 0) {
            return NULL;
        }
        gFreeCallbackTaskType1Count--;
        break;
    case 2:
        if (gFreeCallbackTaskType2Count == 0) {
            return NULL;
        }
        gFreeCallbackTaskType2Count--;
        prev = next;
        break;
    case 3:
        if (gFreeCallbackTaskType3Count == 0) {
            return NULL;
        }
        gFreeCallbackTaskType3Count--;
        break;
    case 4:
        if (gFreeCallbackTaskType4Count == 0) {
            return NULL;
        }
        gFreeCallbackTaskType4Count--;
        break;
    case 5:
        if (gFreeCallbackTaskType5Count == 0) {
            return NULL;
        }
        gFreeCallbackTaskType5Count--;
        break;
    case 6:
        if (gFreeCallbackTaskType6Count == 0) {
            return NULL;
        }
        gFreeCallbackTaskType6Count--;
        break;
    default:
        return NULL;
    }

    index = gFreeCallbackTaskCount;
    if ((index & 0xFFFFu) == 0) {
        return NULL;
    }
    index = (index & 0xFFFF) - 1;
    prev = &gCallbackTaskActiveListSentinel;
    sentinel = &D_80112770.activeListSentinel;
    gFreeCallbackTaskCount = index;
    task = gFreeCallbackTaskPool[(((((((index & 0xFFFF) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu];
    if (prev->header.next != NULL) {
        next = sentinel->header.next;
        do {
            if ((u16)next->header.priority < priority) {
                break;
            }
            prev = next;
            next = next->header.next;
        } while (next != NULL);
    }

    task->header.prev = prev;
    task->header.next = prev->header.next;
    next = prev->header.next;
    if (next != NULL) {
        next->header.prev = task;
    }
    prev->header.next = task;
    task->header.callback = callback;
    task->header.type = type;
    task->header.priority = priority;
    task->header.isActive = 1;

    i = 0;
    do { task->callbackData[i] = 0; i = (i + 1) & 0xFFFF; } while (i < 0x40U);
    return task;
}
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void *createCallbackTaskWithUserId(CallbackTaskCallback callback, s32 type, s32 priority, s32 userId){ CallbackTask *t=createCallbackTaskS32(callback,type&0xFFFF,priority); if(t!=NULL){t->header.userId=userId;} return t;}
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void *createCallbackTaskWithUserIdPreservingArgs(CallbackTaskCallback callback, s32 type, s32 priority, s32 userId){ CallbackTask *t=createCallbackTaskPreservingArgsS32(callback,type&0xFFFF,priority); if(t!=NULL){t->header.userId=userId;} return t;}
// clang-format on

void removeCallbackTask(void *taskPtr) {
    CallbackTask *task = taskPtr;
    CallbackTask *next;
    u16 *counter;
    unsigned char type;

    task->header.isActive = 0;
    task->header.prev->header.next = task->header.next;
    next = task->header.next;
    if (next != NULL) {
        next->header.prev = task->header.prev;
    }

    gFreeCallbackTaskPool[gFreeCallbackTaskCount] = task;
    gFreeCallbackTaskCount++;

    type = task->header.type & 0xFF;
    switch (type) {
        case 0:
            counter = &gFreeCallbackTaskType0Count;
            (*counter)++;
            return;
        case 1:
            counter = &gFreeCallbackTaskType1Count;
            (*counter)++;
            return;
        case 2:
            counter = &gFreeCallbackTaskType2Count;
            (*counter)++;
            return;
        case 3:
            counter = &gFreeCallbackTaskType3Count;
            (*counter)++;
            return;
        case 4:
            counter = &gFreeCallbackTaskType4Count;
            (*counter)++;
            return;
        case 5:
            counter = &gFreeCallbackTaskType5Count;
            (*counter)++;
            return;
        case 6:
            counter = &gFreeCallbackTaskType6Count;
            (*counter)++;
            return;
    }
}

void setCallbackTaskCallback(void *taskPtr, CallbackTaskCallback callback) {
    CallbackTask *task = taskPtr;

    task->header.callback = callback;
}
