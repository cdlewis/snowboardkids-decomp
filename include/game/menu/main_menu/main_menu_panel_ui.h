#ifndef MAIN_MENU_PANEL_UI_H
#define MAIN_MENU_PANEL_UI_H

#include "common.h"

typedef struct MenuPanelActor {
    /* 0x00 */ char pad0[0x18];
    /* 0x18 */ s32 x;
    /* 0x1C */ s32 y;
    /* 0x20 */ u16 *tileList;
    /* 0x24 */ u16 *tileListStart;
    /* 0x28 */ s16 inputRepeatTimer;
    /* 0x2A */ u8 selectedTile;
    /* 0x2B */ u8 selectionState;
} MenuPanelActor; // size = 0x2C

void drawMainMenuModeSelectFrame(void *arg0);
void drawMainMenuModeSelectIcons(MenuPanelActor *arg0);
void updateMainMenuModeSelectGrid(MenuPanelActor *arg0);
void initMainMenuModeSelectGrid(MenuPanelActor *arg0);
void drawMainMenuModeDescriptionPanel(MenuPanelActor *arg0);
void scrollMainMenuModeDescriptionPanelOut(MenuPanelActor *arg0);
void updateMainMenuModeDescriptionPanel(MenuPanelActor *arg0);
void initMainMenuModeDescriptionPanel(MenuPanelActor *arg0);
void drawMainMenuModeSelectMenuOptions(void *arg0);
void updateMainMenuModeSelectMenuOptions(MenuPanelActor *arg0);
void initMainMenuModeSelectMenuOptions(MenuPanelActor *arg0);
void drawRaceSetupCornerPrompts(void *arg0);
void updateRaceSetupCornerPrompts(MenuPanelActor *arg0);
void initRaceSetupCornerPrompts(MenuPanelActor *arg0);
void drawRaceGhostUnavailableMessage(void *arg0);
void updateRaceGhostUnavailableMessage(void *arg0);
void drawMainMenuSettingsPanel(void *arg0);
void drawMainMenuSettingsOptions(void *arg0);
void updateMainMenuSettingsPanel(MenuPanelActor *arg0);
void initMainMenuSettingsPanel(MenuPanelActor *arg0);
void drawRaceRecordSettingsPanel(void *arg0);
void updateRaceRecordSettingsPanel(MenuPanelActor *arg0);
void initRaceRecordSettingsPanel(MenuPanelActor *arg0);

#endif
