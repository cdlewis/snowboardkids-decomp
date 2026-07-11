#ifndef MAIN_MENU_RACE_FLOW_H
#define MAIN_MENU_RACE_FLOW_H

#include "common.h"

void startMainMenuRacePreviewModeSelectFlow(void);
void initMainMenuRacePreviewModeSelectMenu(void);
void fadeInMainMenuRacePreviewModeSelectMenu(void);
void updateMainMenuRacePreviewModeSelectMenu(void);
void fadeOutMainMenuRacePreviewModeSelectMenu(void);
void exitMainMenuRacePreviewModeSelectMenu(void);
void initMainMenuRacePreview(void);
void fadeInMainMenuRacePreview(void);
void waitForMainMenuRacePreviewSelection(void);
void zoomMainMenuRacePreviewViewport(void);
void runMainMenuRacePreview(void);
void fadeOutMainMenuRacePreview(void);
void returnToMainMenuRacePreviewModeSelectMenu(void);
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
void returnToMainMenuFromTrainingCourseFlow(void);

#endif
