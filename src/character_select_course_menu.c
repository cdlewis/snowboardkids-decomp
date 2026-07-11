#include "common.h"
#include "sound_manager.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "character_select_course_menu.h"
#include "character_select_course_ui.h"
#include "game_task_scheduler.h"
#include "menu_rendering.h"
#include "player_count_select_menu.h"
#include "player_select_menu.h"
#include "title_menu.h"
#include "viewport_manager.h"

typedef s16 CharacterSelectOptionList[10];
typedef s16 CharacterSelectCourseUnlockList[11];

typedef struct {
    char pad0[0x4C];
    u8 highestCourse;
    char pad4D[0x78AB];
} CharacterSelectSaveData;

typedef union {
    u8 bytes[8];
    struct {
        /* 0x00 */ u8 state;
        /* 0x01 */ u8 otherState;
        /* 0x02 */ u8 pad2[2];
        /* 0x04 */ s16 spriteIndex;
        /* 0x06 */ u8 timer;
        /* 0x07 */ u8 otherTimer;
    } fields;
} CharacterSelectCursorState;

extern void enqueueSoundEffect(s32, s32);
extern void releaseMenuAssetHandles(void);
extern u8 D_245A80[];
extern u8 D_24C8E0[];
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_59AAA0[];
extern u8 D_59DFE0[];
extern u8 D_59E7F0[];
extern u8 D_5A1ED0[];
extern u8 D_5C5320[];
extern u8 D_5CCD40[];
extern u8 D_5D4280[];
extern CharacterSelectCourseUnlockList gCharacterSelectCourseOptionsByUnlock[];
extern s16 gCharacterSelectShortCourseOptions[];
extern s16 gCharacterSelectSingleCourseOption[];
extern CharacterSelectFlowState *gCurrentGameTask;
extern s8 gFramebufferSwapDelay;
extern f32 D_800E09A4;
extern s16 gMenuFadeAlpha;
extern CharacterSelectSaveData D_800EC9F0[];
extern u8 D_80121D80[];
extern u8 D_80121D88;
extern u8 gPlayerCount;
extern u8 D_80121B5E;
extern s16 D_80121B50;
extern CharacterSelectOptionList *gCharacterSelectActiveCourseOptions;
extern s32 D_8010ADDC;
extern s32 D_8010ADE0;
extern s32 D_8010ADE4;
extern s32 D_801235B4;
extern s32 gPlayerInputHeld;
extern s32 gPlayerInputPressed;
#ifndef NON_MATCHING
extern u16 D_8010ADF0;
#else
extern volatile u16 D_8010ADF0;
#endif
extern u16 gCharacterSelectCourseExitOptionIndex;
extern CharacterSelectCursorState gCharacterSelectCourseCursorState;
extern u8 gCharacterSelectCourseSubmenuState;
extern u8 D_8010ADF8;
extern u8 gHighestUnlockedCourse;
extern u8 D_800EC9C1;
extern u8 D_800EC9C2;
extern s16 D_800EC9D0;
extern u8 D_800EC9DD;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

// initCharacterSelectCourseMenuFromPlayerCount best match: 79.600% (nonmatchings/initCharacterSelectCourseMenuFromPlayerCount-8207005055717715604/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_course_menu/initCharacterSelectCourseMenuFromPlayerCount.s")

