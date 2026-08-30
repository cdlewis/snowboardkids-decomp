#ifndef CONTROLLER_PAK_RACE_RECORD_SAVE_UI_H
#define CONTROLLER_PAK_RACE_RECORD_SAVE_UI_H

#include "common.h"
#include "game/engine/callback_task_scheduler.h"

typedef struct {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ u8 state;
    /* 0x1D */ u8 pad1D[0xB];
} ControllerPakRaceRecordSaveScorePanelActor;

typedef struct {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 scale;
    /* 0x1E */ u8 pad1E;
    /* 0x1F */ u8 state;
    /* 0x20 */ u8 pad20[8];
} ControllerPakRaceRecordSavePromptFrameActor;

typedef struct {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 alpha;
    /* 0x1E */ u16 targetStatus;
    /* 0x20 */ u8 state;
    /* 0x21 */ u8 arrowFrame;
    /* 0x22 */ u8 pad22[6];
} ControllerPakRaceRecordSaveStatusMessageActor;

typedef struct {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 topChoiceY;
    /* 0x1C */ s16 bottomChoiceY;
    /* 0x1E */ s16 slideOffset;
    /* 0x20 */ u16 nextStatus;
    /* 0x22 */ u16 cursorAlpha;
    /* 0x24 */ u16 cursorAlphaTimer;
    /* 0x26 */ u8 pad26[2];
} ControllerPakRaceRecordSaveStatusChoicePromptActor;

typedef struct {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 alpha;
    /* 0x1E */ u8 state;
    /* 0x1F */ u8 pad1F[9];
} ControllerPakRaceRecordSaveExitMessageActor;

void drawControllerPakRaceRecordSaveScorePanel(ControllerPakRaceRecordSaveScorePanelActor *arg0);
void updateControllerPakRaceRecordSaveScorePanel(ControllerPakRaceRecordSaveScorePanelActor *arg0);
void initControllerPakRaceRecordSaveScorePanel(ControllerPakRaceRecordSaveScorePanelActor *arg0);
void drawControllerPakRaceRecordSavePromptFrame(ControllerPakRaceRecordSavePromptFrameActor *arg0);
void updateControllerPakRaceRecordSavePromptFrame(ControllerPakRaceRecordSavePromptFrameActor *arg0);
void initControllerPakRaceRecordSavePromptFrame(ControllerPakRaceRecordSavePromptFrameActor *arg0);
void drawControllerPakRaceRecordSaveStatusMessage(ControllerPakRaceRecordSaveStatusMessageActor *arg0);
void updateControllerPakRaceRecordSaveStatusMessage(ControllerPakRaceRecordSaveStatusMessageActor *arg0);
void initControllerPakRaceRecordSaveStatusMessage(ControllerPakRaceRecordSaveStatusMessageActor *arg0);
void drawControllerPakRaceRecordSaveStatusChoicePrompt(ControllerPakRaceRecordSaveStatusChoicePromptActor *arg0);
void updateControllerPakRaceRecordSaveStatusChoicePrompt(ControllerPakRaceRecordSaveStatusChoicePromptActor *arg0);
void initControllerPakRaceRecordSaveStatusChoicePrompt(ControllerPakRaceRecordSaveStatusChoicePromptActor *arg0);
void drawControllerPakRaceRecordSaveExitMessage(ControllerPakRaceRecordSaveExitMessageActor *arg0);
void updateControllerPakRaceRecordSaveExitMessage(ControllerPakRaceRecordSaveExitMessageActor *arg0);
void initControllerPakRaceRecordSaveExitMessage(ControllerPakRaceRecordSaveExitMessageActor *arg0);

#endif
