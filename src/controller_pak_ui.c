#include "common.h"
#include "memory_allocator.h"
#include "callback_task_scheduler.h"
#include "character_select_course_menu.h"
#include "controller_pak_menu.h"
#include "controller_pak_ui.h"
#include "menu_rendering.h"
#include "title_menu.h"

#define CONTROLLER_PAK_DELETE_PANEL_HANDLE (D_80112130[0x29])

typedef struct {
    u16 x;
    u8 state;
} ControllerPakPromptTransition;

typedef struct {
    s16 pad0;
    u8 state;
    u8 pad3;
    u16 targetScale;
    u16 timer;
    u8 selectedOption;
    u8 confirmSelection;
} ControllerPakRumbleCheckPromptTransition;

extern void addRenderCallback(void *, void *, s32);
extern s32 gMenuRenderCallbackList;
extern s32 D_80124838;
extern CharacterSelectFlowState *gCurrentGameTask;
extern ControllerPakPromptTransition gControllerPakContinuePromptTransition;
extern ControllerPakRumbleCheckPromptTransition gControllerPakRumbleCheckPromptTransition;
extern ControllerPakMenuState gControllerPakMenuState;
extern s16 D_80112130[];
extern u8 gControllerPakRumbleCheckPromptState;
extern u8 gControllerPakRumbleCheckPromptConfirmSelection;
extern u8 gControllerPakMenuConfirmChoice;
extern u8 gControllerPakMenuCursorState;
extern u8 gControllerPakDeletePromptState;
extern s32 gControllerPakFreeBytes;
extern s32 gControllerPakFreeFileCount;
extern s16 gMenuCommonSpritesAssetHandle;
extern s16 D_8011217C;
extern s16 D_80112178;
extern u8 gControllerPakContinuePromptText[];
extern u8 gControllerPakAreYouSureText[];
extern u8 gControllerPakCouldNotEraseNoteText[];

void drawControllerPakContinuePrompt(ControllerPakOptionsActor *arg0) {
    s32 i;
    s32 j;
    s32 alpha;

    drawMenuSprite((s16)(arg0->common.x - 4), (s16)(arg0->common.y - 4), getMemoryBlockBase(D_80112130[0x29]), 2, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->common.x + 0xD4), (s16)(arg0->common.y - 4), getMemoryBlockBase(D_80112130[0x29]), 4, 0x20, 0x20, 0, 0);

    i = 0;
    do {
        drawMenuSprite((s16)(arg0->common.x + i), (s16)(arg0->common.y - 4), getMemoryBlockBase(D_80112130[0x29]), 3, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)(arg0->common.x + i), (s16)(arg0->common.y + 0x24), getMemoryBlockBase(D_80112130[0x29]), 8, 0x20, 0x20, 0, 0);
        i += 0x10;
    } while (i < 0xE0);

    drawMenuSprite((s16)(arg0->common.x - 4), (s16)(arg0->common.y + 0x24), getMemoryBlockBase(D_80112130[0x29]), 7, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->common.x + 0xD4), (s16)(arg0->common.y + 0x24), getMemoryBlockBase(D_80112130[0x29]), 9, 0x20, 0x20, 0, 0);

    i = 0;
loop_outer:
    drawMenuSprite((s16)(arg0->common.x - 4), (s16)(arg0->common.y + i), getMemoryBlockBase(D_80112130[0x29]), 5, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->common.x + 0xD4), (s16)(arg0->common.y + i), getMemoryBlockBase(D_80112130[0x29]), 6, 0x20, 0x20, 0, 0);
    j = 0;
    do {
        drawMenuSprite((s16)(arg0->common.x + j), (s16)(arg0->common.y + i), getMemoryBlockBase(D_80112130[0x29]), 0xB, 0x20, 0x20, 0, 0);
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

    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x4C), (s16)(arg0->common.y + 0x10), getMemoryBlockBase(D_80112130[0x24]), 0x17, 0x20,
                  0x20, 0, alpha, 0);

    if (alpha == 0x100) {
        alpha = 0x60;
    } else {
        alpha = 0x100;
    }

    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x4C), (s16)(arg0->common.y + 0x20), getMemoryBlockBase(D_80112130[0x24]), 0x18, 0x20,
                  0x20, 0, alpha, 0);
    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x4C), (s16)(arg0->common.y + (arg0->selectedOption * 0x10) + 0x10),
                  getMemoryBlockBase(D_80112130[0x24]), 0x12, 0x20, 0x20, 0, arg0->scale, 0);
}

