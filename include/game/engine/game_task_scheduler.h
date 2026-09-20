#ifndef GAME_TASK_SCHEDULER_H
#define GAME_TASK_SCHEDULER_H

#include "common.h"
#include "game/engine/controller_input.h"

#define GAME_TASK_CALLBACK_COUNT 3

#define GAME_TASK_STATE_ACTIVE 0
#define GAME_TASK_STATE_SUSPENDED 1
#define GAME_TASK_STATE_PENDING 2

typedef void (*GameTaskCallback)(void);

typedef struct GameTask {
    /* 0x00 */ struct GameTask *prev;
    /* 0x04 */ struct GameTask *next;
    /* 0x08 */ GameTaskCallback callbacks[GAME_TASK_CALLBACK_COUNT];
    /* 0x14 */ u8 priority;
    /* 0x15 */ u8 id;
    /* 0x16 */ u16 state;
    /* 0x18 */ s32 callbackData0;
    /* 0x1C */ s32 callbackData1;
    /* 0x20 */ s32 callbackData2;
    /* 0x24 */ s32 callbackData3;
} GameTask;

typedef union FramebufferRenderInterval {
    s8 value;
    u8 timerValue;
    u8 raw[4];
} FramebufferRenderInterval;

/* Game-update counter, incremented modulo 0x1000. */
extern s16 gFrameCounter;
extern GameTask *gCurrentGameTask;
extern FramebufferRenderInterval gFramebufferRenderInterval;
/* Only byte zero is a semantic countdown; the remaining bytes preserve storage.
 * Controls framebuffer task submission frequency, not VI swap delay. */
extern u8 gFramebufferSubmissionCountdown[4];
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern u8 gNextFramebufferRenderTaskIndex;

void initGameTaskScheduler(void);
/* Runs active game callbacks before updateFramebufferRenderScheduler. That
 * scheduler can omit framebuffer submission, so callback timers count game
 * updates rather than submitted render frames. A timer alone cannot identify
 * an actor spawn or prove continuity with a previously rendered lifetime. */
void updateGameTaskScheduler(void);
void createGameTask(s32 taskId, GameTaskCallback callback, s32 priority);
void removeGameTask(s32 taskId);
void setCurrentGameTaskCallback(GameTaskCallback callback, s32 callbackIndex);
void suspendGameTask(s32 taskId);
void resumeGameTask(s32 taskId);

#endif
