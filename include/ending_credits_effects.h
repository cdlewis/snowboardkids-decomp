#ifndef ENDING_CREDITS_EFFECTS_H
#define ENDING_CREDITS_EFFECTS_H

#include "common.h"

typedef struct EndingCreditsEffectActor EndingCreditsEffectActor;

void spawnEndingCharacterVanishPoof(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void initEndingNancySpeedLines(EndingCreditsEffectActor *arg0);
void spawnEndingCharacterLoopingSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingTommyBigBurst(EndingCreditsEffectActor *arg0);
void initEndingSlashRisingStar(EndingCreditsEffectActor *arg0);
void initEndingNancyRunDust(EndingCreditsEffectActor *arg0);
void spawnEndingSmallBurst(s16 arg0, s16 arg1);
void spawnEndingCharacterAura(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void spawnEndingPhaseAdvanceSparkle(s16 arg0, s16 arg1);
void spawnEndingDelayedSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingTommySnowmanEntrance(EndingCreditsEffectActor *arg0);
void initEndingTransitionSnowboardIcon(EndingCreditsEffectActor *arg0);
void initEndingTransitionSnowflakeIcon(EndingCreditsEffectActor *arg0);
void initEndingTransitionLogoWipe(EndingCreditsEffectActor *arg0);

#endif
