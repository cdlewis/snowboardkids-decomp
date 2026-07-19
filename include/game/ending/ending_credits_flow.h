#ifndef ENDING_CREDITS_FLOW_H
#define ENDING_CREDITS_FLOW_H

#include "common.h"

extern s16 gEndingCreditsUnusedValue;
extern u16 gEndingCreditsSequencePhase;
extern u8 gEndingCreditsHandshakeState;
extern u8 gEndingCreditsTransitionSnowboardIconSpinStep;
extern s16 gEndingCreditsTransitionSnowboardIconAngle;

void initEndingCreditsFlow(void);
void fadeInEndingCreditsFlow(void);
void updateEndingCreditsFlow(void);
void fadeOutEndingCreditsFlow(void);

#endif
