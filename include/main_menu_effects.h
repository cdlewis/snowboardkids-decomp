#ifndef MAIN_MENU_EFFECTS_H
#define MAIN_MENU_EFFECTS_H

#include "common.h"

typedef struct MainMenuScreenEffectActor MainMenuScreenEffectActor;

void initMenuSnowflakeEffect(MainMenuScreenEffectActor *arg0);
void initRaceSetupBackdrop(MainMenuScreenEffectActor *arg0);
void initRaceSetupCharacterFocus(MainMenuScreenEffectActor *arg0);
void createRaceSetupOpponentFocus(s32 arg0, s32 arg1);
void updateTitleScreenStartPrompt(void *arg0);
void waitForRaceStartPlayerEffect(MainMenuScreenEffectActor *arg0);
void initTitleMenuSparkle(MainMenuScreenEffectActor *arg0);
void waitForRaceSetupNamePlate(MainMenuScreenEffectActor *arg0);
void initMainMenuModeBoardTransition(MainMenuScreenEffectActor *arg0);
void initMainMenuModeLabelFadeIn(MainMenuScreenEffectActor *arg0);
void initMainMenuBoardModels(MainMenuScreenEffectActor *arg0);

#endif
