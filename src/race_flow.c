#include "common.h"
#include "sound_manager.h"
#include "callback_task_scheduler.h"
#include "relocatable_heap.h"
#include "asset_manager.h"
#include "character_select_course_menu.h"
#include "character_select_menu.h"
#include "multiplayer_course_select_menu.h"
#include "controller_pak_race_record_save_flow.h"
#include "controller_pak_replay_save_message_flow.h"
#include "controller_pak_file_delete_flow.h"
#include "course_select_menu.h"
#include "race_setup_menu.h"
#include "game_task_scheduler.h"
#include "controller_main_menu_flow.h"
#include "menu_screen_effects.h"
#include "main_menu_panel_ui.h"
#include "race_type_select_menu.h"
#include "race_splitscreen_select_menu.h"
#include "race_camera.h"
#include "race_course_effects.h"
#include "race_flow.h"
#include "race_player_state.h"
#include "race_scene_loader.h"
#include "race_start_transition.h"
#include "race_timer_ui.h"
#include "ending_credits_flow.h"
#include "race_ui_effects.h"
#include "shop_menu_flow.h"
#include "title_menu.h"
#include "viewport_manager.h"

#define COURSE_GRID_ENTRY_FREE -1
#define COURSE_GRID_ENTRY_END -2
#define COURSE_TIME_MINUTE 0x177000
#define COURSE_TIME_SECOND 0x6400
#define RACE_PLAYER_STATE_SIZE 0x60C
#define RACE_PLAYER_READY_FLAG 0x40
#ifdef NON_MATCHING
#define RACE_PLAYER_RESULT_VALUE(index) (*(s8 *)((u8 *)D_80121D80 + ((((((((index) << 2) - (index)) << 5) + (index)) << 2) - (index)) << 2) + 0x509))
#endif

typedef struct {
    /* 0x00 */ s16 status;
    /* 0x02 */ s16 courseId;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
} CourseGridEntry;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ u8 pad2[0x48 - 0x2];
} CourseSelectTableEntry;

typedef struct {
    /* 0x00 */ s8 minutes;
    /* 0x01 */ s8 seconds;
    /* 0x02 */ s16 fraction;
} RaceTime;

typedef union {
    s16 s;
    u16 u;
} SignedUnsignedShort;

typedef struct {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 unk2;
} RaceFlowInitEntry;

typedef struct {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 pad2;
} RaceFlowResultEntry;

typedef struct {
    /* 0x0000 */ u8 pad0[0x4];
    /* 0x0004 */ s32 unk4[12];
    /* 0x0034 */ u8 pad34[0x4E - 0x34];
    /* 0x004E */ RaceFlowInitEntry unk4E[11][5];
    /* 0x012A */ RaceFlowInitEntry unk12A[11];
    /* 0x0156 */ RaceFlowInitEntry unk156[11][5];
    /* 0x0232 */ RaceFlowResultEntry unk232[9];
    /* 0x0256 */ u8 pad256[0x7756 - 0x256];
    /* 0x7756 */ s16 unk7756[11][5];
    /* 0x77C4 */ u8 unk77C4[11][5];
    /* 0x77FB */ u8 unk77FB[11][5];
    /* 0x7832 */ u8 unk7832[11][5];
    /* 0x7869 */ u8 unk7869[11][5];
    /* 0x78A0 */ u8 unk78A0[11][5];
    /* 0x78D7 */ u8 unk78D7;
} RaceFlowInitScratch;

typedef struct {
    /* 0x0000 */ u8 pad0[0x4E];
    /* 0x004E */ RaceTime timeTrial[5];
    /* 0x0062 */ u8 pad62[0x156 - 0x62];
    /* 0x0156 */ RaceTime raceTimes[5];
} TimeCourseView;

typedef struct {
    /* 0x0000 */ u8 pad0[0x7756];
    /* 0x7756 */ u16 values[5];
} TrickCourseView;

typedef struct {
    /* 0x0000 */ u8 pad0[0x7832];
    /* 0x7832 */ u8 values[5];
} ScoreCourseView;

typedef struct {
    /* 0x000 */ u8 pad0[0x04];
    /* 0x004 */ u8 unk4;
    /* 0x005 */ u8 unk5;
    /* 0x006 */ u8 unk6;
    /* 0x007 */ u8 unk7;
    /* 0x008 */ u8 pad8[0xC - 0x8];
    /* 0x00C */ s32 unkC;
    /* 0x010 */ u8 characterId;
    /* 0x011 */ u8 characterVariant;
    /* 0x012 */ u8 pad12[0x13 - 0x12];
    /* 0x013 */ u8 unk13;
    /* 0x014 */ s8 unk14;
    /* 0x015 */ u8 unk15;
    /* 0x016 */ u8 unk16;
    /* 0x017 */ u8 pad17[0x18 - 0x17];
    /* 0x018 */ s16 unk18;
    /* 0x01A */ u8 pad1A[0x2C0 - 0x1A];
    /* 0x2C0 */ s16 unk2C0;
    /* 0x2C2 */ u8 pad2C2[0x2FC - 0x2C2];
    /* 0x2FC */ s32 flags;
    /* 0x300 */ u8 pad300[0x502 - 0x300];
    /* 0x502 */ s16 courseId;
    /* 0x504 */ u8 pad504[0x509 - 0x504];
    /* 0x509 */ s8 result;
    /* 0x50A */ u8 pad50A[0x56C - 0x50A];
    /* 0x56C */ s32 unk56C;
    /* 0x570 */ u8 pad570[0x574 - 0x570];
    /* 0x574 */ s16 unk574;
    /* 0x576 */ u8 pad576[RACE_PLAYER_STATE_SIZE - 0x576];
} RacePlayerState;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 fadeTimer;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 countdown;
} RaceFlowState;

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

