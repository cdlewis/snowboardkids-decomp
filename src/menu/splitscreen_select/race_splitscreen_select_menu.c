#include "common.h"
#include "assets.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/splitscreen_select/race_splitscreen_select_menu.h"
#include "game/menu/splitscreen_select/race_splitscreen_select_ui.h"
#include "game/menu/race_setup/race_setup_ui.h"
#include "game/engine/viewport_manager.h"

typedef struct {
    char pad[0x18];
    s32 fade;
    s32 timer;
} RaceSplitscreenSelectMenuState;

typedef struct {
    u8 state;
    char pad1[1];
    s16 nextState;
    s16 portraitAlpha;
} RaceSplitscreenSelectCursorTarget;

extern void releaseMenuAssetHandles(void);
extern s32 enqueueSoundEffect(s16, s16);

extern u8 D_800EC9DC;

extern RaceSplitscreenSelectMenuState *gCurrentGameTask;
extern RaceSplitscreenSelectCursorTarget gRaceSplitscreenSelectCursorTarget;
extern u8 gMenuSelectionConfirmTimer;
#ifdef NON_MATCHING
extern volatile char gRaceSplitscreenMode;
#else
extern u8 gRaceSplitscreenMode;
#endif
extern u8 gCourseSelectFromRaceTypeMenu;
extern u8 gMenuTransitionState;
extern s32 gMenuFlowState;
extern u8 gMenuExitSelection;
extern u16 gMenuInputRepeatTimers;
extern s16 gMenuFadeAlpha;
extern s8 gFramebufferSwapDelay;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern u8 D_80121D85;
extern u8 D_80121D88;
extern s32 gPlayerInputHeld;
extern s32 gPlayerInputPressed;

void returnToRaceSplitscreenSelectMenu(void) {
    gCurrentGameTask->fade = 1;
    requestMusicSequenceBank(1);
    gMenuSelectionConfirmTimer = 0;
    gMenuTransitionState = 0;
    gActiveMenuTask = 0;
    gCurrentGameTask->timer = 0;
    gMenuFlowState = 0;
    gMenuExitSelection = 0;
    gCourseSelectFromRaceTypeMenu = 0;
    gMenuInputRepeatTimers = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    setCurrentGameTaskCallback(updateRaceSplitscreenSelectMenu, 0);
    gRaceSplitscreenSelectCursorTarget.state = 0;
    gRaceSplitscreenSelectCursorTarget.nextState = 0;
    updateCallbackTasks();
}

void initRaceSplitscreenSelectMenu(void) {
    requestMusicSequenceBank(1);
    D_800EC9DC = 0;
    gCourseSelectFromRaceTypeMenu = 0;
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
    gFramebufferSwapDelay = 0;
    gCurrentGameTask->fade = 0xFF;
    LOAD_ASSET(_5A1ED0, 0x21);
    LOAD_ASSET(_593D10, 0x22);
    LOAD_ASSET(_598A70, 0x23);
    LOAD_ASSET(_59DFE0, 0x26);
    LOAD_ASSET(_245A80, 0x1F);
    LOAD_ASSET(_59AAA0, 0x24);
    initCallbackTaskScheduler(0);
    createCallbackTask((CallbackTaskCallback)initMenuIconTilemapSpriteActor, 0, 0x5E);
    gMenuSelectionConfirmTimer = 0;
    gMenuTransitionState = 0;
    gActiveMenuTask = 0;
    gCurrentGameTask->timer = 0;
    gMenuExitSelection = 0;
    gMenuInputRepeatTimers = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    setCurrentGameTaskCallback(updateRaceSplitscreenSelectMenu, 0);
    gRaceSplitscreenSelectCursorTarget.state = 0;
    gRaceSplitscreenSelectCursorTarget.nextState = 0;
    updateCallbackTasks();
}

// updateRaceSplitscreenSelectMenu best match: 99.668% (nonmatchings/updateRaceSplitscreenSelectMenu-12/output-75-1/source.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/splitscreen_select/race_splitscreen_select_menu/updateRaceSplitscreenSelectMenu.s")

