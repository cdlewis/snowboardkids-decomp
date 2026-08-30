#ifndef ENDING_CREDITS_SLASH_H
#define ENDING_CREDITS_SLASH_H

#include "game/ending/ending_credits_character.h"

typedef struct EndingCreditsSlashActor {
    /* 0x00 */ EndingCreditsCharacterActor character;
    /* 0x2C */ u16 phase0DAnimationId;
} EndingCreditsSlashActor;

void noopEndingCreditsSlash(void);
void updateEndingSlashFinalPose(EndingCreditsSlashActor *arg0);
void updateEndingSlashStartFinalPose(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase40(EndingCreditsSlashActor *arg0);
void updateEndingSlashPhase40Prep(EndingCreditsSlashActor *arg0);
void updateEndingSlashWaitPhase3FAnim(EndingCreditsSlashActor *arg0);
void updateEndingSlashSpinUntilPhase3F(EndingCreditsSlashActor *arg0);
void updateEndingSlashTumbleToPhase3C(EndingCreditsSlashActor *arg0);
void updateEndingSlashExitUntilPhase3B(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase3A(EndingCreditsSlashActor *arg0);
void updateEndingSlashSlowRotationWipe(EndingCreditsSlashActor *arg0);
void updateEndingSlashSlideToCenter(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase37(EndingCreditsSlashActor *arg0);
void updateEndingSlashSlideLeftFromFarRight(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase34(EndingCreditsSlashActor *arg0);
void updateEndingSlashDashOffLeft(EndingCreditsSlashActor *arg0);
void updateEndingSlashVanishRetreat(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase2F(EndingCreditsSlashActor *arg0);
void updateEndingSlashDriftAfterPhase2D(EndingCreditsSlashActor *arg0);
void updateEndingSlashRunLeftThenPhase2B(EndingCreditsSlashActor *arg0);
void updateEndingSlashSlideFarLeft(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase2A(EndingCreditsSlashActor *arg0);
void updateEndingSlashSlideRightToPose(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase28(EndingCreditsSlashActor *arg0);
void updateEndingSlashSlideLeftToMarker(EndingCreditsSlashActor *arg0);
void updateEndingSlashSlideRightToMarker(EndingCreditsSlashActor *arg0);
void startEndingSlashPhase27Slide(EndingCreditsSlashActor *arg0);
void updateEndingSlashSlideLeftSetPhase26(EndingCreditsSlashActor *arg0);
void noopEndingCreditsSlashAlt(void);
void updateEndingSlashDiagonalSlideSetPhase24(EndingCreditsSlashActor *arg0);
void waitEndingSlashBeforeDiagonalSlide(EndingCreditsSlashActor *arg0);
void updateEndingSlashSlideRightSetPhase23(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase22Aura(EndingCreditsSlashActor *arg0);
void updateEndingSlashExitLeftAfterPhase22(EndingCreditsSlashActor *arg0);
void updateEndingSlashAfterVanishWait(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase1E(EndingCreditsSlashActor *arg0);
void updateEndingSlashRepeatAnimThenVanish(EndingCreditsSlashActor *arg0);
void startEndingSlashRepeatAnim(EndingCreditsSlashActor *arg0);
void updateEndingSlashWaitBeforeRepeatAnim(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase1B(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase1A(EndingCreditsSlashActor *arg0);
void updateEndingSlashHandshakeLoopThird(EndingCreditsSlashActor *arg0);
void updateEndingSlashHandshakeLoopSecond(EndingCreditsSlashActor *arg0);
void updateEndingSlashHandshakeLoopFirst(EndingCreditsSlashActor *arg0);
void startEndingSlashHandshakeLoop(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase19(EndingCreditsSlashActor *arg0);
void updateEndingSlashSlideLeftUntilPhase18(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase17(EndingCreditsSlashActor *arg0);
void updateEndingSlashSlideRightToIdle(EndingCreditsSlashActor *arg0);
void startEndingSlashIdleAfterAnim(EndingCreditsSlashActor *arg0);
void updateEndingSlashWaitBeforeIdleAnim2(EndingCreditsSlashActor *arg0);
void updateEndingSlashWaitBeforeIdleAnim1(EndingCreditsSlashActor *arg0);
void updateEndingSlashHopLeftAnim(EndingCreditsSlashActor *arg0);
void updateEndingSlashSlideRightBurst(EndingCreditsSlashActor *arg0);
void waitEndingSlashRotationPhase14(EndingCreditsSlashActor *arg0);
void updateEndingSlashSlideLeftSlowRotation(EndingCreditsSlashActor *arg0);
void updateEndingSlashWaitRotationStepTen(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase12(EndingCreditsSlashActor *arg0);
void updateEndingSlashSlideRightSetPhase11(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase10Aura(EndingCreditsSlashActor *arg0);
void updateEndingSlashExitRightSetPhase0E(EndingCreditsSlashActor *arg0);
void updateEndingSlashVanishBeforeExitRight(EndingCreditsSlashActor *arg0);
void startEndingSlashVanishBeforeExitRight(EndingCreditsSlashActor *arg0);
void updateEndingSlashWaitRisingStar(EndingCreditsSlashActor *arg0);
void waitEndingSlashBeforeRisingStar(EndingCreditsSlashActor *arg0);
void updateEndingSlashRepeatAnim17(EndingCreditsSlashActor *arg0);
void startEndingSlashRepeatAnim17(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase0D(EndingCreditsSlashActor *arg0);
void updateEndingSlashAfterPhase0DAnim(EndingCreditsSlashActor *arg0);
void updateEndingSlashLongWaitSetShadow(EndingCreditsSlashActor *arg0);
void updateEndingSlashSlideLeftToAura(EndingCreditsSlashActor *arg0);
void updateEndingSlashWaitPhase5To7(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase5(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase3(EndingCreditsSlashActor *arg0);
void updateEndingSlashEnterFromRight(EndingCreditsSlashActor *arg0);
void waitEndingSlashPhase2(EndingCreditsSlashActor *arg0);
void initEndingCreditsSlash(EndingCreditsSlashActor *arg0);

#endif
