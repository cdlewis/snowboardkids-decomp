#ifndef RACE_INTRO_EFFECTS_H
#define RACE_INTRO_EFFECTS_H

#include "common.h"

typedef struct RaceIntroEffectActor RaceIntroEffectActor;
typedef struct RaceIntroMeshActor RaceIntroMeshActor;

void drawRaceIntroModelMeshes(RaceIntroMeshActor *arg0);
void enqueueDrawRaceIntroModelMeshes(RaceIntroMeshActor *arg0);
void initRaceIntroModelMeshes(RaceIntroMeshActor *arg0);
void drawRaceIntroBillboard(RaceIntroEffectActor *arg0);
void updateRaceIntroBillboard(RaceIntroEffectActor *arg0);
void waitRaceIntroBillboardSpawn(RaceIntroEffectActor *arg0);
void initRaceIntroBillboard(RaceIntroEffectActor *arg0);
void drawRaceIntroFlyoverActor(RaceIntroEffectActor *arg0);
void approachRaceIntroFlyoverSpinStep(RaceIntroEffectActor *arg0, s16 arg1);
void approachRaceIntroFlyoverVerticalVelocity(RaceIntroEffectActor *arg0, s32 arg1);
void approachRaceIntroFlyoverOrbitRadius(RaceIntroEffectActor *arg0, s32 arg1);
void approachRaceIntroFlyoverPitchVelocity(RaceIntroEffectActor *arg0, s16 arg1);
void updateRaceIntroFlyoverActor(RaceIntroEffectActor *arg0);
void updateRaceIntroFlyoverIdle(RaceIntroEffectActor *arg0);
void initRaceIntroFlyoverIdle(RaceIntroEffectActor *arg0);
void updateRaceIntroFlyoverLongPanReturn(RaceIntroEffectActor *arg0);
void initRaceIntroFlyoverLongPanReturn(RaceIntroEffectActor *arg0);
void updateRaceIntroFlyoverLongPanHold(RaceIntroEffectActor *arg0);
void updateRaceIntroFlyoverLongPanPitchUp(RaceIntroEffectActor *arg0);
void updateRaceIntroFlyoverLongPanRise(RaceIntroEffectActor *arg0);
void waitRaceIntroFlyoverLongPanTrigger(RaceIntroEffectActor *arg0);
void initRaceIntroFlyoverLongPan(RaceIntroEffectActor *arg0);
void updateRaceIntroFlyoverShortPanFinal(RaceIntroEffectActor *arg0);
void initRaceIntroFlyoverShortPanFinal(RaceIntroEffectActor *arg0);
void waitRaceIntroFlyoverShortPanFinal(RaceIntroEffectActor *arg0);
void initRaceIntroFlyoverShortPanSecond(RaceIntroEffectActor *arg0);
void waitRaceIntroFlyoverShortPanSecond(RaceIntroEffectActor *arg0);
void initRaceIntroFlyoverShortPan(RaceIntroEffectActor *arg0);
void initRaceIntroFlyoverActor(RaceIntroEffectActor *arg0);
void drawRaceIntroAnimatedBillboards(RaceIntroMeshActor *arg0);
void enqueueDrawRaceIntroAnimatedBillboards(RaceIntroMeshActor *arg0);
void initRaceIntroAnimatedBillboards(RaceIntroMeshActor *arg0);

#endif
