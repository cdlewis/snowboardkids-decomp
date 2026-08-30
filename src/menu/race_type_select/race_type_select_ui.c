#include "common.h"
#include "font_encoding.h"
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/race_type_select/race_type_select_ui.h"
#include "game/menu/splitscreen_select/race_splitscreen_select_ui.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/race/race_state.h"
#include "game/race/player/race_player_input.h"

#define ASSET_HANDLE(index) (gAssetHandles[(index)])
RaceTypeSelectFrameTileTable gRaceTypeSelectFrameTileMaps = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000E, 0x000F,
     0x0010, 0x0011, 0x0004, 0x000D, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x001A, 0x001B, 0x001C,
     0x001D, 0x001E, 0x001F, 0x0020, 0x0021, 0x0023, 0x0024, 0x0025, 0x0026, 0x0019, 0x0022, 0x0027, 0x0028, 0x0029,
     0x002A, 0x002B, 0x002C, 0x002D, 0x002F, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0038, 0x0039,
     0x003A, 0x003B, 0x002E, 0x0037, 0x003C, 0x003D, 0x003E, 0x00A4, 0x00A5, 0x00A6, 0x00A7, 0x00A9, 0x00AA, 0x00AB,
     0x00AC, 0x00AD, 0x00AE, 0x00AF, 0x00B0, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00A8, 0x00B1, 0x00B6, 0x00B7, 0x00B8,
};

RaceTypeSelectDescriptionScriptTable gRaceTypeSelectPortraitScripts = {
    {
     {
            // textconv requires these _() invocations to retain their original line layout.
            // clang-format off
            _("{COLOR:6}COLLECT\nSPEED FANS\n{COLOR:7}TO GET THE\nFASTEST\nTIME."),
        },
        {
            _("{COLOR:6}SHOOT\nSNOWMEN\n{COLOR:7}WHILE GOING\nDOWN THE\nCOURSE."),
        },
        {
            _("{COLOR:7}GET POINTS\nBY {COLOR:6}DOING\nTRICKS{COLOR:7}."),
        },
        {
            _("{COLOR:7}RETURN TO\nTHE GAME\nMENU."),
        },
    },
    {
            _("SKILL GAME SELECT"),
     // clang-format on
    },
};

extern u8 gMenuExitSelection;
extern s32 gMenuFlowState;

const char gRaceTypeSelectEntryFeeFormat[] = "%6dG";

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void drawRaceTypeSelectOptionIcons(RaceTypeSelectOptionIconActor *arg0) {
    RaceTypeSelectOptionIconActor *savedArg;
    s32 i;
    s32 alpha;
    s32 yOffset;
    u16 tileIndex;

    savedArg = arg0;
 do { i = 0; if (arg0->visibleCount > 0) { yOffset = 0; do { alpha = 0; if (((((gMenuSelectionConfirmTimer > 0) && (gMenuSelectionConfirmTimer < 8)) && (gMenuExitSelection == 0)) && (i == gRaceTypeSelection)) && (gMenuSelectionConfirmTimer & 1)) { if ((!savedArg->visibleCount) && (!savedArg->visibleCount)) { } alpha = 0xFF; } tileIndex = i + 0xD; drawMenuSprite(arg0->x[i], (s16) (arg0->y + yOffset), getRelocatableHeapBlockBase(gAssetHandles[0x21]), tileIndex, 0x20, 0x20, 0, alpha); yOffset += 0x18; i++; } while (i < savedArg->visibleCount); } } while (0);
}
// clang-format on

void updateRaceTypeSelectOptionIcons(RaceTypeSelectOptionIconActor *arg0) {
    s32 i;
    s32 moved;
    s32 state;
    int stateByte;
    RaceTypeSelectOptionIconActor *row;
    RaceTypeSelectOptionIconActor *actor;

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
                if (row->visibleCount < 4) {
                    row->visibleCount++;
                }
            }

            if (moved == 0) {
                row->state = 1;
                createCallbackTask((CallbackTaskCallback)initRaceTypeSelectOption0Frame, 0, 0x5F);
                createCallbackTask((CallbackTaskCallback)initRaceTypeSelectOption1Frame, 0, 0x60);
                createCallbackTask((CallbackTaskCallback)initRaceTypeSelectOption2Frame, 0, 0x61);
                createCallbackTask((CallbackTaskCallback)initRaceTypeSelectOption3Frame, 0, 0x63);
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceTypeSelectOptionIcons, actor);
}

