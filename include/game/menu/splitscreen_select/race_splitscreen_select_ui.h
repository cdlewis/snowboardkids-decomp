#ifndef RACE_SPLITSCREEN_SELECT_UI_H
#define RACE_SPLITSCREEN_SELECT_UI_H

#include "common.h"

typedef struct {
    /* 0x0 */ u8 state;
    /* 0x1 */ u8 pad1;
    /* 0x2 */ s16 nextState;
} RaceSplitscreenSelectCursorTarget;

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    union {
        /* 0x1C */ s16 spriteIndex;
        struct {
            /* 0x1C */ u8 state;
            /* 0x1D */ u8 pad1D;
        } bytes;
    } sprite;
    union {
        struct {
            /* 0x1E */ u8 state;
            /* 0x1F */ u8 timer;
        } bytes;
        /* 0x1E */ s16 alpha;
    } transition;
    /* 0x20 */ s16 counter;
    union {
        struct {
            /* 0x22 */ u8 subState;
            /* 0x23 */ u8 subTimer;
        } bytes;
        /* 0x22 */ s16 rowY;
    } row;
} RaceSplitscreenSelectWidgetActor;

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 iconX[5];
    /* 0x22 */ s16 iconY;
    /* 0x24 */ u8 state;
    /* 0x25 */ u8 spawnTimer;
    /* 0x26 */ u8 playerCount;
} RaceSplitscreenSelectRowActor;

extern RaceSplitscreenSelectCursorTarget gRaceSplitscreenSelectCursorTarget;
extern s16 gRaceSplitscreenSelectPortraitAlpha;

void drawRaceSplitscreenSelectPlayerCountIcons(RaceSplitscreenSelectRowActor *arg0);
void updateRaceSplitscreenSelectPlayerCountIcons(RaceSplitscreenSelectRowActor *arg0);
void initRaceSplitscreenSelectPlayerCountIcons(RaceSplitscreenSelectRowActor *arg0);
void drawRaceSplitscreenSelectCornerSprites(RaceSplitscreenSelectWidgetActor *arg0);
void updateRaceSplitscreenSelectCornerSprites(RaceSplitscreenSelectWidgetActor *arg0);
void initRaceSplitscreenSelectCornerSprites(RaceSplitscreenSelectWidgetActor *arg0);
void drawRaceSplitscreenSelectOption0Frame(RaceSplitscreenSelectWidgetActor *arg0);
void updateRaceSplitscreenSelectOption0Frame(RaceSplitscreenSelectWidgetActor *arg0);
void initRaceSplitscreenSelectOption0Frame(RaceSplitscreenSelectWidgetActor *arg0);
void drawRaceSplitscreenSelectOption1Frame(RaceSplitscreenSelectWidgetActor *arg0);
void updateRaceSplitscreenSelectOption1Frame(RaceSplitscreenSelectWidgetActor *arg0);
void initRaceSplitscreenSelectOption1Frame(RaceSplitscreenSelectWidgetActor *arg0);
void drawRaceSplitscreenSelectOption2Frame(RaceSplitscreenSelectWidgetActor *arg0);
void updateRaceSplitscreenSelectOption2Frame(RaceSplitscreenSelectWidgetActor *arg0);
void initRaceSplitscreenSelectOption2Frame(RaceSplitscreenSelectWidgetActor *arg0);
void drawRaceSplitscreenSelectOption3Frame(RaceSplitscreenSelectWidgetActor *arg0);
void updateRaceSplitscreenSelectOption3Frame(RaceSplitscreenSelectWidgetActor *arg0);
void initRaceSplitscreenSelectOption3Frame(RaceSplitscreenSelectWidgetActor *arg0);
void drawRaceSplitscreenSelectOption4Frame(RaceSplitscreenSelectWidgetActor *arg0);
void updateRaceSplitscreenSelectOption4Frame(RaceSplitscreenSelectWidgetActor *arg0);
void initRaceSplitscreenSelectOption4Frame(RaceSplitscreenSelectWidgetActor *arg0);
void drawRaceSplitscreenSelectCursor(RaceSplitscreenSelectWidgetActor *arg0);
void updateRaceSplitscreenSelectCursor(RaceSplitscreenSelectWidgetActor *arg0);
void initRaceSplitscreenSelectCursor(RaceSplitscreenSelectWidgetActor *arg0);
void drawRaceSplitscreenSelectPortrait(RaceSplitscreenSelectWidgetActor *arg0);
void updateRaceSplitscreenSelectPortrait(RaceSplitscreenSelectWidgetActor *arg0);
void initRaceSplitscreenSelectPortrait(RaceSplitscreenSelectWidgetActor *arg0);
void drawRaceSplitscreenSelectArrowPrompt(RaceSplitscreenSelectWidgetActor *arg0);
void updateRaceSplitscreenSelectArrowPrompt(RaceSplitscreenSelectWidgetActor *arg0);
void initRaceSplitscreenSelectArrowPrompt(RaceSplitscreenSelectWidgetActor *arg0);
void drawRaceSplitscreenSelectEntryFee(RaceSplitscreenSelectWidgetActor *arg0);
void updateRaceSplitscreenSelectEntryFee(RaceSplitscreenSelectWidgetActor *arg0);
void initRaceSplitscreenSelectEntryFee(RaceSplitscreenSelectWidgetActor *arg0);
void drawMenuPanelBackdrop(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

#endif
