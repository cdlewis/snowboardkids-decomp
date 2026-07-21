#ifndef CALLBACK_TASK_SCHEDULER_H
#define CALLBACK_TASK_SCHEDULER_H

#include "common.h"

typedef struct CallbackTask {
    struct CallbackTask *prev;
    struct CallbackTask *next;
    void (*callback)(struct CallbackTask *);
    u16 type;
    u16 priority;
    s16 userId;
    s16 unk12;
    s16 callbackTimer;
    s16 isActive;
    void *args[0x40];
} CallbackTask; // size = 0x118

void initCallbackTaskScheduler(s32 arg0);
void updateCallbackTasks(void);
void updateCallbackTasksWithMinPriority(s32 minPriority);
void updateRemainingCallbackTasks(void);
void noopCallbackTask(void);
#ifdef CALLBACK_TASK_SCHEDULER_IMPLEMENTATION
void *createCallbackTaskPreservingArgs(void (*callback)(), u16 type, s32 priority);
#else
void *createCallbackTaskPreservingArgs(void (*callback)(), s32 type, s32 priority);
#endif
#ifdef CALLBACK_TASK_SCHEDULER_IMPLEMENTATION
void *createCallbackTask(void (*callback)(), u16 type, s32 priority);
#else
void *createCallbackTask(void (*callback)(), s32 type, s32 priority);
#endif
void *createCallbackTaskWithUserId(void (*callback)(), s32 type, s32 priority, s32 userId);
/*
 * createCallbackTaskWithUserIdPreservingArgs is intentionally not declared here: its callers disagree on the
 * arity (race_item_effects.c passes 4 args, race_ui_effects.c passes 3), and
 * both call shapes are baked into matched ROM code. A single shared prototype
 * cannot satisfy both, so each caller keeps its own local declaration.
 */
void removeCallbackTask(void *task);
void setCallbackTaskCallback(void *task, void (*callback)());

#endif
