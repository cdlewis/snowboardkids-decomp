#include "common.h"
#include "effect_task_scheduler.h"
#include "asset_manager.h"
#include "character_select_course_menu.h"
#include "controller_pak_race_warning_flow.h"
#include "controller_pak_ui.h"
#include "input_task_scheduler.h"
#include "menu_rendering.h"
#include "viewport_manager.h"

extern void releaseMenuAssetHandles(void);

extern CharacterSelectFlowState *D_801235B8;
extern s8 D_800DEED4;
extern s16 D_800DEF14;
extern u8 D_80123750;
extern u8 D_80123751;
extern u8 D_5E26E0[];
extern u8 D_5E34A0[];
extern u8 D_5E0E40[];

void initControllerPakRaceWarningFlow(void) {
    func_800704F0();
    func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
    D_800DEED4 = 0;
    D_801235B8->fade = 0xFF;
    D_801235B8->timer = 0;
    D_800DEF14 = D_801235B8->fade;
    loadCompressedRomAsset(D_5E26E0, D_5E34A0, 0x26);
    func_80070EC0(0);
    func_80071408(initControllerPakMessageIcon, 0, 0x5E);
    func_8009956C(updateControllerPakRaceWarningFirstMessage, 0);
    func_8007105C();
}

void updateControllerPakRaceWarningFirstMessage(void) {
    if (D_801235B8->fade != 0) {
        D_801235B8->fade = func_80013F88(D_801235B8->fade, 0xF, 0);
    } else {
        D_801235B8->timer++;
    }
    if (D_801235B8->timer >= 0x50) {
        D_801235B8->timer = 0;
        func_8009956C(fadeOutControllerPakRaceWarningFirstMessage, 0);
    }
    func_8007105C();
}

void fadeOutControllerPakRaceWarningFirstMessage(void) {
    if (D_801235B8->fade != 0xFF) {
        D_801235B8->fade = func_80013F88(D_801235B8->fade, 0xF, 1);
        if (D_801235B8->fade == 0xFF) {
            releaseMenuAssetHandles();
            loadCompressedRomAsset(D_5E0E40, D_5E26E0, 0x26);
        }
    } else {
        func_8009956C(waitControllerPakRaceWarningSecondMessage, 0);
    }
    func_8007105C();
}

void waitControllerPakRaceWarningSecondMessage(void) {
    D_801235B8->timer++;
    if (D_801235B8->timer >= 0x14) {
        D_801235B8->fade = 0xFF;
        D_800DEF14 = D_801235B8->fade;
        D_801235B8->timer = 0;
        func_8009956C(updateControllerPakRaceWarningSecondMessage, 0);
    }
    func_8007105C();
}

void updateControllerPakRaceWarningSecondMessage(void) {
    if (D_801235B8->fade != 0) {
        D_801235B8->fade = func_80013F88(D_801235B8->fade, 0xF, 0);
    } else {
        D_801235B8->timer++;
    }
    if (D_801235B8->timer >= 0x50) {
        func_8009956C(fadeOutControllerPakRaceWarningFlow, 0);
    }
    func_8007105C();
}

void fadeOutControllerPakRaceWarningFlow(void) {
    if (D_801235B8->fade != 0xFF) {
        D_801235B8->fade = func_80013F88(D_801235B8->fade, 0xF, 1);
        if (D_801235B8->fade == 0xFF) {
            D_80123751 = 1;
        } else {
            func_8007105C();
        }
    } else {
        if (D_80123750 == 2) {
            releaseMenuAssetHandles();
            D_80123751 = 0;
            D_800DEED4 = 0;
            func_80099658(0);
            func_8009954C(4);
        }
    }
}
