#ifndef RACE_COURSE_EFFECTS_H
#define RACE_COURSE_EFFECTS_H

#include "common.h"

struct RacePlayer;
struct RaceCountdownEffect;
struct RaceCourseBackdropEffect;
struct RaceCourseMarkerEffect;
struct RaceCourseRenderEffect;
struct RaceCourseTriggerEffect;
struct RaceMovingEffect;
struct RacePlayerEffect;
struct PatrolCourseObjectEffect;
struct CourseGateObjectEffect;

void drawRaceCountdownReadyPrompt(struct RaceCountdownEffect *arg0);
void drawRaceCountdownGoPrompt(struct RaceCountdownEffect *arg0);
void updateRaceCountdownGoPromptOut(struct RaceCountdownEffect *arg0);
void updateRaceCountdownGoPromptHold(struct RaceCountdownEffect *arg0);
void updateRaceCountdownReadyPromptIn(struct RaceCountdownEffect *arg0);
void updateRaceCountdownReadyPromptHold(struct RaceCountdownEffect *arg0);
void updateRaceCountdownInitialDelay(struct RaceCountdownEffect *arg0);
void initRaceCountdownPrompt(struct RaceCountdownEffect *arg0);
void renderRaceCourseModel(void *arg0);
void renderRaceCourseBackdrop(struct RaceCourseBackdropEffect *arg0);
void updateRaceCourseModelRenderTask(void *arg0);
void initRaceCourseModelRenderTask(void *arg0);
void drawFinalLapPromptForViewport(struct RacePlayerEffect *arg0);
void updateFinalLapPrompt(void *arg0);
void initFinalLapPrompt(void *arg0);
void renderCourseTextureMarkers(struct RaceCourseRenderEffect *arg0);
void updateCourseTextureMarkers(void *arg0);
void initCourseTextureMarkers(struct RaceCourseRenderEffect *arg0);
void renderRaceCourseSceneryObjects(struct RaceCourseRenderEffect *arg0);
void updateRaceCourseSceneryObjects(void *arg0);
void initRaceCourseSceneryObjects(struct RaceCourseRenderEffect *arg0);
void renderPatrolCourseObject(struct PatrolCourseObjectEffect *arg0);
void updatePatrolCourseObject(struct PatrolCourseObjectEffect *arg0);
void initPatrolCourseObject(struct PatrolCourseObjectEffect *arg0);
void spawnPatrolCourseObject(s16 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void renderLaunchRampCourseObject(struct RaceMovingEffect *arg0);
void updateLaunchRampCourseObjectExit(struct RaceMovingEffect *arg0);
void updateLaunchRampCourseObjectArc(struct RaceMovingEffect *arg0);
void initLaunchRampCourseObject(struct RaceMovingEffect *arg0);
void renderSpiralCourseObject(struct RaceMovingEffect *arg0);
void updateSpiralCourseObjectExit(struct RaceMovingEffect *arg0);
void updateSpiralCourseObjectTurn(struct RaceMovingEffect *arg0);
void updateSpiralCourseObjectLaunch(struct RaceMovingEffect *arg0);
void initSpiralCourseObject(struct RaceMovingEffect *arg0);
void renderCourseGateObject(struct CourseGateObjectEffect *arg0);
void updateCourseGateClosing(struct CourseGateObjectEffect *arg0);
void updateCourseGateOpening(struct CourseGateObjectEffect *arg0);
void waitForCourseGateTrigger(struct CourseGateObjectEffect *arg0);
void initCourseGateObject(struct CourseGateObjectEffect *arg0);
void renderCourseBillboardMarker(struct RaceCourseMarkerEffect *arg0);
void updateCourseBillboardMarker(struct RaceCourseMarkerEffect *arg0);
void initCourseBillboardMarker(struct RaceCourseMarkerEffect *arg0);
void renderCourseTriggerVolume(struct RaceCourseTriggerEffect *arg0);
void collidePlayerWithCourseTriggerVolume(struct RacePlayer *player, struct RaceCourseTriggerEffect *trigger);
void updateCourseTriggerVolume(struct RaceCourseTriggerEffect *arg0);
void initCourseTriggerVolume(struct RaceCourseTriggerEffect *arg0);

#endif
