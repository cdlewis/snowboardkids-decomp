#include "common.h"
#include <PR/os_libc.h>
#include "font_encoding.h"
#include "game/save_data.h"
#include "game/audio/sound_manager.h"
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/menu/race_setup/race_setup_menu.h"
#include "game/menu/race_setup/race_setup_ui.h"
#include "game/race/player/race_player_input.h"

#define TITLE_MENU_SECONDARY_TEXTURE_HANDLE (gAssetHandles[31])
#define TITLE_MENU_FRAME_TEXTURE_HANDLE (gAssetHandles[33])
#define TITLE_MENU_BANNER_TEXTURE_HANDLE (gAssetHandles[41])
#define RACE_PLAYER_STATE_SIZE 0x60C
#define CONTROLLER_PAK_STATUS_UNUSED 0x13
#define SAVE_PANEL_MAX_RECORD_ICONS 3
#define SAVE_PANEL_BASE_BADGE_COUNT 6
#define SAVE_PANEL_EMPTY_BADGE_TILE 9
#define SAVE_PANEL_RECORD_ICON_TILE 0x19
#define SAVE_PANEL_FIRST_BADGE_TILE 0x1A
#define SAVE_PANEL_RECORD_ICON_SPACING 0x10
#define SAVE_PANEL_BADGE_SPACING 0xE
#define SAVE_PANEL_LOCKED_ICON_ALPHA 0x70
#define SAVE_PANEL_UNLOCKED_ICON_ALPHA 0x100
#define SAVE_STATUS_FADE_STEP 0x26
#define SAVE_STATUS_TRANSITION_NONE 0
#define SAVE_STATUS_TRANSITION_FADE_IN 1
#define SAVE_STATUS_TRANSITION_FADE_OUT 2
#define SAVE_STATUS_TRANSITION_DONE 3

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ u8 pad1A[0x20 - 0x1A];
    /* 0x20 */ s16 y;
} RaceSetupSavePanelPlayerCoordinates;

typedef struct {
    u8 noControllerPakMessage[0x1A];
    u8 saveChoicePromptBottomSprites[1];
    u8 saveChoicePromptTopSprites[0x41];
} RaceSetupSavePromptData;

#define gRaceSetupNoControllerPakMessage (gRaceSetupSavePromptData.noControllerPakMessage)
#define gRaceSetupSaveChoicePromptBottomSprites (gRaceSetupSavePromptData.saveChoicePromptBottomSprites)
#define gRaceSetupSaveChoicePromptTopSprites (gRaceSetupSavePromptData.saveChoicePromptTopSprites)

MenuGlyphScript gRaceSetupPlayerCountPromptText[5][0x34] = {
    {
        // textconv requires these _() invocations to retain their original line layout.
        // clang-format off
        _("{COLOR:7}PLEASE SELECT\nTHE NUMBER OF\nPLAYERS."),
    },
    {
        _("{COLOR:6}3 DIFFERENT\nMODES{COLOR:7} OF PLAY\nAVAILABLE."),
    },
    {
        _("{COLOR:6}BATTLE MODE\n{COLOR:7}ONLY IN MULTI\nPLAYER MODE."),
    },
    {
        _("{COLOR:7}CHECKING\nCONTROLLER\nPAK."),
    },
    {
        _("{COLOR:7}CHECK\nCOMPLETE."),
    },
};
MenuGlyphScript gRaceSetupOnePlayerOptionText[10] = {
    _("PLAYER"),
};
MenuGlyphScript gRaceSetupTwoPlayerOptionText[10] = {
    _("PLAYERS"),
};
MenuGlyphScript gRaceSetupThreePlayerOptionText[10] = {
    _("PLAYERS"),
};
MenuGlyphScript gRaceSetupFourPlayerOptionText[10] = {
    _("PLAYERS"),
};
MenuGlyphScript gRaceSetupSaveStatusMessages[0x13][0x26] = {
    {
        _("{COLOR:7}CHECKING."),
    },
    {
        _("{COLOR:7}SEARCHING FOR\nGAME NOTE."),
    },
    {
        _("{COLOR:7}ACCESSING."),
    },
    {
        _("{COLOR:7}FIXING."),
    },
    {
        _("{COLOR:7}INSERT AND\nPRESS BUTTON."),
    },
    {
        _("{COLOR:7}FINISHED\nSELECTING."),
    },
    {
        0,
    },
    {
        _("{COLOR:7}ERROR.  DATA\nMAY BE ERASED."),
    },
    {
        0,
    },
    {
        _("{COLOR:7}NO DATA.\nSTART GAME?"),
    },
    {
        _("{COLOR:7}NO CONTROLLER\nPAK.  START?"),
    },
    {
        _("NOT ENOUGH\nPAGES."),
    },
    {
        _("{COLOR:7}NO OPEN\nNOTES."),
    },
    {
        _("{COLOR:7}DATA ERROR.\nSTART GAME?"),
    },
    {
        _("COULD NOT\nREPAIR PAK."),
    },
    {
        _("{COLOR:7}CONTROLLER\nPAK FIXED."),
    },
    {
        _("{COLOR:7}RUMBLE PAK\nDETECTED."),
    },
    {
        _("{COLOR:7}INSERT\nANOTHER PAK."),
    },
    {
        _("{COLOR:7}FINISHED\nSELECTING."),
    },
};
MenuGlyphScript gRaceSetupUnusedStatusMessage[12] = {
    _("-NO ENTRY-"),
    // clang-format on
};
RaceSetupSavePromptData gRaceSetupSavePromptData = {
    { 0x4E, 0x4F, 0x20, 0x43, 0x4F, 0x4E, 0x54, 0x52, 0x4F, 0x4C, 0x4C, 0x45, 0x52,
      0x0A, 0x50, 0x41, 0x4B, 0x20, 0x46, 0x4F, 0x55, 0x4E, 0x44, 0x2E, 0x0A, 0x43 },
    { 0x41 },
    { 0x4E, 0x4E, 0x4F, 0x54, 0x20, 0x53, 0x41, 0x56, 0x45, 0x2E, 0x00, 0x00, 0x00, 0x15, 0x16, 0x10, 0x11,
      0x13, 0x15, 0x13, 0x14, 0x13, 0x15, 0x13, 0x15, 0x17, 0x18, 0x17, 0x18, 0x17, 0x18, 0x13, 0x14, 0x13,
      0x14, 0x00, 0x00, 0x15, 0x16, 0x0E, 0x0F, 0x15, 0x16, 0x0F, 0x14, 0x0F, 0x15, 0x0F, 0x15, 0x17, 0x18,
      0x17, 0x18, 0x17, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};
const char D_800E0A80[] = "%d";
const char D_800E0A84[] = "%6d";
extern TitleMenuWidgetActor *D_8010ADE0;
extern s32 gMenuFlowState;
void drawRaceSetupPlayerCountPrompt(MenuIntroActor *arg0) {
    s32 i;
    s32 j;
    s32 selected;
    s32 limit;
    MenuIntroActor *actor;

    actor = arg0;
    drawMenuSpriteWithAlpha(
        (s16)(actor->x - 4),
        (s16)(actor->y - 4),
        getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE),
        2,
        0x20,
        0x20,
        0,
        actor->alpha,
        0
    );
    drawMenuSpriteWithAlpha(
        (s16)(actor->x + 0xD4),
        (s16)(actor->y - 4),
        getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE),
        4,
        0x20,
        0x20,
        0,
        actor->alpha,
        0
    );
    i = 0;
    do {
        drawMenuSpriteWithAlpha(
            (s16)(actor->x + i),
            (s16)(actor->y - 4),
            getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE),
            3,
            0x20,
            0x20,
            0,
            actor->alpha,
            0
        );
        drawMenuSpriteWithAlpha(
            (s16)(actor->x + i),
            (s16)(actor->y + 0x24),
            getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE),
            8,
            0x20,
            0x20,
            0,
            actor->alpha,
            0
        );
        i += 0x10;
    } while (i < 0xE0);
    drawMenuSpriteWithAlpha(
        (s16)(actor->x - 4),
        (s16)(actor->y + 0x24),
        getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE),
        7,
        0x20,
        0x20,
        0,
        actor->alpha,
        0
    );
    drawMenuSpriteWithAlpha(
        (s16)(actor->x + 0xD4),
        (s16)(actor->y + 0x24),
        getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE),
        9,
        0x20,
        0x20,
        0,
        actor->alpha,
        0
    );
    i = (actor->state == 4) * 0;
    limit = 0xE0;
    do {
        drawMenuSpriteWithAlpha(
            (s16)(actor->x - 4),
            (s16)(actor->y + i),
            getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE),
            5,
            0x20,
            0x20,
            0,
            actor->alpha,
            0
        );
        drawMenuSpriteWithAlpha(
            (s16)(actor->x + 0xD4),
            (s16)(actor->y + i),
            getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE),
            6,
            0x20,
            0x20,
            0,
            actor->alpha,
            0
        );
        j = 0;
        do {
            drawMenuSpriteWithAlpha(
                (s16)(actor->x + j),
                (s16)(actor->y + i),
                getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE),
                0xB,
                0x20,
                0x20,
                0,
                actor->alpha,
                0
            );
            j += 0x10;
        } while (j != limit);
        i += 0x10;
    } while (i <= 0x2F);

    if (actor->state < 3) {
        selected = 0;
    } else {
        selected = actor->state - 2;
        if (actor->state < 5) {
            if (gPlayerCount == 1) {
                selected = 1;
            } else {
                selected = 2;
            }
        } else if (selected >= 5) {
            selected = 4;
        }
    }
    drawMenuGlyphScript(actor->x, actor->y, gRaceSetupPlayerCountPromptText[selected], 0, actor->alpha, 0);

    if ((actor->state == 1) || (actor->state == 6)) {
        drawMenuSprite(
            (s16)(actor->x + 0xD0),
            (s16)(actor->y + 0x20),
            getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
            (actor->timer >= 8) + 5,
            0x20,
            0x20,
            0,
            0
        );
    }
}