// updateControllerPakContinuePrompt best match: 92.821%
#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_ui/updateControllerPakContinuePrompt.s")

#ifdef NON_MATCHING
void updateControllerPakContinuePrompt(ControllerPakOptionsActor *arg0) {
    s32 temp_v0;
    s32 state;
    ControllerPakOptionsActor *temp_a2 = arg0;

    temp_v0 = gControllerPakContinuePromptTransition.state;
    state = arg0->blinkState;
    if (state != temp_v0) {
        arg0->blinkState = temp_v0;
        state = temp_v0 & 0xFF;
        arg0->scale = gControllerPakContinuePromptTransition.x;
    }

    temp_v0 = state;
    switch (temp_v0) {
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
    addRenderCallback(&gMenuRenderCallbackList, drawControllerPakContinuePrompt, (s32)temp_a2);
}
#endif

void initControllerPakContinuePrompt(ControllerPakOptionsActor *arg0) {
    arg0->common.x = -0x70;
    arg0->common.y = -0x28;
    arg0->selectedOption = 0;
    arg0->blinkState = 0;
    arg0->scale = 0x100;
    arg0->timer = 0;
    setCallbackTaskCallback(arg0, updateControllerPakContinuePrompt);
}

// drawControllerPakRumbleCheckPrompt best match: 99.732% (nonmatchings/drawControllerPakRumbleCheckPrompt-5272447827802519043/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_ui/drawControllerPakRumbleCheckPrompt.s")

#ifdef NON_MATCHING
extern u8 gPlayerCount;
extern u8 gRumblePakConnectedByController[];
extern u8 gControllerPakRumbleCheckPromptText[];
extern u8 gControllerPakRumbleCheckNotUsedText[];
extern u8 gControllerPakRumbleCheckNoEntryText[];
extern u8 gControllerPakRumblePakText[];

void drawControllerPakRumbleCheckPrompt(ControllerPakRumbleCheckPromptActor *arg0) {
    register ControllerPakRumbleCheckPromptActor *actor;
    s32 i;
    s32 j;
    s32 playerIndex;
    s32 playerY;
    s32 yAdjust;
    s32 alpha;
    u16 playerNumberText[5];
    u8 *message;

    actor = arg0;

    if (actor->state == 8) {
        yAdjust = 8;
    } else {
        yAdjust = 0;
    }

    drawMenuSpriteWithAlpha((s16)(actor->common.x - 4), (s16)(actor->common.y - yAdjust - 4), getMemoryBlockBase(D_80112130[0x29]), 2,
                  0x20, 0x20, 0, actor->scale, 0);
    drawMenuSpriteWithAlpha((s16)(actor->common.x + 0xD4), (s16)(actor->common.y - yAdjust - 4), getMemoryBlockBase(D_80112130[0x29]),
                  4, 0x20, 0x20, 0, actor->scale, 0);

    i = 0;
    do {
        drawMenuSpriteWithAlpha((s16)(actor->common.x + i), (s16)(actor->common.y - yAdjust - 4), getMemoryBlockBase(D_80112130[0x29]),
                      3, 0x20, 0x20, 0, actor->scale, 0);
        drawMenuSpriteWithAlpha((s16)(actor->common.x + i), (s16)(actor->common.y + yAdjust + 0x24), getMemoryBlockBase(D_80112130[0x29]),
                      8, 0x20, 0x20, 0, actor->scale, 0);
        i += 0x10;
    } while (i < 0xE0);

    drawMenuSpriteWithAlpha((s16)(actor->common.x - 4), (s16)(actor->common.y + yAdjust + 0x24), getMemoryBlockBase(D_80112130[0x29]),
                  7, 0x20, 0x20, 0, actor->scale, 0);
    drawMenuSpriteWithAlpha((s16)(actor->common.x + 0xD4), (s16)(actor->common.y + yAdjust + 0x24), getMemoryBlockBase(D_80112130[0x29]),
                  9, 0x20, 0x20, 0, actor->scale, 0);

    i = 0;
    do {
        drawMenuSpriteWithAlpha((s16)(actor->common.x - 4), (s16)(actor->common.y + i), getMemoryBlockBase(D_80112130[0x29]), 5, 0x20,
                      0x20, 0, actor->scale, 0);
        drawMenuSpriteWithAlpha((s16)(actor->common.x + 0xD4), (s16)(actor->common.y + i), getMemoryBlockBase(D_80112130[0x29]), 6, 0x20,
                      0x20, 0, actor->scale, 0);
        j = 0;
        do {
            drawMenuSpriteWithAlpha((s16)(actor->common.x + j), (s16)(actor->common.y + i), getMemoryBlockBase(D_80112130[0x29]), 0xB,
                          0x20, 0x20, 0, actor->scale, 0);
            j += 0x10;
            // IDO register allocation nudge for matching codegen.
            if (((!actor) && (!actor)) && (!actor)) {
            }
        } while (j != 0xE0);
        i += 0x10;
    } while (i < 0x30);

    if (actor->state == 8) {
        playerIndex = 0;
        playerY = 0;
        do {
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
            drawMenuGlyphScript((s16)(actor->common.x + 0x10), (s16)(actor->common.y - yAdjust + playerY),
                          (u8 *)playerNumberText, 0, alpha, 0);
            drawMenuGlyphScript((s16)(actor->common.x + 0x40), (s16)(actor->common.y - yAdjust + playerY), message, 0, alpha,
                          0);
            playerIndex++;
            playerY = playerY + 0x10;
        } while (playerIndex != 4);
    } else {
        drawMenuGlyphScript(actor->common.x, actor->common.y, &gControllerPakRumbleCheckPromptText[actor->targetScale * 0x68], 0, actor->scale, 0);
    }

    if (actor->state == 9) {
        alpha = 0x60;
        if (gControllerPakRumbleCheckPromptConfirmSelection == 0) {
            alpha = 0x100;
        }
        drawMenuSpriteWithAlpha((s16)(actor->common.x + 0x4C), (s16)(actor->common.y + 0x10), getMemoryBlockBase(D_80112130[0x21]),
                      0x17, 0x20, 0x20, 0, alpha, 0);
        if (alpha == 0x100) {
            alpha = 0x60;
        } else {
            alpha = 0x100;
        }
        drawMenuSpriteWithAlpha((s16)(actor->common.x + 0x4C), (s16)(actor->common.y + 0x20), getMemoryBlockBase(D_80112130[0x21]),
                      0x18, 0x20, 0x20, 0, alpha, 0);
        drawMenuSpriteWithAlpha((s16)(actor->common.x + 0x4C),
                      (s16)(actor->common.y + (gControllerPakRumbleCheckPromptConfirmSelection * 0x10) + 0x10),
                      getMemoryBlockBase(D_80112130[0x21]), 0x12, 0x20, 0x20, 0, actor->optionScale, 0);
        if (1) {
        }
    }

    if ((actor->state == 1) || (actor->state == 3) || (actor->state == 8)) {
        drawMenuSprite((s16)(actor->common.x + 0xD0), (s16)(actor->common.y + yAdjust + 0x20), getMemoryBlockBase(D_80112130[0x21]),
                      ((actor->timer >= 8) + 5) & 0xFFFF, 0x20, 0x20, 0, 0);
    }
}
#endif

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
    addRenderCallback(&gMenuRenderCallbackList, drawControllerPakRumbleCheckPrompt, (s32)arg0);
}

