#include "game/race/race_state.h"
#include "font_encoding.h"
#include "common.h"
#include <PR/os_libc.h>
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/system_runtime.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/splitscreen_select/race_splitscreen_select_ui.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/race/player/race_player_input.h"
#include "game/engine/game_task_scheduler.h"
#include "game/engine/system_render.h"
#include "game/menu/race_setup/race_setup_menu.h"
#include "game/menu/splitscreen_select/race_splitscreen_select_menu.h"

#define ASSET_HANDLE(index) (gAssetHandles[(index)])

#define RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE (gAssetHandles[0x21])

RaceSplitscreenSelectFrameTiles gRaceSplitscreenSelectFrameTiles[5] = {
#if defined(VERSION_JP)
    0x003B, 0x003C, 0x003D, 0x003E, 0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0049, 0x004A,
    0x004B, 0x004C, 0x003F, 0x0048, 0x004D, 0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x0053, 0x0055, 0x0056, 0x0057,
    0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005E, 0x005F, 0x0060, 0x0061, 0x0054, 0x005D, 0x0062, 0x0063, 0x0064,
    0x0065, 0x0066, 0x0067, 0x0068, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F, 0x0070, 0x0071, 0x0073, 0x0074,
    0x0075, 0x0076, 0x0069, 0x0072, 0x0077, 0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007F, 0x0080, 0x0081,
    0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0088, 0x0089, 0x008A, 0x008B, 0x007E, 0x0087, 0x008C, 0x008D, 0x008E,
    0x008F, 0x0090, 0x0091, 0x0092, 0x0094, 0x0095, 0x0096, 0x0097, 0x0098, 0x0099, 0x009A, 0x009B, 0x009D, 0x009E,
    0x009F, 0x00A0, 0x0093, 0x009C, 0x00A1, 0x00A2, 0x00A3
#else
    { { 0x003B,
        0x003C,
        0x003D,
        0x003E,
        0x0040,
        0x0041,
        0x0042,
        0x0043,
        0x0044,
        0x0045,
        0x0046,
        0x0047,
        0x0049,
        0x004A,
        0x004B,
        0x004C },
     { 0x003F, 0x0048 },
     { 0x004D, 0x004E },
     0x004F },
    { { 0x0050,
        0x0051,
        0x0052,
        0x0053,
        0x0055,
        0x0056,
        0x0057,
        0x0058,
        0x0059,
        0x005A,
        0x005B,
        0x005C,
        0x005E,
        0x005F,
        0x0060,
        0x0061 },
     { 0x0054, 0x005D },
     { 0x0062, 0x0063 },
     0x0064 },
    { { 0x0065,
        0x0066,
        0x0067,
        0x0068,
        0x006A,
        0x006B,
        0x006C,
        0x006D,
        0x006E,
        0x006F,
        0x0070,
        0x0071,
        0x0073,
        0x0074,
        0x0075,
        0x0076 },
     { 0x0069, 0x0072 },
     { 0x0077, 0x0078 },
     0x0079 },
    { { 0x007A,
        0x007B,
        0x007C,
        0x007D,
        0x007F,
        0x0080,
        0x0081,
        0x0082,
        0x0083,
        0x0084,
        0x0085,
        0x0086,
        0x0088,
        0x0089,
        0x008A,
        0x008B },
     { 0x007E, 0x0087 },
     { 0x008C, 0x008D },
     0x008E },
    { { 0x008F,
        0x0090,
        0x0091,
        0x0092,
        0x0094,
        0x0095,
        0x0096,
        0x0097,
        0x0098,
        0x0099,
        0x009A,
        0x009B,
        0x009D,
        0x009E,
        0x009F,
        0x00A0 },
     { 0x0093, 0x009C },
     { 0x00A1, 0x00A2 },
     0x00A3 },
#endif
};
#if defined(VERSION_JP)
RaceSplitscreenSelectDescriptionScriptTable gRaceSplitscreenSelectDescriptionScripts = {
    0xFFFC, 0x0006, 0x004B, 0x003D, 0x003A, 0x0053, 0x0105, 0x00B3, 0x0040, 0x0041, 0x0052, 0x0060, 0x008E, 0x0092,
    0x00AE, 0x0097, 0xFFFD, 0x00A1, 0x0095, 0x008A, 0x00A2, 0x0001, 0x0115, 0x00A5, 0x00A4, 0x00BA, 0x0092, 0x0024,
    0xFFFD, 0xFFFC, 0x0007, 0x0003, 0x0115, 0x00AE, 0x00C9, 0x00A5, 0x0070, 0x002B, 0x0068, 0x009C, 0x00B9, 0x00CB,
    0x0114, 0x0109, 0x010A, 0xFFFD, 0x00A3, 0x0045, 0x003A, 0x0082, 0x00BC, 0x00B2, 0x00B6, 0x0093, 0x00AE, 0x009C,
    0x0032, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0xFFFE, 0xFFFE, 0xFFFC, 0x0006, 0x0110, 0x004C, 0x0081, 0x002B, 0x007A, 0x0047, 0x006A, 0x004C, 0x0111, 0xFFFD,
    0xFFFE, 0xFFFE, 0x0110, 0x004B, 0x003C, 0x002B, 0x0053, 0x0047, 0x006A, 0x004C, 0x0111, 0xFFFD, 0xFFFE, 0xFFFE,
    0x0110, 0x0053, 0x0067, 0x003A, 0x0047, 0x006F, 0x002B, 0x0060, 0x0111, 0xFFFD, 0xFFFC, 0x0007, 0x00A8, 0x0003,
    0x00A1, 0x00A8, 0x006F, 0x002B, 0x0060, 0x00BC, 0x0090, 0x009E, 0x00CE, 0x00AE, 0x009C, 0x0032, 0xFFFD, 0x0109,
    0x010A, 0x00B2, 0x00B2, 0x00B6, 0x0093, 0x00AE, 0x009C, 0x0032, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0xFFFC, 0x0007, 0x0049, 0x002B, 0x004C, 0x008E, 0x00D5, 0x008F, 0x00C9, 0x0114, 0x0001, 0x00E6, 0x00A8, 0x004F,
    0x0041, 0x0060, 0xFFFD, 0x008E, 0x0096, 0x009E, 0x0092, 0x006F, 0x002B, 0x0060, 0x00C9, 0x009C, 0x0032, 0xFFFD,
    0xFFFC, 0x0006, 0x004C, 0x0081, 0x002B, 0x007A, 0x005B, 0x0035, 0x003F, 0x008E, 0x00A1, 0x0095, 0x0092, 0x004F,
    0x0041, 0x005F, 0xFFFD, 0x003F, 0x006E, 0x00BC, 0x0084, 0x0041, 0x003F, 0x0053, 0x0024, 0xFFFF, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0xFFFC, 0x0007, 0x00E7, 0x00E8, 0x00E9, 0x00A8, 0x0094, 0x00E8, 0x00C9, 0x009C, 0x0032, 0xFFFD, 0xFFFC, 0x0006,
    0x0090, 0x009F, 0x00B6, 0x009B, 0x0091, 0x007F, 0x002B, 0x007A, 0x008E, 0x00EA, 0x008A, 0x009F, 0x00B7, 0xFFFD,
    0x007F, 0x002B, 0x007A, 0x00A8, 0x0083, 0x0041, 0x003F, 0x0053, 0x008E, 0x0095, 0x0093, 0x00B8, 0xFFFC, 0x0007,
    0x0099, 0x00A3, 0xFFFD, 0x00BC, 0x00C9, 0x0096, 0x00AE, 0x009C, 0x0032, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0xFFFC, 0x0007, 0x0099, 0x00B9, 0x00AE, 0x00C9, 0x00A8, 0x0079, 0x002B, 0x004F, 0x008E, 0xFFFC, 0x0006, 0x004D,
    0x002B, 0x007D, 0xFFFC, 0x0007, 0x009B, 0x00A2, 0xFFFD, 0x004F, 0x0041, 0x0053, 0x0068, 0x00A5, 0x00B2, 0x00CA,
    0x00B7, 0x00AE, 0x009C, 0x0032, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0xFFFC, 0x0007, 0x00C1, 0x008F, 0x00A7, 0x008F, 0x00A4, 0x00BC, 0x00B6, 0x0114, 0x004B, 0x0058, 0x007C, 0x003F,
    0x0108, 0x00A8, 0xFFFD, 0x007F, 0x002B, 0x007A, 0x00A9, 0x0094, 0x0091, 0x00A2, 0x00AE, 0x009D, 0x008F, 0x0032,
    0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0010, 0x000A, 0x0016, 0x000E, 0xFFFE, 0x0016, 0x000E, 0x0017, 0x001E, 0xFFFF
};
u32 gMenuPanelBackdropTexture[16] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                      0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                      0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
#endif

