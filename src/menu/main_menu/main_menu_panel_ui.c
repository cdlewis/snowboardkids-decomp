#include "game/race/race_state.h"
#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/main_menu/main_menu_panel_ui.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"

#define MENU_PANEL_SCROLL_STEP 0x10
#define MENU_PANEL_SCROLL_LIMIT 0x79
#define MENU_PANEL_TILE_SIZE 0x20
#define MENU_PANEL_INPUT_REPEAT_FRAMES 4
#define MENU_PANEL_ACCEPT_SOUND 0x18
#define MENU_PANEL_SOUND_VOLUME 0x32
#define MENU_PANEL_TEXTURE_HANDLE (gAssetHandles[0x2A])

typedef void (*MenuPanelActorCallback)(MenuPanelActor *);


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

extern MenuGlyphScript mainMenuModeSelectTitleText[];
extern u8 mainMenuModeSelectPrimaryIconTiles[];
extern u8 mainMenuModeSelectTopRowIconTiles[];
extern u8 mainMenuModeSelectSecondaryIconTiles[];
extern u8 mainMenuModeSelectBottomRowIconTiles[];
extern u8 mainMenuModeSelectUpTargets[];
extern u8 mainMenuModeSelectDownTargets[];
extern u8 mainMenuModeSelectLeftTargets[];
extern u8 mainMenuModeSelectRightTargets[];
extern MenuGlyphScript raceGhostUnavailableText[];
extern MenuGlyphScript mainMenuSettingsTitleText[];
extern MenuGlyphScript raceRecordSettingsTitleText[];
extern u16 *raceRecordLapCountTexts[];
extern s16 courseRecordDigitTileOffsets[];
extern u16 *mainMenuModeDescriptionTexts[];
extern u16 *mainMenuModeDescriptionTitles[];
extern u8 gMainMenuModeSelection;
extern s16 gRaceLapCount;
extern s16 gMenuFadeAlpha;
extern s32 gMenuFlowState;
extern s16 gFrameCounter;
extern u8 gRaceRecordSettingsEnabled;
extern u8 gRaceCourseModelEffectsDisabled;
extern u8 gRaceCourseOverlayEffectsDisabled;
extern s16 gUiBlinkTimer;

extern void drawMenuFillRectangle(s16, s16, s16, s16, u8, u8, u8);
extern void enqueueSoundEffect(s16, s16);