void initControllerPakRumbleCheckPrompt(ControllerPakRumbleCheckPromptActor *arg0) {
    arg0->common.x = -0x70;
    arg0->common.y = -0x1C;
    arg0->state = 6;
    arg0->scale = 0;
    arg0->timer = 0;
    arg0->targetScale = 0;
    setCallbackTaskCallback(arg0, updateControllerPakRumbleCheckPrompt);
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
    drawMenuSpriteWithAlpha(arg0->common.x, arg0->common.y, getMemoryBlockBase(gMenuCommonSpritesAssetHandle), 1, 0x20, 0x20, 0, drawAlpha, 0);

    if (drawAlpha == 0x100) {
        alpha = 0x80;
    } else {
        alpha = 0x100;
    }

    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x70), arg0->common.y, getMemoryBlockBase(gMenuCommonSpritesAssetHandle), 2, 0x20, 0x20, 0,
                  alpha, 0);
    drawMenuSpriteWithAlpha((s16)(arg0->common.x + (gControllerPakMenuState.mainChoice * 0x70)), arg0->common.y, getMemoryBlockBase(gMenuCommonSpritesAssetHandle),
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
    addRenderCallback(&gMenuRenderCallbackList, drawControllerPakFileDeleteMainOptions, (s32)arg0);
}

