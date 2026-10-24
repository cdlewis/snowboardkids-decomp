#include "common.h"
#include "game/engine/system_render.h"
#include "game/race/race_state.h"
#include "font_encoding.h"
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
#include "game/menu/race_setup/race_setup_menu.h"
#include "game/menu/splitscreen_select/race_splitscreen_select_menu.h"

#define RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE (gAssetHandles[0x21])
#define ASSET_HANDLE(index) (gAssetHandles[(index)])

void drawRaceSplitscreenSelectPlayerCountIcons(RaceSplitscreenSelectOptionIconActor *icons) {
    RaceSplitscreenSelectOptionIconActor *savedIcons;
    s32 iconIndex;
    s32 blinkAlpha;
    s32 yOffset;
    u16 tileIndex;

    savedIcons = icons;
    iconIndex = 0;
    if (iconIndex < (s32)icons->visibleCount) {
        /*
         * The outer do/while (0), repeated iconIndex initialization, empty if,
         * and paired statements must retain this shape for IDO's register
         * allocation. The icon position itself is nevertheless a normal array
         * access; the compiler converts it to the pointer walk in the ROM.
         */
        // clang-format off
        do { iconIndex = 0; yOffset = 0; do {
                // clang-format on
                blinkAlpha = 0;
                if (((((gMenuSelectionConfirmTimer != 0) && (((s32)gMenuSelectionConfirmTimer) < 8)) &&
                      (gMenuExitSelection == 0)) &&
                     (iconIndex == gRaceSplitscreenMode)) &&
                    (gMenuSelectionConfirmTimer & 1)) {
                    blinkAlpha = 0xFF;
                }
                tileIndex = iconIndex + 8;
                drawMenuSprite(
                    icons->x[iconIndex],
                    (s16)(icons->y + yOffset),
                    getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
                    tileIndex,
                    0x20,
                    0x20,
                    0,
                    blinkAlpha
                );
                yOffset += 0x14;
                iconIndex++;
                if (icons->visibleCount) {}
            } while (iconIndex < ((s32)savedIcons->visibleCount));
        } while (0);
    }
}

// clang-format off
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
// clang-format on

void initRaceSplitscreenSelectPlayerCountIcons(RaceSplitscreenSelectOptionIconActor *arg0) {
    s32 i;

    // clang-format off
    for (i = 0; i < 5; i++) { arg0->x[i] = -0x104; }
    // clang-format on

    arg0->y = -0x60;
    arg0->spawnTimer = 0;
    arg0->visibleCount = 1;
    arg0->state = 0;

    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSplitscreenSelectPlayerCountIcons);
}

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

// clang-format off
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
// clang-format on

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

// clang-format off
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
// clang-format on

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

// clang-format off
// drawRaceSplitscreenSelectOption1Frame best standalone match: 99.952% (base_14.c)
#pragma GLOBAL_ASM("asm.jp/nonmatchings/menu/splitscreen_select/race_splitscreen_select_ui/drawRaceSplitscreenSelectOption1Frame.s")

#ifdef NON_MATCHING
void drawRaceSplitscreenSelectOption1Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + ((i & 3) << 5)),
            (s16)(arg0->y + ((i / 4) << 5)),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].centerTiles[tileOffset],
            0,
            0x100,
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
        );
    }

    tileOffset = 0;
    i = 0x80;
    offset = 0;
    for (;;) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + 0x80),
            (s16)(arg0->y + offset),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].rightEdgeTiles[tileOffset],
            0,
            0x100,
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
        );
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + offset),
            (s16)(arg0->y + 0x80),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].bottomEdgeTiles[tileOffset],
            0,
            0x100,
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
        );
        i = 0x80;
        offset += 0x40;
        tileOffset++;

        if (!(offset != i))
            break;
    }
    i++;
    i--;

    drawMenuSpriteTileClipped(
        (s16)(arg0->x + 0x80),
        (s16)(arg0->y + 0x80),
        getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
        gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].cornerTile,
        0,
        0x100,
        gMenuViewportWidth / 2,
        (gMenuViewportHeight / 2) - 0x2F
    );
}
#endif
// clang-format on

// clang-format off
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
// clang-format on

void initRaceSplitscreenSelectOption1Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->optionIndex = 1;
    arg0->unusedTimer = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSplitscreenSelectOption1Frame);
}

// clang-format off
// drawRaceSplitscreenSelectOption2Frame best standalone match: 99.952% (base_4.c)
#pragma GLOBAL_ASM("asm.jp/nonmatchings/menu/splitscreen_select/race_splitscreen_select_ui/drawRaceSplitscreenSelectOption2Frame.s")