#ifdef NON_MATCHING
void updateRaceSplitscreenSelectMenu(void)
{
  s32 newInput;
  s32 heldInput;
  s32 pressedUp;
  s32 selection;
  s32 previousSelection;
  s32 pressedUpCopy;
  s32 repeatTimerCopy;
  s32 repeatTimer;
  u8 waitTimer;
  if (gCurrentGameTask->fade != 0)
  {
    gCurrentGameTask->fade = stepMenuFadeAlpha((s16) gCurrentGameTask->fade, 0x24, 0);
    if (gCurrentGameTask->fade == 0)
    {
      createCallbackTask((0, initRaceSplitscreenSelectPlayerCountIcons), 0, 0x63);
    }
  }
  else
  {
    if ((gRaceSplitscreenSelectCursorTarget.portraitAlpha == 0x100) && (D_80121D88 == 0))
    {
      if (gMenuSelectionConfirmTimer == 0)
      {
        if (gRaceSplitscreenSelectCursorTarget.state == 1)
        {
          selection = gRaceSplitscreenMode;
          newInput = gPlayerInputHeld;
          pressedUp = newInput & 0x10800;
          previousSelection = selection;
          if ((pressedUp == 0) && (!(newInput & 0x20400)))
          {
            gMenuInputRepeatTimers = 0;
          }
          heldInput = gPlayerInputPressed;
          pressedUpCopy = pressedUp;
          if ((heldInput & 0x10800) || (((pressedUpCopy != 0) && (((s32) gMenuInputRepeatTimers) >= 0xB)) && ((((s32) gMenuInputRepeatTimers) % 3) == 0)))
          {
            repeatTimer = gMenuInputRepeatTimers;
            if (repeatTimer == 0)
            {
              gMenuInputRepeatTimers = repeatTimer + 1;
              repeatTimer = gMenuInputRepeatTimers;
            }
            if (selection > 0)
            {
              gRaceSplitscreenMode = selection - 1;
              selection = (u8) (selection - 1);
            }
          }
          else
          {
            repeatTimer = gMenuInputRepeatTimers;
            if ((heldInput & 0x20400) || (((newInput & 0x20400) && ((repeatTimerCopy = repeatTimer & 0xFFFFFFFFFFFFFFFF) >= 0xB)) && ((repeatTimerCopy % 3) == 0)))
            {
              if (repeatTimer == 0)
              {
                gMenuInputRepeatTimers = repeatTimer + 1;
                repeatTimer = gMenuInputRepeatTimers;
              }
              if (selection < 4)
              {
                gRaceSplitscreenMode = selection + 1;
                selection = (u8) (selection + 1);
              }
            }
          }
          newInput = selection;
          pressedUp = (newInput & 0xFFFFFFFF) & 0xFFFFFFFF;
          if (repeatTimer != 0)
          {
            gMenuInputRepeatTimers = repeatTimer - (-1);
            if (gMenuInputRepeatTimers == 0xFFFF)
            {
              gMenuInputRepeatTimers = 0xC;
            }
          }
          if (pressedUp != previousSelection)
          {
            enqueueSoundEffect(0x19, 0x32);
            heldInput = gPlayerInputPressed;
          }
          if ((heldInput & 0x1000) || ((heldInput & 0x8000) && (gMenuFlowState == 5)))
          {
            if ((gRaceSplitscreenMode == 3) && (D_80121D85 == 5))
            {
              enqueueSoundEffect(0x46, 0x32);
            }
            else
            {
              enqueueSoundEffect(0x18, 0x32);
              gMenuSelectionConfirmTimer = 1;
              gRaceSplitscreenSelectCursorTarget.state = 2;
              gRaceSplitscreenSelectCursorTarget.nextState = 0x100;
              gMenuExitSelection = 0;
            }
          }
        }
      }
      else
      {
        gMenuSelectionConfirmTimer++;
      }
    }
    waitTimer = gMenuSelectionConfirmTimer;
    if (((gPlayerInputPressed & 0x4000) && (gMenuFlowState == 5)) && (waitTimer == 0))
    {
      enqueueSoundEffect(1, 0x32);
      gMenuSelectionConfirmTimer = 1;
      gRaceSplitscreenSelectCursorTarget.state = 2;
      gRaceSplitscreenSelectCursorTarget.nextState = 0x100;
      gMenuExitSelection = 1;
    }
    waitTimer = gMenuSelectionConfirmTimer;
    if (waitTimer == 7)
    {
      D_80121D88 = 1;
      gMenuSelectionConfirmTimer++;
    }
    if (D_80121D88 == 2)
    {
      setCurrentGameTaskCallback(handleRaceSplitscreenSelectMenuSelection, 0);
      pressedUpCopy = pressedUp;
      if (gMenuExitSelection == 0)
      {
        requestMusicSequenceStop(4);
      }
    }
  }
  gMenuFlowState = 0;
  updateCallbackTasks();
}
#endif

void handleRaceSplitscreenSelectMenuSelection(void) {
    u8 v0;

    v0 = gRaceSplitscreenMode;
    if ((v0 == 0 || v0 == 2) && gMenuExitSelection == 0) {
        setCurrentGameTaskCallback(initCharacterSelectCourseMenuFromPlayerSelect, 0);
        gMenuFlowState = 0;
    } else {
        setCurrentGameTaskCallback(fadeOutRaceSplitscreenSelectMenu, 0);
    }
    updateCallbackTasks();
}

void fadeOutRaceSplitscreenSelectMenu(void) {
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
            if (gMenuExitSelection == 1) {
                gMenuFlowState = 1;
            } else {
                gMenuFlowState = 0;
            }
            resumeGameTask(2);
            removeGameTask(4);
        }
    }
}
