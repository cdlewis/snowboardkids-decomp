#ifndef CHARACTER_SELECT_UI_H
#define CHARACTER_SELECT_UI_H

#include "common.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/race/player/race_player_input.h"

typedef struct {
    /* 0x00 */ CallbackTaskHeader taskHeader;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 alpha;
    /* 0x1E */ s16 confirmationCursorAlpha;
    /* 0x20 */ u8 state;
    /* 0x21 */ u8 frame;
    /* 0x22 */ u8 mode;
    /* 0x23 */ u8 bounceTimer;
} CharacterSelectUiBannerActor;

typedef struct {
    /* 0x00 */ CallbackTaskHeader taskHeader;
    /* 0x18 */ s16 x[RACE_PLAYER_COUNT];
    /* 0x20 */ s16 y[RACE_PLAYER_COUNT];
    /* 0x28 */ u16 frameLimit;
    /* 0x2A */ u16 frameStep;
    /* 0x2C */ u16 frameCounter;
    /* 0x2E */ u16 unused2E;
    /* 0x30 */ u8 transitionState;
} CharacterSelectUiPlayerPanelFrameActor;

typedef struct {
    /* 0x00 */ CallbackTaskHeader taskHeader;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 targetX;
    /* 0x1E */ s16 targetY;
    /* 0x20 */ s16 speedX;
    /* 0x22 */ u8 leftSecretSlotUnlocked;
    /* 0x23 */ u8 rightSecretSlotUnlocked;
    /* 0x24 */ u8 state;
    /* 0x25 */ u8 timer;
    /* 0x26 */ s16 unused26;
} CharacterSelectUiRosterIconActor;

typedef struct {
    /* 0x00 */ CallbackTaskHeader taskHeader;
    /* 0x18 */ s16 x[RACE_PLAYER_COUNT];
    /* 0x20 */ s16 y[RACE_PLAYER_COUNT];
    /* 0x28 */ u8 unused28[RACE_PLAYER_COUNT];
    /* 0x2C */ u8 blinkTimer[RACE_PLAYER_COUNT];
    /* 0x30 */ u8 blinkVisible[RACE_PLAYER_COUNT];
} CharacterSelectUiPlayerStatsPanelActor;

typedef struct {
    /* 0x00 */ CallbackTaskHeader taskHeader;
    /* 0x18 */ s16 x[RACE_PLAYER_COUNT];
    /* 0x20 */ s16 y[RACE_PLAYER_COUNT];
    /* 0x28 */ s16 xDistance[RACE_PLAYER_COUNT];
    /* 0x30 */ s16 yDistance[RACE_PLAYER_COUNT];
    /* 0x38 */ s16 accumulator[RACE_PLAYER_COUNT];
    /* 0x40 */ s16 tileSize[RACE_PLAYER_COUNT];
    /* 0x48 */ s16 xDirection[RACE_PLAYER_COUNT];
    /* 0x50 */ s16 timer[RACE_PLAYER_COUNT];
    /* 0x58 */ s16 stepCount[RACE_PLAYER_COUNT];
    /* 0x60 */ u8 axis[RACE_PLAYER_COUNT];
    /* 0x64 */ u8 divisor[RACE_PLAYER_COUNT];
    /* 0x68 */ u8 state[RACE_PLAYER_COUNT];
} CharacterSelectUiSelectedCharacterTokenActor;

typedef struct {
    /* 0x00 */ CallbackTaskHeader taskHeader;
    /* 0x18 */ s16 x[RACE_PLAYER_COUNT];
    /* 0x20 */ s16 y;
    /* 0x22 */ s16 baseX;
    /* 0x24 */ s16 scale;
    /* 0x26 */ u8 mode;
    /* 0x27 */ u8 timer;
} CharacterSelectUiPlayerCursorActor;

typedef struct {
    /* 0x0 */ u8 speed;
    /* 0x1 */ u8 turn;
    /* 0x2 */ u8 trick;
} CharacterSelectUiCharacterStats;

typedef struct {
    MenuGlyphScript confirmationText[2][0x1C];
    MenuGlyphScript unusedText[0x14];
} CharacterSelectUiConfirmationBannerText;

extern CharacterSelectUiRosterIconActor *D_8010ADE0;
extern CharacterSelectUiPlayerPanelFrameActor *D_8010ADE4;

void drawCharacterSelectConfirmationBanner(CharacterSelectUiBannerActor *arg0);
void updateCharacterSelectConfirmationBanner(CharacterSelectUiBannerActor *arg0);
void initCharacterSelectConfirmationBanner(CharacterSelectUiBannerActor *arg0);
void drawCharacterSelectPlayerPanelFrames(CharacterSelectUiPlayerPanelFrameActor *arg0);
void updateCharacterSelectPlayerPanelFrames(CharacterSelectUiPlayerPanelFrameActor *arg0);
void initCharacterSelectPlayerPanelFrames(CharacterSelectUiPlayerPanelFrameActor *arg0);
void drawCharacterSelectRosterIcons(CharacterSelectUiRosterIconActor *arg0);
void updateCharacterSelectRosterIcons(CharacterSelectUiRosterIconActor *arg0);
void initCharacterSelectRosterIcons(CharacterSelectUiRosterIconActor *arg0);
void drawCharacterSelectPlayerCursorMarkers(CharacterSelectUiPlayerCursorActor *arg0);
void updateCharacterSelectPlayerCursorMarkers(CharacterSelectUiPlayerCursorActor *arg0);
void initCharacterSelectPlayerCursorMarkers(CharacterSelectUiPlayerCursorActor *arg0);
void drawCharacterSelectPlayerStatsPanels(CharacterSelectUiPlayerStatsPanelActor *arg0);
void updateCharacterSelectPlayerStatsPanels(CharacterSelectUiPlayerStatsPanelActor *statsPanels);
void initCharacterSelectPlayerStatsPanels(CharacterSelectUiPlayerStatsPanelActor *arg0);
void drawCharacterSelectSelectedCharacterTokens(CharacterSelectUiSelectedCharacterTokenActor *arg0);
void updateCharacterSelectSelectedCharacterTokens(CharacterSelectUiSelectedCharacterTokenActor *arg0);
void initCharacterSelectSelectedCharacterTokens(CharacterSelectUiSelectedCharacterTokenActor *arg0);

#endif
