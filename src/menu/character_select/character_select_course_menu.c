#include "common.h"
#include "assets.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/menu/character_select/character_select_course_ui.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/race_type_select/race_type_select_menu.h"
#include "game/menu/splitscreen_select/race_splitscreen_select_menu.h"
#include "game/menu/race_setup/race_setup_ui.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"

typedef s16 CharacterSelectOptionList[10];

typedef struct {
    char pad0[0x4C];
    u8 highestCourse;
    char pad4D[0x78AB];
} CharacterSelectSaveData;

extern void enqueueSoundEffect(s32, s32);
extern void releaseMenuAssetHandles(void);
extern s16 gCharacterSelectShortCourseOptions[];
extern s16 gCharacterSelectSingleCourseOption[];
extern s16 gMenuFadeAlpha;
extern CharacterSelectSaveData gGameSaveDataBuffer[];
extern u8 gPlayerCount;
extern u8 gRaceTypeSelection;
extern s16 gRaceCourseIndex;
extern CharacterSelectOptionList *gCharacterSelectActiveCourseOptions;
extern s32 D_8010ADE0;
extern s32 D_8010ADE4;
extern s32 gMenuFlowState;
extern s32 gPlayerInputHeld;
extern s32 gPlayerInputPressed;
#ifndef NON_MATCHING
extern u16 gMenuInputRepeatTimers;
#else
extern volatile u16 gMenuInputRepeatTimers;
#endif
extern u16 gCharacterSelectCourseExitOptionIndex;
extern u8 gCharacterSelectCourseSubmenuState;
extern u8 gMenuExitSelection;
extern u8 gHighestUnlockedCourse;
extern u8 gMenuSelectionConfirmTimer;
extern u8 gRaceSplitscreenMode;
extern s16 gMenuChoicePromptState;
extern u8 gCourseSelectFromRaceTypeMenu;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

void initCharacterSelectCourseMenuFromRaceTypeSelect(void) {
    s32 var_v1;
    register s32 unused; /* Preserve IDO's spill layout. */
    s32 var_v0_3;
    u8 temp_v1;
    CharacterSelectSaveData *var_v0;

    gCurrentGameTask->fade = 0;
    requestMusicSequenceBank(2);
    createCallbackTask((CallbackTaskCallback) initCharacterSelectLimitedCourseList, 0, 0x63);
    gCurrentGameTask->timer = 0;
    gMenuSelectionConfirmTimer = 0;
    gMenuExitSelection = 0;
    gRacePlayers[0].menuState = 0;
    gMenuInputRepeatTimers = 0;
    gMenuChoicePromptState = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    var_v1 = 0;
    if (gPlayerCount > var_v1) {
        var_v0 = gGameSaveDataBuffer; do {
            temp_v1 = var_v0->highestCourse;
            var_v0 += 1;
            if (gHighestUnlockedCourse < temp_v1) {
                gHighestUnlockedCourse = temp_v1;
            }
        } while (var_v0 < &gGameSaveDataBuffer[gPlayerCount]);
        var_v1++;
        var_v1--;
    }

    var_v1 = 0;
    gActiveMenuTask = NULL;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    setCurrentGameTaskCallback(updateCharacterSelectCourseMenu, 0);
    updateCallbackTasks();

    {
    s16 *activeOptions;
    CharacterSelectCourseUnlockList *courseOptionsByUnlock;

    courseOptionsByUnlock = gCharacterSelectCourseOptionsByUnlock;
    if (gCourseSelectFromRaceTypeMenu == 1) {
        if (gRaceTypeSelection < 2) {
            gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectShortCourseOptions;
            if ((gRaceCourseIndex != 9) && (gRaceCourseIndex != 0) && (gRaceCourseIndex != 1)) {
                gRaceCourseIndex = 9;
            }
        } else {
            gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectSingleCourseOption;
            gRaceCourseIndex = 7;
        }
    } else {
        gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) courseOptionsByUnlock[gHighestUnlockedCourse];
        if (gRaceCourseIndex == -1) {
            gRaceCourseIndex = 9;
        }
    }

    activeOptions = *gCharacterSelectActiveCourseOptions;
    {
    s16 *var_v0_2;

    var_v0_2 = activeOptions;
loop_20:
    if (gRaceCourseIndex == *var_v0_2) {
        gRaceCourseIndex = var_v1;
    } else {
        var_v1 += 1;
        var_v0_2 += 1;
        if (var_v1 != 0xA) {
            goto loop_20;
        }
    }
    }

    gCharacterSelectCourseExitOptionIndex = 0;
    var_v0_3 = 0;
    {
    s16 *var_v1_2;

    var_v1_2 = activeOptions;
loop_24:
    var_v0_3 += 2;
    if (*var_v1_2 != -1) {
        var_v1_2 += 1;
        gCharacterSelectCourseExitOptionIndex += 1;
        if (var_v0_3 != 0x14) {
            goto loop_24;
        }
    }
    }

    gCharacterSelectCourseCursorState.bytes[0] = 0;
    gCharacterSelectCourseCursorState.bytes[1] = 0;
    gCharacterSelectCourseCursorState.bytes[2] = 0;
    gCharacterSelectCourseCursorState.fields.spriteIndex = 0;
    gCharacterSelectCourseCursorState.bytes[6] = 0;
    gCharacterSelectCourseCursorState.bytes[7] = 0;
    }
}

