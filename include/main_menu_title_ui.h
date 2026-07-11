#ifndef MAIN_MENU_TITLE_UI_H
#define MAIN_MENU_TITLE_UI_H

#include "common.h"

typedef struct MainMenuLogoActor MainMenuLogoActor;
typedef struct MainMenuSelectionLabelsActor MainMenuSelectionLabelsActor;
typedef struct MainMenuSelectionArrowActor MainMenuSelectionArrowActor;

void drawMainMenuTitleLogo(MainMenuLogoActor *arg0);
void updateMainMenuTitleLogo(MainMenuLogoActor *arg0);
void initMainMenuTitleLogo(MainMenuLogoActor *arg0);
void drawMainMenuTitleOptions(MainMenuSelectionLabelsActor *arg0);
void updateMainMenuTitleOptions(MainMenuSelectionLabelsActor *arg0);
void initMainMenuTitleOptions(MainMenuSelectionLabelsActor *arg0);
void drawMainMenuTitleCursor(MainMenuSelectionArrowActor *arg0);
void updateMainMenuTitleCursor(MainMenuSelectionArrowActor *arg0);
void initMainMenuTitleCursor(MainMenuSelectionArrowActor *arg0);

#endif
