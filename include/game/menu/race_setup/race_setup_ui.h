#ifndef RACE_SETUP_UI_H
#define RACE_SETUP_UI_H

#include "common.h"
#include "game/menu/renderer/menu_renderer.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 alpha;
    /* 0x1E */ u8 state;
    /* 0x1F */ u8 timer;
    /* 0x20 */ struct MenuItemActor *child;
} MenuIntroActor;

typedef struct MenuItemActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ u8 state;
    /* 0x1D */ u8 pad1D[3];
    /* 0x20 */ struct MenuItemActor *child;
} MenuItemActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    union {
        /* 0x1C */ s16 alpha;
        /* 0x1C */ u16 uAlpha;
    };
} FadeItemActor;

typedef struct {
    /* 0x00 */ s16 x0;
    /* 0x02 */ s16 y0;
    /* 0x04 */ s16 x1;
    /* 0x06 */ s16 y1;
} Rect;

extern s16 gRaceSetupSavePanelRects[2][4];

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    union {
        /* 0x18 */ Rect rects[2];
        /* 0x18 */ s16 rectCoords[8];
    };
    /* 0x28 */ u16 stepLimit;
    /* 0x2A */ u16 stepIncrement;
    /* 0x2C */ u16 stepAccumulator;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ u8 frame;
} RectListActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 panelX[4];
    /* 0x20 */ s16 panelY[4];
    /* 0x28 */ s16 iconOffsetX;
    /* 0x2A */ s16 iconOffsetY;
    /* 0x2C */ s16 badgeOffsetX;
    /* 0x2E */ s16 badgeOffsetY;
} TitleMenuIconStripActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x[4];
    /* 0x20 */ s16 y[4];
    /* 0x28 */ s16 topY[4];
    /* 0x30 */ s16 slideOffset[4];
    /* 0x38 */ u16 selection[4];
    /* 0x40 */ u16 alpha[4];
    /* 0x48 */ s16 alphaTimer[4];
} TitleMenuTransitionActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x[4];
    /* 0x20 */ s16 y[4];
    /* 0x28 */ s16 alpha[4];
    /* 0x30 */ u8 frame;
    /* 0x31 */ u8 statusTransitionStates[4];
    /* 0x35 */ u8 pad35;
    /* 0x36 */ u16 nextStatusCodes[4];
} TitleMenuWidgetActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x[4];
    /* 0x20 */ s16 y[4];
} TitleMenuWidgetItemView;

typedef struct {
    /* 0x00 */ u16 tilemapWidth;
    /* 0x02 */ u16 tilemapHeight;
    /* 0x04 */ u16 tileWidth;
    /* 0x06 */ u16 tileHeight;
    /* 0x08 */ u16 tileCount;
    /* 0x0A */ u16 tilemapOffset;
    /* 0x0C */ u16 paletteOffset;
    /* 0x0E */ u16 imageOffset;
    /* 0x10 */ MenuTilemapTile tiles[1];
} MenuTilemapSpriteAsset;

typedef struct {
    /* 0x00 */ MenuRenderSprite render;
    /* 0x24 */ s16 unk24;
} MenuTilemapSprite;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ MenuTilemapSprite sprite;
} SpriteActor;

void drawRaceSetupPlayerCountPrompt(MenuIntroActor *arg0);
void updateRaceSetupPlayerCountPrompt(MenuIntroActor *arg0);
void initRaceSetupPlayerCountPrompt(void *arg0);
void drawRaceSetupOnePlayerOption(void *arg0);
void updateRaceSetupOnePlayerOption(MenuItemActor *arg0);
void initRaceSetupOnePlayerOption(MenuItemActor *arg0);
void drawRaceSetupTwoPlayerOption(MenuItemActor *arg0);
void updateRaceSetupTwoPlayerOption(MenuItemActor *arg0);
void initRaceSetupTwoPlayerOption(MenuItemActor *arg0);
void drawRaceSetupThreePlayerOption(void *arg0);
void updateRaceSetupThreePlayerOption(MenuItemActor *arg0);
void initRaceSetupThreePlayerOption(MenuItemActor *arg0);
void drawRaceSetupFourPlayerOption(void *arg0);
void updateRaceSetupFourPlayerOption(MenuItemActor *arg0);
void initRaceSetupFourPlayerOption(MenuItemActor *arg0);
void drawRaceSetupPlayerCountCursor(void *arg0);
void updateRaceSetupPlayerCountCursor(FadeItemActor *arg0);
void initRaceSetupPlayerCountCursor(FadeItemActor *arg0);
void drawRaceSetupSavePlayerPanels(TitleMenuWidgetItemView *arg0);
void updateRaceSetupSavePanelFrame(RectListActor *arg0);
void initRaceSetupSavePanelFrame(RectListActor *arg0);
void drawRaceSetupSavePanelIcons(TitleMenuIconStripActor *arg0);
void updateRaceSetupSavePanelIcons(void *arg0);
void initRaceSetupSavePanelIcons(RectListActor *arg0);
void drawRaceSetupSaveStatusWidgets(TitleMenuWidgetActor *arg0);
void updateRaceSetupSaveStatusWidgets(TitleMenuWidgetActor *arg0);
void initRaceSetupSaveStatusWidgets(TitleMenuWidgetActor *arg0);
void drawRaceSetupSaveChoicePrompts(TitleMenuTransitionActor *arg0);
void updateRaceSetupSaveChoicePrompts(TitleMenuTransitionActor *arg0);
void initRaceSetupSaveChoicePrompts(RectListActor *arg0);
void drawMenuIconTilemapSpriteActor(void *arg0);
void updateMenuIconTilemapSpriteActor(SpriteActor *arg0);
void initMenuIconTilemapSpriteActor(SpriteActor *arg0);
void initMenuTilemapSprite(MenuTilemapSprite *sprite, MenuTilemapSpriteAsset *asset);
void n_alSynFreeFX(s32 arg0);

#endif