void updateRaceSetupPlayerCountPrompt(MenuIntroActor *arg0) {
    RaceSetupMenuSubState *global;
    MenuIntroActor *actor;
    s32 globalState;
    s16 alpha;
    u8 state;
    s32 step;
    u32 stateCopy;

    global = &gRaceSetupMenuSubState;
    state = arg0->state;
    globalState = global->state;
    actor = arg0;
    stateCopy = state;
    if ((u32)stateCopy != globalState) {
        arg0->state = globalState;
        state = globalState;
        arg0->alpha = gRaceSetupMenuSubState.alpha;
    }
    step = 0x10;

    alpha = actor->alpha;
    if ((u32)alpha != 0x100) {
        if (state == 0) {
            actor->alpha = alpha + 0x20;
            alpha = actor->alpha;
            if ((u32)alpha == 0x100) {
                alpha = ((volatile MenuIntroActor *)actor)->alpha;
                actor->state = 1;
            }
        } else {
            actor->alpha = alpha - 0x30;
            alpha = actor->alpha;
            if (alpha <= 0) {
                actor->alpha = 0;
                alpha = actor->alpha;
            }
        }
    } else {
        switch (state) {
            case 1:
            case 6:
                alpha = ((volatile MenuIntroActor *)actor)->alpha;
                actor->timer = (actor->timer + 1) & 0xF;
                break;
            case 2:
                actor->y -= 0x10;
                if (actor->y == -0x5C) {
                    actor->child = createCallbackTask((CallbackTaskCallback)initRaceSetupOnePlayerOption, 0, 0x63);
                    enqueueSoundEffect(1, 0x32);
                    actor->state = 3;
                }
                alpha = actor->alpha;
                break;
            case 3:
                if (gCurrentGameTask->callbackData1 == 2) {
                    alpha = ((volatile MenuIntroActor *)actor)->alpha;
                    actor->state = 4;
                }
                break;
            case 4:
                actor->y += step;
                if (actor->y == -0x1C) {
                    actor->state = 5;
                }
                alpha = actor->alpha;
                break;
            case 5:
            case 7:
            case 8:
                break;
        }
    }

    if (alpha == 0) {
        actor->state = 8;
    }
    gRaceSetupMenuSubState.state = actor->state;
    gRaceSetupMenuSubState.alpha = actor->alpha;
    if (actor->state != 8) {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSetupPlayerCountPrompt, (void *)actor);
    }
}

void initRaceSetupPlayerCountPrompt(void *arg0) {
    MenuIntroActor *actor = arg0;

    actor->x = -0x70;
    actor->y = -0x1C;
    actor->alpha = 0;
    actor->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSetupPlayerCountPrompt);
}