#ifdef NON_MATCHING
void drawRaceSplitscreenSelectOption2Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + ((i & 3) << 5)),
            (s16)(arg0->y + ((i / 4) << 5)),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].centerTiles[tileOffset],
            0,
            0x100,
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
        );
    }

    tileOffset = 0;
    i = 0x80;
    offset = 0;
    for (;;) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + 0x80),
            (s16)(arg0->y + offset),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].rightEdgeTiles[tileOffset],
            0,
            0x100,
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
        );
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + offset),
            (s16)(arg0->y + 0x80),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].bottomEdgeTiles[tileOffset],
            0,
            0x100,
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
        );
        i = 0x80;
        offset += 0x40;
        tileOffset++;

        if (!(offset != i))
            break;
    }
    i++;
    i--;

    drawMenuSpriteTileClipped(
        (s16)(arg0->x + 0x80),
        (s16)(arg0->y + 0x80),
        getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
        gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].cornerTile,
        0,
        0x100,
        gMenuViewportWidth / 2,
        (gMenuViewportHeight / 2) - 0x2F
    );
}
#endif
// clang-format on

// clang-format off
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
// clang-format on

void initRaceSplitscreenSelectOption2Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->optionIndex = 2;
    arg0->unusedTimer = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSplitscreenSelectOption2Frame);
}

// clang-format off
// drawRaceSplitscreenSelectOption3Frame best standalone match: 99.952% (base_4.c)
#pragma GLOBAL_ASM("asm.jp/nonmatchings/menu/splitscreen_select/race_splitscreen_select_ui/drawRaceSplitscreenSelectOption3Frame.s")

#ifdef NON_MATCHING
void drawRaceSplitscreenSelectOption3Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + ((i & 3) << 5)),
            (s16)(arg0->y + ((i / 4) << 5)),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].centerTiles[tileOffset],
            0,
            0x100,
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
        );
    }

    tileOffset = 0;
    i = 0x80;
    offset = 0;
    for (;;) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + 0x80),
            (s16)(arg0->y + offset),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].rightEdgeTiles[tileOffset],
            0,
            0x100,
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
        );
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + offset),
            (s16)(arg0->y + 0x80),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].bottomEdgeTiles[tileOffset],
            0,
            0x100,
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
        );
        i = 0x80;
        offset += 0x40;
        tileOffset++;

        if (!(offset != i))
            break;
    }
    i++;
    i--;

    drawMenuSpriteTileClipped(
        (s16)(arg0->x + 0x80),
        (s16)(arg0->y + 0x80),
        getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
        gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].cornerTile,
        0,
        0x100,
        gMenuViewportWidth / 2,
        (gMenuViewportHeight / 2) - 0x2F
    );
}
#endif
// clang-format on

// clang-format off
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
// clang-format on

void initRaceSplitscreenSelectOption3Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->optionIndex = 3;
    arg0->unusedTimer = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSplitscreenSelectOption3Frame);
}

// clang-format off
// drawRaceSplitscreenSelectOption4Frame best standalone match: 99.952% (base_4.c)
#pragma GLOBAL_ASM("asm.jp/nonmatchings/menu/splitscreen_select/race_splitscreen_select_ui/drawRaceSplitscreenSelectOption4Frame.s")

#ifdef NON_MATCHING
void drawRaceSplitscreenSelectOption4Frame(RaceSplitscreenSelectOptionFrameActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + ((i & 3) << 5)),
            (s16)(arg0->y + ((i / 4) << 5)),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].centerTiles[tileOffset],
            0,
            0x100,
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
        );
    }

    tileOffset = 0;
    i = 0x80;
    offset = 0;
    for (;;) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + 0x80),
            (s16)(arg0->y + offset),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].rightEdgeTiles[tileOffset],
            0,
            0x100,
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
        );
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + offset),
            (s16)(arg0->y + 0x80),
            getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
            gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].bottomEdgeTiles[tileOffset],
            0,
            0x100,
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
        );
        i = 0x80;
        offset += 0x40;
        tileOffset++;

        if (!(offset != i))
            break;
    }
    i++;
    i--;

    drawMenuSpriteTileClipped(
        (s16)(arg0->x + 0x80),
        (s16)(arg0->y + 0x80),
        getRelocatableHeapBlockBase(RACE_SPLITSCREEN_SELECT_TEXTURE_HANDLE),
        gRaceSplitscreenSelectFrameTiles[(u16)arg0->optionIndex].cornerTile,
        0,
        0x100,
        gMenuViewportWidth / 2,
        (gMenuViewportHeight / 2) - 0x2F
    );
}
#endif
// clang-format on

// clang-format off
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
// clang-format on

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

// clang-format off
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
// clang-format on

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

// clang-format off
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
// clang-format on

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

// clang-format off
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
// clang-format on

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

// clang-format off
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
// clang-format on

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
