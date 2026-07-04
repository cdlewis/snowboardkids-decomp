#ifndef CONTROLLER_PAK_MENU_H
#define CONTROLLER_PAK_MENU_H

#include "common.h"

typedef struct {
    /* 0x0 */ u8 mainChoice;
    /* 0x1 */ u8 fileIndex;
    /* 0x2 */ u8 confirmChoice;
    /* 0x3 */ u8 state;
    /* 0x4 */ s8 pendingFileIndex;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ u8 unk6;
} ControllerPakMenuState;

#endif
