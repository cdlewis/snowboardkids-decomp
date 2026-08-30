#ifndef ENDING_CREDITS_TOMMY_H
#define ENDING_CREDITS_TOMMY_H

#include "common.h"

typedef struct EndingCreditsTommy {
    /* 0x00 */ char pad0[0x18];
    /* 0x18 */ s32 posX;
    /* 0x1C */ s32 posY;
    /* 0x20 */ s32 posZ;
    /* 0x24 */ s16 rotX;
    /* 0x26 */ s16 rotY;
    /* 0x28 */ s16 rotZ;
    /* 0x2A */ u16 timer;
} EndingCreditsTommy; // size = 0x2C

typedef struct {
    /* 0x00 */ s32 posX;
    /* 0x04 */ s32 posY;
    /* 0x08 */ s32 posZ;
    /* 0x0C */ s8 unkC;
    /* 0x0D */ s8 actorId;
} MainMenuSceneActorShadow;

void noopEndingCreditsTommy(void);
void updateEndingTommyFinalPose(EndingCreditsTommy *arg0);
void updateEndingTommyStartFinalPose(EndingCreditsTommy *arg0);
void updateEndingTommyWaitThenFinalPhase(EndingCreditsTommy *arg0);
void waitEndingTommyPhase3D(EndingCreditsTommy *arg0);
void updateEndingTommyEnterForPhase3A(EndingCreditsTommy *arg0);
void waitEndingTommyPhase39(EndingCreditsTommy *arg0);
void updateEndingTommySlideLeftAfterBurst(EndingCreditsTommy *arg0);
void updateEndingTommyStartBurstExit(EndingCreditsTommy *arg0);
void updateEndingTommyWaitBeforeBurstExit(EndingCreditsTommy *arg0);
void updateEndingTommyWaitBeforeBurst(EndingCreditsTommy *arg0);
void waitEndingTommyPhase0F(EndingCreditsTommy *arg0);
void updateEndingTommyStartPhase0CAuras(EndingCreditsTommy *arg0);
void updateEndingTommyWaitBeforePhase0CAuras(EndingCreditsTommy *arg0);
void updateEndingTommySlideLeftToPhase0A(EndingCreditsTommy *arg0);
void updateEndingTommyHopLeftToPhase0A(EndingCreditsTommy *arg0);
void waitEndingTommyPhase0B(EndingCreditsTommy *arg0);
void waitEndingTommyPhase08Aura(EndingCreditsTommy *arg0);
void updateEndingTommyRepeatAnimThenPhase07(EndingCreditsTommy *arg0);
void updateEndingTommyWaitForPhase06(EndingCreditsTommy *arg0);
void waitEndingTommyPhase04(EndingCreditsTommy *arg0);
void updateEndingTommyEnterToCenter(EndingCreditsTommy *arg0);
void waitEndingTommyPhase01(EndingCreditsTommy *arg0);
void initEndingCreditsTommy(EndingCreditsTommy *arg0);
void addEndingActorShadowRenderCallback(MainMenuSceneActorShadow *arg0);

#endif