#ifdef NON_MATCHING
void initCharacterSelectCourseMenuFromPlayerCount(void) {
    s32 sp1C;
    s16 *var_v0_2;
    s16 *var_v1_2;
    s32 var_v1;
    s32 var_v0_3;
    u8 temp_v1;
    CharacterSelectSaveData *var_v0;
    CharacterSelectSaveData *temp_a0;

    gCurrentGameTask->fade = 0;
    requestMusicSequenceBank(2);
    createCallbackTask((void (*)(CallbackTask *)) initCharacterSelectLimitedCourseList, 0, 0x63);
    gCurrentGameTask->timer = 0;
    D_800EC9C1 = 0;
    D_8010ADF8 = 0;
    D_80121D88 = 0;
    D_8010ADF0 = 0;
    D_800EC9D0 = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    var_v1 = 0;
    if (gPlayerCount > 0) {
        var_v0 = D_800EC9F0;
        temp_a0 = &D_800EC9F0[gPlayerCount];
        do {
            temp_v1 = var_v0->highestCourse;
            var_v0 += 1;
            if (gHighestUnlockedCourse < temp_v1) {
                gHighestUnlockedCourse = temp_v1;
            }
        } while (var_v0 < temp_a0);
        var_v1 = 0;
    }

    D_8010ADDC = 0;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    sp1C = var_v1;
    setCurrentGameTaskCallback(updateCharacterSelectCourseMenu, 0);
    updateCallbackTasks();
    var_v1 = sp1C;
    if (D_800EC9DD == 1) {
        if (D_80121B5E < 2) {
            gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectShortCourseOptions;
            if ((D_80121B50 != 9) && (D_80121B50 != 0) && (D_80121B50 != 1)) {
                D_80121B50 = 9;
            }
        } else {
            gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectSingleCourseOption;
            D_80121B50 = 7;
        }
    } else {
        gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectCourseOptionsByUnlock[gHighestUnlockedCourse];
        if (D_80121B50 == -1) {
            D_80121B50 = 9;
        }
    }

    var_v0_2 = *gCharacterSelectActiveCourseOptions;
loop_20:
    if (D_80121B50 == *var_v0_2) {
        D_80121B50 = var_v1;
    } else {
        var_v1 += 1;
        var_v0_2 += 1;
        if (var_v1 != 0xA) {
            goto loop_20;
        }
    }

    gCharacterSelectCourseExitOptionIndex = 0;
    var_v0_3 = 0;
    var_v1_2 = *gCharacterSelectActiveCourseOptions;
loop_24:
    var_v0_3 += 2;
    if (*var_v1_2 != -1) {
        var_v1_2 += 1;
        gCharacterSelectCourseExitOptionIndex += 1;
        if (var_v0_3 != 0x14) {
            goto loop_24;
        }
    }

    gCharacterSelectCourseCursorState.bytes[0] = 0;
    gCharacterSelectCourseCursorState.bytes[1] = 0;
    gCharacterSelectCourseCursorState.bytes[2] = 0;
    gCharacterSelectCourseCursorState.fields.spriteIndex = 0;
    gCharacterSelectCourseCursorState.bytes[6] = 0;
    gCharacterSelectCourseCursorState.bytes[7] = 0;
}
#endif

// initCharacterSelectCourseMenuFromRace best match: 84.441% (nonmatchings/initCharacterSelectCourseMenuFromRace-8207005055717715604/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_course_menu/initCharacterSelectCourseMenuFromRace.s")

