#ifndef CONTROLLER_PAK_MENU_H
#define CONTROLLER_PAK_MENU_H

#include "common.h"
#include <PR/os_pfs.h>

typedef struct {
    /* 0x0 */ u8 mainChoice;
    /* 0x1 */ u8 fileIndex;
    /* 0x2 */ u8 confirmChoice;
    /* 0x3 */ u8 state;
    /* 0x4 */ u8 visibleFileIndex;
    /* 0x5 */ u8 isEdgeScroll;
    /* 0x6 */ u8 unk6;
} ControllerPakMenuState;

extern ControllerPakMenuState gControllerPakMenuState;
extern OSPfsState gControllerPakFileStates[];

#endif
