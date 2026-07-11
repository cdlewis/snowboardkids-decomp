#ifndef RACE_COURSE_PREVIEW_H
#define RACE_COURSE_PREVIEW_H

#include "common.h"

typedef struct RaceCoursePreviewCamera RaceCoursePreviewCamera;
typedef struct RaceCoursePreviewMeshActor RaceCoursePreviewMeshActor;

void drawRaceCoursePreviewModelMeshes(RaceCoursePreviewMeshActor *arg0);
void enqueueDrawRaceCoursePreviewModelMeshes(s32 arg0);
void initRaceCoursePreviewModelMeshes(RaceCoursePreviewMeshActor *arg0);
void drawRaceCoursePreviewBillboard(RaceCoursePreviewCamera *arg0);
void updateRaceCoursePreviewBillboard(RaceCoursePreviewCamera *arg0);
void waitRaceCoursePreviewBillboardSpawn(RaceCoursePreviewCamera *arg0);
void initRaceCoursePreviewBillboard(RaceCoursePreviewCamera *arg0);
void drawRaceCoursePreviewCameraModel(RaceCoursePreviewCamera *arg0);
void approachRaceCoursePreviewSpinStep(RaceCoursePreviewCamera *arg0, s16 arg1);
void approachRaceCoursePreviewVerticalVelocity(RaceCoursePreviewCamera *arg0, s32 arg1);
void approachRaceCoursePreviewOrbitRadius(RaceCoursePreviewCamera *arg0, s32 arg1);
void approachRaceCoursePreviewPitchVelocity(RaceCoursePreviewCamera *arg0, s16 arg1);
void updateRaceCoursePreviewCameraMotion(RaceCoursePreviewCamera *arg0);
void updateRaceCoursePreviewCameraIdle(RaceCoursePreviewCamera *arg0);
void initRaceCoursePreviewCameraIdle(RaceCoursePreviewCamera *arg0);
void updateRaceCoursePreviewLongPanReturn(RaceCoursePreviewCamera *arg0);
void initRaceCoursePreviewLongPanReturn(RaceCoursePreviewCamera *arg0);
void updateRaceCoursePreviewLongPanHold(RaceCoursePreviewCamera *arg0);
void updateRaceCoursePreviewLongPanPitchUp(RaceCoursePreviewCamera *arg0);
void updateRaceCoursePreviewLongPanRise(RaceCoursePreviewCamera *arg0);
void waitRaceCoursePreviewLongPanTrigger(RaceCoursePreviewCamera *arg0);
void initRaceCoursePreviewLongPan(RaceCoursePreviewCamera *arg0);
void updateRaceCoursePreviewShortPanFinal(RaceCoursePreviewCamera *arg0);
void initRaceCoursePreviewShortPanFinal(RaceCoursePreviewCamera *arg0);
void waitRaceCoursePreviewShortPanFinal(RaceCoursePreviewCamera *arg0);
void initRaceCoursePreviewShortPanSecond(RaceCoursePreviewCamera *arg0);
void waitRaceCoursePreviewShortPanSecond(RaceCoursePreviewCamera *arg0);
void initRaceCoursePreviewShortPan(RaceCoursePreviewCamera *arg0);
void initRaceCoursePreviewCameraCutscene(RaceCoursePreviewCamera *arg0);
void drawRaceCoursePreviewAnimatedBillboards(RaceCoursePreviewMeshActor *arg0);
void enqueueDrawRaceCoursePreviewAnimatedBillboards(s32 arg0);
void initRaceCoursePreviewAnimatedBillboards(RaceCoursePreviewMeshActor *arg0);

#endif
