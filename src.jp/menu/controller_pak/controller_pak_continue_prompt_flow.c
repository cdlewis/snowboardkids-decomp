#include "common.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/controller_input.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/controller_pak/controller_pak_continue_prompt_flow.h"
#include "game/menu/renderer/menu_renderer.h"

void func_8000C31C(void);

// The assembly processor requires each file pragma on one line.
// clang-format off
#pragma GLOBAL_ASM("asm.jp/nonmatchings/menu/controller_pak/controller_pak_continue_prompt_flow/initControllerPakContinuePromptFlow.s")
// clang-format on

void func_8000C1B8(void) {
    s32 temp_a1;
    s32 temp_v0;
    s32 var_v0;

    temp_v0 = gCurrentGameTask->callbackData0;
    if (temp_v0 != 0) {
        gCurrentGameTask->callbackData0 = stepMenuFadeAlpha((s32)(s16)temp_v0, 0x24, 0);
        if (gCurrentGameTask->callbackData0 == 0) {
            gControllerPakContinuePromptTransition.state = 1;
        }
    } else {
        var_v0 = gCurrentGameTask->callbackData1;
        temp_a1 = var_v0;
        if ((gPlayerInputPressed[0] & (STICK_UP | U_JPAD)) && (var_v0 != 0)) {
            gCurrentGameTask->callbackData1 = var_v0 - 1;
            var_v0 = gCurrentGameTask->callbackData1;
        } else if ((gPlayerInputPressed[0] & (STICK_DOWN | D_JPAD)) && (var_v0 != 1)) {
            gCurrentGameTask->callbackData1 = var_v0 + 1;
            var_v0 = gCurrentGameTask->callbackData1;
        }
        if (temp_a1 != var_v0) {
            enqueueSoundEffect(SFX_MENU_CURSOR, SFX_PRIORITY_NORMAL);
        }
        if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
            enqueueSoundEffect(SFX_CONFIRM, SFX_PRIORITY_NORMAL);
            gMenuFlowState = gCurrentGameTask->callbackData1;
            gControllerPakContinuePromptTransition.state = 2;
            gControllerPakContinuePromptTransition.x = 0x100;
            setCurrentGameTaskCallback(func_8000C31C, 0);
        }
    }
    updateCallbackTasks();
    /* Preserve the IDO global task pointer load strategy. */
    if (gCurrentGameTask) {}
}

#pragma GLOBAL_ASM("asm.jp/nonmatchings/menu/controller_pak/controller_pak_continue_prompt_flow/func_8000C31C.s")
