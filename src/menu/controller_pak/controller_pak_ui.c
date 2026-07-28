#include "common.h"
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

#define CONTROLLER_PAK_DELETE_PANEL_HANDLE (gAssetHandles[0x29])

typedef struct {
    s16 pad0;
    u8 state;
    u8 pad3;
    u16 targetScale;
    u16 timer;
    u8 selectedOption;
    u8 confirmSelection;
} ControllerPakRumbleCheckPromptTransition;

extern ControllerPakRumbleCheckPromptTransition gControllerPakRumbleCheckPromptTransition;
extern u8 gControllerPakRumbleCheckPromptState;
extern u8 gControllerPakRumbleCheckPromptConfirmSelection;
extern u8 gControllerPakMenuConfirmChoice;
extern u8 gControllerPakMenuCursorState;
extern u8 gControllerPakDeletePromptState;
extern s32 gControllerPakFreeBytes;
extern s32 gControllerPakFreeFileCount;
extern MenuGlyphScript gControllerPakContinuePromptText[];
extern MenuGlyphScript gControllerPakAreYouSureText[];
extern MenuGlyphScript gControllerPakCouldNotEraseNoteText[];
extern MenuGlyphScript gControllerPakRumbleCheckPromptText[][0x34];
extern MenuGlyphScript gControllerPakRumbleCheckNotUsedText[];
extern MenuGlyphScript gControllerPakRumbleCheckNoEntryText[];
extern MenuGlyphScript gControllerPakRumblePakText[];

void drawControllerPakContinuePrompt(ControllerPakOptionsActor *arg0) {
    s32 i;
    s32 j;
    u16 alpha;

    drawMenuSprite((s16)(arg0->common.x - 4), (s16)(arg0->common.y - 4), getRelocatableHeapBlockBase(gAssetHandles[0x29]), 2, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->common.x + 0xD4), (s16)(arg0->common.y - 4), getRelocatableHeapBlockBase(gAssetHandles[0x29]), 4, 0x20, 0x20, 0, 0);

    i = 0;
    do {
        drawMenuSprite((s16)(arg0->common.x + i), (s16)(arg0->common.y - 4), getRelocatableHeapBlockBase(gAssetHandles[0x29]), 3, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)(arg0->common.x + i), (s16)(arg0->common.y + 0x24), getRelocatableHeapBlockBase(gAssetHandles[0x29]), 8, 0x20, 0x20, 0, 0);
        i += 0x10;
    } while (i < 0xE0);

    drawMenuSprite((s16)(arg0->common.x - 4), (s16)(arg0->common.y + 0x24), getRelocatableHeapBlockBase(gAssetHandles[0x29]), 7, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->common.x + 0xD4), (s16)(arg0->common.y + 0x24), getRelocatableHeapBlockBase(gAssetHandles[0x29]), 9, 0x20, 0x20, 0, 0);

    i = 0;
