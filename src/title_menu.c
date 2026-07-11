#include "common.h"
#include "relocatable_heap.h"
#include "callback_task_scheduler.h"
#define MENU_RENDERER_BROAD_PROTOTYPES
#include "menu_renderer.h"
#include "title_menu.h"

#define TITLE_MENU_SECONDARY_TEXTURE_HANDLE (*(s16 *)&gAssetHandles[0x3E])
#define TITLE_MENU_FRAME_TEXTURE_HANDLE (*(s16 *)&gAssetHandles[0x42])
#define TITLE_MENU_BANNER_TEXTURE_HANDLE (*(s16 *)&gAssetHandles[0x52])
#define RACE_PLAYER_STATE_SIZE 0x60C

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
} Struct801235B8;

typedef struct {
    /* 0x00 */ u8 state;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ s16 alpha;
    /* 0x04 */ u8 pad4[2];
    /* 0x06 */ u16 selection[4];
    /* 0x0E */ u8 padE[4];
    /* 0x12 */ u16 nextSelection[4];
} TitleIntroTransitionState;

typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ u8 badgeIds[0x18];
    /* 0x4C */ u8 iconCount;
    /* 0x4D */ u8 pad4D[0x78F8 - 0x4D];
} TitleMenuPlayerView;

extern void addRenderCallback(void *, void *, s32);
extern s32 enqueueSoundEffect(s32, s32);
extern u8 D_800B5458[][0x4C];
extern u8 D_800B5A14[];
#ifdef NON_MATCHING
extern u8 D_800E0A80[];
extern u8 D_800E0A84[];
extern s32 D_80121D8C;
#endif
extern MenuIntroActor *gActiveMenuTask;
extern TitleMenuWidgetItemView *D_8010ADE0;
extern s16 gControllerPakStatusCodes[];
extern s16 D_800EC9D0[];
extern s16 gPlayerBadgeDisplayOrder[];
extern TitleMenuPlayerView gGameSaveDataBuffer[];
extern TitleIntroTransitionState D_8010AE00;
extern s16 D_8010AE02;
extern u16 D_8010AE06[];
extern u8 D_8010AE0E[];
extern u16 D_8010AE12[];
extern s16 D_8010AE38;
extern s16 D_8010AE3A;
extern s16 D_8010AE3C;
extern s16 D_8010AE3E;
extern s16 D_8010AE40;
extern s16 D_8010AE42;
extern s16 D_8010AE44;
extern s16 D_8010AE46;
extern u16 D_800B5408[];
extern u16 D_800B541C[];
extern u16 D_800B5430[];
extern u16 D_800B5444[];
extern u8 D_800B5200[];
extern u8 gAssetHandles[];
extern s16 gRaceUiSpriteAssetHandle;
extern s16 gMenuCommonSpritesAssetHandle;
extern s16 gMenuIconTilemapAssetHandle;
extern u8 gPlayerCount;
extern u8 D_800B5A2E[];
extern u8 D_800B5A2F[];
extern u8 D_800EC9C1;
extern s32 gMenuFlowState;
extern Struct801235B8 *gCurrentGameTask;
extern s32 gMenuOverlayRenderCallbackList;
extern s32 gMenuRenderCallbackList;

void func_80014600(MenuIntroActor *arg0) {
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
    drawMenuGlyphScript(actor->x, actor->y, &D_800B5200[selected * 0x68], 0, actor->alpha, 0);

    if ((actor->state == 1) || (actor->state == 6)) {
        drawMenuSprite((s16)(actor->x + 0xD0), (s16)(actor->y + 0x20),
                      getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), ((actor->timer >= 8) + 5) & 0xFFFF,
                      0x20, 0x20, 0, 0);
    }
}

