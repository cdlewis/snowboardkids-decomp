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
extern s32 gPlayerInputPressed[PLAYER_INPUT_COUNT];
extern s32 gPlayer2InputPressed;
extern s32 gPlayer3InputPressed;
extern s32 gPlayer4InputPressed;
extern s8 gPlayerStickX[PLAYER_INPUT_COUNT];
/* Legacy aliases for direct access preserve matching scheduler code generation. */
extern s8 gPlayer2StickX;
extern s8 gPlayer3StickX;
extern s8 gPlayer4StickX;
extern s8 gPlayerStickY[PLAYER_INPUT_COUNT];
extern s8 gPlayer2StickY;
extern s8 gPlayer3StickY;
extern s8 gPlayer4StickY;

#endif
