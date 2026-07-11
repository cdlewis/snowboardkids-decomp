#ifndef MAIN_MENU_MODE_FLOW_H
#define MAIN_MENU_MODE_FLOW_H

#include "common.h"

void startMainMenuModeDemoFlow(void);
void initMainMenuModeDemoMenu(void);
void fadeInMainMenuModeDemoMenu(void);
void updateMainMenuModeDemoMenu(void);
void fadeOutMainMenuModeDemoMenu(void);
void exitMainMenuModeDemoMenu(void);
void initMainMenuModeDemoRace(void);
void fadeInMainMenuModeDemoRace(void);
void waitForMainMenuModeDemoRaceSelection(void);
void zoomMainMenuModeDemoRaceViewport(void);
void runMainMenuModeDemoRace(void);
void fadeOutMainMenuModeDemoRace(void);
void returnToMainMenuModeDemoMenu(void);
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
void returnFromMainMenuModeFlow(void);

#endif
