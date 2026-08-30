#include "common.h"
#include <PR/os_libc.h>
#include "font_encoding.h"
#include "game/save_data.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/game_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/menu/splitscreen_select/race_splitscreen_select_ui.h"
#include "game/menu/course_select/course_select_menu.h"
#include "game/menu/course_select/course_select_shop_ui.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/race/player/race_player_input.h"

typedef MenuGlyphScript ShopDescriptionText[0x46];

typedef struct {
    /* 0x00 */ u16 center[16];
    /* 0x20 */ u16 right[2];
    /* 0x24 */ u16 bottom[2];
    /* 0x28 */ u16 corner;
} ShopMenuFrameTileMap;

typedef Vec2s ShopMenuSparklePattern[13];

enum {
    COURSE_DETAILS_PAGE_COUNT = 2,
    COURSE_DETAILS_COURSES_PER_PAGE = 7,
    COURSE_DETAILS_OPTION_COUNT = 10,
    COURSE_DETAILS_ROW_SPACING = 0x13
};

typedef u16 CourseDetailsMenuEntryTilePages[COURSE_DETAILS_PAGE_COUNT][COURSE_DETAILS_COURSES_PER_PAGE];

typedef u16 CourseNumberGlyphScript[5];

typedef enum {
    COURSE_DETAILS_STATE_REVEAL_LIST = 0,
    COURSE_DETAILS_STATE_FADE_IN_CURSOR = 1,
    COURSE_DETAILS_STATE_IDLE = 2,
    COURSE_DETAILS_STATE_SLIDE_PAGE_OUT = 3,
    COURSE_DETAILS_STATE_SLIDE_PAGE_IN = 4,
    COURSE_DETAILS_STATE_EXIT = 5,
    COURSE_DETAILS_STATE_RESET_CURSOR = 6,
    COURSE_DETAILS_STATE_DONE = 7
} CourseDetailsMenuState;

ShopMenuFrameTileMap gShopMenuPanelFrameTilemaps[2] = {
    0x001A, 0x001B, 0x001C, 0x001D, 0x001F, 0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0028, 0x0029,
    0x002A, 0x002B, 0x001E, 0x0027, 0x002C, 0x002D, 0x002E, 0x00A4, 0x00A5, 0x00A6, 0x00A7, 0x00A9, 0x00AA, 0x00AB,
    0x00AC, 0x00AD, 0x00AE, 0x00AF, 0x00B0, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00A8, 0x00B1, 0x00B6, 0x00B7, 0x00B8,
};

ShopDescriptionText gShopMenuModeDescriptionText[5] = {
    {
        // textconv requires these _() invocations to retain their original line layout.
        // clang-format off
        _("{COLOR:6}BUY ONE{COLOR:7}.\nYOU CAN'T\nBUY ONE YOU\nALREADY\nHAVE."),
    },
    {
        _("{COLOR:6}CHANGE\nCOLOR{COLOR:7}.\nFREE OF\nCHARGE."),
    },
    {
        _("{COLOR:7}LEAVE SHOP\nAND RETURN\nTO THE\nMAIN MENU."),
    },
    {
        _("{COLOR:7}WELCOME!"),
    },
    {
        _("{COLOR:7}WE HAVE\nGREAT\nBOARDS IN\nSTOCK. WANNA\nBUY ONE?"),
    },
};

MenuGlyphScript gCourseUnlockPurchasePromptText[24] = {
    _("IS THIS OK?"), _("BOARD SHOP"),
    // clang-format on
};

CourseDetailsMenuEntryTilePages gCourseDetailsMenuEntryTiles = {
    0x000F, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0016, 0x0017, 0x0018, 0x000C, 0x000D, 0x000E, 0x0015, 0x0019,
};

ShopMenuSparklePattern gShopMenuSparklePatterns[5] = {
    0x0000, 0x0000, 0xFFF0, 0x0010, 0xFFE0, 0x0020, 0x0000, 0x0020, 0x0020, 0x0020, 0xFFE0, 0x0040, 0x0000,
    0x0040, 0xFFF0, 0x0060, 0xFFF0, 0x0070, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0010, 0xFFE0, 0x0020, 0xFFF0, 0x0020, 0xFFF0, 0x0030, 0xFFF0, 0x0040, 0xFFF0,
    0x0050, 0x0010, 0x0050, 0xFFF0, 0x0070, 0x0010, 0x0070, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0010, 0xFFF0, 0xFFE0, 0x0010, 0xFFF0, 0x0010, 0x0010, 0x0010, 0xFFF0, 0x0030, 0x0010,
    0x0030, 0xFFF0, 0x0050, 0x0010, 0x0050, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0010, 0xFFF0, 0x0030, 0xFFF0, 0x0010, 0x0010, 0x0030, 0x0010, 0x0020, 0x0030, 0x0010,
    0x0040, 0x0010, 0x0050, 0x0030, 0x0050, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0020, 0x0000, 0xFFF0, 0x0020, 0x0000, 0x0020, 0x0020, 0x0020, 0x0040, 0x0000, 0x0000,
    0x0040, 0x0020, 0x0040, 0x0010, 0x0060, 0x0010, 0x0070, 0x0040, 0x0040, 0x0010, 0x0080, 0xFFFF, 0xFFFF,
};

s16 gShopMenuSparkleInitTable[16] = {
    0x0058, 0xFFC8, 0x0045, 0x0058, 0xFFB8, 0x002F, 0x0058, 0xFFD8,
    0x004E, 0x0038, 0xFFD8, 0x0057, 0x0038, 0xFFB8, 0x0039, 0x0000,

};

Vec2s gCoursePreviewCloseSparkleOffsets[6] = {
    0x0003, 0xFFFC, 0x000C, 0xFFFE, 0x0013, 0xFFFD, 0x001D, 0xFFFD, 0x0023, 0xFFFF, 0x002A, 0xFFFB,
};

Vec2s gCoursePreviewCloseSparkleMirrorStart[2] = {
    0x0000,
    0x0000,
    0x0000,
    0x0000,
};

extern u8 gMenuExitSelection;

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void drawShopMenuModeChoiceRows(ShopMenuRowActor *actor) {
    ShopMenuRowActor *savedActor;
    ShopMenuRowActor *rows;
    s32 rowIndex;
    s32 blinkAlpha;
    s32 yOffset;

    savedActor = actor;
    rowIndex = 0;
    if (actor->visibleRowCount > 0) {
        /* Keeping the loop setup together preserves IDO's original scheduling. */
        yOffset = 0; rows = actor; do {
            blinkAlpha = 0;
            if ((gMenuSelectionConfirmTimer > 0) && (gMenuSelectionConfirmTimer < 8) &&
                (gMenuExitSelection == 0) && (rowIndex == gCourseSelectModeSelection) &&
                (gMenuSelectionConfirmTimer & 1)) {
                blinkAlpha = 0xFF;
            }
            drawMenuSprite(rows->rowXPositions[rowIndex], (s16)(actor->baseY + yOffset),
                           getRelocatableHeapBlockBase(gAssetHandles[0x27]),
                           (u16)(rowIndex + 2), 0x20, 0x20, 0, blinkAlpha);
            yOffset += 0x1C;
            rowIndex += 1;
        } while (rowIndex < savedActor->visibleRowCount);
    }
}
// clang-format on