void drawRaceSetupOnePlayerOption(void *arg0) {
    MenuItemActor *actor = arg0;
    s16 unused;
    s16 width;
    s16 xOffset;
    s16 yOffset;
    u16 alpha;

    if (actor->state == 0) {
        alpha = 0x100;
        width = 0x18;
        xOffset = 0x10;
        yOffset = 0;
    } else if (gPlayerCount == 1) {
        alpha = 0x100;
        width = 0x20;
        xOffset = 0xC;
        yOffset = 8;
    } else {
        alpha = 0x60;
        width = 0x10;
        xOffset = 0x14;
        yOffset = -4;
    }

    if ((gMenuSelectionConfirmTimer == 0) || (gMenuSelectionConfirmTimer & 1) || (gPlayerCount != 1)) {
        drawMenuSpriteWithAlpha(
            actor->x,
            actor->y,
            getRelocatableHeapBlockBase(gAssetHandles[33]),
            3,
            0x20,
            0x20,
            0,
            alpha,
            0
        );
        if (width == 0x20) {
            drawMenuSpriteWithAlpha(
                (s16)(actor->x + xOffset),
                (s16)(actor->y - yOffset),
                getRelocatableHeapBlockBase(gAssetHandles[31]),
                0,
                width,
                width,
                0,
                alpha,
                0
            );
        } else {
            drawMenuSpriteWithPaletteScale(
                (s16)(actor->x + xOffset),
                (s16)(actor->y - yOffset),
                getRelocatableHeapBlockBase(gAssetHandles[31]),
                0,
                alpha
            );
        }
        drawMenuGlyphScriptDefaultFont(
            (s16)(actor->x + 0x30),
            (s16)(actor->y + 4),
            gRaceSetupOnePlayerOptionText,
            1,
            alpha
        );
    }

    drawMenuSpriteWithAlpha(
        (s16)(actor->x + 0x80),
        actor->y,
        getRelocatableHeapBlockBase(gAssetHandles[33]),
        7,
        0x20,
        0x20,
        0,
        alpha,
        0
    );
}

void updateRaceSetupOnePlayerOption(MenuItemActor *arg0) {
    MenuItemActor *child;

    switch (arg0->state) {
        case 0:
            arg0->x -= 0x20;
            if (arg0->x == 0x70) {
                arg0->child = createCallbackTask((CallbackTaskCallback)initRaceSetupTwoPlayerOption, 0, 0x63);
                enqueueSoundEffect(1, 0x32);
            }
            if (arg0->x == -0x50) {
                arg0->state = 1;
            }
            break;
        case 1:
            if (gMenuSelectionConfirmTimer == 0x13) {
                arg0->state = 2;
                if (gPlayerCount == 1) {
                    gMenuFlowState = 1;
                }
            }
            break;
        case 2:
            arg0->x -= 0x20;
            if (arg0->x == -0x70) {
                child = arg0->child;
                child->state = 2;
                if (gPlayerCount == 2) {
                    gMenuFlowState = 1;
                }
            }
            break;
        default:
            break;
    }
    if (arg0->x < -0x108) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSetupOnePlayerOption, (void *)arg0);
}

void initRaceSetupOnePlayerOption(MenuItemActor *arg0) {
    MenuItemActor *actor = arg0;

    actor->x = 0x90;
    actor->y = -0x18;
    actor->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSetupOnePlayerOption);
}

void drawRaceSetupTwoPlayerOption(MenuItemActor *arg0) {
    MenuItemActor *actor = arg0;
    s16 unused;
    s16 width;
    s16 xOffset;
    s16 yOffset;
    u16 alpha;
    s32 i;

    if (actor->state == 0) {
        alpha = 0x100;
        width = 0x18;
        xOffset = 0x10;
        yOffset = 0;
    } else if (gPlayerCount == 2) {
        alpha = 0x100;
        width = 0x20;
        xOffset = 0xC;
        yOffset = 8;
    } else {
        alpha = 0x60;
        width = 0x10;
        xOffset = 0x14;
        yOffset = -4;
    }

    if ((gMenuSelectionConfirmTimer == 0) || (gMenuSelectionConfirmTimer & 1) || (gPlayerCount != 2)) {
        drawMenuSpriteWithAlpha(
            actor->x,
            actor->y,
            getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
            3,
            0x20,
            0x20,
            0,
            alpha,
            2
        );
        if (width == 0x20) {
            drawMenuSpriteWithAlpha(
                (s16)(actor->x + xOffset),
                (s16)(actor->y - yOffset),
                getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE),
                1,
                width,
                width,
                0,
                alpha,
                0
            );
        } else {
            drawMenuSpriteWithPaletteScale(
                (s16)(actor->x + xOffset),
                (s16)(actor->y - yOffset),
                getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE),
                1,
                alpha
            );
        }
        drawMenuGlyphScriptDefaultFont(
            (s16)(actor->x + 0x30),
            (s16)(actor->y + 4),
            gRaceSetupTwoPlayerOptionText,
            1,
            alpha
        );
    }

    for (i = 0; i != 0x28; i += 0x14) {
        drawMenuSpriteWithAlpha(
            (s16)(actor->x + i + 0x80),
            actor->y,
            getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
            7,
            0x20,
            0x20,
            0,
            alpha,
            0
        );
    }
}

void updateRaceSetupTwoPlayerOption(MenuItemActor *arg0) {
    MenuItemActor *child;

    switch (arg0->state) {
        case 0:
            arg0->x -= 0x20;
            if (arg0->x == 0x70) {
                arg0->child = createCallbackTask((CallbackTaskCallback)initRaceSetupThreePlayerOption, 0, 0x63);
                enqueueSoundEffect(1, 0x32);
            }
            if (arg0->x == -0x50) {
                arg0->state = 1;
            }
            break;
        case 1:
            break;
        case 2:
            arg0->x -= 0x20;
            if (arg0->x == -0x70) {
                child = arg0->child;
                child->state = 2;
                if (gPlayerCount == 3) {
                    gMenuFlowState = 1;
                }
            }
            break;
        default:
            break;
    }
    if (arg0->x < -0x108) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSetupTwoPlayerOption, (void *)arg0);
}

void initRaceSetupTwoPlayerOption(MenuItemActor *arg0) {
    MenuItemActor *actor = arg0;

    actor->x = 0x90;
    actor->y = 8;
    actor->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSetupTwoPlayerOption);
}