void initControllerPakFileDeleteMainOptions(ControllerPakOptionsActor *arg0) {
    arg0->common.x = -0x68;
    arg0->common.y = -0x44;
    arg0->scale = 0x100;
    arg0->timer = 0;
    setCallbackTaskCallback(arg0, updateControllerPakFileDeleteMainOptionsUi);
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

    drawMenuSpriteWithAlpha(arg0->common.x, arg0->common.y, getMemoryBlockBase(gMenuCommonSpritesAssetHandle), 3, 0x20, 0x20, 0, alpha, 0);
    drawMenuSpriteWithAlpha((s16)(actor->common.x + 0x70), actor->common.y, getMemoryBlockBase(gMenuCommonSpritesAssetHandle), 4, 0x20, 0x20, 0,
                  otherAlpha, 0);

    if (gControllerPakMenuCursorState == 2) {
        drawMenuSpriteWithAlpha((s16)(actor->common.x + (gControllerPakMenuConfirmChoice * 0x70)), actor->common.y, getMemoryBlockBase(gMenuCommonSpritesAssetHandle), 5, 0x20,
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
    addRenderCallback(&gMenuRenderCallbackList, drawControllerPakFileDeleteConfirmOptions, (s32)arg0);
}

void initControllerPakFileDeleteConfirmOptions(ControllerPakOptionsActor *arg0) {
    arg0->common.x = -0x68;
    arg0->common.y = 0x48;
    arg0->scale = 0x100;
    arg0->timer = 0;
    setCallbackTaskCallback(arg0, updateControllerPakFileDeleteConfirmOptionsUi);
}

// drawControllerPakFileDeleteFreeSpaceInfo best match: 85.330% (nonmatchings/drawControllerPakFileDeleteFreeSpaceInfo-6182772958467082306/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_ui/drawControllerPakFileDeleteFreeSpaceInfo.s")

#ifdef NON_MATCHING
void drawControllerPakFileDeleteFreeSpaceInfo(ControllerPakTwoPointActor *arg0) {
    char before[4];
    u16 text[4];
    char after[8];
    s32 value;
    u16 *digit;
    s32 remainder;

    digit = text;
    do {
        digit++;
        digit[-1] = 0xFFFE;
    } while (digit < text + 4);

    value = gControllerPakFreeFileCount;
    digit = &text[1];
    do {
        remainder = value % 10;
        value = value / 10;
        digit--;
        digit[1] = remainder;
    } while (value != 0);
    text[2] = 0xFFFF;
    drawMenuGlyphScript(arg0->common.x, arg0->common.y, (u8 *)text, 1, 0x100, 8);

    digit = text;
    do {
        digit++;
        digit[-1] = 0xFFFE;
    } while (digit < text + 4);

    value = gControllerPakFreeBytes / 0x100;
    digit = &text[2];
    do {
        remainder = value % 10;
        value = value / 10;
        digit--;
        digit[1] = remainder;
    } while (value != 0);
    text[3] = 0xFFFF;
    drawMenuGlyphScript(arg0->x2, arg0->y2, (u8 *)text, 1, 0x100, 8);
}
#endif

void updateControllerPakFileDeleteFreeSpaceInfo(s32 arg0) {
    addRenderCallback(&gMenuRenderCallbackList, drawControllerPakFileDeleteFreeSpaceInfo, arg0);
}

void initControllerPakFileDeleteFreeSpaceInfo(ControllerPakTwoPointActor *arg0) {
    s32 temp_v0 = 0x34;

    arg0->common.x = -0x24;
    arg0->common.y = temp_v0;
    arg0->x2 = 0x5C;
    arg0->y2 = temp_v0;
    setCallbackTaskCallback(arg0, updateControllerPakFileDeleteFreeSpaceInfo);
}

// drawControllerPakFileDeleteFileList best match: 90.458%
#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_ui/drawControllerPakFileDeleteFileList.s")

#ifdef NON_MATCHING
typedef struct {
    /* 0x00 */ u32 file_size;
    /* 0x04 */ u32 game_code;
    /* 0x08 */ u16 company_code;
    /* 0x0A */ char ext_name[4];
    /* 0x0E */ char game_name[16];
} ControllerPakPfsState;

extern ControllerPakPfsState gControllerPakFileStates[];
extern u8 gControllerPakVisibleFileIndex;
void drawControllerPakFileDeleteFileList(ControllerPakFileListActor *arg0) {
    ControllerPakPfsState *fileState;
    u16 textB0[4];
    u16 fileNameText[19];
    u16 text7C[5];
    u16 *digit;
    u16 *out;
    u8 *name;
    s32 value;
    s32 rowY;
    u16 fileIndex;
    s32 alpha;
    u8 state;
    s32 indexXOffset;
    s32 i;
    s32 insertIndex;
    s32 ch;
    s32 ten;

    if (gControllerPakMenuState.visibleFileIndex == 0) {
        fileIndex = gControllerPakMenuState.fileIndex;
    } else if (gControllerPakMenuState.visibleFileIndex == 4) {
        fileIndex = gControllerPakMenuState.fileIndex - 4;
    } else {
        fileIndex = gControllerPakMenuState.fileIndex - gControllerPakMenuState.visibleFileIndex;
    }

    ten = 10;
    rowY = 0;
    do {
        state = gControllerPakMenuState.state;
        alpha = 0xE0;
        indexXOffset = 0;
        if ((s32)state <= 0) {
        } else {
            alpha = 0xE0;
            if (gControllerPakMenuState.fileIndex == fileIndex) {
                alpha = 0x100;
            }
        }

        if (fileIndex < 9) {
            textB0[0] = 0xFFFE;
            indexXOffset = -4;
        } else {
            textB0[0] = 1;
        }
        textB0[2] = 0xFFFF;
        textB0[1] = (fileIndex + 1) % ten;
        drawMenuGlyphScript((s16)(arg0->positions[0].x + indexXOffset), (s16)(arg0->positions[0].y + rowY), (u8 *)textB0, 1, alpha, 8);

        fileState = &gControllerPakFileStates[fileIndex];
        i = gControllerPakVisibleFileIndex * 0;
        if (fileState->company_code != 0) {
            insertIndex = 0;
            name = fileState->game_name;
            out = fileNameText;
            do {
                ch = *name++;
                if ((ch == 0) && (insertIndex == 0)) {
                    insertIndex = i;
                }
                i++;
                if (ch < 0x10) {
                    *out = 0xFFFE;
                } else {
                    *out = ch - 0x10;
                }
                out++;
            } while (i < 0x10);

            ch = fileState->ext_name[0];
            fileNameText[16] = 0xFFFE;
            fileNameText[17] = 0xFFFE;
            if (ch != 0) {
                if (insertIndex == 0) {
                    insertIndex = 0x10;
                }
                fileNameText[insertIndex] = 0x2C;
                fileNameText[insertIndex + 1] = ch - 0x10;
            }
            fileNameText[18] = 0xFFFF;
            drawMenuGlyphScript(arg0->positions[1].x, (s16)(arg0->positions[1].y + rowY), (u8 *)fileNameText, 1, alpha, 8);

            out = text7C;
            i = 0x18;
            do {
                ch = (fileState->game_code >> i) & 0xFF;
                if ((ch < 0x30) || (ch >= 0x5B)) {
                    *out = 0xFFFE;
                } else if (ch >= 0x41) {
                    *out = ch - 0x37;
                } else if (ch >= 0x3A) {
                    *out = 0xFFFE;
                } else {
                    *out = ch - 0x30;
                }
                out++;
                i -= 8;
            } while (out < &text7C[4]);
            text7C[4] = 0xFFFF;
            drawMenuGlyphScript(arg0->positions[2].x, (s16)(arg0->positions[2].y + rowY), (u8 *)text7C, 1, alpha, 8);

            out = text7C;
            i = 8;
            do {
                ch = (fileState->company_code >> i) & 0xFF;
                if ((ch < 0x30) || (ch >= 0x5B)) {
                    *out = 0xFFFE;
                } else if (ch >= 0x41) {
                    *out = ch - 0x37;
                } else if (ch >= 0x3A) {
                    *out = 0xFFFE;
                } else {
                    *out = ch - 0x30;
                }
                i -= 8;
                out++;
            } while ((i ^ 0) >= -7);
            text7C[2] = 0xFFFF;
            drawMenuGlyphScript(arg0->positions[3].x, (s16)(arg0->positions[3].y + rowY), (u8 *)text7C, 1, alpha, 8);

            digit = textB0;
            do {
                digit++;
                digit[-1] = 0xFFFE;
            } while ((digit - 1) < (&textB0[3] - 1));

            value = fileState->file_size >> 8;
            digit = &textB0[2];
            do {
                digit--;
                digit[1] = value % ten;
                value = value / ten;
            } while (value != 0);
            textB0[3] = 0xFFFF;
            drawMenuGlyphScript(arg0->positions[4].x, (s16)(arg0->positions[4].y + rowY), (u8 *)textB0, 1, alpha, 8);
        }

        rowY += 0x10;
        fileIndex++;
    } while (rowY != 0x50);

    if (gControllerPakMenuCursorState != 0) {
        drawMenuSpriteWithAlpha(arg0->positions[5].x, (s16)(arg0->positions[5].y + (gControllerPakVisibleFileIndex * 0x10)), getMemoryBlockBase(D_80112130[0x21]), 6,
                      0x20, 0x20, 0, arg0->cursorScale, 0);
        drawMenuSpriteWithAlpha((s16)(arg0->positions[5].x + 0x80), (s16)(arg0->positions[5].y + (gControllerPakVisibleFileIndex * 0x10)),
                      getMemoryBlockBase(D_80112130[0x21]), 7, 0x20, 0x20, 0, arg0->cursorScale, 0);
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
    addRenderCallback(&gMenuRenderCallbackList, drawControllerPakFileDeleteFileList, (s32)arg0);
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
    setCallbackTaskCallback(arg0, updateControllerPakFileDeleteFileListUi);
}

void drawControllerPakFileDeleteIcon(ControllerPakSpriteActor *arg0) {
    func_80011D74((MenuRenderSprite *)&arg0->sprite, 0, arg0->common.x, arg0->common.y);
}

void updateControllerPakFileDeleteIcon(s32 arg0) {
    addRenderCallback(&D_80124838, drawControllerPakFileDeleteIcon, arg0);
}

void initControllerPakFileDeleteIcon(ControllerPakSpriteActor *arg0) {
    func_80017168((DstStruct_80017168 *)&arg0->sprite, getMemoryBlockBase(D_8011217C));
    arg0->sprite.x = 0x10;
    arg0->sprite.y = 0x10;
    arg0->common.x = arg0->startX;
    arg0->common.y = arg0->startY;
    setCallbackTaskCallback(arg0, updateControllerPakFileDeleteIcon);
}

void drawControllerPakMessageIcon(ControllerPakSpriteActor *arg0) {
    func_80011D74((MenuRenderSprite *)&arg0->sprite, 1, arg0->common.x, arg0->common.y);
}

void updateControllerPakMessageIcon(s32 arg0) {
    addRenderCallback(&D_80124838, drawControllerPakMessageIcon, arg0);
}

void initControllerPakMessageIcon(ControllerPakSpriteActor *arg0) {
    func_80017168((DstStruct_80017168 *)&arg0->sprite, getMemoryBlockBase(D_8011217C));
    arg0->sprite.x = 0x10;
    arg0->sprite.y = 0x20;
    arg0->common.x = arg0->startX;
    arg0->common.y = arg0->startY;
    setCallbackTaskCallback(arg0, updateControllerPakMessageIcon);
}

void drawControllerPakFileDeleteErrorPrompt(ControllerPakWindowActor *arg0) {
    drawMenuSpriteWithAlpha(arg0->common.x, arg0->common.y, getMemoryBlockBase(D_80112178), 0, 0x20, 0x20, 0, arg0->scale, 0);
    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x40), arg0->common.y, getMemoryBlockBase(D_80112178), 1, 0x20, 0x20, 0, arg0->scale, 0);
    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x78), arg0->common.y, getMemoryBlockBase(D_80112178), 1, 0x20, 0x20, 0, arg0->scale, 0);
    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0xB0), arg0->common.y, getMemoryBlockBase(D_80112178), 2, 0x20, 0x20, 0, arg0->scale, 0);
    drawMenuGlyphScript((s16)(arg0->common.x + 4), (s16)(arg0->common.y + 4), gControllerPakCouldNotEraseNoteText, 0, arg0->scale, 0);
    if (arg0->selectedOption == 1) {
        drawMenuSprite((s16)(arg0->common.x + 0xD4), (s16)(arg0->common.y + 0x24), getMemoryBlockBase(D_80112178),
                      ((arg0->timer >= 8) + 5) & 0xFFFF, 0x20, 0x20, 0, 0);
    }
}

