#include "common.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/character_select/character_select_menu.h"
#include "game/menu/character_select/character_select_ui.h"
#include "game/race/player/race_player_input.h"

#define CHARACTER_SELECT_UI_UNUSED_HANDLE (gAssetHandles[0x1F])
#define CHARACTER_SELECT_UI_PLAYER_FRAME_HANDLE (gAssetHandles[0x21])
#define CHARACTER_SELECT_UI_BANNER_TEXTURE_HANDLE (gAssetHandles[0x29])
#define CHARACTER_SELECT_UI_AVAILABLE_CHARACTER_ICON_HANDLE (gAssetHandles[0x1F])
#define CHARACTER_SELECT_TOKEN_IDLE 0
#define CHARACTER_SELECT_TOKEN_START 1
#define CHARACTER_SELECT_TOKEN_FLYING 2
#define CHARACTER_SELECT_TOKEN_LANDED 3

typedef struct {
    u8 pad0[0x4B];
    /* 0x4B */ u8 flags;
    u8 pad4C[0x78F8 - 0x4C];
} RacePlayerState;

typedef struct {
    u8 pad0[0x24];
    /* 0x24 */ u8 playerFrameReady;
} CharacterSelectUiPlayerPanelFrameController;

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ s16 x[RACE_PLAYER_COUNT];
    /* 0x20 */ s16 y[RACE_PLAYER_COUNT];
    /* 0x28 */ u16 frameLimit;
    /* 0x2A */ u16 frameStep;
    /* 0x2C */ u16 frameCounter;
    u8 pad2E[2];
    /* 0x30 */ u8 mode;
} CharacterSelectUiPanelTransitionActor;

typedef struct {
    /* 0x00 */ u8 phase;
    /* 0x01 */ u8 exitMode;
    /* 0x02 */ u8 readyCount;
    /* 0x03 */ u8 pad3;
    u8 pad4[4];
    /* 0x08 */ s16 fade;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ u8 confirmSelection;
    /* 0x0D */ u8 unkD;
} CharacterSelectUiSharedState;

typedef struct {
    u8 pad0[0x26];
    /* 0x26 */ u8 unk26;
} CharacterSelectUiPanelController;

typedef struct {
    /* 0x0 */ u8 speed;
    /* 0x1 */ u8 turn;
    /* 0x2 */ u8 trick;
} CharacterSelectUiCharacterStats;

extern void addRenderCallback(void *, void *, void *);
extern s8 D_8010AE52;
extern u8 D_8010AE52_state;
extern u8 D_8010AE51;
extern CharacterSelectUiPlayerPanelFrameController *D_8010ADE0;
extern void *D_8010ADE4;
extern CharacterSelectUiPanelController *D_8010ADE8;
extern s16 D_8010AE58;
extern s32 gMenuFlowState;
extern void *gMenuRenderCallbackList;
extern u8 gPlayerCount;
extern s8 D_8010AE64[];
extern s16 gAssetHandles[];
extern u8 gCharacterSelectConfirmationBannerText[];
extern CharacterSelectUiCharacterStats gCharacterSelectCharacterStats[];
extern u16 gCharacterSelectCharacterStatLabels[];
extern const char gCharacterSelectPlayerNumberFormat[];
extern const char gCharacterSelectCharacterStatFormat[];
extern RacePlayerState gGameSaveDataBuffer[];
extern u8 D_8010AE5E;
extern u8 D_8010AE5F;
extern u16 gCharacterSelectPlayerMarkerTiles[];

