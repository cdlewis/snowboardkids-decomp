#include "common.h"
#include "memory_allocator.h"
#include "callback_task_scheduler.h"
#include "menu_rendering.h"
#include "race_hud.h"

#define PLAYER_COUNT 4
#define PLAYER_DATA_SIZE 0x60C
#define RACE_HUD_UNUSED_HANDLE (*(s16 *)&D_80112130[0x3E])
#define RACE_HUD_PLAYER_FRAME_HANDLE (*(s16 *)&D_80112130[0x42])
#define RACE_HUD_BANNER_TEXTURE_HANDLE (*(s16 *)&D_80112130[0x52])

typedef struct {
    u8 pad0[5];
    /* 0x05 */ union {
        u8 characterId;
        u8 playerIndex;
    };
    u8 pad6[2];
    u8 isActive;
    u8 pad9[PLAYER_DATA_SIZE - 9];
} RacePlayer;

typedef struct {
    u8 pad0[0x4B];
    /* 0x4B */ u8 flags;
    u8 pad4C[0x78F8 - 0x4C];
} RacePlayerState;

typedef struct {
    /* 0x00 */ u8 phase;
    /* 0x01 */ u8 exitMode;
    /* 0x02 */ u8 readyCount;
    u8 pad3;
    u8 pad4[4];
    /* 0x08 */ s16 fade;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ u8 confirmSelection;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 blockDecrease;
    /* 0x0F */ u8 blockIncrease;
    /* 0x10 */ u16 cursorX;
    /* 0x12 */ u16 cursorY;
    /* 0x14 */ s8 playerSelections[PLAYER_COUNT];
} RaceHudCharacterSelectState;

typedef struct {
    u8 pad0[0x24];
    /* 0x24 */ u8 playerFrameReady;
} RaceHudPlayerFrameController;

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ s16 x[PLAYER_COUNT];
    /* 0x20 */ s16 y[PLAYER_COUNT];
    /* 0x28 */ u16 frameLimit;
    /* 0x2A */ u16 frameStep;
    /* 0x2C */ u16 frameCounter;
    u8 pad2E[2];
    /* 0x30 */ u8 mode;
} RaceHudPanelTransitionActor;

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
} RaceHudSharedState;

typedef struct {
    u8 pad0[0x26];
    /* 0x26 */ u8 unk26;
} RaceHudPanelController;

extern void func_800483FC(void *, void *, void *);
extern s8 D_8010AE52;
extern u8 D_8010AE51;
extern RaceHudCharacterSelectState gCharacterSelectHudState;
extern RaceHudPlayerFrameController *D_8010ADE0;
extern void *D_8010ADE4;
extern RaceHudPanelController *D_8010ADE8;
extern s16 D_8010AE58;
extern s32 D_801235B4;
extern void *D_80124868;
extern u8 gPlayerCount;
extern RacePlayer D_80121D80[];
extern u8 D_80121D85[];
extern s8 D_8010AE64[];
extern u8 D_80112130[];
extern u8 D_800B5A70[];
extern u8 D_800B5B08[];
extern u16 D_800B5B20[];
extern const char D_800E0AB0[];
extern const char D_800E0AB4[];
extern RacePlayerState gGameSaveDataBuffer[];
extern u8 D_8010AE5E;
extern u8 D_8010AE5F;
extern u16 D_800B5B30[];
extern s16 D_80112172;

