#ifndef ENDING_CREDITS_SLASH_H
#define ENDING_CREDITS_SLASH_H

#include "common.h"

typedef struct EndingCreditsSlash EndingCreditsSlash;

void noopEndingCreditsSlash(void);
void updateEndingSlashFinalPose(EndingCreditsSlash *arg0);
void updateEndingSlashStartFinalPose(EndingCreditsSlash *arg0);
void waitEndingSlashPhase40(EndingCreditsSlash *arg0);
void updateEndingSlashPhase40Prep(EndingCreditsSlash *arg0);
void updateEndingSlashWaitPhase3FAnim(EndingCreditsSlash *arg0);
void updateEndingSlashSpinUntilPhase3F(EndingCreditsSlash *arg0);
void updateEndingSlashTumbleToPhase3C(EndingCreditsSlash *arg0);
void updateEndingSlashExitUntilPhase3B(EndingCreditsSlash *arg0);
void waitEndingSlashPhase3A(EndingCreditsSlash *arg0);
void updateEndingSlashSlowRotationWipe(EndingCreditsSlash *arg0);
void updateEndingSlashSlideToCenter(EndingCreditsSlash *arg0);
void waitEndingSlashPhase37(EndingCreditsSlash *arg0);
void updateEndingSlashSlideLeftFromFarRight(EndingCreditsSlash *arg0);
void waitEndingSlashPhase34(EndingCreditsSlash *arg0);
void updateEndingSlashDashOffLeft(EndingCreditsSlash *arg0);
void updateEndingSlashVanishRetreat(EndingCreditsSlash *arg0);
void waitEndingSlashPhase2F(void *arg0);
void updateEndingSlashDriftAfterPhase2D(EndingCreditsSlash *arg0);
void updateEndingSlashRunLeftThenPhase2B(EndingCreditsSlash *arg0);
void updateEndingSlashSlideFarLeft(EndingCreditsSlash *arg0);
void waitEndingSlashPhase2A(EndingCreditsSlash *arg0);
void updateEndingSlashSlideRightToPose(EndingCreditsSlash *arg0);
void waitEndingSlashPhase28(EndingCreditsSlash *arg0);
void updateEndingSlashSlideLeftToMarker(EndingCreditsSlash *arg0);
void updateEndingSlashSlideRightToMarker(EndingCreditsSlash *arg0);
void startEndingSlashPhase27Slide(void *arg0);
void updateEndingSlashSlideLeftSetPhase26(EndingCreditsSlash *arg0);
void noopEndingCreditsSlashAlt(void);
void updateEndingSlashDiagonalSlideSetPhase24(EndingCreditsSlash *arg0);
void waitEndingSlashBeforeDiagonalSlide(EndingCreditsSlash *arg0);
void updateEndingSlashSlideRightSetPhase23(EndingCreditsSlash *arg0);
void waitEndingSlashPhase22Aura(void *arg0);
void updateEndingSlashExitLeftAfterPhase22(EndingCreditsSlash *arg0);
void updateEndingSlashAfterVanishWait(EndingCreditsSlash *arg0);
void waitEndingSlashPhase1E(void *arg0);
void updateEndingSlashRepeatAnimThenVanish(EndingCreditsSlash *arg0);
void startEndingSlashRepeatAnim(EndingCreditsSlash *arg0);
void updateEndingSlashWaitBeforeRepeatAnim(EndingCreditsSlash *arg0);
void waitEndingSlashPhase1B(EndingCreditsSlash *arg0);
void waitEndingSlashPhase1A(void *arg0);
void updateEndingSlashHandshakeLoopThird(EndingCreditsSlash *arg0);
void updateEndingSlashHandshakeLoopSecond(EndingCreditsSlash *arg0);
void updateEndingSlashHandshakeLoopFirst(EndingCreditsSlash *arg0);
void startEndingSlashHandshakeLoop(EndingCreditsSlash *arg0);
void waitEndingSlashPhase19(void *arg0);
void updateEndingSlashSlideLeftUntilPhase18(EndingCreditsSlash *arg0);
void waitEndingSlashPhase17(EndingCreditsSlash *arg0);
void updateEndingSlashSlideRightToIdle(EndingCreditsSlash *arg0);
void startEndingSlashIdleAfterAnim(EndingCreditsSlash *arg0);
void updateEndingSlashWaitBeforeIdleAnim2(EndingCreditsSlash *arg0);
void updateEndingSlashWaitBeforeIdleAnim1(EndingCreditsSlash *arg0);
void updateEndingSlashHopLeftAnim(EndingCreditsSlash *arg0);
void updateEndingSlashSlideRightBurst(EndingCreditsSlash *arg0);
void waitEndingSlashRotationPhase14(EndingCreditsSlash *arg0);
void updateEndingSlashSlideLeftSlowRotation(EndingCreditsSlash *arg0);
void updateEndingSlashWaitRotationStepTen(EndingCreditsSlash *arg0);
void waitEndingSlashPhase12(EndingCreditsSlash *arg0);
void updateEndingSlashSlideRightSetPhase11(EndingCreditsSlash *arg0);
void waitEndingSlashPhase10Aura(EndingCreditsSlash *arg0);
void updateEndingSlashExitRightSetPhase0E(EndingCreditsSlash *arg0);
void updateEndingSlashVanishBeforeExitRight(EndingCreditsSlash *arg0);
void startEndingSlashVanishBeforeExitRight(EndingCreditsSlash *arg0);
void updateEndingSlashWaitRisingStar(EndingCreditsSlash *arg0);
void waitEndingSlashBeforeRisingStar(EndingCreditsSlash *arg0);
void updateEndingSlashRepeatAnim17(EndingCreditsSlash *arg0);
void startEndingSlashRepeatAnim17(EndingCreditsSlash *arg0);
void waitEndingSlashPhase0D(EndingCreditsSlash *arg0);
void updateEndingSlashAfterPhase0DAnim(EndingCreditsSlash *arg0);
void updateEndingSlashLongWaitSetShadow(EndingCreditsSlash *arg0);
void updateEndingSlashSlideLeftToAura(EndingCreditsSlash *arg0);
void updateEndingSlashWaitPhase5To7(EndingCreditsSlash *arg0);
void waitEndingSlashPhase5(EndingCreditsSlash *arg0);
void waitEndingSlashPhase3(EndingCreditsSlash *arg0);
void updateEndingSlashEnterFromRight(EndingCreditsSlash *arg0);
void waitEndingSlashPhase2(void *arg0);
void initEndingCreditsSlash(EndingCreditsSlash *arg0);

#endif