void initCharacterSelectCourseMenuFromRace(void)
{
  s32 sp2C;
  s16 *var_v0_2;
  s16 *var_v1_2;
  s16 *activeOptions;
  s32 var_v1;
  s32 var_v0_3;
  CharacterSelectCourseUnlockList *courseOptionsByUnlock;
  u8 temp_v1;
  CharacterSelectSaveData *var_v0;
  requestMusicSequenceBank(2);
  resetAllViewports();
  configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
  gFramebufferSwapDelay.value = 0;
  LOAD_ASSET(_5A1ED0, 0x21);
  LOAD_ASSET(_593D10, 0x22);
  LOAD_ASSET(_598A70, 0x23);
  LOAD_ASSET(_59AAA0, 0x24);
  LOAD_ASSET(_59DFE0, 0x26);
  LOAD_ASSET(_245A80, 0x1F);
  initCallbackTaskScheduler(0);
  createCallbackTask((CallbackTaskCallback) initMenuIconTilemapSpriteActor, 0, 0x5E);
  if (gRaceSplitscreenMode == 1)
  {
 LOAD_ASSET(_5CCD40, 0x25); createCallbackTask((CallbackTaskCallback) initCharacterSelectLimitedCourseList, 0, 0x63); gCurrentGameTask->fade = 0; } else { gCurrentGameTask->fade = 0xFF; } gCurrentGameTask->timer = 0; gMenuSelectionConfirmTimer = 0; gMenuExitSelection = 0; gMenuFlowState = 0; gRacePlayers[0].menuState = 0; gMenuInputRepeatTimers = 0; gMenuChoicePromptState = 0; gMenuFadeAlpha = gCurrentGameTask->fade; var_v1 = 0; if (gPlayerCount > var_v1) { var_v0 = gGameSaveDataBuffer; do { temp_v1 = var_v0->highestCourse; var_v0 += 1; if (gHighestUnlockedCourse < temp_v1) { gHighestUnlockedCourse = temp_v1; } } while (var_v0 < (&gGameSaveDataBuffer[gPlayerCount]));
    var_v1 *= 0;
  }
  gActiveMenuTask = 0;
  D_8010ADE0 = 0;
  D_8010ADE4 = 0;
  sp2C = var_v1;
  do { } while (0);
  setCurrentGameTaskCallback(updateCharacterSelectCourseMenu, 0);
  var_v1 = sp2C;
  courseOptionsByUnlock = gCharacterSelectCourseOptionsByUnlock;
  if (gCourseSelectFromRaceTypeMenu == 1)
  {
    if (gRaceTypeSelection < 2)
    {
      gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectShortCourseOptions;
      if (((gRaceCourseIndex != 9) && (gRaceCourseIndex != 0)) && (gRaceCourseIndex != 1))
      {
        gRaceCourseIndex = 9;
      }
    }
    else
    {
      gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectSingleCourseOption;
      gRaceCourseIndex = 7;
    }
  }
  else
  {
    gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) courseOptionsByUnlock[gHighestUnlockedCourse];
    if (gRaceCourseIndex == (-1))
    {
      gRaceCourseIndex = 9;
    }
  }
  activeOptions = *gCharacterSelectActiveCourseOptions;
  var_v0_2 = activeOptions;
  do
  {
    if (gRaceCourseIndex == (*var_v0_2))
    {
      gRaceCourseIndex = var_v1;
      break;
    }
    var_v1 += 1;
    var_v0_2 += 1;
  }
  while (var_v1 != 10);
  gCharacterSelectCourseExitOptionIndex = 0;
  var_v0_3 = 0;
  var_v1_2 = activeOptions;
  do
  {
    var_v0_3 += 2;
    if ((*var_v1_2) == (-1))
    {
      break;
    }
    var_v1_2 += 1;
    gCharacterSelectCourseExitOptionIndex += 1;
  }
  while (var_v0_3 != 20);
  gCharacterSelectCourseCursorState.bytes[0] = 0;
  gCharacterSelectCourseCursorState.bytes[1] = 0;
  gCharacterSelectCourseCursorState.bytes[2] = 0;
  gCharacterSelectCourseCursorState.fields.spriteIndex = 0;
  gCharacterSelectCourseCursorState.bytes[6] = 0;
  gCharacterSelectCourseCursorState.bytes[7] = 0;
  updateCallbackTasks();
}

