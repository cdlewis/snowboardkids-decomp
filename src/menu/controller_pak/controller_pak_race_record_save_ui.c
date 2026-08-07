#include "common.h"
#include "font_encoding.h"
#include "game/save_data.h"
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/controller_pak/controller_pak_race_record_save_flow.h"
#include "game/menu/controller_pak/controller_pak_race_record_save_ui.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/race/player/race_player_input.h"

typedef struct {
    u8 state;
    u8 pad1;
    s16 scale;
    u16 slideOffset;
} ControllerPakRaceRecordSaveUiTransition;

typedef struct {
    MenuGlyphScript statusMessagePrefix[0x2CB];
    u8 choicePromptSprites[0x26];
} ControllerPakRaceRecordSaveUiDataStorage;

typedef struct {
    MenuGlyphScript statusMessages[0x13][0x26];
    u8 choicePromptSpriteSuffix[0x18];
} ControllerPakRaceRecordSaveUiDataTyped;

typedef union {
    ControllerPakRaceRecordSaveUiDataTyped typed;
    ControllerPakRaceRecordSaveUiDataStorage storage;
} ControllerPakRaceRecordSaveUiDataContent;

typedef struct {
    ControllerPakRaceRecordSaveUiDataContent content;
    MenuGlyphScript exitMessage[10];
} ControllerPakRaceRecordSaveUiData;

ControllerPakRaceRecordSaveUiData gControllerPakRaceRecordSaveUiData = {
    {
     {
            {
                // textconv requires these _() invocations to retain their original line layout.
                // clang-format off
                { _("{COLOR:7}CHECKING.") },
                { _("{COLOR:7}SEARCHING FOR\nGAME NOTE.") },
                { _("{COLOR:7}SAVING.") },
                { _("{COLOR:7}FIXING.") },
                { _("{COLOR:7}INSERT AND\nPRESS BUTTON.") },
                { _("{COLOR:7}FINISHED\nSELECTING.") },
                { 0 },
                { _("{COLOR:7}ERROR.  DATA\nMAY BE ERASED.") },
                { 0 },
                { 0 },
                { _("{COLOR:7}END GAME AND\nDON'T SAVE?") },
                { _("{COLOR:7}NOT ENOUGH\nPAGES.") },
                { _("{COLOR:7}NO OPEN\nNOTES.") },
                { _("{COLOR:7}DID NOT SAVE.\nEND GAME?") },
                { _("{COLOR:7}COULD NOT\nREPAIR PAK.") },
                { _("{COLOR:7}IS THIS\nOK?") },
                { _("{COLOR:7}CONTROLLER\nPAK FIXED.") },
                { _("{COLOR:7}RUMBLE PAK\nDETECTED.") },
                { _("{COLOR:7}INSERT\nANOTHER PAK.") },
            },
            {
                0x15, 0x16, 0x0E, 0x0F, 0x15, 0x16, 0x0F, 0x14,
                0x0F, 0x15, 0x0F, 0x15, 0x17, 0x18, 0x17, 0x18,
                0x17, 0x18, 0x17, 0x18, 0x0F, 0x14, 0x00, 0x00,
            },
        },
    },
    { _("DATA SAVE") },
    // clang-format on
};

#define gControllerPakRaceRecordSaveStatusMessages (gControllerPakRaceRecordSaveUiData.content.typed.statusMessages)
#define gControllerPakRaceRecordSaveChoicePromptTopSprites \
    (gControllerPakRaceRecordSaveUiData.content.storage.choicePromptSprites)
#define gControllerPakRaceRecordSaveChoicePromptBottomSprites \
    (&gControllerPakRaceRecordSaveUiData.content.storage.choicePromptSprites[1])
#define gControllerPakRaceRecordSaveExitMessage (gControllerPakRaceRecordSaveUiData.exitMessage)
extern s16 gControllerPakRaceRecordSaveStatusTransitionAlpha;
extern u16 gControllerPakRaceRecordSaveStatusTransitionTargetStatus;
extern u16 gControllerPakRaceRecordSaveStatusTransitionNextStatus;
extern u8 gControllerPakRaceRecordSaveStatusTransitionStep;
extern void *D_8010ADE0;
extern void *D_8010ADE4;

#define CONTROLLER_PAK_RACE_RECORD_SAVE_SCORE_TEXTURE_HANDLE (gAssetHandles[0x21])