#ifdef NON_MATCHING
void initCharacterSelectCourseMenuFromRace(void) {
    s32 sp2C;
    s16 *var_v0_2;
    s16 *var_v1_2;
    s32 var_v1;
    s32 var_v0_3;
    u8 temp_v1;
    CharacterSelectSaveData *var_v0;
    CharacterSelectSaveData *temp_a0;

    requestMusicSequenceBank(2);
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
    gFramebufferSwapDelay = 0;
    loadCompressedRomAsset(D_5A1ED0, D_5C5320, 0x21);
    loadCompressedRomAsset(D_593D10, D_598A70, 0x22);
    loadCompressedRomAsset(D_598A70, D_59AAA0, 0x23);
    loadCompressedRomAsset(D_59AAA0, D_59DFE0, 0x24);
    loadCompressedRomAsset(D_59DFE0, D_59E7F0, 0x26);
    loadCompressedRomAsset(D_245A80, D_24C8E0, 0x1F);
    initCallbackTaskScheduler(0);
    createCallbackTask((void (*)(CallbackTask *))func_8001710C, 0, 0x5E);

    if (D_800EC9C2 == 1) {
        loadCompressedRomAsset(D_5CCD40, D_5D4280, 0x25);
        createCallbackTask((void (*)(CallbackTask *)) initCharacterSelectLimitedCourseList, 0, 0x63);
        gCurrentGameTask->fade = 0;
    } else {
        gCurrentGameTask->fade = 0xFF;
    }

    gCurrentGameTask->timer = 0;
    D_800EC9C1 = 0;
    D_8010ADF8 = 0;
    D_801235B4 = 0;
    D_80121D88 = 0;
    D_8010ADF0 = 0;
    D_800EC9D0 = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    var_v1 = 0;
    if (gPlayerCount > 0) {
        var_v0 = D_800EC9F0;
        temp_a0 = &D_800EC9F0[gPlayerCount];
        do {
            temp_v1 = var_v0->highestCourse;
            var_v0 += 1;
            if (gHighestUnlockedCourse < temp_v1) {
                gHighestUnlockedCourse = temp_v1;
            }
        } while (var_v0 < temp_a0);
    }

    D_8010ADDC = 0;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    sp2C = var_v1;
    setCurrentGameTaskCallback(updateCharacterSelectCourseMenu, 0);
    var_v1 = sp2C;
    if (D_800EC9DD == 1) {
        if (D_80121B5E < 2) {
            gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectShortCourseOptions;
            if ((D_80121B50 != 9) && (D_80121B50 != 0) && (D_80121B50 != 1)) {
                D_80121B50 = 9;
            }
        } else {
            gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectSingleCourseOption;
            D_80121B50 = 7;
        }
    } else {
        gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectCourseOptionsByUnlock[gHighestUnlockedCourse];
        if (D_80121B50 == -1) {
            D_80121B50 = 9;
        }
    }

    var_v0_2 = *gCharacterSelectActiveCourseOptions;
loop_20:
    if (D_80121B50 == *var_v0_2) {
        D_80121B50 = var_v1;
    } else {
        var_v1 += 1;
        var_v0_2 += 1;
        if (var_v1 != 0xA) {
            goto loop_20;
        }
    }

    gCharacterSelectCourseExitOptionIndex = 0;
    var_v0_3 = 0;
    var_v1_2 = *gCharacterSelectActiveCourseOptions;
loop_24:
    var_v0_3 += 2;
    if (*var_v1_2 != -1) {
        var_v1_2 += 1;
        gCharacterSelectCourseExitOptionIndex += 1;
        if (var_v0_3 != 0x14) {
            goto loop_24;
        }
    }

    gCharacterSelectCourseCursorState.bytes[0] = 0;
    gCharacterSelectCourseCursorState.bytes[1] = 0;
    gCharacterSelectCourseCursorState.bytes[2] = 0;
    gCharacterSelectCourseCursorState.fields.spriteIndex = 0;
    gCharacterSelectCourseCursorState.bytes[6] = 0;
    gCharacterSelectCourseCursorState.bytes[7] = 0;
    updateCallbackTasks();
}
#endif

// initCharacterSelectCourseMenuFromPlayerSelect best match: 83.266% (nonmatchings/initCharacterSelectCourseMenuFromPlayerSelect-2127290767680699791/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_course_menu/initCharacterSelectCourseMenuFromPlayerSelect.s")

