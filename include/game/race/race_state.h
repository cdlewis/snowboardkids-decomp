#ifndef RACE_STATE_H
#define RACE_STATE_H

#include "common.h"

typedef union {
    s16 signedValue;
    u16 unsignedValue;
} RaceCourseIndexValue;

typedef struct {
    /* 0x00 */ s16 pathIndex;
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ Vec3i unk8;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ Vec3i pos;
    /* 0x2C */ Vec3i cameraPos;
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 unk3A;
    /* 0x3C */ s16 finishLinePathIndex;
    /* 0x3E */ s16 maxSurfaceIndex;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
} RaceCourseStartEntry;

typedef struct {
    /* 0x00 */ s8 active;
    /* 0x01 */ u8 pad1[0xB0 - 0x01];
} RacePlayerHudStatus;

typedef char RaceCourseStartEntrySizeCheck[(sizeof(RaceCourseStartEntry) == 0x48) ? 1 : -1];
typedef char RacePlayerHudStatusSizeCheck[(sizeof(RacePlayerHudStatus) == 0xB0) ? 1 : -1];

extern RaceCourseIndexValue gRaceCourseIndex;
extern RaceCourseStartEntry gRaceCourseStartEntries[];
extern RacePlayerHudStatus gRacePlayerHudStatuses[];
extern u8 gRaceDemoPlaybackEnabled;
extern u8 gRaceResultState;
extern u8 gRaceRumbleEnabled;
extern u8 gRaceSplitscreenMode;

#endif
