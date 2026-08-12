#ifndef MENU_SCREEN_EFFECTS_H
#define MENU_SCREEN_EFFECTS_H

#include "common.h"

typedef struct MenuScreenEffectActor MenuScreenEffectActor;

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