struct ControllerPakRaceRecordSaveActor {
    char pad[0x18];
    s16 x;
    s16 y;
    union {
        s8 state;
        s16 scale;
    } unk1C;
    union {
        struct {
            s8 unk1E;
            s8 unk1F;
            u8 unk20;
            u8 frame;
            u16 alpha;
            u16 alphaTimer;
        } b;
        struct {
            s16 slideOffset;
            u16 selection;
            u16 alpha;
            u16 alphaTimer;
        } w;
        struct {
            u16 slideOffset;
            u16 selection;
            u16 alpha;
            u16 alphaTimer;
        } wu;
    } state;
};

const char D_800E0F30[] = "%6d";

void drawControllerPakRaceRecordSaveScorePanel(ControllerPakRaceRecordSaveActor *arg0) {
    s16 *new_var;
    s16 *badgeIndex;
    s32 xOffset;
    s32 tile;
    s32 alpha;
    s32 i;
    char text[8];
    s32 next;
    s32 count;

    drawMenuSprite(
        arg0->x,
        arg0->y,
        getRelocatableHeapBlockBase(CONTROLLER_PAK_RACE_RECORD_SAVE_SCORE_TEXTURE_HANDLE),
        0xA,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0x40),
        arg0->y,
        getRelocatableHeapBlockBase(CONTROLLER_PAK_RACE_RECORD_SAVE_SCORE_TEXTURE_HANDLE),
        0xB,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0x80),
        arg0->y,
        getRelocatableHeapBlockBase(CONTROLLER_PAK_RACE_RECORD_SAVE_SCORE_TEXTURE_HANDLE),
        0xC,
        0x20,
        0x20,
        0,
        0
    );

    text[0] = '1';
    text[1] = 0;
    drawMenuAsciiText((s16)(arg0->x + 0x32), (s16)((unsigned long)(arg0->y + 2)), text, 0, 0x100);

    sprintf(text, D_800E0F30, gRacePlayers[0].money);
    drawMenuAsciiText((s16)(arg0->x + 0x44), (s16)(arg0->y + 0x1B), text, 0, 0x100);

    if (gGameSaveDataBuffer[0].progressionLevel == 3) {
        count = 3;
    } else {
        count = gGameSaveDataBuffer[0].progressionLevel + 1;
    }

    i = 0;
    if (count > 0) {
        next = 0;
        do {
            xOffset = 1;
            xOffset = i + xOffset;
            if ((s32)gGameSaveDataBuffer[0].progressionLevel < xOffset) {
                alpha = 0x70;
            } else {
                alpha = 0x100;
            }

            drawMenuSpriteWithAlpha(
                (s16)(arg0->x + next + 4),
                (s16)(arg0->y + 0x11),
                getRelocatableHeapBlockBase(CONTROLLER_PAK_RACE_RECORD_SAVE_SCORE_TEXTURE_HANDLE),
                0x19,
                0x20,
                0x20,
                0,
                alpha,
                9 - i
            );
            i = xOffset;
            next += 0x10;
        } while (xOffset != count);
        i = 0;
    }

    count = 6;
    if (gGameSaveDataBuffer[0].progressionLevel == 1) {
        count = 7;
    } else if (gGameSaveDataBuffer[0].progressionLevel == 2) {
        count = 8;
    } else {
        i = 0;
        if (gGameSaveDataBuffer[0].progressionLevel == 3) {
            count = 9;
        }
    }

    if (count > 0) {
        xOffset = 0;
        new_var = gPlayerBadgeDisplayOrder;
        badgeIndex = new_var;
        do {
            tile = gGameSaveDataBuffer[0].cupPlacements[*badgeIndex];
            alpha = 0x70;
            if (tile != 0) {
                alpha = 0x100;
                tile += 6;
            } else {
                tile = 9;
                if ((!i) && (!i)) {}
            }
            drawMenuSpriteWithAlpha(
                (s16)(arg0->x + xOffset + 4),
                (s16)(arg0->y + 0x27),
                getRelocatableHeapBlockBase(CONTROLLER_PAK_RACE_RECORD_SAVE_SCORE_TEXTURE_HANDLE),
                i + 0x1A,
                0x20,
                0x20,
                0,
                alpha,
                tile
            );
            i++;
            xOffset += 0xE;
            badgeIndex++;
        } while (i != count);
    }
}

