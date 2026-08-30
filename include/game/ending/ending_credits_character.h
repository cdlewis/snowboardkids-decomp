#ifndef ENDING_CREDITS_CHARACTER_H
#define ENDING_CREDITS_CHARACTER_H

#include "common.h"
#include "game/engine/callback_task_scheduler.h"

typedef struct EndingCreditsCharacterActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i position;
    /* 0x24 */ Vec3s rotation;
    /* 0x2A */ u16 stateTimer;
} EndingCreditsCharacterActor;

#endif