void drawRaceSetupThreePlayerOption(void *arg0) {
    MenuItemActor *actor = arg0;
    s16 unused;
    s16 width;
    s16 xOffset;
    s16 yOffset;
    u16 alpha;
    s32 i;

    if (actor->state == 0) {
        alpha = 0x100;
        width = 0x18;
        xOffset = 0x10;
        yOffset = 0;
    } else if (gPlayerCount == 3) {
        alpha = 0x100;
        width = 0x20;
        xOffset = 0xC;
        yOffset = 8;
    } else {
        alpha = 0x60;
        width = 0x10;
        xOffset = 0x14;
        yOffset = -4;
    }

    if ((gMenuSelectionConfirmTimer == 0) || (gMenuSelectionConfirmTimer & 1) || (gPlayerCount != 3)) {
        drawMenuSpriteWithAlpha(
            actor->x,
            actor->y,
            getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
            3,
            0x20,
            0x20,
            0,
            alpha,
            3
        );
        if (width == 0x20) {
            drawMenuSpriteWithAlpha(
                (s16)(actor->x + xOffset),
                (s16)(actor->y - yOffset),
                getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE),
                2,
                width,
                width,
                0,
                alpha,
                0
            );
        } else {
            drawMenuSpriteWithPaletteScale(
                (s16)(actor->x + xOffset),
                (s16)(actor->y - yOffset),
                getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE),
                2,
                alpha
            );
        }
        drawMenuGlyphScriptDefaultFont(
            (s16)(actor->x + 0x30),
            (s16)(actor->y + 4),
            gRaceSetupThreePlayerOptionText,
            1,
            alpha
        );
    }

    for (i = 0; i != 0x3C; i += 0x14) {
        drawMenuSpriteWithAlpha(
            (s16)(actor->x + i + 0x80),
            actor->y,
            getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
            7,
            0x20,
            0x20,
            0,
            alpha,
            0
        );
    }
}

void updateRaceSetupThreePlayerOption(MenuItemActor *arg0) {
    MenuItemActor *child;

    switch (arg0->state) {
        case 0:
            arg0->x -= 0x20;
            if (arg0->x == 0x70) {
                arg0->child = createCallbackTask((CallbackTaskCallback)initRaceSetupFourPlayerOption, 0, 0x63);
                enqueueSoundEffect(1, 0x32);
            }
            if (arg0->x == -0x50) {
                arg0->state = 1;
            }
            break;
        case 1:
            break;
        case 2:
            arg0->x -= 0x20;
            if (arg0->x == -0x70) {
                child = arg0->child;
                child->state = 2;
                if (gPlayerCount == 4) {
                    gMenuFlowState = 1;
                }
            }
            break;
        default:
            break;
    }
    if (arg0->x < -0x108) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSetupThreePlayerOption, (void *)arg0);
}

void initRaceSetupThreePlayerOption(MenuItemActor *arg0) {
    MenuItemActor *actor = arg0;

    actor->x = 0x90;
    actor->y = 0x28;
    actor->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSetupThreePlayerOption);
}

void drawRaceSetupFourPlayerOption(void *arg0) {
    MenuItemActor *actor = arg0;
    s16 unused;
    s16 width;
    s16 xOffset;
    s16 yOffset;
    u16 alpha;
    s32 i;

    if (actor->state == 0) {
        alpha = 0x100;
        width = 0x18;
        xOffset = 0x10;
        yOffset = 0;
    } else if (gPlayerCount == 4) {
        alpha = 0x100;
        width = 0x20;
        xOffset = 0xC;
        yOffset = 8;
    } else {
        alpha = 0x60;
        width = 0x10;
        xOffset = 0x14;
        yOffset = -4;
    }

    if ((gMenuSelectionConfirmTimer == 0) || (gMenuSelectionConfirmTimer & 1) || (gPlayerCount != 4)) {
        drawMenuSpriteWithAlpha(
            actor->x,
            actor->y,
            getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
            3,
            0x20,
            0x20,
            0,
            alpha,
            4
        );
        if (width == 0x20) {
            drawMenuSpriteWithAlpha(
                (s16)(actor->x + xOffset),
                (s16)(actor->y - yOffset),
                getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE),
                3,
                width,
                width,
                0,
                alpha,
                0
            );
        } else {
            drawMenuSpriteWithPaletteScale(
                (s16)(actor->x + xOffset),
                (s16)(actor->y - yOffset),
                getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE),
                3,
                alpha
            );
        }
        drawMenuGlyphScriptDefaultFont(
            (s16)(actor->x + 0x30),
            (s16)(actor->y + 4),
            gRaceSetupFourPlayerOptionText,
            1,
            alpha
        );
    }

    for (i = 0; i != 0x50; i += 0x14) {
        drawMenuSpriteWithAlpha(
            (s16)(actor->x + i + 0x80),
            actor->y,
            getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
            7,
            0x20,
            0x20,
            0,
            alpha,
            0
        );
    }
}

void updateRaceSetupFourPlayerOption(MenuItemActor *arg0) {
    MenuItemActor *actor = arg0;
    s16 x;

    switch (actor->state) {
        case 0:
            actor->x -= 0x20;
            x = actor->x;
            if (x == -0x50) {
                actor->state = 1;
                createCallbackTask((CallbackTaskCallback)initRaceSetupPlayerCountCursor, 0, 0x63);
                x = actor->x;
            }
            break;
        case 1:
            x = actor->x;
            break;
        case 2:
            actor->x -= 0x20;
            x = actor->x;
            break;
        default:
            x = actor->x;
            break;
    }
    if (x < -0x108) {
        removeCallbackTask(actor);
        gCurrentGameTask->callbackData1 = 2;
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSetupFourPlayerOption, (void *)actor);
}

void initRaceSetupFourPlayerOption(MenuItemActor *arg0) {
    MenuItemActor *actor = arg0;

    actor->x = 0x90;
    actor->y = 0x48;
    actor->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSetupFourPlayerOption);
}

void drawRaceSetupPlayerCountCursor(void *arg0) {
    FadeItemActor *actor = arg0;

    drawMenuSpriteWithAlpha(
        actor->x,
        (s16)((actor->y + (gPlayerCount << 5)) - 0x20),
        getRelocatableHeapBlockBase(gAssetHandles[33]),
        4,
        0x20,
        0x20,
        0,
        actor->uAlpha,
        0
    );
}