void updateControllerPakRaceRecordSaveScorePanel(ControllerPakRaceRecordSaveActor *arg0) {
    u8 state = arg0->unk1C.state;

    switch (state) {
        case 0:
            arg0->x -= 0x20;
            if (arg0->x < -0x43) {
                arg0->x = -0x44;
                arg0->unk1C.state = 1;
                D_8010ADE0 =
                    createCallbackTask((CallbackTaskCallback)initControllerPakRaceRecordSavePromptFrame, 0, 0x62);
                createCallbackTask((CallbackTaskCallback)initControllerPakRaceRecordSaveExitMessage, 0, 0x63);
            }
            state = arg0->unk1C.state;
            break;
        case 1:
            if (gControllerPakStatusCodes[0] != 0) {
                if (gMenuSelectionConfirmTimer == 0x14) {
                    state = arg0->unk1C.state = 2;
                }
            }
            break;
        case 2:
            arg0->x -= 0x20;
            if (arg0->x < -0x117) {
                arg0->unk1C.state = 3;
            }
            state = arg0->unk1C.state;
            break;
        case 3:
            break;
    }
    state = arg0->unk1C.state;
    if (state == 3) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawControllerPakRaceRecordSaveScorePanel, arg0);
}

void initControllerPakRaceRecordSaveScorePanel(ControllerPakRaceRecordSaveActor *arg0) {
    arg0->x = 0x90;
    arg0->y = -0x20;
    arg0->unk1C.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakRaceRecordSaveScorePanel);
}

void drawControllerPakRaceRecordSavePromptFrame(ControllerPakRaceRecordSaveActor *arg0) {
    s32 zero = 0;
    u16 alpha;

    if (gControllerPakStatusCodes[0] != 8) {
        alpha = arg0->unk1C.scale;
        drawMenuSpriteWithAlpha(
            (s16)(arg0->x - 2),
            (s16)(arg0->y + 0xC),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            8,
            0x20,
            0x20,
            zero,
            alpha,
            zero
        );
        drawMenuSpriteWithAlpha(
            (s16)(arg0->x + 0x3E),
            (s16)(arg0->y + 0xC),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            9,
            0x20,
            0x20,
            zero,
            alpha,
            zero
        );
    }
}

void updateControllerPakRaceRecordSavePromptFrame(ControllerPakRaceRecordSaveActor *arg0) {
    u8 state = arg0->state.b.unk1F;

    switch (state) {
        case 0:
            arg0->unk1C.scale += 0x26;
            if (arg0->unk1C.scale >= 0x100) {
                arg0->unk1C.scale = 0x100;
                arg0->state.b.unk1F = 1;
                D_8010ADE4 =
                    createCallbackTask((CallbackTaskCallback)initControllerPakRaceRecordSaveStatusMessage, 0, 0x63);
            }
            state = arg0->state.b.unk1F;
            break;
        case 1:
            state = arg0->state.b.unk1F = 2;
            break;
        case 2:
            if ((gControllerPakStatusCodes[0] == 5) && (gMenuSelectionConfirmTimer == 0x14)) {
                state = arg0->state.b.unk1F = 3;
            }
            break;
        case 3:
            arg0->x -= 0x20;
            if (arg0->x < -0x117) {
                arg0->state.b.unk1F = 4;
            }
            state = arg0->state.b.unk1F;
            break;
        case 4:
            break;
    }
    if ((unsigned int)state == 4) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawControllerPakRaceRecordSavePromptFrame, arg0);
}

void initControllerPakRaceRecordSavePromptFrame(ControllerPakRaceRecordSaveActor *arg0) {
    arg0->x = -0x44;
    arg0->y = -0x1E;
    arg0->unk1C.scale = 0;
    arg0->state.b.unk1F = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakRaceRecordSavePromptFrame);
}

void drawControllerPakRaceRecordSaveStatusMessage(ControllerPakRaceRecordSaveActor *arg0) {
    MenuGlyphScript *text;
    void *texture;

    if (gControllerPakStatusCodes[0] != 8) {
        text = gControllerPakRaceRecordSaveStatusMessages[gControllerPakStatusCodes[0]];
        drawMenuGlyphScript(arg0->x, arg0->y, text, 1, arg0->unk1C.scale, 0);
        if (((gControllerPakStatusCodes[0] == 4) || (gControllerPakStatusCodes[0] >= 7)) &&
            (gMenuChoicePromptState[0] == 0)) {
            if (arg0->unk1C.scale == 0x100) {
                texture = getRelocatableHeapBlockBase(gAssetHandles[0x21]);
                drawMenuSprite(
                    (s16)(arg0->x + 0x70),
                    (s16)(arg0->y + 0x10),
                    texture,
                    ((s32)arg0->state.b.frame >= 8) + 5,
                    0x20,
                    0x20,
                    0,
                    0
                );
            }
        }
    }
}