void drawMainMenuModeSelectFrame(void *arg0) {
    s32 edgeX;
    s32 edgeY;
    s32 y;
    s32 yOrigin;
    s32 xOrigin;

    xOrigin = -0x74;
    yOrigin = 0x54;

    drawMenuSprite(-0x84, -0x64, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    drawMenuSprite(0x78, -0x64, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);

    edgeX = xOrigin;
    do {
        drawMenuSprite(edgeX, -0x64, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, 0);
        drawMenuSprite(edgeX, -0x3C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, 0);
        edgeX += 0x10;
    } while (edgeX < 0x7C);

    drawMenuSprite(-0x84, -0x3C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, 0);
    drawMenuSprite(0x78, -0x3C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, 0);

    edgeY = -yOrigin;
    do {
        drawMenuSprite(-0x84, edgeY, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 5, 0x20, 0x20, 0, 0);
        drawMenuSprite(0x78, edgeY, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 6, 0x20, 0x20, 0, 0);
        edgeY += 0x10;
    } while (edgeY < -0x34);

    if (yOrigin) {
    }

    for (edgeX = xOrigin; edgeX != 0x7C; edgeX += 0x10) {
        for (y = -yOrigin; y != -0x34; y += 0x10) {
            drawMenuSprite(edgeX, y, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, 0);
        }
    }

    drawMenuColoredGlyphScript(-0x68, -0x58, mainMenuModeSelectTitleText, 0, 0x100, 5, 0x29);
}

void drawMainMenuModeSelectIcons(MenuPanelActor *arg0) {
    s32 alpha;
    s32 i;
    s32 next;
    s32 x;
    u8 *tile;

    if (arg0) {
    }

    x = -0x6E;
    tile = mainMenuModeSelectTopRowIconTiles; i = 0; do {
        next = i + 1;
        alpha = 0;
        if (next == gMainMenuModeSelection) {
            alpha = arg0->x;
        }

        drawMenuFillRectangle((s16)(x + 0xA), -0x16, 0x22, 0x22, 0x40, alpha / 4, 0);
        drawMenuFillRectangle((s16)(x + 4), -0x1C, 0x22, 0x22, 0xFF, alpha, 0);
        drawAssetTableSprite((s16)(x + 5), -0x1B, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), *tile);
        i = next;
        tile++;
        x += 0x2C;
    } while (next < 5);

    x = -0x84;
    tile = mainMenuModeSelectBottomRowIconTiles;
    i = 0;
    do {
        alpha = 0;
        if ((i + 6) == gMainMenuModeSelection) {
            alpha = arg0->x;
        }

        drawMenuFillRectangle((s16)(x + 0xA), 0x1E, 0x22, 0x22, 0, alpha / 4, 0x40);
        drawMenuFillRectangle((s16)(x + 4), 0x18, 0x22, 0x22, arg0->x * 0, alpha, 0xFF);
        drawAssetTableSprite((s16)(x + 5), 0x19, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), *tile);
        i++;
        tile++;
        x += 0x2C;
    } while (i != 6);

    if (gMainMenuModeSelection == 0xC) {
        if (arg0->x != 0) {
            drawAssetTableSprite(-0x30, 0x4D, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 2);
            drawPulsingAssetTableSprite(-0x30, 0x4D, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 3);
            return;
        }
        drawAssetTableSpriteWithExplicitPalette(-0x30, 0x4D, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 2, 1);
        drawPulsingAssetTableSprite(-0x30, 0x4D, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 3);
        return;
    }

    drawAssetTableSpriteWithExplicitPalette(-0x30, 0x4D, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 2, 1);
}

void updateMainMenuModeSelectGrid(MenuPanelActor *arg0) {
    s32 temp_v1;
    s32 var_v0;
    u8 temp_a2;
    u8 temp_a2_2;
    u8 temp_a2_3;
    u8 temp_a2_4;

    if (arg0) {
    } if ((gMenuFadeAlpha == 0) && (gMainMenuSelectionResult == 0)) { temp_v1 = gPlayerInputPressed[0]; do { if (temp_v1 & (STICK_UP | U_JPAD)) { temp_a2 = mainMenuModeSelectUpTargets[gMainMenuModeSelection]; if (temp_a2 != gMainMenuModeSelection) { gMainMenuModeSelection = temp_a2; enqueueSoundEffect(0x19, 0x32); temp_v1 = gPlayerInputPressed[0]; } } else if (temp_v1 & (STICK_DOWN | D_JPAD)) { var_v0 = gMainMenuModeSelection; temp_a2_2 = mainMenuModeSelectDownTargets[var_v0]; if (temp_a2_2 != var_v0) { gMainMenuModeSelection = temp_a2_2; enqueueSoundEffect(0x19, 0x32); temp_v1 = gPlayerInputPressed[0]; } } else if (temp_v1 & (STICK_RIGHT | R_JPAD)) { temp_a2_3 = mainMenuModeSelectRightTargets[gMainMenuModeSelection]; if (temp_a2_3 != gMainMenuModeSelection) { gMainMenuModeSelection = temp_a2_3; enqueueSoundEffect(0x19, 0x32); temp_v1 = gPlayerInputPressed[0]; } } else if (temp_v1 & (STICK_LEFT | L_JPAD)) { temp_a2_4 = mainMenuModeSelectLeftTargets[gMainMenuModeSelection]; if (temp_a2_4 != gMainMenuModeSelection) { gMainMenuModeSelection = temp_a2_4; enqueueSoundEffect(0x19, 0x32); temp_v1 = gPlayerInputPressed[0]; } }
            if (temp_v1 & (A_BUTTON | B_BUTTON | START_BUTTON)) {
                gMainMenuSelectionResult = 1;
                if (temp_v1 & B_BUTTON) {
                    gMainMenuModeSelection = 0xC;
                }
                enqueueSoundEffect(0x18, 0x32);
            }
        } while (0);
    }

    if (gMainMenuSelectionResult == 0) {
        arg0->x = gFrameCounter & 0xF; if (arg0->x >= 9) { arg0->x = 0x10 - arg0->x; } arg0->x = (arg0->x * 0x10) + 0x7F;
    } else if (gFrameCounter & 1) {
        arg0->x = 0;
    } else {
        arg0->x = 0xFF;
    }

    addRenderCallback(&gMenuRenderCallbackList, drawMainMenuModeSelectFrame, arg0);
    addRenderCallback(&gMenuRenderCallbackList,(RenderCallback)drawMainMenuModeSelectIcons, (void *)arg0);
}

