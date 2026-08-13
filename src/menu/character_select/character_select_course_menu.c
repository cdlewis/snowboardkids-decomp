#include "game/race/race_state.h"
#include "common.h"
#include "game/save_data.h"
#include "assets.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/menu/character_select/character_select_course_ui.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/race_type_select/race_type_select_menu.h"
#include "game/menu/splitscreen_select/race_splitscreen_select_menu.h"
#include "game/menu/race_setup/race_setup_ui.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"

typedef s16 CharacterSelectOptionList[10];

CharacterSelectCourseMenuData gCharacterSelectCourseMenuData = {
    {
     { 9, 0, 1, 2, 3, 4, -1, 0, 0, 0, 0 },
     { 9, 0, 1, 2, 3, 4, 5, -1, 0, 0, 0 },
     { 9, 0, 1, 2, 3, 4, 5, 6, -1, 0, 0 },
     },
    { 9, 0, 1, 2, 3, 4, 5, 6, 8, -1, 0 },
    { 9, 0, 1, -1 },
    { 7, -1, 0, 0, 0, 0, 0, 0 },
};

extern void enqueueSoundEffect(s32, s32);
extern void releaseMenuAssetHandles(void);
extern s16 gMenuFadeAlpha;
extern CharacterSelectOptionList *gCharacterSelectActiveCourseOptions;
extern s32 D_8010ADE0;
extern s32 D_8010ADE4;
extern s32 gMenuFlowState;
extern u8 gMenuExitSelection;
extern u8 gHighestUnlockedCourse;
extern u8 gCourseSelectFromRaceTypeMenu;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initCharacterSelectCourseMenuFromRaceTypeSelect(void) {
    s32 var_v1;
    register s32 unused; /* Preserve IDO's spill layout. */
    s32 var_v0_3;
    u8 temp_v1;
    GameSaveData *var_v0;

    gCurrentGameTask->fade = 0;
    requestMusicSequenceBank(2);
    createCallbackTask((CallbackTaskCallback) initCharacterSelectLimitedCourseList, 0, 0x63);
    gCurrentGameTask->timer = 0;
    gMenuSelectionConfirmTimer = 0;
    gMenuExitSelection = 0;
    gRacePlayers[0].menuState = 0;
    gMenuInputRepeatTimers[0] = 0;
    gMenuChoicePromptState[0] = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    var_v1 = 0;
    if (gPlayerCount > var_v1) {
        var_v0 = &gGameSaveDataBuffer[0]; do {
            temp_v1 = var_v0->progressionLevel;
            var_v0 += 1;
            if (gHighestUnlockedCourse < temp_v1) {
                gHighestUnlockedCourse = temp_v1;
            }
        } while (var_v0 < &gGameSaveDataBuffer[0] + gPlayerCount);
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
            if ((gRaceCourseIndex.signedValue != 9) && (gRaceCourseIndex.signedValue != 0) && (gRaceCourseIndex.signedValue != 1)) {
                gRaceCourseIndex.signedValue = 9;
            }
        } else {
            gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectSingleCourseOption;
            gRaceCourseIndex.signedValue = 7;
        }
    } else {
        gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) courseOptionsByUnlock[gHighestUnlockedCourse];
        if (gRaceCourseIndex.signedValue == -1) {
            gRaceCourseIndex.signedValue = 9;
        }
    }

    activeOptions = *gCharacterSelectActiveCourseOptions;
    {
    s16 *var_v0_2;

    var_v0_2 = activeOptions;
loop_20:
    if (gRaceCourseIndex.signedValue == *var_v0_2) {
        gRaceCourseIndex.signedValue = var_v1;
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

    gCharacterSelectCourseCursorState.fields.listCursorState = 0;
    gCharacterSelectCourseCursorState.fields.submenuState = 0;
    gCharacterSelectCourseCursorState.fields.previewFrameState = 0;
    gCharacterSelectCourseCursorState.fields.listCursorSpriteIndex = 0;
    gCharacterSelectCourseCursorState.fields.listCursorTimer = 0;
    gCharacterSelectCourseCursorState.fields.submenuTimer = 0;
    }
}
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
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
  GameSaveData *var_v0;
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
 LOAD_ASSET(_5CCD40, 0x25); createCallbackTask((CallbackTaskCallback) initCharacterSelectLimitedCourseList, 0, 0x63); gCurrentGameTask->fade = 0; } else { gCurrentGameTask->fade = 0xFF; } gCurrentGameTask->timer = 0; gMenuSelectionConfirmTimer = 0; gMenuExitSelection = 0; gMenuFlowState = 0; gRacePlayers[0].menuState = 0; gMenuInputRepeatTimers[0] = 0; gMenuChoicePromptState[0] = 0; gMenuFadeAlpha = gCurrentGameTask->fade; var_v1 = 0; if (gPlayerCount > var_v1) { var_v0 = &gGameSaveDataBuffer[0]; do { temp_v1 = var_v0->progressionLevel; var_v0 += 1; if (gHighestUnlockedCourse < temp_v1) { gHighestUnlockedCourse = temp_v1; } } while (var_v0 < (&gGameSaveDataBuffer[0] + gPlayerCount));
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
      if (((gRaceCourseIndex.signedValue != 9) && (gRaceCourseIndex.signedValue != 0)) && (gRaceCourseIndex.signedValue != 1))
      {
        gRaceCourseIndex.signedValue = 9;
      }
    }
    else
    {
      gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectSingleCourseOption;
      gRaceCourseIndex.signedValue = 7;
    }
  }
  else
  {
    gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) courseOptionsByUnlock[gHighestUnlockedCourse];
    if (gRaceCourseIndex.signedValue == (-1))
    {
      gRaceCourseIndex.signedValue = 9;
    }
  }
  activeOptions = *gCharacterSelectActiveCourseOptions;
  var_v0_2 = activeOptions;
  do
  {
    if (gRaceCourseIndex.signedValue == (*var_v0_2))
    {
      gRaceCourseIndex.signedValue = var_v1;
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
  gCharacterSelectCourseCursorState.fields.listCursorState = 0;
  gCharacterSelectCourseCursorState.fields.submenuState = 0;
  gCharacterSelectCourseCursorState.fields.previewFrameState = 0;
  gCharacterSelectCourseCursorState.fields.listCursorSpriteIndex = 0;
  gCharacterSelectCourseCursorState.fields.listCursorTimer = 0;
  gCharacterSelectCourseCursorState.fields.submenuTimer = 0;
  updateCallbackTasks();
}
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initCharacterSelectCourseMenuFromPlayerSelect(void) {
    s32 var_v1;
    s32 sp2C;
    s32 var_v0_3;
    s16 *var_v0_2;
    s16 *var_v1_2;
    s16 *activeOptions;
    CharacterSelectCourseUnlockList *courseOptionsByUnlock;
    u8 temp_v1;
    GameSaveData *var_v0;

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
    gMenuInputRepeatTimers[0] = 0;
    gMenuChoicePromptState[0] = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    gActiveMenuTask = 0;
    sp2C = 0;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    var_v1 = sp2C;
    if (gPlayerCount > 0) { var_v0 = &gGameSaveDataBuffer[0]; do { temp_v1 = var_v0->progressionLevel; var_v0 += 1; if (gHighestUnlockedCourse < temp_v1) { gHighestUnlockedCourse = temp_v1; } } while (var_v0 < &gGameSaveDataBuffer[0] + gPlayerCount); var_v1++; var_v1--; } setCurrentGameTaskCallback(updateCharacterSelectCourseMenu, 0); var_v1 = sp2C;
    courseOptionsByUnlock = gCharacterSelectCourseOptionsByUnlock;
    if (gCourseSelectFromRaceTypeMenu == 1) {
        if (gRaceTypeSelection < 2) {
            gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectShortCourseOptions;
            if ((gRaceCourseIndex.signedValue != 9) && (gRaceCourseIndex.signedValue != 0) && (gRaceCourseIndex.signedValue != 1)) {
                gRaceCourseIndex.signedValue = 9;
            }
        } else {
            gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) gCharacterSelectSingleCourseOption;
            gRaceCourseIndex.signedValue = 7;
        }
    } else {
        gCharacterSelectActiveCourseOptions = (CharacterSelectOptionList *) courseOptionsByUnlock[gHighestUnlockedCourse];
        if (gRaceCourseIndex.signedValue == -1) {
            gRaceCourseIndex.signedValue = 9;
        }
    }

    activeOptions = *gCharacterSelectActiveCourseOptions;
    var_v0_2 = activeOptions;
    do {
        if (gRaceCourseIndex.signedValue == *var_v0_2) {
            gRaceCourseIndex.signedValue = var_v1;
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

    gCharacterSelectCourseCursorState.fields.listCursorState = 0;
    gCharacterSelectCourseCursorState.fields.submenuState = 0;
    gCharacterSelectCourseCursorState.fields.previewFrameState = 0;
    gCharacterSelectCourseCursorState.fields.listCursorSpriteIndex = 0;
    gCharacterSelectCourseCursorState.fields.listCursorTimer = 0;
    gCharacterSelectCourseCursorState.fields.submenuTimer = 0 * 0;
    updateCallbackTasks();
}
// clang-format on

// updateCharacterSelectCourseMenu best match: 99.905%
// (nonmatchings/updateCharacterSelectCourseMenu-3327344942128263994/base_45.c)

// asm-processor requires this pragma to remain on one line.
// clang-format off
#pragma GLOBAL_ASM("asm/nonmatchings/menu/character_select/character_select_course_menu/updateCharacterSelectCourseMenu.s")
// clang-format on

#ifdef NON_MATCHING
void updateCharacterSelectCourseMenu(void) {
    s32 *pressedInputPtr;
    u16 *repeatTimerPtr;
    s32 input;
    s32 heldInput;
    s16 *selectionPtr;
    s32 pressedInput;
    s32 heldInputValue;
    s32 spriteIndex;
    s32 upInput;
    s16 previousSelection;
    u16 repeatTimer;
    s32 selection;

    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16)gCurrentGameTask->fade, 0x24, 0);
        if (gCurrentGameTask->fade == 0) {
            createCallbackTask((CallbackTaskCallback)&initCharacterSelectUnlockedCourseList, 0, 0x63);
        }
    } else {
        selectionPtr = &gRaceCourseIndex.signedValue;
        if (gRacePlayers[0].menuState == 0) {
            if (gMenuSelectionConfirmTimer == 0) {
                pressedInputPtr = gPlayerInputPressed;
                repeatTimerPtr = gMenuInputRepeatTimers;
                if (gCharacterSelectCourseCursorState.fields.listCursorState == 1) {
                    for (heldInput = gPlayerInputHeld[0], selection = *selectionPtr;;) {
                        heldInput &= -1;
                        heldInput &= -1;
                        heldInput &= -1;
                        heldInput &= -1;
                        previousSelection = (s16)selection;
                        selection |= 0;
                        selection |= 0;
                        selection |= 0;
                        selection |= 0;
                        selection |= 0;
                        selection |= 0;
                        selection |= 0;
                        selection |= 0;
                        input = heldInput;
 upInput = input & (STICK_UP | U_JPAD); if ((upInput == 0) && ((input & (STICK_DOWN | D_JPAD)) == 0)) { *repeatTimerPtr = 0; } pressedInput = *pressedInputPtr; break; } repeatTimer = *repeatTimerPtr; if ((pressedInput & (STICK_UP | U_JPAD)) || (((upInput != 0) && (((s32)repeatTimer) >= 0xB)) && ((repeatTimer % 3) == 0))) { if (!repeatTimer) { repeatTimer += 1; } if (selection > 0) { gRaceCourseIndex.signedValue = selection - 1; selection = gRaceCourseIndex.signedValue; } *repeatTimerPtr = repeatTimer; } else if ((pressedInput & (STICK_DOWN | D_JPAD)) || ((((*repeatTimerPtr = repeatTimer, input & (STICK_DOWN | D_JPAD))) && ((*repeatTimerPtr = (upInput = repeatTimer), ((s32)repeatTimer) >= 0xB))) && ((*repeatTimerPtr = repeatTimer, (repeatTimer % 3) == 0)))) { *repeatTimerPtr = repeatTimer; if (repeatTimer == 0) { *repeatTimerPtr = repeatTimer + 1; } if ((*gCharacterSelectActiveCourseOptions)[selection] != (-1)) { gRaceCourseIndex.signedValue = selection + 1;
                            selection = gRaceCourseIndex.signedValue;
                        }
                    }

                    repeatTimer = *repeatTimerPtr;
                    if (repeatTimer != 0) {
                        repeatTimer++;
                        *repeatTimerPtr = repeatTimer;
                        if (repeatTimer == 0xFFFF) {
                            *repeatTimerPtr = 0xC;
                        }
                    }

                    if (previousSelection != selection) {
                        enqueueSoundEffect(0x19, 0x32);
                    }

                    input = *pressedInputPtr;
                    spriteIndex = 0x100;
                    if ((((((((((input & START_BUTTON) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) &
                           0xFFFFu) &
                          0xFFFFu) ||
                         (input & A_BUTTON)) &&
                        (gMenuFlowState == (gCharacterSelectCourseExitOptionIndex + 1))) {
                        enqueueSoundEffect(1, 0x32);
                        if ((*gCharacterSelectActiveCourseOptions)[gRaceCourseIndex.signedValue] != -1) {
                            gMenuSelectionConfirmTimer = 1;
                            gCharacterSelectCourseCursorState.fields.listCursorState = 2;
                            gCharacterSelectCourseCursorState.fields.listCursorSpriteIndex = spriteIndex;
                        } else {
                            gCharacterSelectCourseCursorState.fields.listCursorState = 2;
                            gCharacterSelectCourseCursorState.fields.listCursorSpriteIndex = spriteIndex;
                            gRacePlayers[0].menuState = 7;
                            setCurrentGameTaskCallback(&handleCharacterSelectCourseSelection, 0);
                            requestMusicSequenceStop(8);
                        }
                    } else if ((input & B_BUTTON) && (gMenuFlowState == (gCharacterSelectCourseExitOptionIndex + 1))) {
                        enqueueSoundEffect(1, 0x32);
                        gCharacterSelectCourseCursorState.fields.listCursorState = 2;
                        gCharacterSelectCourseCursorState.fields.listCursorSpriteIndex = spriteIndex;
                        gRacePlayers[0].menuState = 7;
                        setCurrentGameTaskCallback(&handleCharacterSelectCourseSelection, 0);
                        requestMusicSequenceStop(8);
                    }
                }
            } else {
                gMenuSelectionConfirmTimer++;
            }

            if (gMenuSelectionConfirmTimer == 8) {
                if ((*gCharacterSelectActiveCourseOptions)[gRaceCourseIndex.signedValue] == -1) {
                    gRacePlayers[0].menuState = 2;
                    setCurrentGameTaskCallback(&handleCharacterSelectCourseSelection, 0);
                    requestMusicSequenceStop(8);
                } else {
                    gRacePlayers[0].menuState = 1;
                }
            }
        }

        if (gCharacterSelectCourseCursorState.fields.submenuState >= 2) {
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
        switch (gCharacterSelectCourseCursorState.fields.submenuState) {
            case 2:
                input = gPlayerInputPressed[0];
                if (input & B_BUTTON) {
                    gRacePlayers[0].menuState = 3;
                    enqueueSoundEffect(1, 0x32);
                } else if ((input & A_BUTTON) || (input & START_BUTTON)) {
                    enqueueSoundEffect(1, 0x32);
                    gMenuChoicePromptState[0] = 1;
                    gCharacterSelectCourseCursorState.fields.submenuState = 3;
                    createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseConfirmCursor, 0, 0x61);
                }
                break;
            case 3:
                if (gMenuChoicePromptState[0] >= 3) {
                    input = gPlayerInputPressed[0];
                    if ((input & (STICK_UP | U_JPAD)) && (gMenuChoicePromptState[0] != 3)) {
                        gMenuChoicePromptState[0]--;
                        enqueueSoundEffect(0x19, 0x32);
                        input = gPlayerInputPressed[0];
                    } else if ((input & (STICK_DOWN | D_JPAD)) && (gMenuChoicePromptState[0] != 4)) {
                        gMenuChoicePromptState[0]++;
                        enqueueSoundEffect(0x19, 0x32);
                        input = gPlayerInputPressed[0];
                    }

                    if ((input & A_BUTTON) || (input & START_BUTTON)) {
                        enqueueSoundEffect(0x18, 0x32);
                        gMenuChoicePromptState[0] += 2;
                        gCharacterSelectCourseCursorState.fields.submenuState = 4;
                    } else if (input & B_BUTTON) {
                        enqueueSoundEffect(1, 0x32);
                        gMenuChoicePromptState[0] = 6;
                        gCharacterSelectCourseCursorState.fields.submenuState = 4;
                    }
                }
                break;
            case 4:
                if (gCharacterSelectCourseCursorState.fields.submenuTimer == 4) {
                    if (gMenuChoicePromptState[0] == 6) {
                        gRacePlayers[0].menuState = 3;
                    } else {
                        gRacePlayers[0].menuState = 7;
                    }
                    gMenuChoicePromptState[0] = 0;
                }
                break;
        }
    } else {
        if (state == 6) {
            gRacePlayers[0].menuState = 0;
            gMenuSelectionConfirmTimer = 0;
            setCurrentGameTaskCallback(updateCharacterSelectCourseMenu, 0);
            gCharacterSelectCourseCursorState.fields.listCursorState = 1;
            gCharacterSelectCourseCursorState.fields.listCursorSpriteIndex = 0x100;
            gCharacterSelectCourseCursorState.fields.listCursorTimer = 0;
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
            gRaceCourseIndex.signedValue = (*gCharacterSelectActiveCourseOptions)[gRaceCourseIndex.signedValue];
            gMenuFlowState = 0;
        }
    }
    updateCallbackTasks();
}

void fadeOutCharacterSelectCourseMenu(void) {
    if (gCurrentGameTask->fade != 0xFF) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16)gCurrentGameTask->fade, 0x24, 1);
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
            gRaceCourseIndex.signedValue = (*gCharacterSelectActiveCourseOptions)[gRaceCourseIndex.signedValue];
            resumeGameTask(2);
            removeGameTask(4);
        }
    }
}
