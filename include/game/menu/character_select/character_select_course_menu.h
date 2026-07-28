#ifndef CHARACTER_SELECT_COURSE_MENU_H
#define CHARACTER_SELECT_COURSE_MENU_H

#include "common.h"

typedef s16 CharacterSelectCourseUnlockList[11];

extern CharacterSelectCourseUnlockList gCharacterSelectCourseOptionsByUnlock[3];

void initCharacterSelectCourseMenuFromRaceTypeSelect(void);
void initCharacterSelectCourseMenuFromRace(void);
void initCharacterSelectCourseMenuFromPlayerSelect(void);
void updateCharacterSelectCourseMenu(void);
void updateCharacterSelectCourseSubmenu(void);
void handleCharacterSelectCourseSelection(void);
void fadeOutCharacterSelectCourseMenu(void);

#endif