void initCharacterSelectCourseMenuFromPlayerSelect(void) {
    s32 var_v1;
    s32 sp2C;
    s32 var_v0_3;
    s16 *var_v0_2;
    s16 *var_v1_2;
    s16 *activeOptions;
    CharacterSelectCourseUnlockList *courseOptionsByUnlock;
    u8 temp_v1;
    CharacterSelectSaveData *var_v0;

    requestMusicSequenceBank(2);
    if (gPlayerCount >= 2) {
        resetAllViewports();
        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
        gFramebufferSwapDelay.value = 0;
        if (gCharacterSelectShortCourseOptions) {
        }
        LOAD_ASSET(_5A1ED0, 0x21);
        LOAD_ASSET(_593D10, 0x22);
        LOAD_ASSET(_598A70, 0x23);
        LOAD_ASSET(_59AAA0, 0x24);
        LOAD_ASSET(_59DFE0, 0x26);
        LOAD_ASSET(_245A80, 0x1F);
        initCallbackTaskScheduler(0);
        createCallbackTask((CallbackTaskCallback)initMenuIconTilemapSpriteActor, 0, 0x5E);
        gCurrentGameTask->fade = 0xFF;
    } else {
        gCurrentGameTask->fade = 0;
        createCallbackTask((CallbackTaskCallback)initCharacterSelectUnlockedCourseList, 0, 0x63);
    }

    gCurrentGameTask->timer = 0;
    gMenuSelectionConfirmTimer = 0;
    gMenuExitSelection = 0;
    gMenuFlowState = 0;
    gRacePlayers[0].menuState = 0;
    gMenuInputRepeatTimers = 0;
    gMenuChoicePromptState = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    gActiveMenuTask = 0;
    sp2C = 0;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    var_v1 = sp2C;
    if (gPlayerCount > 0) { var_v0 = gGameSaveDataBuffer; do { temp_v1 = var_v0->highestCourse; var_v0 += 1; if (gHighestUnlockedCourse < temp_v1) { gHighestUnlockedCourse = temp_v1; } } while (var_v0 < &gGameSaveDataBuffer[gPlayerCount]); var_v1++; var_v1--; } setCurrentGameTaskCallback(updateCharacterSelectCourseMenu, 0); var_v1 = sp2C;
    courseOptionsByUnlock = gCharacterSelectCourseOptionsByUnlock;
    if (gCourseSelectFromRaceTypeMenu == 1) {
        if (gRaceTypeSelection < 2) {
            gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectShortCourseOptions;
            if ((gRaceCourseIndex != 9) && (gRaceCourseIndex != 0) && (gRaceCourseIndex != 1)) {
                gRaceCourseIndex = 9;
            }
        } else {
            gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectSingleCourseOption;
            gRaceCourseIndex = 7;
        }
    } else {
        gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) courseOptionsByUnlock[gHighestUnlockedCourse];
        if (gRaceCourseIndex == -1) {
            gRaceCourseIndex = 9;
        }
    }

    activeOptions = *gCharacterSelectActiveCourseOptions;
    var_v0_2 = activeOptions;
    do {
        if (gRaceCourseIndex == *var_v0_2) {
            gRaceCourseIndex = var_v1;
            break;
        }
        var_v1 += 1;
        var_v0_2 += 1;
    } while (var_v1 != 10);

    gCharacterSelectCourseExitOptionIndex = 0;
    var_v0_3 = 0;
    var_v1_2 = activeOptions;
    do {
        var_v0_3 += 2;
        if (*var_v1_2 == -1) {
            break;
        }
        var_v1_2 += 1;
        gCharacterSelectCourseExitOptionIndex += 1;
    } while (var_v0_3 != 20);

    gCharacterSelectCourseCursorState.bytes[0] = 0;
    gCharacterSelectCourseCursorState.bytes[1] = 0;
    gCharacterSelectCourseCursorState.bytes[2] = 0;
    gCharacterSelectCourseCursorState.fields.spriteIndex = 0;
    gCharacterSelectCourseCursorState.bytes[6] = 0;
    gCharacterSelectCourseCursorState.bytes[7] = 0 * 0;
    updateCallbackTasks();
}

