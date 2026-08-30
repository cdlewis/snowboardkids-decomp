#ifndef RACE_INTRO_EFFECTS_H
#define RACE_INTRO_EFFECTS_H

#include "common.h"
#include "game/engine/callback_task_scheduler.h"

typedef union RaceIntroPackedAngles {
    s32 word;
    struct {
        s16 yaw;
        s16 pitch;
    } half;
} RaceIntroPackedAngles;

typedef struct RaceIntroMeshActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Mtx *matrices;
} RaceIntroMeshActor; // size = 0x1C

typedef struct RaceIntroEffectActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i position;
    /* 0x24 */ s32 velocityY;
    /* 0x28 */ s32 radius;
    /* 0x2C */ RaceIntroPackedAngles angle;
    union {
        struct {
            /* 0x30 */ s16 scale;
            /* 0x32 */ s16 tilt;
            /* 0x34 */ s16 pitchVelocity;
            /* 0x36 */ s16 spinVelocity;
        };
        struct {
            /* 0x30 */ void *image;
            /* 0x34 */ void *palette;
        };
    };
    /* 0x38 */ s16 timer;
    /* 0x3A */ s16 stateTimer;
    /* 0x3C */ Mtx *displayList0;
    union {
        /* 0x40 */ Mtx *displayList1;
        /* 0x40 */ s8 displayList0Valid;
    };
    /* 0x44 */ s8 displayListValid;
} RaceIntroEffectActor; // size = 0x48

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
