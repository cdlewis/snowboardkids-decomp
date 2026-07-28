#include "common.h"
#include "assets.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/menu/controller_pak/controller_pak_race_record_save_flow.h"
#include "game/menu/controller_pak/controller_pak_menu.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/controller_pak/controller_pak_race_record_save_ui.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"

typedef struct {
    /* 0x0 */ s8 step;
    /* 0x1 */ char pad1[0x1];
    /* 0x2 */ s16 alpha;
    /* 0x4 */ s16 targetStatus;
    /* 0x6 */ s16 nextStatus;
} ControllerPakRaceRecordSaveStatusTransition;

extern CharacterSelectFlowState *gCurrentGameTask;
extern ControllerPakMenuState gControllerPakMenuState;
extern ControllerPakRaceRecordSaveStatusTransition gControllerPakRaceRecordSaveStatusTransition;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern s8 gFramebufferSwapDelay;
extern u8 gControllerPakMenuCursorState;
extern s32 gMenuFlowState;
extern s32 gPlayerInputPressed;
extern s16 gControllerPakStatusCodes;
extern s16 gMenuChoicePromptState;
extern u8 gControllerPakRetryCounts;
extern s8 gMenuSelectionConfirmTimer;
extern s32 D_8010ADE0;
extern s32 D_8010ADE4;
extern s16 gMenuFadeAlpha;
extern s32 D_800EC9F4;

extern void releaseMenuAssetHandles(void);
extern s32 enqueueSoundEffect(s16, s16);

void initControllerPakRaceRecordSaveFlow(void) {
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    gFramebufferSwapDelay = 0;
    gControllerPakStatusCodes = 0;
    gMenuChoicePromptState = 0;
    gControllerPakRetryCounts = 0;
    gRacePlayers[0].menuState = 0;
    gMenuSelectionConfirmTimer = 0;
    gCurrentGameTask->fade = 0xFF;
    gActiveMenuTask = 0;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    D_8010ADE8 = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    D_800EC9F4 = gRacePlayers[0].money;
    LOAD_ASSET(_59AAA0, 0x21);
    LOAD_ASSET(_59AAA0, 0x24);
    LOAD_ASSET(_593D10, 0x22);
    LOAD_ASSET(_598A70, 0x23);
    LOAD_ASSET(_60F1A0, 0x29);
    initCallbackTaskScheduler(0);
    gActiveMenuTask = createCallbackTask((CallbackTaskCallback)&initControllerPakRaceRecordSaveScorePanel, 0, 0x61);
    D_8010ADE8 = createCallbackTask((CallbackTaskCallback)&initControllerPakRaceRecordSaveStatusChoicePrompt, 0, 0x60);
    gControllerPakRaceRecordSaveStatusTransition.step = 0;
    gControllerPakRaceRecordSaveStatusTransition.alpha = 0;
    gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0;
    gControllerPakRaceRecordSaveStatusTransition.nextStatus = 0;
    gControllerPakMenuState.state = 0;
    gControllerPakMenuState.confirmChoice = 0;
    setCurrentGameTaskCallback(updateControllerPakRaceRecordSaveFlow, 0);
}

#ifdef NON_MATCHING
extern s32 gRumbleMotorStatuses;
extern s8 gRumblePakConnectedByController;
extern u8 gControllerPakRaceRecordSaveStatusChoicePromptStates[];
extern void requestRumbleMotorInit(u16);
extern void requestControllerPakProbe(u16);
extern void requestControllerPakSaveStatus(u16);
extern void requestControllerPakSaveWrite(u16, s32, ControllerPakRaceRecordSaveStatusTransition *, s16);
extern void requestControllerPakRepair(u16);
extern void initControllerPakDeleteConfirmPrompt(CallbackTask *);
#endif

// updateControllerPakRaceRecordSaveFlow best match: 97.524% (base_16.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/controller_pak/controller_pak_race_record_save_flow/updateControllerPakRaceRecordSaveFlow.s")