void func_80014AA4(MenuIntroActor *arg0) {
    TitleIntroTransitionState *global;
    MenuIntroActor *actor;
    s32 globalState;
    s16 alpha;
    u8 state;
    s32 step;
    u32 stateCopy;

    global = &D_8010AE00;
    state = arg0->state;
    globalState = global->state;
    actor = arg0;
    stateCopy = state;
    if ((u32)stateCopy != globalState) {
        arg0->state = globalState;
        state = globalState;
        arg0->alpha = D_8010AE00.alpha;
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
                actor->child = createCallbackTask(func_80015054, 0, 0x63);
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
    D_8010AE00.state = actor->state;
    D_8010AE02 = actor->alpha;
    if (actor->state != 8) {
        addRenderCallback(&gMenuRenderCallbackList, func_80014600, (s32)actor);
    }
}

void initRaceSetupPlayerCountPrompt(void *arg0) {
    MenuIntroActor *actor = arg0;

    actor->x = -0x70;
    actor->y = -0x1C;
    actor->alpha = 0;
    actor->state = 0;
    setCallbackTaskCallback(arg0, func_80014AA4);
}

void func_80014CB8(void *arg0) {
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

    if ((D_800EC9C1 == 0) || (D_800EC9C1 & 1) || (gPlayerCount != 1)) {
        drawMenuSpriteWithAlpha(actor->x, actor->y, getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), 3, 0x20, 0x20, 0, alpha, 0);
        if (width == 0x20) {
            temp = width & 0xFFFF;
            drawMenuSpriteWithAlpha((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), getRelocatableHeapBlockBase(gRaceUiSpriteAssetHandle), 0, temp, temp, 0, alpha, 0);
        } else {
            drawMenuSpriteWithPaletteScale((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), getRelocatableHeapBlockBase(gRaceUiSpriteAssetHandle), 0, alpha);
        }
        drawMenuGlyphScriptDefaultFont((s16)(actor->x + 0x30), (s16)(actor->y + 4), D_800B5408, 1, alpha);
    }

    drawMenuSpriteWithAlpha((s16)(actor->x + 0x80), actor->y, getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), 7, 0x20, 0x20, 0, alpha, 0);
}