#if defined(VERSION_JP)
const char gRaceSplitscreenSelectEntryFeeFormat[8] = "%6dG";
#else
RaceSplitscreenSelectDescriptionScriptTable gRaceSplitscreenSelectDescriptionScripts = {
    {
     {
            // textconv requires these _() invocations to retain their original line layout.
            // clang-format off
            _("{COLOR:6}HEAD FOR THE\nGOAL{COLOR:7}, WHILE\nCOLLECTING\nITEMS."),
        },
        {
            _("{COLOR:6}{ALT_TITLE_LEFT}SPEED GAME{ALT_TITLE_RIGHT}\n{ALT_TITLE_LEFT}SHOOT GAME{ALT_TITLE_RIGHT}\n{ALT_TITLE_LEFT}TRICK GAME{ALT_TITLE_RIGHT}\n{COLOR:7}ARE\nAVAILABLE."),
        },
        {
            _("{COLOR:7}SELECT A\nCOURSE AND\n{COLOR:6}RECORD YOUR\nBEST TIME\n{COLOR:7}FOR 1 LAP."),
        },
        {
            _("{COLOR:7}BOARD SHOP.\n{COLOR:6}BUY NEW\nBOARDS {COLOR:7}AND\n{COLOR:6}PAINT THEM\n{COLOR:7}HERE."),
        },
        {
            _("{COLOR:6}SAVE DATA\n{COLOR:7}AND RETURN\nTO THE\nTITLE\nSCREEN."),
        },
        {
            _("{COLOR:7}SHINOBIN\nBOARDS\nARE NOT\nAVAILABLE."),
        },
    },
    {
            _("GAME MENU"),
     // clang-format on
    },
};