extern u16 gCourseDetailsPreviewCourseTiles[];
extern u16 gCourseDetailsPreviewExtraTiles[];
extern Vec2s gCoursePreviewCloseSparkleOffsetsEnd[];
extern u8 gShopMenuDescriptionSeen;
extern u8 gShopMenuShowNewCoursesMessage;
extern s16 gCoursePreviewViewportHeight;
extern s32 gMenuFlowState;

const char gShopMenuMoneyFormat[] = "%6dG";
const char gCourseUnlockPriceFormat6[] = "%6dG";
const char gCourseUnlockPriceFormat5[] = "%5dG";
const char gCourseUnlockPriceFormat4[] = "%4dG";

void updateShopMenuModeChoiceRows(ShopMenuRowActor *arg0) {
    s32 i;
    s32 moved;
    ShopMenuRowActor *spawnRow;
    s32 state;
    int stateByte;
    ShopMenuRowActor *row;
    ShopMenuRowActor *actor;

    stateByte = arg0->state;
    actor = arg0;
    state = stateByte;
    row = arg0;
    switch (state) {
        case 0:
            moved = 0;
            for (i = 0; i < row->visibleRowCount; i++) {
                if (row->rowXPositions[i] < -0x7C) {
                    row->rowXPositions[i] += 0x10;
                    moved++;
                    if (row->rowXPositions[i] >= -0x7C) {
                        row->rowXPositions[i] = -0x7C;
                    }
                }
            }
            row->revealTimer++;
            spawnRow = row;
            if (!(spawnRow->revealTimer & 1)) {
                if (spawnRow->visibleRowCount < 3) {
                    spawnRow->visibleRowCount++;
                }
            }
            if (moved == 0) {
                spawnRow->state = 1;
                createCallbackTask((CallbackTaskCallback)initShopMenuSelectedModePanel, 0, 0x5F);
                createCallbackTask((CallbackTaskCallback)initShopMenuCourseListPanel, 0, 0x61);
            }
            state = arg0->state;
            break;
        case 1:
            if (gRacePlayers[0].menuState == 1) {
                state = (u8)(arg0->state = 2);
            }
            break;
        case 2:
            for (i = 0; i < 5; i++) {
                arg0->rowXPositions[i] -= 0x20;
            }
            if (arg0->rowXPositions[0] < -0x103) {
                arg0->state = 3;
            }
            break;
        case 3:
            break;
    }

    if (arg0->state == 3) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawShopMenuModeChoiceRows, actor);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initShopMenuModeChoiceRows(ShopMenuRowActor *arg0) {
    s32 i;

    for (i = 0; i < 5; i++) { arg0->rowXPositions[i] = -0x104; }

    arg0->baseY = -0x50;
    arg0->revealTimer = 0;
    arg0->visibleRowCount = 1;
    arg0->state = 0;

    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateShopMenuModeChoiceRows);
}
// clang-format on

void drawShopMenuSidePanel(ShopMenuWidgetActor *arg0) {
    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(gAssetHandles[0x25]), 3, 0x20, 0x20, 0, 0);
    drawMenuSprite(
        (s16)(arg0->x + 0x40),
        arg0->y,
        getRelocatableHeapBlockBase(gAssetHandles[0x25]),
        4,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        arg0->x,
        (s16)(arg0->y + 0x40),
        getRelocatableHeapBlockBase(gAssetHandles[0x25]),
        5,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0x40),
        (s16)(arg0->y + 0x40),
        getRelocatableHeapBlockBase(gAssetHandles[0x25]),
        6,
        0x20,
        0x20,
        0,
        0
    );
}

void updateShopMenuSidePanel(ShopMenuWidgetActor *arg0) {
    u8 state = arg0->sprite.bytes.state;

    switch (state) {
        case 0:
            arg0->x += 0x20;
            if (arg0->x >= -0x88) {
                arg0->x = -0x88;
                arg0->sprite.bytes.state = 1;
            }
            state = arg0->sprite.bytes.state;
            break;
        case 1:
            if (gRacePlayers[0].menuState == 1) {
                state = arg0->sprite.bytes.state = 2;
            }
            break;
        case 2:
            arg0->x -= 0x20;
            if (arg0->x < -0x10D) {
                arg0->sprite.bytes.state = 3;
            }
            state = arg0->sprite.bytes.state;
            break;
        case 3:
            break;
    }
    if ((unsigned int)state == 3) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawShopMenuSidePanel, arg0);
}

void initShopMenuSidePanel(ShopMenuWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 8;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateShopMenuSidePanel);
}

void drawShopMenuSelectedModePanel(ShopMenuWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;
    s32 borderOffset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTile(
            (s16)(arg0->x + ((i & 3) << 5)),
            (s16)(arg0->y + ((i / 4) << 5)),
            getRelocatableHeapBlockBase(gAssetHandles[0x27]),
            gShopMenuPanelFrameTilemaps[(u16)arg0->item.counter].center[tileOffset],
            0,
            0x100
        );
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    if ((i && i) && i) {}
    do {
        drawMenuSpriteTile(
            (s16)(arg0->x + 0x80),
            (s16)(arg0->y + offset),
            getRelocatableHeapBlockBase(gAssetHandles[0x27]),
            gShopMenuPanelFrameTilemaps[(u16)arg0->item.counter].right[tileOffset],
            0,
            0x100
        );
        i = 0x80;
        drawMenuSpriteTile(
            (s16)(arg0->x + offset),
            (s16)(arg0->y + 0x80),
            getRelocatableHeapBlockBase(gAssetHandles[0x27]),
            gShopMenuPanelFrameTilemaps[(u16)arg0->item.counter].bottom[tileOffset],
            0,
            0x100
        );
        offset += 0x40;
        tileOffset++;
    } while (offset < 0x80);
    i++;
    i--;

    drawMenuSpriteTile(
        (s16)(arg0->x + 0x80),
        (s16)(arg0->y + 0x80),
        getRelocatableHeapBlockBase(gAssetHandles[0x27]),
        gShopMenuPanelFrameTilemaps[(u16)arg0->item.counter].corner,
        0,
        0x100
    );

    drawMenuSprite(
        (s16)(arg0->x - 4),
        (s16)(arg0->y - 4),
        getRelocatableHeapBlockBase(gAssetHandles[0x25]),
        0x33,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x - 4),
        (s16)(arg0->y + 0x8C),
        getRelocatableHeapBlockBase(gAssetHandles[0x25]),
        0x38,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0x8C),
        (s16)(arg0->y - 4),
        getRelocatableHeapBlockBase(gAssetHandles[0x25]),
        0x35,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0x8C),
        (s16)(arg0->y + 0x8C),
        getRelocatableHeapBlockBase(gAssetHandles[0x25]),
        0x3A,
        0x20,
        0x20,
        0,
        0
    );

    for (borderOffset = 0; borderOffset != 0x80; borderOffset += 0x10) {
        drawMenuSprite(
            (s16)((arg0->x + borderOffset) + 0xC),
            (s16)(arg0->y - 4),
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            0x34,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)((arg0->x + borderOffset) + 0xC),
            (s16)(arg0->y + 0x8C),
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            0x39,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)(arg0->x - 4),
            (s16)((arg0->y + borderOffset) + 0xC),
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            0x36,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)(arg0->x + 0x8C),
            (s16)((arg0->y + borderOffset) + 0xC),
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            0x37,
            0x20,
            0x20,
            0,
            0
        );
    }
}

