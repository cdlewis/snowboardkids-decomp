#include "game/race/race_state.h"
#include "common.h"
#include "font_encoding.h"
#include "game/save_data.h"
#include "assets.h"
#include "game/engine/asset_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/audio/sound_manager.h"
#include "game/menu/race_setup/race_setup_menu.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/race_setup/race_setup_ui.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"
#include "game/menu/character_select/character_select_menu.h"
#include "game/menu/course_select/course_select_menu.h"
#include "game/menu/course_select/multiplayer_course_select_menu.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"

typedef struct {
    /* 0x0 */ s16 pad0;
    /* 0x2 */ u8 state;
    /* 0x3 */ u8 pad3;
    /* 0x4 */ u16 targetScale;
    /* 0x6 */ u16 timer;
    /* 0x8 */ u8 selectedOption;
    /* 0x9 */ u8 confirmSelection;
} ControllerPakRumbleCheckPromptTransition;

typedef struct {
    /* 0x0 */ u8 state;
    /* 0x1 */ u8 pad1;
    /* 0x2 */ u16 targetScale;
} ControllerPakRumbleCheckPromptState;

typedef struct RaceSetupSaveDefaultTime {
    s8 minutes;
    s8 seconds;
    s8 fraction;
} RaceSetupSaveDefaultTime;

typedef union RaceSetupSaveDefaultTimeRow {
    u8 bytes[sizeof(RaceSetupSaveDefaultTime) * 5];
    RaceSetupSaveDefaultTime records[5];
} RaceSetupSaveDefaultTimeRow;

typedef union RaceSetupSaveDefaultTimeTable {
    u8 bytes[sizeof(RaceSetupSaveDefaultTimeRow) * 2];
    RaceSetupSaveDefaultTimeRow rows[2];
} RaceSetupSaveDefaultTimeTable;

typedef struct {
    u16 trickAttackScores[5];
    u16 padding;
    MenuGlyphScript courseNames[8][18];
    u16 tailPadding[2];
} RaceSetupDefaultSaveData;

