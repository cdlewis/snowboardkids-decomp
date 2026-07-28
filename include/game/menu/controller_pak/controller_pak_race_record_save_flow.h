#ifndef CONTROLLER_PAK_RACE_RECORD_SAVE_FLOW_H
#define CONTROLLER_PAK_RACE_RECORD_SAVE_FLOW_H

#include "common.h"

typedef struct ControllerPakRaceRecordSaveStatusTransition {
    /* 0x0 */ u8 step;
    /* 0x1 */ u8 pad1;
    /* 0x2 */ s16 alpha;
    /* 0x4 */ u16 targetStatus;
    /* 0x6 */ u16 nextStatus;
} ControllerPakRaceRecordSaveStatusTransition;

extern ControllerPakRaceRecordSaveStatusTransition gControllerPakRaceRecordSaveStatusTransition;

void initControllerPakRaceRecordSaveFlow(void);
void updateControllerPakRaceRecordSaveFlow(void);
void fadeOutControllerPakRaceRecordSaveFlow(void);
void updateControllerPakRaceRecordSaveOverwritePrompt(void);

#endif
