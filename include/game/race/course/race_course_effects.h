#ifndef RACE_COURSE_EFFECTS_H
#define RACE_COURSE_EFFECTS_H

#include "common.h"
#include "game/engine/callback_task_scheduler.h"

struct RacePlayer;

typedef struct RaceCountdownEffect {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 scaleStep;
    /* 0x1A */ u16 timer;
} RaceCountdownEffect;

typedef struct RaceCourseModelRenderEffect {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ u8 padding18[0x2C];
    /* 0x44 */ Mtx *backdropMatrix;
} RaceCourseModelRenderEffect;

typedef struct RaceCourseObjectMatrixEffect {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Mtx *matrices;
} RaceCourseObjectMatrixEffect;

typedef struct RaceCourseSceneryEntry {
    /* 0x00 */ s16 modelIndex;
    /* 0x02 */ s16 padding02;
    /* 0x04 */ Vec3i position;
    /* 0x10 */ s16 rotation;
    /* 0x12 */ s16 padding12;
} RaceCourseSceneryEntry;

typedef struct RaceCourseTextureMarkerEntry {
    /* 0x00 */ s8 type;
    /* 0x01 */ u8 padding01[3];
    /* 0x04 */ Vec3i position;
} RaceCourseTextureMarkerEntry;

typedef struct PatrolCourseObjectEffect {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i position;
    /* 0x24 */ Vec3i startPosition;
    /* 0x30 */ Vec3i endPosition;
    /* 0x3C */ s16 surfaceIndex;
    /* 0x3E */ s16 angle;
    /* 0x40 */ s16 animationPhase;
    /* 0x42 */ s16 movingTowardStart;
    /* 0x44 */ s16 matrixValid;
    /* 0x46 */ s16 padding46;
    /* 0x48 */ Mtx *matrix;
    /* 0x4C */ s16 maxTurnStep;
    /* 0x4E */ s16 animationPhaseStep;
    /* 0x50 */ s32 speed;
} PatrolCourseObjectEffect;

typedef struct RaceCourseGateEffect {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Transform3D baseTransform;
    /* 0x38 */ Vec3i firstPanelPosition;
    /* 0x44 */ Vec3i secondPanelPosition;
    /* 0x50 */ s16 firstPanelAngle;
    /* 0x52 */ s16 unusedAngle;
    /* 0x54 */ s16 openTimer;
    /* 0x56 */ s16 isOpen;
    /* 0x58 */ Mtx *baseMatrix;
    /* 0x5C */ Mtx *firstPanelMatrix;
    /* 0x60 */ Mtx *secondPanelMatrix;
} RaceCourseGateEffect;

typedef struct RaceMovingCourseObjectEffect {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i velocity;
    /* 0x24 */ Vec3i position;
    /* 0x30 */ Transform3D movementTransform;
    /* 0x50 */ s16 timer;
    /* 0x52 */ s16 yaw;
    /* 0x54 */ Mtx *matrix;
} RaceMovingCourseObjectEffect;

typedef struct RaceCourseBillboardEffect {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ void *texture;
    /* 0x1C */ void *palette;
    /* 0x20 */ s16 textureScroll;
    /* 0x22 */ u8 padding22[2];
    /* 0x24 */ Vtx *vertices;
    /* 0x28 */ Vtx *baseVertices;
    /* 0x2C */ s16 vertexCount;
    /* 0x2E */ u8 padding2E[2];
    /* 0x30 */ u32 renderSetupDisplayListAddress;
    /* 0x34 */ u32 geometryDisplayListAddress;
    /* 0x38 */ s32 useAlternateRenderQueue;
    /* 0x3C */ s32 renderFlags;
} RaceCourseBillboardEffect;

typedef struct RaceCourseBillboardEntry {
    /* 0x00 */ u32 renderSetupDisplayListAddress;
    /* 0x04 */ u32 geometryDisplayListAddress;
    /* 0x08 */ u32 baseVerticesAddress;
    /* 0x0C */ s16 vertexCount;
    /* 0x0E */ s16 flags;
    /* 0x10 */ u16 textureIndex;
    /* 0x12 */ s16 padding12;
} RaceCourseBillboardEntry;

typedef struct RaceCourseTriggerEntry {
    /* 0x00 */ s16 halfWidth;
    /* 0x02 */ s16 height;
    /* 0x04 */ s16 halfDepth;
    /* 0x06 */ s16 padding06;
    /* 0x08 */ Vec3i position;
    /* 0x14 */ s16 pitch;
    /* 0x16 */ s16 yaw;
    /* 0x18 */ u32 displayListAddress;
} RaceCourseTriggerEntry;

