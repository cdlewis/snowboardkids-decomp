#ifndef CHARACTER_SELECT_MENU_H
#define CHARACTER_SELECT_MENU_H

#include "common.h"

typedef struct {
    /* 0x00 */ u8 phase;
    /* 0x01 */ u8 exitMode;
    /* 0x02 */ u8 readyCount;
    /* 0x03 */ u8 selectedTokenState[4];
    /* 0x07 */ u8 pad7;
    /* 0x08 */ s16 fade;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ u8 confirmSelection;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 blockDecrease;
    /* 0x0F */ u8 blockIncrease;
    /* 0x10 */ u16 cursorX;
    /* 0x12 */ u16 cursorY;
    /* 0x14 */ s8 playerSelections[4];
} CharacterSelectState;

extern CharacterSelectState gCharacterSelectHudState;

void initCharacterSelectMenu(void);
void updateCharacterSelectMenu(void);
void updateCharacterSelectConfirmationMenu(void);
void fadeOutCharacterSelectMenu(void);

#endif
