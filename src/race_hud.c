#include "common.h"
#include "menu_rendering.h"

#define PLAYER_COUNT 4
#define PLAYER_DATA_SIZE 0x60C
#define RACE_HUD_UNUSED_HANDLE (*(s16 *)&D_80112130[0x3E])
#define RACE_HUD_PLAYER_FRAME_HANDLE (*(s16 *)&D_80112130[0x42])

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

extern void func_80071824(void *task, void (*callback)());
extern void func_80018C80(void);
extern void func_800177F8(void);
extern void func_80017C34(RaceHudPanelActor *);
extern void func_800184C8(void);
extern void func_80018AA0(RaceHudPanelActor *);
extern void func_800182A4(RaceHudPlayerListActor *);
extern void func_80017D6C(RaceHudMessageActor *);
extern void func_800483FC(void *, void *, void *);
extern s8 D_8010AE52;
extern u8 D_8010AE51;
extern RaceHudCharacterSelectState D_8010AE50;
extern RaceHudPlayerFrameController *D_8010ADE0;
extern void *D_8010ADE4;
extern void *D_80124868;
extern u8 D_80121B55;
extern u8 D_80121D80[];
extern u8 D_80112130[];
extern const char D_800E0AB0[];
extern RacePlayerState D_800EC9F0[];
extern u8 D_8010AE5E;
extern u8 D_8010AE5F;
extern s8 D_8010AE64[];
extern s32 func_80043040(s16);
extern void func_8000F030(s16, s16, s32, s32, s32, s32, s32, s32);

#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_800171F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_800177F8.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_80018134.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_800182A4.s")

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
    player = D_80121D80; for (i = 0, src = srcBase, dst = (u8 *)arg0; i != PLAYER_COUNT; i++, dst += 2) {
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

 base = arg0; i = 0; if (D_80121B55 > 0) { player = D_80121D80; do {
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
