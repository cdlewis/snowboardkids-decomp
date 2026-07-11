#include "common.h"
#include "effect_task_scheduler.h"
#include "asset_manager.h"
#include "character_select_course_menu.h"
#include "controller_pak_race_record_save_flow.h"
#include "controller_pak_menu.h"
#include "input_task_scheduler.h"
#include "main_menu_score_ui.h"
#include "menu_rendering.h"
#include "viewport_manager.h"

typedef struct {
    /* 0x0 */ s8 step;
    /* 0x1 */ char pad1[0x1];
    /* 0x2 */ s16 timer;
    /* 0x4 */ s16 targetState;
    /* 0x6 */ s16 nextTimer;
} ControllerPakRaceRecordSaveUiState;

typedef struct {
    /* 0x0 */ char pad0[0x8];
    /* 0x8 */ s8 status;
    /* 0x9 */ char pad9[0x3];
    /* 0xC */ s32 selectedFileInfo;
} ControllerPakRaceRecordSaveFileContext;

extern CharacterSelectFlowState *gCurrentInputTask;
extern ControllerPakMenuState gControllerPakMenuState;
extern ControllerPakRaceRecordSaveUiState gControllerPakRaceRecordSaveUiState;
extern ControllerPakRaceRecordSaveFileContext D_80121D80;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern s8 gFramebufferSwapDelay;
extern u8 gControllerPakMenuCursorState;
extern s32 D_801235B4;
extern s32 gPlayerInputPressed;
extern s16 D_800EC9C8;
extern s16 D_800EC9D0;
extern u8 D_800EC9D8;
extern s8 D_800EC9C1;
extern s32 D_8010ADDC;
extern s32 D_8010ADE0;
extern s32 D_8010ADE4;
extern s32 D_8010ADE8;
extern s16 gMenuFadeAlpha;
extern s32 D_800EC9F4;
extern u8 D_593D10;
extern u8 D_598A70;
extern u8 D_59AAA0;
extern u8 D_59DFE0;
extern u8 D_60F1A0;
extern u8 D_60F990;

extern void releaseMenuAssetHandles(void);
extern s32 enqueueSoundEffect(s16, s16);

void initControllerPakRaceRecordSaveFlow(void) {
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    gFramebufferSwapDelay = 0;
    D_800EC9C8 = 0;
    D_800EC9D0 = 0;
    D_800EC9D8 = 0;
    D_80121D80.status = 0;
    D_800EC9C1 = 0;
    gCurrentInputTask->fade = 0xFF;
    D_8010ADDC = 0;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    D_8010ADE8 = 0;
    gMenuFadeAlpha = gCurrentInputTask->fade;
    D_800EC9F4 = D_80121D80.selectedFileInfo;
    loadCompressedRomAsset(&D_59AAA0, &D_59DFE0, 0x21);
    loadCompressedRomAsset(&D_59AAA0, &D_59DFE0, 0x24);
    loadCompressedRomAsset(&D_593D10, &D_598A70, 0x22);
    loadCompressedRomAsset(&D_598A70, &D_59AAA0, 0x23);
    loadCompressedRomAsset(&D_60F1A0, &D_60F990, 0x29);
    func_80070EC0(0);
    D_8010ADDC = createEffectTask(&func_8002BA00, 0, 0x61);
    D_8010ADE8 = createEffectTask(&func_8002C318, 0, 0x60);
    gControllerPakRaceRecordSaveUiState.step = 0;
    gControllerPakRaceRecordSaveUiState.timer = 0;
    gControllerPakRaceRecordSaveUiState.targetState = 0;
    gControllerPakRaceRecordSaveUiState.nextTimer = 0;
    gControllerPakMenuState.state = 0;
    gControllerPakMenuState.confirmChoice = 0;
    setCurrentInputTaskCallback(updateControllerPakRaceRecordSaveFlow, 0);
}

#ifdef NON_MATCHING
extern s32 D_800EC898;
extern s8 D_800EC8B4;
extern u8 D_800B31A5[];
extern void func_80000A40(u16);
extern void func_80000C48(u16);
extern void func_80000DB4(u16);
extern void func_800012CC(u16, s32, ControllerPakRaceRecordSaveUiState *, s16);
extern void func_80001538(u16);
extern void initControllerPakDeleteConfirmPrompt(EffectTask *);
#endif

// updateControllerPakRaceRecordSaveFlow best match: 95.751% (base_24.c)
#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_race_record_save_flow/updateControllerPakRaceRecordSaveFlow.s")

