#include "common.h"
#include "font_encoding.h"
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/menu/controller_pak/controller_pak_continue_prompt_flow.h"
#include "game/menu/controller_pak/controller_pak_menu.h"
#include "game/menu/controller_pak/controller_pak_ui.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/race_setup/race_setup_ui.h"
#include "game/race/player/race_player_input.h"
#include "game/menu/controller_pak/controller_pak_file_delete_flow.h"

#if defined(VERSION_JP)
#else
#define CONTROLLER_PAK_DELETE_PANEL_HANDLE (gAssetHandles[0x29])

#endif
/* This script ends in the overlapping sparkle sentinel immediately below. */
MenuGlyphScript gControllerPakContinuePromptText[8] = {
#if defined(VERSION_JP)
    0xFFFC, 0x0007, 0x00A1, 0x00C8, 0x0098, 0x00A2, 0x0069, 0x002B
#else
    _FRAGMENT("{COLOR:7}CONTIN"),
#endif
};
#if defined(VERSION_JP)
Vec2s gCoursePreviewCloseSparkleOffsetsEnd[4] = { 0x004C, 0x008E, 0x009B, 0x00AE, 0x009C, 0x0095, 0x0030, 0xFFFF };
MenuGlyphScript gControllerPakRumbleCheckPromptText[4][52] = {
    0xFFFC, 0x0006, 0x00E4, 0x00E5, 0x0080, 0x003A, 0x0047, 0xFFFC, 0x0007, 0x008E, 0x00A1, 0x0095, 0x0092, 0x00CB,
    0x0090, 0x0091, 0x00A9, 0x00F3, 0xFFFD, 0x0049, 0x003F, 0x0053, 0x006A, 0x002B, 0x0066, 0x00A5, 0x009A, 0x009B,
    0x0099, 0x008F, 0x00C9, 0x0097, 0x00C6, 0xFFFD, 0x009A, 0x0091, 0x0032, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFC, 0x0006, 0x00E4, 0x00E5,
    0x0080, 0x003A, 0x0047, 0xFFFC, 0x0007, 0x008E, 0x0050, 0x0038, 0x003A, 0x0047, 0x009B, 0x00A2, 0x0091, 0x00AE,
    0xFFFD, 0x009C, 0x0032, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFC, 0x0007, 0x00E9, 0x00EA, 0x00A8, 0x00EB, 0x00EC, 0x00BC,
    0x0090, 0x00B8, 0x00AE, 0x00C9, 0xFFFC, 0x0006, 0x00E4, 0x00E5, 0x0080, 0x003A, 0xFFFD, 0x0047, 0xFFFC, 0x0007,
    0x008E, 0x00A6, 0x0095, 0x00A4, 0x0091, 0x00C9, 0x0097, 0x00C6, 0x009A, 0x0091, 0x0032, 0xFFFF, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0xFFFC, 0x0007, 0xFFFE, 0xFFFE, 0x0099, 0x00B9, 0x00C9, 0x0091, 0x0091, 0x00C9, 0x009C, 0x0095,
    0x0030, 0xFFFF
#else

Vec2s gCoursePreviewCloseSparkleOffsetsEnd[2] = {
    // textconv requires these _() invocations to retain their original line layout.
    // clang-format off
    _("UE?"),
#endif
};
#if defined(VERSION_JP)
MenuGlyphScript gControllerPakRumbleCheckNotUsedText[10] = { 0xFFFC, 0x0007, 0x00A1, 0x0095, 0x0091,
                                                             0x00AE, 0x009D, 0x008F, 0xFFFF };
MenuGlyphScript gControllerPakRumbleCheckNoEntryText[12] = { 0xFFFC, 0x0007, 0x0017, 0x0018, 0xFFFE, 0x000E,
                                                             0x0017, 0x001D, 0x001B, 0x0022, 0xFFFF };
MenuGlyphScript gControllerPakRumblePakText[10] = { 0xFFFC, 0x0006, 0x00E4, 0x00E5, 0x0080, 0x003A, 0x0047, 0xFFFF };
MenuGlyphScript gControllerPakCouldNotEraseNoteText[48] = {
    0x006F, 0x002B, 0x0060, 0x0058, 0x002B, 0x0053, 0x008E, 0x00DB, 0x009D, 0x00AE, 0x009D, 0x008F,
    0x00C9, 0x009B, 0xFFFD, 0x009F, 0x0032, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0xFFF0, 0x0028, 0x0008, 0x0068, 0x004C, 0x004F, 0x002B, 0x0053, 0xFFFF, 0x0000,
    0x0000, 0x0000, 0x0053, 0x0069, 0x002B, 0x0055, 0x003F, 0x006E, 0xFFFF
#else

MenuGlyphScript gControllerPakRumbleCheckPromptText[4][0x34] = {
    {
        _("{COLOR:7}TO USE {COLOR:6}RUMBLE\nPAK{COLOR:7}, INSERT\nIT NOW."),
    },
    {
        _("{COLOR:7}CHECKING\n{COLOR:6}RUMBLE PAK{COLOR:7}."),
    },
    {
        _("{COLOR:7}DO NOT REMOVE\n{COLOR:6}RUMBLE PAK{COLOR:7}."),
    },
    {
        _("{COLOR:7}IS THIS OK?"),
    },
};

MenuGlyphScript gControllerPakRumbleCheckNotUsedText[12] = {
    _("{COLOR:7}NOT USED"),
};

MenuGlyphScript gControllerPakRumbleCheckNoEntryText[12] = {
    _("{COLOR:7}NO ENTRY"),
};

MenuGlyphScript gControllerPakRumblePakText[20] = {
    _("{COLOR:6}RUMBLE PAK"),
};

MenuGlyphScript gControllerPakAreYouSureText[16] = {
    _("{COLOR:7}ARE YOU SURE?"),
};

MenuGlyphScript gControllerPakCouldNotEraseNoteText[56] = {
    _("COULD NOT\nERASE THE\nNOTE."),
    // clang-format on
    0,
    0,
    0,
    0,
    0,
    0,
    0x0000,
    0x0000,
    0xFFF0,
    0x0028,
    0x0008,
    0x0068,
    0x004C,
    0x004F,
    0x002B,
    0x0053,
    0xFFFF,
    0x0000,
    0x0000,
    0x0000,
    0x0053,
    0x0069,
    0x002B,
    0x0055,
    0x003F,
    0x006E,
    0xFFFF,
    0x0000,
    0x0000,
    0x0000,
#endif
};

void drawControllerPakContinuePrompt(ControllerPakOptionsActor *arg0) {
    s32 i;
    s32 j;
    u16 alpha;

    drawMenuSprite(
        (s16)(arg0->common.x - 4),
        (s16)(arg0->common.y - 4),
        getRelocatableHeapBlockBase(gAssetHandles[0x29]),
        2,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->common.x + 0xD4),
        (s16)(arg0->common.y - 4),
        getRelocatableHeapBlockBase(gAssetHandles[0x29]),
        4,
        0x20,
        0x20,
        0,
        0
    );

    i = 0;
    do {
        drawMenuSprite(
            (s16)(arg0->common.x + i),
            (s16)(arg0->common.y - 4),
            getRelocatableHeapBlockBase(gAssetHandles[0x29]),
            3,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)(arg0->common.x + i),
            (s16)(arg0->common.y + 0x24),
            getRelocatableHeapBlockBase(gAssetHandles[0x29]),
            8,
            0x20,
            0x20,
            0,
            0
        );
        i += 0x10;
    } while (i < 0xE0);

    drawMenuSprite(
        (s16)(arg0->common.x - 4),
        (s16)(arg0->common.y + 0x24),
        getRelocatableHeapBlockBase(gAssetHandles[0x29]),
        7,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->common.x + 0xD4),
        (s16)(arg0->common.y + 0x24),
        getRelocatableHeapBlockBase(gAssetHandles[0x29]),
        9,
        0x20,
        0x20,
        0,
        0
    );

    i = 0;
