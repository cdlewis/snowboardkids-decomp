#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/main_menu/main_menu_title_ui.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/race_setup/race_setup_ui.h"

#define ASSET_HANDLE(index) (gAssetHandles[(index)])
#define TITLE_SCREEN_LOGO_SPRITE_HANDLE ASSET_HANDLE(37)
#define TITLE_SCREEN_TEXTURE_HANDLE (gAssetHandles[0x21])

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
} MainMenuTitleActor;

struct MainMenuTitleLogoActor {
    MainMenuTitleActor common;
    union {
        /* 0x1C */ MenuTilemapSprite tilemapSprite;
        struct {
            /* 0x1C */ s16 width;
            /* 0x1E */ s16 height;
            /* 0x20 */ char pad20[4];
            /* 0x24 */ s16 startX;
            /* 0x26 */ s16 startY;
        };
    };
};

struct MainMenuTitleOptionsActor {
    MainMenuTitleActor common;
    /* 0x1C */ s16 labelY;
    /* 0x1E */ s16 unused1E;
    /* 0x20 */ u16 selectedOption;
    /* 0x22 */ u16 blinkTimer;
    /* 0x24 */ s16 blinkAlpha;
};

struct MainMenuTitleCursorActor {
    MainMenuTitleActor common;
};

extern u8 gConnectedControllerCount;

void drawMainMenuTitleLogo(MainMenuTitleLogoActor *arg0) {
    drawMenuTilemapSprite(&arg0->tilemapSprite.render, 1, arg0->common.x, arg0->common.y);
}

void updateMainMenuTitleLogo(MainMenuTitleLogoActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawMainMenuTitleLogo, arg0);
}

void initMainMenuTitleLogo(MainMenuTitleLogoActor *arg0) {
    initMenuTilemapSprite(&arg0->tilemapSprite, getRelocatableHeapBlockBase(TITLE_SCREEN_LOGO_SPRITE_HANDLE));
    arg0->width = 0x10;
    arg0->height = 0x10;
    arg0->common.x = arg0->startX;
    arg0->common.y = arg0->startY;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateMainMenuTitleLogo);
}

void drawMainMenuTitleOptions(MainMenuTitleOptionsActor *arg0) {
    s32 i;
    s32 tile;
    u16 palette;

    if (gConnectedControllerCount != 0) {
        for (i = 0; i < 3; i++) {
            if (i == arg0->selectedOption) {
                tile = ((i * 2) + 4) & 0xFFFF;
                palette = 2;
            } else {
                tile = ((i * 2) + 5) & 0xFFFF;
                palette = 3;
            }
            drawMenuSpriteWithAlpha(
                arg0->common.x,
                (s16)(arg0->labelY + (i * 0x10)),
                getRelocatableHeapBlockBase(TITLE_SCREEN_TEXTURE_HANDLE),
                tile,
                0x20,
                0x20,
                0,
                0x100,
                palette + 1
            );
        }
    }

    drawMenuSprite(-0x48, 0x4F, getRelocatableHeapBlockBase(TITLE_SCREEN_TEXTURE_HANDLE), 0xA, 0x20, 0x20, 0, 0);
    drawMenuAsciiText(0x68, -0x1A, "TM", 0, 0x100);
    drawMenuSprite(-0x48, 0x5A, getRelocatableHeapBlockBase(TITLE_SCREEN_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, 0);
}

void updateMainMenuTitleOptions(MainMenuTitleOptionsActor *arg0) {
    if (gCurrentGameTask->selection != (u16)(0, arg0->selectedOption)) {
        arg0->blinkAlpha = 0x100;
        arg0->blinkTimer = 0;
        arg0->selectedOption = gCurrentGameTask->selection;
    }
    if ((s32)(u16)arg0->blinkTimer < 0x10) {
        arg0->blinkAlpha -= 9;
    } else {
        arg0->blinkAlpha += 9;
    }
    arg0->blinkTimer = ((u16)arg0->blinkTimer + 1) & 0x1F;
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawMainMenuTitleOptions, arg0);
}

void initMainMenuTitleOptions(MainMenuTitleOptionsActor *arg0) {
    arg0->common.x = -0x2C;
    arg0->common.y = -0x2C;
    arg0->labelY = 0x20;
    arg0->unused1E = 0x30;
    arg0->selectedOption = 0;
    arg0->blinkTimer = 0;
    arg0->blinkAlpha = 0x100;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateMainMenuTitleOptions);
}

void drawMainMenuTitleCursor(MainMenuTitleCursorActor *arg0) {
    void *temp = getRelocatableHeapBlockBase(ASSET_HANDLE(33));

    drawMenuSprite(arg0->common.x, arg0->common.y, temp, 3, 0x20, 0x20, 0, 0);
}

void updateMainMenuTitleCursor(MainMenuTitleCursorActor *arg0) {
    if (gCurrentGameTask->selection != 1) {
        arg0->common.x = -0x34;
    } else {
        arg0->common.x = -0x3C;
    }
    arg0->common.y = (gCurrentGameTask->selection << 4) + 0x20;
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawMainMenuTitleCursor, arg0);
}

void initMainMenuTitleCursor(MainMenuTitleCursorActor *arg0) {
    arg0->common.y = 0x20;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateMainMenuTitleCursor);
}
