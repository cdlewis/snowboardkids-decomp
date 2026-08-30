#ifndef ENDING_CREDITS_LINDA_H
#define ENDING_CREDITS_LINDA_H

#include "common.h"

typedef struct EndingCreditsLinda {
    char pad[0x18];
    s32 posX;
    s32 posY;
    s32 posZ;
    s16 rotX;
    s16 rotY;
    s16 rotZ;
    u16 timer;
    u16 animTimer;
    u16 textureId;
    u16 paletteId;
} EndingCreditsLinda;

typedef struct EndingCreditsTumblingSnowboard {
    char pad0[0x10];
    u16 mode;
    char pad12[0x6];
    Transform3D transform;
    u16 textureId;
    u16 paletteId;
    u16 timer;
} EndingCreditsTumblingSnowboard;

void updateEndingLindaFinalPose(EndingCreditsLinda *arg0);
void updateEndingLindaStartFinalPose(EndingCreditsLinda *arg0);
void updateEndingLindaPhase40Prep(EndingCreditsLinda *arg0);
void updateEndingLindaWaitBeforePhase40Prep(EndingCreditsLinda *arg0);
void updateEndingLindaWaitPhase3FAnim(EndingCreditsLinda *arg0);
void updateEndingLindaSpinUntilPhase3F(EndingCreditsLinda *arg0);
void updateEndingLindaTumbleToPhase3C(EndingCreditsLinda *arg0);
void updateEndingLindaExitUntilPhase3C(EndingCreditsLinda *arg0);
void waitEndingLindaPhase3A(EndingCreditsLinda *arg0);
void updateEndingLindaSlideToCenter(EndingCreditsLinda *arg0);
void waitEndingLindaPhase38(EndingCreditsLinda *arg0);
void updateEndingLindaSlideLeftSetPhase36(EndingCreditsLinda *arg0);
void waitEndingLindaPhase35(EndingCreditsLinda *arg0);
void updateEndingLindaSlideLeftFromFarRight(EndingCreditsLinda *arg0);
void waitEndingLindaPhase31(EndingCreditsLinda *arg0);
void updateEndingLindaSlideLeftSetPhase2E(EndingCreditsLinda *arg0);
void updateEndingLindaWaitBeforePhase2E(EndingCreditsLinda *arg0);
void updateEndingLindaRunLeftSetPhase2D(EndingCreditsLinda *arg0);
void waitEndingLindaPhase2C(EndingCreditsLinda *arg0);
void updateEndingLindaSlideRightSetPhase28(EndingCreditsLinda *arg0);
void updateEndingLindaWaitBeforeSlideRight(EndingCreditsLinda *arg0);
void updateEndingLindaSlideLeftToPose(EndingCreditsLinda *arg0);
void updateEndingLindaSlideRightAfterPause(EndingCreditsLinda *arg0);
void updateEndingLindaSlideLeftAfterPause(EndingCreditsLinda *arg0);
void updateEndingLindaSlideRightToPhase23Wait(EndingCreditsLinda *arg0);
void waitEndingLindaPhase23(EndingCreditsLinda *arg0);
void updateEndingLindaSlideRightSetPhase21(EndingCreditsLinda *arg0);
void updateEndingLindaSlideLeftToFarLeft(EndingCreditsLinda *arg0);
void updateEndingLindaRepeatAnimUntilPhase20(EndingCreditsLinda *arg0);
void waitEndingLindaPhase1D(EndingCreditsLinda *arg0);
void updateEndingLindaSlideLeftSetPhase1C(EndingCreditsLinda *arg0);
void updateEndingLindaAfterIntroAnim2(EndingCreditsLinda *arg0);
void updateEndingLindaAfterIntroAnim1(EndingCreditsLinda *arg0);
void updateEndingLindaWaitBeforePhase1B(EndingCreditsLinda *arg0);
void updateEndingLindaHopRightToIdle(EndingCreditsLinda *arg0);
void updateEndingLindaHopRightToPose(EndingCreditsLinda *arg0);
void updateEndingLindaSlideRightSetPhase1A(EndingCreditsLinda *arg0);
void updateEndingLindaHandshakeAnimComplete(EndingCreditsLinda *arg0);
void updateEndingLindaHandshakeLoop(EndingCreditsLinda *arg0);
void startEndingLindaHandshakeLoop(EndingCreditsLinda *arg0);
void waitEndingLindaPhase19(EndingCreditsLinda *arg0);
void updateEndingLindaWaitThenSetPhase18(EndingCreditsLinda *arg0);
void updateEndingLindaSlideLeftUntilPhase17(EndingCreditsLinda *arg0);
void updateEndingLindaBlinkThenSlideLeft(EndingCreditsLinda *arg0);
void updateEndingLindaStartBlinkLoop(EndingCreditsLinda *arg0);
void updateEndingLindaSlideRightUntilPhase16(EndingCreditsLinda *arg0);
void waitEndingLindaPhase15(EndingCreditsLinda *arg0);
void updateEndingLindaSlideLeftSetPhase14(EndingCreditsLinda *arg0);
void waitEndingLindaPhase13(EndingCreditsLinda *arg0);
void initEndingCreditsLinda(EndingCreditsLinda *arg0);
void spawnEndingCreditsTumblingSnowboard(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u8 arg5);
void drawEndingCreditsTumblingSnowboard(EndingCreditsTumblingSnowboard *arg0);
void updateEndingCreditsTumblingSnowboardWaitForRemove(EndingCreditsTumblingSnowboard *arg0);
void updateEndingCreditsTumblingSnowboardBounce(EndingCreditsTumblingSnowboard *arg0);
void waitEndingCreditsTumblingSnowboardPhase15(EndingCreditsTumblingSnowboard *arg0);
void updateEndingCreditsTumblingSnowboardSlideIn(EndingCreditsTumblingSnowboard *arg0);

#endif
