#ifndef MAIN_MENU_DEMO_RACE_INTRO_H
#define MAIN_MENU_DEMO_RACE_INTRO_H

#include "common.h"

/* Only JP has separately addressed rodata symbols for these values. */
#if defined(VERSION_JP)
extern const f32 gDemoRaceBottomLeftAspectRatio[];
extern const f32 gDemoRaceBottomRightAspectRatio[];
extern const f32 gDemoRaceFullscreenAspectRatio[];
extern const f32 gDemoRaceTopLeftAspectRatio[];
extern const f32 gDemoRaceTopRightAspectRatio[];
#endif

void initMainMenuDemoRaceIntro(void);
void waitForMainMenuDemoRaceIntroStart(void);
void updateMainMenuDemoRaceIntro(void);
void finishMainMenuDemoRaceIntro(void);

#endif
