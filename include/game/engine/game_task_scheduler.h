#ifndef GAME_TASK_SCHEDULER_H
#define GAME_TASK_SCHEDULER_H

#include "common.h"
#include "game/engine/controller_input.h"

#define GAME_TASK_CALLBACK_COUNT 3

typedef void (*GameTaskCallback)(void);

typedef struct GameTask {
    /* 0x00 */ struct GameTask *prev;
    /* 0x04 */ struct GameTask *next;
    /* 0x08 */ GameTaskCallback callbacks[GAME_TASK_CALLBACK_COUNT];
    /* 0x14 */ u8 priority;
    /* 0x15 */ u8 id;
    /* 0x16 */ u16 state;
    union {
        /* 0x18 */ s32 fade;
        /* 0x18 */ s32 fadeDelay;
        /* 0x18 */ s32 fadeTimer;
        /* 0x18 */ s32 transitionTimer;
    };
    union {
        /* 0x1C */ s32 fadeStep;
        /* 0x1C */ s32 selection;
        /* 0x1C */ s32 shopItemPrice;
        /* 0x1C */ s32 timer;
        /* 0x1C */ s32 unk1C;
    };
    union {
        /* 0x20 */ s32 countdown;
        /* 0x20 */ s32 courseSegment;
        /* 0x20 */ s32 delay;
        /* 0x20 */ s32 screenState;
        /* 0x20 */ s32 unk20;
    };
    union {
        /* 0x24 */ s32 mainMenuTimer;
        /* 0x24 */ s32 startDelay;
    };
} GameTask;

typedef union FramebufferSwapDelay {
    s8 value;
    u8 timerValue;
} FramebufferSwapDelay;

extern GameTask *gCurrentGameTask;
extern FramebufferSwapDelay gFramebufferSwapDelay;

void initGameTaskScheduler(void);
void createGameTask(s32 taskId, GameTaskCallback callback, s32 priority);
void removeGameTask(s32 taskId);
void setCurrentGameTaskCallback(GameTaskCallback callback, s32 callbackIndex);
void suspendGameTask(s32 taskId);
void resumeGameTask(s32 taskId);

#endif
