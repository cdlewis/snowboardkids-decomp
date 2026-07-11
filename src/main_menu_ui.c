#include "common.h"
#include "memory_allocator.h"
#include "callback_task_scheduler.h"
#include "main_menu_ui.h"
#define MENU_RENDERING_BROAD_PROTOTYPES
#include "menu_rendering.h"
#include "title_menu.h"

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
} MainMenuUiActor;

typedef struct {
    s16 width;
    s16 height;
} MainMenuUiSprite;

struct MainMenuLogoActor {
    MainMenuUiActor common;
    /* 0x1C */ MainMenuUiSprite sprite;
    /* 0x20 */ char pad20[4];
    /* 0x24 */ s16 startX;
    /* 0x26 */ s16 startY;
};

struct MainMenuSelectionLabelsActor {
    MainMenuUiActor common;
    /* 0x1C */ s16 labelY;
    /* 0x1E */ s16 unused1E;
    /* 0x20 */ u16 selectedOption;
    /* 0x22 */ u16 pulseTimer;
    /* 0x24 */ s16 pulseScale;
};

struct MainMenuSelectionArrowActor {
    MainMenuUiActor common;
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
    /* 0x42 */ s16 menuUiTextureHandle;
} MainMenuAssetHandles;

extern MainMenuState *gCurrentInputTask;
extern s32 D_80124868;
extern MainMenuAssetHandles D_80112130;
extern s16 D_8011217A;
extern s16 D_80112172;
extern u8 gConnectedControllerCount;

extern void func_800483FC(void *, void *, void *);

void func_80032620(MainMenuLogoActor *arg0) {
    func_80011D74(&arg0->sprite, 1, arg0->common.x, arg0->common.y);
}

void func_80032654(MainMenuLogoActor *arg0) {
    func_800483FC(&D_80124868, func_80032620, arg0);
}

void func_80032684(MainMenuLogoActor *arg0) {
    func_80017168((DstStruct_80017168 *)&arg0->sprite, func_80043040(D_8011217A));
    arg0->sprite.width = 0x10;
    arg0->sprite.height = 0x10;
    arg0->common.x = arg0->startX;
    arg0->common.y = arg0->startY;
    setCallbackTaskCallback(arg0, func_80032654);
}

void func_800326EC(MainMenuSelectionLabelsActor *arg0) {
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
            func_8000F8AC(arg0->common.x, (s16)(arg0->labelY + (i * 0x10)),
                          func_80043040(D_80112130.menuUiTextureHandle),
                          tile & 0xFFFF, 0x20, 0x20, 0, 0x100, palette + 1);
        }
    }

    func_8000F030(-0x48, 0x4F, func_80043040(D_80112130.menuUiTextureHandle), 0xA, 0x20, 0x20, 0, 0);
    func_80013D0C(0x68, -0x1A, "TM", 0, 0x100);
    func_8000F030(-0x48, 0x5A, func_80043040(D_80112130.menuUiTextureHandle), 0xB, 0x20, 0x20, 0, 0);
}

void func_8003288C(MainMenuSelectionLabelsActor *arg0) {
    if (gCurrentInputTask->selection != (u16)(0, arg0->selectedOption)) {
        arg0->pulseScale = 0x100;
        arg0->pulseTimer = 0;
        arg0->selectedOption = gCurrentInputTask->selection;
    }
    if ((s32)(u16)arg0->pulseTimer < 0x10) {
        arg0->pulseScale -= 9;
    } else {
        arg0->pulseScale += 9;
    }
    arg0->pulseTimer = ((u16)arg0->pulseTimer + 1) & 0x1F;
    func_800483FC(&D_80124868, func_800326EC, arg0);
}

void func_80032934(MainMenuSelectionLabelsActor *arg0) {
    arg0->common.x = -0x2C;
    arg0->common.y = -0x2C;
    arg0->labelY = 0x20;
    arg0->unused1E = 0x30;
    arg0->selectedOption = 0;
    arg0->pulseTimer = 0;
    arg0->pulseScale = 0x100;
    setCallbackTaskCallback(arg0, func_8003288C);
}

void func_80032984(MainMenuSelectionArrowActor *arg0) {
    s32 temp = func_80043040(D_80112172);

    func_8000F030(arg0->common.x, arg0->common.y, temp, 3, 0x20, 0x20, 0, 0);
}

void func_800329E0(MainMenuSelectionArrowActor *arg0) {
    if (gCurrentInputTask->selection != 1) {
        arg0->common.x = -0x34;
    } else {
        arg0->common.x = -0x3C;
    }
    arg0->common.y = (gCurrentInputTask->selection << 4) + 0x20;
    func_800483FC(&D_80124868, func_80032984, arg0);
}

void func_80032A50(MainMenuSelectionArrowActor *arg0) {
    arg0->common.y = 0x20;
    setCallbackTaskCallback(arg0, func_800329E0);
}
