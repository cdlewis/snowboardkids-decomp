#ifndef INPUT_TASK_SCHEDULER_H
#define INPUT_TASK_SCHEDULER_H

#include "common.h"

typedef void (*InputTaskCallback)(void);

void func_80098D80(void);
void func_800994F4(s32 taskId, InputTaskCallback callback, s32 priority);
void func_8009954C(s32 taskId);
void func_8009956C(InputTaskCallback callback, s32 callbackIndex);
void func_80099614(s32 taskId);
void func_80099658(s32 taskId);

#endif
