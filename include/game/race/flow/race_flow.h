#ifndef RACE_FLOW_H
#define RACE_FLOW_H

#include "common.h"

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
void openRaceStartTransitionFlow(void);
void initRaceSceneFlow(void);
void fadeOutRaceStartTransitionFlow(void);
void fadeInRaceGameplayViewports(void);
void startRaceGameplayFlow(void);
void drawRacePauseMenu(void *arg0);
void updateRaceGameplayFlow(void);
void waitRaceFinishResultsFlow(void);
void interpolateRaceViewport(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6,
                  f32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11, s32 arg12, s32 arg13,
                  f32 arg14);
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