loop_outer:
    drawMenuSprite(
        (s16)(arg0->common.x - 4),
        (s16)(arg0->common.y + i),
        getRelocatableHeapBlockBase(gAssetHandles[0x29]),
        5,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->common.x + 0xD4),
        (s16)(arg0->common.y + i),
        getRelocatableHeapBlockBase(gAssetHandles[0x29]),
        6,
        0x20,
        0x20,
        0,
        0
    );
    j = 0;
    do {
        drawMenuSprite(
            (s16)(arg0->common.x + j),
            (s16)(arg0->common.y + i),
            getRelocatableHeapBlockBase(gAssetHandles[0x29]),
            0xB,
            0x20,
            0x20,
            0,
            0
        );
        j += 0x10;
    } while (j != 0xE0);
    i += 0x10;
    if (i != 0x30) {
        // IDO register allocation nudge for matching codegen.
        if (1) {}
        goto loop_outer;
    }

#if defined(VERSION_JP)
    drawMenuGlyphScript((s16)(arg0->common.x + 8), arg0->common.y, gControllerPakContinuePromptText, 0, 0x100, 0);
#else
    drawMenuGlyphScript((s16)(arg0->common.x + 0x30), arg0->common.y, gControllerPakContinuePromptText, 0, 0x100, 0);
#endif

    alpha = (arg0->selectedOption == 0) ? 0x100 : 0x60;

    drawMenuSpriteWithAlpha(
        (s16)(arg0->common.x + 0x4C),
        (s16)(arg0->common.y + 0x10),
        getRelocatableHeapBlockBase(gAssetHandles[0x24]),
        0x17,
        0x20,
        0x20,
        0,
        alpha,
        0
    );

    if (alpha == 0x100) {
        alpha = 0x60;
    } else {
        alpha = 0x100;
    }

    drawMenuSpriteWithAlpha(
        (s16)(arg0->common.x + 0x4C),
        (s16)(arg0->common.y + 0x20),
        getRelocatableHeapBlockBase(gAssetHandles[0x24]),
        0x18,
        0x20,
        0x20,
        0,
        alpha,
        0
    );
    drawMenuSpriteWithAlpha(
        (s16)(arg0->common.x + 0x4C),
        (s16)(arg0->common.y + (arg0->selectedOption * 0x10) + 0x10),
        getRelocatableHeapBlockBase(gAssetHandles[0x24]),
        0x12,
        0x20,
        0x20,
        0,
        arg0->scale,
        0
    );
}

void updateControllerPakContinuePrompt(ControllerPakOptionsActor *arg0) {
    s32 temp_v0;
    s32 state;
    ControllerPakOptionsActor *temp_a2 = arg0;

    state = arg0->blinkState;
    temp_v0 = gControllerPakContinuePromptTransition.state;
    if (state != temp_v0) {
        arg0->blinkState = temp_v0;
        state = temp_v0 & 0xFF;
        arg0->scale = gControllerPakContinuePromptTransition.x;
    }

    temp_v0 = state;
    switch (temp_v0 ^ 0) {
        case 1:
            temp_a2->selectedOption = gCurrentGameTask->callbackData1;
            if (temp_a2->timer < 0x10) {
                temp_a2->scale -= 9;
            } else {
                temp_a2->scale += 9;
            }
            temp_a2->timer = (temp_a2->timer + 1) & 0x1F;
            state = temp_a2->blinkState;
            break;
        case 0:
        case 2:
            break;
    }

    if (state == 3) {
        removeCallbackTask(temp_a2);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawControllerPakContinuePrompt, (void *)temp_a2);
}

