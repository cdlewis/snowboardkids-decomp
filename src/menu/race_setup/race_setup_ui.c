#include "common.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#define MENU_RENDERER_BROAD_PROTOTYPES
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/race_setup/race_setup_ui.h"

#define TITLE_MENU_SECONDARY_TEXTURE_HANDLE (gAssetHandles[31])
#define TITLE_MENU_FRAME_TEXTURE_HANDLE (gAssetHandles[33])
#define TITLE_MENU_BANNER_TEXTURE_HANDLE (gAssetHandles[41])
#define RACE_PLAYER_STATE_SIZE 0x60C
#define CONTROLLER_PAK_STATUS_UNUSED 0x13
#define SAVE_PANEL_MAX_RECORD_ICONS 3
#define SAVE_PANEL_BASE_BADGE_COUNT 6
#define SAVE_PANEL_EMPTY_BADGE_TILE 9
#define SAVE_PANEL_RECORD_ICON_TILE 0x19
#define SAVE_STATUS_FADE_STEP 0x26
#define SAVE_STATUS_TRANSITION_NONE 0
#define SAVE_STATUS_TRANSITION_FADE_IN 1
#define SAVE_STATUS_TRANSITION_FADE_OUT 2
#define SAVE_STATUS_TRANSITION_DONE 3

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
} Struct801235B8;

typedef struct {
    /* 0x00 */ u8 state;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ s16 alpha;
    /* 0x04 */ u8 savePanelFrameState;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ u16 selection[4];
    /* 0x0E */ u8 saveStatusTransitionStates[4];
    /* 0x12 */ u16 nextSelection[4];
} TitleIntroTransitionState;

typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ u8 badgeIds[0x18];
    /* 0x4C */ u8 iconCount;
    /* 0x4D */ u8 pad4D[0x78F8 - 0x4D];
} TitleMenuPlayerView;

typedef struct {
    /* 0x000 */ s32 money;
    /* 0x004 */ u8 pad4[RACE_PLAYER_STATE_SIZE - 4];
} RaceSetupPlayerMoneyView;

typedef struct {
    u8 value;
} TitleMenuIconStripPlayerStride;

extern void addRenderCallback(void *, void *, s32);
extern s32 enqueueSoundEffect(s32, s32);
extern u8 gRaceSetupSaveStatusMessages[][0x4C];
extern u8 gRaceSetupNoControllerPakMessage[];
#ifdef NON_MATCHING
extern u8 D_800E0A80[];
extern u8 D_800E0A84[];
extern s32 gPlayer1Money;
#endif
extern MenuIntroActor *gActiveMenuTask;
extern TitleMenuWidgetActor *D_8010ADE0;
extern s16 gControllerPakStatusCodes[];
extern s16 gMenuChoicePromptState[];
extern s16 gPlayerBadgeDisplayOrder[];
extern TitleMenuPlayerView gGameSaveDataBuffer[];
extern TitleIntroTransitionState gRaceSetupMenuSubState;
extern s16 gRaceSetupSavePanelRect0X0;
extern s16 gRaceSetupSavePanelRect0Y0;
extern s16 gRaceSetupSavePanelRect0X1;
extern s16 gRaceSetupSavePanelRect0Y1;
extern s16 gRaceSetupSavePanelRect1X0;
extern s16 gRaceSetupSavePanelRect1Y0;
extern s16 gRaceSetupSavePanelRect1X1;
extern s16 gRaceSetupSavePanelRect1Y1;
extern u16 gRaceSetupOnePlayerOptionText[];
extern u16 gRaceSetupTwoPlayerOptionText[];
extern u16 gRaceSetupThreePlayerOptionText[];
extern u16 gRaceSetupFourPlayerOptionText[];
extern u8 gRaceSetupPlayerCountPromptText[];
extern s16 gAssetHandles[];
extern u8 gPlayerCount;
extern u8 gRaceSetupSaveChoicePromptBottomSprites[];
extern u8 gRaceSetupSaveChoicePromptTopSprites[];
extern u8 gMenuSelectionConfirmTimer;
extern s32 gMenuFlowState;
extern Struct801235B8 *gCurrentGameTask;
extern s32 gMenuOverlayRenderCallbackList;
extern s32 gMenuRenderCallbackList;