void initMainMenuModeSelectGrid(MenuPanelActor *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateMainMenuModeSelectGrid);
}

void drawMainMenuModeDescriptionPanel(MenuPanelActor *arg0) {
    s32 i;
    u16 unused;
    u16 lineY;
    s32 visibleIndex;
    s32 scriptIndex;
    MenuGlyphScript glyphText[2];
    u16 lineX;
    u16 colorMode;
    MenuGlyphScript *title;
    MenuGlyphScript *titleCursor;

    drawMenuSprite((s16)(arg0->x - 4), (s16)(arg0->y + 0x14),
                   getRelocatableHeapBlockBase(gAssetHandles[0x2A]), 2, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x + 0xF8), (s16)(arg0->y + 0x14),
                   getRelocatableHeapBlockBase(gAssetHandles[0x2A]), 4, 0x20, 0x20, 0, 0);

    i = 0;
    do {
        drawMenuSprite((s16)(arg0->x + i + 0xC), (s16)(arg0->y + 0x14),
                       getRelocatableHeapBlockBase(gAssetHandles[0x2A]), 3, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)(arg0->x + i + 0xC), (s16)(arg0->y + 0x4C),
                       getRelocatableHeapBlockBase(gAssetHandles[0x2A]), 8, 0x20, 0x20, 0, 0);
        i += 0x10;
    } while (i < 0xF0);

    drawMenuSprite((s16)(arg0->x - 4), (s16)(arg0->y + 0x4C),
                   getRelocatableHeapBlockBase(gAssetHandles[0x2A]), 7, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x + 0xF8), (s16)(arg0->y + 0x4C),
                   getRelocatableHeapBlockBase(gAssetHandles[0x2A]), 9, 0x20, 0x20, 0, 0);

    i = (arg0->selectionState == 4) * 0;
    do {
        drawMenuSprite((s16)(arg0->x - 4), (s16)(arg0->y + i + 0x24),
                       getRelocatableHeapBlockBase(gAssetHandles[0x2A]), 5, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)(arg0->x + 0xF8), (s16)(arg0->y + i + 0x24),
                       getRelocatableHeapBlockBase(gAssetHandles[0x2A]), 6, 0x20, 0x20, 0, 0);
        i += 0x10;
    } while (i <= 0x2F);

    glyphText[1] = 0xFFFF;
    visibleIndex = 0;
    scriptIndex = 0;
    lineX = 0;
    lineY = 0;
    colorMode = 7;

    for (;;) {
        if (visibleIndex == arg0->selectedTile) {
            visibleIndex = 0;
            break;
        }

        glyphText[0] = arg0->tileList[scriptIndex++];
        switch (glyphText[0]) {
        case 0xFFFF:
            arg0->selectionState = 1;
            visibleIndex = arg0->selectedTile;
            continue;
        case 0xFFFD:
            lineY = lineY + 0x10;
            lineX = 0;
            continue;
        case 0xFFFB:
            arg0->selectionState = 2;
            arg0->tileListStart = &arg0->tileList[scriptIndex];
            visibleIndex = arg0->selectedTile;
            continue;
        case 0xFFFC:
            colorMode = arg0->tileList[scriptIndex++];
            continue;
        default:
            drawMenuColoredGlyphScript((s16)(arg0->x + lineX), (s16)(arg0->y + lineY + 0x18), glyphText, 0,
                                       0x100, colorMode, 0x29);
            lineX = lineX + 0x10;
            visibleIndex++;
            continue;
        }
    }

    if ((arg0->selectionState != 0) && (gMainMenuSelectionResult == 0)) {
        drawMenuSprite((s16)(arg0->x + 0xF4), (s16)(arg0->y + 0x48),
                       getRelocatableHeapBlockBase(gAssetHandles[0x2A]), (gFrameCounter >> 4) & 1, 0x20,
                       0x20, 0, 0);
    }

    title = mainMenuModeDescriptionTitles[gMainMenuModeSelection];
    titleCursor = title;
    if (*title != 0xFFFF) {
        do {
            visibleIndex++;
            titleCursor++;
        } while (*titleCursor != 0xFFFF);
    }
    visibleIndex *= 0x10;
    visibleIndex = -(visibleIndex / 2);

    drawMenuColoredGlyphScript((s16)visibleIndex, (s16)(-0x48 - arg0->y), title, 0, 0x100, 4, 0x29);

    if ((s32)gMainMenuModeSelection < 6) {
        drawAssetTableSprite(-0x10, (s16)(-0x30 - arg0->y),
                             getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                             mainMenuModeSelectPrimaryIconTiles[gMainMenuModeSelection]);
        return;
    }

    drawAssetTableSprite(-0x10, (s16)(-0x30 - arg0->y), getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                         mainMenuModeSelectSecondaryIconTiles[gMainMenuModeSelection]);
}

