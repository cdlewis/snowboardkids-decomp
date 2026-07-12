#include "common.h"
#include "asset_manager.h"
#include "callback_task_scheduler.h"
#include "sound_manager.h"
#include "race_setup_menu.h"
#include "game_task_scheduler.h"
#include "race_setup_ui.h"
#include "viewport_manager.h"

// initRaceSetupMenu best match: 98.403% (nonmatchings/initRaceSetupMenu-8331816093655448999/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_setup_menu/initRaceSetupMenu.s")

#ifdef NON_MATCHING
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 fade;
    /* 0x1C */ s32 timer;
} RaceSetupMenuState;

typedef struct {
    /* 0x0 */ u8 state;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ u8 unk4;
} RaceSetupMenuSubState;

extern void enqueueSoundEffect();
extern f32 D_800E0900;
extern u8 gConnectedControllerCount;
extern s8 gFramebufferSwapDelay;
extern s16 gMenuFadeAlpha;
extern u8 gRaceRumbleEnabled;
extern u8 gRumblePakConnectedByController[];
extern s8 D_800EC8B5;
extern s8 D_800EC8B6;
extern s8 D_800EC8B7;
extern u8 gMenuSelectionConfirmTimer;
extern char D_800EC9E5;
extern s8 gCourseSelectModeSelection;
extern CallbackTask *gActiveMenuTask;
extern u16 gMenuInputRepeatTimers;
extern s8 gHighestUnlockedCourse;
extern RaceSetupMenuSubState gRaceSetupMenuSubState;
extern s16 gRaceSetupSaveStatusSelections;
extern s16 D_8010AE08;
extern s16 D_8010AE0A;
extern s16 D_8010AE0C;
extern s8 gRaceSetupSaveStatusTransitionStates;
extern s8 D_8010AE0F;
extern s8 D_8010AE10;
extern s8 D_8010AE11;
extern s16 gRaceSetupSaveStatusNextSelections;
extern s16 D_8010AE14;
extern s16 D_8010AE16;
extern s16 D_8010AE18;
extern s16 gRaceCourseIndex;
extern u8 gPlayerCount;
extern s8 gCourseSelectSelectedCourseId;
extern s8 D_80122392;
extern s8 D_8012299E;
extern s8 D_80122FAA;
extern RaceSetupMenuState *gCurrentGameTask;
extern s32 gMenuFlowState;
extern s32 gPlayerInputHeld;
extern s32 gPlayerInputPressed;
extern u8 D_245A80;
extern u8 D_24C8E0;
extern u8 D_593D10;
extern u8 D_598A70;
extern u8 D_59AAA0;
extern u8 D_59DFE0;
extern u8 D_60F1A0;
extern u8 D_60F990;

void initRaceSetupMenu(void) {
    requestMusicSequenceBank(1);
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, D_800E0900);

    gFramebufferSwapDelay = 0;
    gCurrentGameTask->fade = 0;
    gCurrentGameTask->timer = 0;
    gMenuSelectionConfirmTimer = 0;
    D_800EC9E5 = 1;
    gCourseSelectModeSelection = 0;
    gHighestUnlockedCourse = 0;
    gRaceRumbleEnabled = 0;
    gRaceCourseIndex = 9;
    gMenuFlowState = 0;
    gMenuInputRepeatTimers = 0;
    gPlayerCount = 1;
    gMenuFadeAlpha = gCurrentGameTask->fade;

    loadCompressedRomAsset(&D_59AAA0, &D_59DFE0, 0x21);
    loadCompressedRomAsset(&D_245A80, &D_24C8E0, 0x1F);
    loadCompressedRomAsset(&D_593D10, &D_598A70, 0x22);
    loadCompressedRomAsset(&D_598A70, &D_59AAA0, 0x23);
    loadCompressedRomAsset(&D_60F1A0, &D_60F990, 0x29);

    initCallbackTaskScheduler(0);
    gActiveMenuTask = createCallbackTask(initRaceSetupPlayerCountPrompt, 0, 0x64);

    gRaceSetupMenuSubState.state = 0;
    gRaceSetupMenuSubState.unk1 = 0;
    gRaceSetupMenuSubState.unk2 = 0;
    gRaceSetupMenuSubState.unk4 = 0;
    gRumblePakConnectedByController[0] = 0;
    gRaceSetupSaveStatusSelections = 0;
    gRaceSetupSaveStatusTransitionStates = 0;
    gRaceSetupSaveStatusNextSelections = 0;
    gCourseSelectSelectedCourseId = 0;
    D_800EC8B5 = 0;
    D_8010AE08 = 0;
    D_8010AE0F = 0;
    D_8010AE14 = 0;
    D_80122392 = 0;
    D_800EC8B6 = 0;
    D_8010AE0A = 0;
    D_8010AE10 = 0;
    D_8010AE16 = 0;
    D_8012299E = 0;
    D_800EC8B7 = 0;
    D_8010AE0C = 0;
    D_8010AE11 = 0;
    D_8010AE18 = 0;
    D_80122FAA = 0;

    setCurrentGameTaskCallback(updateRaceSetupPlayerCountMenu, 0);
}
#endif