#ifdef NON_MATCHING
void updateControllerPakRaceRecordSaveFlow(void)
{
  s32 sp1C;
  int new_var;
  s32 sp24;
  s16 var_v0;
  s32 temp_t0;
  s32 temp_v0;
  s32 temp_v1;
  s32 var_t7;
  sp24 = 0;
  temp_v0 = gCurrentInputTask->fade;
  temp_t0 = D_8010ADE4;
  if (temp_v0 != 0)
  {
    gCurrentInputTask->fade = stepMenuFadeAlpha((s32) ((s16) temp_v0), 0x24, 0);
    if (gCurrentInputTask->fade == 0)
    {
      gControllerPakMenuState.state = 3;
      gControllerPakMenuState.confirmChoice = 1;
      createEffectTask(initControllerPakDeleteConfirmPrompt, 0, 0x64);
      setCurrentInputTaskCallback(updateControllerPakRaceRecordSaveConfirmPrompt, 0);
    }
  }
  else
    if (((u8) D_800EC9C1) == 0)
  {
    new_var = 3;
    if (((u8) gControllerPakRaceRecordSaveUiState.step) == 1)
    {
      if (D_800EC9D0 != 0)
      {
        var_v0 = 6;
      }
      else
      {
        var_v0 = D_800EC9C8;
      }
      switch (var_v0)
      {
        case 0:
          D_800EC8B4 = 0;
          func_80000A40(0);
          if (((D_800EC898 != 1) && (D_800EC898 != 0xB)) && (D_800EC898 != 4))
        {
          D_800EC8B4 = 1;
        }
        else
        {
          D_800EC8B4 = 0;
        }
          func_80000C48(0);
          if (D_800EC9C8 == 0x10)
        {
          D_800EC9C8 = 0x11;
        }
          break;

        case 1:
          func_80000DB4(0);
          if ((D_800EC9C8 == 9) || (D_800EC9C8 == 2))
        {
          D_800EC9C8 = 8;
          D_800EC9D0 = 1;
        }
          break;

        case 2:
          if (((u8) gControllerPakRaceRecordSaveUiState.step) != new_var)
        {
          sp1C = temp_t0;
          func_800012CC(0, new_var, &gControllerPakRaceRecordSaveUiState, D_800EC9D0);
          if (D_800EC9D8 == 0)
          {
            gControllerPakRaceRecordSaveUiState.targetState = 5;
            gControllerPakRaceRecordSaveUiState.step = new_var;
            gControllerPakRaceRecordSaveUiState.timer = 0x100;
          }
          else
            if (D_800EC9D8 == 3)
          {
            if (temp_t0 != 0)
            {
              gControllerPakRaceRecordSaveUiState.step = 3;
              gControllerPakRaceRecordSaveUiState.targetState = 0xD;
              gControllerPakRaceRecordSaveUiState.timer = 0x100;
            }
            else
            {
              D_800EC9C8 = 0xD;
            }
            D_800EC9D8 = 0;
          }
        }
          break;

        case 3:
          sp1C = temp_t0;
          func_80001538(0);
          if (D_800EC9D8 == 0)
        {
          if (temp_t0 != 0)
          {
            gControllerPakRaceRecordSaveUiState.step = 3;
            gControllerPakRaceRecordSaveUiState.targetState = 0x10;
          }
          else
          {
            D_800EC9C8 = 0x10;
          }
        }
        else
          if (D_800EC9D8 == 3)
        {
          if (temp_t0 != 0)
          {
            gControllerPakRaceRecordSaveUiState.step = new_var;
            gControllerPakRaceRecordSaveUiState.targetState = 0xE;
          }
          else
          {
            D_800EC9C8 = 0xE;
          }
          D_800EC9D8 = 0;
        }
          break;

        case 4:
          if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000))
        {
          sp1C = temp_t0;
          enqueueSoundEffect(1, 0x32);
          if (temp_t0 != 0)
          {
            gControllerPakRaceRecordSaveUiState.step = 1;
            gControllerPakRaceRecordSaveUiState.timer = 0x100;
            D_800EC9C8 = 0;
          }
          else
          {
            D_800EC9C8 = 0;
          }
        }
          break;

        case 5:
          if (((u8) D_80121D80.status) == 0)
        {
          D_80121D80.status = 1;
        }
          break;

        case 6:
          if ((D_800EC9D0 == 3) || (D_800EC9D0 == 4))
        {
          temp_v1 = gPlayerInputPressed;
          if (temp_v1 & 0x10800)
          {
            if (D_800EC9D0 != new_var)
            {
              D_800EC9D0 -= 1;
              enqueueSoundEffect(0x19, 0x32);
              temp_v1 = gPlayerInputPressed;
            }
          }
          var_t7 = temp_v1 & 0x8000;
          if (temp_v1 & 0x20400)
          {
            if (D_800EC9D0 != 4)
            {
              D_800EC9D0 += 1;
              enqueueSoundEffect(0x19, 0x32);
              temp_v1 = gPlayerInputPressed;
            }
            var_t7 = temp_v1 & 0x8000;
          }
          if ((var_t7 != 0) || (temp_v1 & 0x1000))
          {
            enqueueSoundEffect(1, 0x32);
            if (D_800EC9D0 == 4)
            {
              if (D_800EC9C8 == 8)
              {
                gControllerPakRaceRecordSaveUiState.nextTimer = 0xF;
              }
              else
                if (D_800EC9C8 == 7)
              {
                gControllerPakRaceRecordSaveUiState.nextTimer = 3;
              }
              else
                if (D_800EC9C8 == 0xF)
              {
                gControllerPakRaceRecordSaveUiState.nextTimer = 0;
              }
              else
              {
                gControllerPakRaceRecordSaveUiState.nextTimer = 4;
              }
            }
            else
              if (D_800EC9C8 == 8)
            {
              gControllerPakRaceRecordSaveUiState.nextTimer = 2;
            }
            else
              if ((D_800EC9C8 == 0xA) || (D_800EC9C8 == 0x11))
            {
              gControllerPakRaceRecordSaveUiState.nextTimer = 0xF;
            }
            else
              if (D_800EC9C8 == 7)
            {
              gControllerPakRaceRecordSaveUiState.nextTimer = 4;
            }
            else
            {
              gControllerPakRaceRecordSaveUiState.nextTimer = 5;
            }
            D_800EC9D0 += 2;
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
          if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000))
        {
          enqueueSoundEffect(1, 0x32);
          D_800EC9D0 = D_800B31A5[D_800EC9C8];
        }
          break;

        case 15:
          if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000))
        {
          enqueueSoundEffect(1, 0x32);
          D_800EC9D0 = D_800B31A5[D_800EC9C8];
        }
          break;

        case 14:

        case 16:
          if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000))
        {
          sp1C = temp_t0;
          enqueueSoundEffect(1, 0x32);
          if (D_800EC9C8 == 0xE)
          {
            if (temp_t0 != 0)
            {
              gControllerPakRaceRecordSaveUiState.step = 3;
              gControllerPakRaceRecordSaveUiState.targetState = 0x12;
            }
            else
            {
              D_800EC9C8 = 7;
            }
          }
          else
            if (temp_t0 != 0)
          {
            gControllerPakRaceRecordSaveUiState.step = 1;
            gControllerPakRaceRecordSaveUiState.timer = 0x100;
            D_800EC9C8 = 0;
          }
          else
          {
            D_800EC9C8 = 0;
          }
        }
          break;

        case 18:
          if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000))
        {
          enqueueSoundEffect(1, 0x32);
          gControllerPakRaceRecordSaveUiState.step = 3;
          gControllerPakRaceRecordSaveUiState.timer = 0x100;
          gControllerPakRaceRecordSaveUiState.targetState = 0;
        }
          break;

      }

    }
    sp24 = ((u8) D_80121D80.status) & 1;
  }
  else
  {
    D_800EC9C1 = ((u8) D_800EC9C1) + 1;
  }
  if (sp24 != 0)
  {
 D_800EC9C1 = 1; } if (((u8) D_800EC9C1) == 0x23) { setCurrentInputTaskCallback(fadeOutControllerPakRaceRecordSaveFlow, 0);
  }
  updateEffectTasks();
}
#endif