void updateRaceSetupPlayerCountCursor(FadeItemActor *arg0) {
    FadeItemActor *actor = arg0;
    u16 temp_v0;
    s16 temp_t6;
    FadeItemActor *temp_a2;

    temp_v0 = actor->alpha;
    temp_a2 = actor;
    if ((((((((((temp_v0 & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) &
         0xFFFFu) != 0x100) {
        temp_t6 = temp_v0 + 0x28;
        actor->alpha = temp_t6;
        if ((temp_t6 & 0xFFFF) >= 0x100) {
            actor->alpha = 0x100;
            gCurrentGameTask->callbackData1 = 1;
        }
    } else if (gMenuFlowState != 0) {
        temp_a2->x = temp_a2->x - 0x20;
    }
    if (temp_a2->x < -0x90) {
        removeCallbackTask(temp_a2);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSetupPlayerCountCursor, (void *)temp_a2);
    }
}

void initRaceSetupPlayerCountCursor(FadeItemActor *arg0) {
    FadeItemActor *actor = arg0;

    actor->x = -0x68;
    actor->y = -0x14;
    actor->alpha = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSetupPlayerCountCursor);
}

void drawRaceSetupSavePlayerPanels(TitleMenuWidgetItemView *actor) {
    s32 i;
    u16 alpha;
    register TitleMenuWidgetItemView *panel;
    s32 playerNumber;
    volatile s32 unusedStackPadding;
    char text[8];

    for (i = 0; i < 4; i++) {
        if ((gPlayerCount - 1) < i) {
            alpha = 0x50;
        } else {
            alpha = 0x100;
        }

        panel = actor;
        playerNumber = i + 1;
        drawMenuSpriteWithAlpha(
            actor->x[i],
            actor->y[i],
            getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
            0xA,
            0x20,
            0x20,
            0,
            alpha,
            playerNumber
        );
        drawMenuSpriteWithAlpha(
            (s16)(actor->x[i] + 0x40),
            actor->y[i],
            getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
            0xB,
            0x20,
            0x20,
            0,
            alpha,
            playerNumber
        );
        drawMenuSpriteWithAlpha(
            (s16)(panel->x[i] + 0x80),
            panel->y[i],
            getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
            0xC,
            0x20,
            0x20,
            0,
            alpha,
            playerNumber
        );

        sprintf(text, D_800E0A80, playerNumber);
        drawMenuAsciiText((s16)(actor->x[i] + 0x32), (s16)(panel->y[i] + 2), text, 0, alpha);

        if (alpha == 0x100) {
            sprintf(text, D_800E0A84, gRacePlayers[i].money);
            drawMenuAsciiText((s16)(actor->x[i] + 0x44), (s16)(panel->y[i] + 0x1B), text, 0, alpha);
        } else {
            if ((!panel) && (!panel)) {}
            drawMenuSpriteWithAlpha(
                (s16)(actor->x[i] + 2),
                (s16)(panel->y[i] + 0x14),
                getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE),
                0x90,
                0x20,
                0x20,
                0,
                0xF0,
                0
            );
        }
    }
}

void updateRaceSetupSavePanelFrame(RectListActor *arg0) {
    u8 state;
    s32 outer;
    s32 i;
    s16 delta0;
    s16 delta1;

    outer = gRaceSetupSavePanelFrameState;
    state = arg0->frame;
    if (gRaceSetupSavePanelFrameState != state) {
        state = outer;
        arg0->frame = outer;
    }

    switch (state) {
        case 0:
            outer = 0;
            do {
                i = 0;
                do {
                    if (i < 2) {
                        delta0 = 1;
                    } else {
                        delta0 = -1;
                    }
                    if (i & 1) {
                        delta1 = -1;
                    } else {
                        delta1 = 1;
                    }

                    arg0->rectCoords[i] += delta0;

                    if (i == 0) {
                        arg0->stepAccumulator += arg0->stepIncrement;
                    }
                    if (arg0->stepAccumulator >= arg0->stepLimit) {
                        arg0->rectCoords[i + 4] += delta1;
                        if (i == 3) {
                            arg0->stepAccumulator -= arg0->stepLimit;
                        }
                    }

                    if (i <= 0) {
                        delta0 = 1;
                    } else {
                        delta0 = -1;
                    }
                    if ((i + 1) & 1) {
                        delta1 = -1;
                    } else {
                        delta1 = 1;
                    }

                    arg0->rectCoords[i + 1] += delta0;

                    if (i == -1) {
                        arg0->stepAccumulator += arg0->stepIncrement;
                    }
                    if (arg0->stepAccumulator >= arg0->stepLimit) {
                        arg0->rectCoords[i + 5] += delta1;
                        if (i == 2) {
                            arg0->stepAccumulator -= arg0->stepLimit;
                        }
                    }

                    i += 2;
                } while (i != 4);

                outer++;
                if (arg0->rects[0].x0 == -0x88) {
                    arg0->frame = 1;
                    break;
                }
            } while (outer != 0x10);
            state = arg0->frame;
            break;
        case 2:
            arg0->rects[1].y0 += 0x10;
            arg0->rects[1].y1 += 0x10;
            if (arg0->rects[1].y0 == 0x24) {
                arg0->frame = 3;
            }
            state = arg0->frame;
            break;
        case 1:
        case 3:
        default:
            break;
    }

    gRaceSetupMenuSubState.forceUpdate = state;
    for (i = 0; i < 4; i++) {
        gRaceSetupSavePanelRects[0][i] = arg0->rectCoords[i];
        gRaceSetupSavePanelRects[1][i] = arg0->rectCoords[i + 4];
    }

    if (gMenuFlowState == 0x63) {
        removeCallbackTask(arg0);
        gMenuFlowState = 0;
        return;
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSetupSavePlayerPanels, (void *)arg0);
}

void initRaceSetupSavePanelFrame(RectListActor *arg0) {
    RectListActor *actor = arg0;

    actor->rects[0].x0 = -0x114;
    actor->rects[1].x0 = -0xA4;
    actor->rects[0].y0 = -0x114;
    actor->rects[1].y0 = 0x48;
    actor->rects[0].x1 = 0x90;
    actor->rects[1].x1 = -0xA4;
    actor->rects[0].y1 = 0x90;
    actor->rects[1].y1 = 0x48;
    actor->stepLimit = 0x8C;
    actor->stepIncrement = 0x44;
    actor->frame = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSetupSavePanelFrame);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void drawRaceSetupSavePanelIcons(TitleMenuIconStripActor *actor) {
    volatile s32 unusedStackPadding;
    union {
        s32 value;
        volatile s32 reloaded;
    } playerIndex;
    s32 unusedStackPadding2[6];
    s32 playerCoordinateOffset;
    s16 *controllerPakStatus;
    register s16 *badgeDisplayOrderStart;
    register GameSaveData *saveData;
    register RaceSetupSavePanelPlayerCoordinates *coordinateCursor;
    register s32 xOffset;
    register s32 alpha;
    register s32 nextIconOrBadgeOffset;
    register s16 *badgeOrder;
    register GameSaveData *badgeSaveData;
    s32 iconIndex;
    s32 displayCount;
    s32 paletteIndex;
    u16 tileIndex;

    do {
        playerIndex.value = 0;
        if ((s32)gPlayerCount > 0) {
            controllerPakStatus = gControllerPakStatusCodes;
            playerCoordinateOffset = 0;
            do {
                if (*controllerPakStatus != CONTROLLER_PAK_STATUS_UNUSED) {
                    saveData = &gGameSaveDataBuffer[playerIndex.value];
                    iconIndex = 0;
                    if (saveData->progressionLevel == SAVE_PANEL_MAX_RECORD_ICONS) {
                        displayCount = SAVE_PANEL_MAX_RECORD_ICONS;
                    } else {
                        displayCount = saveData->progressionLevel + 1;
                    }
                    if (displayCount > 0) {
                        xOffset = 0;
                        coordinateCursor = (RaceSetupSavePanelPlayerCoordinates *)&actor->pad0[playerCoordinateOffset];
                        do {
                            nextIconOrBadgeOffset = iconIndex + 1;
                            if ((s32)saveData->progressionLevel < nextIconOrBadgeOffset) {
                                alpha = SAVE_PANEL_LOCKED_ICON_ALPHA;
                            } else {
                                alpha = SAVE_PANEL_UNLOCKED_ICON_ALPHA;
                            }
                            drawMenuSpriteWithAlpha((s16)(coordinateCursor->x + actor->iconOffsetX + xOffset),
                                                    (s16)(coordinateCursor->y + actor->iconOffsetY),
                                                    getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
                                                    SAVE_PANEL_RECORD_ICON_TILE, 0x20, 0x20, 0, alpha,
                                                    SAVE_PANEL_EMPTY_BADGE_TILE - iconIndex);
                            iconIndex = nextIconOrBadgeOffset;
                            xOffset += SAVE_PANEL_RECORD_ICON_SPACING;
                        } while (nextIconOrBadgeOffset != displayCount);
                        iconIndex = 0;
                    }

                    displayCount = SAVE_PANEL_BASE_BADGE_COUNT;
                    if (saveData->progressionLevel == 1) {
                        displayCount = 7;
                    } else if (saveData->progressionLevel == 2) {
                        displayCount = 8;
                    } else if (saveData->progressionLevel == SAVE_PANEL_MAX_RECORD_ICONS) {
                        displayCount = 9;
                    }

                    if (displayCount > 0) {
                        badgeSaveData = &gGameSaveDataBuffer[playerIndex.reloaded]; badgeDisplayOrderStart = gCharacterSelectCourseMenuData.playerBadgeDisplayOrder; badgeOrder = badgeDisplayOrderStart; nextIconOrBadgeOffset = 0;
                        coordinateCursor = (RaceSetupSavePanelPlayerCoordinates *)&actor->pad0[playerCoordinateOffset];
                        do {
                            paletteIndex = badgeSaveData->cupPlacements[*badgeOrder];
                            alpha = SAVE_PANEL_LOCKED_ICON_ALPHA;
                            if (paletteIndex != 0) {
                                alpha = SAVE_PANEL_UNLOCKED_ICON_ALPHA;
                                paletteIndex += SAVE_PANEL_BASE_BADGE_COUNT;
                            } else {
                                paletteIndex = SAVE_PANEL_EMPTY_BADGE_TILE;
                            }
                            tileIndex = iconIndex + SAVE_PANEL_FIRST_BADGE_TILE;
                            drawMenuSpriteWithAlpha((s16)(coordinateCursor->x + actor->badgeOffsetX + nextIconOrBadgeOffset),
                                                    (s16)(coordinateCursor->y + actor->badgeOffsetY),
                                                    getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
                                                    tileIndex,
                                                    0x20, 0x20, 0, alpha, paletteIndex);
                            iconIndex++;
                            nextIconOrBadgeOffset += SAVE_PANEL_BADGE_SPACING;
                            badgeOrder++;
                        } while (iconIndex != displayCount);
                    }
                }
                playerIndex.value++; playerCoordinateOffset += sizeof(s16); controllerPakStatus++;
            } while (playerIndex.value < (s32)gPlayerCount);
        }
    } while (0);
}
// clang-format on

void updateRaceSetupSavePanelIcons(void *arg0) {
    RectListActor *temp_a2;
    RectListActor *actor = arg0;
    s32 i;

    for (i = 0; i < 4; i++) {
        actor->rectCoords[i] = gRaceSetupSavePanelRects[0][i];
        actor->rectCoords[i + 4] = gRaceSetupSavePanelRects[1][i];
    }
    temp_a2 = actor;
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSetupSavePanelIcons, (void *)temp_a2);
}

void initRaceSetupSavePanelIcons(RectListActor *arg0) {
    RectListActor *actor = arg0;

    actor->rects[0].x0 = -0x114;
    actor->rects[1].x0 = -0xA4;
    actor->rects[0].y0 = -0x114;
    actor->rects[1].y0 = 0x48;
    actor->rects[0].x1 = 0x90;
    actor->rects[1].x1 = -0xA4;
    actor->rects[0].y1 = 0x90;
    actor->rects[1].y1 = 0x48;
    actor->stepLimit = 4;
    actor->stepIncrement = 0x11;
    actor->stepAccumulator = 2;
    actor->unk2E = 0x27;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSetupSavePanelIcons);
}

void drawRaceSetupSaveStatusWidgets(TitleMenuWidgetActor *arg0) {
    volatile s32 unused;
    MenuIntroActor *intro;
    register TitleMenuWidgetActor *actor;
    MenuGlyphScript *text;
    u32 tile;
    s32 i;
    s16 state;
    u16 alpha;

    actor = arg0;
    intro = gActiveMenuTask;
    for (i = 0; i < gPlayerCount; i++) {
        state = gControllerPakStatusCodes[i];
        if (gControllerPakStatusCodes[i] != 8) {
            if (gControllerPakStatusCodes[i] == 5) {
                alpha = (u16)actor->alpha[i];
            } else {
                alpha = 0x100;
            }

            tile = 8;
            drawMenuSpriteWithAlpha(
                (s16)(actor->x[i] - 2),
                (s16)(actor->y[i] + 0xC),
                getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
                tile,
                0x20,
                0x20,
                0,
                alpha,
                0
            );
            drawMenuSpriteWithAlpha(
                (s16)(actor->x[i] + 0x3E),
                (s16)(actor->y[i] + 0xC),
                getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
                9,
                0x20,
                0x20,
                0,
                alpha,
                0
            );

            state = gControllerPakStatusCodes[i];
            if (state == 0xA) {
                drawMenuAsciiText(
                    (s16)(actor->x[i] + 2),
                    (s16)(actor->y[i] + 0x10),
                    gRaceSetupNoControllerPakMessage,
                    7,
                    actor->alpha[i]
                );
            } else {
                text = gRaceSetupSaveStatusMessages[state];
                state = 2;
                drawMenuGlyphScript((s16)(actor->x[i] + state), (s16)(actor->y[i] + 0x10), text, 1, actor->alpha[i], 0);
            }

            if ((actor->alpha[i] == 0x100) && (intro->state == 8)) {
                state = gControllerPakStatusCodes[i];
                if ((state == 4) || ((state >= 7) && (state != 0x12) && (gMenuChoicePromptState[i] == 0))) {
                    drawMenuSprite(
                        (s16)(actor->x[i] + 0x70),
                        (s16)(actor->y[i] + 0x20),
                        getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
                        ((s32)actor->frame >= 8) + 5,
                        0x20,
                        0x20,
                        0,
                        0
                    );
                }
            }
        }
    }
}

void updateRaceSetupSaveStatusWidgets(TitleMenuWidgetActor *arg0) {
    s32 transitionStateSum;
    TitleMenuWidgetActor *actor;
    s32 playerIndex;
    TitleMenuWidgetActor *panelPositions;

    panelPositions = D_8010ADE0;
    actor = arg0;
    for (playerIndex = 0; playerIndex < (s32)gPlayerCount; playerIndex++) {
        actor->x[playerIndex] = panelPositions->x[playerIndex];
        actor->y[playerIndex] = panelPositions->y[playerIndex];

        if (gRaceSetupMenuSubState.statusTransitionStates[playerIndex] != actor->statusTransitionStates[playerIndex]) {
            actor->statusTransitionStates[playerIndex] = gRaceSetupMenuSubState.statusTransitionStates[playerIndex];
            actor->nextStatusCodes[playerIndex] = gRaceSetupMenuSubState.nextStatusCodes[playerIndex];
        }

        switch (actor->statusTransitionStates[playerIndex]) {
            case SAVE_STATUS_TRANSITION_FADE_IN:
                actor->alpha[playerIndex] += SAVE_STATUS_FADE_STEP;
                if (actor->alpha[playerIndex] >= 0x100) {
                    actor->alpha[playerIndex] = 0x100;
                    actor->statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_NONE;
                }
                break;
            case SAVE_STATUS_TRANSITION_FADE_OUT:
                actor->alpha[playerIndex] -= SAVE_STATUS_FADE_STEP;
                if (actor->alpha[playerIndex] <= 0) {
                    actor->alpha[playerIndex] = 0;
                    actor->statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_FADE_IN;
                    gControllerPakStatusCodes[playerIndex] = arg0->nextStatusCodes[playerIndex];
                }
                break;
            case SAVE_STATUS_TRANSITION_NONE:
            case SAVE_STATUS_TRANSITION_DONE:
                break;
        }

        gRaceSetupMenuSubState.statusTransitionStates[playerIndex] = actor->statusTransitionStates[playerIndex];
    }

    arg0->frame = (arg0->frame + 1) & 0xF;
    transitionStateSum = 0;
    for (playerIndex = 0; playerIndex < (s32)gPlayerCount; playerIndex++) {
        transitionStateSum += actor->statusTransitionStates[playerIndex];
    }

    if (transitionStateSum == (gPlayerCount * SAVE_STATUS_TRANSITION_DONE)) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSetupSaveStatusWidgets, (void *)arg0);
    }
}

