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
#define GAME_SAVE_REPLAY_DATA_TAIL_SIZE (GAME_SAVE_REPLAY_DATA_SIZE - GAME_SAVE_REPLAY_COPY_SIZE)

typedef struct GameSaveRecordTime {
    /* 0x0 */ s8 minutes;
    /* 0x1 */ s8 seconds;
    /* 0x2 */ s16 fraction;
} GameSaveRecordTime;

typedef struct GameSaveReplaySlot {
    /* 0x0 */ u16 length;
    /* 0x2 */ u16 offset;
} GameSaveReplaySlot;

typedef struct GameSaveResultEntry {
    /* 0x0 */ s16 value;
    /* 0x2 */ s16 padding;
} GameSaveResultEntry;

typedef struct GameSaveData {
    union {
        struct {
            union {
                /* 0x0000 */ s32 checksum;
                struct {
                    /* 0x0000 */ u8 pad0[4];
                    union {
                        /* 0x0004 */ s32 highScores[12];
                        struct {
                            /* 0x0004 */ s32 money;
                            /* 0x0008 */ u8 pad8[0x34 - 0x8];
                        };
                    };
                };
            };
            union {
                /* 0x0034 */ u8 cupPlacements[0x1A];
                struct {
                    /* 0x0034 */ u8 pad34[0x3F - 0x34];
                    /* 0x003F */ s8 courseUnlockStates[12];
                    union {
                        /* 0x004B */ u8 characterFlags;
                        /* 0x004B */ u8 recordFlags;
                    };
                    union {
                        /* 0x004C */ u8 highestUnlockedCourse;
                        /* 0x004C */ u8 progressionLevel;
                    };
                    /* 0x004D */ u8 pad4D;
                };
            };
            /* 0x004E */ GameSaveRecordTime timeTrialRecords[11][5];
            /* 0x012A */ GameSaveRecordTime bestLapRecords[11];
            /* 0x0156 */ GameSaveRecordTime raceRecords[11][5];
            union {
                /* 0x0232 */ GameSaveReplaySlot replaySlots[GAME_SAVE_REPLAY_SLOT_COUNT];
                /* 0x0232 */ GameSaveResultEntry resultEntries[GAME_SAVE_REPLAY_SLOT_COUNT];
            };
            union {
                /* 0x0256 */ u8 replayDataAndPadding[GAME_SAVE_REPLAY_DATA_SIZE];
                /* 0x0256 */ u16 replayData[GAME_SAVE_REPLAY_DATA_HALFWORD_COUNT];
                struct {
                    /* 0x0256 */ u16 replayCopyData[GAME_SAVE_REPLAY_COPY_HALFWORD_COUNT];
                    /* 0x7500 */ u8 replayDataTail[GAME_SAVE_REPLAY_DATA_TAIL_SIZE];
                };
            };
            /* 0x7756 */ u16 trickAttackScores[11][5];
            /* 0x77C4 */ u8 trickAttackCharacterIds[11][5];
            /* 0x77FB */ u8 timeTrialCharacterIds[11][5];
            /* 0x7832 */ u8 scoreAttackScores[11][5];
            /* 0x7869 */ u8 scoreAttackCharacterIds[11][5];
            /* 0x78A0 */ u8 raceRecordCharacterIds[11][5];
            /* 0x78D7 */ u8 extraCourseUnlockFlags;
            /* 0x78D8 */ u8 tail[GAME_SAVE_SLOT_SIZE - 0x78D8];
        };
        struct {
            /* 0x0000 */ u8 checksumBytes[4];
            /* 0x0004 */ u8 bytes[0x78DC];
            /* 0x78E0 */ u8 saveSlotTail[0x18];
        };
        /* 0x0000 */ u8 rawBytes[GAME_SAVE_SLOT_SIZE];
    };
} GameSaveData;

typedef GameSaveData GameSaveDataBuffer[GAME_SAVE_SLOT_COUNT];

typedef struct GameSaveCourseUnlockStates {
    s8 values[12];
} GameSaveCourseUnlockStates;

typedef struct GameSaveRawByte {
    s8 value;
} GameSaveRawByte;

typedef char GameSaveReplaySlotSizeCheck[(sizeof(GameSaveReplaySlot) == 0x4) ? 1 : -1];
typedef char GameSaveDataSizeCheck[(sizeof(GameSaveData) == GAME_SAVE_SLOT_SIZE) ? 1 : -1];
typedef char GameSaveDataBufferSizeCheck[
    (sizeof(GameSaveDataBuffer) == (GAME_SAVE_SLOT_COUNT * GAME_SAVE_SLOT_SIZE)) ? 1 : -1
];

extern GameSaveDataBuffer gGameSaveDataBuffer;
extern GameSaveCourseUnlockStates gPrimaryCourseUnlockStates;
extern GameSaveRawByte gCourseUnlockScanStart;
extern GameSaveRawByte gCourseUnlockScanEnd;

#endif