void func_800171F0(RaceHudBannerActor *arg0) {
    s32 i;
    s32 j;
    unsigned int alpha;
    s32 selected;
    s32 limit;
    RaceHudBannerActor *actor;

    actor = arg0;
    if (actor->state != 3) {
        func_8000F8AC((s16)(actor->x - 4), (s16)(actor->y - 4), func_80043040(RACE_HUD_BANNER_TEXTURE_HANDLE), 2, 0x20,
                      0x20, 0, actor->alpha, 0);
        func_8000F8AC((s16)(actor->x + 0xD4), (s16)(actor->y - 4), func_80043040(RACE_HUD_BANNER_TEXTURE_HANDLE),
                      4, 0x20, 0x20, 0, actor->alpha, 0);
        i = 0;
        do {
            func_8000F8AC((s16)(actor->x + i), (s16)(actor->y - 4), func_80043040(RACE_HUD_BANNER_TEXTURE_HANDLE),
                          3, 0x20, 0x20, 0, actor->alpha, 0);
            func_8000F8AC((s16)(actor->x + i), (s16)(actor->y + 0x24), func_80043040(RACE_HUD_BANNER_TEXTURE_HANDLE),
                          8, 0x20, 0x20, 0, actor->alpha, 0);
            i += 0x10;
        } while (i < 0xE0);
        func_8000F8AC((s16)(actor->x - 4), (s16)(actor->y + 0x24), func_80043040(RACE_HUD_BANNER_TEXTURE_HANDLE),
                      7, 0x20, 0x20, 0, actor->alpha, 0);
        func_8000F8AC((s16)(actor->x + 0xD4), (s16)(actor->y + 0x24),
                      func_80043040(RACE_HUD_BANNER_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, actor->alpha, 0);
        i = (actor->state == 4) * 0;
        limit = 0xE0;
        do {
            func_8000F8AC((s16)(actor->x - 4), (s16)(actor->y + i), func_80043040(RACE_HUD_BANNER_TEXTURE_HANDLE),
                          5, 0x20, 0x20, 0, actor->alpha, 0);
            func_8000F8AC((s16)(actor->x + 0xD4), (s16)(actor->y + i), func_80043040(RACE_HUD_BANNER_TEXTURE_HANDLE),
                          6, 0x20, 0x20, 0, actor->alpha, 0);
            j = 0;
            do {
                func_8000F8AC((s16)(actor->x + j), (s16)(actor->y + i),
                              func_80043040(RACE_HUD_BANNER_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, actor->alpha, 0);
                j += 0x10;
            } while (j != limit);
            i += 0x10;
        } while (i <= 0x2F);
        if (actor->state < 3) {
            selected = 0;
        } else {
            selected = 1;
        }
        func_80013154(actor->x, actor->y, &D_800B5A70[selected * 0x38], 0, actor->alpha, 0);
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
            func_8000F8AC((s16)(actor->x + 0x4C), (s16)(actor->y + 0x10),
                          func_80043040(RACE_HUD_PLAYER_FRAME_HANDLE), 0x17, 0x20, 0x20, 0, alpha, 0);
            if (actor->alpha < 0x60) {
                alpha = actor->alpha & 0xFFFF;
            } else {
                if (actor->mode != 0) {
                    alpha = 0x100;
                } else {
                    alpha = 0x60;
                }
            }
            func_8000F8AC((s16)(actor->x + 0x4C), (s16)(actor->y + 0x20),
                          func_80043040(RACE_HUD_PLAYER_FRAME_HANDLE), 0x18, 0x20, 0x20, 0, alpha, 0);
            if (actor->alpha != 0x100) {
                alpha = actor->alpha & 0xFFFF;
                ;
            } else {
                alpha = (u16)actor->unk1E;
            }
            func_8000F8AC((s16)(actor->x + 0x4C), (s16)((actor->y + (actor->mode * 0x10)) + 0x10),
                          func_80043040(RACE_HUD_PLAYER_FRAME_HANDLE), 0x12, 0x20, 0x20, 0, alpha, 0);
        }
        if (actor->state == 1) {
            func_8000F030((s16)(actor->x + 0xD0), (s16)(actor->y + 0x20),
                          func_80043040(RACE_HUD_PLAYER_FRAME_HANDLE), ((actor->frame >= 8) + 5) & 0xFFFF, 0x20,
                          0x20, 0, 0);
        }
    }
}

void func_800177F8(RaceHudBannerActor *arg0) {
    s16 alpha;
    s32 state;
    RaceHudBannerActor *actor;

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
            D_801235B4 = 0x63;
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
        func_800483FC(&D_80124868, func_800171F0, actor);
    }
}

void func_800179D4(RaceHudBannerActor *arg0) {
    arg0->x = -0x70;
    arg0->y = -0x1C;
    arg0->alpha = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, func_800177F8);
}

// func_80017A10 best match: 99.232% (nonmatchings/func_80017A10-4033633224288138541/base_12.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_80017A10.s")

