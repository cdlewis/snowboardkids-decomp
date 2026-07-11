#ifndef TITLE_SCREEN_UI_H
#define TITLE_SCREEN_UI_H

#include "common.h"

typedef struct TitleScreenLogoActor TitleScreenLogoActor;
typedef struct TitleScreenMenuOptionsActor TitleScreenMenuOptionsActor;
typedef struct TitleScreenMenuCursorActor TitleScreenMenuCursorActor;

void drawTitleScreenLogo(TitleScreenLogoActor *arg0);
void updateTitleScreenLogo(TitleScreenLogoActor *arg0);
void initTitleScreenLogo(TitleScreenLogoActor *arg0);
void drawTitleScreenMenuOptions(TitleScreenMenuOptionsActor *arg0);
void updateTitleScreenMenuOptions(TitleScreenMenuOptionsActor *arg0);
void initTitleScreenMenuOptions(TitleScreenMenuOptionsActor *arg0);
void drawTitleScreenMenuCursor(TitleScreenMenuCursorActor *arg0);
void updateTitleScreenMenuCursor(TitleScreenMenuCursorActor *arg0);
void initTitleScreenMenuCursor(TitleScreenMenuCursorActor *arg0);

#endif
