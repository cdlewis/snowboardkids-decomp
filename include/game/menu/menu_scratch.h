#ifndef MENU_SCRATCH_H
#define MENU_SCRATCH_H

#include "common.h"

/* Scene-owned callback actor references. Menus and ending credits reuse these
 * three words after resetting or replacing their tasks. Members are alternate
 * views of the SAME storage, never simultaneous allocations. */
struct CallbackTask;
struct CharacterSelectUiRosterIconActor;
struct CharacterSelectUiPlayerPanelFrameActor;
struct CharacterSelectUiPlayerCursorActor;
struct ControllerPakRaceRecordSavePromptFrameActor;
struct ControllerPakRaceRecordSaveStatusMessageActor;
struct TitleMenuWidgetActor;
struct EndingCreditsTumblingSnowboard;
struct CourseSelectAnimatedActor;

typedef union MenuScratchSlot {
    s32 value;
    struct CallbackTask *task;
    struct CharacterSelectUiRosterIconActor *rosterIcons;
    struct CharacterSelectUiPlayerPanelFrameActor *playerFrames;
    struct CharacterSelectUiPlayerCursorActor *playerCursors;
    struct ControllerPakRaceRecordSavePromptFrameActor *recordSaveFrame;
    struct ControllerPakRaceRecordSaveStatusMessageActor *recordSaveMessage;
    struct TitleMenuWidgetActor *saveWidgets;
    struct EndingCreditsTumblingSnowboard *endingSnowboard;
    struct CourseSelectAnimatedActor *coursePreview;
} MenuScratchSlot;

typedef char MenuScratchSlotSizeCheck[(sizeof(MenuScratchSlot) == 4) ? 1 : -1];

extern MenuScratchSlot gMenuScratch0;
extern MenuScratchSlot gMenuScratch1;
extern MenuScratchSlot gMenuScratch2;

#endif