void updateShopMenuSelectedModePanel(ShopMenuWidgetActor *arg0) {
    int state;

    if ((gCourseSelectModeSelection >= (u16)arg0->item.counter) && (arg0->item.bytes.subState != 0) &&
        (arg0->y != -0x48)) {
        state = arg0->item.bytes.subState = 2;
    } else {
        state = arg0->item.bytes.subState;
        if ((gCourseSelectModeSelection < (u16)arg0->item.counter) && (state != 0) && (arg0->y != -0x140)) {
            state = arg0->item.bytes.subState = 1;
        } else {
            state = arg0->item.bytes.subState;
            if ((state != 0) && (state < 4)) {
                state = arg0->item.bytes.subState = 3;
            }
        }
    }

    switch (state) {
        case 0:
            arg0->x -= 0x20;
            if (arg0->item.bytes.subTimer == 0) {
                createCallbackTask((CallbackTaskCallback)initShopMenuSidePanel, 0, 0x63);
            }
            arg0->item.bytes.subTimer++;
            if (arg0->x < -7) {
                arg0->x = -8;
                arg0->item.bytes.subState = 3;
                createCallbackTask((CallbackTaskCallback)initShopMenuDescriptionText, 0, 0x64);
                gActiveMenuTask = createCallbackTask((CallbackTaskCallback)initShopMenuModeCursor, 0, 0x64);
                createCallbackTask((CallbackTaskCallback)initShopMenuPromptPanel, 0, 0x64);
                createCallbackTask((CallbackTaskCallback)initShopMenuSparkles, 0, 0x60);
                createCallbackTask((CallbackTaskCallback)initShopMenuMoneyPanel, 0, 0x64);
            }
            state = arg0->item.bytes.subState;
            break;
        case 1:
            arg0->y -= 0x24;
            if (arg0->y < -0x13F) {
                arg0->y = -0x140;
                arg0->item.bytes.subState = 3;
            }
            state = arg0->item.bytes.subState;
            break;
        case 2:
            arg0->y += 0x24;
            if (arg0->y >= -0x48) {
                arg0->y = -0x48;
                arg0->item.bytes.subState = 3;
            }
            state = arg0->item.bytes.subState;
            break;
        case 3:
            gMenuFlowState += 1;
            if (gRacePlayers[0].menuState == 1) {
                arg0->item.bytes.subState = 4;
            }
            state = arg0->item.bytes.subState;
            break;
        case 4:
            arg0->x += 0x20;
            if (arg0->x >= 0xA0) {
                arg0->item.bytes.subState = 5;
            }
            state = arg0->item.bytes.subState;
            break;
        case 5:
            gRacePlayers[0].menuState = 2;
            state = arg0->item.bytes.subState;
            break;
    }

    if ((state == 5) && (gRacePlayers[0].menuState == 2)) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawShopMenuSelectedModePanel, arg0);
}

void initShopMenuSelectedModePanel(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x94;
    arg0->y = -0x48;
    arg0->sprite.index = -8;
    arg0->transition.alpha = -0x74;
    arg0->item.counter = 0;
    arg0->item.bytes.subTimer = 0;
    arg0->item.bytes.subState = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateShopMenuSelectedModePanel);
}

