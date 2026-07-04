#ifndef RACE_INPUT_HISTORY_H
#define RACE_INPUT_HISTORY_H

#include "common.h"

#define RACE_INPUT_PLAYER_COUNT 4

typedef struct {
    /* 0x000 */ u16 playerIndex;
    /* 0x002 */ char pad2[2];
    /* 0x004 */ u8 unk4;
    /* 0x005 */ char pad5[0xE];
    /* 0x013 */ s8 isActive;
    /* 0x014 */ char pad14[1];
    /* 0x015 */ s8 unk15;
    /* 0x016 */ s8 replayInputSource;
    /* 0x017 */ char pad17[0x6D];
    /* 0x084 */ u32 inputFlags;
    /* 0x088 */ u32 currentInputFlags;
    /* 0x08C */ u32 disabledInputFlags;
    /* 0x090 */ s8 stickX;
    /* 0x091 */ s8 stickY;
    /* 0x092 */ char pad92[0x26A];
    /* 0x2FC */ u32 stateFlags;
    /* 0x300 */ char pad300[2];
    /* 0x302 */ s16 updateState;
    /* 0x304 */ s16 updateTimer;
    /* 0x306 */ char pad306[0x27C];
    /* 0x582 */ s16 unk582;
    /* 0x584 */ char pad584[0x86];
    /* 0x60A */ s16 replayFrame;
} RaceInputPlayer;

extern RaceInputPlayer D_80121D80[RACE_INPUT_PLAYER_COUNT];
extern RaceInputPlayer D_8012238C[];
extern RaceInputPlayer D_80122998[];
extern RaceInputPlayer D_80122FA4[];

void func_80084730(RaceInputPlayer *player);
s32 func_80084958(RaceInputPlayer *player);

#endif