// updateControllerPakFileDeleteErrorPromptUi best match: 84.757% (nonmatchings/updateControllerPakFileDeleteErrorPromptUi-4839787584499344943/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_ui/updateControllerPakFileDeleteErrorPromptUi.s")

#ifdef NON_MATCHING
void updateControllerPakFileDeleteErrorPromptUi(ControllerPakDeletePromptActor *arg0) {
    u8 globalState;
    u8 state;
    u8 switchState;

    globalState = gControllerPakDeletePromptState;
    state = arg0->timer;
    switchState = state;
    if (state != globalState) {
        state = globalState;
        switchState = globalState;
        arg0->timer = globalState;
    }

    switch (switchState) {
    case 0:
        arg0->scale = (s16)arg0->scale + 0x28;
        if ((s16)arg0->scale >= 0x100) {
            arg0->scale = 0x100;
            arg0->timer = 1;
        }
        state = arg0->timer;
        break;
    case 1:
        state = arg0->timer;
        arg0->selectedOption = (arg0->selectedOption + 1) & 0xF;
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

    gControllerPakDeletePromptState = state;
    if (arg0->timer == 3) {
        removeCallbackTask((CallbackTask *)arg0);
        return;
    }

    addRenderCallback(&gMenuRenderCallbackList, drawControllerPakFileDeleteErrorPrompt, (s32)arg0);
}
#endif

void initControllerPakFileDeleteErrorPrompt(ControllerPakDeletePromptActor *arg0) {
    arg0->common.x = -0x70;
    arg0->common.y = -0x1C;
    arg0->timer = 0;
    arg0->scale = 0;
    arg0->selectedOption = 0;
    setCallbackTaskCallback(arg0, updateControllerPakFileDeleteErrorPromptUi);
}

void drawControllerPakDeleteConfirmPrompt(ControllerPakDeletePromptActor *arg0) {
    s32 i;
    s32 j;
    s32 alpha;
    s32 limit;

    drawMenuSprite(arg0->common.x, arg0->common.y, getMemoryBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 2, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->common.x + 0x90), arg0->common.y, getMemoryBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 4, 0x20, 0x20, 0, 0);

    i = 0;
    do {
        drawMenuSprite((s16)(arg0->common.x + i + 0x10), arg0->common.y, getMemoryBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 3, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)(arg0->common.x + i + 0x10), (s16)(arg0->common.y + 0x30), getMemoryBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 8, 0x20, 0x20, 0, 0);
        i += 0x10;
    } while (i < 0x80);

    drawMenuSprite(arg0->common.x, (s16)(arg0->common.y + 0x30), getMemoryBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 7, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->common.x + 0x90), (s16)(arg0->common.y + 0x30), getMemoryBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 9, 0x20, 0x20, 0, 0);

    i = 0;
    limit = 0x80;