void drawRaceSetupPlayerCountPrompt(MenuIntroActor *arg0) {
    s32 i;
    s32 j;
    s32 selected;
    s32 limit;
    MenuIntroActor *actor;

    actor = arg0;
    drawMenuSpriteWithAlpha((s16)(actor->x - 4), (s16)(actor->y - 4), getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE), 2,
                  0x20, 0x20, 0, actor->alpha, 0);
    drawMenuSpriteWithAlpha((s16)(actor->x + 0xD4), (s16)(actor->y - 4), getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE), 4,
                  0x20, 0x20, 0, actor->alpha, 0);
    i = 0;
    do {
        drawMenuSpriteWithAlpha((s16)(actor->x + i), (s16)(actor->y - 4), getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE),
                      3, 0x20, 0x20, 0, actor->alpha, 0);
        drawMenuSpriteWithAlpha((s16)(actor->x + i), (s16)(actor->y + 0x24), getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE),
                      8, 0x20, 0x20, 0, actor->alpha, 0);
        i += 0x10;
    } while (i < 0xE0);
    drawMenuSpriteWithAlpha((s16)(actor->x - 4), (s16)(actor->y + 0x24), getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE),
                  7, 0x20, 0x20, 0, actor->alpha, 0);
    drawMenuSpriteWithAlpha((s16)(actor->x + 0xD4), (s16)(actor->y + 0x24),
                  getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, actor->alpha, 0);
    i = (actor->state == 4) * 0;
    limit = 0xE0;
    do {
        drawMenuSpriteWithAlpha((s16)(actor->x - 4), (s16)(actor->y + i), getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE),
                      5, 0x20, 0x20, 0, actor->alpha, 0);
        drawMenuSpriteWithAlpha((s16)(actor->x + 0xD4), (s16)(actor->y + i), getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE),
                      6, 0x20, 0x20, 0, actor->alpha, 0);
        j = 0;
        do {
            drawMenuSpriteWithAlpha((s16)(actor->x + j), (s16)(actor->y + i),
                          getRelocatableHeapBlockBase(TITLE_MENU_BANNER_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, actor->alpha, 0);
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
    drawMenuGlyphScript(actor->x, actor->y, &gRaceSetupPlayerCountPromptText[selected * 0x68], 0, actor->alpha, 0);

    if ((actor->state == 1) || (actor->state == 6)) {
        drawMenuSprite((s16)(actor->x + 0xD0), (s16)(actor->y + 0x20),
                      getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), ((actor->timer >= 8) + 5) & 0xFFFF,
                      0x20, 0x20, 0, 0);
    }
}

void updateRaceSetupPlayerCountPrompt(MenuIntroActor *arg0) {
    TitleIntroTransitionState *global;
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
                actor->child = createCallbackTask(initRaceSetupOnePlayerOption, 0, 0x63);
                enqueueSoundEffect(1, 0x32);
                actor->state = 3;
            }
            alpha = actor->alpha;
            break;
        case 3:
            if (gCurrentGameTask->unk1C == 2) {
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
        addRenderCallback(&gMenuRenderCallbackList, drawRaceSetupPlayerCountPrompt, (s32)actor);
    }
}

void initRaceSetupPlayerCountPrompt(void *arg0) {
    MenuIntroActor *actor = arg0;

    actor->x = -0x70;
    actor->y = -0x1C;
    actor->alpha = 0;
    actor->state = 0;
    setCallbackTaskCallback(arg0, updateRaceSetupPlayerCountPrompt);
}

void drawRaceSetupOnePlayerOption(void *arg0) {
    MenuItemActor *actor = arg0;
    s16 unused;
    s16 width;
    s16 xOffset;
    s16 yOffset;
    s32 alpha;
    s32 temp;

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
        drawMenuSpriteWithAlpha(actor->x, actor->y, getRelocatableHeapBlockBase(gAssetHandles[33]), 3, 0x20, 0x20, 0, alpha, 0);
        if (width == 0x20) {
            temp = width & 0xFFFF;
            drawMenuSpriteWithAlpha((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), getRelocatableHeapBlockBase(gAssetHandles[31]), 0, temp, temp, 0, alpha, 0);
        } else {
            drawMenuSpriteWithPaletteScale((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), getRelocatableHeapBlockBase(gAssetHandles[31]), 0, alpha);
        }
        drawMenuGlyphScriptDefaultFont((s16)(actor->x + 0x30), (s16)(actor->y + 4), gRaceSetupOnePlayerOptionText, 1, alpha);
    }

    drawMenuSpriteWithAlpha((s16)(actor->x + 0x80), actor->y, getRelocatableHeapBlockBase(gAssetHandles[33]), 7, 0x20, 0x20, 0, alpha, 0);
}

