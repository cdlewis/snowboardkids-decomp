#ifndef PLAYER_COUNT_SELECT_UI_H
#define PLAYER_COUNT_SELECT_UI_H

#include "common.h"

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
} PlayerCountSelectWidgetActor;

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 iconX[5];
    /* 0x22 */ s16 iconY;
    /* 0x24 */ u8 state;
    /* 0x25 */ u8 spawnTimer;
    /* 0x26 */ u8 playerCount;
} PlayerCountSelectRowActor;

void func_80029200(PlayerCountSelectRowActor *arg0);
void func_80029344(PlayerCountSelectRowActor *arg0);
void func_80029548(PlayerCountSelectRowActor *arg0);
void func_80029598(PlayerCountSelectWidgetActor *arg0);
void func_800296D8(PlayerCountSelectWidgetActor *arg0);
void func_800297D8(PlayerCountSelectWidgetActor *arg0);
void func_8002980C(PlayerCountSelectWidgetActor *arg0);
void func_80029CE4(PlayerCountSelectWidgetActor *arg0);
void func_80029FB8(PlayerCountSelectWidgetActor *arg0);
void func_8002A008(PlayerCountSelectWidgetActor *arg0);
void func_8002A27C(PlayerCountSelectWidgetActor *arg0);
void func_8002A458(PlayerCountSelectWidgetActor *arg0);
void func_8002A49C(PlayerCountSelectWidgetActor *arg0);
void func_8002A710(PlayerCountSelectWidgetActor *arg0);
void func_8002A8EC(PlayerCountSelectWidgetActor *arg0);
void func_8002A930(PlayerCountSelectWidgetActor *arg0);
void func_8002AB24(PlayerCountSelectWidgetActor *arg0);
void func_8002AD74(PlayerCountSelectWidgetActor *arg0);
void func_8002ADB8(PlayerCountSelectWidgetActor *arg0);
void func_8002AE3C(PlayerCountSelectWidgetActor *arg0);
void func_8002AFB8(PlayerCountSelectWidgetActor *arg0);
void func_8002AFF8(PlayerCountSelectWidgetActor *arg0);
void func_8002B05C(PlayerCountSelectWidgetActor *arg0);
void func_8002B15C(PlayerCountSelectWidgetActor *arg0);
void func_8002B198(PlayerCountSelectWidgetActor *arg0);
void func_8002B1FC(PlayerCountSelectWidgetActor *arg0);
void func_8002B2FC(PlayerCountSelectWidgetActor *arg0);
void func_8002B338(PlayerCountSelectWidgetActor *arg0);
void func_8002B424(PlayerCountSelectWidgetActor *arg0);
void func_8002B524(PlayerCountSelectWidgetActor *arg0);

#endif
