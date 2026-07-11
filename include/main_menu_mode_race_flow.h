#ifndef MAIN_MENU_MODE_RACE_FLOW_H
#define MAIN_MENU_MODE_RACE_FLOW_H

#include "common.h"

void startMainMenuModeDemoRaceFlow(void);
void initMainMenuModeDemoRaceMenu(void);
void fadeInMainMenuModeDemoRaceMenu(void);
void updateMainMenuModeDemoRaceMenu(void);
void fadeOutMainMenuModeDemoRaceMenu(void);
void exitMainMenuModeDemoRaceMenu(void);
void initMainMenuModeDemoRace(void);
void fadeInMainMenuModeDemoRace(void);
void waitForMainMenuModeDemoRaceSelection(void);
void zoomMainMenuModeDemoRaceViewport(void);
void runMainMenuModeDemoRace(void);
void fadeOutMainMenuModeDemoRace(void);
void returnToMainMenuModeDemoRaceMenu(void);
void startTrainingCourseFlow(void);
void deferTrainingCourseRaceInit(void);
void initTrainingCourseRace(void);
void fadeInTrainingCourseRace(void);
void waitForTrainingCourseStartSelection(void);
void zoomTrainingCourseRaceViewport(void);
void runTrainingCourseUntilPrompt(void);
void fadeInTrainingCourseActionMenu(void);
void waitForTrainingCourseActionMenuSelection(void);
void fadeOutTrainingCourseActionMenu(void);
void applyTrainingCourseActionMenuSelection(void);
void finishOrAdvanceTrainingCourse(void);
void fadeInTrainingCourseEndingDialog(void);
void waitForTrainingCourseEndingDialog(void);
void fadeOutTrainingCourseEndingDialog(void);
void exitTrainingCourseEndingDialog(void);
void returnFromMainMenuModeRaceFlow(void);

#endif