extern CourseGridEntry *D_800DC490[];
extern u16 D_800DC5C0[];
extern CourseSelectTableEntry D_800B9542[];
extern RaceFlowState *gCurrentGameTask;
extern RacePlayerState D_80121D80[];
extern s32 gRaceCameraReplayStartX;
extern s32 gRaceCameraReplayStartY;
extern s32 gRaceCameraReplayStartZ;
extern s32 gMenuFlowState;
extern s32 gPlayerInputHeld;
extern s32 D_8012207C;
extern s32 gPlayerInputPressed[];
extern void *gMenuRenderCallbackList;
extern s16 gAssetHandles[];
extern s16 gRaceReplayInputBufferHandle;
extern s16 gRaceReplayPlayerStateBufferHandle;
extern s16 gRaceUiSpriteAssetHandle;
extern s16 D_801124B8;
extern SignedUnsignedShort gRaceCourseIndex;
extern s16 gRaceLapCount;
extern s32 D_80121B7C;
extern s16 gMenuFadeAlpha;
extern s8 gMenuFadeOverlayActive;
extern u8 gRaceRumbleEnabled;
extern RaceFlowInitScratch gGameSaveDataBuffer;
extern u8 gRaceTypeSelection;
#ifdef NON_MATCHING
extern u8 D_8011228C;
extern u8 D_8011233C;
extern f32 D_800E16CC;
extern f32 D_800E16D0;
extern u8 D_59AAA0[];
extern s16 D_80121B5C;
extern u8 D_80121B5F;
extern u8 D_80121D94;
extern u8 D_80121D95;
extern u8 D_80121D96;
extern u8 D_801223A0;
extern u8 D_801223A1;
extern u8 D_801223A2;
extern u8 D_801229AC;
extern u8 D_801229AD;
extern u8 D_801229AE;
extern u8 D_80122FB8;
extern u8 D_80122FB9;
extern u8 D_80122FBA;
#endif
extern u8 D_2427D0[];
extern u8 D_243270[];
extern u8 gPlayerCount;
extern u8 D_80121B57;
extern u8 gRaceCameraModeChangeDisabled;
extern u8 gRaceTypeSelection;
extern s8 D_80121B5F;
extern u8 D_80121B60;
extern s8 D_80121B61;
extern RaceTime D_80121B74;
extern u8 D_80121B81;
extern u8 gRaceRecordSettingsEnabled;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern s8 gFramebufferSwapDelay;
extern u8 gRaceSplitscreenMode;
extern u8 gRumblePakConnectedMask;
extern u8 D_800DC4C0;
extern u8 gRaceUpdatePaused;
extern ViewportSlot D_801121E0[];
extern u8 D_1E74E0[];
extern u8 D_1EC0F0[];
extern u8 D_1EC4A0[];
extern u8 D_1EC810[];
extern u8 D_1ECD00[];
extern u8 D_1ED910[];
extern u8 D_1EDC80[];
extern u8 D_1EE060[];
extern u8 D_1EEEA0[];
extern u8 D_1EF1D0[];
extern u8 D_1EF530[];
extern u8 D_1F1A90[];
extern u8 D_1F2220[];
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_59DFE0[];
extern u8 D_59E7F0[];
extern u8 D_60F1A0[];
extern u8 D_60F990[];
extern s16 D_80122040;
extern s16 D_801222F4;

extern s32 saveRaceRecordReplayData(void);
extern void releaseMenuAssetHandles(void);
extern void func_80045A78(s32, s32, s32, s32);
extern void drawAssetTableSpriteWithExplicitPalette(s32, s32, s32, s32, s32);
extern void addRenderCallback(void *, void (*)(s32), s32);
extern void enqueueSoundEffect(s32, s32);