void func_80014EF0(MenuItemActor *arg0) {
    MenuItemActor *child;

    switch (arg0->state) {
    case 0:
        arg0->x -= 0x20;
        if (arg0->x == 0x70) {
            arg0->child = createCallbackTask(func_80015404, 0, 0x63);
            enqueueSoundEffect(1, 0x32);
        }
        if (arg0->x == -0x50) {
            arg0->state = 1;
        }
        break;
    case 1:
        if (D_800EC9C1 == 0x13) {
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
    addRenderCallback(&gMenuRenderCallbackList, func_80014CB8, (s32)arg0);
}

void func_80015054(MenuItemActor *arg0) {
    MenuItemActor *actor = arg0;

    actor->x = 0x90;
    actor->y = -0x18;
    actor->state = 0;
    setCallbackTaskCallback(arg0, func_80014EF0);
}

void func_8001508C(MenuItemActor *arg0) {
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

    if ((D_800EC9C1 == 0) || (D_800EC9C1 & 1) || (gPlayerCount != 2)) {
        drawMenuSpriteWithAlpha(actor->x, actor->y, getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, alpha, 2);
        if (width == 0x20) {
            temp = width & 0xFFFF;
            drawMenuSpriteWithAlpha((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 1, temp, temp, 0, alpha, 0);
        } else {
            drawMenuSpriteWithPaletteScale((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 1, alpha);
        }
        drawMenuGlyphScriptDefaultFont((s16)(actor->x + 0x30), (s16)(actor->y + 4), D_800B541C, 1, alpha);
    }

    for (i = 0; i != 0x28; i += 0x14) {
        drawMenuSpriteWithAlpha((s16)(actor->x + i + 0x80), actor->y, getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, alpha, 0);
    }
}

void func_800152D0(MenuItemActor *arg0) {
    MenuItemActor *child;

    switch (arg0->state) {
    case 0:
        arg0->x -= 0x20;
        if (arg0->x == 0x70) {
            arg0->child = createCallbackTask(func_800157B4, 0, 0x63);
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
    addRenderCallback(&gMenuRenderCallbackList, func_8001508C, (s32)arg0);
}

void func_80015404(MenuItemActor *arg0) {
    MenuItemActor *actor = arg0;

    actor->x = 0x90;
    actor->y = 8;
    actor->state = 0;
    setCallbackTaskCallback(arg0, func_800152D0);
}

void func_8001543C(void *arg0) {
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

    if ((D_800EC9C1 == 0) || (D_800EC9C1 & 1) || (gPlayerCount != 3)) {
        drawMenuSpriteWithAlpha(actor->x, actor->y, getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, alpha, 3);
        if (width == 0x20) {
            temp = width & 0xFFFF;
            drawMenuSpriteWithAlpha((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 2, temp, temp, 0, alpha, 0);
        } else {
            drawMenuSpriteWithPaletteScale((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 2, alpha);
        }
        drawMenuGlyphScriptDefaultFont((s16)(actor->x + 0x30), (s16)(actor->y + 4), D_800B5430, 1, alpha);
    }

    for (i = 0; i != 0x3C; i += 0x14) {
        drawMenuSpriteWithAlpha((s16)(actor->x + i + 0x80), actor->y, getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, alpha, 0);
    }
}

void func_80015680(MenuItemActor *arg0) {
    MenuItemActor *child;

    switch (arg0->state) {
    case 0:
        arg0->x -= 0x20;
        if (arg0->x == 0x70) {
            arg0->child = createCallbackTask(func_80015B20, 0, 0x63);
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
    addRenderCallback(&gMenuRenderCallbackList, func_8001543C, (s32)arg0);
}

void func_800157B4(MenuItemActor *arg0) {
    MenuItemActor *actor = arg0;

    actor->x = 0x90;
    actor->y = 0x28;
    actor->state = 0;
    setCallbackTaskCallback(arg0, func_80015680);
}

void func_800157EC(void *arg0) {
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

    if ((D_800EC9C1 == 0) || (D_800EC9C1 & 1) || (gPlayerCount != 4)) {
        drawMenuSpriteWithAlpha(actor->x, actor->y, getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, alpha, 4);
        if (width == 0x20) {
            temp = width & 0xFFFF;
            drawMenuSpriteWithAlpha((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 3, temp, temp, 0, alpha, 0);
        } else {
            drawMenuSpriteWithPaletteScale((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 3, alpha);
        }
        drawMenuGlyphScriptDefaultFont((s16)(actor->x + 0x30), (s16)(actor->y + 4), D_800B5444, 1, alpha);
    }

    for (i = 0; i != 0x50; i += 0x14) {
        drawMenuSpriteWithAlpha((s16)(actor->x + i + 0x80), actor->y, getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, alpha, 0);
    }
}

void func_80015A30(MenuItemActor *arg0) {
    MenuItemActor *actor = arg0;
    s16 x;

    switch (actor->state) {
    case 0:
        actor->x -= 0x20;
        x = actor->x;
        if (x == -0x50) {
            actor->state = 1;
            createCallbackTask(func_80015C84, 0, 0x63);
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
    addRenderCallback(&gMenuRenderCallbackList, func_800157EC, (s32)actor);
}

void func_80015B20(MenuItemActor *arg0) {
    MenuItemActor *actor = arg0;

    actor->x = 0x90;
    actor->y = 0x48;
    actor->state = 0;
    setCallbackTaskCallback(arg0, func_80015A30);
}

void func_80015B58(void *arg0) {
    FadeItemActor *actor = arg0;

    drawMenuSpriteWithAlpha(
        actor->x,
        (s16)((actor->y + (gPlayerCount << 5)) - 0x20),
        getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle),
        4,
        0x20,
        0x20,
        0,
        (u16)actor->alpha,
        0);
}

void func_80015BD8(FadeItemActor *arg0) {
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
        addRenderCallback(&gMenuRenderCallbackList, func_80015B58, (s32)temp_a2);
    }
}

void func_80015C84(FadeItemActor *arg0) {
    FadeItemActor *actor = arg0;

    actor->x = -0x68;
    actor->y = -0x14;
    actor->alpha = 0;
    setCallbackTaskCallback(arg0, func_80015BD8);
}

// func_80015CBC best match: 97.244% (nonmatchings/func_80015CBC-6276316234415602851/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80015CBC.s")

#ifdef NON_MATCHING
void func_80015CBC(TitleMenuWidgetItemView *arg0) {
    char buf[0xC];
    s32 alpha;
    s32 next;
    s32 i;
    TitleMenuWidgetItemView *item;
    volatile u8 *selected;
    u8 *text;

    selected = &gPlayerCount;
    item = arg0;
    text = buf - 0x10;
    for (i = 0; i != 4;) {
        alpha = 0x100;
        next = (((s32)*selected - 1) < i);
        if (next) {
            alpha = 0x50;
        }

        drawMenuSpriteWithAlpha(item->x, item->y, getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 0xA, 0x20, 0x20,
                      (i * RACE_PLAYER_STATE_SIZE) * 0, alpha, (i + 1) & 0xFF);
        next = i + 1;
        drawMenuSpriteWithAlpha((s16)(item->x + 0x40), item->y, getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, alpha, next & 0xFF);
        drawMenuSpriteWithAlpha((s16)(item->x + 0x80), item->y, getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 0xC, 0x20, 0x20, 0, alpha, next & 0xFF);

        sprintf(text, D_800E0A80, next);
        drawMenuAsciiText((s16)(item->x + 0x32), (s16)(item->y + 2), text, 0, alpha);
        if (alpha == 0x100) {
            sprintf(text, D_800E0A84, *(s32 *)((u8 *)&D_80121D8C + (i * RACE_PLAYER_STATE_SIZE)));
            drawMenuAsciiText((s16)(item->x + 0x44), (s16)(item->y + 0x1B), text, 0, alpha);
        } else {
            drawMenuSpriteWithAlpha((s16)(item->x + 2), (s16)(item->y + 0x14), getRelocatableHeapBlockBase(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 0x90, 0x20, 0x20, 0, 0xF0, 0);
        }
        i = next;
        item = (TitleMenuWidgetItemView *)((s16 *)item + 1);
    }
}
#endif

// func_80015F4C best match: 99.888% (nonmatchings/func_80015F4C-8662636370764828261/base_12.c)
#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80015F4C.s")

#ifdef NON_MATCHING
void func_80015F4C(RectListActor *arg0) {
    u8 state;
    s32 outer;
    s32 i;
    s16 delta0;
    s16 delta1;
    s16 *coords;

    outer = D_8010AE04;
    state = arg0->frame;
    if (D_8010AE04 != state) {
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

    D_8010AE04 = state;
    D_8010AE38 = arg0->rects[0].x0;
    D_8010AE40 = arg0->rects[1].x0;
    D_8010AE3A = arg0->rects[0].y0;
    D_8010AE42 = arg0->rects[1].y0;
    D_8010AE3C = arg0->rects[0].x1;
    D_8010AE44 = arg0->rects[1].x1;
    D_8010AE3E = arg0->rects[0].y1;
    D_8010AE46 = arg0->rects[1].y1;

    if (gMenuFlowState == 0x63) {
        removeCallbackTask(arg0);
        gMenuFlowState = 0;
        return;
    }

    addRenderCallback(&gMenuRenderCallbackList, func_80015CBC, (s32)arg0);
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
    setCallbackTaskCallback(arg0, func_80015F4C);
}

// func_80016284 best match: 90.000% (nonmatchings/func_80016284-180949888360117632/base_10.c)
#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80016284.s")

#ifdef NON_MATCHING
void func_80016284(TitleMenuIconStripActor *arg0) {
    volatile s32 unused;
    s32 pad[6];
    s32 i;
    s32 actorOffset;
    s16 *courseId;
    register TitleMenuPlayerView *player;
    register TitleMenuIconStripActor *item;
    register s32 xOffset;
    register s32 alpha;
    register s32 nextJ;
    register s16 *badgeIndex;
    register TitleMenuPlayerView *player2;
    s32 j;
    s32 count;
    s32 tile;

    i = 0;
    if ((s32)gPlayerCount > 0) {
        courseId = gControllerPakStatusCodes;
        actorOffset = 0;
        do {
            if (*courseId != 0x13) {
                player = &gGameSaveDataBuffer[i];
                j = 0;
                if (player->iconCount == 3) {
                    count = 3;
                } else {
                    count = player->iconCount + 1;
                }
                if (count > 0) {
                    xOffset = 0;
                    item = (TitleMenuIconStripActor *)((u8 *)arg0 + actorOffset);
                    do {
                        nextJ = j + 1;
                        if ((s32)player->iconCount < nextJ) {
                            alpha = 0x70;
                        } else {
                            alpha = 0x100;
                        }
                        drawMenuSpriteWithAlpha((s16)(item->rects[0].x0 + arg0->iconOffsetX + xOffset),
                                      (s16)(item->rects[1].x0 + arg0->iconOffsetY), getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), 0x19,
                                      0x20, 0x20, 0, alpha, 9 - j);
                        j = nextJ;
                        xOffset += 0x10;
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
                    player2 = &gGameSaveDataBuffer[i];
                    badgeIndex = gPlayerBadgeDisplayOrder;
                    xOffset = 0;
                    item = (TitleMenuIconStripActor *)((u8 *)arg0 + actorOffset);
                    do {
                        tile = player2->badgeIds[*badgeIndex];
                        alpha = 0x70;
                        if (tile != 0) {
                            alpha = 0x100;
                            tile += 6;
                        } else {
                            tile = 9;
                        }
                        drawMenuSpriteWithAlpha((s16)(item->rects[0].x0 + arg0->badgeOffsetX + xOffset),
                                      (s16)(item->rects[1].x0 + arg0->badgeOffsetY), getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle),
                                      (j + 0x1A) & 0xFFFF, 0x20, 0x20, 0, alpha, tile);
                        j++;
                        xOffset += 0xE;
                        badgeIndex++;
                    } while (j != count);
                }
            }
            i++;
            actorOffset += 2;
            courseId++;
        } while (i < (s32)gPlayerCount);
    }
}
#endif

void func_80016560(void *arg0) {
    RectListActor *temp_a2;
    RectListActor *actor = arg0;

    actor->rects[0].x0 = D_8010AE38;
    actor->rects[1].x0 = D_8010AE40;
    actor->rects[0].y0 = D_8010AE3A;
    actor->rects[1].y0 = D_8010AE42;
    actor->rects[0].x1 = D_8010AE3C;
    actor->rects[1].x1 = D_8010AE44;
    actor->rects[0].y1 = D_8010AE3E;
    temp_a2 = actor;
    actor->rects[1].y1 = D_8010AE46;
    addRenderCallback(&gMenuRenderCallbackList, func_80016284, (s32)temp_a2);
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
    setCallbackTaskCallback(arg0, func_80016560);
}

void func_80016664(TitleMenuWidgetActor *arg0) {
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
                drawMenuAsciiText((s16)(actor->x[i] + 2), (s16)(actor->y[i] + 0x10), D_800B5A14, 7, actor->alpha[i]);
            } else {
                text = D_800B5458[state];
                state = 2;
                drawMenuGlyphScript((s16)(actor->x[i] + state), (s16)(actor->y[i] + 0x10), text, 1, actor->alpha[i], 0);
            }

            if ((actor->alpha[i] == 0x100) && (intro->state == 8)) {
                state = gControllerPakStatusCodes[i];
                if ((state == 4) || ((state >= 7) && (state != 0x12) && (D_800EC9D0[i] == 0))) {
                    drawMenuSprite((s16)(actor->x[i] + 0x70), (s16)(actor->y[i] + 0x20), getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
                                  (((s32)actor->frame >= 8) + 5) & 0xFFFF, 0x20, 0x20, 0, 0);
                }
            }
        }
    }
}

// func_80016948 best match: 90.557% (nonmatchings/func_80016948-6688367443449623229/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80016948.s")

#ifdef NON_MATCHING
void func_80016948(TitleMenuWidgetActor *arg0) {
    register TitleMenuWidgetActor *actor;
    s32 i;
    s32 sum;
    TitleIntroTransitionState *global;
    TitleMenuWidgetItemView *view;
    u8 state;

    view = D_8010ADE0;
    actor = arg0;
    i = 0;
    if ((s32) gPlayerCount > 0) {
        global = &D_8010AE00;
        do {
            actor->x[i] = view->x;
            actor->y[i] = view->y;

            state = global->padE[i];
            if (state != actor->unk31[i]) {
                actor->unk31[i] = state;
                actor->unk36[i] = global->nextSelection[i];
            }

            state = actor->unk31[i];
            switch (state) {
            case 1:
                actor->alpha[i] += 0x26;
                if (actor->alpha[i] >= 0x100) {
                    actor->alpha[i] = 0x100;
                    actor->unk31[i] = 0;
                }
                state = actor->unk31[i];
                break;
            case 2:
                actor->alpha[i] -= 0x26;
                if (actor->alpha[i] <= 0) {
                    actor->alpha[i] = 0;
                    actor->unk31[i] = 1;
                    gControllerPakStatusCodes[i] = actor->unk36[i];
                }
                state = actor->unk31[i];
                break;
            case 0:
            case 3:
                break;
            }

            i++;
            view = (TitleMenuWidgetItemView *) ((u8 *) view + 2);
            global->padE[i - 1] = state;
        } while (i < (s32) gPlayerCount);
    }

    actor->frame = (actor->frame + 1) & 0xF;
    sum = 0;
    i = 0;
    if ((s32) gPlayerCount > 0) {
        do {
            sum += actor->unk31[i];
            i++;
        } while (i < (s32) gPlayerCount);
    }

    if (sum == (gPlayerCount * 3)) {
        removeCallbackTask((CallbackTask *) actor);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, func_80016664, (s32) actor);
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
        new_var->unk31[i] = 0;
    }

    setCallbackTaskCallback(new_var, func_80016948);
}

// func_80016BE8 best match: 99.233% (nonmatchings/func_80016BE8-6061209858023118177/base_12.c)
#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80016BE8.s")

#ifdef NON_MATCHING
void func_80016BE8(TitleMenuTransitionActor *arg0) {
    MenuIntroActor *intro;
    TitleMenuTransitionActor *new_var;
    TitleMenuTransitionActor *new_var2;
    s32 i;
    s32 alpha;
    s16 state;

    intro = gActiveMenuTask;
    new_var2 = arg0;
    if (intro->state == 8) {
        for (i = 0; i < gPlayerCount; i++) {
            if (D_8010AE00.selection[i] != arg0->selection[i]) {
                arg0->selection[i] = D_8010AE00.selection[i];
            }

            state = D_800EC9D0[i];
            if (state != 0) {
                if (!(state & 1)) {
                    alpha = 0x100;
                } else {
                    alpha = 0x60;
                }

                drawMenuSpriteWithAlpha(arg0->x[i], arg0->topY[i], getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
                              D_800B5A2F[gControllerPakStatusCodes[i] * 2], 0x20, 0x20, 0, alpha, 0);

                if (alpha == 0x100) {
                    alpha = 0x60;
                } else {
                    alpha = 0x100;
                }

                new_var = arg0;
                drawMenuSpriteWithAlpha(arg0->x[i], new_var->y[i], getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE),
                              D_800B5A2E[gControllerPakStatusCodes[i] * 2], 0x20, 0x20, 0, alpha, 0);

                if (new_var->alpha == 0) {
                }

                state = D_800EC9D0[i];
                if ((state == 3) || (state == 4)) {
                    drawMenuSpriteWithAlpha(new_var2->x[i], (s16)(((D_800EC9D0[i] * 0x10) + new_var->y[i]) - 0x30),
                                  getRelocatableHeapBlockBase(TITLE_MENU_FRAME_TEXTURE_HANDLE), 0x12, 0x20, 0x20, 0, new_var->alpha[i],
                                  i + 7);
                    state = D_800EC9D0[i];
                }
            }

            if ((state >= 5) && (new_var2->slideOffset[i] == 0)) {
                D_8010AE00.nextSelection[i] = arg0->selection[i];
                D_8010AE0E[i] = 2;
                D_800EC9D0[i] = 0;
            }
        }
    }
}
#endif

// func_80016E40 best match: 97.350% (nonmatchings/func_80016E40-6061209858023118177/base_10.c)
#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80016E40.s")

#ifdef NON_MATCHING
void func_80016E40(TitleMenuTransitionActor *arg0) {
    TitleMenuTransitionActor *actor;
    u8 *count;
    s32 i;
    s32 step;
    s32 stepAmount;
    s16 state;

    count = &gPlayerCount;
    actor = arg0;
    i = 0;
    if (*count > 0) {
        do {
            if (D_8010AE00.selection[i] != actor->selection[i]) {
                actor->selection[i] = D_8010AE00.selection[i];
            }

            state = D_800EC9D0[i];
            if ((state != 0) && (state != 3) && (state != 4)) {
                if (state < 5) {
                    step = 1;
                } else {
                    step = -1;
                }

                stepAmount = step * 8;
                actor->slideOffset[i] += stepAmount;
                state = actor->slideOffset[i];
                if (state == 0x20) {
                    D_800EC9D0[i] += 2;
                    actor->alphaTimer[i] = 0;
                    actor->alpha[i] = 0x100;
                    state = actor->slideOffset[i];
                }

                if (stepAmount == 8) {
                    if (state < 0x18) {
                        actor->y[i] += 8;
                    }
                    actor->topY[i] += 8;
                    state = D_800EC9D0[i];
                } else {
                    if (state < 0x10) {
                        actor->y[i] -= 8;
                    }
                    actor->topY[i] -= 8;
                    state = D_800EC9D0[i];
                }
            }

            if ((state == 3) || (state == 4)) {
                if ((s32)(u16)actor->alphaTimer[i] < 0x10) {
                    actor->alpha[i] -= 9;
                } else {
                    actor->alpha[i] += 9;
                }
                actor->alphaTimer[i] = ((u16)actor->alphaTimer[i] + 1) & 0x1F;
            }
            i++;
        } while (i < *count);
    }

    addRenderCallback(&gMenuRenderCallbackList, func_80016BE8, (s32)arg0);
}
#endif

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
    setCallbackTaskCallback(arg0, func_80016E40);
}

void func_80017078(void *arg0) {
    SpriteActor *actor = arg0;

    drawMenuTilemapSprite(&actor->sprite, 0, actor->x, actor->y);
}

void func_800170AC(SpriteActor *arg0) {
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
    addRenderCallback(&gMenuOverlayRenderCallbackList, func_80017078, (s32)temp_a2);
}

void func_8001710C(SpriteActor *arg0) {
    SpriteActor *temp_a2 = arg0;

    initMenuTilemapSprite(&temp_a2->sprite, getRelocatableHeapBlockBase(gMenuIconTilemapAssetHandle));
    temp_a2->x = temp_a2->sprite.unk8;
    temp_a2->y = temp_a2->sprite.unkA;
    setCallbackTaskCallback(temp_a2, func_800170AC);
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
