#ifndef MAIN_MENU_DEMO_RACE_INTRO_H
#define MAIN_MENU_DEMO_RACE_INTRO_H

#include "common.h"

extern const f32 gDemoRaceBottomLeftAspectRatio[];
extern const f32 gDemoRaceBottomRightAspectRatio[];
extern const f32 gDemoRaceFullscreenAspectRatio[];
extern const f32 gDemoRaceTopLeftAspectRatio[];
extern const f32 gDemoRaceTopRightAspectRatio[];

void initMainMenuDemoRaceIntro(void);
void waitForMainMenuDemoRaceIntroStart(void);
void updateMainMenuDemoRaceIntro(void);
void finishMainMenuDemoRaceIntro(void);

#endif