// updateCharacterSelectCourseMenu best compliant match: 83.324% (nonmatchings/updateCharacterSelectCourseMenu-8498672362023432715/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/character_select/character_select_course_menu/updateCharacterSelectCourseMenu.s")

#ifdef NON_MATCHING
void updateCharacterSelectCourseMenu(void) {
    s32 upMask;
    s32 input;
    s32 cursorState;
    s32 pressedInput;
    s32 *pressedInputPtr;
    s32 upInput;
    s16 previousSelection;
    u16 repeatTimer;

    pressedInputPtr = &gPlayerInputPressed;
    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) gCurrentGameTask->fade, 0x24, 0);
        if (gCurrentGameTask->fade == 0) {
            createCallbackTask((CallbackTaskCallback)&initCharacterSelectUnlockedCourseList, 0, 0x63);
        }
    } else {
        if (gRacePlayers[0].menuState == 0) {
            if (gMenuSelectionConfirmTimer == 0) {
                if (gCharacterSelectCourseCursorState.fields.state == 1) {
                    previousSelection = gRaceCourseIndex;
                    input = gPlayerInputHeld;
                    upInput = input & (STICK_UP | U_JPAD);
                    if ((upInput == 0) && ((input & (STICK_DOWN | D_JPAD)) == 0)) {
                        gMenuInputRepeatTimers = 0;
                    }

                    pressedInput = *pressedInputPtr;
                    repeatTimer = gMenuInputRepeatTimers;
                    upMask = STICK_UP | U_JPAD;
                    if ((pressedInput & upMask) ||
                        ((upInput != 0) && ((s32) repeatTimer >= 0xB) && ((repeatTimer % 3) == 0))) {
                        if (repeatTimer == 0) {
                            repeatTimer += 1;
                        }
                        if (gRaceCourseIndex > 0) {
                            gRaceCourseIndex = gRaceCourseIndex - 1;
                        }
                        gMenuInputRepeatTimers = repeatTimer;
                    } else if ((pressedInput & (STICK_DOWN | D_JPAD)) ||
                               ((input & (STICK_DOWN | D_JPAD)) && ((s32) repeatTimer >= 0xB) &&
                                ((repeatTimer % 3) == 0))) {
                        gMenuInputRepeatTimers = repeatTimer;
                        if (repeatTimer == 0) {
                            gMenuInputRepeatTimers = repeatTimer + 1;
                        }
                        if ((*gCharacterSelectActiveCourseOptions)[gRaceCourseIndex] != -1) {
                            gRaceCourseIndex = gRaceCourseIndex + 1;
                        }
                    }

                    repeatTimer = gMenuInputRepeatTimers;
                    if (repeatTimer != 0) {
                        repeatTimer++;
                        gMenuInputRepeatTimers = repeatTimer;
                        if (repeatTimer == 0xFFFF) {
                            gMenuInputRepeatTimers = 0xC;
                        }
                    }

                    if (previousSelection != gRaceCourseIndex) {
                        enqueueSoundEffect(0x19, 0x32);
                    }

                    input = gPlayerInputPressed;
                    if (((input & START_BUTTON) || (input & A_BUTTON)) &&
                        (gMenuFlowState == (gCharacterSelectCourseExitOptionIndex + 1))) {
                        cursorState = 2;
                        enqueueSoundEffect(1, 0x32);
                        if ((*gCharacterSelectActiveCourseOptions)[gRaceCourseIndex] != -1) {
                            gMenuSelectionConfirmTimer = 1;
                            gCharacterSelectCourseCursorState.fields.state = cursorState;
                            gCharacterSelectCourseCursorState.fields.spriteIndex = 0x100;
                        } else {
                            gCharacterSelectCourseCursorState.fields.state = cursorState;
                            gCharacterSelectCourseCursorState.fields.spriteIndex = 0x100;
                            gRacePlayers[0].menuState = 7;
                            setCurrentGameTaskCallback(&handleCharacterSelectCourseSelection, 0);
                            requestMusicSequenceStop(8);
                        }
                    } else if ((input & B_BUTTON) &&
                               (gMenuFlowState == (gCharacterSelectCourseExitOptionIndex + 1))) {
                        enqueueSoundEffect(1, 0x32);
                        gCharacterSelectCourseCursorState.fields.state = 2;
                        gCharacterSelectCourseCursorState.fields.spriteIndex = 0x100;
                        gRacePlayers[0].menuState = 7;
                        setCurrentGameTaskCallback(&handleCharacterSelectCourseSelection, 0);
                        requestMusicSequenceStop(8);
                    }
                }
            } else {
                gMenuSelectionConfirmTimer++;
            }

            if (gMenuSelectionConfirmTimer == 8) {
                if ((*gCharacterSelectActiveCourseOptions)[gRaceCourseIndex] == -1) {
                    gRacePlayers[0].menuState = 2;
                    setCurrentGameTaskCallback(&handleCharacterSelectCourseSelection, 0);
                    requestMusicSequenceStop(8);
                } else {
                    gRacePlayers[0].menuState = 1;
                }
            }
        }

        if (gCharacterSelectCourseSubmenuState >= 2) {
            setCurrentGameTaskCallback(&updateCharacterSelectCourseSubmenu, 0);
        }
    }

    gMenuFlowState = 0;
    updateCallbackTasks();
}
#endif

