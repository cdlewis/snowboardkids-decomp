#ifndef CHARACTER_SELECT_UI_H
#define CHARACTER_SELECT_UI_H

#include "common.h"

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 alpha;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ u8 state;
    /* 0x21 */ u8 frame;
    /* 0x22 */ u8 mode;
    /* 0x23 */ u8 bounceTimer;
} CharacterSelectUiBannerActor;

typedef struct {
    u8 pad0[0x18];
    union {
        struct {
            /* 0x18 */ s16 x;
            u8 pad1A[6];
            /* 0x20 */ s16 y;
        };
        struct {
            /* 0x18 */ s16 panelX[4];
            /* 0x20 */ s16 panelY[4];
        };
    };
} CharacterSelectUiPlayerPanelFrameActor;

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 targetX;
    /* 0x1E */ s16 targetY;
    /* 0x20 */ s16 speedX;
    union {
        /* 0x22 */ s16 speedY;
        struct {
            /* 0x22 */ u8 playerFlags;
            /* 0x23 */ u8 unk23;
        };
    };
    /* 0x24 */ u8 state;
    /* 0x25 */ u8 timer;
    /* 0x26 */ s16 unk26;
} CharacterSelectUiRosterIconActor;

typedef union {
    s16 target[4];
    struct {
        u8 pad0[4];
        u8 statsBlinkTimer[4];
    };
} CharacterSelectUiPanelTargetX;

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ s16 x[4];
    /* 0x20 */ s16 y[4];
    /* 0x28 */ CharacterSelectUiPanelTargetX targetX;
    union {
        s16 target[4];
        u8 statsBlinkVisible[4];
        u8 mode;
    } targetY;
    /* 0x38 */ s16 accumulator[4];
    /* 0x40 */ u16 tileSize[4];
    /* 0x48 */ s16 xDirection[4];
    /* 0x50 */ s16 timer[4];
    /* 0x58 */ s16 stepCount[4];
    /* 0x60 */ u8 axis[4];
    /* 0x64 */ u8 divisor[4];
    /* 0x68 */ u8 state[4];
} CharacterSelectUiPanelActor;

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ s16 x[4];
    /* 0x20 */ s16 y[4];
    /* 0x28 */ s16 xDistance[4];
    /* 0x30 */ s16 yDistance[4];
    /* 0x38 */ s16 accumulator[4];
    union {
        /* 0x40 */ u16 tileSize[4];
        /* 0x40 */ s16 signedTileSize[4];
    };
    /* 0x48 */ s16 xDirection[4];
    /* 0x50 */ s16 timer[4];
    /* 0x58 */ s16 stepCount[4];
    /* 0x60 */ u8 axis[4];
    /* 0x64 */ u8 divisor[4];
    /* 0x68 */ u8 state[4];
} CharacterSelectUiSelectedCharacterTokenActor;

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ s16 x[4];
    /* 0x20 */ s16 y;
    /* 0x22 */ s16 baseX;
    /* 0x24 */ s16 scale;
    /* 0x26 */ u8 mode;
    /* 0x27 */ u8 timer;
} CharacterSelectUiPlayerCursorActor;

void drawCharacterSelectConfirmationBanner(CharacterSelectUiBannerActor *arg0);
void updateCharacterSelectConfirmationBanner(CharacterSelectUiBannerActor *arg0);
void initCharacterSelectConfirmationBanner(CharacterSelectUiBannerActor *arg0);
void drawCharacterSelectPlayerPanelFrames(CharacterSelectUiPlayerPanelFrameActor *arg0);
void updateCharacterSelectPlayerPanelFrames(CharacterSelectUiPanelActor *arg0);
void initCharacterSelectPlayerPanelFrames(CharacterSelectUiPanelActor *arg0);
void drawCharacterSelectRosterIcons(CharacterSelectUiRosterIconActor *arg0);
void updateCharacterSelectRosterIcons(CharacterSelectUiRosterIconActor *arg0);
void initCharacterSelectRosterIcons(CharacterSelectUiRosterIconActor *arg0);
void drawCharacterSelectPlayerCursorMarkers(CharacterSelectUiPlayerCursorActor *arg0);
void updateCharacterSelectPlayerCursorMarkers(CharacterSelectUiPlayerCursorActor *arg0);
void initCharacterSelectPlayerCursorMarkers(CharacterSelectUiPlayerCursorActor *arg0);
void drawCharacterSelectPlayerStatsPanels(CharacterSelectUiPanelActor *arg0);
void updateCharacterSelectPlayerStatsPanels(CharacterSelectUiPanelActor *statsPanels);
void initCharacterSelectPlayerStatsPanels(CharacterSelectUiPanelActor *arg0);
void drawCharacterSelectSelectedCharacterTokens(CharacterSelectUiSelectedCharacterTokenActor *arg0);
void updateCharacterSelectSelectedCharacterTokens(CharacterSelectUiSelectedCharacterTokenActor *arg0);
void initCharacterSelectSelectedCharacterTokens(CharacterSelectUiSelectedCharacterTokenActor *arg0);

#endif
