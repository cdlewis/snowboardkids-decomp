#ifndef RACE_SPLITSCREEN_SELECT_MENU_H
#define RACE_SPLITSCREEN_SELECT_MENU_H

#include "common.h"

extern u8 gRaceSplitscreenSelectCursorAnimState;

void returnToRaceSplitscreenSelectMenu(void);
void initRaceSplitscreenSelectMenu(void);
void updateRaceSplitscreenSelectMenu(void);
void handleRaceSplitscreenSelectMenuSelection(void);
void fadeOutRaceSplitscreenSelectMenu(void);

#endif
