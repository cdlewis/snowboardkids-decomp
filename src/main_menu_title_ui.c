#include "common.h"
#include "memory_allocator.h"
#include "callback_task_scheduler.h"
#include "main_menu_title_ui.h"
#define MENU_RENDERING_BROAD_PROTOTYPES
#include "menu_rendering.h"
#include "title_menu.h"

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
} MainMenuTitleActor;

typedef struct {
    s16 width;
    s16 height;
} MainMenuTitleSprite;

struct MainMenuLogoActor {
    MainMenuTitleActor common;
    /* 0x1C */ MainMenuTitleSprite sprite;
    /* 0x20 */ char pad20[4];
    /* 0x24 */ s16 startX;
    /* 0x26 */ s16 startY;
};

struct MainMenuSelectionLabelsActor {
    MainMenuTitleActor common;
    /* 0x1C */ s16 labelY;
    /* 0x1E */ s16 unused1E;
    /* 0x20 */ u16 selectedOption;
    /* 0x22 */ u16 pulseTimer;
    /* 0x24 */ s16 pulseScale;
};

struct MainMenuSelectionArrowActor {
    MainMenuTitleActor common;
};

typedef struct {
    char pad0[0x18];
    s32 fade;
    s32 selection;
    s32 delay;
    s32 timer;
} MainMenuState;

typedef struct {
    char pad0[0x42];
    /* 0x42 */ s16 titleTextureHandle;
} MainMenuTitleAssetHandles;

extern MainMenuState *gCurrentGameTask;
extern s32 gMenuRenderCallbackList;
extern MainMenuTitleAssetHandles D_80112130;
extern s16 D_8011217A;
extern s16 gMenuCommonSpritesAssetHandle;
extern u8 gConnectedControllerCount;

extern void addRenderCallback(void *, void *, void *);

void drawMainMenuTitleLogo(MainMenuLogoActor *arg0) {
    func_80011D74(&arg0->sprite, 1, arg0->common.x, arg0->common.y);
}

void updateMainMenuTitleLogo(MainMenuLogoActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, drawMainMenuTitleLogo, arg0);
}

void initMainMenuTitleLogo(MainMenuLogoActor *arg0) {
    func_80017168((DstStruct_80017168 *)&arg0->sprite, getMemoryBlockBase(D_8011217A));
    arg0->sprite.width = 0x10;
    arg0->sprite.height = 0x10;
    arg0->common.x = arg0->startX;
    arg0->common.y = arg0->startY;
    setCallbackTaskCallback(arg0, updateMainMenuTitleLogo);
}

void drawMainMenuTitleOptions(MainMenuSelectionLabelsActor *arg0) {
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
            drawMenuSpriteWithAlpha(arg0->common.x, (s16)(arg0->labelY + (i * 0x10)),
                          getMemoryBlockBase(D_80112130.titleTextureHandle),
                          tile & 0xFFFF, 0x20, 0x20, 0, 0x100, palette + 1);
        }
    }

    drawMenuSprite(-0x48, 0x4F, getMemoryBlockBase(D_80112130.titleTextureHandle), 0xA, 0x20, 0x20, 0, 0);
    drawMenuAsciiText(0x68, -0x1A, "TM", 0, 0x100);
    drawMenuSprite(-0x48, 0x5A, getMemoryBlockBase(D_80112130.titleTextureHandle), 0xB, 0x20, 0x20, 0, 0);
}

void updateMainMenuTitleOptions(MainMenuSelectionLabelsActor *arg0) {
    if (gCurrentGameTask->selection != (u16)(0, arg0->selectedOption)) {
        arg0->pulseScale = 0x100;
        arg0->pulseTimer = 0;
        arg0->selectedOption = gCurrentGameTask->selection;
    }
    if ((s32)(u16)arg0->pulseTimer < 0x10) {
        arg0->pulseScale -= 9;
    } else {
        arg0->pulseScale += 9;
    }
    arg0->pulseTimer = ((u16)arg0->pulseTimer + 1) & 0x1F;
    addRenderCallback(&gMenuRenderCallbackList, drawMainMenuTitleOptions, arg0);
}

void initMainMenuTitleOptions(MainMenuSelectionLabelsActor *arg0) {
    arg0->common.x = -0x2C;
    arg0->common.y = -0x2C;
    arg0->labelY = 0x20;
    arg0->unused1E = 0x30;
    arg0->selectedOption = 0;
    arg0->pulseTimer = 0;
    arg0->pulseScale = 0x100;
    setCallbackTaskCallback(arg0, updateMainMenuTitleOptions);
}

void drawMainMenuTitleCursor(MainMenuSelectionArrowActor *arg0) {
    s32 temp = getMemoryBlockBase(gMenuCommonSpritesAssetHandle);

    drawMenuSprite(arg0->common.x, arg0->common.y, temp, 3, 0x20, 0x20, 0, 0);
}

void updateMainMenuTitleCursor(MainMenuSelectionArrowActor *arg0) {
    if (gCurrentGameTask->selection != 1) {
        arg0->common.x = -0x34;
    } else {
        arg0->common.x = -0x3C;
    }
    arg0->common.y = (gCurrentGameTask->selection << 4) + 0x20;
    addRenderCallback(&gMenuRenderCallbackList, drawMainMenuTitleCursor, arg0);
}

void initMainMenuTitleCursor(MainMenuSelectionArrowActor *arg0) {
    arg0->common.y = 0x20;
    setCallbackTaskCallback(arg0, updateMainMenuTitleCursor);
}