void scrollMainMenuModeDescriptionPanelOut(MenuPanelActor *arg0) {
    arg0->y += MENU_PANEL_SCROLL_STEP;
    if (arg0->y >= MENU_PANEL_SCROLL_LIMIT) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawMainMenuModeDescriptionPanel, arg0);
}

void updateMainMenuModeDescriptionPanel(MenuPanelActor *arg0) {
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

    if (gPlayerInputPressed[0] & (A_BUTTON | START_BUTTON)) {
        state = arg0->selectionState;
        switch (state) {
        case 0:
            arg0->selectedTile = 0xE7;
            break;
        case 1:
            gMainMenuSelectionResult = 1;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)scrollMainMenuModeDescriptionPanelOut);
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

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawMainMenuModeDescriptionPanel, arg0);
}

void initMainMenuModeDescriptionPanel(MenuPanelActor *arg0) {
    arg0->x = -0x80;
    arg0->y = 4;
    arg0->selectionState = 0;
    arg0->inputRepeatTimer = 0;
    arg0->tileList = mainMenuModeDescriptionTexts[gMainMenuModeSelection];
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateMainMenuModeDescriptionPanel);
}

void drawMainMenuModeSelectMenuOptions(void *arg0) {
    if (gMenuFlowState == 0) {
        if ((gMainMenuSelectionResult == 0) || (gFrameCounter & 1)) {
            drawAssetTableSprite(-0x30, -0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0);
            drawPulsingAssetTableSprite(-0x30, -0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 3);
        } else {
            drawAssetTableSpriteWithExplicitPalette(-0x30, -0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0, 1);
            drawPulsingAssetTableSprite(-0x30, -0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 3);
        }
    } else {
        drawAssetTableSpriteWithExplicitPalette(-0x30, -0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0, 1);
    }

    if (gMenuFlowState == 1) {
        if ((gMainMenuSelectionResult == 0) || (gFrameCounter & 1)) {
            drawAssetTableSprite(-0x30, -8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 1);
            drawPulsingAssetTableSprite(-0x30, -8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 3);
        } else {
            drawAssetTableSpriteWithExplicitPalette(-0x30, -8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 1, 1);
            drawPulsingAssetTableSprite(-0x30, -8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 3);
        }
    } else {
        drawAssetTableSpriteWithExplicitPalette(-0x30, -8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 1, 1);
    }

    if (gMenuFlowState == 2) {
        if ((gMainMenuSelectionResult == 0) || (gFrameCounter & 1)) {
            drawAssetTableSprite(-0x30, 0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 2);
            drawPulsingAssetTableSprite(-0x30, 0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 3);
            return;
        }
        drawAssetTableSpriteWithExplicitPalette(-0x30, 0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 2, 1);
        drawPulsingAssetTableSprite(-0x30, 0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 3);
        return;
    }

    drawAssetTableSpriteWithExplicitPalette(-0x30, 0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 2, 1);
}

