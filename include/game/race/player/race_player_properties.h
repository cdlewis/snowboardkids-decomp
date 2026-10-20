#ifndef RACE_PLAYER_PROPERTIES_H
#define RACE_PLAYER_PROPERTIES_H

#include "common.h"

#define RACE_BOARD_PROPERTIES_COUNT 15
#define RACE_CHARACTER_PROPERTIES_COUNT 7
#define RACE_DEMO_CHARACTER_PROPERTIES_COUNT 6

typedef struct RacePlayerProperties {
    /* 0x00 */ s16 maxSpeed;
    /* 0x02 */ s16 gravity;
    /* 0x04 */ s16 aerialGravity;
    /* 0x06 */ s16 turnStrength;
    /* 0x08 */ s16 lateralDeceleration;
    /* 0x0A */ s16 turnRadiusAtFullLean;
    /* 0x0C */ s16 turnRadiusAtHalfLean;
    /* 0x0E */ s16 forwardDeceleration;
    /* 0x10 */ s16 reverseDeceleration;
} RacePlayerProperties;

typedef char RacePlayerPropertiesSizeCheck[(sizeof(RacePlayerProperties) == 0x12) ? 1 : -1];

extern RacePlayerProperties gRaceBoardProperties[RACE_BOARD_PROPERTIES_COUNT];
extern RacePlayerProperties gRaceCharacterProperties[RACE_CHARACTER_PROPERTIES_COUNT];
extern RacePlayerProperties gRaceDemoBoardProperties[RACE_BOARD_PROPERTIES_COUNT];
extern RacePlayerProperties gRaceDemoCharacterProperties[RACE_DEMO_CHARACTER_PROPERTIES_COUNT];

#endif
