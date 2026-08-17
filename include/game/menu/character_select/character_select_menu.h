#ifndef CHARACTER_SELECT_MENU_H
#define CHARACTER_SELECT_MENU_H

#include "common.h"

#define CHARACTER_SELECT_PHASE_INIT 0
#define CHARACTER_SELECT_PHASE_PROMPT 1
#define CHARACTER_SELECT_PHASE_PROMPT_EXIT 2
#define CHARACTER_SELECT_PHASE_ROSTER 3
#define CHARACTER_SELECT_PHASE_CONFIRM 4

#define CHARACTER_SELECT_TOKEN_IDLE 0
#define CHARACTER_SELECT_TOKEN_START 1
#define CHARACTER_SELECT_TOKEN_FLYING 2
#define CHARACTER_SELECT_TOKEN_LANDED 3

typedef struct {
    /* 0x00 */ u8 phase;
    /* 0x01 */ u8 exitMode;
    /* 0x02 */ u8 rosterReady;
    /* 0x03 */ u8 selectedTokenState[4];
    /* 0x07 */ u8 pad7;
    /* 0x08 */ s16 bannerAlpha;
    /* 0x0A */ s16 bannerBounceOffset;
    /* 0x0C */ u8 confirmationChoice;
    /* 0x0D */ u8 bannerBounceTimer;
    /* 0x0E */ u8 leftSecretSlotUnlocked;
    /* 0x0F */ u8 rightSecretSlotUnlocked;
    /* 0x10 */ u16 cursorX;
    /* 0x12 */ u16 cursorY;
} CharacterSelectState;

typedef char CharacterSelectStateSizeCheck[(sizeof(CharacterSelectState) == 0x14) ? 1 : -1];

extern CharacterSelectState gCharacterSelectHudState;
extern s8 gCharacterSelectHighlightedRosterIndices[4];

void initCharacterSelectMenu(void);
void updateCharacterSelectMenu(void);
void updateCharacterSelectConfirmationMenu(void);
void fadeOutCharacterSelectMenu(void);

#endif