void updateMainMenuModeSelectMenuOptions(MenuPanelActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, drawMainMenuModeSelectMenuOptions, arg0);
}

void initMainMenuModeSelectMenuOptions(MenuPanelActor *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateMainMenuModeSelectMenuOptions);
}

void drawRaceSetupCornerPrompts(void *arg0) {
    drawAssetTableSpriteWithDefaultPalette(-0x84, -0x64, getRelocatableHeapBlockBase(gAssetHandles[6]), 6);
    drawAssetTableSpriteWithDefaultPalette(0x74, -0x64, getRelocatableHeapBlockBase(gAssetHandles[6]), 7);
    drawAssetTableSpriteWithDefaultPalette(-0x84, 4, getRelocatableHeapBlockBase(gAssetHandles[6]), 8);
    drawAssetTableSpriteWithDefaultPalette(0x74, 4, getRelocatableHeapBlockBase(gAssetHandles[6]), 9);
}

void updateRaceSetupCornerPrompts(MenuPanelActor *arg0) {
    if (gMainMenuSelectionResult == 0) {
        addRenderCallback(&gMenuRenderCallbackList, drawRaceSetupCornerPrompts, arg0);
        return;
    }
    removeCallbackTask(arg0);
}

void initRaceSetupCornerPrompts(MenuPanelActor *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSetupCornerPrompts);
}

void drawRaceGhostUnavailableMessage(void *arg0) {
    drawMenuColoredGlyphScript(-0x78, -0x58, raceGhostUnavailableText, 1, 0x100, 5, 0x28);
}

void updateRaceGhostUnavailableMessage(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceGhostUnavailableMessage, arg0);
}

