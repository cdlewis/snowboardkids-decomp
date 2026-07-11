#ifndef GAME_TASK_SCHEDULER_H
#define GAME_TASK_SCHEDULER_H

#include "common.h"

typedef void (*GameTaskCallback)(void);

void initGameTaskScheduler(void);
void createGameTask(s32 taskId, GameTaskCallback callback, s32 priority);
void removeGameTask(s32 taskId);
void setCurrentGameTaskCallback(GameTaskCallback callback, s32 callbackIndex);
void suspendGameTask(s32 taskId);
void resumeGameTask(s32 taskId);

#endif