void updateControllerPakRaceRecordSaveStatusMessage(ControllerPakRaceRecordSaveActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->state.b.unk20;
    if (state != (globalState = gControllerPakRaceRecordSaveStatusTransition.step)) {
        arg0->state.b.unk20 = globalState;
        state = globalState;
        arg0->unk1C.scale = gControllerPakRaceRecordSaveStatusTransition.alpha;
        arg0->state.wu.slideOffset = gControllerPakRaceRecordSaveStatusTransition.targetStatus;
    }

    switch (state) {
        case 0:
            arg0->state.b.unk20 = 1;
            state = arg0->state.b.unk20;
            break;
        case 1:
            if ((gControllerPakStatusCodes[0] == 5) && (gMenuSelectionConfirmTimer == 0x14)) {
                arg0->state.b.unk20 = 4;
                state = arg0->state.b.unk20;
            }
            break;
        case 2:
            arg0->unk1C.scale += 0x26;
            if (arg0->unk1C.scale >= 0x100) {
                arg0->unk1C.scale = 0x100;
                arg0->state.b.unk20 = 1;
            }
            state = arg0->state.b.unk20;
            break;
        case 3:
            arg0->unk1C.scale -= 0x26;
            if (arg0->unk1C.scale <= (state * 0)) {
                arg0->unk1C.scale = 0;
                arg0->state.b.unk20 = 2;
                gMenuChoicePromptState[0] = 0;
                gControllerPakStatusCodes[0] = arg0->state.wu.slideOffset;
            }
            state = arg0->state.b.unk20;
            break;
        case 4:
            arg0->x -= 0x20;
            if (arg0->x < -0x117) {
                arg0->state.b.unk20 = 5;
            }
            state = arg0->state.b.unk20;
            break;
    }

    arg0->state.b.frame = (arg0->state.b.frame + 1) & 0xF;
    gControllerPakRaceRecordSaveStatusTransitionStep = state;
    if ((u8)arg0->state.b.unk20 == 5) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawControllerPakRaceRecordSaveStatusMessage, arg0);
}

void initControllerPakRaceRecordSaveStatusMessage(ControllerPakRaceRecordSaveActor *arg0) {
    arg0->x = -0x42;
    arg0->y = -0xE;
    arg0->unk1C.scale = 0x100;
    gControllerPakRaceRecordSaveStatusTransitionAlpha = 0x100;
    arg0->state.b.unk20 = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakRaceRecordSaveStatusMessage);
}

void drawControllerPakRaceRecordSaveStatusChoicePrompt(ControllerPakRaceRecordSaveActor *arg0) {
    u32 drawAlpha;
    int isEvenState;
    u16 alpha;
    s16 state;

    state = gMenuChoicePromptState[0];
    if (state != 0) {
        isEvenState = !(state & 1);
        if (isEvenState) {
            alpha = 0x100;
        } else {
            alpha = 0x60;
        }

        drawAlpha = alpha;
        drawMenuSpriteWithAlpha(
            arg0->x,
            arg0->unk1C.scale,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gControllerPakRaceRecordSaveChoicePromptBottomSprites[gControllerPakStatusCodes[0] * 2],
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
            arg0->x,
            arg0->y,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gControllerPakRaceRecordSaveChoicePromptTopSprites[gControllerPakStatusCodes[0] * 2],
            0x20,
            0x20,
            0,
            alpha,
            0
        );

        state = gMenuChoicePromptState[0];
        if ((state == 3) || (state == 4)) {
            drawMenuSpriteWithAlpha(
                arg0->x,
                (s16)(((gMenuChoicePromptState[0] * 0x10) + arg0->y) - 0x30),
                getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                0x12,
                0x20,
                0x20,
                0,
                (u16)arg0->state.w.alpha,
                7
            );
            state = gMenuChoicePromptState[0];
        }
    }
    if ((state >= 5) && (arg0->state.w.slideOffset == 0)) {
        gControllerPakRaceRecordSaveStatusTransitionTargetStatus = arg0->state.w.selection;
        gControllerPakRaceRecordSaveStatusTransition.step = 3;
        gMenuChoicePromptState[0] = 0;
        if (arg0->state.w.selection == 0) {
            gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
            gControllerPakRaceRecordSaveStatusTransition.step = 1;
            gControllerPakStatusCodes[0] = arg0->state.w.selection;
        }
    }
}