void initRaceTypeSelectOptionIcons(RaceTypeSelectOptionIconActor *arg0) {
    s32 temp_v1 = -0x104;
    long long zero;
    s32 temp_t6 = -0x58;
    s32 temp_t7 = 1;

    arg0->x[0] = temp_v1;
    arg0->x[1] = temp_v1;
    arg0->x[2] = temp_v1;
    arg0->x[3] = temp_v1;
    arg0->y = temp_t6;
    zero = 0;
    arg0->spawnTimer = zero;
    arg0->visibleCount = temp_t7;
    arg0->state = zero;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceTypeSelectOptionIcons);
}

void drawRaceTypeSelectCornerSprites(RaceTypeSelectCornerSpriteActor *arg0) {
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

void updateRaceTypeSelectCornerSprites(RaceTypeSelectCornerSpriteActor *arg0) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceTypeSelectCornerSprites, arg0);
}

void initRaceTypeSelectCornerSprites(RaceTypeSelectCornerSpriteActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 8;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceTypeSelectCornerSprites);
}

void drawRaceTypeSelectOption0Frame(RaceTypeSelectOption0FrameActor *arg0) {
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
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            gRaceTypeSelectFrameTileMaps.optionFrames[(u16)arg0->optionIndex].centerTiles[tileOffset],
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
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            gRaceTypeSelectFrameTileMaps.optionFrames[(u16)arg0->optionIndex].rightEdgeTiles[tileOffset],
            0,
            0x100
        );
        i = 0x80;
        drawMenuSpriteTile(
            (s16)(arg0->x + offset),
            (s16)(arg0->y + 0x80),
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            gRaceTypeSelectFrameTileMaps.optionFrames[(u16)arg0->optionIndex].bottomEdgeTiles[tileOffset],
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
        getRelocatableHeapBlockBase(gAssetHandles[0x25]),
        gRaceTypeSelectFrameTileMaps.optionFrames[(u16)arg0->optionIndex].cornerTile,
        0,
        0x100
    );

    drawMenuSprite(
        (s16)(arg0->x - 4),
        (s16)(arg0->y - 4),
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        0x33,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x - 4),
        (s16)(arg0->y + 0x8C),
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        0x38,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0x8C),
        (s16)(arg0->y - 4),
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        0x35,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0x8C),
        (s16)(arg0->y + 0x8C),
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
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
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            0x34,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)((arg0->x + borderOffset) + 0xC),
            (s16)(arg0->y + 0x8C),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            0x39,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)(arg0->x - 4),
            (s16)((arg0->y + borderOffset) + 0xC),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            0x36,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)(arg0->x + 0x8C),
            (s16)((arg0->y + borderOffset) + 0xC),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            0x37,
            0x20,
            0x20,
            0,
            0
        );
    }
}

void updateRaceTypeSelectOption0Frame(RaceTypeSelectOption0FrameActor *arg0) {
    int state;

    if ((gRaceTypeSelection >= (u16)arg0->optionIndex) && (arg0->state != 0) && (arg0->y != -0x48)) {
        state = arg0->state = 2;
    } else {
        state = arg0->state;
        if ((gRaceTypeSelection < (u16)arg0->optionIndex) && (state != 0) && (arg0->y != -0x140)) {
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
                createCallbackTask((CallbackTaskCallback)initRaceTypeSelectCornerSprites, 0, 0x63);
            }
            arg0->cornerSpawnTimer++;
            if (arg0->x < -7) {
                arg0->x = -8;
                arg0->state = 3;
                gActiveMenuTask = createCallbackTask((CallbackTaskCallback)initRaceTypeSelectCursor, 0, 0x64);
                createCallbackTask((CallbackTaskCallback)initRaceTypeSelectPortrait, 0, 0x64);
                createCallbackTask((CallbackTaskCallback)initRaceTypeSelectArrowPrompt, 0, 0x64);
                createCallbackTask((CallbackTaskCallback)initRaceTypeSelectEntryFee, 0, 0x64);
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceTypeSelectOption0Frame, arg0);
}

void initRaceTypeSelectOption0Frame(RaceTypeSelectOption0FrameActor *arg0) {
    arg0->x = 0x94;
    arg0->y = -0x48;
    arg0->unused1C = -8;
    arg0->unused1E = -0x74;
    arg0->optionIndex = 0;
    arg0->cornerSpawnTimer = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceTypeSelectOption0Frame);
}

