#ifndef MAIN_MENU_DEMO_RACE_INTRO_DATA_H
#define MAIN_MENU_DEMO_RACE_INTRO_DATA_H

#include "common.h"

typedef struct {
    /* 0x00 */ u8 courseIndex;
    /* 0x01 */ u8 splitscreen;
    /* 0x02 */ u8 raceType;
    /* 0x03 */ u8 loadCourseAssets;
    /* 0x04 */ void *romStart;
    /* 0x08 */ void *romEnd;
    /* 0x0C */ u8 player0Character;
    /* 0x0D */ u8 player1Character;
    /* 0x0E */ u8 player2Character;
    /* 0x0F */ u8 player3Character;
} MainMenuDemoRaceIntroEntry;

extern u8 gMainMenuDemoRaceIntroIndex;
extern MainMenuDemoRaceIntroEntry gMainMenuDemoRaceIntroEntries[];

#endif
