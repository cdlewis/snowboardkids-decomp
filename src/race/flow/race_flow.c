#include "common.h"
#include "game/save_data.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/engine/render_callback.h"
#include "assets.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/asset_manager.h"
#include "game/engine/system_runtime.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/menu/character_select/character_select_menu.h"
#include "game/menu/course_select/multiplayer_course_select_menu.h"
#include "game/menu/controller_pak/controller_pak_race_record_save_flow.h"
#include "game/menu/controller_pak/controller_pak_replay_save_message_flow.h"
#include "game/menu/controller_pak/controller_pak_file_delete_flow.h"
#include "game/menu/course_select/course_select_menu.h"
#include "game/menu/race_setup/race_setup_menu.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/renderer/menu_screen_effects.h"
#include "game/menu/main_menu/main_menu_panel_ui.h"
#include "game/menu/race_type_select/race_type_select_menu.h"
#include "game/menu/splitscreen_select/race_splitscreen_select_menu.h"
#include "game/race/camera/race_camera.h"
#include "game/race/course/race_course_effects.h"
#include "game/race/flow/race_flow.h"
#include "game/race/race_state.h"
#include "game/race/race_replay.h"
#include "game/race/player/race_player_update.h"
#include "game/race/scene/race_scene_setup.h"
#include "game/race/effects/race_start_transition.h"
#include "game/race/ui/race_hud.h"
#include "game/ending/ending_credits_flow.h"
#include "game/race/ui/race_ui_effects.h"
#include "game/menu/controller_pak/controller_pak_continue_prompt_flow.h"
#include "game/menu/race_setup/race_setup_ui.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"

#define COURSE_GRID_ENTRY_FREE -1

#define COURSE_GRID_ENTRY_END -2

#define COURSE_TIME_MINUTE 0x177000

#define COURSE_TIME_SECOND 0x6400

#define RACE_PLAYER_READY_FLAG 0x40

typedef union {
    s32 value;
    u8 padding[12];
} RaceResultsReadyState;

typedef struct {
    /* 0x00 */ s32 unk0;
#if defined(VERSION_JP)
    u8 pad4[4];
#else
    /* 0x04 */ u8 pad4[0x8 - 0x4];
#endif
    /* 0x08 */ s8 unk8;
} Unk80043040;

#if defined(VERSION_JP)
extern u32 gRaceReplayCourseGridEntries[];
#endif

CourseGridEntry *gRaceReplayCourseGridLists[12] = {
    (CourseGridEntry *)gRaceReplayCourseGridEntries,
    &((CourseGridEntry *)gRaceReplayCourseGridEntries)[12],
    &((CourseGridEntry *)gRaceReplayCourseGridEntries)[24],
    &((CourseGridEntry *)gRaceReplayCourseGridEntries)[39],
    &((CourseGridEntry *)gRaceReplayCourseGridEntries)[54],
    &((CourseGridEntry *)gRaceReplayCourseGridEntries)[67],
    &((CourseGridEntry *)gRaceReplayCourseGridEntries)[83],
    &((CourseGridEntry *)gRaceReplayCourseGridEntries)[119],
    &((CourseGridEntry *)gRaceReplayCourseGridEntries)[107],
    &((CourseGridEntry *)gRaceReplayCourseGridEntries)[95],
    NULL,
    NULL,
};

u8 gPendingEndingCreditsFlow = 0;
RaceCourseCharacterIdTable gRaceCourseOpponentCharacterIds = {
    {
     2, 3, 4, 0, 0, 1, 2, 3, 1, 2, 3, 4, 0, 3, 4, 1, 0, 2, 3, 4,
     0, 1, 2, 3, 1, 3, 4, 0, 0, 1, 2, 3, 5, 0, 1, 3, 0, 1, 4, 2,
     },
};
RaceCpuBoardConfig gBigSnowmanCpuBoardConfigs[8] = {
    { 1,  0 },
    { 1,  1 },
    { 1,  2 },
    { 1,  3 },
    { 1,  4 },
    { 14, 5 },
    { 1,  6 },
    { 0,  0 },
};
RaceCpuBoardConfig gSunsetRockCpuBoardConfigs[8] = {
    { 1, 0 },
    { 1, 1 },
    { 1, 2 },
    { 1, 3 },
    { 1, 4 },
    { 1, 5 },
    { 1, 6 },
    { 0, 0 },
};
RaceCpuBoardConfig gNightHighwayCpuBoardConfigs[8] = {
    { 4, 0 },
    { 4, 1 },
    { 4, 2 },
    { 4, 3 },
    { 4, 4 },
    { 4, 5 },
    { 4, 6 },
    { 0, 0 },
};
RaceCpuBoardConfig gGrassValleyCpuBoardConfigs[8] = {
    { 4, 0 },
    { 4, 1 },
    { 4, 2 },
    { 4, 3 },
    { 4, 4 },
    { 4, 5 },
    { 4, 6 },
    { 0, 0 },
};
#if defined(VERSION_JP)

RaceCpuBoardConfig gDizzyLandCpuBoardConfigs[8] = { { 4, 0 }, { 4, 1 }, { 4, 2 }, { 4, 3 }, { 4, 4 }, { 4, 5 }, { 4, 6 }, { 0, 0 } };

RaceCpuBoardConfig gQuicksandValleyCpuBoardConfigs[8] = { { 7, 0 }, { 7, 1 }, { 7, 2 }, { 7, 3 }, { 7, 4 }, { 7, 5 }, { 7, 6 }, { 0, 0 } };

RaceCpuBoardConfig gSilverMountainCpuBoardConfigs[8] = { { 7, 0 }, { 7, 1 }, { 7, 2 }, { 7, 3 }, { 7, 4 }, { 7, 5 }, { 7, 6 }, { 0, 0 } };

#else
RaceCpuBoardConfig gDizzyLandCpuBoardConfigs[8] = {
    { 7, 0 },
    { 7, 1 },
    { 7, 2 },
    { 7, 3 },
    { 7, 4 },
    { 7, 5 },
    { 7, 6 },
    { 0, 0 },
};
RaceCpuBoardConfig gQuicksandValleyCpuBoardConfigs[8] = {
    { 8, 0 },
    { 8, 1 },
    { 8, 2 },
    { 8, 3 },
    { 8, 4 },
    { 8, 5 },
    { 8, 6 },
    { 0, 0 },
};
RaceCpuBoardConfig gSilverMountainCpuBoardConfigs[8] = {
    { 8, 0 },
    { 8, 1 },
    { 8, 2 },
    { 8, 3 },
    { 8, 4 },
    { 8, 5 },
    { 8, 6 },
    { 0, 0 },
};
#endif
RaceCpuBoardConfig gAnimalLandCpuBoardConfigs[8] = {
    { 0, 0 },
    { 0, 1 },
    { 0, 2 },
    { 0, 3 },
    { 0, 4 },
    { 0, 5 },
    { 0, 6 },
    { 0, 0 },
};
#if defined(VERSION_JP)

RaceCpuBoardConfig gNinjaLandCpuBoardConfigs[8] = { { 7, 0 }, { 7, 1 }, { 7, 2 }, { 7, 3 }, { 7, 4 }, { 14, 5 }, { 7, 6 }, { 0, 0 } };

#else
RaceCpuBoardConfig gNinjaLandCpuBoardConfigs[8] = {
    { 8,  0 },
    { 8,  1 },
    { 8,  2 },
    { 8,  3 },
    { 8,  4 },
    { 14, 5 },
    { 8,  6 },
    { 0,  0 },
};
#endif
RaceCpuBoardConfig gRookieMountainCpuBoardConfigs[8] = {
    { 1, 0 },
    { 1, 1 },
    { 1, 2 },
    { 1, 3 },
    { 1, 4 },
    { 1, 5 },
    { 1, 6 },
    { 0, 0 },
};
/* Fixed course/character board setup, independent of dynamic rank assistance.
 * Silver Mountain (6): board properties index 8; Animal Land (7): board properties index 0;
 * Ninja Land (8): board properties index 8, except character 5 uses 14. */
/* Indexed by [course][character]. boardPropertiesIndex selects CPU board tuning:
 * Silver Mountain uses 8; Ninja Land uses 8 except character 5 uses 14;
 * Animal Land uses 0. Separate from rank assistance and updateRacePlayer's
 * fixed CPU target-speed addition of 0x6000. */
RaceCpuBoardConfig *gRaceCpuBoardConfigsByCourse[10] = {
    gBigSnowmanCpuBoardConfigs,     gSunsetRockCpuBoardConfigs, gNightHighwayCpuBoardConfigs,
    gGrassValleyCpuBoardConfigs,    gDizzyLandCpuBoardConfigs,  gQuicksandValleyCpuBoardConfigs,
    gSilverMountainCpuBoardConfigs, gAnimalLandCpuBoardConfigs, gNinjaLandCpuBoardConfigs,
    gRookieMountainCpuBoardConfigs,
};
/* Shared action/item trigger chance: indices 0..8 use 100; Rookie Mountain (9)
 * uses 10. Animal Land (7) is the Trick Game course. */
#if defined(VERSION_JP)
u8 gRaceCourseCpuPickupUseChances[12] = { 10, 10, 40, 40, 40, 80, 128, 0, 255, 10, 0, 0 };

#else
u8 gRaceCourseCpuPickupUseChances[12] = { 100, 100, 100, 100, 100, 100, 100, 100, 100, 10, 0, 0 };
#endif
u16 gRaceGhostReplayGridSwitchFrames[16] = {
    0x0000, 0x0068, 0x00CD, 0x0135, 0x019C, 0x0203, 0x0268, 0x02CE,
    0x0335, 0x03A0, 0x040C, 0x0466, 0x7FFF, 0x0000, 0x0000, 0x0000,
};

s32 gRaceCameraReplayStartX;
s32 gRaceCameraReplayStartY;
s32 gRaceCameraReplayStartZ;
static u32 sRaceCameraReplayStartPadding;
RaceCourseIndexValue gRaceCourseIndex;
s16 gRaceLapCount;
s8 gRacePlayerCount;
u8 gPlayerCount;
u8 gRaceUpdatePaused;
u8 gRacePauseMenuSelection;
u8 gRaceCameraModeChangeDisabled;
u8 gRaceDemoPlaybackEnabled;
u8 gTrainingCourseLesson;
u8 gMainMenuModeSelection;
s16 gRacePlayerAttackStartTimer;
u8 gRaceTypeSelection;
u8 gRaceResultState;
u8 gRaceResultsAwardPlayerNumber;
s8 gRaceGhostRecordStatus;

void initStartupControllerPakFlow(void) {
    LOAD_ASSET(_2427D0, 6);
    initCallbackTaskScheduler(0);
    gCurrentGameTask->callbackData0 = 0xA;
    setCurrentGameTaskCallback(&waitStartupRumbleInit, 0);
}

void waitStartupRumbleInit(void) {
    gCurrentGameTask->callbackData0 -= 1;
    if (gCurrentGameTask->callbackData0 == 0) {
        requestRumbleMotorInit(0);
        requestRumbleMotorInit(1);
        requestRumbleMotorInit(2);
        requestRumbleMotorInit(3);
        setCurrentGameTaskCallback(&routeStartupControllerPakFlow, 0);
    }
}

void routeStartupControllerPakFlow(void) {
    if (gPlayerInputHeld[0] & START_BUTTON) {
        setCurrentGameTaskCallback(&openStartupControllerPakFileDeleteFlow, 0);
    } else {
        setCurrentGameTaskCallback(&openStartupReplaySaveMessageFlow, 0);
    }
}

