#ifndef CONTROLLER_PAK_RACE_RECORD_SAVE_UI_H
#define CONTROLLER_PAK_RACE_RECORD_SAVE_UI_H

#include "common.h"
#include "game/engine/callback_task_scheduler.h"

typedef struct {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
} ControllerPakRaceRecordSaveActor;

typedef struct {
    /* 0x00 */ ControllerPakRaceRecordSaveActor common;
    /* 0x1C */ u8 state;
} ControllerPakRaceRecordSaveScorePanelActor;

typedef struct {
    /* 0x00 */ ControllerPakRaceRecordSaveActor common;
    /* 0x1C */ s16 scale;
    /* 0x1E */ u8 unused1E;
    /* 0x1F */ u8 state;
} ControllerPakRaceRecordSavePromptFrameActor;

typedef struct {
    /* 0x00 */ ControllerPakRaceRecordSaveActor common;
    /* 0x1C */ s16 alpha;
    /* 0x1E */ u16 targetStatus;
    /* 0x20 */ u8 state;
    /* 0x21 */ u8 arrowFrame;
} ControllerPakRaceRecordSaveStatusMessageActor;

typedef struct {
    /* 0x00 */ ControllerPakRaceRecordSaveActor common;
    /* 0x1C */ s16 bottomChoiceY;
    /* 0x1E */ s16 slideOffset;
    /* 0x20 */ u16 nextStatus;
    /* 0x22 */ u16 cursorAlpha;
    /* 0x24 */ u16 cursorAlphaTimer;
} ControllerPakRaceRecordSaveStatusChoicePromptActor;

typedef struct {
    /* 0x00 */ ControllerPakRaceRecordSaveActor common;
    /* 0x1C */ s16 alpha;
    /* 0x1E */ u8 state;
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
