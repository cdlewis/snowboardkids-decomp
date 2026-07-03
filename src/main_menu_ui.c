#include "common.h"

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
} MainMenuUiActor;

typedef struct {
    s16 width;
    s16 height;
} MainMenuUiSprite;

typedef struct {
    MainMenuUiActor common;
    /* 0x1C */ MainMenuUiSprite sprite;
    /* 0x20 */ char pad20[4];
    /* 0x24 */ s16 startX;
    /* 0x26 */ s16 startY;
} MainMenuLogoActor;

typedef struct {
    MainMenuUiActor common;
    /* 0x1C */ s16 labelY;
    /* 0x1E */ s16 unused1E;
    /* 0x20 */ u16 selectedOption;
    /* 0x22 */ u16 pulseTimer;
    /* 0x24 */ s16 pulseScale;
} MainMenuSelectionLabelsActor;

typedef struct {
    MainMenuUiActor common;
} MainMenuSelectionArrowActor;

typedef struct {
    char pad0[0x18];
    s32 fade;
    s32 selection;
    s32 delay;
    s32 timer;
} MainMenuState;

extern MainMenuState *D_801235B8;
extern s32 D_80124868;
extern s16 D_8011217A;
extern s16 D_80112172;

extern void func_800483FC(void *, void *, void *);
extern void func_80071824(void *, void *);
extern void func_80011D74(void *, s32, s16, s16);
extern void func_80017168(void *, s32);
extern s32 func_80043040(s16);
extern void func_8000F030(s16, s16, s32, s32, s32, s32, s32, s32);

void func_80032620(MainMenuLogoActor *arg0);
void func_80032654(MainMenuLogoActor *arg0);
void func_8003288C(MainMenuSelectionLabelsActor *arg0);
void func_800326EC(MainMenuSelectionLabelsActor *arg0);
void func_80032934(MainMenuSelectionLabelsActor *arg0);
void func_80032984(MainMenuSelectionArrowActor *arg0);
void func_800329E0(MainMenuSelectionArrowActor *arg0);
void func_80032A50(MainMenuSelectionArrowActor *arg0);

void func_80032620(MainMenuLogoActor *arg0) {
    func_80011D74(&arg0->sprite, 1, arg0->common.x, arg0->common.y);
}

void func_80032654(MainMenuLogoActor *arg0) {
    func_800483FC(&D_80124868, func_80032620, arg0);
}

void func_80032684(MainMenuLogoActor *arg0) {
    func_80017168(&arg0->sprite, func_80043040(D_8011217A));
    arg0->sprite.width = 0x10;
    arg0->sprite.height = 0x10;
    arg0->common.x = arg0->startX;
    arg0->common.y = arg0->startY;
    func_80071824(arg0, func_80032654);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_ui/func_800326EC.s")

void func_8003288C(MainMenuSelectionLabelsActor *arg0) {
    if (D_801235B8->selection != (u16)(0, arg0->selectedOption)) {
        arg0->pulseScale = 0x100;
        arg0->pulseTimer = 0;
        arg0->selectedOption = D_801235B8->selection;
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
    func_80071824(arg0, func_8003288C);
}

void func_80032984(MainMenuSelectionArrowActor *arg0) {
    s32 temp = func_80043040(D_80112172);

    func_8000F030(arg0->common.x, arg0->common.y, temp, 3, 0x20, 0x20, 0, 0);
}

void func_800329E0(MainMenuSelectionArrowActor *arg0) {
    if (D_801235B8->selection != 1) {
        arg0->common.x = -0x34;
    } else {
        arg0->common.x = -0x3C;
    }
    arg0->common.y = (D_801235B8->selection << 4) + 0x20;
    func_800483FC(&D_80124868, func_80032984, arg0);
}

void func_80032A50(MainMenuSelectionArrowActor *arg0) {
    arg0->common.y = 0x20;
    func_80071824(arg0, func_800329E0);
}