void drawShopMenuUnselectedModePanel(ShopMenuWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(
            arg0->x + ((i & 3) << 5),
            arg0->y + ((i / 4) << 5),
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            gShopMenuPanelFrameTilemaps[(u16)arg0->sprite.index].center[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped(
            arg0->x + 0x80,
            arg0->y + offset,
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            gShopMenuPanelFrameTilemaps[(u16)arg0->sprite.index].right[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        drawMenuSpriteTileClipped(
            arg0->x + offset,
            arg0->y + 0x80,
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            gShopMenuPanelFrameTilemaps[(u16)arg0->sprite.index].bottom[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped(
        arg0->x + 0x80,
        arg0->y + 0x80,
        getRelocatableHeapBlockBase(gAssetHandles[0x25]),
        gShopMenuPanelFrameTilemaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        0xA0,
        0x49
    );
}

void updateShopMenuUnselectedModePanel(ShopMenuWidgetActor *arg0) {
    int state;

    if ((gCourseSelectModeSelection >= (u16)arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->item.bytes.state = 2;
    } else if ((gCourseSelectModeSelection < (u16)arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->item.bytes.state = 1;
    } else {
        state = arg0->item.bytes.state;
        if (state < 4) {
            state = arg0->item.bytes.state = 3;
        }
    }

    switch (state) {
        case 0:
        case 5:
            break;
        case 1:
            arg0->y -= 0x24;
            if (arg0->y < -0x13F) {
                arg0->y = -0x140;
                arg0->item.bytes.state = 3;
            }
            state = arg0->item.bytes.state;
            break;
        case 2:
            arg0->y += 0x24;
            if (arg0->y >= -0x48) {
                arg0->y = -0x48;
                arg0->item.bytes.state = 3;
            }
            state = arg0->item.bytes.state;
            break;
        case 3:
            gMenuFlowState += 1;
            if (gRacePlayers[0].menuState == 1) {
                if (arg0->y == -0x140) {
                    arg0->item.bytes.state = 5;
                } else {
                    arg0->item.bytes.state = 4;
                }
            }
            state = arg0->item.bytes.state;
            break;
        case 4:
            arg0->x += 0x20;
            if (arg0->x >= 0xA0) {
                arg0->item.bytes.state = 5;
            }
            state = arg0->item.bytes.state;
            break;
    }

    if (state == 5) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawShopMenuUnselectedModePanel, arg0);
}

void initShopMenuUnselectedModePanel(ShopMenuWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.index = 1;
    arg0->item.bytes.timer = 0;
    arg0->item.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateShopMenuUnselectedModePanel);
}

void drawShopMenuCourseListPanel(ShopMenuWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(
            arg0->x + ((i & 3) << 5),
            arg0->y + ((i / 4) << 5),
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            gShopMenuPanelFrameTilemaps[(u16)arg0->sprite.index - 1].center[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped(
            arg0->x + 0x80,
            arg0->y + offset,
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            gShopMenuPanelFrameTilemaps[(u16)arg0->sprite.index - 1].right[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        drawMenuSpriteTileClipped(
            arg0->x + offset,
            arg0->y + 0x80,
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            gShopMenuPanelFrameTilemaps[(u16)arg0->sprite.index - 1].bottom[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped(
        arg0->x + 0x80,
        arg0->y + 0x80,
        getRelocatableHeapBlockBase(gAssetHandles[0x25]),
        gShopMenuPanelFrameTilemaps[(u16)arg0->sprite.index - 1].corner,
        0,
        0x100,
        0xA0,
        0x49
    );
}

void updateShopMenuCourseListPanel(ShopMenuWidgetActor *arg0) {
    u8 *stateField;
    int state;

    stateField = &arg0->item.bytes.state;

    if ((gCourseSelectModeSelection == 2) && (arg0->y != -0x48) && (arg0->item.bytes.state < 6)) {
        state = arg0->item.bytes.state = 2;
    } else {
        state = arg0->item.bytes.state;
        if ((gCourseSelectModeSelection != 2) && (arg0->y != -0x140) && (state < 6)) {
            state = arg0->item.bytes.state = 1;
        } else if (state < 4) {
            state = arg0->item.bytes.state = 3;
        }
    }

    switch (state) {
        case 0:
        case 4:
            break;
        case 1:
            arg0->y -= 0x24;
            if (arg0->y < -0x13F) {
                arg0->y = -0x140;
                arg0->item.bytes.state = 3;
            }
            state = arg0->item.bytes.state;
            break;
        case 2:
            arg0->y += 0x24;
            if (arg0->y >= -0x48) {
                arg0->y = -0x48;
                arg0->item.bytes.state = 6;
                arg0->transition.counter = 8;
            }
            state = arg0->item.bytes.state;
            break;
        case 3:
            gMenuFlowState += 1;
            if (gRacePlayers[0].menuState == 1) {
                arg0->item.bytes.state = 5;
            }
            state = arg0->item.bytes.state;
            break;
        case 5:
            arg0->x += 0x20;
            state = *stateField;
            break;
        case 6:
            arg0->y -= arg0->transition.counter;
            arg0->item.bytes.state = 7;
            state = arg0->item.bytes.state;
            break;
        case 7:
            arg0->y += arg0->transition.counter;
            arg0->transition.counter = arg0->transition.counter / 2;
            if (arg0->transition.counter == 0) {
                arg0->item.bytes.state = 3;
            } else {
                arg0->item.bytes.state = 6;
            }
            state = arg0->item.bytes.state;
            break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawShopMenuCourseListPanel, arg0);
}

void initShopMenuCourseListPanel(ShopMenuWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.index = 2;
    arg0->item.bytes.timer = 0;
    arg0->item.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateShopMenuCourseListPanel);
}

void drawShopMenuModeCursor(ShopMenuWidgetActor *arg0) {
    if (arg0->transition.bytes.state != 5) {
        drawMenuSpriteWithAlpha(
            arg0->x,
            (s16)(arg0->y + (gCourseSelectModeSelection * 0x1C)),
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            7,
            0x20,
            0x20,
            0,
            arg0->sprite.index,
            0
        );
    }
}

void updateShopMenuModeCursor(ShopMenuWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = COURSE_SELECT_STATUS_LAYOUT.cursorState)) {
        arg0->transition.bytes.state = globalState;
        /* Preserve IDO's state/globalState register allocation. */
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        state = globalState;
        arg0->sprite.index = COURSE_SELECT_STATUS_LAYOUT.cursorValue;
        arg0->transition.bytes.timer = COURSE_SELECT_STATUS_LAYOUT.purchaseMessageState;
    }

    switch (state) {
        case 0:
            arg0->sprite.index += 0x26;
            if (arg0->sprite.index >= 0x100) {
                arg0->sprite.index = 0x100;
                arg0->transition.bytes.state = 1;
            }
            state = arg0->transition.bytes.state;
            break;
        case 1:
            if ((s32)arg0->transition.bytes.timer < 0x10) {
                arg0->sprite.index -= 9;
            } else {
                arg0->sprite.index += 9;
            }
            state = arg0->transition.bytes.state;
            arg0->transition.bytes.timer = (arg0->transition.bytes.timer + 1) & 0x1F;
            break;
        case 2:
            if (gRacePlayers[0].menuState == 1) {
                state = arg0->transition.bytes.state = 3;
            }
            break;
        case 3:
            arg0->x -= 0x20;
            if (arg0->x < -0xEF) {
                arg0->transition.bytes.state = 4;
            }
            state = arg0->transition.bytes.state;
            break;
        case 4:
        case 5:
            break;
    }

    gCourseSelectCursorState = state;
    if (arg0->transition.bytes.state == 4) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawShopMenuModeCursor, arg0);
}

void initShopMenuModeCursor(ShopMenuWidgetActor *arg0) {
    arg0->x = -0x7C;
    arg0->y = -0x50;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    arg0->transition.bytes.timer = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateShopMenuModeCursor);
}

void drawShopMenuDescriptionText(ShopMenuWidgetActor *arg0) {
    u8 var_v0;

    if (gShopMenuDescriptionSeen == 0) {
        var_v0 = 3;
    } else if (gShopMenuShowNewCoursesMessage == 1) {
        var_v0 = 4;
    } else {
        var_v0 = gCourseSelectModeSelection;
    }
    drawMenuGlyphScript(arg0->x, arg0->y, gShopMenuModeDescriptionText[var_v0], 1, arg0->sprite.index, 0);
}

void updateShopMenuDescriptionText(ShopMenuWidgetActor *arg0) {
    u8 state = arg0->transition.bytes.state;

    switch (state) {
        case 0:
            arg0->sprite.index += 0x26;
            if (arg0->sprite.index >= 0x100) {
                arg0->sprite.index = 0x100;
                arg0->transition.bytes.state = 1;
            }
            state = arg0->transition.bytes.state;
            break;
        case 1:
            if (gRacePlayers[0].menuState == 1) {
                state = arg0->transition.bytes.state = 2;
            }
            break;
        case 2:
            arg0->x -= 0x20;
            if (arg0->x < -0xFF) {
                arg0->transition.bytes.state = 3;
            }
            state = arg0->transition.bytes.state;
            break;
        case 3:
            break;
    }
    if ((unsigned int)state == 3) {
        removeCallbackTask(arg0);
        if (gShopMenuDescriptionSeen == 0) {
            gShopMenuDescriptionSeen = 1;
        }
        if (gShopMenuShowNewCoursesMessage == 1) {
            gShopMenuShowNewCoursesMessage = 0;
        }
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawShopMenuDescriptionText, arg0);
}

void initShopMenuDescriptionText(ShopMenuWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = 0xC;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateShopMenuDescriptionText);
}

void drawShopMenuPromptPanel(ShopMenuWidgetActor *arg0) {
    drawMenuSpriteWithAlpha(
        arg0->x,
        arg0->y,
        getRelocatableHeapBlockBase(gAssetHandles[0x27]),
        0,
        0x20,
        0x20,
        0,
        arg0->sprite.index,
        0
    );
}

void updateShopMenuPromptPanel(ShopMenuWidgetActor *arg0) {
    u8 state = arg0->transition.bytes.state;

    switch (state) {
        case 0:
            arg0->sprite.index += 0x26;
            if (arg0->sprite.index >= 0x100) {
                arg0->sprite.index = 0x100;
                arg0->transition.bytes.state = 1;
            }
            state = arg0->transition.bytes.state;
            break;
        case 1:
            if (gRacePlayers[0].menuState == 1) {
                state = arg0->transition.bytes.state = 2;
            }
            break;
        case 2:
            arg0->x += 0x20;
            if (arg0->x >= 0xA0) {
                arg0->transition.bytes.state = 3;
            }
            state = arg0->transition.bytes.state;
            break;
        case 3:
            break;
    }
    if ((unsigned int)state == 3) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawShopMenuPromptPanel, arg0);
}

void initShopMenuPromptPanel(ShopMenuWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x5C;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateShopMenuPromptPanel);
}

void drawShopMenuMoneyPanel(ShopMenuWidgetActor *arg0) {
    char sp40[0x18];

    drawMenuPanelBackdrop(arg0->x, arg0->y, 0x5000, 0x4000);
    drawMenuSpriteWithAlpha(
        (s16)(arg0->x + 8),
        (s16)(arg0->y + 4),
        getRelocatableHeapBlockBase(gAssetHandles[0x25]),
        0x11,
        0x20,
        0x20,
        0,
        arg0->sprite.index,
        0
    );
    sprintf(sp40, gShopMenuMoneyFormat, gRacePlayers[0].money);
    drawMenuAsciiText((s16)(arg0->x + 0x10), (s16)(arg0->y + 0x10), sp40, 0, arg0->sprite.index);
}

void updateShopMenuMoneyPanel(ShopMenuWidgetActor *arg0) {
    u8 state = arg0->transition.bytes.state;

    switch (state) {
        case 0:
            arg0->sprite.index += 0x26;
            if (arg0->sprite.index >= 0x100) {
                arg0->sprite.index = 0x100;
                arg0->transition.bytes.state = 1;
            }
            state = arg0->transition.bytes.state;
            break;
        case 1:
            if (gRacePlayers[0].menuState == 1) {
                state = arg0->transition.bytes.state = 2;
            }
            break;
        case 2:
            arg0->x += 0x20;
            if (arg0->x >= 0xA0) {
                arg0->transition.bytes.state = 3;
            }
            state = arg0->transition.bytes.state;
            break;
        case 3:
            break;
    }
    if ((unsigned int)state == 3) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawShopMenuMoneyPanel, arg0);
}

void initShopMenuMoneyPanel(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x30;
    arg0->y = 0x40;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateShopMenuMoneyPanel);
}

void updateShopMenuMoneyPanelForCourseSelectReturn(ShopMenuWidgetActor *arg0) {
    u8 state = arg0->transition.bytes.state;

    switch (state) {
        case 0:
            arg0->sprite.index += 0x26;
            if (arg0->sprite.index >= 0x100) {
                arg0->sprite.index = 0x100;
                arg0->transition.bytes.state = 1;
            }
            state = arg0->transition.bytes.state;
            break;
        case 1:
            if (gRacePlayers[0].menuState == 3) {
                state = arg0->transition.bytes.state = 2;
            }
            break;
        case 2:
            arg0->x += 0x20;
            if (arg0->x >= 0xA0) {
                arg0->transition.bytes.state = 3;
            }
            state = arg0->transition.bytes.state;
            break;
        case 3:
            break;
    }
    if ((unsigned int)state == 3) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawShopMenuMoneyPanel, arg0);
}

void initShopMenuMoneyPanelForCourseSelectReturn(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x30;
    arg0->y = 0x40;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateShopMenuMoneyPanelForCourseSelectReturn);
}