void drawCharacterSelectConfirmationBanner(CharacterSelectUiBannerActor *arg0) {
    s32 i;
    s32 j;
    unsigned int alpha;
    s32 selected;
    s32 limit;
    CharacterSelectUiBannerActor *actor;

    actor = arg0;
    if (actor->state != 3) {
        drawMenuSpriteWithAlpha((s16)(actor->x - 4), (s16)(actor->y - 4), getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_BANNER_TEXTURE_HANDLE), 2, 0x20,
                      0x20, 0, actor->alpha, 0);
        drawMenuSpriteWithAlpha((s16)(actor->x + 0xD4), (s16)(actor->y - 4), getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_BANNER_TEXTURE_HANDLE),
                      4, 0x20, 0x20, 0, actor->alpha, 0);
        i = 0;
        do {
            drawMenuSpriteWithAlpha((s16)(actor->x + i), (s16)(actor->y - 4), getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_BANNER_TEXTURE_HANDLE),
                          3, 0x20, 0x20, 0, actor->alpha, 0);
            drawMenuSpriteWithAlpha((s16)(actor->x + i), (s16)(actor->y + 0x24), getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_BANNER_TEXTURE_HANDLE),
                          8, 0x20, 0x20, 0, actor->alpha, 0);
            i += 0x10;
        } while (i < 0xE0);
        drawMenuSpriteWithAlpha((s16)(actor->x - 4), (s16)(actor->y + 0x24), getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_BANNER_TEXTURE_HANDLE),
                      7, 0x20, 0x20, 0, actor->alpha, 0);
        drawMenuSpriteWithAlpha((s16)(actor->x + 0xD4), (s16)(actor->y + 0x24),
                      getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_BANNER_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, actor->alpha, 0);
        i = (actor->state == 4) * 0;
        limit = 0xE0;
        do {
            drawMenuSpriteWithAlpha((s16)(actor->x - 4), (s16)(actor->y + i), getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_BANNER_TEXTURE_HANDLE),
                          5, 0x20, 0x20, 0, actor->alpha, 0);
            drawMenuSpriteWithAlpha((s16)(actor->x + 0xD4), (s16)(actor->y + i), getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_BANNER_TEXTURE_HANDLE),
                          6, 0x20, 0x20, 0, actor->alpha, 0);
            j = 0;
            do {
                drawMenuSpriteWithAlpha((s16)(actor->x + j), (s16)(actor->y + i),
                              getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_BANNER_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, actor->alpha, 0);
                j += 0x10;
            } while (j != limit);
            i += 0x10;
        } while (i <= 0x2F);
        if (actor->state < 3) {
            selected = 0;
        } else {
            selected = 1;
        }
        drawMenuGlyphScript(actor->x, actor->y, &gCharacterSelectConfirmationBannerText[selected * 0x38], 0, actor->alpha, 0);
        if (actor->state == 4) {
            if (actor->alpha != 0x100) {
                alpha = actor->alpha & 0xFFFF;
            } else {
                if (actor->mode != 0) {
                    alpha = 0x60;
                } else {
                    alpha = 0x100;
                }
            }
            drawMenuSpriteWithAlpha((s16)(actor->x + 0x4C), (s16)(actor->y + 0x10),
                          getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_PLAYER_FRAME_HANDLE), 0x17, 0x20, 0x20, 0, alpha, 0);
            if (actor->alpha < 0x60) {
                alpha = actor->alpha & 0xFFFF;
            } else {
                if (actor->mode != 0) {
                    alpha = 0x100;
                } else {
                    alpha = 0x60;
                }
            }
            drawMenuSpriteWithAlpha((s16)(actor->x + 0x4C), (s16)(actor->y + 0x20),
                          getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_PLAYER_FRAME_HANDLE), 0x18, 0x20, 0x20, 0, alpha, 0);
            if (actor->alpha != 0x100) {
                alpha = actor->alpha & 0xFFFF;
                ;
            } else {
                alpha = (u16)actor->unk1E;
            }
            drawMenuSpriteWithAlpha((s16)(actor->x + 0x4C), (s16)((actor->y + (actor->mode * 0x10)) + 0x10),
                          getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_PLAYER_FRAME_HANDLE), 0x12, 0x20, 0x20, 0, alpha, 0);
        }
        if (actor->state == 1) {
            drawMenuSprite((s16)(actor->x + 0xD0), (s16)(actor->y + 0x20),
                          getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_PLAYER_FRAME_HANDLE), ((actor->frame >= 8) + 5) & 0xFFFF, 0x20,
                          0x20, 0, 0);
        }
    }
}

void updateCharacterSelectConfirmationBanner(CharacterSelectUiBannerActor *arg0) {
    s16 alpha;
    s32 state;
    CharacterSelectUiBannerActor *actor;

    actor = arg0;
    if (gCharacterSelectHudState.phase != arg0->state) {
        arg0->state = gCharacterSelectHudState.phase;
        arg0->alpha = gCharacterSelectHudState.fade;
        arg0->bounceTimer = gCharacterSelectHudState.unkD;
        arg0->unk1E = gCharacterSelectHudState.unkA;
    }

    if (gCharacterSelectHudState.confirmSelection != actor->mode) {
        actor->mode = gCharacterSelectHudState.confirmSelection;
    }

    alpha = actor->alpha;
    state = actor->state;
    if ((0x100 != (alpha ^ 0)) && (state != 0)) {
        if ((state == 4) || (state == 1)) {
            actor->alpha = alpha + 0x30;
            if (actor->alpha >= 0x100) {
                actor->alpha = 0x100;
            }
        } else {
            actor->alpha = alpha - 0x30;
            if (actor->alpha <= 0) {
                actor->alpha = 0;
            }
        }
    } else {
        switch (state) {
        case 0:
        case 3:
            break;
        case 1:
            actor->frame = (actor->frame + 1) & 0xF;
            break;
        case 2:
            actor->alpha = alpha - 0x25;
            if (actor->alpha <= 0) {
                actor->alpha = 0;
            }
            actor->state = 3;
            gCharacterSelectHudState.exitMode = 1;
            gMenuFlowState = 0x63;
            break;
        case 4:
            if (actor->bounceTimer < 0x10) {
                actor->unk1E -= 9;
            } else {
                actor->unk1E += 9;
            }
            actor->bounceTimer = (actor->bounceTimer + 1) & 0x1F;
            break;
        }
    }

    gCharacterSelectHudState.phase = actor->state;
    D_8010AE58 = actor->alpha;
    if (actor->state != 8) {
        addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectConfirmationBanner, actor);
    }
}

