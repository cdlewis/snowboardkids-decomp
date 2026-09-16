#ifndef CALLBACK_TASK_SCHEDULER_H
#define CALLBACK_TASK_SCHEDULER_H

#include "common.h"
#include "compiler_diagnostics.h"

typedef struct CallbackTask CallbackTask;

typedef void (*CallbackTaskCallback)(void *);

typedef struct CallbackTaskHeader {
    /* 0x00 */ CallbackTask *prev;
    /* 0x04 */ CallbackTask *next;
    /* 0x08 */ CallbackTaskCallback callback;
    /* 0x0C */ u16 type;
    /* 0x0E */ u16 priority;
    /* 0x10 */ u16 userId;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 callbackTimer;
    /* 0x16 */ s16 isActive;
} CallbackTaskHeader;

/*
 * The scheduler initializes 216 contiguous slots with a MIPS stride of 0x118.
 * removeCallbackTask returns a slot to gFreeCallbackTaskPool; the next successful
 * createCallbackTaskPreservingArgs pops the most recently freed slot. Storage
 * can therefore be reused immediately, including by an actor for another player.
 * A task address and userId do not identify an allocation lifetime. Consumers
 * retaining render history must track allocation lifetimes separately.
 */
struct CallbackTask {
    /* 0x000 */ CallbackTaskHeader header;
    /* 0x018 */ u32 callbackData[0x40];
};

typedef struct CallbackTaskGroup {
    CallbackTask tasks[4];
} CallbackTaskGroup;

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
CLANG_DIAGNOSTIC_PUSH
CLANG_DIAGNOSTIC_IGNORE_STRICT_PROTOTYPES
void *createCallbackTaskWithUserIdPreservingArgs(void (*callback)(), s32 type, s32 priority, s32 userId);
CLANG_DIAGNOSTIC_POP
void removeCallbackTask(void *task);
void setCallbackTaskCallback(void *task, CallbackTaskCallback callback);

extern void *gActiveMenuTask;

#endif
