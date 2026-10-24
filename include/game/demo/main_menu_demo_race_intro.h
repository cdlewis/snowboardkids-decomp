#ifndef MAIN_MENU_DEMO_RACE_INTRO_H
#define MAIN_MENU_DEMO_RACE_INTRO_H

#include "common.h"

#if defined(VERSION_JP)
extern f32 gDemoRaceBottomLeftAspectRatio;
extern f32 gDemoRaceBottomRightAspectRatio;
extern f32 gDemoRaceFullscreenAspectRatio;
extern f32 gDemoRaceTopLeftAspectRatio;
extern f32 gDemoRaceTopRightAspectRatio;
#endif

void initMainMenuDemoRaceIntro(void);
void waitForMainMenuDemoRaceIntroStart(void);
void updateMainMenuDemoRaceIntro(void);
void finishMainMenuDemoRaceIntro(void);

#endif