void initRaceSetupSaveStatusWidgets(TitleMenuWidgetActor *arg0) {
    s32 i;
    TitleMenuWidgetActor *new_var;

    arg0->x[0] = -0x114;
    arg0->y[0] = -0xA4;
    arg0->x[1] = -0x114;
    arg0->y[1] = 0x48;
    arg0->x[2] = 0x90;
    new_var = arg0;
    new_var->y[2] = -0xA4;
    new_var->x[3] = 0x90;
    new_var->y[3] = 0x48;

    for (i = 0; i < gPlayerCount; i++) {
        new_var->alpha[i] = 0x100;
        new_var->statusTransitionStates[i] = SAVE_STATUS_TRANSITION_NONE;
    }

    setCallbackTaskCallback(new_var, (CallbackTaskCallback)updateRaceSetupSaveStatusWidgets);
}

void drawRaceSetupSaveChoicePrompts(TitleMenuTransitionActor *arg0) {
    MenuIntroActor *intro;
    TitleMenuTransitionActor *new_var;
    TitleMenuTransitionActor *new_var2;
    s32 new_var3;
    s32 i;
    s32 alpha;
    TitleMenuTransitionActor *new_var4;
    s16 state;
    u16 selection;

    intro = gActiveMenuTask;
    new_var2 = arg0;
    if (intro->state == 8) {
        for (i = 0; i < gPlayerCount; i++) {
            selection = gRaceSetupMenuSubState.pendingStatusCodes[i];
            if (1) {
                state = gMenuChoicePromptState[i];
                if (selection != arg0->selection[i]) {
                    arg0->selection[i] = gRaceSetupMenuSubState.pendingStatusCodes[i];
                }

                state = gMenuChoicePromptState[i];
                if (state != 0) {
                    new_var4 = arg0;
                    if (!(state & 1)) {
                        alpha = 0x100;
                    } else {
                        alpha = 0x60;
                    }

                    drawMenuSpriteWithAlpha(
                        new_var4->x[i],
                        arg0->topY[i],
                        getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
                        gRaceSetupSaveChoicePromptTopSprites[gControllerPakStatusCodes[i] * 2],
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

                    new_var = arg0;
                    drawMenuSpriteWithAlpha(
                        arg0->x[i],
                        new_var->y[i],
                        getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
                        gRaceSetupSaveChoicePromptBottomSprites[gControllerPakStatusCodes[i] * 2],
                        0x20,
                        0x20,
                        0,
                        alpha,
                        0
                    );

                    if (new_var->alpha == 0) {}

                    if (!arg0) {}

                    state = gMenuChoicePromptState[i];
                    if ((state == 3) || (state == 4)) {
                        drawMenuSpriteWithAlpha(
                            new_var2->x[i],
                            (s16)(((gMenuChoicePromptState[i] * 0x10) + new_var->y[i]) - (new_var3 = 0x30)),
                            getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
                            0x12,
                            0x20,
                            0x20,
                            0,
                            new_var->alpha[i],
                            i + 7
                        );
                        state = gMenuChoicePromptState[i];
                    }
                }
            }

            if ((state >= 5) && (new_var2->slideOffset[i] == 0)) {
                gRaceSetupMenuSubState.nextStatusCodes[i] = arg0->selection[i];
                gRaceSetupMenuSubState.statusTransitionStates[i] = 2;
                gMenuChoicePromptState[i] = 0;
            }
        }
    }
}

void updateRaceSetupSaveChoicePrompts(TitleMenuTransitionActor *arg0) {
    TitleMenuTransitionActor *actor;
    s32 i;
    s32 step;
    s32 stepAmount;
    u32 new_var;
    s16 state;

    actor = arg0;
    i = 0;
    if (gPlayerCount > 0) {
        do {
            if (gRaceSetupMenuSubState.pendingStatusCodes[i] != actor->selection[i]) {
                actor->selection[i] = gRaceSetupMenuSubState.pendingStatusCodes[i];
            }

            state = gMenuChoicePromptState[i];
            new_var = 4;
            if ((state != 0) && ((state ^ 0) != 3) && (state != new_var)) {
                if (state < 5) {
                    step = 1;
                } else {
                    step = -1;
                }

                stepAmount = step * 8;
                actor->slideOffset[i] += stepAmount;
                state = actor->slideOffset[i];
                new_var = 0x20;
                if (state == new_var) {
                    gMenuChoicePromptState[i] += 2;
                    actor->alphaTimer[i] = 0;
                    actor->alpha[i] = 0x100;
                    state = actor->slideOffset[i];
                }

                if (stepAmount == 8) {
                    if (state < 0x18) {
                        actor->y[i] += 8;
                    }
                    actor->topY[i] += 8;
                    state = gMenuChoicePromptState[i];
                } else {
                    if (state < 0x10) {
                        actor->y[i] -= 8;
                    }
                    actor->topY[i] -= 8;
                    state = gMenuChoicePromptState[i];
                }
            }

            new_var = 4;
            if (((state ^ 0) == 3) || (state == new_var)) {
                if ((s32)(u16)actor->alphaTimer[i] < 0x10) {
                    actor->alpha[i] -= 9;
                } else {
                    actor->alpha[i] += 9;
                }
                actor->alphaTimer[i] = ((u16)(*actor).alphaTimer[i] + 1) & 0x1F;
            }
            i++;
        } while (i < gPlayerCount);

        if (((!state) && (!state)) && (!state)) {}
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceSetupSaveChoicePrompts, (void *)arg0);
}

void initRaceSetupSaveChoicePrompts(RectListActor *arg0) {
    RectListActor *actor = arg0;

    actor->rects[0].x0 = -0x70;
    actor->rects[1].x0 = -0x34;
    actor->rects[2].x0 = -0x34;
    actor->rects[0].y0 = -0x70;
    actor->rects[1].y0 = 0x30;
    actor->rects[2].y0 = 0x30;
    actor->rects[0].x1 = 0x1C;
    actor->rects[1].x1 = -0x34;
    actor->rects[2].x1 = -0x34;
    actor->rects[0].y1 = 0x1C;
    actor->rects[1].y1 = 0x30;
    actor->rects[2].y1 = 0x30;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSetupSaveChoicePrompts);
}

