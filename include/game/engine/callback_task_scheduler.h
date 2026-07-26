#ifndef CALLBACK_TASK_SCHEDULER_H
#define CALLBACK_TASK_SCHEDULER_H

#include "common.h"

typedef struct CallbackTask {
    struct CallbackTask *prev;
    struct CallbackTask *next;
    void (*callback)(void *);
    u16 type;
    u16 priority;
    s16 userId;
    s16 unk12;
    s16 callbackTimer;
    s16 isActive;
    void *args[0x40];
} CallbackTask; // size = 0x118

typedef void (*CallbackTaskCallback)(void *);

void initCallbackTaskScheduler(s32 arg0);
void updateCallbackTasks(void);
void updateCallbackTasksWithMinPriority(s32 minPriority);
void updateRemainingCallbackTasks(void);
void noopCallbackTask(void);
#ifdef CALLBACK_TASK_SCHEDULER_IMPLEMENTATION
void *createCallbackTaskPreservingArgs(CallbackTaskCallback callback, u16 type, s32 priority);
void *createCallbackTask(CallbackTaskCallback callback, u16 type, s32 priority);
#else
void *createCallbackTaskPreservingArgs(CallbackTaskCallback callback, s32 type, s32 priority);
void *createCallbackTask(CallbackTaskCallback callback, s32 type, s32 priority);
#endif
void *createCallbackTaskWithUserId(CallbackTaskCallback callback, s32 type, s32 priority, s32 userId);
/*
 * createCallbackTaskWithUserIdPreservingArgs is intentionally not declared here: its callers disagree on the
 * arity (race_item_effects.c passes 4 args, race_ui_effects.c passes 3), and
 * both call shapes are baked into matched ROM code. A single shared prototype
 * cannot satisfy both, so each caller keeps its own local declaration.
 */
void removeCallbackTask(void *task);
void setCallbackTaskCallback(void *task, CallbackTaskCallback callback);

extern void *gActiveMenuTask;
extern void *D_8010ADE8;

#endif