// updateRaceSetupPlayerCountMenu best match: 88.677% (nonmatchings/updateRaceSetupPlayerCountMenu-5802343343535905907/base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_setup_menu/updateRaceSetupPlayerCountMenu.s")

#ifdef NON_MATCHING

void updateRaceSetupPlayerCountMenu(void) {
    s32 sp18;
    s32 temp_a3;
    s32 temp_v0;
    register volatile u16 *var_a2;
    u16 temp_t6;
    u16 temp_v0_2;
    u8 temp_v1;
    s32 one;

    one = 1;
    if ((gRaceSetupMenuSubState.state == one) && ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000))) {
        enqueueSoundEffect((s16) (one & 0xFFFFFFFF), 0x32);
        gRaceSetupMenuSubState.state = 2;
        gRaceSetupMenuSubState.unk1 = 0;
        one = 1;
    }
    if (gCurrentGameTask->timer == one) {
        temp_v1 = gMenuSelectionConfirmTimer;
        if (temp_v1 == 0) {
            temp_a3 = gPlayerInputPressed;
            temp_v0 = gPlayerInputHeld & 0x10800;
            if ((temp_v0 == 0) && !(gPlayerInputHeld & 0x20400)) {
                gMenuInputRepeatTimers = 0;
            }
            var_a2 = &gMenuInputRepeatTimers;
            if ((temp_a3 & 0x10800) || ((temp_v0 != 0) && (gMenuInputRepeatTimers >= 9) && ((gMenuInputRepeatTimers % 3) == 0))) {
                if (gMenuInputRepeatTimers == 0) {
                    gMenuInputRepeatTimers += 1;
                }
                if (gPlayerCount != one) {
                    gPlayerCount -= 1;
                    sp18 = temp_a3;
                    enqueueSoundEffect(0x19, 0x32, &gMenuInputRepeatTimers, temp_a3);
                    goto block_25;
                }
            } else if ((temp_a3 & 0x20400) || ((gPlayerInputHeld & 0x20400) && (gMenuInputRepeatTimers >= 9) && ((gMenuInputRepeatTimers % 3) == 0))) {
                if (gMenuInputRepeatTimers == 0) {
                    gMenuInputRepeatTimers += 1;
                }
                if (gConnectedControllerCount != gPlayerCount) {
                    gPlayerCount += 1;
                    sp18 = temp_a3;
                    enqueueSoundEffect(0x19, 0x32, &gMenuInputRepeatTimers, temp_a3);
block_25:
                    var_a2 = &gMenuInputRepeatTimers;
                }
            }
            temp_v0_2 = *var_a2;
            temp_t6 = temp_v0_2 + 1;
            if (temp_v0_2 != 0) {
                *var_a2 = temp_t6;
                if (temp_t6 == 0xFFFF) {
                    *var_a2 = 0xA;
                }
            }
            if ((temp_a3 & 0x8000) || (temp_a3 & 0x1000)) {
                gMenuSelectionConfirmTimer = 1;
                enqueueSoundEffect(0x18, 0x32, var_a2, temp_a3);
            }
        } else if (temp_v1 < 0x13) {
            gMenuSelectionConfirmTimer = temp_v1 + 1;
        }
    }
    if (gRaceSetupMenuSubState.state == 5) {
        setCurrentGameTaskCallback(initRaceSetupSaveMenu, 0);
        gCurrentGameTask->fade = 0;
        gMenuSelectionConfirmTimer = 0;
    }
    updateCallbackTasks();
}
#endif

