#include "common.h"
#include "effect_task_scheduler.h"
#include "asset_manager.h"
#include "character_select_course_menu.h"
#include "controller_pak_file_delete_flow.h"
#include "controller_pak_menu.h"
#include "controller_pak_ui.h"
#include "input_task_scheduler.h"
#include "main_menu.h"
#include "menu_rendering.h"
#include "viewport_manager.h"

extern s32 enqueueSoundEffect(s32, s32);
extern void releaseMenuAssetHandles(void);

extern ControllerPakMenuState gControllerPakMenuState;
extern ControllerPakFileEntry gControllerPakFileEntries[];
extern CharacterSelectFlowState *D_801235B8;
extern s8 gFramebufferSwapDelay;
extern u8 D_800EC9D8;
extern u8 gControllerPakMenuCursorState;
extern u8 gControllerPakDeletePromptState;
extern s32 gPlayerInputPressed;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern s16 gMenuFadeAlpha;
extern s16 D_800EC9C8;
extern s8 D_800EC8B0;
extern s32 gControllerPakFreeBytes;
extern s32 gControllerPakFreeFileCount;
extern s32 D_801235B4;
extern u8 D_593D10;
extern u8 D_598A70;
extern u8 D_59AAA0;
extern u8 D_59DFE0;
extern u8 D_5DFDD0;
extern u8 D_5E0350;
extern u8 gControllerPakReplaySaveNoticeSecondMessageStart;
extern u8 D_60F1A0;
extern u8 D_60F990;

void initControllerPakFileDeleteFlow(void) {
    func_80000C48(0);
    if (D_800EC9C8 != 1) {
        func_8009956C(fadeOutControllerPakFileDeleteFlow, 0);
        return;
    }
    func_800704F0();
    func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
    gFramebufferSwapDelay = 0;
    D_801235B8->fade = 0;
    D_801235B8->timer = 0;
    D_801235B4 = 0;
    D_800EC9D8 = 0;
    gControllerPakFreeBytes = 0;
    gControllerPakFreeFileCount = 0;
    D_800EC8B0 = 0;
    gMenuFadeAlpha = D_801235B8->fade;
    loadCompressedRomAsset(&D_5DFDD0, &D_5E0350, 0x21);
    loadCompressedRomAsset(&D_593D10, &D_598A70, 0x22);
    loadCompressedRomAsset(&D_598A70, &D_59AAA0, 0x23);
    loadCompressedRomAsset(&D_59AAA0, &D_59DFE0, 0x24);
    loadCompressedRomAsset(&D_5E0350, &gControllerPakReplaySaveNoticeSecondMessageStart, 0x26);
    loadCompressedRomAsset(&D_60F1A0, &D_60F990, 0x29);
    func_80070EC0(0);
    func_80071408(&initControllerPakFileDeleteMainOptions, 0, 0x63);
    func_80071408(&initControllerPakFileDeleteConfirmOptions, 0, 0x63);
    func_80071408(&initControllerPakFileDeleteFreeSpaceInfo, 0, 0x63);
    func_80071408(&initControllerPakFileDeleteFileList, 0, 0x63);
    func_80071408(&initControllerPakFileDeleteIcon, 0, 0x5E);
    gControllerPakMenuState.mainChoice = 0;
    gControllerPakMenuState.fileIndex = 0;
    gControllerPakMenuState.confirmChoice = 0;
    gControllerPakMenuState.state = 0;
    gControllerPakMenuState.visibleFileIndex = 0;
    gControllerPakMenuState.isEdgeScroll = 0;
    requestControllerPakFileList();
    requestControllerPakFreeSpaceUpdate();
    func_8009956C(updateControllerPakFileDeleteMainOptions, 0);
}

void updateControllerPakFileDeleteMainOptions(void) {
    if ((gPlayerInputPressed & 0x40100) && (gControllerPakMenuState.mainChoice != 1)) {
        gControllerPakMenuState.mainChoice = 1;
        enqueueSoundEffect(0x19, 0x32);
    } else if ((gPlayerInputPressed & 0x80200) && (gControllerPakMenuState.mainChoice != 0)) {
        gControllerPakMenuState.mainChoice = 0;
        enqueueSoundEffect(0x19, 0x32);
    }
    if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
        enqueueSoundEffect(0x18, 0x32);
        if (gControllerPakMenuState.mainChoice == 1) {
            func_8009956C(fadeOutControllerPakFileDeleteFlow, 0);
        } else {
            func_8009956C(updateControllerPakFileDeleteFileList, 0);
            gControllerPakMenuState.state = 1;
            gControllerPakMenuState.isEdgeScroll = 0;
        }
    }
    func_8007105C();
}