#ifdef NON_MATCHING
void func_80017A10(RaceHudPlayerFrameActor *arg0) {
    char buf[0x10];
    u8 *sp6C;
    s32 flip;
    s32 alpha;
    s32 i;
    s32 playerNumber;
    s32 texture;
    RaceHudPlayerFrameActor *actor;

    sp6C = buf - 0xC;
    actor = arg0; i = 0; do {
        if ((gPlayerCount - 1) < i) {
            alpha = 0x50;
        } else {
            alpha = 0x100;
        }

        texture = func_80043040(RACE_HUD_PLAYER_FRAME_HANDLE);
        playerNumber = i + 1;
        flip = playerNumber & 0xFF;
        func_8000F8AC(actor->x, actor->y, texture, 0x23, 0x20, 0x20, 0, alpha, flip);
        texture = func_80043040(RACE_HUD_PLAYER_FRAME_HANDLE);
        func_8000F8AC((s16)(actor->x + 0x40), actor->y, texture, 0x24, 0x20, 0x20, 0, alpha, flip);
        texture = func_80043040(RACE_HUD_PLAYER_FRAME_HANDLE);
        i = 2;
        func_8000F8AC((s16)(actor->x + 0x80), actor->y, texture, 0xC, 0x20, 0x20, 0, alpha, flip);
        sprintf(sp6C, D_800E0AB0, playerNumber);
        func_80013D0C((s16)(actor->x + 0x32), (s16)(actor->y + 2), sp6C, 0, alpha);
        if (alpha != 0x100) {
            texture = func_80043040(RACE_HUD_UNUSED_HANDLE);
            func_8000F8AC((s16)(actor->x + i), (s16)(actor->y + 0x14), texture, 0x90, 0x20, 0x20, 0, 0xF0, 0);
        }

        i = playerNumber;
        actor = (RaceHudPlayerFrameActor *)((u8 *)actor + 2);
    } while (playerNumber != 4);
}
#endif

// func_80017C34 best match: 97.500% (nonmatchings/func_80017C34-6061209858023118177/base_10.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_80017C34.s")