void drawRaceTypeSelectOption1Frame(RaceTypeSelectOptionFrameActor *arg0) {
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
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            gRaceTypeSelectFrameTileMaps.optionFrames[(u16)arg0->optionIndex].centerTiles[tileOffset],
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
            (s16)(arg0->x + 0x80),
            (s16)(arg0->y + offset),
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            gRaceTypeSelectFrameTileMaps.optionFrames[(u16)arg0->optionIndex].rightEdgeTiles[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + offset),
            (s16)(arg0->y + 0x80),
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            gRaceTypeSelectFrameTileMaps.optionFrames[(u16)arg0->optionIndex].bottomEdgeTiles[tileOffset],
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
        (s16)(arg0->x + 0x80),
        (s16)(arg0->y + 0x80),
        getRelocatableHeapBlockBase(gAssetHandles[0x25]),
        gRaceTypeSelectFrameTileMaps.optionFrames[(u16)arg0->optionIndex].cornerTile,
        0,
        0x100,
        0xA0,
        0x49
    );
}

void updateRaceTypeSelectOption1Frame(RaceTypeSelectOptionFrameActor *arg0) {
    int state;

    if ((gRaceTypeSelection >= (u16)arg0->optionIndex) && (arg0->y != -0x48)) {
        state = arg0->state = 2;
    } else if ((gRaceTypeSelection < (u16)arg0->optionIndex) && (arg0->y != -0x140)) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceTypeSelectOption1Frame, arg0);
}

void initRaceTypeSelectOption1Frame(RaceTypeSelectOptionFrameActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->optionIndex = 1;
    arg0->unusedTimer = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceTypeSelectOption1Frame);
}

void drawRaceTypeSelectOption2Frame(RaceTypeSelectOptionFrameActor *arg0) {
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
            gRaceTypeSelectFrameTileMaps.optionFrames[(u16)arg0->optionIndex].centerTiles[tileOffset],
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
            gRaceTypeSelectFrameTileMaps.optionFrames[(u16)arg0->optionIndex].rightEdgeTiles[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        drawMenuSpriteTileClipped(
            arg0->x + offset,
            arg0->y + 0x80,
            getRelocatableHeapBlockBase(gAssetHandles[0x25]),
            gRaceTypeSelectFrameTileMaps.optionFrames[(u16)arg0->optionIndex].bottomEdgeTiles[tileOffset],
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
        gRaceTypeSelectFrameTileMaps.optionFrames[(u16)arg0->optionIndex].cornerTile,
        0,
        0x100,
        0xA0,
        0x49
    );
}

void updateRaceTypeSelectOption2Frame(RaceTypeSelectOptionFrameActor *arg0) {
    int state;

    if ((gRaceTypeSelection >= (u16)arg0->optionIndex) && (arg0->y != -0x48)) {
        state = arg0->state = 2;
    } else if ((gRaceTypeSelection < (u16)arg0->optionIndex) && (arg0->y != -0x140)) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceTypeSelectOption2Frame, arg0);
}

void initRaceTypeSelectOption2Frame(RaceTypeSelectOptionFrameActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->optionIndex = 2;
    arg0->unusedTimer = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceTypeSelectOption2Frame);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void drawRaceTypeSelectOption3Frame(RaceTypeSelectExitFrameActor *arg0) {
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5), getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                      gRaceTypeSelectFrameTileMaps.exitFrame.centerTiles[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    tileOffset = 0;
    offset = 0; i = 0x80; do {
        drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + offset, getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                      gRaceTypeSelectFrameTileMaps.exitFrame.rightEdgeTiles[tileOffset], 0, 0x100, 0xA0, 0x49);
        drawMenuSpriteTileClipped(arg0->x + offset, arg0->y + 0x80, getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                      gRaceTypeSelectFrameTileMaps.exitFrame.bottomEdgeTiles[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + 0x80, getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                  gRaceTypeSelectFrameTileMaps.exitFrameCornerTile, 0, 0x100, 0xA0, 0x49);
}
// clang-format on

void updateRaceTypeSelectOption3Frame(RaceTypeSelectExitFrameActor *arg0) {
    int state;

    if ((gRaceTypeSelection == 3) && (arg0->y != -0x48) && ((s32)arg0->state < 6)) {
        state = arg0->state = 2;
    } else {
        state = (s32)arg0->state;
        if ((gRaceTypeSelection != 3) && (arg0->y != -0x140) && (state < 6)) {
            state = arg0->state = 1;
        } else {
            state = arg0->state;
            if (state < 4) {
                state = arg0->state = 3;
            }
        }
    }

    switch (state) {
        case 0:
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
                arg0->state = 6;
                arg0->bounceStep = 8;
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
        case 6:
            arg0->y -= arg0->bounceStep;
            state = arg0->state = 7;
            break;
        case 7:
            arg0->y += arg0->bounceStep;
            arg0->bounceStep /= 2;
            if (arg0->bounceStep == 0) {
                arg0->state = 3;
            } else {
                arg0->state = 6;
            }
            state = arg0->state;
            break;
    }

    if (state == 5) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceTypeSelectOption3Frame, arg0);
}

