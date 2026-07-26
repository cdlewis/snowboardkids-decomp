#ifndef RACE_HUD_H
#define RACE_HUD_H

#include "common.h"

typedef struct {
    /* 0x0 */ s8 minutes;
    /* 0x1 */ s8 seconds;
    /* 0x2 */ s16 fraction;
} RaceTimer;

void initRaceHud(void);
void drawTrickAttackChallengeHud(void *arg0);
void drawTrickAttackChallengeLabels(void *arg0);
void drawScoreAttackChallengeHud(void *arg0);
void drawScoreAttackChallengeLabels(void *arg0);
void drawTargetTimeChallengeHud(void *arg0);
void drawTargetTimeChallengeLabels(void *arg0);
void drawTimeTrialHud(void *arg0);
void drawTimeTrialLabels(void *arg0);
void drawSinglePlayerRaceHud(void *arg0);
void noopRaceHudCallback(void *arg0);
void drawTrainingRaceHud(void *arg0);
void drawTwoPlayerRaceHud(void *arg0);
void drawTwoPlayerLapCounter(void *arg0);
void drawMultiplayerRaceHud(void *arg0);
void drawMultiplayerLapCounter(void *arg0);
void drawThreePlayerHudDivider(void *arg0);
void incrementRaceElapsedTimer(void);
void decrementRaceChallengeTimeLimit(void);
void drawRaceCourseProgressMeter(void *arg0);
void updateRaceCourseProgressMeter(void);
void updateRaceHud(void);
s32 calculateRaceTimerDelta(RaceTimer *arg0, RaceTimer *arg1, RaceTimer *arg2);

#endif