void drawMainMenuSettingsPanel(void *arg0) {
    s32 y;
    s32 edgeX;
    s32 edgeY;
    s32 yOrigin;
    s32 xOrigin;

    xOrigin = -0x74;
    yOrigin = 0x54;

    drawMenuSprite(-0x84, -0x64, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    drawMenuSprite(0x78, -0x64, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);

    edgeX = xOrigin;
    do {
        drawMenuSprite(edgeX, -0x64, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, 0);
        drawMenuSprite(edgeX, -0x2C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, 0);
        edgeX += 0x10;
    } while (edgeX < 0x7C);

    drawMenuSprite(-0x84, -0x2C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, 0);
    drawMenuSprite(0x78, -0x2C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, 0);

    edgeY = -yOrigin;
    do {
        drawMenuSprite(-0x84, edgeY, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 5, 0x20, 0x20, 0, 0);
        drawMenuSprite(0x78, edgeY, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 6, 0x20, 0x20, 0, 0);
        edgeY += 0x10;
    } while (edgeY < -0x24);

    if (yOrigin) {
    }

    for (edgeX = xOrigin; edgeX != 0x7C; edgeX += 0x10) {
        for (y = -yOrigin; y != -0x24; y += 0x10) {
            drawMenuSprite(edgeX, y, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, 0);
        }
    }
    drawMenuColoredGlyphScript(-0x76, -0x58, mainMenuSettingsTitleText, 0, 0x100, 5, 0x29);
}

void drawMainMenuSettingsOptions(void *arg0) {
    s32 var_v0;
    s32 sp28;

    var_v0 = 1;
    sp28 = var_v0;
    if (gMenuFlowState == 0) {
        var_v0 = 2;
        sp28 = var_v0;
    }
    drawAssetTableSpriteWithExplicitPalette(-0x78, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 4, sp28);
    sp28 = 1;
    if (gRaceRecordSettingsEnabled == 1) {
        drawAssetTableSpriteWithExplicitPalette(0x10, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 7, 2);
        drawAssetTableSpriteWithExplicitPalette(0x48, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 8, 1);
        if (gMenuFlowState == 0) {
            drawPulsingAssetTableSprite(0x10, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        } else {
            drawAssetTableSprite(0x10, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        }
    } else {
        drawAssetTableSpriteWithExplicitPalette(0x10, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 7, 1);
        drawAssetTableSpriteWithExplicitPalette(0x48, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 8, 2);
        if (gMenuFlowState == 0) {
            drawPulsingAssetTableSprite(0x48, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        } else {
            drawAssetTableSprite(0x48, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        }
    }
    if (gMenuFlowState == 1) {
        sp28 = 2;
    }
    drawAssetTableSpriteWithExplicitPalette(-0x78, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 5, sp28);
    sp28 = 1;
    if (gRaceCourseModelEffectsDisabled == 0) {
        drawAssetTableSpriteWithExplicitPalette(0x10, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 9, 2);
        drawAssetTableSpriteWithExplicitPalette(0x48, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xA, 1);
        if (gMenuFlowState == 1) {
            drawPulsingAssetTableSprite(0x10, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        } else {
            drawAssetTableSprite(0x10, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        }
    } else {
        drawAssetTableSpriteWithExplicitPalette(0x10, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 9, 1);
        drawAssetTableSpriteWithExplicitPalette(0x48, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xA, 2);
        if (gMenuFlowState == 1) {
            drawPulsingAssetTableSprite(0x48, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        } else {
            drawAssetTableSprite(0x48, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        }
    }
    if (gMenuFlowState == 2) {
        sp28 = 2;
    }
    drawAssetTableSpriteWithExplicitPalette(-0x78, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 6, sp28);
    sp28 = 1;
    if (gRaceCourseOverlayEffectsDisabled == 0) {
        drawAssetTableSpriteWithExplicitPalette(0x10, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 9, 2);
        drawAssetTableSpriteWithExplicitPalette(0x48, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xA, 1);
        if (gMenuFlowState == 2) {
            drawPulsingAssetTableSprite(0x10, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        } else {
            drawAssetTableSprite(0x10, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        }
    } else {
        drawAssetTableSpriteWithExplicitPalette(0x10, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 9, 1);
        drawAssetTableSpriteWithExplicitPalette(0x48, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xA, 2);
        if (gMenuFlowState == 2) {
            drawPulsingAssetTableSprite(0x48, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        } else {
            drawAssetTableSprite(0x48, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        }
    }
    if (gMenuFlowState == 3) {
        sp28 = 2;
    }
    drawAssetTableSpriteWithExplicitPalette(-0x68, 0x48, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 2, sp28);
}

void updateMainMenuSettingsPanel(MenuPanelActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, drawMainMenuSettingsPanel, arg0);
    addRenderCallback(&gMenuRenderCallbackList, drawMainMenuSettingsOptions, arg0);
}

void initMainMenuSettingsPanel(MenuPanelActor *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateMainMenuSettingsPanel);
}

void drawRaceRecordSettingsPanel(void *arg0) {
    s32 x;
    s32 x2;
    s16 xStart;
    s32 y1;
    s32 y;
    s32 y2;

    xStart = 0x74;

    drawMenuSprite(-0x84, 0x1C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    drawMenuSprite(0x78, 0x1C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);
    x = -xStart;
    do {
        drawMenuSprite((s16)x, 0x1C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)x, 0x54, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, 0);
        x += 0x10;
    } while (x < 0x7C);
    drawMenuSprite(-0x84, 0x54, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, 0);
    drawMenuSprite(0x78, 0x54, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, 0);
    y1 = 0x2C;
    do {
        drawMenuSprite(-0x84, (s16)y1, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 5, 0x20, 0x20, 0, 0);
        drawMenuSprite(0x78, (s16)y1, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 6, 0x20, 0x20, 0, 0);
        y1 = (y1 + 0x10) * 1;
    } while (y1 < 0x5C);
    for (x = -0x74; x < 0x7C; x += 0x10) {
        for (y = 0x2C; y != 0x5C; y += 0x10) {
            drawMenuSprite((s16)x, (s16)y, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, 0);
        }
    }
    drawMenuColoredGlyphScript(-0x76, 0x28, raceRecordSettingsTitleText, 0, 0x100, 7, 0x29);
    drawMenuSprite(-0x40, -0x30, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    drawMenuSprite(0x30, -0x30, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);
    x2 = -0x30;
    do {
        drawMenuSprite((s16)x2, -0x30, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)x2, 0, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, 0);
        x2 = (x2 + 0x10) * 1;
    } while (x2 < 0x30);
    y2 = -0x20;
    do {
        drawMenuSprite(-0x40, (s16)y2, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 5, 0x20, 0x20, 0, 0);
        drawMenuSprite(0x30, (s16)y2, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 6, 0x20, 0x20, 0, 0);
        y2 += 0x10;
    } while ((y2 - 0) < 0);
    for (x2 = -0x30; x2 != 0x30; x2 += 0x10) {
        for (y = -0x20; y != 0; y += 0x10) {
            drawMenuSprite((s16)x2, (s16)y, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, 0);
        }
    }
    drawMenuSprite(-0x40, 0, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, 0);
    drawMenuSprite(0x30, 0, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, 0);
    if (gMenuFlowState == 0) {
        if (gRaceLapCount != 1) {
            if (gUiBlinkTimer & 8) {
                drawAssetTableSprite(-0x30, -0x2A, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xC);
            }
        }
        if ((gRaceLapCount != 9) && (gUiBlinkTimer & 8)) {
            drawAssetTableSprite(-0x30, -6, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xD);
        }
    }
    if ((gMenuFlowState != 0) && (gUiBlinkTimer & 1)) {
        drawMenuColoredGlyphScript(-0x30, -0x18, raceRecordLapCountTexts[gRaceLapCount], 0, 0x100, 0, 0x29);
    } else {
        drawMenuColoredGlyphScript(-0x30, -0x18, raceRecordLapCountTexts[gRaceLapCount], 0, 0x100, 5, 0x29);
    }
    drawAssetTableSprite(-0x54, -0x60, getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x5D);
    drawAssetTableSprite(4, -0x60, getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x5E);
    drawAssetTableSprite(-0x4C, -0x5C, getRelocatableHeapBlockBase(gAssetHandles[0x1D]),
                         (u16)courseRecordDigitTileOffsets[gRaceCourseIndex.signedValue]);
    drawAssetTableSprite(4, -0x5C, getRelocatableHeapBlockBase(gAssetHandles[0x1D]),
                         courseRecordDigitTileOffsets[gRaceCourseIndex.signedValue] + 1);
}

void updateRaceRecordSettingsPanel(MenuPanelActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, drawRaceRecordSettingsPanel, arg0);
}

void initRaceRecordSettingsPanel(MenuPanelActor *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceRecordSettingsPanel);
}
