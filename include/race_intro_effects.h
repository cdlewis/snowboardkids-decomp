#ifndef RACE_INTRO_EFFECTS_H
#define RACE_INTRO_EFFECTS_H

#include "common.h"

typedef struct RaceIntroCamera RaceIntroCamera;
typedef struct RaceIntroMeshActor RaceIntroMeshActor;

void drawRaceIntroModelMeshes(RaceIntroMeshActor *arg0);
void enqueueDrawRaceIntroModelMeshes(s32 arg0);
void initRaceIntroModelMeshes(RaceIntroMeshActor *arg0);
void drawRaceIntroBillboard(RaceIntroCamera *arg0);
void updateRaceIntroBillboard(RaceIntroCamera *arg0);
void waitRaceIntroBillboardSpawn(RaceIntroCamera *arg0);
void initRaceIntroBillboard(RaceIntroCamera *arg0);
void drawRaceIntroCamera(RaceIntroCamera *arg0);
void approachRaceIntroSpinStep(RaceIntroCamera *arg0, s16 arg1);
void approachRaceIntroVerticalVelocity(RaceIntroCamera *arg0, s32 arg1);
void approachRaceIntroOrbitRadius(RaceIntroCamera *arg0, s32 arg1);
void approachRaceIntroPitchVelocity(RaceIntroCamera *arg0, s16 arg1);
void updateRaceIntroCamera(RaceIntroCamera *arg0);
void updateRaceIntroCameraIdle(RaceIntroCamera *arg0);
void initRaceIntroCameraIdle(RaceIntroCamera *arg0);
void updateRaceIntroLongPanReturn(RaceIntroCamera *arg0);
void initRaceIntroLongPanReturn(RaceIntroCamera *arg0);
void updateRaceIntroLongPanHold(RaceIntroCamera *arg0);
void updateRaceIntroLongPanPitchUp(RaceIntroCamera *arg0);
void updateRaceIntroLongPanRise(RaceIntroCamera *arg0);
void waitRaceIntroLongPanTrigger(RaceIntroCamera *arg0);
void initRaceIntroLongPan(RaceIntroCamera *arg0);
void updateRaceIntroShortPanFinal(RaceIntroCamera *arg0);
void initRaceIntroShortPanFinal(RaceIntroCamera *arg0);
void waitRaceIntroShortPanFinal(RaceIntroCamera *arg0);
void initRaceIntroShortPanSecond(RaceIntroCamera *arg0);
void waitRaceIntroShortPanSecond(RaceIntroCamera *arg0);
void initRaceIntroShortPan(RaceIntroCamera *arg0);
void initRaceIntroCamera(RaceIntroCamera *arg0);
void drawRaceIntroAnimatedBillboards(RaceIntroMeshActor *arg0);
void enqueueDrawRaceIntroAnimatedBillboards(s32 arg0);
void initRaceIntroAnimatedBillboards(RaceIntroMeshActor *arg0);

#endif
