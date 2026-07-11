#include "common.h"
#include "effect_task_scheduler.h"
#include "asset_manager.h"
#include "character_select_course_menu.h"
#include "controller_pak_delete_flow.h"
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
} ControllerPakDeleteFlow;

typedef struct {
    /* 0x0 */ char pad0[0x8];
    /* 0x8 */ s8 status;
    /* 0x9 */ char pad9[0x3];
    /* 0xC */ s32 selectedFileInfo;
} ControllerPakFileContext;

extern CharacterSelectFlowState *D_801235B8;
extern ControllerPakMenuState D_8010AF90;
extern ControllerPakDeleteFlow D_8010AF60;
extern ControllerPakFileContext D_80121D80;
extern u8 D_80123750;
extern u8 D_80123751;
extern s8 D_800DEED4;
extern u8 D_8010AF92;
extern u8 D_8010AF93;
extern s32 D_801235B4;
extern s32 D_80123778;
extern s16 D_800EC9C8;
extern s16 D_800EC9D0;
extern u8 D_800EC9D8;
extern s8 D_800EC9C1;
extern s32 D_8010ADDC;
extern s32 D_8010ADE0;
extern s32 D_8010ADE4;
extern s32 D_8010ADE8;
extern s16 D_800DEF14;
extern s32 D_800EC9F4;
extern u8 D_593D10;
extern u8 D_598A70;
extern u8 D_59AAA0;
extern u8 D_59DFE0;
extern u8 D_60F1A0;
extern u8 D_60F990;

extern void func_80045914(void);
extern s32 func_80072138(s16, s16);

void func_80008D60(void) {
    func_800704F0();
    func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    D_800DEED4 = 0;
    D_800EC9C8 = 0;
    D_800EC9D0 = 0;
    D_800EC9D8 = 0;
    D_80121D80.status = 0;
    D_800EC9C1 = 0;
    D_801235B8->fade = 0xFF;
    D_8010ADDC = 0;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    D_8010ADE8 = 0;
    D_800DEF14 = D_801235B8->fade;
    D_800EC9F4 = D_80121D80.selectedFileInfo;
    loadCompressedRomAsset(&D_59AAA0, &D_59DFE0, 0x21);
    loadCompressedRomAsset(&D_59AAA0, &D_59DFE0, 0x24);
    loadCompressedRomAsset(&D_593D10, &D_598A70, 0x22);
    loadCompressedRomAsset(&D_598A70, &D_59AAA0, 0x23);
    loadCompressedRomAsset(&D_60F1A0, &D_60F990, 0x29);
    func_80070EC0(0);
    D_8010ADDC = func_80071408(&func_8002BA00, 0, 0x61);
    D_8010ADE8 = func_80071408(&func_8002C318, 0, 0x60);
    D_8010AF60.step = 0;
    D_8010AF60.timer = 0;
    D_8010AF60.targetState = 0;
    D_8010AF60.nextTimer = 0;
    D_8010AF90.state = 0;
    D_8010AF90.confirmChoice = 0;
    func_8009956C(func_80008F2C, 0);
}

#ifdef NON_MATCHING
extern s32 D_800EC898;
extern s8 D_800EC8B4;
extern u8 D_800B31A5[];
extern void func_80000A40(u16);
extern void func_80000C48(u16);
extern void func_80000DB4(u16);
extern void func_800012CC(u16, s32, ControllerPakDeleteFlow *, s16);
extern void func_80001538(u16);
extern void func_800325D0(EffectTask *);
#endif

// func_80008F2C best match: 95.751% (base_24.c)
#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_delete_flow/func_80008F2C.s")