loop_outer:
    drawMenuSprite((s16)(arg0->common.x - 4), (s16)(arg0->common.y + i), getRelocatableHeapBlockBase(gAssetHandles[0x29]), 5, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->common.x + 0xD4), (s16)(arg0->common.y + i), getRelocatableHeapBlockBase(gAssetHandles[0x29]), 6, 0x20, 0x20, 0, 0);
    j = 0;
    do {
        drawMenuSprite((s16)(arg0->common.x + j), (s16)(arg0->common.y + i), getRelocatableHeapBlockBase(gAssetHandles[0x29]), 0xB, 0x20, 0x20, 0, 0);
        j += 0x10;
    } while (j != 0xE0);
    i += 0x10;
    if (i != 0x30) {
        // IDO register allocation nudge for matching codegen.
        if (1) {
        }
        goto loop_outer;
    }

    drawMenuGlyphScript((s16)(arg0->common.x + 0x30), arg0->common.y, gControllerPakContinuePromptText, 0, 0x100, 0);

    alpha = (arg0->selectedOption == 0) ? 0x100 : 0x60;

    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x4C), (s16)(arg0->common.y + 0x10), getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0x17, 0x20,
                  0x20, 0, alpha, 0);

    if (alpha == 0x100) {
        alpha = 0x60;
    } else {
        alpha = 0x100;
    }

    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x4C), (s16)(arg0->common.y + 0x20), getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0x18, 0x20,
                  0x20, 0, alpha, 0);
    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x4C), (s16)(arg0->common.y + (arg0->selectedOption * 0x10) + 0x10),
                  getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0x12, 0x20, 0x20, 0, arg0->scale, 0);
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
            temp_a2->selectedOption = gCurrentGameTask->timer;
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

    if (actor->state == 8) {
        yAdjust = 8;
    } else {
        yAdjust = 0;
    }

    drawMenuSpriteWithAlpha((s16)(actor->common.x - 4), (s16)(actor->common.y - yAdjust - 4), getRelocatableHeapBlockBase(gAssetHandles[0x29]), 2,
                  0x20, 0x20, 0, actor->scale, 0);
    drawMenuSpriteWithAlpha((s16)(actor->common.x + 0xD4), (s16)(actor->common.y - yAdjust - 4), getRelocatableHeapBlockBase(gAssetHandles[0x29]),
                  4, 0x20, 0x20, 0, actor->scale, 0);

    for (i = 0; i < 0xE0; i += 0x10) {
        drawMenuSpriteWithAlpha((s16)(actor->common.x + i), (s16)(actor->common.y - yAdjust - 4), getRelocatableHeapBlockBase(gAssetHandles[0x29]),
                      3, 0x20, 0x20, 0, actor->scale, 0);
        drawMenuSpriteWithAlpha((s16)(actor->common.x + i), (s16)(actor->common.y + yAdjust + 0x24), getRelocatableHeapBlockBase(gAssetHandles[0x29]),
                      8, 0x20, 0x20, 0, actor->scale, 0);
    }

    drawMenuSpriteWithAlpha((s16)(actor->common.x - 4), (s16)(actor->common.y + yAdjust + 0x24), getRelocatableHeapBlockBase(gAssetHandles[0x29]),
                  7, 0x20, 0x20, 0, actor->scale, 0);
    drawMenuSpriteWithAlpha((s16)(actor->common.x + 0xD4), (s16)(actor->common.y + yAdjust + 0x24), getRelocatableHeapBlockBase(gAssetHandles[0x29]),
                  9, 0x20, 0x20, 0, actor->scale, 0);

    for (i = 0; i < 0x30; i += 0x10) {
        drawMenuSpriteWithAlpha((s16)(actor->common.x - 4), (s16)(actor->common.y + i), getRelocatableHeapBlockBase(gAssetHandles[0x29]), 5, 0x20,
                      0x20, 0, actor->scale, 0);
        drawMenuSpriteWithAlpha((s16)(actor->common.x + 0xD4), (s16)(actor->common.y + i), getRelocatableHeapBlockBase(gAssetHandles[0x29]), 6, 0x20,
                      0x20, 0, actor->scale, 0);
        for (j = 0; j < 0xE0; j += 0x10) {
            drawMenuSpriteWithAlpha((s16)(actor->common.x + j), (s16)(actor->common.y + i), getRelocatableHeapBlockBase(gAssetHandles[0x29]), 0xB,
                          0x20, 0x20, 0, actor->scale, 0);
            if (((!actor) && (!actor)) && (!actor)) {
            }
        }
    }

    if (actor->state == 8) {
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
            drawMenuGlyphScript((s16)(actor->common.x + 0x10), (s16)(actor->common.y - yAdjust + playerIndex * 0x10),
                          playerNumberText, 0, alpha, 0);
            drawMenuGlyphScript((s16)(actor->common.x + 0x40), (s16)(actor->common.y - yAdjust + playerIndex * 0x10), message, 0, alpha,
                          0);
        }
    } else {
        drawMenuGlyphScript(actor->common.x, actor->common.y, gControllerPakRumbleCheckPromptText[actor->targetScale], 0,
                            actor->scale, 0);
    }

    if (actor->state == 9) {
        alpha = (gControllerPakRumbleCheckPromptTransition.confirmSelection == 0) ? 0x100 : 0x60;
        drawMenuSpriteWithAlpha((s16)(actor->common.x + 0x4C), (s16)(actor->common.y + 0x10), getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                      0x17, 0x20, 0x20, 0, alpha, 0);
        if (alpha == 0x100) {
            alpha = 0x60;
        } else {
            alpha = 0x100;
        }
        drawMenuSpriteWithAlpha((s16)(actor->common.x + 0x4C), (s16)(actor->common.y + 0x20), getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                      0x18, 0x20, 0x20, 0, alpha, 0);
        drawMenuSpriteWithAlpha((s16)(actor->common.x + 0x4C),
                      (s16)(actor->common.y + (gControllerPakRumbleCheckPromptTransition.confirmSelection * 0x10) + 0x10),
                      getRelocatableHeapBlockBase(gAssetHandles[0x21]), 0x12, 0x20, 0x20, 0, actor->optionScale, 0);
    }

    if ((actor->state == 1) || (actor->state == 3) || (actor->state == 8)) {
        drawMenuSprite((s16)(actor->common.x + 0xD0), (s16)(actor->common.y + yAdjust + 0x20),
                       getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                       (actor->timer >= 8) + 5, 0x20, 0x20, 0, 0);
    }
}