// initRaceSetupSaveMenu best match: 99.808% (nonmatchings/initRaceSetupSaveMenu-5802343343535905907/base_22.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_setup_menu/initRaceSetupSaveMenu.s")

#ifdef NON_MATCHING
typedef struct {
    u8 pad0[0x8];
    u8 unk8;
    u8 pad9[0x3];
    s32 unkC;
    u8 pad10[0x558];
    s32 unk568;
    u8 pad56C[0xA0];
} RaceSetupPlayerState03798;

extern s16 D_800B31B8;
extern s16 D_800B31BA;
extern s16 D_800B31BC;
extern s16 D_800B31BE;
extern s16 D_800B31C0;
extern s16 D_800B31C2;
extern s16 D_800B31C4;
extern s16 D_800B31C6;
extern s16 gControllerPakStatusCodes[];
extern s16 gMenuChoicePromptState[];
extern u8 gControllerPakRetryCounts[];
extern u8 gControllerPakOperationCounts[];
extern u8 D_800EC9E4;
extern s32 D_8010ADE0;
extern s32 D_8010ADE4;
extern s32 D_8010ADE8;
extern s16 gRaceSetupSavePanelRect0X0;
extern s16 gRaceSetupSavePanelRect0Y0;
extern s16 gRaceSetupSavePanelRect0X1;
extern s16 gRaceSetupSavePanelRect0Y1;
extern s16 gRaceSetupSavePanelRect1X0;
extern s16 gRaceSetupSavePanelRect1Y0;
extern s16 gRaceSetupSavePanelRect1X1;
extern s16 gRaceSetupSavePanelRect1Y1;
extern RaceSetupPlayerState03798 D_80121D80[];

void initRaceSetupSaveMenu(void) {
    RaceSetupPlayerState03798 *player;
    s32 i;
    u8 count;
    int zero;

    i = 0;
    do {
        i++;
        gControllerPakStatusCodes[i - 1] = 0;
        zero = 0;
        gMenuChoicePromptState[i - 1] = 0;
        gControllerPakRetryCounts[i - 1] = zero;
        gControllerPakOperationCounts[i - 1] = 0;
    } while (&gControllerPakOperationCounts[i] < &D_800EC9E4);

    count = gConnectedControllerCount;
    do { D_800EC9E4 = zero; i = zero; if (count > zero) { player = D_80121D80; do { player++; player[-1].unk8 = 0; i++; } while (player < (&D_80121D80[count])); i = 0; } do { initRaceSetupPlayerSaveData(i); i++; } while (i < 4); D_8010ADE0 = 0; D_8010ADE4 = zero; D_8010ADE8 = 0; gMenuSelectionConfirmTimer = 0; gMenuFlowState = zero; gRaceRumbleEnabled = 0; gRaceSetupSavePanelRect0X0 = D_800B31B8; gRaceSetupSavePanelRect1X0 = D_800B31BA; gRaceSetupSavePanelRect0Y0 = D_800B31BC; } while (0);
    gRaceSetupSavePanelRect1Y0 = D_800B31BE;
    gRaceSetupSavePanelRect0X1 = D_800B31C0;
    gRaceSetupSavePanelRect1X1 = D_800B31C2;
    gRaceSetupSavePanelRect0Y1 = D_800B31C4;
    gRaceSetupSavePanelRect1Y1 = D_800B31C6;

    setCurrentGameTaskCallback(updateRaceSetupSaveMenu, zero);
    updateCallbackTasks();
}
#endif

