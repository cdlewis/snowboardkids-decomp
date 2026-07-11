#ifndef MAIN_MENU_MODE_FLOW_H
#define MAIN_MENU_MODE_FLOW_H

#include "common.h"

void startMainMenuModePreviewSelectFlow(void);
void initMainMenuModePreviewSelectMenu(void);
void fadeInMainMenuModePreviewSelectMenu(void);
void updateMainMenuModePreviewSelectMenu(void);
void fadeOutMainMenuModePreviewSelectMenu(void);
void exitMainMenuModePreviewSelectMenu(void);
void initMainMenuModePreviewRace(void);
void fadeInMainMenuModePreviewRace(void);
void waitForMainMenuModePreviewRaceSelection(void);
void zoomMainMenuModePreviewRaceViewport(void);
void runMainMenuModePreviewRace(void);
void fadeOutMainMenuModePreviewRace(void);
void returnToMainMenuModePreviewSelectMenu(void);
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
void returnToMainMenuFromModeFlow(void);

#endif