u8 gMainMenuSecretCodeUnlocked = 0;
u8 gMainMenuSecretCodeStep = 0;
RaceSetupMenuPackedData gRaceSetupMenuPackedData = {
    0,
    { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 0 },
};
s16 gRaceSetupSavePanelInitialRects[4][2] = {
    { -0x114, -0xA4 },
    { -0x114, 0x48 },
    { 0x90, -0xA4 },
    { 0x90, 0x48 },
};
RaceSetupSaveDefaultTime D_800B31C8[11][5] = {
    { { 0x02, 0x0F, 0 }, { 0x02, 0x14, 0 }, { 0x02, 0x1E, 0 }, { 0x02, 0x28, 0 }, { 0x02, 0x32, 0 } },
    { { 0x02, 0x0C, 0 }, { 0x02, 0x0F, 0 }, { 0x02, 0x14, 0 }, { 0x02, 0x1E, 0 }, { 0x02, 0x26, 0 } },
    { { 0x01, 0x2F, 0 }, { 0x01, 0x32, 0 }, { 0x01, 0x38, 0 }, { 0x02, 0x00, 0 }, { 0x02, 0x08, 0 } },
    { { 0x01, 0x38, 0 }, { 0x02, 0x00, 0 }, { 0x02, 0x0C, 0 }, { 0x02, 0x15, 0 }, { 0x02, 0x19, 0 } },
    { { 0x01, 0x34, 0 }, { 0x02, 0x00, 0 }, { 0x02, 0x05, 0 }, { 0x02, 0x09, 0 }, { 0x02, 0x0D, 0 } },
    { { 0x01, 0x35, 0 }, { 0x01, 0x38, 0 }, { 0x02, 0x00, 0 }, { 0x02, 0x04, 0 }, { 0x02, 0x0A, 0 } },
    { { 0x01, 0x3B, 0 }, { 0x02, 0x02, 0 }, { 0x02, 0x0B, 0 }, { 0x02, 0x11, 0 }, { 0x02, 0x14, 0 } },
    { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
    { { 0, 0x1C, 0 }, { 0, 0x21, 0 }, { 0, 0x24, 0 }, { 0, 0x28, 0 }, { 0, 0x2A, 0 } },
    { { 0, 0x24, 0 }, { 0, 0x28, 0 }, { 0, 0x2D, 0 }, { 0, 0x32, 0 }, { 0, 0x37, 0 } },
    { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
};
RaceSetupSaveDefaultTime D_800B3270[11] = {
    { 1, 0x0E, 0 }, { 1, 0x03, 0 }, { 0, 0x31, 0 }, { 1, 0x14, 0 }, { 1, 0x06, 0 },
    { 0, 0x31, 0 }, { 0, 0x37, 0 }, { 0, 0, 0 }, { 0, 0x11, 0 }, { 0, 0x13, 0 }, { 0, 0, 0 },
};
RaceSetupSaveDefaultTimeRow D_800B3294[1] = {
    { { 0, 0x24, 0, 0, 0x2A, 0, 0, 0x2C, 0, 0, 0x30, 0, 0, 0x35, 0 } },
};
RaceSetupSaveDefaultTimeTable D_800B32A4 = {
    { 1, 0x3A, 0, 2, 0x03, 0, 2, 0x0B, 0, 2, 0x0D, 0, 2, 0x15, 0,
      2, 0x01, 0, 2, 0x06, 0, 2, 0x0D, 0, 2, 0x12, 0, 2, 0x16, 0 },
};
u8 D_800B32C4[10] = { 0x14, 0x0F, 0x0A, 0x05, 0, 0x28, 0x1E, 0x14, 0x0A, 0 };
RaceSetupDefaultSaveData D_800B32D0 = {
    { 0x190, 0xC8, 0x64, 0x32, 0 },
    0,
    {
        { _("BEGISNOW") },
        { _("TRAPED") },
        { _("NIGHT HIGHWAY") },
        { _("GRASS VALLEY") },
        { _("SNOW GARDEN") },
        { _("QUICKSAND VALLEY") },
        { _("SILVER MOUNTAIN") },
        { _("ZIPANGU") },
    },
    { 0, 0 },
};

/* Raw BSS owns the overlapping transition fields; this weak view preserves IDO's codegen. */
ControllerPakRumbleCheckPromptState gControllerPakRumbleCheckPromptState = { 0 };
#pragma weak gControllerPakRumbleCheckPromptState
RaceSetupMenuSubState gRaceSetupMenuSubState;
s16 gRaceSetupSavePanelRects[2][4];

extern u8 gConnectedControllerCount;
extern s32 enqueueSoundEffect(s16 soundId, s16 priority);
extern s16 gMenuFadeAlpha;
extern char D_800EC9E5;
extern u8 gHighestUnlockedCourse;
extern ControllerPakRumbleCheckPromptTransition gControllerPakRumbleCheckPromptTransition;
extern u8 gControllerPakRumbleCheckPromptConfirmSelection;
extern u8 gRumblePakConnectedMask;
extern CallbackTask *D_8010ADE0;
extern CallbackTask *D_8010ADE4;
extern s32 gMenuFlowState;

void initRaceSetupMenu(void) {
    s32 i;

    requestMusicSequenceBank(1);
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);

    gFramebufferSwapDelay.value = 0;
    gCurrentGameTask->fade = 0;
    gCurrentGameTask->timer = 0;
    gMenuSelectionConfirmTimer = 0;
    D_800EC9E5 = 1;
    gCourseSelectModeSelection = 0;
    gHighestUnlockedCourse = 0;
    gRaceRumbleEnabled = 0;
    gRaceCourseIndex.signedValue = 9;
    gMenuFlowState = 0;
    gMenuInputRepeatTimers[0] = 0;
    gPlayerCount = 1;
    gMenuFadeAlpha = gCurrentGameTask->fade;

    LOAD_ASSET(_59AAA0, 0x21);
    LOAD_ASSET(_245A80, 0x1F);
    LOAD_ASSET(_593D10, 0x22);
    LOAD_ASSET(_598A70, 0x23);
    LOAD_ASSET(_60F1A0, 0x29);

    initCallbackTaskScheduler(0);
    gActiveMenuTask = createCallbackTask((CallbackTaskCallback)initRaceSetupPlayerCountPrompt, 0, 0x64);

    gRaceSetupMenuSubState.state = 0;
    gRaceSetupMenuSubState.timer = 0;
    gRaceSetupMenuSubState.alpha = 0;
    gRaceSetupMenuSubState.forceUpdate = 0;
    for (i = 0; i < 4; i++) {
        gRumblePakConnectedByController[i] = 0;
        gRaceSetupMenuSubState.pendingStatusCodes[i] = 0;
        gRaceSetupMenuSubState.statusTransitionStates[i] = 0;
        gRaceSetupMenuSubState.nextStatusCodes[i] = 0;
        gRacePlayers[i].menuSelection = 0;
    }

    setCurrentGameTaskCallback(updateRaceSetupPlayerCountMenu, 0);
}

void updateRaceSetupPlayerCountMenu(void) {
    s32 confirmationNext;
    s32 temp_a3;
    s32 substateNext;
    s32 temp_v0;
    u8 temp_v1;
    u32 confirmationValue;
    u16 repeatTimer;
    s32 one;

    one = 1;
    substateNext = 2;
    if ((gRaceSetupMenuSubState.state == one) &&
        ((gPlayerInputPressed[0] & 0x8000) || (gPlayerInputPressed[0] & 0x1000))) {
        enqueueSoundEffect(one, 0x32);
        gRaceSetupMenuSubState.state = substateNext;
        gRaceSetupMenuSubState.timer = 0;
    }
    if (gCurrentGameTask->timer == one) {
        temp_v1 = gMenuSelectionConfirmTimer;
        confirmationValue = temp_v1;
        temp_v0 = temp_v1;
        if (temp_v1 == 0) {
            temp_a3 = gPlayerInputPressed[0];
            temp_v0 = gPlayerInputHeld[0] & 0x10800;
            if ((temp_v0 == 0) && !(gPlayerInputHeld[0] & 0x20400)) {
                gMenuInputRepeatTimers[0] = 0;
            }
            if ((temp_a3 & 0x10800) ||
                ((temp_v0 != 0) && ((s32)gMenuInputRepeatTimers[0] >= 9) &&
                 (((s32)gMenuInputRepeatTimers[0] % 3) == 0))) {
                if (gMenuInputRepeatTimers[0] == 0) {
                    gMenuInputRepeatTimers[0] += 1;
                }
                if (gPlayerCount != one) {
                    gPlayerCount -= 1;
                    enqueueSoundEffect(0x19, 0x32);
                }
            } else if ((temp_a3 & 0x20400) ||
                       ((gPlayerInputHeld[0] & 0x20400) && ((s32)gMenuInputRepeatTimers[0] >= 9) &&
                        (((s32)gMenuInputRepeatTimers[0] % 3) == 0))) {
                if (gMenuInputRepeatTimers[0] == 0) {
                    gMenuInputRepeatTimers[0] += 1;
                }
                if (gPlayerCount != gConnectedControllerCount) {
                    gPlayerCount += 1;
                    enqueueSoundEffect(0x19, 0x32);
                }
            }
            repeatTimer = gMenuInputRepeatTimers[0];
            if (repeatTimer != 0) {
                gMenuInputRepeatTimers[0] = repeatTimer + 1;
                if (gMenuInputRepeatTimers[0] == 0xFFFF) {
                    gMenuInputRepeatTimers[0] = 0xA;
                }
            }
            if ((temp_a3 & 0x8000) || (temp_a3 & 0x1000)) {
                gMenuSelectionConfirmTimer = 1;
                enqueueSoundEffect(0x18, 0x32);
            }
        } else if (temp_v0 < 0x13) {
            gMenuSelectionConfirmTimer = (confirmationNext = confirmationValue + 1);
        }
    }
    if (gRaceSetupMenuSubState.state == 5) {
        setCurrentGameTaskCallback(initRaceSetupSaveMenu, 0);
        gCurrentGameTask->fade = 0;
        gMenuSelectionConfirmTimer = 0;
    }
    updateCallbackTasks();
}

extern u8 gControllerPakOperationCounts[];
extern u8 D_800EC9E4;

void initRaceSetupSaveMenu(void) {
    s32 i;
    s32 connectedControllerCount;
    RacePlayer *player;

    for (i = 0; i < 4; i++) {
        gControllerPakStatusCodes[i] = 0;
        gMenuChoicePromptState[i] = 0;
        gControllerPakRetryCounts[i] = 0;
        gControllerPakOperationCounts[i] = 0;
    }

    D_800EC9E4 = 0;
    do { i = 0; connectedControllerCount = gConnectedControllerCount; if (connectedControllerCount > 0) { player = gRacePlayers; do { player++; player[-1].menuState = 0; i++; } while (player < &gRacePlayers[connectedControllerCount]); i = 0; } do { initRaceSetupPlayerSaveData(i); i++; } while (i < 4); D_8010ADE0 = 0; D_8010ADE4 = 0; } while (0);
    D_8010ADE8 = 0;
    gMenuSelectionConfirmTimer = 0;
    gMenuFlowState = 0;
    gRaceRumbleEnabled = 0;

    for (i = 0; i < 4; i++) {
        gRaceSetupSavePanelRects[0][i] = gRaceSetupSavePanelInitialRects[i][0];
        gRaceSetupSavePanelRects[1][i] = gRaceSetupSavePanelInitialRects[i][1];
    }

    setCurrentGameTaskCallback(updateRaceSetupSaveMenu, 0);
    updateCallbackTasks();
}

// updateRaceSetupSaveMenu best match: 99.498% (nonmatchings/updateRaceSetupSaveMenu-8498672362023432715/base_18.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/race_setup/race_setup_menu/updateRaceSetupSaveMenu.s")

#ifdef NON_MATCHING
#define CONTROLLER_PAK_STATUS_PROBE 0
#define CONTROLLER_PAK_STATUS_SAVE_STATUS 1
#define CONTROLLER_PAK_STATUS_SAVE_READ 2
#define CONTROLLER_PAK_STATUS_REPAIR 3
#define CONTROLLER_PAK_STATUS_RETRY 4
#define CONTROLLER_PAK_STATUS_READY 5
#define CONTROLLER_PAK_STATUS_NO_PAK 7
#define CONTROLLER_PAK_STATUS_SAVE_FOUND 8
#define CONTROLLER_PAK_STATUS_READ_FAILED 0xD
#define CONTROLLER_PAK_STATUS_REPAIR_FAILED 0xE
#define CONTROLLER_PAK_STATUS_REPAIRED 0xF
#define CONTROLLER_PAK_STATUS_REPAIR_CONFIRM 0x11
#define CONTROLLER_PAK_STATUS_USE_EXISTING_SAVE 0x12
#define CONTROLLER_PAK_RETRY_LIMIT 3
#define CONTROLLER_PAK_STATUS_CHOICE_PROMPT 6

#define SAVE_CHOICE_USE_PAK 3
#define SAVE_CHOICE_SKIP_PAK 4
#define SAVE_STATUS_TRANSITION_FADE_OUT 2
#define SAVE_STATUS_TRANSITION_DONE 3
#define SAVE_PANEL_CREATE_DELAY 5
#define SAVE_READY_CONFIRM_DELAY 0xF

extern void requestRumbleMotorInitWithContext(u16 controllerIndex, s32 playerCount, s32 choiceValue);
extern void requestControllerPakSaveStatusWithContext(u16 controllerIndex, s32 playerCount, s32 choiceValue);
extern void requestControllerPakSaveReadWithContext(u16 controllerIndex, s32 playerCount, s32 choiceValue);
extern void requestControllerPakRepairWithContext(u16 controllerIndex, s32 playerCount, s32 choiceValue);
extern void initControllerPakRumbleCheckPrompt(CallbackTask *);

extern u8 gControllerPakOperationCounts[];
extern u8 D_800EC9E4;

#if 0
void updateRaceSetupSaveMenu(void) {
    s32 allPlayersReady = 0;
    s16 allControllerPakOpsComplete = 0;
    CallbackTask *savePanelTask = D_8010ADE0;
    CallbackTask *saveStatusTask = D_8010ADE8;
    s32 playerIndex;
    u8 *operationCountPtr;

    if ((gRaceSetupMenuSubState.forceUpdate == 1) && (gRaceSetupMenuSubState.state == CONTROLLER_PAK_STATUS_READY)) {
        gRaceSetupMenuSubState.state = 6;
    }

    if ((savePanelTask == NULL) || (gRaceSetupMenuSubState.forceUpdate != 0)) {
        if ((gRaceSetupMenuSubState.state >= 6) && (gRaceSetupMenuSubState.state < 8)) {
            if (gRaceSetupMenuSubState.state == 6) {
                s32 input = gPlayerInputPressed[0];
                if ((input & A_BUTTON) || (input & START_BUTTON)) {
                    enqueueSoundEffect(1, 0x32);
                    gRaceSetupMenuSubState.state = 7;
                    gRaceSetupMenuSubState.alpha = 0xFF;
                    gRaceSetupMenuSubState.timer = 0;
                }
            }
        } else {
            allPlayersReady = 1;
            allControllerPakOpsComplete = 1;
            playerIndex = 0;
            if (gPlayerCount > 0) {
                operationCountPtr = gControllerPakOperationCounts;
                do {
                    u8 operationCount = *operationCountPtr;

                    if (operationCount != 1) {
                        RacePlayer *player = &gRacePlayers[playerIndex];
                        s32 statusCode;
                        s16 *choiceState;
                        s16 choiceValue;
                        s32 input;

                        if ((saveStatusTask == NULL) || (gRaceSetupMenuSubState.statusTransitionStates[playerIndex] == 0)) {
                            statusCode = playerIndex * 2;
                            choiceState = &gMenuChoicePromptState[playerIndex];
                            choiceValue = *choiceState;
                            if (choiceValue != 0) {
                                statusCode = 6;
                            } else {
                                statusCode = gControllerPakStatusCodes[playerIndex];
                            }

                            switch (statusCode) {
                                case CONTROLLER_PAK_STATUS_PROBE: {
                                    s32 pakState;
                                    u16 controllerIndex = playerIndex;

                                    gRumblePakConnectedByController[playerIndex] = 0;
                                    requestRumbleMotorInitWithContext(controllerIndex, gPlayerCount, choiceValue);
                                    pakState = gRumbleMotorStatuses[playerIndex];
                                    if ((pakState != 1) && (pakState != 0xB) && (pakState != 4)) {
                                        gRumblePakConnectedByController[playerIndex] = 1;
                                    } else {
                                        gRumblePakConnectedByController[playerIndex] = 0;
                                    }
                                    requestControllerPakProbe(controllerIndex);
                                    break;
                                }

                                case CONTROLLER_PAK_STATUS_SAVE_STATUS:
                                    requestControllerPakSaveStatusWithContext((u16)playerIndex, gPlayerCount, choiceValue);
                                    break;

                                case CONTROLLER_PAK_STATUS_SAVE_READ: {
                                    u8 result;

                                    requestControllerPakSaveReadWithContext((u16)playerIndex, gPlayerCount, choiceValue);
                                    result = gControllerPakRetryCounts[playerIndex];
                                    if (result == 0) {
                                        player->score = 0;
                                        player->money = gGameSaveDataBuffer[playerIndex].money;
                                        gControllerPakStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_SAVE_FOUND;
                                        *choiceState = 1;
                                    } else if (result == CONTROLLER_PAK_RETRY_LIMIT) {
                                        if (saveStatusTask != NULL) {
                                            gRaceSetupMenuSubState.statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_FADE_OUT;
                                            gRaceSetupMenuSubState.nextStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_READ_FAILED;
                                        } else {
                                            gControllerPakStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_READ_FAILED;
                                        }
                                        gControllerPakRetryCounts[playerIndex] = 0;
                                    }
                                    break;
                                }

                                case CONTROLLER_PAK_STATUS_REPAIR: {
                                    u8 result;

                                    requestControllerPakRepairWithContext((u16)playerIndex, gPlayerCount, choiceValue);
                                    result = gControllerPakRetryCounts[playerIndex];
                                    if (result == 0) {
                                        if (saveStatusTask != NULL) {
                                            gRaceSetupMenuSubState.statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_FADE_OUT;
                                            gRaceSetupMenuSubState.nextStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_REPAIRED;
                                        } else {
                                            gControllerPakStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_REPAIRED;
                                        }
                                    } else if (result == CONTROLLER_PAK_RETRY_LIMIT) {
                                        if (saveStatusTask != NULL) {
                                            gRaceSetupMenuSubState.statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_FADE_OUT;
                                            gRaceSetupMenuSubState.nextStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_REPAIR_FAILED;
                                        } else {
                                            gControllerPakStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_REPAIR_FAILED;
                                        }
                                        gControllerPakRetryCounts[playerIndex] = 0;
                                    }
                                    break;
                                }

                                case CONTROLLER_PAK_STATUS_RETRY:
                                    input = gPlayerInputPressed[playerIndex];
                                    if ((input & A_BUTTON) || (input & START_BUTTON)) {
                                        enqueueSoundEffect(1, 0x32);
                                        if (saveStatusTask != NULL) {
                                            gRaceSetupMenuSubState.statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_FADE_OUT;
                                            gRaceSetupMenuSubState.nextStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_PROBE;
                                        } else {
                                            gControllerPakStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_PROBE;
                                        }
                                    }
                                    break;

                                case CONTROLLER_PAK_STATUS_READY:
                                    if (player->menuState == 0) {
                                        player->menuState = 1;
                                    }
                                    break;

                                case 6:
                                    if ((choiceValue == SAVE_CHOICE_USE_PAK) || (choiceValue == SAVE_CHOICE_SKIP_PAK)) {
                                        input = gPlayerInputPressed[playerIndex];
                                        if ((input & (STICK_UP | U_JPAD)) && (choiceValue != SAVE_CHOICE_USE_PAK)) {
                                            *choiceState = choiceValue - 1;
                                            enqueueSoundEffect(0x19, 0x32);
                                            input = gPlayerInputPressed[playerIndex];
                                        }
                                        if (input & (STICK_DOWN | D_JPAD)) {
                                            choiceValue = *choiceState;
                                            if (choiceValue != SAVE_CHOICE_SKIP_PAK) {
                                                *choiceState = choiceValue + 1;
                                                enqueueSoundEffect(0x19, 0x32);
                                                input = gPlayerInputPressed[playerIndex];
                                            }
                                        }
                                        if ((input & A_BUTTON) || (input & START_BUTTON)) {
                                            s16 state = gControllerPakStatusCodes[playerIndex];

                                            enqueueSoundEffect(1, 0x32);
                                            if (*choiceState == SAVE_CHOICE_SKIP_PAK) {
                                                if (state == CONTROLLER_PAK_STATUS_SAVE_FOUND) {
                                                    GameSaveData *save = &gGameSaveDataBuffer[playerIndex];

                                                    initRaceSetupPlayerSaveData(playerIndex);
                                                    gRaceSetupMenuSubState.pendingStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_READY;
                                                    player->money = save->money;
                                                } else if (state == CONTROLLER_PAK_STATUS_NO_PAK) {
                                                    gRaceSetupMenuSubState.pendingStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_REPAIR;
                                                } else {
                                                    gRaceSetupMenuSubState.pendingStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_RETRY;
                                                }
                                            } else {
                                                if (state == CONTROLLER_PAK_STATUS_SAVE_FOUND) {
                                                    gRaceSetupMenuSubState.pendingStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_USE_EXISTING_SAVE;
                                                } else if (state == CONTROLLER_PAK_STATUS_NO_PAK) {
                                                    gRaceSetupMenuSubState.pendingStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_RETRY;
                                                } else {
                                                    GameSaveData *save = &gGameSaveDataBuffer[playerIndex];

                                                    gRaceSetupMenuSubState.pendingStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_READY;
                                                    initRaceSetupPlayerSaveData(playerIndex);
                                                    player->money = save->money;
                                                }
                                            }
                                            *choiceState += 2;
                                        }
                                    }
                                    break;

                                case 7:
                                case 8:
                                case 9:
                                case 10:
                                case 11:
                                case 12:
                                case 13:
                                case 16:
                                    input = gPlayerInputPressed[playerIndex];
                                    if ((input & A_BUTTON) || (input & START_BUTTON)) {
                                        enqueueSoundEffect(1, 0x32);
                                        *choiceState = gRaceSetupMenuPackedData.choicePromptStates[
                                            gControllerPakStatusCodes[playerIndex]
                                        ];
                                    }
                                    break;

                                case 14:
                                case 15:
                                    input = gPlayerInputPressed[playerIndex];
                                    if ((input & A_BUTTON) || (input & START_BUTTON)) {
                                        enqueueSoundEffect(1, 0x32);
                                        if (gControllerPakStatusCodes[playerIndex] == CONTROLLER_PAK_STATUS_REPAIR_FAILED) {
                                            if (saveStatusTask != NULL) {
                                                gRaceSetupMenuSubState.statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_FADE_OUT;
                                                gRaceSetupMenuSubState.nextStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_REPAIR_CONFIRM;
                                            } else {
                                                gControllerPakStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_NO_PAK;
                                            }
                                        } else if (saveStatusTask != NULL) {
                                            gRaceSetupMenuSubState.statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_FADE_OUT;
                                            gRaceSetupMenuSubState.nextStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_PROBE;
                                        } else {
                                            gControllerPakStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_PROBE;
                                        }
                                    }
                                    break;

                                case CONTROLLER_PAK_STATUS_REPAIR_CONFIRM:
                                    input = gPlayerInputPressed[playerIndex];
                                    if ((input & A_BUTTON) || (input & START_BUTTON)) {
                                        enqueueSoundEffect(1, 0x32);
                                        gRaceSetupMenuSubState.statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_FADE_OUT;
                                        gRaceSetupMenuSubState.nextStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_PROBE;
                                    }
                                    break;

                                case CONTROLLER_PAK_STATUS_USE_EXISTING_SAVE:
                                    if (player->menuState == 0) {
                                        player->menuState = 1;
                                    }
                                    break;
                            }
                        }
                        allPlayersReady &= player->menuState;
                    }

                    allControllerPakOpsComplete &= operationCount;
                    playerIndex++;
                    operationCountPtr++;
                } while (playerIndex < gPlayerCount);
            }
        }
    }

    if (allControllerPakOpsComplete != 0) {
        if (savePanelTask == NULL) {
            D_800EC9E4++;
            if (D_800EC9E4 >= SAVE_PANEL_CREATE_DELAY) {
                u8 *operationCount;
                u8 *operationCountEnd;

                D_800EC9E4 = 0;
                D_8010ADE8 = createCallbackTask((CallbackTaskCallback)initRaceSetupSaveStatusWidgets, 0, 0x63);
                createCallbackTask((CallbackTaskCallback)initRaceSetupSavePanelIcons, 0, 0x63);
                D_8010ADE0 = createCallbackTask((CallbackTaskCallback)initRaceSetupSavePanelFrame, 0, 0x63);
                D_8010ADE4 = createCallbackTask((CallbackTaskCallback)initRaceSetupSaveChoicePrompts, 0, 0x63);
                if (gPlayerCount > 0) {
                    operationCount = gControllerPakOperationCounts;
                    operationCountEnd = &gControllerPakOperationCounts[gPlayerCount];
                    do {
                        *operationCount += 1;
                        operationCount++;
                    } while (operationCount < operationCountEnd);
                }
            }
        }
    }

    if (allPlayersReady != 0) {
        gMenuSelectionConfirmTimer++;
        if (gMenuSelectionConfirmTimer == SAVE_READY_CONFIRM_DELAY) {
            u8 *statusTransitionState;
            GameSaveData *save;
            GameSaveData *end;

            setCurrentGameTaskCallback(updateRaceSetupRumblePrompt, 0);
            createCallbackTask((CallbackTaskCallback)initControllerPakRumbleCheckPrompt, 0, 0x64);
            gControllerPakRumbleCheckPromptTransition.state = 6;
            gControllerPakRumbleCheckPromptTransition.selectedOption = 0;
            gControllerPakRumbleCheckPromptTransition.targetScale = 2;
            if (gPlayerCount > 0) {
                statusTransitionState = gRaceSetupMenuSubState.statusTransitionStates;
                save = &gGameSaveDataBuffer[0];
                end = &gGameSaveDataBuffer[0] + gPlayerCount;
                do {
                    u8 progressionLevel = save->progressionLevel;

                    *statusTransitionState = SAVE_STATUS_TRANSITION_DONE;
                    if (gHighestUnlockedCourse < progressionLevel) {
                        gHighestUnlockedCourse = progressionLevel;
                    }
                    save++;
                    statusTransitionState++;
                } while (save < end);
            }
        }
    }

    updateCallbackTasks();
}
#endif

void updateRaceSetupSaveMenu(void) {
    CallbackTaskCallback saveChoicePromptInitializer;
    s16 *choiceState;
    s32 allPlayersReady = 0;
    s16 allControllerPakOpsComplete = 0;
    s16 statusCode;
    CallbackTask *saveStatusTask = D_8010ADE8;
    CallbackTask * volatile savePanelTask;
    s32 i;
    s32 controllerIndex;

    savePanelTask = D_8010ADE0;

    if ((gRaceSetupMenuSubState.forceUpdate == 1) &&
        (gRaceSetupMenuSubState.state == CONTROLLER_PAK_STATUS_READY)) {
        gRaceSetupMenuSubState.state = 6;
    }

    if ((savePanelTask == NULL) || (gRaceSetupMenuSubState.forceUpdate != 0)) {
        if ((gRaceSetupMenuSubState.state >= 6) && (gRaceSetupMenuSubState.state < 8)) {
            if (gRaceSetupMenuSubState.state == 6) {
                if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                    enqueueSoundEffect(1, 0x32);
                    gRaceSetupMenuSubState.state = 7;
                    gRaceSetupMenuSubState.alpha = 0xFF;
                    gRaceSetupMenuSubState.timer = 0;
                }
            }
        } else {
            allPlayersReady = 1;
            allControllerPakOpsComplete = 1;
            for (i = 0; i < gPlayerCount; i++) {
                choiceState = &gMenuChoicePromptState[i];
                if (gControllerPakOperationCounts[i] != 1) {
                    if ((saveStatusTask == NULL) || (gRaceSetupMenuSubState.statusTransitionStates[i] == 0)) {
                        if (*(&gMenuChoicePromptState[i]) != 0) {
                            statusCode = CONTROLLER_PAK_STATUS_CHOICE_PROMPT;
                        } else {
                            statusCode = gControllerPakStatusCodes[i];
                        }
                        switch (statusCode) {
                            case CONTROLLER_PAK_STATUS_PROBE: {
                                s32 currentControllerIndex = i & 0xFFFF;

                                gRumblePakConnectedByController[i] = 0;
                                requestRumbleMotorInit((u16)currentControllerIndex);
                                if ((gRumbleMotorStatuses[i] != 1) && (gRumbleMotorStatuses[i] != 0xB) &&
                                    (gRumbleMotorStatuses[i] != 4)) {
                                    gRumblePakConnectedByController[i] = 1;
                                } else {
                                    gRumblePakConnectedByController[i] = 0;
                                }
                                requestControllerPakProbe((u16)currentControllerIndex);
                                break;
                            }

                            case CONTROLLER_PAK_STATUS_SAVE_STATUS:
                                requestControllerPakSaveStatus((u16)i);
                                break;

                            case CONTROLLER_PAK_STATUS_SAVE_READ: {
                                requestControllerPakSaveRead((u16)i);
                                if (gControllerPakRetryCounts[i] == 0) {
                                    gRacePlayers[i].money = gGameSaveDataBuffer[i].money;
                                    gRacePlayers[i].score = 0;
                                    gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_SAVE_FOUND;
                                    *(&gMenuChoicePromptState[i]) = 1;
                                } else if (gControllerPakRetryCounts[i] == CONTROLLER_PAK_RETRY_LIMIT) {
                                    if (saveStatusTask != NULL) {
                                        gRaceSetupMenuSubState.statusTransitionStates[i] =
                                            SAVE_STATUS_TRANSITION_FADE_OUT;
                                        gRaceSetupMenuSubState.nextStatusCodes[i] =
                                            CONTROLLER_PAK_STATUS_READ_FAILED;
                                    } else {
                                        gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_READ_FAILED;
                                    }
                                    gControllerPakRetryCounts[i] = 0;
                                }
                                break;
                            }

                            case CONTROLLER_PAK_STATUS_REPAIR: {
                                requestControllerPakRepair((u16)i);
                                if (gControllerPakRetryCounts[i] == 0) {
                                    if (saveStatusTask != NULL) {
                                        gRaceSetupMenuSubState.statusTransitionStates[i] =
                                            SAVE_STATUS_TRANSITION_FADE_OUT;
                                        gRaceSetupMenuSubState.nextStatusCodes[i] =
                                            CONTROLLER_PAK_STATUS_REPAIRED;
                                    } else {
                                        gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_REPAIRED;
                                    }
                                } else if (gControllerPakRetryCounts[i] == CONTROLLER_PAK_RETRY_LIMIT) {
                                    if (saveStatusTask != NULL) {
                                        gRaceSetupMenuSubState.statusTransitionStates[i] =
                                            SAVE_STATUS_TRANSITION_FADE_OUT;
                                        gRaceSetupMenuSubState.nextStatusCodes[i] =
                                            CONTROLLER_PAK_STATUS_REPAIR_FAILED;
                                    } else {
                                        gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_REPAIR_FAILED;
                                    }
                                    gControllerPakRetryCounts[i] = 0;
                                }
                                break;
                            }

                            case CONTROLLER_PAK_STATUS_RETRY:
                                if ((gPlayerInputPressed[i] & A_BUTTON) || (gPlayerInputPressed[i] & START_BUTTON)) {
                                    enqueueSoundEffect(1, 0x32);
                                    if (saveStatusTask != NULL) {
                                        gRaceSetupMenuSubState.statusTransitionStates[i] =
                                            SAVE_STATUS_TRANSITION_FADE_OUT;
                                        gRaceSetupMenuSubState.nextStatusCodes[i] =
                                            CONTROLLER_PAK_STATUS_PROBE;
                                    } else {
                                        gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_PROBE;
                                    }
                                }
                                break;

                            case CONTROLLER_PAK_STATUS_READY:
                                if ((statusCode && statusCode) && statusCode) {
                                }
                                if (gRacePlayers[i].menuState == 0) {
                                    gRacePlayers[i].menuState = 1;
                                }
                                break;

                            case CONTROLLER_PAK_STATUS_CHOICE_PROMPT:
                                if ((*(&gMenuChoicePromptState[i]) == SAVE_CHOICE_USE_PAK) ||
                                    (*(&gMenuChoicePromptState[i]) == SAVE_CHOICE_SKIP_PAK)) {
                                    if ((gPlayerInputPressed[i] & (STICK_UP | U_JPAD)) &&
                                        (*(&gMenuChoicePromptState[i]) != SAVE_CHOICE_USE_PAK)) {
                                        *(&gMenuChoicePromptState[i]) = *(&gMenuChoicePromptState[i]) - 1;
                                        enqueueSoundEffect(0x19, 0x32);
                                    }
                                    if (gPlayerInputPressed[i] & (STICK_DOWN | D_JPAD)) {
                                        if (*(&gMenuChoicePromptState[i]) != SAVE_CHOICE_SKIP_PAK) {
                                            *(&gMenuChoicePromptState[i]) = *(&gMenuChoicePromptState[i]) - -1;
                                            enqueueSoundEffect(0x19, 0x32);
                                        }
                                    }
                                    if ((gPlayerInputPressed[i] & A_BUTTON) || (gPlayerInputPressed[i] & START_BUTTON)) {
                                        enqueueSoundEffect(1, 0x32);
                                        if (*(&gMenuChoicePromptState[i]) == SAVE_CHOICE_SKIP_PAK) {
                                            if (gControllerPakStatusCodes[i] == CONTROLLER_PAK_STATUS_SAVE_FOUND) {
                                                initRaceSetupPlayerSaveData(i);
                                                gRacePlayers[i].money = gGameSaveDataBuffer[i].money;
                                                gRaceSetupMenuSubState.pendingStatusCodes[i] =
                                                    CONTROLLER_PAK_STATUS_READY;
                                            } else if (gControllerPakStatusCodes[i] == CONTROLLER_PAK_STATUS_NO_PAK) {
                                                gRaceSetupMenuSubState.pendingStatusCodes[i] =
                                                    CONTROLLER_PAK_STATUS_REPAIR;
                                            } else {
                                                gRaceSetupMenuSubState.pendingStatusCodes[i] =
                                                    CONTROLLER_PAK_STATUS_RETRY;
                                            }
                                        } else {
                                            if (gControllerPakStatusCodes[i] == CONTROLLER_PAK_STATUS_SAVE_FOUND) {
                                                gRaceSetupMenuSubState.pendingStatusCodes[i] =
                                                    CONTROLLER_PAK_STATUS_USE_EXISTING_SAVE;
                                            } else if (gControllerPakStatusCodes[i] == CONTROLLER_PAK_STATUS_NO_PAK) {
                                                gRaceSetupMenuSubState.pendingStatusCodes[i] =
                                                    CONTROLLER_PAK_STATUS_RETRY;
                                            } else {
                                                gRaceSetupMenuSubState.pendingStatusCodes[i] =
                                                    CONTROLLER_PAK_STATUS_READY;
                                                initRaceSetupPlayerSaveData(i);
                                                gRacePlayers[i].money = gGameSaveDataBuffer[i].money;
                                            }
                                        }
                                        *choiceState += 2;
                                    }
                                }
                                break;

                            case CONTROLLER_PAK_STATUS_NO_PAK:
                            case CONTROLLER_PAK_STATUS_SAVE_FOUND:
                            case 9:
                            case 10:
                            case 11:
                            case 12:
                            case CONTROLLER_PAK_STATUS_READ_FAILED:
                            case 16:
                                if ((gPlayerInputPressed[i] & A_BUTTON) || (gPlayerInputPressed[i] & START_BUTTON)) {
                                    enqueueSoundEffect(1, 0x32);
                                    *(&gMenuChoicePromptState[i]) =
                                        gRaceSetupMenuPackedData.choicePromptStates[gControllerPakStatusCodes[i]];
                                }
                                break;

                            case CONTROLLER_PAK_STATUS_REPAIR_FAILED:
                            case CONTROLLER_PAK_STATUS_REPAIRED:
                                if ((gPlayerInputPressed[i] & A_BUTTON) || (gPlayerInputPressed[i] & START_BUTTON)) {
                                    enqueueSoundEffect(1, 0x32);
                                    if (gControllerPakStatusCodes[i] ==
                                        CONTROLLER_PAK_STATUS_REPAIR_FAILED) {
                                        if (saveStatusTask != NULL) {
                                            gRaceSetupMenuSubState.statusTransitionStates[i] =
                                                SAVE_STATUS_TRANSITION_FADE_OUT;
                                            gRaceSetupMenuSubState.nextStatusCodes[i] =
                                                CONTROLLER_PAK_STATUS_REPAIR_CONFIRM;
                                        } else {
                                            gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_NO_PAK;
                                        }
                                    } else if (saveStatusTask != NULL) {
                                        gRaceSetupMenuSubState.statusTransitionStates[i] =
                                            SAVE_STATUS_TRANSITION_FADE_OUT;
                                        gRaceSetupMenuSubState.nextStatusCodes[i] =
                                            CONTROLLER_PAK_STATUS_PROBE;
                                    } else {
                                        gControllerPakStatusCodes[i] = CONTROLLER_PAK_STATUS_PROBE;
                                        break;
                                    }
                                }
                                break;

                            case CONTROLLER_PAK_STATUS_REPAIR_CONFIRM:
                                if (gPlayerInputPressed[i] & A_BUTTON || gPlayerInputPressed[i] & START_BUTTON) {
                                    enqueueSoundEffect(1, 0x32);
                                    gRaceSetupMenuSubState.statusTransitionStates[i] =
                                        SAVE_STATUS_TRANSITION_FADE_OUT;
                                    gRaceSetupMenuSubState.nextStatusCodes[i] =
                                        CONTROLLER_PAK_STATUS_PROBE;
                                }
                                break;

                            case CONTROLLER_PAK_STATUS_USE_EXISTING_SAVE:
                                if (gRacePlayers[i].menuState == 0) {
                                    gRacePlayers[i].menuState = 1;
                                }
                        }
                    }
                    allPlayersReady &= gRacePlayers[i].menuState;
                }

                allControllerPakOpsComplete &= gControllerPakOperationCounts[i];
            }
        }
    }

    if (allControllerPakOpsComplete) {
        if (savePanelTask == NULL) {
            D_800EC9E4++;
            saveChoicePromptInitializer = (CallbackTaskCallback)initRaceSetupSaveChoicePrompts;
            if (D_800EC9E4 >= SAVE_PANEL_CREATE_DELAY) {
                D_800EC9E4 = 0;
                D_8010ADE8 =
                    createCallbackTask((CallbackTaskCallback)initRaceSetupSaveStatusWidgets, 0, 0x63);
                createCallbackTask((CallbackTaskCallback)initRaceSetupSavePanelIcons, 0, 0x63);
                D_8010ADE0 =
                    createCallbackTask((CallbackTaskCallback)initRaceSetupSavePanelFrame, 0, 0x63);
                D_8010ADE4 = createCallbackTask(saveChoicePromptInitializer, 0, 0x63);

                for (controllerIndex = 0; controllerIndex < gPlayerCount;
                     controllerIndex++) {
                    gControllerPakOperationCounts[controllerIndex]++;
                }
            }
        }
    }

    if (allPlayersReady) {
        gMenuSelectionConfirmTimer++;
        if (gMenuSelectionConfirmTimer == SAVE_READY_CONFIRM_DELAY) {
            setCurrentGameTaskCallback(updateRaceSetupRumblePrompt, 0);
            createCallbackTask((CallbackTaskCallback)initControllerPakRumbleCheckPrompt, 0, 0x64);
            gControllerPakRumbleCheckPromptTransition.state = 6;
            gControllerPakRumbleCheckPromptTransition.selectedOption = 0;
            gControllerPakRumbleCheckPromptTransition.targetScale = 2;

            for (i = 0; i < gPlayerCount; i++) {
                gRaceSetupMenuSubState.statusTransitionStates[i] = SAVE_STATUS_TRANSITION_DONE;
                if (gHighestUnlockedCourse < gGameSaveDataBuffer[i].progressionLevel) {
                    gHighestUnlockedCourse = gGameSaveDataBuffer[i].progressionLevel;
                }
            }
        }
    }

    updateCallbackTasks();
}

