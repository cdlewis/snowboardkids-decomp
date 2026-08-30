#include "common.h"
#include "assets.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/menu/controller_pak/controller_pak_file_delete_flow.h"
#include "game/menu/controller_pak/controller_pak_menu.h"
#include "game/menu/controller_pak/controller_pak_ui.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/engine/viewport_manager.h"
#include "game/race/race_state.h"


extern u8 gControllerPakMenuCursorState;
extern u8 gControllerPakDeletePromptState;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern s16 gMenuFadeAlpha;
extern s32 gControllerPakFreeBytes;
extern s32 gControllerPakFreeFileCount;
extern s32 gMenuFlowState;

void initControllerPakFileDeleteFlow(void) {
    requestControllerPakProbe(0);
    if (gControllerPakStatusCodes[0] != 1) {
        setCurrentGameTaskCallback(fadeOutControllerPakFileDeleteFlow, 0);
        return;
    }
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
    gFramebufferRenderInterval.value = 0;
    gCurrentGameTask->fade = 0;
    gCurrentGameTask->timer = 0;
    gMenuFlowState = 0;
    gControllerPakRetryCounts[0] = 0;
    gControllerPakFreeBytes = 0;
    gControllerPakFreeFileCount = 0;
    gRaceRumbleEnabled = 0;
    gMenuFadeAlpha = gCurrentGameTask->fade;
    LOAD_ASSET(_5DFDD0, 0x21);
    LOAD_ASSET(_593D10, 0x22);
    LOAD_ASSET(_598A70, 0x23);
    LOAD_ASSET(_59AAA0, 0x24);
    LOAD_ASSET(_5E0350, 0x26);
    LOAD_ASSET(_60F1A0, 0x29);
    initCallbackTaskScheduler(0);
    createCallbackTask((CallbackTaskCallback)&initControllerPakFileDeleteMainOptions, 0, 0x63);
    createCallbackTask((CallbackTaskCallback)&initControllerPakFileDeleteConfirmOptions, 0, 0x63);
    createCallbackTask((CallbackTaskCallback)&initControllerPakFileDeleteFreeSpaceInfo, 0, 0x63);
    createCallbackTask((CallbackTaskCallback)&initControllerPakFileDeleteFileList, 0, 0x63);
    createCallbackTask((CallbackTaskCallback)&initControllerPakFileDeleteIcon, 0, 0x5E);
    gControllerPakMenuState.mainChoice = 0;
    gControllerPakMenuState.fileIndex = 0;
    gControllerPakMenuState.confirmChoice = 0;
    gControllerPakMenuState.state = 0;
    gControllerPakMenuState.visibleFileIndex = 0;
    gControllerPakMenuState.isEdgeScroll = 0;
    requestControllerPakFileList();
    requestControllerPakFreeSpaceUpdate();
    setCurrentGameTaskCallback(updateControllerPakFileDeleteMainOptions, 0);
}

void updateControllerPakFileDeleteMainOptions(void) {
    if ((gPlayerInputPressed[0] & (STICK_RIGHT | R_JPAD)) && (gControllerPakMenuState.mainChoice != 1)) {
        gControllerPakMenuState.mainChoice = 1;
        enqueueSoundEffect(0x19, 0x32);
    } else if ((gPlayerInputPressed[0] & (STICK_LEFT | L_JPAD)) && (gControllerPakMenuState.mainChoice != 0)) {
        gControllerPakMenuState.mainChoice = 0;
        enqueueSoundEffect(0x19, 0x32);
    }
    if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
        enqueueSoundEffect(0x18, 0x32);
        if (gControllerPakMenuState.mainChoice == 1) {
            setCurrentGameTaskCallback(fadeOutControllerPakFileDeleteFlow, 0);
        } else {
            setCurrentGameTaskCallback(updateControllerPakFileDeleteFileList, 0);
            gControllerPakMenuState.state = 1;
            gControllerPakMenuState.isEdgeScroll = 0;
        }
    }
    updateCallbackTasks();
}

void updateControllerPakFileDeleteFileList(void) {
    u8 direction = 0;

    if ((gPlayerInputPressed[0] & (STICK_UP | U_JPAD)) && (gControllerPakMenuState.fileIndex != 0)) {
        gControllerPakMenuState.fileIndex--;
        direction = 1;
    } else if ((gPlayerInputPressed[0] & (STICK_DOWN | D_JPAD)) && (gControllerPakMenuState.fileIndex != 0xF)) {
        gControllerPakMenuState.fileIndex++;
        direction = 2;
    }
    if (direction == 0) {
        gControllerPakMenuState.isEdgeScroll = 0;
    } else {
        enqueueSoundEffect(0x19, 0x32);
        if ((gControllerPakMenuState.visibleFileIndex != 0) && (gControllerPakMenuState.visibleFileIndex != 4)) {
            gControllerPakMenuState.isEdgeScroll = 0;
        } else if (gControllerPakMenuState.visibleFileIndex == 0 && direction == 2) {
            gControllerPakMenuState.isEdgeScroll = 0;
        } else {
            if ((gControllerPakMenuState.visibleFileIndex == 4) && (direction == 1)) {
                gControllerPakMenuState.isEdgeScroll = 0;
            } else {
                gControllerPakMenuState.isEdgeScroll = 1;
            }
        }

        if (gControllerPakMenuState.isEdgeScroll == 0) {
            if (direction == 1) {
                gControllerPakMenuState.visibleFileIndex--;
            } else {
                gControllerPakMenuState.visibleFileIndex++;
            }
        }
    }

    if (((gPlayerInputPressed[0] & A_BUTTON) ||
         (gPlayerInputPressed[0] & B_BUTTON, ((gPlayerInputPressed[0] & START_BUTTON) != 0))) &&
        (gPlayerInputPressed[0] & B_BUTTON,
         (gControllerPakFileStates[gControllerPakMenuState.fileIndex].company_code != 0))) {
        enqueueSoundEffect(0x18, 0x32);
        setCurrentGameTaskCallback(updateControllerPakFileDeleteConfirm, 0);
        gControllerPakMenuState.state = 2;
        gControllerPakMenuState.confirmChoice = 1;
    } else if ((gPlayerInputPressed[0] & B_BUTTON) != 0) {
        enqueueSoundEffect(0x18, 0x32);
        setCurrentGameTaskCallback(updateControllerPakFileDeleteMainOptions, 0);
        gControllerPakMenuState.state = 0;
    }
    updateCallbackTasks();
}

