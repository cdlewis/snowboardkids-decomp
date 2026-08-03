#ifndef RACE_SETUP_MENU_H
#define RACE_SETUP_MENU_H

#include "common.h"

typedef struct {
    /* 0x00 */ u8 state;
    /* 0x01 */ u8 timer;
    /* 0x02 */ s16 alpha;
    /* 0x04 */ u8 forceUpdate;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ u16 pendingStatusCodes[4];
    /* 0x0E */ u8 statusTransitionStates[4];
    /* 0x12 */ u16 nextStatusCodes[4];
    /* 0x1A */ u8 pad1A[0x1E];
} RaceSetupMenuSubState;

typedef struct RaceSetupMenuPackedData {
    u8 alignmentPadding;
    u8 choicePromptStates[12];
    u8 raceRecordChoicePromptStates[19];
} RaceSetupMenuPackedData;

extern RaceSetupMenuSubState gRaceSetupMenuSubState;
extern RaceSetupMenuPackedData gRaceSetupMenuPackedData;
extern u8 gRaceSetupSavePanelFrameState;

#define gControllerPakRaceRecordSaveStatusChoicePromptStates \
    gRaceSetupMenuPackedData.raceRecordChoicePromptStates

void initRaceSetupMenu(void);
void updateRaceSetupPlayerCountMenu(void);
void initRaceSetupSaveMenu(void);
void updateRaceSetupSaveMenu(void);
void raceSetupMenuNoop(void);
void updateRaceSetupRumblePrompt(void);
void initRaceSetupPlayerSaveData(s32 arg0);

#endif
