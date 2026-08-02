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

typedef struct {
    /* 0x00 */ s16 status;
    /* 0x02 */ s16 courseId;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
} CourseGridEntry;

typedef struct {
    /* 0x00 */ u8 pad0[0xAC];
    /* 0xAC */ u8 active;
    /* 0xAD */ u8 padAD[0xB0 - 0xAD];
} ViewportSlot;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad4[0x8 - 0x4];
    /* 0x08 */ s8 unk8;
} Unk80043040;

typedef union {
    s32 value;
    u8 padding[12];
} RaceResultsReadyState;

#ifdef NON_MATCHING
typedef struct {
    /* 0x00 */ u8 stickX;
    /* 0x01 */ u8 stickY;
} RaceCourseCharacterEntry;

#endif

extern CourseGridEntry *D_800DC490[];
extern u16 D_800DC5C0[];
extern s32 gRaceCameraReplayStartX;
extern s32 gRaceCameraReplayStartY;
extern s32 gRaceCameraReplayStartZ;
extern s32 gMenuFlowState;
extern s16 gRaceLapCount;
extern GameSaveRecordTime gRaceTimeTrialFinishTime;
extern s16 gMenuFadeAlpha;
extern s8 gRacePlayerCount;
extern s16 gRacePlayerAttackStartTimer;
#ifdef NON_MATCHING
extern u8 gTrainingCourseLesson;
extern u8 gMainMenuModeSelection;
extern u8 D_800DC5B4[];
extern u8 D_800DC4C4[][4];
extern RaceCourseCharacterEntry *D_800DC58C[];
extern void initCourseRecordBannerFadeOut(void *);
#endif
extern u8 D_80121B57;
extern u8 D_80121B60;
extern s8 D_80121B61;
extern u8 gRaceChallengeFailed;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern u8 gRumblePakConnectedMask;
extern u8 gPendingEndingCreditsFlow;
extern u8 gRaceUpdatePaused;
extern s16 gFrameCounter;
extern ViewportSlot D_801121E0[];

extern void releaseMenuAssetHandles(void);
extern void enqueueSoundEffect(s32, s32);

void initStartupControllerPakFlow(void) {
    LOAD_ASSET(_2427D0, 6);
    initCallbackTaskScheduler(0);
    gCurrentGameTask->fadeTimer = 0xA;
    setCurrentGameTaskCallback(&waitStartupRumbleInit, 0);
}

