#ifndef ENDING_CREDITS_JAM_H
#define ENDING_CREDITS_JAM_H

#include "common.h"

typedef struct EndingCreditsJam {
    /* 0x00 */ char pad0[0x18];
    /* 0x18 */ s32 posX;
    /* 0x1C */ s32 posY;
    /* 0x20 */ s32 posZ;
    /* 0x24 */ s16 rotX;
    /* 0x26 */ s16 rotY;
    /* 0x28 */ s16 rotZ;
    /* 0x2A */ u16 timer;
} EndingCreditsJam;

void updateEndingJamFinalPose(EndingCreditsJam *arg0);
void updateEndingJamStartFinalPose(EndingCreditsJam *arg0);
void waitEndingJamPhase40(EndingCreditsJam *arg0);
void updateEndingJamWaitBeforePhase40(EndingCreditsJam *arg0);
void updateEndingJamPhase3FAnim3(EndingCreditsJam *arg0);
void updateEndingJamPhase3FAnim2(EndingCreditsJam *arg0);
void updateEndingJamPhase3FAnim1(EndingCreditsJam *arg0);
void waitEndingJamPhase3E(EndingCreditsJam *arg0);
void updateEndingJamSetPhase3D(EndingCreditsJam *arg0);
void updateEndingJamPhase3DPrep(EndingCreditsJam *arg0);
void updateEndingJamWaitBeforePhase3D(EndingCreditsJam *arg0);
void updateEndingJamWaitBeforeIdleAnim2(EndingCreditsJam *arg0);
void updateEndingJamWaitBeforeIdleAnim1(EndingCreditsJam *arg0);
void updateEndingJamHopRightToIdle(EndingCreditsJam *arg0);
void updateEndingJamSlideLeftSetPhase3B(EndingCreditsJam *arg0);
void waitEndingJamPhase3A(EndingCreditsJam *arg0);
void updateEndingJamSlideRightToCenter(EndingCreditsJam *arg0);
void waitEndingJamPhase38(EndingCreditsJam *arg0);
void updateEndingJamSlideLeftFromFarRight(EndingCreditsJam *arg0);
void waitEndingJamPhase33Aura(EndingCreditsJam *arg0);
void updateEndingJamDashOffLeftSetPhase31(EndingCreditsJam *arg0);
void updateEndingJamVanishRetreat(EndingCreditsJam *arg0);
void waitEndingJamPhase2F(EndingCreditsJam *arg0);
void updateEndingJamDriftAfterPhase2D(EndingCreditsJam *arg0);
void updateEndingJamWaitThenSetPhase2C(EndingCreditsJam *arg0);
void updateEndingJamRunLeftThenPhase2C(EndingCreditsJam *arg0);
void updateEndingJamSlideFarLeft(EndingCreditsJam *arg0);
void waitEndingJamPhase2A(EndingCreditsJam *arg0);
void updateEndingJamSlideRightToPose(EndingCreditsJam *arg0);
void waitEndingJamPhase28(EndingCreditsJam *arg0);
void updateEndingJamSlideRightToMarker(EndingCreditsJam *arg0);
void waitEndingJamBeforeMarkerSlide(EndingCreditsJam *arg0);
void updateEndingJamSlideLeftToMarker(EndingCreditsJam *arg0);
void updateEndingJamWaitThenSetPhase27(EndingCreditsJam *arg0);
void updateEndingJamSlideRightToPhase27Start(EndingCreditsJam *arg0);
void startEndingJamPhase26Slide(EndingCreditsJam *arg0);
void updateEndingJamSlideLeftSetPhase25(EndingCreditsJam *arg0);
void waitEndingJamBeforeDiagonalSlide(EndingCreditsJam *arg0);
void updateEndingJamDiagonalSlideSetPhase22(EndingCreditsJam *arg0);
void waitEndingJamPhase21Aura(EndingCreditsJam *arg0);
void updateEndingJamExitLeftAfterPhase21(EndingCreditsJam *arg0);
void updateEndingJamAfterVanishWait(EndingCreditsJam *arg0);
void updateEndingJamWaitBeforeVanishReturn(EndingCreditsJam *arg0);
void startEndingJamVanishReturn(EndingCreditsJam *arg0);
void updateEndingJamSlideLeftThenVanish(EndingCreditsJam *arg0);
void waitEndingJamPhase1E(EndingCreditsJam *arg0);
void updateEndingJamExitLeftSetPhase1D(EndingCreditsJam *arg0);
void updateEndingJamRepeatAnimThenVanish(EndingCreditsJam *arg0);
void waitEndingJamPhase1C(EndingCreditsJam *arg0);
void updateEndingJamWaitBeforePhase1C(EndingCreditsJam *arg0);
void updateEndingJamStepBackAfterHandshake(EndingCreditsJam *arg0);
void waitEndingJamHandshakeExit(EndingCreditsJam *arg0);
void updateEndingJamHandshakeLoopThird(EndingCreditsJam *arg0);
void updateEndingJamHandshakeLoopSecond(EndingCreditsJam *arg0);
void updateEndingJamHandshakeLoopFirst(EndingCreditsJam *arg0);
void startEndingJamHandshakeLoop(EndingCreditsJam *arg0);
void updateEndingJamSlideRightUntilPhase18(EndingCreditsJam *arg0);
void waitEndingJamPhase17(EndingCreditsJam *arg0);
void updateEndingJamSlideLeftToIdle(EndingCreditsJam *arg0);
void startEndingJamIdleAfterAnim(EndingCreditsJam *arg0);
void updateEndingJamWaitBeforeIdleAnimB(EndingCreditsJam *arg0);
void updateEndingJamWaitBeforeIdleAnimA(EndingCreditsJam *arg0);
void updateEndingJamHopRightToPose(EndingCreditsJam *arg0);
void waitEndingJamPhase15(EndingCreditsJam *arg0);
void updateEndingJamSlideLeftToPhase15Wait(EndingCreditsJam *arg0);
void waitEndingJamPhase13(EndingCreditsJam *arg0);
void initEndingCreditsJam(EndingCreditsJam *arg0);

#endif
