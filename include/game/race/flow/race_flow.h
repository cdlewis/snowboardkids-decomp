#ifndef RACE_FLOW_H
#define RACE_FLOW_H

#include "common.h"

typedef struct {
    /* 0x00 */ s16 status;
    /* 0x02 */ s16 courseId;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
} CourseGridEntry;

typedef struct {
    /* 0x00 */ u8 boardPropertiesIndex;
    /* 0x01 */ u8 snowboardTextureIndex;
} RaceCpuBoardConfig;

typedef struct {
    u8 characterIds[10 * 4];
} RaceCourseCharacterIdTable;

extern u8 gMainMenuModeSelection;
extern u8 gPendingEndingCreditsFlow;
extern s32 gRaceCameraReplayStartX;
extern s32 gRaceCameraReplayStartY;
extern s32 gRaceCameraReplayStartZ;
extern s16 gRaceLapCount;
extern s16 gRacePlayerAttackStartTimer;
extern s8 gRacePlayerCount;
extern u8 gRaceUpdatePaused;
extern u8 gTrainingCourseLesson;

#if defined(VERSION_JP)
extern u8 gRaceCourseCpuPickupUseChances[];
extern RaceCourseCharacterIdTable gRaceCourseOpponentCharacterIds;
extern RaceCpuBoardConfig *gRaceCpuBoardConfigsByCourse[];
extern s8 gRaceGhostRecordStatus;
extern u16 gRaceGhostReplayGridSwitchFrames[];
extern u8 gRacePauseMenuSelection;
extern CourseGridEntry *gRaceReplayCourseGridLists[];
extern u8 gRaceResultsAwardPlayerNumber;
#endif

void initStartupControllerPakFlow(void);
void waitStartupRumbleInit(void);
void routeStartupControllerPakFlow(void);
void openStartupControllerPakFileDeleteFlow(void);
void openStartupReplaySaveMessageFlow(void);
void enterMainMenuAfterStartupControllerPakFlow(void);
void clearRaceReplayCourseGrid(void);
void saveRaceReplayCourseGridEntry(void);
s32 loadNextRaceReplayCourseGridEntry(void);
s32 hasPendingRaceReplayCourseGridEntry(void);
void initNewGameSaveData(void);
void openRaceSetupMenuFlow(void);
void openRaceCharacterSelectMenuFlow(void);
void routeRaceCharacterSetupFlow(void);
void openPlayerSelectCourseMenuFlow(void);
void handlePlayerSelectCourseMenuFlow(void);
void openRaceCourseMenuFlow(void);
void openRaceSplitscreenSelectFlow(void);
void handleRaceSplitscreenSelectFlow(void);
void openRaceTypeSelectFlow(void);
void handleRaceTypeSelectFlow(void);
void openSinglePlayerCourseSelectFlow(void);
void openRaceRecordSaveOptionsFlow(void);
void handleRaceRecordSaveOptionsFlow(void);
void openMultiplayerCourseSelectFlow(void);
void handleMultiplayerCourseSelectFlow(void);
void exitRaceFlowToMainMenu(void);
void openPostRaceControllerPakContinuePromptFlow(void);
void handleControllerPakContinuePromptFlow(void);
void openEndingCreditsIfUnlockedFlow(void);
void openPassAwardCelebrationFlow(void);
void initRaceSceneFlow(void);
void fadeOutPassAwardCelebrationFlow(void);
void fadeInRaceGameplayViewports(void);
void startRaceGameplayFlow(void);
void drawRacePauseMenu(void *arg0);
void updateRaceGameplayFlow(void);
void waitRaceFinishResultsFlow(void);
void interpolateRaceViewport(
    s32 arg0,
    s32 arg1,
    s32 arg2,
    s32 arg3,
    s32 arg4,
    s32 arg5,
    s32 arg6,
    f32 arg7,
    s32 arg8,
    s32 arg9,
    s32 arg10,
    s32 arg11,
    s32 arg12,
    s32 arg13,
    f32 arg14
);
void zoomRaceWinnerViewport(void);
void prepareRaceResultsFlow(void);
void updateRaceResultsFlow(void);
void updateRaceResultsMusicFlow(void);
void fadeOutRaceResultsFlow(void);
void initRaceGhostReplayFlow(void);
void updateRaceGhostReplayFlow(void);
void fadeOutRaceGhostReplayFlow(void);
void finalizeRaceExitFlow(void);
void updateRaceFlowFrameWithCourseEffects(void);
void updateRaceFlowFrame(void);
void updateRaceReplayFrame(void);
s32 areRacePlayersFinished(void);
void initRaceRecordSettingsFlow(void);
void updateRaceRecordSettingsFlow(void);
void fadeOutRaceRecordSettingsFlow(void);
void closeRaceRecordSettingsFlow(void);

#endif