#ifdef NON_MATCHING
void updateControllerPakRaceRecordSaveFlow(void)
{
  s32 sp1C;
  u32 new_var;
  s32 sp24;
  s16 var_v0;
  s32 temp_t0;
  s32 temp_v0;
  s32 temp_v1;
  s32 var_t7;
  sp24 = 0;
  temp_v0 = gCurrentGameTask->fade;
  temp_t0 = D_8010ADE4;
  if (temp_v0 != 0)
  {
    gCurrentGameTask->fade = stepMenuFadeAlpha((s32) ((s16) temp_v0), 0x24, 0);
    if (gCurrentGameTask->fade == 0)
    {
      gControllerPakMenuState.state = 3;
      gControllerPakMenuState.confirmChoice = 1;
      createCallbackTask((CallbackTaskCallback)initControllerPakDeleteConfirmPrompt, 0, 0x64);
      setCurrentGameTaskCallback(updateControllerPakRaceRecordSaveOverwritePrompt, 0);
    }
  }
  else
    if (((u8) gMenuSelectionConfirmTimer) == 0)
  {
    new_var = 3;
    if (((u8) gControllerPakRaceRecordSaveStatusTransition.step) == 1)
    {
      if (gMenuChoicePromptState != 0)
      {
        var_v0 = 6;
      }
      else
      {
        var_v0 = gControllerPakStatusCodes;
      }
      switch (var_v0)
      {
        case 0:
          gRumblePakConnectedByController = 0;
          requestRumbleMotorInit(0);
          if (((gRumbleMotorStatuses != 1) && (gRumbleMotorStatuses != 0xB)) && (gRumbleMotorStatuses != 4))
        {
          gRumblePakConnectedByController = 1;
        }
        else
        {
          gRumblePakConnectedByController = 0;
        }
          requestControllerPakProbe(0);
          if (gControllerPakStatusCodes == 0x10)
        {
          gControllerPakStatusCodes = 0x11;
        }
          break;

        case 1:
          requestControllerPakSaveStatus(0);
          if ((gControllerPakStatusCodes == 9) || (gControllerPakStatusCodes == 2))
        {
          gControllerPakStatusCodes = 8;
          gMenuChoicePromptState = 1;
        }
          break;

        case 2:
          if (((u8) gControllerPakRaceRecordSaveStatusTransition.step) != 3)
        {
          sp1C = temp_t0;
          requestControllerPakSaveWrite(0, 3, &gControllerPakRaceRecordSaveStatusTransition, gMenuChoicePromptState);
          if (gControllerPakRetryCounts == 0)
          {
            gControllerPakRaceRecordSaveStatusTransition.targetStatus = 5;
            gControllerPakRaceRecordSaveStatusTransition.step = 3;
            gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
          }
          else
            if (gControllerPakRetryCounts == 3)
          {
            if (temp_t0 != 0)
            {
              gControllerPakRaceRecordSaveStatusTransition.step = 3;
              gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0xD;
              gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
            }
            else
            {
              gControllerPakStatusCodes = 0xD;
            }
            gControllerPakRetryCounts = 0;
          }
        }
          break;

        case 3:
          sp1C = temp_t0;
          requestControllerPakRepair(0);
          if (gControllerPakRetryCounts == 0)
        {
          if (temp_t0 != 0)
          {
            gControllerPakRaceRecordSaveStatusTransition.step = 3;
            gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0x10;
          }
          else
          {
            gControllerPakStatusCodes = 0x10;
          }
        }
        else
          if (gControllerPakRetryCounts == new_var)
        {
          if (temp_t0 != 0)
          {
            gControllerPakRaceRecordSaveStatusTransition.step = 3;
            gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0xE;
          }
          else
          {
            gControllerPakStatusCodes = 0xE;
          }
          gControllerPakRetryCounts = 0;
        }
          break;

        case 4:
          temp_v1 = gPlayerInputPressed;
          if ((temp_v1 & A_BUTTON) || (temp_v1 & START_BUTTON))
        {
          sp1C = temp_t0;
          enqueueSoundEffect(1, 0x32);
          if (temp_t0 != 0)
          {
            gControllerPakRaceRecordSaveStatusTransition.step = 1;
            gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
            gControllerPakStatusCodes = 0;
          }
          else
          {
            gControllerPakStatusCodes = 0;
          }
        }
          break;

        case 5:
          if (((u8) gRacePlayers[0].menuState) == 0)
        {
          gRacePlayers[0].menuState = 1;
        }
          break;

        case 6:
          if ((gMenuChoicePromptState == 3) || (gMenuChoicePromptState == 4))
        {
          temp_v1 = gPlayerInputPressed;
          if (temp_v1 & (STICK_UP | U_JPAD))
          {
            if (gMenuChoicePromptState != (new_var & 0xFFFFFFFFu))
            {
              gMenuChoicePromptState -= 1;
              enqueueSoundEffect(0x19, 0x32);
              temp_v1 = gPlayerInputPressed;
            }
          }
          var_t7 = temp_v1 & A_BUTTON;
          if (temp_v1 & (STICK_DOWN | D_JPAD))
          {
            if (gMenuChoicePromptState != 4)
            {
              gMenuChoicePromptState += 1;
              enqueueSoundEffect(0x19, 0x32);
              temp_v1 = gPlayerInputPressed;
            }
            var_t7 = temp_v1 & A_BUTTON;
          }
          if ((var_t7 != 0) || (temp_v1 & START_BUTTON))
          {
            enqueueSoundEffect(1, 0x32);
            if (gMenuChoicePromptState == 4)
            {
              if (gControllerPakStatusCodes == 8)
              {
                gControllerPakRaceRecordSaveStatusTransition.nextStatus = 0xF;
              }
              else
                if (gControllerPakStatusCodes == 7)
              {
                gControllerPakRaceRecordSaveStatusTransition.nextStatus = 3;
              }
              else
                if (gControllerPakStatusCodes == 0xF)
              {
                gControllerPakRaceRecordSaveStatusTransition.nextStatus = 0;
              }
              else
              {
                gControllerPakRaceRecordSaveStatusTransition.nextStatus = 4;
              }
            }
            else
              if (gControllerPakStatusCodes == 8)
            {
              gControllerPakRaceRecordSaveStatusTransition.nextStatus = 2;
            }
            else
              if ((gControllerPakStatusCodes == 0xA) || (gControllerPakStatusCodes == 0x11))
            {
              gControllerPakRaceRecordSaveStatusTransition.nextStatus = 0xF;
            }
            else
              if (gControllerPakStatusCodes == 7)
            {
              gControllerPakRaceRecordSaveStatusTransition.nextStatus = 4;
            }
            else
            {
              gControllerPakRaceRecordSaveStatusTransition.nextStatus = 5;
            }
            gMenuChoicePromptState += 2;
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

        case 17:
          temp_v1 = gPlayerInputPressed;
          if ((temp_v1 & A_BUTTON) || (temp_v1 & START_BUTTON))
        {
          enqueueSoundEffect(1, 0x32);
          gMenuChoicePromptState = gControllerPakRaceRecordSaveStatusChoicePromptStates[(long) gControllerPakStatusCodes];
        }
          break;

        case 15:
          temp_v1 = gPlayerInputPressed;
          if ((temp_v1 & A_BUTTON) || (temp_v1 & START_BUTTON))
        {
          enqueueSoundEffect(1, 0x32);
          gMenuChoicePromptState = gControllerPakRaceRecordSaveStatusChoicePromptStates[gControllerPakStatusCodes];
        }
          break;

        case 14:

        case 16:
          temp_v1 = gPlayerInputPressed;
          if ((temp_v1 & A_BUTTON) || (temp_v1 & START_BUTTON))
        {
          sp1C = temp_t0;
          enqueueSoundEffect(1, 0x32);
          if (gControllerPakStatusCodes == 0xE)
          {
            if (temp_t0 != 0)
            {
              gControllerPakRaceRecordSaveStatusTransition.step = 3;
              gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0x12;
            }
            else
            {
              gControllerPakStatusCodes = 7;
            }
          }
          else
            if (temp_t0 != 0)
          {
            gControllerPakRaceRecordSaveStatusTransition.step = 1;
            gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
            gControllerPakStatusCodes = 0;
          }
          else
          {
            gControllerPakStatusCodes = 0;
          }
        }
          break;

        case 18:
          temp_v1 = gPlayerInputPressed;
          if ((temp_v1 & A_BUTTON) || (temp_v1 & START_BUTTON))
        {
          enqueueSoundEffect(1, 0x32);
          gControllerPakRaceRecordSaveStatusTransition.step = 3;
          gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
          gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0;
        }
          break;

      }

    }
    sp24 = ((u8) gRacePlayers[0].menuState) & 1;
  }
  else
  {
    gMenuSelectionConfirmTimer = ((u8) gMenuSelectionConfirmTimer) + 1;
  }
  if (sp24 != 0)
  {
 gMenuSelectionConfirmTimer = 1; } if (((u8) gMenuSelectionConfirmTimer) == 0x23) { setCurrentGameTaskCallback(fadeOutControllerPakRaceRecordSaveFlow, 0);
  }
  updateCallbackTasks();
}
#endif

void fadeOutControllerPakRaceRecordSaveFlow(void) {
    s32 temp_v0 = gCurrentGameTask->fade;
    if (temp_v0 != 0xFF) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) temp_v0, 0x20, 1);
        updateCallbackTasks();
        if (gCurrentGameTask->fade == 0xFF) {
            gFramebufferSwapHold = 1;
        }
    } else if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        resumeGameTask(2);
        removeGameTask(4);
    }
}

