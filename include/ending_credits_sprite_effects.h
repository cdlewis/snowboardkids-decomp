#ifndef ENDING_CREDITS_SPRITE_EFFECTS_H
#define ENDING_CREDITS_SPRITE_EFFECTS_H

#include "common.h"

typedef struct EndingCreditsSpriteEffectActor EndingCreditsSpriteEffectActor;

void spawnEndingCreditsCharacterVanishPoof(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void initEndingCreditsNancySpeedLines(EndingCreditsSpriteEffectActor *arg0);
void spawnEndingCreditsCharacterLoopingSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingCreditsTommyBigBurst(EndingCreditsSpriteEffectActor *arg0);
void initEndingCreditsSlashRisingStar(EndingCreditsSpriteEffectActor *arg0);
void initEndingCreditsNancyRunDust(EndingCreditsSpriteEffectActor *arg0);
void spawnEndingCreditsSmallBurst(s16 arg0, s16 arg1);
void spawnEndingCreditsCharacterAura(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void spawnEndingCreditsPhaseAdvanceSparkle(s16 arg0, s16 arg1);
void spawnEndingCreditsDelayedSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingCreditsTommySnowmanEntrance(EndingCreditsSpriteEffectActor *arg0);
void initEndingCreditsTransitionSnowboardIcon(EndingCreditsSpriteEffectActor *arg0);
void initEndingCreditsTransitionSnowflakeIcon(EndingCreditsSpriteEffectActor *arg0);
void initEndingCreditsTransitionLogoWipe(EndingCreditsSpriteEffectActor *arg0);

#endif
