#include "common.h"
#include "effect_task_scheduler.h"
#include "asset_manager.h"
#include "character_select_course_menu.h"
#include "controller_pak_replay_save_warning_flow.h"
#include "controller_pak_ui.h"
#include "input_task_scheduler.h"
#include "menu_rendering.h"
#include "viewport_manager.h"

extern void releaseMenuAssetHandles(void);

extern CharacterSelectFlowState *D_801235B8;
extern s8 gFramebufferSwapDelay;
extern s16 gMenuFadeAlpha;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern u8 gControllerPakReplaySaveWarningSecondMessageStart[];
extern u8 gControllerPakReplaySaveWarningFirstMessageStart[];
extern u8 gMainMenuSceneModelAssetStart[];

void initControllerPakReplaySaveWarningFlow(void) {
    func_800704F0();
    func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
    gFramebufferSwapDelay = 0;
    D_801235B8->fade = 0xFF;
    D_801235B8->timer = 0;
    gMenuFadeAlpha = D_801235B8->fade;
    loadCompressedRomAsset(gControllerPakReplaySaveWarningFirstMessageStart,
                           gMainMenuSceneModelAssetStart, 0x26);
    func_80070EC0(0);
    func_80071408(initControllerPakMessageIcon, 0, 0x5E);
    func_8009956C(fadeInControllerPakReplaySaveWarningFirstMessage, 0);
    func_8007105C();
}

void fadeInControllerPakReplaySaveWarningFirstMessage(void) {
    if (D_801235B8->fade != 0) {
        D_801235B8->fade = func_80013F88(D_801235B8->fade, 0xF, 0);
    } else {
        D_801235B8->timer++;
    }
    if (D_801235B8->timer >= 0x50) {
        D_801235B8->timer = 0;
        func_8009956C(fadeOutControllerPakReplaySaveWarningFirstMessage, 0);
    }
    func_8007105C();
}

void fadeOutControllerPakReplaySaveWarningFirstMessage(void) {
    if (D_801235B8->fade != 0xFF) {
        D_801235B8->fade = func_80013F88(D_801235B8->fade, 0xF, 1);
        if (D_801235B8->fade == 0xFF) {
            releaseMenuAssetHandles();
            loadCompressedRomAsset(gControllerPakReplaySaveWarningSecondMessageStart,
                                   gControllerPakReplaySaveWarningFirstMessageStart, 0x26);
        }
    } else {
        func_8009956C(waitBeforeControllerPakReplaySaveWarningSecondMessage, 0);
    }
    func_8007105C();
}

void waitBeforeControllerPakReplaySaveWarningSecondMessage(void) {
    D_801235B8->timer++;
    if (D_801235B8->timer >= 0x14) {
        D_801235B8->fade = 0xFF;
        gMenuFadeAlpha = D_801235B8->fade;
        D_801235B8->timer = 0;
        func_8009956C(fadeInControllerPakReplaySaveWarningSecondMessage, 0);
    }
    func_8007105C();
}

void fadeInControllerPakReplaySaveWarningSecondMessage(void) {
    if (D_801235B8->fade != 0) {
        D_801235B8->fade = func_80013F88(D_801235B8->fade, 0xF, 0);
    } else {
        D_801235B8->timer++;
    }
    if (D_801235B8->timer >= 0x50) {
        func_8009956C(fadeOutControllerPakReplaySaveWarningFlow, 0);
    }
    func_8007105C();
}

void fadeOutControllerPakReplaySaveWarningFlow(void) {
    if (D_801235B8->fade != 0xFF) {
        D_801235B8->fade = func_80013F88(D_801235B8->fade, 0xF, 1);
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
