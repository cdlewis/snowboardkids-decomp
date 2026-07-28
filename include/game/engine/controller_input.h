#ifndef CONTROLLER_INPUT_H
#define CONTROLLER_INPUT_H

#include "common.h"

#define PLAYER_INPUT_COUNT 4

typedef struct ControllerInputState {
    u16 buttons;
    s8 stickX;
    s8 stickY;
    u8 pad4[2];
} ControllerInputState;

extern ControllerInputState gControllerInputState[PLAYER_INPUT_COUNT];
extern s32 gPlayerInputHeld[PLAYER_INPUT_COUNT];
extern s32 gPlayer2InputHeld;
extern s32 gPlayer3InputHeld;
extern s32 gPlayer4InputHeld;

#endif