void initControllerPakContinuePrompt(ControllerPakOptionsActor *arg0) {
    arg0->common.x = -0x70;
    arg0->common.y = -0x28;
    arg0->selectedOption = 0;
    arg0->blinkState = 0;
    arg0->scale = 0x100;
    arg0->timer = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakContinuePrompt);
}

void drawControllerPakRumbleCheckPrompt(ControllerPakRumbleCheckPromptActor *actor) {
    s32 i;
    s32 j;
    s32 playerIndex;
    s32 yAdjust;
    MenuGlyphScript *message;
    u16 alpha;
    MenuGlyphScript playerNumberText[5];

    if (actor->state == CONTROLLER_PAK_RUMBLE_DEVICE_SUMMARY) {
        yAdjust = 8;
    } else {
        yAdjust = 0;
    }

    drawMenuSpriteWithAlpha(
        (s16)(actor->common.x - 4),
        (s16)(actor->common.y - yAdjust - 4),
        getRelocatableHeapBlockBase(gAssetHandles[0x29]),
        2,
        0x20,
        0x20,
        0,
        actor->scale,
        0
    );
    drawMenuSpriteWithAlpha(
        (s16)(actor->common.x + 0xD4),
        (s16)(actor->common.y - yAdjust - 4),
        getRelocatableHeapBlockBase(gAssetHandles[0x29]),
        4,
        0x20,
        0x20,
        0,
        actor->scale,
        0
    );

    for (i = 0; i < 0xE0; i += 0x10) {
        drawMenuSpriteWithAlpha(
            (s16)(actor->common.x + i),
            (s16)(actor->common.y - yAdjust - 4),
            getRelocatableHeapBlockBase(gAssetHandles[0x29]),
            3,
            0x20,
            0x20,
            0,
            actor->scale,
            0
        );
        drawMenuSpriteWithAlpha(
            (s16)(actor->common.x + i),
            (s16)(actor->common.y + yAdjust + 0x24),
            getRelocatableHeapBlockBase(gAssetHandles[0x29]),
            8,
            0x20,
            0x20,
            0,
            actor->scale,
            0
        );
    }

    drawMenuSpriteWithAlpha(
        (s16)(actor->common.x - 4),
        (s16)(actor->common.y + yAdjust + 0x24),
        getRelocatableHeapBlockBase(gAssetHandles[0x29]),
        7,
        0x20,
        0x20,
        0,
        actor->scale,
        0
    );
    drawMenuSpriteWithAlpha(
        (s16)(actor->common.x + 0xD4),
        (s16)(actor->common.y + yAdjust + 0x24),
        getRelocatableHeapBlockBase(gAssetHandles[0x29]),
        9,
        0x20,
        0x20,
        0,
        actor->scale,
        0
    );

    for (i = 0; i < 0x30; i += 0x10) {
        drawMenuSpriteWithAlpha(
            (s16)(actor->common.x - 4),
            (s16)(actor->common.y + i),
            getRelocatableHeapBlockBase(gAssetHandles[0x29]),
            5,
            0x20,
            0x20,
            0,
            actor->scale,
            0
        );
        drawMenuSpriteWithAlpha(
            (s16)(actor->common.x + 0xD4),
            (s16)(actor->common.y + i),
            getRelocatableHeapBlockBase(gAssetHandles[0x29]),
            6,
            0x20,
            0x20,
            0,
            actor->scale,
            0
        );
        for (j = 0; j < 0xE0; j += 0x10) {
            drawMenuSpriteWithAlpha(
                (s16)(actor->common.x + j),
                (s16)(actor->common.y + i),
                getRelocatableHeapBlockBase(gAssetHandles[0x29]),
                0xB,
                0x20,
                0x20,
                0,
                actor->scale,
                0
            );
            if (((!actor) && (!actor)) && (!actor)) {}
        }
    }

    if (actor->state == CONTROLLER_PAK_RUMBLE_DEVICE_SUMMARY) {
        for (playerIndex = 0; playerIndex < 4; playerIndex++) {
            playerNumberText[0] = -4;
            playerNumberText[1] = 7;
            playerNumberText[2] = playerIndex + 1;
            playerNumberText[3] = 0x19;
            playerNumberText[4] = 0xFFFF;
            if (playerIndex < gPlayerCount) {
                if (gRumblePakConnectedByController[playerIndex] == 1) {
                    message = gControllerPakRumblePakText;
                } else {
                    message = gControllerPakRumbleCheckNotUsedText;
                }
                alpha = (u16)actor->scale;
            } else {
                message = gControllerPakRumbleCheckNoEntryText;
                alpha = 0x60;
            }
            drawMenuGlyphScript(
                (s16)(actor->common.x + 0x10),
                (s16)(actor->common.y - yAdjust + playerIndex * 0x10),
                playerNumberText,
                0,
                alpha,
                0
            );
            drawMenuGlyphScript(
                (s16)(actor->common.x + 0x40),
                (s16)(actor->common.y - yAdjust + playerIndex * 0x10),
                message,
                0,
                alpha,
                0
            );
        }
    } else {
        drawMenuGlyphScript(
            actor->common.x,
            actor->common.y,
            gControllerPakRumbleCheckPromptText[actor->messageIndex],
            0,
            actor->scale,
            0
        );
    }

    if (actor->state == CONTROLLER_PAK_RUMBLE_CONFIRM_DEVICES) {
        alpha = (gControllerPakRumbleCheckPromptTransition.confirmSelection == 0) ? 0x100 : 0x60;
        drawMenuSpriteWithAlpha(
            (s16)(actor->common.x + 0x4C),
            (s16)(actor->common.y + 0x10),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            0x17,
            0x20,
            0x20,
            0,
            alpha,
            0
        );
        if (alpha == 0x100) {
            alpha = 0x60;
        } else {
            alpha = 0x100;
        }
        drawMenuSpriteWithAlpha(
            (s16)(actor->common.x + 0x4C),
            (s16)(actor->common.y + 0x20),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            0x18,
            0x20,
            0x20,
            0,
            alpha,
            0
        );
        drawMenuSpriteWithAlpha(
            (s16)(actor->common.x + 0x4C),
            (s16)(actor->common.y + (gControllerPakRumbleCheckPromptTransition.confirmSelection * 0x10) + 0x10),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            0x12,
            0x20,
            0x20,
            0,
            actor->optionScale,
            0
        );
    }

    if ((actor->state == CONTROLLER_PAK_RUMBLE_INSERT_ACK) || (actor->state == CONTROLLER_PAK_RUMBLE_WARNING_ACK) ||
        (actor->state == CONTROLLER_PAK_RUMBLE_DEVICE_SUMMARY)) {
        drawMenuSprite(
            (s16)(actor->common.x + 0xD0),
            (s16)(actor->common.y + yAdjust + 0x20),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            (actor->timer >= 8) + 5,
            0x20,
            0x20,
            0,
            0
        );
    }
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void updateControllerPakRumbleCheckPrompt(ControllerPakRumbleCheckPromptActor *arg0) {
    u8 state;
    u8 globalState; ControllerPakRumbleCheckPromptState *prompt = &gControllerPakRumbleCheckPromptTransition.prompt;

    state = arg0->state;
    if (state != (globalState = gControllerPakRumbleCheckPromptTransition.state)) {
        arg0->state = globalState;
        if (1) {}
        {}
        if (1) {}
        if (1) {}
        if (1) {}
        arg0->messageIndex = gControllerPakRumbleCheckPromptTransition.messageIndex;
        arg0->timer = 0;
        arg0->optionScale = 0x100; state = globalState;
    }

    switch (state) {
        case CONTROLLER_PAK_RUMBLE_OPEN:
            arg0->scale += 0x28;
            if (arg0->scale >= 0x100) {
                arg0->scale = 0x100;
                if (gControllerPakRumbleCheckPromptTransition.selectedOption == 1) {
                    arg0->state = CONTROLLER_PAK_RUMBLE_WARNING_ACK;
                } else {
                    arg0->state = CONTROLLER_PAK_RUMBLE_INSERT_ACK;
                }
            }
            state = arg0->state;
            break;
        case CONTROLLER_PAK_RUMBLE_INSERT_ACK:
            state = arg0->state;
            arg0->timer = (arg0->timer + 1) & 0xF;
            break;
        case CONTROLLER_PAK_RUMBLE_CHECK_MOTORS:
            state = arg0->state;
            arg0->timer = 0;
            break;
        case CONTROLLER_PAK_RUMBLE_WARNING_ACK:
            state = arg0->state;
            arg0->timer = (arg0->timer + 1) & 0xF;
            break;
        case CONTROLLER_PAK_RUMBLE_FADE_OUT:
            arg0->scale -= 0x28;
            if (arg0->scale <= 0) {
                arg0->scale = 0;
                arg0->state = CONTROLLER_PAK_RUMBLE_DISMISSED;
            }
            state = arg0->state;
            break;
        case CONTROLLER_PAK_RUMBLE_CHECK_HOLD:
            state = arg0->state;
            arg0->timer = 0;
            break;
        case CONTROLLER_PAK_RUMBLE_DEVICE_SUMMARY:
            state = arg0->state;
            arg0->timer = (arg0->timer + 1) & 0xF;
            break;
        case CONTROLLER_PAK_RUMBLE_CONFIRM_DEVICES:
            if ((s32)arg0->timer < 0x10) {
                arg0->optionScale -= 9;
            } else {
                arg0->optionScale += 9;
            }
            state = arg0->state;
            arg0->timer = (arg0->timer + 1) & 0x1F;
            break;
        case CONTROLLER_PAK_RUMBLE_DISMISSED:
        case CONTROLLER_PAK_RUMBLE_INITIAL_PROBE:
            break;
    }

    prompt->state = state;
    if (arg0->state == CONTROLLER_PAK_RUMBLE_DISMISSED) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawControllerPakRumbleCheckPrompt, (void *)arg0);
}
// clang-format on