void updateControllerPakRumbleCheckPrompt(ControllerPakRumbleCheckPromptActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->state;
    if (state != (globalState = gControllerPakRumbleCheckPromptTransition.state)) {
        arg0->state = globalState;
        if (1) {}
        {}
        if (1) {}
        if (1) {}
        if (1) {}
        arg0->targetScale = gControllerPakRumbleCheckPromptTransition.targetScale;
        arg0->timer = 0;
        arg0->optionScale = 0x100; state = globalState;
    }

    switch (state) {
        case 0:
            arg0->scale += 0x28;
            if (arg0->scale >= 0x100) {
                arg0->scale = 0x100;
                if (gControllerPakRumbleCheckPromptTransition.selectedOption == 1) {
                    arg0->state = 3;
                } else {
                    arg0->state = 1;
                }
            }
            state = arg0->state;
            break;
        case 1:
            state = arg0->state;
            arg0->timer = (arg0->timer + 1) & 0xF;
            break;
        case 2:
            state = arg0->state;
            arg0->timer = 0;
            break;
        case 3:
            state = arg0->state;
            arg0->timer = (arg0->timer + 1) & 0xF;
            break;
        case 4:
            arg0->scale -= 0x28;
            if (arg0->scale <= 0) {
                arg0->scale = 0;
                arg0->state = 5;
            }
            state = arg0->state;
            break;
        case 7:
            state = arg0->state;
            arg0->timer = 0;
            break;
        case 8:
            state = arg0->state;
            arg0->timer = (arg0->timer + 1) & 0xF;
            break;
        case 9:
            if ((s32)arg0->timer < 0x10) {
                arg0->optionScale -= 9;
            } else {
                arg0->optionScale += 9;
            }
            state = arg0->state;
            arg0->timer = (arg0->timer + 1) & 0x1F;
            break;
        case 5:
        case 6:
            break;
    }

    gControllerPakRumbleCheckPromptState = state;
    if (arg0->state == 5) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawControllerPakRumbleCheckPrompt, (void *)arg0);
}

void initControllerPakRumbleCheckPrompt(ControllerPakRumbleCheckPromptActor *arg0) {
    arg0->common.x = -0x70;
    arg0->common.y = -0x1C;
    arg0->state = 6;
    arg0->scale = 0;
    arg0->timer = 0;
    arg0->targetScale = 0;
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
    drawMenuSpriteWithAlpha(arg0->common.x, arg0->common.y, getRelocatableHeapBlockBase(gAssetHandles[0x21]), 1, 0x20, 0x20, 0, drawAlpha, 0);

    if (drawAlpha == 0x100) {
        alpha = 0x80;
    } else {
        alpha = 0x100;
    }

    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x70), arg0->common.y, getRelocatableHeapBlockBase(gAssetHandles[0x21]), 2, 0x20, 0x20, 0,
                  alpha, 0);
    drawMenuSpriteWithAlpha((s16)(arg0->common.x + (gControllerPakMenuState.mainChoice * 0x70)), arg0->common.y, getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                  5, 0x20, 0x20, 0, arg0->scale, 0);
}

void updateControllerPakFileDeleteMainOptionsUi(ControllerPakOptionsActor *arg0) {
    switch (gControllerPakMenuCursorState) {
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
        if (!gControllerPakMenuConfirmChoice) {
        }
    }

    drawMenuSpriteWithAlpha(arg0->common.x, arg0->common.y, getRelocatableHeapBlockBase(gAssetHandles[0x21]), 3, 0x20, 0x20, 0, alpha, 0);
    drawMenuSpriteWithAlpha((s16)(actor->common.x + 0x70), actor->common.y, getRelocatableHeapBlockBase(gAssetHandles[0x21]), 4, 0x20, 0x20, 0,
                  otherAlpha, 0);

    if (gControllerPakMenuCursorState == 2) {
        drawMenuSpriteWithAlpha((s16)(actor->common.x + (gControllerPakMenuConfirmChoice * 0x70)), actor->common.y, getRelocatableHeapBlockBase(gAssetHandles[0x21]), 5, 0x20,
                      0x20, 0, actor->scale, 0);
    }
}