void openStartupControllerPakFileDeleteFlow(void) {
    setCurrentGameTaskCallback(&openStartupReplaySaveMessageFlow, 0);
    createGameTask(4, &initControllerPakFileDeleteFlow, 0x64);
    suspendGameTask(0);
}

void openStartupReplaySaveMessageFlow(void) {
    setCurrentGameTaskCallback(&enterMainMenuAfterStartupControllerPakFlow, 0);
    createGameTask(4, &initControllerPakReplaySaveMessageFlow, 0x64);
    suspendGameTask(0);
}

void enterMainMenuAfterStartupControllerPakFlow(void) {
    createGameTask(3, &enterMainMenuFromRace, 0x64);
    removeGameTask(0);
}

void clearRaceReplayCourseGrid(void) {
    CourseGridEntry *entry = gRaceReplayCourseGridLists[gRaceCourseIndex.signedValue];
    s32 count = 0;
loop:
    if (entry->status != COURSE_GRID_ENTRY_END) {
        entry->status = COURSE_GRID_ENTRY_FREE;
        entry++;
        count += 1;
        goto loop;
    }
    if (count != 0) {
        gAssetHandles[0x2C] = allocRelocatableHeapBlock(count * sizeof(RacePlayer));
    }
}

void saveRaceReplayCourseGridEntry(void) {
    CourseGridEntry *entry = gRaceReplayCourseGridLists[gRaceCourseIndex.signedValue];
    s32 i = 0;

loop:
    if (entry->status != COURSE_GRID_ENTRY_END) {
        if ((entry->status == COURSE_GRID_ENTRY_FREE) && (entry->courseId == gRacePlayers[0].state.coursePathIndex)) {
            s32 *status = (s32 *)getRelocatableHeapBlockBase(gAssetHandles[0x2B]);
            if (*status < 0x1194) {
                entry->status = *status;
                ((RacePlayer *)getRelocatableHeapBlockBase(gAssetHandles[0x2C]))[i] = gRacePlayers[0];
            }
        }
        entry++;
        i++;
        goto loop;
    }
}

s32 loadNextRaceReplayCourseGridEntry(void) {
    s32 count;
    CourseGridEntry *entry;

    entry = gRaceReplayCourseGridLists[gRaceCourseIndex.signedValue];
    count = 0;
loop:
    if (entry->status != COURSE_GRID_ENTRY_END) {
        if (entry->status != COURSE_GRID_ENTRY_FREE) {
            gRacePlayers[0] = ((RacePlayer *)getRelocatableHeapBlockBase(gAssetHandles[0x2C]))[count];
            *(s32 *)getRelocatableHeapBlockBase(gAssetHandles[0x2B]) = entry->status;
            entry->status = COURSE_GRID_ENTRY_FREE;
            gRaceCameraReplayStartX = entry->unk4;
            gRaceCameraReplayStartY = entry->unk8;
            gRaceCameraReplayStartZ = entry->unkC;
            setRaceCameraModeForced(0, 7);
            return 1;
        }
        entry++;
        count += 1;
        goto loop;
    }
    return 0;
}

s32 hasPendingRaceReplayCourseGridEntry(void) {
    CourseGridEntry *entry = gRaceReplayCourseGridLists[gRaceCourseIndex.signedValue];

loop:
    if (entry->status != COURSE_GRID_ENTRY_END) {
        if (entry->status != COURSE_GRID_ENTRY_FREE) {
            return 1;
        }
        entry++;
        goto loop;
    }
    return 0;
}

void initNewGameSaveData(void) {
    s32 course;
    s32 one;
    s32 player;

    for (course = 0; course < 11; course++) {
        one = 1;
        for (player = 0; player < 5; player++) {
            gGameSaveDataBuffer[0].timeTrialRecords[course][player].minutes = 3;
            gGameSaveDataBuffer[0].timeTrialRecords[course][player].seconds = 0x3B;
            gGameSaveDataBuffer[0].timeTrialRecords[course][player].fraction = 0;
            gGameSaveDataBuffer[0].raceRecords[course][player].minutes = 3;
            gGameSaveDataBuffer[0].raceRecords[course][player].seconds = 0;
            gGameSaveDataBuffer[0].raceRecords[course][player].fraction = 0;
            gGameSaveDataBuffer[0].timeTrialCharacterIds[course][player] = player;
            gGameSaveDataBuffer[0].scoreAttackScores[course][player] = 0;
            gGameSaveDataBuffer[0].scoreAttackCharacterIds[course][player] = player;
            gGameSaveDataBuffer[0].raceRecordCharacterIds[course][player] = player;
            gGameSaveDataBuffer[0].trickAttackScores[course][player] = 0;
            gGameSaveDataBuffer[0].trickAttackCharacterIds[course][player] = player;
        }
        gGameSaveDataBuffer[0].bestLapRecords[course].minutes = one;
        gGameSaveDataBuffer[0].bestLapRecords[course].seconds = 0x18;
        gGameSaveDataBuffer[0].bestLapRecords[course].fraction = 0;
        gGameSaveDataBuffer[0].highScores[course + 1] = 0;
    }

    gGameSaveDataBuffer[0].replaySlots[0].length = 0;
    gGameSaveDataBuffer[0].replaySlots[1].length = 0;
    gGameSaveDataBuffer[0].replaySlots[2].length = 0;
    gGameSaveDataBuffer[0].replaySlots[3].length = 0;
    gGameSaveDataBuffer[0].replaySlots[4].length = 0;
    gGameSaveDataBuffer[0].replaySlots[5].length = (u8)0;
    gGameSaveDataBuffer[0].replaySlots[6].length = 0;
    gGameSaveDataBuffer[0].replaySlots[7].length = 0;
    gGameSaveDataBuffer[0].replaySlots[8].length = 0;
    gPlayerCount = 1;
    gRaceTypeSelection = 0;
    gRacePlayers[0].selectedCharacterId = 0;
    gRacePlayers[1].selectedCharacterId = 0;
    gRacePlayers[2].selectedCharacterId = 0;
    gRacePlayers[3].selectedCharacterId = 0;
    gRacePlayers[0].menuSelection = 0;
    gRacePlayers[1].menuSelection = 0;
    gRacePlayers[2].menuSelection = 0;
    gRacePlayers[3].menuSelection = 0;
    gRacePlayers[0].selectionUnlockState = 0;
    gRacePlayers[1].selectionUnlockState = 0;
    gRacePlayers[2].selectionUnlockState = 0;
    gRacePlayers[3].selectionUnlockState = 0;
    gRacePlayers[0].money = 0;
    gRacePlayers[1].money = 0;
    gRacePlayers[2].money = 0;
    gRacePlayers[3].money = 0;
    gRacePlayers[0].unk18 = 0;
    gRacePlayers[1].unk18 = 0;
    gRacePlayers[2].unk18 = 0;
    gRacePlayers[3].unk18 = 0;
    gRaceCourseIndex.signedValue = RACE_COURSE_BIG_SNOWMAN;
    gRaceSplitscreenMode = 0;
    gRaceCameraModeChangeDisabled = 0;
    setCurrentGameTaskCallback(openRaceSetupMenuFlow, 0);
}

void openRaceSetupMenuFlow(void) {
    setCurrentGameTaskCallback(&routeRaceCharacterSetupFlow, 0);
    createGameTask(4, &initRaceSetupMenu, 0x64);
    suspendGameTask(2);
}

void openRaceCharacterSelectMenuFlow(void) {
    setCurrentGameTaskCallback(&routeRaceCharacterSetupFlow, 0);
    createGameTask(4, &initCharacterSelectMenu, 0x64);
    suspendGameTask(2);
}

void routeRaceCharacterSetupFlow(void) {
    if (gPlayerCount >= 2) {
        setCurrentGameTaskCallback(&openPlayerSelectCourseMenuFlow, 0);
    } else {
        setCurrentGameTaskCallback(&openRaceSplitscreenSelectFlow, 0);
    }
}

void openPlayerSelectCourseMenuFlow(void) {
    setCurrentGameTaskCallback(&handlePlayerSelectCourseMenuFlow, 0);
    createGameTask(4, &initCharacterSelectCourseMenuFromPlayerSelect, 0x64);
    suspendGameTask(2);
}

void handlePlayerSelectCourseMenuFlow(void) {
    if (gMenuFlowState == 0) {
        setCurrentGameTaskCallback(&openMultiplayerCourseSelectFlow, 0);
    } else {
        gMenuFlowState = 0;
        setCurrentGameTaskCallback(&openRaceCharacterSelectMenuFlow, 0);
    }
}

void openRaceCourseMenuFlow(void) {
    if (gRaceSplitscreenMode == 1) {
        setCurrentGameTaskCallback(&handleRaceTypeSelectFlow, 0);
    } else {
        setCurrentGameTaskCallback(&handleRaceSplitscreenSelectFlow, 0);
    }
    createGameTask(4, &initCharacterSelectCourseMenuFromRace, 0x64);
    suspendGameTask(2);
}

void openRaceSplitscreenSelectFlow(void) {
    setCurrentGameTaskCallback(&handleRaceSplitscreenSelectFlow, 0);
    createGameTask(4, &initRaceSplitscreenSelectMenu, 0x64);
    suspendGameTask(2);
}

void handleRaceSplitscreenSelectFlow(void) {
    if (gMenuFlowState == 1) {
        gMenuFlowState = 0;
        setCurrentGameTaskCallback(&openRaceCharacterSelectMenuFlow, 0);
        return;
    }
    switch (gRaceSplitscreenMode) {
        case 0:
        case 2:
            setCurrentGameTaskCallback(&openMultiplayerCourseSelectFlow, 0);
            return;
        case 1:
            setCurrentGameTaskCallback(&openRaceTypeSelectFlow, 0);
            return;
        case 3:
            setCurrentGameTaskCallback(&openSinglePlayerCourseSelectFlow, 0);
            return;
        case 4:
            setCurrentGameTaskCallback(&openRaceRecordSaveOptionsFlow, 0);
            /* fallthrough */
        default:
            return;
    }
}

void openRaceTypeSelectFlow(void) {
    setCurrentGameTaskCallback(&handleRaceTypeSelectFlow, 0);
    createGameTask(4, &initRaceTypeSelectMenu, 0x64);
    suspendGameTask(2);
}

void handleRaceTypeSelectFlow(void) {
    if (gMenuFlowState == 1) {
        gMenuFlowState = 0;
        setCurrentGameTaskCallback(&openRaceSplitscreenSelectFlow, 0);
    } else {
        setCurrentGameTaskCallback(&openMultiplayerCourseSelectFlow, 0);
    }
}

void openSinglePlayerCourseSelectFlow(void) {
    setCurrentGameTaskCallback(&openRaceSplitscreenSelectFlow, 0);
    createGameTask(4, &initCourseSelectMenu, 0x64);
    suspendGameTask(2);
}

void openRaceRecordSaveOptionsFlow(void) {
    gRumblePakConnectedMask = 0;
#if defined(VERSION_JP)
    setCurrentGameTaskCallback(&exitRaceFlowToMainMenu, 0);
#else
    setCurrentGameTaskCallback(&handleRaceRecordSaveOptionsFlow, 0);
#endif
    createGameTask(4, &initControllerPakRaceRecordSaveFlow, 0x64);
    suspendGameTask(2);
#if defined(VERSION_US)
}

void handleRaceRecordSaveOptionsFlow(void) {
    if (gMenuFlowState == 1) {
        gMenuFlowState = 0;
        setCurrentGameTaskCallback(&openRaceSplitscreenSelectFlow, 0);
    } else {
        setCurrentGameTaskCallback(&exitRaceFlowToMainMenu, 0);
    }
#endif
}