void initCharacterSelectConfirmationBanner(CharacterSelectUiBannerActor *arg0) {
    arg0->x = -0x70;
    arg0->y = -0x1C;
    arg0->alpha = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectConfirmationBanner);
}

// drawCharacterSelectPlayerPanelFrames best match: 99.232% (nonmatchings/drawCharacterSelectPlayerPanelFrames-4033633224288138541/base_12.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/character_select/character_select_ui/drawCharacterSelectPlayerPanelFrames.s")

#ifdef NON_MATCHING
void drawCharacterSelectPlayerPanelFrames(CharacterSelectUiPlayerPanelFrameActor *arg0) {
    char playerNumberBuffer[0x10];
    u8 *playerNumberText;
    s32 palette;
    s32 alpha;
    s32 playerIndex;
    s32 playerNumber;
    s32 texture;
    CharacterSelectUiPlayerPanelFrameActor *panel;

    playerNumberText = playerNumberBuffer - 0xC;
    panel = arg0; playerIndex = 0; do {
        if ((gPlayerCount - 1) < playerIndex) {
            alpha = 0x50;
        } else {
            alpha = 0x100;
        }

        texture = getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_PLAYER_FRAME_HANDLE);
        playerNumber = playerIndex + 1;
        palette = playerNumber & 0xFF;
        drawMenuSpriteWithAlpha(panel->x, panel->y, texture, 0x23, 0x20, 0x20, 0, alpha, palette);
        texture = getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_PLAYER_FRAME_HANDLE);
        drawMenuSpriteWithAlpha((s16)(panel->x + 0x40), panel->y, texture, 0x24, 0x20, 0x20, 0, alpha, palette);
        texture = getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_PLAYER_FRAME_HANDLE);
        playerIndex = 2;
        drawMenuSpriteWithAlpha((s16)(panel->x + 0x80), panel->y, texture, 0xC, 0x20, 0x20, 0, alpha, palette);
        sprintf(playerNumberText, gCharacterSelectPlayerNumberFormat, playerNumber);
        drawMenuAsciiText((s16)(panel->x + 0x32), (s16)(panel->y + 2), playerNumberText, 0, alpha);
        if (alpha != 0x100) {
            texture = getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_UNUSED_HANDLE);
            drawMenuSpriteWithAlpha((s16)(panel->x + playerIndex), (s16)(panel->y + 0x14), texture, 0x90, 0x20, 0x20, 0, 0xF0, 0);
        }

        playerIndex = playerNumber;
        panel = (CharacterSelectUiPlayerPanelFrameActor *)((u8 *)panel + 2);
    } while (playerNumber != 4);
}
#endif

void updateCharacterSelectPlayerPanelFrames(CharacterSelectUiPanelActor *arg0) {
    u8 var_v0;
    u8 desired;
    u8 var_v1;
    CharacterSelectUiPanelActor *actor;
    CharacterSelectUiPlayerPanelFrameController *owner;

    owner = D_8010ADE0;
    actor = arg0;
    var_v0 = actor->targetY.mode;
    desired = gCharacterSelectHudState.exitMode;
    var_v1 = var_v0;
    if (desired != var_v0) {
        actor->targetY.mode = desired;
        var_v0 = desired & 0xFF;
        actor->targetX.target[0] = gCharacterSelectHudState.cursorX;
        var_v0 = actor->targetY.mode;
        var_v1 = var_v0;
        actor->targetX.target[1] = gCharacterSelectHudState.cursorY;
    }

    switch (var_v1) {
    case 1:
        actor->y[1] += 8;
        actor->y[3] += 8;
        if (actor->y[3] == 0x24) {
            actor->targetY.mode = 2;
            owner->playerFrameReady = 1;
        }
        var_v0 = actor->targetY.mode;
        break;
    case 0:
    case 2:
    case 3:
    case 4:
        break;
    }

    D_8010AE51 = var_v0;
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectPlayerPanelFrames, actor);
}

void initCharacterSelectPlayerPanelFrames(CharacterSelectUiPanelActor *arg0) {
    arg0->x[0] = -0x88;
    arg0->y[0] = -0x60;
    arg0->x[1] = -0x88;
    arg0->y[1] = 4;
    arg0->x[2] = 4;
    arg0->y[2] = -0x60;
    arg0->x[3] = 4;
    arg0->y[3] = 4;
    arg0->targetX.target[0] = 0x8C;
    arg0->targetX.target[1] = 0x44;
    arg0->targetY.mode = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectPlayerPanelFrames);
}

