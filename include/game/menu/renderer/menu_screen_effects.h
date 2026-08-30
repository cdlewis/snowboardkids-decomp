#ifndef MENU_SCREEN_EFFECTS_H
#define MENU_SCREEN_EFFECTS_H

#include "common.h"

typedef union {
    s32 word;
    struct {
        s16 hi;
        s16 lo;
    } half;
    struct {
        u8 b0;
        u8 b1;
        u8 b2;
        u8 b3;
    } byte;
} OverlayActorWord;

typedef struct MenuScreenEffectActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ OverlayActorWord unk18;
    /* 0x1C */ OverlayActorWord unk1C;
    /* 0x20 */ OverlayActorWord unk20;
    /* 0x24 */ OverlayActorWord unk24;
    /* 0x28 */ u16 timer;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ u8 pad2C[2];
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ OverlayActorWord unk30;
    /* 0x34 */ s16 spriteIndex;
    /* 0x36 */ s16 alpha;
} MenuScreenEffectActor;

void initFallingMenuSnowflake(MenuScreenEffectActor *arg0);
void initRaceSetupBackdrop(MenuScreenEffectActor *arg0);
void initRaceSetupCharacterFocus(MenuScreenEffectActor *arg0);
void createRaceSetupOpponentFocus(s32 arg0, s32 arg1);
void updateTitleScreenStartPrompt(void *arg0);
void waitForRaceStartPlayerEffect(MenuScreenEffectActor *arg0);
void initTitleMenuSparkle(MenuScreenEffectActor *arg0);
void waitForRaceSetupNamePlate(MenuScreenEffectActor *arg0);
void initMainMenuModeBoardTransition(MenuScreenEffectActor *arg0);
void initMainMenuModeLabelFadeIn(MenuScreenEffectActor *arg0);
void initMainMenuBoardModels(MenuScreenEffectActor *arg0);
void initCourseRecordBannerFadeOut(MenuScreenEffectActor *arg0);

#endif
