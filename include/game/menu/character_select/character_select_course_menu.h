#ifndef CHARACTER_SELECT_COURSE_MENU_H
#define CHARACTER_SELECT_COURSE_MENU_H

#include "common.h"

typedef s16 CharacterSelectCourseUnlockList[11];

typedef struct {
    CharacterSelectCourseUnlockList courseOptionsByUnlock[3];
    s16 playerBadgeDisplayOrder[11];
    s16 shortCourseOptions[4];
    s16 singleCourseOption[8];
} CharacterSelectCourseMenuData;

extern CharacterSelectCourseMenuData gCharacterSelectCourseMenuData;
extern u16 gCharacterSelectCourseExitOptionIndex;

#define gCharacterSelectCourseOptionsByUnlock (gCharacterSelectCourseMenuData.courseOptionsByUnlock)
#define gPlayerBadgeDisplayOrder (gCharacterSelectCourseMenuData.playerBadgeDisplayOrder)
#define gCharacterSelectShortCourseOptions (gCharacterSelectCourseMenuData.shortCourseOptions)
#define gCharacterSelectSingleCourseOption (gCharacterSelectCourseMenuData.singleCourseOption)

void initCharacterSelectCourseMenuFromRaceTypeSelect(void);
void initCharacterSelectCourseMenuFromRace(void);
void initCharacterSelectCourseMenuFromPlayerSelect(void);
void updateCharacterSelectCourseMenu(void);
void updateCharacterSelectCourseSubmenu(void);
void handleCharacterSelectCourseSelection(void);
void fadeOutCharacterSelectCourseMenu(void);

#endif