#ifdef NON_MATCHING
void initCharacterSelectCourseMenuFromPlayerSelect(void) {
    s32 sp2C;
    s16 *var_v0_2;
    s16 *var_v1_2;
    s32 var_v1;
    s32 var_v0_3;
    u8 temp_v1;
    CharacterSelectSaveData *var_v0;
    CharacterSelectSaveData *temp_a0;

    requestMusicSequenceBank(2);
    if (gPlayerCount >= 2) {
        resetAllViewports();
        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, D_800E09A4);
        gFramebufferSwapDelay = 0;
        loadCompressedRomAsset(D_5A1ED0, D_5C5320, 0x21);
        loadCompressedRomAsset(D_593D10, D_598A70, 0x22);
        loadCompressedRomAsset(D_598A70, D_59AAA0, 0x23);
        loadCompressedRomAsset(D_59AAA0, D_59DFE0, 0x24);
        loadCompressedRomAsset(D_59DFE0, D_59E7F0, 0x26);
        loadCompressedRomAsset(D_245A80, D_24C8E0, 0x1F);
        initCallbackTaskScheduler(0);
        createCallbackTask((void (*)(CallbackTask *))func_8001710C, 0, 0x5E);
        gCurrentGameTask->fade = 0xFF;
    } else {
        gCurrentGameTask->fade = 0;
        createCallbackTask(initCharacterSelectUnlockedCourseList, 0, 0x63);
    }

    gCurrentGameTask->timer = 0;
    D_800EC9C1 = 0;
    D_8010ADF8 = 0;
    D_801235B4 = 0;
    D_80121D88 = 0;
    D_8010ADF0 = 0;
    D_800EC9D0 = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    D_8010ADDC = 0;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    var_v1 = 0;
    if (gPlayerCount > 0) {
        var_v0 = D_800EC9F0;
        temp_a0 = &D_800EC9F0[gPlayerCount];
        do {
            temp_v1 = var_v0->highestCourse;
            var_v0 += 1;
            if (gHighestUnlockedCourse < temp_v1) {
                gHighestUnlockedCourse = temp_v1;
            }
        } while (var_v0 < temp_a0);
    }

    sp2C = var_v1;
    setCurrentGameTaskCallback(updateCharacterSelectCourseMenu, 0);
    var_v1 = sp2C;
    if (D_800EC9DD == 1) {
        if (D_80121B5E < 2) {
            gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectShortCourseOptions;
            if ((D_80121B50 != 9) && (D_80121B50 != 0) && (D_80121B50 != 1)) {
                D_80121B50 = 9;
            }
        } else {
            gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectSingleCourseOption;
            D_80121B50 = 7;
        }
    } else {
        gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectCourseOptionsByUnlock[gHighestUnlockedCourse];
        if (D_80121B50 == -1) {
            D_80121B50 = 9;
        }
    }

    var_v0_2 = *gCharacterSelectActiveCourseOptions;
loop_20:
    if (D_80121B50 == *var_v0_2) {
        D_80121B50 = var_v1;
    } else {
        var_v1 += 1;
        var_v0_2 += 1;
        if (var_v1 != 0xA) {
            goto loop_20;
        }
    }

    gCharacterSelectCourseExitOptionIndex = 0;
    var_v0_3 = 0;
    var_v1_2 = *gCharacterSelectActiveCourseOptions;
loop_24:
    var_v0_3 += 2;
    if (*var_v1_2 != -1) {
        var_v1_2 += 1;
        gCharacterSelectCourseExitOptionIndex += 1;
        if (var_v0_3 != 0x14) {
            goto loop_24;
        }
    }

    gCharacterSelectCourseCursorState.bytes[0] = 0;
    gCharacterSelectCourseCursorState.bytes[1] = 0;
    gCharacterSelectCourseCursorState.bytes[2] = 0;
    gCharacterSelectCourseCursorState.fields.spriteIndex = 0;
    gCharacterSelectCourseCursorState.bytes[6] = 0;
    gCharacterSelectCourseCursorState.bytes[7] = 0;
    updateCallbackTasks();
}
#endif

// updateCharacterSelectCourseMenu best match: 72.688% (nonmatchings/updateCharacterSelectCourseMenu-2785870559185086986/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_course_menu/updateCharacterSelectCourseMenu.s")