#undef SAVE_READY_CONFIRM_DELAY
#undef SAVE_PANEL_CREATE_DELAY
#undef SAVE_STATUS_TRANSITION_DONE
#undef SAVE_STATUS_TRANSITION_FADE_OUT
#undef SAVE_CHOICE_SKIP_PAK
#undef SAVE_CHOICE_USE_PAK
#undef CONTROLLER_PAK_RETRY_LIMIT
#undef CONTROLLER_PAK_STATUS_USE_EXISTING_SAVE
#undef CONTROLLER_PAK_STATUS_REPAIR_CONFIRM
#undef CONTROLLER_PAK_STATUS_REPAIRED
#undef CONTROLLER_PAK_STATUS_REPAIR_FAILED
#undef CONTROLLER_PAK_STATUS_READ_FAILED
#undef CONTROLLER_PAK_STATUS_SAVE_FOUND
#undef CONTROLLER_PAK_STATUS_NO_PAK
#undef CONTROLLER_PAK_STATUS_READY
#undef CONTROLLER_PAK_STATUS_RETRY
#undef CONTROLLER_PAK_STATUS_REPAIR
#undef CONTROLLER_PAK_STATUS_SAVE_READ
#undef CONTROLLER_PAK_STATUS_SAVE_STATUS
#undef CONTROLLER_PAK_STATUS_PROBE
#endif