void initControllerPakRumbleCheckPrompt(ControllerPakRumbleCheckPromptActor *arg0) {
    arg0->common.x = -0x70;
    arg0->common.y = -0x1C;
    arg0->state = CONTROLLER_PAK_RUMBLE_INITIAL_PROBE;
    arg0->scale = 0;
    arg0->timer = 0;
    arg0->messageIndex = CONTROLLER_PAK_RUMBLE_MESSAGE_INSERT;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakRumbleCheckPrompt);
}

void drawControllerPakFileDeleteMainOptions(ControllerPakOptionsActor *arg0) {
    u32 drawAlpha;
    u16 alpha;

    if (gControllerPakMenuState.mainChoice == 0) {
        alpha = 0x100;
    } else {
        alpha = 0x80;
    }

    drawAlpha = alpha;
    drawMenuSpriteWithAlpha(
        arg0->common.x,
        arg0->common.y,
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        1,
        0x20,
        0x20,
        0,
        drawAlpha,
        0
    );

    if (drawAlpha == 0x100) {
        alpha = 0x80;
    } else {
        alpha = 0x100;
    }

    drawMenuSpriteWithAlpha(
        (s16)(arg0->common.x + 0x70),
        arg0->common.y,
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        2,
        0x20,
        0x20,
        0,
        alpha,
        0
    );
    drawMenuSpriteWithAlpha(
        (s16)(arg0->common.x + (gControllerPakMenuState.mainChoice * 0x70)),
        arg0->common.y,
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        5,
        0x20,
        0x20,
        0,
        arg0->scale,
        0
    );
}

