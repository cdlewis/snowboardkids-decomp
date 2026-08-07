#ifndef RACE_REPLAY_H
#define RACE_REPLAY_H

#include "common.h"

#define RACE_INPUT_HISTORY_LENGTH 0x1194
#define PACKED_RACE_RECORD_REPLAY_BUFFER_SIZE 0x2F80
#define COMPRESSED_RACE_RECORD_REPLAY_HALFWORD_COUNT 0x1000

typedef struct RaceInputHistoryBuffer {
    /* 0x0000 */ s32 writeIndex;
    /* 0x0004 */ s32 lastWriteIndex;
    /* 0x0008 */ s8 enabled;
    /* 0x0009 */ s8 courseId;
    /* 0x000A */ s8 characterId;
    /* 0x000B */ s8 characterVariant;
    /* 0x000C */ s8 unkC;
    /* 0x000D */ s8 stickX[RACE_INPUT_HISTORY_LENGTH];
    /* 0x11A1 */ s8 stickY[RACE_INPUT_HISTORY_LENGTH];
    /* 0x2335 */ u8 buttons[RACE_INPUT_HISTORY_LENGTH];
} RaceInputHistoryBuffer;

typedef struct PackedRaceRecordInput {
    /* 0x0 */ s8 stickX;
    /* 0x1 */ s8 stickY;
    /* 0x2 */ u8 buttons;
} PackedRaceRecordInput;

typedef union PackedRaceRecordReplay {
    u8 bytes[PACKED_RACE_RECORD_REPLAY_BUFFER_SIZE];
    struct {
        /* 0x0 */ s16 frameCount;
        /* 0x2 */ s8 characterId;
        /* 0x3 */ s8 characterVariant;
        /* 0x4 */ PackedRaceRecordInput inputs[1];
    } fields;
} PackedRaceRecordReplay;

typedef char RaceInputHistoryBufferSizeCheck[(sizeof(RaceInputHistoryBuffer) == 0x34CC) ? 1 : -1];
typedef char PackedRaceRecordInputSizeCheck[(sizeof(PackedRaceRecordInput) == 0x3) ? 1 : -1];
typedef char
    PackedRaceRecordReplaySizeCheck[(sizeof(PackedRaceRecordReplay) == PACKED_RACE_RECORD_REPLAY_BUFFER_SIZE) ? 1 : -1];

extern PackedRaceRecordReplay gPackedRaceRecordReplayBuffer;
extern u16 gCompressedRaceRecordReplayBuffer[COMPRESSED_RACE_RECORD_REPLAY_HALFWORD_COUNT];

s32 compressRaceRecordReplayData(u8 *src, s32 srcLen, u16 *dst);
s32 saveRaceRecordReplayData(void);
void loadCurrentRaceRecordReplayData(void);

#endif
