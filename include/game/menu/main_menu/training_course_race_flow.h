#ifndef TRAINING_COURSE_RACE_FLOW_H
#define TRAINING_COURSE_RACE_FLOW_H

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
