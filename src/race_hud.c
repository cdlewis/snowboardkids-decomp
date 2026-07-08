#include "common.h"
#include "menu_rendering.h"

#define PLAYER_COUNT 4
#define PLAYER_DATA_SIZE 0x60C
#define RACE_HUD_UNUSED_HANDLE (*(s16 *)&D_80112130[0x3E])
#define RACE_HUD_PLAYER_FRAME_HANDLE (*(s16 *)&D_80112130[0x42])
#define RACE_HUD_BANNER_TEXTURE_HANDLE (*(s16 *)&D_80112130[0x52])

typedef struct {
    u8 pad0[8];
    u8 isActive;
    u8 pad9[PLAYER_DATA_SIZE - 9];
} RacePlayer;

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
} RaceHudBannerActor;

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
} RaceHudMessageActor;

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

typedef union {
    s16 target[PLAYER_COUNT];
    struct {
        u8 pad0[4];
        u8 inactiveTimer[PLAYER_COUNT];
    } overlay;
} RaceHudPanelTargetX;

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ s16 x[PLAYER_COUNT];
    /* 0x20 */ s16 y[PLAYER_COUNT];
    /* 0x28 */ RaceHudPanelTargetX targetX;
    union {
        s16 target[PLAYER_COUNT];
        u8 mode;
    } targetY;
    /* 0x38 */ s16 accumulator[PLAYER_COUNT];
    /* 0x40 */ u16 tileSize[PLAYER_COUNT];
    /* 0x48 */ s16 xDirection[PLAYER_COUNT];
    /* 0x50 */ s16 timer[PLAYER_COUNT];
    /* 0x58 */ s16 stepCount[PLAYER_COUNT];
    /* 0x60 */ u8 axis[PLAYER_COUNT];
    /* 0x64 */ u8 divisor[PLAYER_COUNT];
    /* 0x68 */ u8 state[PLAYER_COUNT];
} RaceHudPanelActor;

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ s16 x;
    u8 pad1A[6];
    /* 0x20 */ s16 y;
    u8 pad22[0x1E];
    /* 0x40 */ u16 tileSize;
} RaceHudPanelSlot;

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ s16 x[PLAYER_COUNT];
    /* 0x20 */ s16 y;
    /* 0x22 */ s16 baseX;
    /* 0x24 */ s16 scale;
    /* 0x26 */ u8 mode;
    /* 0x27 */ u8 timer;
} RaceHudPlayerListActor;

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

extern void func_80071824(void *task, void (*callback)());
extern void func_800171F0(RaceHudBannerActor *);
extern void func_80018C80(void);
extern void func_800177F8(RaceHudBannerActor *);
extern void func_80017C34(RaceHudPanelActor *);
extern void func_800184C8(void);
extern void func_80018AA0(RaceHudPanelActor *);
extern void func_80018134(RaceHudPlayerListActor *);
extern void func_800182A4(RaceHudPlayerListActor *);
extern void func_80017D6C(RaceHudMessageActor *);
extern void func_800483FC(void *, void *, void *);
extern s8 D_8010AE52;
extern u8 D_8010AE51;
extern RaceHudCharacterSelectState D_8010AE50;
extern RaceHudPlayerFrameController *D_8010ADE0;
extern void *D_8010ADE4;
extern s16 D_8010AE58;
extern s32 D_801235B4;
extern void *D_80124868;
extern u8 D_80121B55;
extern RacePlayer D_80121D80[];
extern s8 D_8010AE64[];
extern u8 D_80112130[];
extern u8 D_800B5A70[];
extern const char D_800E0AB0[];
extern RacePlayerState D_800EC9F0[];
extern u8 D_8010AE5E;
extern u8 D_8010AE5F;
extern u16 D_800B5B30[];
extern s16 D_80112172;
extern s32 func_80043040(s16);

// func_800171F0 best match: 99.425% (nonmatchings/func_800171F0-1315772375853892447/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_800171F0.s")