// updateRaceSetupSaveMenu best match: 73.013% (nonmatchings/updateRaceSetupSaveMenu-5802343343535905907/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_setup_menu/updateRaceSetupSaveMenu.s")

#ifdef NON_MATCHING
typedef struct {
    u8 state;
    u8 unk1;
    s16 unk2;
    u8 unk4;
    u8 pad5;
    s16 unk6[4];
    u8 unkE[4];
    s16 unk12[4];
} RaceSetupSubState03798;

typedef struct {
    u8 pad0[0x4];
    s32 unk4;
    u8 pad8[0x44];
    u8 unk4C;
    u8 pad4D[0x78AB];
} RaceSetupSavePlayer03798;

typedef struct {
    u8 pad0[2];
    u8 state;
    u8 pad3;
    s16 targetScale;
    s16 timer;
    u8 selectedOption;
} ControllerPakRumbleCheckPromptTransition03798;

extern void requestControllerPakProbe();
extern void requestControllerPakSaveStatus();
extern void requestControllerPakSaveRead();
extern void requestControllerPakRepair();
extern void initControllerPakRumbleCheckPrompt(CallbackTask *);

extern u8 D_800B3199[];
extern s16 gControllerPakStatusCodes[];
extern s16 gMenuChoicePromptState[];
extern u8 gControllerPakRetryCounts[];
extern u8 gControllerPakOperationCounts[];
extern u8 D_800EC9E4;
extern RaceSetupSavePlayer03798 gGameSaveDataBuffer[];
extern ControllerPakRumbleCheckPromptTransition03798 gControllerPakRumbleCheckPromptTransition;
extern CallbackTask *D_8010ADE0;
extern CallbackTask *D_8010ADE4;
extern CallbackTask *D_8010ADE8;
extern RaceSetupPlayerState03798 D_80121D80[];

#define gRaceSetupSaveSubState (*(RaceSetupSubState03798 *)&gRaceSetupMenuSubState)
#define gRumblePakConnectedByControllerArray_03798 gRumblePakConnectedByController
#define gPlayerInputPressedArray_03798 (&gPlayerInputPressed)