loop_outer:
    drawMenuSprite(arg0->common.x, (s16)(arg0->common.y + i + 0x10), getMemoryBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 5, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->common.x + 0x90), (s16)(arg0->common.y + i + 0x10), getMemoryBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 6, 0x20, 0x20, 0, 0);

    j = 0;
    do {
        drawMenuSprite((s16)(arg0->common.x + j + 0x10), (s16)(arg0->common.y + i + 0x10), getMemoryBlockBase(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 0xB, 0x20, 0x20, 0, 0);
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

    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x2C), (s16)(arg0->common.y + 0x18), getMemoryBlockBase(D_80112130[0x24]), 0x17, 0x20, 0x20, 0, alpha, 0);

    if (alpha == 0x100) {
        alpha = 0x60;
    } else {
        alpha = 0x100;
    }

    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x2C), (s16)(arg0->common.y + 0x28), getMemoryBlockBase(D_80112130[0x24]), 0x18, 0x20, 0x20, 0, alpha, 0);
    drawMenuSpriteWithAlpha((s16)(arg0->common.x + 0x2C), (s16)(arg0->common.y + (arg0->selectedOption * 0x10) + 0x18), getMemoryBlockBase(D_80112130[0x24]), 0x12, 0x20, 0x20, 0, arg0->scale, 0);
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
        addRenderCallback(&gMenuRenderCallbackList, drawControllerPakDeleteConfirmPrompt, (s32)arg0);
    }
}

void initControllerPakDeleteConfirmPrompt(ControllerPakDeletePromptActor *arg0) {
    arg0->common.x = -0x54;
    arg0->common.y = -0x20;
    arg0->scale = 0x100;
    arg0->selectedOption = 1;
    arg0->timer = 0;
    setCallbackTaskCallback(arg0, updateControllerPakDeleteConfirmPrompt);
}
