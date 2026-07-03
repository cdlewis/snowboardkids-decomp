#include "common.h"

#define PLAYER_COUNT 4
#define PLAYER_DATA_SIZE 0x60C

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
    /* 0x22 */ s16 speedY;
    /* 0x24 */ u8 state;
    /* 0x25 */ u8 timer;
} RaceHudMessageActor;

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

extern void func_80071824(void *, void *);
extern void func_80018C80(void);
extern void func_800177F8(void);
extern void func_80017C34(void);
extern void func_80018AA0(void);
extern void func_80017D6C(RaceHudMessageActor *);
extern void func_800483FC(void *, void *, void *);
extern s8 D_8010AE52;
extern void *D_80124868;
extern u8 D_80121B55;
extern u8 D_80121D80[];
extern u8 D_80112130[];
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

#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_80017A10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_80017C34.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_80018060.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_80018134.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_800182A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_800183DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_800184C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_hud/func_80018AA0.s")

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