#ifdef NON_MATCHING
void updateCharacterSelectCourseMenu(void) {
    s32 temp_input;
    s32 pressed;
    s32 previousSelection;
    s32 heldInput;
    s32 selection;
    u16 repeatTimer;

    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) gCurrentGameTask->fade, 0x24, 0);
        if (gCurrentGameTask->fade == 0) {
            createCallbackTask(&initCharacterSelectUnlockedCourseList, 0, 0x63);
        }
    } else {
        if (D_80121D88 == 0) {
            if (D_800EC9C1 == 0) {
                if (gCharacterSelectCourseCursorState.fields.state == 1) {
                    selection = D_80121B50;
                    previousSelection = (s16) selection;
                    temp_input = gPlayerInputHeld;
                    pressed = temp_input & 0x10800;
                    if ((pressed == 0) && ((temp_input & 0x20400) == 0)) {
                        D_8010ADF0 = 0;
                    }

                    heldInput = gPlayerInputPressed;
                    repeatTimer = D_8010ADF0;
                    if ((heldInput & 0x10800) == 0) {
                        if (((pressed != 0) && (repeatTimer >= 0xB)) && ((repeatTimer % 3) == 0)) {
                            if (repeatTimer == 0) {
                                repeatTimer++;
                            }
                            if (selection > 0) {
                                D_80121B50 = selection - 1;
                                selection = D_80121B50;
                            }
                            D_8010ADF0 = repeatTimer;
                        } else if (((heldInput & 0x20400) != 0) ||
                                   ((((temp_input & 0x20400) != 0) && (repeatTimer >= 0xB)) &&
                                    ((repeatTimer % 3) == 0))) {
                            D_8010ADF0 = repeatTimer;
                            if (repeatTimer == 0) {
                                D_8010ADF0 = repeatTimer + 1;
                            }
                            if ((*gCharacterSelectActiveCourseOptions)[selection] != -1) {
                                D_80121B50 = selection + 1;
                                selection = D_80121B50;
                            }
                        } else {
                            D_8010ADF0 = repeatTimer;
                        }
                    } else {
                        if (repeatTimer == 0) {
                            repeatTimer++;
                        }
                        if (selection > 0) {
                            D_80121B50 = selection - 1;
                            selection = D_80121B50;
                        }
                        D_8010ADF0 = repeatTimer;
                    }

                    repeatTimer = D_8010ADF0;
                    if (repeatTimer != 0) {
                        repeatTimer++;
                        D_8010ADF0 = repeatTimer;
                        if (repeatTimer == 0xFFFF) {
                            D_8010ADF0 = 0xC;
                        }
                    }

                    if (previousSelection != selection) {
                        enqueueSoundEffect(0x19, 0x32);
                    }

                    temp_input = gPlayerInputPressed;
                    if (((temp_input & 0x1000) || (temp_input & 0x8000)) &&
                        (D_801235B4 == (gCharacterSelectCourseExitOptionIndex + 1))) {
                        enqueueSoundEffect(1, 0x32);
                        if ((*gCharacterSelectActiveCourseOptions)[D_80121B50] != -1) {
                            D_800EC9C1 = 1;
                            gCharacterSelectCourseCursorState.fields.state = 2;
                            gCharacterSelectCourseCursorState.fields.spriteIndex = 0x100;
                        } else {
                            gCharacterSelectCourseCursorState.fields.state = 2;
                            gCharacterSelectCourseCursorState.fields.spriteIndex = 0x100;
                            D_80121D88 = 7;
                            setCurrentGameTaskCallback(&handleCharacterSelectCourseSelection, 0);
                            requestMusicSequenceStop(8);
                        }
                    } else if ((temp_input & 0x4000) && (D_801235B4 == (gCharacterSelectCourseExitOptionIndex + 1))) {
                        enqueueSoundEffect(1, 0x32);
                        gCharacterSelectCourseCursorState.fields.state = 2;
                        gCharacterSelectCourseCursorState.fields.spriteIndex = 0x100;
                        D_80121D88 = 7;
                        setCurrentGameTaskCallback(&handleCharacterSelectCourseSelection, 0);
                        requestMusicSequenceStop(8);
                    }
                }
            } else {
                D_800EC9C1++;
            }

            if (D_800EC9C1 == 8) {
                if ((*gCharacterSelectActiveCourseOptions)[D_80121B50] == -1) {
                    D_80121D88 = 2;
                    setCurrentGameTaskCallback(&handleCharacterSelectCourseSelection, 0);
                    requestMusicSequenceStop(8);
                } else {
                    D_80121D88 = 1;
                }
            }
        }

        if (gCharacterSelectCourseSubmenuState >= 2) {
            setCurrentGameTaskCallback(&updateCharacterSelectCourseSubmenu, 0);
        }
    }

    D_801235B4 = 0;
    updateCallbackTasks();
}
#endif

