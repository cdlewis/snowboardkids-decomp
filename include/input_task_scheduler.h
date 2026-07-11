#ifndef INPUT_TASK_SCHEDULER_H
#define INPUT_TASK_SCHEDULER_H

#include "common.h"

typedef void (*InputTaskCallback)(void);

void initInputTaskScheduler(void);
void createInputTask(s32 taskId, InputTaskCallback callback, s32 priority);
void removeInputTask(s32 taskId);
void setCurrentInputTaskCallback(InputTaskCallback callback, s32 callbackIndex);
void suspendInputTask(s32 taskId);
void resumeInputTask(s32 taskId);

#endif
