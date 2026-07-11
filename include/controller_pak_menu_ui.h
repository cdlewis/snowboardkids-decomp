#ifndef CONTROLLER_PAK_MENU_UI_H
#define CONTROLLER_PAK_MENU_UI_H

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
    /* 0x24 */ u16 targetScale;
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

void func_8002FFD0(ControllerPakTitleActor *arg0);
void func_8003048C(ControllerPakTitleActor *arg0);
void func_80030570(ControllerPakTitleActor *arg0);
void func_800305B8(ControllerPakConfirmActor *arg0);
void func_80030CC4(ControllerPakConfirmActor *arg0);
void func_80030EA8(ControllerPakConfirmActor *arg0);
void func_80030EF0(ControllerPakTitleActor *arg0);
void func_80031038(ControllerPakTitleActor *arg0);
void initControllerPakFileDeleteMainOptions(ControllerPakTitleActor *arg0);
void func_8003112C(ControllerPakTitleActor *arg0);
void func_80031294(ControllerPakTitleActor *arg0);
void initControllerPakFileDeleteConfirmOptions(ControllerPakTitleActor *arg0);
void func_80031370(ControllerPakTwoPointActor *arg0);
void func_800314E0(s32 arg0);
void initControllerPakFreeSpaceInfo(ControllerPakTwoPointActor *arg0);
void func_80031550(ControllerPakFileListActor *arg0);
void func_80031A88(ControllerPakFileListActor *arg0);
void initControllerPakFileList(ControllerPakFileListActor *arg0);
void func_80031BA0(ControllerPakSpriteActor *arg0);
void func_80031BD4(s32 arg0);
void initControllerPakFileDeleteIcon(ControllerPakSpriteActor *arg0);
void func_80031C6C(ControllerPakSpriteActor *arg0);
void func_80031CA0(s32 arg0);
void func_80031CD0(ControllerPakSpriteActor *arg0);
void func_80031D3C(ControllerPakWindowActor *arg0);
void func_80031F40(ControllerPakDeletePromptActor *arg0);
void initControllerPakDeleteErrorPrompt(ControllerPakDeletePromptActor *arg0);
void func_8003209C(ControllerPakDeletePromptActor *arg0);
void func_80032534(ControllerPakDeletePromptActor *arg0);
void initControllerPakDeleteConfirmPrompt(ControllerPakDeletePromptActor *arg0);

#endif