void updateControllerPakFileDeleteConfirmOptionsUi(ControllerPakOptionsActor *arg0) {
    switch (gControllerPakMenuCursorState) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawControllerPakFileDeleteConfirmOptions, (void *)arg0);
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
    arg0->x2 = 0x5C;
    arg0->y2 = temp_v0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakFileDeleteFreeSpaceInfo);
}

// drawControllerPakFileDeleteFileList best match: 94.081% (nonmatchings/drawControllerPakFileDeleteFileList-8498672362023432715/base_31.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/controller_pak/controller_pak_ui/drawControllerPakFileDeleteFileList.s")

#ifdef NON_MATCHING
extern u8 gControllerPakVisibleFileIndex;
void drawControllerPakFileDeleteFileList(ControllerPakFileListActor *arg0) {
    char padAfter[0x14];
    register s32 alpha;
    register s32 i;
    register s32 insertIndex;
    u16 textB0[4];
    u16 fileNameText[19];
    u16 text7C[5];
    s32 rowY;
    u16 fileIndex;
    s32 state;

    if (gControllerPakMenuState.visibleFileIndex == 0) {
        fileIndex = gControllerPakMenuState.fileIndex;
    } else if (gControllerPakMenuState.visibleFileIndex == 4) {
        fileIndex = gControllerPakMenuState.fileIndex - 4;
    } else {
        fileIndex = gControllerPakMenuState.fileIndex - gControllerPakMenuState.visibleFileIndex;
    }

    rowY = 0;
    do {
        state = gControllerPakMenuState.state;
        alpha = 0xE0;
        i = 0;
        if ((s32)state <= 0) {
        } else {
            alpha = 0xE0;
            if (1) {
            }
            if (fileIndex == gControllerPakMenuState.fileIndex) {
                alpha = 0x100;
            }
        }

        if (fileIndex < 9) {
            textB0[0] = 0xFFFE;
            i = -4;
        } else {
            textB0[0] = 1;
        }
        textB0[2] = 0xFFFF;
        textB0[1] = (fileIndex + 1) % 10;
        drawMenuGlyphScript((s16)(arg0->positions[0].x + i), (s16)(arg0->positions[0].y + rowY), textB0, 1, alpha, 8);

        if (gControllerPakFileStates[fileIndex].company_code != 0) {
            i = 0;
            insertIndex = 0;
            do {
                if ((gControllerPakFileStates[fileIndex].game_name[i] == 0) && (insertIndex == 0)) {
                    insertIndex = i;
                }
                if (gControllerPakFileStates[fileIndex].game_name[i] < 0x10) {
                    fileNameText[i] = 0xFFFE;
                } else {
                    fileNameText[i] = gControllerPakFileStates[fileIndex].game_name[i] - 0x10;
                }
                i++;
            } while (i < 0x10);

            fileNameText[16] = 0xFFFE;
            fileNameText[17] = 0xFFFE;
            if (gControllerPakFileStates[fileIndex].ext_name[0] != 0) {
                if (insertIndex == 0) {
                    insertIndex = 0x10;
                }
                fileNameText[insertIndex] = 0x2C;
                fileNameText[insertIndex + 1] = gControllerPakFileStates[fileIndex].ext_name[0] - 0x10;
            }
            fileNameText[18] = 0xFFFF;
            drawMenuGlyphScript(arg0->positions[1].x, (s16)(arg0->positions[1].y + rowY), fileNameText, 1, alpha, 8);

            i = 0;
            do {
                insertIndex = (gControllerPakFileStates[fileIndex].game_code >> ((3 - i) * 8)) & 0xFF;
                if ((insertIndex < 0x30) || (insertIndex >= 0x5B)) {
                    text7C[i] = 0xFFFE;
                } else if (insertIndex >= 0x41) {
                    text7C[i] = insertIndex - 0x37;
                } else if (insertIndex >= 0x3A) {
                    text7C[i] = 0xFFFE;
                } else {
                    text7C[i] = insertIndex - 0x30;
                }
                i++;
            } while (i < 4);
            text7C[4] = 0xFFFF;
            drawMenuGlyphScript(arg0->positions[2].x, (s16)(arg0->positions[2].y + rowY), text7C, 1, alpha, 8);

            i = 0;
            do {
                state = (gControllerPakFileStates[fileIndex].company_code >> ((1 - i) * 8)) & 0xFF;
                insertIndex = state;
                if ((insertIndex < 0x30) || (insertIndex >= 0x5B)) {
                    text7C[i] = 0xFFFE;
                } else if (insertIndex >= 0x41) {
                    text7C[i] = insertIndex - 0x37;
                } else if (insertIndex >= 0x3A) {
                    text7C[i] = 0xFFFE;
                } else {
                    text7C[i] = insertIndex;
                    text7C[i] = text7C[i] - 0x30;
                }
                i++;
            } while (i < 2);
            text7C[2] = 0xFFFF;
            drawMenuGlyphScript(arg0->positions[3].x, (s16)(arg0->positions[3].y + rowY), text7C, 1, alpha, 8);

            i = 0;
            do {
                textB0[i] = 0xFFFE;
                i++;
            } while (i < 3);
            insertIndex = gControllerPakFileStates[fileIndex].file_size >> 8;
            i = 2;
            do {
                textB0[i] = insertIndex % 10;
                insertIndex = insertIndex / 10;
                i--;
            } while (insertIndex != 0);
            textB0[3] = 0xFFFF;
            drawMenuGlyphScript(arg0->positions[4].x, (s16)(arg0->positions[4].y + rowY), textB0, 1, alpha, 8);
        }

        rowY += 0x10;
        fileIndex++;
    } while (rowY != 0x50);

    if (gControllerPakMenuCursorState != 0) {
        drawMenuSpriteWithAlpha(arg0->positions[5].x, (s16)(arg0->positions[5].y + (gControllerPakVisibleFileIndex * 0x10)), getRelocatableHeapBlockBase(gAssetHandles[0x21]), 6,
                      0x20, 0x20, 0, arg0->cursorScale, 0);
        drawMenuSpriteWithAlpha((s16)(arg0->positions[5].x + 0x80),
                      (s16)(arg0->positions[5].y + (((((gControllerPakVisibleFileIndex & 0xFFFF) & 0xFFFF) & 0xFFFF)) * 0x10)),
                      getRelocatableHeapBlockBase(gAssetHandles[0x21]), 7, 0x20, 0x20, 0, arg0->cursorScale, 0);
    }
}
#endif