void updateRaceSetupSaveMenu(void) {
    s32 allReady = 0;
    s16 allPresent = 0;
    CallbackTask *task = D_8010ADE0;
    CallbackTask *transitionTask = D_8010ADE8;
    s32 i;
    u8 *present;

    if ((gRaceSetupSaveSubState.unk4 == 1) && (gRaceSetupSaveSubState.state == 5)) {
        gRaceSetupSaveSubState.state = 6;
    }

    if ((task == NULL) || (gRaceSetupSaveSubState.unk4 != 0)) {
        if ((gRaceSetupSaveSubState.state >= 6) && (gRaceSetupSaveSubState.state < 8)) {
            if (gRaceSetupSaveSubState.state == 6) {
                s32 input = gPlayerInputPressedArray_03798[0];
                if ((input & 0x8000) || (input & 0x1000)) {
                    enqueueSoundEffect(1, 0x32);
                    gRaceSetupSaveSubState.state = 7;
                    gRaceSetupSaveSubState.unk2 = 0xFF;
                    gRaceSetupSaveSubState.unk1 = 0;
                }
            }
        } else {
            allReady = 1;
            allPresent = 1;
            i = 0;
            if (gPlayerCount > 0) {
                present = gControllerPakOperationCounts;
                do {
                    u8 presentValue = *present;

                    if (presentValue != 1) {
                        RaceSetupPlayerState03798 *player = &D_80121D80[i];
                        s32 stateIndex;
                        s16 *choice;
                        s16 choiceValue;
                        s32 input;

                        if ((transitionTask == NULL) || (gRaceSetupSaveSubState.unkE[i] == 0)) {
                            stateIndex = i * 2;
                            choice = &gMenuChoicePromptState[i];
                            choiceValue = *choice;
                            if (choiceValue != 0) {
                                stateIndex = 6;
                            } else {
                                stateIndex = gControllerPakStatusCodes[i];
                            }

                            switch (stateIndex) {
                                case 0: {
                                    s32 pakState;
                                    u16 playerIndex = i;

                                    gRumblePakConnectedByControllerArray_03798[i] = 0;
                                    requestRumbleMotorInit(playerIndex, gPlayerCount, choiceValue);
                                    pakState = gRumbleMotorStatuses[i];
                                    if ((pakState != 1) && (pakState != 0xB) && (pakState != 4)) {
                                        gRumblePakConnectedByControllerArray_03798[i] = 1;
                                    } else {
                                        gRumblePakConnectedByControllerArray_03798[i] = 0;
                                    }
                                    requestControllerPakProbe(playerIndex);
                                    break;
                                }

                                case 1:
                                    requestControllerPakSaveStatus((u16)i, gPlayerCount, choiceValue);
                                    break;

                                case 2: {
                                    u8 result;

                                    requestControllerPakSaveRead((u16)i, gPlayerCount, choiceValue);
                                    result = gControllerPakRetryCounts[i];
                                    if (result == 0) {
                                        player->unk568 = 0;
                                        player->unkC = gGameSaveDataBuffer[i].unk4;
                                        gControllerPakStatusCodes[i] = 8;
                                        *choice = 1;
                                    } else if (result == 3) {
                                        if (transitionTask != NULL) {
                                            gRaceSetupSaveSubState.unkE[i] = 2;
                                            gRaceSetupSaveSubState.unk12[i] = 0xD;
                                        } else {
                                            gControllerPakStatusCodes[i] = 0xD;
                                        }
                                        gControllerPakRetryCounts[i] = 0;
                                    }
                                    break;
                                }

                                case 3: {
                                    u8 result;

                                    requestControllerPakRepair((u16)i, gPlayerCount, choiceValue);
                                    result = gControllerPakRetryCounts[i];
                                    if (result == 0) {
                                        if (transitionTask != NULL) {
                                            gRaceSetupSaveSubState.unkE[i] = 2;
                                            gRaceSetupSaveSubState.unk12[i] = 0xF;
                                        } else {
                                            gControllerPakStatusCodes[i] = 0xF;
                                        }
                                    } else if (result == 3) {
                                        if (transitionTask != NULL) {
                                            gRaceSetupSaveSubState.unkE[i] = 2;
                                            gRaceSetupSaveSubState.unk12[i] = 0xE;
                                        } else {
                                            gControllerPakStatusCodes[i] = 0xE;
                                        }
                                        gControllerPakRetryCounts[i] = 0;
                                    }
                                    break;
                                }

                                case 4:
                                    input = gPlayerInputPressedArray_03798[i];
                                    if ((input & 0x8000) || (input & 0x1000)) {
                                        enqueueSoundEffect(1, 0x32);
                                        if (transitionTask != NULL) {
                                            gRaceSetupSaveSubState.unkE[i] = 2;
                                            gRaceSetupSaveSubState.unk12[i] = 0;
                                        } else {
                                            gControllerPakStatusCodes[i] = 0;
                                        }
                                    }
                                    break;

                                case 5:
                                    if (player->unk8 == 0) {
                                        player->unk8 = 1;
                                    }
                                    break;

                                case 6:
                                    if ((choiceValue == 3) || (choiceValue == 4)) {
                                        input = gPlayerInputPressedArray_03798[i];
                                        if ((input & 0x10800) && (choiceValue != 3)) {
                                            *choice = choiceValue - 1;
                                            enqueueSoundEffect(0x19, 0x32);
                                            input = gPlayerInputPressedArray_03798[i];
                                        }
                                        if (input & 0x20400) {
                                            choiceValue = *choice;
                                            if (choiceValue != 4) {
                                                *choice = choiceValue + 1;
                                                enqueueSoundEffect(0x19, 0x32);
                                                input = gPlayerInputPressedArray_03798[i];
                                            }
                                        }
                                        if ((input & 0x8000) || (input & 0x1000)) {
                                            s16 state = gControllerPakStatusCodes[i];

                                            enqueueSoundEffect(1, 0x32);
                                            if (*choice == 4) {
                                                if (state == 8) {
                                                    RaceSetupSavePlayer03798 *save = &gGameSaveDataBuffer[i];

                                                    initRaceSetupPlayerSaveData(i);
                                                    gRaceSetupSaveSubState.unk6[i] = 5;
                                                    player->unkC = save->unk4;
                                                } else if (state == 7) {
                                                    gRaceSetupSaveSubState.unk6[i] = 3;
                                                } else {
                                                    gRaceSetupSaveSubState.unk6[i] = 4;
                                                }
                                            } else {
                                                if (state == 8) {
                                                    gRaceSetupSaveSubState.unk6[i] = 0x12;
                                                } else if (state == 7) {
                                                    gRaceSetupSaveSubState.unk6[i] = 4;
                                                } else {
                                                    RaceSetupSavePlayer03798 *save = &gGameSaveDataBuffer[i];

                                                    gRaceSetupSaveSubState.unk6[i] = 5;
                                                    initRaceSetupPlayerSaveData(i);
                                                    player->unkC = save->unk4;
                                                }
                                            }
                                            *choice += 2;
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
                                    input = gPlayerInputPressedArray_03798[i];
                                    if ((input & 0x8000) || (input & 0x1000)) {
                                        enqueueSoundEffect(1, 0x32);
                                        *choice = D_800B3199[gControllerPakStatusCodes[i]];
                                    }
                                    break;

                                case 14:
                                case 15:
                                    input = gPlayerInputPressedArray_03798[i];
                                    if ((input & 0x8000) || (input & 0x1000)) {
                                        enqueueSoundEffect(1, 0x32);
                                        if (gControllerPakStatusCodes[i] == 0xE) {
                                            if (transitionTask != NULL) {
                                                gRaceSetupSaveSubState.unkE[i] = 2;
                                                gRaceSetupSaveSubState.unk12[i] = 0x11;
                                            } else {
                                                gControllerPakStatusCodes[i] = 7;
                                            }
                                        } else if (transitionTask != NULL) {
                                            gRaceSetupSaveSubState.unkE[i] = 2;
                                            gRaceSetupSaveSubState.unk12[i] = 0;
                                        } else {
                                            gControllerPakStatusCodes[i] = 0;
                                        }
                                    }
                                    break;

                                case 17:
                                    input = gPlayerInputPressedArray_03798[i];
                                    if ((input & 0x8000) || (input & 0x1000)) {
                                        enqueueSoundEffect(1, 0x32);
                                        gRaceSetupSaveSubState.unkE[i] = 2;
                                        gRaceSetupSaveSubState.unk12[i] = 0;
                                    }
                                    break;

                                case 18:
                                    if (player->unk8 == 0) {
                                        player->unk8 = 1;
                                    }
                                    break;
                            }
                        }
                        allReady &= player->unk8;
                    }

                    allPresent &= presentValue;
                    i++;
                    present++;
                } while (i < gPlayerCount);
            }
        }
    }

    if (allPresent != 0) {
        if (task == NULL) {
            D_800EC9E4++;
            if (D_800EC9E4 >= 5) {
                u8 *ptr;
                u8 *end;

                D_800EC9E4 = 0;
                D_8010ADE8 = createCallbackTask((void (*)(CallbackTask *))initRaceSetupSaveStatusWidgets, 0, 0x63);
                createCallbackTask((void (*)(CallbackTask *))initRaceSetupSavePanelIcons, 0, 0x63);
                D_8010ADE0 = createCallbackTask((void (*)(CallbackTask *))initRaceSetupSavePanelFrame, 0, 0x63);
                D_8010ADE4 = createCallbackTask((void (*)(CallbackTask *))initRaceSetupSaveChoicePrompts, 0, 0x63);
                if (gPlayerCount > 0) {
                    ptr = gControllerPakOperationCounts;
                    end = &gControllerPakOperationCounts[gPlayerCount];
                    do {
                        *ptr += 1;
                        ptr++;
                    } while (ptr < end);
                }
            }
        }
    }

    if (allReady != 0) {
        gMenuSelectionConfirmTimer++;
        if (gMenuSelectionConfirmTimer == 0xF) {
            RaceSetupSubState03798 *subState;
            RaceSetupSavePlayer03798 *save;
            RaceSetupSavePlayer03798 *end;

            setCurrentGameTaskCallback(updateRaceSetupRumblePrompt, 0);
            createCallbackTask(initControllerPakRumbleCheckPrompt, 0, 0x64);
            gControllerPakRumbleCheckPromptTransition.state = 6;
            gControllerPakRumbleCheckPromptTransition.selectedOption = 0;
            gControllerPakRumbleCheckPromptTransition.targetScale = 2;
            if (gPlayerCount > 0) {
                subState = &gRaceSetupSaveSubState;
                save = gGameSaveDataBuffer;
                end = &gGameSaveDataBuffer[gPlayerCount];
                do {
                    u8 value = save->unk4C;

                    subState->unkE[0] = 3;
                    if (gHighestUnlockedCourse < value) {
                        gHighestUnlockedCourse = value;
                    }
                    save++;
                    subState = (RaceSetupSubState03798 *)((u8 *)subState + 1);
                } while (save < end);
            }
        }
    }

    updateCallbackTasks();
}

#undef gRaceSetupSaveSubState
#undef gRumblePakConnectedByControllerArray_03798
#undef gPlayerInputPressedArray_03798
#endif

void raceSetupMenuNoop(void) {
}

// updateRaceSetupRumblePrompt best match: 92.414% (nonmatchings/updateRaceSetupRumblePrompt-5802343343535905907/base_12.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_setup_menu/updateRaceSetupRumblePrompt.s")

#ifdef NON_MATCHING
typedef struct {
    /* 0x0 */ s16 pad0;
    /* 0x2 */ u8 state;
    /* 0x3 */ u8 pad3;
    /* 0x4 */ u16 targetScale;
    /* 0x6 */ u16 timer;
    /* 0x8 */ u8 selectedOption;
    /* 0x9 */ u8 confirmSelection;
} ControllerPakRumbleCheckPromptTransition;

extern ControllerPakRumbleCheckPromptTransition gControllerPakRumbleCheckPromptTransition;
extern u8 gControllerPakRumbleCheckPromptState;
extern u16 D_8010ADD4;
extern u8 gControllerPakRumbleCheckPromptConfirmSelection;
extern void enqueueSoundEffect(s16 arg0, s32 arg1);
extern void requestRumbleMotorInit(u16 arg0);
extern void initCharacterSelectMenu(void);
extern s32 gRumbleMotorStatuses[];

void updateRaceSetupRumblePrompt(void) {
    s32 connectedCount;
    s32 i;
    u8 state;

    state = gControllerPakRumbleCheckPromptTransition.state;
    switch (state) {
        case 1:
            if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
                enqueueSoundEffect(1, 0x32);
                gControllerPakRumbleCheckPromptState = 2;
                D_8010ADD4 = 1;
                state = 2;
            }
            break;
        case 2:
            gRumblePakConnectedMask = 0;
            i = 0;
            if ((s32)gPlayerCount > 0) {
                do {
                    requestRumbleMotorInit(i);
                    if ((gRumbleMotorStatuses[i] != 1) && (gRumbleMotorStatuses[i] != 0xB) && (gRumbleMotorStatuses[i] != 4)) {
                        gRumblePakConnectedByController[i] = 1;
                        gRumblePakConnectedMask |= 1 << i;
                    } else {
                        gRumblePakConnectedByController[i] = 0;
                    }
                    i++;
                } while (i < (s32)gPlayerCount);
            }
            gControllerPakRumbleCheckPromptTransition.state = 7;
            gControllerPakRumbleCheckPromptTransition.timer = 0x11;
            state = 7;
            break;
        case 3:
            if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
                enqueueSoundEffect(1, 0x32);
                gControllerPakRumbleCheckPromptState = 4;
                state = 4;
            }
            break;
        case 6:
            connectedCount = 0;
            gRumblePakConnectedMask = 0;
            i = 0;
            if ((s32)gPlayerCount > 0) {
                do {
                    requestRumbleMotorInit(i);
                    if ((gRumbleMotorStatuses[i] != 1) && (gRumbleMotorStatuses[i] != 0xB) && (gRumbleMotorStatuses[i] != 4)) {
                        gRumblePakConnectedMask |= 1 << i;
                        gRumblePakConnectedByController[i] = 1;
                        connectedCount++;
                    } else {
                        gRumblePakConnectedByController[i] = 0;
                    }
                    i++;
                } while (i < (s32)gPlayerCount);
            }
            if (connectedCount == gPlayerCount) {
                gControllerPakRumbleCheckPromptTransition.selectedOption = 1;
                gControllerPakRumbleCheckPromptTransition.targetScale = 2;
            } else {
                gControllerPakRumbleCheckPromptTransition.selectedOption = 0;
                gControllerPakRumbleCheckPromptTransition.targetScale = 0;
            }
            gControllerPakRumbleCheckPromptTransition.state = 0;
            state = 0;
            break;
        case 7:
            gControllerPakRumbleCheckPromptTransition.timer--;
            if (gControllerPakRumbleCheckPromptTransition.timer == 0) {
                gControllerPakRumbleCheckPromptTransition.state = 8;
                gControllerPakRumbleCheckPromptTransition.targetScale = 2;
                state = 8;
            }
            break;
        case 8:
            if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
                enqueueSoundEffect(1, 0x32);
                gControllerPakRumbleCheckPromptState = 9;
                D_8010ADD4 = 3;
                gControllerPakRumbleCheckPromptConfirmSelection = 1;
                state = 9;
            }
            break;
        case 9:
            if ((gPlayerInputPressed & 0x10800) && (gControllerPakRumbleCheckPromptTransition.confirmSelection != 0)) {
                gControllerPakRumbleCheckPromptTransition.confirmSelection--;
                enqueueSoundEffect(0x19, 0x32);
            } else if (gPlayerInputPressed & 0x20400) {
                if (gControllerPakRumbleCheckPromptTransition.confirmSelection != 1) {
                    gControllerPakRumbleCheckPromptTransition.confirmSelection++;
                    enqueueSoundEffect(0x19, 0x32);
                }
            }
            if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
                enqueueSoundEffect(1, 0x32);
                if (gControllerPakRumbleCheckPromptConfirmSelection == 1) {
                    gControllerPakRumbleCheckPromptState = 1;
                    D_8010ADD4 = 0;
                } else {
                    connectedCount = 0;
                    i = 0;
                    if ((s32)gPlayerCount > 0) {
                        do {
                            if (gRumblePakConnectedByController[i] == 1) {
                                connectedCount++;
                            }
                            i++;
                        } while (i < (s32)gPlayerCount);
                    }
                    if (connectedCount > 0) {
                        gControllerPakRumbleCheckPromptState = 3;
                        D_8010ADD4 = 2;
                    } else {
                        gControllerPakRumbleCheckPromptState = 4;
                    }
                }
            }
            state = gControllerPakRumbleCheckPromptState;
            break;
    }
    if (state == 5) {
        setCurrentGameTaskCallback(initCharacterSelectMenu, 0);
    }
    updateCallbackTasks();
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/race_setup_menu/initRaceSetupPlayerSaveData.s")
