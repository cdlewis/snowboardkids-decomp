#ifndef MENU_TRANSITION_EFFECTS_H
#define MENU_TRANSITION_EFFECTS_H

#include "common.h"

typedef struct MainMenuEffectActor MainMenuEffectActor;

void spawnEndingCharacterVanishPoof(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void initEndingSpeedLines(MainMenuEffectActor *arg0);
void spawnEndingCharacterLoopingSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingBigBurst(MainMenuEffectActor *arg0);
void initEndingRisingStar(MainMenuEffectActor *arg0);
void initEndingRunDust(MainMenuEffectActor *arg0);
void spawnEndingSmallBurst(s16 arg0, s16 arg1);
void spawnEndingCharacterAura(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void spawnEndingPhaseAdvanceSparkle(s16 arg0, s16 arg1);
void spawnEndingDelayedSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingSnowmanEntranceEffect(MainMenuEffectActor *arg0);
void initRaceToMenuSnowboardIcon(MainMenuEffectActor *arg0);
void initRaceToMenuSnowflakeIcon(MainMenuEffectActor *arg0);
void initRaceToMenuLogoWipe(MainMenuEffectActor *arg0);

#endif
