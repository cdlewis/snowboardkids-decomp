#include "common.h"

#define MENU_PANEL_SCROLL_STEP 0x10
#define MENU_PANEL_SCROLL_LIMIT 0x79
#define MENU_PANEL_TILE_SIZE 0x20
#define MENU_PANEL_INPUT_REPEAT_FRAMES 4
#define MENU_PANEL_ACCEPT_SOUND 0x18
#define MENU_PANEL_SOUND_VOLUME 0x32
#define MENU_PANEL_TEXTURE_HANDLE (*(s16 *)&D_80112130[0x54])

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
extern u8 D_800D4A40[];
extern u8 D_800D54F8[];
extern u8 D_800D5508[];
extern u8 D_800D5518[];
extern u8 D_800D5528[];
extern u8 D_800D5598[];
extern u8 D_800D55E8[];
extern u8 D_800D5630[];
extern u16 *D_800D5538[];
extern u8 D_80112130[];
extern u8 D_80121B5B;
extern s16 D_800DEF14;
extern s16 D_8011213C;
extern s32 D_80123778;
extern s32 D_801235B4;
extern s16 D_801235B0;
extern u8 D_8012482A;
extern u8 D_8012482B;

extern void func_80071824(void *task, void (*callback)());
extern void func_800483FC(RenderCallbackNode **, RenderCallback, s32);
extern void func_800716E4(MenuPanelActor *);
extern void func_8000F030(s16, s16, s32, s32, s32, s32, s32, s32);
extern void func_8001303C(s32, s32, u8 *, s32, s32, s32, s32);
extern s32 func_80043040(s16);
extern void func_80046358(s32, s32, s32, s32);
extern void func_80072138(s16, s16);

extern void func_800515F0(MenuPanelActor *);
extern void func_80051878(s32);
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

// func_80050FF0 best match: 93.040%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_panel_ui/func_80050FF0.s")