void updateControllerPakRaceRecordSaveStatusChoicePrompt(ControllerPakRaceRecordSaveActor *arg0) {
    s32 temp_a0;
    s32 var_v1;

    if (gControllerPakRaceRecordSaveStatusTransitionNextStatus != arg0->state.w.selection) {
        arg0->state.w.selection = gControllerPakRaceRecordSaveStatusTransitionNextStatus;
    }
    if ((gMenuChoicePromptState[0] != 0) && (gMenuChoicePromptState[0] != 3) && (gMenuChoicePromptState[0] != 4)) {
        if (gMenuChoicePromptState[0] < 5) {
            var_v1 = 1;
        } else {
            var_v1 = -1;
        }
        temp_a0 = var_v1 * 8;
        arg0->state.w.slideOffset += temp_a0;
        if (arg0->state.w.slideOffset == 0x20) {
            gMenuChoicePromptState[0] += 2;
            arg0->state.b.alphaTimer = 0;
            arg0->state.b.alpha = 0x100;
        }
        if (temp_a0 == 8) {
            if (arg0->state.w.slideOffset < 0x18) {
                arg0->y += 8;
            }
            arg0->unk1C.scale += 8;
        } else {
            if (arg0->state.w.slideOffset < 0x10) {
                arg0->y -= 8;
            }
            arg0->unk1C.scale -= 8;
        }
    }
    if ((gMenuChoicePromptState[0] == 3) || (gMenuChoicePromptState[0] == 4)) {
        if ((s32)arg0->state.b.alphaTimer < 0x10) {
            arg0->state.b.alpha -= 9;
        } else {
            arg0->state.b.alpha += 9;
        }
        arg0->state.b.alphaTimer = (arg0->state.b.alphaTimer + 1) & 0x1F;
    }
    addRenderCallback(
        &gMenuRenderCallbackList,
        (RenderCallback)drawControllerPakRaceRecordSaveStatusChoicePrompt,
        arg0
    );
}

void initControllerPakRaceRecordSaveStatusChoicePrompt(ControllerPakRaceRecordSaveActor *arg0) {
    arg0->x = -0x28;
    arg0->y = 0xC;
    arg0->unk1C.scale = 0xC;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakRaceRecordSaveStatusChoicePrompt);
}

void drawControllerPakRaceRecordSaveExitMessage(ControllerPakRaceRecordSaveActor *arg0) {
    drawMenuGlyphScriptDefaultFont(arg0->x, arg0->y, gControllerPakRaceRecordSaveExitMessage, 1, arg0->unk1C.scale);
}

void updateControllerPakRaceRecordSaveExitMessage(ControllerPakRaceRecordSaveActor *arg0) {
    u8 state = arg0->state.b.unk1E;

    switch (state) {
        case 0:
            arg0->unk1C.scale += 0x26;
            if (arg0->unk1C.scale >= 0x100) {
                arg0->unk1C.scale = 0x100;
                arg0->state.b.unk1E = 1;
            }
            state = arg0->state.b.unk1E;
            break;
        case 1:
            if ((gControllerPakStatusCodes[0] == 5) && (gMenuSelectionConfirmTimer == 0x14)) {
                state = arg0->state.b.unk1E = 2;
            }
            break;
        case 2:
            arg0->x -= 0x20;
            if (arg0->x < -0xD7) {
                arg0->state.b.unk1E = 4;
            }
            state = arg0->state.b.unk1E;
            break;
    }
    if ((unsigned int)state == 4) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawControllerPakRaceRecordSaveExitMessage, arg0);
}

void initControllerPakRaceRecordSaveExitMessage(ControllerPakRaceRecordSaveActor *arg0) {
    arg0->x = -0x24;
    arg0->y = -0x38;
    arg0->unk1C.scale = 0;
    arg0->state.b.unk1E = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakRaceRecordSaveExitMessage);
}