void updateCharacterSelectCourseSubmenu(void) {
    s32 input;
    int state;

    state = D_80121D80[8];
    if (state < 3) {
        switch (gCharacterSelectCourseCursorState.fields.otherState) {
        case 2:
            input = gPlayerInputPressed;
            if (input & 0x4000) {
                D_80121D80[8] = 3;
                enqueueSoundEffect(1, 0x32);
            } else if ((input & 0x8000) || (input & 0x1000)) {
                enqueueSoundEffect(1, 0x32);
                D_800EC9D0 = 1;
                gCharacterSelectCourseCursorState.fields.otherState = 3;
                createCallbackTask(initCharacterSelectCourseConfirmCursor, 0, 0x61);
            }
            break;
        case 3:
            if (D_800EC9D0 >= 3) {
                input = gPlayerInputPressed;
                if ((input & 0x10800) && (D_800EC9D0 != 3)) {
                    D_800EC9D0--;
                    enqueueSoundEffect(0x19, 0x32);
                    input = gPlayerInputPressed;
                } else if ((input & 0x20400) && (D_800EC9D0 != 4)) {
                    D_800EC9D0++;
                    enqueueSoundEffect(0x19, 0x32);
                    input = gPlayerInputPressed;
                }

                if ((input & 0x8000) || (input & 0x1000)) {
                    enqueueSoundEffect(0x18, 0x32);
                    D_800EC9D0 += 2;
                    gCharacterSelectCourseCursorState.fields.otherState = 4;
                } else if (input & 0x4000) {
                    enqueueSoundEffect(1, 0x32);
                    D_800EC9D0 = 6;
                    gCharacterSelectCourseCursorState.fields.otherState = 4;
                }
            }
            break;
        case 4:
            if (gCharacterSelectCourseCursorState.fields.otherTimer == 4) {
                if (D_800EC9D0 == 6) {
                    D_80121D80[8] = 3;
                } else {
                    D_80121D80[8] = 7;
                }
                D_800EC9D0 = 0;
            }
            break;
        }
    } else {
        if (state == 6) {
            D_80121D80[8] = 0;
            D_800EC9C1 = 0;
            setCurrentGameTaskCallback(updateCharacterSelectCourseMenu, 0);
            gCharacterSelectCourseCursorState.fields.state = 1;
            gCharacterSelectCourseCursorState.fields.spriteIndex = 0x100;
            gCharacterSelectCourseCursorState.fields.timer = 0;
            state = D_80121D88;
        }
        if (state == 8) {
            setCurrentGameTaskCallback(fadeOutCharacterSelectCourseMenu, 0);
        }
    }
    updateCallbackTasks();
}

void handleCharacterSelectCourseSelection(void) {
    if (D_80121D88 == 8) {
        if (gPlayerCount >= 2) {
            setCurrentGameTaskCallback(&fadeOutCharacterSelectCourseMenu, 0);
            D_801235B4 = 1;
            D_8010ADF8 = 1;
        } else {
            if (D_800EC9DD == 0) {
                setCurrentGameTaskCallback(&func_80005540, 0);
            } else {
                setCurrentGameTaskCallback(&func_80008620, 0);
            }
            D_80121B50 = (*gCharacterSelectActiveCourseOptions)[D_80121B50];
            D_801235B4 = 0;
        }
    }
    updateCallbackTasks();
}

void fadeOutCharacterSelectCourseMenu(void) {
    if (gCurrentGameTask->fade != 0xFF) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) gCurrentGameTask->fade, 0x24, 1);
        if (gCurrentGameTask->fade == 0xFF) {
            gFramebufferSwapHold = 1;
        } else {
            updateCallbackTasks();
        }
    } else {
        if (gPendingFramebufferSwapCount == 2) {
            releaseMenuAssetHandles();
            gFramebufferSwapHold = 0;
            gFramebufferSwapDelay = 0;
            D_80121B50 = (*gCharacterSelectActiveCourseOptions)[D_80121B50];
            resumeGameTask(2);
            removeGameTask(4);
        }
    }
}