void drawCharacterSelectRosterIcons(CharacterSelectUiRosterIconActor *arg0) {
    s16 *assetHandles;
    s32 i;
    s32 j;
    s32 xOffset;
    u16 alpha;
    u16 tile;
    u8 textureIndex;
    s32 selectedAlpha;
    s32 tileTemp;

    i = 0; if (arg0->state != 0) { assetHandles = gAssetHandles; xOffset = 0; selectedAlpha = 0x60; do { alpha = 0x100; j = 0; if (gPlayerCount > 0) { do { if ((i == gRacePlayers[j].selectedCharacterId) && (gRacePlayers[j].menuState != 0)) {
                        tileTemp = i + 0x41;
                        alpha = 0x60;
                        tile = tileTemp;
                        textureIndex = 0x1F;
                        break;
                    }
                    j++;
                } while (j < gPlayerCount);
            }
            if (selectedAlpha != alpha) {
                textureIndex = 0x21;
                if (arg0->timer < 0xA) {
                    tileTemp = i + 0x37;
                    tile = tileTemp;
                } else {
                    tileTemp = i + 0x3D;
                    tile = tileTemp;
                }
            }
            drawMenuSpriteTile((s16)(arg0->x + xOffset), arg0->y,
                          getRelocatableHeapBlockBase(assetHandles[textureIndex]), tile, 0, alpha);
            i++;
            xOffset += 0x20;
        } while (i < 5);

        if (arg0->playerFlags != 0) {
            alpha = 0x100;
            j = 0;
            if (gPlayerCount > 0) {
                do {
                    if ((gRacePlayers[j].selectedCharacterId == 5) && (gRacePlayers[j].menuState != 0)) {
                        alpha = 0x60;
                        tile = 0x46;
                        textureIndex = 0x1F;
                        break;
                    }
                    j++;
                } while (j < gPlayerCount);
            }
            if (selectedAlpha != alpha) {
                textureIndex = 0x21;
                if (arg0->timer < 0xA) {
                    tile = 0x3C;
                } else {
                    tile = 0x42;
                }
            }
            drawMenuSpriteTile((s16)(arg0->x - 0x20), arg0->y,
                          getRelocatableHeapBlockBase(assetHandles[textureIndex]), tile, 0, alpha);
        }
    }
}

void updateCharacterSelectRosterIcons(CharacterSelectUiRosterIconActor *arg0) {
    s16 temp_v0;
    int new_var;
    u8 var_v1;

    var_v1 = arg0->state;
    new_var = 1;
    switch (var_v1) {
    case 0:
    case 3:
        break;
    case 1:
        temp_v0 = arg0->targetX;
        arg0->x = arg0->x - 0x10;
        if (temp_v0 >= arg0->x) {
            arg0->state = 2;
            arg0->x = temp_v0;
        }
        var_v1 = arg0->state;
        break;
    case 2:
        D_8010AE52 = new_var;
        var_v1 = arg0->state;
        break;
    }
    if (var_v1) {
        arg0->timer = (arg0->timer + new_var) % 20;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectRosterIcons, arg0);
}

void initCharacterSelectRosterIcons(CharacterSelectUiRosterIconActor *arg0) {
    s32 i;
    RacePlayerState *player;
    CharacterSelectUiRosterIconActor *actor;
    s32 targetX;

    targetX = -0x50;
    arg0->targetX = targetX;
    arg0->x = 0xB8;
    arg0->y = -0x10;
    arg0->targetY = -0x48;
    arg0->speedX = 0x12;
    arg0->unk26 = 0;
    arg0->state = 0;
    arg0->timer = 0;
    arg0->playerFlags = 0;
    actor = arg0;
    arg0->unk23 = 0;

    i = 0;
    if (gPlayerCount > 0) {
        player = gGameSaveDataBuffer;
        do {
            actor->playerFlags = actor->playerFlags | (player->flags & 1);
            D_8010AE5E = actor->playerFlags;
            D_8010AE5F = actor->unk23;
            i++;
            player++;
        } while (i < gPlayerCount);
    }

    if (actor->playerFlags == 1) {
        actor->targetX = -0x40;
    } else {
        actor->targetX = targetX;
    }
    setCallbackTaskCallback(actor, updateCharacterSelectRosterIcons);
}