void openMultiplayerCourseSelectFlow(void) {
    setCurrentGameTaskCallback(&handleMultiplayerCourseSelectFlow, 0);
    createGameTask(4, &initMultiplayerCourseSelectMenu, 0x64);
    suspendGameTask(2);
}

void handleMultiplayerCourseSelectFlow(void) {
    if (gMenuFlowState == 1) {
        gMenuFlowState = 0;
        if (gPlayerCount >= 2) {
            setCurrentGameTaskCallback(&openPlayerSelectCourseMenuFlow, 0);
        } else {
            setCurrentGameTaskCallback(&openRaceCourseMenuFlow, 0);
        }
    } else {
        setCurrentGameTaskCallback(&initRaceRecordSettingsFlow, 0);
    }
}

void exitRaceFlowToMainMenu(void) {
    requestRumbleMotorInit(0);
    requestRumbleMotorInit(1);
    requestRumbleMotorInit(2);
    requestRumbleMotorInit(3);
    requestMusicSequenceStop(0);
    createGameTask(3, &enterMainMenuFromRace, 0x64);
    removeGameTask(2);
}

void openPostRaceControllerPakContinuePromptFlow(void) {
    setCurrentGameTaskCallback(&handleControllerPakContinuePromptFlow, 0);
    createGameTask(4, &initControllerPakContinuePromptFlow, 0x64);
    suspendGameTask(2);
}

void handleControllerPakContinuePromptFlow(void) {
    if (gMenuFlowState == 1) {
        gMenuFlowState = 0;
        setCurrentGameTaskCallback(&exitRaceFlowToMainMenu, 0);
    } else {
        setCurrentGameTaskCallback(&openRaceCharacterSelectMenuFlow, 0);
    }
}

void openEndingCreditsIfUnlockedFlow(void) {
    setCurrentGameTaskCallback(&openRaceSplitscreenSelectFlow, 0);
    if (gPendingEndingCreditsFlow != 0) {
        createGameTask(4, &initEndingCreditsFlow, 0x64);
        suspendGameTask(2);
    }
}

void openPassAwardCelebrationFlow(void) {
    setCurrentGameTaskCallback(&openEndingCreditsIfUnlockedFlow, 0);
    createGameTask(4, &initPassAwardCelebration, 0x64);
    suspendGameTask(2);
}

void initRaceSceneFlow(void) {
    s32 i;
    s32 j;
    s32 k;
    s32 pad;
    s32 duplicate;
    RaceCpuBoardConfig *characterEntry;
    RaceInputHistoryBuffer *history;
    u16 *replayRecordFlag;

    gRaceUpdatePaused = 0;
    gRacePauseMenuSelection = 0;
    gRaceCameraModeChangeDisabled = 0;
    gRaceResultState = 0;
    gRaceDemoPlaybackEnabled = 0;
    gTrainingCourseLesson = 0;
    gMainMenuModeSelection = 0;
    gPendingEndingCreditsFlow = 0;
    stopSoundEffects();
    requestMusicSequenceStop(0);
    gRacePlayers[0].isCpu = 1;
    gRacePlayers[1].isCpu = 1;
    gRacePlayers[2].isCpu = 1;
    gRacePlayers[3].isCpu = 1;
    gRacePlayers[0].cpuItemUseChance = gRaceCourseCpuPickupUseChances[gRaceCourseIndex.signedValue];
    gRacePlayers[1].cpuItemUseChance = gRaceCourseCpuPickupUseChances[gRaceCourseIndex.signedValue];
    gRacePlayers[2].cpuItemUseChance = gRaceCourseCpuPickupUseChances[gRaceCourseIndex.signedValue];
    gRacePlayers[3].cpuItemUseChance = gRaceCourseCpuPickupUseChances[gRaceCourseIndex.signedValue];
    gRacePlayers[0].cpuShotUseChance = gRaceCourseCpuPickupUseChances[gRaceCourseIndex.signedValue];
    gRacePlayers[1].cpuShotUseChance = gRaceCourseCpuPickupUseChances[gRaceCourseIndex.signedValue];
    gRacePlayers[2].cpuShotUseChance = gRaceCourseCpuPickupUseChances[gRaceCourseIndex.signedValue];
    gRacePlayers[3].cpuShotUseChance = gRaceCourseCpuPickupUseChances[gRaceCourseIndex.signedValue];
    if (gPlayerCount != 1) {
        gRacePlayers[0].cpuItemUseChance = 0;
        gRacePlayers[1].cpuItemUseChance = 0;
        gRacePlayers[2].cpuItemUseChance = 0;
        gRacePlayers[3].cpuItemUseChance = 0;
        gRacePlayers[0].cpuShotUseChance = 0;
        gRacePlayers[1].cpuShotUseChance = 0;
        gRacePlayers[2].cpuShotUseChance = 0;
        gRacePlayers[3].cpuShotUseChance = 0;
    }
    gRacePlayers[0].unk17 = 0;
    gRacePlayers[1].unk17 = 1;
    gRacePlayers[2].unk17 = 2;
    gRacePlayers[3].unk17 = 3;
    for (i = 0; i < gPlayerCount; i++) {
        gRacePlayers[i].isCpu = 0;
    }
    gRacePlayers[0].characterId = gRacePlayers[0].selectedCharacterId;
    gRacePlayers[1].characterId = gRacePlayers[1].selectedCharacterId;
    gRacePlayers[2].characterId = gRacePlayers[2].selectedCharacterId;
    gRacePlayers[3].characterId = gRacePlayers[3].selectedCharacterId;
    gRacePlayers[0].snowboardIndex = gRacePlayers[0].menuSelection;
    gRacePlayers[1].snowboardIndex = gRacePlayers[1].menuSelection;
    gRacePlayers[2].snowboardIndex = gRacePlayers[2].menuSelection;
    gRacePlayers[3].snowboardIndex = gRacePlayers[3].menuSelection;
    gRacePlayers[0].snowboardTextureIndex = gRacePlayers[0].selectionUnlockState;
    gRacePlayers[1].snowboardTextureIndex = gRacePlayers[1].selectionUnlockState;
    gRacePlayers[2].snowboardTextureIndex = gRacePlayers[2].selectionUnlockState;
    gRacePlayers[3].snowboardTextureIndex = gRacePlayers[3].selectionUnlockState;

    for (i = gPlayerCount, k = 0; i < 4; i++) {
        while (1) {
            duplicate = 1;
            gRacePlayers[i].characterId =
                gRaceCourseOpponentCharacterIds.characterIds[gRaceCourseIndex.signedValue * 4 + k++];
            for (j = 0; j < gPlayerCount; j++) {
                if (gRacePlayers[i].characterId == gRacePlayers[j].characterId) {
                    duplicate = 0;
                }
            }
            if (duplicate != 0) {
                break;
            }
        }
        characterEntry =
            &gRaceCpuBoardConfigsByCourse[gRaceCourseIndex.signedValue][gRacePlayers[i].characterId & 0xFFFFFFFFu];
        gRacePlayers[i].snowboardIndex = characterEntry->boardPropertiesIndex;
        gRacePlayers[i].snowboardTextureIndex = characterEntry->snowboardTextureIndex;
    }
    if (gRaceSplitscreenMode == 0) {
        gRacePlayers[0].isActive = 1;
        gRacePlayers[1].isActive = 1;
        gRacePlayers[2].isActive = 1;
        gRacePlayers[3].isActive = 1;
        gRacePlayerCount = 4;
        gRacePlayerAttackStartTimer = 0x64;
        initCallbackTaskScheduler(1);
    } else {
        gRacePlayers[0].isActive = 1;
        gRacePlayers[1].isActive = 0;
        gRacePlayers[2].isActive = 0;
        gRacePlayers[3].isActive = 0;
        gRacePlayerCount = 1;
        gRaceLapCount = 1;
        gRacePlayerAttackStartTimer = 0x64;
        initCallbackTaskScheduler(2);
    }
    for (i = 0; i < 4; i++) {
        gRacePlayers[i].unk15 = 0;
        gRacePlayers[i].replayInputSource = RACE_PLAYER_INPUT_LIVE;
        gRacePlayers[i].soundDisabled = 0;
    }

    if (gRaceSplitscreenMode == 2) {
        clearRaceReplayCourseGrid();
        gRacePlayers[0].unk15 = 1;
        gAssetHandles[0x2B] = allocRelocatableHeapBlock(0x34CC);
        history = getRelocatableHeapBlockBase(gAssetHandles[0x2B]);
        history->writeIndex = 0;
        history->enabled = 1;
        history->courseId = gRaceCourseIndex.signedValue;
        history->characterId = gRacePlayers[0].characterId;
        history->snowboardIndex = gRacePlayers[0].snowboardIndex;
        history->unkC = gRacePlayers[0].snowboardTextureIndex;

        for (i = 0; i < 0x1194; i++) {
            history->stickX[i] = 0;
            history->stickY[i] = 0;
            history->buttons[i] = 0;
        }

        switch (gRaceCourseIndex.signedValue) {
            case RACE_COURSE_BIG_SNOWMAN:
                replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[0].length;
                break;

            case RACE_COURSE_SUNSET_ROCK:
                replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[1].length;
                break;

            case RACE_COURSE_NIGHT_HIGHWAY:
                replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[2].length;
                break;

            case RACE_COURSE_GRASS_VALLEY:
                replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[3].length;
                break;

            case RACE_COURSE_DIZZY_LAND:
                replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[4].length;
                break;

            case RACE_COURSE_QUICKSAND_VALLEY:
                replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[5].length;
                break;

            case RACE_COURSE_SILVER_MOUNTAIN:
                replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[6].length;
                break;

            case RACE_COURSE_NINJA_LAND:
                replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[7].length;
                break;

            case RACE_COURSE_ROOKIE_MOUNTAIN:
                replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[8].length;
                break;
        }

        if ((gRaceCourseIndex.signedValue != RACE_COURSE_ANIMAL_LAND) && ((*replayRecordFlag) != 0)) {
            gAssetHandles[0x7] = allocRelocatableHeapBlock(0x34CC);
            loadCurrentRaceRecordReplayData();
            history = getRelocatableHeapBlockBase(gAssetHandles[0x7]);
            if (history->enabled) {
                history->writeIndex = 0;
                gRacePlayers[1].isActive = 1;
                gRacePlayers[1].isCpu = 0;
                gRacePlayers[1].soundDisabled = 1;
                gRacePlayers[1].replayInputSource = RACE_PLAYER_INPUT_HISTORY_07;
                gRacePlayers[1].characterId = history->characterId;
                gRacePlayers[1].snowboardIndex = history->snowboardIndex;
                gRacePlayers[1].snowboardTextureIndex = history->unkC;
                gRacePlayerCount = 2;
            }
        }
    }

    loadRaceCourseAssets();
    loadRaceCharacterAssets();
    gMenuFlowState = 1;
    resetRaceCameras();
    resetAllViewports();

    if (gRaceCourseIndex.signedValue != RACE_COURSE_SILVER_MOUNTAIN) {
        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    } else {
        configureRaceViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    }
    gRacePlayerHudStatuses[0].active = 1;
    gFramebufferRenderInterval.value = 0;
    initRacePlayers();
    initRaceHud();
    initRaceCourseSceneTasks();
    gMenuFadeAlpha = 0xFF;
    if (gRaceSplitscreenMode == 2) {
        resetSecondaryRng();
    }
    gRaceRumbleEnabled = 1;
    if (gRaceSplitscreenMode == 0) {
        gCurrentGameTask->callbackData0 = 0;
        gCurrentGameTask->callbackData1 = 0;
        gCurrentGameTask->callbackData2 = 0x1E;
        createCallbackTask((CallbackTaskCallback)initCourseRecordBannerFadeOut, 0, 0x64);
        setCurrentGameTaskCallback(fadeOutPassAwardCelebrationFlow, 0);
    } else {
        createCallbackTask((CallbackTaskCallback)initRaceCountdownPrompt, 6, 0x64);
        createCallbackTask((CallbackTaskCallback)initRaceStartOverlay, 6, 0x64);
        setCurrentGameTaskCallback(startRaceGameplayFlow, 0);
    }
    for (i = 0; i < gPlayerCount; i++) {
        requestRumbleMotorInit(i);
    }
    updateRelocatableHeap();
}

