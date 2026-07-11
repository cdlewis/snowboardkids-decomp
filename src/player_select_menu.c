#include "common.h"
#include "game_audio.h"
#include "effect_task_scheduler.h"
#include "asset_manager.h"
#include "character_select_course_menu.h"
#include "input_task_scheduler.h"
#include "menu_rendering.h"
#include "player_select_menu.h"
#include "player_select_ui.h"
#include "title_menu.h"
#include "viewport_manager.h"

typedef struct {
    char pad[0x18];
    s32 fade;
    s32 timer;
} PlayerSelectMenuState;

typedef struct {
    u8 state;
    char pad1[1];
    s16 nextState;
    s16 confirmState;
} PlayerSelectCursorState;

extern void releaseMenuAssetHandles(void);
extern s32 enqueueSoundEffect(s16, s16);

extern u8 D_800EC9DC;
extern u8 D_5A1ED0[];
extern u8 D_5C5320[];
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_59AAA0[];
extern u8 D_59DFE0[];
extern u8 D_59E7F0[];
extern u8 D_245A80[];
extern u8 D_24C8E0[];

extern PlayerSelectMenuState *D_801235B8;
extern PlayerSelectCursorState D_8010AE70;
extern u8 D_800EC9C1;
extern u8 D_800EC9C2;
extern u8 D_800EC9DD;
extern u8 D_80121D88;
extern s32 D_8010ADDC;
extern s32 D_801235B4;
extern u8 D_8010ADF8;
extern u16 D_8010ADF0;
extern s16 gMenuFadeAlpha;
extern s8 D_800DEED4;
extern u8 D_80123750;
extern u8 D_80123751;

void func_80005540(void) {
    D_801235B8->fade = 1;
    func_800720E4(1);
    D_800EC9C1 = 0;
    D_80121D88 = 0;
    D_8010ADDC = 0;
    D_801235B8->timer = 0;
    D_801235B4 = 0;
    D_8010ADF8 = 0;
    D_800EC9DD = 0;
    D_8010ADF0 = 0;
    gMenuFadeAlpha = D_801235B8->fade;
    func_8009956C(func_80005788, 0);
    D_8010AE70.state = 0;
    D_8010AE70.nextState = 0;
    func_8007105C();
}

void func_800055EC(void) {
    func_800720E4(1);
    D_800EC9DC = 0;
    D_800EC9DD = 0;
    func_800704F0();
    func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
    D_800DEED4 = 0;
    D_801235B8->fade = 0xFF;
    loadCompressedRomAsset(D_5A1ED0, D_5C5320, 0x21);
    loadCompressedRomAsset(D_593D10, D_598A70, 0x22);
    loadCompressedRomAsset(D_598A70, D_59AAA0, 0x23);
    loadCompressedRomAsset(D_59DFE0, D_59E7F0, 0x26);
    loadCompressedRomAsset(D_245A80, D_24C8E0, 0x1F);
    loadCompressedRomAsset(D_59AAA0, D_59DFE0, 0x24);
    func_80070EC0(0);
    func_80071408((void (*)(EffectTask *))func_8001710C, 0, 0x5E);
    D_800EC9C1 = 0;
    D_80121D88 = 0;
    D_8010ADDC = 0;
    D_801235B8->timer = 0;
    D_8010ADF8 = 0;
    D_8010ADF0 = 0;
    gMenuFadeAlpha = D_801235B8->fade;
    func_8009956C(func_80005788, 0);
    D_8010AE70.state = 0;
    D_8010AE70.nextState = 0;
    func_8007105C();
}

// func_80005788 best match: 98.535% (nonmatchings/func_80005788-7273315160691878794/base_15.c)
#pragma GLOBAL_ASM("asm/nonmatchings/player_select_menu/func_80005788.s")