u32 gMenuPanelBackdropTexture[16] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
};

const char gRaceSplitscreenSelectEntryFeeFormat[] = "%6dG";

// IDO code generation for this function is sensitive to source line layout.
#endif
// clang-format off
void drawRaceSplitscreenSelectPlayerCountIcons(RaceSplitscreenSelectOptionIconActor *icons) {
    RaceSplitscreenSelectOptionIconActor *savedIcons;
    s32 iconIndex;
    s32 blinkAlpha;
    s32 yOffset;
    u16 tileIndex;

    savedIcons = icons;
    iconIndex = 0;
    if (iconIndex < (s32) icons->visibleCount) {
        /*
         * The outer do/while (0), repeated iconIndex initialization, empty if,
         * and paired statements must retain this shape for IDO's register
         * allocation. The icon position itself is nevertheless a normal array
         * access; the compiler converts it to the pointer walk in the ROM.
         */
        do { iconIndex = 0; yOffset = 0; do {
            blinkAlpha = 0;
            if (((((gMenuSelectionConfirmTimer != 0) && (((s32) gMenuSelectionConfirmTimer) < 8)) &&
                 (gMenuExitSelection == 0)) &&
                (iconIndex == gRaceSplitscreenMode)) &&
                (gMenuSelectionConfirmTimer & 1)) {
                blinkAlpha = 0xFF;
            }
            tileIndex = iconIndex + 8;
            drawMenuSprite(icons->x[iconIndex], (s16)(icons->y + yOffset),
                           getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
                           tileIndex, 0x20, 0x20, 0, blinkAlpha);
            yOffset += 0x14; iconIndex++;
            if (icons->visibleCount) {}
        } while (iconIndex < ((s32) savedIcons->visibleCount)); } while (0);
    }
}
// clang-format on

void updateRaceSplitscreenSelectPlayerCountIcons(RaceSplitscreenSelectOptionIconActor *arg0) {
    s32 i;
    s32 moved;
    void (*callback)(RaceSplitscreenSelectOptionFrameActor *);
    s32 state;
    int stateByte;
    RaceSplitscreenSelectOptionIconActor *row;
    RaceSplitscreenSelectOptionIconActor *actor;

    stateByte = arg0->state;
    actor = arg0;
    state = stateByte;
    row = arg0;
    switch (state) {
        case 0:
            moved = 0;
            for (i = 0; i < row->visibleCount; i++) {
                if (row->x[i] < -0x7C) {
                    row->x[i] += 0x10;
                    moved++;
                    if (row->x[i] >= -0x7C) {
                        row->x[i] = -0x7C;
                    }
                }
            }

            row->spawnTimer++;
            if (!(row->spawnTimer & 1)) {
                if (row->visibleCount < 5) {
                    row->visibleCount++;
                }
            }

            if (moved == 0) {
                row->state = 1;
                createCallbackTask((CallbackTaskCallback)initRaceSplitscreenSelectOption0Frame, 0, 0x5F);
                callback = initRaceSplitscreenSelectOption1Frame;
                createCallbackTask((CallbackTaskCallback)callback, 0, 0x60);
                createCallbackTask((CallbackTaskCallback)initRaceSplitscreenSelectOption2Frame, 0, 0x61);
                createCallbackTask((CallbackTaskCallback)initRaceSplitscreenSelectOption3Frame, 0, 0x62);
                createCallbackTask((CallbackTaskCallback)initRaceSplitscreenSelectOption4Frame, 0, 0x63);
                state++;
                state--;
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
                arg0->x[i] -= 0x20;
            }
            if (arg0->x[0] < -0x103) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSplitscreenSelectPlayerCountIcons, actor);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initRaceSplitscreenSelectPlayerCountIcons(RaceSplitscreenSelectOptionIconActor *arg0) {
    s32 i;

    for (i = 0; i < 5; i++) { arg0->x[i] = -0x104; }

    arg0->y = -0x60;
    arg0->spawnTimer = 0;
    arg0->visibleCount = 1;
    arg0->state = 0;

    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSplitscreenSelectPlayerCountIcons);
}
// clang-format on

void drawRaceSplitscreenSelectCornerSprites(RaceSplitscreenSelectCornerSpriteActor *arg0) {
    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(ASSET_HANDLE(33)), 3, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x + 0x40), arg0->y, getRelocatableHeapBlockBase(ASSET_HANDLE(33)), 4, 0x20, 0x20, 0, 0);
    drawMenuSprite(arg0->x, (s16)(arg0->y + 0x40), getRelocatableHeapBlockBase(ASSET_HANDLE(33)), 5, 0x20, 0x20, 0, 0);
    drawMenuSprite(
        (s16)(arg0->x + 0x40),
        (s16)(arg0->y + 0x40),
        getRelocatableHeapBlockBase(ASSET_HANDLE(33)),
        6,
        0x20,
        0x20,
        0,
        0
    );
}