void drawShopMenuSparkles(ShopMenuWidgetActor *arg0) {
    s32 patternOffset;
    s32 tileIndex;
    ShopMenuWidgetActor *drawActor;
    ShopMenuWidgetActor *actor;

    actor = arg0;
    tileIndex = 0;
    if (gShopMenuSparklePatterns[actor->sparkle.patternIndex][0].x != -1) {
        patternOffset = 0;
        do {
            drawActor = actor;
            drawMenuSpriteTile(
                (s16)(drawActor->randomValues[0] +
                      gShopMenuSparklePatterns[drawActor->sparkle.patternIndex][patternOffset].x),
                (s16)(drawActor->randomValues[1] +
                      gShopMenuSparklePatterns[drawActor->sparkle.patternIndex][patternOffset].y),
                getRelocatableHeapBlockBase(gAssetHandles[0x27]),
                tileIndex + drawActor->sparkle.tileBase,
                0,
                drawActor->sparkle.alpha
            );
            patternOffset++;
            tileIndex++;
        } while (gShopMenuSparklePatterns[actor->sparkle.patternIndex][patternOffset].x != -1);
    }
}

void updateShopMenuSparkles(ShopMenuWidgetActor *arg0) {
    s16 temp_v0;
    u8 var_v0;

    var_v0 = arg0->slide.slideState;
    switch (var_v0) {
        case 0:
            temp_v0 = arg0->sprite.index;
            arg0->x -= 0x20;
            if (temp_v0 >= arg0->x) {
                arg0->x = temp_v0;
                arg0->slide.slideState = 1U;
            }
            var_v0 = arg0->slide.slideState;
            break;
        case 1:
            if (gRacePlayers[0].menuState == 1) {
                arg0->slide.slideState = 2U;
                var_v0 = 2 & 0xFF;
            }
            break;
    }
    var_v0 = arg0->slide.slideState;
    if (var_v0 == 2) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawShopMenuSparkles, arg0);
}

void initShopMenuSparkles(ShopMenuWidgetActor *arg0) {
    s16 *entry;

    arg0->sparkle.patternIndex = gRacePlayers[0].selectedCharacterId;
    entry = &gShopMenuSparkleInitTable[arg0->sparkle.patternIndex * 3];
    arg0->sprite.index = entry[0];
    arg0->x = 0x94;
    arg0->y = entry[1];
    arg0->sparkle.tileBase = entry[2];
    arg0->sparkle.alpha = 0x100;
    arg0->slide.slideState = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateShopMenuSparkles);
}