void updateControllerPakFileDeleteFileList(void) {
    u8 direction = 0;

    if ((gPlayerInputPressed & 0x10800) && (gControllerPakMenuState.fileIndex != 0)) {
        gControllerPakMenuState.fileIndex--;
        direction = 1;
    } else if ((gPlayerInputPressed & 0x20400) && (gControllerPakMenuState.fileIndex != 0xF)) {
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

    if (((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x4000, ((gPlayerInputPressed & 0x1000) != 0))) &&
        (gPlayerInputPressed & 0x4000, (gControllerPakFileEntries[gControllerPakMenuState.fileIndex].exists != 0))) {
        enqueueSoundEffect(0x18, 0x32);
        func_8009956C(updateControllerPakFileDeleteConfirm, 0);
        gControllerPakMenuState.state = 2;
        gControllerPakMenuState.confirmChoice = 1;
    } else if ((gPlayerInputPressed & 0x4000) != 0) {
        enqueueSoundEffect(0x18, 0x32);
        func_8009956C(updateControllerPakFileDeleteMainOptions, 0);
        gControllerPakMenuState.state = 0;
    }
    func_8007105C();
}

void updateControllerPakFileDeleteConfirm(void) {
    if ((gPlayerInputPressed & 0x40100) && (gControllerPakMenuState.confirmChoice != 1)) {
        gControllerPakMenuState.confirmChoice = 1;
        enqueueSoundEffect(0x19, 0x32);
    } else if ((gPlayerInputPressed & 0x80200) && (gControllerPakMenuState.confirmChoice != 0)) {
        gControllerPakMenuState.confirmChoice = 0;
        enqueueSoundEffect(0x19, 0x32);
    }
    if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
        enqueueSoundEffect(0x18, 0x32);
        if (gControllerPakMenuState.confirmChoice == 0) {
            func_80071408(&initControllerPakDeleteConfirmPrompt, 0, 0x64);
            func_8009956C(updateControllerPakFileDeletePrompt, 0);
            gControllerPakMenuState.state = 3;
            gControllerPakMenuState.confirmChoice = 1;
        } else {
            func_8009956C(updateControllerPakFileDeleteFileList, 0);
            gControllerPakMenuState.state = 1;
        }
    } else if (gPlayerInputPressed & 0x4000) {
        enqueueSoundEffect(0x18, 0x32);
        func_8009956C(updateControllerPakFileDeleteFileList, 0);
        gControllerPakMenuState.state = 1;
    }
    func_8007105C();
}

void updateControllerPakFileDeletePrompt(void) {
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
            requestControllerPakDeleteFile(gControllerPakMenuState.fileIndex);
            if (D_800EC9D8 == 0) {
                gControllerPakFileEntries[gControllerPakMenuState.fileIndex].exists = 0;
                requestControllerPakFileList();
                requestControllerPakFreeSpaceUpdate();
                func_8009956C(updateControllerPakFileDeleteFileList, 0);
                gControllerPakMenuState.state = 1;
            } else {
                func_80071408(initControllerPakFileDeleteErrorPrompt, 0, 0x64);
                gControllerPakMenuState.unk6 = 0;
                gControllerPakMenuState.state = 4;
                func_8009956C(updateControllerPakFileDeleteErrorPrompt, 0);
            }
        } else {
            func_8009956C(updateControllerPakFileDeleteConfirm, 0);
            gControllerPakMenuState.state = 2;
            gControllerPakMenuState.confirmChoice = 0;
        }
    } else if (gPlayerInputPressed & 0x4000) {
        enqueueSoundEffect(0x18, 0x32);
        func_8009956C(updateControllerPakFileDeleteConfirm, 0);
        gControllerPakMenuState.state = 2;
        gControllerPakMenuState.confirmChoice = 0;
    }
    func_8007105C();
}

void updateControllerPakFileDeleteErrorPrompt(void) {
    s32 state;

    state = gControllerPakDeletePromptState;
    if (state == 1) {
        if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
            enqueueSoundEffect(0x18, 0x32);
            gControllerPakDeletePromptState = 2;
            state = 2;
        }
    }
    if (state == 3) {
        D_800EC9D8 = 0;
        func_8009956C(&updateControllerPakFileDeleteFileList, 0);
        gControllerPakMenuCursorState = 1;
    }
    func_8007105C();
}

void fadeOutControllerPakFileDeleteFlow(void) {
    if (D_801235B8->fade != 0xFF) {
        D_801235B8->fade = func_80013F88((s16) D_801235B8->fade, 0x24, 1);
        if (D_801235B8->fade == 0xFF) {
            gFramebufferSwapHold = 1;
        } else {
            func_8007105C();
        }
    } else {
        if (gPendingFramebufferSwapCount == 2) {
            releaseMenuAssetHandles();
            gFramebufferSwapHold = 0;
            gFramebufferSwapDelay = 0;
            func_80099658(0);
            func_8009954C(4);
        }
    }
}
