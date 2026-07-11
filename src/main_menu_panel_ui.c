#include "common.h"
#include "memory_allocator.h"
#include "effect_task_scheduler.h"
#include "main_menu_panel_ui.h"
#define MENU_RENDERING_BROAD_PROTOTYPES
#include "menu_rendering.h"

#define MENU_PANEL_SCROLL_STEP 0x10
#define MENU_PANEL_SCROLL_LIMIT 0x79
#define MENU_PANEL_TILE_SIZE 0x20
#define MENU_PANEL_INPUT_REPEAT_FRAMES 4
#define MENU_PANEL_ACCEPT_SOUND 0x18
#define MENU_PANEL_SOUND_VOLUME 0x32
#define MENU_PANEL_TEXTURE_HANDLE (D_80112130.textureHandle)

typedef void (*MenuPanelActorCallback)(MenuPanelActor *);

typedef struct RenderCallbackNode RenderCallbackNode;
typedef void (*RenderCallback)(s32);

typedef struct MenuPanelAssetHandles {
    /* 0x00 */ char pad0[0x38];
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 unk3A;
    /* 0x3C */ char pad3C[2];
    /* 0x3E */ s16 fontHandle;
    /* 0x40 */ char pad40[0x10];
    /* 0x50 */ s16 cancelHandle;
    /* 0x52 */ char pad52[2];
    /* 0x54 */ s16 textureHandle;
} MenuPanelAssetHandles;

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
extern u8 D_800D54E7[];
extern u8 D_800D54E8[];
extern u8 D_800D54EA[];
extern u8 D_800D54F0[];
extern u8 D_800D54F8[];
extern u8 D_800D5508[];
extern u8 D_800D5518[];
extern u8 D_800D5528[];
extern u8 D_800D5598[];
extern u8 D_800D55E8[];
extern u8 D_800D5630[];
extern u16 *D_800D5704[];
extern s16 D_800D57B4[];
extern u16 *D_800D5538[];
extern u16 *D_800D5568[];
extern MenuPanelAssetHandles D_80112130;
extern u8 D_80121B5B;
extern s16 D_80121B50;
extern s16 D_80121B52;
extern s16 gMenuFadeAlpha;
extern s16 D_8011213C;
extern s16 D_80112180;
extern s32 gPlayerInputPressed;
extern s32 D_801235B4;
extern s16 gFrameCounter;
extern u8 D_8012482A;
extern u8 D_8012482B;
extern u8 D_8012482C;
extern s16 D_80156612;

extern void func_800483FC(RenderCallbackNode **, RenderCallback, s32);
extern void func_80045A78(s16, s16, s32, u16);
extern void func_80045E84(s16, s16, s32, s32);
extern void func_80046358(s32, s32, s32, s32);
extern void func_80046748(s16, s16, s32, s32, s32, s32, s32);
extern void func_80046D68(s16, s16, s32, s32, s32);
extern void enqueueSoundEffect(s16, s16);

