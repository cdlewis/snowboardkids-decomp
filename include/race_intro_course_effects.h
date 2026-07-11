#ifndef RACE_INTRO_COURSE_EFFECTS_H
#define RACE_INTRO_COURSE_EFFECTS_H

#include "common.h"

typedef struct RaceIntroFlyoverCamera RaceIntroFlyoverCamera;
typedef struct RaceIntroMeshActor RaceIntroMeshActor;

void drawRaceIntroModelMeshes(RaceIntroMeshActor *arg0);
void enqueueDrawRaceIntroModelMeshes(s32 arg0);
void initRaceIntroModelMeshes(RaceIntroMeshActor *arg0);
void drawRaceIntroBillboard(RaceIntroFlyoverCamera *arg0);
void updateRaceIntroBillboard(RaceIntroFlyoverCamera *arg0);
void waitRaceIntroBillboardSpawn(RaceIntroFlyoverCamera *arg0);
void initRaceIntroBillboard(RaceIntroFlyoverCamera *arg0);
void drawRaceIntroFlyoverCamera(RaceIntroFlyoverCamera *arg0);
void approachRaceIntroFlyoverSpinStep(RaceIntroFlyoverCamera *arg0, s16 arg1);
void approachRaceIntroFlyoverVerticalVelocity(RaceIntroFlyoverCamera *arg0, s32 arg1);
void approachRaceIntroFlyoverOrbitRadius(RaceIntroFlyoverCamera *arg0, s32 arg1);
void approachRaceIntroFlyoverPitchVelocity(RaceIntroFlyoverCamera *arg0, s16 arg1);
void updateRaceIntroFlyoverCamera(RaceIntroFlyoverCamera *arg0);
void updateRaceIntroFlyoverCameraIdle(RaceIntroFlyoverCamera *arg0);
void initRaceIntroFlyoverCameraIdle(RaceIntroFlyoverCamera *arg0);
void updateRaceIntroFlyoverLongPanReturn(RaceIntroFlyoverCamera *arg0);
void initRaceIntroFlyoverLongPanReturn(RaceIntroFlyoverCamera *arg0);
void updateRaceIntroFlyoverLongPanHold(RaceIntroFlyoverCamera *arg0);
void updateRaceIntroFlyoverLongPanPitchUp(RaceIntroFlyoverCamera *arg0);
void updateRaceIntroFlyoverLongPanRise(RaceIntroFlyoverCamera *arg0);
void waitRaceIntroFlyoverLongPanTrigger(RaceIntroFlyoverCamera *arg0);
void initRaceIntroFlyoverLongPan(RaceIntroFlyoverCamera *arg0);
void updateRaceIntroFlyoverShortPanFinal(RaceIntroFlyoverCamera *arg0);
void initRaceIntroFlyoverShortPanFinal(RaceIntroFlyoverCamera *arg0);
void waitRaceIntroFlyoverShortPanFinal(RaceIntroFlyoverCamera *arg0);
void initRaceIntroFlyoverShortPanSecond(RaceIntroFlyoverCamera *arg0);
void waitRaceIntroFlyoverShortPanSecond(RaceIntroFlyoverCamera *arg0);
void initRaceIntroFlyoverShortPan(RaceIntroFlyoverCamera *arg0);
void initRaceIntroFlyoverCamera(RaceIntroFlyoverCamera *arg0);
void drawRaceIntroAnimatedBillboards(RaceIntroMeshActor *arg0);
void enqueueDrawRaceIntroAnimatedBillboards(s32 arg0);
void initRaceIntroAnimatedBillboards(RaceIntroMeshActor *arg0);

#endif