void initRaceTypeSelectOption3Frame(RaceTypeSelectExitFrameActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->unused1C = 2;
    arg0->unusedTimer = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceTypeSelectOption3Frame);
}

void drawRaceTypeSelectCursor(RaceTypeSelectAnimatedWidgetActor *arg0) {
    drawMenuSpriteWithAlpha(
        arg0->x,
        (s16)(arg0->y + (gRaceTypeSelection * 0x18)),
        getRelocatableHeapBlockBase(ASSET_HANDLE(33)),
        7,
        0x20,
        0x20,
        0,
        arg0->alpha,
        0
    );
}

void updateRaceTypeSelectCursor(RaceTypeSelectAnimatedWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->state;
    if (state != (globalState = gRaceTypeSelectCursorTarget.state)) {
        arg0->state = globalState;
        /* Preserve IDO's state/globalState register allocation. */
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        state = globalState;
        arg0->alpha = gRaceTypeSelectCursorTarget.alpha;
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

    gRaceTypeSelectCursorAnimState = state;
    if (arg0->state == 4) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceTypeSelectCursor, arg0);
}

void initRaceTypeSelectCursor(RaceTypeSelectAnimatedWidgetActor *arg0) {
    arg0->x = -0x7C;
    arg0->y = -0x58;
    arg0->alpha = 0;
    arg0->state = 0;
    arg0->animationTimer = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceTypeSelectCursor);
}

void drawRaceTypeSelectPortrait(RaceTypeSelectAnimatedWidgetActor *arg0) {
    RaceTypeSelectDescription *description = &gRaceTypeSelectPortraitScripts.descriptions[gRaceTypeSelection];

    drawMenuGlyphScript(arg0->x, arg0->y, *description, 1, arg0->alpha, 0);
}

void updateRaceTypeSelectPortrait(RaceTypeSelectAnimatedWidgetActor *arg0) {
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
            arg0->x -= 0x20;
            if (arg0->x < -0xFF) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceTypeSelectPortrait, arg0);
}

void initRaceTypeSelectPortrait(RaceTypeSelectAnimatedWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = 0xC;
    arg0->alpha = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceTypeSelectPortrait);
}

void drawRaceTypeSelectArrowPrompt(RaceTypeSelectAnimatedWidgetActor *arg0) {
    drawMenuSpriteWithAlpha(
        arg0->x,
        arg0->y,
        getRelocatableHeapBlockBase(ASSET_HANDLE(33)),
        2,
        0x20,
        0x20,
        0,
        arg0->alpha,
        0
    );
}

void updateRaceTypeSelectArrowPrompt(RaceTypeSelectAnimatedWidgetActor *arg0) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceTypeSelectArrowPrompt, arg0);
}

void initRaceTypeSelectArrowPrompt(RaceTypeSelectAnimatedWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x5C;
    arg0->alpha = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceTypeSelectArrowPrompt);
}

void drawRaceTypeSelectEntryFee(RaceTypeSelectAnimatedWidgetActor *arg0) {
    char sp40[0x18];

    if (gRaceTypeSelection != 3) {
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
        sprintf(sp40, gRaceTypeSelectEntryFeeFormat, gRacePlayers[0].money);
        drawMenuAsciiText((s16)(arg0->x + 0x10), (s16)(arg0->y + 0x10), sp40, 0, arg0->alpha);
    }
}

void updateRaceTypeSelectEntryFee(RaceTypeSelectAnimatedWidgetActor *arg0) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceTypeSelectEntryFee, arg0);
}

void initRaceTypeSelectEntryFee(RaceTypeSelectAnimatedWidgetActor *arg0) {
    arg0->x = 0x30;
    arg0->y = 0x40;
    arg0->alpha = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceTypeSelectEntryFee);
}