void fadeOutControllerPakRaceRecordSaveFlow(void) {
    s32 temp_v0 = gCurrentInputTask->fade;
    if (temp_v0 != 0xFF) {
        gCurrentInputTask->fade = stepMenuFadeAlpha((s16) temp_v0, 0x20, 1);
        updateEffectTasks();
        if (gCurrentInputTask->fade == 0xFF) {
            gFramebufferSwapHold = 1;
        }
    } else if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        resumeInputTask(2);
        removeInputTask(4);
    }
}

void updateControllerPakRaceRecordSaveConfirmPrompt(void) {
    if ((gPlayerInputPressed & 0x10800) && (gControllerPakMenuState.confirmChoice != 0)) {
        gControllerPakMenuState.confirmChoice = 0;
        enqueueSoundEffect(0x19, 0x32);
    } else if ((gPlayerInputPressed & 0x20400) && (gControllerPakMenuState.confirmChoice != 1)) {
        gControllerPakMenuState.confirmChoice = 1;
        enqueueSoundEffect(0x19, 0x32);
    }
    if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
        enqueueSoundEffect(0x18, 0x32);
        if (gControllerPakMenuState.confirmChoice == 0) {
            gControllerPakMenuCursorState = 0;
            setCurrentInputTaskCallback(updateControllerPakRaceRecordSaveFlow, 0);
        } else {
            D_801235B4 = 1;
            setCurrentInputTaskCallback(fadeOutControllerPakRaceRecordSaveFlow, 0);
        }
    } else if (gPlayerInputPressed & 0x4000) {
        enqueueSoundEffect(0x18, 0x32);
        D_801235B4 = 1;
        setCurrentInputTaskCallback(fadeOutControllerPakRaceRecordSaveFlow, 0);
    }
    updateEffectTasks();
}
