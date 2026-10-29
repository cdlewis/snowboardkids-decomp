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

extern s16 courseRecordDigitTileOffsets[];

#if defined(VERSION_JP)
extern const char gTitleScreenStartPromptText[];
extern Vtx congratulationsBannerModelVertices[];
extern s16 congratulationsBannerSpriteTileOffsets[];
extern s16 passAwardLabelFlashTileOffsets[];
extern u8 passAwardOpponentAnimationIds[];
extern s32 passAwardOpponentXPositions[];
extern u8 raceSetupCharacterFocusAnimationIds[];
extern s16 raceSetupCharacterFocusSoundIds[];
extern Vtx raceStartPlayerEffectVertices[];
extern s16 titleMenuSparklePositions[];
extern u8 titleMenuSparkleTileFrames[];
#endif

#if defined(VERSION_JP)
void drawCongratulationsBannerModel(MenuScreenEffectActor *);
void drawFallingConfetti(MenuScreenEffectActor *arg0);
void drawMainMenuRotatingBoardModel(MenuScreenEffectActor *);
void drawMainMenuStaticBoardModel(void *);
void drawPassAwardCourseBackdrop(void *);
void drawPassAwardPodiumModels(MenuScreenEffectActor *);
void drawRaceSetupNamePlate(MenuScreenEffectActor *);
void drawTitleMenuSparkle(MenuScreenEffectActor *);
void drawTitleScreenStartPrompt(MenuScreenEffectActor *);
void holdRaceSetupNamePlate(MenuScreenEffectActor *);
void initPassAwardOpponent(MenuScreenEffectActor *arg0);
void renderMainMenuBoardModels(MenuScreenEffectActor *arg0);
void renderPassAwardPodium(MenuScreenEffectActor *);
void renderTitleMenuSparkle(MenuScreenEffectActor *arg0);
void updateCongratulationsBannerModel(MenuScreenEffectActor *);
void updateCongratulationsBannerSprite(MenuScreenEffectActor *);
void updateCourseRecordBannerFadeOut(MenuScreenEffectActor *);
void updateFallingConfettiDrift(MenuScreenEffectActor *arg0);
void updateFallingConfettiSway(MenuScreenEffectActor *arg0);
void updatePassAwardLabelFadeIn(MenuScreenEffectActor *);
void updateRaceSetupNamePlateSlideOut(MenuScreenEffectActor *);
void updateRaceStartPlayerEffectActive(MenuScreenEffectActor *);
#endif

void initFallingConfetti(MenuScreenEffectActor *arg0);
void initPassAwardPodium(MenuScreenEffectActor *arg0);
void initPassAwardWinner(MenuScreenEffectActor *arg0);
void createPassAwardOpponent(s32 arg0, s32 arg1);
void updateTitleScreenStartPrompt(void *arg0);
void waitForRaceStartPlayerEffect(MenuScreenEffectActor *arg0);
void initTitleMenuSparkle(MenuScreenEffectActor *arg0);
void waitForRaceSetupNamePlate(MenuScreenEffectActor *arg0);
void initCongratulationsBannerModel(MenuScreenEffectActor *arg0);
void initPassAwardLabelFadeIn(MenuScreenEffectActor *arg0);
void initMainMenuBoardModels(MenuScreenEffectActor *arg0);
void initCourseRecordBannerFadeOut(MenuScreenEffectActor *arg0);

#endif