void initStartupControllerPakFlow(void) {
    loadCompressedRomAsset(D_2427D0, D_243270, 6);
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
    if (gPlayerInputHeld & 0x1000) {
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
    CourseGridEntry *entry = D_800DC490[gRaceCourseIndex.s];
    s32 count = 0;
loop:
    if (entry->status != COURSE_GRID_ENTRY_END) {
        entry->status = COURSE_GRID_ENTRY_FREE;
        entry++;
        count += 1;
        goto loop;
    }
    if (count != 0) {
        gRaceReplayPlayerStateBufferHandle = allocRelocatableHeapBlock(count * sizeof(RacePlayerState));
    }
}

void saveRaceReplayCourseGridEntry(void) {
    CourseGridEntry *entry = D_800DC490[gRaceCourseIndex.s];
    s32 i = 0;

loop:
    if (entry->status != COURSE_GRID_ENTRY_END) {
        if ((entry->status == COURSE_GRID_ENTRY_FREE) && (entry->courseId == D_80121D80[0].courseId)) {
            s32 *status = (s32 *)getRelocatableHeapBlockBase(gAssetHandles[0x2B]);
            if (*status < 0x1194) {
                entry->status = *status;
                ((RacePlayerState *) getRelocatableHeapBlockBase(gAssetHandles[0x2C]))[i] = D_80121D80[0];
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

    entry = D_800DC490[gRaceCourseIndex.s];
    count = 0;
loop:
    if (entry->status != COURSE_GRID_ENTRY_END) {
        if (entry->status != COURSE_GRID_ENTRY_FREE) {
            D_80121D80[0] = ((RacePlayerState *)getRelocatableHeapBlockBase(gRaceReplayPlayerStateBufferHandle))[count];
            *(s32 *)getRelocatableHeapBlockBase(gRaceReplayInputBufferHandle) = entry->status;
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
    CourseGridEntry *entry = D_800DC490[gRaceCourseIndex.s];

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
            gGameSaveDataBuffer.unk4E[course][player].unk0 = 3;
            gGameSaveDataBuffer.unk4E[course][player].unk1 = 0x3B;
            gGameSaveDataBuffer.unk4E[course][player].unk2 = 0;
            gGameSaveDataBuffer.unk156[course][player].unk0 = 3;
            gGameSaveDataBuffer.unk156[course][player].unk1 = 0;
            gGameSaveDataBuffer.unk156[course][player].unk2 = 0;
            gGameSaveDataBuffer.unk77FB[course][player] = player;
            gGameSaveDataBuffer.unk7832[course][player] = 0;
            gGameSaveDataBuffer.unk7869[course][player] = player;
            gGameSaveDataBuffer.unk78A0[course][player] = player;
            gGameSaveDataBuffer.unk7756[course][player] = 0;
            gGameSaveDataBuffer.unk77C4[course][player] = player;
        }
        gGameSaveDataBuffer.unk12A[course].unk0 = one;
        gGameSaveDataBuffer.unk12A[course].unk1 = 0x18;
        gGameSaveDataBuffer.unk12A[course].unk2 = 0;
        gGameSaveDataBuffer.unk4[course + 1] = 0;
    }

    gGameSaveDataBuffer.unk232[0].unk0 = 0;
    gGameSaveDataBuffer.unk232[1].unk0 = 0;
    gGameSaveDataBuffer.unk232[2].unk0 = 0;
    gGameSaveDataBuffer.unk232[3].unk0 = 0;
    gGameSaveDataBuffer.unk232[4].unk0 = 0;
    gGameSaveDataBuffer.unk232[5].unk0 = (u8)0;
    gGameSaveDataBuffer.unk232[6].unk0 = 0;
    gGameSaveDataBuffer.unk232[7].unk0 = 0;
    gGameSaveDataBuffer.unk232[8].unk0 = 0;
    gPlayerCount = 1;
    gRaceTypeSelection = 0;
    D_80121D80[0].unk5 = 0;
    D_80121D80[1].unk5 = 0;
    D_80121D80[2].unk5 = 0;
    D_80121D80[3].unk5 = 0;
    D_80121D80[0].unk6 = 0;
    D_80121D80[1].unk6 = 0;
    D_80121D80[2].unk6 = 0;
    D_80121D80[3].unk6 = 0;
    D_80121D80[0].unk7 = 0;
    D_80121D80[1].unk7 = 0;
    D_80121D80[2].unk7 = 0;
    D_80121D80[3].unk7 = 0;
    D_80121D80[0].unkC = 0;
    D_80121D80[1].unkC = 0;
    D_80121D80[2].unkC = 0;
    D_80121D80[3].unkC = 0;
    D_80121D80[0].unk18 = 0;
    D_80121D80[1].unk18 = 0;
    D_80121D80[2].unk18 = 0;
    D_80121D80[3].unk18 = 0;
    gRaceCourseIndex.s = 0;
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
    if (D_800DC4C0 != 0) {
        createGameTask(4, &initEndingCreditsFlow, 0x64);
        suspendGameTask(2);
    }
}

void openRaceStartTransitionFlow(void) {
    setCurrentGameTaskCallback(&openEndingCreditsIfUnlockedFlow, 0);
    createGameTask(4, &initRaceStartTransition, 0x64);
    suspendGameTask(2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/initRaceSceneFlow.s")

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
    D_801124B8 = 0xFF;
}

void fadeInRaceGameplayViewports(void) {
    s32 temp_a0;
    RaceFlowState *state;

    state = gCurrentGameTask;
    temp_a0 = state->unk1C;
    if (temp_a0 >= 0x32) {
        gMenuFadeOverlayActive = 1;
        gMenuFadeAlpha += 0x16;
        if (gMenuFadeAlpha >= 0x100) {
            setRaceCameraMode(0, 1);
            gMenuFadeAlpha = 0xFF;
            createCallbackTask(initRaceCountdownPrompt, 6, 0x64);
            createCallbackTask((void (*)(CallbackTask *))func_80065E90, 6, 0x64);
            switch (gPlayerCount & 0xFFFFFFFF) {
            case 1:
                if (gRaceCourseIndex.s != 6) {
                    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
                } else {
                    func_80070A70(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
                }
                D_801121E0[0].active = 1;
                gFramebufferSwapDelay = 0;
                break;
            case 2:
                if (gRaceCourseIndex.s != 6) {
                    configureViewport(0, 0xA0, 0x43, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
                    configureViewport(1, 0xA0, 0xAD, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
                } else {
                    func_80070A70(0, 0xA0, 0x43, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
                    func_80070A70(1, 0xA0, 0xAD, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
                }
                D_801121E0[0].active = 1;
                D_801121E0[1].active = 1;
                gFramebufferSwapDelay = 1;
                break;
            case 3:
                if (gRaceCourseIndex.s != 6) {
                    configureViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    configureViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    configureViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                } else {
                    func_80070A70(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    func_80070A70(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    func_80070A70(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                }
                D_801121E0[0].active = 1;
                D_801121E0[1].active = 1;
                D_801121E0[2].active = 1;
                gFramebufferSwapDelay = 1;
                break;
            case 4:
                if (gRaceCourseIndex.s != 6) {
                    configureViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    configureViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    configureViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    configureViewport(3, 0xE9, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                } else {
                    func_80070A70(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    func_80070A70(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    func_80070A70(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    func_80070A70(3, 0xE9, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                }
                D_801121E0[0].active = 1;
                D_801121E0[1].active = 1;
                D_801121E0[2].active = 1;
                D_801121E0[3].active = 1;
                gFramebufferSwapDelay = 1;
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

void drawRacePauseMenu(s32 arg0) {
    s32 color;

    func_80045A78(-0x14, -0x10, getRelocatableHeapBlockBase(gRaceUiSpriteAssetHandle), 0x57);
    color = 0x1A;
    if (D_80121B57 == 0) {
        color = 0x1B;
    }
    drawAssetTableSpriteWithExplicitPalette(-0x1C, 0, getRelocatableHeapBlockBase(gRaceUiSpriteAssetHandle), 0x58, color);
    color = 0x1A;
    if (D_80121B57 == 1) {
        color = 0x1B;
    }
    drawAssetTableSpriteWithExplicitPalette(-0x1C, 0xA, getRelocatableHeapBlockBase(gRaceUiSpriteAssetHandle), 0x59, color);
    color = 0x1A;
    if (D_80121B57 == 2) {
        color = 0x1B;
    }
    drawAssetTableSpriteWithExplicitPalette(-0x1C, 0x14, getRelocatableHeapBlockBase(gRaceUiSpriteAssetHandle), 0x5A, color);
}

void updateRaceGameplayFlow(void) {
    RacePlayerState *player;
    s32 opened;
    s32 i;
    s32 input;
    s32 selection;
    s32 valueTwo;

    opened = 0; i = 0; if (gPlayerCount > 0) { player = D_80121D80; do { if (((player->unk14 == 0) && (gRaceUpdatePaused == 0)) && (gPlayerInputPressed[i] & 0x1000)) { D_80121B57 = 0; gRaceUpdatePaused = 1; opened = 1; enqueueSoundEffect(1, 0x32); requestRumbleMotorInit(0); requestRumbleMotorInit(1); requestRumbleMotorInit(2); requestRumbleMotorInit(3); } i++; player++; } while (i < gPlayerCount); } valueTwo = 2; if ((gRaceUpdatePaused != 0) && (opened == 0)) { i = 0; if (gPlayerCount > 0) { player = D_80121D80; do { if (player->unk14 == 0) { input = gPlayerInputPressed[i]; if (input & 0x10800) { selection = D_80121B57; if (selection != 0) { D_80121B57 = selection - 1; enqueueSoundEffect(1, 0x32); input = gPlayerInputPressed[i]; }
                    }
                    if (input & 0x20400) {
                        selection = D_80121B57;
                        if (valueTwo != selection) {
                            D_80121B57 = selection + 1;
                            enqueueSoundEffect(1, 0x32);
                            input = gPlayerInputPressed[i];
                        }
                    }
                    if (input & 0x1000) {
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

// waitRaceFinishResultsFlow best match: 88.931% (nonmatchings/waitRaceFinishResultsFlow-3236181511606361864/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/waitRaceFinishResultsFlow.s")

#ifdef NON_MATCHING
void waitRaceFinishResultsFlow(void) {
    s32 bestPlayer;
    s8 value;

    D_80121B57 = 0;
    updateRaceFlowFrameWithCourseEffects();
    gCurrentGameTask->fadeTimer--;
    if (gCurrentGameTask->fadeTimer == 0) {
        gMenuFlowState |= 8;
        switch (gPlayerCount) {
        case 3:
            gCurrentGameTask->unk1C = 0;
            bestPlayer = gCurrentGameTask->unk1C;
            value = RACE_PLAYER_RESULT_VALUE(bestPlayer);
            if (RACE_PLAYER_RESULT_VALUE(1) < value) {
                gCurrentGameTask->unk1C = 1;
                bestPlayer = gCurrentGameTask->unk1C;
                value = RACE_PLAYER_RESULT_VALUE(bestPlayer);
            }
            if (RACE_PLAYER_RESULT_VALUE(2) < value) {
                gCurrentGameTask->unk1C = 2;
                bestPlayer = gCurrentGameTask->unk1C;
            }
            gCurrentGameTask->unk1C = bestPlayer + 2;
            setCurrentGameTaskCallback(zoomRaceWinnerViewport, 0);
            break;
        case 4:
            gCurrentGameTask->unk1C = 0;
            bestPlayer = gCurrentGameTask->unk1C;
            value = RACE_PLAYER_RESULT_VALUE(bestPlayer);
            if (RACE_PLAYER_RESULT_VALUE(1) < value) {
                gCurrentGameTask->unk1C = 1;
                bestPlayer = gCurrentGameTask->unk1C;
                value = RACE_PLAYER_RESULT_VALUE(bestPlayer);
            }
            if (RACE_PLAYER_RESULT_VALUE(2) < value) {
                gCurrentGameTask->unk1C = 2;
                bestPlayer = gCurrentGameTask->unk1C;
                value = RACE_PLAYER_RESULT_VALUE(bestPlayer);
            }
            if (RACE_PLAYER_RESULT_VALUE(3) < value) {
                gCurrentGameTask->unk1C = 3;
                bestPlayer = gCurrentGameTask->unk1C;
            }
            gCurrentGameTask->unk1C = bestPlayer + 5;
            setCurrentGameTaskCallback(zoomRaceWinnerViewport, 0);
            break;
        case 2:
            if (RACE_PLAYER_RESULT_VALUE(0) < RACE_PLAYER_RESULT_VALUE(1)) {
                gCurrentGameTask->unk1C = 0;
                D_8011233C = 0;
            } else {
                gCurrentGameTask->unk1C = 1;
                D_8011228C = 0;
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
#endif

// interpolateRaceViewport best match: 60.289%
#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/interpolateRaceViewport.s")

#ifdef NON_MATCHING

void interpolateRaceViewport(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6,
                  f32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11, s32 arg12, s32 arg13,
                  f32 arg14) {
    volatile s32 sp2C;
    volatile s32 sp28;
    volatile f32 temp_fv0;
    volatile s32 temp_t0;
    volatile s32 temp_t1;
    volatile s32 temp_t2;
    volatile s32 temp_t3;
    s32 temp_v0;

    temp_v0 = gCurrentGameTask->fadeTimer;
    sp2C = (((arg8 - arg1) * temp_v0) / 15) + arg1;
    sp28 = (((arg9 - arg2) * temp_v0) / 15) + arg2;
    temp_t0 = (((arg10 - arg3) * temp_v0) / 15) + arg3;
    temp_t1 = (((arg11 - arg4) * temp_v0) / 15) + arg4;
    temp_t2 = (((arg12 - arg5) * temp_v0) / 15) + arg5;
    temp_fv0 = (((arg14 - arg7) * (f32) temp_v0) / 15.0f) + arg7;
    temp_t3 = (((arg13 - arg6) * temp_v0) / 15) + arg6;
    if (gRaceCourseIndex.s != 6) {
        configureViewport(arg7, sp2C, sp28, temp_t0 & 0xFFFF, temp_t1, temp_t2, temp_t3, temp_fv0);
    } else {
        func_80070A70(arg7, sp2C, sp28, temp_t0 & 0xFFFF, temp_t1, temp_t2, temp_t3, temp_fv0);
    }
}
#endif

void zoomRaceWinnerViewport(void) {
    gCurrentGameTask->fadeTimer += 1;
    switch (gCurrentGameTask->unk1C) {
    case 0:
        interpolateRaceViewport(0, 0xA0, 0x43, 0x120, 0x68, 0x140, 0x78, 2.6666667f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        interpolateRaceViewport(1, 0xA0, 0xAD, 0x120, 0x68, 0x140, 0x78, 2.6666667f, 0xA0, 0x115, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
        if (gCurrentGameTask->fadeTimer == 0xF) {
            func_80070614(1);
            D_801121E0[1].active = 0;
        }
        break;
    case 1:
        interpolateRaceViewport(1, 0xA0, 0xAD, 0x120, 0x68, 0x140, 0x78, 2.6666667f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        interpolateRaceViewport(0, 0xA0, 0x43, 0x120, 0x68, 0x140, 0x78, 2.6666667f, 0xA0, -0x25, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
        if (gCurrentGameTask->fadeTimer == 0xF) {
            func_80070614(0);
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
            func_80070614(1);
            func_80070614(2);
        }
        break;
    case 3:
        interpolateRaceViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        interpolateRaceViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x57, -0x25, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        interpolateRaceViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x179, -0x25, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        if (gCurrentGameTask->fadeTimer == 0xF) {
            D_801121E0[0].active = 0;
            D_801121E0[2].active = 0;
            func_80070614(0);
            func_80070614(2);
        }
        break;
    case 4:
        interpolateRaceViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        interpolateRaceViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, -0x39, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        interpolateRaceViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, -0x39, 0x115, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        if (gCurrentGameTask->fadeTimer == 0xF) {
            D_801121E0[0].active = 0;
            D_801121E0[1].active = 0;
            func_80070614(0);
            func_80070614(1);
        }
        break;
    case 5:
        interpolateRaceViewport(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        interpolateRaceViewport(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x57, 0x115, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        interpolateRaceViewport(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x179, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        interpolateRaceViewport(3, 0xE9, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x179, 0x115, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        if (gCurrentGameTask->fadeTimer == 0xF) {
            func_80070614(1);
            func_80070614(2);
            func_80070614(3);
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
            func_80070614(0);
            func_80070614(2);
            func_80070614(3);
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
            func_80070614(0);
            func_80070614(1);
            func_80070614(3);
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
            func_80070614(0);
            func_80070614(1);
            func_80070614(2);
        }
        break;
    }
    updateRaceFlowFrame();
    if (gCurrentGameTask->fadeTimer == 0xF) {
        setCurrentGameTaskCallback(prepareRaceResultsFlow, 0);
    }
}

// prepareRaceResultsFlow best match: 81.749% (nonmatchings/prepareRaceResultsFlow-7273315160691878794/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/prepareRaceResultsFlow.s")

#ifdef NON_MATCHING
void prepareRaceResultsFlow(void) {
    RacePlayerState *player;
    TimeCourseView *timeCourse;
    TrickCourseView *trickCourse;
    ScoreCourseView *scoreCourse;
    RaceTime *time;
    u16 *trickScore;
    u8 *scoreValue;
    s32 i;
    s32 index;
    s32 currentTime;
    s32 recordTime;
    s32 courseOffset;

    D_800DC4C0 = 0;
    gFramebufferSwapDelay = 0;
    D_80121B60 = 0;
    D_80121B61 = 0;

    switch (gRaceSplitscreenMode) {
    case 0:
        D_80121B5F = 1;
        i = 0;
        if (gPlayerCount > 0) {
            player = D_80121D80;
            do {
                if (player->result == 0) {
                    D_80121B60 = i + 1;
                }
                i++;
                player++;
            } while (i < gPlayerCount);
        }
        break;

    case 2:
        currentTime = (D_80121B74.seconds * COURSE_TIME_SECOND) + D_80121B74.fraction + (D_80121B74.minutes * COURSE_TIME_MINUTE);
        courseOffset = (gRaceCourseIndex.s << 2) + gRaceCourseIndex.s;
        courseOffset <<= 2;
        timeCourse = (TimeCourseView *)((u8 *)&gGameSaveDataBuffer + courseOffset);
        index = 0;
        time = timeCourse->timeTrial;
        do {
            recordTime = (time->minutes * COURSE_TIME_MINUTE) + time->fraction + (time->seconds * COURSE_TIME_SECOND);
            if (currentTime < recordTime) {
                break;
            }
            index += 4;
            time++;
        } while (index != 0x14);
        if (index < 0x14) {
            D_80121B5F = 1;
            D_80121B60 = 1;
            if (index == 0) {
                D_80121B61 = 1;
            }
        } else {
            D_80121B5F = 2;
        }
        break;

    case 1:
        if (gRaceTypeSelection != 0) {
            i = 0;
            if (gRaceTypeSelection != 1) {
                if (gRaceTypeSelection == 2) {
                    courseOffset = (gRaceCourseIndex.s << 2) + gRaceCourseIndex.s;
                    courseOffset <<= 1;
                    trickCourse = (TrickCourseView *)((u8 *)&gGameSaveDataBuffer + courseOffset);
                    trickScore = trickCourse->values;
                    do {
                        if (*trickScore < D_80122040) {
                            break;
                        }
                        i++;
                        trickScore++;
                    } while (i < 5);
                    if (D_80121B81 != 0) {
                        i = 5;
                    }
                    if (i < 5) {
                        D_80121B5F = 1;
                        D_80121B60 = 1;
                    } else {
                        D_80121B5F = 2;
                    }
                }
            } else {
                courseOffset = (gRaceCourseIndex.s << 2) + gRaceCourseIndex.s;
                scoreCourse = (ScoreCourseView *)((u8 *)&gGameSaveDataBuffer + courseOffset);
                scoreValue = scoreCourse->values;
                do {
                    if (*scoreValue < D_801222F4) {
                        break;
                    }
                    i++;
                    scoreValue++;
                } while (i < 5);
                if (D_80121B81 != 0) {
                    i = 5;
                }
                if (i < 5) {
                    D_80121B5F = 1;
                    D_80121B60 = 1;
                } else {
                    D_80121B5F = 2;
                }
            }
        } else {
            currentTime = (D_80121B74.seconds * COURSE_TIME_SECOND) + D_80121B74.fraction + (D_80121B74.minutes * COURSE_TIME_MINUTE);
            courseOffset = (gRaceCourseIndex.s << 2) + gRaceCourseIndex.s;
            courseOffset <<= 2;
            timeCourse = (TimeCourseView *)((u8 *)&gGameSaveDataBuffer + courseOffset);
            i = 0;
            time = timeCourse->raceTimes;
            do {
                recordTime = (time->minutes * COURSE_TIME_MINUTE) + time->fraction + (time->seconds * COURSE_TIME_SECOND);
                if (currentTime < recordTime) {
                    break;
                }
                i++;
                time++;
            } while (i < 5);
            if (D_80121B81 != 0) {
                i = 5;
            }
            if (i < 5) {
                D_80121B5F = 1;
                D_80121B60 = 1;
            } else {
                D_80121B5F = 2;
            }
        }
        break;
    }

    gCurrentGameTask->unk1C = 0x3C;
    setCurrentGameTaskCallback(updateRaceResultsFlow, 0);
    if (D_80121B60 != 0) {
        requestMusicSequenceBank(6);
        if ((gPlayerCount == 1) && (gRaceSplitscreenMode == 0)) {
            createCallbackTaskWithUserId((void (*)(CallbackTask *))func_8005E68C, 6, 0x64, 0xA9);
        }
        createCallbackTaskWithUserId(initFallingMenuSnowflake, 5, 0x64, D_80121B60 - 1);
    }
    updateRaceFlowFrame();
}
#endif

// updateRaceResultsFlow best match: 93.039% at nonmatchings/updateRaceResultsFlow-731940616440357983/base_1.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/updateRaceResultsFlow.s")

#ifdef NON_MATCHING
void updateRaceResultsFlow(void) {
    volatile u8 padding[8];
    s32 allDone;
    s32 i;
    s32 j;
    s32 currentTime;
    s32 recordTime;
    RacePlayerState *player;
    EffectTask *task;

    D_800DC4C0 = 0;
    D_800DEED4 = 0;
    allDone = 1;

    if (D_80121B55 > 0) {
        player = D_80121D80;
        do {
            if (!(player->flags & 0x08000000)) {
                allDone = 0;
            }
            player++;
        } while ((u32)player < (u32)&D_80121D80[D_80121B55]);
    }

    if (D_801235B8->unk1C != 0) {
        D_801235B8->unk1C -= 1;
    } else {
        allDone = 1;
    }

    if ((D_80121B60 != 0) && (func_80072938() != 0)) {
        allDone = 0;
    }

    if (allDone != 0) {
        if (D_80121B60 == 0) {
            func_80072114(4);
        }

        D_801235B4 |= 0x20;

        switch (gRaceSplitscreenMode) {
        case 0:
            if (D_80121B55 == 1) {
                task = func_80071408((void (*)(EffectTask *))func_80058BAC, 6, 0x64);
                if (task != NULL) {
                    if (D_80121D80[0].result != 3) {
                        i = D_80121D80[0].result + 1;
                        if (D_800EC9F0.pad34[gRaceCourseIndex.s] == 0) {
                            D_800EC9F0.pad34[gRaceCourseIndex.s] = i;
                        } else if (i < D_800EC9F0.pad34[gRaceCourseIndex.s]) {
                            D_800EC9F0.pad34[gRaceCourseIndex.s] = i;
                        }
                        if (D_800EC9F0.unk4[gRaceCourseIndex.s + 1] < D_80121D80[0].unk56C) {
                            D_800EC9F0.unk4[gRaceCourseIndex.s + 1] = D_80121D80[0].unk56C;
                        }
                    }
                    if (D_80121D80[0].result == 0) {
                        if (gRaceCourseIndex.s == 8) {
                            D_800DC4C0 = 1;
                            D_800EC9F0.unk78D7 |= 1;
                            D_800EC9F0.pad34[0x17] |= 1;
                        }
                    } else {
                        D_801235B4 |= 0x20;
                    }
                    D_801235B8->fadeTimer = 0xA;
                    func_8009956C(updateRaceResultsMusicFlow, 0);
                }
            } else {
                task = func_80071408((void (*)(EffectTask *))func_800599DC, 6, 0x64);
                if (task != NULL) {
                    player = D_80121D80;
                    do {
                        if (player->result == 0) {
                            player->unk18++;
                            if (player->unk18 >= 0x64) {
                                player->unk18 = 0x63;
                            }
                        }
                        player++;
                    } while ((u32)player < (u32)&D_80121D80[4]);

                    j = 0;
                    if (D_80121B55 > 0) {
                        player = D_80121D80;
                        do {
                            if (player->result == 0) {
                                j = 1;
                            }
                            player++;
                        } while ((u32)player < (u32)&D_80121D80[D_80121B55]);
                    }
                    if (j == 0) {
                        D_801235B4 |= 0x20;
                    }
                    D_801235B8->fadeTimer = 0xA;
                    func_8009956C(updateRaceResultsMusicFlow, 0);
                }
            }
            break;

        case 2:
            task = func_80071408((void (*)(EffectTask *))func_8005A2F0, 6, 0x64);
            if (task != NULL) {
                currentTime = D_80121B74.fraction + (D_80121B74.seconds * COURSE_TIME_SECOND) +
                              (D_80121B74.minutes * COURSE_TIME_MINUTE);
                i = 0;
                j = 0;
                do {
                    recordTime = (D_800EC9F0.unk4E[gRaceCourseIndex.s][i].unk0 * COURSE_TIME_MINUTE) +
                                 (D_800EC9F0.unk4E[gRaceCourseIndex.s][i].unk1 * COURSE_TIME_SECOND) +
                                 D_800EC9F0.unk4E[gRaceCourseIndex.s][i].unk2;
                    if (currentTime < recordTime) {
                        break;
                    }
                    i++;
                    j += 4;
                } while (i < 5);
                task->unk10 = i;
                if (i < 5) {
                    for (j = 3; j >= i; j--) {
                        D_800EC9F0.unk4E[gRaceCourseIndex.s][j + 1] = D_800EC9F0.unk4E[gRaceCourseIndex.s][j];
                        D_800EC9F0.unk77FB[gRaceCourseIndex.s][j + 1] = D_800EC9F0.unk77FB[gRaceCourseIndex.s][j];
                    }
                    D_800EC9F0.unk4E[gRaceCourseIndex.s][i] = *(RaceFlowInitEntry *)&D_80121B74;
                    D_800EC9F0.unk77FB[gRaceCourseIndex.s][i] =
                        (D_80121D80[0].characterVariant * 8) + D_80121D80[0].characterId;
                    if (i == 0) {
                        D_800EC9F0.unk12A[gRaceCourseIndex.s] = *(RaceFlowInitEntry *)&D_80121B7C;
                    }
                }
                if (i < 5) {
                    D_80121B5F = 1;
                } else {
                    D_80121B5F = 2;
                    D_801235B4 |= 0x20;
                }
                D_801235B8->fadeTimer = 0xA;
                func_8009956C(updateRaceResultsMusicFlow, 0);
            }
            break;

        case 1:
            switch (D_80121B5E) {
            case 2:
                task = func_80071408((void (*)(EffectTask *))func_8005E534, 6, 0x64);
                if (task != NULL) {
                    i = 0;
                    do {
                        if (D_800EC9F0.unk7756[gRaceCourseIndex.s][i] < D_80122040) {
                            break;
                        }
                        i++;
                    } while (i < 5);
                    if (D_80121B81 != 0) {
                        i = 5;
                    }
                    task->unk10 = i;
                    if (i < 5) {
                        for (j = 3; j >= i; j--) {
                            D_800EC9F0.unk7756[gRaceCourseIndex.s][j + 1] = D_800EC9F0.unk7756[gRaceCourseIndex.s][j];
                            D_800EC9F0.unk77C4[gRaceCourseIndex.s][j + 1] = D_800EC9F0.unk77C4[gRaceCourseIndex.s][j];
                        }
                        D_800EC9F0.unk7756[gRaceCourseIndex.s][i] = D_80121D80[0].unk2C0;
                        D_800EC9F0.unk77C4[gRaceCourseIndex.s][i] =
                            (D_80121D80[0].characterVariant * 8) + D_80121D80[0].characterId;
                    }
                    if (D_80121D80[0].unk2C0 >= 0x7D0) {
                        D_800EC9F0.unk78D7 |= 2;
                    }
                    if (i < 5) {
                        D_80121B5F = 1;
                    } else {
                        D_80121B5F = 2;
                        D_801235B4 |= 0x20;
                    }
                    D_801235B8->fadeTimer = 0xA;
                    func_8009956C(updateRaceResultsMusicFlow, 0);
                }
                break;

            case 1:
                task = func_80071408((void (*)(EffectTask *))func_8005B834, 6, 0x64);
                if (task != NULL) {
                    i = 0;
                    do {
                        if (D_800EC9F0.unk7832[gRaceCourseIndex.s][i] < D_801222F4) {
                            break;
                        }
                        i++;
                    } while (i < 5);
                    if (D_80121B81 != 0) {
                        i = 5;
                    }
                    task->unk10 = i;
                    if (i < 5) {
                        for (j = 3; j >= i; j--) {
                            D_800EC9F0.unk7832[gRaceCourseIndex.s][j + 1] = D_800EC9F0.unk7832[gRaceCourseIndex.s][j];
                            D_800EC9F0.unk7869[gRaceCourseIndex.s][j + 1] = D_800EC9F0.unk7869[gRaceCourseIndex.s][j];
                        }
                        D_800EC9F0.unk7832[gRaceCourseIndex.s][i] = D_80121D80[0].unk574;
                        D_800EC9F0.unk7869[gRaceCourseIndex.s][i] =
                            (D_80121D80[0].characterVariant * 8) + D_80121D80[0].characterId;
                    }
                    if (i < 5) {
                        D_80121B5F = 1;
                    } else {
                        D_80121B5F = 2;
                        D_801235B4 |= 0x20;
                    }
                    D_801235B8->fadeTimer = 0xA;
                    func_8009956C(updateRaceResultsMusicFlow, 0);
                }
                break;

            case 0:
                task = func_80071408((void (*)(EffectTask *))func_8005CE4C, 6, 0x64);
                if (task != NULL) {
                    currentTime = D_80121B74.fraction + (D_80121B74.seconds * COURSE_TIME_SECOND) +
                                  (D_80121B74.minutes * COURSE_TIME_MINUTE);
                    i = 0;
                    do {
                        recordTime = (D_800EC9F0.unk156[gRaceCourseIndex.s][i].unk0 * COURSE_TIME_MINUTE) +
                                     (D_800EC9F0.unk156[gRaceCourseIndex.s][i].unk1 * COURSE_TIME_SECOND) +
                                     D_800EC9F0.unk156[gRaceCourseIndex.s][i].unk2;
                        if (currentTime < recordTime) {
                            break;
                        }
                        i++;
                    } while (i < 5);
                    if (D_80121B81 != 0) {
                        i = 5;
                    }
                    task->unk10 = i;
                    if (i < 5) {
                        for (j = 3; j >= i; j--) {
                            D_800EC9F0.unk156[gRaceCourseIndex.s][j + 1] = D_800EC9F0.unk156[gRaceCourseIndex.s][j];
                            D_800EC9F0.unk78A0[gRaceCourseIndex.s][j + 1] = D_800EC9F0.unk78A0[gRaceCourseIndex.s][j];
                        }
                        D_800EC9F0.unk156[gRaceCourseIndex.s][i] = *(RaceFlowInitEntry *)&D_80121B74;
                        D_800EC9F0.unk78A0[gRaceCourseIndex.s][i] =
                            (D_80121D80[0].characterVariant * 8) + D_80121D80[0].characterId;
                    }
                    if (i < 5) {
                        D_80121B5F = 1;
                    } else {
                        D_80121B5F = 2;
                        D_801235B4 |= 0x20;
                    }
                    D_801235B8->fadeTimer = 0xA;
                    func_8009956C(updateRaceResultsMusicFlow, 0);
                }
                break;
            }
            break;
        }
    }

    if (D_80121B60 != 0) {
        func_80071664((void (*)(EffectTask *))func_8005393C, 5, 0x64, D_80121B60 - 1);
    }
    updateRaceFlowFrame();
}
#endif

void updateRaceResultsMusicFlow(void) {
    if (countActiveMusicSequences() == 0) {
        requestMusicSequenceBank(7);
        gCurrentGameTask->unk1C = 0x3C;
        gMenuFlowState |= 0x20;
    }
    if (D_80121B60 != 0) {
        createCallbackTaskWithUserId(&initFallingMenuSnowflake, 5, 0x64, D_80121B60 - 1);
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
    gFramebufferSwapDelay = 0;
    if (D_80121B60 != 0) {
        createCallbackTaskWithUserId(initFallingMenuSnowflake, 5, 0x64, D_80121B60 - 1);
    }
    updateRaceFlowFrame();
    gRaceRumbleEnabled = 0;
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0xFF) {
        gMenuFadeAlpha = 0xFF;
        gCurrentGameTask->fadeTimer -= 1;
        if (gCurrentGameTask->fadeTimer == 0) {
            stopSoundEffects();
            if ((gRaceSplitscreenMode == 2) && (((Unk80043040 *)getRelocatableHeapBlockBase(gRaceReplayInputBufferHandle))->unk8 != 0) && (D_80121B61 != 0) &&
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

// initRaceGhostReplayFlow best match: 99.464%
#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/initRaceGhostReplayFlow.s")

#ifdef NON_MATCHING
void initRaceGhostReplayFlow(void) {
    s32 one;
    s32 asset;

    gRaceUpdatePaused = 0;
    gRaceCameraModeChangeDisabled = 1;
    D_80121B5F = 0;
    asset = getRelocatableHeapBlockBase(gAssetHandles[0x2B]);
    if (((Unk80043040 *)asset)->unk8 == 0) {
        gFramebufferSwapHold = 1;
        setCurrentGameTaskCallback(finalizeRaceExitFlow, 0);
        return;
    }
    if (hasPendingRaceReplayCourseGridEntry() == 0) {
        gFramebufferSwapHold = 1;
        setCurrentGameTaskCallback(finalizeRaceExitFlow, 0);
        return;
    }
    ((Unk80043040 *)asset)->unk0 = 0;
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
        gAssetHandles[0x25] = freeRelocatableHeapBlock(gAssetHandles[0x25]);
    }
    if (gAssetHandles[0x26] != -1) {
        gAssetHandles[0x26] = freeRelocatableHeapBlock(gAssetHandles[0x26]);
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
    ((Unk80043040 *)asset)->unk0 = 0;
    one = 1;
    D_80121D80[0].unk4 = 0;
    D_80121D80[1].unk4 = one;
    D_80121D80[2].unk4 = one;
    D_80121D80[3].unk4 = one;
    D_80121D80[0].unk13 = one;
    D_80121D80[1].unk13 = 0;
    D_80121D80[2].unk13 = 0;
    D_80121D80[3].unk13 = 0;
    gRacePlayerCount = 1;
    *(u16 *)&gRaceLapCount = 1;
    D_80121B5C = 0x64;
    initCallbackTaskScheduler(2);
    D_80121D95 = 0;
    D_80121D96 = 0;
    D_80121D94 = 0;
    D_801223A1 = 0;
    D_801223A2 = 0;
    D_801223A0 = 0;
    D_801229AD = 0;
    D_801229AE = 0;
    D_801229AC = 0;
    D_80122FB9 = 0;
    D_80122FBA = 0;
    D_80122FB8 = 0;
    D_80121D80[0].unk16 = 2;
    gMenuFlowState = 0;
    resetRaceCameras();
    resetAllViewports();
    if (gRaceCourseIndex.s != 6) {
        configureViewport(0, 0xA0, 0x78, 0x100, 0xB0, 0x120, 0xD0, D_800E16CC);
    } else {
        func_80070A70(0, 0xA0, 0x78, 0x100, 0xB0, 0x120, 0xD0, D_800E16D0);
    }
    loadCompressedRomAsset(D_598A70, D_59AAA0, 0x29);
    D_8011228C = 1;
    gFramebufferSwapDelay = 0;
    resetSecondaryRng();
    func_8008BEB0();
    func_80078430();
    initRaceCourseEffects();
    gMenuFadeAlpha = 0xFF;
    updateRelocatableHeap();
    requestMusicSequenceBank(0);
    gCurrentGameTask->fadeTimer = 0;
    gCurrentGameTask->unk1C = 0;
    createCallbackTask((void (*)(CallbackTask *))func_80057E60, 6, 0x64);
    if (D_80121B61 == -1) {
        createCallbackTask((void (*)(CallbackTask *))updateRaceGhostUnavailableMessage, 6, 0x64);
    }
    setCurrentGameTaskCallback(updateRaceGhostReplayFlow, 0);
}
#endif

void updateRaceGhostReplayFlow(void) {
    void *sp18;

    if (gCurrentGameTask->fadeTimer == D_800DC5C0[gCurrentGameTask->unk1C]) {
        if (loadNextRaceReplayCourseGridEntry() != 0) {
            if (gCurrentGameTask->unk1C != 0xB) {
                gCurrentGameTask->unk1C++;
                D_80121D80[0].unk16 = 2;
                D_80121D80[0].unk15 = 0;
                D_801124B8 = 0;
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
    if (gPlayerInputPressed[0] & 0x1000) {
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
        gFramebufferSwapDelay = 0;
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
    func_8008C704();
    updateCallbackTasksWithMinPriority(0x63);
    func_80096E3C();
    updateRemainingCallbackTasks();
    updateRaceCameras();
    func_8007AA50();
}

void updateRaceFlowFrame(void) {
    func_8008C704();
    updateCallbackTasksWithMinPriority(0x63);
    func_80096E3C();
    updateRemainingCallbackTasks();
    updateRaceCameras();
}

void updateRaceReplayFrame(void) {
    func_8008C704();
    updateCallbackTasksWithMinPriority(0x63);
    func_80096E3C();
    updateRemainingCallbackTasks();
    updateRaceCameras();
}

s32 areRacePlayersFinished(void) {
    s32 var_v0;
    RacePlayerState *player;

    if (gRaceSplitscreenMode == 1) {
        if (!(D_8012207C & 0x40)) {
            return 0;
        }
    } else {
        var_v0 = 0;
        if ((s32)gPlayerCount > 0) {
            player = D_80121D80;
loop:
            if (!(player->flags & RACE_PLAYER_READY_FLAG)) {
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
    gRaceLapCount = D_800B9542[gRaceCourseIndex.s].unk0;
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
    gFramebufferSwapDelay = 0;
    loadCompressedRomAsset(D_593D10, D_598A70, 0x29);
    loadCompressedRomAsset(D_60F1A0, D_60F990, 0x2A);
    loadCompressedRomAsset(D_59DFE0, D_59E7F0, 0x26);
    loadCompressedRomAsset(D_1F1A90, D_1F2220, 0x28);
    switch (gRaceCourseIndex.u) {
    case 0:
        loadCompressedRomAsset(D_1EC0F0, D_1EC4A0, 0x1D);
        break;
    case 1:
        loadCompressedRomAsset(D_1EC4A0, D_1EC810, 0x1D);
        break;
    case 2:
        loadCompressedRomAsset(D_1EC810, D_1ECD00, 0x1D);
        break;
    case 3:
        loadCompressedRomAsset(D_1ECD00, D_1ED910, 0x1D);
        break;
    case 4:
        loadCompressedRomAsset(D_1ED910, D_1EDC80, 0x1D);
        break;
    case 5:
        loadCompressedRomAsset(D_1EDC80, D_1EE060, 0x1D);
        break;
    case 6:
        loadCompressedRomAsset(D_1EE060, D_1EEEA0, 0x1D);
        break;
    case 8:
        loadCompressedRomAsset(D_1EEEA0, D_1EF1D0, 0x1D);
        break;
    case 9:
        loadCompressedRomAsset(D_1EF1D0, D_1EF530, 0x1D);
        break;
    }
    loadCompressedRomAsset(D_1E74E0, D_1EC0F0, 0x1C);
    initCallbackTaskScheduler(0);
    gMenuFlowState = 0;
    createCallbackTaskWithUserId(initRaceRecordSettingsPanel, 0, 0x64, 0);
    createCallbackTask((void (*)(CallbackTask *))func_8001710C, 0, 0x5E);
    setCurrentGameTaskCallback(updateRaceRecordSettingsFlow, 0);
    requestMusicSequenceBank(7);
}

void updateRaceRecordSettingsFlow(void) {
    s32 temp_v0;

    gMenuFadeAlpha -= 0x10;
    if (gMenuFadeAlpha < 0) {
        gMenuFadeAlpha = 0;
        temp_v0 = gPlayerInputPressed[0];
        if ((temp_v0 & 0x20400) && (gRaceLapCount != 9)) {
            enqueueSoundEffect(0x19, 0x32);
            gRaceLapCount += 1;
            temp_v0 = gPlayerInputPressed[0];
        }
        if ((temp_v0 & 0x10800) && (gRaceLapCount != 1)) {
            enqueueSoundEffect(0x19, 0x32);
            gRaceLapCount -= 1;
            temp_v0 = gPlayerInputPressed[0];
        }
        if (temp_v0 & 0x9000) {
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
        gFramebufferSwapDelay = 0;
        setCurrentGameTaskCallback(&initRaceSceneFlow, 0);
    }
}
