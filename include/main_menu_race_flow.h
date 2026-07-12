#ifndef MAIN_MENU_RACE_FLOW_H
#define MAIN_MENU_RACE_FLOW_H

#include "common.h"

void startMainMenuModePreviewRaceFlow(void);
void initMainMenuModePreviewRaceSelectionMenu(void);
void fadeInMainMenuModePreviewRaceSelectionMenu(void);
void updateMainMenuModePreviewRaceSelectionMenu(void);
void fadeOutMainMenuModePreviewRaceSelectionMenu(void);
void exitMainMenuModePreviewRaceSelectionMenu(void);
void initMainMenuModePreviewRace(void);
void fadeInMainMenuModePreviewRace(void);
void waitForMainMenuModePreviewRaceStart(void);
void zoomMainMenuModePreviewRaceViewport(void);
void runMainMenuModePreviewRace(void);
void fadeOutMainMenuModePreviewRace(void);
void returnToMainMenuModePreviewRaceSelectionMenu(void);
void startTrainingCourseFlow(void);
void queueTrainingCourseRaceInit(void);
void initTrainingCourseRace(void);
void fadeInTrainingCourseRace(void);
void waitForTrainingCourseStartSelection(void);
void zoomTrainingCourseRaceViewport(void);
void runTrainingCourseUntilLessonEnd(void);
void fadeInTrainingCourseLessonEndMenu(void);
void waitForTrainingCourseLessonEndMenuSelection(void);
void fadeOutTrainingCourseLessonEndMenu(void);
void handleTrainingCourseLessonEndMenuSelection(void);
void finishTrainingCourse(void);
void fadeInTrainingCourseEndingDialog(void);
void waitForTrainingCourseEndingDialog(void);
void fadeOutTrainingCourseEndingDialog(void);
void exitTrainingCourseEndingDialog(void);
void returnToMainMenuFromTrainingCourse(void);

#endif
