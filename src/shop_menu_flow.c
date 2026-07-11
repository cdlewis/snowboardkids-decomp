#include "common.h"
#include "game_audio.h"
#include "effect_task_scheduler.h"
#include "asset_manager.h"
#include "character_select_course_menu.h"
#include "controller_pak_ui.h"
#include "input_task_scheduler.h"
#include "menu_rendering.h"
#include "shop_menu_flow.h"
#include "viewport_manager.h"

typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ u8 state;
} ShopMenuPromptTransition;

extern void enqueueSoundEffect(s32, s32);
extern void releaseMenuAssetHandles(void);

extern CharacterSelectFlowState *D_801235B8;
extern ShopMenuPromptTransition gControllerPakContinuePromptTransition;
extern s8 D_800DEED4;
extern s16 gMenuFadeAlpha;
extern s32 D_801235B4;
extern u8 D_80123750;
extern u8 D_80123751;
extern s32 gPlayerInputPressed;
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_59AAA0[];
extern u8 D_59DFE0[];
extern u8 D_60F1A0[];
extern u8 D_60F990[];

void func_8000C280(void) {
    func_800720E4(3);
    func_800704F0();
    func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
    D_800DEED4 = 0;
    D_801235B8->fade = 0xFF;
    D_801235B8->timer = 0;
    D_801235B4 = 0;
    gControllerPakContinuePromptTransition.state = 0;
    gControllerPakContinuePromptTransition.x = 0x100;
    loadCompressedRomAsset(D_593D10, D_598A70, 0x22);
    loadCompressedRomAsset(D_598A70, D_59AAA0, 0x23);
    loadCompressedRomAsset(D_59AAA0, D_59DFE0, 0x24);
    loadCompressedRomAsset(D_60F1A0, D_60F990, 0x29);
    func_80070EC0(0);
    func_80071408(initControllerPakContinuePrompt, 0, 0x64);
    gMenuFadeAlpha = D_801235B8->fade;
    func_8009956C(func_8000C3C8, 0);
    func_8007105C();
}

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_flow/func_8000C3C8.s")

void func_8000C52C(void) {
    if (D_801235B8->fade != 0xFF) {
        D_801235B8->fade = func_80013F88((s16) D_801235B8->fade, 0x24, 1);
        if (D_801235B8->fade == 0xFF) {
            D_80123751 = 1;
            gControllerPakContinuePromptTransition.state = 3;
        } else {
            func_8007105C();
        }
    } else {
        if (D_80123750 == 2) {
            releaseMenuAssetHandles();
            D_80123751 = 0;
            D_800DEED4 = 0;
            func_80099658(2);
            func_8009954C(4);
        }
    }
}