#ifdef NON_MATCHING
void func_80050FF0(s32 arg0) {
    s16 edgeX;
    s16 edgeY;
    s32 x;
    s32 y;

    func_8000F030(-0x84, -0x64, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    func_8000F030(0x78, -0x64, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);

    edgeX = -0x74;
    do {
        func_8000F030(edgeX, -0x64, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, 0);
        func_8000F030(edgeX, -0x3C, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, 0);
        edgeX += 0x10;
    } while (edgeX < 0x7C);

    func_8000F030(-0x84, -0x3C, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, 0);
    func_8000F030(0x78, -0x3C, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, 0);

    edgeY = -0x54;
    do {
        func_8000F030(-0x84, edgeY, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 5, 0x20, 0x20, 0, 0);
        func_8000F030(0x78, edgeY, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 6, 0x20, 0x20, 0, 0);
        edgeY += 0x10;
    } while (edgeY < -0x34);

    for (x = -0x74; x != 0x7C; x += 0x10) {
        for (y = -0x54; y != -0x34; y += 0x10) {
            func_8000F030(x, y, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, 0);
        }
    }

    func_8001303C(-0x68, -0x58, D_800D4A40, 0, 0x100, 5, 0x29);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_panel_ui/func_80051308.s")

// func_800515F0 best match: 71.684%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_panel_ui/func_800515F0.s")

#ifdef NON_MATCHING
void func_800515F0(MenuPanelActor *arg0) {
    s32 input;
    s32 pulse;
    u8 current;
    u8 next;

    if ((D_800DEF14 == 0) && (D_8010B1F0 == 0)) {
        input = D_80123778;
        if (input & 0x10800) {
            current = D_80121B5B;
            next = D_800D54F8[current];
            if (current != next) {
                D_80121B5B = next;
                func_80072138(0x19, MENU_PANEL_SOUND_VOLUME);
                input = D_80123778;
            }
        } else if (input & 0x20400) {
            current = D_80121B5B;
            next = D_800D5508[current];
            if (current != next) {
                D_80121B5B = next;
                func_80072138(0x19, MENU_PANEL_SOUND_VOLUME);
                input = D_80123778;
            }
        } else if (input & 0x40100) {
            current = D_80121B5B;
            next = D_800D5528[current];
            if (current != next) {
                D_80121B5B = next;
                func_80072138(0x19, MENU_PANEL_SOUND_VOLUME);
                input = D_80123778;
            }
        } else if (input & 0x80200) {
            current = D_80121B5B;
            next = D_800D5518[current];
            if (current != next) {
                D_80121B5B = next;
                func_80072138(0x19, MENU_PANEL_SOUND_VOLUME);
                input = D_80123778;
            }
        }

        if (input & 0xD000) {
            D_8010B1F0 = 1;
            if (input & 0x4000) {
                D_80121B5B = 0xC;
            }
            func_80072138(MENU_PANEL_ACCEPT_SOUND, MENU_PANEL_SOUND_VOLUME);
        }
    }

    if (D_8010B1F0 == 0) {
        pulse = D_801235B0 & 0xF;
        arg0->x = pulse;
        if (pulse >= 9) {
            pulse = 0x10 - pulse;
            arg0->x = pulse;
        }
        arg0->x = (pulse << 4) + 0x7F;
    } else if (D_801235B0 & 1) {
        arg0->x = 0;
    } else {
        arg0->x = 0xFF;
    }

    func_800483FC(&D_80124868, func_80050FF0, (s32)arg0);
    func_800483FC(&D_80124868, func_80051308, (s32)arg0);
}
#endif

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

void func_80051ED4(MenuPanelActor *arg0) {
    s16 timer;
    u8 state;

    timer = arg0->inputRepeatTimer;
    if (timer == 0) {
        if (arg0->selectionState == 0) {
            arg0->selectedTile++;
        }
        arg0->inputRepeatTimer = MENU_PANEL_INPUT_REPEAT_FRAMES;
    } else {
        arg0->inputRepeatTimer = timer - 1;
    }

    if (D_80123778 & 0x9000) {
        state = arg0->selectionState;
        switch (state) {
        case 0:
            arg0->selectedTile = 0xE7;
            break;
        case 1:
            D_8010B1F0 = 1;
            func_80071824(arg0, func_80051E80);
            break;
        case 2:
            arg0->selectedTile = 0;
            arg0->selectionState = 0;
            arg0->inputRepeatTimer = 0;
            arg0->tileList = arg0->tileListStart;
            break;
        }
        func_80072138(MENU_PANEL_ACCEPT_SOUND, MENU_PANEL_SOUND_VOLUME);
    }

    func_800483FC(&D_80124868, func_80051878, (s32)arg0);
}

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

// func_80052550 best match: 93.040%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_panel_ui/func_80052550.s")

#ifdef NON_MATCHING
void func_80052550(s32 arg0) {
    s16 edgeX;
    s16 edgeY;
    s32 x;
    s32 y;

    func_8000F030(-0x84, -0x64, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    func_8000F030(0x78, -0x64, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);

    edgeX = -0x74;
    do {
        func_8000F030(edgeX, -0x64, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, 0);
        func_8000F030(edgeX, -0x2C, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, 0);
        edgeX += 0x10;
    } while (edgeX < 0x7C);

    func_8000F030(-0x84, -0x2C, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, 0);
    func_8000F030(0x78, -0x2C, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, 0);

    edgeY = -0x54;
    do {
        func_8000F030(-0x84, edgeY, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 5, 0x20, 0x20, 0, 0);
        func_8000F030(0x78, edgeY, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 6, 0x20, 0x20, 0, 0);
        edgeY += 0x10;
    } while (edgeY < -0x24);

    for (x = -0x74; x != 0x7C; x += 0x10) {
        for (y = -0x54; y != -0x24; y += 0x10) {
            func_8000F030(x, y, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, 0);
        }
    }

    func_8001303C(-0x76, -0x58, D_800D55E8, 0, 0x100, 5, 0x29);
}
#endif

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