void func_80050FF0(s32 arg0) {
    s32 edgeX;
    s32 edgeY;
    s32 y;
    s32 yOrigin;
    s32 xOrigin;

    xOrigin = -0x74;
    yOrigin = 0x54;

    func_8000F030(-0x84, -0x64, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    func_8000F030(0x78, -0x64, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);

    edgeX = xOrigin;
    do {
        func_8000F030(edgeX, -0x64, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, 0);
        func_8000F030(edgeX, -0x3C, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, 0);
        edgeX += 0x10;
    } while (edgeX < 0x7C);

    func_8000F030(-0x84, -0x3C, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, 0);
    func_8000F030(0x78, -0x3C, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, 0);

    edgeY = -yOrigin;
    do {
        func_8000F030(-0x84, edgeY, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 5, 0x20, 0x20, 0, 0);
        func_8000F030(0x78, edgeY, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 6, 0x20, 0x20, 0, 0);
        edgeY += 0x10;
    } while (edgeY < -0x34);

    if (yOrigin) {
    }

    for (edgeX = xOrigin; edgeX != 0x7C; edgeX += 0x10) {
        for (y = -yOrigin; y != -0x34; y += 0x10) {
            func_8000F030(edgeX, y, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, 0);
        }
    }

    func_8001303C(-0x68, -0x58, D_800D4A40, 0, 0x100, 5, 0x29);
}

void func_80051308(MenuPanelActor *arg0) {
    s32 alpha;
    s32 i;
    s32 next;
    s32 x;
    u8 *tile;

    if (arg0) {
    }

    x = -0x6E;
    tile = D_800D54E8; i = 0; do {
        next = i + 1;
        alpha = 0;
        if (next == D_80121B5B) {
            alpha = arg0->x;
        }

        func_80046748((s16)(x + 0xA), -0x16, 0x22, 0x22, 0x40, alpha / 4, 0);
        func_80046748((s16)(x + 4), -0x1C, 0x22, 0x22, 0xFF, alpha, 0);
        func_80045A78((s16)(x + 5), -0x1B, func_80043040(D_80112130.fontHandle), *tile);
        i = next;
        tile++;
        x += 0x2C;
    } while (next < 5);

    x = -0x84;
    tile = D_800D54F0;
    i = 0;
    do {
        alpha = 0;
        if ((i + 6) == D_80121B5B) {
            alpha = arg0->x;
        }

        func_80046748((s16)(x + 0xA), 0x1E, 0x22, 0x22, 0, alpha / 4, 0x40);
        func_80046748((s16)(x + 4), 0x18, 0x22, 0x22, arg0->x * 0, alpha, 0xFF);
        func_80045A78((s16)(x + 5), 0x19, func_80043040(D_80112130.fontHandle), *tile);
        i++;
        tile++;
        x += 0x2C;
    } while (i != 6);

    if (D_80121B5B == 0xC) {
        if (arg0->x != 0) {
            func_80045A78(-0x30, 0x4D, func_80043040(D_80112130.cancelHandle), 2);
            func_80045E84(-0x30, 0x4D, func_80043040(D_80112130.cancelHandle), 3);
            return;
        }
        func_80046D68(-0x30, 0x4D, func_80043040(D_80112130.cancelHandle), 2, 1);
        func_80045E84(-0x30, 0x4D, func_80043040(D_80112130.cancelHandle), 3);
        return;
    }

    func_80046D68(-0x30, 0x4D, func_80043040(D_80112130.cancelHandle), 2, 1);
}

// func_800515F0 best match: 75.664%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_panel_ui/func_800515F0.s")

#ifdef NON_MATCHING
void func_800515F0(MenuPanelActor *arg0) {
    s32 temp_t8;
    s32 temp_v1;
    s32 var_v0;
    u8 temp_a2;
    u8 temp_a2_2;
    u8 temp_a2_3;
    u8 temp_a2_4;

    if ((gMenuFadeAlpha == 0) && (D_8010B1F0 == 0)) {
        temp_v1 = gPlayerInputPressed;
        if (temp_v1 & 0x10800) {
            temp_a2 = D_800D54F8[D_80121B5B];
            if (D_80121B5B != temp_a2) {
                D_80121B5B = temp_a2;
                enqueueSoundEffect(0x19, 0x32);
                temp_v1 = gPlayerInputPressed;
            }
        } else if (temp_v1 & 0x20400) {
            temp_a2_2 = D_800D5508[D_80121B5B];
            if (D_80121B5B != temp_a2_2) {
                D_80121B5B = temp_a2_2;
                enqueueSoundEffect(0x19, 0x32);
                temp_v1 = gPlayerInputPressed;
            }
        } else if (temp_v1 & 0x40100) {
            temp_a2_3 = D_800D5528[D_80121B5B];
            if (D_80121B5B != temp_a2_3) {
                D_80121B5B = temp_a2_3;
                enqueueSoundEffect(0x19, 0x32);
                temp_v1 = gPlayerInputPressed;
            }
        } else if (temp_v1 & 0x80200) {
            temp_a2_4 = D_800D5518[D_80121B5B];
            if (D_80121B5B != temp_a2_4) {
                D_80121B5B = temp_a2_4;
                enqueueSoundEffect(0x19, 0x32);
                temp_v1 = gPlayerInputPressed;
            }
        }

        if (temp_v1 & 0xD000) {
            D_8010B1F0 = 1;
            if (temp_v1 & 0x4000) {
                D_80121B5B = 0xC;
            }
            enqueueSoundEffect(0x18, 0x32);
        }
    }

    if (D_8010B1F0 == 0) {
        temp_t8 = gFrameCounter & 0xF;
        arg0->x = temp_t8;
        var_v0 = temp_t8;
        if (temp_t8 >= 9) {
            var_v0 = 0x10 - temp_t8;
            arg0->x = var_v0;
        }
        arg0->x = (var_v0 * 0x10) + 0x7F;
    } else if (gFrameCounter & 1) {
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

// func_80051878 best match: 90.948%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_panel_ui/func_80051878.s")

#ifdef NON_MATCHING
void func_80051878(MenuPanelActor *arg0) {
    s32 i;
    u8 *new_var;

    func_8000F030((s16)(arg0->x - 4), (s16)(arg0->y + 0x14), func_80043040(D_80112130.textureHandle),
                  2, 0x20, 0x20, 0, 0);
    func_8000F030((s16)(arg0->x + 0xF8), (s16)(arg0->y + 0x14), func_80043040(D_80112130.textureHandle),
                  4, 0x20, 0x20, 0, 0);

    i = 0;
    do {
        func_8000F030((s16)((arg0->x + i) + 0xC), (s16)(arg0->y + 0x14),
                      func_80043040(D_80112130.textureHandle), 3, 0x20, 0x20, 0, 0);
        func_8000F030((s16)((arg0->x + i) + 0xC), (s16)(arg0->y + 0x4C),
                      func_80043040(D_80112130.textureHandle), 8, 0x20, 0x20, 0, 0);
        i += 0x10;
    } while (i < 0xF0);

    func_8000F030((s16)(arg0->x - 4), (s16)(arg0->y + 0x4C), func_80043040(D_80112130.textureHandle),
                  7, 0x20, 0x20, 0, 0);
    func_8000F030((s16)(arg0->x + 0xF8), (s16)(arg0->y + 0x4C), func_80043040(D_80112130.textureHandle),
                  9, 0x20, 0x20, 0, 0);

    i = 0;
    do {
        func_8000F030((s16)(arg0->x - 4), (s16)((arg0->y + i) + 0x24),
                      func_80043040(D_80112130.textureHandle), 5, 0x20, 0x20, 0, 0);
        func_8000F030((s16)(arg0->x + 0xF8), (s16)((arg0->y + i) + 0x24),
                      func_80043040(D_80112130.textureHandle), 6, 0x20, 0x20, 0, 0);
        i += 0x10;
    } while (i < 0x30);

    {
        u16 glyphText[2];
        u16 glyph;
        s32 visibleIndex;
        s32 streamIndex;
        s32 lineX;
        u16 lineY;
        s32 color;

        glyphText[1] = 0xFFFF;
        visibleIndex = 0;
        streamIndex = 0;
        lineX = 0;
        i = 7;
        lineY = 0;
        color = i;

        while (visibleIndex != arg0->selectedTile) {
            glyph = arg0->tileList[streamIndex++];
            glyphText[0] = glyph;

            switch (glyph) {
            case 0xFFFF:
                arg0->selectionState = 1;
                visibleIndex = arg0->selectedTile;
                break;

            case 0xFFFD:
                lineY = lineY + 0x10;
                lineX = 0;
                break;

            case 0xFFFB:
                arg0->selectionState = 2;
                arg0->tileListStart = &arg0->tileList[streamIndex];
                visibleIndex = arg0->selectedTile;
                break;

            case 0xFFFC:
                color = arg0->tileList[streamIndex++];
                break;

            default:
                func_8001303C((s16)(arg0->x + lineX), (s16)((arg0->y + lineY) + 0x18), (u8 *)glyphText, 0,
                              0x100, color, 0x29);
                lineX = (lineX + 0x10) & 0xFFFF;
                visibleIndex++;
                break;
            }
        }

        if ((arg0->selectionState != 0) && (D_8010B1F0 == 0)) {
            func_8000F030((s16)(arg0->x + 0xF4), (s16)(arg0->y + 0x48),
                          func_80043040(D_80112130.textureHandle), (gFrameCounter >> 4) & 1, 0x20, 0x20, 0,
                          0);
        }
    }

    {
        s32 count;
        u16 *text;

        count = 0;
        text = D_800D5568[D_80121B5B];
        if (D_800D5568[D_80121B5B][0] != 0xFFFF) {
            do {
                count++;
            } while (text[count] != 0xFFFF);
        }

        func_8001303C((s16)(-((count * 0x10) / 2)), (s16)((-0x48) - arg0->y),
                      (u8 *)D_800D5568[D_80121B5B], 0, 0x100, 4, 0x29);
    }

    if (D_80121B5B < 6) {
        func_80045A78(-0x10, (s16)((-0x30) - arg0->y), func_80043040(D_80112130.fontHandle),
                      (new_var = D_800D54E7)[D_80121B5B]);
        return;
    }

    func_80045A78(-0x10, (s16)((-0x30) - arg0->y), func_80043040(D_80112130.fontHandle),
                  D_800D54EA[D_80121B5B]);
}
#endif

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

    if (gPlayerInputPressed & 0x9000) {
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
        enqueueSoundEffect(MENU_PANEL_ACCEPT_SOUND, MENU_PANEL_SOUND_VOLUME);
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

void func_80052034(s32 arg0) {
    if (D_801235B4 == 0) {
        if ((D_8010B1F0 == 0) || (gFrameCounter & 1)) {
            func_80045A78(-0x30, -0x20, func_80043040(D_80112180), 0);
            func_80045E84(-0x30, -0x20, func_80043040(D_80112180), 3);
        } else {
            func_80046D68(-0x30, -0x20, func_80043040(D_80112180), 0, 1);
            func_80045E84(-0x30, -0x20, func_80043040(D_80112180), 3);
        }
    } else {
        func_80046D68(-0x30, -0x20, func_80043040(D_80112180), 0, 1);
    }

    if (D_801235B4 == 1) {
        if ((D_8010B1F0 == 0) || (gFrameCounter & 1)) {
            func_80045A78(-0x30, -8, func_80043040(D_80112180), 1);
            func_80045E84(-0x30, -8, func_80043040(D_80112180), 3);
        } else {
            func_80046D68(-0x30, -8, func_80043040(D_80112180), 1, 1);
            func_80045E84(-0x30, -8, func_80043040(D_80112180), 3);
        }
    } else {
        func_80046D68(-0x30, -8, func_80043040(D_80112180), 1, 1);
    }

    if (D_801235B4 == 2) {
        if ((D_8010B1F0 == 0) || (gFrameCounter & 1)) {
            func_80045A78(-0x30, 0x10, func_80043040(D_80112180), 2);
            func_80045E84(-0x30, 0x10, func_80043040(D_80112180), 3);
            return;
        }
        func_80046D68(-0x30, 0x10, func_80043040(D_80112180), 2, 1);
        func_80045E84(-0x30, 0x10, func_80043040(D_80112180), 3);
        return;
    }

    func_80046D68(-0x30, 0x10, func_80043040(D_80112180), 2, 1);
}

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

void func_80052550(s32 arg0) {
    s32 y;
    s32 edgeX;
    s32 edgeY;
    s32 yOrigin;
    s32 xOrigin;

    xOrigin = -0x74;
    yOrigin = 0x54;

    func_8000F030(-0x84, -0x64, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    func_8000F030(0x78, -0x64, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);

    edgeX = xOrigin;
    do {
        func_8000F030(edgeX, -0x64, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, 0);
        func_8000F030(edgeX, -0x2C, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, 0);
        edgeX += 0x10;
    } while (edgeX < 0x7C);

    func_8000F030(-0x84, -0x2C, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, 0);
    func_8000F030(0x78, -0x2C, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, 0);

    edgeY = -yOrigin;
    do {
        func_8000F030(-0x84, edgeY, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 5, 0x20, 0x20, 0, 0);
        func_8000F030(0x78, edgeY, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 6, 0x20, 0x20, 0, 0);
        edgeY += 0x10;
    } while (edgeY < -0x24);

    if (yOrigin) {
    }

    for (edgeX = xOrigin; edgeX != 0x7C; edgeX += 0x10) {
        for (y = -yOrigin; y != -0x24; y += 0x10) {
            func_8000F030(edgeX, y, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, 0);
        }
    }
    func_8001303C(-0x76, -0x58, D_800D55E8, 0, 0x100, 5, 0x29);
}

void func_80052868(s32 arg0) {
    s32 var_v0;
    s32 sp28;

    var_v0 = 1;
    sp28 = var_v0;
    if (D_801235B4 == 0) {
        var_v0 = 2;
        sp28 = var_v0;
    }
    func_80046D68(-0x78, -0x10, func_80043040(D_80112130.cancelHandle), 4, sp28);
    sp28 = 1;
    if (D_8012482A == 1) {
        func_80046D68(0x10, -0x10, func_80043040(D_80112130.cancelHandle), 7, 2);
        func_80046D68(0x48, -0x10, func_80043040(D_80112130.cancelHandle), 8, 1);
        if (D_801235B4 == 0) {
            func_80045E84(0x10, -0x10, func_80043040(D_80112130.cancelHandle), 0xB);
        } else {
            func_80045A78(0x10, -0x10, func_80043040(D_80112130.cancelHandle), 0xB);
        }
    } else {
        func_80046D68(0x10, -0x10, func_80043040(D_80112130.cancelHandle), 7, 1);
        func_80046D68(0x48, -0x10, func_80043040(D_80112130.cancelHandle), 8, 2);
        if (D_801235B4 == 0) {
            func_80045E84(0x48, -0x10, func_80043040(D_80112130.cancelHandle), 0xB);
        } else {
            func_80045A78(0x48, -0x10, func_80043040(D_80112130.cancelHandle), 0xB);
        }
    }
    if (D_801235B4 == 1) {
        sp28 = 2;
    }
    func_80046D68(-0x78, 8, func_80043040(D_80112130.cancelHandle), 5, sp28);
    sp28 = 1;
    if (D_8012482B == 0) {
        func_80046D68(0x10, 8, func_80043040(D_80112130.cancelHandle), 9, 2);
        func_80046D68(0x48, 8, func_80043040(D_80112130.cancelHandle), 0xA, 1);
        if (D_801235B4 == 1) {
            func_80045E84(0x10, 8, func_80043040(D_80112130.cancelHandle), 0xB);
        } else {
            func_80045A78(0x10, 8, func_80043040(D_80112130.cancelHandle), 0xB);
        }
    } else {
        func_80046D68(0x10, 8, func_80043040(D_80112130.cancelHandle), 9, 1);
        func_80046D68(0x48, 8, func_80043040(D_80112130.cancelHandle), 0xA, 2);
        if (D_801235B4 == 1) {
            func_80045E84(0x48, 8, func_80043040(D_80112130.cancelHandle), 0xB);
        } else {
            func_80045A78(0x48, 8, func_80043040(D_80112130.cancelHandle), 0xB);
        }
    }
    if (D_801235B4 == 2) {
        sp28 = 2;
    }
    func_80046D68(-0x78, 0x20, func_80043040(D_80112130.cancelHandle), 6, sp28);
    sp28 = 1;
    if (D_8012482C == 0) {
        func_80046D68(0x10, 0x20, func_80043040(D_80112130.cancelHandle), 9, 2);
        func_80046D68(0x48, 0x20, func_80043040(D_80112130.cancelHandle), 0xA, 1);
        if (D_801235B4 == 2) {
            func_80045E84(0x10, 0x20, func_80043040(D_80112130.cancelHandle), 0xB);
        } else {
            func_80045A78(0x10, 0x20, func_80043040(D_80112130.cancelHandle), 0xB);
        }
    } else {
        func_80046D68(0x10, 0x20, func_80043040(D_80112130.cancelHandle), 9, 1);
        func_80046D68(0x48, 0x20, func_80043040(D_80112130.cancelHandle), 0xA, 2);
        if (D_801235B4 == 2) {
            func_80045E84(0x48, 0x20, func_80043040(D_80112130.cancelHandle), 0xB);
        } else {
            func_80045A78(0x48, 0x20, func_80043040(D_80112130.cancelHandle), 0xB);
        }
    }
    if (D_801235B4 == 3) {
        sp28 = 2;
    }
    func_80046D68(-0x68, 0x48, func_80043040(D_80112130.cancelHandle), 2, sp28);
}

void func_80052E00(MenuPanelActor *arg0) {
    func_800483FC(&D_80124868, func_80052550, (s32) arg0);
    func_800483FC(&D_80124868, func_80052868, (s32) arg0);
}

void func_80052E4C(MenuPanelActor *arg0) {
    func_80071824(arg0, func_80052E00);
}

// func_80052E70 best match: 98.939%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_panel_ui/func_80052E70.s")

#ifdef NON_MATCHING
void func_80052E70(s32 arg0) {
    s32 x;
    s32 x2;
    s16 xStart;
    s32 y1;
    s32 y;
    s32 y2;

    xStart = 0x74;

    func_8000F030(-0x84, 0x1C, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    func_8000F030(0x78, 0x1C, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);
    x = -xStart;
    do {
        func_8000F030((s16)x, 0x1C, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, 0);
        func_8000F030((s16)x, 0x54, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, 0);
        x += 0x10;
    } while (x < 0x7C);
    func_8000F030(-0x84, 0x54, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, 0);
    func_8000F030(0x78, 0x54, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, 0);
    y1 = 0x2C;
    do {
        func_8000F030(-0x84, (s16)y1, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 5, 0x20, 0x20, 0, 0);
        func_8000F030(0x78, (s16)y1, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 6, 0x20, 0x20, 0, 0);
        y1 = (((((((s16)(y1 + 0x10)) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu;
    } while (y1 < 0x5C);
    for (x = -0x74; x < 0x7C; x += 0x10) {
        for (y = 0x2C; y != 0x5C; y += 0x10) {
            func_8000F030((s16)x, (s16)y, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, 0);
        }
    }
    func_8001303C(-0x76, 0x28, D_800D5630, 0, 0x100, 7, 0x29);
    func_8000F030(-0x40, -0x30, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    func_8000F030(0x30, -0x30, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);
    x2 = -0x30;
    do {
        func_8000F030((s16)x2, -0x30, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, 0);
        func_8000F030((s16)x2, 0, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, 0);
        x2 = (s16)(x2 + 0x10);
    } while (x2 < 0x30);
    y2 = -0x20;
    do {
        func_8000F030(-0x40, (s16)y2, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 5, 0x20, 0x20, 0, 0);
        func_8000F030(0x30, (s16)y2, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 6, 0x20, 0x20, 0, 0);
        y2 += 0x10;
    } while ((y2 - 0) < 0);
    for (x2 = -0x30; x2 != 0x30; x2 += 0x10) {
        for (y = -0x20; y != 0; y += 0x10) {
            func_8000F030((s16)x2, (s16)y, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, 0);
        }
    }
    func_8000F030(-0x40, 0, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, 0);
    func_8000F030(0x30, 0, func_80043040(MENU_PANEL_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, 0);
    if (D_801235B4 == 0) {
        if (D_80121B52 != 1) {
            if (D_80156612 & 8) {
                func_80045A78(-0x30, -0x2A, func_80043040(D_80112130.cancelHandle), 0xC);
            }
        }
        if ((D_80121B52 != 9) && (D_80156612 & 8)) {
            func_80045A78(-0x30, -6, func_80043040(D_80112130.cancelHandle), 0xD);
        }
    }
    if ((D_801235B4 != 0) && (D_80156612 & 1)) {
        func_8001303C(-0x30, -0x18, D_800D5704[D_80121B52], 0, 0x100, 0, 0x29);
    } else {
        func_8001303C(-0x30, -0x18, D_800D5704[D_80121B52], 0, 0x100, 5, 0x29);
    }
    func_80045A78(-0x54, -0x60, func_80043040(D_80112130.unk38), 0x5D);
    func_80045A78(4, -0x60, func_80043040(D_80112130.unk38), 0x5E);
    func_80045A78(-0x4C, -0x5C, func_80043040(D_80112130.unk3A), D_800D57B4[D_80121B50]);
    func_80045A78(4, -0x5C, func_80043040(D_80112130.unk3A), D_800D57B4[D_80121B50] + 1);
}
#endif

void func_80053604(MenuPanelActor *arg0) {
    func_800483FC(&D_80124868, func_80052E70, (s32) arg0);
}

void func_80053634(MenuPanelActor *arg0) {
    func_80071824(arg0, func_80053604);
}
