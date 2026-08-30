#ifndef ENDING_CREDITS_UI_H
#define ENDING_CREDITS_UI_H

#include "common.h"

typedef struct EndingCreditsPageTextActor {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s16 x;
    /* 0x1E */ s16 y;
    /* 0x20 */ u16 pageIndex;
    /* 0x22 */ u16 timer;
    /* 0x24 */ s16 alpha;
    /* 0x26 */ u8 state;
} EndingCreditsPageTextActor; // size = 0x28

void initEndingCreditsPageTextActor(EndingCreditsPageTextActor *arg0);

#endif
