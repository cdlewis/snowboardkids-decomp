#ifndef RACE_TYPE_SELECT_UI_H
#define RACE_TYPE_SELECT_UI_H

#include "common.h"

typedef struct {
    s16 alpha;
    s8 state;
} RaceTypeSelectCursorState;

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
        /* 0x1E */ u16 step;
    } transition;
    union {
        struct {
            /* 0x20 */ u8 state;
            /* 0x21 */ u8 timer;
        } bytes;
        /* 0x20 */ s16 counter;
    } widget;
    union {
        struct {
            /* 0x22 */ u8 subState;
            /* 0x23 */ u8 subTimer;
        } bytes;
        /* 0x22 */ s16 rowY;
    } row;
} RaceTypeSelectWidgetActor;

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 iconX[5];
    /* 0x22 */ s16 iconY;
    /* 0x24 */ u8 state;
    /* 0x25 */ u8 spawnTimer;
    /* 0x26 */ u8 playerCount;
} RaceTypeSelectRowActor;

extern RaceTypeSelectCursorState gRaceTypeSelectCursorTarget;
extern u8 gRaceTypeSelectCursorAnimState;

void drawRaceTypeSelectOptionIcons(RaceTypeSelectRowActor *arg0);
void updateRaceTypeSelectOptionIcons(RaceTypeSelectRowActor *arg0);
void initRaceTypeSelectOptionIcons(RaceTypeSelectRowActor *arg0);
void drawRaceTypeSelectCornerSprites(RaceTypeSelectWidgetActor *arg0);
void updateRaceTypeSelectCornerSprites(RaceTypeSelectWidgetActor *arg0);
void initRaceTypeSelectCornerSprites(RaceTypeSelectWidgetActor *arg0);
void drawRaceTypeSelectOption0Frame(RaceTypeSelectWidgetActor *arg0);
void updateRaceTypeSelectOption0Frame(RaceTypeSelectWidgetActor *arg0);
void initRaceTypeSelectOption0Frame(RaceTypeSelectWidgetActor *arg0);
void drawRaceTypeSelectOption1Frame(RaceTypeSelectWidgetActor *arg0);
void updateRaceTypeSelectOption1Frame(RaceTypeSelectWidgetActor *arg0);
void initRaceTypeSelectOption1Frame(RaceTypeSelectWidgetActor *arg0);
void drawRaceTypeSelectOption2Frame(RaceTypeSelectWidgetActor *arg0);
void updateRaceTypeSelectOption2Frame(RaceTypeSelectWidgetActor *arg0);
void initRaceTypeSelectOption2Frame(RaceTypeSelectWidgetActor *arg0);
void drawRaceTypeSelectOption3Frame(RaceTypeSelectWidgetActor *arg0);
void updateRaceTypeSelectOption3Frame(RaceTypeSelectWidgetActor *arg0);
void initRaceTypeSelectOption3Frame(RaceTypeSelectWidgetActor *arg0);
void drawRaceTypeSelectCursor(RaceTypeSelectWidgetActor *arg0);
void updateRaceTypeSelectCursor(RaceTypeSelectWidgetActor *arg0);
void initRaceTypeSelectCursor(RaceTypeSelectWidgetActor *arg0);
void drawRaceTypeSelectPortrait(RaceTypeSelectWidgetActor *arg0);
void updateRaceTypeSelectPortrait(RaceTypeSelectWidgetActor *arg0);
void initRaceTypeSelectPortrait(RaceTypeSelectWidgetActor *arg0);
void drawRaceTypeSelectArrowPrompt(RaceTypeSelectWidgetActor *arg0);
void updateRaceTypeSelectArrowPrompt(RaceTypeSelectWidgetActor *arg0);
void initRaceTypeSelectArrowPrompt(RaceTypeSelectWidgetActor *arg0);
void drawRaceTypeSelectEntryFee(RaceTypeSelectWidgetActor *arg0);
void updateRaceTypeSelectEntryFee(RaceTypeSelectWidgetActor *arg0);
void initRaceTypeSelectEntryFee(RaceTypeSelectWidgetActor *arg0);

#endif