void updateControllerPakFileDeleteMainOptionsUi(ControllerPakOptionsActor *arg0) {
    switch (gControllerPakMenuState.state) {
        case 0:
            if (arg0->timer < 0x10) {
                arg0->scale -= 8;
            } else {
                arg0->scale += 8;
            }
            arg0->timer = (arg0->timer + 1) & 0x1F;
            break;
        case 1:
        case 2:
            arg0->scale = 0x100;
            arg0->timer = 0;
            break;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawControllerPakFileDeleteMainOptions, (void *)arg0);
}

void initControllerPakFileDeleteMainOptions(ControllerPakOptionsActor *arg0) {
    arg0->common.x = -0x68;
    arg0->common.y = -0x44;
    arg0->scale = 0x100;
    arg0->timer = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakFileDeleteMainOptionsUi);
}

void drawControllerPakFileDeleteConfirmOptions(ControllerPakOptionsActor *arg0) {
    ControllerPakOptionsActor *actor;
    u16 alpha;
    u16 otherAlpha;

    actor = arg0;

    if (gControllerPakMenuState.state == 2) {
        otherAlpha = 0x80;
        alpha = 0x80;
        if (gControllerPakMenuState.confirmChoice == 0) {
            alpha = 0x100;
        } else {
            otherAlpha = 0x100;
        }
    } else {
        alpha = 0x80;
        otherAlpha = 0x80;
        if (!gControllerPakMenuState.confirmChoice) {}
    }

    drawMenuSpriteWithAlpha(
        arg0->common.x,
        arg0->common.y,
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        3,
        0x20,
        0x20,
        0,
        alpha,
        0
    );
    drawMenuSpriteWithAlpha(
        (s16)(actor->common.x + 0x70),
        actor->common.y,
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        4,
        0x20,
        0x20,
        0,
        otherAlpha,
        0
    );

    if (gControllerPakMenuState.state == 2) {
        drawMenuSpriteWithAlpha(
            (s16)(actor->common.x + (gControllerPakMenuState.confirmChoice * 0x70)),
            actor->common.y,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            5,
            0x20,
            0x20,
            0,
            actor->scale,
            0
        );
    }
}

void updateControllerPakFileDeleteConfirmOptionsUi(ControllerPakOptionsActor *arg0) {
    switch (gControllerPakMenuState.state) {
        case 0:
        case 1:
            break;
        case 2:
            if (arg0->timer < 0x10) {
                arg0->scale -= 8;
            } else {
                arg0->scale += 8;
            }
            arg0->timer = (arg0->timer + 1) & 0x1F;
            break;
    }
    addRenderCallback(
        &gMenuRenderCallbackList,
        (RenderCallback)drawControllerPakFileDeleteConfirmOptions,
        (void *)arg0
    );
}

void initControllerPakFileDeleteConfirmOptions(ControllerPakOptionsActor *arg0) {
    arg0->common.x = -0x68;
    arg0->common.y = 0x48;
    arg0->scale = 0x100;
    arg0->timer = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakFileDeleteConfirmOptionsUi);
}

void drawControllerPakFileDeleteFreeSpaceInfo(ControllerPakTwoPointActor *arg0) {
    char padBefore[4];
    u16 text[4];
    char padAfter[8];
    s32 value;
    s32 i;

    // IDO register allocation nudge.
    if (1) {
        i = 0;
        do {
            text[i] = 0xFFFE;
            i++;
        } while (i < 4);

        value = gControllerPakFreeFileCount;
        i = 0;
        do {
            text[1 - i] = value % 10;
            value = value / 10;
            i++;
        } while (value != 0);
        text[2] = 0xFFFF;
        drawMenuGlyphScript(arg0->common.x, arg0->common.y, text, 1, 0x100, 8);
        i = 0;
    }
    do {
        text[i] = 0xFFFE;
        i++;
    } while (i < 4);

    value = gControllerPakFreeBytes / 0x100;
    i = 0;
    do {
        text[2 - i] = value % 10;
        value = value / 10;
        i++;
    } while (value != 0);
    text[3] = 0xFFFF;
    drawMenuGlyphScript(arg0->x2, arg0->y2, text, 1, 0x100, 8);
}

void updateControllerPakFileDeleteFreeSpaceInfo(ControllerPakTwoPointActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawControllerPakFileDeleteFreeSpaceInfo, arg0);
}

void initControllerPakFileDeleteFreeSpaceInfo(ControllerPakTwoPointActor *arg0) {
    s32 temp_v0 = 0x34;

    arg0->common.x = -0x24;
    arg0->common.y = temp_v0;
#if defined(VERSION_JP)
    arg0->x2 = 0x50;
#else
    arg0->x2 = 0x5C;
#endif
    arg0->y2 = temp_v0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakFileDeleteFreeSpaceInfo);
}