void updateRaceSplitscreenSelectCornerSprites(RaceSplitscreenSelectCornerSpriteActor *arg0) {
    u8 state = arg0->state;

    switch (state) {
        case 0:
            arg0->x += 0x20;
            if (arg0->x >= -0x88) {
                arg0->x = -0x88;
                arg0->state = 1;
            }
            state = arg0->state;
            break;
        case 1:
            if (gRacePlayers[0].menuState == 1) {
                state = arg0->state = 2;
            }
            break;
        case 2:
            arg0->x -= 0x20;
            if (arg0->x < -0x10D) {
                arg0->state = 3;
            }
            state = arg0->state;
            break;
        case 3:
            break;
    }
    if ((unsigned int)state == 3) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSplitscreenSelectCornerSprites, arg0);
}

void initRaceSplitscreenSelectCornerSprites(RaceSplitscreenSelectCornerSpriteActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 8;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSplitscreenSelectCornerSprites);
}

void drawRaceSplitscreenSelectOption0Frame(RaceSplitscreenSelectOption0FrameActor *arg0) {
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
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].centerTiles[tileOffset],
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
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].rightEdgeTiles[tileOffset],
            0,
            0x100
        );
        i = 0x80;
        drawMenuSpriteTile(
            (s16)(arg0->x + offset),
            (s16)(arg0->y + 0x80),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].bottomEdgeTiles[tileOffset],
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
        getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
        gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].cornerTile,
        0,
        0x100
    );

    drawMenuSprite(
        arg0->x - 4,
        (s16)(arg0->y - 4),
        getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
        0x33,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x - 4),
        (s16)(arg0->y + 0x8C),
        getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
        0x38,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0x8C),
        (s16)(arg0->y - 4),
        getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
        0x35,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0x8C),
        (s16)(arg0->y + 0x8C),
        getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
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
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            0x34,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)((arg0->x + borderOffset) + 0xC),
            (s16)(arg0->y + 0x8C),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            0x39,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)(arg0->x - 4),
            (s16)((arg0->y + borderOffset) + 0xC),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            0x36,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)(arg0->x + 0x8C),
            (s16)((arg0->y + borderOffset) + 0xC),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            0x37,
            0x20,
            0x20,
            0,
            0
        );
    }
}

void updateRaceSplitscreenSelectOption0Frame(RaceSplitscreenSelectOption0FrameActor *arg0) {
    int state;

    if ((gRaceSplitscreenMode >= (u16)arg0->optionIndex) && (arg0->state != 0) && (arg0->y != -0x48)) {
        state = arg0->state = 2;
    } else {
        state = arg0->state;
        if ((gRaceSplitscreenMode < (u16)arg0->optionIndex) && (state != 0) && (arg0->y != -0x140)) {
            state = arg0->state = 1;
        } else {
            state = arg0->state;
            if ((state != 0) && (state < 4)) {
                state = arg0->state = 3;
            }
        }
    }

    switch (state) {
        case 0:
            arg0->x -= 0x20;
            if (arg0->cornerSpawnTimer == 0) {
                createCallbackTask((CallbackTaskCallback)initRaceSplitscreenSelectCornerSprites, 0, 0x63);
            }
            arg0->cornerSpawnTimer++;
            if (arg0->x < -7) {
                arg0->x = -8;
                arg0->state = 3;
                gActiveMenuTask = createCallbackTask((CallbackTaskCallback)initRaceSplitscreenSelectCursor, 0, 0x64);
                createCallbackTask((CallbackTaskCallback)initRaceSplitscreenSelectPortrait, 0, 0x64);
                createCallbackTask((CallbackTaskCallback)initRaceSplitscreenSelectArrowPrompt, 0, 0x64);
                createCallbackTask((CallbackTaskCallback)initRaceSplitscreenSelectEntryFee, 0, 0x64);
            }
            state = arg0->state;
            break;
        case 1:
            arg0->y -= 0x24;
            if (arg0->y < -0x13F) {
                arg0->y = -0x140;
                arg0->state = 3;
            }
            state = arg0->state;
            break;
        case 2:
            arg0->y += 0x24;
            if (arg0->y >= -0x48) {
                arg0->y = -0x48;
                arg0->state = 3;
            }
            state = arg0->state;
            break;
        case 3:
            gMenuFlowState += 1;
            if (gRacePlayers[0].menuState == 1) {
                arg0->state = 4;
            }
            state = arg0->state;
            break;
        case 4:
            arg0->x += 0x20;
            if (arg0->x >= 0xA0) {
                arg0->state = 5;
            }
            state = arg0->state;
            break;
        case 5:
            gRacePlayers[0].menuState = 2;
            state = arg0->state;
            break;
    }

    if ((state == 5) && (gRacePlayers[0].menuState == 2)) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSplitscreenSelectOption0Frame, arg0);
}