void drawCourseUnlockPricePanel(ShopMenuWidgetActor *arg0) {
    s16 new_var;
    s32 temp;
    char sp4C[4];
    u16 sp4A;
    u16 sp48;
    s32 palette;

    if (gGameSaveDataBuffer[0].courseUnlockStates[gRacePlayers[0].menuSelection] == -1) {
        new_var = 0x3000;
        temp = gCourseUnlockPrices[gRacePlayers[0].menuSelection];
        if ((u32)temp >= 0x186A0U) {
            sp4A = 0x4000;
            sp48 = 0;
            sprintf(sp4C, gCourseUnlockPriceFormat6, arg0->item.price);
        } else if ((u32)temp >= 0x2710U) {
            sp4A = 0x3800;
            sp48 = 4;
            sprintf(sp4C, gCourseUnlockPriceFormat5, arg0->item.price);
        } else {
            sp4A = new_var;
            sp48 = 8;
            sprintf(sp4C, gCourseUnlockPriceFormat4, arg0->item.price);
        }

        drawMenuPanelBackdrop((s16)(arg0->x + sp48), arg0->y, sp4A, 0x2000);

        if ((u32)gRacePlayers[0].money < (u32)gCourseUnlockPrices[gRacePlayers[0].menuSelection]) {
            palette = 1;
        } else {
            palette = 0;
        }
        if (arg0->slide.slideState == 2) {
            palette = 0;
        }

        drawMenuAsciiText((s16)(arg0->x + sp48 + 4), (s16)(arg0->y + 4), sp4C, (u8)palette, arg0->sprite.index);
        return;
    }

    drawMenuSpriteWithAlpha(
        arg0->x,
        arg0->y,
        getRelocatableHeapBlockBase(gAssetHandles[0x27]),
        6,
        0x20,
        0x20,
        0,
        arg0->sprite.index,
        0
    );
}

void updateCourseUnlockPricePanel(ShopMenuWidgetActor *arg0) {
    s32 amount;
    s32 price;

    switch (arg0->slide.bytes.state) {
        case 0:
            arg0->sprite.index += 0x26;
            if (arg0->sprite.index >= 0x100) {
                arg0->sprite.index = 0x100;
                arg0->slide.bytes.state = 1;
            }
            break;
        case 1:
            arg0->item.price = gCourseUnlockPrices[gRacePlayers[0].menuSelection];
            if (gCurrentGameTask->callbackData1 >= 2) {
                arg0->slide.bytes.state = 2;
            }
            break;
        case 2:
            price = arg0->item.price;
            if ((u32)price < 100U) {
                amount = 0xFFFF;
                amount = price & amount;
            } else {
                if (gPlayerInputHeld[0] & A_BUTTON) {
                    if ((u32)price < 5000U) {
                        amount = price & 0xFFFF;
                    } else {
                        amount = 5000;
                    }
                } else {
                    amount = 500;
                }
            }
            arg0->item.price = price - amount;
            gRacePlayers[0].money -= amount;
            /* Required for IDO register allocation. */
            if (gCurrentGameTask && gCurrentGameTask) {}
            if (arg0->item.price == 0) {
                arg0->slide.bytes.state = 3;
                gGameSaveDataBuffer[0].courseUnlockStates[gRacePlayers[0].menuSelection] = 9;
            }
            break;
        case 3:
            arg0->slide.bytes.timer++;
            if (arg0->slide.bytes.timer >= 0x14) {
                arg0->slide.bytes.timer = 0;
                arg0->slide.bytes.state = 4;
                gCurrentGameTask->callbackData1 = gCurrentGameTask->callbackData1 + 2;
            }
            break;
        case 4:
            if (gRacePlayers[0].menuState == 3) {
                arg0->slide.bytes.state = 5;
            }
            /* fallthrough */
        case 5:
            arg0->x += 0x20;
            if (arg0->x >= 0x94) {
                arg0->slide.bytes.state = 6;
            }
            break;
    }
    if (((gMenuChoicePromptState[0] >= 5) && (gMenuChoicePromptState[0] != 9)) || (arg0->slide.bytes.state == 6)) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseUnlockPricePanel, arg0);
}

void initCourseUnlockPricePanel(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x20;
    arg0->y = -8;
    arg0->item.price = gCourseUnlockPrices[gRacePlayers[0].menuSelection];
    arg0->sprite.index = 0;
    arg0->slide.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseUnlockPricePanel);
}

void drawCourseUnlockPurchasePrompt(ShopMenuWidgetActor *arg0) {
    volatile u64 unused;
    u32 drawAlpha;
    u16 alpha;

    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0, 0x20, 0x20, 0, 0);
    drawMenuSprite(
        (s16)(arg0->x + 0x40),
        arg0->y,
        getRelocatableHeapBlockBase(gAssetHandles[0x24]),
        1,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0x78),
        arg0->y,
        getRelocatableHeapBlockBase(gAssetHandles[0x24]),
        1,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0xB0),
        arg0->y,
        getRelocatableHeapBlockBase(gAssetHandles[0x24]),
        2,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuGlyphScript((s16)(arg0->x + 0x30), (s16)(arg0->y + 4), gCourseUnlockPurchasePromptText, 0, 0x100, 0);

    if (arg0->item.price == 0) {
        alpha = 0x100;
    } else {
        alpha = 0x60;
    }

    drawAlpha = alpha;
    drawMenuSpriteWithAlpha(
        (s16)(arg0->x + 0x50),
        (s16)(arg0->y + 0x14),
        getRelocatableHeapBlockBase(gAssetHandles[0x24]),
        0x17,
        0x20,
        0x20,
        0,
        drawAlpha,
        0
    );

    if (drawAlpha == 0x100) {
        alpha = 0x60;
    } else {
        alpha = 0x100;
    }

    drawMenuSpriteWithAlpha(
        (s16)(arg0->x + 0x50),
        (s16)(arg0->y + 0x24),
        getRelocatableHeapBlockBase(gAssetHandles[0x24]),
        0x18,
        0x20,
        0x20,
        0,
        alpha,
        0
    );
    drawMenuSpriteWithAlpha(
        (s16)(arg0->x + 0x50),
        (s16)(arg0->y + (arg0->item.price * 0x10) + 0x14),
        getRelocatableHeapBlockBase(gAssetHandles[0x24]),
        0x12,
        0x20,
        0x20,
        0,
        arg0->sprite.index,
        0
    );
}

void updateCourseUnlockPurchasePrompt(ShopMenuWidgetActor *arg0) {
    ShopMenuWidgetActor *temp_a2;

    arg0->item.price = gCurrentGameTask->callbackData1;
    temp_a2 = arg0;
    if (arg0->transition.counter < 0x10) {
        arg0->sprite.index -= 9;
    } else {
        temp_a2->sprite.index += 9;
    }
    temp_a2->transition.counter = (temp_a2->transition.counter + 1) & 0x1F;
    if (gMenuFlowState == 1) {
        removeCallbackTask(temp_a2);
        gMenuFlowState = 0;
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseUnlockPurchasePrompt, temp_a2);
    }
}