void raceSetupMenuNoop(void) {
}

void updateRaceSetupRumblePrompt(void) {
    s32 statusIndex;
    s32 connectedCount;
    s32 i;
    s32 state;
    s32 one;

    state = gControllerPakRumbleCheckPromptTransition.state;
    switch (state) {
        case 0:
        case 4:
        case 5:
            break;

        case 1:
            if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                state = 2;
                enqueueSoundEffect(1, 0x32);
                gControllerPakRumbleCheckPromptState.state = state;
                gControllerPakRumbleCheckPromptState.targetScale = 1;
                state = 2;
            }
            break;

        case 2:
            gRumblePakConnectedMask = 0;
            for (i = 0; i < (s32)gPlayerCount; i++) {
                requestRumbleMotorInit(i);
                if ((gRumbleMotorStatuses[i] != 1) && (gRumbleMotorStatuses[i] != 0xB) &&
                    (gRumbleMotorStatuses[i] != 4)) {
                    gRumblePakConnectedByController[i] = 1;
                    gRumblePakConnectedMask |= 1 << i;
                } else {
                    gRumblePakConnectedByController[i] = 0;
                }
            }
            gControllerPakRumbleCheckPromptTransition.state = 7;
            gControllerPakRumbleCheckPromptTransition.timer = 0x11;
            state = 7;
            state = gControllerPakRumbleCheckPromptTransition.state;
            break;

        case 3:
            if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                enqueueSoundEffect(1, 0x32);
                gControllerPakRumbleCheckPromptState.state = 4;
                state = 4;
            }
            break;

        case 6:
            connectedCount = 0;
            gRumblePakConnectedMask = 0;
            for (i = 0; i < (s32)gPlayerCount; i++) {
                statusIndex = i;
                requestRumbleMotorInit(i);
                if ((gRumbleMotorStatuses[statusIndex] != 1) && (gRumbleMotorStatuses[i] != 0xB) &&
                    (gRumbleMotorStatuses[i] != 4)) {
                    gRumblePakConnectedMask |= 1 << i;
                    gRumblePakConnectedByController[i] = 1;
                    connectedCount++;
                } else {
                    gRumblePakConnectedByController[i] = 0;
                }
            }
            if (connectedCount == gPlayerCount) {
                gControllerPakRumbleCheckPromptTransition.selectedOption = 1;
                gControllerPakRumbleCheckPromptTransition.targetScale = 2;
            } else {
                gControllerPakRumbleCheckPromptTransition.selectedOption = 0;
                gControllerPakRumbleCheckPromptTransition.targetScale = 0;
            }
            state = (gControllerPakRumbleCheckPromptTransition.state = 0);
            break;

        case 7:
            gControllerPakRumbleCheckPromptTransition.timer--;
            if (gControllerPakRumbleCheckPromptTransition.timer == 0) {
                gControllerPakRumbleCheckPromptTransition.state = 8;
                gControllerPakRumbleCheckPromptTransition.targetScale = 2;
                state = 8;
                state = gControllerPakRumbleCheckPromptTransition.state;
            }
            break;

        case 8:
            if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                state = 9;
                enqueueSoundEffect(1, 0x32);
                gControllerPakRumbleCheckPromptState.state = state;
                gControllerPakRumbleCheckPromptState.targetScale = 3;
                gControllerPakRumbleCheckPromptConfirmSelection = 1;
                state = 9;
            }
            break;

        case 9:
            if ((gPlayerInputPressed[0] & (STICK_UP | U_JPAD)) &&
                (gControllerPakRumbleCheckPromptTransition.confirmSelection != 0)) {
                gControllerPakRumbleCheckPromptTransition.confirmSelection--;
                enqueueSoundEffect(0x19, 0x32);
            } else if (gPlayerInputPressed[0] & (STICK_DOWN | D_JPAD)) {
                if (gControllerPakRumbleCheckPromptTransition.confirmSelection != 1) {
                    gControllerPakRumbleCheckPromptTransition.confirmSelection++;
                    enqueueSoundEffect(0x19, 0x32);
                }
            }
            one = 1;
            if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                enqueueSoundEffect(one, 0x32);
                statusIndex = 1;
                if (gControllerPakRumbleCheckPromptConfirmSelection == one) {
                    gControllerPakRumbleCheckPromptState.state = 1;
                    gControllerPakRumbleCheckPromptState.targetScale =
                        (statusIndex != 1) * 0;
                } else {
                    connectedCount = 0;
                    for (i = 0; i < (s32)gPlayerCount; i++) {
                        if (gRumblePakConnectedByController[i] == one) {
                            connectedCount++;
                        }
                    }
                    i = 4;
                    statusIndex = 3;
                    if (connectedCount > 0) {
                        gControllerPakRumbleCheckPromptState.state = statusIndex;
                        gControllerPakRumbleCheckPromptState.targetScale = 2;
                    } else {
                        gControllerPakRumbleCheckPromptState.state = i;
                    }
                }
            }
            state = gControllerPakRumbleCheckPromptState.state;
            break;
    }

    if (state == 5) {
        setCurrentGameTaskCallback(initCharacterSelectMenu, 0);
        gControllerPakRumbleCheckPromptTransition.confirmSelection =
            gControllerPakRumbleCheckPromptTransition.confirmSelection;
    }
    updateCallbackTasks();
}