void initRaceSplitscreenSelectOption0Frame(RaceSplitscreenSelectOption0FrameActor *arg0) {
    arg0->x = 0x94;
    arg0->y = -0x48;
    arg0->unused1C = -8;
    arg0->unused1E = -0x74;
    arg0->optionIndex = 0;
    arg0->cornerSpawnTimer = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSplitscreenSelectOption0Frame);
}

void drawRaceSplitscreenSelectOption1Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    s32 i;

    for (i = 0; i < 16; i++) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + ((i & 3) << 5)),
            (s16)(arg0->y + ((i / 4) << 5)),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].centerTiles[i],
            0,
            0x100,
#if defined(VERSION_JP)
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
#else
            0xA0,
            0x49
#endif
        );
    }

    for (i = 0; i < 2; i++) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + 0x80),
            (s16)(arg0->y + i * 0x40),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].rightEdgeTiles[i],
            0,
            0x100,
#if defined(VERSION_JP)
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
#else
            0xA0,
            0x49
#endif
        );
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + i * 0x40),
            (s16)(arg0->y + 0x80),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].bottomEdgeTiles[i],
            0,
            0x100,
#if defined(VERSION_JP)
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
#else
            0xA0,
            0x49
#endif
        );
    }

    drawMenuSpriteTileClipped(
        (s16)(arg0->x + 0x80),
        (s16)(arg0->y + 0x80),
        getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
        gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].cornerTile,
        0,
        0x100,
#if defined(VERSION_JP)
        gMenuViewportWidth / 2,
        (gMenuViewportHeight / 2) - 0x2F
#else
        0xA0,
        0x49
#endif
    );
}

void updateRaceSplitscreenSelectOption1Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    int state;

    if ((gRaceSplitscreenMode >= (u16)arg0->optionIndex) && (arg0->y != -0x48)) {
        state = arg0->state = 2;
    } else if ((gRaceSplitscreenMode < (u16)arg0->optionIndex) && (arg0->y != -0x140)) {
        state = arg0->state = 1;
    } else {
        state = arg0->state;
        if (state < 4) {
            state = arg0->state = 3;
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
                arg0->state = 3;
            }
            state = arg0->state;
            break;
        case 2:
            arg0->y += 0x24;
            if (arg0->y >= -0x48) {
                arg0->y = -0x48;
                arg0->state = 3;
            }
            state = arg0->state;
            break;
        case 3:
            gMenuFlowState += 1;
            if (gRacePlayers[0].menuState == 1) {
                if (arg0->y == -0x140) {
                    arg0->state = 5;
                } else {
                    arg0->state = 4;
                }
            }
            state = arg0->state;
            break;
        case 4:
            arg0->x += 0x20;
            if (arg0->x >= 0xA0) {
                arg0->state = 5;
            }
            state = arg0->state;
            break;
    }

    if (state == 5) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSplitscreenSelectOption1Frame, arg0);
}

void initRaceSplitscreenSelectOption1Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->optionIndex = 1;
    arg0->unusedTimer = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSplitscreenSelectOption1Frame);
}

void drawRaceSplitscreenSelectOption2Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    s32 i;

    for (i = 0; i < 16; i++) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + ((i & 3) << 5)),
            (s16)(arg0->y + ((i / 4) << 5)),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].centerTiles[i],
            0,
            0x100,
#if defined(VERSION_JP)
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
#else
            0xA0,
            0x49
#endif
        );
    }

    for (i = 0; i < 2; i++) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + 0x80),
            (s16)(arg0->y + i * 0x40),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].rightEdgeTiles[i],
            0,
            0x100,
#if defined(VERSION_JP)
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
#else
            0xA0,
            0x49
#endif
        );
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + i * 0x40),
            (s16)(arg0->y + 0x80),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].bottomEdgeTiles[i],
            0,
            0x100,
#if defined(VERSION_JP)
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
#else
            0xA0,
            0x49
#endif
        );
    }

    drawMenuSpriteTileClipped(
        (s16)(arg0->x + 0x80),
        (s16)(arg0->y + 0x80),
        getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
        gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].cornerTile,
        0,
        0x100,
#if defined(VERSION_JP)
        gMenuViewportWidth / 2,
        (gMenuViewportHeight / 2) - 0x2F
#else
        0xA0,
        0x49
#endif
    );
}

