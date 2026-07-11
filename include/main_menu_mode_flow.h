#ifndef MAIN_MENU_MODE_FLOW_H
#define MAIN_MENU_MODE_FLOW_H

#include "common.h"

void startMainMenuModePreviewFlow(void);
void initMainMenuModePreviewMenu(void);
void fadeInMainMenuModePreviewMenu(void);
void updateMainMenuModePreviewMenu(void);
void fadeOutMainMenuModePreviewMenu(void);
void exitMainMenuModePreviewMenu(void);
void initMainMenuModePreviewRace(void);
void fadeInMainMenuModePreviewRace(void);
void waitForMainMenuModePreviewRaceSelection(void);
void zoomMainMenuModePreviewRaceViewport(void);
void runMainMenuModePreviewRace(void);
void fadeOutMainMenuModePreviewRace(void);
void returnToMainMenuModePreviewMenu(void);
void startTrainingCourseFlow(void);
void deferTrainingCourseInit(void);
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
