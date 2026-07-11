#ifndef MAIN_MENU_RACE_MODE_FLOW_H
#define MAIN_MENU_RACE_MODE_FLOW_H

#include "common.h"

void startMainMenuDemoRaceSelectionFlow(void);
void initMainMenuDemoRaceSelectionMenu(void);
void fadeInMainMenuDemoRaceSelectionMenu(void);
void updateMainMenuDemoRaceSelectionMenu(void);
void fadeOutMainMenuDemoRaceSelectionMenu(void);
void exitMainMenuDemoRaceSelectionMenu(void);
void initMainMenuDemoRace(void);
void fadeInMainMenuDemoRace(void);
void waitForMainMenuDemoRaceStart(void);
void zoomMainMenuDemoRaceViewport(void);
void runMainMenuDemoRace(void);
void fadeOutMainMenuDemoRace(void);
void returnToMainMenuDemoRaceSelectionMenu(void);
void startTrainingCourseFlow(void);
void queueTrainingCourseRaceInit(void);
void initTrainingCourseRace(void);
void fadeInTrainingCourseRace(void);
void waitForTrainingCourseStartSelection(void);
void zoomTrainingCourseRaceViewport(void);
void runTrainingCourseUntilActionPrompt(void);
void fadeInTrainingCourseActionMenu(void);
void waitForTrainingCourseActionMenuSelection(void);
void fadeOutTrainingCourseActionMenu(void);
void handleTrainingCourseActionMenuSelection(void);
void finishTrainingCourse(void);
void fadeInTrainingCourseEndingDialog(void);
void waitForTrainingCourseEndingDialog(void);
void fadeOutTrainingCourseEndingDialog(void);
void exitTrainingCourseEndingDialog(void);
void returnToMainMenuFromTrainingCourse(void);

#endif
