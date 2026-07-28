#ifndef CHARACTER_SELECT_COURSE_MENU_H
#define CHARACTER_SELECT_COURSE_MENU_H

#include "common.h"

typedef s16 CharacterSelectCourseUnlockList[11];

typedef struct CharacterSelectFlowState {
    char pad0[0x18];
    /* 0x18 */ s32 fade;
    /* 0x1C */ s32 timer;
    /* 0x20 */ s32 unk20;
} CharacterSelectFlowState;

extern CharacterSelectCourseUnlockList gCharacterSelectCourseOptionsByUnlock[3];

void initCharacterSelectCourseMenuFromRaceTypeSelect(void);
void initCharacterSelectCourseMenuFromRace(void);
void initCharacterSelectCourseMenuFromPlayerSelect(void);
void updateCharacterSelectCourseMenu(void);
void updateCharacterSelectCourseSubmenu(void);
void handleCharacterSelectCourseSelection(void);
void fadeOutCharacterSelectCourseMenu(void);

#endif