void updateControllerPakFileDeleteFileListUi(ControllerPakFileListActor *arg0) {
    if (gControllerPakMenuCursorState == 1) {
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
    s32 temp_v1 = -0x84;

    arg0->positions[0].x = temp_v1;
    arg0->positions[0].y = temp_v0;
    arg0->positions[1].x = -0x6A;
    arg0->positions[1].y = temp_v0;
    arg0->positions[2].x = 0x2C;
    arg0->positions[2].y = temp_v0;
    arg0->positions[3].x = 0x54;
    arg0->positions[3].y = temp_v0;
    arg0->positions[4].x = 0x69;
    arg0->positions[4].y = temp_v0;
    arg0->positions[5].x = temp_v1;
    arg0->positions[5].y = -0x1F;
    arg0->cursorScale = 0x100;
    arg0->cursorTimer = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakFileDeleteFileListUi);
}

void drawControllerPakFileDeleteIcon(ControllerPakSpriteActor *arg0) {
    drawMenuTilemapSprite((MenuRenderSprite *)&arg0->sprite, 0, arg0->common.x, arg0->common.y);
}

void updateControllerPakFileDeleteIcon(ControllerPakSpriteActor *arg0) {
    addRenderCallback(&gMenuOverlayRenderCallbackList, (RenderCallback)drawControllerPakFileDeleteIcon, arg0);
}

void initControllerPakFileDeleteIcon(ControllerPakSpriteActor *arg0) {
    initMenuTilemapSprite((MenuTilemapSprite *)&arg0->sprite, getRelocatableHeapBlockBase(gAssetHandles[0x26]));
    arg0->sprite.x = 0x10;
    arg0->sprite.y = 0x10;
    arg0->common.x = arg0->startX;
    arg0->common.y = arg0->startY;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakFileDeleteIcon);
}

void drawControllerPakMessageIcon(ControllerPakSpriteActor *arg0) {
    drawMenuTilemapSprite((MenuRenderSprite *)&arg0->sprite, 1, arg0->common.x, arg0->common.y);
}

void updateControllerPakMessageIcon(ControllerPakSpriteActor *arg0) {
    addRenderCallback(&gMenuOverlayRenderCallbackList, (RenderCallback)drawControllerPakMessageIcon, arg0);
}