void updateControllerPakFileDeleteConfirm(void) {
    if ((gPlayerInputPressed[0] & (STICK_RIGHT | R_JPAD)) && (gControllerPakMenuState.confirmChoice != 1)) {
        gControllerPakMenuState.confirmChoice = 1;
        enqueueSoundEffect(0x19, 0x32);
    } else if ((gPlayerInputPressed[0] & (STICK_LEFT | L_JPAD)) && (gControllerPakMenuState.confirmChoice != 0)) {
        gControllerPakMenuState.confirmChoice = 0;
        enqueueSoundEffect(0x19, 0x32);
    }
    if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
        enqueueSoundEffect(0x18, 0x32);
        if (gControllerPakMenuState.confirmChoice == 0) {
            createCallbackTask((CallbackTaskCallback)&initControllerPakDeleteConfirmPrompt, 0, 0x64);
            setCurrentGameTaskCallback(updateControllerPakFileDeletePrompt, 0);
            gControllerPakMenuState.state = 3;
            gControllerPakMenuState.confirmChoice = 1;
        } else {
            setCurrentGameTaskCallback(updateControllerPakFileDeleteFileList, 0);
            gControllerPakMenuState.state = 1;
        }
    } else if (gPlayerInputPressed[0] & B_BUTTON) {
        enqueueSoundEffect(0x18, 0x32);
        setCurrentGameTaskCallback(updateControllerPakFileDeleteFileList, 0);
        gControllerPakMenuState.state = 1;
    }
    updateCallbackTasks();
}

void updateControllerPakFileDeletePrompt(void) {
    if ((gPlayerInputPressed[0] & (STICK_UP | U_JPAD)) && (gControllerPakMenuState.confirmChoice != 0)) {
        gControllerPakMenuState.confirmChoice = 0;
        enqueueSoundEffect(0x19, 0x32);
    } else if ((gPlayerInputPressed[0] & (STICK_DOWN | D_JPAD)) && (gControllerPakMenuState.confirmChoice != 1)) {
        gControllerPakMenuState.confirmChoice = 1;
        enqueueSoundEffect(0x19, 0x32);
    }
    if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
        enqueueSoundEffect(0x18, 0x32);
        if (gControllerPakMenuState.confirmChoice == 0) {
            requestControllerPakDeleteFile(gControllerPakMenuState.fileIndex);
            if (gControllerPakRetryCounts[0] == 0) {
                gControllerPakFileStates[gControllerPakMenuState.fileIndex].company_code = 0;
                requestControllerPakFileList();
                requestControllerPakFreeSpaceUpdate();
                setCurrentGameTaskCallback(updateControllerPakFileDeleteFileList, 0);
                gControllerPakMenuState.state = 1;
            } else {
                createCallbackTask((CallbackTaskCallback)initControllerPakFileDeleteErrorPrompt, 0, 0x64);
                gControllerPakMenuState.unk6 = 0;
                gControllerPakMenuState.state = 4;
                setCurrentGameTaskCallback(updateControllerPakFileDeleteErrorPrompt, 0);
            }
        } else {
            setCurrentGameTaskCallback(updateControllerPakFileDeleteConfirm, 0);
            gControllerPakMenuState.state = 2;
            gControllerPakMenuState.confirmChoice = 0;
        }
    } else if (gPlayerInputPressed[0] & B_BUTTON) {
        enqueueSoundEffect(0x18, 0x32);
        setCurrentGameTaskCallback(updateControllerPakFileDeleteConfirm, 0);
        gControllerPakMenuState.state = 2;
        gControllerPakMenuState.confirmChoice = 0;
    }
    updateCallbackTasks();
}

void updateControllerPakFileDeleteErrorPrompt(void) {
    s32 state;

    state = gControllerPakDeletePromptState;
    if (state == 1) {
        if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
            enqueueSoundEffect(0x18, 0x32);
            gControllerPakDeletePromptState = 2;
            state = 2;
        }
    }
    if (state == 3) {
        gControllerPakRetryCounts[0] = 0;
        setCurrentGameTaskCallback(&updateControllerPakFileDeleteFileList, 0);
        gControllerPakMenuCursorState = 1;
    }
    updateCallbackTasks();
}

void fadeOutControllerPakFileDeleteFlow(void) {
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
            gFramebufferRenderInterval.value = 0;
            resumeGameTask(0);
            removeGameTask(4);
        }
    }
}
