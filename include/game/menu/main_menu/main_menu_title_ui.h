#ifndef MAIN_MENU_TITLE_UI_H
#define MAIN_MENU_TITLE_UI_H

#include "common.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"

typedef struct MainMenuTitleActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
} MainMenuTitleActor;

typedef struct MainMenuTitleLogoActor {
    /* 0x00 */ MainMenuTitleActor common;
    /* 0x1C */ MenuTilemapSprite tilemapSprite;
} MainMenuTitleLogoActor;

typedef struct MainMenuTitleOptionsActor {
    /* 0x00 */ MainMenuTitleActor common;
    /* 0x1C */ s16 labelY;
    /* 0x1E */ s16 unused1E;
    /* 0x20 */ u16 selectedOption;
    /* 0x22 */ u16 blinkTimer;
    /* 0x24 */ s16 blinkAlpha;
} MainMenuTitleOptionsActor;

typedef struct MainMenuTitleCursorActor {
    /* 0x00 */ MainMenuTitleActor common;
} MainMenuTitleCursorActor;

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
