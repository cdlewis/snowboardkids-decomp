#ifndef CONTROLLER_PAK_UI_H
#define CONTROLLER_PAK_UI_H

#include "common.h"
#include "game/menu/renderer/menu_renderer.h"

/* These linker symbols overlap: transition + 2 is the state view.
 * confirmSelection also aliases gControllerPakRumbleCheckPromptConfirmSelection.
 * They must never be allocated as independent objects. */
typedef struct {
    /* 0x00 */ s16 pad0;
    /* 0x02 */ u8 state;
    /* 0x03 */ u8 pad3;
    /* 0x04 */ u16 messageIndex;
    /* 0x06 */ u16 timer;
    /* 0x08 */ u8 selectedOption;
    /* 0x09 */ u8 confirmSelection;
} ControllerPakRumbleCheckPromptTransition;

typedef struct {
    /* 0x00 */ u8 state;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ u16 messageIndex;
    /* 0x04 */ u16 timer;
    /* 0x06 */ u8 selectedOption;
    /* 0x07 */ u8 confirmSelection;
} ControllerPakRumbleCheckPromptState;

typedef char ControllerPakRumbleTransitionSizeCheck[
    (sizeof(ControllerPakRumbleCheckPromptTransition) == 0xA) ? 1 : -1];
typedef char ControllerPakRumbleStateSizeCheck[
    (sizeof(ControllerPakRumbleCheckPromptState) == 8) ? 1 : -1];

extern ControllerPakRumbleCheckPromptTransition gControllerPakRumbleCheckPromptTransition;
extern ControllerPakRumbleCheckPromptState gControllerPakRumbleCheckPromptState;

typedef struct {
    /* 0x00 */ char pad[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
} ControllerPakMenuActor;

typedef struct {
    /* 0x00 */ ControllerPakMenuActor common;
    /* 0x1C */ s16 scale;
    /* 0x1E */ u16 timer;
    /* 0x20 */ s32 selectedOption;
    /* 0x24 */ u8 blinkState;
} ControllerPakOptionsActor;

typedef struct {
    /* 0x00 */ ControllerPakMenuActor common;
    /* 0x1C */ s16 scale;
    /* 0x1E */ s16 optionScale;
    /* 0x20 */ u16 timer;
    /* 0x22 */ u8 state;
    /* 0x23 */ u8 selectedOption;
    /* 0x24 */ u16 messageIndex;
} ControllerPakRumbleCheckPromptActor;

typedef struct {
    /* 0x00 */ char pad[0x18];
    /* 0x18 */ Vec2s positions[6];
    /* 0x30 */ s16 cursorScale;
    /* 0x32 */ u16 cursorTimer;
} ControllerPakFileListActor;

typedef struct {
    /* 0x00 */ ControllerPakMenuActor common;
    /* 0x1C */ s16 x2;
    /* 0x1E */ s16 y2;
} ControllerPakTwoPointActor;

typedef struct {
    /* 0x00 */ ControllerPakMenuActor common;
    /* 0x1C */ MenuTilemapSprite sprite;
} ControllerPakSpriteActor;

typedef struct {
    /* 0x00 */ ControllerPakMenuActor common;
    /* 0x1C */ u16 scale;
    /* 0x1E */ u16 selectedOption;
    /* 0x20 */ u8 timer;
} ControllerPakDeletePromptActor;

typedef struct {
    /* 0x00 */ ControllerPakMenuActor common;
    /* 0x1C */ s16 scale;
    /* 0x1E */ u16 timer;
    /* 0x20 */ u8 selectedOption;
} ControllerPakWindowActor;

void drawControllerPakContinuePrompt(ControllerPakOptionsActor *arg0);
void updateControllerPakContinuePrompt(ControllerPakOptionsActor *arg0);
void initControllerPakContinuePrompt(ControllerPakOptionsActor *arg0);
void drawControllerPakRumbleCheckPrompt(ControllerPakRumbleCheckPromptActor *arg0);
void updateControllerPakRumbleCheckPrompt(ControllerPakRumbleCheckPromptActor *arg0);
void initControllerPakRumbleCheckPrompt(ControllerPakRumbleCheckPromptActor *arg0);
void drawControllerPakFileDeleteMainOptions(ControllerPakOptionsActor *arg0);
void updateControllerPakFileDeleteMainOptionsUi(ControllerPakOptionsActor *arg0);
void initControllerPakFileDeleteMainOptions(ControllerPakOptionsActor *arg0);
void drawControllerPakFileDeleteConfirmOptions(ControllerPakOptionsActor *arg0);
void updateControllerPakFileDeleteConfirmOptionsUi(ControllerPakOptionsActor *arg0);
void initControllerPakFileDeleteConfirmOptions(ControllerPakOptionsActor *arg0);
void drawControllerPakFileDeleteFreeSpaceInfo(ControllerPakTwoPointActor *arg0);
void updateControllerPakFileDeleteFreeSpaceInfo(ControllerPakTwoPointActor *arg0);
void initControllerPakFileDeleteFreeSpaceInfo(ControllerPakTwoPointActor *arg0);
void drawControllerPakFileDeleteFileList(ControllerPakFileListActor *arg0);
void updateControllerPakFileDeleteFileListUi(ControllerPakFileListActor *arg0);
void initControllerPakFileDeleteFileList(ControllerPakFileListActor *arg0);
void drawControllerPakFileDeleteIcon(ControllerPakSpriteActor *arg0);
void updateControllerPakFileDeleteIcon(ControllerPakSpriteActor *arg0);
void initControllerPakFileDeleteIcon(ControllerPakSpriteActor *arg0);
void drawControllerPakMessageIcon(ControllerPakSpriteActor *arg0);
void updateControllerPakMessageIcon(ControllerPakSpriteActor *arg0);
void initControllerPakMessageIcon(ControllerPakSpriteActor *arg0);
void drawControllerPakFileDeleteErrorPrompt(ControllerPakWindowActor *arg0);
void updateControllerPakFileDeleteErrorPromptUi(ControllerPakDeletePromptActor *arg0);
void initControllerPakFileDeleteErrorPrompt(ControllerPakDeletePromptActor *arg0);
void drawControllerPakDeleteConfirmPrompt(ControllerPakDeletePromptActor *arg0);
void updateControllerPakDeleteConfirmPrompt(ControllerPakDeletePromptActor *arg0);
void initControllerPakDeleteConfirmPrompt(ControllerPakDeletePromptActor *arg0);

#endif
