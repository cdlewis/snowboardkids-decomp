#ifndef ENDING_CREDITS_EFFECTS_H
#define ENDING_CREDITS_EFFECTS_H

#include "common.h"

typedef struct EndingCreditsEffectActor EndingCreditsEffectActor;

enum EndingCreditsCharacterId {
    ENDING_CREDITS_CHARACTER_SLASH,
    ENDING_CREDITS_CHARACTER_NANCY,
    ENDING_CREDITS_CHARACTER_JAM,
    ENDING_CREDITS_CHARACTER_LINDA,
    ENDING_CREDITS_CHARACTER_TOMMY,
    ENDING_CREDITS_CHARACTER_COUNT
};

extern u8 gEndingCreditsCharacterAuraDoneFlags[];

void spawnEndingCreditsCharacterVanishPoof(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void initEndingCreditsNancySpeedLines(EndingCreditsEffectActor *arg0);
void spawnEndingCreditsCharacterLoopingSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingCreditsTommyBigBurst(EndingCreditsEffectActor *arg0);
void initEndingCreditsSlashRisingStar(EndingCreditsEffectActor *arg0);
void initEndingCreditsNancyRunDust(EndingCreditsEffectActor *arg0);
void spawnEndingCreditsSmallBurst(s16 arg0, s16 arg1);
void spawnEndingCreditsCharacterAura(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void spawnEndingCreditsPhaseAdvanceSparkle(s16 arg0, s16 arg1);
void spawnEndingCreditsDelayedSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingCreditsTommySnowmanEntrance(EndingCreditsEffectActor *arg0);
void initEndingCreditsTransitionSnowboardIcon(EndingCreditsEffectActor *arg0);
void initEndingCreditsTransitionSnowflakeIcon(EndingCreditsEffectActor *arg0);
void initEndingCreditsTransitionLogoWipe(EndingCreditsEffectActor *arg0);

#endif
