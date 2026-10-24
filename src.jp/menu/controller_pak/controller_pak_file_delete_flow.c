#include "common.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/controller_input.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/controller_pak/controller_pak_menu.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"

void func_8000C6FC(void);
void func_8000CB0C(void);

// The assembly processor requires each file pragma on one line.
// clang-format off
#pragma GLOBAL_ASM("asm.jp/nonmatchings/menu/controller_pak/controller_pak_file_delete_flow/initControllerPakFileDeleteFlow.s")
// clang-format on

void func_8000C5F0(void) {
    if ((gPlayerInputPressed[0] & (STICK_RIGHT | R_JPAD)) && (gControllerPakMenuState.mainChoice != 1)) {
        gControllerPakMenuState.mainChoice = 1;
        enqueueSoundEffect(SFX_MENU_CURSOR, SFX_PRIORITY_NORMAL);
    } else if ((gPlayerInputPressed[0] & (STICK_LEFT | L_JPAD)) && (gControllerPakMenuState.mainChoice != 0)) {
        gControllerPakMenuState.mainChoice = 0;
        enqueueSoundEffect(SFX_MENU_CURSOR, SFX_PRIORITY_NORMAL);
    }
    if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
        enqueueSoundEffect(SFX_CONFIRM, SFX_PRIORITY_NORMAL);
        if (gControllerPakMenuState.mainChoice == 1) {
            setCurrentGameTaskCallback(func_8000CB0C, 0);
        } else {
            setCurrentGameTaskCallback(func_8000C6FC, 0);
            gControllerPakMenuState.state = 1;
            gControllerPakMenuState.isEdgeScroll = 0;
        }
    }
    updateCallbackTasks();
}

#pragma GLOBAL_ASM("asm.jp/nonmatchings/menu/controller_pak/controller_pak_file_delete_flow/func_8000C6FC.s")

#pragma GLOBAL_ASM("asm.jp/nonmatchings/menu/controller_pak/controller_pak_file_delete_flow/func_8000C8E0.s")

#pragma GLOBAL_ASM("asm.jp/nonmatchings/menu/controller_pak/controller_pak_file_delete_flow/func_8000CA78.s")

void func_8000CB0C(void) {
    if (gCurrentGameTask->callbackData0 != 0xFF) {
        gCurrentGameTask->callbackData0 = stepMenuFadeAlpha((s16)gCurrentGameTask->callbackData0, 0x24, 1);
        if (gCurrentGameTask->callbackData0 == 0xFF) {
            gFramebufferSwapHold = 1;
        } else {
            updateCallbackTasks();
        }
    } else {
        if (gPendingFramebufferSwapCount == 2) {
            releaseMenuAssetHandles();
            gFramebufferSwapHold = 0;
            gFramebufferRenderInterval.value = 0;
            resumeGameTask(0);
            removeGameTask(4);
        }
    }
}