void drawCharacterSelectPlayerCursorMarkers(CharacterSelectUiPlayerCursorActor *arg0) {
    s32 i;
    s32 j;
    s32 evenMatch;
    s32 oddMatch;
    u16 alpha;
    u16 *tiles;
    RacePlayer *player;
    CharacterSelectUiPlayerCursorActor *actorX;

 do { if (arg0->mode != 0) { i = 0; if (((s32) gPlayerCount) > 0) { player = gRacePlayers; tiles = gCharacterSelectPlayerMarkerTiles; actorX = arg0; do { evenMatch = 0; oddMatch = 0; j = 0; if (player->menuState != 0) { alpha = 0x100; } else { alpha = arg0->scale; } if (((s32) gPlayerCount) > 0) { do { if ((j != i) && (D_8010AE64[i] == D_8010AE64[j])) { if (!(j & 1)) { evenMatch = 1; } else { oddMatch = 2; } } j++; } while (j < ((s32) gPlayerCount)); } drawMenuSpriteWithAlpha(actorX->x[0], arg0->y, getRelocatableHeapBlockBase(gAssetHandles[0x21]), tiles[evenMatch + oddMatch], 0x20, 0x20, 0, alpha, 0); i++; player++; tiles += 4; actorX = (CharacterSelectUiPlayerCursorActor *) (((u8 *) actorX) + 2); } while (i < ((s32) gPlayerCount)); } } } while (0);
}