void updateControllerPakRaceRecordSaveOverwritePrompt(void) {
    if ((gPlayerInputPressed & (STICK_UP | U_JPAD)) && (gControllerPakMenuState.confirmChoice != 0)) {
        gControllerPakMenuState.confirmChoice = 0;
        enqueueSoundEffect(0x19, 0x32);
    } else if ((gPlayerInputPressed & (STICK_DOWN | D_JPAD)) && (gControllerPakMenuState.confirmChoice != 1)) {
        gControllerPakMenuState.confirmChoice = 1;
        enqueueSoundEffect(0x19, 0x32);
    }
    if ((gPlayerInputPressed & A_BUTTON) || (gPlayerInputPressed & START_BUTTON)) {
        enqueueSoundEffect(0x18, 0x32);
        if (gControllerPakMenuState.confirmChoice == 0) {
            gControllerPakMenuCursorState = 0;
            setCurrentGameTaskCallback(updateControllerPakRaceRecordSaveFlow, 0);
        } else {
            gMenuFlowState = 1;
            setCurrentGameTaskCallback(fadeOutControllerPakRaceRecordSaveFlow, 0);
        }
    } else if (gPlayerInputPressed & B_BUTTON) {
        enqueueSoundEffect(0x18, 0x32);
        gMenuFlowState = 1;
        setCurrentGameTaskCallback(fadeOutControllerPakRaceRecordSaveFlow, 0);
    }
    updateCallbackTasks();
}
