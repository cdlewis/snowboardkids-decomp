#ifndef ENDING_SEQUENCE_EFFECTS_H
#define ENDING_SEQUENCE_EFFECTS_H

#include "common.h"

typedef struct EndingSequenceEffectActor EndingSequenceEffectActor;

void spawnEndingCharacterVanishPoof(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void initEndingSpeedLines(EndingSequenceEffectActor *arg0);
void spawnEndingCharacterLoopingSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingBigBurst(EndingSequenceEffectActor *arg0);
void initEndingRisingStar(EndingSequenceEffectActor *arg0);
void initEndingRunDust(EndingSequenceEffectActor *arg0);
void spawnEndingSmallBurst(s16 arg0, s16 arg1);
void spawnEndingCharacterAura(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void spawnEndingPhaseAdvanceSparkle(s16 arg0, s16 arg1);
void spawnEndingDelayedSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingSnowmanEntranceEffect(EndingSequenceEffectActor *arg0);
void initRaceToMainMenuSnowboardIcon(EndingSequenceEffectActor *arg0);
void initRaceToMainMenuSnowflakeIcon(EndingSequenceEffectActor *arg0);
void initRaceToMainMenuLogoWipe(EndingSequenceEffectActor *arg0);

#endif