void updateRaceSplitscreenSelectOption2Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    int state;

    if ((gRaceSplitscreenMode >= (u16)arg0->optionIndex) && (arg0->y != -0x48)) {
        state = arg0->state = 2;
    } else if ((gRaceSplitscreenMode < (u16)arg0->optionIndex) && (arg0->y != -0x140)) {
        state = arg0->state = 1;
    } else {
        state = arg0->state;
        if (state < 4) {
            state = arg0->state = 3;
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
                arg0->state = 3;
            }
            state = arg0->state;
            break;
        case 2:
            arg0->y += 0x24;
            if (arg0->y >= -0x48) {
                arg0->y = -0x48;
                arg0->state = 3;
            }
            state = arg0->state;
            break;
        case 3:
            gMenuFlowState += 1;
            if (gRacePlayers[0].menuState == 1) {
                if (arg0->y == -0x140) {
                    arg0->state = 5;
                } else {
                    arg0->state = 4;
                }
            }
            state = arg0->state;
            break;
        case 4:
            arg0->x += 0x20;
            if (arg0->x >= 0xA0) {
                arg0->state = 5;
            }
            state = arg0->state;
            break;
    }

    if (state == 5) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSplitscreenSelectOption2Frame, arg0);
}

void initRaceSplitscreenSelectOption2Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->optionIndex = 2;
    arg0->unusedTimer = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSplitscreenSelectOption2Frame);
}

void drawRaceSplitscreenSelectOption3Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    s32 i;

    for (i = 0; i < 16; i++) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + ((i & 3) << 5)),
            (s16)(arg0->y + ((i / 4) << 5)),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].centerTiles[i],
            0,
            0x100,
#if defined(VERSION_JP)
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
#else
            0xA0,
            0x49
#endif
        );
    }

    for (i = 0; i < 2; i++) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + 0x80),
            (s16)(arg0->y + i * 0x40),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].rightEdgeTiles[i],
            0,
            0x100,
#if defined(VERSION_JP)
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
#else
            0xA0,
            0x49
#endif
        );
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + i * 0x40),
            (s16)(arg0->y + 0x80),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].bottomEdgeTiles[i],
            0,
            0x100,
#if defined(VERSION_JP)
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
#else
            0xA0,
            0x49
#endif
        );
    }

    drawMenuSpriteTileClipped(
        (s16)(arg0->x + 0x80),
        (s16)(arg0->y + 0x80),
        getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
        gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].cornerTile,
        0,
        0x100,
#if defined(VERSION_JP)
        gMenuViewportWidth / 2,
        (gMenuViewportHeight / 2) - 0x2F
#else
        0xA0,
        0x49
#endif
    );
}

void updateRaceSplitscreenSelectOption3Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    int state;

    if ((gRaceSplitscreenMode >= (u16)arg0->optionIndex) && (arg0->y != -0x48)) {
        state = arg0->state = 2;
    } else if ((gRaceSplitscreenMode < (u16)arg0->optionIndex) && (arg0->y != -0x140)) {
        state = arg0->state = 1;
    } else {
        state = arg0->state;
        if (state < 4) {
            state = arg0->state = 3;
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
                arg0->state = 3;
            }
            state = arg0->state;
            break;
        case 2:
            arg0->y += 0x24;
            if (arg0->y >= -0x48) {
                arg0->y = -0x48;
                arg0->state = 3;
            }
            state = arg0->state;
            break;
        case 3:
            gMenuFlowState += 1;
            if (gRacePlayers[0].menuState == 1) {
                if (arg0->y == -0x140) {
                    arg0->state = 5;
                } else {
                    arg0->state = 4;
                }
            }
            state = arg0->state;
            break;
        case 4:
            arg0->x += 0x20;
            if (arg0->x >= 0xA0) {
                arg0->state = 5;
            }
            state = arg0->state;
            break;
    }

    if (state == 5) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSplitscreenSelectOption3Frame, arg0);
}

void initRaceSplitscreenSelectOption3Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->optionIndex = 3;
    arg0->unusedTimer = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSplitscreenSelectOption3Frame);
}

void drawRaceSplitscreenSelectOption4Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    s32 i;

    for (i = 0; i < 16; i++) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + ((i & 3) << 5)),
            (s16)(arg0->y + ((i / 4) << 5)),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].centerTiles[i],
            0,
            0x100,
#if defined(VERSION_JP)
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
#else
            0xA0,
            0x49
#endif
        );
    }

    for (i = 0; i < 2; i++) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + 0x80),
            (s16)(arg0->y + i * 0x40),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].rightEdgeTiles[i],
            0,
            0x100,
#if defined(VERSION_JP)
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
#else
            0xA0,
            0x49
#endif
        );
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + i * 0x40),
            (s16)(arg0->y + 0x80),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].bottomEdgeTiles[i],
            0,
            0x100,
#if defined(VERSION_JP)
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
#else
            0xA0,
            0x49
#endif
        );
    }

    drawMenuSpriteTileClipped(
        (s16)(arg0->x + 0x80),
        (s16)(arg0->y + 0x80),
        getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
        gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].cornerTile,
        0,
        0x100,
#if defined(VERSION_JP)
        gMenuViewportWidth / 2,
        (gMenuViewportHeight / 2) - 0x2F
#else
        0xA0,
        0x49
#endif
    );
}