void drawControllerPakFileDeleteFileList(ControllerPakFileListActor *arg0) {
    s32 i;
    s32 digits;
    u8 insertIndex;
    u32 var_a3;
    s32 temp;
    u16 alpha; // s5
    u16 fileIndex;
    s32 state;
    s32 j;
    u16 textB0[4];
    u16 fileNameText[19];
#if defined(VERSION_US)
    u16 text7C[5];
#endif
    s32 var_a0;

    if (gControllerPakMenuState.visibleFileIndex == 0) {
        fileIndex = gControllerPakMenuState.fileIndex;
    } else if (gControllerPakMenuState.visibleFileIndex == 4) {
        fileIndex = gControllerPakMenuState.fileIndex - 4;
    } else {
        fileIndex = gControllerPakMenuState.fileIndex - gControllerPakMenuState.visibleFileIndex;
    }

    for (j = 0; j < 5; j++) {
        if (gControllerPakMenuState.state <= 0) {
            alpha = 0xE0;
        } else if (fileIndex == gControllerPakMenuState.fileIndex) {
            alpha = 0x100;
        } else {
            alpha = 0xE0;
        }

        if (fileIndex < 9) {
            textB0[0] = ~1;
            state = -4;
        } else {
            textB0[0] = 1;
            state = 0;
        }
        textB0[1] = (fileIndex + 1) % 10;
        textB0[2] = 0xFFFF;
        drawMenuGlyphScript(arg0->positions[0].x + state, arg0->positions[0].y + j * 0x10, textB0, 1, alpha, 8);

#if defined(VERSION_JP)
        for (;;) {
#endif
        if (gControllerPakFileStates[fileIndex].company_code != 0) {
            var_a3 = 0;
            for (i = 0; i < 0x10; i++) {
                insertIndex = gControllerPakFileStates[fileIndex].game_name[i];
                if (insertIndex == 0 && var_a3 == 0) {
                    var_a3 = i;
                }
                if (insertIndex < 0x10) {
                    fileNameText[i] = 0xFFFE;
                } else {
                    fileNameText[i] = insertIndex - 0x10;
                }
            }

            fileNameText[16] = 0xFFFE;
            fileNameText[17] = 0xFFFE;
#if defined(VERSION_JP)
                if (gControllerPakFileStates[fileIndex].ext_name[0] != 0) {
#else
            insertIndex = gControllerPakFileStates[fileIndex].ext_name[0];
            if (insertIndex != 0) {
#endif
                if (var_a3 == 0) {
                    var_a3 = 0x10;
                }
                fileNameText[var_a3] = 0x2C;
#if defined(VERSION_JP)
                    fileNameText[var_a3 + 1] = gControllerPakFileStates[fileIndex].ext_name[0] - 0x10;
#else
                fileNameText[var_a3 + 1] = insertIndex - 0x10;
#endif
            }
            fileNameText[18] = 0xFFFF;
            drawMenuGlyphScript(arg0->positions[1].x, arg0->positions[1].y + j * 0x10, fileNameText, 1, alpha, 8);
#if defined(VERSION_US)

            for (i = 0; i < 4; i++) {
                insertIndex = (gControllerPakFileStates[fileIndex].game_code >> ((3 - i) * 8)) & 0xFF & 0xFF;
                if ((insertIndex < 0x30) || (insertIndex >= 0x5B)) {
                    text7C[i] = 0xFFFE;
                } else if (insertIndex >= 0x41) {
                    text7C[i] = insertIndex - 0x37;
                } else if (insertIndex >= 0x3A) {
                    text7C[i] = 0xFFFE;
                } else {
                    text7C[i] = insertIndex - 0x30;
                }
            }
            text7C[4] = 0xFFFF;
            drawMenuGlyphScript(arg0->positions[2].x, arg0->positions[2].y + j * 0x10, text7C, 1, alpha, 8);

            for (i = 0; i < 2; i++) {
                insertIndex = (gControllerPakFileStates[fileIndex].company_code >> ((1 - i) * 8)) & 0xFF & 0xFF;
                if ((insertIndex < 0x30) || (insertIndex >= 0x5B)) {
                    text7C[i] = 0xFFFE;
                } else if (insertIndex >= 0x41) {
                    text7C[i] = insertIndex - 0x37;
                } else if (insertIndex >= 0x3A) {
                    text7C[i] = 0xFFFE;
                } else {
                    text7C[i] = insertIndex - 0x30;
                }
            }
            text7C[2] = 0xFFFF;
            drawMenuGlyphScript(arg0->positions[3].x, arg0->positions[3].y + j * 0x10, text7C, 1, alpha, 8);
#endif

            for (i = 0; i < 3; i++) {
                textB0[i] = 0xFFFE;
            }
            digits = gControllerPakFileStates[fileIndex].file_size >> 8;
            i = 0;
            var_a0 = 0;
            while (1) {
                textB0[2 - var_a0] = digits % 10;
                digits = digits / 10;
                var_a0++;
                if (digits == 0) {
                    break;
                }
            }
            textB0[3] = 0xFFFF;
#if defined(VERSION_JP)
                drawMenuGlyphScript(arg0->positions[2].x, arg0->positions[2].y + j * 0x10, textB0, 1, alpha, 8);
            }
            break;
#else
            drawMenuGlyphScript(arg0->positions[4].x, arg0->positions[4].y + j * 0x10, textB0, 1, alpha, 8);
#endif
        }
        fileIndex++;
    }

    if (gControllerPakMenuState.state) {
#if defined(VERSION_JP)
        drawMenuSpriteWithAlphaWideArgs(
            arg0->positions[3].x,
            (s16)(arg0->positions[3].y + (gControllerPakMenuState.visibleFileIndex * 0x10)),
#else
        drawMenuSpriteWithAlpha(
            arg0->positions[5].x,
            arg0->positions[5].y + (gControllerPakMenuState.visibleFileIndex * 0x10),
#endif
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
#if defined(VERSION_US)
            6,
            0x20,
            0x20,
#endif
            0,
#if defined(VERSION_US)
            arg0->cursorScale,
            0
        );
        drawMenuSpriteWithAlpha(
            arg0->positions[5].x + 0x80,
            arg0->positions[5].y + (gControllerPakMenuState.visibleFileIndex * 0x10),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            7,
#endif
            0x20,
            0x20,
            0,
            arg0->cursorScale,
            0
        );
    }
}

void updateControllerPakFileDeleteFileListUi(ControllerPakFileListActor *arg0) {
    if (gControllerPakMenuState.state == 1) {
        if (arg0->cursorTimer < 0x10) {
            arg0->cursorScale -= 9;
        } else {
            arg0->cursorScale += 9;
        }
        arg0->cursorTimer = (arg0->cursorTimer + 1) & 0x1F;
    } else {
        arg0->cursorScale = 0x100;
        arg0->cursorTimer = 0;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawControllerPakFileDeleteFileList, (void *)arg0);
}

void initControllerPakFileDeleteFileList(ControllerPakFileListActor *arg0) {
    s32 temp_v0 = -0x20;
#if defined(VERSION_JP)
    s32 temp_v1 = -0x6C;
#else
    s32 temp_v1 = -0x84;
#endif

    arg0->positions[0].x = temp_v1;
    arg0->positions[0].y = temp_v0;
#if defined(VERSION_JP)
    arg0->positions[1].x = -0x50;
#else
    arg0->positions[1].x = -0x6A;
#endif
    arg0->positions[1].y = temp_v0;
#if defined(VERSION_JP)
    arg0->positions[2].x = 0x58;
#else
    arg0->positions[2].x = 0x2C;
#endif
    arg0->positions[2].y = temp_v0;
#if defined(VERSION_JP)
    arg0->positions[3].x = temp_v1;
    arg0->positions[3].y = -0x1F;
#else
    arg0->positions[3].x = 0x54;
    arg0->positions[3].y = temp_v0;
    arg0->positions[4].x = 0x69;
    arg0->positions[4].y = temp_v0;
    arg0->positions[5].x = temp_v1;
    arg0->positions[5].y = -0x1F;
#endif
    arg0->cursorScale = 0x100;
    arg0->cursorTimer = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakFileDeleteFileListUi);
}

void drawControllerPakFileDeleteIcon(ControllerPakSpriteActor *arg0) {
    drawMenuTilemapSprite(&arg0->sprite.render, MENU_TILEMAP_TEXEL_4B, arg0->common.x, arg0->common.y);
}

void updateControllerPakFileDeleteIcon(ControllerPakSpriteActor *arg0) {
    addRenderCallback(&gMenuOverlayRenderCallbackList, (RenderCallback)drawControllerPakFileDeleteIcon, arg0);
}

void initControllerPakFileDeleteIcon(ControllerPakSpriteActor *arg0) {
    initMenuTilemapSprite(&arg0->sprite, getRelocatableHeapBlockBase(gAssetHandles[0x26]));
    arg0->sprite.render.scrollX = 0x10;
    arg0->sprite.render.scrollY = 0x10;
    arg0->common.x = arg0->sprite.render.tilemapWidth;
    arg0->common.y = arg0->sprite.render.tilemapHeight;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakFileDeleteIcon);
}

