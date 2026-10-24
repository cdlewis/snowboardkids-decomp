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
} EndingCreditsPageTextActor;

typedef struct EndingObjectSpriteDebugViewerActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 spriteId;
    /* 0x1E */ u8 enabled;
    /* 0x1F */ u8 palette;
} EndingObjectSpriteDebugViewerActor;

#if defined(VERSION_JP)
void drawEndingCreditsPageText(EndingCreditsPageTextActor *arg0);
void updateEndingCreditsPageText(EndingCreditsPageTextActor *arg0);
void updateEndingCreditsTheEndTextFadeIn(EndingCreditsPageTextActor *arg0);
void updateEndingObjectSpriteDebugViewer(EndingObjectSpriteDebugViewerActor *arg0);
#endif

void initEndingCreditsPageTextActor(EndingCreditsPageTextActor *arg0);

#endif
