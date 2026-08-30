#ifndef CHARACTER_SELECT_COURSE_MENU_H
#define CHARACTER_SELECT_COURSE_MENU_H

#include "common.h"

typedef s16 CharacterSelectCourseUnlockList[11];

typedef struct CharacterSelectCourseCursorState {
    /* 0x00 */ u8 listCursorState;
    /* 0x01 */ u8 submenuState;
    /* 0x02 */ u8 previewFrameState;
    /* 0x04 */ s16 listCursorSpriteIndex;
    /* 0x06 */ u8 listCursorTimer;
    /* 0x07 */ u8 submenuTimer;
} CharacterSelectCourseCursorState;

typedef struct {
    CharacterSelectCourseUnlockList courseOptionsByUnlock[3];
    s16 playerBadgeDisplayOrder[11];
    s16 shortCourseOptions[4];
    s16 singleCourseOption[8];
} CharacterSelectCourseMenuData;

extern CharacterSelectCourseMenuData gCharacterSelectCourseMenuData;
extern u16 gCharacterSelectCourseExitOptionIndex;
extern CharacterSelectCourseCursorState gCharacterSelectCourseCursorState;

void initCharacterSelectCourseMenuFromRaceTypeSelect(void);
void initCharacterSelectCourseMenuFromRace(void);
void initCharacterSelectCourseMenuFromPlayerSelect(void);
void updateCharacterSelectCourseMenu(void);
void updateCharacterSelectCourseSubmenu(void);
void handleCharacterSelectCourseSelection(void);
void fadeOutCharacterSelectCourseMenu(void);

#endif