typedef struct RaceCourseTriggerEffect {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Mtx *matrix;
    /* 0x1C */ Vec3i positiveDepthCollisionCenter;
    /* 0x28 */ Vec3i negativeDepthCollisionCenter;
    /* 0x34 */ s16 pitch;
    /* 0x36 */ s16 yaw;
    /* 0x38 */ s32 halfWidth;
    /* 0x3C */ s32 height;
    /* 0x40 */ s32 halfDepth;
    /* 0x44 */ u32 displayListAddress;
} RaceCourseTriggerEffect;

typedef struct RaceCourseGateEntry {
    /* 0x00 */ Vec3i position;
    /* 0x0C */ s16 angle;
    /* 0x0E */ s16 padding0E;
} RaceCourseGateEntry;

void drawRaceCountdownReadyPrompt(RaceCountdownEffect *effect);
void drawRaceCountdownGoPrompt(RaceCountdownEffect *effect);
void updateRaceCountdownGoPromptOut(RaceCountdownEffect *effect);
void updateRaceCountdownGoPromptHold(RaceCountdownEffect *effect);
void updateRaceCountdownReadyPromptIn(RaceCountdownEffect *effect);
void updateRaceCountdownReadyPromptHold(RaceCountdownEffect *effect);
void updateRaceCountdownInitialDelay(RaceCountdownEffect *effect);
void initRaceCountdownPrompt(RaceCountdownEffect *effect);
void renderRaceCourseModel(RaceCourseModelRenderEffect *effect);
void renderRaceCourseBackdrop(RaceCourseModelRenderEffect *effect);
void updateRaceCourseModelRenderTask(RaceCourseModelRenderEffect *effect);
void initRaceCourseModelRenderTask(RaceCourseModelRenderEffect *effect);
void drawFinalLapPromptForViewport(CallbackTask *task);
void updateFinalLapPrompt(CallbackTask *task);
void initFinalLapPrompt(CallbackTask *task);
void renderCourseTextureMarkers(RaceCourseObjectMatrixEffect *effect);
void updateCourseTextureMarkers(RaceCourseObjectMatrixEffect *effect);
void initCourseTextureMarkers(RaceCourseObjectMatrixEffect *effect);
void renderRaceCourseSceneryObjects(RaceCourseObjectMatrixEffect *effect);
void updateRaceCourseSceneryObjects(RaceCourseObjectMatrixEffect *effect);
void initRaceCourseSceneryObjects(RaceCourseObjectMatrixEffect *effect);
void renderPatrolCourseObject(PatrolCourseObjectEffect *effect);
void updatePatrolCourseObject(PatrolCourseObjectEffect *effect);
void initPatrolCourseObject(PatrolCourseObjectEffect *effect);
void spawnPatrolCourseObject(s16 surfaceIndex, s32 startX, s32 startZ, s32 endX, s32 endZ);
void renderLaunchRampCourseObject(RaceMovingCourseObjectEffect *effect);
void updateLaunchRampCourseObjectExit(RaceMovingCourseObjectEffect *effect);
void updateLaunchRampCourseObjectArc(RaceMovingCourseObjectEffect *effect);
void initLaunchRampCourseObject(RaceMovingCourseObjectEffect *effect);
void renderSpiralCourseObject(RaceMovingCourseObjectEffect *effect);
void updateSpiralCourseObjectExit(RaceMovingCourseObjectEffect *effect);
void updateSpiralCourseObjectTurn(RaceMovingCourseObjectEffect *effect);
void updateSpiralCourseObjectLaunch(RaceMovingCourseObjectEffect *effect);
void initSpiralCourseObject(RaceMovingCourseObjectEffect *effect);
void renderCourseGateObject(RaceCourseGateEffect *effect);
void updateCourseGateClosing(RaceCourseGateEffect *effect);
void updateCourseGateOpening(RaceCourseGateEffect *effect);
void waitForCourseGateTrigger(RaceCourseGateEffect *effect);
void initCourseGateObject(RaceCourseGateEffect *effect);
void renderCourseBillboardMarker(RaceCourseBillboardEffect *effect);
void updateCourseBillboardMarker(RaceCourseBillboardEffect *effect);
void initCourseBillboardMarker(RaceCourseBillboardEffect *effect);
void renderCourseTriggerVolume(RaceCourseTriggerEffect *effect);
void collidePlayerWithCourseTriggerVolume(struct RacePlayer *player, RaceCourseTriggerEffect *trigger);
void updateCourseTriggerVolume(RaceCourseTriggerEffect *effect);
void initCourseTriggerVolume(RaceCourseTriggerEffect *effect);

#endif
