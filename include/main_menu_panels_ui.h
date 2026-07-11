#ifndef MAIN_MENU_PANELS_UI_H
#define MAIN_MENU_PANELS_UI_H

#include "common.h"

typedef struct MenuPanelActor MenuPanelActor;

void drawMainMenuModeSelectFrame(s32 arg0);
void drawMainMenuModeSelectIcons(MenuPanelActor *arg0);
void updateMainMenuModeSelectGrid(MenuPanelActor *arg0);
void initMainMenuModeSelectGrid(MenuPanelActor *arg0);
void drawMainMenuModeDescriptionPanel(s32 arg0);
void scrollMainMenuModeDescriptionPanelOut(MenuPanelActor *arg0);
void updateMainMenuModeDescriptionPanel(MenuPanelActor *arg0);
void initMainMenuModeDescriptionPanel(MenuPanelActor *arg0);
void drawMainMenuModeSelectList(s32 arg0);
void updateMainMenuModeSelectList(MenuPanelActor *arg0);
void initMainMenuModeSelectList(MenuPanelActor *arg0);
void drawRaceSetupCornerPrompts(s32 arg0);
void updateRaceSetupCornerPrompts(MenuPanelActor *arg0);
void initRaceSetupCornerPrompts(MenuPanelActor *arg0);
void drawRaceGhostUnavailableMessage(s32 arg0);
void updateRaceGhostUnavailableMessage(s32 arg0);
void drawMainMenuSettingsPanel(s32 arg0);
void drawMainMenuSettingsOptions(s32 arg0);
void updateMainMenuSettingsPanel(MenuPanelActor *arg0);
void initMainMenuSettingsPanel(MenuPanelActor *arg0);
void drawRaceRecordSettingsPanel(s32 arg0);
void updateRaceRecordSettingsPanel(MenuPanelActor *arg0);
void initRaceRecordSettingsPanel(MenuPanelActor *arg0);

#endif