void initControllerPakMessageIcon(ControllerPakSpriteActor *arg0) {
    initMenuTilemapSprite((MenuTilemapSprite *)&arg0->sprite, getRelocatableHeapBlockBase(gAssetHandles[0x26]));
    arg0->sprite.x = 0x10;
    arg0->sprite.y = 0x20;
    arg0->common.x = arg0->startX;
    arg0->common.y = arg0->startY;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateControllerPakMessageIcon);
}

void drawControllerPakFileDeleteErrorPrompt(ControllerPakWindowActor *arg0) {
    void *texture;

    drawMenuSpriteWithAlpha(arg0->common.x, arg0->common.y, getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0, 0x20, 0x20, 0, arg0->scale, 0);
    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x40), arg0->common.y, getRelocatableHeapBlockBase(gAssetHandles[0x24]), 1, 0x20, 0x20, 0, arg0->scale, 0);
    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x78), arg0->common.y, getRelocatableHeapBlockBase(gAssetHandles[0x24]), 1, 0x20, 0x20, 0, arg0->scale, 0);
    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0xB0), arg0->common.y, getRelocatableHeapBlockBase(gAssetHandles[0x24]), 2, 0x20, 0x20, 0, arg0->scale, 0);
    drawMenuGlyphScript((s16)(arg0->common.x + 4), (s16)(arg0->common.y + 4), gControllerPakCouldNotEraseNoteText, 0, arg0->scale, 0);
    if (arg0->selectedOption == 1) {
        texture = getRelocatableHeapBlockBase(gAssetHandles[0x24]);
        drawMenuSprite((s16)(arg0->common.x + 0xD4), (s16)(arg0->common.y + 0x24), texture,
                       (arg0->timer >= 8) + 5, 0x20, 0x20, 0, 0);
    }
}

void updateControllerPakFileDeleteErrorPromptUi(ControllerPakDeletePromptActor *arg0) {
    u8 globalState;
    u8 state;
    u8 *statePtr;

    globalState = gControllerPakDeletePromptState;
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

    gControllerPakMenuState.unk6 = state;
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

void drawControllerPakDeleteConfirmPrompt(ControllerPakDeletePromptActor *arg0) {
    s32 i;
    s32 j;
    s32 alpha;
    s32 limit;

    drawMenuSprite(arg0->common.x, arg0->common.y, getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 2, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->common.x + 0x90), arg0->common.y, getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 4, 0x20, 0x20, 0, 0);

    i = 0;
    do {
        drawMenuSprite((s16)(arg0->common.x + i + 0x10), arg0->common.y, getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 3, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)(arg0->common.x + i + 0x10), (s16)(arg0->common.y + 0x30), getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 8, 0x20, 0x20, 0, 0);
        i += 0x10;
    } while (i < 0x80);

    drawMenuSprite(arg0->common.x, (s16)(arg0->common.y + 0x30), getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 7, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->common.x + 0x90), (s16)(arg0->common.y + 0x30), getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 9, 0x20, 0x20, 0, 0);

    i = 0;
    limit = 0x80;
loop_outer:
    drawMenuSprite(arg0->common.x, (s16)(arg0->common.y + i + 0x10), getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 5, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->common.x + 0x90), (s16)(arg0->common.y + i + 0x10), getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 6, 0x20, 0x20, 0, 0);

    j = 0;
    do {
        drawMenuSprite((s16)(arg0->common.x + j + 0x10), (s16)(arg0->common.y + i + 0x10), getRelocatableHeapBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 0xB, 0x20, 0x20, 0, 0);
        j += 0x10;
    } while (j != limit);

    i += 0x10;
    if (i != 0x20) {
        if (1) {
        }
        goto loop_outer;
    }

    drawMenuGlyphScript((s16)(arg0->common.x + 0x1C), (s16)(arg0->common.y + 4), gControllerPakAreYouSureText, 1, 0x100, 0);

    alpha = (arg0->selectedOption == 0) ? 0x100 : 0x60;

    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x2C), (s16)(arg0->common.y + 0x18), getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0x17, 0x20, 0x20, 0, alpha, 0);

    if (alpha == 0x100) {
        alpha = 0x60;
    } else {
        alpha = 0x100;
    }

    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x2C), (s16)(arg0->common.y + 0x28), getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0x18, 0x20, 0x20, 0, alpha, 0);
    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x2C), (s16)(arg0->common.y + (arg0->selectedOption * 0x10) + 0x18), getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0x12, 0x20, 0x20, 0, arg0->scale, 0);
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
