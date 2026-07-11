#ifndef MAIN_MENU_MODE_FLOW_H
#define MAIN_MENU_MODE_FLOW_H

#include "common.h"

void startMainMenuModePreviewRaceFlow(void);
void initMainMenuModePreviewRaceMenu(void);
void fadeInMainMenuModePreviewRaceMenu(void);
void updateMainMenuModePreviewRaceMenu(void);
void fadeOutMainMenuModePreviewRaceMenu(void);
void exitMainMenuModePreviewRaceMenu(void);
void initMainMenuModePreviewRace(void);
void fadeInMainMenuModePreviewRace(void);
void waitForMainMenuModePreviewRaceSelection(void);
void zoomMainMenuModePreviewRaceViewport(void);
void runMainMenuModePreviewRace(void);
void fadeOutMainMenuModePreviewRace(void);
void returnToMainMenuModePreviewRaceMenu(void);
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
