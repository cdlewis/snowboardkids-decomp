#ifndef ENDING_SEQUENCE_ACTORS_H
#define ENDING_SEQUENCE_ACTORS_H

#include "common.h"

typedef struct EndingSequenceActor EndingSequenceActor;

void spawnEndingCharacterVanishPoof(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void initEndingNancySpeedLines(EndingSequenceActor *arg0);
void spawnEndingCharacterLoopingSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingTommyBigBurst(EndingSequenceActor *arg0);
void initEndingSlashRisingStar(EndingSequenceActor *arg0);
void initEndingNancyRunDust(EndingSequenceActor *arg0);
void spawnEndingSmallBurst(s16 arg0, s16 arg1);
void spawnEndingCharacterAura(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void spawnEndingPhaseAdvanceSparkle(s16 arg0, s16 arg1);
void spawnEndingDelayedSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingTommySnowmanEntrance(EndingSequenceActor *arg0);
void initRaceToMainMenuSnowboardIcon(EndingSequenceActor *arg0);
void initRaceToMainMenuSnowflakeIcon(EndingSequenceActor *arg0);
void initRaceToMainMenuLogoWipe(EndingSequenceActor *arg0);

#endif