#ifdef NON_MATCHING
void func_80005788(void) {
    s32 newInput;
    s32 heldInput;
    s32 pressedUp;
    s32 repeatTimer;
    s32 selection;
    s32 newInputCopy;
    s32 previousSelection;
    s32 tempSelection;
    u8 waitTimer;

    if (D_801235B8->fade != 0) {
        D_801235B8->fade = func_80013F88((s16) D_801235B8->fade, 0x24, 0);
        if (D_801235B8->fade == 0) {
            func_80071408(func_8001952C, 0, 0x63);
        }
    } else {
        if ((D_8010AE70.confirmState == 0x100) && (D_80121D88 == 0)) {
            if (D_800EC9C1 == 0) {
                if (D_8010AE70.state == 1) {
                    selection = D_800EC9C2;
                    newInputCopy = gPlayerInputHeld;
                    newInput = newInputCopy;
                    pressedUp = newInput & 0x10800;
                    previousSelection = selection;

                    if ((pressedUp == 0) && ((newInput & 0x20400) == 0)) {
                        D_8010ADF0 = 0;
                    }

                    heldInput = gPlayerInputPressed;
                    if ((heldInput & 0x10800) ||
                        ((pressedUp != 0) && (D_8010ADF0 >= 0xB) && ((D_8010ADF0 % 3) == 0))) {
                        repeatTimer = D_8010ADF0;
                        tempSelection = selection - 1;
                        if (repeatTimer == 0) {
                            D_8010ADF0 = repeatTimer + 1;
                            repeatTimer = D_8010ADF0;
                        }
                        if (selection > 0) {
                            D_800EC9C2 = tempSelection;
                            selection = ((tempSelection & 0xFF) & 0xFF) & 0xFF;
                        }
                    } else {
                        repeatTimer = D_8010ADF0;
                        if ((heldInput & 0x20400) ||
                            ((newInput & 0x20400) && (repeatTimer >= 0xB) && ((repeatTimer % 3) == 0))) {
                            if (repeatTimer == 0) {
                                D_8010ADF0 = repeatTimer + 1;
                                repeatTimer = D_8010ADF0;
                            }
                            if (selection < 4) {
                                D_800EC9C2 = selection + 1;
                                selection = (u8) (selection + 1);
                            }
                        }
                    }

                    if (repeatTimer != 0) {
                        D_8010ADF0 = repeatTimer + 1;
                        if (D_8010ADF0 == 0xFFFF) {
                            D_8010ADF0 = 0xC;
                        }
                    }

                    if (selection != previousSelection) {
                        enqueueSoundEffect(0x19, 0x32);
                        heldInput = gPlayerInputPressed;
                    }

                    if ((heldInput & 0x1000) || ((heldInput & 0x8000) && (D_801235B4 == 5))) {
                        if ((D_800EC9C2 == 3) && (D_80121D85 == 5)) {
                            enqueueSoundEffect(0x46, 0x32);
                        } else {
                            enqueueSoundEffect(0x18, 0x32);
                            D_800EC9C1 = 1;
                            if (D_8010ADF0 && D_8010ADF0) {
                            }
                            D_8010AE70.state = 2;
                            D_8010AE70.nextState = 0x100;
                            D_8010ADF8 = 0;
                        }
                    }
                }
            } else {
                D_800EC9C1++;
            }
        }

        waitTimer = D_800EC9C1;
        if ((gPlayerInputPressed & 0x4000) && (D_801235B4 == 5) && (waitTimer == 0)) {
            enqueueSoundEffect(1, 0x32);
            D_800EC9C1 = 1;
            D_8010AE70.state = 2;
            D_8010AE70.nextState = 0x100;
            D_8010ADF8 = 1;
        }

        waitTimer = D_800EC9C1;
        if (waitTimer == 7) {
            D_80121D88 = 1;
            D_800EC9C1++;
        }

        if (D_80121D88 == 2) {
            func_8009956C(func_80005B14, 0);
            if (D_8010ADF8 == 0) {
                func_80072114(4);
            }
        }
    }
    D_801235B4 = 0;
    func_8007105C();
}
#endif

void func_80005B14(void) {
    u8 v0;

    v0 = D_800EC9C2;
    if ((v0 == 0 || v0 == 2) && D_8010ADF8 == 0) {
        func_8009956C(initCharacterSelectCourseMenuFromPlayerSelect, 0);
        D_801235B4 = 0;
    } else {
        func_8009956C(func_80005B80, 0);
    }
    func_8007105C();
}

void func_80005B80(void) {
    if (D_801235B8->fade != 0xFF) {
        D_801235B8->fade = func_80013F88((s16) D_801235B8->fade, 0x24, 1);
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
            if (D_8010ADF8 == 1) {
                D_801235B4 = 1;
            } else {
                D_801235B4 = 0;
            }
            func_80099658(2);
            func_8009954C(4);
        }
    }
}