void fadeOutPassAwardCelebrationFlow(void) {
    gMenuFadeAlpha -= 8;
    if (gMenuFadeAlpha < 0) {
        gMenuFadeAlpha = 0;
        gCurrentGameTask->callbackData2--;
    }
    if (gCurrentGameTask->callbackData2 == 0) {
        setRaceCameraMode(0, 0x1E);
        requestMusicSequenceBank(5);
        setCurrentGameTaskCallback(&fadeInRaceGameplayViewports, 0);
    }
    updateRaceFlowFrame();
    gViewportStates[0].overlayAlpha = 0xFF;
}

void fadeInRaceGameplayViewports(void) {
    s32 temp_a0;
    GameTask *state;

    state = gCurrentGameTask;
    temp_a0 = state->callbackData1;
    if (temp_a0 >= 0x32) {
        gMenuFadeOverlayActive = 1;
        gMenuFadeAlpha += 0x16;
        if (gMenuFadeAlpha >= 0x100) {
            setRaceCameraMode(0, 1);
            gMenuFadeAlpha = 0xFF;
            createCallbackTask((CallbackTaskCallback)initRaceCountdownPrompt, 6, 0x64);
            createCallbackTask((CallbackTaskCallback)initRaceStartOverlay, 6, 0x64);
            switch (gPlayerCount & 0xFFFFFFFF) {
                case 1:
                    if (gRaceCourseIndex.signedValue != RACE_COURSE_SILVER_MOUNTAIN) {
                        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
                    } else {
                        configureRaceViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
                    }
                    gRaceCameras[0].initialized.value = 1;
                    gFramebufferRenderInterval.value = 0;
                    break;
                case 2:
                    if (gRaceCourseIndex.signedValue != RACE_COURSE_SILVER_MOUNTAIN) {
                        configureViewport(0, 0xA0, 0x43, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
                        configureViewport(1, 0xA0, 0xAD, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
                    } else {
                        configureRaceViewport(0, 0xA0, 0x43, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
                        configureRaceViewport(1, 0xA0, 0xAD, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
                    }
                    gRaceCameras[0].initialized.value = 1;
                    gRaceCameras[1].initialized.value = 1;
                    gFramebufferRenderInterval.value = 1;
                    break;
                case 3:
                    if (gRaceCourseIndex.signedValue != RACE_COURSE_SILVER_MOUNTAIN) {
                        configureViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                        configureViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                        configureViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    } else {
                        configureRaceViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                        configureRaceViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                        configureRaceViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    }
                    gRaceCameras[0].initialized.value = 1;
                    gRaceCameras[1].initialized.value = 1;
                    gRaceCameras[2].initialized.value = 1;
                    gFramebufferRenderInterval.value = 1;
                    break;
                case 4:
                    if (gRaceCourseIndex.signedValue != RACE_COURSE_SILVER_MOUNTAIN) {
                        configureViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                        configureViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                        configureViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                        configureViewport(3, 0xE9, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    } else {
                        configureRaceViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                        configureRaceViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                        configureRaceViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                        configureRaceViewport(3, 0xE9, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    }
                    gRaceCameras[0].initialized.value = 1;
                    gRaceCameras[1].initialized.value = 1;
                    gRaceCameras[2].initialized.value = 1;
                    gRaceCameras[3].initialized.value = 1;
                    gFramebufferRenderInterval.value = 1;
                    break;
            }
            setCurrentGameTaskCallback(startRaceGameplayFlow, 0);
        }
    } else {
        state->callbackData1 = temp_a0 + 1;
        gMenuFadeAlpha -= 0x10;
        if (gMenuFadeAlpha < 0) {
            gMenuFadeAlpha = 0;
        }
    }
    updateRaceFlowFrame();
}

void startRaceGameplayFlow(void) {
    if (gMenuFadeAlpha != 0) {
        gMenuFadeAlpha -= 0x10;
    }
    if (gMenuFadeAlpha < 0) {
        gMenuFadeAlpha = 0;
    }
    updateRaceFlowFrameWithCourseEffects();
    if (!(gMenuFlowState & 1)) {
        gMenuFadeOverlayActive = 0;
        requestCourseMusicSequence();
        setCurrentGameTaskCallback(updateRaceGameplayFlow, 0);
    }
}

void drawRacePauseMenu(void *arg0) {
    s32 color;

    drawAssetTableSprite(-0x14, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x57);
    color = 0x1A;
    if (gRacePauseMenuSelection == 0) {
        color = 0x1B;
    }
    drawAssetTableSpriteWithExplicitPalette(-0x1C, 0, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x58, color);
    color = 0x1A;
    if (gRacePauseMenuSelection == 1) {
        color = 0x1B;
    }
    drawAssetTableSpriteWithExplicitPalette(-0x1C, 0xA, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x59, color);
    color = 0x1A;
    if (gRacePauseMenuSelection == 2) {
        color = 0x1B;
    }
    drawAssetTableSpriteWithExplicitPalette(-0x1C, 0x14, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x5A, color);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void updateRaceGameplayFlow(void) {
#if defined(VERSION_US)
    RacePlayer *player;
#endif
    s32 opened;
    s32 i;
    s32 input;
    s32 selection;
    s32 valueTwo;

#if defined(VERSION_JP)
    opened = 0;
    for (i = 0; i < gPlayerCount; i++) {
        if (((gRacePlayers[i].soundDisabled == 0) && (gRaceUpdatePaused == 0)) &&
            (gPlayerInputPressed[i] & START_BUTTON)) {
            gRacePauseMenuSelection = 0;
            gRaceUpdatePaused = 1;
            opened = 1;
            enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);
            requestRumbleMotorInit(0);
            requestRumbleMotorInit(1);
            requestRumbleMotorInit(2);
            requestRumbleMotorInit(3);
        }
    }
    valueTwo = 2;
    if ((gRaceUpdatePaused != 0) && (opened == 0)) {
        for (i = 0; i < gPlayerCount; i++) {
            if (gRacePlayers[i].soundDisabled == 0) {
                input = gPlayerInputPressed[i];
                if (input & (STICK_UP | U_JPAD)) {
                    selection = gRacePauseMenuSelection;
                    if (selection != 0) {
                        gRacePauseMenuSelection = selection - 1;
                        enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);
                        input = gPlayerInputPressed[i];
                    }
#else
    opened = 0; \
    i = 0; \
    if (gPlayerCount > 0) { \
        player = gRacePlayers; \
        do { \
            if (((player->soundDisabled == 0) && (gRaceUpdatePaused == 0)) && (gPlayerInputPressed[i] & START_BUTTON)) { \
                gRacePauseMenuSelection = 0; \
                gRaceUpdatePaused = 1; \
                opened = 1; \
                enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL); \
                requestRumbleMotorInit(0); \
                requestRumbleMotorInit(1); \
                requestRumbleMotorInit(2); \
                requestRumbleMotorInit(3); \
            } \
            i++; \
            player++; \
        } while (i < gPlayerCount); \
    } \
    valueTwo = 2; \
    if ((gRaceUpdatePaused != 0) && (opened == 0)) { \
        i = 0; \
        if (gPlayerCount > 0) { \
            player = gRacePlayers; \
            do { \
                if (player->soundDisabled == 0) { \
                    input = gPlayerInputPressed[i]; \
                    if (input & (STICK_UP | U_JPAD)) { \
                        selection = gRacePauseMenuSelection; \
                        if (selection != 0) { \
                            gRacePauseMenuSelection = selection - 1; \
                            enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL); \
                            input = gPlayerInputPressed[i]; \
                        }
#endif
                    }
                    if (input & (STICK_DOWN | D_JPAD)) {
                        selection = gRacePauseMenuSelection;
                        if (valueTwo != selection) {
                            gRacePauseMenuSelection = selection + 1;
                            enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);
                            input = gPlayerInputPressed[i];
                        }
                    }
                    if (input & START_BUTTON) {
                        input = gRacePauseMenuSelection;
                        if (input == 0) {
                            gRaceUpdatePaused = 0;
                            enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);
                            input = gRacePauseMenuSelection;
                        }
                        if (input == 1) {
                            gRaceRumbleEnabled = 0;
                            gRaceUpdatePaused = 0;
                            gFramebufferSwapHold = 1;
                            enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);
                            setCurrentGameTaskCallback(finalizeRaceExitFlow, 0);
                            return;
                        }
                        if (valueTwo == input) {
                            gRaceRumbleEnabled = 0;
                            gRaceUpdatePaused = 0;
                            gFramebufferSwapHold = 1;
                            enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);
                            setCurrentGameTaskCallback(finalizeRaceExitFlow, 0);
                            return;
                        }
                    }
                }
#if defined(VERSION_US)
                i++;
                player++;
            } while (i < gPlayerCount);
#endif
        }
        addRenderCallback(&gMenuRenderCallbackList, drawRacePauseMenu, 0);
    }

    if (gRaceSplitscreenMode == valueTwo) {
        saveRaceReplayCourseGridEntry();
    }
    updateRaceFlowFrameWithCourseEffects();
    if (areRacePlayersFinished() != 0) {
        gCurrentGameTask->callbackData0 = 0x3C;
        gRaceRumbleEnabled = 0;
        setCurrentGameTaskCallback(waitRaceFinishResultsFlow, 0);
    }
}
// clang-format on

void waitRaceFinishResultsFlow(void) {
    gRacePauseMenuSelection = 0;
    updateRaceFlowFrameWithCourseEffects();
    gCurrentGameTask->callbackData0--;
    if (gCurrentGameTask->callbackData0 == 0) {
        gMenuFlowState |= 8;
        switch (gPlayerCount) {
            case 3:
                gCurrentGameTask->callbackData1 = 0;
                if (gRacePlayers[1].state.rankIndex < gRacePlayers[gCurrentGameTask->callbackData1].state.rankIndex) {
                    gCurrentGameTask->callbackData1 = 1;
                }
                if (gRacePlayers[2].state.rankIndex < gRacePlayers[gCurrentGameTask->callbackData1].state.rankIndex) {
                    gCurrentGameTask->callbackData1 = 2;
                }
                gCurrentGameTask->callbackData1 += 2;
                setCurrentGameTaskCallback(zoomRaceWinnerViewport, 0);
                break;
            case 4:
                gCurrentGameTask->callbackData1 = 0;
                if (gRacePlayers[1].state.rankIndex < gRacePlayers[gCurrentGameTask->callbackData1].state.rankIndex) {
                    gCurrentGameTask->callbackData1 = 1;
                }
                if (gRacePlayers[2].state.rankIndex < gRacePlayers[gCurrentGameTask->callbackData1].state.rankIndex) {
                    gCurrentGameTask->callbackData1 = 2;
                }
                if (gRacePlayers[3].state.rankIndex < gRacePlayers[gCurrentGameTask->callbackData1].state.rankIndex) {
                    gCurrentGameTask->callbackData1 = 3;
                }
                gCurrentGameTask->callbackData1 += 5;
                setCurrentGameTaskCallback(zoomRaceWinnerViewport, 0);
                break;
            case 2:
                if (gRacePlayers[0].state.rankIndex < gRacePlayers[1].state.rankIndex) {
                    gCurrentGameTask->callbackData1 = 0;
                    gRacePlayerHudStatuses[1].active = 0;
                } else {
                    gCurrentGameTask->callbackData1 = 1;
                    gRacePlayerHudStatuses[0].active = 0;
                }
                gCurrentGameTask->callbackData0 = 0;
                setCurrentGameTaskCallback(zoomRaceWinnerViewport, 0);
                break;
            case 1:
                gCurrentGameTask->callbackData0 = 0;
                setCurrentGameTaskCallback(prepareRaceResultsFlow, 0);
                break;
        }
    }
}

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
) {
    s32 sp2C;
    s32 sp28;
    f32 temp_fv0;
    s32 temp_t0;
    s32 temp_t1;
    s32 temp_t2;
    s32 temp_t3;

    sp2C = (((arg8 - arg1) * gCurrentGameTask->callbackData0) / 15) + arg1;
    sp28 = (((arg9 - arg2) * gCurrentGameTask->callbackData0) / 15) + arg2;
    temp_t0 = (((arg10 - arg3) * gCurrentGameTask->callbackData0) / 15) + arg3;
    temp_t1 = (((arg11 - arg4) * gCurrentGameTask->callbackData0) / 15) + arg4;
    temp_t2 = (((arg12 - arg5) * gCurrentGameTask->callbackData0) / 15) + arg5;
    temp_t3 = (((arg13 - arg6) * gCurrentGameTask->callbackData0) / 15) + arg6;
    temp_fv0 = (((arg14 - arg7) * (f32)gCurrentGameTask->callbackData0) / 15.0f) + arg7;
    if (gRaceCourseIndex.signedValue != RACE_COURSE_SILVER_MOUNTAIN) {
        configureViewport(arg0, sp2C, sp28, temp_t0, temp_t1, temp_t2, temp_t3, temp_fv0);
    } else {
        configureRaceViewport(arg0, sp2C, sp28, temp_t0, temp_t1, temp_t2, temp_t3, temp_fv0);
    }
}

void zoomRaceWinnerViewport(void) {
    gCurrentGameTask->callbackData0 += 1;
    switch (gCurrentGameTask->callbackData1) {
        case 0:
            interpolateRaceViewport(
                0,
                0xA0,
                0x43,
                0x120,
                0x68,
                0x140,
                0x78,
                2.6666667f,
                0xA0,
                0x78,
                0x120,
                0xD0,
                0x140,
                0xF0,
                1.3333334f
            );
            interpolateRaceViewport(
                1,
                0xA0,
                0xAD,
                0x120,
                0x68,
                0x140,
                0x78,
                2.6666667f,
                0xA0,
                0x115,
                0x120,
                0x68,
                0x140,
                0x78,
                2.6666667f
            );
            if (gCurrentGameTask->callbackData0 == 0xF) {
                resetViewport(1);
                gRaceCameras[1].initialized.value = 0;
            }
            break;
        case 1:
            interpolateRaceViewport(
                1,
                0xA0,
                0xAD,
                0x120,
                0x68,
                0x140,
                0x78,
                2.6666667f,
                0xA0,
                0x78,
                0x120,
                0xD0,
                0x140,
                0xF0,
                1.3333334f
            );
            interpolateRaceViewport(
                0,
                0xA0,
                0x43,
                0x120,
                0x68,
                0x140,
                0x78,
                2.6666667f,
                0xA0,
                -0x25,
                0x120,
                0x68,
                0x140,
                0x78,
                2.6666667f
            );
            if (gCurrentGameTask->callbackData0 == 0xF) {
                resetViewport(0);
                gRaceCameras[0].initialized.value = 0;
            }
            break;
        case 2:
            interpolateRaceViewport(
                0,
                0x57,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0xA0,
                0x78,
                0x120,
                0xD0,
                0x140,
                0xF0,
                1.3333334f
            );
            interpolateRaceViewport(
                1,
                0x57,
                0xAD,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0x57,
                0x115,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            interpolateRaceViewport(
                2,
                0xE9,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0x179,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            if (gCurrentGameTask->callbackData0 == 0xF) {
                gRaceCameras[1].initialized.value = 0;
                gRaceCameras[2].initialized.value = 0;
                resetViewport(1);
                resetViewport(2);
            }
            break;
        case 3:
            interpolateRaceViewport(
                1,
                0x57,
                0xAD,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0xA0,
                0x78,
                0x120,
                0xD0,
                0x140,
                0xF0,
                1.3333334f
            );
            interpolateRaceViewport(
                0,
                0x57,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0x57,
                -0x25,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            interpolateRaceViewport(
                2,
                0xE9,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0x179,
                -0x25,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            if (gCurrentGameTask->callbackData0 == 0xF) {
                gRaceCameras[0].initialized.value = 0;
                gRaceCameras[2].initialized.value = 0;
                resetViewport(0);
                resetViewport(2);
            }
            break;
        case 4:
            interpolateRaceViewport(
                2,
                0xE9,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0xA0,
                0x78,
                0x120,
                0xD0,
                0x140,
                0xF0,
                1.3333334f
            );
            interpolateRaceViewport(
                0,
                0x57,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                -0x39,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            interpolateRaceViewport(
                1,
                0x57,
                0xAD,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                -0x39,
                0x115,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            if (gCurrentGameTask->callbackData0 == 0xF) {
                gRaceCameras[0].initialized.value = 0;
                gRaceCameras[1].initialized.value = 0;
                resetViewport(0);
                resetViewport(1);
            }
            break;
        case 5:
            interpolateRaceViewport(
                0,
                0x57,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0xA0,
                0x78,
                0x120,
                0xD0,
                0x140,
                0xF0,
                1.3333334f
            );
            interpolateRaceViewport(
                1,
                0x57,
                0xAD,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0x57,
                0x115,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            interpolateRaceViewport(
                2,
                0xE9,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0x179,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            interpolateRaceViewport(
                3,
                0xE9,
                0xAD,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0x179,
                0x115,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            if (gCurrentGameTask->callbackData0 == 0xF) {
                resetViewport(1);
                resetViewport(2);
                resetViewport(3);
                gRaceCameras[1].initialized.value = 0;
                gRaceCameras[2].initialized.value = 0;
                gRaceCameras[3].initialized.value = 0;
            }
            break;
        case 6:
            interpolateRaceViewport(
                1,
                0x57,
                0xAD,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0xA0,
                0x78,
                0x120,
                0xD0,
                0x140,
                0xF0,
                1.3333334f
            );
            interpolateRaceViewport(
                0,
                0x57,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0x57,
                -0x25,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            interpolateRaceViewport(
                2,
                0xE9,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0x179,
                -0x25,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            interpolateRaceViewport(
                3,
                0xE9,
                0xAD,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0x179,
                0xAD,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            if (gCurrentGameTask->callbackData0 == 0xF) {
                gRaceCameras[0].initialized.value = 0;
                gRaceCameras[2].initialized.value = 0;
                gRaceCameras[3].initialized.value = 0;
                resetViewport(0);
                resetViewport(2);
                resetViewport(3);
            }
            break;
        case 7:
            interpolateRaceViewport(
                2,
                0xE9,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0xA0,
                0x78,
                0x120,
                0xD0,
                0x140,
                0xF0,
                1.3333334f
            );
            interpolateRaceViewport(
                0,
                0x57,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                -0x39,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            interpolateRaceViewport(
                1,
                0x57,
                0xAD,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                -0x39,
                0x115,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            interpolateRaceViewport(
                3,
                0xE9,
                0xAD,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0xE9,
                0x115,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            if (gCurrentGameTask->callbackData0 == 0xF) {
                gRaceCameras[0].initialized.value = 0;
                gRaceCameras[1].initialized.value = 0;
                gRaceCameras[3].initialized.value = 0;
                resetViewport(0);
                resetViewport(1);
                resetViewport(3);
            }
            break;
        case 8:
            interpolateRaceViewport(
                3,
                0xE9,
                0xAD,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0xA0,
                0x78,
                0x120,
                0xD0,
                0x140,
                0xF0,
                1.3333334f
            );
            interpolateRaceViewport(
                0,
                0x57,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                -0x39,
                -0x25,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            interpolateRaceViewport(
                1,
                0x57,
                0xAD,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                -0x39,
                0xAD,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            interpolateRaceViewport(
                2,
                0xE9,
                0x43,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f,
                0xE9,
                -0x25,
                0x90,
                0x68,
                0xA0,
                0x78,
                1.3333334f
            );
            if (gCurrentGameTask->callbackData0 == 0xF) {
                gRaceCameras[0].initialized.value = 0;
                gRaceCameras[1].initialized.value = 0;
                gRaceCameras[2].initialized.value = 0;
                resetViewport(0);
                resetViewport(1);
                resetViewport(2);
            }
            break;
    }
    updateRaceFlowFrame();
    if (gCurrentGameTask->callbackData0 == 0xF) {
        setCurrentGameTaskCallback(prepareRaceResultsFlow, 0);
    }
}

void prepareRaceResultsFlow(void) {
    RacePlayer *player;
    s32 i;
    s32 recordTime;
    s32 currentTime;

    gPendingEndingCreditsFlow = 0;
    gFramebufferRenderInterval.value = 0;
    gRaceResultsAwardPlayerNumber = 0;
    gRaceGhostRecordStatus = 0;

    switch (gRaceSplitscreenMode) {
        case 0:
            gRaceResultState = 1;
            i = 0;
            if (gPlayerCount > 0) {
                player = gRacePlayers;
#if defined(VERSION_JP)
                for (;;) {
#else
                do {
#endif
                    if (player->state.rankIndex == 0) {
                        gRaceResultsAwardPlayerNumber = i + 1;
                    }
                    i++;
                    player++;
#if defined(VERSION_JP)

                    if (!(i < gPlayerCount)) {
                        break;
                    }
                }
#else
                } while (i < gPlayerCount);
#endif
            }
            break;

        case 2:
            currentTime = gRaceElapsedTimer.fraction + (gRaceElapsedTimer.seconds * COURSE_TIME_SECOND) +
                          (gRaceElapsedTimer.minutes * COURSE_TIME_MINUTE);
            i = 0;
#if defined(VERSION_JP)
            for (;;) {
#else
            do {
#endif
                recordTime = gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][i].fraction +
                             (gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][i].seconds *
                              COURSE_TIME_SECOND) +
                             (gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][i].minutes *
                              COURSE_TIME_MINUTE);
                if (currentTime < recordTime) {
                    break;
                }
                i++;
#if defined(VERSION_JP)

                if (!(i < 5)) {
                    break;
                }
            }
#else
            } while (i < 5);
#endif
            if (i < 5) {
                gRaceResultState = 1;
                gRaceResultsAwardPlayerNumber = 1;
                if (i == 0) {
                    gRaceGhostRecordStatus = 1;
                }
            } else {
                gRaceResultState = 2;
            }
            break;

        case 1:
            if (gRaceTypeSelection != 0) {
                i = gRaceTypeSelection;
                if (i != 1) {
                    if (gRaceTypeSelection == 2) {
                        i = 0;
#if defined(VERSION_JP)
                        for (;;) {
#else
                        do {
#endif
                            if (gGameSaveDataBuffer[0].trickAttackScores[gRaceCourseIndex.signedValue][i] <
                                gRacePlayers[0].state.trickAttackPointTotal) {
                                break;
                            }
                            i++;
#if defined(VERSION_JP)

                            if (!(i < 5)) {
                                break;
                            }
                        }
#else
                        } while (i < 5);
#endif
                        if (gRaceChallengeFailed != 0) {
                            i = 5;
                        }
                        if (i < 5) {
                            gRaceResultState = 1;
                            gRaceResultsAwardPlayerNumber = 1;
                        } else {
                            gRaceResultState = 2;
                        }
                    }
                } else {
                    i = 0;
#if defined(VERSION_JP)
                    for (;;) {
#else
                    do {
#endif
                        if (gGameSaveDataBuffer[0].scoreAttackScores[gRaceCourseIndex.signedValue][i] <
                            gRacePlayers[0].state.scoreAttackPointTotal) {
                            break;
                        }
                        i++;
#if defined(VERSION_JP)

                        if (!(i < 5)) {
                            break;
                        }
                    }
#else
                    } while (i < 5);
#endif
                    if (gRaceChallengeFailed != 0) {
                        i = 5;
                    }
                    if (i < 5) {
                        gRaceResultState = 1;
                        gRaceResultsAwardPlayerNumber = 1;
                    } else {
                        gRaceResultState = 2;
                    }
                }
            } else {
                currentTime = gRaceElapsedTimer.fraction + (gRaceElapsedTimer.seconds * COURSE_TIME_SECOND) +
                              (gRaceElapsedTimer.minutes * COURSE_TIME_MINUTE);
                i = 0;
#if defined(VERSION_JP)
                for (;;) {
#else
                do {
#endif
                    recordTime = gGameSaveDataBuffer[0].raceRecords[gRaceCourseIndex.signedValue][i].fraction +
                                 (gGameSaveDataBuffer[0].raceRecords[gRaceCourseIndex.signedValue][i].seconds *
                                  COURSE_TIME_SECOND) +
                                 (gGameSaveDataBuffer[0].raceRecords[gRaceCourseIndex.signedValue][i].minutes *
                                  COURSE_TIME_MINUTE);
                    if (currentTime < recordTime) {
                        break;
                    }
                    i++;
#if defined(VERSION_JP)

                    if (!(i < 5)) {
                        break;
                    }
                }
#else
                } while (i < 5);
#endif
                if (gRaceChallengeFailed != 0) {
                    i = 5;
                }
                if (i < 5) {
                    gRaceResultState = 1;
                    gRaceResultsAwardPlayerNumber = 1;
                } else {
                    gRaceResultState = 2;
                }
            }
            break;
    }

    gCurrentGameTask->callbackData1 = 0x3C;
    setCurrentGameTaskCallback(updateRaceResultsFlow, 0);
    if (gRaceResultsAwardPlayerNumber != 0) {
        requestMusicSequenceBank(6);
        if ((gPlayerCount == 1) && (gRaceSplitscreenMode == 0)) {
            createCallbackTaskWithUserId((CallbackTaskCallback)updateRaceUiResultAwardBadgeDelay, 6, 0x64, 0xA9);
        }
        createCallbackTaskWithUserId(
            (CallbackTaskCallback)initFallingConfetti,
            5,
            0x64,
            gRaceResultsAwardPlayerNumber - 1
        );
    }
    updateRaceFlowFrame();
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void updateRaceResultsFlow(void) {
    RaceResultsReadyState ready;
    s32 currentTime;
    s32 recordTime;
    s32 recordIndex;
    s32 shiftIndex;
    CallbackTask *resultActor;

    gPendingEndingCreditsFlow = 0;
    gFramebufferRenderInterval.value = 0;
    ready.value = 1;
    if ((s32)gPlayerCount > 0) {
        recordIndex = 0;
#if defined(VERSION_JP)
        for (;;) {
#else
        do {
#endif
            if (!(gRacePlayers[recordIndex].state.stateFlags & 0x08000000)) {
                ready.value = 0;
            }
            recordIndex++;
#if defined(VERSION_JP)

            if (!(recordIndex < (s32)gPlayerCount)) {
                break;
            }
        }
#else
        } while (recordIndex < (s32)gPlayerCount);
#endif
    }

    if (gCurrentGameTask->callbackData1 != 0) {
        gCurrentGameTask->callbackData1--;
    } else {
        ready.value = 1;
    }
    if ((gRaceResultsAwardPlayerNumber != 0) && (countActiveMusicSequences() != 0)) {
        ready.value = 0;
    }

    if (ready.value != 0) {
        if (gRaceResultsAwardPlayerNumber == 0) {
            requestMusicSequenceStop(4);
        }
        gMenuFlowState |= 0x20;
        ready.value = 6;

        switch (gRaceSplitscreenMode) {
        case 0:
            if (gPlayerCount == 1) {
                resultActor =
                    createCallbackTask((CallbackTaskCallback)initRaceUiPrizePayout, ready.value, 0x64);
                if (resultActor != NULL) {
                    if (gRacePlayers[0].state.rankIndex != 3) {
                        if (gGameSaveDataBuffer[0].cupPlacements[gRaceCourseIndex.signedValue] == 0) {
                            gGameSaveDataBuffer[0].cupPlacements[gRaceCourseIndex.signedValue] =
                                gRacePlayers[0].state.rankIndex + 1;
                        } else if ((gRacePlayers[0].state.rankIndex + 1) <
                                   gGameSaveDataBuffer[0].cupPlacements[gRaceCourseIndex.signedValue]) {
                            gGameSaveDataBuffer[0].cupPlacements[gRaceCourseIndex.signedValue] =
                                gRacePlayers[0].state.rankIndex + 1;
                        }
                        if (gGameSaveDataBuffer[0].highScores[gRaceCourseIndex.signedValue + 1] <
                            gRacePlayers[0].state.cumulativeRaceScore) {
                            gGameSaveDataBuffer[0].highScores[gRaceCourseIndex.signedValue + 1] =
                                gRacePlayers[0].state.cumulativeRaceScore;
                        }
                    }
                    if (gRacePlayers[0].state.rankIndex == 0) {
                        if (gRaceCourseIndex.signedValue == RACE_COURSE_NINJA_LAND) {
                            gPendingEndingCreditsFlow = 1;
                            gGameSaveDataBuffer[0].extraCourseUnlockFlags |= 1;
                            gGameSaveDataBuffer[0].cupPlacements[0x17] |= 1;
                        }
                    } else {
                        gMenuFlowState |= 0x20;
                    }
                    gCurrentGameTask->callbackData0 = 0xA;
                    setCurrentGameTaskCallback(updateRaceResultsMusicFlow, 0);
                }
            } else {
                resultActor =
                    createCallbackTask((CallbackTaskCallback)initRaceUiResultsBanner, 6, 0x64);
                if (resultActor != NULL) {
#if defined(VERSION_JP)
                        recordIndex = 0;
                        for (;;) {
#else
                    recordIndex = 0; do {
#endif
                        if (gRacePlayers[recordIndex].state.rankIndex == 0) {
                            gRacePlayers[recordIndex].unk18++;
                            if (gRacePlayers[recordIndex].unk18 >= 0x64) {
                                gRacePlayers[recordIndex].unk18 = 0x63;
                            }
                        }
                        recordIndex++;
#if defined(VERSION_JP)

                            if (!(&gRacePlayers[recordIndex] < (RacePlayer *)&gFrameCounter)) {
                                break;
                            }
                        }
#else
                    } while (&gRacePlayers[recordIndex] < (RacePlayer *)&gFrameCounter);
#endif

                    ready.value = 0;
                    if ((s32)gPlayerCount > 0) {
                        recordIndex = 0;
#if defined(VERSION_JP)
                            for (;;) {
#else
                        do {
#endif
                            if (gRacePlayers[recordIndex].state.rankIndex == 0) {
                                ready.value = 1;
                            }
                            recordIndex++;
#if defined(VERSION_JP)

                                if (!(recordIndex < (s32)gPlayerCount)) {
                                    break;
                                }
                            }
#else
                        } while (recordIndex < (s32)gPlayerCount);
#endif
                    }
                    if (ready.value == 0) {
                        gMenuFlowState |= 0x20;
                    }
                    gCurrentGameTask->callbackData0 = 0xA;
                    setCurrentGameTaskCallback(updateRaceResultsMusicFlow, 0);
                }
            }
            break;

        case 2:
            resultActor = createCallbackTask((CallbackTaskCallback)initRaceUiCourseStats, 6, 0x64);
            if (resultActor != NULL) {
                currentTime = gRaceElapsedTimer.fraction +
                              (gRaceElapsedTimer.seconds * COURSE_TIME_SECOND) +
                              (gRaceElapsedTimer.minutes * COURSE_TIME_MINUTE);
                recordIndex = 0;
#if defined(VERSION_JP)
                    for (;;) {
                        recordTime = gGameSaveDataBuffer[0]
                                         .timeTrialRecords[gRaceCourseIndex.signedValue][recordIndex]
                                         .fraction +
                                     (gGameSaveDataBuffer[0]
                                          .timeTrialRecords[gRaceCourseIndex.signedValue][recordIndex]
                                          .seconds *
#else
                do {
                    recordTime =
                        gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][recordIndex].fraction +
                        (gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][recordIndex].seconds *
#endif
                         COURSE_TIME_SECOND) +
                        (gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][recordIndex].minutes *
                         COURSE_TIME_MINUTE);
                    if (currentTime < recordTime) {
                        break;
                    }
                    recordIndex++;
#if defined(VERSION_JP)

                        if (!(recordIndex < 5)) {
                            break;
                        }
                    }
#else
                } while (recordIndex < 5);
#endif

                resultActor->header.userId = recordIndex;
                if (recordIndex < 5) {
                    for (shiftIndex = 3; shiftIndex >= recordIndex; shiftIndex--) {
                        gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][shiftIndex + 1] =
                            gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][shiftIndex];
                        gGameSaveDataBuffer[0].timeTrialCharacterIds[gRaceCourseIndex.signedValue][shiftIndex + 1] =
                            gGameSaveDataBuffer[0].timeTrialCharacterIds[gRaceCourseIndex.signedValue][shiftIndex];
                    }
                    gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][recordIndex] =
                        gRaceElapsedTimer;
                    gGameSaveDataBuffer[0].timeTrialCharacterIds[gRaceCourseIndex.signedValue][recordIndex] =
                        gRacePlayers[0].characterId + (gRacePlayers[0].snowboardIndex * 8);
                    if (recordIndex == 0) {
                        gGameSaveDataBuffer[0].bestLapRecords[gRaceCourseIndex.signedValue] =
                            gRaceTimeTrialFinishTime;
                    }
                }
                if (recordIndex < 5) {
                    gRaceResultState = 1;
                } else {
                    gRaceResultState = 2;
                    gMenuFlowState |= 0x20;
                }
                gCurrentGameTask->callbackData0 = 0xA;
                setCurrentGameTaskCallback(updateRaceResultsMusicFlow, 0);
            }
            break;

        case 1:
            switch (gRaceTypeSelection) {
            case 2:
                resultActor =
                    createCallbackTask((CallbackTaskCallback)initRaceUiTrickPrizePayout, 6, 0x64);
                if (resultActor != NULL) {
#if defined(VERSION_JP)
                            // clang-format off
                    recordIndex = 0; for (;;) {
#else
                    recordIndex = 0; do {
#endif
                        if (gGameSaveDataBuffer[0].trickAttackScores[gRaceCourseIndex.signedValue][recordIndex] <
                            gRacePlayers[0].state.trickAttackPointTotal) {
                            break;
                        }
                        recordIndex++;
#if defined(VERSION_JP)

                                if (!(recordIndex < 5)) {
                                    break;
                                }
                            }
#else
                    } while (recordIndex < 5);
#endif
                    if (gRaceChallengeFailed != 0) {
                        recordIndex = 5;
                    }
                    resultActor->header.userId = recordIndex;
                    if (recordIndex < 5) {
                        for (shiftIndex = 3; shiftIndex >= recordIndex; shiftIndex--) {
                            gGameSaveDataBuffer[0].trickAttackScores[gRaceCourseIndex.signedValue][shiftIndex + 1] =
                                gGameSaveDataBuffer[0].trickAttackScores[gRaceCourseIndex.signedValue][shiftIndex];
                            gGameSaveDataBuffer[0]
                                .trickAttackCharacterIds[gRaceCourseIndex.signedValue][shiftIndex + 1] =
                                gGameSaveDataBuffer[0]
                                    .trickAttackCharacterIds[gRaceCourseIndex.signedValue][shiftIndex];
                        }
                        gGameSaveDataBuffer[0].trickAttackScores[gRaceCourseIndex.signedValue][recordIndex] =
                            gRacePlayers[0].state.trickAttackPointTotal;
                        gGameSaveDataBuffer[0].trickAttackCharacterIds[gRaceCourseIndex.signedValue][recordIndex] =
                            gRacePlayers[0].characterId + (gRacePlayers[0].snowboardIndex * 8);
                    }
                    if (gRacePlayers[0].state.trickAttackPointTotal >= 0x7D0) {
                        gGameSaveDataBuffer[0].extraCourseUnlockFlags |= 2;
                    }
                    if (recordIndex < 5) {
                        gRaceResultState = 1;
                    } else {
                        gRaceResultState = 2;
                        gMenuFlowState |= 0x20;
                    }
                    gCurrentGameTask->callbackData0 = 0xA;
                    setCurrentGameTaskCallback(updateRaceResultsMusicFlow, 0);
                }
                break;

            case 1:
                resultActor =
                    createCallbackTask((CallbackTaskCallback)initRaceUiHitPrizePayout, 6, 0x64);
                if (resultActor != NULL) {
#if defined(VERSION_JP)
                    recordIndex = 0; for (;;) {
#else
                    recordIndex = 0; do {
#endif
                        if (gGameSaveDataBuffer[0].scoreAttackScores[gRaceCourseIndex.signedValue][recordIndex] <
                            gRacePlayers[0].state.scoreAttackPointTotal) {
                            break;
                        }
                        recordIndex++;
#if defined(VERSION_JP)

                                if (!(recordIndex < 5)) {
                                    break;
                                }
                            }
#else
                    } while (recordIndex < 5);
#endif
                    if (gRaceChallengeFailed != 0) {
                        recordIndex = 5;
                    }
                    resultActor->header.userId = recordIndex;
                    if (recordIndex < 5) {
                        for (shiftIndex = 3; shiftIndex >= recordIndex; shiftIndex--) {
                            gGameSaveDataBuffer[0].scoreAttackScores[gRaceCourseIndex.signedValue][shiftIndex + 1] =
                                gGameSaveDataBuffer[0].scoreAttackScores[gRaceCourseIndex.signedValue][shiftIndex];
                            gGameSaveDataBuffer[0]
                                .scoreAttackCharacterIds[gRaceCourseIndex.signedValue][shiftIndex + 1] =
                                gGameSaveDataBuffer[0]
                                    .scoreAttackCharacterIds[gRaceCourseIndex.signedValue][shiftIndex];
                        }
                        gGameSaveDataBuffer[0].scoreAttackScores[gRaceCourseIndex.signedValue][recordIndex] =
                            gRacePlayers[0].state.scoreAttackPointTotal;
                        gGameSaveDataBuffer[0].scoreAttackCharacterIds[gRaceCourseIndex.signedValue][recordIndex] =
                            gRacePlayers[0].characterId + (gRacePlayers[0].snowboardIndex * 8);
                    }
                    if (recordIndex < 5) {
                        gRaceResultState = 1;
                    } else {
                        gRaceResultState = 2;
                        gMenuFlowState |= 0x20;
                    }
                    gCurrentGameTask->callbackData0 = 0xA;
                    setCurrentGameTaskCallback(updateRaceResultsMusicFlow, 0);
                }
                break;

            case 0:
                resultActor = createCallbackTask((CallbackTaskCallback)initRaceUiCourseRecordPayout, 6, 0x64);
                if (resultActor != NULL) {
                    currentTime = gRaceElapsedTimer.fraction +
                                  (gRaceElapsedTimer.seconds * COURSE_TIME_SECOND) +
                                  (gRaceElapsedTimer.minutes * COURSE_TIME_MINUTE);
                    recordIndex = 0;
#if defined(VERSION_JP)
                            for (;;) {
                                recordTime = gGameSaveDataBuffer[0]
                                                 .raceRecords[gRaceCourseIndex.signedValue][recordIndex]
                                                 .fraction +
                                             (gGameSaveDataBuffer[0]
                                                  .raceRecords[gRaceCourseIndex.signedValue][recordIndex]
                                                  .seconds *
#else
                    do {
                        recordTime =
                            gGameSaveDataBuffer[0].raceRecords[gRaceCourseIndex.signedValue][recordIndex].fraction +
                            (gGameSaveDataBuffer[0].raceRecords[gRaceCourseIndex.signedValue][recordIndex].seconds *
#endif
                             COURSE_TIME_SECOND) +
                            (gGameSaveDataBuffer[0].raceRecords[gRaceCourseIndex.signedValue][recordIndex].minutes *
                             COURSE_TIME_MINUTE);
                        if (currentTime < recordTime) {
                            break;
                        }
                        recordIndex++;
#if defined(VERSION_JP)

                                if (!(recordIndex < 5)) {
                                    break;
                                }
                            }
#else
                    } while (recordIndex < 5);
#endif
                    if (gRaceChallengeFailed != 0) {
                        recordIndex = 5;
                    }
                    resultActor->header.userId = recordIndex;
                    if (recordIndex < 5) {
                        for (shiftIndex = 3; shiftIndex >= recordIndex; shiftIndex--) {
                            gGameSaveDataBuffer[0].raceRecords[gRaceCourseIndex.signedValue][shiftIndex + 1] =
                                gGameSaveDataBuffer[0].raceRecords[gRaceCourseIndex.signedValue][shiftIndex];
                            gGameSaveDataBuffer[0]
                                .raceRecordCharacterIds[gRaceCourseIndex.signedValue][shiftIndex + 1] =
                                gGameSaveDataBuffer[0]
                                    .raceRecordCharacterIds[gRaceCourseIndex.signedValue][shiftIndex];
                        }
                        gGameSaveDataBuffer[0].raceRecords[gRaceCourseIndex.signedValue][recordIndex] =
                            gRaceElapsedTimer;
                        gGameSaveDataBuffer[0].raceRecordCharacterIds[gRaceCourseIndex.signedValue][recordIndex] =
                            gRacePlayers[0].characterId + (gRacePlayers[0].snowboardIndex * 8);
                    }
                    if (recordIndex < 5) {
                        gRaceResultState = 1;
                    } else {
                        gRaceResultState = 2;
                        gMenuFlowState |= 0x20;
                    }
                    gCurrentGameTask->callbackData0 = 0xA;
                    setCurrentGameTaskCallback(updateRaceResultsMusicFlow, 0);
                }
                break;
            }
            break;
        }
    }

    if (gRaceResultsAwardPlayerNumber != 0) {
        createCallbackTaskWithUserId((CallbackTaskCallback)initFallingConfetti, 5, 0x64,
                                     gRaceResultsAwardPlayerNumber - 1);
    }
    updateRaceFlowFrame();
}
// clang-format on

void updateRaceResultsMusicFlow(void) {
    if (countActiveMusicSequences() == 0) {
        requestMusicSequenceBank(7);
        gCurrentGameTask->callbackData1 = 0x3C;
        gMenuFlowState |= 0x20;
    }
    if (gRaceResultsAwardPlayerNumber != 0) {
        createCallbackTaskWithUserId(
            (CallbackTaskCallback)&initFallingConfetti,
            5,
            0x64,
            gRaceResultsAwardPlayerNumber - 1
        );
    }
    updateRaceFlowFrame();
    if (gMenuFlowState & 0x10) {
        gCurrentGameTask->callbackData0 -= 1;
        if (gCurrentGameTask->callbackData0 == 0) {
            gCurrentGameTask->callbackData0 = 4;
            setCurrentGameTaskCallback(&fadeOutRaceResultsFlow, 0);
            requestMusicSequenceStop(0x14);
        }
    }
}

void fadeOutRaceResultsFlow(void) {
    gFramebufferRenderInterval.value = 0;
    if (gRaceResultsAwardPlayerNumber != 0) {
        createCallbackTaskWithUserId(
            (CallbackTaskCallback)initFallingConfetti,
            5,
            0x64,
            gRaceResultsAwardPlayerNumber - 1
        );
    }
    updateRaceFlowFrame();
    gRaceRumbleEnabled = 0;
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0xFF) {
        gMenuFadeAlpha = 0xFF;
        gCurrentGameTask->callbackData0 -= 1;
        if (gCurrentGameTask->callbackData0 == 0) {
            stopSoundEffects();
            if ((gRaceSplitscreenMode == 2) &&
                (((Unk80043040 *)getRelocatableHeapBlockBase(gAssetHandles[0x2B]))->unk8 != 0) &&
                (gRaceGhostRecordStatus != 0) && (saveRaceRecordReplayData() != 0)) {
                gRaceGhostRecordStatus = -1;
            }
            if (gRaceSplitscreenMode == 2) {
                setCurrentGameTaskCallback(&initRaceGhostReplayFlow, 0);
            } else {
                gFramebufferSwapHold = 1;
                setCurrentGameTaskCallback(finalizeRaceExitFlow, 0);
            }
        }
    }
}

void initRaceGhostReplayFlow(void) {
    s32 one;
    s8 active;
    s32 inactive;
    s32 freedHandle26;
    s32 freedHandle25;
    Unk80043040 *asset;

    gRaceUpdatePaused = 0;
    gRaceCameraModeChangeDisabled = 1;
    gRaceResultState = 0;
    asset = (Unk80043040 *)getRelocatableHeapBlockBase(gAssetHandles[0x2B]);
    if (asset->unk8 == 0) {
        gFramebufferSwapHold = 1;
        setCurrentGameTaskCallback(finalizeRaceExitFlow, 0);
        return;
    }
    if (hasPendingRaceReplayCourseGridEntry() == 0) {
        gFramebufferSwapHold = 1;
        setCurrentGameTaskCallback(finalizeRaceExitFlow, 0);
        return;
    }
    asset->unk0 = 0;
    if (gAssetHandles[0x21] != -1) {
        gAssetHandles[0x21] = freeRelocatableHeapBlock(gAssetHandles[0x21]);
    }
    if (gAssetHandles[0x22] != -1) {
        gAssetHandles[0x22] = freeRelocatableHeapBlock(gAssetHandles[0x22]);
    }
    if (gAssetHandles[0x23] != -1) {
        gAssetHandles[0x23] = freeRelocatableHeapBlock(gAssetHandles[0x23]);
    }
    if (gAssetHandles[0x24] != -1) {
        gAssetHandles[0x24] = freeRelocatableHeapBlock(gAssetHandles[0x24]);
    }
    if (gAssetHandles[0x25] != -1) {
        freedHandle25 = freeRelocatableHeapBlock(gAssetHandles[0x25]);
        gAssetHandles[0x25] = freedHandle25;
    }
    if (gAssetHandles[0x26] != -1) {
        freedHandle26 = freeRelocatableHeapBlock(gAssetHandles[0x26]);
        gAssetHandles[0x26] = freedHandle26;
    }
    if (gAssetHandles[0x27] != -1) {
        gAssetHandles[0x27] = freeRelocatableHeapBlock(gAssetHandles[0x27]);
    }
    if (gAssetHandles[0x28] != -1) {
        gAssetHandles[0x28] = freeRelocatableHeapBlock(gAssetHandles[0x28]);
    }
    if (gAssetHandles[0x1F] != -1) {
        gAssetHandles[0x1F] = freeRelocatableHeapBlock(gAssetHandles[0x1F]);
    }
    asset->unk0 = 0;
    one = 1;
    active = 1;
    gRacePlayers[0].isCpu = 0;
    gRacePlayers[1].isCpu = one;
    gRacePlayers[2].isCpu = one;
    gRacePlayers[3].isCpu = one;
    gRacePlayers[0].isActive = active;
    gRacePlayers[1].isActive = 0;
    gRacePlayers[2].isActive = 0;
    inactive = 0;
    gRacePlayers[3].isActive = inactive;
    gRacePlayerCount = active;
    gRaceLapCount = active;
    gRacePlayerAttackStartTimer = 0x64;
    initCallbackTaskScheduler(2);
    for (one = 0; one < RACE_PLAYER_COUNT; one++) {
        gRacePlayers[one].unk15 = 0;
        gRacePlayers[one].replayInputSource = RACE_PLAYER_INPUT_LIVE;
        gRacePlayers[one].soundDisabled = 0;
    }
    gRacePlayers[0].replayInputSource = RACE_PLAYER_INPUT_HISTORY_2B;
    gMenuFlowState = 0;
    resetRaceCameras();
    resetAllViewports();
    if (gRaceCourseIndex.signedValue != RACE_COURSE_SILVER_MOUNTAIN) {
        configureViewport(0, 0xA0, 0x78, 0x100, 0xB0, 0x120, 0xD0, 1.3333334f);
    } else {
        configureRaceViewport(0, 0xA0, 0x78, 0x100, 0xB0, 0x120, 0xD0, 1.3333334f);
    }
    LOAD_ASSET(_598A70, 0x29);
    gRacePlayerHudStatuses[0].active = 1;
    gFramebufferRenderInterval.value = 0;
    resetSecondaryRng();
    initRacePlayers();
    initRaceHud();
    initRaceCourseSceneTasks();
    gMenuFadeAlpha = 0xFF;
    updateRelocatableHeap();
    requestMusicSequenceBank(0);
    gCurrentGameTask->callbackData0 = 0;
    gCurrentGameTask->callbackData1 = 0;
    createCallbackTask((CallbackTaskCallback)updateRaceUiPrizePayoutContinuePrompt, 6, 0x64);
    if (gRaceGhostRecordStatus == -1) {
        createCallbackTask((CallbackTaskCallback)updateRaceGhostUnavailableMessage, 6, 0x64);
    }
    setCurrentGameTaskCallback(updateRaceGhostReplayFlow, 0);
}

void updateRaceGhostReplayFlow(void) {
    void *sp18;

    if (gCurrentGameTask->callbackData0 == gRaceGhostReplayGridSwitchFrames[gCurrentGameTask->callbackData1]) {
        if (loadNextRaceReplayCourseGridEntry() != 0) {
            if (gCurrentGameTask->callbackData1 != 0xB) {
                gCurrentGameTask->callbackData1++;
                gRacePlayers[0].replayInputSource = RACE_PLAYER_INPUT_HISTORY_2B;
                gRacePlayers[0].unk15 = 0;
                gViewportStates[0].overlayAlpha = 0;
            } else {
                requestMusicSequenceStop(0x48);
                setCurrentGameTaskCallback(fadeOutRaceGhostReplayFlow, 0);
            }
        } else {
            requestMusicSequenceStop(0x48);
            setCurrentGameTaskCallback(fadeOutRaceGhostReplayFlow, 0);
        }
    }
    if (gCurrentGameTask->callbackData0 == 1) {
        gMenuFadeAlpha = 0;
    }
    if (gRaceUpdatePaused == 0) {
        gCurrentGameTask->callbackData0++;
    }
    updateRaceReplayFrame();
    if (gPlayerInputPressed[0] & START_BUTTON) {
        sp18 = fadeOutRaceGhostReplayFlow;
        requestMusicSequenceStop(0x48);
        setCurrentGameTaskCallback(sp18, 0);
    }
}

void fadeOutRaceGhostReplayFlow(void) {
    updateRaceReplayFrame();
    gRaceRumbleEnabled = 0;
    gMenuFadeAlpha += 7;
    if (gMenuFadeAlpha >= 0xFF) {
        gMenuFadeAlpha = 0xFF;
        setCurrentGameTaskCallback(&finalizeRaceExitFlow, 0);
    }
}

void finalizeRaceExitFlow(void) {
    gFramebufferSwapHold = 1;
    if (gPendingFramebufferSwapCount == 2) {
        gRaceRumbleEnabled = 0;
        gRaceCameraModeChangeDisabled = 0;
        updateRelocatableHeap();
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferRenderInterval.value = 0;
        stopSoundEffects();
        requestRumbleMotorInit(0);
        requestRumbleMotorInit(1);
        requestRumbleMotorInit(2);
        requestRumbleMotorInit(3);
        gMenuFlowState = 0;
        if (gRacePauseMenuSelection == 2) {
            setCurrentGameTaskCallback(initRaceSceneFlow, 0);
        } else if (gPlayerCount == 1) {
            if (gRaceSplitscreenMode == 1) {
                setCurrentGameTaskCallback(openRaceTypeSelectFlow, 0);
            } else {
                setCurrentGameTaskCallback(openPassAwardCelebrationFlow, 0);
            }
        } else {
            setCurrentGameTaskCallback(openPostRaceControllerPakContinuePromptFlow, 0);
        }
        requestMusicSequenceStop(0);
    }
}

void updateRaceFlowFrameWithCourseEffects(void) {
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
    updateRaceHud();
}

void updateRaceFlowFrame(void) {
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
}

void updateRaceReplayFrame(void) {
    updateRacePlayers();
    updateCallbackTasksWithMinPriority(0x63);
    updateRacePlayersPostUpdate();
    updateRemainingCallbackTasks();
    updateRaceCameras();
}

s32 areRacePlayersFinished(void) {
    s32 var_v0;
    RacePlayer *player;

    if (gRaceSplitscreenMode == 1) {
        if (!(gRacePlayers[0].state.stateFlags & 0x40)) {
            return 0;
        }
    } else {
        var_v0 = 0;
        if ((s32)gPlayerCount > 0) {
            player = gRacePlayers;
        loop:
            if (!(player->state.stateFlags & RACE_PLAYER_READY_FLAG)) {
                return 0;
            }
            var_v0++;
            player++;
            if (var_v0 < (s32)gPlayerCount) {
                goto loop;
            }
        }
    }
    gMenuFlowState |= 2;
    return 1;
}

void initRaceRecordSettingsFlow(void) {
    gRaceLapCount = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].lapCount;
    if (gRaceSplitscreenMode != 0) {
        setCurrentGameTaskCallback(initRaceSceneFlow, 0);
        return;
    }
    if (gPlayerCount == 1) {
        setCurrentGameTaskCallback(initRaceSceneFlow, 0);
        return;
    }
    if (gRaceRecordSettingsEnabled == 0) {
        setCurrentGameTaskCallback(initRaceSceneFlow, 0);
        return;
    }
    resetAllViewports();
    gFramebufferRenderInterval.value = 0;
    LOAD_ASSET(_593D10, 0x29);
    LOAD_ASSET(_60F1A0, 0x2A);
    LOAD_ASSET(_59DFE0, 0x26);
    LOAD_ASSET(_1F1A90, 0x28);
    switch (gRaceCourseIndex.unsignedValue) {
        case RACE_COURSE_BIG_SNOWMAN:
            LOAD_ASSET(BIG_SNOWMAN_COURSE_SPRITES, 0x1D);
            break;
        case RACE_COURSE_SUNSET_ROCK:
            LOAD_ASSET(SUNSET_ROCK_COURSE_SPRITES, 0x1D);
            break;
        case RACE_COURSE_NIGHT_HIGHWAY:
            LOAD_ASSET(NIGHT_HIGHWAY_COURSE_SPRITES, 0x1D);
            break;
        case RACE_COURSE_GRASS_VALLEY:
            LOAD_ASSET(GRASS_VALLEY_COURSE_SPRITES, 0x1D);
            break;
        case RACE_COURSE_DIZZY_LAND:
            LOAD_ASSET(DIZZY_LAND_COURSE_SPRITES, 0x1D);
            break;
        case RACE_COURSE_QUICKSAND_VALLEY:
            LOAD_ASSET(QUICKSAND_VALLEY_COURSE_SPRITES, 0x1D);
            break;
        case RACE_COURSE_SILVER_MOUNTAIN:
            LOAD_ASSET(SILVER_MOUNTAIN_COURSE_SPRITES, 0x1D);
            break;
        case RACE_COURSE_NINJA_LAND:
            LOAD_ASSET(NINJA_LAND_COURSE_SPRITES, 0x1D);
            break;
        case RACE_COURSE_ROOKIE_MOUNTAIN:
            LOAD_ASSET(ROOKIE_MOUNTAIN_COURSE_SPRITES, 0x1D);
            break;
    }
    LOAD_ASSET(RACE_EFFECT_SPRITES, 0x1C);
    initCallbackTaskScheduler(0);
    gMenuFlowState = 0;
    createCallbackTaskWithUserId((CallbackTaskCallback)initRaceRecordSettingsPanel, 0, 0x64, 0);
    createCallbackTask((CallbackTaskCallback)initScrollingMenuBackdropActor, 0, 0x5E);
    setCurrentGameTaskCallback(updateRaceRecordSettingsFlow, 0);
    requestMusicSequenceBank(7);
}

void updateRaceRecordSettingsFlow(void) {
    s32 pressedInput;

    gMenuFadeAlpha -= 0x10;
    if (gMenuFadeAlpha < 0) {
        gMenuFadeAlpha = 0;
        pressedInput = gPlayerInputPressed[0];
        if ((pressedInput & (STICK_DOWN | D_JPAD)) && (gRaceLapCount != 9)) {
            enqueueSoundEffect(SFX_MENU_CURSOR, SFX_PRIORITY_NORMAL);
            gRaceLapCount += 1;
            pressedInput = gPlayerInputPressed[0];
        }
        if ((pressedInput & (STICK_UP | U_JPAD)) && (gRaceLapCount != 1)) {
            enqueueSoundEffect(SFX_MENU_CURSOR, SFX_PRIORITY_NORMAL);
            gRaceLapCount -= 1;
            pressedInput = gPlayerInputPressed[0];
        }
        if (pressedInput & (A_BUTTON | START_BUTTON)) {
            gMenuFlowState = 1;
            enqueueSoundEffect(SFX_CONFIRM, SFX_PRIORITY_NORMAL);
            requestMusicSequenceStop(0x3C);
            setCurrentGameTaskCallback(fadeOutRaceRecordSettingsFlow, 0);
        }
    }
    updateCallbackTasks();
}

void fadeOutRaceRecordSettingsFlow(void) {
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0x100) {
        gMenuFadeAlpha = 0xFF;
        gFramebufferSwapHold = 1;
        setCurrentGameTaskCallback(&closeRaceRecordSettingsFlow, 0);
    }
    updateCallbackTasks();
}

void closeRaceRecordSettingsFlow(void) {
    if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferRenderInterval.value = 0;
        setCurrentGameTaskCallback(&initRaceSceneFlow, 0);
    }
}
