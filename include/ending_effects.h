#ifndef ENDING_EFFECTS_H
#define ENDING_EFFECTS_H

#include "common.h"

typedef struct EndingEffectActor EndingEffectActor;

void spawnEndingCharacterVanishPoof(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void initEndingNancySpeedLines(EndingEffectActor *arg0);
void spawnEndingCharacterLoopingSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingTommyBigBurst(EndingEffectActor *arg0);
void initEndingSlashRisingStar(EndingEffectActor *arg0);
void initEndingNancyRunDust(EndingEffectActor *arg0);
void spawnEndingSmallBurst(s16 arg0, s16 arg1);
void spawnEndingCharacterAura(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void spawnEndingPhaseAdvanceSparkle(s16 arg0, s16 arg1);
void spawnEndingDelayedSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingTommySnowmanEntrance(EndingEffectActor *arg0);
void initRaceToMainMenuSnowboardIcon(EndingEffectActor *arg0);
void initRaceToMainMenuSnowflakeIcon(EndingEffectActor *arg0);
void initRaceToMainMenuLogoWipe(EndingEffectActor *arg0);

#endif