void drawControllerPakMessageIcon(ControllerPakSpriteActor *arg0) {
    drawMenuTilemapSprite(&arg0->sprite.render, MENU_TILEMAP_TEXEL_8B, arg0->common.x, arg0->common.y);
}

void updateControllerPakMessageIcon(ControllerPakSpriteActor *arg0) {
    addRenderCallback(&gMenuOverlayRenderCallbackList, (RenderCallback)drawControllerPakMessageIcon, arg0);
}

void initControllerPakMessageIcon(ControllerPakSpriteActor *arg0) {
    initMenuTilemapSprite(&arg0->sprite, getRelocatableHeapBlockBase(gAssetHandles[0x26]));
    arg0->sprite.render.scrollX = 0x10;
    arg0->sprite.render.scrollY = 0x20;
    arg0->common.x = arg0->sprite.render.tilemapWidth;
    arg0->common.y = arg0->sprite.render.tilemapHeight;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakMessageIcon);
}

void drawControllerPakFileDeleteErrorPrompt(ControllerPakWindowActor *arg0) {
    void *texture;

    drawMenuSpriteWithAlpha(
        arg0->common.x,
        arg0->common.y,
        getRelocatableHeapBlockBase(gAssetHandles[0x24]),
        0,
        0x20,
        0x20,
        0,
        arg0->scale,
        0
    );
    drawMenuSpriteWithAlpha(
        (s16)(arg0->common.x + 0x40),
        arg0->common.y,
        getRelocatableHeapBlockBase(gAssetHandles[0x24]),
        1,
        0x20,
        0x20,
        0,
        arg0->scale,
        0
    );
    drawMenuSpriteWithAlpha(
        (s16)(arg0->common.x + 0x78),
        arg0->common.y,
        getRelocatableHeapBlockBase(gAssetHandles[0x24]),
        1,
        0x20,
        0x20,
        0,
        arg0->scale,
        0
    );
    drawMenuSpriteWithAlpha(
        (s16)(arg0->common.x + 0xB0),
        arg0->common.y,
        getRelocatableHeapBlockBase(gAssetHandles[0x24]),
        2,
        0x20,
        0x20,
        0,
        arg0->scale,
        0
    );
    drawMenuGlyphScript(
        (s16)(arg0->common.x + 4),
        (s16)(arg0->common.y + 4),
        gControllerPakCouldNotEraseNoteText,
        0,
        arg0->scale,
        0
    );
    if (arg0->selectedOption == 1) {
        texture = getRelocatableHeapBlockBase(gAssetHandles[0x24]);
        drawMenuSprite(
            (s16)(arg0->common.x + 0xD4),
            (s16)(arg0->common.y + 0x24),
            texture,
            (arg0->timer >= 8) + 5,
            0x20,
            0x20,
            0,
            0
        );
    }
}