void updateRaceSetupOnePlayerOption(MenuItemActor *arg0) {
    MenuItemActor *child;

    switch (arg0->state) {
    case 0:
        arg0->x -= 0x20;
        if (arg0->x == 0x70) {
            arg0->child = createCallbackTask(initRaceSetupTwoPlayerOption, 0, 0x63);
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
    addRenderCallback(&gMenuRenderCallbackList, drawRaceSetupOnePlayerOption, (s32)arg0);
}

void initRaceSetupOnePlayerOption(MenuItemActor *arg0) {
    MenuItemActor *actor = arg0;

    actor->x = 0x90;
    actor->y = -0x18;
    actor->state = 0;
    setCallbackTaskCallback(arg0, updateRaceSetupOnePlayerOption);
}

void drawRaceSetupTwoPlayerOption(MenuItemActor *arg0) {
    MenuItemActor *actor = arg0;
    s16 unused;
    s16 width;
    s16 xOffset;
    s16 yOffset;
    s32 alpha;
    s32 temp;
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
        drawMenuSpriteWithAlpha(actor->x, actor->y, getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, alpha, 2);
        if (width == 0x20) {
            temp = width & 0xFFFF;
            drawMenuSpriteWithAlpha((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 1, temp, temp, 0, alpha, 0);
        } else {
            drawMenuSpriteWithPaletteScale((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 1, alpha);
        }
        drawMenuGlyphScriptDefaultFont((s16)(actor->x + 0x30), (s16)(actor->y + 4), gRaceSetupTwoPlayerOptionText, 1, alpha);
    }

    for (i = 0; i != 0x28; i += 0x14) {
        drawMenuSpriteWithAlpha((s16)(actor->x + i + 0x80), actor->y, getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, alpha, 0);
    }
}

void updateRaceSetupTwoPlayerOption(MenuItemActor *arg0) {
    MenuItemActor *child;

    switch (arg0->state) {
    case 0:
        arg0->x -= 0x20;
        if (arg0->x == 0x70) {
            arg0->child = createCallbackTask(initRaceSetupThreePlayerOption, 0, 0x63);
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
    addRenderCallback(&gMenuRenderCallbackList, drawRaceSetupTwoPlayerOption, (s32)arg0);
}

void initRaceSetupTwoPlayerOption(MenuItemActor *arg0) {
    MenuItemActor *actor = arg0;

    actor->x = 0x90;
    actor->y = 8;
    actor->state = 0;
    setCallbackTaskCallback(arg0, updateRaceSetupTwoPlayerOption);
}

void drawRaceSetupThreePlayerOption(void *arg0) {
    MenuItemActor *actor = arg0;
    s16 unused;
    s16 width;
    s16 xOffset;
    s16 yOffset;
    s32 alpha;
    s32 temp;
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
        drawMenuSpriteWithAlpha(actor->x, actor->y, getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, alpha, 3);
        if (width == 0x20) {
            temp = width & 0xFFFF;
            drawMenuSpriteWithAlpha((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 2, temp, temp, 0, alpha, 0);
        } else {
            drawMenuSpriteWithPaletteScale((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 2, alpha);
        }
        drawMenuGlyphScriptDefaultFont((s16)(actor->x + 0x30), (s16)(actor->y + 4), gRaceSetupThreePlayerOptionText, 1, alpha);
    }

    for (i = 0; i != 0x3C; i += 0x14) {
        drawMenuSpriteWithAlpha((s16)(actor->x + i + 0x80), actor->y, getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, alpha, 0);
    }
}

void updateRaceSetupThreePlayerOption(MenuItemActor *arg0) {
    MenuItemActor *child;

    switch (arg0->state) {
    case 0:
        arg0->x -= 0x20;
        if (arg0->x == 0x70) {
            arg0->child = createCallbackTask(initRaceSetupFourPlayerOption, 0, 0x63);
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
    addRenderCallback(&gMenuRenderCallbackList, drawRaceSetupThreePlayerOption, (s32)arg0);
}

void initRaceSetupThreePlayerOption(MenuItemActor *arg0) {
    MenuItemActor *actor = arg0;

    actor->x = 0x90;
    actor->y = 0x28;
    actor->state = 0;
    setCallbackTaskCallback(arg0, updateRaceSetupThreePlayerOption);
}

void drawRaceSetupFourPlayerOption(void *arg0) {
    MenuItemActor *actor = arg0;
    s16 unused;
    s16 width;
    s16 xOffset;
    s16 yOffset;
    s32 alpha;
    s32 temp;
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
        drawMenuSpriteWithAlpha(actor->x, actor->y, getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, alpha, 4);
        if (width == 0x20) {
            temp = width & 0xFFFF;
            drawMenuSpriteWithAlpha((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 3, temp, temp, 0, alpha, 0);
        } else {
            drawMenuSpriteWithPaletteScale((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 3, alpha);
        }
        drawMenuGlyphScriptDefaultFont((s16)(actor->x + 0x30), (s16)(actor->y + 4), gRaceSetupFourPlayerOptionText, 1, alpha);
    }

    for (i = 0; i != 0x50; i += 0x14) {
        drawMenuSpriteWithAlpha((s16)(actor->x + i + 0x80), actor->y, getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, alpha, 0);
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
            createCallbackTask(initRaceSetupPlayerCountCursor, 0, 0x63);
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
        gCurrentGameTask->unk1C = 2;
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawRaceSetupFourPlayerOption, (s32)actor);
}

void initRaceSetupFourPlayerOption(MenuItemActor *arg0) {
    MenuItemActor *actor = arg0;

    actor->x = 0x90;
    actor->y = 0x48;
    actor->state = 0;
    setCallbackTaskCallback(arg0, updateRaceSetupFourPlayerOption);
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
        (u16)actor->alpha,
        0);
}

void updateRaceSetupPlayerCountCursor(FadeItemActor *arg0) {
    FadeItemActor *actor = arg0;
    u16 temp_v0;
    s16 temp_t6;
    FadeItemActor *temp_a2;

    temp_v0 = actor->alpha;
    temp_a2 = actor;
    if ((((((((((temp_v0 & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) != 0x100) {
        temp_t6 = temp_v0 + 0x28;
        actor->alpha = temp_t6;
        if ((temp_t6 & 0xFFFF) >= 0x100) {
            actor->alpha = 0x100;
            gCurrentGameTask->unk1C = 1;
        }
    } else if (gMenuFlowState != 0) {
        temp_a2->x = temp_a2->x - 0x20;
    }
    if (temp_a2->x < -0x90) {
        removeCallbackTask(temp_a2);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, drawRaceSetupPlayerCountCursor, (s32)temp_a2);
    }
}

void initRaceSetupPlayerCountCursor(FadeItemActor *arg0) {
    FadeItemActor *actor = arg0;

    actor->x = -0x68;
    actor->y = -0x14;
    actor->alpha = 0;
    setCallbackTaskCallback(arg0, updateRaceSetupPlayerCountCursor);
}

// drawRaceSetupSavePlayerPanels best match: 99.297% (nonmatchings/drawRaceSetupSavePlayerPanels-5755426475870421788/base_16.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/race_setup/race_setup_ui/drawRaceSetupSavePlayerPanels.s")

#ifdef NON_MATCHING
void drawRaceSetupSavePlayerPanels(TitleMenuWidgetItemView *arg0) {
    char textBuffer[4];
    s32 alpha;
    s32 playerNumber;
    s32 playerIndex;
    s32 texture;
    s32 palette;
    TitleMenuWidgetItemView *panel;
    volatile u8 *playerCount;
    u8 *text;

    playerCount = &gPlayerCount; panel = arg0; text = textBuffer - 0x18; for (playerIndex = 0; playerIndex != 4;) { alpha = (((s32)*playerCount - 1) < playerIndex) ? 0x50 : 0x100; drawMenuSpriteWithAlpha(panel->x, panel->y, getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 0xA, 0x20, 0x20, 0, alpha, palette = (playerNumber = playerIndex + 1) & 0xFF);
        if ((playerIndex && playerIndex) && playerIndex) {
        }
        playerNumber = TITLE_MENU_FRAME_TEXTURE_HANDLE;
        texture = getRelocatableHeapBlockBase(playerNumber);
        playerNumber = playerIndex + 1;
        drawMenuSpriteWithAlpha((s16)(panel->x + 0x40), panel->y, texture, 0xB, 0x20, 0x20, 0, alpha, palette);
        drawMenuSpriteWithAlpha((s16)(panel->x + 0x80), panel->y, getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 0xC, 0x20, 0x20, 0, alpha, palette);
        sprintf(text, D_800E0A80, playerNumber);
        drawMenuAsciiText((s16)(panel->x + 0x32), (s16)(panel->y + 2), text, 0, alpha);
        if (alpha == 0x100) {
            sprintf(text, D_800E0A84, ((RaceSetupPlayerMoneyView *)&gPlayer1Money)[playerIndex].money);
            drawMenuAsciiText((s16)(panel->x + 0x44), (s16)(panel->y + 0x1B), text, 0, alpha);
        } else {
            drawMenuSpriteWithAlpha((s16)(panel->x + 2), (s16)(panel->y + 0x14), getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 0x90, 0x20, 0x20, 0, 0xF0, 0);
        }
        playerIndex = playerNumber;
        panel = (TitleMenuWidgetItemView *)((s16 *)panel + 1);
    }
}
#endif

// updateRaceSetupSavePanelFrame best match: 99.944% (nonmatchings/updateRaceSetupSavePanelFrame-2/base.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/race_setup/race_setup_ui/updateRaceSetupSavePanelFrame.s")

#ifdef NON_MATCHING
void updateRaceSetupSavePanelFrame(RectListActor *arg0) {
    u8 state;
    s32 outer;
    s32 i;
    s16 delta0;
    s16 delta1;
    s16 *coords;

    outer = gRaceSetupMenuSubState.savePanelFrameState;
    state = arg0->frame;
    if (gRaceSetupMenuSubState.savePanelFrameState != state) {
        state = outer;
        arg0->frame = outer;
    }

    switch (state) {
    case 0:
        outer = 0;
        do {
            i = 0;
            coords = &arg0->rects[0].x0;
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

                (&arg0->rects[0].x0)[i] += delta0;

                if (i == 0) {
                    arg0->stepAccumulator += arg0->stepIncrement;
                }
                if (arg0->stepAccumulator >= arg0->stepLimit) {
                    (&arg0->rects[0].x0)[i + 4] += delta1;
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

                (&arg0->rects[0].x0)[i + 1] += delta0;

                if (i == -1) {
                    arg0->stepAccumulator += arg0->stepIncrement;
                }
                if (arg0->stepAccumulator >= arg0->stepLimit) {
                    (&arg0->rects[0].x0)[i + 5] += delta1;
                    if (i == 2) {
                        arg0->stepAccumulator -= arg0->stepLimit;
                    }
                }

                i += 2;
                coords += 2;
            } while (i != 4);

            outer++;
            if (arg0->rects[(arg0->rects[0].x0 == -0x88) * 0].x0 == -0x88) {
                arg0->frame = 1;
                break;
            }
            if (outer == 0x10) {
                break;
            }
        } while (1);
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

    gRaceSetupMenuSubState.savePanelFrameState = state;
    gRaceSetupSavePanelRect0X0 = arg0->rects[0].x0;
    gRaceSetupSavePanelRect1X0 = arg0->rects[1].x0;
    gRaceSetupSavePanelRect0Y0 = arg0->rects[0].y0;
    gRaceSetupSavePanelRect1Y0 = arg0->rects[1].y0;
    gRaceSetupSavePanelRect0X1 = arg0->rects[0].x1;
    gRaceSetupSavePanelRect1X1 = arg0->rects[1].x1;
    gRaceSetupSavePanelRect0Y1 = arg0->rects[0].y1;
    gRaceSetupSavePanelRect1Y1 = arg0->rects[1].y1;

    if (gMenuFlowState == 0x63) {
        removeCallbackTask(arg0);
        gMenuFlowState = 0;
        return;
    }

    addRenderCallback(&gMenuRenderCallbackList, drawRaceSetupSavePlayerPanels, (s32)arg0);
}
#endif

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
    setCallbackTaskCallback(arg0, updateRaceSetupSavePanelFrame);
}

void drawRaceSetupSavePanelIcons(TitleMenuIconStripActor *arg0) {
    volatile s32 unused;
    union {
        s32 plain;
        volatile s32 observed;
    } i;
    s32 pad[6];
    s32 actorOffset;
    s16 *courseId;
    register s16 *new_var;
    register TitleMenuPlayerView *player;
    register TitleMenuIconStripActor *item;
    register s32 iconXOffset;
    register s32 alpha;
    register s32 nextJ;
    register s16 *badgeIndex;
    register TitleMenuPlayerView *player2;
    s32 j;
    s32 count;
    s32 tile;

    do {
        i.plain = 0;
        if ((s32)gPlayerCount > 0) {
            courseId = gControllerPakStatusCodes;
            actorOffset = 0;
            do {
                if (*courseId != 0x13) {
                    player = &gGameSaveDataBuffer[i.plain];
                    j = 0;
                    if (player->iconCount == 3) {
                        count = 3;
                    } else {
                        count = player->iconCount + 1;
                    }
                    if (count > 0) {
                        iconXOffset = 0;
                        item = (TitleMenuIconStripActor *)&((TitleMenuIconStripPlayerStride *)arg0)[actorOffset];
                        do {
                            nextJ = j + 1;
                            if ((s32)player->iconCount < nextJ) {
                                alpha = 0x70;
                            } else {
                                alpha = 0x100;
                            }
                            drawMenuSpriteWithAlpha((s16)(item->panelX[0] + arg0->iconOffsetX + iconXOffset),
                                                    (s16)(item->panelY[0] + arg0->iconOffsetY),
                                                    getRelocatableHeapBlockBase(gAssetHandles[33]), 0x19,
                                                    0x20, 0x20, 0, alpha, 9 - j);
                            j = nextJ;
                            iconXOffset += 0x10;
                        } while (nextJ != count);
                        j = 0;
                    }

                    count = 6;
                    if (player->iconCount == 1) {
                        count = 7;
                    } else if (player->iconCount == 2) {
                        count = 8;
                    } else if (player->iconCount == 3) {
                        count = 9;
                    }

                    if (count > 0) {
                        player2 = &gGameSaveDataBuffer[i.observed]; new_var = gPlayerBadgeDisplayOrder; badgeIndex = new_var; nextJ = 0;
                        item = (TitleMenuIconStripActor *)&((TitleMenuIconStripPlayerStride *)arg0)[actorOffset];
                        do {
                            tile = player2->badgeIds[*badgeIndex];
                            alpha = 0x70;
                            if (tile != 0) {
                                alpha = 0x100;
                                tile += 6;
                            } else {
                                tile = 9;
                            }
                            drawMenuSpriteWithAlpha((s16)(item->panelX[0] + arg0->badgeOffsetX + nextJ),
                                                    (s16)(item->panelY[0] + arg0->badgeOffsetY),
                                                    getRelocatableHeapBlockBase(gAssetHandles[33]),
                                                    (j + 0x1A) & 0xFFFF, 0x20, 0x20, 0, alpha, tile);
                            j++;
                            nextJ += 0xE;
                            badgeIndex++;
                        } while (j != count);
                    }
                }
                i.plain++; actorOffset += 2; courseId++;
            } while ((i.plain < (s32)gPlayerCount));
        }
    } while (0);
}

void updateRaceSetupSavePanelIcons(void *arg0) {
    RectListActor *temp_a2;
    RectListActor *actor = arg0;

    actor->rects[0].x0 = gRaceSetupSavePanelRect0X0;
    actor->rects[1].x0 = gRaceSetupSavePanelRect1X0;
    actor->rects[0].y0 = gRaceSetupSavePanelRect0Y0;
    actor->rects[1].y0 = gRaceSetupSavePanelRect1Y0;
    actor->rects[0].x1 = gRaceSetupSavePanelRect0X1;
    actor->rects[1].x1 = gRaceSetupSavePanelRect1X1;
    actor->rects[0].y1 = gRaceSetupSavePanelRect0Y1;
    temp_a2 = actor;
    actor->rects[1].y1 = gRaceSetupSavePanelRect1Y1;
    addRenderCallback(&gMenuRenderCallbackList, drawRaceSetupSavePanelIcons, (s32)temp_a2);
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
    setCallbackTaskCallback(arg0, updateRaceSetupSavePanelIcons);
}

void drawRaceSetupSaveStatusWidgets(TitleMenuWidgetActor *arg0) {
    volatile s32 unused;
    MenuIntroActor *intro;
    register TitleMenuWidgetActor *actor;
    u8 *text;
    u32 tile;
    s32 i;
    s16 state;
    s32 alpha;

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
            drawMenuSpriteWithAlpha((s16)(actor->x[i] - 2), (s16)(actor->y[i] + 0xC), getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), tile,
                          0x20, 0x20, 0, alpha, 0);
            drawMenuSpriteWithAlpha((s16)(actor->x[i] + 0x3E), (s16)(actor->y[i] + 0xC), getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 9,
                          0x20, 0x20, 0, alpha, 0);

            state = gControllerPakStatusCodes[i];
            if (state == 0xA) {
                drawMenuAsciiText((s16)(actor->x[i] + 2), (s16)(actor->y[i] + 0x10), gRaceSetupNoControllerPakMessage, 7, actor->alpha[i]);
            } else {
                text = gRaceSetupSaveStatusMessages[state];
                state = 2;
                drawMenuGlyphScript((s16)(actor->x[i] + state), (s16)(actor->y[i] + 0x10), text, 1, actor->alpha[i], 0);
            }

            if ((actor->alpha[i] == 0x100) && (intro->state == 8)) {
                state = gControllerPakStatusCodes[i];
                if ((state == 4) || ((state >= 7) && (state != 0x12) && (gMenuChoicePromptState[i] == 0))) {
                    drawMenuSprite((s16)(actor->x[i] + 0x70), (s16)(actor->y[i] + 0x20), getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
                                  (((s32)actor->frame >= 8) + 5) & 0xFFFF, 0x20, 0x20, 0, 0);
                }
            }
        }
    }
}

// updateRaceSetupSaveStatusWidgets best match: 99.885% (nonmatchings/updateRaceSetupSaveStatusWidgets-8699393380584516020/base_18.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/race_setup/race_setup_ui/updateRaceSetupSaveStatusWidgets.s")

#ifdef NON_MATCHING
void updateRaceSetupSaveStatusWidgets(TitleMenuWidgetActor *arg0) {
    s32 transitionStateSum;
    register TitleMenuWidgetActor *actor;
    s32 playerIndex;
    TitleMenuWidgetActor *panelPositions;
    u8 transitionState;
    u8 previousTransitionState;

    panelPositions = D_8010ADE0;
    actor = arg0;
    for (playerIndex = 0; playerIndex < (s32) gPlayerCount; playerIndex++) {
        actor->x[playerIndex] = panelPositions->x[playerIndex];
        actor->y[playerIndex] = panelPositions->y[playerIndex];

        previousTransitionState = actor->statusTransitionStates[playerIndex];
        transitionState = gRaceSetupMenuSubState.saveStatusTransitionStates[playerIndex];
        if (transitionState != previousTransitionState) {
            actor->statusTransitionStates[playerIndex] = transitionState;
            actor->nextStatusCodes[playerIndex] = gRaceSetupMenuSubState.nextSelection[playerIndex];
            previousTransitionState = actor->statusTransitionStates[playerIndex];
            transitionState = previousTransitionState;
        }

        if (1) {}
        if (1) {}
        switch (transitionState) {
        case SAVE_STATUS_TRANSITION_FADE_IN:
            actor->alpha[playerIndex] += SAVE_STATUS_FADE_STEP;
            if (actor->alpha[playerIndex] >= 0x100) {
                actor->alpha[playerIndex] = 0x100;
                actor->statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_NONE;
            }
            transitionState = actor->statusTransitionStates[playerIndex];
            break;
        case SAVE_STATUS_TRANSITION_FADE_OUT:
            actor->alpha[playerIndex] -= SAVE_STATUS_FADE_STEP;
            if (actor->alpha[playerIndex] <= 0) {
                actor->alpha[playerIndex] = 0;
                actor->statusTransitionStates[playerIndex] = SAVE_STATUS_TRANSITION_FADE_IN;
                gControllerPakStatusCodes[playerIndex] = arg0->nextStatusCodes[playerIndex];
            }
            transitionState = actor->statusTransitionStates[playerIndex];
            break;
        case SAVE_STATUS_TRANSITION_NONE:
        case SAVE_STATUS_TRANSITION_DONE:
            break;
        }

        gRaceSetupMenuSubState.saveStatusTransitionStates[playerIndex] = transitionState;
    }

    arg0->frame = (arg0->frame + 1) & 0xF;
    transitionStateSum = 0;
    for (playerIndex = 0; playerIndex < (s32) gPlayerCount; playerIndex++) {
        transitionStateSum += actor->statusTransitionStates[playerIndex];
    }

    if (transitionStateSum == (gPlayerCount * SAVE_STATUS_TRANSITION_DONE)) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, drawRaceSetupSaveStatusWidgets, (s32) arg0);
    }
}
#endif

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

    setCallbackTaskCallback(new_var, updateRaceSetupSaveStatusWidgets);
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
            selection = gRaceSetupMenuSubState.selection[i];
            if (1) {
                state = gMenuChoicePromptState[i];
                if (selection != arg0->selection[i]) {
                    arg0->selection[i] = gRaceSetupMenuSubState.selection[i];
                }

                state = gMenuChoicePromptState[i];
                if (state != 0) {
                    new_var4 = arg0;
                    if (!(state & 1)) {
                        alpha = 0x100;
                    } else {
                        alpha = 0x60;
                    }

                    drawMenuSpriteWithAlpha(new_var4->x[i], arg0->topY[i], getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
                                  gRaceSetupSaveChoicePromptTopSprites[gControllerPakStatusCodes[i] * 2], 0x20, 0x20, 0, alpha, 0);

                    if (alpha == 0x100) {
                        alpha = 0x60;
                    } else {
                        alpha = 0x100;
                    }

                    new_var = arg0;
                    drawMenuSpriteWithAlpha(arg0->x[i], new_var->y[i], getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
                                  gRaceSetupSaveChoicePromptBottomSprites[gControllerPakStatusCodes[i] * 2], 0x20, 0x20, 0, alpha, 0);

                    if (new_var->alpha == 0) {
                    }

                    if (!arg0) {
                    }

                    state = gMenuChoicePromptState[i];
                    if ((state == 3) || (state == 4)) {
                        drawMenuSpriteWithAlpha(new_var2->x[i], (s16)(((gMenuChoicePromptState[i] * 0x10) + new_var->y[i]) - (new_var3 = 0x30)),
                                      getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 0x12, 0x20, 0x20, 0, new_var->alpha[i],
                                      i + 7);
                        state = gMenuChoicePromptState[i];
                    }
                }
            }

            if ((state >= 5) && (new_var2->slideOffset[i] == 0)) {
                gRaceSetupMenuSubState.nextSelection[i] = arg0->selection[i];
                gRaceSetupMenuSubState.saveStatusTransitionStates[i] = 2;
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
            if (gRaceSetupMenuSubState.selection[i] != actor->selection[i]) {
                actor->selection[i] = gRaceSetupMenuSubState.selection[i];
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

        if (((!state) && (!state)) && (!state)) {
        }
    }

    addRenderCallback(&gMenuRenderCallbackList, drawRaceSetupSaveChoicePrompts, (s32)arg0);
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
    setCallbackTaskCallback(arg0, updateRaceSetupSaveChoicePrompts);
}

void drawMenuIconTilemapSpriteActor(void *arg0) {
    SpriteActor *actor = arg0;

    drawMenuTilemapSprite(&actor->sprite, 0, actor->x, actor->y);
}

void updateMenuIconTilemapSpriteActor(SpriteActor *arg0) {
    SpriteActor *temp_a2;
    MenuTilemapSprite *temp_v0;
    SpriteActor *actor = arg0;

    temp_a2 = actor;
    actor->sprite.unk0--;
    temp_v0 = &temp_a2->sprite;
    if (actor->sprite.unk0 < 0) {
        actor->sprite.unk0 = 0x2FF;
    }
    temp_v0->unk2 = (temp_v0->unk2 + 1) & 0x1FF;
    addRenderCallback(&gMenuOverlayRenderCallbackList, drawMenuIconTilemapSpriteActor, (s32)temp_a2);
}

void initMenuIconTilemapSpriteActor(SpriteActor *arg0) {
    SpriteActor *temp_a2 = arg0;

    initMenuTilemapSprite(&temp_a2->sprite, getRelocatableHeapBlockBase(gAssetHandles[38]));
    temp_a2->x = temp_a2->sprite.unk8;
    temp_a2->y = temp_a2->sprite.unkA;
    setCallbackTaskCallback(temp_a2, updateMenuIconTilemapSpriteActor);
}

void initMenuTilemapSprite(MenuTilemapSprite *arg0, s32 arg1) {
    MenuTilemapSprite *dst = arg0;
    MenuTilemapSpriteAsset *src = (MenuTilemapSpriteAsset *)arg1;

    dst->unk0 = 0;
    dst->unk2 = 0;
    dst->unk8 = src->unk0;
    dst->unkA = src->unk2;
    dst->unk4 = src->unk4;
    dst->unk6 = src->unk6;
    dst->unkC = -0x90;
    dst->unkE = -0x68;
    dst->unk10 = 0x120;
    dst->unk12 = 0xD0;
    dst->unk18 = src->unkA + (u8 *)src;
    dst->unk14 = src->unkE + (u8 *)src;
    dst->unk1C = src->unk10;
    dst->unk20 = src->unkC + (u8 *)src;
    dst->unk24 = -1;
}

void n_alSynFreeFX(s32 arg0) {

}