#ifdef NON_MATCHING
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
                alpha = 0x100;
                if (actor->mode != 0) {
                    alpha = 0x60;
                }
            }
            func_8000F8AC((s16)(actor->x + 0x4C), (s16)(actor->y + 0x10),
                          func_80043040(RACE_HUD_PLAYER_FRAME_HANDLE), 0x17, 0x20, 0x20, 0, alpha, 0);
            if (actor->alpha < 0x60) {
                alpha = actor->alpha & 0xFFFF;
            } else {
                alpha = 0x60;
                if (actor->mode != 0) {
                    alpha = 0x100;
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
#endif

void func_800177F8(RaceHudBannerActor *arg0) {
    s16 alpha;
    s32 state;
    RaceHudBannerActor *actor;

    actor = arg0;
    if (D_8010AE50.phase != arg0->state) {
        arg0->state = D_8010AE50.phase;
        arg0->alpha = D_8010AE50.fade;
        arg0->bounceTimer = D_8010AE50.unkD;
        arg0->unk1E = D_8010AE50.unkA;
    }

    if (D_8010AE50.confirmSelection != actor->mode) {
        actor->mode = D_8010AE50.confirmSelection;
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
            D_8010AE50.exitMode = 1;
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

    D_8010AE50.phase = actor->state;
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
    func_80071824(arg0, func_800177F8);
}

// func_80017A10 best match: 98.362% (nonmatchings/func_80017A10-180949888360117632/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_80017A10.s")

#ifdef NON_MATCHING
typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ s16 x;
    u8 pad1A[6];
    /* 0x20 */ s16 y;
} RaceHudPlayerFrameActor;

void func_80017A10(RaceHudPlayerFrameActor *arg0) {
    char sp6C[0x10];
    s32 alpha;
    s32 i;
    s32 playerNumber;
    s32 flip;
    s32 texture;
    RaceHudPlayerFrameActor *actor;

    actor = arg0; i = 0; do {
        if ((D_80121B55 - 1) < i) {
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
        func_8000F8AC((s16)(actor->x + 0x80), actor->y, texture, 0xC, 0x20, 0x20, 0, alpha, flip);
        sprintf(sp6C, D_800E0AB0, playerNumber);
        func_80013D0C((s16)(actor->x + 0x32), (s16)(actor->y + 2), sp6C, 0, alpha);
        if (alpha != 0x100) {
            texture = func_80043040(RACE_HUD_UNUSED_HANDLE);
            func_8000F8AC((s16)(actor->x + 2), (s16)(actor->y + 0x14), texture, 0x90, 0x20, 0x20, 0, 0xF0, 0);
        }

        i = playerNumber;
        actor = (RaceHudPlayerFrameActor *)((u8 *)actor + 2);
    } while (playerNumber != 4);
}
#endif

// func_80017C34 best match: 92.981% (nonmatchings/func_80017C34-5635509610426229442/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_80017C34.s")

#ifdef NON_MATCHING
void func_80017C34(RaceHudPanelActor *arg0) {
    u8 var_v0;
    u8 desired;
    u8 var_v1;
    RaceHudPanelActor *actor;
    RaceHudPlayerFrameController *owner;

    owner = D_8010ADE0;
    actor = arg0;
    var_v0 = actor->targetY.mode;
    desired = D_8010AE50.exitMode;
    var_v1 = var_v0;
    if (desired != var_v0) {
        actor->targetY.mode = desired;
        var_v0 = desired & 0xFF;
        actor->targetX.target[0] = D_8010AE50.cursorX;
        var_v1 = var_v0;
        actor->targetX.target[1] = D_8010AE50.cursorY;
    }

    switch (var_v1) {
    case 1:
        actor->y[3] += 8;
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
    func_80071824(arg0, func_80017C34);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_80017D6C.s")

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
    if (D_80121B55 > 0) {
        player = D_800EC9F0;
        do {
            actor->playerFlags = actor->playerFlags | (player->flags & 1);
            D_8010AE5E = actor->playerFlags;
            D_8010AE5F = actor->unk23;
            i++;
            player++;
        } while (i < D_80121B55);
    }

    if (actor->playerFlags == 1) {
        actor->targetX = -0x40;
    } else {
        actor->targetX = targetX;
    }
    func_80071824(actor, func_80017F94);
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

 do { if (arg0->mode != 0) { i = 0; if (((s32) D_80121B55) > 0) { player = D_80121D80; tiles = D_800B5B30; actorX = arg0; do { evenMatch = 0; oddMatch = 0; j = 0; if (player->isActive != 0) { alpha = 0x100; } else { alpha = arg0->scale; } if (((s32) D_80121B55) > 0) { do { if ((j != i) && (D_8010AE64[i] == D_8010AE64[j])) { if (!(j & 1)) { evenMatch = 1; } else { oddMatch = 2; } } j++; } while (j < ((s32) D_80121B55)); } func_8000F8AC(actorX->x[0], arg0->y, func_80043040(D_80112172), tiles[evenMatch + oddMatch], 0x20, 0x20, 0, alpha, 0); i++; player++; tiles += 4; actorX = (RaceHudPlayerListActor *) (((u8 *) actorX) + 2); } while (i < ((s32) D_80121B55)); } } } while (0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_800182A4.s")

#ifdef NON_MATCHING
void func_800182A4(RaceHudPlayerListActor *arg0) {
    s32 i;
    u8 mode;
    u8 globalMode;
    u8 *player;

    globalMode = D_8010AE52;
    mode = arg0->mode;
    {
        if (globalMode != mode) {
            mode = (arg0->mode = globalMode);
        }

        if (mode != 0) {
 do { i = 0; if (((s32) D_80121B55) > 0) { player = (u8 *)D_80121D80; do { if (player[5] < 5) { arg0->x[i] = (D_8010AE64[i] * 0x20) + arg0->baseX; } else if (player[5] == 5) { arg0->x[i] = arg0->baseX; } i++; player += 0x60C; } while (i < ((s32) D_80121B55)); } if (((s32) arg0->timer) < 0x10) { arg0->scale -= 9; } else { arg0->scale += 9; } arg0->timer = (arg0->timer + 1) & 0x1F; mode = arg0->mode; } while (0);
        }

        D_8010AE52 = mode;
    }
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
 if ((s32)D_80121B55 > 0) { player = D_800EC9F0; do {
            playerFlags |= player->flags;
            i = (s32)&D_800EC9F0[D_80121B55];
            player++;
        } while ((u32)player < (u32)i);
        i = 0;
    }

    if (playerFlags == 1) {
        arg0->baseX = -0x60;
    } else {
        arg0->baseX = -0x70;
    }

    if ((s32)D_80121B55 > 0) {
        playerLayout = D_8010AE64;
        actor = arg0;
        do {
            actor->x[0] = (*playerLayout * 0x20) + arg0->baseX;
            i++;
            playerLayout++;
            actor = (RaceHudPlayerListActor *)((u8 *)actor + 2);
        } while (i < (s32)D_80121B55);
    }

    arg0->y = -0x18;
    arg0->mode = 0;
    arg0->scale = 0x100;
    func_80071824(arg0, func_800182A4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_800184C8.s")

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
    func_80071824(arg0, func_80018AA0);
}

void func_80018BC0(void *arg0) {
    u8 *base;
    s32 i;
    u8 *player;
    u8 *temp_s0;
    int new_var;
    s32 color;
    u16 temp_v1;

 base = arg0; i = 0; if (D_80121B55 > 0) { player = (u8 *)D_80121D80; do {
            if (player[8] != 0) {
                new_var = i * 2;
                temp_s0 = base + new_var;
                color = func_80043040(*(s16 *) &D_80112130[0x42]);
                temp_v1 = *(u16 *) (temp_s0 + 0x40);
                func_8000F030(*(s16 *) (temp_s0 + 0x18), *(s16 *) (temp_s0 + 0x20), color, 0xD, temp_v1, temp_v1, 0, 0);
            }
            i++;
            player += PLAYER_DATA_SIZE;
        } while (i < D_80121B55);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_80018C80.s")

void func_800191A0(RaceHudPanelActor *arg0) {
    func_80071824(arg0, func_80018C80);
}
