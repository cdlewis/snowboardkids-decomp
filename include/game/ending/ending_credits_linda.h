#ifndef ENDING_CREDITS_LINDA_H
#define ENDING_CREDITS_LINDA_H

#include "game/ending/ending_credits_character.h"

typedef struct EndingCreditsLindaActor {
    /* 0x00 */ EndingCreditsCharacterActor character;
    /* 0x2C */ u16 animationTimer;
    /* 0x2E */ u16 textureId;
    /* 0x30 */ u16 paletteId;
} EndingCreditsLindaActor;

typedef struct EndingCreditsTumblingSnowboard {
    char pad0[0x10];
    u16 mode;
    char pad12[0x6];
    Transform3D transform;
    u16 textureId;
    u16 paletteId;
    u16 timer;
} EndingCreditsTumblingSnowboard;

void updateEndingLindaFinalPose(EndingCreditsLindaActor *arg0);
void updateEndingLindaStartFinalPose(EndingCreditsLindaActor *arg0);
void updateEndingLindaPhase40Prep(EndingCreditsLindaActor *arg0);
void updateEndingLindaWaitBeforePhase40Prep(EndingCreditsLindaActor *arg0);
void updateEndingLindaWaitPhase3FAnim(EndingCreditsLindaActor *arg0);
void updateEndingLindaSpinUntilPhase3F(EndingCreditsLindaActor *arg0);
void updateEndingLindaTumbleToPhase3C(EndingCreditsLindaActor *arg0);
void updateEndingLindaExitUntilPhase3C(EndingCreditsLindaActor *arg0);
void waitEndingLindaPhase3A(EndingCreditsLindaActor *arg0);
void updateEndingLindaSlideToCenter(EndingCreditsLindaActor *arg0);
void waitEndingLindaPhase38(EndingCreditsLindaActor *arg0);
void updateEndingLindaSlideLeftSetPhase36(EndingCreditsLindaActor *arg0);
void waitEndingLindaPhase35(EndingCreditsLindaActor *arg0);
void updateEndingLindaSlideLeftFromFarRight(EndingCreditsLindaActor *arg0);
void waitEndingLindaPhase31(EndingCreditsLindaActor *arg0);
void updateEndingLindaSlideLeftSetPhase2E(EndingCreditsLindaActor *arg0);
void updateEndingLindaWaitBeforePhase2E(EndingCreditsLindaActor *arg0);
void updateEndingLindaRunLeftSetPhase2D(EndingCreditsLindaActor *arg0);
void waitEndingLindaPhase2C(EndingCreditsLindaActor *arg0);
void updateEndingLindaSlideRightSetPhase28(EndingCreditsLindaActor *arg0);
void updateEndingLindaWaitBeforeSlideRight(EndingCreditsLindaActor *arg0);
void updateEndingLindaSlideLeftToPose(EndingCreditsLindaActor *arg0);
void updateEndingLindaSlideRightAfterPause(EndingCreditsLindaActor *arg0);
void updateEndingLindaSlideLeftAfterPause(EndingCreditsLindaActor *arg0);
void updateEndingLindaSlideRightToPhase23Wait(EndingCreditsLindaActor *arg0);
void waitEndingLindaPhase23(EndingCreditsLindaActor *arg0);
void updateEndingLindaSlideRightSetPhase21(EndingCreditsLindaActor *arg0);
void updateEndingLindaSlideLeftToFarLeft(EndingCreditsLindaActor *arg0);
void updateEndingLindaRepeatAnimUntilPhase20(EndingCreditsLindaActor *arg0);
void waitEndingLindaPhase1D(EndingCreditsLindaActor *arg0);
void updateEndingLindaSlideLeftSetPhase1C(EndingCreditsLindaActor *arg0);
void updateEndingLindaAfterIntroAnim2(EndingCreditsLindaActor *arg0);
void updateEndingLindaAfterIntroAnim1(EndingCreditsLindaActor *arg0);
void updateEndingLindaWaitBeforePhase1B(EndingCreditsLindaActor *arg0);
void updateEndingLindaHopRightToIdle(EndingCreditsLindaActor *arg0);
void updateEndingLindaHopRightToPose(EndingCreditsLindaActor *arg0);
void updateEndingLindaSlideRightSetPhase1A(EndingCreditsLindaActor *arg0);
void updateEndingLindaHandshakeAnimComplete(EndingCreditsLindaActor *arg0);
void updateEndingLindaHandshakeLoop(EndingCreditsLindaActor *arg0);
void startEndingLindaHandshakeLoop(EndingCreditsLindaActor *arg0);
void waitEndingLindaPhase19(EndingCreditsLindaActor *arg0);
void updateEndingLindaWaitThenSetPhase18(EndingCreditsLindaActor *arg0);
void updateEndingLindaSlideLeftUntilPhase17(EndingCreditsLindaActor *arg0);
void updateEndingLindaBlinkThenSlideLeft(EndingCreditsLindaActor *arg0);
void updateEndingLindaStartBlinkLoop(EndingCreditsLindaActor *arg0);
void updateEndingLindaSlideRightUntilPhase16(EndingCreditsLindaActor *arg0);
void waitEndingLindaPhase15(EndingCreditsLindaActor *arg0);
void updateEndingLindaSlideLeftSetPhase14(EndingCreditsLindaActor *arg0);
void waitEndingLindaPhase13(EndingCreditsLindaActor *arg0);
void initEndingCreditsLinda(EndingCreditsLindaActor *arg0);
void spawnEndingCreditsTumblingSnowboard(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u8 arg5);
void drawEndingCreditsTumblingSnowboard(EndingCreditsTumblingSnowboard *arg0);
void updateEndingCreditsTumblingSnowboardWaitForRemove(EndingCreditsTumblingSnowboard *arg0);
void updateEndingCreditsTumblingSnowboardBounce(EndingCreditsTumblingSnowboard *arg0);
void waitEndingCreditsTumblingSnowboardPhase15(EndingCreditsTumblingSnowboard *arg0);
void updateEndingCreditsTumblingSnowboardSlideIn(EndingCreditsTumblingSnowboard *arg0);

#endif
