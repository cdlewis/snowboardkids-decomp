#ifndef ENDING_CREDITS_NANCY_H
#define ENDING_CREDITS_NANCY_H

#include "common.h"

typedef struct EndingCreditsNancy EndingCreditsNancy;

void updateEndingNancyFinalPose(EndingCreditsNancy *arg0);
void updateEndingNancyStartFinalPose(EndingCreditsNancy *arg0);
void waitEndingNancyPhase40(EndingCreditsNancy *arg0);
void updateEndingNancyPhase40Prep(EndingCreditsNancy *arg0);
void updateEndingNancyWaitBeforePhase40Prep(EndingCreditsNancy *arg0);
void updateEndingNancySpinUntilPhase3F(EndingCreditsNancy *arg0);
void updateEndingNancyTumbleToPhase3C(EndingCreditsNancy *arg0);
void updateEndingNancyExitUntilPhase3C(EndingCreditsNancy *arg0);
void waitEndingNancyPhase3A(EndingCreditsNancy *arg0);
void updateEndingNancySlideToCenter(EndingCreditsNancy *arg0);
void waitEndingNancyPhase38(EndingCreditsNancy *arg0);
void updateEndingNancySlideLeftSetPhase37(EndingCreditsNancy *arg0);
void waitEndingNancyPhase36(EndingCreditsNancy *arg0);
void updateEndingNancySlideLeftSetPhase33(EndingCreditsNancy *arg0);
void waitEndingNancyPhase32Aura(EndingCreditsNancy *arg0);
void waitEndingNancyPhase30Vanish(EndingCreditsNancy *arg0);
void updateEndingNancyDashRightSetPhase2F(EndingCreditsNancy *arg0);
void waitEndingNancyPhase2E(EndingCreditsNancy *arg0);
void updateEndingNancyWaitPhase2EAnim(EndingCreditsNancy *arg0);
void updateEndingNancyDriftAfterPhase2D(EndingCreditsNancy *arg0);
void waitEndingNancyPhase2B(EndingCreditsNancy *arg0);
void updateEndingNancyRunRightThenPhase2A(EndingCreditsNancy *arg0);
void waitEndingNancyPhase29(EndingCreditsNancy *arg0);
void updateEndingNancySlideFarLeftSetPhase12(EndingCreditsNancy *arg0);
void updateEndingNancyWaitBeforeSlideFarLeft(EndingCreditsNancy *arg0);
void updateEndingNancySlideRightToPose(EndingCreditsNancy *arg0);
void waitEndingNancyPhase11(EndingCreditsNancy *arg0);
void updateEndingNancyExitRightSetPhase0F(EndingCreditsNancy *arg0);
void waitEndingNancyPhase0E(EndingCreditsNancy *arg0);
void updateEndingNancyStartRunDust(EndingCreditsNancy *arg0);
void updateEndingNancyRepeatAnimThenVanish(EndingCreditsNancy *arg0);
void startEndingNancyRepeatAnim(EndingCreditsNancy *arg0);
void updateEndingNancyWaitBeforeRepeatAnim(EndingCreditsNancy *arg0);
void waitEndingNancyPhase0C(EndingCreditsNancy *arg0);
void updateEndingNancyEnterUntilPhase0B(EndingCreditsNancy *arg0);
void waitEndingNancyPhase9(EndingCreditsNancy *arg0);
void initEndingCreditsNancy(EndingCreditsNancy *arg0);

#endif
