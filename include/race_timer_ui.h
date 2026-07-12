#ifndef RACE_TIMER_UI_H
#define RACE_TIMER_UI_H

#include "common.h"

typedef struct {
    /* 0x0 */ s8 minutes;
    /* 0x1 */ s8 seconds;
    /* 0x2 */ s16 fraction;
} RaceTimer;

void initRaceHud(void);
void drawTrickAttackChallengeHud(s32 arg0);
void drawTrickAttackChallengeLabels(s32 arg0);
void drawScoreAttackChallengeHud(s32 arg0);
void drawScoreAttackChallengeLabels(s32 arg0);
void drawTargetTimeChallengeHud(s32 arg0);
void drawTargetTimeChallengeLabels(s32 arg0);
void drawTimeTrialHud(s32 arg0);
void drawTimeTrialLabels(s32 arg0);
void drawSinglePlayerRaceHud(s32 arg0);
void noopRaceHudCallback(s32 arg0);
void drawTrainingRaceHud(s32 arg0);
void drawTwoPlayerRaceHud(s32 arg0);
void drawTwoPlayerLapCounter(s32 arg0);
void drawMultiplayerRaceHud(s32 arg0);
void drawMultiplayerLapCounter(s32 arg0);
void drawThreePlayerHudDivider(s32 arg0);
void incrementRaceElapsedTimer(void);
void decrementRaceChallengeTimeLimit(void);
void drawRaceCourseProgressMeter(s32 arg0);
void updateRaceCourseProgressMeter(void);
void updateRaceHud(void);
s32 calculateRaceTimerDelta(RaceTimer *arg0, RaceTimer *arg1, RaceTimer *arg2);

#endif
