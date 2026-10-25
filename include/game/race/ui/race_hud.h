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

/* Shot tiles add shotCount - 1; item tiles use itemType directly. */
extern u8 gRaceHudShotTileOffsets[8];
extern u8 gRaceHudItemTileIds[8];
extern RaceTimer gRaceElapsedTimer;
extern RaceTimer gRaceChallengeTimeLimit;
extern RaceTimer gRaceTimeTrialFinishTime;
extern RaceTimer gRaceCourseTargetTimes[10];
/* Stored as s16 for the original ABI; values are RaceHudMode. */
extern s16 gRaceHudMode;
extern s16 gRaceHudCoinSpinnerFrame;
extern const char gRaceHudSinglePlayerScoreFormat[];
extern const char gRaceHudTrainingScoreFormat[];
extern const char gRaceHudTwoPlayerScoreFormat[];
extern const char gRaceHudMultiplayerScoreFormat[];

extern u8 gRaceChallengeFailed;

#if defined(VERSION_US)
extern u32 gRaceCoursePlayerPathOffsetData[];
#endif

#if defined(VERSION_JP)
extern const char gRaceHudScoreAttackPointTargetFormat[];
extern const char gRaceHudScoreAttackTimeLimitLabel[];
extern const char gRaceHudTargetTimeChallengeFractionFormat[];
extern const char gRaceHudTargetTimeChallengeLapProgressFormat[];
extern const char gRaceHudTargetTimeChallengeLapTimeLabel[];
extern const char gRaceHudTargetTimeChallengeMinutesFormat[];
extern const char gRaceHudTargetTimeChallengeSecondaryCounterFormat[];
extern const char gRaceHudTargetTimeChallengeSecondsFormat[];
extern const char gRaceHudTargetTimeChallengeTargetTimeFormat[];
extern const char gRaceHudTargetTimeChallengeTargetTimeLabel[];
extern const char gRaceHudTimeTrialBestLapFormat[];
extern const char gRaceHudTimeTrialBestLapLabel[];
extern const char gRaceHudTimeTrialLapTimeLabel[];
extern const char gRaceHudTrickAttackPointLabel[];
extern const char gRaceHudTrickAttackTimeLimitLabel[];
extern u16 gRaceProgressMeterIconPalettes[];
extern u16 gRaceProgressMeterIconTiles[];
extern u8 gRaceTimeTrialFinishRecorded;
extern RaceTimer gScoreAttackTimeLimitsByCourse[];
extern RaceTimer gTrickAttackTimeLimit;
#endif

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
/* Viewport-local HUD callbacks differ between 2P and 3/4P. The shared meter is
 * queued afterwards: race overlay in 1P, menu queue in multiplayer. */
void drawTwoPlayerRaceHud(void *arg0);
void drawTwoPlayerLapCounter(void *arg0);
void drawMultiplayerRaceHud(void *arg0);
void drawMultiplayerScoreAndLapCounter(void *arg0);
void drawThreePlayerNoEntryPanel(void *arg0);
void incrementRaceElapsedTimer(void);
void decrementRaceChallengeTimeLimit(void);
/* Entry 0x50 starts at (xBase + 4, yBase + 4), relative to menu viewport center.
 * Asset bounds are distinct from the visible meter stroke. */
void drawRaceCourseProgressMeter(void *arg0);
void updateRaceCourseProgressMeter(void);
void updateRaceHud(void);
s32 calculateRaceTimerDelta(RaceTimer *arg0, RaceTimer *arg1, RaceTimer *arg2);

#endif
