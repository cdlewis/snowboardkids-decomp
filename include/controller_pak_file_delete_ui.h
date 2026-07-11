#ifndef CONTROLLER_PAK_FILE_DELETE_UI_H
#define CONTROLLER_PAK_FILE_DELETE_UI_H

#include "common.h"

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
} ControllerPakVec2s;

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
} ControllerPakTitleActor;

typedef struct {
    /* 0x00 */ ControllerPakMenuActor common;
    /* 0x1C */ s16 scale;
    /* 0x1E */ s16 optionScale;
    /* 0x20 */ u16 timer;
    /* 0x22 */ u8 state;
    /* 0x23 */ u8 selectedOption;
    /* 0x24 */ s16 targetScale;
} ControllerPakConfirmActor;

typedef struct {
    /* 0x00 */ char pad[0x18];
    /* 0x18 */ ControllerPakVec2s positions[6];
    /* 0x30 */ s16 cursorScale;
    /* 0x32 */ u16 cursorTimer;
} ControllerPakFileListActor;

typedef struct {
    /* 0x00 */ ControllerPakMenuActor common;
    /* 0x1C */ s16 x2;
    /* 0x1E */ s16 y2;
} ControllerPakTwoPointActor;

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ u8 unk4[4];
} ControllerPakSprite;

typedef struct {
    /* 0x00 */ ControllerPakMenuActor common;
    /* 0x1C */ ControllerPakSprite sprite;
    /* 0x24 */ s16 startX;
    /* 0x26 */ s16 startY;
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

void drawControllerPakTitlePrompt(ControllerPakTitleActor *arg0);
void updateControllerPakTitlePrompt(ControllerPakTitleActor *arg0);
void initControllerPakTitlePrompt(ControllerPakTitleActor *arg0);
void drawControllerPakCheckPrompt(ControllerPakConfirmActor *arg0);
void updateControllerPakCheckPrompt(ControllerPakConfirmActor *arg0);
void initControllerPakCheckPrompt(ControllerPakConfirmActor *arg0);
void drawControllerPakFileDeleteMainOptions(ControllerPakTitleActor *arg0);
void updateControllerPakFileDeleteMainOptionsUi(ControllerPakTitleActor *arg0);
void initControllerPakFileDeleteMainOptions(ControllerPakTitleActor *arg0);
void drawControllerPakFileDeleteConfirmOptions(ControllerPakTitleActor *arg0);
void updateControllerPakFileDeleteConfirmOptionsUi(ControllerPakTitleActor *arg0);
void initControllerPakFileDeleteConfirmOptions(ControllerPakTitleActor *arg0);
void drawControllerPakFileDeleteFreeSpaceInfo(ControllerPakTwoPointActor *arg0);
void updateControllerPakFileDeleteFreeSpaceInfo(s32 arg0);
void initControllerPakFileDeleteFreeSpaceInfo(ControllerPakTwoPointActor *arg0);
void drawControllerPakFileDeleteFileList(ControllerPakFileListActor *arg0);
void updateControllerPakFileDeleteFileListUi(ControllerPakFileListActor *arg0);
void initControllerPakFileDeleteFileList(ControllerPakFileListActor *arg0);
void drawControllerPakFileDeleteIcon(ControllerPakSpriteActor *arg0);
void updateControllerPakFileDeleteIcon(s32 arg0);
void initControllerPakFileDeleteIcon(ControllerPakSpriteActor *arg0);
void drawControllerPakMessageIcon(ControllerPakSpriteActor *arg0);
void updateControllerPakMessageIcon(s32 arg0);
void initControllerPakMessageIcon(ControllerPakSpriteActor *arg0);
void drawControllerPakFileDeleteErrorPrompt(ControllerPakWindowActor *arg0);
void updateControllerPakFileDeleteErrorPromptUi(ControllerPakDeletePromptActor *arg0);
void initControllerPakFileDeleteErrorPrompt(ControllerPakDeletePromptActor *arg0);
void drawControllerPakFileDeleteConfirmPrompt(ControllerPakDeletePromptActor *arg0);
void updateControllerPakFileDeleteConfirmPrompt(ControllerPakDeletePromptActor *arg0);
void initControllerPakFileDeleteConfirmPrompt(ControllerPakDeletePromptActor *arg0);

#endif
