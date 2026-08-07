#ifndef GAME_SAVE_DATA_H
#define GAME_SAVE_DATA_H

#include "common.h"

#define GAME_SAVE_SLOT_COUNT 4
#define GAME_SAVE_SLOT_SIZE 0x78F8
#define GAME_SAVE_REPLAY_SLOT_COUNT 9
#define GAME_SAVE_REPLAY_DATA_SIZE 0x7500
#define GAME_SAVE_REPLAY_DATA_HALFWORD_COUNT (GAME_SAVE_REPLAY_DATA_SIZE / sizeof(u16))
#define GAME_SAVE_REPLAY_COPY_SIZE 0x72AA
#define GAME_SAVE_REPLAY_COPY_HALFWORD_COUNT (GAME_SAVE_REPLAY_COPY_SIZE / sizeof(u16))

typedef struct GameSaveRecordTime {
    /* 0x0 */ s8 minutes;
    /* 0x1 */ s8 seconds;
    /* 0x2 */ s16 fraction;
} GameSaveRecordTime;

typedef struct GameSaveReplaySlot {
    /* 0x0 */ u16 length;
    /* 0x2 */ u16 offset;
} GameSaveReplaySlot;

typedef struct GameSaveData {
    /* 0x0000 */ s32 checksum;

    /* Money occupies the first high-score word in the save format. */
    union {
        /* 0x0004 */ s32 highScores[12];
        struct {
            /* 0x0004 */ s32 money;
            /* 0x0008 */ u8 unknown0008[0x34 - 0x8];
        };
    };

    /* Course progression is also addressed as one contiguous placement table. */
    union {
        /* 0x0034 */ u8 cupPlacements[0x1A];
        struct {
            /* 0x0034 */ u8 unknown0034[0x3F - 0x34];
            /* 0x003F */ s8 courseUnlockStates[12];
            /* 0x004B */ u8 characterFlags;
            /* 0x004C */ u8 progressionLevel;
            /* 0x004D */ u8 unknown004D;
        };
    };

    /* 0x004E */ GameSaveRecordTime timeTrialRecords[11][5];
    /* 0x012A */ GameSaveRecordTime bestLapRecords[11];
    /* 0x0156 */ GameSaveRecordTime raceRecords[11][5];
    /* 0x0232 */ GameSaveReplaySlot replaySlots[GAME_SAVE_REPLAY_SLOT_COUNT];
    /* 0x0256 */ u16 replayData[GAME_SAVE_REPLAY_DATA_HALFWORD_COUNT];
    /* 0x7756 */ u16 trickAttackScores[11][5];
    /* 0x77C4 */ u8 trickAttackCharacterIds[11][5];
    /* 0x77FB */ u8 timeTrialCharacterIds[11][5];
    /* 0x7832 */ u8 scoreAttackScores[11][5];
    /* 0x7869 */ u8 scoreAttackCharacterIds[11][5];
    /* 0x78A0 */ u8 raceRecordCharacterIds[11][5];
    /* 0x78D7 */ u8 extraCourseUnlockFlags;
    /* 0x78D8 */ u8 reserved[GAME_SAVE_SLOT_SIZE - 0x78D8];
} GameSaveData;

typedef GameSaveData GameSaveDataBuffer[GAME_SAVE_SLOT_COUNT];

/* Matching-only slot-zero views for code that requires distinct IDO relocations. */
typedef struct GameSaveCourseUnlockStates {
    s8 values[12];
} GameSaveCourseUnlockStates;

typedef struct GameSaveRawByte {
    s8 value;
} GameSaveRawByte;

typedef struct GameSaveRawData {
    s8 values[GAME_SAVE_SLOT_SIZE];
} GameSaveRawData;

typedef char GameSaveReplaySlotSizeCheck[(sizeof(GameSaveReplaySlot) == 0x4) ? 1 : -1];
typedef char GameSaveDataSizeCheck[(sizeof(GameSaveData) == GAME_SAVE_SLOT_SIZE) ? 1 : -1];
typedef char
    GameSaveDataBufferSizeCheck[(sizeof(GameSaveDataBuffer) == (GAME_SAVE_SLOT_COUNT * GAME_SAVE_SLOT_SIZE)) ? 1 : -1];

extern GameSaveDataBuffer gGameSaveDataBuffer;
extern GameSaveCourseUnlockStates gPrimaryCourseUnlockStates;
extern GameSaveRawByte gCourseUnlockScanStart;
extern GameSaveRawByte gCourseUnlockScanEnd;
extern GameSaveRawData gPrimaryGameSaveRawData;

#endif
