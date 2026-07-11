#ifndef ENDING_CREDITS_EFFECTS_H
#define ENDING_CREDITS_EFFECTS_H

#include "common.h"

typedef struct EndingCreditsEffectActor EndingCreditsEffectActor;

void spawnEndingCharacterVanishPoof(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void initEndingSpeedLines(EndingCreditsEffectActor *arg0);
void spawnEndingCharacterLoopingSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingBigBurst(EndingCreditsEffectActor *arg0);
void initEndingRisingStar(EndingCreditsEffectActor *arg0);
void initEndingRunDust(EndingCreditsEffectActor *arg0);
void spawnEndingSmallBurst(s16 arg0, s16 arg1);
void spawnEndingCharacterAura(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void spawnEndingPhaseAdvanceSparkle(s16 arg0, s16 arg1);
void spawnEndingDelayedSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingSnowmanEntranceEffect(EndingCreditsEffectActor *arg0);
void initRaceToMainMenuSnowboardIcon(EndingCreditsEffectActor *arg0);
void initRaceToMainMenuSnowflakeIcon(EndingCreditsEffectActor *arg0);
void initRaceToMainMenuLogoWipe(EndingCreditsEffectActor *arg0);

#endif