#ifdef NON_MATCHING
void func_80017C34(RaceHudPanelActor *arg0) {
    s32 var_v0;
    u8 desired;
    s32 var_v1;
    RaceHudPanelActor *actor;
    RaceHudPlayerFrameController *owner;

    owner = D_8010ADE0;
    actor = arg0;
    var_v0 = actor->targetY.mode;
    desired = gCharacterSelectHudState.exitMode;
    var_v1 = var_v0;
    if (desired != var_v0) {
        actor->targetY.mode = desired;
        var_v0 = desired & 0xFF;
        actor->targetX.target[0] = gCharacterSelectHudState.cursorX;
        var_v1 = var_v0;
        actor->targetX.target[1] = gCharacterSelectHudState.cursorY;
    }

    switch (var_v1) {
    case 1:
        var_v1 = actor->y[3] + 8;
        actor->y[3] = var_v1;
        actor->y[1] += 8;
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
    func_800483FC(&D_80124868, func_80017A10, actor);
}
#endif

void func_80017D08(RaceHudPanelActor *arg0) {
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
    setCallbackTaskCallback(arg0, func_80017C34);
}

// func_80017D6C best match: 84.174% (nonmatchings/func_80017D6C-7273315160691878794/base.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_80017D6C.s")

#ifdef NON_MATCHING
void func_80017D6C(RaceHudMessageActor *arg0) {
    RacePlayer *player;
    s32 i;
    s32 j;
    s32 xOffset;
    u16 tile;
    s32 alpha;
    u8 textureIndex;

    i = 0;
    if (arg0->state != 0) {
        xOffset = 0;
        do {
            alpha = 0x100;
            j = 0;
            if (gPlayerCount > 0) {
                player = D_80121D80;
loop_4:
                j++;
                if ((i == player->playerIndex) && (player->isActive != 0)) {
                    tile = (i + 0x41) & 0xFFFF;
                    alpha = 0x60;
                    textureIndex = 0x1F;
                } else {
                    player++;
                    if (j < gPlayerCount) {
                        goto loop_4;
                    }
                }
            }
            if (alpha != 0x60) {
                textureIndex = 0x21;
                if (arg0->timer < 0xA) {
                    tile = (i + 0x37) & 0xFFFF;
                } else {
                    tile = (i + 0x3D) & 0xFFFF;
                }
            }
            func_80011264((s16)(arg0->x + xOffset), arg0->y,
                          func_80043040(*(s16 *)&D_80112130[textureIndex * 2]), tile, 0, alpha);
            i++;
            xOffset += 0x20;
        } while (i < 5);

        if (arg0->playerFlags != 0) {
            alpha = 0x100;
            j = 0;
            if (gPlayerCount > 0) {
                player = D_80121D80;
loop_17:
                j++;
                if ((player->playerIndex == 5) && (player->isActive != 0)) {
                    alpha = 0x60;
                    tile = 0x46;
                    textureIndex = 0x1F;
                } else {
                    player++;
                    if (j < gPlayerCount) {
                        goto loop_17;
                    }
                }
            }
            if (alpha != 0x60) {
                textureIndex = 0x21;
                if (arg0->timer < 0xA) {
                    tile = 0x3C;
                } else {
                    tile = 0x42;
                }
            }
            func_80011264((s16)(arg0->x - 0x20), arg0->y,
                          func_80043040(*(s16 *)&D_80112130[textureIndex * 2]), tile, 0, alpha);
        }
    }
}
#endif

void func_80017F94(RaceHudMessageActor *arg0) {
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
    func_800483FC(&D_80124868, func_80017D6C, arg0);
}

void func_80018060(RaceHudMessageActor *arg0) {
    s32 i;
    RacePlayerState *player;
    RaceHudMessageActor *actor;
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
    setCallbackTaskCallback(actor, func_80017F94);
}

void func_80018134(RaceHudPlayerListActor *arg0) {
    s32 i;
    s32 j;
    s32 evenMatch;
    s32 oddMatch;
    u16 alpha;
    u16 *tiles;
    RacePlayer *player;
    RaceHudPlayerListActor *actorX;

 do { if (arg0->mode != 0) { i = 0; if (((s32) gPlayerCount) > 0) { player = D_80121D80; tiles = D_800B5B30; actorX = arg0; do { evenMatch = 0; oddMatch = 0; j = 0; if (player->isActive != 0) { alpha = 0x100; } else { alpha = arg0->scale; } if (((s32) gPlayerCount) > 0) { do { if ((j != i) && (D_8010AE64[i] == D_8010AE64[j])) { if (!(j & 1)) { evenMatch = 1; } else { oddMatch = 2; } } j++; } while (j < ((s32) gPlayerCount)); } func_8000F8AC(actorX->x[0], arg0->y, func_80043040(D_80112172), tiles[evenMatch + oddMatch], 0x20, 0x20, 0, alpha, 0); i++; player++; tiles += 4; actorX = (RaceHudPlayerListActor *) (((u8 *) actorX) + 2); } while (i < ((s32) gPlayerCount)); } } } while (0);
}

// func_800182A4 best match: 99.740% (nonmatchings/func_800182A4-1315772375853892447/base_16.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_800182A4.s")

#ifdef NON_MATCHING
void func_800182A4(RaceHudPlayerListActor *arg0) {
    s32 i;
    u8 mode;
    u8 globalMode;
    u8 *player;
    s8 *layout;

    globalMode = D_8010AE52;
    mode = arg0->mode;

    if (globalMode != mode) {
        mode = (arg0->mode = globalMode);
    }

    if (mode != 0) {
        i = 0;
        if ((s32)gPlayerCount > 0) {
            layout = D_8010AE64;
            player = D_80121D80;
            do {
                if (player[5] < 5) {
                    arg0->x[i] = (layout[i] * 0x20) + arg0->baseX;
                } else if (player[5] == 5) {
                    arg0->x[i] = arg0->baseX;
                }
                i++;
                player += 0x60C;
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

    *(u8 *)0x8010AE52 = mode;
    func_800483FC(&D_80124868, func_80018134, arg0);
}
#endif

void func_800183DC(RaceHudPlayerListActor *arg0) {
    RacePlayerState *player;
    s8 *playerLayout;
    RaceHudPlayerListActor *actor;
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
        playerLayout = D_8010AE64;
        actor = arg0;
        do {
            actor->x[0] = (*playerLayout * 0x20) + arg0->baseX;
            i++;
            playerLayout++;
            actor = (RaceHudPlayerListActor *)((u8 *)actor + 2);
        } while (i < (s32)gPlayerCount);
    }

    arg0->y = -0x18;
    arg0->mode = 0;
    arg0->scale = 0x100;
    setCallbackTaskCallback(arg0, func_800182A4);
}

// func_800184C8 best match: 86.336% (nonmatchings/func_800184C8-4923837976568703863/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_800184C8.s")

#ifdef NON_MATCHING
void func_800184C8(RaceHudPanelActor *arg0) {
    char text[4];
    s32 i;
    s32 j;
    s32 xOffset;
    s32 iconIndex;
    u16 tile;
    u8 *stats;
    u8 stat;
    RacePlayer *player;
    RaceHudPanelActor *actor;
    RaceHudPanelActor *base;
    u8 *statsBase;
    u8 *textureHandles;
    s32 stride;

    base = arg0;
    if (D_8010ADE8->unk26 != 0) {
        i = 0;
        if ((s32)gPlayerCount > 0) {
            player = D_80121D80;
            statsBase = D_800B5B08;
            textureHandles = D_80112130;
            actor = arg0;
            stride = 3;
            do {
                xOffset = 0;
                j = 0;
                if (player->isActive == 0) {
                    iconIndex = 0x21;
                    if (base->targetX.overlay.inactiveTimer[i] >= 0xB) {
                        base->targetY.finishedBlink[i] = 1;
                    } else {
                        base->targetY.finishedBlink[i] = 0;
                    }

                    if (base->targetY.finishedBlink[i] != 0) {
                        tile = (player->characterId + 0x3D) & 0xFFFF;
                    } else {
                        tile = (player->characterId + 0x37) & 0xFFFF;
                    }
                } else {
                    iconIndex = 0x1F;
                    tile = (player->characterId + 0x41) & 0xFFFF;
                }

                func_80011264((s16)(actor->x[0] + 6), (s16)(actor->y[0] + 0xD),
                              func_80043040(*(s16 *)&textureHandles[iconIndex * 2]), tile, 0, 0x100);
                func_8000F030((s16)(actor->x[0] + 2), (s16)(actor->y[0] + 0x28),
                              func_80043040(*(s16 *)&textureHandles[0x3E]), (player->characterId + 0x91) & 0xFFFF,
                              0x20, 0x20, 0, 0);

                if (player->characterId == 5) {
                    text[0] = 0x3F;
                    text[1] = 0x3F;
                    text[2] = 0;
                } else {
                    sprintf(text, D_800E0AB4, D_800B5B20[player->characterId]);
                }
                func_80013D0C((s16)(actor->x[0] + 0x70), (s16)(actor->y[0] + 0xD), (u8 *)&text[0], 0, 0x100);

                stats = &statsBase[player->characterId * stride];
                stat = stats[0];
                if ((s32)stat / 2 > 0) {
                    do {
                        func_8000F030((s16)(actor->x[0] + xOffset + 0x5D), (s16)(actor->y[0] + 0x16),
                                      func_80043040(*(s16 *)&textureHandles[0x42]), 0x25, 0x20, 0x20, 0, 0);
                        j++;
                        xOffset += 0xC;
                        stats = &statsBase[player->characterId * stride];
                        stat = stats[0];
                    } while (j < (s32)stat / 2);
                    j = 0;
                }
                if (stat & 1) {
                    func_8000F030((s16)(actor->x[0] + xOffset + 0x5D), (s16)(actor->y[0] + 0x16),
                                  func_80043040(*(s16 *)&textureHandles[0x42]), 0x26, 0x20, 0x20, 0, 0);
                    stats = &statsBase[player->characterId * stride];
                }

                stat = stats[1];
                xOffset = 0;
                if ((s32)stat / 2 > 0) {
                    do {
                        func_8000F030((s16)(actor->x[0] + xOffset + 0x5D), (s16)(actor->y[0] + 0x22),
                                      func_80043040(*(s16 *)&textureHandles[0x42]), 0x25, 0x20, 0x20, 0, 0);
                        j++;
                        xOffset += 0xC;
                        stats = &statsBase[player->characterId * stride];
                        stat = stats[1];
                    } while (j < (s32)stat / 2);
                    j = 0;
                }
                if (stat & 1) {
                    func_8000F030((s16)(actor->x[0] + xOffset + 0x5D), (s16)(actor->y[0] + 0x22),
                                  func_80043040(*(s16 *)&textureHandles[0x42]), 0x26, 0x20, 0x20, 0, 0);
                    stats = &statsBase[player->characterId * stride];
                }

                stat = stats[2];
                xOffset = 0;
                if ((s32)stat / 2 > 0) {
                    do {
                        func_8000F030((s16)(actor->x[0] + xOffset + 0x5D), (s16)(actor->y[0] + 0x2E),
                                      func_80043040(*(s16 *)&textureHandles[0x42]), 0x25, 0x20, 0x20, 0, 0);
                        j++;
                        xOffset += 0xC;
                        stat = statsBase[(player->characterId * stride) + 2];
                    } while (j < (s32)stat / 2);
                }
                if (stat & 1) {
                    func_8000F030((s16)(actor->x[0] + xOffset + 0x5D), (s16)(actor->y[0] + 0x2E),
                                  func_80043040(*(s16 *)&textureHandles[0x42]), 0x26, 0x20, 0x20, 0, 0);
                }

                i++;
                player++;
                actor = (RaceHudPanelActor *)((u8 *)actor + 2);
            } while (i < (s32)gPlayerCount);
        }
    }
}
#endif

void func_80018AA0(RaceHudPanelActor *arg0) {
    s32 i;
    u8 *src;
    void *srcBase;
    u8 *dst;
    u8 *player;
    u8 *timer;

    srcBase = D_8010ADE4;
    player = (u8 *)D_80121D80; for (i = 0, src = srcBase, dst = (u8 *)arg0; i != PLAYER_COUNT; i++, dst += 2) {
        *(s16 *)(dst + 0x18) = *(s16 *)(src + 0x18);
        src += 2;
        *(s16 *)(dst + 0x20) = *(s16 *)(src + 0x1E);
        player += PLAYER_DATA_SIZE;
        timer = (u8 *)arg0;
        timer += i;
        if (player[-0x604] == 0) {
            timer[0x2C] = (timer[0x2C] + 1) % 20;
        }
    }
    func_800483FC(&D_80124868, func_800184C8, arg0);
}

void func_80018B6C(RaceHudPanelActor *arg0) {
    arg0->x[0] = -0x88;
    arg0->y[0] = -0x60;
    arg0->x[1] = -0x88;
    arg0->y[1] = 0x24;
    arg0->x[2] = 4;
    arg0->y[2] = -0x60;
    arg0->x[3] = 4;
    arg0->y[3] = 0x24;
    setCallbackTaskCallback(arg0, func_80018AA0);
}

void func_80018BC0(RaceHudPanelSlot *arg0) {
    u8 *base;
    s32 i;
    u8 *player;
    u8 *temp_s0;
    int new_var;
    s32 color;
    u16 temp_v1;

 base = (u8 *)arg0; i = 0; if (gPlayerCount > 0) { player = (u8 *)D_80121D80; do {
            if (player[8] != 0) {
                new_var = i * 2;
                temp_s0 = base + new_var;
                color = func_80043040(*(s16 *) &D_80112130[0x42]);
                temp_v1 = *(u16 *) (temp_s0 + 0x40);
                func_8000F030(*(s16 *) (temp_s0 + 0x18), *(s16 *) (temp_s0 + 0x20), color, 0xD, temp_v1, temp_v1, 0, 0);
            }
            i++;
            player += PLAYER_DATA_SIZE;
        } while (i < gPlayerCount);
    }
}

// func_80018C80 best match: 86.079% (nonmatchings/func_80018C80-7892263622508053986/base_3.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_80018C80.s")

#ifdef NON_MATCHING
void func_80018C80(RaceHudPanelActor *arg0) {
    RaceHudPanelTransitionActor *slots;
    u8 *stateMirror;
    RaceHudPanelActor *base;
    RaceHudPanelActor *actor;
    RaceHudPanelActor *actor2;
    s32 i;
    s32 state;
    s32 offsetX;
    s16 direction;
    s32 j;
    s32 moveX;
    s32 moveY;
    s16 tempX;
    s16 tempY;

    base = arg0;
    actor2 = arg0;
    slots = D_8010ADE4;
    i = 0;
    if ((s32)gPlayerCount > 0) {
        stateMirror = (u8 *)&gCharacterSelectHudState;
        actor = base;
        do {
            state = actor->state[0];
            if (stateMirror[3] != state) {
                state = stateMirror[3];
                actor->state[0] = state;
            }

            switch (state) {
            case 0:
                break;
            case 1:
                state = D_80121D85[i * PLAYER_DATA_SIZE];
                if ((s32)state < 5) {
                    offsetX = (state * 0x20) - 0x40;
                } else if (state == 5) {
                    offsetX = -0x70;
                } else {
                    offsetX = 0x50;
                }

                tempX = slots->x[i] + 0x38;
                tempY = slots->y[i];
                base->targetX.target[i] = tempX - offsetX;
                base->targetY.target[i] = tempY + 0x20;
                base->xDirection[i] = 1;
                if (base->targetX.target[i] < 0) {
                    base->xDirection[i] = -1;
                    base->targetX.target[i] = base->targetX.target[i] * -1;
                }
                if (base->targetY.target[i] < 0) {
                    base->targetY.target[i] = base->targetY.target[i] * -1;
                }

                actor->axis[0] = 0;
                actor->divisor[0] = base->targetX.target[i] / 0x20;
                base->stepCount[i] = base->targetX.target[i] / 4;
                if (base->targetX.target[i] < base->targetY.target[i]) {
                    actor->axis[0] = 1;
                    actor->divisor[0] = base->targetY.target[i] / 0x20;
                    base->stepCount[i] = base->targetY.target[i] / 4;
                }
                base->x[i] = offsetX;
                base->y[i] = -8;
                base->tileSize[i] = 1;
                base->timer[i] = 0;
                actor->state[0] = 2;
                state = actor->state[0];
                break;
            case 2:
                j = 0;
                if (i & 1) {
                    direction = 1;
                } else {
                    direction = -1;
                }
                if (base->stepCount[i] > 0) {
                    do {
                        if (actor->axis[0] != 0) {
                            base->y[i] += direction;
                            base->accumulator[i] += base->targetX.target[i];
                            if (base->accumulator[i] >= base->targetY.target[i]) {
                                base->accumulator[i] -= base->targetY.target[i];
                                base->x[i] += base->xDirection[i];
                            }
                        } else {
                            base->x[i] += base->xDirection[i];
                            base->accumulator[i] += base->targetY.target[i];
                            if (base->accumulator[i] >= base->targetX.target[i]) {
                                base->accumulator[i] -= base->targetX.target[i];
                                base->y[i] += direction;
                            }
                        }
                        base->timer[i]++;
                        if ((base->timer[i] % actor->divisor[0]) == 0) {
                            base->tileSize[i]++;
                        }
                        if (base->tileSize[i] >= 0x21) {
                            base->tileSize[i] = 0x20;
                        }
                        if ((base->x[i] == slots->x[i] + 0x38) && (base->y[i] == slots->y[i] + 0x18)) {
                            actor->state[0] = 3;
                            base->tileSize[i] = 0x20;
                            state = actor->state[0];
                            goto next_player;
                        }
                        j++;
                    } while (j < base->stepCount[i]);
                }
                break;
            case 3:
                break;
            default:
                break;
            }

next_player:
            i++;
            stateMirror[3] = state;
            stateMirror++;
            actor = (RaceHudPanelActor *)((u8 *)actor + 1);
        } while (i < (s32)gPlayerCount);
    }

    if (slots->mode == 3) {
        j = 0;
        do {
            for (i = 0; i != PLAYER_COUNT; i++) {
                if (i < 2) {
                    moveX = -1;
                } else {
                    moveX = 1;
                }
                if (i & 1) {
                    moveY = 1;
                } else {
                    moveY = -1;
                }
                slots->x[i] += moveX;
                if (i == 0) {
                    slots->frameCounter += slots->frameStep;
                }
                if (slots->frameCounter >= slots->frameLimit) {
                    slots->y[i] += moveY;
                    if (i == 3) {
                        slots->frameCounter -= slots->frameLimit;
                    }
                }
                actor2->x[i] = slots->x[i] + 0x38;
                actor2->y[i] = slots->y[i] + 0x18;
            }
            j++;
            if (slots->x[0] == -0x114) {
                slots->mode = 4;
                break;
            }
        } while (j != 0x10);
    }

    func_800483FC(&D_80124868, func_80018BC0, base);
}
#endif

void func_800191A0(RaceHudPanelActor *arg0) {
    setCallbackTaskCallback(arg0, func_80018C80);
}