#ifdef NON_MATCHING
void func_80008F2C(void)
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
  temp_v0 = D_801235B8->fade;
  temp_t0 = D_8010ADE4;
  if (temp_v0 != 0)
  {
    D_801235B8->fade = func_80013F88((s32) ((s16) temp_v0), 0x24, 0);
    if (D_801235B8->fade == 0)
    {
      D_8010AF90.state = 3;
      D_8010AF90.confirmChoice = 1;
      func_80071408(func_800325D0, 0, 0x64);
      func_8009956C(func_80009690, 0);
    }
  }
  else
    if (((u8) D_800EC9C1) == 0)
  {
    new_var = 3;
    if (((u8) D_8010AF60.step) == 1)
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
          if (((u8) D_8010AF60.step) != new_var)
        {
          sp1C = temp_t0;
          func_800012CC(0, new_var, &D_8010AF60, D_800EC9D0);
          if (D_800EC9D8 == 0)
          {
            D_8010AF60.targetState = 5;
            D_8010AF60.step = new_var;
            D_8010AF60.timer = 0x100;
          }
          else
            if (D_800EC9D8 == 3)
          {
            if (temp_t0 != 0)
            {
              D_8010AF60.step = 3;
              D_8010AF60.targetState = 0xD;
              D_8010AF60.timer = 0x100;
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
            D_8010AF60.step = 3;
            D_8010AF60.targetState = 0x10;
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
            D_8010AF60.step = new_var;
            D_8010AF60.targetState = 0xE;
          }
          else
          {
            D_800EC9C8 = 0xE;
          }
          D_800EC9D8 = 0;
        }
          break;

        case 4:
          if ((D_80123778 & 0x8000) || (D_80123778 & 0x1000))
        {
          sp1C = temp_t0;
          func_80072138(1, 0x32);
          if (temp_t0 != 0)
          {
            D_8010AF60.step = 1;
            D_8010AF60.timer = 0x100;
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
          temp_v1 = D_80123778;
          if (temp_v1 & 0x10800)
          {
            if (D_800EC9D0 != new_var)
            {
              D_800EC9D0 -= 1;
              func_80072138(0x19, 0x32);
              temp_v1 = D_80123778;
            }
          }
          var_t7 = temp_v1 & 0x8000;
          if (temp_v1 & 0x20400)
          {
            if (D_800EC9D0 != 4)
            {
              D_800EC9D0 += 1;
              func_80072138(0x19, 0x32);
              temp_v1 = D_80123778;
            }
            var_t7 = temp_v1 & 0x8000;
          }
          if ((var_t7 != 0) || (temp_v1 & 0x1000))
          {
            func_80072138(1, 0x32);
            if (D_800EC9D0 == 4)
            {
              if (D_800EC9C8 == 8)
              {
                D_8010AF60.nextTimer = 0xF;
              }
              else
                if (D_800EC9C8 == 7)
              {
                D_8010AF60.nextTimer = 3;
              }
              else
                if (D_800EC9C8 == 0xF)
              {
                D_8010AF60.nextTimer = 0;
              }
              else
              {
                D_8010AF60.nextTimer = 4;
              }
            }
            else
              if (D_800EC9C8 == 8)
            {
              D_8010AF60.nextTimer = 2;
            }
            else
              if ((D_800EC9C8 == 0xA) || (D_800EC9C8 == 0x11))
            {
              D_8010AF60.nextTimer = 0xF;
            }
            else
              if (D_800EC9C8 == 7)
            {
              D_8010AF60.nextTimer = 4;
            }
            else
            {
              D_8010AF60.nextTimer = 5;
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
          if ((D_80123778 & 0x8000) || (D_80123778 & 0x1000))
        {
          func_80072138(1, 0x32);
          D_800EC9D0 = D_800B31A5[D_800EC9C8];
        }
          break;

        case 15:
          if ((D_80123778 & 0x8000) || (D_80123778 & 0x1000))
        {
          func_80072138(1, 0x32);
          D_800EC9D0 = D_800B31A5[D_800EC9C8];
        }
          break;

        case 14:

        case 16:
          if ((D_80123778 & 0x8000) || (D_80123778 & 0x1000))
        {
          sp1C = temp_t0;
          func_80072138(1, 0x32);
          if (D_800EC9C8 == 0xE)
          {
            if (temp_t0 != 0)
            {
              D_8010AF60.step = 3;
              D_8010AF60.targetState = 0x12;
            }
            else
            {
              D_800EC9C8 = 7;
            }
          }
          else
            if (temp_t0 != 0)
          {
            D_8010AF60.step = 1;
            D_8010AF60.timer = 0x100;
            D_800EC9C8 = 0;
          }
          else
          {
            D_800EC9C8 = 0;
          }
        }
          break;

        case 18:
          if ((D_80123778 & 0x8000) || (D_80123778 & 0x1000))
        {
          func_80072138(1, 0x32);
          D_8010AF60.step = 3;
          D_8010AF60.timer = 0x100;
          D_8010AF60.targetState = 0;
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
 D_800EC9C1 = 1; } if (((u8) D_800EC9C1) == 0x23) { func_8009956C(func_800095DC, 0);
  }
  func_8007105C();
}
#endif

void func_800095DC(void) {
    s32 temp_v0 = D_801235B8->fade;
    if (temp_v0 != 0xFF) {
        D_801235B8->fade = func_80013F88((s16) temp_v0, 0x20, 1);
        func_8007105C();
        if (D_801235B8->fade == 0xFF) {
            D_80123751 = 1;
        }
    } else if (D_80123750 == 2) {
        func_80045914();
        D_80123751 = 0;
        D_800DEED4 = 0;
        func_80099658(2);
        func_8009954C(4);
    }
}

void func_80009690(void) {
    if ((D_80123778 & 0x10800) && (D_8010AF90.confirmChoice != 0)) {
        D_8010AF90.confirmChoice = 0;
        func_80072138(0x19, 0x32);
    } else if ((D_80123778 & 0x20400) && (D_8010AF90.confirmChoice != 1)) {
        D_8010AF90.confirmChoice = 1;
        func_80072138(0x19, 0x32);
    }
    if ((D_80123778 & 0x8000) || (D_80123778 & 0x1000)) {
        func_80072138(0x18, 0x32);
        if (D_8010AF92 == 0) {
            D_8010AF93 = 0;
            func_8009956C(func_80008F2C, 0);
        } else {
            D_801235B4 = 1;
            func_8009956C(func_800095DC, 0);
        }
    } else if (D_80123778 & 0x4000) {
        func_80072138(0x18, 0x32);
        D_801235B4 = 1;
        func_8009956C(func_800095DC, 0);
    }
    func_8007105C();
}