void updateRaceSplitscreenSelectOption4Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    int state;

    gMenuFlowState = 0;
    if ((gRaceSplitscreenMode >= (u16)arg0->optionIndex) && (arg0->y != -0x48)) {
        state = arg0->state = 2;
    } else if ((gRaceSplitscreenMode < (u16)arg0->optionIndex) && (arg0->y != -0x140)) {
        state = arg0->state = 1;
    } else {
        state = arg0->state;
        if (state < 4) {
            state = arg0->state = 3;
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
                arg0->state = 3;
            }
            state = arg0->state;
            break;
        case 2:
            arg0->y += 0x24;
            if (arg0->y >= -0x48) {
                arg0->y = -0x48;
                arg0->state = 3;
            }
            state = arg0->state;
            break;
        case 3:
            gMenuFlowState += 1;
            if (gRacePlayers[0].menuState == 1) {
                if (arg0->y == -0x140) {
                    arg0->state = 5;
                } else {
                    arg0->state = 4;
                }
            }
            state = arg0->state;
            break;
        case 4:
            arg0->x += 0x20;
            if (arg0->x >= 0xA0) {
                arg0->state = 5;
            }
            state = arg0->state;
            break;
    }

    if (state == 5) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSplitscreenSelectOption4Frame, arg0);
}

void initRaceSplitscreenSelectOption4Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->optionIndex = 4;
    arg0->unusedTimer = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSplitscreenSelectOption4Frame);
}

void drawRaceSplitscreenSelectCursor(RaceSplitscreenSelectAnimatedWidgetActor *arg0) {
    drawMenuSpriteWithAlpha(
        arg0->x,
        (s16)(arg0->y + (gRaceSplitscreenMode * 0x14)),
        getRelocatableHeapBlockBase(ASSET_HANDLE(33)),
        7,
        0x20,
        0x20,
        0,
        arg0->alpha,
        0
    );
}

void updateRaceSplitscreenSelectCursor(RaceSplitscreenSelectAnimatedWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->state;
    if (state != (globalState = gRaceSplitscreenSelectCursorTarget.state)) {
        arg0->state = globalState;
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        state = globalState;
        arg0->alpha = gRaceSplitscreenSelectCursorTarget.alphaTarget;
    }

    switch (state) {
        case 0:
            arg0->alpha += 0x26;
            if (arg0->alpha >= 0x100) {
                arg0->alpha = 0x100;
                arg0->state = 1;
            }
            state = arg0->state;
            break;
        case 1:
            if ((s32)arg0->animationTimer < 0x10) {
                arg0->alpha -= 9;
            } else {
                arg0->alpha += 9;
            }
            state = arg0->state;
            arg0->animationTimer = (arg0->animationTimer + 1) & 0x1F;
            break;
        case 2:
            if (gRacePlayers[0].menuState == 1) {
                state = arg0->state = 3;
            }
            break;
        case 3:
            arg0->x -= 0x20;
            if (arg0->x < -0xEF) {
                arg0->state = 4;
            }
            state = arg0->state;
            break;
        case 4:
            break;
    }

    gRaceSplitscreenSelectCursorAnimState = state;
    if (arg0->state == 4) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSplitscreenSelectCursor, arg0);
}

void initRaceSplitscreenSelectCursor(RaceSplitscreenSelectAnimatedWidgetActor *arg0) {
    arg0->x = -0x7C;
    arg0->y = -0x60;
    arg0->alpha = 0;
    arg0->state = 0;
    arg0->animationTimer = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSplitscreenSelectCursor);
}

void drawRaceSplitscreenSelectPortrait(RaceSplitscreenSelectAnimatedWidgetActor *arg0) {
    s32 portraitIndex;

    if (gRaceSplitscreenMode == 3) {
        portraitIndex = gRaceSplitscreenMode & 0xFF;
        if (gRacePlayers[0].selectedCharacterId == 5) {
            portraitIndex = 5;
        }
    } else {
        portraitIndex = gRaceSplitscreenMode & 0xFF;
    }
    drawMenuGlyphScript(
        arg0->x,
        arg0->y,
        gRaceSplitscreenSelectDescriptionScripts.descriptions[portraitIndex],
        1,
        arg0->alpha,
        0
    );
}

void updateRaceSplitscreenSelectPortrait(RaceSplitscreenSelectAnimatedWidgetActor *arg0) {
    u8 state = arg0->state;

    switch (state) {
        case 0:
            arg0->alpha += 0x26;
            if (arg0->alpha >= 0x100) {
                arg0->alpha = 0x100;
                arg0->state = 1;
            }
            break;
        case 1:
            if (gRacePlayers[0].menuState == 1) {
                arg0->state = 2;
            }
            break;
        case 2:
            arg0->x -= 0x20;
            if (arg0->x < -0xFF) {
                arg0->state = 3;
            }
            break;
        case 3:
            break;
    }
    gRaceSplitscreenSelectCursorTarget.portraitAlpha = arg0->alpha;
    if (arg0->state == 3) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSplitscreenSelectPortrait, arg0);
}

void initRaceSplitscreenSelectPortrait(RaceSplitscreenSelectAnimatedWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = 0xC;
    arg0->alpha = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSplitscreenSelectPortrait);
}