void waitStartupRumbleInit(void) {
    gCurrentGameTask->fadeTimer -= 1;
    if (gCurrentGameTask->fadeTimer == 0) {
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
    CourseGridEntry *entry = D_800DC490[gRaceCourseIndex.signedValue];
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
    CourseGridEntry *entry = D_800DC490[gRaceCourseIndex.signedValue];
    s32 i = 0;

loop:
    if (entry->status != COURSE_GRID_ENTRY_END) {
        if ((entry->status == COURSE_GRID_ENTRY_FREE) && (entry->courseId == gRacePlayers[0].coursePathIndex)) {
            s32 *status = (s32 *)getRelocatableHeapBlockBase(gAssetHandles[0x2B]);
            if (*status < 0x1194) {
                entry->status = *status;
                ((RacePlayer *) getRelocatableHeapBlockBase(gAssetHandles[0x2C]))[i] = gRacePlayers[0];
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

    entry = D_800DC490[gRaceCourseIndex.signedValue];
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
    CourseGridEntry *entry = D_800DC490[gRaceCourseIndex.signedValue];

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
    gRaceCourseIndex.signedValue = 0;
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
    setCurrentGameTaskCallback(&handleRaceRecordSaveOptionsFlow, 0);
    createGameTask(4, &initControllerPakRaceRecordSaveFlow, 0x64);
    suspendGameTask(2);
}

void handleRaceRecordSaveOptionsFlow(void) {
    if (gMenuFlowState == 1) {
        gMenuFlowState = 0;
        setCurrentGameTaskCallback(&openRaceSplitscreenSelectFlow, 0);
    } else {
        setCurrentGameTaskCallback(&exitRaceFlowToMainMenu, 0);
    }
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

void openRaceStartTransitionFlow(void) {
    setCurrentGameTaskCallback(&openEndingCreditsIfUnlockedFlow, 0);
    createGameTask(4, &initRaceStartTransition, 0x64);
    suspendGameTask(2);
}

// initRaceSceneFlow best match: 95.527% (nonmatchings/initRaceSceneFlow-2781615007300307775/base_23.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/flow/race_flow/initRaceSceneFlow.s")

#ifdef NON_MATCHING
void initRaceSceneFlow(void) {
    s32 i;
    RacePlayer *player;
    RacePlayer *end;
    RacePlayer *other;
    RaceInputHistoryBuffer *history;
    RaceCourseCharacterEntry *courseCharacters;
    u8 *courseCharacterIds;
    u16 *replayRecordFlag;
    s32 duplicate;
    s32 playerCount;
    s32 courseIndex;

    gRaceUpdatePaused = 0;
    D_80121B57 = 0;
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

    courseIndex = gRaceCourseIndex.signedValue;
    gRacePlayers[0].actionTriggerChance = D_800DC5B4[courseIndex];
    gRacePlayers[1].actionTriggerChance = D_800DC5B4[courseIndex];
    gRacePlayers[2].actionTriggerChance = D_800DC5B4[courseIndex];
    gRacePlayers[3].actionTriggerChance = D_800DC5B4[courseIndex];
    gRacePlayers[0].itemTriggerChance = D_800DC5B4[courseIndex];
    gRacePlayers[1].itemTriggerChance = D_800DC5B4[courseIndex];
    gRacePlayers[2].itemTriggerChance = D_800DC5B4[courseIndex];
    gRacePlayers[3].itemTriggerChance = D_800DC5B4[courseIndex];
    playerCount = gPlayerCount;
    if (playerCount != 1) {
        gRacePlayers[0].actionTriggerChance = 0;
        gRacePlayers[1].actionTriggerChance = 0;
        gRacePlayers[2].actionTriggerChance = 0;
        gRacePlayers[3].actionTriggerChance = 0;
        gRacePlayers[0].itemTriggerChance = 0;
        gRacePlayers[1].itemTriggerChance = 0;
        gRacePlayers[2].itemTriggerChance = 0;
        gRacePlayers[3].itemTriggerChance = 0;
    }

    gRacePlayers[0].unk17 = 0;
    gRacePlayers[1].unk17 = 1;
    gRacePlayers[2].unk17 = 2;
    gRacePlayers[3].unk17 = 3;

    if (playerCount > 0) {
        player = gRacePlayers;
        end = &gRacePlayers[playerCount];
        do {
            player->isCpu = 0;
            player++;
        } while (player < &gRacePlayers[playerCount]);
    }

    gRacePlayers[0].characterId = gRacePlayers[0].selectedCharacterId;
    gRacePlayers[1].characterId = gRacePlayers[1].selectedCharacterId;
    gRacePlayers[2].characterId = gRacePlayers[2].selectedCharacterId;
    gRacePlayers[3].characterId = gRacePlayers[3].selectedCharacterId;
    gRacePlayers[0].characterVariant = gRacePlayers[0].menuSelection;
    gRacePlayers[1].characterVariant = gRacePlayers[1].menuSelection;
    gRacePlayers[2].characterVariant = gRacePlayers[2].menuSelection;
    gRacePlayers[3].characterVariant = gRacePlayers[3].menuSelection;
    gRacePlayers[0].unk12 = gRacePlayers[0].selectionUnlockState;
    gRacePlayers[1].unk12 = gRacePlayers[1].selectionUnlockState;
    gRacePlayers[2].unk12 = gRacePlayers[2].selectionUnlockState;
    gRacePlayers[3].unk12 = gRacePlayers[3].selectionUnlockState;

    if (playerCount < 4) {
        player = &gRacePlayers[playerCount];
        courseCharacters = D_800DC58C[courseIndex];
        courseCharacterIds = D_800DC4C4[courseIndex];
        do {
            for (;;) {
                duplicate = 1;
                player->characterId = *courseCharacterIds++;
                if (playerCount > 0) {
                    other = gRacePlayers;
                    end = &gRacePlayers[playerCount];
                    do {
                        if (player->characterId == other->characterId) {
                            duplicate = 0;
                        }
                        other++;
                    } while (other < end);
                }
                if (duplicate != 0) {
                    break;
                }
            }
            player->characterVariant = courseCharacters[player->characterId].stickX;
            player->unk12 = courseCharacters[player->characterId].stickY;
            player++;
        } while (player < gRacePlayersEnd);
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

    for (i = 0; i < RACE_PLAYER_COUNT; i++) {
        gRacePlayers[i].unk15 = 0;
        gRacePlayers[i].replayInputSource = 0;
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
        history->characterVariant = gRacePlayers[0].characterVariant;
        history->unkC = gRacePlayers[0].unk12;
        i = 0;
        do {
            history->stickX[i + 1] = 0;
            history->stickY[i + 1] = 0;
            history->buttons[i + 1] = 0;
            history->stickX[i + 2] = 0;
            history->stickY[i + 2] = 0;
            history->buttons[i + 2] = 0;
            history->stickX[i + 3] = 0;
            history->stickY[i + 3] = 0;
            history->buttons[i + 3] = 0;
            i += 4;
            history->stickX[i - 4] = 0;
            history->stickY[i - 4] = 0;
            history->buttons[i - 4] = 0;
        } while (i != RACE_INPUT_HISTORY_LENGTH);

        switch (gRaceCourseIndex.signedValue) {
        case 0:
            replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[0].length;
            break;
        case 1:
            replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[1].length;
            break;
        case 2:
            replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[2].length;
            break;
        case 3:
            replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[3].length;
            break;
        case 4:
            replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[4].length;
            break;
        case 5:
            replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[5].length;
            break;
        case 6:
            replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[6].length;
            break;
        case 8:
            replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[7].length;
            break;
        case 9:
            replayRecordFlag = &gGameSaveDataBuffer[0].replaySlots[8].length;
            break;
        }
        if ((gRaceCourseIndex.signedValue != 7) && (*replayRecordFlag != 0)) {
            gAssetHandles[0x7] = allocRelocatableHeapBlock(0x34CC);
            loadCurrentRaceRecordReplayData();
            history = getRelocatableHeapBlockBase(gAssetHandles[0x7]);
            if (history->enabled != 0) {
                history->writeIndex = 0;
                gRacePlayers[1].isActive = 1;
                gRacePlayers[1].isCpu = 0;
                gRacePlayers[1].soundDisabled = 1;
                gRacePlayers[1].replayInputSource = 1;
                gRacePlayers[1].characterId = history->characterId;
                gRacePlayers[1].characterVariant = history->characterVariant;
                gRacePlayers[1].unk12 = history->unkC;
                gRacePlayerCount = 2;
            }
        }
    }

    loadRaceCourseAssets();
    loadRaceCharacterAssets();
    gMenuFlowState = 1;
    resetRaceCameras();
    resetAllViewports();
    if (gRaceCourseIndex.signedValue != 6) {
        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    } else {
        configureRaceViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    }
    gRacePlayerHudStatuses[0].active = 1;
    gFramebufferSwapDelay.value = 0;
    initRacePlayers();
    initRaceHud();
    initRaceCourseSceneTasks();
    gMenuFadeAlpha = 0xFF;
    if (gRaceSplitscreenMode == 2) {
        resetSecondaryRng();
    }
    gRaceRumbleEnabled = 1;
    if (gRaceSplitscreenMode == 0) {
        gCurrentGameTask->fadeTimer = 0;
        gCurrentGameTask->unk1C = 0;
        gCurrentGameTask->countdown = 0x1E;
        createCallbackTask((CallbackTaskCallback)initCourseRecordBannerFadeOut, 0, 0x64);
        setCurrentGameTaskCallback(fadeOutRaceStartTransitionFlow, 0);
    } else {
        createCallbackTask((CallbackTaskCallback)initRaceCountdownPrompt, 6, 0x64);
        createCallbackTask((CallbackTaskCallback)func_80065E90, 6, 0x64);
        setCurrentGameTaskCallback(startRaceGameplayFlow, 0);
    }

    i = 0;
    if ((s32)gPlayerCount > 0) {
        do {
            requestRumbleMotorInit(i);
            i++;
        } while (i < (s32)gPlayerCount);
    }
    updateRelocatableHeap();
}
#endif

void fadeOutRaceStartTransitionFlow(void) {
    gMenuFadeAlpha -= 8;
    if (gMenuFadeAlpha < 0) {
        gMenuFadeAlpha = 0;
        gCurrentGameTask->countdown--;
    }
    if (gCurrentGameTask->countdown == 0) {
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
    temp_a0 = state->unk1C;
    if (temp_a0 >= 0x32) {
        gMenuFadeOverlayActive = 1;
        gMenuFadeAlpha += 0x16;
        if (gMenuFadeAlpha >= 0x100) {
            setRaceCameraMode(0, 1);
            gMenuFadeAlpha = 0xFF;
            createCallbackTask((CallbackTaskCallback)initRaceCountdownPrompt, 6, 0x64);
            createCallbackTask((CallbackTaskCallback)func_80065E90, 6, 0x64);
            switch (gPlayerCount & 0xFFFFFFFF) {
            case 1:
                if (gRaceCourseIndex.signedValue != 6) {
                    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
                } else {
                    configureRaceViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
                }
                D_801121E0[0].active = 1;
                gFramebufferSwapDelay.value = 0;
                break;
            case 2:
                if (gRaceCourseIndex.signedValue != 6) {
                    configureViewport(0, 0xA0, 0x43, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
                    configureViewport(1, 0xA0, 0xAD, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
                } else {
                    configureRaceViewport(0, 0xA0, 0x43, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
                    configureRaceViewport(1, 0xA0, 0xAD, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
                }
                D_801121E0[0].active = 1;
                D_801121E0[1].active = 1;
                gFramebufferSwapDelay.value = 1;
                break;
            case 3:
                if (gRaceCourseIndex.signedValue != 6) {
                    configureViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    configureViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    configureViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                } else {
                    configureRaceViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    configureRaceViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    configureRaceViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                }
                D_801121E0[0].active = 1;
                D_801121E0[1].active = 1;
                D_801121E0[2].active = 1;
                gFramebufferSwapDelay.value = 1;
                break;
            case 4:
                if (gRaceCourseIndex.signedValue != 6) {
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
                D_801121E0[0].active = 1;
                D_801121E0[1].active = 1;
                D_801121E0[2].active = 1;
                D_801121E0[3].active = 1;
                gFramebufferSwapDelay.value = 1;
                break;
            }
            setCurrentGameTaskCallback(startRaceGameplayFlow, 0);
        }
    } else {
        state->unk1C = temp_a0 + 1;
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
    if (D_80121B57 == 0) {
        color = 0x1B;
    }
    drawAssetTableSpriteWithExplicitPalette(-0x1C, 0, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x58, color);
    color = 0x1A;
    if (D_80121B57 == 1) {
        color = 0x1B;
    }
    drawAssetTableSpriteWithExplicitPalette(-0x1C, 0xA, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x59, color);
    color = 0x1A;
    if (D_80121B57 == 2) {
        color = 0x1B;
    }
    drawAssetTableSpriteWithExplicitPalette(-0x1C, 0x14, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x5A, color);
}

void updateRaceGameplayFlow(void) {
    RacePlayer *player;
    s32 opened;
    s32 i;
    s32 input;
    s32 selection;
    s32 valueTwo;

    opened = 0; i = 0; if (gPlayerCount > 0) { player = gRacePlayers; do { if (((player->soundDisabled == 0) && (gRaceUpdatePaused == 0)) && (gPlayerInputPressed[i] & START_BUTTON)) { D_80121B57 = 0; gRaceUpdatePaused = 1; opened = 1; enqueueSoundEffect(1, 0x32); requestRumbleMotorInit(0); requestRumbleMotorInit(1); requestRumbleMotorInit(2); requestRumbleMotorInit(3); } i++; player++; } while (i < gPlayerCount); } valueTwo = 2; if ((gRaceUpdatePaused != 0) && (opened == 0)) { i = 0; if (gPlayerCount > 0) { player = gRacePlayers; do { if (player->soundDisabled == 0) { input = gPlayerInputPressed[i]; if (input & (STICK_UP | U_JPAD)) { selection = D_80121B57; if (selection != 0) { D_80121B57 = selection - 1; enqueueSoundEffect(1, 0x32); input = gPlayerInputPressed[i]; }
                    }
                    if (input & (STICK_DOWN | D_JPAD)) {
                        selection = D_80121B57;
                        if (valueTwo != selection) {
                            D_80121B57 = selection + 1;
                            enqueueSoundEffect(1, 0x32);
                            input = gPlayerInputPressed[i];
                        }
                    }
                    if (input & START_BUTTON) {
                        input = D_80121B57;
                        if (input == 0) {
                            gRaceUpdatePaused = 0;
                            enqueueSoundEffect(1, 0x32);
                            input = D_80121B57;
                        }
                        if (input == 1) {
                            gRaceRumbleEnabled = 0;
                            gRaceUpdatePaused = 0;
                            gFramebufferSwapHold = 1;
                            enqueueSoundEffect(1, 0x32);
                            setCurrentGameTaskCallback(finalizeRaceExitFlow, 0);
                            return;
                        }
                        if (valueTwo == input) {
                            gRaceRumbleEnabled = 0;
                            gRaceUpdatePaused = 0;
                            gFramebufferSwapHold = 1;
                            enqueueSoundEffect(1, 0x32);
                            setCurrentGameTaskCallback(finalizeRaceExitFlow, 0);
                            return;
                        }
                    }
                }
                i++;
                player++;
            } while (i < gPlayerCount);
        }
        addRenderCallback(&gMenuRenderCallbackList, drawRacePauseMenu, 0);
    }

    if (gRaceSplitscreenMode == valueTwo) {
        saveRaceReplayCourseGridEntry();
    }
    updateRaceFlowFrameWithCourseEffects();
    if (areRacePlayersFinished() != 0) {
        gCurrentGameTask->fadeTimer = 0x3C;
        gRaceRumbleEnabled = 0;
        setCurrentGameTaskCallback(waitRaceFinishResultsFlow, 0);
    }
}

void waitRaceFinishResultsFlow(void) {
    D_80121B57 = 0;
    updateRaceFlowFrameWithCourseEffects();
    gCurrentGameTask->fadeTimer--;
    if (gCurrentGameTask->fadeTimer == 0) {
        gMenuFlowState |= 8;
        switch (gPlayerCount) {
        case 3:
            gCurrentGameTask->unk1C = 0;
            if (gRacePlayers[1].rankIndex < gRacePlayers[gCurrentGameTask->unk1C].rankIndex) {
                gCurrentGameTask->unk1C = 1;
            }
            if (gRacePlayers[2].rankIndex < gRacePlayers[gCurrentGameTask->unk1C].rankIndex) {
                gCurrentGameTask->unk1C = 2;
            }
            gCurrentGameTask->unk1C += 2;
            setCurrentGameTaskCallback(zoomRaceWinnerViewport, 0);
            break;
        case 4:
            gCurrentGameTask->unk1C = 0;
            if (gRacePlayers[1].rankIndex < gRacePlayers[gCurrentGameTask->unk1C].rankIndex) {
                gCurrentGameTask->unk1C = 1;
            }
            if (gRacePlayers[2].rankIndex < gRacePlayers[gCurrentGameTask->unk1C].rankIndex) {
                gCurrentGameTask->unk1C = 2;
            }
            if (gRacePlayers[3].rankIndex < gRacePlayers[gCurrentGameTask->unk1C].rankIndex) {
                gCurrentGameTask->unk1C = 3;
            }
            gCurrentGameTask->unk1C += 5;
            setCurrentGameTaskCallback(zoomRaceWinnerViewport, 0);
            break;
        case 2:
            if (gRacePlayers[0].rankIndex < gRacePlayers[1].rankIndex) {
                gCurrentGameTask->unk1C = 0;
                gRacePlayerHudStatuses[1].active = 0;
            } else {
                gCurrentGameTask->unk1C = 1;
                gRacePlayerHudStatuses[0].active = 0;
            }
            gCurrentGameTask->fadeTimer = 0;
            setCurrentGameTaskCallback(zoomRaceWinnerViewport, 0);
            break;
        case 1:
            gCurrentGameTask->fadeTimer = 0;
            setCurrentGameTaskCallback(prepareRaceResultsFlow, 0);
            break;
        }
    }
}

void interpolateRaceViewport(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6,
                  f32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11, s32 arg12, s32 arg13,
                  f32 arg14) {
    s32 sp2C;
    s32 sp28;
    f32 temp_fv0;
    s32 temp_t0;
    s32 temp_t1;
    s32 temp_t2;
    s32 temp_t3;

    sp2C = (((arg8 - arg1) * gCurrentGameTask->fadeTimer) / 15) + arg1;
    sp28 = (((arg9 - arg2) * gCurrentGameTask->fadeTimer) / 15) + arg2;
    temp_t0 = (((arg10 - arg3) * gCurrentGameTask->fadeTimer) / 15) + arg3;
    temp_t1 = (((arg11 - arg4) * gCurrentGameTask->fadeTimer) / 15) + arg4;
    temp_t2 = (((arg12 - arg5) * gCurrentGameTask->fadeTimer) / 15) + arg5;
    temp_t3 = (((arg13 - arg6) * gCurrentGameTask->fadeTimer) / 15) + arg6;
    temp_fv0 = (((arg14 - arg7) * (f32)gCurrentGameTask->fadeTimer) / 15.0f) + arg7;
    if (gRaceCourseIndex.signedValue != 6) {
        configureViewport(arg0, sp2C, sp28, temp_t0, temp_t1, temp_t2, temp_t3, temp_fv0);
    } else {
        configureRaceViewport(arg0, sp2C, sp28, temp_t0, temp_t1, temp_t2, temp_t3, temp_fv0);
    }
}

void zoomRaceWinnerViewport(void) {
    gCurrentGameTask->fadeTimer += 1;
    switch (gCurrentGameTask->unk1C) {
    case 0:
        interpolateRaceViewport(0, 0xA0, 0x43, 0x120, 0x68, 0x140, 0x78, 2.6666667f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        interpolateRaceViewport(1, 0xA0, 0xAD, 0x120, 0x68, 0x140, 0x78, 2.6666667f, 0xA0, 0x115, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
        if (gCurrentGameTask->fadeTimer == 0xF) {
            resetViewport(1);
            D_801121E0[1].active = 0;
        }
        break;
    case 1:
        interpolateRaceViewport(1, 0xA0, 0xAD, 0x120, 0x68, 0x140, 0x78, 2.6666667f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        interpolateRaceViewport(0, 0xA0, 0x43, 0x120, 0x68, 0x140, 0x78, 2.6666667f, 0xA0, -0x25, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
        if (gCurrentGameTask->fadeTimer == 0xF) {
            resetViewport(0);
            D_801121E0[0].active = 0;
        }
        break;
    case 2:
        interpolateRaceViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        interpolateRaceViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x57, 0x115, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        interpolateRaceViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x179, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        if (gCurrentGameTask->fadeTimer == 0xF) {
            D_801121E0[1].active = 0;
            D_801121E0[2].active = 0;
            resetViewport(1);
            resetViewport(2);
        }
        break;
    case 3:
        interpolateRaceViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        interpolateRaceViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x57, -0x25, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        interpolateRaceViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x179, -0x25, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        if (gCurrentGameTask->fadeTimer == 0xF) {
            D_801121E0[0].active = 0;
            D_801121E0[2].active = 0;
            resetViewport(0);
            resetViewport(2);
        }
        break;
    case 4:
        interpolateRaceViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        interpolateRaceViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, -0x39, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        interpolateRaceViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, -0x39, 0x115, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        if (gCurrentGameTask->fadeTimer == 0xF) {
            D_801121E0[0].active = 0;
            D_801121E0[1].active = 0;
            resetViewport(0);
            resetViewport(1);
        }
        break;
    case 5:
        interpolateRaceViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        interpolateRaceViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x57, 0x115, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        interpolateRaceViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x179, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        interpolateRaceViewport(3, 0xE9, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x179, 0x115, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        if (gCurrentGameTask->fadeTimer == 0xF) {
            resetViewport(1);
            resetViewport(2);
            resetViewport(3);
            D_801121E0[1].active = 0;
            D_801121E0[2].active = 0;
            D_801121E0[3].active = 0;
        }
        break;
    case 6:
        interpolateRaceViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        interpolateRaceViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x57, -0x25, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        interpolateRaceViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x179, -0x25, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        interpolateRaceViewport(3, 0xE9, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x179, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        if (gCurrentGameTask->fadeTimer == 0xF) {
            D_801121E0[0].active = 0;
            D_801121E0[2].active = 0;
            D_801121E0[3].active = 0;
            resetViewport(0);
            resetViewport(2);
            resetViewport(3);
        }
        break;
    case 7:
        interpolateRaceViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        interpolateRaceViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, -0x39, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        interpolateRaceViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, -0x39, 0x115, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        interpolateRaceViewport(3, 0xE9, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xE9, 0x115, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        if (gCurrentGameTask->fadeTimer == 0xF) {
            D_801121E0[0].active = 0;
            D_801121E0[1].active = 0;
            D_801121E0[3].active = 0;
            resetViewport(0);
            resetViewport(1);
            resetViewport(3);
        }
        break;
    case 8:
        interpolateRaceViewport(3, 0xE9, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        interpolateRaceViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, -0x39, -0x25, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        interpolateRaceViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, -0x39, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        interpolateRaceViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xE9, -0x25, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        if (gCurrentGameTask->fadeTimer == 0xF) {
            D_801121E0[0].active = 0;
            D_801121E0[1].active = 0;
            D_801121E0[2].active = 0;
            resetViewport(0);
            resetViewport(1);
            resetViewport(2);
        }
        break;
    }
    updateRaceFlowFrame();
    if (gCurrentGameTask->fadeTimer == 0xF) {
        setCurrentGameTaskCallback(prepareRaceResultsFlow, 0);
    }
}

void prepareRaceResultsFlow(void) {
    RacePlayer *player;
    s32 i;
    s32 recordTime;
    s32 currentTime;

    gPendingEndingCreditsFlow = 0;
    gFramebufferSwapDelay.value = 0;
    D_80121B60 = 0;
    D_80121B61 = 0;

    switch (gRaceSplitscreenMode) {
    case 0:
        gRaceResultState = 1;
        i = 0;
        if (gPlayerCount > 0) {
            player = gRacePlayers;
            do {
                if (player->rankIndex == 0) {
                    D_80121B60 = i + 1;
                }
                i++;
                player++;
            } while (i < gPlayerCount);
        }
        break;

    case 2:
        currentTime = gRaceElapsedTimer.fraction + (gRaceElapsedTimer.seconds * COURSE_TIME_SECOND) +
                      (gRaceElapsedTimer.minutes * COURSE_TIME_MINUTE);
        i = 0;
        do {
            recordTime = gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][i].fraction +
                         (gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][i].seconds * COURSE_TIME_SECOND) +
                         (gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][i].minutes * COURSE_TIME_MINUTE);
            if (currentTime < recordTime) {
                break;
            }
            i++;
        } while (i < 5);
        if (i < 5) {
            gRaceResultState = 1;
            D_80121B60 = 1;
            if (i == 0) {
                D_80121B61 = 1;
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
                    do {
                        if (gGameSaveDataBuffer[0].trickAttackScores[gRaceCourseIndex.signedValue][i] < gRacePlayers[0].trickAttackPointTotal) {
                            break;
                        }
                        i++;
                    } while (i < 5);
                    if (gRaceChallengeFailed != 0) {
                        i = 5;
                    }
                    if (i < 5) {
                        gRaceResultState = 1;
                        D_80121B60 = 1;
                    } else {
                        gRaceResultState = 2;
                    }
                }
            } else {
                i = 0;
                do {
                    if (gGameSaveDataBuffer[0].scoreAttackScores[gRaceCourseIndex.signedValue][i] < gRacePlayers[0].scoreAttackPointTotal) {
                        break;
                    }
                    i++;
                } while (i < 5);
                if (gRaceChallengeFailed != 0) {
                    i = 5;
                }
                if (i < 5) {
                    gRaceResultState = 1;
                    D_80121B60 = 1;
                } else {
                    gRaceResultState = 2;
                }
            }
        } else {
            currentTime = gRaceElapsedTimer.fraction + (gRaceElapsedTimer.seconds * COURSE_TIME_SECOND) +
                          (gRaceElapsedTimer.minutes * COURSE_TIME_MINUTE);
            i = 0;
            do {
                recordTime = gGameSaveDataBuffer[0].raceRecords[gRaceCourseIndex.signedValue][i].fraction +
                             (gGameSaveDataBuffer[0].raceRecords[gRaceCourseIndex.signedValue][i].seconds * COURSE_TIME_SECOND) +
                             (gGameSaveDataBuffer[0].raceRecords[gRaceCourseIndex.signedValue][i].minutes * COURSE_TIME_MINUTE);
                if (currentTime < recordTime) {
                    break;
                }
                i++;
            } while (i < 5);
            if (gRaceChallengeFailed != 0) {
                i = 5;
            }
            if (i < 5) {
                gRaceResultState = 1;
                D_80121B60 = 1;
            } else {
                gRaceResultState = 2;
            }
        }
        break;
    }

    gCurrentGameTask->unk1C = 0x3C;
    setCurrentGameTaskCallback(updateRaceResultsFlow, 0);
    if (D_80121B60 != 0) {
        requestMusicSequenceBank(6);
        if ((gPlayerCount == 1) && (gRaceSplitscreenMode == 0)) {
            createCallbackTaskWithUserId((CallbackTaskCallback)updateRaceUiResultAwardBadgeDelay, 6, 0x64, 0xA9);
        }
        createCallbackTaskWithUserId((CallbackTaskCallback)initFallingMenuSnowflake, 5, 0x64, D_80121B60 - 1);
    }
    updateRaceFlowFrame();
}

void updateRaceResultsFlow(void) {
    RaceResultsReadyState ready;
    s32 currentTime;
    s32 recordTime;
    s32 recordIndex;
    s32 shiftIndex;
    CallbackTask *resultActor;

    gPendingEndingCreditsFlow = 0;
    gFramebufferSwapDelay.value = 0;
    ready.value = 1;
    if ((s32)gPlayerCount > 0) {
        recordIndex = 0;
        do {
            if (!(gRacePlayers[recordIndex].stateFlags & 0x08000000)) {
                ready.value = 0;
            }
            recordIndex++;
        } while (recordIndex < (s32)gPlayerCount);
    }

    if (gCurrentGameTask->fadeStep != 0) {
        gCurrentGameTask->fadeStep--;
    } else {
        ready.value = 1;
    }
    if ((D_80121B60 != 0) && (countActiveMusicSequences() != 0)) {
        ready.value = 0;
    }

    if (ready.value != 0) {
        if (D_80121B60 == 0) {
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
                    if (gRacePlayers[0].rankIndex != 3) {
                        if (gGameSaveDataBuffer[0].cupPlacements[gRaceCourseIndex.signedValue] == 0) {
                            gGameSaveDataBuffer[0].cupPlacements[gRaceCourseIndex.signedValue] =
                                gRacePlayers[0].rankIndex + 1;
                        } else if ((gRacePlayers[0].rankIndex + 1) <
                                   gGameSaveDataBuffer[0].cupPlacements[gRaceCourseIndex.signedValue]) {
                            gGameSaveDataBuffer[0].cupPlacements[gRaceCourseIndex.signedValue] =
                                gRacePlayers[0].rankIndex + 1;
                        }
                        if (gGameSaveDataBuffer[0].highScores[gRaceCourseIndex.signedValue + 1] <
                            gRacePlayers[0].unk56C) {
                            gGameSaveDataBuffer[0].highScores[gRaceCourseIndex.signedValue + 1] =
                                gRacePlayers[0].unk56C;
                        }
                    }
                    if (gRacePlayers[0].rankIndex == 0) {
                        if (gRaceCourseIndex.signedValue == 8) {
                            gPendingEndingCreditsFlow = 1;
                            gGameSaveDataBuffer[0].extraCourseUnlockFlags |= 1;
                            gGameSaveDataBuffer[0].cupPlacements[0x17] |= 1;
                        }
                    } else {
                        gMenuFlowState |= 0x20;
                    }
                    gCurrentGameTask->fade = 0xA;
                    setCurrentGameTaskCallback(updateRaceResultsMusicFlow, 0);
                }
            } else {
                resultActor =
                    createCallbackTask((CallbackTaskCallback)initRaceUiResultsBanner, 6, 0x64);
                if (resultActor != NULL) {
                    recordIndex = 0; do {
                        if (gRacePlayers[recordIndex].rankIndex == 0) {
                            gRacePlayers[recordIndex].unk18++;
                            if (gRacePlayers[recordIndex].unk18 >= 0x64) {
                                gRacePlayers[recordIndex].unk18 = 0x63;
                            }
                        }
                        recordIndex++;
                    } while (&gRacePlayers[recordIndex] < (RacePlayer *)&gFrameCounter);

                    ready.value = 0;
                    if ((s32)gPlayerCount > 0) {
                        recordIndex = 0;
                        do {
                            if (gRacePlayers[recordIndex].rankIndex == 0) {
                                ready.value = 1;
                            }
                            recordIndex++;
                        } while (recordIndex < (s32)gPlayerCount);
                    }
                    if (ready.value == 0) {
                        gMenuFlowState |= 0x20;
                    }
                    gCurrentGameTask->fade = 0xA;
                    setCurrentGameTaskCallback(updateRaceResultsMusicFlow, 0);
                }
            }
            break;

        case 2:
            resultActor = createCallbackTask((CallbackTaskCallback)func_8005A2F0, 6, 0x64);
            if (resultActor != NULL) {
                currentTime = gRaceElapsedTimer.fraction +
                              (gRaceElapsedTimer.seconds * COURSE_TIME_SECOND) +
                              (gRaceElapsedTimer.minutes * COURSE_TIME_MINUTE);
                recordIndex = 0;
                do {
                    recordTime =
                        gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][recordIndex].fraction +
                        (gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][recordIndex].seconds *
                         COURSE_TIME_SECOND) +
                        (gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][recordIndex].minutes *
                         COURSE_TIME_MINUTE);
                    if (currentTime < recordTime) {
                        break;
                    }
                    recordIndex++;
                } while (recordIndex < 5);

                resultActor->userId = recordIndex;
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
                        gRacePlayers[0].characterId + (gRacePlayers[0].characterVariant * 8);
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
                gCurrentGameTask->fade = 0xA;
                setCurrentGameTaskCallback(updateRaceResultsMusicFlow, 0);
            }
            break;

        case 1:
            switch (gRaceTypeSelection) {
            case 2:
                resultActor =
                    createCallbackTask((CallbackTaskCallback)initRaceUiTrickPrizePayout, 6, 0x64);
                if (resultActor != NULL) {
                    recordIndex = 0; do {
                        if (gGameSaveDataBuffer[0].trickAttackScores[gRaceCourseIndex.signedValue][recordIndex] <
                            gRacePlayers[0].trickAttackPointTotal) {
                            break;
                        }
                        recordIndex++;
                    } while (recordIndex < 5);
                    if (gRaceChallengeFailed != 0) {
                        recordIndex = 5;
                    }
                    resultActor->userId = recordIndex;
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
                            gRacePlayers[0].trickAttackPointTotal;
                        gGameSaveDataBuffer[0].trickAttackCharacterIds[gRaceCourseIndex.signedValue][recordIndex] =
                            gRacePlayers[0].characterId + (gRacePlayers[0].characterVariant * 8);
                    }
                    if (gRacePlayers[0].trickAttackPointTotal >= 0x7D0) {
                        gGameSaveDataBuffer[0].extraCourseUnlockFlags |= 2;
                    }
                    if (recordIndex < 5) {
                        gRaceResultState = 1;
                    } else {
                        gRaceResultState = 2;
                        gMenuFlowState |= 0x20;
                    }
                    gCurrentGameTask->fade = 0xA;
                    setCurrentGameTaskCallback(updateRaceResultsMusicFlow, 0);
                }
                break;

            case 1:
                resultActor =
                    createCallbackTask((CallbackTaskCallback)initRaceUiHitPrizePayout, 6, 0x64);
                if (resultActor != NULL) {
                    recordIndex = 0; do {
                        if (gGameSaveDataBuffer[0].scoreAttackScores[gRaceCourseIndex.signedValue][recordIndex] <
                            gRacePlayers[0].scoreAttackPointTotal) {
                            break;
                        }
                        recordIndex++;
                    } while (recordIndex < 5);
                    if (gRaceChallengeFailed != 0) {
                        recordIndex = 5;
                    }
                    resultActor->userId = recordIndex;
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
                            gRacePlayers[0].scoreAttackPointTotal;
                        gGameSaveDataBuffer[0].scoreAttackCharacterIds[gRaceCourseIndex.signedValue][recordIndex] =
                            gRacePlayers[0].characterId + (gRacePlayers[0].characterVariant * 8);
                    }
                    if (recordIndex < 5) {
                        gRaceResultState = 1;
                    } else {
                        gRaceResultState = 2;
                        gMenuFlowState |= 0x20;
                    }
                    gCurrentGameTask->fade = 0xA;
                    setCurrentGameTaskCallback(updateRaceResultsMusicFlow, 0);
                }
                break;

            case 0:
                resultActor = createCallbackTask((CallbackTaskCallback)func_8005CE4C, 6, 0x64);
                if (resultActor != NULL) {
                    currentTime = gRaceElapsedTimer.fraction +
                                  (gRaceElapsedTimer.seconds * COURSE_TIME_SECOND) +
                                  (gRaceElapsedTimer.minutes * COURSE_TIME_MINUTE);
                    recordIndex = 0;
                    do {
                        recordTime =
                            gGameSaveDataBuffer[0].raceRecords[gRaceCourseIndex.signedValue][recordIndex].fraction +
                            (gGameSaveDataBuffer[0].raceRecords[gRaceCourseIndex.signedValue][recordIndex].seconds *
                             COURSE_TIME_SECOND) +
                            (gGameSaveDataBuffer[0].raceRecords[gRaceCourseIndex.signedValue][recordIndex].minutes *
                             COURSE_TIME_MINUTE);
                        if (currentTime < recordTime) {
                            break;
                        }
                        recordIndex++;
                    } while (recordIndex < 5);
                    if (gRaceChallengeFailed != 0) {
                        recordIndex = 5;
                    }
                    resultActor->userId = recordIndex;
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
                            gRacePlayers[0].characterId + (gRacePlayers[0].characterVariant * 8);
                    }
                    if (recordIndex < 5) {
                        gRaceResultState = 1;
                    } else {
                        gRaceResultState = 2;
                        gMenuFlowState |= 0x20;
                    }
                    gCurrentGameTask->fade = 0xA;
                    setCurrentGameTaskCallback(updateRaceResultsMusicFlow, 0);
                }
                break;
            }
            break;
        }
    }

    if (D_80121B60 != 0) {
        createCallbackTaskWithUserId((CallbackTaskCallback)initFallingMenuSnowflake, 5, 0x64,
                                     D_80121B60 - 1);
    }
    updateRaceFlowFrame();
}

void updateRaceResultsMusicFlow(void) {
    if (countActiveMusicSequences() == 0) {
        requestMusicSequenceBank(7);
        gCurrentGameTask->unk1C = 0x3C;
        gMenuFlowState |= 0x20;
    }
    if (D_80121B60 != 0) {
        createCallbackTaskWithUserId((CallbackTaskCallback)&initFallingMenuSnowflake, 5, 0x64, D_80121B60 - 1);
    }
    updateRaceFlowFrame();
    if (gMenuFlowState & 0x10) {
        gCurrentGameTask->fadeTimer -= 1;
        if (gCurrentGameTask->fadeTimer == 0) {
            gCurrentGameTask->fadeTimer = 4;
            setCurrentGameTaskCallback(&fadeOutRaceResultsFlow, 0);
            requestMusicSequenceStop(0x14);
        }
    }
}

void fadeOutRaceResultsFlow(void) {
    gFramebufferSwapDelay.value = 0;
    if (D_80121B60 != 0) {
        createCallbackTaskWithUserId((CallbackTaskCallback)initFallingMenuSnowflake, 5, 0x64, D_80121B60 - 1);
    }
    updateRaceFlowFrame();
    gRaceRumbleEnabled = 0;
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0xFF) {
        gMenuFadeAlpha = 0xFF;
        gCurrentGameTask->fadeTimer -= 1;
        if (gCurrentGameTask->fadeTimer == 0) {
            stopSoundEffects();
            if ((gRaceSplitscreenMode == 2) && (((Unk80043040 *)getRelocatableHeapBlockBase(gAssetHandles[0x2B]))->unk8 != 0) && (D_80121B61 != 0) &&
                (saveRaceRecordReplayData() != 0)) {
                D_80121B61 = -1;
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
        gRacePlayers[one].replayInputSource = 0;
        gRacePlayers[one].soundDisabled = 0;
    }
    gRacePlayers[0].replayInputSource = 2;
    gMenuFlowState = 0;
    resetRaceCameras();
    resetAllViewports();
    if (gRaceCourseIndex.signedValue != 6) {
        configureViewport(0, 0xA0, 0x78, 0x100, 0xB0, 0x120, 0xD0, 1.3333334f);
    } else {
        configureRaceViewport(0, 0xA0, 0x78, 0x100, 0xB0, 0x120, 0xD0, 1.3333334f);
    }
    LOAD_ASSET(_598A70, 0x29);
    gRacePlayerHudStatuses[0].active = 1;
    gFramebufferSwapDelay.value = 0;
    resetSecondaryRng();
    initRacePlayers();
    initRaceHud();
    initRaceCourseSceneTasks();
    gMenuFadeAlpha = 0xFF;
    updateRelocatableHeap();
    requestMusicSequenceBank(0);
    gCurrentGameTask->fadeTimer = 0;
    gCurrentGameTask->unk1C = 0;
    createCallbackTask((CallbackTaskCallback)func_80057E60, 6, 0x64);
    if (D_80121B61 == -1) {
        createCallbackTask((CallbackTaskCallback)updateRaceGhostUnavailableMessage, 6, 0x64);
    }
    setCurrentGameTaskCallback(updateRaceGhostReplayFlow, 0);
}

void updateRaceGhostReplayFlow(void) {
    void *sp18;

    if (gCurrentGameTask->fadeTimer == D_800DC5C0[gCurrentGameTask->unk1C]) {
        if (loadNextRaceReplayCourseGridEntry() != 0) {
            if (gCurrentGameTask->unk1C != 0xB) {
                gCurrentGameTask->unk1C++;
                gRacePlayers[0].replayInputSource = 2;
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
    if (gCurrentGameTask->fadeTimer == 1) {
        gMenuFadeAlpha = 0;
    }
    if (gRaceUpdatePaused == 0) {
        gCurrentGameTask->fadeTimer++;
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
        gFramebufferSwapDelay.value = 0;
        stopSoundEffects();
        requestRumbleMotorInit(0);
        requestRumbleMotorInit(1);
        requestRumbleMotorInit(2);
        requestRumbleMotorInit(3);
        gMenuFlowState = 0;
        if (D_80121B57 == 2) {
            setCurrentGameTaskCallback(initRaceSceneFlow, 0);
        } else if (gPlayerCount == 1) {
            if (gRaceSplitscreenMode == 1) {
                setCurrentGameTaskCallback(openRaceTypeSelectFlow, 0);
            } else {
                setCurrentGameTaskCallback(openRaceStartTransitionFlow, 0);
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
        if (!(gRacePlayers[0].stateFlags & 0x40)) {
            return 0;
        }
    } else {
        var_v0 = 0;
        if ((s32)gPlayerCount > 0) {
            player = gRacePlayers;
loop:
            if (!(player->stateFlags & RACE_PLAYER_READY_FLAG)) {
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
    gFramebufferSwapDelay.value = 0;
    LOAD_ASSET(_593D10, 0x29);
    LOAD_ASSET(_60F1A0, 0x2A);
    LOAD_ASSET(_59DFE0, 0x26);
    LOAD_ASSET(_1F1A90, 0x28);
    switch (gRaceCourseIndex.unsignedValue) {
    case 0:
        LOAD_ASSET(LEVEL_BIG_SNOWMAN_SPRITES, 0x1D);
        break;
    case 1:
        LOAD_ASSET(LEVEL_SUNSET_ROCK_SPRITES, 0x1D);
        break;
    case 2:
        LOAD_ASSET(LEVEL_NIGHT_HIGHWAY_SPRITES, 0x1D);
        break;
    case 3:
        LOAD_ASSET(LEVEL_GRASS_VALLEY_SPRITES, 0x1D);
        break;
    case 4:
        LOAD_ASSET(LEVEL_DIZZY_LAND_SPRITES, 0x1D);
        break;
    case 5:
        LOAD_ASSET(LEVEL_QUICKSAND_VALLEY_SPRITES, 0x1D);
        break;
    case 6:
        LOAD_ASSET(LEVEL_SILVER_MOUNTAIN_SPRITES, 0x1D);
        break;
    case 8:
        LOAD_ASSET(LEVEL_NINJA_LAND_SPRITES, 0x1D);
        break;
    case 9:
        LOAD_ASSET(LEVEL_ROOKIE_MOUNTAIN_SPRITES, 0x1D);
        break;
    }
    LOAD_ASSET(_1E74E0, 0x1C);
    initCallbackTaskScheduler(0);
    gMenuFlowState = 0;
    createCallbackTaskWithUserId((CallbackTaskCallback)initRaceRecordSettingsPanel, 0, 0x64, 0);
    createCallbackTask((CallbackTaskCallback)initMenuIconTilemapSpriteActor, 0, 0x5E);
    setCurrentGameTaskCallback(updateRaceRecordSettingsFlow, 0);
    requestMusicSequenceBank(7);
}

void updateRaceRecordSettingsFlow(void) {
    s32 temp_v0;

    gMenuFadeAlpha -= 0x10;
    if (gMenuFadeAlpha < 0) {
        gMenuFadeAlpha = 0;
        temp_v0 = gPlayerInputPressed[0];
        if ((temp_v0 & (STICK_DOWN | D_JPAD)) && (gRaceLapCount != 9)) {
            enqueueSoundEffect(0x19, 0x32);
            gRaceLapCount += 1;
            temp_v0 = gPlayerInputPressed[0];
        }
        if ((temp_v0 & (STICK_UP | U_JPAD)) && (gRaceLapCount != 1)) {
            enqueueSoundEffect(0x19, 0x32);
            gRaceLapCount -= 1;
            temp_v0 = gPlayerInputPressed[0];
        }
        if (temp_v0 & (A_BUTTON | START_BUTTON)) {
            gMenuFlowState = 1;
            enqueueSoundEffect(0x18, 0x32);
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
        gFramebufferSwapDelay.value = 0;
        setCurrentGameTaskCallback(&initRaceSceneFlow, 0);
    }
}
