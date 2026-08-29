#ifndef RACE_SPLITSCREEN_SELECT_UI_H
#define RACE_SPLITSCREEN_SELECT_UI_H

#include "common.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"

typedef struct RaceSplitscreenSelectMenuState {
    /* 0x00 */ u8 state;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ s16 alphaTarget;
    /* 0x04 */ s16 portraitAlpha;
} RaceSplitscreenSelectMenuState;

typedef struct RaceSplitscreenSelectOptionIconActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x[5];
    /* 0x22 */ s16 y;
    /* 0x24 */ u8 state;
    /* 0x25 */ u8 spawnTimer;
    /* 0x26 */ u8 visibleCount;
} RaceSplitscreenSelectOptionIconActor;

typedef struct RaceSplitscreenSelectCornerSpriteActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ u8 state;
    /* 0x1D */ u8 pad1D;
} RaceSplitscreenSelectCornerSpriteActor;

typedef struct RaceSplitscreenSelectOption0FrameActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 unused1C;
    /* 0x1E */ s16 unused1E;
    /* 0x20 */ s16 optionIndex;
    /* 0x22 */ u8 state;
    /* 0x23 */ u8 cornerSpawnTimer;
} RaceSplitscreenSelectOption0FrameActor;

typedef struct RaceSplitscreenSelectOptionFrameActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 optionIndex;
    /* 0x1E */ u8 state;
    /* 0x1F */ u8 unusedTimer;
} RaceSplitscreenSelectOptionFrameActor;

typedef struct RaceSplitscreenSelectAnimatedWidgetActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 alpha;
    /* 0x1E */ u8 state;
    /* 0x1F */ u8 animationTimer;
} RaceSplitscreenSelectAnimatedWidgetActor;

typedef struct RaceSplitscreenSelectFrameTiles {
    /* 0x00 */ u16 centerTiles[16];
    /* 0x20 */ u16 rightEdgeTiles[2];
    /* 0x24 */ u16 bottomEdgeTiles[2];
    /* 0x28 */ u16 cornerTile;
} RaceSplitscreenSelectFrameTiles;

typedef MenuGlyphScript RaceSplitscreenSelectDescription[0x46];

typedef struct RaceSplitscreenSelectDescriptionScriptTable {
    /* 0x000 */ RaceSplitscreenSelectDescription descriptions[6];
    /* 0x348 */ MenuGlyphScript title[10];
} RaceSplitscreenSelectDescriptionScriptTable;

extern RaceSplitscreenSelectMenuState gRaceSplitscreenSelectCursorTarget;
extern RaceSplitscreenSelectFrameTiles gRaceSplitscreenSelectFrameTiles[5];
extern RaceSplitscreenSelectDescriptionScriptTable gRaceSplitscreenSelectDescriptionScripts;

void drawRaceSplitscreenSelectPlayerCountIcons(RaceSplitscreenSelectOptionIconActor *arg0);
void updateRaceSplitscreenSelectPlayerCountIcons(RaceSplitscreenSelectOptionIconActor *arg0);
void initRaceSplitscreenSelectPlayerCountIcons(RaceSplitscreenSelectOptionIconActor *arg0);
void drawRaceSplitscreenSelectCornerSprites(RaceSplitscreenSelectCornerSpriteActor *arg0);
void updateRaceSplitscreenSelectCornerSprites(RaceSplitscreenSelectCornerSpriteActor *arg0);
void initRaceSplitscreenSelectCornerSprites(RaceSplitscreenSelectCornerSpriteActor *arg0);
void drawRaceSplitscreenSelectOption0Frame(RaceSplitscreenSelectOption0FrameActor *arg0);
void updateRaceSplitscreenSelectOption0Frame(RaceSplitscreenSelectOption0FrameActor *arg0);
void initRaceSplitscreenSelectOption0Frame(RaceSplitscreenSelectOption0FrameActor *arg0);
void drawRaceSplitscreenSelectOption1Frame(RaceSplitscreenSelectOptionFrameActor *arg0);
void updateRaceSplitscreenSelectOption1Frame(RaceSplitscreenSelectOptionFrameActor *arg0);
void initRaceSplitscreenSelectOption1Frame(RaceSplitscreenSelectOptionFrameActor *arg0);
void drawRaceSplitscreenSelectOption2Frame(RaceSplitscreenSelectOptionFrameActor *arg0);
void updateRaceSplitscreenSelectOption2Frame(RaceSplitscreenSelectOptionFrameActor *arg0);
void initRaceSplitscreenSelectOption2Frame(RaceSplitscreenSelectOptionFrameActor *arg0);
void drawRaceSplitscreenSelectOption3Frame(RaceSplitscreenSelectOptionFrameActor *arg0);
void updateRaceSplitscreenSelectOption3Frame(RaceSplitscreenSelectOptionFrameActor *arg0);
void initRaceSplitscreenSelectOption3Frame(RaceSplitscreenSelectOptionFrameActor *arg0);
void drawRaceSplitscreenSelectOption4Frame(RaceSplitscreenSelectOptionFrameActor *arg0);
void updateRaceSplitscreenSelectOption4Frame(RaceSplitscreenSelectOptionFrameActor *arg0);
void initRaceSplitscreenSelectOption4Frame(RaceSplitscreenSelectOptionFrameActor *arg0);
void drawRaceSplitscreenSelectCursor(RaceSplitscreenSelectAnimatedWidgetActor *arg0);
void updateRaceSplitscreenSelectCursor(RaceSplitscreenSelectAnimatedWidgetActor *arg0);
void initRaceSplitscreenSelectCursor(RaceSplitscreenSelectAnimatedWidgetActor *arg0);
void drawRaceSplitscreenSelectPortrait(RaceSplitscreenSelectAnimatedWidgetActor *arg0);
void updateRaceSplitscreenSelectPortrait(RaceSplitscreenSelectAnimatedWidgetActor *arg0);
void initRaceSplitscreenSelectPortrait(RaceSplitscreenSelectAnimatedWidgetActor *arg0);
void drawRaceSplitscreenSelectArrowPrompt(RaceSplitscreenSelectAnimatedWidgetActor *arg0);
void updateRaceSplitscreenSelectArrowPrompt(RaceSplitscreenSelectAnimatedWidgetActor *arg0);
void initRaceSplitscreenSelectArrowPrompt(RaceSplitscreenSelectAnimatedWidgetActor *arg0);
void drawRaceSplitscreenSelectEntryFee(RaceSplitscreenSelectAnimatedWidgetActor *arg0);
void updateRaceSplitscreenSelectEntryFee(RaceSplitscreenSelectAnimatedWidgetActor *arg0);
void initRaceSplitscreenSelectEntryFee(RaceSplitscreenSelectAnimatedWidgetActor *arg0);
void drawMenuPanelBackdrop(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

#endif