void updateCharacterSelectCourseSubmenu(void) {
    u8 *menuState;
    s32 input;
    int state;

    state = gRacePlayers[0].menuState;
    menuState = &gRacePlayers[0].menuState;
    if (state < 3) {
        switch (gCharacterSelectCourseCursorState.fields.otherState) {
        case 2:
            input = gPlayerInputPressed;
            if (input & B_BUTTON) {
                gRacePlayers[0].menuState = 3;
                enqueueSoundEffect(1, 0x32);
            } else if ((input & A_BUTTON) || (input & START_BUTTON)) {
                enqueueSoundEffect(1, 0x32);
                gMenuChoicePromptState = 1;
                gCharacterSelectCourseCursorState.fields.otherState = 3;
                createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseConfirmCursor, 0, 0x61);
            }
            break;
        case 3:
            if (gMenuChoicePromptState >= 3) {
                input = gPlayerInputPressed;
                if ((input & (STICK_UP | U_JPAD)) && (gMenuChoicePromptState != 3)) {
                    gMenuChoicePromptState--;
                    enqueueSoundEffect(0x19, 0x32);
                    input = gPlayerInputPressed;
                } else if ((input & (STICK_DOWN | D_JPAD)) && (gMenuChoicePromptState != 4)) {
                    gMenuChoicePromptState++;
                    enqueueSoundEffect(0x19, 0x32);
                    input = gPlayerInputPressed;
                }

                if ((input & A_BUTTON) || (input & START_BUTTON)) {
                    enqueueSoundEffect(0x18, 0x32);
                    gMenuChoicePromptState += 2;
                    gCharacterSelectCourseCursorState.fields.otherState = 4;
                } else if (input & B_BUTTON) {
                    enqueueSoundEffect(1, 0x32);
                    gMenuChoicePromptState = 6;
                    gCharacterSelectCourseCursorState.fields.otherState = 4;
                }
            }
            break;
        case 4:
            if (gCharacterSelectCourseCursorState.fields.otherTimer == 4) {
                if (gMenuChoicePromptState == 6) {
                    gRacePlayers[0].menuState = 3;
                } else {
                    gRacePlayers[0].menuState = 7;
                }
                gMenuChoicePromptState = 0;
            }
            break;
        }
    } else {
        if (state == 6) {
            gRacePlayers[0].menuState = 0;
            gMenuSelectionConfirmTimer = 0;
            setCurrentGameTaskCallback(updateCharacterSelectCourseMenu, 0);
            gCharacterSelectCourseCursorState.fields.state = 1;
            gCharacterSelectCourseCursorState.fields.spriteIndex = 0x100;
            gCharacterSelectCourseCursorState.fields.timer = 0;
            state = *menuState;
        }
        if (state == 8) {
            setCurrentGameTaskCallback(fadeOutCharacterSelectCourseMenu, 0);
        }
    }
    updateCallbackTasks();
}

void handleCharacterSelectCourseSelection(void) {
    if (gRacePlayers[0].menuState == 8) {
        if (gPlayerCount >= 2) {
            setCurrentGameTaskCallback(&fadeOutCharacterSelectCourseMenu, 0);
            gMenuFlowState = 1;
            gMenuExitSelection = 1;
        } else {
            if (gCourseSelectFromRaceTypeMenu == 0) {
                setCurrentGameTaskCallback(&returnToRaceSplitscreenSelectMenu, 0);
            } else {
                setCurrentGameTaskCallback(&returnToRaceTypeSelectMenu, 0);
            }
            gRaceCourseIndex = (*gCharacterSelectActiveCourseOptions)[gRaceCourseIndex];
            gMenuFlowState = 0;
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
            gFramebufferSwapDelay.value = 0;
            gRaceCourseIndex = (*gCharacterSelectActiveCourseOptions)[gRaceCourseIndex];
            resumeGameTask(2);
            removeGameTask(4);
        }
    }
}
