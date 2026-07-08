#include "common.h"
#include "effect_task_scheduler.h"
#include "asset_decompression.h"
#include "character_select_flow.h"
#include "controller_pak_message_flow.h"
#include "controller_pak_menu_ui.h"
#include "input_task_scheduler.h"

extern void func_800704F0(void);
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern s32 func_80013F88(s32, s32, s32);
extern void func_80045914(void);

extern CharacterSelectFlowState *D_801235B8;
extern s8 D_800DEED4;
extern s16 D_800DEF14;
extern u8 D_80123750;
extern u8 D_80123751;
extern u8 D_5E26E0[];
extern u8 D_5E34A0[];
extern u8 D_5E0E40[];

void func_8000CF70(void) {
    func_800704F0();
    func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
    D_800DEED4 = 0;
    D_801235B8->fade = 0xFF;
    D_801235B8->timer = 0;
    D_800DEF14 = D_801235B8->fade;
    func_800437F0(D_5E26E0, D_5E34A0, 0x26);
    func_80070EC0(0);
    func_80071408(func_80031CD0, 0, 0x5E);
    func_8009956C(func_8000D048, 0);
    func_8007105C();
}

void func_8000D048(void) {
    if (D_801235B8->fade != 0) {
        D_801235B8->fade = func_80013F88(D_801235B8->fade, 0xF, 0);
    } else {
        D_801235B8->timer++;
    }
    if (D_801235B8->timer >= 0x50) {
        D_801235B8->timer = 0;
        func_8009956C(func_8000D0D4, 0);
    }
    func_8007105C();
}

void func_8000D0D4(void) {
    if (D_801235B8->fade != 0xFF) {
        D_801235B8->fade = func_80013F88(D_801235B8->fade, 0xF, 1);
        if (D_801235B8->fade == 0xFF) {
            func_80045914();
            func_800437F0(D_5E0E40, D_5E26E0, 0x26);
        }
    } else {
        func_8009956C(func_8000D17C, 0);
    }
    func_8007105C();
}

void func_8000D17C(void) {
    D_801235B8->timer++;
    if (D_801235B8->timer >= 0x14) {
        D_801235B8->fade = 0xFF;
        D_800DEF14 = D_801235B8->fade;
        D_801235B8->timer = 0;
        func_8009956C(func_8000D1F4, 0);
    }
    func_8007105C();
}

void func_8000D1F4(void) {
    if (D_801235B8->fade != 0) {
        D_801235B8->fade = func_80013F88(D_801235B8->fade, 0xF, 0);
    } else {
        D_801235B8->timer++;
    }
    if (D_801235B8->timer >= 0x50) {
        func_8009956C(func_8000D280, 0);
    }
    func_8007105C();
}

void func_8000D280(void) {
    if (D_801235B8->fade != 0xFF) {
        D_801235B8->fade = func_80013F88(D_801235B8->fade, 0xF, 1);
        if (D_801235B8->fade == 0xFF) {
            D_80123751 = 1;
        } else {
            func_8007105C();
        }
    } else {
        if (D_80123750 == 2) {
            func_80045914();
            D_80123751 = 0;
            D_800DEED4 = 0;
            func_80099658(0);
            func_8009954C(4);
        }
    }
}
