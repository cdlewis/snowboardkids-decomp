#ifndef RACE_TYPE_SELECT_UI_H
#define RACE_TYPE_SELECT_UI_H

#include "common.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"

typedef struct RaceTypeSelectCursorState {
    /* 0x00 */ s16 alpha;
    /* 0x02 */ u8 state;
} RaceTypeSelectCursorState;

typedef struct RaceTypeSelectOptionIconActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x[5];
    /* 0x22 */ s16 y;
    /* 0x24 */ u8 state;
    /* 0x25 */ u8 spawnTimer;
    /* 0x26 */ u8 visibleCount;
} RaceTypeSelectOptionIconActor;

typedef struct RaceTypeSelectCornerSpriteActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ u8 state;
    /* 0x1D */ u8 pad1D;
} RaceTypeSelectCornerSpriteActor;

typedef struct RaceTypeSelectOption0FrameActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 unused1C;
    /* 0x1E */ s16 unused1E;
    /* 0x20 */ s16 optionIndex;
    /* 0x22 */ u8 state;
    /* 0x23 */ u8 cornerSpawnTimer;
} RaceTypeSelectOption0FrameActor;

typedef struct RaceTypeSelectOptionFrameActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 optionIndex;
    /* 0x1E */ u8 state;
    /* 0x1F */ u8 unusedTimer;
} RaceTypeSelectOptionFrameActor;

typedef struct RaceTypeSelectExitFrameActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 unused1C;
    /* 0x1E */ u16 bounceStep;
    /* 0x20 */ u8 state;
    /* 0x21 */ u8 unusedTimer;
} RaceTypeSelectExitFrameActor;

typedef struct RaceTypeSelectAnimatedWidgetActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 alpha;
    /* 0x1E */ u8 state;
    /* 0x1F */ u8 animationTimer;
} RaceTypeSelectAnimatedWidgetActor;

typedef struct RaceTypeSelectFrameTiles {
    /* 0x00 */ u16 centerTiles[16];
    /* 0x20 */ u16 rightEdgeTiles[2];
    /* 0x24 */ u16 bottomEdgeTiles[2];
    /* 0x28 */ u16 cornerTile;
} RaceTypeSelectFrameTiles;

typedef struct RaceTypeSelectPartialFrameTiles {
    /* 0x00 */ u16 centerTiles[16];
    /* 0x20 */ u16 rightEdgeTiles[2];
    /* 0x24 */ u16 bottomEdgeTiles[2];
} RaceTypeSelectPartialFrameTiles;

typedef struct RaceTypeSelectFrameTileTable {
    /* 0x00 */ RaceTypeSelectFrameTiles optionFrames[3];
    /* 0x7E */ RaceTypeSelectPartialFrameTiles exitFrame;
    /* 0xA6 */ u16 exitFrameCornerTile;
} RaceTypeSelectFrameTileTable;

typedef MenuGlyphScript RaceTypeSelectDescription[0x46];

typedef struct RaceTypeSelectDescriptionScriptTable {
    /* 0x000 */ RaceTypeSelectDescription descriptions[4];
    /* 0x230 */ MenuGlyphScript title[0x14];
} RaceTypeSelectDescriptionScriptTable;

extern RaceTypeSelectCursorState gRaceTypeSelectCursorTarget;
extern u8 gRaceTypeSelectCursorAnimState;
extern RaceTypeSelectFrameTileTable gRaceTypeSelectFrameTileMaps;
extern RaceTypeSelectDescriptionScriptTable gRaceTypeSelectPortraitScripts;

void drawRaceTypeSelectOptionIcons(RaceTypeSelectOptionIconActor *arg0);
void updateRaceTypeSelectOptionIcons(RaceTypeSelectOptionIconActor *arg0);
void initRaceTypeSelectOptionIcons(RaceTypeSelectOptionIconActor *arg0);
void drawRaceTypeSelectCornerSprites(RaceTypeSelectCornerSpriteActor *arg0);
void updateRaceTypeSelectCornerSprites(RaceTypeSelectCornerSpriteActor *arg0);
void initRaceTypeSelectCornerSprites(RaceTypeSelectCornerSpriteActor *arg0);
void drawRaceTypeSelectOption0Frame(RaceTypeSelectOption0FrameActor *arg0);
void updateRaceTypeSelectOption0Frame(RaceTypeSelectOption0FrameActor *arg0);
void initRaceTypeSelectOption0Frame(RaceTypeSelectOption0FrameActor *arg0);
void drawRaceTypeSelectOption1Frame(RaceTypeSelectOptionFrameActor *arg0);
void updateRaceTypeSelectOption1Frame(RaceTypeSelectOptionFrameActor *arg0);
void initRaceTypeSelectOption1Frame(RaceTypeSelectOptionFrameActor *arg0);
void drawRaceTypeSelectOption2Frame(RaceTypeSelectOptionFrameActor *arg0);
void updateRaceTypeSelectOption2Frame(RaceTypeSelectOptionFrameActor *arg0);
void initRaceTypeSelectOption2Frame(RaceTypeSelectOptionFrameActor *arg0);
void drawRaceTypeSelectOption3Frame(RaceTypeSelectExitFrameActor *arg0);
void updateRaceTypeSelectOption3Frame(RaceTypeSelectExitFrameActor *arg0);
void initRaceTypeSelectOption3Frame(RaceTypeSelectExitFrameActor *arg0);
void drawRaceTypeSelectCursor(RaceTypeSelectAnimatedWidgetActor *arg0);
void updateRaceTypeSelectCursor(RaceTypeSelectAnimatedWidgetActor *arg0);
void initRaceTypeSelectCursor(RaceTypeSelectAnimatedWidgetActor *arg0);
void drawRaceTypeSelectPortrait(RaceTypeSelectAnimatedWidgetActor *arg0);
void updateRaceTypeSelectPortrait(RaceTypeSelectAnimatedWidgetActor *arg0);
void initRaceTypeSelectPortrait(RaceTypeSelectAnimatedWidgetActor *arg0);
void drawRaceTypeSelectArrowPrompt(RaceTypeSelectAnimatedWidgetActor *arg0);
void updateRaceTypeSelectArrowPrompt(RaceTypeSelectAnimatedWidgetActor *arg0);
void initRaceTypeSelectArrowPrompt(RaceTypeSelectAnimatedWidgetActor *arg0);
void drawRaceTypeSelectEntryFee(RaceTypeSelectAnimatedWidgetActor *arg0);
void updateRaceTypeSelectEntryFee(RaceTypeSelectAnimatedWidgetActor *arg0);
void initRaceTypeSelectEntryFee(RaceTypeSelectAnimatedWidgetActor *arg0);

#endif