void updateControllerPakFileDeleteErrorPromptUi(ControllerPakDeletePromptActor *arg0) {
    u8 globalState;
    u8 state;
    u8 *statePtr;
#if defined(VERSION_US)
    ControllerPakMenuState *menuState = &gControllerPakMenuState;
#endif

/* JP uses a separate load relocation; merging it with the final struct
     * store changes IDO address reuse and switch scheduling. */
#if defined(VERSION_JP)
    globalState = gControllerPakDeletePromptState;
#else
    globalState = gControllerPakMenuState.unk6;
#endif
    state = arg0->timer;
    if (globalState != state) {
        state = globalState;
        arg0->timer = globalState;
    }

    statePtr = &arg0->timer;
    // IDO register allocation nudge for matching codegen.
    switch (state ^ 0) {
        case 0:
            arg0->scale = (s16)arg0->scale + 0x28;
            if ((s16)arg0->scale >= 0x100) {
                arg0->scale = 0x100;
                arg0->timer = 1;
            }
            state = arg0->timer;
            break;
        case 1:
            arg0->selectedOption = (arg0->selectedOption + 1) & 0xF;
            state = arg0->timer;
            break;
        case 2:
            arg0->scale = (s16)arg0->scale - 0x28;
            if ((s16)arg0->scale <= 0) {
                arg0->scale = 0;
                arg0->timer = 3;
            }
            state = arg0->timer;
            break;
        case 3:
            break;
    }

#if defined(VERSION_JP)
    gControllerPakMenuState.unk6 = state;
#else
    menuState->unk6 = state;
#endif
    if (*statePtr == 3) {
        removeCallbackTask(arg0);
        return;
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawControllerPakFileDeleteErrorPrompt, (void *)arg0);
}

void initControllerPakFileDeleteErrorPrompt(ControllerPakDeletePromptActor *arg0) {
    arg0->common.x = -0x70;
    arg0->common.y = -0x1C;
    arg0->timer = 0;
    arg0->scale = 0;
    arg0->selectedOption = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakFileDeleteErrorPromptUi);
}
#if defined(VERSION_US)

void drawControllerPakDeleteConfirmPrompt(ControllerPakDeletePromptActor *arg0) {
    s32 i;
    s32 j;
    s32 alpha;
    s32 limit;

    drawMenuSprite(
        arg0->common.x,
        arg0->common.y,
        getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE),
        2,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->common.x + 0x90),
        arg0->common.y,
        getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE),
        4,
        0x20,
        0x20,
        0,
        0
    );

    i = 0;
    do {
        drawMenuSprite(
            (s16)(arg0->common.x + i + 0x10),
            arg0->common.y,
            getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE),
            3,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)(arg0->common.x + i + 0x10),
            (s16)(arg0->common.y + 0x30),
            getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE),
            8,
            0x20,
            0x20,
            0,
            0
        );
        i += 0x10;
    } while (i < 0x80);

    drawMenuSprite(
        arg0->common.x,
        (s16)(arg0->common.y + 0x30),
        getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE),
        7,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->common.x + 0x90),
        (s16)(arg0->common.y + 0x30),
        getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE),
        9,
        0x20,
        0x20,
        0,
        0
    );

    i = 0;
    limit = 0x80;
loop_outer:
    drawMenuSprite(
        arg0->common.x,
        (s16)(arg0->common.y + i + 0x10),
        getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE),
        5,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->common.x + 0x90),
        (s16)(arg0->common.y + i + 0x10),
        getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE),
        6,
        0x20,
        0x20,
        0,
        0
    );

    j = 0;
    do {
        drawMenuSprite(
            (s16)(arg0->common.x + j + 0x10),
            (s16)(arg0->common.y + i + 0x10),
            getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE),
            0xB,
            0x20,
            0x20,
            0,
            0
        );
        j += 0x10;
    } while (j != limit);

    i += 0x10;
    if (i != 0x20) {
        if (1) {}
        goto loop_outer;
    }

    drawMenuGlyphScript(
        (s16)(arg0->common.x + 0x1C),
        (s16)(arg0->common.y + 4),
        gControllerPakAreYouSureText,
        1,
        0x100,
        0
    );

    alpha = (arg0->selectedOption == 0) ? 0x100 : 0x60;

    drawMenuSpriteWithAlpha(
        (s16)(arg0->common.x + 0x2C),
        (s16)(arg0->common.y + 0x18),
        getRelocatableHeapBlockBase(gAssetHandles[0x24]),
        0x17,
        0x20,
        0x20,
        0,
        alpha,
        0
    );

    if (alpha == 0x100) {
        alpha = 0x60;
    } else {
        alpha = 0x100;
    }

    drawMenuSpriteWithAlpha(
        (s16)(arg0->common.x + 0x2C),
        (s16)(arg0->common.y + 0x28),
        getRelocatableHeapBlockBase(gAssetHandles[0x24]),
        0x18,
        0x20,
        0x20,
        0,
        alpha,
        0
    );
    drawMenuSpriteWithAlpha(
        (s16)(arg0->common.x + 0x2C),
        (s16)(arg0->common.y + (arg0->selectedOption * 0x10) + 0x18),
        getRelocatableHeapBlockBase(gAssetHandles[0x24]),
        0x12,
        0x20,
        0x20,
        0,
        arg0->scale,
        0
    );
}

void updateControllerPakDeleteConfirmPrompt(ControllerPakDeletePromptActor *arg0) {
    arg0->selectedOption = gControllerPakMenuState.confirmChoice;
    if (arg0->timer < 0x10) {
        arg0->scale -= 9;
    } else {
        arg0->scale += 9;
    }
    arg0->timer = (arg0->timer + 1) & 0x1F;
    if (gControllerPakMenuState.state != 3) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawControllerPakDeleteConfirmPrompt, (void *)arg0);
    }
}

void initControllerPakDeleteConfirmPrompt(ControllerPakDeletePromptActor *arg0) {
    arg0->common.x = -0x54;
    arg0->common.y = -0x20;
    arg0->scale = 0x100;
    arg0->selectedOption = 1;
    arg0->timer = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakDeleteConfirmPrompt);
}
#endif