void initCourseUnlockPurchasePrompt(ShopMenuWidgetActor *arg0) {
    arg0->x = -0x76;
    arg0->y = -0x40;
    arg0->item.price = 0;
    arg0->sprite.index = 0x100;
    arg0->transition.counter = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseUnlockPurchasePrompt);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void drawCourseDetailsMenu(ShopMenuWidgetActor *actor) {
    u16 *entryTile;
    s32 entryIndex;
    s32 yOffset;
    CourseNumberGlyphScript courseNumberScript;
    s32 courseNumber;
    s32 tensDigit;
    s16 selectionX;
    s16 drawX;

    /* Keeping the loop setup together preserves IDO's original scheduling. */
    entryIndex = 0; yOffset = 0; entryTile = gCourseDetailsMenuEntryTiles[0]; do {
        drawMenuSprite(actor->cursorPositions[entryIndex], (s16)(actor->targetY + yOffset),
                       getRelocatableHeapBlockBase(gAssetHandles[0x27]),
                       entryTile[gCourseDetailsPreviewPage * COURSE_DETAILS_COURSES_PER_PAGE],
                       0x20, 0x20, 0, 0);
        courseNumber = ((gCourseDetailsPreviewPage * COURSE_DETAILS_COURSES_PER_PAGE) + entryIndex) + 1;
        if (actor && actor) { /* preserves IDO's original register allocation */ }
        tensDigit = courseNumber / 10;
        if (tensDigit == 0) {
            courseNumberScript[0] = -2;
        } else {
            courseNumberScript[0] = tensDigit;
        }
        courseNumberScript[1] = courseNumber % 10;
        courseNumberScript[2] = -1;
        drawMenuGlyphScriptDefaultFont((s16)(actor->cursorPositions[entryIndex] - 0x12),
                                       (s16)(actor->targetY + yOffset),
                                       courseNumberScript, 1, 0x100);

        yOffset += COURSE_DETAILS_ROW_SPACING;
        entryTile++;
        entryIndex++;
    } while (entryIndex < COURSE_DETAILS_COURSES_PER_PAGE);

    entryIndex = COURSE_DETAILS_COURSES_PER_PAGE;
    yOffset = COURSE_DETAILS_COURSES_PER_PAGE * COURSE_DETAILS_ROW_SPACING;
    do {
        drawMenuSprite(actor->cursorPositions[entryIndex], (s16)(actor->targetY + yOffset),
                       getRelocatableHeapBlockBase(gAssetHandles[0x27]), (u16)entryIndex,
                       0x20, 0x20, 0, 0);
        yOffset += COURSE_DETAILS_ROW_SPACING;
        entryIndex++;
    } while (entryIndex != COURSE_DETAILS_OPTION_COUNT);

    if ((actor->state != COURSE_DETAILS_STATE_REVEAL_LIST) && (gCourseDetailsCloseFromBack == 0)) {
        if (actor->state < COURSE_DETAILS_STATE_EXIT) {
            selectionX = actor->targetX;
        } else {
            selectionX = actor->cursorPositions[0];
        }
        drawX = selectionX;
        drawMenuSpriteWithAlpha(drawX,
                                (s16)(actor->targetY + (gCourseDetailsMenuSelection * COURSE_DETAILS_ROW_SPACING)),
                                getRelocatableHeapBlockBase(gAssetHandles[0x25]), 0x12,
                                0x20, 0x20, 0, actor->prompt.bytes.pulseAlpha, 0);
    }
}
// clang-format on

void updateCourseDetailsMenu(ShopMenuWidgetActor *arg0) {
    ShopMenuWidgetActor *visibleActor;
    s32 i;
    s32 movingEntryCount;
    s32 menuState;
    u8 state;
    s32 screenState;
    u16 visibleCount;
    s16 cursorX;

    visibleActor = arg0;
    menuState = gCourseSelectSubmenuState;
    state = arg0->state;
    if ((menuState & 0xFFFFFFFF) != state) {
        state = menuState;
        arg0->state = menuState;
    }

    screenState = gCurrentGameTask->callbackData2;
    if (((screenState == 3) || (screenState == 9)) && (state < COURSE_DETAILS_STATE_EXIT)) {
        state = (arg0->state = COURSE_DETAILS_STATE_EXIT);
        arg0->prompt.bytes.pulseAlpha = 0x100;
    }

    switch (state) {
        case COURSE_DETAILS_STATE_REVEAL_LIST:
            movingEntryCount = 0;
            for (menuState = 0; menuState < (u16)visibleActor->visibleCount; menuState++) {
                cursorX = arg0->cursorPositions[menuState];
                if (cursorX < arg0->targetX) {
                    arg0->cursorPositions[menuState] = cursorX + 0x10;
                    movingEntryCount++;
                    if (arg0->cursorPositions[menuState] >= arg0->targetX) {
                        arg0->cursorPositions[menuState] = arg0->targetX;
                    }
                }
            }

            arg0->spawnTimer++;
            if (!(arg0->spawnTimer & 1)) {
                visibleCount = arg0->visibleCount;
                if ((s32)visibleCount < COURSE_DETAILS_OPTION_COUNT) {
                    arg0->visibleCount = visibleCount + 1;
                    if ((u16)arg0->visibleCount == COURSE_DETAILS_OPTION_COUNT) {
                        createCallbackTask((CallbackTaskCallback)initCourseDetailsPreviewTile, 0, 0x63);
                    }
                }
            }
            if (movingEntryCount == 0) {
                arg0->state = COURSE_DETAILS_STATE_FADE_IN_CURSOR;
            }
            state = arg0->state;
            break;
        case COURSE_DETAILS_STATE_FADE_IN_CURSOR:
            arg0->prompt.bytes.pulseAlpha += 0x26;
            if (arg0->prompt.bytes.pulseAlpha >= 0x100) {
                arg0->prompt.bytes.pulseAlpha = 0x100;
                arg0->state = COURSE_DETAILS_STATE_IDLE;
            }
            state = arg0->state;
            break;
        case COURSE_DETAILS_STATE_SLIDE_PAGE_OUT:
            for (i = 0; i < COURSE_DETAILS_COURSES_PER_PAGE; i++) {
                arg0->cursorPositions[i] -= 0x20;
                if (arg0->cursorPositions[i] < -0x117) {
                    arg0->cursorPositions[i] = -0x118;
                }
            }
            if (arg0->cursorPositions[0] == -0x118) {
                arg0->state = COURSE_DETAILS_STATE_SLIDE_PAGE_IN;
                gCourseDetailsPreviewPage = (gCourseDetailsPreviewPage + 1) % COURSE_DETAILS_PAGE_COUNT;
            }
            state = arg0->state;
            break;
        case COURSE_DETAILS_STATE_SLIDE_PAGE_IN:
            for (i = 0; i < COURSE_DETAILS_COURSES_PER_PAGE; i++) {
                arg0->cursorPositions[i] += 0x20;
                if (arg0->cursorPositions[i] >= arg0->targetX) {
                    arg0->cursorPositions[i] = arg0->targetX;
                }
            }
            if (arg0->targetX == arg0->cursorPositions[0]) {
                arg0->state = COURSE_DETAILS_STATE_IDLE;
            }
            state = arg0->state;
            break;
        case COURSE_DETAILS_STATE_EXIT:
            for (i = 0; i < COURSE_DETAILS_OPTION_COUNT; i++) {
                arg0->cursorPositions[i] -= 0x20;
            }
            if (arg0->cursorPositions[0] < -0x117) {
                arg0->state = COURSE_DETAILS_STATE_DONE;
            }
            state = arg0->state;
            break;
        case COURSE_DETAILS_STATE_RESET_CURSOR:
            arg0->prompt.bytes.pulseAlpha = 0x100;
            arg0->prompt.bytes.pulseTimer = 0;
            state = arg0->state;
            break;
        case COURSE_DETAILS_STATE_DONE:
            break;
    }

    if ((state >= COURSE_DETAILS_STATE_IDLE) && (state < COURSE_DETAILS_STATE_EXIT)) {
        if ((s32)(u16)arg0->prompt.bytes.pulseTimer < 0x10) {
            arg0->prompt.bytes.pulseAlpha -= 9;
        } else {
            arg0->prompt.bytes.pulseAlpha += 9;
        }
        arg0->prompt.bytes.pulseTimer = (arg0->prompt.bytes.pulseTimer + 1) & 0x1F;
        state = arg0->state;
    }

    gCourseSelectSubmenuState = state;
    if (arg0->state == COURSE_DETAILS_STATE_DONE) {
        removeCallbackTask((CallbackTask *)arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseDetailsMenu, arg0);
}

void initCourseDetailsMenu(ShopMenuWidgetActor *arg0) {
    s32 i;

    for (i = 0; i < COURSE_DETAILS_OPTION_COUNT; i++) {
        arg0->cursorPositions[i] = -0xFC;
    }

    arg0->targetX = -0x7C;
    arg0->targetY = -0x5C;
    arg0->prompt.bytes.pulseTimer = 0;
    arg0->spawnTimer = 0;
    arg0->visibleCount = 1;
    arg0->prompt.bytes.pulseAlpha = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseDetailsMenu);
}

