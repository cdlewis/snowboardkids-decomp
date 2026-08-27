#ifndef TITLE_DEMO_RACE_INTRO_H
#define TITLE_DEMO_RACE_INTRO_H

#include "common.h"
#include "game/race/player/race_player_input.h"

typedef union RacePlayerReplaySnapshot {
    u32 words[sizeof(RacePlayer) / sizeof(u32)];
    RacePlayer player;
    u8 bytes[sizeof(RacePlayer)];
} RacePlayerReplaySnapshot;

typedef char RacePlayerReplaySnapshotSizeCheck[
    (sizeof(RacePlayerReplaySnapshot) == sizeof(RacePlayer)) ? 1 : -1
];

extern s16 gTitleDemoReplaySegmentFrames[16];
extern s16 gTitleDemoCameraModeFrames[22];
extern u8 gTitleDemoCameraModes[24];
extern RacePlayerReplaySnapshot gTitleDemoReplayInputs[4][16];
extern volatile char gTitleDemoRaceIntroViewportHeight;
extern s8 gTitleDemoRaceIntroFadeStep;

void initTitleDemoRaceIntro(void);
void waitForTitleDemoRaceIntroStart(void);
void updateTitleDemoRaceIntro(void);
void finishTitleDemoRaceIntro(void);

#endif