void updateCharacterSelectPlayerCursorMarkers(CharacterSelectUiPlayerCursorActor *arg0) {
    s32 i;
    u8 mode;
    u8 globalMode;
    RacePlayer *player;
    s8 *layout;

    globalMode = D_8010AE52;
    mode = arg0->mode;

    if (globalMode != mode) {
        mode = (arg0->mode = globalMode);
    }

    if (mode != 0) {
 i = 0; if ((s32)gPlayerCount > 0) { layout = D_8010AE64; player = gRacePlayers; do {
                if (player->selectedCharacterId < 5) {
                    arg0->x[i] = (layout[i] * 0x20) + arg0->baseX;
                } else if (player->selectedCharacterId == 5) {
                    arg0->x[i] = arg0->baseX;
                }
                i++;
                player++;
            } while (i < (s32)gPlayerCount);
        }

        if ((s32)arg0->timer < 0x10) {
            arg0->scale -= 9;
        } else {
            arg0->scale += 9;
        }
        arg0->timer = (arg0->timer + 1) & 0x1F;
        mode = (((((((((arg0->mode & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF;
    }

    D_8010AE52_state = mode;
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectPlayerCursorMarkers, arg0);
}

void initCharacterSelectPlayerCursorMarkers(CharacterSelectUiPlayerCursorActor *arg0) {
    RacePlayerState *player;
    s8 *playerMarkerLayout;
    CharacterSelectUiPlayerCursorActor *markerCursor;
    s32 i;
    s32 playerFlags;

    playerFlags = 0;
    i = 0;
 if ((s32)gPlayerCount > 0) { player = gGameSaveDataBuffer; do {
            playerFlags |= player->flags;
            i = (s32)&gGameSaveDataBuffer[gPlayerCount];
            player++;
        } while ((u32)player < (u32)i);
        i = 0;
    }

    if (playerFlags == 1) {
        arg0->baseX = -0x60;
    } else {
        arg0->baseX = -0x70;
    }

    if ((s32)gPlayerCount > 0) {
        playerMarkerLayout = D_8010AE64;
        markerCursor = arg0;
        do {
            markerCursor->x[0] = (*playerMarkerLayout * 0x20) + arg0->baseX;
            i++;
            playerMarkerLayout++;
            markerCursor = (CharacterSelectUiPlayerCursorActor *)((u8 *)markerCursor + 2);
        } while (i < (s32)gPlayerCount);
    }

    arg0->y = -0x18;
    arg0->mode = 0;
    arg0->scale = 0x100;
    setCallbackTaskCallback(arg0, updateCharacterSelectPlayerCursorMarkers);
}

// drawCharacterSelectPlayerStatsPanels best match: 96.939% (nonmatchings/drawCharacterSelectPlayerStatsPanels-7475224831549593718/base_20.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/character_select/character_select_ui/drawCharacterSelectPlayerStatsPanels.s")

#ifdef NON_MATCHING
void drawCharacterSelectPlayerStatsPanels(CharacterSelectUiPanelActor *arg0) {
    CharacterSelectUiPanelActor *base;
    u8 *statsBase;
    s32 stride;
    RacePlayer *player;
    volatile s16 *textureHandles;
    CharacterSelectUiPanelActor *actor;
    s32 playerIndex;
    s32 statIndex;
    s32 xOffset;
    s32 textureIndex;
    s32 characterIconTile;
    u8 *stats;
    s32 stat;
    char statLabelText[4];
    CharacterSelectUiPanelController *controller;

    controller = D_8010ADE8;
    base = arg0;
    if (controller->unk26 != 0) {
        playerIndex = 0;
        if ((s32)gPlayerCount > 0) {
            textureHandles = gAssetHandles;
            statsBase = (u8 *)gCharacterSelectCharacterStats;
            player = gRacePlayers;
            actor = arg0;
            stride = sizeof(CharacterSelectUiCharacterStats);
            do {
                statIndex = (xOffset = 0);
                if (player->menuState == 0) {
                    textureIndex = 0x21;
                    if (base->targetX.overlay.inactiveTimer[playerIndex] >= 0xB) {
                        base->targetY.finishedBlink[playerIndex] = 1;
                    } else {
                        base->targetY.finishedBlink[playerIndex] = 0;
                    }

                    if (base->targetY.finishedBlink[playerIndex] != 0) {
                        characterIconTile = (player->selectedCharacterId + 0x3D) & 0xFFFF;
                    } else {
                        characterIconTile = (player->selectedCharacterId + 0x37) & 0xFFFF;
                    }
                } else {
                    textureIndex = 0x1F;
                    characterIconTile = (player->selectedCharacterId + 0x41) & 0xFFFF;
                }

                drawMenuSpriteTile((s16)(actor->x[0] + 6), (s16)(actor->y[0] + 0xD),
                                   getRelocatableHeapBlockBase(textureHandles[textureIndex]), characterIconTile, 0, 0x100);
                drawMenuSprite((s16)(actor->x[0] + 2), (s16)(actor->y[0] + 0x28),
                               getRelocatableHeapBlockBase(textureHandles[0x1F]),
                               (player->selectedCharacterId + 0x91) & 0xFFFF, 0x20, 0x20, 0, 0);

                if (player->selectedCharacterId == 5) {
                    statLabelText[0] = 0x3F;
                    statLabelText[1] = 0x3F;
                    statLabelText[2] = 0;
                } else {
                    sprintf(statLabelText, gCharacterSelectCharacterStatFormat, gCharacterSelectCharacterStatLabels[player->selectedCharacterId]);
                }
                drawMenuAsciiText((s16)(actor->x[0] + 0x70), (s16)(actor->y[0] + 0xD), (u8 *)statLabelText, 0, 0x100);

                stats = &statsBase[player->selectedCharacterId * stride];
                stat = stats[0];
                if (stat / 2 > 0) {
                    do {
                        drawMenuSprite((s16)(actor->x[0] + xOffset + 0x5D), (s16)(actor->y[0] + 0x16),
                                       getRelocatableHeapBlockBase(textureHandles[0x21]), 0x25, 0x20, 0x20, 0, 0);
                        statIndex++;
                        xOffset += 0xC;
                        stats = &statsBase[player->selectedCharacterId * stride];
                        stat = stats[0];
                    } while (statIndex < stat / 2);
                    statIndex = 0;
                }
                if (stat & 1) {
                    drawMenuSprite((s16)(actor->x[0] + xOffset + 0x5D), (s16)(actor->y[0] + 0x16),
                                   getRelocatableHeapBlockBase(textureHandles[0x21]), 0x26, 0x20, 0x20, 0, 0);
                    stats = (u8 *)&gCharacterSelectCharacterStats[player->selectedCharacterId];
                }

                stat = stats[1];
                xOffset = 0;
                if (stat / 2 > 0) {
                    do {
                        drawMenuSprite((s16)(actor->x[0] + xOffset + 0x5D), (s16)(actor->y[0] + 0x22),
                                       getRelocatableHeapBlockBase(textureHandles[0x21]), 0x25, 0x20, 0x20, 0, 0);
                        statIndex++;
                        xOffset += 0xC;
                        stats = &statsBase[player->selectedCharacterId * stride];
                        stat = stats[1];
                    } while (statIndex < stat / 2);
                    statIndex = 0;
                }
                if (stat & 1) {
                    drawMenuSprite((s16)(actor->x[0] + xOffset + 0x5D), (s16)(actor->y[0] + 0x22),
                                   getRelocatableHeapBlockBase(textureHandles[0x21]), 0x26, 0x20, 0x20, 0, 0);
                    stats = (u8 *)&gCharacterSelectCharacterStats[player->selectedCharacterId];
                }

                stat = stats[2];
                xOffset = 0;
                if (stat / 2 > 0) {
                    do {
                        drawMenuSprite((s16)(actor->x[0] + xOffset + 0x5D), (s16)(actor->y[0] + 0x2E),
                                       getRelocatableHeapBlockBase(textureHandles[0x21]), 0x25, 0x20, 0x20, 0, 0);
                        statIndex++;
                        xOffset += 0xC;
                        stat = statsBase[(player->selectedCharacterId * stride) + 2];
                    } while (statIndex < stat / 2);
                }
                if (stat & 1) {
                    drawMenuSprite((s16)(actor->x[0] + xOffset + 0x5D), (s16)(actor->y[0] + 0x2E),
                                   getRelocatableHeapBlockBase(textureHandles[0x21]), 0x26, 0x20, 0x20, 0, 0);
                }

                playerIndex++;
                player++;
                actor = (CharacterSelectUiPanelActor *)((u8 *)actor + 2);
            } while (playerIndex < (s32)gPlayerCount);
        }
    }
}
#endif

void updateCharacterSelectPlayerStatsPanels(CharacterSelectUiPanelActor *arg0) {
    s32 i;
    u8 *src;
    void *srcBase;
    u8 *dst;
    RacePlayer *player;

    srcBase = D_8010ADE4;
    player = gRacePlayers; for (i = 0, src = srcBase, dst = (u8 *)arg0; i != RACE_PLAYER_COUNT; i++, dst += 2) {
        *(s16 *)(dst + 0x18) = *(s16 *)(src + 0x18);
        src += 2;
        *(s16 *)(dst + 0x20) = *(s16 *)(src + 0x1E);
        player++;
        if (player[-1].menuState == 0) {
            arg0->targetX.overlay.inactiveTimer[i] = (arg0->targetX.overlay.inactiveTimer[i] + 1) % 20;
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectPlayerStatsPanels, arg0);
}

void initCharacterSelectPlayerStatsPanels(CharacterSelectUiPanelActor *arg0) {
    arg0->x[0] = -0x88;
    arg0->y[0] = -0x60;
    arg0->x[1] = -0x88;
    arg0->y[1] = 0x24;
    arg0->x[2] = 4;
    arg0->y[2] = -0x60;
    arg0->x[3] = 4;
    arg0->y[3] = 0x24;
    setCallbackTaskCallback(arg0, updateCharacterSelectPlayerStatsPanels);
}

void drawCharacterSelectSelectedCharacterTokens(CharacterSelectUiSelectedCharacterTokenActor *arg0) {
    CharacterSelectUiSelectedCharacterTokenActor *tokens;
    s32 i;
    s32 color;
    u16 tileSize;

    tokens = arg0;
    i = 0;
    if (gPlayerCount > 0) {
        do {
            if (gRacePlayers[i].menuState != 0) {
                color = getRelocatableHeapBlockBase(CHARACTER_SELECT_UI_PLAYER_FRAME_HANDLE);
                tileSize = tokens->tileSize[i];
                drawMenuSprite(tokens->x[i], tokens->y[i], color, 0xD, tileSize, tileSize, 0, 0);
            }
            i++;
        } while (i < gPlayerCount);
    }
}

// updateCharacterSelectSelectedCharacterTokens best match: 96.329% (nonmatchings/updateCharacterSelectSelectedCharacterTokens-5787290371232622032/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/character_select/character_select_ui/updateCharacterSelectSelectedCharacterTokens.s")

#ifdef NON_MATCHING
void updateCharacterSelectSelectedCharacterTokens(CharacterSelectUiSelectedCharacterTokenActor *arg0) {
    CharacterSelectUiPanelTransitionActor *panelFrames;
    CharacterSelectUiSelectedCharacterTokenActor *tokens;
    s32 playerIndex;
    s32 menuState;
    u8 state;
    s16 rosterX;
    s16 yDirection;
    s32 step;
    s32 exitXStep;
    s32 exitYStep;
    s16 panelX;
    s16 panelY;

    panelFrames = D_8010ADE4;
    tokens = arg0;
    playerIndex = 0;
    if ((s32)gPlayerCount > 0) {
        do {
            menuState = gCharacterSelectHudState.selectedTokenState[playerIndex];
            step = 0;
            state = tokens->state[playerIndex];
            if (state != (menuState & 0xFFFFFFFF)) {
                state = menuState;
                arg0->state[playerIndex] = menuState;
            }

            switch (state) {
            case CHARACTER_SELECT_TOKEN_IDLE:
                break;
            case CHARACTER_SELECT_TOKEN_START:
                menuState = gRacePlayers[playerIndex].menuSelection;
                if ((s32)menuState < 5) {
                    rosterX = menuState;
                    rosterX = (rosterX * 0x20) - 0x40;
                } else if (menuState == 5) {
                    rosterX = -0x70;
                } else {
                    rosterX = 0x50;
                }

                panelX = panelFrames->x[playerIndex] + 0x38;
                panelY = panelFrames->y[playerIndex];
                arg0->xDistance[playerIndex] = panelX - rosterX;
                arg0->yDistance[playerIndex] = panelY + 0x20;
                arg0->xDirection[playerIndex] = 1;
                if (arg0->xDistance[playerIndex] < 0) {
                    tokens->xDirection[playerIndex] = -1;
                    tokens->xDistance[playerIndex] = tokens->xDistance[playerIndex] * -1;
                }
                if (tokens->yDistance[playerIndex] < 0) {
                    tokens->yDistance[playerIndex] = tokens->yDistance[playerIndex] * -1;
                }

                tokens->divisor[playerIndex] = tokens->xDistance[playerIndex] / 0x20;
                tokens->axis[playerIndex] = 0;
                tokens->stepCount[playerIndex] = tokens->xDistance[playerIndex] / 4;
                if (tokens->xDistance[playerIndex] < tokens->yDistance[playerIndex]) {
                    tokens->axis[playerIndex] = 1;
                    tokens->divisor[playerIndex] = tokens->yDistance[playerIndex] / 0x20;
                    tokens->stepCount[playerIndex] = tokens->yDistance[playerIndex] / 4;
                }
                tokens->x[playerIndex] = rosterX;
                tokens->y[playerIndex] = -8;
                tokens->signedTileSize[playerIndex] = 1;
                tokens->timer[playerIndex] = 0;
                tokens->state[playerIndex] = CHARACTER_SELECT_TOKEN_FLYING;
                state = tokens->state[playerIndex];
                break;
            case CHARACTER_SELECT_TOKEN_FLYING:
                exitXStep = playerIndex;
                if (exitXStep & 1) {
                    yDirection = 1;
                    state = tokens->state[exitXStep];
                } else {
                    yDirection = -1;
                }
                if (tokens->stepCount[exitXStep] > 0) {
                    do {
                        if (tokens->axis[exitXStep] != 0) {
                            tokens->y[exitXStep] += yDirection;
                            tokens->accumulator[exitXStep] += tokens->xDistance[exitXStep];
                            if (tokens->accumulator[exitXStep] >= tokens->yDistance[exitXStep]) {
                                tokens->accumulator[exitXStep] -= tokens->yDistance[exitXStep];
                                arg0->x[exitXStep] += arg0->xDirection[exitXStep];
                            }
                        } else {
                            arg0->x[exitXStep] += arg0->xDirection[exitXStep];
                            arg0->accumulator[exitXStep] += arg0->yDistance[exitXStep];
                            if (arg0->accumulator[exitXStep] >= arg0->xDistance[exitXStep]) {
                                arg0->y[exitXStep] += yDirection;
                                arg0->accumulator[exitXStep] -= arg0->xDistance[exitXStep];
                            }
                        }
                        arg0->timer[exitXStep]++;
                        if ((arg0->timer[exitXStep] % arg0->divisor[exitXStep]) == 0) {
                            arg0->signedTileSize[exitXStep]++;
                        }
                        if (arg0->signedTileSize[exitXStep] >= 0x21) {
                            arg0->signedTileSize[exitXStep] = 0x20;
                        }
                        if ((arg0->x[exitXStep] == panelFrames->x[exitXStep] + 0x38) &&
                            (arg0->y[exitXStep] == panelFrames->y[exitXStep] + 0x18)) {
                            arg0->state[exitXStep] = CHARACTER_SELECT_TOKEN_LANDED;
                            arg0->signedTileSize[exitXStep] = 0x20;
                            state = arg0->state[playerIndex];
                            break;
                        }
                        step++;
                    } while (step < tokens->stepCount[playerIndex]);
                    state = tokens->state[exitXStep];
                }
                break;
            case CHARACTER_SELECT_TOKEN_LANDED:
                break;
            default:
                break;
            }

            playerIndex++;
            gCharacterSelectHudState.selectedTokenState[playerIndex - 1] = state;
        } while (playerIndex < (s32)gPlayerCount);
    }

    if (panelFrames->mode == 3) {
        step = 0;
        do {
            for (playerIndex = 0; playerIndex != RACE_PLAYER_COUNT; playerIndex++) {
                if (playerIndex < 2) {
                    exitXStep = -1;
                } else {
                    exitXStep = 1;
                }
                if (playerIndex & 1) {
                    exitYStep = 1;
                } else {
                    exitYStep = -1;
                }
                panelFrames->x[playerIndex] += exitXStep;
                if (playerIndex == 0) {
                    panelFrames->frameCounter += panelFrames->frameStep;
                }
                if (panelFrames->frameCounter >= panelFrames->frameLimit) {
                    panelFrames->y[playerIndex] += exitYStep;
                    if (playerIndex == 3) {
                        panelFrames->frameCounter -= panelFrames->frameLimit;
                    }
                }
                arg0->x[playerIndex] = panelFrames->x[playerIndex] + 0x38;
                yDirection = panelFrames->y[playerIndex] + 0x18;
                arg0->y[playerIndex] = yDirection;
            }
            step++;
            if (panelFrames->x[0] == -0x114) {
                panelFrames->mode = 4;
                break;
            }
        } while (step != 0x10);
    }

    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectSelectedCharacterTokens, arg0);
}
#endif

void initCharacterSelectSelectedCharacterTokens(CharacterSelectUiSelectedCharacterTokenActor *arg0) {
    setCallbackTaskCallback(arg0, updateCharacterSelectSelectedCharacterTokens);
}
