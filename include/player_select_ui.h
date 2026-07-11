#ifndef PLAYER_SELECT_UI_H
#define PLAYER_SELECT_UI_H

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
    } transition;
    /* 0x20 */ s16 counter;
    union {
        struct {
            /* 0x22 */ u8 subState;
            /* 0x23 */ u8 subTimer;
        } bytes;
        /* 0x22 */ s16 rowY;
    } row;
} PlayerSelectWidgetActor;

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 iconX[5];
    /* 0x22 */ s16 iconY;
    /* 0x24 */ u8 state;
    /* 0x25 */ u8 spawnTimer;
    /* 0x26 */ u8 playerCount;
} PlayerSelectRowActor;

void func_800191D0(PlayerSelectRowActor *arg0);
void func_80019314(PlayerSelectRowActor *arg0);
void func_8001952C(PlayerSelectRowActor *arg0);
void func_8001958C(PlayerSelectWidgetActor *arg0);
void func_800196CC(PlayerSelectWidgetActor *arg0);
void func_800197CC(PlayerSelectWidgetActor *arg0);
void func_80019800(PlayerSelectWidgetActor *arg0);
void func_80019CD8(PlayerSelectWidgetActor *arg0);
void func_80019FAC(PlayerSelectWidgetActor *arg0);
void func_80019FFC(PlayerSelectWidgetActor *arg0);
void func_8001A270(PlayerSelectWidgetActor *arg0);
void func_8001A44C(PlayerSelectWidgetActor *arg0);
void func_8001A490(PlayerSelectWidgetActor *arg0);
void func_8001A704(PlayerSelectWidgetActor *arg0);
void func_8001A8E0(PlayerSelectWidgetActor *arg0);
void func_8001A924(PlayerSelectWidgetActor *arg0);
void func_8001AB98(PlayerSelectWidgetActor *arg0);
void func_8001AD74(PlayerSelectWidgetActor *arg0);
void func_8001ADB8(PlayerSelectWidgetActor *arg0);
void func_8001B02C(PlayerSelectWidgetActor *arg0);
void func_8001B210(PlayerSelectWidgetActor *arg0);
void func_8001B254(PlayerSelectWidgetActor *arg0);
void func_8001B2D8(PlayerSelectWidgetActor *arg0);
void func_8001B454(PlayerSelectWidgetActor *arg0);
void func_8001B494(PlayerSelectWidgetActor *arg0);
void func_8001B520(PlayerSelectWidgetActor *arg0);
void func_8001B638(PlayerSelectWidgetActor *arg0);
void func_8001B674(PlayerSelectWidgetActor *arg0);
void func_8001B6D8(PlayerSelectWidgetActor *arg0);
void func_8001B7D8(PlayerSelectWidgetActor *arg0);
void func_8001B814(PlayerSelectWidgetActor *arg0);
void func_8001B8F0(PlayerSelectWidgetActor *arg0);
void func_8001B9F0(PlayerSelectWidgetActor *arg0);
void func_8001BA2C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

#endif