void drawCourseDetailsPreviewTile(ShopMenuWidgetActor *arg0) {
    s32 unused;
    u16 tileIndex;

    if (gCourseDetailsMenuSelection < COURSE_DETAILS_COURSES_PER_PAGE) {
        tileIndex = gCourseDetailsPreviewCourseTiles
            [gCourseDetailsPreviewPage * COURSE_DETAILS_COURSES_PER_PAGE + gCourseDetailsMenuSelection];
    } else {
        tileIndex = gCourseDetailsPreviewExtraTiles[gCourseDetailsPreviewPage * COURSE_DETAILS_COURSES_PER_PAGE];
    }

    drawMenuSpriteSubrect(
        arg0->x,
        arg0->y,
        getRelocatableHeapBlockBase(gAssetHandles[0xD]),
        tileIndex,
        0,
        0,
        0x40,
        0x1C,
        0x20,
        0x20
    );
    drawMenuSpriteSubrect(
        (s16)(arg0->x + 0x40),
        arg0->y,
        getRelocatableHeapBlockBase(gAssetHandles[0xD]),
        tileIndex,
        0,
        0x1C,
        0x40,
        0x1C,
        0x20,
        0x20
    );
    drawMenuSprite(
        (s16)(arg0->x - 4),
        (s16)(arg0->y - 1),
        getRelocatableHeapBlockBase(gAssetHandles[0x27]),
        0xA,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0x3C),
        (s16)(arg0->y - 1),
        getRelocatableHeapBlockBase(gAssetHandles[0x27]),
        0xB,
        0x20,
        0x20,
        1,
        0
    );
}

void updateCourseDetailsPreviewTile(ShopMenuWidgetActor *arg0) {
    u8 state;

    if ((gCurrentGameTask->callbackData2 == 3) || (gCurrentGameTask->callbackData2 == 9)) {
        arg0->sprite.bytes.state = 2;
    }
    state = arg0->sprite.bytes.state;
    switch (state) {
        case 0:
            arg0->x -= 0x20;
            if (arg0->x < 5) {
                arg0->x = 4;
                arg0->sprite.bytes.state = 1;
            }
            state = arg0->sprite.bytes.state;
            break;
        case 1:
            break;
        case 2:
            arg0->x += 0x20;
            if (arg0->x >= 0x90) {
                arg0->sprite.bytes.state = 3;
            }
            state = arg0->sprite.bytes.state;
            break;
        case 3:
            state = arg0->sprite.bytes.state = 4;
            break;
    }
    if ((u32)state == 4) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseDetailsPreviewTile, arg0);
}

void initCourseDetailsPreviewTile(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x90;
    arg0->y = 0x44;
    arg0->sprite.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseDetailsPreviewTile);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void drawCoursePreviewCloseSparkles(ShopMenuWidgetActor *arg0) {
    Vec2s *offset;
    ShopMenuWidgetActor *actor;
    ShopMenuWidgetActor *counterActor;
    void *texture;

    actor = arg0; offset = gCoursePreviewCloseSparkleOffsets; counterActor = arg0; do {
        if (offset < gCoursePreviewCloseSparkleMirrorStart) {
            texture = getRelocatableHeapBlockBase(gAssetHandles[0x1C]);
            drawMenuSprite((s16)(offset->x + actor->x), (s16)(offset->y + actor->sprite.index - 8),
                          texture, counterActor->transition.counter + 0x39,
                          0x20, 0x20, 0, 0);
        } else {
            texture = getRelocatableHeapBlockBase(gAssetHandles[0x1C]);
            drawMenuSprite((s16)(offset[-6].x + actor->y), (s16)((offset[-6].y + actor->sprite.index) * -1 - 8),
                          texture, counterActor->transition.counter + 0x39,
                          0x20, 0x20, 0, 0);
        }
        offset++;
        counterActor = (ShopMenuWidgetActor *)&counterActor->pad0[2];
    } while (offset != gCoursePreviewCloseSparkleOffsetsEnd);
}
// clang-format on

void updateCoursePreviewCloseSparkles(ShopMenuWidgetActor *arg0) {
    s32 i;

    arg0->sprite.index = gCoursePreviewViewportHeight / 2;
    arg0->x++;
    arg0->y--;

    for (i = 0; i < 12; i += 4) {
        arg0->sparkle.counters[i] = (arg0->sparkle.counters[i] + 1) % 6;
        arg0->sparkle.counters[i + 1] = (arg0->sparkle.counters[i + 1] + 1) % 6;
        arg0->sparkle.counters[i + 2] = (arg0->sparkle.counters[i + 2] + 1) % 6;
        arg0->sparkle.counters[i + 3] = (arg0->sparkle.counters[i + 3] + 1) % 6;
    }

    if (arg0->sprite.index == 0) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCoursePreviewCloseSparkles, arg0);
}

void initCoursePreviewCloseSparkles(ShopMenuWidgetActor *arg0) {
    ShopMenuWidgetActor *new_var;
    s32 i;

    arg0->x = 5;
    arg0->y = 0x48;
    new_var = arg0;
    new_var->sprite.index = gCoursePreviewViewportHeight / 2;

    for (i = 0; i < 12; i++) {
        new_var->randomValues[i + 3] = randomNextMain() % 6;
    }

    setCallbackTaskCallback(new_var, (CallbackTaskCallback)updateCoursePreviewCloseSparkles);
}