void drawMenuIconTilemapSpriteActor(void *arg0) {
    SpriteActor *actor = arg0;

    drawMenuTilemapSprite(&actor->sprite.render, MENU_TILEMAP_TEXEL_4B, actor->x, actor->y);
}

void updateMenuIconTilemapSpriteActor(SpriteActor *arg0) {
    SpriteActor *temp_a2;
    MenuTilemapSprite *temp_v0;
    SpriteActor *actor = arg0;

    temp_a2 = actor;
    actor->sprite.render.scrollX--;
    temp_v0 = &temp_a2->sprite;
    if (actor->sprite.render.scrollX < 0) {
        actor->sprite.render.scrollX = 0x2FF;
    }
    temp_v0->render.scrollY = (temp_v0->render.scrollY + 1) & 0x1FF;
    addRenderCallback(&gMenuOverlayRenderCallbackList, (RenderCallback)drawMenuIconTilemapSpriteActor, (void *)temp_a2);
}

void initMenuIconTilemapSpriteActor(SpriteActor *arg0) {
    SpriteActor *temp_a2 = arg0;

    initMenuTilemapSprite(&temp_a2->sprite, getRelocatableHeapBlockBase(gAssetHandles[38]));
    temp_a2->x = temp_a2->sprite.render.tilemapWidth;
    temp_a2->y = temp_a2->sprite.render.tilemapHeight;
    setCallbackTaskCallback(temp_a2, (CallbackTaskCallback)updateMenuIconTilemapSpriteActor);
}

void initMenuTilemapSprite(MenuTilemapSprite *arg0, MenuTilemapSpriteAsset *arg1) {
    MenuTilemapSprite *dst = arg0;
    MenuTilemapSpriteAsset *src = arg1;

    dst->render.scrollX = 0;
    dst->render.scrollY = 0;
    dst->render.tilemapWidth = src->tilemapWidth;
    dst->render.tilemapHeight = src->tilemapHeight;
    dst->render.tileWidth = src->tileWidth;
    dst->render.tileHeight = src->tileHeight;
    dst->render.viewportX = -0x90;
    dst->render.viewportY = -0x68;
    dst->render.viewportWidth = 0x120;
    dst->render.viewportHeight = 0xD0;
    dst->render.tilemap = (s16 *)(src->tilemapOffset + (u8 *)src);
    dst->render.images = (u16 *)(src->imageOffset + (u8 *)src);
    dst->render.tiles = src->tiles;
    dst->render.paletteData = src->paletteOffset + (u8 *)src;
    dst->unk24 = -1;
}

void n_alSynFreeFX(s32 arg0) {
}
