#ifndef RACE_HUD_H
#define RACE_HUD_H

#include "common.h"
#include "game/save_data.h"

typedef GameSaveRecordTime RaceTimer;

typedef enum RaceHudMode {
    RACE_HUD_MODE_NONE = -1,
    RACE_HUD_MODE_ONE_PLAYER = 0,
    RACE_HUD_MODE_TWO_PLAYER = 1,
    RACE_HUD_MODE_THREE_PLAYER = 2,
    RACE_HUD_MODE_FOUR_PLAYER = 3,
    RACE_HUD_MODE_TIME_TRIAL = 4,
    RACE_HUD_MODE_TARGET_TIME_CHALLENGE = 5,
    RACE_HUD_MODE_SCORE_ATTACK = 6,
    RACE_HUD_MODE_TRICK_ATTACK = 7,
    RACE_HUD_MODE_TRAINING_OR_DEMO = 8
} RaceHudMode;

extern RaceTimer gRaceElapsedTimer;
extern RaceTimer gRaceCourseTargetTimes[10];
/* Stored as s16 for the original ABI; values are RaceHudMode. */
extern s16 gRaceHudMode;
extern s16 gRaceHudSpinnerFrame;
extern const char gRaceHudSinglePlayerTimerFormat[];
extern const char gRaceHudTrainingTimerFormat[];
extern const char gRaceHudTwoPlayerTimerFormat[];
extern const char gRaceHudMultiplayerLapCounterFormat[];

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