// initRaceSetupPlayerSaveData best match: 83.095% (nonmatchings/initRaceSetupPlayerSaveData-8101714008744796594/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/race_setup/race_setup_menu/initRaceSetupPlayerSaveData.s")

#ifdef NON_MATCHING
typedef struct RaceSetupSaveByteCursor {
    u8 value;
} RaceSetupSaveByteCursor;

typedef struct RaceSetupSaveSecretCupCursorView {
    u8 pad[0x34];
    u8 value;
} RaceSetupSaveSecretCupCursorView;

typedef struct RaceSetupSaveUnlockCursorView {
    u8 pad[0x3F];
    s8 values[4];
} RaceSetupSaveUnlockCursorView;

typedef struct RaceSetupSaveUnlockForwardView {
    u8 pad[0x40];
    s8 values[3];
} RaceSetupSaveUnlockForwardView;

typedef struct RaceSetupSaveUnlockPreviousView {
    u8 pad[0x3B];
    s8 value;
} RaceSetupSaveUnlockPreviousView;

typedef struct RaceSetupSaveReservedCursorView {
    u8 pad[0x78D7];
    u8 value;
} RaceSetupSaveReservedCursorView;

void initRaceSetupPlayerSaveData(s32 arg0) {
    GameSaveData *header;
    GameSaveData *save;
    RaceSetupSaveByteCursor *destination;
    s32 fifteen;
    const s8 *courseStateSource;
    s32 course;
    s32 player;

    header = &gGameSaveDataBuffer[arg0 & 0xFFFFFFFFFFFFFFFF];
    save = &gGameSaveDataBuffer[arg0];
    header->checksum = 0;
    header->money = 0;

    for (player = 0; player < 11; player++) {
        save->highScores[player + 1] = 0;
        save->cupPlacements[player] = 0;
    }

    player = 0;
    destination = (RaceSetupSaveByteCursor *)save;
    while (player < 3) {
        ((RaceSetupSaveUnlockCursorView *)destination)->values[0] = player;
        player++;
        destination++;
    }

    {
        register s32 negativeOne;

        negativeOne = -1;
        save->courseUnlockStates[3] = negativeOne;
        player = 4;
        destination = (RaceSetupSaveByteCursor *)save + 4;
        while (player != 12) {
            player += 4;
            ((RaceSetupSaveUnlockForwardView *)destination)->values[0] = negativeOne;
            ((RaceSetupSaveUnlockForwardView *)destination)->values[1] = negativeOne;
            ((RaceSetupSaveUnlockForwardView *)destination)->values[2] = negativeOne;
            destination += 4;
            ((RaceSetupSaveUnlockPreviousView *)destination)->value = negativeOne;
        }
    }

    header->characterFlags = 0;
    header->progressionLevel = 0;
    header->extraCourseUnlockFlags = 0;

    fifteen = sizeof(RaceSetupSaveDefaultTimeRow);
    for (course = 0; course < 11; course++) {
        for (player = 0; player < 5; player++) {
            const RaceSetupSaveDefaultTime *raceDefaults;

            save->timeTrialRecords[course][player].seconds = D_800B31C8[course][player].seconds;
            save->timeTrialRecords[course][player].fraction =
                D_800B31C8[course][player].fraction << 8;
            save->timeTrialRecords[course][player].minutes = D_800B31C8[course][player].minutes;

            save->timeTrialCharacterIds[course][player] = 0x10;
            save->timeTrialCharacterIds[course][player] += player;

            if (course == 9) {
                raceDefaults = D_800B3294[0].records;
            } else {
                raceDefaults = (const RaceSetupSaveDefaultTime *)&D_800B32A4.bytes[
                    (course & 1) * fifteen
                ];
            }
            save->raceRecords[course][player].fraction = raceDefaults[player].fraction << 8;
            save->raceRecords[course][player].minutes = raceDefaults[player].minutes;
            save->raceRecords[course][player].seconds = raceDefaults[player].seconds;

            save->raceRecordCharacterIds[course][player] = 0x10;
            save->raceRecordCharacterIds[course][player] += player;

            if (course == 9) {
                save->scoreAttackScores[course][player] = D_800B32C4[player];
            } else {
                save->scoreAttackScores[course][player] = D_800B32C4[player + 5];
            }
            save->scoreAttackCharacterIds[course][player] = player;
            save->trickAttackScores[course][player] = D_800B32D0.trickAttackScores[player];
            save->trickAttackCharacterIds[course][player] = player;
        }

        {
            register s8 bestMinutes;
            register s8 bestSeconds;
            register s8 bestFraction;

            bestMinutes = D_800B3270[course].minutes;
            bestSeconds = D_800B3270[course].seconds;
            bestFraction = D_800B3270[course].fraction;
            save->bestLapRecords[course].minutes = bestMinutes;
            save->bestLapRecords[course].seconds = bestSeconds;
            save->bestLapRecords[course].fraction = bestFraction;
        }
    }

    header->replaySlots[0].length = 0;
    header->replaySlots[1].length = 0;
    header->replaySlots[2].length = 0;
    header->replaySlots[3].length = 0;
    header->replaySlots[4].length = 0;
    header->replaySlots[5].length = 0;
    header->replaySlots[6].length = 0;
    header->replaySlots[7].length = 0;
    header->replaySlots[8].length = 0;

    player = 0;
    destination = (RaceSetupSaveByteCursor *)save;
compact_tail_loop:
    player++;
    destination++;
    ((RaceSetupSaveReservedCursorView *)destination)->value = 0;
    if (player < 0x20) {
        goto compact_tail_loop;
    }

    if (gMainMenuSecretCodeUnlocked == 1) {
        player = 0;
        destination = (RaceSetupSaveByteCursor *)save;
compact_cup_loop:
        player++;
        ((RaceSetupSaveSecretCupCursorView *)destination)->value = 1;
        destination++;
        if (player < 11) {
            goto compact_cup_loop;
        }

        courseStateSource = gMultiplayerCourseSelectDefaultCourseIds;
        destination = (RaceSetupSaveByteCursor *)save;
        {
            register s8 state0;
            register s8 state1;
            register s8 state2;
            register s8 state3;

compact_state_loop:
            state0 = courseStateSource[0];
            state1 = courseStateSource[1];
            state2 = courseStateSource[2];
            state3 = courseStateSource[3];
            courseStateSource += 4;
            ((GameSaveData *)destination)->courseUnlockStates[0] = state0;
            ((GameSaveData *)destination)->courseUnlockStates[1] = state1;
            ((GameSaveData *)destination)->courseUnlockStates[2] = state2;
            ((GameSaveData *)destination)->courseUnlockStates[3] = state3;
            destination += 4;
            if (courseStateSource != &gMultiplayerCourseSelectDefaultCourseIds[
                    MULTIPLAYER_COURSE_SELECT_DEFAULT_COURSE_COUNT
                ]) {
                goto compact_state_loop;
            }
        }

        header->characterFlags = 1;
        header->progressionLevel = 3;
        header->extraCourseUnlockFlags = 0x3F;
    }
}
#endif
