#ifndef MAIN_MENU_TITLE_UI_H
#define MAIN_MENU_TITLE_UI_H

#include "common.h"

typedef struct MainMenuTitleLogoActor MainMenuTitleLogoActor;
typedef struct MainMenuTitleOptionsActor MainMenuTitleOptionsActor;
typedef struct MainMenuTitleCursorActor MainMenuTitleCursorActor;

void drawMainMenuTitleLogo(MainMenuTitleLogoActor *arg0);
void updateMainMenuTitleLogo(MainMenuTitleLogoActor *arg0);
void initMainMenuTitleLogo(MainMenuTitleLogoActor *arg0);
void drawMainMenuTitleOptions(MainMenuTitleOptionsActor *arg0);
void updateMainMenuTitleOptions(MainMenuTitleOptionsActor *arg0);
void initMainMenuTitleOptions(MainMenuTitleOptionsActor *arg0);
void drawMainMenuTitleCursor(MainMenuTitleCursorActor *arg0);
void updateMainMenuTitleCursor(MainMenuTitleCursorActor *arg0);
void initMainMenuTitleCursor(MainMenuTitleCursorActor *arg0);

#endif
