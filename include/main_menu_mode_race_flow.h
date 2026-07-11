#ifndef MAIN_MENU_MODE_RACE_FLOW_H
#define MAIN_MENU_MODE_RACE_FLOW_H

#include "common.h"

void startMainMenuModeDemoRaceFlow(void);
void initMainMenuModeDemoSelectMenu(void);
void fadeInMainMenuModeDemoSelectMenu(void);
void updateMainMenuModeDemoSelectMenu(void);
void fadeOutMainMenuModeDemoSelectMenu(void);
void exitMainMenuModeDemoSelectMenu(void);
void initMainMenuModeDemoRace(void);
void fadeInMainMenuModeDemoRace(void);
void waitForMainMenuModeDemoRaceSelection(void);
void zoomMainMenuModeDemoRaceViewport(void);
void runMainMenuModeDemoRace(void);
void fadeOutMainMenuModeDemoRace(void);
void returnToMainMenuModeDemoSelectMenu(void);
void startTrainingCourseRaceFlow(void);
void deferTrainingCourseRaceInit(void);
void initTrainingCourseRace(void);
void fadeInTrainingCourseRace(void);
void waitForTrainingCourseStartPrompt(void);
void zoomTrainingCourseRaceViewport(void);
void runTrainingCourseRaceUntilActionPrompt(void);
void fadeInTrainingCourseActionMenu(void);
void waitForTrainingCourseActionMenuSelection(void);
void fadeOutTrainingCourseActionMenu(void);
void applyTrainingCourseActionMenuSelection(void);
void finishTrainingCourseFlow(void);
void fadeInTrainingCourseEndingDialog(void);
void waitForTrainingCourseEndingDialog(void);
void fadeOutTrainingCourseEndingDialog(void);
void exitTrainingCourseEndingDialog(void);
void returnToMainMenuFromModeRaceFlow(void);

#endif
