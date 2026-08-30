#ifndef ENDING_CREDITS_TOMMY_H
#define ENDING_CREDITS_TOMMY_H

#include "game/ending/ending_credits_character.h"

typedef struct {
    /* 0x00 */ Vec3i offset;
    /* 0x0C */ s8 modelPartIndex;
    /* 0x0D */ s8 characterId;
} MainMenuSceneActorShadow;

void noopEndingCreditsTommy(void);
void updateEndingTommyFinalPose(EndingCreditsCharacterActor *arg0);
void updateEndingTommyStartFinalPose(EndingCreditsCharacterActor *arg0);
void updateEndingTommyWaitThenFinalPhase(EndingCreditsCharacterActor *arg0);
void waitEndingTommyPhase3D(EndingCreditsCharacterActor *arg0);
void updateEndingTommyEnterForPhase3A(EndingCreditsCharacterActor *arg0);
void waitEndingTommyPhase39(EndingCreditsCharacterActor *arg0);
void updateEndingTommySlideLeftAfterBurst(EndingCreditsCharacterActor *arg0);
void updateEndingTommyStartBurstExit(EndingCreditsCharacterActor *arg0);
void updateEndingTommyWaitBeforeBurstExit(EndingCreditsCharacterActor *arg0);
void updateEndingTommyWaitBeforeBurst(EndingCreditsCharacterActor *arg0);
void waitEndingTommyPhase0F(EndingCreditsCharacterActor *arg0);
void updateEndingTommyStartPhase0CAuras(EndingCreditsCharacterActor *arg0);
void updateEndingTommyWaitBeforePhase0CAuras(EndingCreditsCharacterActor *arg0);
void updateEndingTommySlideLeftToPhase0A(EndingCreditsCharacterActor *arg0);
void updateEndingTommyHopLeftToPhase0A(EndingCreditsCharacterActor *arg0);
void waitEndingTommyPhase0B(EndingCreditsCharacterActor *arg0);
void waitEndingTommyPhase08Aura(EndingCreditsCharacterActor *arg0);
void updateEndingTommyRepeatAnimThenPhase07(EndingCreditsCharacterActor *arg0);
void updateEndingTommyWaitForPhase06(EndingCreditsCharacterActor *arg0);
void waitEndingTommyPhase04(EndingCreditsCharacterActor *arg0);
void updateEndingTommyEnterToCenter(EndingCreditsCharacterActor *arg0);
void waitEndingTommyPhase01(EndingCreditsCharacterActor *arg0);
void initEndingCreditsTommy(EndingCreditsCharacterActor *arg0);
void addEndingActorShadowRenderCallback(MainMenuSceneActorShadow *arg0);

#endif
