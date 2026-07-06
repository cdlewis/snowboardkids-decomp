#include "common.h"

#define MENU_PANEL_SCROLL_STEP 0x10
#define MENU_PANEL_SCROLL_LIMIT 0x79
#define MENU_PANEL_TILE_SIZE 0x20
#define MENU_PANEL_INPUT_REPEAT_FRAMES 4
#define MENU_PANEL_ACCEPT_SOUND 0x18
#define MENU_PANEL_SOUND_VOLUME 0x32

typedef struct MenuPanelActor MenuPanelActor;
typedef void (*MenuPanelActorCallback)(MenuPanelActor *);

typedef struct RenderCallbackNode RenderCallbackNode;
typedef void (*RenderCallback)(s32);

struct MenuPanelActor {
    /* 0x00 */ char pad0[0x18];
    /* 0x18 */ s32 x;
    /* 0x1C */ s32 y;
    /* 0x20 */ u16 *tileList;
    /* 0x24 */ u16 *tileListStart;
    /* 0x28 */ s16 inputRepeatTimer;
    /* 0x2A */ u8 selectedTile;
    /* 0x2B */ u8 selectionState;
};

extern RenderCallbackNode *D_80124868;
extern u8 D_8010B1F0;
extern u8 D_800D5598[];
extern u8 D_800D55E8[];
extern u8 D_800D5630[];
extern u16 *D_800D5538[];
extern u8 D_80121B5B;
extern s16 D_8011213C;
extern s32 D_80123778;
extern s32 D_801235B4;
extern s16 D_801235B0;
extern u8 D_8012482A;
extern u8 D_8012482B;

extern void func_80071824(void *task, void (*callback)());
extern void func_800483FC(RenderCallbackNode **, RenderCallback, s32);
extern void func_800716E4(MenuPanelActor *);
extern void func_8001303C(s32, s32, u8 *, s32, s32, s32, s32);
extern s32 func_80043040(s16);
extern void func_80046358(s32, s32, s32, s32);
extern void func_80072138(s16, s16);

extern void func_800515F0(MenuPanelActor *);
extern void func_80051878(s32);
extern void func_80051ED4(MenuPanelActor *);
extern void func_80052034(s32);
extern void func_80052364(MenuPanelActor *);
extern void func_800523B8(s32);
extern void func_80052464(MenuPanelActor *);
extern void func_800524D4(s32);
extern void func_80052550(s32);
extern void func_80052868(s32);
extern void func_80052E00(MenuPanelActor *);
extern void func_80052E70(s32);
extern void func_80053604(MenuPanelActor *);

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_panel_ui/func_80050FF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_panel_ui/func_80051308.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_panel_ui/func_800515F0.s")

void func_80051854(MenuPanelActor *arg0) {
    func_80071824(arg0, func_800515F0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_panel_ui/func_80051878.s")

void func_80051E80(MenuPanelActor *arg0) {
    arg0->y += MENU_PANEL_SCROLL_STEP;
    if (arg0->y >= MENU_PANEL_SCROLL_LIMIT) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_80051878, (s32) arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_panel_ui/func_80051ED4.s")

void func_80051FDC(MenuPanelActor *arg0) {
    arg0->x = -0x80;
    arg0->y = 4;
    arg0->selectionState = 0;
    arg0->inputRepeatTimer = 0;
    arg0->tileList = D_800D5538[D_80121B5B];
    func_80071824(arg0, func_80051ED4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_panel_ui/func_80052034.s")

void func_80052364(MenuPanelActor *arg0) {
    func_800483FC(&D_80124868, func_80052034, (s32) arg0);
}

void func_80052394(MenuPanelActor *arg0) {
    func_80071824(arg0, func_80052364);
}

void func_800523B8(s32 arg0) {
    func_80046358(-0x84, -0x64, func_80043040(D_8011213C), 6);
    func_80046358(0x74, -0x64, func_80043040(D_8011213C), 7);
    func_80046358(-0x84, 4, func_80043040(D_8011213C), 8);
    func_80046358(0x74, 4, func_80043040(D_8011213C), 9);
}

void func_80052464(MenuPanelActor *arg0) {
    if (D_8010B1F0 == 0) {
        func_800483FC(&D_80124868, func_800523B8, (s32) arg0);
        return;
    }
    func_800716E4(arg0);
}

void func_800524B0(MenuPanelActor *arg0) {
    func_80071824(arg0, func_80052464);
}

void func_800524D4(s32 arg0) {
    func_8001303C(-0x78, -0x58, D_800D5598, 1, 0x100, 5, 0x28);
}

void func_80052520(s32 arg0) {
    func_800483FC(&D_80124868, func_800524D4, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_panel_ui/func_80052550.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_panel_ui/func_80052868.s")

void func_80052E00(MenuPanelActor *arg0) {
    func_800483FC(&D_80124868, func_80052550, (s32) arg0);
    func_800483FC(&D_80124868, func_80052868, (s32) arg0);
}

void func_80052E4C(MenuPanelActor *arg0) {
    func_80071824(arg0, func_80052E00);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_panel_ui/func_80052E70.s")

void func_80053604(MenuPanelActor *arg0) {
    func_800483FC(&D_80124868, func_80052E70, (s32) arg0);
}

void func_80053634(MenuPanelActor *arg0) {
    func_80071824(arg0, func_80053604);
}
