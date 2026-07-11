#ifndef ENDING_CREDITS_SHARED_EFFECTS_H
#define ENDING_CREDITS_SHARED_EFFECTS_H

#include "common.h"

typedef struct EndingCreditsSharedEffectActor EndingCreditsSharedEffectActor;

void spawnEndingCreditsCharacterVanishPoof(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void initEndingCreditsNancySpeedLines(EndingCreditsSharedEffectActor *arg0);
void spawnEndingCreditsCharacterLoopingSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingCreditsTommyBigBurst(EndingCreditsSharedEffectActor *arg0);
void initEndingCreditsSlashRisingStar(EndingCreditsSharedEffectActor *arg0);
void initEndingCreditsNancyRunDust(EndingCreditsSharedEffectActor *arg0);
void spawnEndingCreditsSmallBurst(s16 arg0, s16 arg1);
void spawnEndingCreditsCharacterAura(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void spawnEndingCreditsPhaseAdvanceSparkle(s16 arg0, s16 arg1);
void spawnEndingCreditsDelayedSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingCreditsTommySnowmanEntrance(EndingCreditsSharedEffectActor *arg0);
void initEndingCreditsTransitionSnowboardIcon(EndingCreditsSharedEffectActor *arg0);
void initEndingCreditsTransitionSnowflakeIcon(EndingCreditsSharedEffectActor *arg0);
void initEndingCreditsTransitionLogoWipe(EndingCreditsSharedEffectActor *arg0);

#endif
