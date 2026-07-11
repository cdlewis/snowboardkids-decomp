#include "common.h"
#include "relocatable_heap.h"
#include "callback_task_scheduler.h"
#include "title_screen_ui.h"
#define MENU_RENDERING_BROAD_PROTOTYPES
#include "menu_rendering.h"
#include "title_menu.h"

#define TITLE_SCREEN_LOGO_SPRITE_HANDLE D_8011217A
#define TITLE_SCREEN_TEXTURE_HANDLE (D_80112130.titleTextureHandle)

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
} TitleScreenActor;

typedef struct {
    s16 width;
    s16 height;
} TitleScreenSprite;

struct TitleScreenLogoActor {
    TitleScreenActor common;
    /* 0x1C */ TitleScreenSprite sprite;
    /* 0x20 */ char pad20[4];
    /* 0x24 */ s16 startX;
    /* 0x26 */ s16 startY;
};

struct TitleScreenMenuOptionsActor {
    TitleScreenActor common;
    /* 0x1C */ s16 labelY;
    /* 0x1E */ s16 unused1E;
    /* 0x20 */ u16 selectedOption;
    /* 0x22 */ u16 pulseTimer;
    /* 0x24 */ s16 pulseScale;
};

struct TitleScreenMenuCursorActor {
    TitleScreenActor common;
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
} TitleScreenAssetHandles;

extern MainMenuState *gCurrentGameTask;
extern s32 gMenuRenderCallbackList;
extern TitleScreenAssetHandles D_80112130;
extern s16 D_8011217A;
extern s16 gMenuCommonSpritesAssetHandle;
extern u8 gConnectedControllerCount;

extern void addRenderCallback(void *, void *, void *);

void drawTitleScreenLogo(TitleScreenLogoActor *arg0) {
    func_80011D74(&arg0->sprite, 1, arg0->common.x, arg0->common.y);
}

void updateTitleScreenLogo(TitleScreenLogoActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, drawTitleScreenLogo, arg0);
}

void initTitleScreenLogo(TitleScreenLogoActor *arg0) {
    func_80017168((DstStruct_80017168 *)&arg0->sprite, getRelocatableHeapBlockBase(TITLE_SCREEN_LOGO_SPRITE_HANDLE));
    arg0->sprite.width = 0x10;
    arg0->sprite.height = 0x10;
    arg0->common.x = arg0->startX;
    arg0->common.y = arg0->startY;
    setCallbackTaskCallback(arg0, updateTitleScreenLogo);
}

void drawTitleScreenMenuOptions(TitleScreenMenuOptionsActor *arg0) {
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
                          getRelocatableHeapBlockBase(TITLE_SCREEN_TEXTURE_HANDLE),
                          tile & 0xFFFF, 0x20, 0x20, 0, 0x100, palette + 1);
        }
    }

    drawMenuSprite(-0x48, 0x4F, getRelocatableHeapBlockBase(TITLE_SCREEN_TEXTURE_HANDLE), 0xA, 0x20, 0x20, 0, 0);
    drawMenuAsciiText(0x68, -0x1A, "TM", 0, 0x100);
    drawMenuSprite(-0x48, 0x5A, getRelocatableHeapBlockBase(TITLE_SCREEN_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, 0);
}

void updateTitleScreenMenuOptions(TitleScreenMenuOptionsActor *arg0) {
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
    addRenderCallback(&gMenuRenderCallbackList, drawTitleScreenMenuOptions, arg0);
}

void initTitleScreenMenuOptions(TitleScreenMenuOptionsActor *arg0) {
    arg0->common.x = -0x2C;
    arg0->common.y = -0x2C;
    arg0->labelY = 0x20;
    arg0->unused1E = 0x30;
    arg0->selectedOption = 0;
    arg0->pulseTimer = 0;
    arg0->pulseScale = 0x100;
    setCallbackTaskCallback(arg0, updateTitleScreenMenuOptions);
}

void drawTitleScreenMenuCursor(TitleScreenMenuCursorActor *arg0) {
    s32 temp = getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle);

    drawMenuSprite(arg0->common.x, arg0->common.y, temp, 3, 0x20, 0x20, 0, 0);
}

void updateTitleScreenMenuCursor(TitleScreenMenuCursorActor *arg0) {
    if (gCurrentGameTask->selection != 1) {
        arg0->common.x = -0x34;
    } else {
        arg0->common.x = -0x3C;
    }
    arg0->common.y = (gCurrentGameTask->selection << 4) + 0x20;
    addRenderCallback(&gMenuRenderCallbackList, drawTitleScreenMenuCursor, arg0);
}

void initTitleScreenMenuCursor(TitleScreenMenuCursorActor *arg0) {
    arg0->common.y = 0x20;
    setCallbackTaskCallback(arg0, updateTitleScreenMenuCursor);
}
