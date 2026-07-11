#ifndef MAIN_MENU_VISUAL_EFFECTS_H
#define MAIN_MENU_VISUAL_EFFECTS_H

#include "common.h"

typedef struct MainMenuVisualEffectActor MainMenuVisualEffectActor;

void initMenuSnowflakeEffect(MainMenuVisualEffectActor *arg0);
void initRaceSetupBackdrop(MainMenuVisualEffectActor *arg0);
void initRaceSetupCharacterFocus(MainMenuVisualEffectActor *arg0);
void createRaceSetupOpponentFocus(s32 arg0, s32 arg1);
void updateTitleStartPrompt(void *arg0);
void waitForRaceStartPlayerEffect(MainMenuVisualEffectActor *arg0);
void initTitleMenuSparkle(MainMenuVisualEffectActor *arg0);
void waitForRaceSetupNamePlate(MainMenuVisualEffectActor *arg0);
void initSpinningBoardTransition(MainMenuVisualEffectActor *arg0);
void initMainMenuModeLabelFadeIn(MainMenuVisualEffectActor *arg0);
void initTitleMenuBoardModels(MainMenuVisualEffectActor *arg0);

#endif
