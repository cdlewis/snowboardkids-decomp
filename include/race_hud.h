#ifndef RACE_HUD_H
#define RACE_HUD_H

#include "common.h"

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 alpha;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ u8 state;
    /* 0x21 */ u8 frame;
    /* 0x22 */ u8 mode;
    /* 0x23 */ u8 bounceTimer;
} RaceHudBannerActor;

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ s16 x;
    u8 pad1A[6];
    /* 0x20 */ s16 y;
} RaceHudPlayerFrameActor;

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 targetX;
    /* 0x1E */ s16 targetY;
    /* 0x20 */ s16 speedX;
    union {
        /* 0x22 */ s16 speedY;
        struct {
            /* 0x22 */ u8 playerFlags;
            /* 0x23 */ u8 unk23;
        };
    };
    /* 0x24 */ u8 state;
    /* 0x25 */ u8 timer;
    /* 0x26 */ s16 unk26;
} RaceHudMessageActor;

typedef union {
    s16 target[4];
    struct {
        u8 pad0[4];
        u8 inactiveTimer[4];
    } overlay;
} RaceHudPanelTargetX;

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ s16 x[4];
    /* 0x20 */ s16 y[4];
    /* 0x28 */ RaceHudPanelTargetX targetX;
    union {
        s16 target[4];
        u8 finishedBlink[4];
        u8 mode;
    } targetY;
    /* 0x38 */ s16 accumulator[4];
    /* 0x40 */ u16 tileSize[4];
    /* 0x48 */ s16 xDirection[4];
    /* 0x50 */ s16 timer[4];
    /* 0x58 */ s16 stepCount[4];
    /* 0x60 */ u8 axis[4];
    /* 0x64 */ u8 divisor[4];
    /* 0x68 */ u8 state[4];
} RaceHudPanelActor;

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ s16 x;
    u8 pad1A[6];
    /* 0x20 */ s16 y;
    u8 pad22[0x1E];
    /* 0x40 */ u16 tileSize;
} RaceHudPanelSlot;

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ s16 x[4];
    /* 0x20 */ s16 y;
    /* 0x22 */ s16 baseX;
    /* 0x24 */ s16 scale;
    /* 0x26 */ u8 mode;
    /* 0x27 */ u8 timer;
} RaceHudPlayerListActor;

void func_800171F0(RaceHudBannerActor *arg0);
void func_800177F8(RaceHudBannerActor *arg0);
void func_800179D4(RaceHudBannerActor *arg0);
void func_80017A10(RaceHudPlayerFrameActor *arg0);
void func_80017C34(RaceHudPanelActor *arg0);
void func_80017D08(RaceHudPanelActor *arg0);
void func_80017D6C(RaceHudMessageActor *arg0);
void func_80017F94(RaceHudMessageActor *arg0);
void func_80018060(RaceHudMessageActor *arg0);
void func_80018134(RaceHudPlayerListActor *arg0);
void func_800182A4(RaceHudPlayerListActor *arg0);
void func_800183DC(RaceHudPlayerListActor *arg0);
void func_800184C8(RaceHudPanelActor *arg0);
void func_80018AA0(RaceHudPanelActor *arg0);
void func_80018B6C(RaceHudPanelActor *arg0);
void func_80018BC0(RaceHudPanelSlot *arg0);
void func_80018C80(RaceHudPanelActor *arg0);
void func_800191A0(RaceHudPanelActor *arg0);

#endif
