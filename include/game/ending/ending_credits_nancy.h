#ifndef ENDING_CREDITS_NANCY_H
#define ENDING_CREDITS_NANCY_H

#include "common.h"

typedef struct EndingCreditsNancyActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Vec3s rot;
    /* 0x2A */ u16 timer;
} EndingCreditsNancyActor;

void updateEndingNancyFinalPose(EndingCreditsNancyActor *arg0);
void updateEndingNancyStartFinalPose(EndingCreditsNancyActor *arg0);
void waitEndingNancyPhase40(EndingCreditsNancyActor *arg0);
void updateEndingNancyPhase40Prep(EndingCreditsNancyActor *arg0);
void updateEndingNancyWaitBeforePhase40Prep(EndingCreditsNancyActor *arg0);
void updateEndingNancySpinUntilPhase3F(EndingCreditsNancyActor *arg0);
void updateEndingNancyTumbleToPhase3C(EndingCreditsNancyActor *arg0);
void updateEndingNancyExitUntilPhase3C(EndingCreditsNancyActor *arg0);
void waitEndingNancyPhase3A(EndingCreditsNancyActor *arg0);
void updateEndingNancySlideToCenter(EndingCreditsNancyActor *arg0);
void waitEndingNancyPhase38(EndingCreditsNancyActor *arg0);
void updateEndingNancySlideLeftSetPhase37(EndingCreditsNancyActor *arg0);
void waitEndingNancyPhase36(EndingCreditsNancyActor *arg0);
void updateEndingNancySlideLeftSetPhase33(EndingCreditsNancyActor *arg0);
void waitEndingNancyPhase32Aura(EndingCreditsNancyActor *arg0);
void waitEndingNancyPhase30Vanish(EndingCreditsNancyActor *arg0);
void updateEndingNancyDashRightSetPhase2F(EndingCreditsNancyActor *arg0);
void waitEndingNancyPhase2E(EndingCreditsNancyActor *arg0);
void updateEndingNancyWaitPhase2EAnim(EndingCreditsNancyActor *arg0);
void updateEndingNancyDriftAfterPhase2D(EndingCreditsNancyActor *arg0);
void waitEndingNancyPhase2B(EndingCreditsNancyActor *arg0);
void updateEndingNancyRunRightThenPhase2A(EndingCreditsNancyActor *arg0);
void waitEndingNancyPhase29(EndingCreditsNancyActor *arg0);
void updateEndingNancySlideFarLeftSetPhase12(EndingCreditsNancyActor *arg0);
void updateEndingNancyWaitBeforeSlideFarLeft(EndingCreditsNancyActor *arg0);
void updateEndingNancySlideRightToPose(EndingCreditsNancyActor *arg0);
void waitEndingNancyPhase11(EndingCreditsNancyActor *arg0);
void updateEndingNancyExitRightSetPhase0F(EndingCreditsNancyActor *arg0);
void waitEndingNancyPhase0E(EndingCreditsNancyActor *arg0);
void updateEndingNancyStartRunDust(EndingCreditsNancyActor *arg0);
void updateEndingNancyRepeatAnimThenVanish(EndingCreditsNancyActor *arg0);
void startEndingNancyRepeatAnim(EndingCreditsNancyActor *arg0);
void updateEndingNancyWaitBeforeRepeatAnim(EndingCreditsNancyActor *arg0);
void waitEndingNancyPhase0C(EndingCreditsNancyActor *arg0);
void updateEndingNancyEnterUntilPhase0B(EndingCreditsNancyActor *arg0);
void waitEndingNancyPhase9(EndingCreditsNancyActor *arg0);
void initEndingCreditsNancy(EndingCreditsNancyActor *arg0);

#endif