void drawRaceSplitscreenSelectArrowPrompt(RaceSplitscreenSelectAnimatedWidgetActor *arg0) {
    drawMenuSpriteWithAlpha(
        arg0->x,
        arg0->y,
        getRelocatableHeapBlockBase(ASSET_HANDLE(33)),
        0,
        0x20,
        0x20,
        0,
        arg0->alpha,
        0
    );
}

void updateRaceSplitscreenSelectArrowPrompt(RaceSplitscreenSelectAnimatedWidgetActor *arg0) {
    u8 state = arg0->state;

    switch (state) {
        case 0:
            arg0->alpha += 0x26;
            if (arg0->alpha >= 0x100) {
                arg0->alpha = 0x100;
                arg0->state = 1;
            }
            state = arg0->state;
            break;
        case 1:
            if (gRacePlayers[0].menuState == 1) {
                state = arg0->state = 2;
            }
            break;
        case 2:
            arg0->x += 0x20;
            if (arg0->x >= 0xA0) {
                arg0->state = 3;
            }
            state = arg0->state;
            break;
        case 3:
            break;
    }
    if ((unsigned int)state == 3) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSplitscreenSelectArrowPrompt, arg0);
}

void initRaceSplitscreenSelectArrowPrompt(RaceSplitscreenSelectAnimatedWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x5C;
    arg0->alpha = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSplitscreenSelectArrowPrompt);
}

void drawRaceSplitscreenSelectEntryFee(RaceSplitscreenSelectAnimatedWidgetActor *arg0) {
    char sp40[0x18];

    drawMenuPanelBackdrop(arg0->x, arg0->y, 0x5000, 0x4000);
    drawMenuSpriteWithAlpha(
        (s16)(arg0->x + 8),
        (s16)(arg0->y + 4),
        getRelocatableHeapBlockBase(ASSET_HANDLE(33)),
        0x11,
        0x20,
        0x20,
        0,
        arg0->alpha,
        0
    );
    sprintf(sp40, gRaceSplitscreenSelectEntryFeeFormat, gRacePlayers[0].money);
    drawMenuAsciiText((s16)(arg0->x + 0x10), (s16)(arg0->y + 0x10), sp40, 0, arg0->alpha);
}

void updateRaceSplitscreenSelectEntryFee(RaceSplitscreenSelectAnimatedWidgetActor *arg0) {
    u8 state = arg0->state;

    switch (state) {
        case 0:
            arg0->alpha += 0x26;
            if (arg0->alpha >= 0x100) {
                arg0->alpha = 0x100;
                arg0->state = 1;
            }
            state = arg0->state;
            break;
        case 1:
            if (gRacePlayers[0].menuState == 1) {
                state = arg0->state = 2;
            }
            break;
        case 2:
            arg0->x += 0x20;
            if (arg0->x >= 0xA0) {
                arg0->state = 3;
            }
            state = arg0->state;
            break;
        case 3:
            break;
    }
    if ((unsigned int)state == 3) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSplitscreenSelectEntryFee, arg0);
}

void initRaceSplitscreenSelectEntryFee(RaceSplitscreenSelectAnimatedWidgetActor *arg0) {
    arg0->x = 0x30;
    arg0->y = 0x40;
    arg0->alpha = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSplitscreenSelectEntryFee);
}

void drawMenuPanelBackdrop(s32 x, s32 y, s32 width, s32 height) {
    /* These scratch declarations and their scopes reproduce IDO's texture-load spill layout. */
    Gfx *volatile unused0;
    Gfx *volatile unused1;
    Gfx *volatile unused2;
    register s32 widthU;
    register s32 heightU;
    s32 ulx;
    s32 uly;

    gDPPipeSync(gRegionAllocPtr++);
    gDPSetTextureLUT(gRegionAllocPtr++, G_TT_NONE);
    gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEI_PRIM, G_CC_MODULATEI_PRIM);
    gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPLoadTextureTile_4b(
        gRegionAllocPtr++,
        gMenuPanelBackdropTexture,
        G_IM_FMT_I,
        16,
        8,
        0,
        0,
        16,
        8,
        0,
        G_TX_NOMIRROR | G_TX_CLAMP,
        G_TX_NOMIRROR | G_TX_CLAMP,
        G_TX_NOMASK,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOLOD
    );
    gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0, 0, 0, 0x64);

    /* The repeated cast assignment is required for the original register allocation. */
    widthU = (u16)width;
    widthU = (u16)width;
    heightU = (u16)height;
    ulx = ((s16)x + gMenuViewportCenterX) << 2;
    uly = ((s16)y + gMenuViewportCenterY) << 2;

    gSPTextureRectangle(
        gRegionAllocPtr++,
        ulx,
        uly,
        ulx + (((widthU << 4) << 2) / 0x1000),
        (((heightU << 3) << 2) / 0x1000) + uly,
        G_TX_RENDERTILE,
        0,
        0,
        (0x400000 / widthU) & 0xFFFFU,
        (0x400000 / (u16)height) & 0xFFFF
    );
    gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    { Gfx *volatile unused3; }
}
